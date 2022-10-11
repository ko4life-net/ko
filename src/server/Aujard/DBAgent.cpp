// DBAgent.cpp: implementation of the CDBAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Aujard.h"
#include "DBAgent.h"
#include "AujardDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CRITICAL_SECTION g_LogFileWrite;

CDBAgent::CDBAgent() {}

CDBAgent::~CDBAgent() {}

BOOL CDBAgent::DatabaseInit() {
    //    Main DB Connecting..
    /////////////////////////////////////////////////////////////////////////////////////
    m_pMain = (CAujardDlg *)AfxGetApp()->GetMainWnd();

    CString strConnect;
    strConnect.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s"), m_pMain->m_strGameDSN, m_pMain->m_strGameUID,
                      m_pMain->m_strGamePWD);

    m_GameDB.SetLoginTimeout(10);
    if (!m_GameDB.Open(NULL, FALSE, FALSE, strConnect)) {
        AfxMessageBox("GameDB SQL Connection Fail...");
        return FALSE;
    }

    strConnect.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s"), m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                      m_pMain->m_strAccountPWD);

    m_AccountDB.SetLoginTimeout(10);

    if (!m_AccountDB.Open(NULL, FALSE, FALSE, strConnect)) {
        AfxMessageBox("AccountDB SQL Connection Fail...");
        return FALSE;
    }

    m_AccountDB1.SetLoginTimeout(10);

    if (!m_AccountDB1.Open(NULL, FALSE, FALSE, strConnect)) {
        AfxMessageBox("AccountDB1 SQL Connection Fail...");
        return FALSE;
    }

    return TRUE;
}

