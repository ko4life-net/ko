// N3JpegFile.h: interface for the CN3JpegFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/JpegFile.h"

class CN3JpegFile  : public CJpegFile
{
private:
	BYTE*		m_pImageBuf;
	UINT		m_uiWidth;
	UINT		m_uiHeight;
public:
	void		DrawImage(HDC hDC);
	BOOL		LoadJpegFile(std::string csJpeg);
	UINT		GetWidth() { return m_uiWidth; }
	UINT		GetHeight() { return m_uiHeight; }
	BYTE*		GetImageBuf() { return m_pImageBuf; }
	void		Release();

	CN3JpegFile();
	virtual ~CN3JpegFile();

};

