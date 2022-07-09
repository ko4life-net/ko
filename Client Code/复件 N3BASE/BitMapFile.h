// BitMapFile.h: interface for the CBitMapFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_)
#define AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CBitMapFile : public CN3Base
{
protected:
	BITMAPFILEHEADER m_bmfHeader;
	BITMAPINFOHEADER m_bmInfoHeader;
	void* m_pPixels; // 실제 픽셀 데이터

public:
	bool SaveRectToFile(const char* szFileName, RECT rc);
	void* Pixels();

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	int Width() { return m_bmInfoHeader.biWidth; }
	int Height() { return m_bmInfoHeader.biHeight; }
	
	void Release();
	CBitMapFile();
	virtual ~CBitMapFile();

};

#endif // !defined(AFX_BITMAPFILE_H__F2C24227_F5AE_43B2_8616_2F2E057B59B8__INCLUDED_)
