// DBProcess.cpp: implementation of the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LoginServer.h"
#include "Define.h"
#include "DBProcess.h"
#include "LoginServerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBProcess::CDBProcess() {}

CDBProcess::~CDBProcess() {}

BOOL CDBProcess::InitDatabase(char * szConnectString) {
    m_VersionDB.SetLoginTimeout(100);

    m_pMain = (CLoginServerDlg *)AfxGetApp()->GetMainWnd();

    if (!m_VersionDB.Open(NULL, FALSE, FALSE, szConnectString)) {
        return FALSE;
    }

    return TRUE;
}

void CDBProcess::ReConnectODBC(CDatabase * pDb, const char * szDsn, const char * szUid, const char * szPwd) {
    CTime t = CTime::GetCurrentTime();
    n3std::log_file_write(std::format("Try ReConnectODBC... {:d} month {:d} day {:d} hour {:d} minute", t.GetMonth(),
                                      t.GetDay(), t.GetHour(), t.GetMinute())
                              .c_str());

    // DATABASE 연결...
    std::string szConnStr = std::format("DSN={:s};UID={:s};PWD={:s}", szDsn, szUid, szPwd);

    int iCount = 0;
    while (iCount++ < 4 && !pDb->IsOpen()) {
        pDb->SetLoginTimeout(10);
        try {
            pDb->OpenEx(szConnStr.c_str(), CDatabase::noOdbcDialog);
        } catch (CDBException * e) {
            e->Delete();
        }
    }
}

BOOL CDBProcess::LoadVersionList() {
    char szSQL[1024]{};
    wsprintf(szSQL, TEXT("SELECT * FROM %s"), m_pMain->m_TableName);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc != SQL_SUCCESS) {
        return FALSE;
    }

    rc = SQLExecDirect(hStmt, (unsigned char *)szSQL, SQL_NTS);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        if (DisplayErrorMsg(hStmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_szOdbcLogDsn, m_pMain->m_szOdbcLogUid, m_pMain->m_szOdbcLogPwd);
                return FALSE;
            }
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
        return FALSE;
    }

    while (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        rc = SQLFetch(hStmt);
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
            SQLSMALLINT sVersion = 0, sHistoryVersion = 0;
            wchar_t     szFile[261]{};
            char        szPatchFileName[14]{};

            SQLINTEGER cbParmRet = SQL_NTS;
            SQLGetData(hStmt, 1, SQL_C_SSHORT, &sVersion, 0, &cbParmRet);
            SQLGetData(hStmt, 2, SQL_C_WCHAR, szFile, sizeof(szFile), &cbParmRet);
            SQLGetData(hStmt, 3, SQL_C_CHAR, szPatchFileName, sizeof(szPatchFileName), &cbParmRet);
            SQLGetData(hStmt, 4, SQL_C_SSHORT, &sHistoryVersion, 0, &cbParmRet);

            _VERSION_INFO * pInfo = new _VERSION_INFO;
            pInfo->sVersion = sVersion;
            pInfo->fsFile = szFile;
            pInfo->fsPatchFileName = szPatchFileName;
            pInfo->sHistoryVersion = sHistoryVersion;
            if (!m_pMain->m_VersionList.PutData(pInfo->fsFile, pInfo)) {
                TRACE("VersionInfo PutData Fail - %d\n", pInfo->fsFile);
                delete pInfo;
                pInfo = NULL;
            }
        }
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);

    m_pMain->m_nLastVersion = 0;
    for (const auto & [_, pInfo] : m_pMain->m_VersionList.m_UserTypeMap) {
        m_pMain->m_nLastVersion = std::max(m_pMain->m_nLastVersion, pInfo->sVersion);
    }

    return TRUE;
}

