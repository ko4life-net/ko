// N3Base.cpp: implementation of the CN3Base class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Base.h"
#include "N3Texture.h"
#include "N3Mesh.h"
#include "N3VMesh.h"
#include "N3PMesh.h"
#include "N3Chr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CN3Mng<CN3Texture>	CN3Base::s_MngTex; // Texture Manager
CN3Mng<CN3Mesh>		CN3Base::s_MngMesh; // Mesh Manager
CN3Mng<CN3VMesh>	CN3Base::s_MngVMesh; // 단순히 폴리곤만 갖고 있는 메시 - 주로 충돌 체크에 쓴다..
CN3Mng<CN3PMesh>	CN3Base::s_MngPMesh; // Progressive Mesh Manager
CN3Mng<CN3Joint>	CN3Base::s_MngJoint; // Joint Manager
CN3Mng<CN3CPart>	CN3Base::s_MngCPart; // Character 에 쓰는 Part Manager
CN3Mng<CN3AnimControl>	CN3Base::s_MngAniCtrl; // Animation Manager

LPDIRECT3DDEVICE8 CN3Base::s_lpD3DDev = NULL; // 참조 포인터.. 멋대로 해제하면 안된다..
D3DPRESENT_PARAMETERS CN3Base::s_DevParam; // Device 생성 Present Parameter
D3DCAPS8 CN3Base::s_DevCaps; // Device 호환성...
DWORD CN3Base::s_dwTextureCaps = 0; // Texture 호환성..
CN3Base::__CameraData CN3Base::s_CameraData; // Camera Data
CN3Base::__RenderInfo CN3Base::s_RenderInfo; // Rendering Information
CN3Base::__ResrcInfo CN3Base::s_ResrcInfo; // Rendering Information
float CN3Base::s_fFrmPerSec = 30.0f; // Frame Per Second
char CN3Base::s_szPath[512] = "";

HWND CN3Base::s_hWndDevice; // Device Window Handle
HWND CN3Base::s_hWndPresent; // 최근에 Present 한 Window Handle
HWND CN3Base::s_hWndBase; // Init 할때 쓴 Window Handle


LARGE_INTEGER CN3Base::s_TimePrev; // 시간..

CN3Base::CN3Base()
{
	m_dwType = OBJ_BASE; // "MESH", "CAMERA", "SCENE", "???" .... 등등등...
	m_szName = NULL;
	m_szFileName = NULL;
}

CN3Base::~CN3Base()
{
	CN3Base::Release();
}

void CN3Base::Release()
{
	delete [] m_szName; m_szName = NULL;
	delete [] m_szFileName; m_szFileName = NULL;
}

void CN3Base::ReleaseResrc()
{
	s_MngTex.Release();
	s_MngMesh.Release();
	s_MngPMesh.Release();
	s_MngVMesh.Release();
	
	s_MngJoint.Release();
	s_MngCPart.Release();
	s_MngAniCtrl.Release();
}

void CN3Base::SaveResrc()
{
	s_MngTex.SaveToFiles();
	s_MngMesh.SaveToFiles();
	s_MngPMesh.SaveToFiles();
	s_MngVMesh.SaveToFiles();
	
	s_MngJoint.SaveToFiles();
	s_MngCPart.SaveToFiles();
	s_MngAniCtrl.SaveToFiles();
}

bool CN3Base::Load(HANDLE hFile)
{
	delete [] m_szName; m_szName = NULL;

	DWORD dwRWC = 0;

	int nL = 0;
	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) 
	{
		m_szName = new char[nL+1];
		ReadFile(hFile, m_szName, nL, &dwRWC, NULL);
		m_szName[nL] = NULL;
	}

	return true;
}

bool CN3Base::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	int nL = lstrlen(m_szName);
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) WriteFile(hFile, m_szName, nL, &dwRWC, NULL);

	return true;
}

bool CN3Base::LoadFromFile()
{
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

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		char szErr[512];
		wsprintf(szErr, "%s - Can't open file (read)", szFullPath);
		MessageBox(s_hWndBase, szErr, "File Handle error", MB_OK);
		return false;
	}

	this->Load(hFile);

	CloseHandle(hFile);

	return true;
}

bool CN3Base::LoadFromFile(const char* szFileName)
{
	this->FileNameSet(szFileName);
	return this->LoadFromFile();
}

bool CN3Base::SaveToFile()
{
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

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szFullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		char szErr[512];
		wsprintf(szErr, "%s - Can't open file(write)", szFullPath);
		MessageBox(s_hWndBase, szErr, "File Handle error", MB_OK);
		return false;
	}

	this->Save(hFile);

	CloseHandle(hFile);
	return true;
}

bool CN3Base::SaveToFile(const char* szFileName)
{
	this->FileNameSet(szFileName);
	return this->SaveToFile();
}

