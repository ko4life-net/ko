// DBProcess.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CLoginServerDlg;
class CDBProcess {
  public:
    BOOL IsCurrentUser(const char * szAccountId, char * szServerIp, int & nServerNo);
    void ReConnectODBC(CDatabase * pDb, const char * szDsn, const char * szUid, const char * szPwd);
    BOOL DeleteVersion(const fs::path & fsFile);
    BOOL InsertVersion(short sVersion, const fs::path & fsFile, const fs::path & fsPatchFileName,
                       short sHistoryVersion);
    BOOL InitDatabase(char * szConnectString);
    int  MgameLogin(const char * szId, const char * szPwd);
    int  AccountLogin(const char * szId, const char * szPwd);
    BOOL LoadVersionList();
    BOOL LoadUserCountList();
    BOOL LoadNews();

    CDBProcess();
    virtual ~CDBProcess();

    CDatabase         m_VersionDB;
    CLoginServerDlg * m_pMain;
};
