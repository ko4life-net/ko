// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "IOCPSocket2.h"

class CLoginServerDlg;
class CUser : public CIOCPSocket2 {
  public:
    CUser();
    virtual ~CUser();

    void Initialize();
    void Parsing(int len, char * pData);
    void CloseProcess();

    void SendDownloadInfo(int version);
    void MgameLogin(char * pBuf);
    void LogInReq(char * pBuf);

    CLoginServerDlg * m_pMain;
};
