// DBAgent.cpp: implementation of the CDBAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ItemManager.h"
#include "DBAgent.h"
#include "ItemManagerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBAgent::CDBAgent() {}

CDBAgent::~CDBAgent() {}

BOOL CDBAgent::DatabaseInit() {
    //    Main DB Connecting..
    /////////////////////////////////////////////////////////////////////////////////////
    m_pMain = (CItemManagerDlg *)AfxGetApp()->GetMainWnd();

    CString szConnStr = std::format("ODBC;DSN={:s};UID={:s};PWD={:s}", m_pMain->m_szOdbcGameDsn,
                                    m_pMain->m_szOdbcGameUid, m_pMain->m_szOdbcGamePwd)
                            .c_str();

    m_GameDB.SetLoginTimeout(10);
    if (!m_GameDB.Open(NULL, FALSE, FALSE, szConnStr)) {
        AfxMessageBox("GameDB SQL Connection Fail...");
        return FALSE;
    }

    return TRUE;
}

void CDBAgent::ReConnectODBC(CDatabase * pDb, const CString & szConnStr) {
    char strlog[256];
    memset(strlog, 0x00, 256);
    CTime t = CTime::GetCurrentTime();
    sprintf(strlog, "Try ReConnectODBC... %d월 %d일 %d시 %d분\r\n", t.GetMonth(), t.GetDay(), t.GetHour(),
            t.GetMinute());
    m_pMain->m_ItemLogFile.Write(strlog, strlen(strlog));

    int iCount = 0;
    do {
        iCount++;
        if (iCount >= 4) {
            break;
        }

        pDb->SetLoginTimeout(10);

        try {
            pDb->OpenEx(szConnStr, CDatabase::noOdbcDialog);
        } catch (CDBException * e) {
            e->Delete();
        }

    } while (!pDb->IsOpen());
}
