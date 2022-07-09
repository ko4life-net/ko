// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Texture::CN3Texture()
{
	m_dwType |= OBJ_TEXTURE;

	m_lpTexture = NULL;
	this->Release();
}

CN3Texture::~CN3Texture()
{
	this->Release();
}

void CN3Texture::Release()
{
	if(32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32--;
	else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64--;
	else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128--;
	else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256--;
	else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512--;
	else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge--;
	else s_ResrcInfo.nTexture_Loaded_OtherSize--;

	memset(&m_Header, 0, sizeof(m_Header));
	if(m_lpTexture != NULL)
		if(m_lpTexture->Release() == 0) m_lpTexture = NULL;
}

bool CN3Texture::Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap)
{
	if(nWidth <= 1 || nHeight <= 1 || D3DFMT_UNKNOWN == Format) return false;
	if(m_lpTexture != NULL) this->Release();

	if(s_dwTextureCaps & TEX_CAPS_POW2) // 2 의 승수만 된다면..
	{
		for(int nW = 1; nW <= nWidth; nW *= 2); nW /= 2;
		for(int nH = 1; nH <= nHeight; nH *= 2); nH /= 2;

		nWidth = nW;
		nHeight = nH;
	}

	if((s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // 정사각형 텍스처만 되면..
	{
		if(nWidth > nHeight) nWidth = nHeight;
		else nHeight = nWidth;
	}

	memset(&m_Header, 0, sizeof(m_Header));
	HRESULT rval = s_lpD3DDev->CreateTexture(nWidth, nHeight, 0, 0, Format, D3DPOOL_MANAGED, &m_lpTexture);
	if(NULL == m_lpTexture)
	{
		__ASSERT(m_lpTexture, "Texture pointer is NULL!");
		return false;
	}

	m_Header.nWidth = nWidth;
	m_Header.nHeight = nHeight;
	m_Header.Format = Format;

	if(32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32++;
	else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64++;
	else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128++;
	else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256++;
	else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512++;
	else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge++;
	else s_ResrcInfo.nTexture_Loaded_OtherSize++;

	return true;
}

bool CN3Texture::CreateFromSurface(LPDIRECT3DSURFACE8 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap)
{
	if(lpSurf == NULL) return false;

	D3DSURFACE_DESC sd;
	lpSurf->GetDesc(&sd);

	if(this->Create(sd.Width, sd.Height, Format, bGenerateMipMap) == false) return false;
	if(bGenerateMipMap)
	{
		return this->GenerateMipMap();
	}
	
	return true;
}

bool CN3Texture::LoadFromFile(const char* szFileName)
{
	int nFNL = lstrlen(szFileName);
	if(nFNL <= 0) return false;
	
	this->FileNameSet(szFileName); // 파일 이름을 복사하고..
	char szFullPath[_MAX_PATH] = "";
	if(strstr(m_szFileName, ":") || strstr(m_szFileName, "\\\\") || strstr(m_szFileName, "//")) // 문자열에 ':', '\\', '//' 이 들어 있으면 전체 경로이다..
	{
		lstrcpy(szFullPath, m_szFileName);
	}
	else
	{
		if(NULL != s_szPath[0]) lstrcpy(szFullPath, s_szPath);
		lstrcat(szFullPath, m_szFileName);
	}

	nFNL = lstrlen(szFullPath);

	if(m_lpTexture != NULL) this->Release();
	if(lstrcmpi(&szFullPath[nFNL-3], "DXT") == 0)
	{
		HANDLE hFile = ::CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			char szErr[512];
			wsprintf(szErr, "%s - 파일이 없거나 읽기 전용등의 이유로 접근이 거부되었습니다.", szFullPath);
			MessageBox(s_hWndBase, szErr, "File Handle 오류", MB_OK);
			return false;
		}
		this->Load(hFile);
		CloseHandle(hFile);
	}
	else
	{
		D3DXIMAGE_INFO ImgInfo;
		HRESULT rval = D3DXCreateTextureFromFileEx(	s_lpD3DDev, 
													szFullPath,
													D3DX_DEFAULT, 
													D3DX_DEFAULT, 
													D3DX_DEFAULT, 
													0,
													D3DFMT_UNKNOWN,
													D3DPOOL_MANAGED,
													D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
													D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
													0,
													&ImgInfo,
													NULL,
													&m_lpTexture );
		if(rval == D3D_OK)
		{
			D3DSURFACE_DESC sd;
			m_lpTexture->GetLevelDesc(0, &sd);

			m_Header.nWidth = sd.Width;
			m_Header.nHeight = sd.Height;
			m_Header.Format = sd.Format;
		}

		if(32 == m_Header.nWidth && 32 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_32X32++;
		else if(64 == m_Header.nWidth && 64 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_64X64++;
		else if(128 == m_Header.nWidth && 128 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_128X128++;
		else if(256 == m_Header.nWidth && 256 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_256X256++;
		else if(512 == m_Header.nWidth && 512 == m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_512X512++;
		else if(512 < m_Header.nWidth && 512 < m_Header.nHeight) s_ResrcInfo.nTexture_Loaded_Huge++;
		else s_ResrcInfo.nTexture_Loaded_OtherSize++;
	}

	if(NULL == m_lpTexture) return false;
	return true;
}

bool CN3Texture::Load(HANDLE hFile)
{
	CN3Base::Load(hFile);

	DWORD dwRWC = 0;

	ReadFile(hFile, &m_Header, sizeof(m_Header), &dwRWC, NULL); // 헤더를 읽는다..
	if(	'N' != m_Header.szID[0] || 'T' != m_Header.szID[1] || 'F' != m_Header.szID[2] || 2 != m_Header.szID[3] ) // "NTF"2 - Noah Texture File Ver. 2.0
	{
		MessageBox(s_hWndBase, m_szFileName, "Old format DXT file", MB_OK);
		return false;
	}

	BOOL bDXTSupport = FALSE;
	D3DFORMAT fmtOld = m_Header.Format;
	D3DFORMAT fmtNew = m_Header.Format;
	if(D3DFMT_DXT1 == fmtOld) // DXT Format 을 읽어야 하는데 지원이 되는지 안되는지 보고 지원안되면 대체 포맷을 정한다.
	{
		if(s_dwTextureCaps & TEX_CAPS_DXT1) bDXTSupport = TRUE;
		else fmtNew = D3DFMT_A1R5G5B5;
	}
	else if(D3DFMT_DXT2 == fmtOld)
	{
		if(s_dwTextureCaps & TEX_CAPS_DXT2) bDXTSupport = TRUE;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT3 == fmtOld)
	{
		if(s_dwTextureCaps & TEX_CAPS_DXT3) bDXTSupport = TRUE;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT4 == fmtOld)
	{
		if(s_dwTextureCaps & TEX_CAPS_DXT4) bDXTSupport = TRUE;
		else fmtNew = D3DFMT_A4R4G4B4;
	}
	else if(D3DFMT_DXT5 == fmtOld)
	{
		if(s_dwTextureCaps & TEX_CAPS_DXT5) bDXTSupport = TRUE;
		else fmtNew = D3DFMT_A4R4G4B4;
	}

	if(fmtNew != fmtOld) // DXT 지원이 안되면 너비 높이를 줄인다.
	{
		m_Header.nWidth /= 2;
		m_Header.nHeight /= 2;
	}

	this->Create(m_Header.nWidth, m_Header.nHeight, fmtNew, m_Header.bMipMap);

	if(m_lpTexture == NULL)
	{
		char szErr[512];
		wsprintf(szErr, "%s - Can't create texture", m_szFileName);
		MessageBox(s_hWndBase, szErr, "Texture loading error", MB_OK);
		return false;
	}

	D3DSURFACE_DESC sd;
	D3DLOCKED_RECT LR;

	if(	D3DFMT_DXT1 == fmtOld || 
		D3DFMT_DXT2 == fmtOld || 
		D3DFMT_DXT3 == fmtOld || 
		D3DFMT_DXT4 == fmtOld || 
		D3DFMT_DXT5 == fmtOld )
	{
		int nMipMapCount = m_lpTexture->GetLevelCount();
		
		if(TRUE == bDXTSupport)
		{
			for(int i = 0; i < nMipMapCount; i++)
			{
				m_lpTexture->GetLevelDesc(i, &sd);
				
				m_lpTexture->LockRect(i, &LR, NULL, NULL);
				ReadFile(hFile, (BYTE*)LR.pBits, sd.Size, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
				m_lpTexture->UnlockRect(i);
			}

			int nSkip = 0;
			int nW = m_Header.nWidth / 2;
			int nH = m_Header.nHeight / 2;
			for(; nW >= 1 && nH >= 1; nW /= 2, nH /= 2)
			{
				nSkip += nW * nH * 2; // DXT1 형식은 1/4 로 압축..
			}
			::SetFilePointer(hFile, nSkip, 0, FILE_CURRENT); // 건너뛰고.
		}
		else // DXT 지원이 안되면..
		{
			int nSkip = 0;
			for(int i = 0; i < nMipMapCount; i++)
			{
				m_lpTexture->GetLevelDesc(i, &sd);

				if(D3DFMT_DXT1 == fmtOld) nSkip += (sd.Width*2) * (sd.Height*2) * 2 / 4; // DXT1 형식은 1/4 로 압축..
				else nSkip += (sd.Width*2) * (sd.Height*2); // DXT2 ~ DXT5 형식은 1/2 로 압축..
			}
			::SetFilePointer(hFile, nSkip, 0, FILE_CURRENT); // 건너뛰고.

			for(i = 0; i < nMipMapCount; i++)
			{
				m_lpTexture->GetLevelDesc(i, &sd);
				m_lpTexture->LockRect(i, &LR, NULL, NULL);
				int nH = sd.Height;
				for(int y = 0; y < nH; y++)
					ReadFile(hFile, (BYTE*)LR.pBits + y * LR.Pitch, 2 * sd.Width, &dwRWC, NULL);
				m_lpTexture->UnlockRect(i);
			}
		}
	}
	else
	{
		int nMipMapCount = m_lpTexture->GetLevelCount();

		int nPixelSize = 0;
		if(	fmtNew == D3DFMT_A1R5G5B5 ||
			fmtNew == D3DFMT_A4R4G4B4) nPixelSize = 2;
		else if(fmtNew == D3DFMT_R8G8B8) nPixelSize = 3;
		else if(fmtNew == D3DFMT_A8R8G8B8 || fmtNew == D3DFMT_X8R8G8B8) nPixelSize = 4;
		else 
		{
			__ASSERT(0, "Not supported texture format");
		}

		for(int i = 0; i < nMipMapCount; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);
			m_lpTexture->LockRect(i, &LR, NULL, NULL);
			for(int y = 0; y < (int)sd.Height; y++)
				ReadFile(hFile, (BYTE*)LR.pBits + y * LR.Pitch, nPixelSize * sd.Width, &dwRWC, NULL);
			m_lpTexture->UnlockRect(i);
		}
	}
//	this->GenerateMipMap(); // Mip Map 을 만든다..
	return true;
}

bool CN3Texture::Save(HANDLE hFile)
{
	if(NULL == m_lpTexture) return false;

	CN3Base::Save(hFile);

	DWORD dwRWC = 0;

	int nMipMapCount = m_lpTexture->GetLevelCount();
	D3DSURFACE_DESC sd;
	m_lpTexture->GetLevelDesc(0, &sd);

	m_Header.szID[0] = 'N';
	m_Header.szID[1] = 'T';
	m_Header.szID[2] = 'F';
	m_Header.szID[3] = 2; // Noah Texture File Ver '2'
	m_Header.nWidth = sd.Width;
	m_Header.nHeight = sd.Height;
	m_Header.bMipMap = (nMipMapCount > 1) ? TRUE : FALSE;
	
	WriteFile(hFile, &m_Header, sizeof(m_Header), &dwRWC, NULL); // 헤더를 쓰고

	if(m_lpTexture == NULL) return false;

	if(	D3DFMT_DXT1 == sd.Format || 
		D3DFMT_DXT2 == sd.Format || 
		D3DFMT_DXT3 == sd.Format || 
		D3DFMT_DXT4 == sd.Format || 
		D3DFMT_DXT5 == sd.Format )
	{
		D3DLOCKED_RECT LR;

		for(int i = 0; i < nMipMapCount; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);

			m_lpTexture->LockRect(i, &LR, NULL, NULL);
			WriteFile(hFile, (BYTE*)LR.pBits, sd.Size, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
			m_lpTexture->UnlockRect(i);
		}

		// 추가로 압축되지 않은 형식을 써준다.. 절반 크기이다.
		// 압축되지 않은 형식을 해상도를 한단계 낮추어서 저장.
		LPDIRECT3DSURFACE8 lpSurfSrc = NULL, lpSurfDest = NULL;
		D3DFORMAT fmtExtra = D3DFMT_UNKNOWN;
		if(D3DFMT_DXT1 == sd.Format) fmtExtra = D3DFMT_A1R5G5B5;
		else fmtExtra = D3DFMT_A4R4G4B4;
		for(i = 1; i < nMipMapCount; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);
			m_lpTexture->GetSurfaceLevel(i, &lpSurfSrc);
			s_lpD3DDev->CreateImageSurface(sd.Width, sd.Height, fmtExtra, &lpSurfDest);

			D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_NONE, 0); // 서피스 복사.

			int nPixelSize = 2;

			lpSurfDest->LockRect(&LR, NULL, NULL);
			for(int y = 0; y < (int)(sd.Height); y++)
				WriteFile(hFile, (BYTE*)LR.pBits + y * LR.Pitch, sd.Width * 2, &dwRWC, NULL);
			lpSurfDest->UnlockRect();

			lpSurfDest->Release();
			lpSurfDest = NULL;
			lpSurfSrc->Release();
			lpSurfSrc = NULL;
		}
	}
	else // 일반적인 포맷이면.
	{
		int nPixelSize = 0;
		if(	D3DFMT_A1R5G5B5 == sd.Format ||
			D3DFMT_A4R4G4B4 == sd.Format) nPixelSize = 2;
		else if(D3DFMT_R8G8B8 == sd.Format) nPixelSize = 3;
		else if(D3DFMT_A8R8G8B8 == sd.Format || D3DFMT_X8R8G8B8 == sd.Format) nPixelSize = 4;
		else 
		{
			__ASSERT(0, "this Texture Format Not Supported");
		}

		D3DLOCKED_RECT LR;
		for(int i = 0; i < nMipMapCount; i++)
		{
			m_lpTexture->GetLevelDesc(i, &sd);
			m_lpTexture->LockRect(i, &LR, NULL, 0); // 각 레벨 Lock
			int nH = sd.Height;
			for(int y = 0; y < nH; y++) // 그냥 픽셀 저장..
				WriteFile(hFile, (BYTE*)LR.pBits + y * LR.Pitch, sd.Width * nPixelSize, &dwRWC, NULL);
			m_lpTexture->UnlockRect(i);
		}
	}

	return true;
}

bool CN3Texture::Convert(D3DFORMAT Format, int nWidth, int nHeight)
{
	if(m_lpTexture == NULL) return false;

	D3DSURFACE_DESC dsd;
	m_lpTexture->GetLevelDesc(0, &dsd);
	if(0 >= nWidth || 0 >= nHeight)
	{
		nWidth = dsd.Width;
		nHeight = dsd.Height;
	}

	LPDIRECT3DTEXTURE8 lpTexOld = m_lpTexture;

	m_lpTexture = NULL;
	this->Release();
	
	int nMMC = lpTexOld->GetLevelCount(); // MipMap Count
	BOOL bGenerateMipMap = FALSE;
	if(nMMC > 1) bGenerateMipMap = TRUE;
	if(this->Create(nWidth, nHeight, Format, bGenerateMipMap) == false) return false;

	LPDIRECT3DSURFACE8 lpTSOld, lpTSNew;

	lpTexOld->GetSurfaceLevel(0, &lpTSOld);
	m_lpTexture->GetSurfaceLevel(0, &lpTSNew);

	D3DXLoadSurfaceFromSurface(lpTSNew, NULL, NULL, lpTSOld, NULL, NULL, D3DX_FILTER_NONE, 0); // 첫번재 레벨 서피스 복사.
	if(nMMC > 0) this->GenerateMipMap(lpTSOld); // MipMap 생성
	
	lpTSOld->Release();
	lpTSNew->Release();
	lpTexOld->Release();

	return true;
}

bool CN3Texture::GenerateMipMap(LPDIRECT3DSURFACE8 lpSurf)
{
	if(m_lpTexture == NULL) return false;
	int nMMC = m_lpTexture->GetLevelCount();
	if(nMMC <= 0) return false;

	int nMMStart = 0;
	if(NULL == lpSurf) // Source Surface 가 NULL 이면..
	{
		nMMStart = 1;
		m_lpTexture->GetSurfaceLevel(0, &lpSurf);
	}

	D3DSURFACE_DESC sdSrc, sdDest;
	LPDIRECT3DSURFACE8 lpSurfDest;
	lpSurf->GetDesc(&sdSrc);
	
	for(int i = nMMStart; i < nMMC; i++)
	{
		m_lpTexture->GetSurfaceLevel(i, &lpSurfDest);
		lpSurfDest->GetDesc(&sdDest);

		DWORD dwFilter = D3DX_FILTER_NONE; // 기본 필터는 없다..
		lpSurf->GetDesc(&sdSrc);
		if(sdSrc.Width != sdDest.Width || sdSrc.Height != sdDest.Height) dwFilter = D3DX_FILTER_TRIANGLE; // 너비 높이가 다르면.. 필터 적용

		HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurf, NULL, NULL, dwFilter, 0); // 작은 맵 체인에 서피스 이미지 축소 복사 
		
		lpSurfDest->Release();
	}

	if(nMMStart == 1)
	{
		lpSurf->Release(); lpSurf = NULL; // 0 레벨 서피스 릴리즈..
	}

	return true;
}

void CN3Texture::UpdateRenderInfo()
{

}
