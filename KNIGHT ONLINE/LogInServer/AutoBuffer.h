// AutoBuffer.h: interface for the CAutoBuffer class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOBUFFER_H__DEC28C20_83FE_11D3_B7C3_EDEC47A8A86C__INCLUDED_)
#define AFX_AUTOBUFFER_H__DEC28C20_83FE_11D3_B7C3_EDEC47A8A86C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutoBuffer  
{
public:
	operator char*();
	char* Allocate(DWORD iSize, bool bZeroMemory = false);
	void Release();
	DWORD GetSize();
	bool IsAllocated();
	CAutoBuffer(DWORD iSize, bool bZeroMemory = false);
	CAutoBuffer();
	CAutoBuffer(CAutoBuffer& buffer);
	virtual ~CAutoBuffer();
	CAutoBuffer& operator=(const CAutoBuffer& buffer);
protected:
	char* m_pBuffer;
	DWORD m_iSize;
};

#endif // !defined(AFX_AUTOBUFFER_H__DEC28C20_83FE_11D3_B7C3_EDEC47A8A86C__INCLUDED_)