void CDBAgent::ReConnectODBC(CDatabase * m_db, char * strdb, char * strname, char * strpwd) {
    char strlog[256];
    memset(strlog, 0x00, 256);
    CTime t = CTime::GetCurrentTime();
    sprintf(strlog, "Try ReConnectODBC... \r\n");
    m_pMain->WriteLogFile(strlog);
    //m_pMain->m_LogFile.Write(strlog, strlen(strlog));

    // DATABASE 연결...
    CString strConnect;
    strConnect.Format(_T("DSN=%s;UID=%s;PWD=%s"), strdb, strname, strpwd);
    int iCount = 0;

    DBProcessNumber(1);

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

void CDBAgent::MUserInit(int uid) {
    _USER_DATA * pUser = NULL;

    pUser = (_USER_DATA *)m_UserDataArray[uid];
    if (!pUser) {
        return;
    }

    memset(pUser->m_id, 0x00, MAX_ID_SIZE + 1);
    memset(pUser->m_Accountid, 0x00, MAX_ID_SIZE + 1);

    pUser->m_bZone = 0;
    pUser->m_curx = 0;
    pUser->m_curz = 0;
    pUser->m_cury = 0;

    pUser->m_bNation = 0;
    pUser->m_bRace = 0;
    pUser->m_sClass = 0;
    pUser->m_bHairColor = 0;
    pUser->m_bRank = 0;
    pUser->m_bTitle = 0;
    pUser->m_bLevel = 0;
    pUser->m_iExp = 0;
    pUser->m_iLoyalty = 0;
    pUser->m_bFace = 0;
    pUser->m_bCity = 0;
    pUser->m_bKnights = 0;
    //pUser->m_sClan = 0;
    pUser->m_bFame = 0;
    pUser->m_sHp = 0;
    pUser->m_sMp = 0;
    pUser->m_sSp = 0;
    pUser->m_bStr = 0;
    pUser->m_bSta = 0;
    pUser->m_bDex = 0;
    pUser->m_bIntel = 0;
    pUser->m_bCha = 0;
    pUser->m_iGold = 0;
    pUser->m_sBind = -1;
    pUser->m_iBank = 0;

    // 스킬 초기화
    for (int i = 0; i < 9; i++) {
        pUser->m_bstrSkill[i] = 0;
    }

    for (int i = 0; i < SLOT_MAX + HAVE_MAX; i++) { // 착용갯수 + 소유갯수(14+28=42)
        pUser->m_sItemArray[i].nNum = 0;
        pUser->m_sItemArray[i].sDuration = 0;
        pUser->m_sItemArray[i].sCount = 0;
    }
    for (int i = 0; i < WAREHOUSE_MAX; i++) {
        pUser->m_sWarehouseArray[i].nNum = 0;
        pUser->m_sWarehouseArray[i].sDuration = 0;
        pUser->m_sWarehouseArray[i].sCount = 0;
    }
    pUser->m_bLogout = 0;
    pUser->m_bWarehouse = 0;
    pUser->m_dwTime = 0;
}

BOOL CDBAgent::LoadUserData(char * userid, int uid) {
    SQLHSTMT     hstmt;
    SQLRETURN    retcode;
    BOOL         retval;
    _USER_DATA * pUser = NULL;
    TCHAR        szSQL[1024];
    memset(szSQL, 0x00, 1024);

    //wsprintf(szSQL, TEXT("{? = call LOAD_USER_DATA ('%s')}"), userid);
    wsprintf(szSQL, TEXT("{call LOAD_USER_DATA ('%s', ?)}"), userid);

    SQLCHAR     Nation, Race, HairColor, Rank, Title, Level;
    SQLINTEGER  Exp, Loyalty, Gold, PX, PZ, PY, dwTime;
    SQLCHAR     Face, City, Fame, Authority, Points;
    SQLSMALLINT Hp, Mp, Sp, sRet, Class, Bind, Knights;
    SQLCHAR     Str, Sta, Dex, Intel, Cha, Zone;
    TCHAR       strSkill[10], strItem[400], strSerial[400];
    memset(strSkill, 0x00, 10);
    memset(strItem, 0x00, 400);
    memset(strSerial, 0x00, 400);

    SQLINTEGER Indexind = SQL_NTS;

    hstmt = NULL;

    DBProcessNumber(2);

    char logstr[256];
    memset(logstr, 0x00, 256);
    sprintf(logstr, "LoadUserData : name=%s\r\n", userid);
    //m_pMain->m_LogFile.Write(logstr, strlen(logstr));

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData Fail 000 : name=%s\r\n", userid);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }

    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &Indexind);
    if (retcode != SQL_SUCCESS) {
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData Fail : name=%s, retcode=%d\r\n", userid, retcode);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS) { //|| retcode == SQL_SUCCESS_WITH_INFO){
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_TINYINT, &Nation, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_TINYINT, &Race, 0, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_SSHORT, &Class, 0, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_TINYINT, &HairColor, 0, &Indexind);
            SQLGetData(hstmt, 5, SQL_C_TINYINT, &Rank, 0, &Indexind);
            SQLGetData(hstmt, 6, SQL_C_TINYINT, &Title, 0, &Indexind);
            SQLGetData(hstmt, 7, SQL_C_TINYINT, &Level, 0, &Indexind);
            SQLGetData(hstmt, 8, SQL_C_LONG, &Exp, 0, &Indexind);
            SQLGetData(hstmt, 9, SQL_C_LONG, &Loyalty, 0, &Indexind);
            SQLGetData(hstmt, 10, SQL_C_TINYINT, &Face, 0, &Indexind);
            SQLGetData(hstmt, 11, SQL_C_TINYINT, &City, 0, &Indexind);
            SQLGetData(hstmt, 12, SQL_C_SSHORT, &Knights, 0, &Indexind);
            SQLGetData(hstmt, 13, SQL_C_TINYINT, &Fame, 0, &Indexind);
            SQLGetData(hstmt, 14, SQL_C_SSHORT, &Hp, 0, &Indexind);
            SQLGetData(hstmt, 15, SQL_C_SSHORT, &Mp, 0, &Indexind);
            SQLGetData(hstmt, 16, SQL_C_SSHORT, &Sp, 0, &Indexind);
            SQLGetData(hstmt, 17, SQL_C_TINYINT, &Str, 0, &Indexind);
            SQLGetData(hstmt, 18, SQL_C_TINYINT, &Sta, 0, &Indexind);
            SQLGetData(hstmt, 19, SQL_C_TINYINT, &Dex, 0, &Indexind);
            SQLGetData(hstmt, 20, SQL_C_TINYINT, &Intel, 0, &Indexind);
            SQLGetData(hstmt, 21, SQL_C_TINYINT, &Cha, 0, &Indexind);
            SQLGetData(hstmt, 22, SQL_C_TINYINT, &Authority, 0, &Indexind);
            SQLGetData(hstmt, 23, SQL_C_TINYINT, &Points, 0, &Indexind);
            SQLGetData(hstmt, 24, SQL_C_LONG, &Gold, 0, &Indexind);
            SQLGetData(hstmt, 25, SQL_C_TINYINT, &Zone, 0, &Indexind);
            SQLGetData(hstmt, 26, SQL_C_SSHORT, &Bind, 0, &Indexind);
            SQLGetData(hstmt, 27, SQL_C_LONG, &PX, 0, &Indexind);
            SQLGetData(hstmt, 28, SQL_C_LONG, &PZ, 0, &Indexind);
            SQLGetData(hstmt, 29, SQL_C_LONG, &PY, 0, &Indexind);
            SQLGetData(hstmt, 30, SQL_C_LONG, &dwTime, 0, &Indexind);
            SQLGetData(hstmt, 31, SQL_C_CHAR, strSkill, 10, &Indexind);
            SQLGetData(hstmt, 32, SQL_C_CHAR, strItem, 400, &Indexind);
            SQLGetData(hstmt, 33, SQL_C_CHAR, strSerial, 400, &Indexind);
            retval = TRUE;
        } else {
            memset(logstr, 0x00, 256);
            sprintf(logstr, "LoadUserData Fail 222 : name=%s, retcode=%d\r\n", userid, retcode);
            //        m_pMain->m_LogFile.Write(logstr, strlen(logstr));
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            char logstr[256];
            memset(logstr, 0x00, 256);
            sprintf(logstr, "[Error-DB Fail] LoadUserData : name=%s\r\n", userid);
            //        m_pMain->m_LogFile.Write(logstr, strlen(logstr));

            m_GameDB.Close();
            if (!m_GameDB.IsOpen()) {
                ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                return FALSE;
            }
        }
        retval = FALSE;

        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData Fail 333 : name=%s, retcode=%d\r\n", userid, retcode);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    // 엠겜 유저가 아니다.
    /*    if(sRet == 0)    {
        memset( logstr, 0x00, 256);
        sprintf( logstr, "LoadUserData Fail : name=%s, sRet= %d, retval=%d, nation=%d \r\n", userid, sRet, retval, Nation );
        m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }    */

    if (retval == FALSE) {
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData Fail : name=%s, retval= %d \r\n", userid, retval);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }

    pUser = (_USER_DATA *)m_UserDataArray[uid];
    if (!pUser) {
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData point is Fail : name=%s, uid= %d \r\n", userid, uid);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }
    if (strlen(pUser->m_id) != 0) {
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData id length is null Fail : name=%s,  \r\n", userid);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }
    if (dwTime != 0) {
        char logstr[256];
        memset(logstr, 0x00, 256);
        sprintf(logstr, "[LoadUserData dwTime Error : name=%s, dwTime=%d\r\n", userid, dwTime);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        TRACE(logstr);
    }
    if (pUser->m_bLogout) { // 아직 종료되지 않은 유저...
        memset(logstr, 0x00, 256);
        sprintf(logstr, "LoadUserData logout Fail : name=%s, logout= %d \r\n", userid, pUser->m_bLogout);
        //    m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        return FALSE;
    }

    memset(logstr, 0x00, 256);
    sprintf(logstr, "LoadUserData Success : name=%s\r\n", userid);
    //m_pMain->m_LogFile.Write(logstr, strlen(logstr));

    strcpy(pUser->m_id, userid);

    pUser->m_bZone = Zone;
    pUser->m_curx = (float)(PX / 100);
    pUser->m_curz = (float)(PZ / 100);
    pUser->m_cury = (float)(PY / 100);

    pUser->m_bNation = Nation;
    pUser->m_bRace = Race;
    pUser->m_sClass = Class;
    pUser->m_bHairColor = HairColor;
    pUser->m_bRank = Rank;
    pUser->m_bTitle = Title;
    pUser->m_bLevel = Level;
    pUser->m_iExp = Exp;
    pUser->m_iLoyalty = Loyalty;
    pUser->m_bFace = Face;
    pUser->m_bCity = City;
    pUser->m_bKnights = Knights;
    // 작업 : clan정보를 받아와야 한다
    //pUser->m_sClan = clan;
    pUser->m_bFame = Fame;
    pUser->m_sHp = Hp;
    pUser->m_sMp = Mp;
    pUser->m_sSp = Sp;
    pUser->m_bStr = Str;
    pUser->m_bSta = Sta;
    pUser->m_bDex = Dex;
    pUser->m_bIntel = Intel;
    pUser->m_bCha = Cha;
    pUser->m_bAuthority = Authority;
    pUser->m_bPoints = Points;
    pUser->m_iGold = Gold;
    pUser->m_sBind = Bind;
    pUser->m_dwTime = dwTime + 1;

    CTime t = CTime::GetCurrentTime();
    memset(logstr, 0x00, 256);
    sprintf(logstr, "[LoadUserData %d-%d-%d]: name=%s, nation=%d, zone=%d, level=%d, exp=%d, money=%d\r\n", t.GetHour(),
            t.GetMinute(), t.GetSecond(), userid, Nation, Zone, Level, Exp, Gold);
    //m_pMain->m_LogFile.Write(logstr, strlen(logstr));

    int index = 0, serial_index = 0;
    for (int i = 0; i < 9; i++) {
        pUser->m_bstrSkill[i] = GetByte(strSkill, index);
    }

    index = 0;
    DWORD         itemid = 0;
    short         count = 0, duration = 0;
    __int64       serial = 0;
    _ITEM_TABLE * pTable = NULL;

    for (int i = 0; i < HAVE_MAX + SLOT_MAX; i++) // 착용갯수 + 소유갯수(14+28=42)
    {
        itemid = GetDWORD(strItem, index);
        duration = GetShort(strItem, index);
        count = GetShort(strItem, index);

        serial = GetInt64(strSerial, serial_index); // item serial number

        pTable = m_pMain->m_ItemtableArray.GetData(itemid);

        if (pTable) {
            pUser->m_sItemArray[i].nNum = itemid;
            pUser->m_sItemArray[i].sDuration = duration;
            pUser->m_sItemArray[i].nSerialNum = serial;

            if (count > ITEMCOUNT_MAX) {
                pUser->m_sItemArray[i].sCount = ITEMCOUNT_MAX;
            } else if (pTable->m_bCountable && count <= 0) {
                pUser->m_sItemArray[i].nNum = 0;
                pUser->m_sItemArray[i].sDuration = 0;
                pUser->m_sItemArray[i].sCount = 0;
                pUser->m_sItemArray[i].nSerialNum = 0;
            } else {
                if (count <= 0) {
                    pUser->m_sItemArray[i].sCount = 1;
                }
                pUser->m_sItemArray[i].sCount = count;
            }
            TRACE("%s : %d slot (%d : %I64d)\n", pUser->m_id, i, pUser->m_sItemArray[i].nNum,
                  pUser->m_sItemArray[i].nSerialNum);
            sprintf(logstr, "%s : %d slot (%d : %I64d)\n", pUser->m_id, i, pUser->m_sItemArray[i].nNum,
                    pUser->m_sItemArray[i].nSerialNum);
            //m_pMain->WriteLogFile( logstr );
            //m_pMain->m_LogFile.Write(logstr, strlen(logstr));
        } else {
            pUser->m_sItemArray[i].nNum = 0;
            pUser->m_sItemArray[i].sDuration = 0;
            pUser->m_sItemArray[i].sCount = 0;
            pUser->m_sItemArray[i].nSerialNum = 0;
            if (itemid > 0) {
                sprintf(logstr, "Item Drop : name=%s, itemid=%d\r\n", userid, itemid);
                m_pMain->WriteLogFile(logstr);
                //m_pMain->m_LogFile.Write(logstr, strlen(logstr));
            }
            continue;
        }
    }

    if (pUser->m_bLevel == 1 && pUser->m_iExp == 0 && pUser->m_iGold == 0) {
        int empty_slot = 0;
        for (int j = SLOT_MAX; j < HAVE_MAX + SLOT_MAX; j++) {
            if (pUser->m_sItemArray[j].nNum == 0) {
                empty_slot = j;
                break;
            }
        }
        if (empty_slot == HAVE_MAX + SLOT_MAX) {
            return retval;
        }

        switch (pUser->m_sClass) {
        case 101:
            pUser->m_sItemArray[empty_slot].nNum = 120010000;
            pUser->m_sItemArray[empty_slot].sDuration = 5000;
            break;
        case 102:
            pUser->m_sItemArray[empty_slot].nNum = 110010000;
            pUser->m_sItemArray[empty_slot].sDuration = 4000;
            break;
        case 103:
            pUser->m_sItemArray[empty_slot].nNum = 180010000;
            pUser->m_sItemArray[empty_slot].sDuration = 5000;
            break;
        case 104:
            pUser->m_sItemArray[empty_slot].nNum = 190010000;
            pUser->m_sItemArray[empty_slot].sDuration = 10000;
            break;
        case 201:
            pUser->m_sItemArray[empty_slot].nNum = 120050000;
            pUser->m_sItemArray[empty_slot].sDuration = 5000;
            break;
        case 202:
            pUser->m_sItemArray[empty_slot].nNum = 110050000;
            pUser->m_sItemArray[empty_slot].sDuration = 4000;
            break;
        case 203:
            pUser->m_sItemArray[empty_slot].nNum = 180050000;
            pUser->m_sItemArray[empty_slot].sDuration = 5000;
            break;
        case 204:
            pUser->m_sItemArray[empty_slot].nNum = 190050000;
            pUser->m_sItemArray[empty_slot].sDuration = 10000;
            break;
        }
        pUser->m_sItemArray[empty_slot].sCount = 1;
        pUser->m_sItemArray[empty_slot].nSerialNum = 0;
    }

    return retval;
}

int CDBAgent::UpdateUser(const char * userid, int uid, int type) {
    SQLHSTMT  hstmt;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    SDWORD    sStrItem, sStrSkill, sStrSerial;

    _USER_DATA * pUser = NULL;
    memset(szSQL, 0x00, 1024);

    DBProcessNumber(3);

    pUser = (_USER_DATA *)m_UserDataArray[uid];
    if (!pUser) {
        return -1;
    }
    if (_strnicmp(pUser->m_id, userid, MAX_ID_SIZE) != 0) {
        return -1;
    }

    if (type == UPDATE_PACKET_SAVE) {
        pUser->m_dwTime++;
    } else if (type == UPDATE_LOGOUT || type == UPDATE_ALL_SAVE) {
        pUser->m_dwTime = 0;
    }

    TCHAR strSkill[10];
    TCHAR strItem[400];
    TCHAR strSerial[400];
    memset(strSkill, 0x00, 10);
    memset(strItem, 0x00, 400);
    memset(strSerial, 0x00, 400);
    sStrSkill = sizeof(strSkill);
    sStrItem = sizeof(strItem);
    sStrSerial = sizeof(strSerial);

    int index = 0, serial_index = 0;
    for (int i = 0; i < 9; i++) {
        SetByte(strSkill, pUser->m_bstrSkill[i], index);
    }

    index = 0;
    for (int i = 0; i < HAVE_MAX + SLOT_MAX; i++) // 착용갯수 + 소유갯수(14+28=42)
    {
        if (pUser->m_sItemArray[i].nNum > 0) {
            if (m_pMain->m_ItemtableArray.GetData(pUser->m_sItemArray[i].nNum) == FALSE) {
                TRACE("Item Drop Saved(%d) : %d (%s)\n", i, pUser->m_sItemArray[i].nNum, pUser->m_id);
            }
        }
        SetDWORD(strItem, pUser->m_sItemArray[i].nNum, index);
        SetShort(strItem, pUser->m_sItemArray[i].sDuration, index);
        SetShort(strItem, pUser->m_sItemArray[i].sCount, index);

        SetInt64(strSerial, pUser->m_sItemArray[i].nSerialNum, serial_index);
    }

    // 작업 : clan정보도 업데이트
    wsprintf(szSQL,
             TEXT("{call UPDATE_USER_DATA ( \'%s\', "
                  "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,?,?,?)}"),
             pUser->m_id, pUser->m_bNation, pUser->m_bRace, pUser->m_sClass, pUser->m_bHairColor, pUser->m_bRank,
             pUser->m_bTitle, pUser->m_bLevel, pUser->m_iExp, pUser->m_iLoyalty, pUser->m_bFace, pUser->m_bCity,
             pUser->m_bKnights, pUser->m_bFame, pUser->m_sHp, pUser->m_sMp, pUser->m_sSp, pUser->m_bStr, pUser->m_bSta,
             pUser->m_bDex, pUser->m_bIntel, pUser->m_bCha, pUser->m_bAuthority, pUser->m_bPoints, pUser->m_iGold,
             pUser->m_bZone, pUser->m_sBind, (int)(pUser->m_curx * 100), (int)(pUser->m_curz * 100),
             (int)(pUser->m_cury * 100), pUser->m_dwTime);

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(strSkill), 0, strSkill, 0,
                                   &sStrSkill);
        retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(strItem), 0, strItem, 0,
                                   &sStrItem);
        retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(strSerial), 0, strSerial, 0,
                                   &sStrSerial);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return 0;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

                char logstr[1024];
                memset(logstr, 0x00, 1024);
                sprintf(logstr, "[Error-DB Fail] %s, Skill[%s] Item[%s] \r\n", szSQL, strSkill, strItem);
                m_pMain->WriteLogFile(logstr);
                //m_pMain->m_LogFile.Write(logstr, strlen(logstr));
                return 0;
            }

            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return 1;
        } else {
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        }
    }

    return 0;
}

