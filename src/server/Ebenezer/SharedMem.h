// SharedMem.h: interface for the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "define.h"

struct _SMQ_HEADER {
    LONG  Front;
    LONG  Rear;
    short nCount;

    BYTE FrontMode;
    BYTE RearMode;

    DWORD WritePid;
    DWORD ReadPid;

    DWORD CreatePid;
};

class CSharedMemQueue {
  private:
    HANDLE m_hMMFile;
    char * m_lpMMFile;

    BOOL          m_bMMFCreate;
    short         m_nMaxCount;
    DWORD         m_wOffset;
    LONG          m_lReference;
    _SMQ_HEADER * m_pHeader;

  public:
    inline LONG  GetFrontPointer() { return m_pHeader->Front; };
    inline LONG  GetRearPointer() { return m_pHeader->Rear; };
    inline DWORD GetProcessId() { return m_pHeader->CreatePid; };
    inline BYTE  GetFrontMode() { return m_pHeader->FrontMode; };
    inline BYTE  GetRearMode() { return m_pHeader->RearMode; };
    inline int   GetCount() { return m_pHeader->nCount; };
    int          GetData(char * pBuf);
    int          PutData(char * pBuf, int size);
    BOOL         InitailizeMMF(DWORD dwOffsetsize, int maxcount, LPCTSTR lpname, BOOL bCreate = TRUE);
    CSharedMemQueue();
    virtual ~CSharedMemQueue();
};