void CN3Base::TimerProcess()
{
	static bool bInit = false;
	static bool bUseHWTimer = FALSE;
	static LARGE_INTEGER nTime, nFrequency;
	
	if(bInit == false)
	{
		if(TRUE == ::QueryPerformanceCounter(&nTime))
		{
			s_TimePrev = nTime;
			::QueryPerformanceFrequency(&nFrequency);
			bUseHWTimer = TRUE;
		}
		else 
		{
			bUseHWTimer = FALSE;
		}

		bInit = true;
	}

	if(bUseHWTimer)
	{
		::QueryPerformanceCounter(&nTime);
		s_fFrmPerSec = (float)(1.0 / ((double)(nTime.QuadPart - s_TimePrev.QuadPart)/(double)nFrequency.QuadPart));
	}
	else
	{
		nTime.QuadPart = timeGetTime();
		s_fFrmPerSec = 1000.0f / (float)(nTime.QuadPart - s_TimePrev.QuadPart);
	}

	if(s_fFrmPerSec < 1.0f) s_fFrmPerSec = 1000.0f;

	s_TimePrev = nTime;
}

void CN3Base::PathSet(const char* szPath)
{
	int n = lstrlen(szPath);
	if(n <= 0)
	{
		lstrcpy(s_szPath, "");
		return;
	}

	lstrcpy(s_szPath, szPath);
	CharLower(s_szPath); // 반드시 소문자로 만들어 준다..
	if(s_szPath[n-1] != '\\')
	{
		s_szPath[n] = '\\';
		s_szPath[n+1] = NULL;
	}
}

void CN3Base::NameSet(const char* szName)
{
	if(szName == m_szName) return;

	delete [] m_szName;
	m_szName = NULL;

	int nL = lstrlen(szName);
	if(nL <= 0) return;

	m_szName = new char[nL+1];
	lstrcpy(m_szName, szName);
}

void CN3Base::FileNameSet(const char* szFileName)
{
	if(szFileName == m_szFileName) return;

	delete [] m_szFileName;
	m_szFileName = NULL;

	int nL = lstrlen(szFileName);
	if(nL <= 0) return;

	char szFNTmp[512];
	lstrcpy(szFNTmp, szFileName); // 임시로 복사하고..
	CharLower(szFNTmp); // 모두 소문자로 만든다..

	// 문자열에 Base Path 와 일치하는 이름이 있는지 본다.
	char* szFNFinal = NULL;
	if(s_szPath[0])
	{
		char* szBasePath = strstr(szFNTmp, s_szPath);
		if(szBasePath) // 일치하면..
		{
			nL = lstrlen(s_szPath);
			szFNFinal = szBasePath + nL;
		}
		else
		{
			szFNFinal = szFNTmp;
		}
	}
	else
	{
		szFNFinal = szFNTmp;
	}

	nL = lstrlen(szFNFinal);
	m_szFileName = new char[nL+1];
	lstrcpy(m_szFileName, szFNFinal);
}

void CN3Base::RenderLines(const __Vector3 *pvLines, int nCount, D3DCOLOR color)
{
	DWORD dwAlpha, dwFog, dwLight;
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	static __Matrix44 stm;
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		stm.Identity();
		smtl.Init();
		bInit = true;
	}

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &stm);
	s_lpD3DDev->SetTexture(0, NULL);

	static __VertexColor svLines[512];

	s_lpD3DDev->SetVertexShader(FVF_CV);

	int nRepeat = nCount/512;
	for(int i = 0; i < nRepeat; i++)
	{
		for(int j = 0; j < 512; j++) svLines[j].Set(pvLines[i*512+j].x, pvLines[i*512+j].y, pvLines[i*512+j].z, color);
		
		s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 511, svLines, sizeof(__VertexColor));
	}
	int nPC = nCount%512;
	for(int j = 0; j < nPC+1; j++) svLines[j].Set(pvLines[nRepeat*512+j].x, pvLines[nRepeat*512+j].y, pvLines[nRepeat*512+j].z, color);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, nPC, svLines, sizeof(__VertexColor)); // Y

	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

float CN3Base::TimeGet()
{
	static bool bInit = false;
	static bool bUseHWTimer = FALSE;
	static LARGE_INTEGER nTime, nFrequency;
	
	if(bInit == false)
	{
		if(TRUE == ::QueryPerformanceCounter(&nTime))
		{
			::QueryPerformanceFrequency(&nFrequency);
			bUseHWTimer = TRUE;
		}
		else 
		{
			bUseHWTimer = FALSE;
		}

		bInit = true;
	}

	if(bUseHWTimer)
	{
		::QueryPerformanceCounter(&nTime);
		return (float)((double)(nTime.QuadPart)/(double)nFrequency.QuadPart);
	}

	return (float)timeGetTime();
}