int CDBAgent::AccountLogInReq(char * id, char * pw) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call CHAR_SELECT( \'%s\', \'%s\', ?)}"), id, pw);

    DBProcessNumber(4);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                if (sParmRet == 0) {
                    return -1;
                } else {
                    return sParmRet - 1; // sParmRet == Nation + 1....
                }
            } else {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return FALSE;
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return FALSE;
}

BOOL CDBAgent::NationSelect(char * id, int nation) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call NATION_SELECT ( ?, \'%s\', %d)}"), id, nation);

    DBProcessNumber(5);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return FALSE;
            }

            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            if (sParmRet < 0) {
                return FALSE;
            } else {
                return TRUE;
            }
        }
    }

    return FALSE;
}

int CDBAgent::CreateNewChar(char * accountid, int index, char * charid, int race, int Class, int hair, int face,
                            int str, int sta, int dex, int intel, int cha) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call CREATE_NEW_CHAR ( ?, \'%s\', %d, \'%s\', %d,%d,%d,%d,%d,%d,%d,%d,%d)}"), accountid,
             index, charid, race, Class, hair, face, str, sta, dex, intel, cha);

    DBProcessNumber(6);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return -1;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return sParmRet;
            }

            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return sParmRet;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return -1;
}

BOOL CDBAgent::DeleteChar(int index, char * id, char * charid, char * socno) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{ call DELETE_CHAR ( \'%s\', %d, \'%s\', \'%s\', ? )}"), id, index, charid, socno);

    DBProcessNumber(7);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return FALSE;
            }
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return sParmRet;
        }
    }

    return FALSE;
}