int CDBProcess::AccountLogin(const char * szId, const char * szPwd) {
    SQLSMALLINT sParmRet = 3;

    char szSQL[1024]{};
    wsprintf(szSQL, TEXT("{CALL ACCOUNT_LOGIN(\'%s\',\'%s\',?)}"), szId, szPwd);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc == SQL_SUCCESS) {
        SQLINTEGER cbParmRet = SQL_NTS;
        rc = SQLBindParameter(hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (rc == SQL_SUCCESS) {
            rc = SQLExecDirect(hStmt, (unsigned char *)szSQL, SQL_NTS);
            if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
                if (DisplayErrorMsg(hStmt) == -1) {
                    m_VersionDB.Close();
                    if (!m_VersionDB.IsOpen()) {
                        ReConnectODBC(&m_VersionDB, m_pMain->m_szOdbcLogDsn, m_pMain->m_szOdbcLogUid,
                                      m_pMain->m_szOdbcLogPwd);
                        return 2;
                    }
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
    }

    return sParmRet;
}

int CDBProcess::MgameLogin(const char * szId, const char * szPwd) {
    SQLSMALLINT sParmRet = -1;

    char szSQL[1024]{};
    wsprintf(szSQL, TEXT("{CALL MGAME_LOGIN(\'%s\',\'%s\',?)}"), szId, szPwd);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc == SQL_SUCCESS) {
        SQLINTEGER cbParmRet = SQL_NTS;
        rc = SQLBindParameter(hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (rc == SQL_SUCCESS) {
            rc = SQLExecDirect(hStmt, (unsigned char *)szSQL, SQL_NTS);
            if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
                if (DisplayErrorMsg(hStmt) == -1) {
                    m_VersionDB.Close();
                    if (!m_VersionDB.IsOpen()) {
                        ReConnectODBC(&m_VersionDB, m_pMain->m_szOdbcLogDsn, m_pMain->m_szOdbcLogUid,
                                      m_pMain->m_szOdbcLogPwd);
                        return 2;
                    }
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
    }

    return sParmRet;
}

BOOL CDBProcess::InsertVersion(short sVersion, const fs::path & fsFile, const fs::path & fsPatchFileName,
                               short sHistoryVersion) {
    BOOL retvalue = TRUE;

    std::string szSql = std::format(
        "INSERT INTO {:s} (sVersion, strFile, strPatchFileName, sHistoryVersion) VALUES ({:d}, '{:s}', '{:s}', {:d})",
        m_pMain->m_TableName, sVersion, fsFile, fsPatchFileName, sHistoryVersion);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc == SQL_SUCCESS) {
        rc = SQLExecDirect(hStmt, (SQLCHAR *)szSql.c_str(), SQL_NTS);
        if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
            DisplayErrorMsg(hStmt);
            retvalue = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
    }

    return retvalue;
}

BOOL CDBProcess::DeleteVersion(const fs::path & fsFile) {
    BOOL retvalue = TRUE;

    std::string szSql = std::format("DELETE FROM {:s} WHERE strFile = '{:s}'", m_pMain->m_TableName, fsFile);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc == SQL_SUCCESS) {
        rc = SQLExecDirect(hStmt, (SQLCHAR *)szSql.c_str(), SQL_NTS);
        if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
            DisplayErrorMsg(hStmt);
            retvalue = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
    }

    return retvalue;
}

BOOL CDBProcess::LoadUserCountList() {
    TCHAR szSQL[1024]{};
    wsprintf(szSQL, TEXT("SELECT * FROM CONCURRENT"));

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc != SQL_SUCCESS) {
        return FALSE;
    }

    rc = SQLExecDirect(hStmt, (unsigned char *)szSQL, SQL_NTS);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        if (DisplayErrorMsg(hStmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_szOdbcLogDsn, m_pMain->m_szOdbcLogUid, m_pMain->m_szOdbcLogPwd);
                return FALSE;
            }
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);
        return FALSE;
    }

    while (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        rc = SQLFetch(hStmt);
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
            SQLCHAR     sServerId = 0;
            SQLSMALLINT sZone1Count = 0, sZone2Count = 0, sZone3Count = 0;

            SQLINTEGER cbParmRet = SQL_NTS;
            SQLGetData(hStmt, 1, SQL_C_TINYINT, &sServerId, 0, &cbParmRet);
            SQLGetData(hStmt, 2, SQL_C_SSHORT, &sZone1Count, 0, &cbParmRet);
            SQLGetData(hStmt, 3, SQL_C_SSHORT, &sZone2Count, 0, &cbParmRet);
            SQLGetData(hStmt, 4, SQL_C_SSHORT, &sZone3Count, 0, &cbParmRet);

            // 여기에서 데이타를 받아서 알아서 사용....
            if (sServerId - 1 < m_pMain->m_nServerCount) {
                m_pMain->m_ServerList[sServerId - 1]->sUserCount =
                    sZone1Count + sZone2Count + sZone3Count; // 기범이가 ^^;
            }
        }
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);

    return TRUE;
}

BOOL CDBProcess::IsCurrentUser(const char * szAccountId, char * szServerIp, int & nServerNo) {
    BOOL retval = FALSE;

    TCHAR szSQL[1024]{};
    wsprintf(szSQL, TEXT("SELECT nServerNo, strServerIP FROM CURRENTUSER WHERE strAccountID = \'%s\'"), szAccountId);

    SQLHSTMT  hStmt = NULL;
    SQLRETURN rc = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hStmt);
    if (rc != SQL_SUCCESS) {
        return FALSE;
    }

    rc = SQLExecDirect(hStmt, (unsigned char *)szSQL, SQL_NTS);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        rc = SQLFetch(hStmt);
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
            SQLINTEGER nServerNo = 0;
            TCHAR      szServerIp[20]{};

            SQLINTEGER cbParmRet = SQL_NTS;
            SQLGetData(hStmt, 1, SQL_C_SSHORT, &nServerNo, 0, &cbParmRet);
            SQLGetData(hStmt, 2, SQL_C_CHAR, szServerIp, 20, &cbParmRet);

            strcpy(szServerIp, szServerIp);
            nServerNo = nServerNo;
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hStmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_szOdbcLogDsn, m_pMain->m_szOdbcLogUid, m_pMain->m_szOdbcLogPwd);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hStmt);

    return retval;
}
