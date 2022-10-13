// EbenezerDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Ebenezer.h"
#include "EbenezerDlg.h"
#include "User.h"
#include "AiPacket.h"
#include "PacketDefine.h"

#include "ItemTableSet.h"
#include "MagicTableSet.h"
#include "MagicType1Set.h"
#include "MagicType2Set.h"
#include "MagicType3Set.h"
#include "MagicType4Set.h"
#include "MagicType5Set.h"
#include "MagicType8Set.h"
#include "ZoneInfoSet.h"
#include "CoefficientSet.h"
#include "LevelUpTableSet.h"
#include "KnightsSet.h"
#include "KnightsUserSet.h"
#include "KnightsRankSet.h"
#include "HomeSet.h"
#include "BattleSet.h"

#define GAME_TIME       100
#define SEND_TIME       200
#define PACKET_CHECK    300
#define ALIVE_TIME      400
#define MARKET_BBS_TIME 1000

#define NUM_FLAG_VICTORY 4
#define AWARD_GOLD       5000

// Cryption
T_KEY g_private_key = 0x1234567890123456;
///~

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRITICAL_SECTION g_serial_critical;
CRITICAL_SECTION g_region_critical;
CRITICAL_SECTION g_LogFile_critical;
CIOCPort         CEbenezerDlg::m_Iocport;

WORD g_increase_serial = 1;
BYTE g_serverdown_flag = FALSE;

DWORD WINAPI ReadQueueThread(LPVOID lp);

DWORD WINAPI ReadQueueThread(LPVOID lp) {
    CEbenezerDlg * pMain = (CEbenezerDlg *)lp;
    int            recvlen = 0, index = 0, uid = -1, send_index = 0, buff_length = 0;
    BYTE           command, result;
    char           pBuf[1024], send_buff[1024];
    memset(pBuf, NULL, 1024);
    memset(send_buff, NULL, 1024);
    CUser * pUser = NULL;
    int     currenttime = 0;

    while (TRUE) {
        if (pMain->m_LoggerRecvQueue.GetFrontMode() != R) {
            index = 0;
            recvlen = pMain->m_LoggerRecvQueue.GetData(pBuf);

            if (recvlen > MAX_PKTSIZE || recvlen == 0) {
                Sleep(1);
                continue;
            }

            command = GetByte(pBuf, index);
            uid = GetShort(pBuf, index);
            if (command == KNIGHTS_ALLLIST_REQ + 0x10 && uid == -1) {
                pMain->m_KnightsManager.RecvKnightsAllList(pBuf + index);
                continue;
            }
            if (uid < 0 || uid >= MAX_USER) {
                goto loop_pass;
            }
            if (!pMain->m_Iocport.m_SockArray[uid]) {
                goto loop_pass;
            }

            pUser = (CUser *)pMain->m_Iocport.m_SockArray[uid];

            switch (command) {
            case WIZ_LOGIN:
                result = GetByte(pBuf, index);
                if (result == 0xFF) {
                    memset(pUser->m_strAccountID, NULL, MAX_ID_SIZE + 1);
                }
                SetByte(send_buff, WIZ_LOGIN, send_index);
                SetByte(send_buff, result, send_index); // 성공시 국가 정보
                pUser->Send(send_buff, send_index);
                break;
            case WIZ_SEL_NATION:
                SetByte(send_buff, WIZ_SEL_NATION, send_index);
                SetByte(send_buff, GetByte(pBuf, index), send_index); // 국가 정보
                pUser->Send(send_buff, send_index);
                break;
            case WIZ_NEW_CHAR:
                result = GetByte(pBuf, index);
                SetByte(send_buff, WIZ_NEW_CHAR, send_index);
                SetByte(send_buff, result, send_index); // 성공시 국가 정보
                pUser->Send(send_buff, send_index);
                break;
            case WIZ_DEL_CHAR:
                pUser->RecvDeleteChar(pBuf + index);
                /*    result = GetByte( pBuf, index );
                    SetByte( send_buff, WIZ_DEL_CHAR, send_index );
                    SetByte( send_buff, result, send_index );                    // 성공시 국가 정보
                    SetByte( send_buff, GetByte( pBuf, index ), send_index );
                    pUser->Send( send_buff, send_index );    */
                break;
            case WIZ_SEL_CHAR:
                pUser->SelectCharacter(pBuf + index);
                break;
            case WIZ_ALLCHAR_INFO_REQ:
                buff_length = GetShort(pBuf, index);
                if (buff_length > recvlen) {
                    break;
                }
                SetByte(send_buff, WIZ_ALLCHAR_INFO_REQ, send_index);
                SetString(send_buff, pBuf + index, buff_length, send_index);
                pUser->Send(send_buff, send_index);
                break;
            case WIZ_LOGOUT:
                if (pUser) {
                    if (strlen(pUser->m_pUserData->m_id) != 0) {
                        TRACE("Logout Strange...%s\n", pUser->m_pUserData->m_id);
                        pUser->Close();
                    }
                }
                break;
            case KNIGHTS_CREATE + 0x10:
            case KNIGHTS_JOIN + 0x10:
            case KNIGHTS_WITHDRAW + 0x10:
            case KNIGHTS_REMOVE + 0x10:
            case KNIGHTS_ADMIT + 0x10:
            case KNIGHTS_REJECT + 0x10:
            case KNIGHTS_CHIEF + 0x10:
            case KNIGHTS_VICECHIEF + 0x10:
            case KNIGHTS_OFFICER + 0x10:
            case KNIGHTS_PUNISH + 0x10:
            case KNIGHTS_DESTROY + 0x10:
            case KNIGHTS_MEMBER_REQ + 0x10:
            case KNIGHTS_STASH + 0x10:
            case KNIGHTS_LIST_REQ + 0x10:
            case KNIGHTS_ALLLIST_REQ + 0x10:
                pMain->m_KnightsManager.ReceiveKnightsProcess(pUser, pBuf + index, command);
                break;
            case WIZ_LOGIN_INFO:
                result = GetByte(pBuf, index);
                if (result == 0x00) {
                    pUser->Close();
                }
                break;
            case DB_COUPON_EVENT:
                if (pUser) {
                    pUser->CouponEvent(pBuf + index);
                }
                break;
            }
        } else {
            continue;
        }

    loop_pass:
        recvlen = 0;
        memset(pBuf, NULL, 1024);
        send_index = 0;
        memset(send_buff, NULL, 1024);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

CEbenezerDlg::CEbenezerDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CEbenezerDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CEbenezerDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bMMFCreate = FALSE;
    m_hReadQueueThread = NULL;

    m_nYear = 0;
    m_nMonth = 0;
    m_nDate = 0;
    m_nHour = 0;
    m_nMin = 0;
    m_nWeather = 0;
    m_nAmount = 0;
    m_sPartyIndex = 0;

    m_nCastleCapture = 0;

    m_bKarusFlag = 0;
    m_bElmoradFlag = 0;

    m_byKarusOpenFlag = m_byElmoradOpenFlag = 0;
    m_byBanishFlag = 0;
    m_sBanishDelay = 0;

    m_sKarusDead = 0;
    m_sElmoradDead = 0;

    m_bVictory = 0;
    m_byOldVictory = 0;
    m_bBanishDelayStart = 0;
    m_byBattleSave = 0;
    m_sKarusCount = 0;
    m_sElmoradCount = 0;

    m_nBattleZoneOpenWeek = m_nBattleZoneOpenHourStart = m_nBattleZoneOpenHourEnd = 0;

    m_byBattleOpen = NO_BATTLE;
    m_byOldBattleOpen = NO_BATTLE;
    m_bFirstServerFlag = FALSE;
    m_bPointCheckFlag = FALSE;

    m_nServerNo = 0;
    m_nServerGroupNo = 0;
    m_nServerGroup = 0;
    m_iPacketCount = 0;
    m_iSendPacketCount = 0;
    m_iRecvPacketCount = 0;
    m_sDiscount = 0;

    m_pUdpSocket = NULL;

    for (int h = 0; h < MAX_BBS_POST; h++) {
        m_sBuyID[h] = -1;
        memset(m_strBuyTitle[h], NULL, MAX_BBS_TITLE);
        memset(m_strBuyMessage[h], NULL, MAX_BBS_MESSAGE);
        m_iBuyPrice[h] = 0;
        m_fBuyStartTime[h] = 0.0f;

        m_sSellID[h] = -1;
        memset(m_strSellTitle[h], NULL, MAX_BBS_TITLE);
        memset(m_strSellMessage[h], NULL, MAX_BBS_MESSAGE);
        m_iSellPrice[h] = 0;
        m_fSellStartTime[h] = 0.0f;
    }

    for (int i = 0; i < 20; i++) {
        memset(m_ppNotice[i], NULL, 128);
    }
    memset(m_AIServerIP, NULL, 20);

    m_bPermanentChatMode = FALSE; // 비러머글 남는 공지 --;
    m_bPermanentChatFlag = FALSE;
    memset(m_strPermanentChat, NULL, 1024);
    memset(m_strKarusCaptain, 0x00, MAX_ID_SIZE + 1);
    memset(m_strElmoradCaptain, 0x00, MAX_ID_SIZE + 1);

    m_bSanta = FALSE; // 갓댐 산타!!! >.<
}

void CEbenezerDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEbenezerDlg)
    DDX_Control(pDX, IDC_GONGJI_EDIT, m_AnnounceEdit);
    DDX_Control(pDX, IDC_LIST1, m_StatusList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEbenezerDlg, CDialog)
//{{AFX_MSG_MAP(CEbenezerDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg message handlers