BOOL CDBAgent::LoadCharInfo(char * id, char * buff, int & buff_index) {
    SQLHSTMT  hstmt;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    BOOL      retval;
    CString   userid;

    userid = id;
    userid.TrimRight();
    strcpy(id, (char *)(LPCTSTR)userid);

    memset(szSQL, 0x00, 1024);

    wsprintf(szSQL, TEXT("{call LOAD_CHAR_INFO ('%s', ?)}"), id);

    DBProcessNumber(8);

    SQLCHAR     Race = 0x00, HairColor = 0x00, Level = 0x00, Face = 0x00, Zone = 0x00;
    SQLSMALLINT sRet, Class = 0;
    TCHAR       strItem[400];
    memset(strItem, 0x00, 400);

    SQLINTEGER Indexind = SQL_NTS;

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &Indexind);
    if (retcode != SQL_SUCCESS) {
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_TINYINT, &Race, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_SSHORT, &Class, 0, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_TINYINT, &HairColor, 0, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_TINYINT, &Level, 0, &Indexind);
            SQLGetData(hstmt, 5, SQL_C_TINYINT, &Face, 0, &Indexind);
            SQLGetData(hstmt, 6, SQL_C_TINYINT, &Zone, 0, &Indexind);
            SQLGetData(hstmt, 7, SQL_C_CHAR, strItem, 400, &Indexind);
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_GameDB.Close();
            if (!m_GameDB.IsOpen()) {
                ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                return FALSE;
            }
        }
        retval = FALSE;
    }
    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    SetShort(buff, strlen(id), buff_index);
    SetString(buff, (char *)id, strlen(id), buff_index);
    SetByte(buff, Race, buff_index);
    SetShort(buff, Class, buff_index);
    SetByte(buff, Level, buff_index);
    SetByte(buff, Face, buff_index);
    SetByte(buff, HairColor, buff_index);
    SetByte(buff, Zone, buff_index);

    int tempid = 0, count = 0, index = 0, duration = 0;
    for (int i = 0; i < SLOT_MAX; i++) {
        tempid = GetDWORD(strItem, index);
        duration = GetShort(strItem, index);
        count = GetShort(strItem, index);
        if (i == HEAD || i == BREAST || i == SHOULDER || i == LEG || i == GLOVE || i == FOOT) {
            SetDWORD(buff, tempid, buff_index);
            SetShort(buff, duration, buff_index);
        }
    }

    return retval;
}

