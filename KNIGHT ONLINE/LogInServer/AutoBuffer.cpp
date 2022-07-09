// AutoBuffer.cpp: implementation of the CAutoBuffer class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoBuffer::CAutoBuffer()
{
	m_iSize = 0;
	m_pBuffer = NULL;	
}

CAutoBuffer::CAutoBuffer(DWORD iSize, bool bZeroMemory)
{
	m_iSize = 0;
	m_pBuffer = NULL;
	Allocate(iSize, bZeroMemory);
}

CAutoBuffer::~CAutoBuffer()
{
	Release();
}

bool CAutoBuffer::IsAllocated()
{
	return (m_pBuffer != NULL);
}


DWORD CAutoBuffer::GetSize()
{
	return m_iSize;
}

void CAutoBuffer::Release()
{
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_iSize = 0;
		m_pBuffer = NULL;
	}
}

char* CAutoBuffer::Allocate(DWORD iSize, bool bZeroMemory)
{
	if (iSize != m_iSize)
		Release();
	else
	{
		if (bZeroMemory)
			memset(m_pBuffer, 0, iSize); // zerowanie bufora
		return m_pBuffer;
	}

	if (iSize > 0)
	{
// 		try
// 		{
			m_pBuffer = new char [iSize];


			m_iSize = iSize;
// 		}
// 		catch (CMemoryException *e)
// 		{
// 			e->Delete();
// 			return NULL;
// 		}
	}
	else 
		m_pBuffer = NULL;

	return m_pBuffer;
}

CAutoBuffer::operator char*()
{
	return m_pBuffer;
}

CAutoBuffer::CAutoBuffer(CAutoBuffer& buffer)
{
	m_pBuffer = NULL;
	m_iSize = 0;

	if (buffer.m_pBuffer)
	{
		Allocate(buffer.m_iSize);
		memcpy(m_pBuffer, buffer.m_pBuffer, buffer.m_iSize);
	}	
}
CAutoBuffer& CAutoBuffer::operator=(const CAutoBuffer& buffer)
{
	if (this == &buffer)
		return *this;
	Release();
	if (buffer.m_pBuffer)
	{
		Allocate(buffer.m_iSize);
		memcpy(m_pBuffer, buffer.m_pBuffer, buffer.m_iSize);
	}
	return *this;
}