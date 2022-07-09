// SharedMem.h: interface for the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_)
#define AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"

struct _SMQ_HEADER
{
	LONG Front;
	LONG Rear;
	short	nCount;

	BYTE FrontMode;
	BYTE RearMode;

	int  WritePid;
	int	 ReadPid;
	
	DWORD CreatePid;
};

class CSharedMemQueue  
{
private:
	HANDLE	m_hMMFile;
	char*	m_lpMMFile;

	BOOL	m_bMMFCreate;
	short	m_nMaxCount;
	DWORD	m_wOffset;
	LONG	m_lReference;
	_SMQ_HEADER*	m_pHeader;

public:
	inline LONG GetFrontPointer() {return m_pHeader->Front;};
	inline LONG GetRearPointer() {return m_pHeader->Rear;};
	inline DWORD GetProcessId() {return m_pHeader->CreatePid;};
	inline BYTE GetFrontMode() {return m_pHeader->FrontMode;};
	inline BYTE GetRearMode() {return m_pHeader->RearMode;};
	inline int GetCount() {return m_pHeader->nCount;};
	int GetData( char* pBuf );
	int PutData( char* pBuf, int size );
	BOOL InitailizeMMF(DWORD dwOffsetsize, int maxcount, LPCTSTR lpname, BOOL bCreate = TRUE);
	CSharedMemQueue();
	virtual ~CSharedMemQueue();
};

#endif // !defined(AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_)