BOOL CDBAgent::GetAllCharID(const char * id, char * char1, char * char2, char * char3, char * char4, char * char5) {
    SQLHSTMT     hstmt;
    SQLRETURN    retcode;
    TCHAR        szSQL[1024];
    BOOL         retval;
    _USER_DATA * pUser = NULL;
    CString      Item;

    memset(szSQL, 0x00, 1024);

    wsprintf(szSQL, TEXT("{? = call LOAD_ACCOUNT_CHARID ('%s')}"), id);

    DBProcessNumber(9);

    SQLSMALLINT sRet;
    TCHAR       charid1[21], charid2[21], charid3[21], charid4[21], charid5[21];
    memset(charid1, 0x00, 21);
    memset(charid2, 0x00, 21);
    memset(charid3, 0x00, 21);
    memset(charid4, 0x00, 21);
    memset(charid5, 0x00, 21);

    SQLINTEGER Indexind = SQL_NTS;

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &Indexind);
    if (retcode != SQL_SUCCESS) {
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_CHAR, charid1, 21, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_CHAR, charid2, 21, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_CHAR, charid3, 21, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_CHAR, charid4, 21, &Indexind);
            SQLGetData(hstmt, 5, SQL_C_CHAR, charid5, 21, &Indexind);
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_GameDB.Close();
            if (!m_GameDB.IsOpen()) {
                ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    if (sRet == 0) {
        retval = FALSE;
    } else {
        strcpy(char1, charid1);
        strcpy(char2, charid2);
        strcpy(char3, charid3);
        strcpy(char4, charid4);
        strcpy(char5, charid5);
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    return retval;
}

int CDBAgent::CreateKnights(int knightsindex, int nation, char * name, char * chief, int iFlag) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet = 0, sKnightIndex = 0;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call CREATE_KNIGHTS ( ?, %d, %d, %d, \'%s\', \'%s\' )}"), knightsindex, nation, iFlag, name,
             chief);
    //wsprintf( szSQL, TEXT( "{call CREATE_KNIGHTS ( ?, ?, %d, %d, \'%s\', \'%s\' )}" ), nation, iFlag, name, chief );

    DBProcessNumber(10);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        //retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0,0, &sKnightIndex,0, &cbParmRet );
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return sParmRet;
            }

            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return sParmRet;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return -1;
}

int CDBAgent::UpdateKnights(int type, char * userid, int knightsindex, int domination) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call UPDATE_KNIGHTS ( ?, %d, \'%s\', %d, %d)}"), (BYTE)type, userid, knightsindex,
             (BYTE)domination);
    //wsprintf( szSQL, TEXT( "{call UPDATE_KNIGHTS2 ( ?, %d, \'%s\', %d, %d)}" ), type, userid, knightsindex, domination );

    DBProcessNumber(11);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return sParmRet;
            }

            TRACE("DB - UpdateKnights - command=%d, name=%s, index=%d, result=%d \n", type, userid, knightsindex,
                  domination);
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return sParmRet;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return -1;
}

int CDBAgent::DeleteKnights(int knightsindex) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call DELETE_KNIGHTS ( ?, %d )}"), knightsindex);
    //wsprintf( szSQL, TEXT( "{call DELETE_KNIGHTS2 ( ?, %d )}" ), knightsindex );

    DBProcessNumber(12);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return -1;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return sParmRet;
            }

            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return sParmRet;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return -1;
}

int CDBAgent::LoadKnightsAllMembers(int knightsindex, int start, char * temp_buff, int & buff_index) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE;
    int       count = 0, temp_index = 0, sid = 0;
    CString   tempid;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLCHAR      userid[MAX_ID_SIZE + 1];
    SQLCHAR      Fame, Level;
    SQLSMALLINT  Class;
    SQLINTEGER   Indexind = SQL_NTS;
    _USER_DATA * pUser = NULL;

    //wsprintf( szSQL, TEXT( "SELECT strUserId, Fame, [Level], Class FROM USERDATA WHERE Knights = %d" ), knightsindex );
    wsprintf(szSQL, TEXT("{call LOAD_KNIGHTS_MEMBERS ( %d )}"), knightsindex);

    DBProcessNumber(13);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            while (bData) {
                retcode = SQLFetch(hstmt);
                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                    SQLGetData(hstmt, 1, SQL_C_CHAR, userid, MAX_ID_SIZE + 1, &Indexind);
                    SQLGetData(hstmt, 2, SQL_C_TINYINT, &Fame, 0, &Indexind);
                    SQLGetData(hstmt, 3, SQL_C_TINYINT, &Level, 0, &Indexind);
                    SQLGetData(hstmt, 4, SQL_C_SSHORT, &Class, 0, &Indexind);

                    //    if( count < start ) {
                    //        count++;
                    //        continue;
                    //    }
                    strcpy((char *)(LPCTSTR)tempid, (char *)userid);
                    tempid.TrimRight();

                    SetShort(temp_buff, strlen((char *)(LPCTSTR)tempid), temp_index);
                    SetString(temp_buff, (char *)(LPCTSTR)tempid, strlen((char *)(LPCTSTR)tempid), temp_index);
                    SetByte(temp_buff, Fame, temp_index);
                    SetByte(temp_buff, Level, temp_index);
                    SetShort(temp_buff, Class, temp_index);

                    sid = -1;
                    //(_USER_DATA*)m_UserDataArray[uid];
                    pUser = m_pMain->GetUserPtr((const char *)(LPCTSTR)tempid, sid);
                    //pUser = (_USER_DATA*)m_UserDataArray[sid];
                    if (pUser) {
                        SetByte(temp_buff, 1, temp_index);
                    } else {
                        SetByte(temp_buff, 0, temp_index);
                    }

                    count++;
                    //if( count >= start + 10 )
                    //    break;

                    bData = TRUE;
                } else {
                    bData = FALSE;
                }
            }
        } else {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_GameDB.Close();
                if (!m_GameDB.IsOpen()) {
                    ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                    return 0;
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    buff_index = temp_index;
    return (count - start);
}

BOOL CDBAgent::UpdateConCurrentUserCount(int serverno, int zoneno, int t_count) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    switch (zoneno) {
    case 1:
        wsprintf(szSQL, TEXT("UPDATE CONCURRENT SET zone1_count = %d WHERE serverid = %d"), t_count, serverno);
        break;
    case 2:
        wsprintf(szSQL, TEXT("UPDATE CONCURRENT SET zone2_count = %d WHERE serverid = %d"), t_count, serverno);
        break;
    case 3:
        wsprintf(szSQL, TEXT("UPDATE CONCURRENT SET zone3_count = %d WHERE serverid = %d"), t_count, serverno);
        break;
    }

    DBProcessNumber(14);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_AccountDB1.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_ERROR) {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_AccountDB1.Close();
                if (!m_AccountDB1.IsOpen()) {
                    ReConnectODBC(&m_AccountDB1, m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                                  m_pMain->m_strAccountPWD);
                    return FALSE;
                }
            }
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return FALSE;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return TRUE;
}

