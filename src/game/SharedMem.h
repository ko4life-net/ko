// SharedMem.h: interface for the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define WM_RECEIVEDATA WM_USER + 21

struct _SMQ_HEADER {
    BOOL   Connect;
    WORD   Head;
    WORD   Tail;
    char * Data;
};

class CSharedMemQueue //:public CObject
{
  private:
  public:
    CSharedMemQueue();
    virtual ~CSharedMemQueue();

    void DestroySMQ();
    int  GetData(char * pBuf);
    BOOL PutData(char * pBuf, WORD);
    BOOL IsPartner();

    BOOL CreateSmq(BOOL Server);
    // hwnd settings to deliver messages
    void SetReadData();
    // The read data is brought into the buffer.
    void GetReadData(LPSTR data, int nLength);
    // Set the Windows platform to send messages.
    void SetHwnd(HWND hwnd);
    void CloseSmq();
    BOOL IsData();

    HANDLE       m_hReadQueueThread;
    HANDLE       m_hrMMFile;
    HANDLE       m_hsMMFile;
    char *       m_lpMMFile;
    HWND         m_hwnd;         // Windows plug to transmit messages
    BYTE         abIn[1024 + 1]; // Data coming from Comfort
    BOOL         fConnected;
    _SMQ_HEADER *Txd, *Rxd;
};
