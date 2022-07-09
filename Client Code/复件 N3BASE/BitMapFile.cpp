// BitMapFile.cpp: implementation of the CBitMapFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "BitMapFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitMapFile::CBitMapFile()
{
	m_pPixels = NULL;
	Release();
}

CBitMapFile::~CBitMapFile()
{
	Release();
}

void CBitMapFile::Release()
{
	memset(&m_bmfHeader, 0, sizeof(m_bmfHeader));
	memset(&m_bmInfoHeader, 0, sizeof(m_bmInfoHeader));
	::GlobalFree(m_pPixels); // 실제 픽셀 데이터
	m_pPixels = NULL;
}

bool CBitMapFile::Load(HANDLE hFile)
{
	this->Release(); // 일단 다 해제하고..

	DWORD dwRWC = 0;

	// 파일 헤더 읽기
	ReadFile(hFile, &m_bmfHeader, sizeof(m_bmfHeader), &dwRWC, NULL);

	// bmp 파일임을 나타내는 "BM"마커 확인
	if (m_bmfHeader.bfType != 0x4D42)
	{
		MessageBox(s_hWndBase, "원본 파일이 bitmap파일이 아닙니다.", "error", MB_OK);
		return FALSE;
	}

	// BITMAPINFOHEADER 얻기
	ReadFile(hFile, &m_bmInfoHeader, sizeof(m_bmInfoHeader), &dwRWC, NULL);

	// 픽셀당 비트 수 확인
	WORD wBitCount = m_bmInfoHeader.biBitCount;
	if (24 != wBitCount || m_bmInfoHeader.biWidth <= 0 || m_bmInfoHeader.biHeight <= 0)		// 24비트 bmp가 아니면 return해 버린다.
	{
		MessageBox(s_hWndBase, "원본 bitmap이 너비, 높이에 이상이 있거나 24bit파일이 아닙니다.", "error", NULL);
		return FALSE;
	}

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidthSrc = ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4;	

	// 새로 만들 이미지 메모리 할당
	int iDIBSize = iRealWidthSrc * m_bmInfoHeader.biHeight;

	if ((m_pPixels = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iDIBSize )) == NULL )
	{
		MessageBox(s_hWndBase, "메모리를 할당하지 못했습니다.", "error", MB_OK);
		return FALSE;
	}

	// 픽셀을 읽는다..
	for(int y = m_bmInfoHeader.biHeight; y >= 0; y--) // 비트맵은 위아래가 거꾸로 있다..
	{
		ReadFile(hFile, (BYTE*)m_pPixels + y * iRealWidthSrc, iRealWidthSrc, &dwRWC, NULL);
	}

	return TRUE;
}

void* CBitMapFile::Pixels()
{
	return m_pPixels;
}

bool CBitMapFile::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	// 파일 헤더 쓰기
	WriteFile(hFile, &m_bmfHeader, sizeof(m_bmfHeader), &dwRWC, NULL);

	// BITMAPINFOHEADER 쓰기
	WriteFile(hFile, &m_bmInfoHeader, sizeof(m_bmInfoHeader), &dwRWC, NULL);

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidthSrc = ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4;	

	// 새로 만들 이미지 메모리 할당
	int iDIBSize = iRealWidthSrc * m_bmInfoHeader.biHeight;

	// 픽셀을 저장한다...
	WriteFile(hFile, m_pPixels, iDIBSize, &dwRWC, NULL);

	CloseHandle(hFile);
	return TRUE;
}

bool CBitMapFile::SaveRectToFile(const char* szFileName, RECT rc)
{
	if(rc.right <= rc.left) return false;
	if(rc.bottom <= rc.top) return false;
	if(rc.left < 0) rc.left = 0;
	if(rc.top < 0) rc.top = 0;
	if(rc.right > m_bmInfoHeader.biWidth) rc.right = m_bmInfoHeader.biWidth;
	if(rc.bottom > m_bmInfoHeader.biHeight) rc.bottom = m_bmInfoHeader.biHeight;

	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	if (lstrlen(szFileName) == 0 || nWidth<=0 || nHeight<=0)
	{
		MessageBox(s_hWndBase, "가로 세로가 0이하인 bitmap으로 저장할수 없습니다.", "error", MB_OK);
		return FALSE;
	}

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// 쓰기 모드로 파일 열기
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(s_hWndBase, "원본 bitmap을 열 수 없습니다.", "error", MB_OK);
		return false;
	}

	// 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
	int iRealWidthDest = ((int)((nWidth*3 + 3)/4))*4;	
	int iDestDIBSize = sizeof(BITMAPINFOHEADER) + iRealWidthDest * nHeight;

	// 새로 만들 이미지 file header 정보 채우기
	BITMAPFILEHEADER bmfHeaderDest = m_bmfHeader;
	bmfHeaderDest.bfType = 0x4D42; // "BM"
	bmfHeaderDest.bfSize = sizeof(bmfHeaderDest) + iDestDIBSize;
	bmfHeaderDest.bfOffBits = sizeof(bmfHeaderDest) + sizeof(BITMAPINFOHEADER);

	// 새로 만들 이미지 bitmap info header 정보 채우기
	BITMAPINFOHEADER bmInfoHeaderDest = m_bmInfoHeader;
	bmInfoHeaderDest.biSize = sizeof(bmInfoHeaderDest);
	bmInfoHeaderDest.biWidth = nWidth;
	bmInfoHeaderDest.biHeight = nHeight;
	bmInfoHeaderDest.biPlanes = 1;
	bmInfoHeaderDest.biSizeImage = iRealWidthDest * nHeight;

	// 파일 헤더 쓰기
	WriteFile(hFile, &bmfHeaderDest, sizeof(bmfHeaderDest), &dwRWC, NULL);

	// BITMAPINFOHEADER 쓰기
	WriteFile(hFile, &bmInfoHeaderDest, sizeof(bmInfoHeaderDest), &dwRWC, NULL);

	// 픽셀을 저장한다...
	RECT rcPixel = { rc.left, m_bmInfoHeader.biHeight - rc.bottom, rc.right, m_bmInfoHeader.biHeight - rc.top };
	int iRealWidthSrc = ((int)((m_bmInfoHeader.biWidth*3 + 3)/4))*4;	
	for(int y = rcPixel.bottom - 1; y >= rcPixel.top; y--)
	{
		void* pPixelDest = ((byte*)m_pPixels) + iRealWidthSrc * y + (rcPixel.left * 3);
		WriteFile(hFile, pPixelDest, iRealWidthDest, &dwRWC, NULL); // 라인 쓰기..
	}

	CloseHandle(hFile);
	return true;
}