BOOL CDBAgent::LoadWarehouseData(const char * accountid, int uid) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      retval;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    _USER_DATA *  pUser = NULL;
    _ITEM_TABLE * pTable = NULL;
    SQLINTEGER    Money = 0, dwTime = 0;
    TCHAR         strItem[1600], strSerial[1600];
    memset(strItem, 0x00, 1600);
    memset(strSerial, 0x00, 1600);
    SQLINTEGER Indexind = SQL_NTS;

    wsprintf(szSQL, TEXT("SELECT nMoney, dwTime, WarehouseData, strSerial FROM WAREHOUSE WHERE strAccountID = \'%s\'"),
             accountid);

    DBProcessNumber(15);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLFetch(hstmt);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            SQLGetData(hstmt, 1, SQL_C_LONG, &Money, 0, &Indexind);
            SQLGetData(hstmt, 2, SQL_C_LONG, &dwTime, 0, &Indexind);
            SQLGetData(hstmt, 3, SQL_C_CHAR, strItem, 1600, &Indexind);
            SQLGetData(hstmt, 4, SQL_C_CHAR, strSerial, 1600, &Indexind);
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_GameDB.Close();
            if (!m_GameDB.IsOpen()) {
                ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    if (!retval) {
        return FALSE;
    }

    pUser = (_USER_DATA *)m_UserDataArray[uid];
    if (!pUser) {
        return FALSE;
    }
    if (strlen(pUser->m_id) == 0) {
        return FALSE;
    }

    pUser->m_iBank = Money;

    int     index = 0, serial_index = 0;
    DWORD   itemid = 0;
    short   count = 0, duration = 0;
    __int64 serial = 0;
    for (int i = 0; i < WAREHOUSE_MAX; i++) {
        itemid = GetDWORD(strItem, index);
        duration = GetShort(strItem, index);
        count = GetShort(strItem, index);

        serial = GetInt64(strSerial, serial_index);

        pTable = m_pMain->m_ItemtableArray.GetData(itemid);
        if (pTable) {
            pUser->m_sWarehouseArray[i].nNum = itemid;
            pUser->m_sWarehouseArray[i].sDuration = duration;
            if (count > ITEMCOUNT_MAX) {
                pUser->m_sWarehouseArray[i].sCount = ITEMCOUNT_MAX;
            } else if (count <= 0) {
                count = 1;
            }
            pUser->m_sWarehouseArray[i].sCount = count;
            pUser->m_sWarehouseArray[i].nSerialNum = serial;
            TRACE("%s : %d ware slot (%d : %I64d)\n", pUser->m_id, i, pUser->m_sWarehouseArray[i].nNum,
                  pUser->m_sWarehouseArray[i].nSerialNum);
        } else {
            pUser->m_sWarehouseArray[i].nNum = 0;
            pUser->m_sWarehouseArray[i].sDuration = 0;
            pUser->m_sWarehouseArray[i].sCount = 0;
            if (itemid > 0) {
                char logstr[256];
                memset(logstr, 0x00, 256);
                sprintf(logstr, "Warehouse Item Drop(%d) : %d (%s)\r\n", i, itemid, accountid);
                //m_pMain->WriteLogFile( logstr );
                //m_pMain->m_LogFile.Write(logstr, strlen(logstr));
            }
        }
    }

    return retval;
}

int CDBAgent::UpdateWarehouseData(const char * accountid, int uid, int type) {
    SQLHSTMT  hstmt;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    SDWORD    sStrItem, sStrSerial;

    _USER_DATA * pUser = NULL;
    memset(szSQL, 0x00, 1024);

    pUser = (_USER_DATA *)m_UserDataArray[uid];
    if (!pUser) {
        return -1;
    }
    if (strlen(accountid) == 0) {
        return -1;
    }
    if (_strnicmp(pUser->m_Accountid, accountid, MAX_ID_SIZE) != 0) {
        return -1;
    }

    else if (type == UPDATE_LOGOUT || type == UPDATE_ALL_SAVE) {
        pUser->m_dwTime = 0;
    }

    TCHAR strItem[1600], strSerial[1600];
    memset(strItem, 0x00, 1600);
    memset(strSerial, 0x00, 1600);
    sStrItem = sizeof(strItem);
    sStrSerial = sizeof(strSerial);

    int index = 0, serial_index = 0;
    for (int i = 0; i < WAREHOUSE_MAX; i++) {
        SetDWORD(strItem, pUser->m_sWarehouseArray[i].nNum, index);
        SetShort(strItem, pUser->m_sWarehouseArray[i].sDuration, index);
        SetShort(strItem, pUser->m_sWarehouseArray[i].sCount, index);

        SetInt64(strSerial, pUser->m_sWarehouseArray[i].nSerialNum, serial_index);
    }

    wsprintf(szSQL, TEXT("{call UPDATE_WAREHOUSE ( \'%s\', %d,%d,?,?)}"), accountid, pUser->m_iBank, pUser->m_dwTime);

    DBProcessNumber(16);

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(strItem), 0, strItem, 0,
                                   &sStrItem);
        retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(strSerial), 0, strSerial, 0,
                                   &sStrSerial);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_ERROR) {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_GameDB.Close();
                    if (!m_GameDB.IsOpen()) {
                        ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                        return 0;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

                char logstr[2048];
                memset(logstr, 0x00, 2048);
                sprintf(logstr, "%s, Item[%s] \r\n", szSQL, strItem);
                m_pMain->WriteLogFile(logstr);
                //m_pMain->m_LogFile.Write(logstr, strlen(logstr));
                return FALSE;
            }
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return 1;
        } else {
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        }
    }

    return 0;
}

