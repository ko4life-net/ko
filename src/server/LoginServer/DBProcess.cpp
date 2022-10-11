// DBProcess.cpp: implementation of the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LoginServer.h"
#include "define.h"
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

BOOL CDBProcess::InitDatabase(char * strconnection) {
    m_VersionDB.SetLoginTimeout(100);

    m_pMain = (CLoginServerDlg *)AfxGetApp()->GetMainWnd();

    if (!m_VersionDB.Open(NULL, FALSE, FALSE, strconnection)) {
        return FALSE;
    }

    return TRUE;
}

void CDBProcess::ReConnectODBC(CDatabase * m_db, const char * strdb, const char * strname, const char * strpwd) {
    char strlog[256];
    memset(strlog, 0x00, 256);
    CTime t = CTime::GetCurrentTime();
    sprintf(strlog, "Try ReConnectODBC... %d월 %d일 %d시 %d분\r\n", t.GetMonth(), t.GetDay(), t.GetHour(),
            t.GetMinute());
    LogFileWrite(strlog);

    // DATABASE 연결...
    CString strConnect;
    strConnect.Format(_T("DSN=%s;UID=%s;PWD=%s"), strdb, strname, strpwd);
    int iCount = 0;

    do {
        iCount++;
        if (iCount >= 4) {
            break;
        }

        m_db->SetLoginTimeout(10);

        try {
            m_db->OpenEx((LPCTSTR)strConnect, CDatabase::noOdbcDialog);
        } catch (CDBException * e) {
            e->Delete();
        }

    } while (!m_db->IsOpen());
}

BOOL CDBProcess::LoadVersionList() {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];

    CString tempfilename, tempcompname;

    memset(szSQL, 0x00, 1024);
    wsprintf(szSQL, TEXT("select * from %s"), m_pMain->m_TableName);

    SQLSMALLINT version = 0, historyversion = 0;
    TCHAR       strfilename[256], strcompname[256];
    memset(strfilename, NULL, 256);
    memset(strcompname, NULL, 256);
    SQLINTEGER Indexind = SQL_NTS;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_ODBCName, m_pMain->m_ODBCLogin, m_pMain->m_ODBCPwd);
                return FALSE;
            }
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        return FALSE;
    }
    while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_SSHORT, &version, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_CHAR, strfilename, 256, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_CHAR, strcompname, 256, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_SSHORT, &historyversion, 0, &Indexind);

            _VERSION_INFO * pInfo = new _VERSION_INFO;

            tempfilename = strfilename;
            tempcompname = strcompname;
            tempfilename.TrimRight();
            tempcompname.TrimRight();

            pInfo->sVersion = version;
            pInfo->strFileName = tempfilename;
            pInfo->strCompName = tempcompname;
            pInfo->sHistoryVersion = historyversion;

            if (!m_pMain->m_VersionList.PutData(pInfo->strFileName, pInfo)) {
                TRACE("VersionInfo PutData Fail - %d\n", pInfo->strFileName);
                delete pInfo;
                pInfo = NULL;
            }
        }
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    m_pMain->m_nLastVersion = 0;

    map<string, _VERSION_INFO *>::iterator Iter1, Iter2;
    Iter1 = m_pMain->m_VersionList.m_UserTypeMap.begin();
    Iter2 = m_pMain->m_VersionList.m_UserTypeMap.end();
    for (; Iter1 != Iter2; Iter1++) {
        if (m_pMain->m_nLastVersion < ((*Iter1).second)->sVersion) {
            m_pMain->m_nLastVersion = ((*Iter1).second)->sVersion;
        }
    }

    return TRUE;
}

int CDBProcess::AccountLogin(const char * id, const char * pwd) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet = 3;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call ACCOUNT_LOGIN(\'%s\',\'%s\',?)}"), id, pwd);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_VersionDB.Close();
                    if (!m_VersionDB.IsOpen()) {
                        ReConnectODBC(&m_VersionDB, m_pMain->m_ODBCName, m_pMain->m_ODBCLogin, m_pMain->m_ODBCPwd);
                        return 2;
                    }
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return sParmRet;
}

int CDBProcess::MgameLogin(const char * id, const char * pwd) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet = -1;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call MGAME_LOGIN(\'%s\',\'%s\',?)}"), id, pwd);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_VersionDB.Close();
                    if (!m_VersionDB.IsOpen()) {
                        ReConnectODBC(&m_VersionDB, m_pMain->m_ODBCName, m_pMain->m_ODBCLogin, m_pMain->m_ODBCPwd);
                        return 2;
                    }
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return sParmRet;
}

BOOL CDBProcess::InsertVersion(int version, const char * filename, const char * compname, int historyversion) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    BOOL retvalue = TRUE;

    wsprintf(
        szSQL,
        TEXT(
            "INSERT INTO %s (sVersion, strFileName, strCompressName, sHistoryVersion) VALUES (%d, \'%s\', \'%s\', %d)"),
        m_pMain->m_TableName, version, filename, compname, historyversion);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            DisplayErrorMsg(hstmt);
            retvalue = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return retvalue;
}

BOOL CDBProcess::DeleteVersion(const char * filename) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    BOOL retvalue = TRUE;

    wsprintf(szSQL, TEXT("DELETE FROM %s WHERE strFileName = \'%s\'"), m_pMain->m_TableName, filename);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            DisplayErrorMsg(hstmt);
            retvalue = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return retvalue;
}

BOOL CDBProcess::LoadUserCountList() {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];

    CString tempfilename, tempcompname;

    memset(szSQL, 0x00, 1024);
    wsprintf(szSQL, TEXT("select * from CONCURRENT"));

    SQLCHAR     serverid;
    SQLSMALLINT zone_1 = 0, zone_2 = 0, zone_3 = 0;
    SQLINTEGER  Indexind = SQL_NTS;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_ODBCName, m_pMain->m_ODBCLogin, m_pMain->m_ODBCPwd);
                return FALSE;
            }
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        return FALSE;
    }
    while (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_TINYINT, &serverid, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_SSHORT, &zone_1, 0, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_SSHORT, &zone_2, 0, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_SSHORT, &zone_3, 0, &Indexind);

            // 여기에서 데이타를 받아서 알아서 사용....
            if (serverid - 1 < m_pMain->m_nServerCount) {
                m_pMain->m_ServerList[serverid - 1]->sUserCount = zone_1 + zone_2 + zone_3; // 기범이가 ^^;
            }
        }
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    return TRUE;
}

BOOL CDBProcess::IsCurrentUser(const char * accountid, char * strServerIP, int & serverno) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      retval;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLINTEGER nServerNo = 0;
    TCHAR      strIP[20];
    memset(strIP, 0x00, 20);
    SQLINTEGER Indexind = SQL_NTS;

    wsprintf(szSQL, TEXT("SELECT nServerNo, strServerIP FROM CURRENTUSER WHERE strAccountID = \'%s\'"), accountid);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_VersionDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_SSHORT, &nServerNo, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_CHAR, strIP, 20, &Indexind);

            strcpy(strServerIP, strIP);
            serverno = nServerNo;
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_VersionDB.Close();
            if (!m_VersionDB.IsOpen()) {
                ReConnectODBC(&m_VersionDB, m_pMain->m_ODBCName, m_pMain->m_ODBCLogin, m_pMain->m_ODBCPwd);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    return retval;
}