BOOL CEbenezerDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // Compress Init
    memset(m_CompBuf, NULL, 10240); // 압축할 데이터를 모으는 버퍼
    m_iCompIndex = 0;               // 압축할 데이터의 길이
    m_CompCount = 0;                // 압축할 데이터의 개수

    m_sZoneCount = 0;
    m_sSocketCount = 0;
    m_sErrorSocketCount = 0;
    m_KnightsManager.m_pMain = this;
    // sungyong 2002.05.23
    m_sSendSocket = 0;
    m_bFirstServerFlag = FALSE;
    m_bServerCheckFlag = FALSE;
    m_sReSocketCount = 0;
    m_fReConnectStart = 0.0f;
    // sungyong~ 2002.05.23

    if (AfxMessageBox("If you are restarting, please restart after all data is saved...do you want to continue?",
                      MB_OKCANCEL) == IDCANCEL) {
        AfxPostQuitMessage(0);
        return FALSE;
    }
    //----------------------------------------------------------------------
    //    Logfile initialize
    //----------------------------------------------------------------------
    CTime cur = CTime::GetCurrentTime();
    char  strLogFile[50];
    memset(strLogFile, 0x00, 50);
    wsprintf(strLogFile, "RegionLog-%d-%d-%d.txt", cur.GetYear(), cur.GetMonth(), cur.GetDay());
    m_RegionLogFile.Open(strLogFile,
                         CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone);
    m_RegionLogFile.SeekToEnd();

    wsprintf(strLogFile, "PacketLog-%d-%d-%d.txt", cur.GetYear(), cur.GetMonth(), cur.GetDay());
    m_LogFile.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone);
    m_LogFile.SeekToEnd();

    InitializeCriticalSection(&g_LogFile_critical);
    InitializeCriticalSection(&g_serial_critical);

    GetTimeFromIni();

    m_Iocport.Init(MAX_USER, CLIENT_SOCKSIZE, 4);

    for (int i = 0; i < MAX_USER; i++) {
        m_Iocport.m_SockArrayInActive[i] = new CUser;
    }

    _ZONE_SERVERINFO * pInfo = NULL;
    pInfo = m_ServerArray.GetData(m_nServerNo);
    if (!pInfo) {
        AfxMessageBox("No Listen Port!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    if (!m_Iocport.Listen(pInfo->sPort)) {
        AfxMessageBox("FAIL TO CREATE LISTEN STATE", MB_OK);
    }

    if (!InitializeMMF()) {
        AfxMessageBox("Main Shared Memory Initialize Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    if (!m_LoggerSendQueue.InitailizeMMF(MAX_PKTSIZE, MAX_COUNT, SMQ_LOGGERSEND)) {
        AfxMessageBox("SMQ Send Shared Memory Initialize Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    if (!m_LoggerRecvQueue.InitailizeMMF(MAX_PKTSIZE, MAX_COUNT, SMQ_LOGGERRECV)) {
        AfxMessageBox("SMQ Recv Shared Memory Initialize Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    if (!m_ItemLoggerSendQ.InitailizeMMF(MAX_PKTSIZE, MAX_COUNT, SMQ_ITEMLOGGER)) {
        AfxMessageBox("SMQ ItemLog Shared Memory Initialize Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before item\r\n");
    if (LoadItemTable() == FALSE) {
        AfxMessageBox("ItemTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before main\r\n");
    if (LoadMagicTable() == FALSE) {
        AfxMessageBox("MagicTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 1\r\n");
    if (LoadMagicType1() == FALSE) {
        AfxMessageBox("MagicType1 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 2\r\n");
    if (LoadMagicType2() == FALSE) {
        AfxMessageBox("MagicType2 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 3\r\n");
    if (LoadMagicType3() == FALSE) {
        AfxMessageBox("MagicType3 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 4\r\n");
    if (LoadMagicType4() == FALSE) {
        AfxMessageBox("MagicType4 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 5\r\n");
    if (LoadMagicType5() == FALSE) {
        AfxMessageBox("MagicType5 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before 8\r\n");
    if (LoadMagicType8() == FALSE) {
        AfxMessageBox("MagicType8 Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before Coeffi\r\n");
    if (LoadCoefficientTable() == FALSE) {
        AfxMessageBox("CharaterDataTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    LogFileWrite("before Level\r\n");
    if (LoadLevelUpTable() == FALSE) {
        AfxMessageBox("LevelUpTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before All Kinghts\r\n");
    if (LoadAllKnights() == FALSE) {
        AfxMessageBox("KnightsData Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before All Knights User\r\n");
    if (LoadAllKnightsUserData() == FALSE) {
        AfxMessageBox("LoadAllKnightsUserData Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before home\r\n");
    if (LoadHomeTable() == FALSE) {
        AfxMessageBox("LoadHomeTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before battle\r\n");
    if (LoadBattleTable() == FALSE) {
        AfxMessageBox("LoadBattleTable Load Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    LogFileWrite("before map file\r\n");
    if (!MapFileLoad()) {
        AfxPostQuitMessage(0);
    }

    LogFileWrite("after map file\r\n");

    LoadNoticeData();

    srand(time(NULL));

    DWORD id;
    m_hReadQueueThread = ::CreateThread(NULL, 0, ReadQueueThread, (LPVOID)this, 0, &id);

    m_pUdpSocket = new CUdpSocket(this);
    if (m_pUdpSocket->CreateSocket() == false) {
        AfxMessageBox("Udp Socket Create Fail");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    if (!AIServerConnect()) {
#ifndef _DEBUG
        AfxPostQuitMessage(0);
#endif
    }

    LogFileWrite("success\r\n");
    UserAcceptThread();

    //CTime cur = CTime::GetCurrentTime();
    CString starttime;
    starttime.Format("Game Server Start : %d월 %d일 %d시 %d분\r\n", cur.GetMonth(), cur.GetDay(), cur.GetHour(),
                     cur.GetMinute());
    LogFileWrite((char *)(LPCTSTR)starttime);
    m_StatusList.AddString(starttime);

    return TRUE; // return TRUE  unless you set the focus to a control
}

void CEbenezerDlg::OnSysCommand(UINT nID, LPARAM lParam) {

    CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEbenezerDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int   cxIcon = GetSystemMetrics(SM_CXICON);
        int   cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEbenezerDlg::OnQueryDragIcon() {
    return (HCURSOR)m_hIcon;
}

BOOL CEbenezerDlg::DestroyWindow() {
    KillTimer(GAME_TIME);
    KillTimer(SEND_TIME);
    KillTimer(ALIVE_TIME);
    KillTimer(MARKET_BBS_TIME);
    KillTimer(PACKET_CHECK);

    if (m_hReadQueueThread) {
        ::TerminateThread(m_hReadQueueThread, 0);
    }

    if (m_bMMFCreate) {
        UnmapViewOfFile(m_lpMMFile);
        CloseHandle(m_hMMFile);
    }

    if (m_RegionLogFile.m_hFile != CFile::hFileNull) {
        m_RegionLogFile.Close();
    }
    if (m_LogFile.m_hFile != CFile::hFileNull) {
        m_LogFile.Close();
    }

    DeleteCriticalSection(&g_LogFile_critical);
    DeleteCriticalSection(&g_serial_critical);

    if (!m_ItemtableArray.IsEmpty()) {
        m_ItemtableArray.DeleteAllData();
    }

    if (!m_MagictableArray.IsEmpty()) {
        m_MagictableArray.DeleteAllData();
    }

    if (!m_Magictype1Array.IsEmpty()) {
        m_Magictype1Array.DeleteAllData();
    }

    if (!m_Magictype2Array.IsEmpty()) {
        m_Magictype2Array.DeleteAllData();
    }

    if (!m_Magictype3Array.IsEmpty()) {
        m_Magictype3Array.DeleteAllData();
    }

    if (!m_Magictype4Array.IsEmpty()) {
        m_Magictype4Array.DeleteAllData();
    }

    if (!m_Magictype5Array.IsEmpty()) {
        m_Magictype5Array.DeleteAllData();
    }

    if (!m_Magictype8Array.IsEmpty()) {
        m_Magictype8Array.DeleteAllData();
    }

    if (!m_arNpcArray.IsEmpty()) {
        m_arNpcArray.DeleteAllData();
    }

    if (!m_AISocketArray.IsEmpty()) {
        m_AISocketArray.DeleteAllData();
    }

    if (!m_PartyArray.IsEmpty()) {
        m_PartyArray.DeleteAllData();
    }

    if (!m_CoefficientArray.IsEmpty()) {
        m_CoefficientArray.DeleteAllData();
    }

    if (!m_KnightsArray.IsEmpty()) {
        m_KnightsArray.DeleteAllData();
    }

    if (!m_ServerArray.IsEmpty()) {
        m_ServerArray.DeleteAllData();
    }

    if (!m_ServerGroupArray.IsEmpty()) {
        m_ServerGroupArray.DeleteAllData();
    }

    if (!m_HomeArray.IsEmpty()) {
        m_HomeArray.DeleteAllData();
    }

    for (int i = 0; i < m_ZoneArray.size(); i++) {
        delete m_ZoneArray[i];
    }
    m_ZoneArray.clear();

    for (int i = 0; i < m_LevelUpArray.size(); i++) {
        delete m_LevelUpArray[i];
    }
    m_LevelUpArray.clear();

    if (!m_Event.IsEmpty()) {
        m_Event.DeleteAllData();
    }

    if (m_pUdpSocket) {
        delete m_pUdpSocket;
    }

    return CDialog::DestroyWindow();
}

void CEbenezerDlg::UserAcceptThread() {
    // User Socket Accept
    ::ResumeThread(m_Iocport.m_hAcceptThread);
}

CUser * CEbenezerDlg::GetUserPtr(const char * userid, BYTE type) {
    CUser * pUser = NULL;
    BOOL    bFind = FALSE;

    if (type == 0x01) { // Account id check....
        for (int i = 0; i < MAX_USER; i++) {
            pUser = (CUser *)m_Iocport.m_SockArray[i];
            if (pUser) {
                if (!_strnicmp(pUser->m_strAccountID, userid, MAX_ID_SIZE)) {
                    bFind = TRUE;
                    break;
                }
            }
        }
    } else { // character id check...
        for (int i = 0; i < MAX_USER; i++) {
            pUser = (CUser *)m_Iocport.m_SockArray[i];
            if (pUser) {
                if (!_strnicmp(pUser->m_pUserData->m_id, userid, MAX_ID_SIZE)) {
                    bFind = TRUE;
                    break;
                }
            }
        }
    }

    if (!bFind) {
        return NULL;
    }

    return pUser;
}

void CEbenezerDlg::OnTimer(UINT nIDEvent) {
    // sungyong 2002.05.23
    int count = 0, retval = 0;

    switch (nIDEvent) {
    case GAME_TIME:
        UpdateGameTime();
        { // AIServer Socket Alive Check Routine
            if (m_bFirstServerFlag != FALSE) {
                CAISocket * pAISock = NULL;
                for (int i = 0; i < MAX_AI_SOCKET; i++) {
                    pAISock = m_AISocketArray.GetData(i);
                    if (pAISock && pAISock->GetState() == STATE_DISCONNECTED) {
                        AISocketConnect(i, 1);
                    } else if (!pAISock) {
                        AISocketConnect(i, 1);
                    } else {
                        count++;
                    }
                }

                if (count <= 0) {
                    DeleteAllNpcList();
                }
            }
            // sungyong~ 2002.05.23
        }
        break;
    case SEND_TIME:
        m_Iocport.m_PostOverlapped.Offset = OVL_SEND;
        retval =
            PostQueuedCompletionStatus(m_Iocport.m_hSendIOCPort, (DWORD)0, (DWORD)0, &(m_Iocport.m_PostOverlapped));
        if (!retval) {
            int errValue;
            errValue = GetLastError();
            TRACE("Send PostQueued Error : %d\n", errValue);
        }
        break;
    case ALIVE_TIME:
        CheckAliveUser();
        break;
    case MARKET_BBS_TIME:
        MarketBBSTimeCheck();
        break;
    case PACKET_CHECK:
        WritePacketLog();
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

// sungyong 2002.05.22
BOOL CEbenezerDlg::AIServerConnect() {
    C3DMap * pMap = NULL;

    strcpy(m_AIServerIP, m_Ini.GetProfileString("AI_SERVER", "IP", "127.0.0.1"));

    for (int i = 0; i < MAX_AI_SOCKET; i++) {
        if (!AISocketConnect(i)) {
            AfxMessageBox("AI Server Connect Fail!!");
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CEbenezerDlg::AISocketConnect(int zone, int flag) {
    CAISocket * pAISock = NULL;
    int         send_index = 0;
    char        pBuf[128];
    memset(pBuf, NULL, 128);

    //if( m_nServerNo == 3 ) return FALSE;

    pAISock = m_AISocketArray.GetData(zone);
    if (pAISock) {
        if (pAISock->GetState() != STATE_DISCONNECTED) {
            return TRUE;
        }
        m_AISocketArray.DeleteData(zone);
    }

    pAISock = new CAISocket(zone);

    if (!pAISock->Create()) {
        delete pAISock;
        return FALSE;
    }

    if (m_nServerNo == KARUS) {
        if (!pAISock->Connect(&m_Iocport, m_AIServerIP, AI_KARUS_SOCKET_PORT)) {
            delete pAISock;
            return FALSE;
        }
    } else if (m_nServerNo == ELMORAD) {
        if (!pAISock->Connect(&m_Iocport, m_AIServerIP, AI_ELMO_SOCKET_PORT)) {
            delete pAISock;
            return FALSE;
        }
    } else if (m_nServerNo == BATTLE) {
        if (!pAISock->Connect(&m_Iocport, m_AIServerIP, AI_BATTLE_SOCKET_PORT)) {
            delete pAISock;
            return FALSE;
        }
    }

    SetByte(pBuf, AI_SERVER_CONNECT, send_index);
    SetByte(pBuf, zone, send_index);
    if (flag == 1) {
        SetByte(pBuf, 1, send_index); // 재접속
    } else {
        SetByte(pBuf, 0, send_index); // 처음 접속..
    }
    pAISock->Send(pBuf, send_index);

    // 해야할일 :이 부분 처리.....
    //SendAllUserInfo();
    //m_sSocketCount = zone;
    m_AISocketArray.PutData(zone, pAISock);

    TRACE("**** AISocket Connect Success!! ,, zone = %d ****\n", zone);
    return TRUE;
}
// ~sungyong 2002.05.22

void CEbenezerDlg::Send_All(char * pBuf, int len, CUser * pExceptUser, int nation) {
    CUser * pUser = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser == pExceptUser) {
            continue;
        }
        if (pUser->GetState() == STATE_GAMESTART) {
            if (nation == 0) {
                pUser->Send(pBuf, len);
            } else if (nation == pUser->m_pUserData->m_bNation) {
                pUser->Send(pBuf, len);
            }
        }
    }
}

void CEbenezerDlg::Send_Region(char * pBuf, int len, int zone, int x, int z, CUser * pExceptUser, bool bDirect) {
    int zoneindex = GetZoneIndex(zone);
    if (zoneindex == -1) {
        return;
    }

    Send_UnitRegion(pBuf, len, zoneindex, x, z, pExceptUser, bDirect);
    Send_UnitRegion(pBuf, len, zoneindex, x - 1, z - 1, pExceptUser, bDirect); // NW
    Send_UnitRegion(pBuf, len, zoneindex, x, z - 1, pExceptUser, bDirect);     // N
    Send_UnitRegion(pBuf, len, zoneindex, x + 1, z - 1, pExceptUser, bDirect); // NE
    Send_UnitRegion(pBuf, len, zoneindex, x - 1, z, pExceptUser, bDirect);     // W
    Send_UnitRegion(pBuf, len, zoneindex, x + 1, z, pExceptUser, bDirect);     // E
    Send_UnitRegion(pBuf, len, zoneindex, x - 1, z + 1, pExceptUser, bDirect); // SW
    Send_UnitRegion(pBuf, len, zoneindex, x, z + 1, pExceptUser, bDirect);     // S
    Send_UnitRegion(pBuf, len, zoneindex, x + 1, z + 1, pExceptUser, bDirect); // SE
}

void CEbenezerDlg::Send_UnitRegion(char * pBuf, int len, int zoneindex, int x, int z, CUser * pExceptUser,
                                   bool bDirect) {
    int      t_count = 0, uid = -1;
    CUser *  pUser = NULL;
    C3DMap * pMap = NULL;

    pMap = (C3DMap *)m_ZoneArray[zoneindex];
    if (!pMap) {
        return;
    }
    if (x < 0 || z < 0 || x > pMap->GetXRegionMax() || z > pMap->GetZRegionMax()) {
        return;
    }

    EnterCriticalSection(&g_region_critical);

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.end();

    for (; Iter1 != Iter2; Iter1++) {
        uid = *((*Iter1).second);
        if (uid < 0) {
            continue;
        }
        pUser = (CUser *)m_Iocport.m_SockArray[uid];
        if (pUser == pExceptUser) {
            continue;
        }
        if (pUser && (pUser->GetState() == STATE_GAMESTART)) {
            if (bDirect) {
                pUser->Send(pBuf, len);
            } else {
                pUser->RegionPacketAdd(pBuf, len);
            }
        }
    }

    LeaveCriticalSection(&g_region_critical);
}

void CEbenezerDlg::Send_NearRegion(char * pBuf, int len, int zone, int region_x, int region_z, float curx, float curz,
                                   CUser * pExceptUser) {
    int left_border = region_x * VIEW_DISTANCE, top_border = region_z * VIEW_DISTANCE;
    int zoneindex = GetZoneIndex(zone);
    if (zoneindex == -1) {
        return;
    }

    Send_FilterUnitRegion(pBuf, len, zoneindex, region_x, region_z, curx, curz, pExceptUser);
    if (((curx - left_border) > (VIEW_DISTANCE / 2.0f))) {    // RIGHT
        if (((curz - top_border) > (VIEW_DISTANCE / 2.0f))) { // BOTTOM
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x + 1, region_z, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x, region_z + 1, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x + 1, region_z + 1, curx, curz, pExceptUser);
        } else { // TOP
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x + 1, region_z, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x, region_z - 1, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x + 1, region_z - 1, curx, curz, pExceptUser);
        }
    } else {                                                  // LEFT
        if (((curz - top_border) > (VIEW_DISTANCE / 2.0f))) { // BOTTOM
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x - 1, region_z, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x, region_z + 1, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x - 1, region_z + 1, curx, curz, pExceptUser);
        } else { // TOP
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x - 1, region_z, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x, region_z - 1, curx, curz, pExceptUser);
            Send_FilterUnitRegion(pBuf, len, zoneindex, region_x - 1, region_z - 1, curx, curz, pExceptUser);
        }
    }
}

void CEbenezerDlg::Send_FilterUnitRegion(char * pBuf, int len, int zoneindex, int x, int z, float ref_x, float ref_z,
                                         CUser * pExceptUser) {
    int      t_count = 0, uid = -1;
    CUser *  pUser = NULL;
    C3DMap * pMap = NULL;

    pMap = (C3DMap *)m_ZoneArray[zoneindex];
    if (!pMap) {
        return;
    }
    if (x < 0 || z < 0 || x > pMap->GetXRegionMax() || z > pMap->GetZRegionMax()) {
        return;
    }

    //    EnterCriticalSection( &g_region_critical );

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.end();

    for (; Iter1 != Iter2; Iter1++) {
        uid = *((*Iter1).second);
        if (uid < 0) {
            continue;
        }
        pUser = (CUser *)m_Iocport.m_SockArray[uid];
        if (pUser == pExceptUser) {
            continue;
        }
        if (pUser && (pUser->GetState() == STATE_GAMESTART)) {
            if (sqrt(pow((pUser->m_pUserData->m_curx - ref_x), 2) + pow((pUser->m_pUserData->m_curz - ref_z), 2)) <
                32) {
                pUser->RegionPacketAdd(pBuf, len);
            }
        }
    }

    //    LeaveCriticalSection( &g_region_critical );
}

void CEbenezerDlg::Send_PartyMember(int party, char * pBuf, int len) {
    if (party < 0) {
        return;
    }
    _PARTY_GROUP * pParty = NULL;
    CUser *        pUser = NULL;

    pParty = m_PartyArray.GetData(party);
    if (!pParty) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (pParty->uid[i] == -1 || pParty->uid[i] >= MAX_USER) {
            continue;
        }
        pUser = (CUser *)m_Iocport.m_SockArray[pParty->uid[i]];
        if (pUser) {
            pUser->Send(pBuf, len);
        }
    }
}

void CEbenezerDlg::Send_KnightsMember(int index, char * pBuf, int len, int zone) {
    if (index <= 0) {
        return;
    }
    CKnights * pKnights = NULL;
    CUser *    pUser = NULL;
    pKnights = m_KnightsArray.GetData(index);
    if (!pKnights) {
        return;
    }
    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->m_pUserData->m_bKnights != index) {
            continue;
        }
        if (zone == 100) {
            pUser->Send(pBuf, len);
        } else {
            if (pUser->m_pUserData->m_bZone == zone) {
                pUser->Send(pBuf, len);
            }
        }
    }
}

// sungyong 2002.05.22
void CEbenezerDlg::Send_AIServer(int zone, char * pBuf, int len) {
    CAISocket * pSocket = NULL;
    int         send_size = 0, old_send_socket = 0;

    for (int i = 0; i < MAX_AI_SOCKET; i++) {
        pSocket = m_AISocketArray.GetData(i);
        if (pSocket == NULL) {
            m_sSendSocket++;
            if (m_sSendSocket >= MAX_AI_SOCKET) {
                m_sSendSocket = 0;
            }
            continue;
        }
        if (i == m_sSendSocket) {
            send_size = pSocket->Send(pBuf, len);
            old_send_socket = m_sSendSocket;
            m_sSendSocket++;
            if (m_sSendSocket >= MAX_AI_SOCKET) {
                m_sSendSocket = 0;
            }
            if (send_size == 0) {
                continue;
            } else {
                //TRACE(" <--- Send_AIServer : length = %d, socket = %d \n", send_size, old_send_socket);
                return;
            }
        } else {
            continue;
        }
    }
}
// ~sungyong 2002.05.22

BOOL CEbenezerDlg::InitializeMMF() {
    BOOL    bCreate = TRUE;
    CString logstr;

    DWORD filesize = MAX_USER * 4000; // 1명당 3000 bytes 이내 소요
    m_hMMFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, filesize, "KNIGHT_DB");

    if (m_hMMFile != NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
        m_hMMFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, "KNIGHT_DB");
        if (m_hMMFile == NULL) {
            logstr = "Shared Memory Load Fail!!";
            m_hMMFile = INVALID_HANDLE_VALUE;
            return FALSE;
        }
        bCreate = FALSE;
    }
    logstr = "Shared Memory Create Success!!";
    m_StatusList.AddString(logstr);

    m_lpMMFile = (char *)MapViewOfFile(m_hMMFile, FILE_MAP_WRITE, 0, 0, 0);
    if (!m_lpMMFile) {
        return FALSE;
    }
    memset(m_lpMMFile, NULL, filesize);

    m_bMMFCreate = bCreate;

    CUser * pUser = NULL;
    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArrayInActive[i];
        if (pUser) {
            pUser->m_pUserData = (_USER_DATA *)(m_lpMMFile + i * 4000); // 1 Person Offset are 4000 bytes
        }
    }

    return TRUE;
}

BOOL CEbenezerDlg::MapFileLoad() {
    CFile    file;
    CString  szFullPath, errormsg, sZoneName;
    C3DMap * pMap = NULL;
    EVENT *  pEvent = NULL;

    CZoneInfoSet ZoneInfoSet;

    if (!ZoneInfoSet.Open()) {
        AfxMessageBox(_T("ZoneInfoTable Open Fail!"));
        return FALSE;
    }
    if (ZoneInfoSet.IsBOF() || ZoneInfoSet.IsEOF()) {
        AfxMessageBox(_T("ZoneInfoTable Empty!"));
        return FALSE;
    }

    m_ZoneArray.reserve(10);

    ZoneInfoSet.MoveFirst();

    while (!ZoneInfoSet.IsEOF()) {
        sZoneName = ZoneInfoSet.m_strZoneName;
        szFullPath.Format(".\\Ebenezer_MAP\\%s", sZoneName);

        LogFileWrite("mapfile load\r\n");
        if (!file.Open(szFullPath, CFile::modeRead)) {
            errormsg.Format("File Open Fail - %s\n", szFullPath);
            AfxMessageBox(errormsg);
            return FALSE;
        }

        pMap = new C3DMap;

        pMap->m_nServerNo = ZoneInfoSet.m_ServerNo;
        pMap->m_nZoneNumber = ZoneInfoSet.m_ZoneNo;
        strcpy(pMap->m_MapName, (char *)(LPCTSTR)sZoneName);
        pMap->m_fInitX = (float)(ZoneInfoSet.m_InitX / 100.0);
        pMap->m_fInitZ = (float)(ZoneInfoSet.m_InitZ / 100.0);
        pMap->m_fInitY = (float)(ZoneInfoSet.m_InitY / 100.0);
        pMap->m_bType = ZoneInfoSet.m_Type;

        if (!pMap->LoadMap((HANDLE)file.m_hFile)) {
            errormsg.Format("Map Load Fail - %s\n", szFullPath);
            AfxMessageBox(errormsg);
            delete pMap;
            return FALSE;
        }
        m_ZoneArray.push_back(pMap);

        // 스트립트를 읽어 들인다.
        LogFileWrite("before script\r\n");

        pEvent = new EVENT;
        if (!pEvent->LoadEvent(ZoneInfoSet.m_ZoneNo)) {
            delete pEvent;
            pEvent = NULL;
        }
        if (pEvent) {
            if (!m_Event.PutData(pEvent->m_Zone, pEvent)) {
                delete pEvent;
                pEvent = NULL;
            }
        }

        ZoneInfoSet.MoveNext();

        file.Close();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadItemTable() {
    CItemTableSet ItemTableSet;

    if (!ItemTableSet.Open()) {
        AfxMessageBox(_T("ItemTable Open Fail!"));
        return FALSE;
    }
    if (ItemTableSet.IsBOF() || ItemTableSet.IsEOF()) {
        AfxMessageBox(_T("ItemTable Empty!"));
        return FALSE;
    }

    ItemTableSet.MoveFirst();

    while (!ItemTableSet.IsEOF()) {
        _ITEM_TABLE * pTableItem = new _ITEM_TABLE;

        pTableItem->m_iNum = ItemTableSet.m_Num;
        strcpy(pTableItem->m_strName, ItemTableSet.m_strName);
        pTableItem->m_bKind = ItemTableSet.m_Kind;
        pTableItem->m_bSlot = ItemTableSet.m_Slot;
        pTableItem->m_bRace = ItemTableSet.m_Race;
        pTableItem->m_bClass = ItemTableSet.m_Class;
        pTableItem->m_sDamage = ItemTableSet.m_Damage;
        pTableItem->m_sDelay = ItemTableSet.m_Delay;
        pTableItem->m_sRange = ItemTableSet.m_Range;
        pTableItem->m_sWeight = ItemTableSet.m_Weight;
        pTableItem->m_sDuration = ItemTableSet.m_Duration;
        pTableItem->m_iBuyPrice = ItemTableSet.m_BuyPrice;
        pTableItem->m_iSellPrice = ItemTableSet.m_SellPrice;
        pTableItem->m_sAc = ItemTableSet.m_Ac;
        pTableItem->m_bCountable = ItemTableSet.m_Countable;
        pTableItem->m_iEffect1 = ItemTableSet.m_Effect1;
        pTableItem->m_iEffect2 = ItemTableSet.m_Effect2;
        pTableItem->m_bReqLevel = ItemTableSet.m_ReqLevel;
        pTableItem->m_bReqRank = ItemTableSet.m_ReqRank;
        pTableItem->m_bReqTitle = ItemTableSet.m_ReqTitle;
        pTableItem->m_bReqStr = ItemTableSet.m_ReqStr;
        pTableItem->m_bReqSta = ItemTableSet.m_ReqSta;
        pTableItem->m_bReqDex = ItemTableSet.m_ReqDex;
        pTableItem->m_bReqIntel = ItemTableSet.m_ReqIntel;
        pTableItem->m_bReqCha = ItemTableSet.m_ReqCha;
        pTableItem->m_bSellingGroup = ItemTableSet.m_SellingGroup;
        pTableItem->m_ItemType = ItemTableSet.m_ItemType;
        pTableItem->m_sHitrate = ItemTableSet.m_Hitrate;
        pTableItem->m_sEvarate = ItemTableSet.m_Evasionrate;
        pTableItem->m_sDaggerAc = ItemTableSet.m_DaggerAc;
        pTableItem->m_sSwordAc = ItemTableSet.m_SwordAc;
        pTableItem->m_sMaceAc = ItemTableSet.m_MaceAc;
        pTableItem->m_sAxeAc = ItemTableSet.m_AxeAc;
        pTableItem->m_sSpearAc = ItemTableSet.m_SpearAc;
        pTableItem->m_sBowAc = ItemTableSet.m_BowAc;
        pTableItem->m_bFireDamage = ItemTableSet.m_FireDamage;
        pTableItem->m_bIceDamage = ItemTableSet.m_IceDamage;
        pTableItem->m_bLightningDamage = ItemTableSet.m_LightningDamage;
        pTableItem->m_bPoisonDamage = ItemTableSet.m_PoisonDamage;
        pTableItem->m_bHPDrain = ItemTableSet.m_HPDrain;
        pTableItem->m_bMPDamage = ItemTableSet.m_MPDamage;
        pTableItem->m_bMPDrain = ItemTableSet.m_MPDrain;
        pTableItem->m_bMirrorDamage = ItemTableSet.m_MirrorDamage;
        pTableItem->m_bDroprate = ItemTableSet.m_Droprate;
        pTableItem->m_bStrB = ItemTableSet.m_StrB;
        pTableItem->m_bStaB = ItemTableSet.m_StaB;
        pTableItem->m_bDexB = ItemTableSet.m_DexB;
        pTableItem->m_bIntelB = ItemTableSet.m_IntelB;
        pTableItem->m_bChaB = ItemTableSet.m_ChaB;
        pTableItem->m_MaxHpB = ItemTableSet.m_MaxHpB;
        pTableItem->m_MaxMpB = ItemTableSet.m_MaxMpB;
        pTableItem->m_bFireR = ItemTableSet.m_FireR;
        pTableItem->m_bColdR = ItemTableSet.m_ColdR;
        pTableItem->m_bLightningR = ItemTableSet.m_LightningR;
        pTableItem->m_bMagicR = ItemTableSet.m_MagicR;
        pTableItem->m_bPoisonR = ItemTableSet.m_PoisonR;
        pTableItem->m_bCurseR = ItemTableSet.m_CurseR;

        if (!m_ItemtableArray.PutData(pTableItem->m_iNum, pTableItem)) {
            TRACE("ItemTable PutData Fail - %d\n", pTableItem->m_iNum);
            delete pTableItem;
            pTableItem = NULL;
        }

        ItemTableSet.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicTable() {
    CMagicTableSet MagicTableSet;

    if (!MagicTableSet.Open()) {
        AfxMessageBox(_T("MagicTable Open Fail!"));
        return FALSE;
    }
    if (MagicTableSet.IsBOF() || MagicTableSet.IsEOF()) {
        AfxMessageBox(_T("MagicTable Empty!"));
        return FALSE;
    }

    MagicTableSet.MoveFirst();

    while (!MagicTableSet.IsEOF()) {
        _MAGIC_TABLE * pTableMagic = new _MAGIC_TABLE;

        pTableMagic->iNum = MagicTableSet.m_MagicNum;
        pTableMagic->sFlyingEffect = MagicTableSet.m_FlyingEffect;
        pTableMagic->bMoral = MagicTableSet.m_Moral;
        pTableMagic->bSkillLevel = MagicTableSet.m_SkillLevel;
        pTableMagic->sSkill = MagicTableSet.m_Skill;
        pTableMagic->sMsp = MagicTableSet.m_Msp;
        pTableMagic->sHP = MagicTableSet.m_HP;
        pTableMagic->bItemGroup = MagicTableSet.m_ItemGroup;
        pTableMagic->iUseItem = MagicTableSet.m_UseItem;
        pTableMagic->bCastTime = MagicTableSet.m_CastTime;
        pTableMagic->bReCastTime = MagicTableSet.m_ReCastTime;
        pTableMagic->bSuccessRate = MagicTableSet.m_SuccessRate;
        pTableMagic->bType1 = MagicTableSet.m_Type1;
        pTableMagic->bType2 = MagicTableSet.m_Type2;
        pTableMagic->sRange = MagicTableSet.m_Range;
        pTableMagic->bEtc = MagicTableSet.m_Etc;

        if (!m_MagictableArray.PutData(pTableMagic->iNum, pTableMagic)) {
            TRACE("MagicTable PutData Fail - %d\n", pTableMagic->iNum);
            delete pTableMagic;
            pTableMagic = NULL;
        }

        MagicTableSet.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType1() {
    CMagicType1Set MagicType1Set;

    if (!MagicType1Set.Open()) {
        AfxMessageBox(_T("MagicType1 Open Fail!"));
        return FALSE;
    }
    if (MagicType1Set.IsBOF() || MagicType1Set.IsEOF()) {
        AfxMessageBox(_T("MagicType1 Empty!"));
        return FALSE;
    }

    MagicType1Set.MoveFirst();

    while (!MagicType1Set.IsEOF()) {
        _MAGIC_TYPE1 * pType1Magic = new _MAGIC_TYPE1;

        pType1Magic->iNum = MagicType1Set.m_iNum;
        pType1Magic->bHitType = MagicType1Set.m_Type;
        pType1Magic->bDelay = MagicType1Set.m_Delay;
        pType1Magic->bComboCount = MagicType1Set.m_ComboCount;
        pType1Magic->bComboType = MagicType1Set.m_ComboType;
        pType1Magic->sComboDamage = MagicType1Set.m_ComboDamage;
        pType1Magic->sHit = MagicType1Set.m_Hit;
        pType1Magic->sHitRate = MagicType1Set.m_HitRate;
        pType1Magic->sRange = MagicType1Set.m_Range;

        if (!m_Magictype1Array.PutData(pType1Magic->iNum, pType1Magic)) {
            TRACE("MagicType1 PutData Fail - %d\n", pType1Magic->iNum);
            delete pType1Magic;
            pType1Magic = NULL;
        }

        MagicType1Set.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType2() {
    CMagicType2Set MagicType2Set;

    if (!MagicType2Set.Open()) {
        AfxMessageBox(_T("MagicType2 Open Fail!"));
        return FALSE;
    }
    if (MagicType2Set.IsBOF() || MagicType2Set.IsEOF()) {
        AfxMessageBox(_T("MagicType2 Empty!"));
        return FALSE;
    }

    MagicType2Set.MoveFirst();

    while (!MagicType2Set.IsEOF()) {
        _MAGIC_TYPE2 * pType2Magic = new _MAGIC_TYPE2;

        pType2Magic->iNum = MagicType2Set.m_iNum;
        pType2Magic->bHitType = MagicType2Set.m_HitType;
        pType2Magic->sHitRate = MagicType2Set.m_HitRate;
        pType2Magic->sAddDamage = MagicType2Set.m_AddDamage;
        pType2Magic->sAddRange = MagicType2Set.m_AddRange;
        pType2Magic->bNeedArrow = MagicType2Set.m_NeedArrow;

        if (!m_Magictype2Array.PutData(pType2Magic->iNum, pType2Magic)) {
            TRACE("MagicType2 PutData Fail - %d\n", pType2Magic->iNum);
            delete pType2Magic;
            pType2Magic = NULL;
        }
        MagicType2Set.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType3() {
    CMagicType3Set MagicType3Set;

    if (!MagicType3Set.Open()) {
        AfxMessageBox(_T("MagicType3 Open Fail!"));
        return FALSE;
    }
    if (MagicType3Set.IsBOF() || MagicType3Set.IsEOF()) {
        AfxMessageBox(_T("MagicType3 Empty!"));
        return FALSE;
    }

    MagicType3Set.MoveFirst();

    while (!MagicType3Set.IsEOF()) {
        _MAGIC_TYPE3 * pType3Magic = new _MAGIC_TYPE3;

        pType3Magic->iNum = MagicType3Set.m_iNum;
        pType3Magic->bAttribute = MagicType3Set.m_Attribute;
        pType3Magic->bDirectType = MagicType3Set.m_DirectType;
        pType3Magic->bRadius = MagicType3Set.m_Radius;
        pType3Magic->sAngle = MagicType3Set.m_Angle;
        pType3Magic->sDuration = MagicType3Set.m_Duration;
        pType3Magic->sEndDamage = MagicType3Set.m_EndDamage;
        pType3Magic->sFirstDamage = MagicType3Set.m_FirstDamage;
        pType3Magic->sTimeDamage = MagicType3Set.m_TimeDamage;

        if (!m_Magictype3Array.PutData(pType3Magic->iNum, pType3Magic)) {
            TRACE("MagicType3 PutData Fail - %d\n", pType3Magic->iNum);
            delete pType3Magic;
            pType3Magic = NULL;
        }
        MagicType3Set.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType4() {
    CMagicType4Set MagicType4Set;

    if (!MagicType4Set.Open()) {
        AfxMessageBox(_T("MagicType4 Open Fail!"));
        return FALSE;
    }
    if (MagicType4Set.IsBOF() || MagicType4Set.IsEOF()) {
        AfxMessageBox(_T("MagicType4 Empty!"));
        return FALSE;
    }

    MagicType4Set.MoveFirst();

    while (!MagicType4Set.IsEOF()) {
        _MAGIC_TYPE4 * pType4Magic = new _MAGIC_TYPE4;

        pType4Magic->iNum = MagicType4Set.m_iNum;
        pType4Magic->bBuffType = MagicType4Set.m_BuffType;
        pType4Magic->bRadius = MagicType4Set.m_Radius;
        pType4Magic->sDuration = MagicType4Set.m_Duration;
        pType4Magic->bAttackSpeed = MagicType4Set.m_AttackSpeed;
        pType4Magic->bSpeed = MagicType4Set.m_Speed;
        pType4Magic->sAC = MagicType4Set.m_AC;
        pType4Magic->bAttack = MagicType4Set.m_Attack;
        pType4Magic->sMaxHP = MagicType4Set.m_MaxHP;
        pType4Magic->bHitRate = MagicType4Set.m_HitRate;
        pType4Magic->sAvoidRate = MagicType4Set.m_AvoidRate;
        pType4Magic->bStr = MagicType4Set.m_Str;
        pType4Magic->bSta = MagicType4Set.m_Sta;
        pType4Magic->bDex = MagicType4Set.m_Dex;
        pType4Magic->bIntel = MagicType4Set.m_Intel;
        pType4Magic->bCha = MagicType4Set.m_Cha;
        pType4Magic->bFireR = MagicType4Set.m_FireR;
        pType4Magic->bColdR = MagicType4Set.m_ColdR;
        pType4Magic->bLightningR = MagicType4Set.m_LightningR;
        pType4Magic->bMagicR = MagicType4Set.m_MagicR;
        pType4Magic->bDiseaseR = MagicType4Set.m_DiseaseR;
        pType4Magic->bPoisonR = MagicType4Set.m_PoisonR;

        if (!m_Magictype4Array.PutData(pType4Magic->iNum, pType4Magic)) {
            TRACE("MagicType4 PutData Fail - %d\n", pType4Magic->iNum);
            delete pType4Magic;
            pType4Magic = NULL;
        }
        MagicType4Set.MoveNext();
    }
    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType5() {
    CMagicType5Set MagicType5Set;

    if (!MagicType5Set.Open()) {
        AfxMessageBox(_T("MagicType5 Open Fail!"));
        return FALSE;
    }

    if (MagicType5Set.IsBOF() || MagicType5Set.IsEOF()) {
        AfxMessageBox(_T("MagicType5 Empty!"));
        return FALSE;
    }

    MagicType5Set.MoveFirst();

    while (!MagicType5Set.IsEOF()) {
        _MAGIC_TYPE5 * pType5Magic = new _MAGIC_TYPE5;

        pType5Magic->iNum = MagicType5Set.m_iNum;
        pType5Magic->bType = MagicType5Set.m_Type;
        pType5Magic->bExpRecover = MagicType5Set.m_ExpRecover;
        pType5Magic->sNeedStone = MagicType5Set.m_NeedStone;

        if (!m_Magictype5Array.PutData(pType5Magic->iNum, pType5Magic)) {
            TRACE("MagicType5 PutData Fail - %d\n", pType5Magic->iNum);
            delete pType5Magic;
            pType5Magic = NULL;
        }
        MagicType5Set.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadMagicType8() {
    CMagicType8Set MagicType8Set;

    if (!MagicType8Set.Open()) {
        AfxMessageBox(_T("MagicType8 Open Fail!"));
        return FALSE;
    }
    if (MagicType8Set.IsBOF() || MagicType8Set.IsEOF()) {
        AfxMessageBox(_T("MagicType8 Empty!"));
        return FALSE;
    }

    MagicType8Set.MoveFirst();

    while (!MagicType8Set.IsEOF()) {
        _MAGIC_TYPE8 * pType8Magic = new _MAGIC_TYPE8;

        pType8Magic->iNum = MagicType8Set.m_iNum;
        pType8Magic->bTarget = MagicType8Set.m_Target;
        pType8Magic->sRadius = MagicType8Set.m_Radius;
        pType8Magic->bWarpType = MagicType8Set.m_WarpType;
        pType8Magic->sExpRecover = MagicType8Set.m_ExpRecover;

        if (!m_Magictype8Array.PutData(pType8Magic->iNum, pType8Magic)) {
            TRACE("MagicType8 PutData Fail - %d\n", pType8Magic->iNum);
            delete pType8Magic;
            pType8Magic = NULL;
        }

        MagicType8Set.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadCoefficientTable() {
    CCoefficientSet CoefficientSet;

    if (!CoefficientSet.Open()) {
        AfxMessageBox(_T("CharacterDataTable Open Fail!"));
        return FALSE;
    }
    if (CoefficientSet.IsBOF() || CoefficientSet.IsEOF()) {
        AfxMessageBox(_T("CharaterDataTable Empty!"));
        return FALSE;
    }

    CoefficientSet.MoveFirst();

    while (!CoefficientSet.IsEOF()) {
        _CLASS_COEFFICIENT * p_TableCoefficient = new _CLASS_COEFFICIENT;

        p_TableCoefficient->sClassNum = (short)CoefficientSet.m_sClass;
        p_TableCoefficient->ShortSword = (float)CoefficientSet.m_ShortSword;
        p_TableCoefficient->Sword = (float)CoefficientSet.m_Sword;
        p_TableCoefficient->Axe = (float)CoefficientSet.m_Axe;
        p_TableCoefficient->Club = (float)CoefficientSet.m_Club;
        p_TableCoefficient->Spear = (float)CoefficientSet.m_Spear;
        p_TableCoefficient->Pole = (float)CoefficientSet.m_Pole;
        p_TableCoefficient->Staff = (float)CoefficientSet.m_Staff;
        p_TableCoefficient->Bow = (float)CoefficientSet.m_Bow;
        p_TableCoefficient->HP = (float)CoefficientSet.m_Hp;
        p_TableCoefficient->MP = (float)CoefficientSet.m_Mp;
        p_TableCoefficient->SP = (float)CoefficientSet.m_Sp;
        p_TableCoefficient->AC = (float)CoefficientSet.m_Ac;
        p_TableCoefficient->Hitrate = (float)CoefficientSet.m_Hitrate;
        p_TableCoefficient->Evasionrate = (float)CoefficientSet.m_Evasionrate;

        if (!m_CoefficientArray.PutData(p_TableCoefficient->sClassNum, p_TableCoefficient)) {
            TRACE("Coefficient PutData Fail - %d\n", p_TableCoefficient->sClassNum);
            delete p_TableCoefficient;
            p_TableCoefficient = NULL;
        }

        CoefficientSet.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadLevelUpTable() {
    CLevelUpTableSet LevelUpTableSet;

    if (!LevelUpTableSet.Open()) {
        AfxMessageBox(_T("LevelUpTable Open Fail!"));
        return FALSE;
    }
    if (LevelUpTableSet.IsBOF() || LevelUpTableSet.IsEOF()) {
        AfxMessageBox(_T("LevelUpTable Empty!"));
        return FALSE;
    }

    m_LevelUpArray.reserve(MAX_LEVEL);

    LevelUpTableSet.MoveFirst();

    while (!LevelUpTableSet.IsEOF()) {
        _LEVELUP * pTableLevelUp = new _LEVELUP;

        pTableLevelUp->m_sLevel = LevelUpTableSet.m_level;
        pTableLevelUp->m_iExp = LevelUpTableSet.m_Exp;

        m_LevelUpArray.push_back(pTableLevelUp);

        LevelUpTableSet.MoveNext();
    }

    return TRUE;
}

void CEbenezerDlg::GetTimeFromIni() {
    int  year = 0, month = 0, date = 0, hour = 0, server_count = 0, sgroup_count = 0;
    char ipkey[20];
    memset(ipkey, 0x00, 20);

    m_Ini.SetPath("server.ini");
    m_nYear = m_Ini.GetProfileInt("TIMER", "YEAR", 1);
    m_nMonth = m_Ini.GetProfileInt("TIMER", "MONTH", 1);
    m_nDate = m_Ini.GetProfileInt("TIMER", "DATE", 1);
    m_nHour = m_Ini.GetProfileInt("TIMER", "HOUR", 1);
    m_nWeather = m_Ini.GetProfileInt("TIMER", "WEATHER", 1);

    //    m_nBattleZoneOpenWeek  = m_Ini.GetProfileInt("BATTLE", "WEEK", 3);
    m_nBattleZoneOpenWeek = m_Ini.GetProfileInt("BATTLE", "WEEK", 5);
    m_nBattleZoneOpenHourStart = m_Ini.GetProfileInt("BATTLE", "START_TIME", 20);
    m_nBattleZoneOpenHourEnd = m_Ini.GetProfileInt("BATTLE", "END_TIME", 0);

    m_nCastleCapture = m_Ini.GetProfileInt("CASTLE", "NATION", 1);
    m_nServerNo = m_Ini.GetProfileInt("ZONE_INFO", "MY_INFO", 1);
    m_nServerGroup = m_Ini.GetProfileInt("ZONE_INFO", "SERVER_NUM", 0);
    server_count = m_Ini.GetProfileInt("ZONE_INFO", "SERVER_COUNT", 1);
    if (server_count < 1) {
        AfxMessageBox("ServerCount Error!!");
        return;
    }

    for (int i = 0; i < server_count; i++) {
        _ZONE_SERVERINFO * pInfo = new _ZONE_SERVERINFO;
        sprintf(ipkey, "SERVER_%02d", i);
        pInfo->sServerNo = m_Ini.GetProfileInt("ZONE_INFO", ipkey, 1);
        sprintf(ipkey, "SERVER_IP_%02d", i);
        strcpy(pInfo->strServerIP, m_Ini.GetProfileString("ZONE_INFO", ipkey, "127.0.0.1"));
        pInfo->sPort = _LISTEN_PORT + pInfo->sServerNo;

        m_ServerArray.PutData(pInfo->sServerNo, pInfo);
    }

    if (m_nServerGroup != 0) {
        m_nServerGroupNo = m_Ini.GetProfileInt("SG_INFO", "GMY_INFO", 1);
        sgroup_count = m_Ini.GetProfileInt("SG_INFO", "GSERVER_COUNT", 1);
        if (server_count < 1) {
            AfxMessageBox("ServerCount Error!!");
            return;
        }
        for (int i = 0; i < sgroup_count; i++) {
            _ZONE_SERVERINFO * pInfo = new _ZONE_SERVERINFO;
            sprintf(ipkey, "GSERVER_%02d", i);
            pInfo->sServerNo = m_Ini.GetProfileInt("SG_INFO", ipkey, 1);
            sprintf(ipkey, "GSERVER_IP_%02d", i);
            strcpy(pInfo->strServerIP, m_Ini.GetProfileString("SG_INFO", ipkey, "127.0.0.1"));
            pInfo->sPort = _LISTEN_PORT + pInfo->sServerNo;

            m_ServerGroupArray.PutData(pInfo->sServerNo, pInfo);
        }
    }

    SetTimer(GAME_TIME, 6000, NULL);
    SetTimer(SEND_TIME, 200, NULL);
    SetTimer(ALIVE_TIME, 34000, NULL);
    SetTimer(MARKET_BBS_TIME, 300000, NULL);
    SetTimer(PACKET_CHECK, 360000, NULL);
}

void CEbenezerDlg::UpdateGameTime() {
    CUser * pUser = NULL;
    BOOL    bKnights = FALSE;

    m_nMin++;

    BattleZoneOpenTimer(); // Check if it's time for the BattleZone to open or end.

    if (m_nMin == 60) {
        m_nHour++;
        m_nMin = 0;
        UpdateWeather();
        SetGameTime();
        //  갓댐 산타!! >.<
        if (m_bSanta) {
            FlySanta();
        }
        //
    }
    if (m_nHour == 24) {
        m_nDate++;
        m_nHour = 0;
        bKnights = TRUE;
    }
    if (m_nDate == 31) {
        m_nMonth++;
        m_nDate = 1;
    }
    if (m_nMonth == 13) {
        m_nYear++;
        m_nMonth = 1;
    }

    // ai status check packet...
    m_sErrorSocketCount++;
    int  send_index = 0;
    char pSendBuf[256];
    ::ZeroMemory(pSendBuf, sizeof(pSendBuf));
    //SetByte(pSendBuf, AG_CHECK_ALIVE_REQ, send_index);
    //Send_AIServer(1000, pSendBuf, send_index);

    // 시간과 날씨 정보를 보낸다..
    ::ZeroMemory(pSendBuf, sizeof(pSendBuf));
    send_index = 0;
    SetByte(pSendBuf, AG_TIME_WEATHER, send_index);
    SetShort(pSendBuf, m_nYear, send_index); // time info
    SetShort(pSendBuf, m_nMonth, send_index);
    SetShort(pSendBuf, m_nDate, send_index);
    SetShort(pSendBuf, m_nHour, send_index);
    SetShort(pSendBuf, m_nMin, send_index);
    SetByte(pSendBuf, (BYTE)m_nWeather, send_index); // weather info
    SetShort(pSendBuf, m_nAmount, send_index);
    Send_AIServer(1000, pSendBuf, send_index);

    if (bKnights) {
        ::ZeroMemory(pSendBuf, sizeof(pSendBuf));
        send_index = 0;
        SetByte(pSendBuf, WIZ_KNIGHTS_PROCESS, send_index);
        SetByte(pSendBuf, KNIGHTS_ALLLIST_REQ + 0x10, send_index);
        SetByte(pSendBuf, m_nServerNo, send_index);
        m_LoggerSendQueue.PutData(pSendBuf, send_index);
    }
}

void CEbenezerDlg::UpdateWeather() {
    int  weather = 0, result = 0, send_index = 0;
    char send_buff[256];
    memset(send_buff, NULL, 256);

    result = myrand(0, 100);

    //    if( result < 5 )
    if (result < 2) {
        weather = WEATHER_SNOW;
    }
    //    else if( result < 15 )
    else if (result < 7) {
        weather = WEATHER_RAIN;
    } else {
        weather = WEATHER_FINE;
    }

    m_nAmount = myrand(0, 100);
    if (weather == WEATHER_FINE) { // WEATHER_FINE 일때 m_nAmount 는 안개 정도 표시
        if (m_nAmount > 70) {
            m_nAmount = m_nAmount / 2;
        } else {
            m_nAmount = 0;
        }
    }
    m_nWeather = weather;
    SetByte(send_buff, WIZ_WEATHER, send_index);
    SetByte(send_buff, (BYTE)m_nWeather, send_index);
    SetShort(send_buff, m_nAmount, send_index);
    Send_All(send_buff, send_index);
}

void CEbenezerDlg::SetGameTime() {
    m_Ini.SetProfileInt("TIMER", "YEAR", m_nYear);
    m_Ini.SetProfileInt("TIMER", "MONTH", m_nMonth);
    m_Ini.SetProfileInt("TIMER", "DATE", m_nDate);
    m_Ini.SetProfileInt("TIMER", "HOUR", m_nHour);
    m_Ini.SetProfileInt("TIMER", "WEATHER", m_nWeather);
}

void CEbenezerDlg::UserInOutForMe(CUser * pSendUser) {
    int      send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0, prev_index = 0;
    C3DMap * pMap = NULL;
    int      region_x = -1, region_z = -1, user_count = 0, uid = -1;
    char     buff[16384], send_buff[49152];
    memset(buff, NULL, 16384);
    memset(send_buff, NULL, 49152);

    if (!pSendUser) {
        return;
    }
    if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) {
        return;
    }
    pMap = (C3DMap *)m_ZoneArray[pSendUser->m_iZoneIndex];
    if (!pMap) {
        return;
    }

    send_index = 3; // packet command 와 user_count 를 나중에 셋팅한다...
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ; // CENTER
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH WEST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ - 1; // NORTH
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH EAST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ; // WEST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ; // EAST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH WEST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 16384);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH EAST
    buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
    prev_index = buff_index + send_index;
    if (prev_index >= 49152) {
        TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
        return;
    }
    SetString(send_buff, buff, buff_index, send_index);

    int temp_index = 0;
    SetByte(send_buff, WIZ_REQ_USERIN, temp_index);
    SetShort(send_buff, t_count, temp_index);

    pSendUser->SendCompressingPacket(send_buff, send_index);
}

void CEbenezerDlg::RegionUserInOutForMe(CUser * pSendUser) {
    int      send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0;
    C3DMap * pMap = NULL;
    int      region_x = -1, region_z = -1, user_count = 0, uid_sendindex = 0;
    char     uid_buff[2048], send_buff[16384];
    memset(uid_buff, NULL, 2048);
    memset(send_buff, NULL, 16384);

    if (!pSendUser) {
        return;
    }
    if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) {
        return;
    }
    pMap = (C3DMap *)m_ZoneArray[pSendUser->m_iZoneIndex];
    if (!pMap) {
        return;
    }

    uid_sendindex = 3; // packet command 와 user_count 는 나중에 셋팅한다...

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ; // CENTER
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH WEST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ - 1; // NORTH
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH EAST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ; // WEST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ; // EAST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH WEST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);
    memset(uid_buff, NULL, 2048);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH EAST
    buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
    SetString(send_buff, uid_buff, buff_index, uid_sendindex);

    int temp_index = 0;
    SetByte(send_buff, WIZ_REGIONCHANGE, temp_index);
    SetShort(send_buff, user_count, temp_index);

    pSendUser->Send(send_buff, uid_sendindex);

    if (user_count > 500) {
        TRACE("Req UserIn: %d \n", user_count);
    }
}

int CEbenezerDlg::GetRegionUserIn(C3DMap * pMap, int region_x, int region_z, char * buff, int & t_count) {
    int        buff_index = 0, i = 0, j = 0;
    CUser *    pUser = NULL;
    CKnights * pKnights = NULL;
    int        user_count = 0, uid = -1, iLength = 0;

    if (!pMap) {
        return 0;
    }
    if (region_x < 0 || region_z < 0 || region_x > pMap->GetXRegionMax() || region_z > pMap->GetZRegionMax()) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.end();

    for (; Iter1 != Iter2; Iter1++) {
        uid = *((*Iter1).second);
        if (uid < 0) {
            continue;
        }
        pUser = (CUser *)m_Iocport.m_SockArray[uid];
        if (!pUser) {
            continue;
        }
        if (pUser->m_RegionX != region_x || pUser->m_RegionZ != region_z) {
            continue;
        }
        if (pUser->GetState() != STATE_GAMESTART) {
            continue;
        }

        SetShort(buff, pUser->GetSocketID(), buff_index);
        SetShort(buff, strlen(pUser->m_pUserData->m_id), buff_index);
        SetString(buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), buff_index);
        SetByte(buff, pUser->m_pUserData->m_bNation, buff_index);
        SetShort(buff, pUser->m_pUserData->m_bKnights, buff_index);
        // 666 work
        SetByte(buff, pUser->m_pUserData->m_bFame, buff_index);

        pKnights = m_KnightsArray.GetData(pUser->m_pUserData->m_bKnights);
        if (pUser->m_pUserData->m_bKnights == 0) {
            SetShort(buff, 0, buff_index);
            SetByte(buff, 0, buff_index);
            SetByte(buff, 0, buff_index);
        } else {
            //pKnights = m_pMain->m_KnightsArray.GetData( m_pUserData->m_bKnights );
            if (pKnights) {
                iLength = 0;
                iLength = strlen(pKnights->m_strName);
                SetShort(buff, (short)iLength, buff_index);
                SetString(buff, pKnights->m_strName, iLength, buff_index);
                SetByte(buff, pKnights->m_byGrade, buff_index);   // knights grade
                SetByte(buff, pKnights->m_byRanking, buff_index); // knights grade
                //TRACE("getregionuserin knights index = %d, kname=%s, name=%s\n" , iLength, pKnights->strName, pUser->m_pUserData->m_id);
            } else {
                SetShort(buff, 0, buff_index);
                SetByte(buff, 0, buff_index);
                SetByte(buff, 0, buff_index);
            }
        }

        SetByte(buff, pUser->m_pUserData->m_bLevel, buff_index);
        SetByte(buff, pUser->m_pUserData->m_bRace, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sClass, buff_index);
        SetShort(buff, (WORD)pUser->m_pUserData->m_curx * 10, buff_index);
        SetShort(buff, pUser->m_pUserData->m_curz * 10, buff_index);
        SetShort(buff, pUser->m_pUserData->m_cury * 10, buff_index);
        SetByte(buff, pUser->m_pUserData->m_bFace, buff_index);
        SetByte(buff, pUser->m_pUserData->m_bHairColor, buff_index);
        SetByte(buff, pUser->m_bResHpType, buff_index);
        // 비러머글 수능...
        SetByte(buff, pUser->m_bAbnormalType, buff_index);
        //
        SetByte(buff, pUser->m_bNeedParty, buff_index);
        // 여기두 주석처리
        SetByte(buff, pUser->m_pUserData->m_bAuthority, buff_index);
        //
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[BREAST].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[BREAST].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[LEG].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[LEG].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[HEAD].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[HEAD].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[GLOVE].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[GLOVE].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[FOOT].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[FOOT].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[SHOULDER].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[SHOULDER].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[RIGHTHAND].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[RIGHTHAND].sDuration, buff_index);
        SetDWORD(buff, pUser->m_pUserData->m_sItemArray[LEFTHAND].nNum, buff_index);
        SetShort(buff, pUser->m_pUserData->m_sItemArray[LEFTHAND].sDuration, buff_index);
        t_count++;
    }

    LeaveCriticalSection(&g_region_critical);

    return buff_index;
}

int CEbenezerDlg::GetRegionUserList(C3DMap * pMap, int region_x, int region_z, char * buff, int & t_count) {
    int     buff_index = 0, i = 0;
    int     user_count = 0, uid = -1;
    CUser * pUser = NULL;

    if (!pMap) {
        return 0;
    }
    if (region_x < 0 || region_z < 0 || region_x > pMap->GetXRegionMax() || region_z > pMap->GetZRegionMax()) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.end();

    for (; Iter1 != Iter2; Iter1++) {
        uid = *((*Iter1).second);
        if (uid < 0) {
            continue;
        }
        pUser = (CUser *)m_Iocport.m_SockArray[uid];
        if (pUser && (pUser->GetState() == STATE_GAMESTART)) {
            SetShort(buff, pUser->GetSocketID(), buff_index);
            t_count++;
        }
    }

    LeaveCriticalSection(&g_region_critical);

    return buff_index;
}

void CEbenezerDlg::NpcInOutForMe(CUser * pSendUser) {
    int      send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0;
    C3DMap * pMap = NULL;
    int      region_x = -1, region_z = -1, npc_count = 0, nid = -1;
    char     buff[8192], send_buff[32768];
    memset(buff, NULL, 8192);
    memset(send_buff, NULL, 32768);

    if (!pSendUser) {
        return;
    }
    if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) {
        return;
    }
    pMap = (C3DMap *)m_ZoneArray[pSendUser->m_iZoneIndex];
    if (!pMap) {
        return;
    }

    send_index = 3; // packet command 와 user_count 를 나중에 셋팅한다...
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ; // CENTER
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH WEST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ - 1; // NORTH
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH EAST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ; // WEST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ; // EAST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH WEST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);
    memset(buff, NULL, 8192);
    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH EAST
    buff_index = GetRegionNpcIn(pMap, region_x, region_z, buff, t_count);
    SetString(send_buff, buff, buff_index, send_index);

    int temp_index = 0;
    SetByte(send_buff, WIZ_REQ_NPCIN, temp_index);
    SetShort(send_buff, t_count, temp_index);

    pSendUser->SendCompressingPacket(send_buff, send_index);
}

int CEbenezerDlg::GetRegionNpcIn(C3DMap * pMap, int region_x, int region_z, char * buff, int & t_count) {
    if (m_bPointCheckFlag == FALSE) {
        return 0; // 포인터 참조하면 안됨
    }
    int     buff_index = 0, i = 0, j = 0;
    CNpc *  pNpc = NULL;
    int     npc_count = 0, nid = -1;
    CString string;

    if (!pMap) {
        return 0;
    }
    if (region_x < 0 || region_z < 0 || region_x > pMap->GetXRegionMax() || region_z > pMap->GetZRegionMax()) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.end();

    //string.Format("---- GetRegionNpcIn , x=%d, z=%d ----\r\n", region_x, region_z);
    //EnterCriticalSection( &g_LogFile_critical );
    //m_RegionLogFile.Write( string, string.GetLength() );
    //LeaveCriticalSection( &g_LogFile_critical );

    for (; Iter1 != Iter2; Iter1++) {
        nid = *((*Iter1).second);
        if (nid < 0) {
            continue;
        }
        pNpc = (CNpc *)m_arNpcArray.GetData(nid);
        if (!pNpc) {
            continue;
        }
        if (pNpc->m_sRegion_X != region_x || pNpc->m_sRegion_Z != region_z) {
            continue;
        }
        SetShort(buff, pNpc->m_sNid, buff_index);
        SetShort(buff, pNpc->m_sPid, buff_index);
        SetByte(buff, pNpc->m_tNpcType, buff_index);
        SetDWORD(buff, pNpc->m_iSellingGroup, buff_index);
        SetShort(buff, pNpc->m_sSize, buff_index);
        SetDWORD(buff, pNpc->m_iWeapon_1, buff_index);
        SetDWORD(buff, pNpc->m_iWeapon_2, buff_index);
        SetShort(buff, strlen(pNpc->m_strName), buff_index);
        SetString(buff, pNpc->m_strName, strlen(pNpc->m_strName), buff_index);
        SetByte(buff, pNpc->m_byGroup, buff_index);
        SetByte(buff, pNpc->m_byLevel, buff_index);
        SetShort(buff, (WORD)pNpc->m_fCurX * 10, buff_index);
        SetShort(buff, (WORD)pNpc->m_fCurZ * 10, buff_index);
        SetShort(buff, (short)pNpc->m_fCurY * 10, buff_index);
        SetDWORD(buff, (int)pNpc->m_byGateOpen, buff_index);
        SetByte(buff, pNpc->m_byObjectType, buff_index);

        t_count++;
        //string.Format("nid=%d, name=%s, count=%d \r\n", pNpc->m_sNid, pNpc->m_strName, t_count);
        //EnterCriticalSection( &g_LogFile_critical );
        //m_RegionLogFile.Write( string, string.GetLength() );
        //LeaveCriticalSection( &g_LogFile_critical );

        //if( pNpc->m_sCurZone > 100 )
        //    TRACE("GetRegionNpcIn rx=%d, rz=%d, nid=%d, name=%s, count=%d\n", region_x, region_z, pNpc->m_sNid, pNpc->m_strName, t_count);
    }

    LeaveCriticalSection(&g_region_critical);

    return buff_index;
}

void CEbenezerDlg::RegionNpcInfoForMe(CUser * pSendUser, int nType) {
    int      send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0;
    C3DMap * pMap = NULL;
    int      region_x = -1, region_z = -1, npc_count = 0, nid_sendindex = 0;
    char     nid_buff[1024], send_buff[8192];
    memset(nid_buff, NULL, 1024);
    memset(send_buff, NULL, 8192);
    CString string;

    if (!pSendUser) {
        return;
    }
    if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) {
        return;
    }
    pMap = (C3DMap *)m_ZoneArray[pSendUser->m_iZoneIndex];
    if (!pMap) {
        return;
    }

    nid_sendindex = 3; // packet command 와 user_count 는 나중에 셋팅한다...

    char strLog[256];
    if (nType == 1) { // test
        memset(strLog, 0x00, 256);
        CTime t = CTime::GetCurrentTime();
        wsprintf(strLog, "**** RegionNpcInfoForMe start(%d:%d-%d) : name=%s, x=%d, z=%d **** \r\n", t.GetHour(),
                 t.GetMinute(), t.GetSecond(), pSendUser->m_pUserData->m_id, pSendUser->m_RegionX,
                 pSendUser->m_RegionZ);
        EnterCriticalSection(&g_LogFile_critical);
        m_RegionLogFile.Write(strLog, strlen(strLog));
        LeaveCriticalSection(&g_LogFile_critical);
        //TRACE(strLog);
    }

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ; // CENTER
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH WEST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ - 1; // NORTH
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ - 1; // NORTH EAST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ; // WEST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ; // EAST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX - 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH WEST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);
    memset(nid_buff, NULL, 1024);

    region_x = pSendUser->m_RegionX + 1;
    region_z = pSendUser->m_RegionZ + 1; // SOUTH EAST
    buff_index = GetRegionNpcList(pMap, region_x, region_z, nid_buff, npc_count, nType);
    SetString(send_buff, nid_buff, buff_index, nid_sendindex);

    int temp_index = 0;
    if (nType == 1) { // test
        SetByte(send_buff, WIZ_TEST_PACKET, temp_index);
        memset(strLog, 0x00, 256);
        wsprintf(strLog, "**** RegionNpcInfoForMe end : name=%s, x=%d, z=%d, count **** \r\n",
                 pSendUser->m_pUserData->m_id, pSendUser->m_RegionX, pSendUser->m_RegionZ, npc_count);
        EnterCriticalSection(&g_LogFile_critical);
        m_RegionLogFile.Write(strLog, strlen(strLog));
        LeaveCriticalSection(&g_LogFile_critical);
        //TRACE(strLog);
    } else {
        SetByte(send_buff, WIZ_NPC_REGION, temp_index);
    }

    SetShort(send_buff, npc_count, temp_index);

    pSendUser->Send(send_buff, nid_sendindex);

    if (npc_count > 500) {
        TRACE("Req Npc In: %d \n", npc_count);
    }
}

int CEbenezerDlg::GetRegionNpcList(C3DMap * pMap, int region_x, int region_z, char * nid_buff, int & t_count,
                                   int nType) {
    if (m_bPointCheckFlag == FALSE) {
        return 0; // 포인터 참조하면 안됨
    }

    int    buff_index = 0, i = 0;
    int    npc_count = 0, nid = -1;
    CNpc * pNpc = NULL;
    char   strLog[1024];
    memset(strLog, 0x00, 1024);

    if (!pMap) {
        return 0;
    }
    if (region_x < 0 || region_z < 0 || region_x > pMap->GetXRegionMax() || region_z > pMap->GetZRegionMax()) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);

    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;

    Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.begin();
    Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.end();

    if (nType == 1) {
        wsprintf(strLog, "++++ GetRegionNpcList , x=%d, z=%d ++++\r\n", region_x, region_z);
        EnterCriticalSection(&g_LogFile_critical);
        m_RegionLogFile.Write(strLog, strlen(strLog));
        LeaveCriticalSection(&g_LogFile_critical);
        memset(strLog, 0x00, 1024);
    }

    for (; Iter1 != Iter2; Iter1++) {
        nid = *((*Iter1).second);
        if (nid < 0) {
            continue;
        }
        pNpc = m_arNpcArray.GetData(nid);
        //if( pNpc && (pNpc->m_NpcState == NPC_LIVE ) ) {  // 수정할 것,,
        if (pNpc) {
            SetShort(nid_buff, pNpc->m_sNid, buff_index);
            t_count++;
            if (nType == 1) {
                wsprintf(strLog, "%d   ", pNpc->m_sNid);
                EnterCriticalSection(&g_LogFile_critical);
                m_RegionLogFile.Write(strLog, strlen(strLog));
                LeaveCriticalSection(&g_LogFile_critical);
            }
        } else {
            if (nType == 1) {
                wsprintf(strLog, "%d(Err)   ", nid);
                EnterCriticalSection(&g_LogFile_critical);
                m_RegionLogFile.Write(strLog, strlen(strLog));
                LeaveCriticalSection(&g_LogFile_critical);
            }
        }
    }

    if (nType == 1) {
        wsprintf(strLog, "\r\n");
        EnterCriticalSection(&g_LogFile_critical);
        m_RegionLogFile.Write(strLog, strlen(strLog));
        LeaveCriticalSection(&g_LogFile_critical);
    }

    LeaveCriticalSection(&g_region_critical);

    return buff_index;
}

int CEbenezerDlg::GetZoneIndex(int zonenumber) {
    int t_count = m_ZoneArray.size();
    for (int i = 0; i < t_count; i++) {
        if (m_ZoneArray[i] && (zonenumber == m_ZoneArray[i]->m_nZoneNumber)) {
            return i;
        }
    }

    return -1;
}

BOOL CEbenezerDlg::PreTranslateMessage(MSG * pMsg) {
    char buff[1024];
    memset(buff, 0x00, 1024);
    char chatstr[256], killstr[256];
    memset(chatstr, 0x00, 256);
    memset(killstr, 0x00, 256);
    int                chatlen = 0, buffindex = 0;
    _ZONE_SERVERINFO * pInfo = NULL;

    std::string buff2;
    //
    BOOL permanent_off = FALSE;
    //
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
            m_AnnounceEdit.GetWindowText(chatstr, 256);
            UpdateData(TRUE);
            chatlen = strlen(chatstr);
            if (chatlen == 0) {
                return TRUE;
            }

            m_AnnounceEdit.SetWindowText("");
            UpdateData(FALSE);

            if (_strnicmp("/kill", chatstr, 5) == 0) {
                strcpy(killstr, chatstr + 6);
                KillUser(killstr);
                return TRUE;
            }
            if (_strnicmp("/Open", chatstr, 5) == 0) {
                BattleZoneOpen(BATTLEZONE_OPEN);
                return TRUE;
            }
            if (_strnicmp("/snowopen", chatstr, 9) == 0) {
                BattleZoneOpen(SNOW_BATTLEZONE_OPEN);
                return TRUE;
            }
            if (_strnicmp("/Close", chatstr, 6) == 0) {
                m_byBanishFlag = 1;
                //WithdrawUserOut();
                return TRUE;
            }
            if (_strnicmp("/down", chatstr, 5) == 0) {
                g_serverdown_flag = TRUE;
                ::SuspendThread(m_Iocport.m_hAcceptThread);
                KickOutAllUsers();
                return TRUE;
            }
            if (_strnicmp("/discount", chatstr, 9) == 0) {
                m_sDiscount = 1;
                return TRUE;
            }
            if (_strnicmp("/alldiscount", chatstr, 12) == 0) {
                m_sDiscount = 2;
                return TRUE;
            }
            if (_strnicmp("/undiscount", chatstr, 11) == 0) {
                m_sDiscount = 0;
                return TRUE;
            }
            // 비러머글 남는 공지 --;
            if (_strnicmp("/permanent", chatstr, 10) == 0) {
                m_bPermanentChatMode = TRUE;
                m_bPermanentChatFlag = TRUE;
                return TRUE;
            }
            if (_strnicmp("/captain", chatstr, 8) == 0) {
                LoadKnightsRankTable(); // captain
                return TRUE;
            }

            if (_strnicmp("/offpermanent", chatstr, 13) == 0) {
                m_bPermanentChatMode = FALSE;
                m_bPermanentChatFlag = FALSE;
                permanent_off = TRUE;
                //                return TRUE;    //이것은 고의적으로 TRUE를 뺐었음
            }
            //

            // 갓댐 산타!!! >.<
            if (_strnicmp("/santa", chatstr, 6) == 0) {
                m_bSanta = TRUE; // Make Motherfucking Santa Claus FLY!!!
                return TRUE;
            }

            if (_strnicmp("/offsanta", chatstr, 9) == 0) {
                m_bSanta = FALSE; // SHOOT DOWN Motherfucking Santa Claus!!!
                return TRUE;
            }
            //

            char finalstr[256];
            memset(finalstr, NULL, 256);
            //            sprintf( finalstr, "#### 공지 : %s ####", chatstr );

            // 비러머글 남는 공지
            if (m_bPermanentChatFlag) {
                sprintf(finalstr, "- %s -", chatstr);
            } else {
                //sprintf( finalstr, "#### 공지 : %s ####", chatstr );
                ::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff2);
                sprintf(finalstr, buff2.c_str(), chatstr);
            }
            //
            SetByte(buff, WIZ_CHAT, buffindex);
            //            SetByte( buff, PUBLIC_CHAT, buffindex );

            // 비러머글 남는 공지
            if (permanent_off) {
                SetByte(buff, END_PERMANENT_CHAT, buffindex);
            } else if (!m_bPermanentChatFlag) {
                SetByte(buff, PUBLIC_CHAT, buffindex);
            } else {
                SetByte(buff, PERMANENT_CHAT, buffindex);
                strcpy(m_strPermanentChat, finalstr);
                m_bPermanentChatFlag = FALSE;
            }
            //
            SetByte(buff, 0x01, buffindex); // nation
            SetShort(buff, -1, buffindex);  // sid
            SetShort(buff, strlen(finalstr), buffindex);
            SetString(buff, finalstr, strlen(finalstr), buffindex);
            Send_All(buff, buffindex);

            buffindex = 0;
            memset(buff, 0x00, 1024);
            SetByte(buff, STS_CHAT, buffindex);
            SetShort(buff, strlen(finalstr), buffindex);
            SetString(buff, finalstr, strlen(finalstr), buffindex);

            map<int, _ZONE_SERVERINFO *>::iterator Iter1;
            map<int, _ZONE_SERVERINFO *>::iterator Iter2;

            Iter1 = m_ServerArray.m_UserTypeMap.begin();
            Iter2 = m_ServerArray.m_UserTypeMap.end();

            for (; Iter1 != Iter2; Iter1++) {
                pInfo = (*Iter1).second;
                if (!pInfo) {
                    continue;
                }
                if (pInfo->sServerNo != m_nServerNo) {
                    m_pUdpSocket->SendUDPPacket(pInfo->strServerIP, buff, buffindex);
                }
            }

            return TRUE;
        }
        if (pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        }
    }
    if (pMsg->wParam == VK_F8) {
        SyncTest(1);
    } else if (pMsg->wParam == VK_F9) {
        SyncTest(2);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

bool CEbenezerDlg::LoadNoticeData() {
    memset(&m_ppNotice, 0, sizeof(m_ppNotice));
    if (!fs::exists("Notice.txt")) {
        // Create an empty file, so that people know of its existence
        std::ofstream fileEmpty("Notice.txt");
        return true;
    }

    std::ifstream inFile("Notice.txt");
    if (!inFile) {
        AfxMessageBox("ERROR: Notice.txt could not be opened.\n");
        return false;
    }

    size_t      iCurLine = 0;
    std::string szCurLine;
    while (std::getline(inFile, szCurLine)) {
        if (iCurLine > 19) {
            memset(&m_ppNotice, 0, sizeof(m_ppNotice));
            AfxMessageBox("ERROR: Notice.txt exceeds max 20 lines.\n");
            return false;
        }

        if (szCurLine.length() > 128) {
            memset(&m_ppNotice, 0, sizeof(m_ppNotice));
            AfxMessageBox(std::string("ERROR: Notice.txt line [" + std::to_string(iCurLine + 1) +
                                      "] exceeds the limit of 128 characters per line.\n")
                              .c_str());
            return false;
        }

        strcpy_s(m_ppNotice[iCurLine++], szCurLine.c_str());
    }

    return true;
}

void CEbenezerDlg::SyncTest(int nType) {
    char strPath[100];
    memset(strPath, 0x00, 100);
    if (nType == 1) {
        strcpy(strPath, "c:\\userlist.txt");
    } else if (nType == 2) {
        strcpy(strPath, "c:\\npclist.txt");
    }
    FILE * stream = fopen(strPath, "w");

    int         size = m_arNpcArray.GetSize();
    CNpc *      pNpc = NULL;
    CAISocket * pSocket = NULL;
    int         count = 0;

    int  len = 0;
    char pBuf[256];
    ::ZeroMemory(pBuf, sizeof(pBuf));
    int iErrorCode = 0, send_index = 0;

    SetByte(pBuf, AG_CHECK_ALIVE_REQ, len);
    int total_user = 0, total_mon = 0;

    for (int i = 0; i < m_ZoneArray.size(); i++) {
        pSocket = m_AISocketArray.GetData(m_ZoneArray[i]->m_nZoneNumber);
        if (!pSocket) {
            continue;
        } else {
            size = pSocket->Send(pBuf, len);
            fprintf(stream, "size=%d, zone=%d, number=%d\n", size, pSocket->m_iZoneNum, m_ZoneArray[i]->m_nZoneNumber);

            //return;
        }
    }

    /*    for(int i=0; i<size; i++)
    {
        pNpc = m_arNpcArray.GetData(i+NPC_BAND);
        if(pNpc == NULL)
        {
            fprintf(stream, "Fail -> nid=%d\n", i+NPC_BAND);
            TRACE("##### allNpcInfo Fail = %d\n", i);
            continue;
        }

        fprintf(stream, "nid=%d, name=%s, zone=%d, x=%.2f, z=%.2f, rx=%d, rz=%d\n", pNpc->m_sNid, pNpc->m_strName, pNpc->m_sCurZone, pNpc->m_fCurX, pNpc->m_fCurZ, pNpc->m_sRegion_X, pNpc->m_sRegion_X);
    }    */

    fprintf(stream, "*****   Region List  *****\n");

    C3DMap * pMap = NULL;

    for (int k = 0; k < m_ZoneArray.size(); k++) {
        //if( k != 2 ) continue;        // 201 존만 체크..
        pMap = m_ZoneArray[k];
        if (pMap == NULL) {
            continue;
        }
        for (int i = 0; i < pMap->GetXRegionMax(); i++) {
            for (int j = 0; j < pMap->GetZRegionMax(); j++) {
                EnterCriticalSection(&g_region_critical);
                total_user = pMap->m_ppRegion[i][j].m_RegionUserArray.GetSize();
                total_mon = pMap->m_ppRegion[i][j].m_RegionNpcArray.GetSize();
                LeaveCriticalSection(&g_region_critical);

                if (total_user > 0 || total_mon > 0) {
                    fprintf(stream, "rx=%d, rz=%d, user=%d, monster=%d\n", i, j, total_user, total_mon);
                    SyncRegionTest(pMap, i, j, stream, nType);
                }
            }
        }
    }

    fclose(stream);
}

void CEbenezerDlg::SyncRegionTest(C3DMap * pMap, int rx, int rz, FILE * pfile, int nType) {
    fprintf(pfile, "ZONE=%d, [%d,%d] : ", pMap->m_nZoneNumber, rx, rz);
    int nid = 0;
    EnterCriticalSection(&g_region_critical);
    map<int, int *>::iterator Iter1;
    map<int, int *>::iterator Iter2;
    CUser *                   pUser = NULL;
    CNpc *                    pNpc = NULL;

    if (nType == 2) {
        Iter1 = pMap->m_ppRegion[rx][rz].m_RegionNpcArray.m_UserTypeMap.begin();
        Iter2 = pMap->m_ppRegion[rx][rz].m_RegionNpcArray.m_UserTypeMap.end();
    } else if (nType == 1) {
        Iter1 = pMap->m_ppRegion[rx][rz].m_RegionUserArray.m_UserTypeMap.begin();
        Iter2 = pMap->m_ppRegion[rx][rz].m_RegionUserArray.m_UserTypeMap.end();
    }

    for (; Iter1 != Iter2; Iter1++) {
        nid = *((*Iter1).second);
        if (nType == 1) {
            pUser = (CUser *)m_Iocport.m_SockArray[nid];
            if (!pUser) {
                TRACE("SyncRegionTest : nid fail = %d\n", nid);
                fprintf(pfile, "%d(fail)    ", nid);
                continue;
            }
            fprintf(pfile, "%d(%d,%d)    ", nid, (int)pUser->m_pUserData->m_curx, (int)pUser->m_pUserData->m_curz);
        } else if (nType == 2) {
            pNpc = m_arNpcArray.GetData(nid);
            if (!pNpc) {
                TRACE("SyncRegionTest : nid fail = %d\n", nid);
                fprintf(pfile, "%d(fail)    ", nid);
                continue;
            }
            fprintf(pfile, "%d(%d,%d)    ", nid, (int)pNpc->m_fCurX, (int)pNpc->m_fCurZ);
        }
    }
    fprintf(pfile, "\n");
    LeaveCriticalSection(&g_region_critical);
}

void CEbenezerDlg::SendAllUserInfo() {
    CUser * pUser = NULL;

    int  send_index = 0;
    char send_buff[2048];
    ::ZeroMemory(send_buff, sizeof(send_buff));

    SetByte(send_buff, AG_SERVER_INFO, send_index);
    SetByte(send_buff, SERVER_INFO_START, send_index);
    Send_AIServer(1000, send_buff, send_index);

    int count = 0;
    send_index = 2;
    ::ZeroMemory(send_buff, sizeof(send_buff));
    int send_count = 0;
    int send_tot = 0;
    int tot = 20;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (pUser) {
            pUser->SendUserInfo(send_buff, send_index);
            count++;
            if (count == tot) {
                SetByte(send_buff, AG_USER_INFO_ALL, send_count);
                SetByte(send_buff, (BYTE)count, send_count);
                m_CompCount++;
                memset(m_CompBuf, 0x00, 10240);
                ::CopyMemory(m_CompBuf, send_buff, send_index);
                m_iCompIndex = send_index;
                SendCompressedData();
                send_index = 2;
                send_count = 0;
                count = 0;
                send_tot++;
                //TRACE("AllNpcInfo - send_count=%d, count=%d\n", send_tot, count);
                ::ZeroMemory(send_buff, sizeof(send_buff));
                //Sleep(320);
            }
        }
    }

    if (count != 0 && count < (tot - 1)) {
        send_count = 0;
        SetByte(send_buff, AG_USER_INFO_ALL, send_count);
        SetByte(send_buff, (BYTE)count, send_count);
        Send_AIServer(1000, send_buff, send_index);
        send_tot++;
        //TRACE("AllNpcInfo - send_count=%d, count=%d\n", send_tot, count);
        //Sleep(1);
    }

    // 파티에 대한 정보도 보내도록 한다....
    _PARTY_GROUP * pParty = NULL;

    EnterCriticalSection(&g_region_critical);

    for (int i = 0; i < m_PartyArray.GetSize(); i++) {
        pParty = m_PartyArray.GetData(i);
        if (!pParty) {
            return;
        }
        send_index = 0;
        ::ZeroMemory(send_buff, sizeof(send_buff));
        SetByte(send_buff, AG_PARTY_INFO_ALL, send_index);
        SetShort(send_buff, i, send_index); // 파티 번호
        //if( i == pParty->wIndex )
        for (int j = 0; j < 8; j++) {
            SetShort(send_buff, pParty->uid[j], send_index); // 유저 번호
            //SetShort(send_buff, pParty->sHp[j], send_index );                // HP
            //SetByte(send_buff, pParty->bLevel[j], send_index );                // Level
            //SetShort(send_buff, pParty->sClass[j], send_index );            // Class
        }

        Send_AIServer(1000, send_buff, send_index);
    }

    LeaveCriticalSection(&g_region_critical);

    send_index = 0;
    ::ZeroMemory(send_buff, sizeof(send_buff));
    SetByte(send_buff, AG_SERVER_INFO, send_index);
    SetByte(send_buff, SERVER_INFO_END, send_index);
    Send_AIServer(1000, send_buff, send_index);

    TRACE("** SendAllUserInfo() **\n");
}

void CEbenezerDlg::SendCompressedData() {
    if (!m_CompCount || m_CompCount < 0 || !m_iCompIndex || m_iCompIndex < 0) {
        m_CompCount = 0;
        m_iCompIndex = 0;
        return;
    }

    m_CompMng.PreCompressWork(m_CompBuf, m_iCompIndex);
    m_CompMng.Compress();

    int  send_index = 0;
    char send_buff[2048];
    ::ZeroMemory(send_buff, sizeof(send_buff));
    SetByte(send_buff, AG_COMPRESSED_DATA, send_index);
    SetShort(send_buff, (short)m_CompMng.m_nOutputBufferCurPos, send_index);
    SetShort(send_buff, (short)m_CompMng.m_nOrgDataLength, send_index);
    SetDWORD(send_buff, m_CompMng.m_dwCrc, send_index);
    SetShort(send_buff, (short)m_CompCount, send_index);
    SetString(send_buff, m_CompMng.m_pOutputBuffer, m_CompMng.m_nOutputBufferCurPos, send_index);

    if (!m_CompMng.m_pOutputBuffer) {
        m_CompCount = 0;
        m_iCompIndex = 0;
        m_CompMng.Initialize();
        return;
    }

    Send_AIServer(1000, send_buff, send_index);

    m_CompCount = 0;
    m_iCompIndex = 0;
    m_CompMng.Initialize();
}

// sungyong 2002. 05. 23
void CEbenezerDlg::DeleteAllNpcList(int flag) {
    if (m_bServerCheckFlag == FALSE) {
        return;
    }
    if (m_bPointCheckFlag == TRUE) {
        m_bPointCheckFlag = FALSE;
        TRACE("*** Point 참조 하면 안되여 *** \n");
        return;
    }

    CString logstr;
    logstr.Format("[Monster Point Delete]");
    m_StatusList.AddString(logstr);

    TRACE("*** DeleteAllNpcList - Start *** \n");

    CUser *  pUser = NULL;
    C3DMap * pMap = NULL;

    // region Npc Array Delete
    for (int i = 0; i < m_ZoneArray.size(); i++) {
        pMap = (C3DMap *)m_ZoneArray[i];
        if (!pMap) {
            continue;
        }

        for (i = 0; i < pMap->GetXRegionMax(); i++) { // TODO: Check if this is not a bug that resets parent loop.
            for (int j = 0; j < pMap->GetZRegionMax(); j++) {
                if (!pMap->m_ppRegion[i][j].m_RegionNpcArray.IsEmpty()) {
                    pMap->m_ppRegion[i][j].m_RegionNpcArray.DeleteAllData();
                }
            }
        }
    }

    // Npc Array Delete
    if (!m_arNpcArray.IsEmpty()) {
        m_arNpcArray.DeleteAllData();
    }

    m_bServerCheckFlag = FALSE;

    TRACE("*** DeleteAllNpcList - End *** \n");
}
// ~sungyong 2002. 05. 23

void CEbenezerDlg::KillUser(const char * strbuff) {
    if (strlen(strbuff) <= 0 || strlen(strbuff) > MAX_ID_SIZE) {
        return;
    }

    CUser * pUser = NULL;
    pUser = GetUserPtr(strbuff, 0x02);
    if (!pUser) {
        return;
    }

    pUser->Close();
}

CNpc * CEbenezerDlg::GetNpcPtr(int sid, int cur_zone) {
    if (m_bPointCheckFlag == FALSE) {
        return NULL; // 포인터 참조하면 안됨
    }

    CNpc * pNpc = NULL;

    int nSize = m_arNpcArray.GetSize();

    for (int i = 0; i < nSize; i++) {
        pNpc = m_arNpcArray.GetData(i + NPC_BAND);
        if (!pNpc) {
            continue;
        }
        if (pNpc->m_sCurZone != cur_zone) {
            continue;
        }

        if (pNpc->m_sPid == sid) {
            return pNpc;
        }
    }

    return NULL;
}

void CEbenezerDlg::WithdrawUserOut() {
    int      zoneindex = -1;
    CUser *  pUser = NULL;
    C3DMap * pMap = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (pUser && pUser->m_pUserData->m_bZone == pUser->m_pUserData->m_bNation) {
            zoneindex = GetZoneIndex(pUser->m_pUserData->m_bNation);
            if (zoneindex < 0) {
                continue;
            }
            pMap = m_ZoneArray[zoneindex];
            if (!pMap) {
                continue;
            }
            pUser->ZoneChange(pMap->m_nZoneNumber, pMap->m_fInitX, pMap->m_fInitZ);
        }
    }
}

void CEbenezerDlg::AliveUserCheck() {
    CUser * pUser = NULL;
    float   currenttime = TimeGet();

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->GetState() != STATE_GAMESTART) {
            continue;
        }
        /*
        if( (currenttime - pUser->m_fHPLastTime) > 300 )
            pUser->Close();
*/
        for (int k = 0; k < MAX_TYPE3_REPEAT; k++) {
            if ((currenttime - pUser->m_fHPLastTime[k]) > 300) {
                pUser->Close();
                break;
            }
        }
    }
}
/////// BATTLEZONE RELATED by Yookozuna 2002.6.18 /////////////////
void CEbenezerDlg::BattleZoneOpenTimer() {
    CTime cur = CTime::GetCurrentTime();

    // sungyong modify
    int  nWeek = cur.GetDayOfWeek();
    int  nTime = cur.GetHour();
    char send_buff[128];
    memset(send_buff, 0x00, 128);
    int     send_index = 0, loser_nation = 0, snow_battle = 0;
    CUser * pKarusUser = NULL;
    CUser * pElmoUser = NULL;

    /*    if( m_byBattleOpen == NO_BATTLE )    {    // When Battlezone is closed, open it!
        if( nWeek == m_nBattleZoneOpenWeek && nTime == m_nBattleZoneOpenHourStart )    {    // 수요일, 20시에 전쟁존 open
            TRACE("전쟁 자동 시작 - week=%d, time=%d\n", nWeek, nTime);
            BattleZoneOpen(BATTLEZONE_OPEN);
//            KickOutZoneUsers(ZONE_FRONTIER);    // Kick out users in frontier zone.
        }
    }
    else {      // When Battlezone is open, close it!
        if( nWeek == (m_nBattleZoneOpenWeek+1) && nTime == m_nBattleZoneOpenHourEnd )    {    // 목요일, 0시에 전쟁존 close
            TRACE("전쟁 자동 종료 - week=%d, time=%d\n", nWeek, nTime);
            m_byBanishFlag = 1;
        }
    }    */

    if (m_byBattleOpen == NATION_BATTLE) {
        BattleZoneCurrentUsers();
    }

    if (m_byBanishFlag == 1) {
        if (m_sBanishDelay == 0) {
            m_byBattleOpen = NO_BATTLE;
            m_byKarusOpenFlag = 0;   // 카루스 땅으로 넘어갈 수 없도록
            m_byElmoradOpenFlag = 0; // 엘모 땅으로 넘어갈 수 없도록
            memset(m_strKarusCaptain, 0x00, MAX_ID_SIZE + 1);
            memset(m_strElmoradCaptain, 0x00, MAX_ID_SIZE + 1);
            TRACE("전쟁 종료 0단계\n");
            if (m_nServerNo == KARUS) {
                memset(send_buff, 0x00, 128);
                send_index = 0;
                SetByte(send_buff, UDP_BATTLE_EVENT_PACKET, send_index);
                SetByte(send_buff, BATTLE_EVENT_KILL_USER, send_index);
                SetByte(send_buff, 1, send_index); // karus의 정보 전송
                SetShort(send_buff, m_sKarusDead, send_index);
                SetShort(send_buff, m_sElmoradDead, send_index);
                Send_UDP_All(send_buff, send_index);
            }
        }

        m_sBanishDelay++;

        if (m_sBanishDelay == 3) {
            if (m_byOldBattleOpen == SNOW_BATTLE) { // 눈싸움 전쟁
                if (m_sKarusDead > m_sElmoradDead) {
                    m_bVictory = ELMORAD;
                    loser_nation = KARUS;
                } else if (m_sKarusDead < m_sElmoradDead) {
                    m_bVictory = KARUS;
                    loser_nation = ELMORAD;
                } else if (m_sKarusDead == m_sElmoradDead) {
                    m_bVictory = 0;
                }
            }

            if (m_bVictory == 0) {
                BattleZoneOpen(BATTLEZONE_CLOSE);
            } else if (m_bVictory) {
                if (m_bVictory == KARUS) {
                    loser_nation = ELMORAD;
                } else if (m_bVictory == ELMORAD) {
                    loser_nation = KARUS;
                }
                Announcement(DECLARE_WINNER, m_bVictory);
                Announcement(DECLARE_LOSER, loser_nation);
            }
            TRACE("전쟁 종료 1단계, m_bVictory=%d\n", m_bVictory);
        } else if (m_sBanishDelay == 8) {
            Announcement(DECLARE_BAN);
        } else if (m_sBanishDelay == 10) {
            TRACE("전쟁 종료 2단계 - 모든 유저 자기 국가로 가 \n");
            BanishLosers();
        } else if (m_sBanishDelay == 20) {
            TRACE("전쟁 종료 3단계 - 초기화 해주세여 \n");
            SetByte(send_buff, AG_BATTLE_EVENT, send_index);
            SetByte(send_buff, BATTLE_EVENT_OPEN, send_index);
            SetByte(send_buff, BATTLEZONE_CLOSE, send_index);
            Send_AIServer(1000, send_buff, send_index);
            ResetBattleZone();
        }
    }

    // ~
}

void CEbenezerDlg::BattleZoneOpen(int nType) {
    int  send_index = 0;
    char send_buff[1024];
    memset(send_buff, NULL, 1024);
    char strLogFile[100];
    memset(strLogFile, NULL, 100);
    CTime time = CTime::GetCurrentTime();

    if (nType == BATTLEZONE_OPEN) { // Open battlezone.
        m_byBattleOpen = NATION_BATTLE;
        m_byOldBattleOpen = NATION_BATTLE;
    } else if (nType == SNOW_BATTLEZONE_OPEN) { // Open snow battlezone.
        m_byBattleOpen = SNOW_BATTLE;
        m_byOldBattleOpen = SNOW_BATTLE;
        wsprintf(strLogFile, "EventLog-%d-%d-%d.txt", time.GetYear(), time.GetMonth(), time.GetDay());
        m_EvnetLogFile.Open(strLogFile,
                            CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone);
        m_EvnetLogFile.SeekToEnd();
    } else if (nType == BATTLEZONE_CLOSE) { // battle close
        m_byBattleOpen = NO_BATTLE;
        Announcement(BATTLEZONE_CLOSE);
    } else {
        return;
    }

    Announcement(nType); // Send an announcement out that the battlezone is open/closed.
                         //
    KickOutZoneUsers(ZONE_FRONTIER);
    //
    memset(send_buff, NULL, 1024);
    SetByte(send_buff, AG_BATTLE_EVENT, send_index); // Send packet to AI server.
    SetByte(send_buff, BATTLE_EVENT_OPEN, send_index);
    SetByte(send_buff, nType, send_index);
    Send_AIServer(1000, send_buff, send_index);
}

void CEbenezerDlg::BattleZoneVictoryCheck() {
    if (m_bKarusFlag >= NUM_FLAG_VICTORY) { // WINNER DECLARATION PROCEDURE !!!
        m_bVictory = KARUS;
    } else if (m_bElmoradFlag >= NUM_FLAG_VICTORY) {
        m_bVictory = ELMORAD;
    } else {
        return;
    }

    m_bBanishDelayStart = TimeGet();

    Announcement(DECLARE_WINNER);

    for (int i = 0; i < MAX_USER; i++) {            // GOLD DISTRIBUTION PROCEDURE FOR WINNERS !!!
        CUser * pTUser = NULL;                      // Pointer initialization!
        pTUser = (CUser *)m_Iocport.m_SockArray[i]; // Get target info.
        if (!pTUser) {
            continue;
        }

        if (pTUser->m_pUserData->m_bNation == m_bVictory) {
            if (pTUser->m_pUserData->m_bZone == pTUser->m_pUserData->m_bNation) { // Zone Check!
                pTUser->m_pUserData->m_iGold += AWARD_GOLD;                       // Target is in the area.
            }
        }
    }
}

void CEbenezerDlg::BanishLosers() {
    int zoneindex = -1, send_index = 0;
    ;
    CUser *  pTUser = NULL; // Pointer initialization!
    C3DMap * pMap = NULL;
    char     send_buff[256];
    memset(send_buff, 0x00, 256);

    for (int i = 0; i < MAX_USER; i++) {            // EVACUATION PROCEDURE FOR LOSERS !!!
        pTUser = (CUser *)m_Iocport.m_SockArray[i]; // Get target info.
        if (!pTUser) {
            continue;
        }
        if (pTUser->m_pUserData->m_bFame == COMMAND_CAPTAIN) {
            pTUser->m_pUserData->m_bFame = CHIEF;
            send_index = 0;
            memset(send_buff, 0x00, 256);
            SetByte(send_buff, WIZ_AUTHORITY_CHANGE, send_index);
            SetByte(send_buff, COMMAND_AUTHORITY, send_index);
            SetShort(send_buff, pTUser->GetSocketID(), send_index);
            SetByte(send_buff, pTUser->m_pUserData->m_bFame, send_index);
            pTUser->Send(send_buff, send_index);
        }
        if (pTUser->m_pUserData->m_bZone != pTUser->m_pUserData->m_bNation) {
            pTUser->KickOutZoneUser(TRUE);
        }
    }
}

void CEbenezerDlg::ResetBattleZone() {
    if (m_byOldBattleOpen == SNOW_BATTLE) {
        if (m_EvnetLogFile.m_hFile != CFile::hFileNull) {
            m_EvnetLogFile.Close();
        }
        TRACE("Event Log close\n");
    }

    m_bVictory = 0;
    m_bBanishDelayStart = 0;
    m_byBanishFlag = 0;
    m_sBanishDelay = 0;
    m_bKarusFlag = 0, m_bElmoradFlag = 0;
    m_byKarusOpenFlag = m_byElmoradOpenFlag = 0;
    m_byBattleOpen = NO_BATTLE;
    m_byOldBattleOpen = NO_BATTLE;
    m_sKarusDead = m_sElmoradDead = 0;
    m_byBattleSave = 0;
    m_sKarusCount = 0;
    m_sElmoradCount = 0;
    // REMEMBER TO MAKE ALL FLAGS AND LEVERS NEUTRAL AGAIN!!!!!!!!!!
}

void CEbenezerDlg::Announcement(BYTE type, int nation, int chat_type) {
    int send_index = 0;

    char chatstr[1024];
    memset(chatstr, NULL, 1024);
    char finalstr[1024];
    memset(finalstr, NULL, 1024);
    char send_buff[1024];
    memset(send_buff, NULL, 1024);

    std::string buff;
    std::string buff2;

    switch (type) {
    case BATTLEZONE_OPEN:
        ::_LoadStringFromResource(IDP_BATTLEZONE_OPEN, buff);
        sprintf(chatstr, buff.c_str());
        break;
    case SNOW_BATTLEZONE_OPEN:
        ::_LoadStringFromResource(IDP_BATTLEZONE_OPEN, buff);
        sprintf(chatstr, buff.c_str());
        break;

    case DECLARE_WINNER:
        if (m_bVictory == KARUS) {
            ::_LoadStringFromResource(IDP_KARUS_VICTORY, buff);
            sprintf(chatstr, buff.c_str(), m_sElmoradDead, m_sKarusDead);
        } else if (m_bVictory == ELMORAD) {
            ::_LoadStringFromResource(IDP_ELMORAD_VICTORY, buff);
            sprintf(chatstr, buff.c_str(), m_sKarusDead, m_sElmoradDead);
        } else {
            return;
        }

        break;
    case DECLARE_LOSER:
        if (m_bVictory == KARUS) {
            ::_LoadStringFromResource(IDS_ELMORAD_LOSER, buff);
            sprintf(chatstr, buff.c_str(), m_sKarusDead, m_sElmoradDead);
        } else if (m_bVictory == ELMORAD) {
            ::_LoadStringFromResource(IDS_KARUS_LOSER, buff);
            sprintf(chatstr, buff.c_str(), m_sElmoradDead, m_sKarusDead);
        } else {
            return;
        }
        break;

    case DECLARE_BAN:
        ::_LoadStringFromResource(IDS_BANISH_USER, buff);
        sprintf(chatstr, buff.c_str());
        break;
    case BATTLEZONE_CLOSE:
        ::_LoadStringFromResource(IDS_BATTLE_CLOSE, buff);
        sprintf(chatstr, buff.c_str());
        break;
    case KARUS_CAPTAIN_NOTIFY:
        ::_LoadStringFromResource(IDS_KARUS_CAPTAIN, buff);
        sprintf(chatstr, buff.c_str(), m_strKarusCaptain);
        break;
    case ELMORAD_CAPTAIN_NOTIFY:
        ::_LoadStringFromResource(IDS_ELMO_CAPTAIN, buff);
        sprintf(chatstr, buff.c_str(), m_strElmoradCaptain);
        break;
    case KARUS_CAPTAIN_DEPRIVE_NOTIFY:
        ::_LoadStringFromResource(IDS_KARUS_CAPTAIN_DEPRIVE, buff);
        sprintf(chatstr, buff.c_str(), m_strKarusCaptain);
        break;
    case ELMORAD_CAPTAIN_DEPRIVE_NOTIFY:
        ::_LoadStringFromResource(IDS_ELMO_CAPTAIN_DEPRIVE, buff);
        sprintf(chatstr, buff.c_str(), m_strElmoradCaptain);
        break;
    }

    ::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff2);
    sprintf(finalstr, buff2.c_str(), chatstr);
    //sprintf( finalstr, "## 공지 : %s ##", chatstr );
    SetByte(send_buff, WIZ_CHAT, send_index);
    SetByte(send_buff, chat_type, send_index);
    SetByte(send_buff, 1, send_index);
    SetShort(send_buff, -1, send_index);
    SetShort(send_buff, strlen(finalstr), send_index);
    SetString(send_buff, finalstr, strlen(finalstr), send_index);

    CUser * pUser = NULL;
    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->GetState() == STATE_GAMESTART) {
            if (nation == 0) {
                pUser->Send(send_buff, send_index);
            } else if (nation == pUser->m_pUserData->m_bNation) {
                pUser->Send(send_buff, send_index);
            }
        }
    }
}

BOOL CEbenezerDlg::LoadHomeTable() {
    CHomeSet HomeSet;

    if (!HomeSet.Open()) {
        AfxMessageBox(_T("Home Data Open Fail!"));
        return FALSE;
    }
    if (HomeSet.IsBOF() || HomeSet.IsEOF()) {
        AfxMessageBox(_T("Home Data Empty!"));
        return FALSE;
    }

    HomeSet.MoveFirst();

    while (!HomeSet.IsEOF()) {
        _HOME_INFO * pHomeInfo = new _HOME_INFO;

        pHomeInfo->bNation = HomeSet.m_Nation;

        pHomeInfo->KarusZoneX = HomeSet.m_KarusZoneX;
        pHomeInfo->KarusZoneZ = HomeSet.m_KarusZoneZ;
        pHomeInfo->KarusZoneLX = HomeSet.m_KarusZoneLX;
        pHomeInfo->KarusZoneLZ = HomeSet.m_KarusZoneLZ;

        pHomeInfo->ElmoZoneX = HomeSet.m_ElmoZoneX;
        pHomeInfo->ElmoZoneZ = HomeSet.m_ElmoZoneZ;
        pHomeInfo->ElmoZoneLX = HomeSet.m_ElmoZoneLX;
        pHomeInfo->ElmoZoneLZ = HomeSet.m_ElmoZoneLZ;

        pHomeInfo->FreeZoneX = HomeSet.m_FreeZoneX;
        pHomeInfo->FreeZoneZ = HomeSet.m_FreeZoneZ;
        pHomeInfo->FreeZoneLX = HomeSet.m_FreeZoneLX;
        pHomeInfo->FreeZoneLZ = HomeSet.m_FreeZoneLZ;
        //
        pHomeInfo->BattleZoneX = HomeSet.m_BattleZoneX;
        pHomeInfo->BattleZoneZ = HomeSet.m_BattleZoneZ;
        pHomeInfo->BattleZoneLX = HomeSet.m_BattleZoneLX;
        pHomeInfo->BattleZoneLZ = HomeSet.m_BattleZoneLZ;
        //
        if (!m_HomeArray.PutData(pHomeInfo->bNation, pHomeInfo)) {
            TRACE("Home Info PutData Fail - %d\n", pHomeInfo->bNation);
            delete pHomeInfo;
            pHomeInfo = NULL;
        }

        HomeSet.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadAllKnights() {
    CKnightsSet KnightsSet;
    CString     strKnightsName, strChief, strViceChief_1, strViceChief_2, strViceChief_3;

    if (!KnightsSet.Open()) {
        AfxMessageBox(_T("Knights Open Fail!"));
        return FALSE;
    }
    if (KnightsSet.IsBOF() || KnightsSet.IsEOF()) {
        //    AfxMessageBox(_T("Knights Data Empty!"));
        return TRUE;
    }

    KnightsSet.MoveFirst();

    while (!KnightsSet.IsEOF()) {
        // sungyong ,, zone server : 카루스와 전쟁존을 합치므로 인해서,,
        /*    if( m_nServerNo == KARUS )    {
            if( KnightsSet.m_IDNum < 15000 )    {
                CKnights* pKnights = new _KNIGHTS;
                pKnights->sIndex = KnightsSet.m_IDNum;
                pKnights->bFlag = KnightsSet.m_Flag;
                pKnights->bNation = KnightsSet.m_Nation;
                strKnightsName = KnightsSet.m_IDName;
                strKnightsName.TrimRight();
                strChief = KnightsSet.m_Chief;
                strChief.TrimRight();
                strViceChief_1 = KnightsSet.m_ViceChief_1;
                strViceChief_1.TrimRight();
                strViceChief_2 = KnightsSet.m_ViceChief_2;
                strViceChief_2.TrimRight();
                strViceChief_3 = KnightsSet.m_ViceChief_3;
                strViceChief_3.TrimRight();

                strcpy( pKnights->strName, (char*)(LPCTSTR)strKnightsName );
                pKnights->sMembers = KnightsSet.m_Members;
                strcpy( pKnights->strChief, (char*)(LPCTSTR)strChief );
                strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)strViceChief_1 );
                strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)strViceChief_2 );
                strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)strViceChief_3 );
                pKnights->nMoney = atoi((const char*)(LPCTSTR)KnightsSet.m_Gold);
                pKnights->sDomination = KnightsSet.m_Domination;
                pKnights->nPoints = KnightsSet.m_Points;
                pKnights->bGrade = GetKnightsGrade( KnightsSet.m_Points );
                pKnights->bRanking = KnightsSet.m_Ranking;

                for(int i=0; i<MAX_CLAN; i++)    {
                    pKnights->arKnightsUser[i].byUsed = 0;
                    strcpy(pKnights->arKnightsUser[i].strUserName, "");
                }    

                if( !m_KnightsArray.PutData(pKnights->sIndex, pKnights) ) {
                    TRACE("Knights PutData Fail - %d\n", pKnights->sIndex);
                    delete pKnights;
                    pKnights = NULL;
                }
            }
        }
        else if( m_nServerNo == ELMORAD )    {    */
        /*    if( m_nServerNo == ELMORAD )    {
            if( KnightsSet.m_IDNum >= 15000 && KnightsSet.m_IDNum < 30000 )    {
                CKnights* pKnights = new CKnights;
                pKnights->InitializeValue();

                pKnights->m_sIndex = KnightsSet.m_IDNum;
                pKnights->m_byFlag = KnightsSet.m_Flag;
                pKnights->m_byNation = KnightsSet.m_Nation;
                //strcpy( pKnights->strName, (char*)(LPCTSTR)KnightsSet.m_IDName );
                strKnightsName = KnightsSet.m_IDName;
                strKnightsName.TrimRight();
                strChief = KnightsSet.m_Chief;
                strChief.TrimRight();
                strViceChief_1 = KnightsSet.m_ViceChief_1;
                strViceChief_1.TrimRight();
                strViceChief_2 = KnightsSet.m_ViceChief_2;
                strViceChief_2.TrimRight();
                strViceChief_3 = KnightsSet.m_ViceChief_3;
                strViceChief_3.TrimRight();

                strcpy( pKnights->m_strName, (char*)(LPCTSTR)strKnightsName );
                pKnights->m_sMembers = KnightsSet.m_Members;
                strcpy( pKnights->m_strChief, (char*)(LPCTSTR)strChief );
                strcpy( pKnights->m_strViceChief_1, (char*)(LPCTSTR)strViceChief_1 );
                strcpy( pKnights->m_strViceChief_2, (char*)(LPCTSTR)strViceChief_2 );
                strcpy( pKnights->m_strViceChief_3, (char*)(LPCTSTR)strViceChief_3 );
                pKnights->m_nMoney = atoi((const char*)(LPCTSTR)KnightsSet.m_Gold);
                pKnights->m_sDomination = KnightsSet.m_Domination;
                pKnights->m_nPoints = KnightsSet.m_Points;
                pKnights->m_byGrade = GetKnightsGrade( KnightsSet.m_Points );
                pKnights->m_byRanking = KnightsSet.m_Ranking;

                for(int i=0; i<MAX_CLAN; i++)    {
                    pKnights->m_arKnightsUser[i].byUsed = 0;
                    strcpy(pKnights->m_arKnightsUser[i].strUserName, "");
                }    

                if( !m_KnightsArray.PutData(pKnights->m_sIndex, pKnights) ) {
                    TRACE("Knights PutData Fail - %d\n", pKnights->m_sIndex);
                    delete pKnights;
                    pKnights = NULL;
                }

                //TRACE("knightindex = %d\n", IDNum);

            }
        }
        else    */
        {
            CKnights * pKnights = new CKnights;
            pKnights->InitializeValue();

            pKnights->m_sIndex = KnightsSet.m_IDNum;
            pKnights->m_byFlag = KnightsSet.m_Flag;
            pKnights->m_byNation = KnightsSet.m_Nation;
            strKnightsName = KnightsSet.m_IDName;
            strKnightsName.TrimRight();
            strChief = KnightsSet.m_Chief;
            strChief.TrimRight();
            strViceChief_1 = KnightsSet.m_ViceChief_1;
            strViceChief_1.TrimRight();
            strViceChief_2 = KnightsSet.m_ViceChief_2;
            strViceChief_2.TrimRight();
            strViceChief_3 = KnightsSet.m_ViceChief_3;
            strViceChief_3.TrimRight();

            strcpy(pKnights->m_strName, (char *)(LPCTSTR)strKnightsName);
            pKnights->m_sMembers = KnightsSet.m_Members;
            strcpy(pKnights->m_strChief, (char *)(LPCTSTR)strChief);
            strcpy(pKnights->m_strViceChief_1, (char *)(LPCTSTR)strViceChief_1);
            strcpy(pKnights->m_strViceChief_2, (char *)(LPCTSTR)strViceChief_2);
            strcpy(pKnights->m_strViceChief_3, (char *)(LPCTSTR)strViceChief_3);
            pKnights->m_nMoney = atoi((const char *)(LPCTSTR)KnightsSet.m_Gold);
            pKnights->m_sDomination = KnightsSet.m_Domination;
            pKnights->m_nPoints = KnightsSet.m_Points;
            pKnights->m_byGrade = GetKnightsGrade(KnightsSet.m_Points);
            pKnights->m_byRanking = KnightsSet.m_Ranking;

            for (int i = 0; i < MAX_CLAN; i++) {
                pKnights->m_arKnightsUser[i].byUsed = 0;
                strcpy(pKnights->m_arKnightsUser[i].strUserName, "");
            }

            if (!m_KnightsArray.PutData(pKnights->m_sIndex, pKnights)) {
                TRACE("Knights PutData Fail - %d\n", pKnights->m_sIndex);
                delete pKnights;
                pKnights = NULL;
            }
        }

        KnightsSet.MoveNext();
    }

    return TRUE;
}

BOOL CEbenezerDlg::LoadAllKnightsUserData() {
    CKnightsUserSet KnightsSet;
    CString         strUserName;
    int             iFame = 0, iLevel = 0, iClass = 0;

    if (!KnightsSet.Open()) {
        AfxMessageBox(_T("KnightsUser Open Fail!"));
        return FALSE;
    }
    if (KnightsSet.IsBOF() || KnightsSet.IsEOF()) {
        //    AfxMessageBox(_T("KnightsUser Data Empty!"));
        return TRUE;
    }

    KnightsSet.MoveFirst();

    while (!KnightsSet.IsEOF()) {
        // sungyong ,, zone server : 카루스와 전쟁존을 합치므로 인해서,,
        /*    if( m_nServerNo == KARUS )    {
            if( KnightsSet.m_sIDNum < 15000 )    {
                strUserName = KnightsSet.m_strUserID;
                strUserName.TrimRight();
                m_KnightsManager.AddKnightsUser( KnightsSet.m_sIDNum, (char*)(LPCTSTR) strUserName );
            }
        }
        else if( m_nServerNo == ELMORAD )    {    */
        /*    if( m_nServerNo == ELMORAD )    {
            if( KnightsSet.m_sIDNum >= 15000 && KnightsSet.m_sIDNum < 30000 )    {
                strUserName = KnightsSet.m_strUserID;
                strUserName.TrimRight();
                m_KnightsManager.AddKnightsUser( KnightsSet.m_sIDNum, (char*)(LPCTSTR) strUserName );
            }
        }
        else    */
        {
            strUserName = KnightsSet.m_strUserID;
            strUserName.TrimRight();
            m_KnightsManager.AddKnightsUser(KnightsSet.m_sIDNum, (char *)(LPCTSTR)strUserName);
        }

        KnightsSet.MoveNext();
    }

    return TRUE;
}

int CEbenezerDlg::GetKnightsAllMembers(int knightsindex, char * temp_buff, int & buff_index, int type) {
    if (knightsindex <= 0) {
        return 0;
    }

    CUser *    pUser = NULL;
    CKnights * pKnights = NULL;
    int        count = 0;

    if (type == 0) {
        for (int i = 0; i < MAX_USER; i++) {
            pUser = (CUser *)m_Iocport.m_SockArray[i];
            if (!pUser) {
                continue;
            }
            if (pUser->m_pUserData->m_bKnights == knightsindex) { // 같은 소속의 클랜..
                SetShort(temp_buff, strlen(pUser->m_pUserData->m_id), buff_index);
                SetString(temp_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), buff_index);
                SetByte(temp_buff, pUser->m_pUserData->m_bFame, buff_index);
                SetByte(temp_buff, pUser->m_pUserData->m_bLevel, buff_index);
                SetShort(temp_buff, pUser->m_pUserData->m_sClass, buff_index);
                SetByte(temp_buff, 1, buff_index);
                count++;
            }
        }
    } else if (type == 1) {
        pKnights = m_KnightsArray.GetData(knightsindex);
        if (!pKnights) {
            return 0;
        }

        for (int i = 0; i < MAX_CLAN; i++) {
            if (pKnights->m_arKnightsUser[i].byUsed == 1) { //
                pUser = GetUserPtr(pKnights->m_arKnightsUser[i].strUserName, 0x02);
                if (pUser) { // 접속중인 회원
                    if (pUser->m_pUserData->m_bKnights == knightsindex) {
                        SetShort(temp_buff, strlen(pUser->m_pUserData->m_id), buff_index);
                        SetString(temp_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), buff_index);
                        SetByte(temp_buff, pUser->m_pUserData->m_bFame, buff_index);
                        SetByte(temp_buff, pUser->m_pUserData->m_bLevel, buff_index);
                        SetShort(temp_buff, pUser->m_pUserData->m_sClass, buff_index);
                        SetByte(temp_buff, 1, buff_index);
                        count++;
                    } else { // 다른존에서 탈퇴나 추방된 유저이므로 메모리에서 삭제
                        m_KnightsManager.RemoveKnightsUser(knightsindex, pUser->m_pUserData->m_id);
                    }
                } else { // 비접속중인 회원
                    SetShort(temp_buff, strlen(pKnights->m_arKnightsUser[i].strUserName), buff_index);
                    SetString(temp_buff, pKnights->m_arKnightsUser[i].strUserName,
                              strlen(pKnights->m_arKnightsUser[i].strUserName), buff_index);
                    SetByte(temp_buff, 0, buff_index);
                    SetByte(temp_buff, 0, buff_index);
                    SetShort(temp_buff, 0, buff_index);
                    SetByte(temp_buff, 0, buff_index);
                    count++;
                }
            }
        }
    }

    return count;
}

void CEbenezerDlg::MarketBBSTimeCheck() {
    CUser * pUser = NULL; // Basic Initializations.
    int     send_index = 0;
    char    send_buff[256];
    memset(send_buff, NULL, 256);
    float currenttime = 0.0f;
    int   price = 0;

    currenttime = TimeGet();

    for (int i = 0; i < MAX_BBS_POST; i++) {
        if (m_sBuyID[i] != -1) { // BUY!!!
            pUser = (CUser *)m_Iocport.m_SockArray[m_sBuyID[i]];
            if (!pUser) {
                MarketBBSBuyDelete(i);
                continue;
            }

            if (m_fBuyStartTime[i] + BBS_CHECK_TIME < currenttime) {
                //                if (pUser->m_pUserData->m_iGold >= BUY_POST_PRICE) {
                //                    pUser->m_pUserData->m_iGold -= BUY_POST_PRICE ;
                //                    m_fBuyStartTime[i] = TimeGet();

                //                    memset(send_buff, NULL, 256); send_index = 0;
                //                    SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );    // Now the target
                //                    SetByte( send_buff, 0x02, send_index );
                //                    SetDWORD( send_buff, BUY_POST_PRICE, send_index );
                //                    SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
                //                    pUser->Send( send_buff, send_index );
                //                }
                //                else {
                MarketBBSBuyDelete(i);
                //                }
            }
        }

        if (m_sSellID[i] != -1) { // SELL!!!
            pUser = (CUser *)m_Iocport.m_SockArray[m_sSellID[i]];
            if (!pUser) {
                MarketBBSSellDelete(i);
                continue;
            }

            if (m_fSellStartTime[i] + BBS_CHECK_TIME < currenttime) {
                //                if (pUser->m_pUserData->m_iGold >= SELL_POST_PRICE) {
                //                    pUser->m_pUserData->m_iGold -= SELL_POST_PRICE ;
                //                    m_fSellStartTime[i] = TimeGet();

                //                    memset(send_buff, NULL, 256); send_index = 0;
                //                    SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );    // Now the target
                //                    SetByte( send_buff, 0x02, send_index );
                //                    SetDWORD( send_buff, SELL_POST_PRICE, send_index );
                //                    SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
                //                    pUser->Send( send_buff, send_index );
                //                }
                //                else {
                MarketBBSSellDelete(i);
                //                }
            }
        }
    }
}

void CEbenezerDlg::MarketBBSBuyDelete(short index) {
    m_sBuyID[index] = -1;
    memset(m_strBuyTitle[index], NULL, MAX_BBS_TITLE);
    memset(m_strBuyMessage[index], NULL, MAX_BBS_MESSAGE);
    m_iBuyPrice[index] = 0;
    m_fBuyStartTime[index] = 0.0f;
}

void CEbenezerDlg::MarketBBSSellDelete(short index) {
    m_sSellID[index] = -1;
    memset(m_strSellTitle[index], NULL, MAX_BBS_TITLE);
    memset(m_strSellMessage[index], NULL, MAX_BBS_MESSAGE);
    m_iSellPrice[index] = 0;
    m_fSellStartTime[index] = 0.0f;
}

void CEbenezerDlg::WritePacketLog() {
    CTime   cur = CTime::GetCurrentTime();
    CString starttime;
    starttime.Format("* Packet Check : send=%d, realsend=%d, recv=%d, time %d:%d분\r\n", m_iPacketCount,
                     m_iSendPacketCount, m_iRecvPacketCount, cur.GetHour(), cur.GetMinute());
    LogFileWrite((char *)(LPCTSTR)starttime);
}

int CEbenezerDlg::GetKnightsGrade(int nPoints) {
    int nGrade = 5;
    int nClanPoints = nPoints / 24; // 클랜등급 = 클랜원 국가 기여도의 총합 / 24

    if (nClanPoints >= 0 && nClanPoints < 2000) {
        nGrade = 5;
    } else if (nClanPoints >= 2000 && nClanPoints < 5000) {
        nGrade = 4;
    } else if (nClanPoints >= 5000 && nClanPoints < 10000) {
        nGrade = 3;
    } else if (nClanPoints >= 10000 && nClanPoints < 20000) {
        nGrade = 2;
    } else if (nClanPoints >= 20000) {
        nGrade = 1;
    } else {
        nGrade = 5;
    }

    return nGrade;
}

void CEbenezerDlg::CheckAliveUser() {
    CUser * pUser = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->GetState() == STATE_GAMESTART) {
            if (pUser->m_sAliveCount > 3) {
                pUser->Close();
                char logstr[1024];
                memset(logstr, NULL, 1024);
                sprintf(logstr, "User Alive Close - (%d) %s\r\n", pUser->GetSocketID(), pUser->m_pUserData->m_id);
                LogFileWrite(logstr);
            }
            pUser->m_sAliveCount++;
        }
    }
}

void CEbenezerDlg::KickOutAllUsers() {
    CUser * pUser = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }

        pUser->Close();
        Sleep(1000);
    }
}

__int64 CEbenezerDlg::GenerateItemSerial() {
    MYINT64 serial;
    MYSHORT increase;
    serial.i = 0;
    increase.w = 0;

    CTime t = CTime::GetCurrentTime();

    EnterCriticalSection(&g_serial_critical);

    increase.w = g_increase_serial++;

    serial.b[7] = (BYTE)m_nServerNo;
    serial.b[6] = (BYTE)(t.GetYear() % 100);
    serial.b[5] = (BYTE)t.GetMonth();
    serial.b[4] = (BYTE)t.GetDay();
    serial.b[3] = (BYTE)t.GetHour();
    serial.b[2] = (BYTE)t.GetMinute();
    serial.b[1] = increase.b[1];
    serial.b[0] = increase.b[0];

    LeaveCriticalSection(&g_serial_critical);

    //    TRACE("Generate Item Serial : %I64d\n", serial.i);
    return serial.i;
}

void CEbenezerDlg::KickOutZoneUsers(short zone) {
    int zoneindex = -1;

    CUser *  pTUser = NULL; // Pointer initialization!
    C3DMap * pMap = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pTUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pTUser) {
            continue;
        }

        if (pTUser->m_pUserData->m_bZone == zone) { // Only kick out users in requested zone.
            zoneindex = GetZoneIndex(pTUser->m_pUserData->m_bNation);
            if (zoneindex < 0) {
                continue;
            }

            pMap = m_ZoneArray[zoneindex];
            if (!pMap) {
                continue;
            }

            pTUser->ZoneChange(pMap->m_nZoneNumber, pMap->m_fInitX, pMap->m_fInitZ); // Move user to native zone.
        }
    }
}

void CEbenezerDlg::Send_UDP_All(char * pBuf, int len, int group_type) {
    map<int, _ZONE_SERVERINFO *>::iterator Iter1;
    map<int, _ZONE_SERVERINFO *>::iterator Iter2;

    int server_number = 0;

    if (group_type == 0) {
        Iter1 = m_ServerArray.m_UserTypeMap.begin();
        Iter2 = m_ServerArray.m_UserTypeMap.end();
        server_number = m_nServerNo;
    } else {
        Iter1 = m_ServerGroupArray.m_UserTypeMap.begin();
        Iter2 = m_ServerGroupArray.m_UserTypeMap.end();
        server_number = m_nServerGroupNo;
    }

    _ZONE_SERVERINFO * pInfo = NULL;

    for (; Iter1 != Iter2; Iter1++) {
        pInfo = (*Iter1).second;
        if (!pInfo) {
            continue;
        }
        if (pInfo->sServerNo != server_number) {
            m_pUdpSocket->SendUDPPacket(pInfo->strServerIP, pBuf, len);
        }
    }
}

BOOL CEbenezerDlg::LoadBattleTable() {
    CBattleSet BattleSet;

    if (!BattleSet.Open()) {
        AfxMessageBox(_T("BattleSet Data Open Fail!"));
        return FALSE;
    }
    if (BattleSet.IsBOF() || BattleSet.IsEOF()) {
        AfxMessageBox(_T("BattleSet Data Empty!"));
        return FALSE;
    }

    BattleSet.MoveFirst();

    while (!BattleSet.IsEOF()) {
        m_byOldVictory = BattleSet.m_byNation;
        BattleSet.MoveNext();
    }

    return TRUE;
}

void CEbenezerDlg::Send_CommandChat(char * pBuf, int len, int nation, CUser * pExceptUser) {
    CUser * pUser = NULL;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        //if( pUser == pExceptUser )            continue;
        if (pUser->GetState() == STATE_GAMESTART) {
            if (pUser->m_pUserData->m_bNation == nation) {
                pUser->Send(pBuf, len);
            }
        }
    }
}

void CEbenezerDlg::GetCaptainUserPtr() {
    CUser *                        pUser = NULL;
    CKnights *                     pKnights = NULL;
    map<int, CKnights *>::iterator Iter1, Iter2;

    Iter1 = m_KnightsArray.m_UserTypeMap.begin();
    Iter2 = m_KnightsArray.m_UserTypeMap.end();

    for (; Iter1 != Iter2; Iter1++) {
        pKnights = (*Iter1).second;
        if (!pKnights) {
            continue;
        }
        if (pKnights->m_byRanking == 1) {
        }
    }
}

BOOL CEbenezerDlg::LoadKnightsRankTable() {
    CKnightsRankSet KRankSet;
    int nRank = 0, nKnightsIndex = 0, nKaursRank = 0, nElmoRank = 0, nFindKarus = 0, nFindElmo = 0, send_index = 0,
        temp_index = 0;
    CKnights * pKnights = NULL;
    CUser *    pUser = NULL;
    CString    strKnightsName;

    std::string buff;

    char send_buff[1024];
    memset(send_buff, 0x00, 1024);
    char temp_buff[1024];
    memset(temp_buff, 0x00, 1024);
    char strKarusCaptainName[1024];
    memset(send_buff, 0x00, 1024);
    char strElmoCaptainName[1024];
    memset(send_buff, 0x00, 1024);
    char strKarusCaptain[5][50];
    char strElmoCaptain[5][50];
    for (int i = 0; i < 5; i++) {
        memset(strKarusCaptain[i], 0x00, 50);
        memset(strElmoCaptain[i], 0x00, 50);
    }

    if (!KRankSet.Open()) {
        TRACE("### KnightsRankTable Open Fail! ###\n");
        return TRUE;
    }
    if (KRankSet.IsBOF() || KRankSet.IsEOF()) {
        TRACE("### KnightsRankTable Empty! ###\n");
        return TRUE;
    }

    KRankSet.MoveFirst();

    while (!KRankSet.IsEOF()) {
        nRank = KRankSet.m_nRank;
        nKnightsIndex = KRankSet.m_shIndex;
        pKnights = m_KnightsArray.GetData(nKnightsIndex);
        strKnightsName = KRankSet.m_strName;
        strKnightsName.TrimRight();
        if (!pKnights) {
            KRankSet.MoveNext();
            continue;
        }
        if (pKnights->m_byNation == KARUS) {
            //if( nKaursRank == 5 || nFindKarus == 1 )    {
            if (nKaursRank == 5) {
                KRankSet.MoveNext();
                continue; // 5위까지 클랜장이 없으면 대장은 없음
            }
            pUser = GetUserPtr(pKnights->m_strChief, 0x02);
            //nKaursRank++;
            if (!pUser) {
                KRankSet.MoveNext();
                continue;
            }
            if (pUser->m_pUserData->m_bZone != ZONE_BATTLE) {
                KRankSet.MoveNext();
                continue;
            }
            if (pUser->m_pUserData->m_bKnights == nKnightsIndex) {
                pUser->m_pUserData->m_bFame = COMMAND_CAPTAIN;
                sprintf(strKarusCaptain[nKaursRank], "[%s][%s]", strKnightsName, pUser->m_pUserData->m_id);
                nKaursRank++;
                nFindKarus = 1;
                memset(send_buff, NULL, 1024);
                send_index = 0;
                SetByte(send_buff, WIZ_AUTHORITY_CHANGE, send_index);
                SetByte(send_buff, COMMAND_AUTHORITY, send_index);
                SetShort(send_buff, pUser->GetSocketID(), send_index);
                SetByte(send_buff, pUser->m_pUserData->m_bFame, send_index);
                //pUser->Send( send_buff, send_index );
                Send_Region(send_buff, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ);

                //strcpy( m_strKarusCaptain, pUser->m_pUserData->m_id );
                //Announcement( KARUS_CAPTAIN_NOTIFY, KARUS );
                memset(send_buff, NULL, 1024);
                send_index = 0;
                //TRACE("Karus Captain - %s, rank=%d, index=%d\n", pUser->m_pUserData->m_id, nRank, nKnightsIndex);
            }
        } else if (pKnights->m_byNation == ELMORAD) {
            //if( nElmoRank == 5 || nFindElmo == 1 )    {
            if (nElmoRank == 5) {
                KRankSet.MoveNext();
                continue; // 5위까지 클랜장이 없으면 대장은 없음
            }
            pUser = GetUserPtr(pKnights->m_strChief, 0x02);
            //nElmoRank++;
            if (!pUser) {
                KRankSet.MoveNext();
                continue;
            }
            if (pUser->m_pUserData->m_bZone != ZONE_BATTLE) {
                KRankSet.MoveNext();
                continue;
            }
            if (pUser->m_pUserData->m_bKnights == nKnightsIndex) {
                pUser->m_pUserData->m_bFame = COMMAND_CAPTAIN;
                sprintf(strElmoCaptain[nElmoRank], "[%s][%s]", strKnightsName, pUser->m_pUserData->m_id);
                nFindElmo = 1;
                nElmoRank++;
                memset(send_buff, NULL, 1024);
                send_index = 0;
                SetByte(send_buff, WIZ_AUTHORITY_CHANGE, send_index);
                SetByte(send_buff, COMMAND_AUTHORITY, send_index);
                SetShort(send_buff, pUser->GetSocketID(), send_index);
                SetByte(send_buff, pUser->m_pUserData->m_bFame, send_index);
                //pUser->Send( send_buff, send_index );
                Send_Region(send_buff, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ);

                //strcpy( m_strElmoradCaptain, pUser->m_pUserData->m_id );
                //Announcement( ELMORAD_CAPTAIN_NOTIFY, ELMORAD );
                //memset( send_buff, NULL, 1024 );    send_index = 0;
                //TRACE("Elmo Captain - %s, rank=%d, index=%d\n", pUser->m_pUserData->m_id, nRank, nKnightsIndex);
            }
        }

        KRankSet.MoveNext();
    }

    ::_LoadStringFromResource(IDS_KARUS_CAPTAIN, buff);
    sprintf(strKarusCaptainName, buff.c_str(), strKarusCaptain[0], strKarusCaptain[1], strKarusCaptain[2],
            strKarusCaptain[3], strKarusCaptain[4]);

    ::_LoadStringFromResource(IDS_ELMO_CAPTAIN, buff);
    sprintf(strElmoCaptainName, buff.c_str(), strElmoCaptain[0], strElmoCaptain[1], strElmoCaptain[2],
            strElmoCaptain[3], strElmoCaptain[4]);

    //sprintf( strKarusCaptainName, "카루스의 지휘관은 %s, %s, %s, %s, %s 입니다", strKarusCaptain[0], strKarusCaptain[1], strKarusCaptain[2], strKarusCaptain[3], strKarusCaptain[4]);
    //sprintf( strElmoCaptainName, "엘모라드의 지휘관은 %s, %s, %s, %s, %s 입니다", strKarusCaptain[0], strKarusCaptain[1], strKarusCaptain[2], strKarusCaptain[3], strKarusCaptain[4]);
    TRACE("LoadKnightsRankTable Success\n");

    SetByte(send_buff, WIZ_CHAT, send_index);
    SetByte(send_buff, WAR_SYSTEM_CHAT, send_index);
    SetByte(send_buff, 1, send_index);
    SetShort(send_buff, -1, send_index);
    SetShort(send_buff, strlen(strKarusCaptainName), send_index);
    SetString(send_buff, strKarusCaptainName, strlen(strKarusCaptainName), send_index);

    SetByte(temp_buff, WIZ_CHAT, temp_index);
    SetByte(temp_buff, WAR_SYSTEM_CHAT, temp_index);
    SetByte(temp_buff, 1, temp_index);
    SetShort(temp_buff, -1, temp_index);
    SetShort(temp_buff, strlen(strElmoCaptainName), temp_index);
    SetString(temp_buff, strElmoCaptainName, strlen(strElmoCaptainName), temp_index);

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->GetState() == STATE_GAMESTART) {
            if (pUser->m_pUserData->m_bNation == KARUS) {
                pUser->Send(send_buff, send_index);
            } else if (pUser->m_pUserData->m_bNation == ELMORAD) {
                pUser->Send(temp_buff, temp_index);
            }
        }
    }

    return TRUE;
}

void CEbenezerDlg::BattleZoneCurrentUsers() {
    C3DMap * pMap = NULL;
    CUser *  pUser = NULL;
    int      nBattleZoneIndex = GetZoneIndex(ZONE_BATTLE);
    pMap = (C3DMap *)m_ZoneArray[nBattleZoneIndex];
    if (!pMap) {
        return;
    }
    if (m_nServerNo != pMap->m_nServerNo) {
        return; // 현재의 서버가 배틀존 서버가 아니라면 리턴
    }

    char send_buff[128];
    memset(send_buff, 0x00, 128);
    int nKarusMan = 0, nElmoradMan = 0, send_index = 0;

    for (int i = 0; i < MAX_USER; i++) {
        pUser = (CUser *)m_Iocport.m_SockArray[i];
        if (!pUser) {
            continue;
        }
        if (pUser->m_pUserData->m_bZone == ZONE_BATTLE) {
            if (pUser->m_pUserData->m_bNation == KARUS) {
                nKarusMan++;
            } else if (pUser->m_pUserData->m_bNation == ELMORAD) {
                nElmoradMan++;
            }
        }
    }

    m_sKarusCount = nKarusMan;
    m_sElmoradCount = nElmoradMan;

    //TRACE("---> BattleZoneCurrentUsers - karus=%d, elmorad=%d\n", m_sKarusCount, m_sElmoradCount);

    SetByte(send_buff, UDP_BATTLEZONE_CURRENT_USERS, send_index);
    SetShort(send_buff, m_sKarusCount, send_index);
    SetShort(send_buff, m_sElmoradCount, send_index);
    Send_UDP_All(send_buff, send_index);
}

void CEbenezerDlg::FlySanta() {
    int  send_index = 0;
    char send_buff[128];
    memset(send_buff, 0x00, 128);

    SetByte(send_buff, WIZ_SANTA, send_index);
    Send_All(send_buff, send_index);
}

void CEbenezerDlg::WriteEventLog(char * pBuf) {
    char strLog[256];
    memset(strLog, 0x00, 256);
    CTime t = CTime::GetCurrentTime();
    wsprintf(strLog, "%d:%d-%d : %s \r\n", t.GetHour(), t.GetMinute(), t.GetSecond(), pBuf);
    EnterCriticalSection(&g_LogFile_critical);
    m_EvnetLogFile.Write(strLog, strlen(strLog));
    LeaveCriticalSection(&g_LogFile_critical);
}