BOOL CDBAgent::LoadKnightsInfo(int index, char * buff, int & buff_index) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE, retval = FALSE;
    CString   tempid;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLCHAR IDName[MAX_ID_SIZE + 1], Nation;
    char    szKnightsName[MAX_ID_SIZE + 1];
    memset(IDName, 0x00, MAX_ID_SIZE + 1);
    memset(szKnightsName, 0x00, MAX_ID_SIZE + 1);
    SQLSMALLINT IDNum, Members;
    SQLINTEGER  Indexind = SQL_NTS, nPoints = 0;

    int len = 0;

    wsprintf(szSQL, TEXT("SELECT IDNum, Nation, IDName, Members, Points FROM KNIGHTS WHERE IDNum=%d"), index);

    DBProcessNumber(17);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLFetch(hstmt);
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                SQLGetData(hstmt, 1, SQL_C_SSHORT, &IDNum, 0, &Indexind);
                SQLGetData(hstmt, 2, SQL_C_TINYINT, &Nation, 0, &Indexind);
                SQLGetData(hstmt, 3, SQL_C_CHAR, IDName, MAX_ID_SIZE + 1, &Indexind);
                SQLGetData(hstmt, 4, SQL_C_SSHORT, &Members, 0, &Indexind);
                SQLGetData(hstmt, 5, SQL_C_LONG, &nPoints, 0, &Indexind);

                tempid = IDName;
                tempid.TrimRight();

                strcpy(szKnightsName, (char *)(LPCTSTR)tempid);

                SetShort(buff, IDNum, buff_index);
                SetByte(buff, Nation, buff_index);
                SetShort(buff, strlen(szKnightsName), buff_index);
                SetString(buff, szKnightsName, strlen(szKnightsName), buff_index);
                SetShort(buff, Members, buff_index);
                SetDWORD(buff, nPoints, buff_index);

                retval = TRUE;
            }

        } else {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_GameDB.Close();
                if (!m_GameDB.IsOpen()) {
                    ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                    return FALSE;
                }
            }
            retval = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    } else {
        return FALSE;
    }

    return retval;
}

BOOL CDBAgent::SetLogInInfo(const char * accountid, const char * charid, const char * serverip, int serverno,
                            const char * clientip, BYTE bInit) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLINTEGER cbParmRet = SQL_NTS;
    BOOL       bSuccess = TRUE;

    if (bInit == 0x01) {
        wsprintf(szSQL,
                 TEXT("INSERT INTO CURRENTUSER (strAccountID, strCharID, nServerNo, strServerIP, strClientIP) VALUES "
                      "(\'%s\',\'%s\',%d,\'%s\',\'%s\')"),
                 accountid, charid, serverno, serverip, clientip);
    } else if (bInit == 0x02) {
        wsprintf(szSQL, TEXT("UPDATE CURRENTUSER SET nServerNo=%d, strServerIP=\'%s\' WHERE strAccountID = \'%s\'"),
                 serverno, serverip, accountid);
    } else {
        return FALSE;
    }

    DBProcessNumber(18);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_AccountDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_ERROR) {
            bSuccess = FALSE;
            if (DisplayErrorMsg(hstmt) == -1) {
                m_AccountDB.Close();
                if (!m_AccountDB.IsOpen()) {
                    ReConnectODBC(&m_AccountDB, m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                                  m_pMain->m_strAccountPWD);
                    return FALSE;
                }
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return bSuccess;
}

int CDBAgent::AccountLogout(const char * accountid) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);
    SQLSMALLINT sParmRet = 0;
    SQLINTEGER  cbParmRet = SQL_NTS;

    wsprintf(szSQL, TEXT("{call ACCOUNT_LOGOUT( \'%s\', ?)}"), accountid);

    DBProcessNumber(19);

    CTime t = CTime::GetCurrentTime();
    char  strlog[256];
    memset(strlog, 0x00, 256);
    sprintf(strlog, "[AccountLogout] acname=%s \r\n", accountid);
    m_pMain->WriteLogFile(strlog);
    //m_pMain->m_LogFile.Write(strlog, strlen(strlog));
    TRACE(strlog);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_AccountDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode =
            SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sParmRet, 0, &cbParmRet);
        if (retcode == SQL_SUCCESS) {
            retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                if (sParmRet == 0) {
                    return -1;
                } else {
                    return sParmRet; // 1 == success
                }
            } else {
                if (DisplayErrorMsg(hstmt) == -1) {
                    m_AccountDB.Close();
                    if (!m_AccountDB.IsOpen()) {
                        ReConnectODBC(&m_AccountDB, m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                                      m_pMain->m_strAccountPWD);
                        return FALSE;
                    }
                }
                SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
                return -1;
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return -1;
}

BOOL CDBAgent::CheckUserData(const char * accountid, const char * charid, int type, int nTimeNumber, int comparedata) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE, retval = FALSE;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLINTEGER Indexind = SQL_NTS, dwTime = 0, iData = 0;

    if (type == 1) {
        wsprintf(szSQL, TEXT("SELECT dwTime, nMoney FROM WAREHOUSE WHERE strAccountID=\'%s\'"), accountid);
    } else {
        wsprintf(szSQL, TEXT("SELECT dwTime, [Exp] FROM USERDATA WHERE strUserID=\'%s\'"), charid);
    }

    DBProcessNumber(20);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLFetch(hstmt);
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                SQLGetData(hstmt, 1, SQL_C_LONG, &dwTime, 0, &Indexind);
                SQLGetData(hstmt, 2, SQL_C_LONG, &iData, 0, &Indexind); // type:1 -> Bank Money type:2 -> Exp

                if (nTimeNumber != dwTime || comparedata != iData) { // check userdata have saved
                    retval = FALSE;
                } else {
                    retval = TRUE;
                }
            }

        } else {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_GameDB.Close();
                if (!m_GameDB.IsOpen()) {
                    ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                    return FALSE;
                }
            }
            retval = FALSE;
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    } else {
        return FALSE;
    }

    return retval;
}

