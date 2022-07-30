// AutoBuffer.h: interface for the CAutoBuffer class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
////////////////////////////////////////////////////////////////////////////////

#pragma once


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