void CDBAgent::LoadKnightsAllList(int nation) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE;
    int       count = 0;
    CString   tempid;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    char send_buff[512];
    memset(send_buff, 0x00, 512);
    char temp_buff[512];
    memset(temp_buff, 0x00, 512);
    int send_index = 0, temp_index = 0;

    SQLCHAR     bRanking = 0;
    SQLSMALLINT shKnights = 0;
    SQLINTEGER  Indexind = SQL_NTS, nPoints = 0;

    if (nation == 3) { // battle zone
        wsprintf(szSQL, TEXT("SELECT IDNum, Points, Ranking FROM KNIGHTS WHERE Points <> 0 ORDER BY Points DESC"),
                 nation);
    } else {
        wsprintf(
            szSQL,
            TEXT("SELECT IDNum, Points, Ranking FROM KNIGHTS WHERE Nation=%d AND Points <> 0 ORDER BY Points DESC"),
            nation);
    }

    DBProcessNumber(21);

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            while (bData) {
                retcode = SQLFetch(hstmt);
                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                    SQLGetData(hstmt, 1, SQL_C_SSHORT, &shKnights, 0, &Indexind);
                    SQLGetData(hstmt, 2, SQL_C_LONG, &nPoints, 0, &Indexind);
                    SQLGetData(hstmt, 3, SQL_C_TINYINT, &bRanking, 0, &Indexind);

                    SetShort(temp_buff, shKnights, temp_index);
                    SetDWORD(temp_buff, nPoints, temp_index);
                    SetByte(temp_buff, bRanking, temp_index);

                    count++;

                    if (count >= 40) { // 40개 단위로 보낸다
                        SetByte(send_buff, KNIGHTS_ALLLIST_REQ, send_index);
                        SetShort(send_buff, -1, send_index);
                        SetByte(send_buff, count, send_index);
                        SetString(send_buff, temp_buff, temp_index, send_index);

                        do {
                            if (m_pMain->m_LoggerSendQueue.PutData(send_buff, send_index) == 1) {
                                break;
                            } else {
                                count++;
                            }
                        } while (count < 50);
                        if (count >= 50) {
                            m_pMain->m_OutputList.AddString("LoadKnightsAllList Packet Drop!!!");
                        }

                        memset(send_buff, 0x00, 512);
                        memset(temp_buff, 0x00, 512);
                        send_index = temp_index = 0;
                        count = 0;
                    }
                    bData = TRUE;
                } else {
                    bData = FALSE;
                }
            }
        } else {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_GameDB.Close();
                if (!m_GameDB.IsOpen()) {
                    ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                    return;
                }
            }
        }

        if (count < 40) { // 40개를 보내지 못한 경우
            SetByte(send_buff, KNIGHTS_ALLLIST_REQ, send_index);
            SetShort(send_buff, -1, send_index);
            SetByte(send_buff, count, send_index);
            SetString(send_buff, temp_buff, temp_index, send_index);

            do {
                if (m_pMain->m_LoggerSendQueue.PutData(send_buff, send_index) == 1) {
                    break;
                } else {
                    count++;
                }
            } while (count < 50);
            if (count >= 50) {
                m_pMain->m_OutputList.AddString("LoadKnightsAllList Packet Drop!!!");
            }
        }

        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }
}

void CDBAgent::DBProcessNumber(int number) {
    CString strDBNum;

    strDBNum.Format(" %4d ", number);

    m_pMain->GetDlgItem(IDC_DB_PROCESS)->SetWindowText(strDBNum);
    m_pMain->GetDlgItem(IDC_DB_PROCESS)->UpdateWindow();
}

BOOL CDBAgent::UpdateBattleEvent(const char * charid, int nation) {
    SQLHSTMT  hstmt;
    SQLRETURN retcode;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    DBProcessNumber(22);

    wsprintf(szSQL, TEXT("UPDATE BATTLE SET byNation=%d, strUserName=\'%s\' WHERE sIndex=%d"), nation, charid, 1);

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_GameDB.m_hdbc, &hstmt);
    if (retcode == SQL_SUCCESS) {
        retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
        if (retcode == SQL_ERROR) {
            if (DisplayErrorMsg(hstmt) == -1) {
                m_GameDB.Close();
                if (!m_GameDB.IsOpen()) {
                    ReConnectODBC(&m_GameDB, m_pMain->m_strGameDSN, m_pMain->m_strGameUID, m_pMain->m_strGamePWD);
                    return 0;
                }
            }
            SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
            return FALSE;
        }
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
    }

    return TRUE;
}

BOOL CDBAgent::CheckCouponEvent(const char * accountid) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE, retval = FALSE;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLINTEGER  Indexind = SQL_NTS;
    SQLSMALLINT sRet = 0;

    wsprintf(szSQL, TEXT("{call CHECK_COUPON_EVENT (\'%s\', ?)}"), accountid);

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_AccountDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &Indexind);
    if (retcode != SQL_SUCCESS) {
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
        return FALSE;
    }

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        //SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT,hstmt);
        if (sRet == 0) {
            retval = TRUE;
        } else {
            retval = FALSE;
        }
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_AccountDB.Close();
            if (!m_AccountDB.IsOpen()) {
                ReConnectODBC(&m_AccountDB, m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                              m_pMain->m_strAccountPWD);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    return retval;
}

BOOL CDBAgent::UpdateCouponEvent(const char * accountid, char * charid, char * cpid, int itemid, int count) {
    SQLHSTMT  hstmt = NULL;
    SQLRETURN retcode;
    BOOL      bData = TRUE, retval = FALSE;
    TCHAR     szSQL[1024];
    memset(szSQL, 0x00, 1024);

    SQLINTEGER  Indexind = SQL_NTS;
    SQLSMALLINT sRet = 0;

    wsprintf(szSQL, TEXT("{call UPDATE_COUPON_EVENT (\'%s\', \'%s\', \'%s\', %d, %d)}"), accountid, charid, cpid,
             itemid, count);

    hstmt = NULL;

    retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, m_AccountDB.m_hdbc, &hstmt);
    if (retcode != SQL_SUCCESS) {
        return FALSE;
    }

    /*    retcode = SQLBindParameter(hstmt,1,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,0,0, &sRet,0,&Indexind);
    if (retcode != SQL_SUCCESS){
        SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT,hstmt);
        return FALSE;
    }    */

    retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, 1024);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        //SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT,hstmt);
        retval = TRUE;
        //if( sRet == 1 )    retval = TRUE;
        //else retval = FALSE;
    } else {
        if (DisplayErrorMsg(hstmt) == -1) {
            m_AccountDB.Close();
            if (!m_AccountDB.IsOpen()) {
                ReConnectODBC(&m_AccountDB, m_pMain->m_strAccountDSN, m_pMain->m_strAccountUID,
                              m_pMain->m_strAccountPWD);
                return FALSE;
            }
        }
        retval = FALSE;
    }

    SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

    return retval;
}