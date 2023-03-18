// ServerDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "AIServer.h"
#include "AIServerDlg.h"
#include "GameSocket.h"
#include "math.h"
#include "MagicTableSet.h"
#include "MagicType1Set.h"
#include "MagicType2Set.h"
#include "MagicType3Set.h"
#include "MagicType4Set.h"
#include "NpcPosSet.h"
#include "ZoneInfoSet.h"
#include "NpcItemSet.h"
#include "NpcTableSet.h"
#include "MonTableSet.h"
#include "RNpcPosSet.h"
#include "MakeWeaponTableSet.h"
#include "MakeDefensiveTableSet.h"
#include "MakeGradeItemTableSet.h"
#include "MakeLareItemTableSet.h"
#include "Region.h"
#include "ini.h"

//#include "extern.h"            // ���� ��ü

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL      g_bNpcExit = FALSE;
ZoneArray g_arZone;

CRITICAL_SECTION g_User_critical;
CRITICAL_SECTION g_region_critical;
CRITICAL_SECTION g_LogFileWrite;

#define CHECK_ALIVE   100 //  ���Ӽ����� ����� ���迩�� �Ǵ�, Ÿ�̸� ����
#define REHP_TIME     200
#define MONSTER_SPEED 1500

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/*
     ** Repent AI Server �۾��� ���� ���� **
    1. 3���� �Լ� �߰�
        int GetSpeed(BYTE bySpeed); 
        int GetAttackSpeed(BYTE bySpeed); 
        int GetCatsSpeed(BYTE bySpeed); 
    2. Repent��  �°� �Ʒ��� �Լ� ����
        CreateNpcThread();
        GetMonsterTableData();
        GetNpcTableData();
        GetNpcItemTable();
*/

class CAboutDlg : public CDialog {
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum {
        IDD = IDD_ABOUTBOX
    };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
    : CDialog(CAboutDlg::IDD) {
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CServerDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CServerDlg)
    m_strStatus = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_iYear = 0;
    m_iMonth = 0;
    m_iDate = 0;
    m_iHour = 0;
    m_iMin = 0;
    m_iWeather = 0;
    m_iAmount = 0;
    m_byNight = 1;
    m_byZone = KARUS_ZONE;
    m_byBattleEvent = BATTLEZONE_CLOSE;
    m_sKillKarusNpc = 0;
    m_sKillElmoNpc = 0;
    m_pZoneEventThread = NULL;
    m_byTestMode = 0;
    //m_ppUserActive = NULL;
    //m_ppUserInActive = NULL;
}

void CServerDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CServerDlg)
    DDX_Control(pDX, IDC_LIST1, m_StatusList);
    DDX_Text(pDX, IDC_STATUS, m_strStatus);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
//{{AFX_MSG_MAP(CServerDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_GAMESERVER_LOGIN, OnGameServerLogin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

///////////////////////////////////////////////////////////////////////////////
//    ���� �ʱ�ȭ
//
BOOL CServerDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // Default Init ...
    DefaultInit();

    // TestCode
    TestCode();

    //----------------------------------------------------------------------
    //    Sets a random number starting point.
    //----------------------------------------------------------------------
    SetTimer(CHECK_ALIVE, 10000, NULL);
    srand((unsigned)time(NULL));
    for (int i = 0; i < 10; i++) {
        myrand(1, 10000); // don't delete
    }
    // Compress Init
    memset(m_CompBuf, NULL, 10240); // ������ �����͸� ������ ����
    m_iCompIndex = 0;               // ������ �������� ����
    m_CompCount = 0;                // ������ �������� ����
    InitializeCriticalSection(&g_User_critical);
    InitializeCriticalSection(&g_LogFileWrite);
    m_sSocketCount = 0;
    m_sErrorSocketCount = 0;
    m_sMapEventNpc = 0;
    m_sReSocketCount = 0;
    m_fReConnectStart = 0.0f;
    m_bFirstServerFlag = FALSE;
    m_byTestMode = NOW_TEST_MODE;

    // User Point Init
    for (int i = 0; i < MAX_USER; i++) {
        m_pUser[i] = NULL;
    }

    // Server Start
    CString logstr;
    CTime   time = CTime::GetCurrentTime();
    logstr.Format("[AI ServerStart - %d-%d-%d, %d:%d]", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(),
                  time.GetMinute());
    m_StatusList.AddString(logstr);
    logstr.Format("[AI ServerStart - %d-%d-%d, %d:%d]\r\n", time.GetYear(), time.GetMonth(), time.GetDay(),
                  time.GetHour(), time.GetMinute());
    TRACE(logstr);
    LogFileWrite(logstr);

    //----------------------------------------------------------------------
    //    Logfile initialize
    //----------------------------------------------------------------------
    char strLogFile[50];
    memset(strLogFile, 0x00, 50);
    wsprintf(strLogFile, "UserLog-%d-%d-%d.txt", time.GetYear(), time.GetMonth(), time.GetDay());
    m_UserLogFile.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone);
    m_UserLogFile.SeekToEnd();

    memset(strLogFile, 0x00, 50);
    wsprintf(strLogFile, "ItemLog-%d-%d-%d.txt", time.GetYear(), time.GetMonth(), time.GetDay());
    m_ItemLogFile.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone);
    m_ItemLogFile.SeekToEnd();

    //----------------------------------------------------------------------
    //    DB part initialize
    //----------------------------------------------------------------------
    GetServerInfoIni();

    if (m_byZone == UNIFY_ZONE) {
        m_strStatus.Format("UNIFY_ZONE ������ ���� ����");
    } else if (m_byZone == KARUS_ZONE) {
        m_strStatus.Format("KARUS ������ ���� ����");
    } else if (m_byZone == ELMORAD_ZONE) {
        m_strStatus.Format("ELMORAD ������ ���� ����");
    } else if (m_byZone == BATTLE_ZONE) {
        m_strStatus.Format("BATTLE ������ ���� ����");
    }

    //----------------------------------------------------------------------
    //    DB part initialize
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    //    Communication Part Initialize ...
    //----------------------------------------------------------------------
    m_Iocport.Init(MAX_SOCKET, 1, 1);

    for (int i = 0; i < MAX_SOCKET; i++) {
        m_Iocport.m_SockArrayInActive[i] = new CGameSocket;
    }

    //----------------------------------------------------------------------
    //    Load Magic Table
    //----------------------------------------------------------------------
    if (!GetMagicTableData()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }
    if (!GetMagicType1Data()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }
    if (!GetMagicType2Data()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }
    if (!GetMagicType3Data()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }
    if (!GetMagicType4Data()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    //----------------------------------------------------------------------
    //    Load NPC Item Table
    //----------------------------------------------------------------------
    if (!GetNpcItemTable()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    if (!GetMakeWeaponItemTableData()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    if (!GetMakeDefensiveItemTableData()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    if (!GetMakeGradeItemTableData()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    if (!GetMakeLareItemTableData()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    //----------------------------------------------------------------------
    //    Load NPC Chat Table
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    //    Load NPC Data & Activate NPC
    //----------------------------------------------------------------------
    if (!GetMonsterTableData()) { // Monster Ư��ġ ���̺� Load
        EndDialog(IDCANCEL);
        return FALSE;
    }

    if (!GetNpcTableData()) { // NPC Ư��ġ ���̺� Load
        EndDialog(IDCANCEL);
        return FALSE;
    }

    //----------------------------------------------------------------------
    //    Load Zone & Event...
    //----------------------------------------------------------------------
    if (!MapFileLoad()) {
        AfxPostQuitMessage(0);
    }

    if (!CreateNpcThread()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    //----------------------------------------------------------------------
    //    Load NPC DN Table
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    //    Start NPC THREAD
    //----------------------------------------------------------------------
    ResumeAI();

    //----------------------------------------------------------------------
    //    Start Accepting...
    //----------------------------------------------------------------------
    if (m_byZone == KARUS_ZONE || m_byZone == UNIFY_ZONE) {
        if (!m_Iocport.Listen(AI_KARUS_SOCKET_PORT)) {
            AfxMessageBox("FAIL TO CREATE LISTEN STATE", MB_OK);
        }
    } else if (m_byZone == ELMORAD_ZONE) {
        if (!m_Iocport.Listen(AI_ELMO_SOCKET_PORT)) {
            AfxMessageBox("FAIL TO CREATE LISTEN STATE", MB_OK);
        }
    } else if (m_byZone == BATTLE_ZONE) {
        if (!m_Iocport.Listen(AI_BATTLE_SOCKET_PORT)) {
            AfxMessageBox("FAIL TO CREATE LISTEN STATE", MB_OK);
        }
    }

    //::ResumeThread( m_Iocport.m_hAcceptThread );
    UpdateData(FALSE);
    return TRUE; // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() {
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
HCURSOR CServerDlg::OnQueryDragIcon() {
    return (HCURSOR)m_hIcon;
}

void CServerDlg::DefaultInit() {
    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu * pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon
}

//    Magic Table �� �д´�.
BOOL CServerDlg::GetMagicTableData() {
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

BOOL CServerDlg::GetMakeWeaponItemTableData() {
    CMakeWeaponTableSet MakeItemTableSet;

    if (!MakeItemTableSet.Open()) {
        AfxMessageBox(_T("GetMakeWeaponItemTableData Open Fail!"));
        return FALSE;
    }
    if (MakeItemTableSet.IsBOF() || MakeItemTableSet.IsEOF()) {
        AfxMessageBox(_T("GetMakeWeaponItemTableData Empty!"));
        return FALSE;
    }

    MakeItemTableSet.MoveFirst();

    while (!MakeItemTableSet.IsEOF()) {
        _MAKE_WEAPON * pTable = new _MAKE_WEAPON;

        pTable->byIndex = MakeItemTableSet.m_byLevel;
        pTable->sClass[0] = MakeItemTableSet.m_sClass_1;
        pTable->sClass[1] = MakeItemTableSet.m_sClass_2;
        pTable->sClass[2] = MakeItemTableSet.m_sClass_3;
        pTable->sClass[3] = MakeItemTableSet.m_sClass_4;
        pTable->sClass[4] = MakeItemTableSet.m_sClass_5;
        pTable->sClass[5] = MakeItemTableSet.m_sClass_6;
        pTable->sClass[6] = MakeItemTableSet.m_sClass_7;
        pTable->sClass[7] = MakeItemTableSet.m_sClass_8;
        pTable->sClass[8] = MakeItemTableSet.m_sClass_9;
        pTable->sClass[9] = MakeItemTableSet.m_sClass_10;
        pTable->sClass[10] = MakeItemTableSet.m_sClass_11;
        pTable->sClass[11] = MakeItemTableSet.m_sClass_12;

        if (!m_MakeWeaponItemArray.PutData(pTable->byIndex, pTable)) {
            TRACE("GetMakeWeaponItemTableData PutData Fail - %d\n", pTable->byIndex);
            delete pTable;
            pTable = NULL;
        }

        MakeItemTableSet.MoveNext();
    }
    return TRUE;
}

BOOL CServerDlg::GetMakeDefensiveItemTableData() {
    CMakeDefensiveTableSet MakeItemTableSet;

    if (!MakeItemTableSet.Open()) {
        AfxMessageBox(_T("GetMakeDefensiveItemTableData Open Fail!"));
        return FALSE;
    }
    if (MakeItemTableSet.IsBOF() || MakeItemTableSet.IsEOF()) {
        AfxMessageBox(_T("GetMakeDefensiveItemTableData Empty!"));
        return FALSE;
    }

    MakeItemTableSet.MoveFirst();

    while (!MakeItemTableSet.IsEOF()) {
        _MAKE_WEAPON * pTable = new _MAKE_WEAPON;

        pTable->byIndex = MakeItemTableSet.m_byLevel;
        pTable->sClass[0] = MakeItemTableSet.m_sClass_1;
        pTable->sClass[1] = MakeItemTableSet.m_sClass_2;
        pTable->sClass[2] = MakeItemTableSet.m_sClass_3;
        pTable->sClass[3] = MakeItemTableSet.m_sClass_4;
        pTable->sClass[4] = MakeItemTableSet.m_sClass_5;
        pTable->sClass[5] = MakeItemTableSet.m_sClass_6;
        pTable->sClass[6] = MakeItemTableSet.m_sClass_7;

        if (!m_MakeDefensiveItemArray.PutData(pTable->byIndex, pTable)) {
            TRACE("GetMakeDefensiveItemTableData PutData Fail - %d\n", pTable->byIndex);
            delete pTable;
            pTable = NULL;
        }

        MakeItemTableSet.MoveNext();
    }
    return TRUE;
}

BOOL CServerDlg::GetMakeGradeItemTableData() {
    CMakeGradeItemTableSet MakeItemTableSet;

    if (!MakeItemTableSet.Open()) {
        AfxMessageBox(_T("MakeGradeItemTable Open Fail!"));
        return FALSE;
    }
    if (MakeItemTableSet.IsBOF() || MakeItemTableSet.IsEOF()) {
        AfxMessageBox(_T("MakeGradeItemTable Empty!"));
        return FALSE;
    }

    MakeItemTableSet.MoveFirst();

    while (!MakeItemTableSet.IsEOF()) {
        _MAKE_ITEM_GRADE_CODE * pTable = new _MAKE_ITEM_GRADE_CODE;

        pTable->byItemIndex = MakeItemTableSet.m_byItemIndex;
        pTable->sGrade_1 = MakeItemTableSet.m_byGrade_1;
        pTable->sGrade_2 = MakeItemTableSet.m_byGrade_2;
        pTable->sGrade_3 = MakeItemTableSet.m_byGrade_3;
        pTable->sGrade_4 = MakeItemTableSet.m_byGrade_4;
        pTable->sGrade_5 = MakeItemTableSet.m_byGrade_5;
        pTable->sGrade_6 = MakeItemTableSet.m_byGrade_6;
        pTable->sGrade_7 = MakeItemTableSet.m_byGrade_7;
        pTable->sGrade_8 = MakeItemTableSet.m_byGrade_8;
        pTable->sGrade_9 = MakeItemTableSet.m_byGrade_9;

        if (!m_MakeGradeItemArray.PutData(pTable->byItemIndex, pTable)) {
            TRACE("MakeGradeItemTable PutData Fail - %d\n", pTable->byItemIndex);
            delete pTable;
            pTable = NULL;
        }

        MakeItemTableSet.MoveNext();
    }
    return TRUE;
}

BOOL CServerDlg::GetMakeLareItemTableData() {
    CMakeLareItemTableSet MakeItemTableSet;

    if (!MakeItemTableSet.Open()) {
        AfxMessageBox(_T("MakeLareItemTable Open Fail!"));
        return FALSE;
    }
    if (MakeItemTableSet.IsBOF() || MakeItemTableSet.IsEOF()) {
        AfxMessageBox(_T("MakeLareItemTable Empty!"));
        return FALSE;
    }

    MakeItemTableSet.MoveFirst();

    while (!MakeItemTableSet.IsEOF()) {
        _MAKE_ITEM_LARE_CODE * pTable = new _MAKE_ITEM_LARE_CODE;

        pTable->byItemLevel = MakeItemTableSet.m_byLevelGrade;
        pTable->sLareItem = MakeItemTableSet.m_sLareItem;
        pTable->sMagicItem = MakeItemTableSet.m_sMagicItem;
        pTable->sGereralItem = MakeItemTableSet.m_sGereralItem;

        if (!m_MakeLareItemArray.PutData(pTable->byItemLevel, pTable)) {
            TRACE("MakeItemTable PutData Fail - %d\n", pTable->byItemLevel);
            delete pTable;
            pTable = NULL;
        }

        MakeItemTableSet.MoveNext();
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//    NPC Item Table �� �д´�.
//
BOOL CServerDlg::GetNpcItemTable() {
    CNpcItemSet NpcItemSet;
    int         nRowCount = 0;
    int         nItem = 0;

    try {
        if (NpcItemSet.IsOpen()) {
            NpcItemSet.Close();
        }

        NpcItemSet.m_strSort = _T("sIndex");

        if (!NpcItemSet.Open()) {
            AfxMessageBox(_T("MONSTER ITEM DB Open Fail!"));
            return FALSE;
        }

        if (NpcItemSet.IsBOF()) {
            AfxMessageBox(_T("MONSTER ITEM DB Empty!"));
            return FALSE;
        }

        while (!NpcItemSet.IsEOF()) {
            nRowCount++;
            NpcItemSet.MoveNext();
        }

        m_NpcItem.m_nField = NpcItemSet.m_nFields;
        m_NpcItem.m_nRow = nRowCount;

        if (nRowCount == 0) {
            return FALSE;
        }

        m_NpcItem.m_ppItem = new int *[m_NpcItem.m_nRow];
        for (int i = 0; i < m_NpcItem.m_nRow; i++) {
            m_NpcItem.m_ppItem[i] = new int[m_NpcItem.m_nField];
        }

        NpcItemSet.MoveFirst();

        int i = 0;
        while (!NpcItemSet.IsEOF()) {
            m_NpcItem.m_ppItem[i][0] = NpcItemSet.m_sIndex;
            m_NpcItem.m_ppItem[i][1] = NpcItemSet.m_iItem01;
            m_NpcItem.m_ppItem[i][2] = NpcItemSet.m_sPersent01;
            m_NpcItem.m_ppItem[i][3] = NpcItemSet.m_iItem02;
            m_NpcItem.m_ppItem[i][4] = NpcItemSet.m_sPersent02;
            m_NpcItem.m_ppItem[i][5] = NpcItemSet.m_iItem03;
            m_NpcItem.m_ppItem[i][6] = NpcItemSet.m_sPersent03;
            m_NpcItem.m_ppItem[i][7] = NpcItemSet.m_iItem04;
            m_NpcItem.m_ppItem[i][8] = NpcItemSet.m_sPersent04;
            m_NpcItem.m_ppItem[i][9] = NpcItemSet.m_iItem05;
            m_NpcItem.m_ppItem[i][10] = NpcItemSet.m_sPersent05;

            i++;
            NpcItemSet.MoveNext();
        }
    } catch (CMemoryException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    } catch (CDBException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    }

    return TRUE;
}

//    Monster Table Data �� �д´�.
BOOL CServerDlg::GetMonsterTableData() {
    CMonTableSet NpcTableSet;

    try {
        //if(m_arMonTable.GetSize()) return FALSE;

        if (NpcTableSet.IsOpen()) {
            NpcTableSet.Close();
        }

        if (!NpcTableSet.Open()) {
            AfxMessageBox(_T("MONSTER DB Open Fail!"));
            return FALSE;
        }
        if (NpcTableSet.IsBOF()) {
            AfxMessageBox(_T("MONSTER DB Empty!"));
            return FALSE;
        }

        while (!NpcTableSet.IsEOF()) {
            CNpcTable * Npc = new CNpcTable;
            CString     tmpNpcName;
            Npc->Initialize();

            Npc->m_sSid = NpcTableSet.m_sSid;                   // MONSTER(NPC) Serial ID
            _tcscpy(Npc->m_strName, NpcTableSet.m_strName);     // MONSTER(NPC) Name
            Npc->m_sPid = NpcTableSet.m_sPid;                   // MONSTER(NPC) Picture ID
            Npc->m_sSize = NpcTableSet.m_sSize;                 // MONSTER(NPC) ĳ�� ũ�� ����
            Npc->m_iWeapon_1 = NpcTableSet.m_iWeapon1;          // ���빫��
            Npc->m_iWeapon_2 = NpcTableSet.m_iWeapon2;          // ���빫��
            Npc->m_byGroup = NpcTableSet.m_byGroup;             // �Ҽ�����
            Npc->m_byActType = NpcTableSet.m_byActType;         // �ൿ����
            Npc->m_byRank = NpcTableSet.m_byRank;               // ����
            Npc->m_byTitle = NpcTableSet.m_byTitle;             // ����
            Npc->m_iSellingGroup = NpcTableSet.m_iSellingGroup; // item group
            Npc->m_sLevel = NpcTableSet.m_sLevel;               // level
            Npc->m_iExp = NpcTableSet.m_iExp;                   // ����ġ
            Npc->m_iLoyalty = NpcTableSet.m_iLoyalty;           // loyalty
            Npc->m_iMaxHP = NpcTableSet.m_iHpPoint;             // �ִ� HP
            Npc->m_sMaxMP = NpcTableSet.m_sMpPoint;             // �ִ� MP
            Npc->m_sAttack = NpcTableSet.m_sAtk;                // ���ݰ�
            Npc->m_sDefense = NpcTableSet.m_sAc;                // ��
            Npc->m_sHitRate = NpcTableSet.m_sHitRate;           // Ÿ�ݼ�����
            Npc->m_sEvadeRate = NpcTableSet.m_sEvadeRate;       // ȸ�Ǽ�����
            Npc->m_sDamage = NpcTableSet.m_sDamage;             // �⺻ ������
            Npc->m_sAttackDelay = NpcTableSet.m_sAttackDelay;   // ���ݵ�����
            Npc->m_bySpeed_1 = NpcTableSet.m_bySpeed1;          // �̵��ӵ�    (�ȱ�)
            Npc->m_bySpeed_2 = NpcTableSet.m_bySpeed2;          // �̵��ӵ�    (�ٱ�)
            Npc->m_sSpeed = MONSTER_SPEED;                      // �̵��ӵ�
            Npc->m_sStandTime = NpcTableSet.m_sStandtime;       // ���ִ� �ð�
            Npc->m_iMagic1 = NpcTableSet.m_iMagic1;             // ��븶�� 1
            Npc->m_iMagic2 = NpcTableSet.m_iMagic2;             // ��븶�� 2
            Npc->m_iMagic3 = NpcTableSet.m_iMagic3;             // ��븶�� 3
            Npc->m_sFireR = NpcTableSet.m_sFireR;               // ȭ�� ���׷�
            Npc->m_sColdR = NpcTableSet.m_sColdR;               // �ñ� ���׷�
            Npc->m_sLightningR = NpcTableSet.m_sLightningR;     // ���� ���׷�
            Npc->m_sMagicR = NpcTableSet.m_sMagicR;             // ���� ���׷�
            Npc->m_sDiseaseR = NpcTableSet.m_sDiseaseR;         // ���� ���׷�
            Npc->m_sPoisonR = NpcTableSet.m_sPoisonR;           // �� ���׷�
            Npc->m_sLightR = NpcTableSet.m_sLightR;             // �� ���׷�
            Npc->m_sBulk = NpcTableSet.m_sBulk;
            Npc->m_bySearchRange = NpcTableSet.m_bySearchRange;   // �� Ž�� ����
            Npc->m_byAttackRange = NpcTableSet.m_byAttackRange;   // �����Ÿ�
            Npc->m_byTracingRange = NpcTableSet.m_byTracingRange; // �߰ݰŸ�
            //Npc->m_sAI = NpcTableSet.m_sAI;                // �ΰ����� �ε���
            Npc->m_tNpcType = NpcTableSet.m_byType; // NPC Type
                                                    // 0 : Monster
                                                    // 1 : Normal NPC

            Npc->m_byFamilyType = NpcTableSet.m_byFamily; // ������̿��� �������踦 �����Ѵ�.
            //Npc->m_tItemPer;        // �������� ������ Ȯ��
            //Npc->m_tDnPer;            // ���� ������Ȯ��

            Npc->m_iMoney = NpcTableSet.m_iMoney; // �������� ��
            Npc->m_iItem = NpcTableSet.m_sItem;   // �������� ������
            Npc->m_byDirectAttack = NpcTableSet.m_byDirectAttack;
            Npc->m_byMagicAttack = NpcTableSet.m_byMagicAttack;

            if (!m_arMonTable.PutData(Npc->m_sSid, Npc)) {
                TRACE("GetMonsterTableData - PutData Fail - %d\n", Npc->m_sSid);
                delete Npc;
                Npc = NULL;
            }
            //m_arMonTable.Add(Npc);

            NpcTableSet.MoveNext();
        }

        NpcTableSet.Close();
    } catch (CMemoryException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    } catch (CDBException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    }

    return TRUE;
}

//    NPC Table Data �� �д´�. (��� & NPC)
BOOL CServerDlg::GetNpcTableData() {
    CNpcTableSet NpcTableSet;

    try {
        //if(m_arNpcTable.GetSize()) return FALSE;

        if (NpcTableSet.IsOpen()) {
            NpcTableSet.Close();
        }

        if (!NpcTableSet.Open()) {
            AfxMessageBox(_T("NPC DB Open Fail!"));
            return FALSE;
        }
        if (NpcTableSet.IsBOF()) {
            AfxMessageBox(_T("NPC DB Empty!"));
            return FALSE;
        }

        while (!NpcTableSet.IsEOF()) {
            CNpcTable * Npc = new CNpcTable;
            CString     tmpNpcName;
            Npc->Initialize();

            Npc->m_sSid = NpcTableSet.m_sSid;                   // MONSTER(NPC) Serial ID
            _tcscpy(Npc->m_strName, NpcTableSet.m_strName);     // MONSTER(NPC) Name
            Npc->m_sPid = NpcTableSet.m_sPid;                   // MONSTER(NPC) Picture ID
            Npc->m_sSize = NpcTableSet.m_sSize;                 // MONSTER(NPC) ĳ�� ũ�� ����
            Npc->m_iWeapon_1 = NpcTableSet.m_iWeapon1;          // ���빫��
            Npc->m_iWeapon_2 = NpcTableSet.m_iWeapon2;          // ���빫��
            Npc->m_byGroup = NpcTableSet.m_byGroup;             // �Ҽ�����
            Npc->m_byActType = NpcTableSet.m_byActType;         // �ൿ����
            Npc->m_byRank = NpcTableSet.m_byRank;               // ����
            Npc->m_byTitle = NpcTableSet.m_byTitle;             // ����
            Npc->m_iSellingGroup = NpcTableSet.m_iSellingGroup; // item group
            Npc->m_sLevel = NpcTableSet.m_sLevel;               // level
            Npc->m_iExp = NpcTableSet.m_iExp;                   // ����ġ
            Npc->m_iLoyalty = NpcTableSet.m_iLoyalty;           // loyalty
            Npc->m_iMaxHP = NpcTableSet.m_iHpPoint;             // �ִ� HP
            Npc->m_sMaxMP = NpcTableSet.m_sMpPoint;             // �ִ� MP
            Npc->m_sAttack = NpcTableSet.m_sAtk;                // ���ݰ�
            Npc->m_sDefense = NpcTableSet.m_sAc;                // ��
            Npc->m_sHitRate = NpcTableSet.m_sHitRate;           // Ÿ�ݼ�����
            Npc->m_sEvadeRate = NpcTableSet.m_sEvadeRate;       // ȸ�Ǽ�����
            Npc->m_sDamage = NpcTableSet.m_sDamage;             // �⺻ ������
            Npc->m_sAttackDelay = NpcTableSet.m_sAttackDelay;   // ���ݵ�����
            Npc->m_bySpeed_1 = NpcTableSet.m_bySpeed1;          // �̵��ӵ�    (�ȱ�)
            Npc->m_bySpeed_2 = NpcTableSet.m_bySpeed2;          // �̵��ӵ�    (�ٱ�)
            Npc->m_sSpeed = MONSTER_SPEED;                      // �̵��ӵ�
            Npc->m_sStandTime = NpcTableSet.m_sStandtime;       // ���ִ� �ð�
            Npc->m_iMagic1 = NpcTableSet.m_iMagic1;             // ��븶�� 1
            Npc->m_iMagic2 = NpcTableSet.m_iMagic2;             // ��븶�� 2
            Npc->m_iMagic3 = NpcTableSet.m_iMagic3;             // ��븶�� 3
            Npc->m_sFireR = NpcTableSet.m_sFireR;               // ȭ�� ���׷�
            Npc->m_sColdR = NpcTableSet.m_sColdR;               // �ñ� ���׷�
            Npc->m_sLightningR = NpcTableSet.m_sLightningR;     // ���� ���׷�
            Npc->m_sMagicR = NpcTableSet.m_sMagicR;             // ���� ���׷�
            Npc->m_sDiseaseR = NpcTableSet.m_sDiseaseR;         // ���� ���׷�
            Npc->m_sPoisonR = NpcTableSet.m_sPoisonR;           // �� ���׷�
            Npc->m_sLightR = NpcTableSet.m_sLightR;             // �� ���׷�
            Npc->m_sBulk = NpcTableSet.m_sBulk;
            Npc->m_bySearchRange = NpcTableSet.m_bySearchRange;   // �� Ž�� ����
            Npc->m_byAttackRange = NpcTableSet.m_byAttackRange;   // �����Ÿ�
            Npc->m_byTracingRange = NpcTableSet.m_byTracingRange; // �߰ݰŸ�
            //Npc->m_sAI = NpcTableSet.m_sAI;                // �ΰ����� �ε���
            Npc->m_tNpcType = NpcTableSet.m_byType; // NPC Type
                                                    // 0 : Monster
                                                    // 1 : Normal NPC

            Npc->m_byFamilyType = NpcTableSet.m_byFamily; // ������̿��� �������踦 �����Ѵ�.
            //Npc->m_tItemPer;        // �������� ������ Ȯ��
            //Npc->m_tDnPer;            // ���� ������Ȯ��

            Npc->m_iMoney = NpcTableSet.m_iMoney; // �������� ��
            Npc->m_iItem = NpcTableSet.m_sItem;   // �������� ������
            Npc->m_byDirectAttack = NpcTableSet.m_byDirectAttack;
            Npc->m_byMagicAttack = NpcTableSet.m_byMagicAttack;

            if (!m_arNpcTable.PutData(Npc->m_sSid, Npc)) {
                TRACE("GetNpcTableData - PutData Fail - %d\n", Npc->m_sSid);
                delete Npc;
                Npc = NULL;
            }
            //m_arNpcTable.Add(Npc);

            NpcTableSet.MoveNext();
        }

        NpcTableSet.Close();
    } catch (CMemoryException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    } catch (CDBException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    }

    return TRUE;
}

//    Npc Thread �� �����.
BOOL CServerDlg::CreateNpcThread() {
    BOOL   bMoveNext = TRUE;
    int    nSerial = m_sMapEventNpc;
    int    nPathSerial = 1;
    int    nNpcCount = 0;
    int    nRandom = 0, nServerNum = 0;
    double dbSpeed = 0;

    m_TotalNPC = 0; // DB�� �ִ� ��
    m_CurrentNPC = 0;
    m_CurrentNPCError = 0;

    CNpcTable *  pNpcTable = NULL;
    CRoomEvent * pRoom = NULL;

    // sungyong test
    //CRNpcPosSet NpcPosSet;        // �Ѹ��� �׽�Ʈ��
    CNpcPosSet NpcPosSet;

    char szPath[500];
    char szX[5];
    char szZ[5];

    float fRandom_X = 0.0f;
    float fRandom_Z = 0.0f;
    BOOL  bFindNpcTable = FALSE;

    int nMonsterNumber = 0;

    try {
        if (NpcPosSet.IsOpen()) {
            NpcPosSet.Close();
        }
        if (!NpcPosSet.Open()) {
            AfxMessageBox(_T("MONSTER_POS DB Open Fail!"));
            return FALSE;
        }
        if (NpcPosSet.IsBOF()) {
            AfxMessageBox(_T("MONSTER_POS DB Empty!"));
            return FALSE;
        }

        NpcPosSet.MoveFirst();

        while (!NpcPosSet.IsEOF()) {
            nMonsterNumber = NpcPosSet.m_NumNPC;
            //if( NpcPosSet.m_ZoneID == 101 )    {    // �׽�Ʈ�� ���ؼ�,,
            //    nMonsterNumber = 1;
            //if(nMonsterNumber > 4)    {
            //    nMonsterNumber = nMonsterNumber / 4;
            //}
            //}
            nServerNum = 0;
            nServerNum = GetServerNumber(NpcPosSet.m_ZoneID);

            if (m_byZone == nServerNum || m_byZone == UNIFY_ZONE) {
                for (int j = 0; j < nMonsterNumber; j++) {
                    CNpc * pNpc = new CNpc;
                    pNpc->m_sNid = nSerial++;                // ���� �������� ���� ��ȣ
                    pNpc->m_sSid = (short)NpcPosSet.m_NpcID; // MONSTER(NPC) Serial ID

                    pNpc->m_byMoveType = NpcPosSet.m_ActType;
                    pNpc->m_byInitMoveType = NpcPosSet.m_ActType;
                    bFindNpcTable = FALSE;

                    if (NpcPosSet.m_ActType >= 0 && NpcPosSet.m_ActType < 100) {
                        pNpcTable = m_arMonTable.GetData(pNpc->m_sSid);
                        if (pNpcTable != NULL) {
                            bFindNpcTable = TRUE;
                        }
                    } else if (NpcPosSet.m_ActType >= 100) {
                        pNpc->m_byMoveType = NpcPosSet.m_ActType - 100;
                        //pNpc->m_byInitMoveType = NpcPosSet.m_ActType - 100;

                        pNpcTable = m_arNpcTable.GetData(pNpc->m_sSid);
                        if (pNpcTable != NULL) {
                            bFindNpcTable = TRUE;
                        }
                    }

                    pNpc->m_byBattlePos = 0;

                    if (pNpc->m_byMoveType >= 2) {
                        pNpc->m_byBattlePos = myrand(1, 3);
                        pNpc->m_byPathCount = nPathSerial++;
                    }

                    pNpc->InitPos();

                    if (bFindNpcTable == FALSE) {
                        TRACE("#### CreateNpcThread Fail : [nid = %d, sid = %d] #####\n", pNpc->m_sNid, pNpc->m_sSid);
                    }

                    if (bMoveNext) {
                        bMoveNext = FALSE;
                        nNpcCount = NpcPosSet.m_NumNPC;
                    }

                    _tcscpy(pNpc->m_strName, pNpcTable->m_strName); // MONSTER(NPC) Name
                    pNpc->m_sPid = pNpcTable->m_sPid;               // MONSTER(NPC) Picture ID
                    pNpc->m_sSize = pNpcTable->m_sSize;             // ĳ������ ����(100 �ۼ�Ʈ ����)
                    pNpc->m_iWeapon_1 = pNpcTable->m_iWeapon_1;     // ���빫��
                    pNpc->m_iWeapon_2 = pNpcTable->m_iWeapon_2;     // ���빫��
                    pNpc->m_byGroup = pNpcTable->m_byGroup;         // �Ҽ�����
                    pNpc->m_byActType = pNpcTable->m_byActType;     // �ൿ����
                    pNpc->m_byRank = pNpcTable->m_byRank;           // ����
                    pNpc->m_byTitle = pNpcTable->m_byTitle;         // ����
                    pNpc->m_iSellingGroup = pNpcTable->m_iSellingGroup;
                    pNpc->m_sLevel = pNpcTable->m_sLevel;             // level
                    pNpc->m_iExp = pNpcTable->m_iExp;                 // ����ġ
                    pNpc->m_iLoyalty = pNpcTable->m_iLoyalty;         // loyalty
                    pNpc->m_iHP = pNpcTable->m_iMaxHP;                // �ִ� HP
                    pNpc->m_iMaxHP = pNpcTable->m_iMaxHP;             // ���� HP
                    pNpc->m_sMP = pNpcTable->m_sMaxMP;                // �ִ� MP
                    pNpc->m_sMaxMP = pNpcTable->m_sMaxMP;             // ���� MP
                    pNpc->m_sAttack = pNpcTable->m_sAttack;           // ���ݰ�
                    pNpc->m_sDefense = pNpcTable->m_sDefense;         // ��
                    pNpc->m_sHitRate = pNpcTable->m_sHitRate;         // Ÿ�ݼ�����
                    pNpc->m_sEvadeRate = pNpcTable->m_sEvadeRate;     // ȸ�Ǽ�����
                    pNpc->m_sDamage = pNpcTable->m_sDamage;           // �⺻ ������
                    pNpc->m_sAttackDelay = pNpcTable->m_sAttackDelay; // ���ݵ�����
                    pNpc->m_sSpeed = pNpcTable->m_sSpeed;             // �̵��ӵ�
                    dbSpeed = pNpcTable->m_sSpeed;
                    pNpc->m_fSpeed_1 = (float)pNpcTable->m_bySpeed_1 * (dbSpeed / 1000);    // �⺻ �̵� Ÿ��
                    pNpc->m_fSpeed_2 = (float)pNpcTable->m_bySpeed_2 * (dbSpeed / 1000);    // �ٴ� �̵� Ÿ��..
                    pNpc->m_fOldSpeed_1 = (float)pNpcTable->m_bySpeed_1 * (dbSpeed / 1000); // �⺻ �̵� Ÿ��
                    pNpc->m_fOldSpeed_2 = (float)pNpcTable->m_bySpeed_2 * (dbSpeed / 1000); // �ٴ� �̵� Ÿ��..
                    pNpc->m_fSecForMetor = 4.0f;                                            // �ʴ� �� �� �ִ� �Ÿ�..
                    pNpc->m_sStandTime = pNpcTable->m_sStandTime;                           // ���ִ� �ð�
                    pNpc->m_iMagic1 = pNpcTable->m_iMagic1;                                 // ��븶�� 1
                    pNpc->m_iMagic2 = pNpcTable->m_iMagic2;                                 // ��븶�� 2
                    pNpc->m_iMagic3 = pNpcTable->m_iMagic3;                                 // ��븶�� 3
                    pNpc->m_sFireR = pNpcTable->m_sFireR;                                   // ȭ�� ���׷�
                    pNpc->m_sColdR = pNpcTable->m_sColdR;                                   // �ñ� ���׷�
                    pNpc->m_sLightningR = pNpcTable->m_sLightningR;                         // ���� ���׷�
                    pNpc->m_sMagicR = pNpcTable->m_sMagicR;                                 // ���� ���׷�
                    pNpc->m_sDiseaseR = pNpcTable->m_sDiseaseR;                             // ���� ���׷�
                    pNpc->m_sPoisonR = pNpcTable->m_sPoisonR;                               // �� ���׷�
                    pNpc->m_sLightR = pNpcTable->m_sLightR;                                 // �� ���׷�
                    pNpc->m_fBulk = (float)(((double)pNpcTable->m_sBulk / 100) * ((double)pNpcTable->m_sSize / 100));
                    pNpc->m_bySearchRange = pNpcTable->m_bySearchRange;   // �� Ž�� ����
                    pNpc->m_byAttackRange = pNpcTable->m_byAttackRange;   // �����Ÿ�
                    pNpc->m_byTracingRange = pNpcTable->m_byTracingRange; // �߰ݰŸ�
                    pNpc->m_sAI = pNpcTable->m_sAI;                       // �ΰ����� �ε���
                    pNpc->m_tNpcType = pNpcTable->m_tNpcType;             // NPC Type
                    pNpc->m_byFamilyType = pNpcTable->m_byFamilyType;     // ������̿��� �������踦 �����Ѵ�.
                    pNpc->m_iMoney = pNpcTable->m_iMoney;                 // �������� ��
                    pNpc->m_iItem = pNpcTable->m_iItem;                   // �������� ������
                    pNpc->m_tNpcLongType = pNpcTable->m_byDirectAttack;
                    pNpc->m_byWhatAttackType = pNpcTable->m_byMagicAttack;

                    //////// MONSTER POS ////////////////////////////////////////
                    pNpc->m_sCurZone = NpcPosSet.m_ZoneID;

                    // map�� ������ ��ġ�� �����ϰ� ��ġ��Ų��.. (�׽�Ʈ �� : ����-DB���� �о���µ��� �� ��ġ ����)
                    nRandom = abs(NpcPosSet.m_LeftX - NpcPosSet.m_RightX);
                    if (nRandom <= 1) {
                        fRandom_X = (float)NpcPosSet.m_LeftX;
                    } else {
                        if (NpcPosSet.m_LeftX < NpcPosSet.m_RightX) {
                            fRandom_X = (float)myrand(NpcPosSet.m_LeftX, NpcPosSet.m_RightX);
                        } else {
                            fRandom_X = (float)myrand(NpcPosSet.m_RightX, NpcPosSet.m_LeftX);
                        }
                    }
                    nRandom = abs(NpcPosSet.m_TopZ - NpcPosSet.m_BottomZ);
                    if (nRandom <= 1) {
                        fRandom_Z = (float)NpcPosSet.m_TopZ;
                    } else {
                        if (NpcPosSet.m_TopZ < NpcPosSet.m_BottomZ) {
                            fRandom_Z = (float)myrand(NpcPosSet.m_TopZ, NpcPosSet.m_BottomZ);
                        } else {
                            fRandom_Z = (float)myrand(NpcPosSet.m_BottomZ, NpcPosSet.m_TopZ);
                        }
                    }

                    pNpc->m_fCurX = fRandom_X;
                    pNpc->m_fCurY = 0;
                    pNpc->m_fCurZ = fRandom_Z;

                    if (NpcPosSet.m_RegTime < 15) {
                        TRACE("##### ServerDlg:CreateNpcThread - RegenTime Error :  nid=%d, name=%s, regentime=%d "
                              "#####\n",
                              pNpc->m_sNid + NPC_BAND, pNpc->m_strName, NpcPosSet.m_RegTime);
                        NpcPosSet.m_RegTime = 30;
                    }

                    pNpc->m_sRegenTime = NpcPosSet.m_RegTime * 1000; // ��(DB)����-> �и��������

                    pNpc->m_sMaxPathCount = NpcPosSet.m_DotCnt;

                    if (pNpc->m_byMoveType >= 2 && NpcPosSet.m_DotCnt == 0) {
                        TRACE("##### ServerDlg:CreateNpcThread - Path type Error :  nid=%d, sid=%d, name=%s, "
                              "acttype=%d, path=%d #####\n",
                              pNpc->m_sNid + NPC_BAND, pNpc->m_sSid, pNpc->m_strName, pNpc->m_byMoveType,
                              pNpc->m_sMaxPathCount);
                        return FALSE;
                    }

                    int index = 0;
                    memset(szPath, 0, 500);
                    strcpy(szPath, NpcPosSet.m_path);

                    if (NpcPosSet.m_DotCnt != 0) {
                        for (int l = 0; l < NpcPosSet.m_DotCnt; l++) {
                            memset(szX, 0x00, 5);
                            memset(szZ, 0x00, 5);
                            GetString(szX, szPath, 4, index);
                            GetString(szZ, szPath, 4, index);
                            pNpc->m_PathList.pPattenPos[l].x = atoi(szX);
                            pNpc->m_PathList.pPattenPos[l].z = atoi(szZ);
                            //    TRACE(" l=%d, x=%d, z=%d\n", l, pNpc->m_PathList.pPattenPos[l].x, pNpc->m_PathList.pPattenPos[l].z);
                        }
                    }

                    //pNpc->m_byType            = NpcPosSet.m_byType;

                    pNpc->m_tItemPer = pNpcTable->m_tItemPer; // NPC Type
                    pNpc->m_tDnPer = pNpcTable->m_tDnPer;     // NPC Type

                    pNpc->m_nInitMinX = pNpc->m_nLimitMinX = NpcPosSet.m_LeftX;
                    pNpc->m_nInitMinY = pNpc->m_nLimitMinZ = NpcPosSet.m_TopZ;
                    pNpc->m_nInitMaxX = pNpc->m_nLimitMaxX = NpcPosSet.m_RightX;
                    pNpc->m_nInitMaxY = pNpc->m_nLimitMaxZ = NpcPosSet.m_BottomZ;
                    // dungeon work
                    pNpc->m_byDungeonFamily = NpcPosSet.m_DungeonFamily;
                    pNpc->m_bySpecialType = NpcPosSet.m_SpecialType;
                    pNpc->m_byRegenType = NpcPosSet.m_RegenType;
                    pNpc->m_byTrapNumber = NpcPosSet.m_TrapNumber;

                    if (pNpc->m_byDungeonFamily > 0) {
                        pNpc->m_nLimitMinX = NpcPosSet.m_LimitMinX;
                        pNpc->m_nLimitMinZ = NpcPosSet.m_LimitMinZ;
                        pNpc->m_nLimitMaxX = NpcPosSet.m_LimitMaxX;
                        pNpc->m_nLimitMaxZ = NpcPosSet.m_LimitMaxZ;
                    }

                    pNpc->m_ZoneIndex = -1;

                    for (int i = 0; i < g_arZone.size(); i++) {
                        if (g_arZone[i]->m_nZoneNumber == pNpc->m_sCurZone) {
                            pNpc->m_ZoneIndex = i;
                            break;
                        }
                    }

                    if (pNpc->m_ZoneIndex == -1) {
                        AfxMessageBox("Error : CServerDlg,, Invaild zone Index!!");
                        return FALSE;
                    }

                    //pNpc->Init();
                    //m_arNpc.Add(pNpc);
                    if (!m_arNpc.PutData(pNpc->m_sNid, pNpc)) {
                        TRACE("Npc PutData Fail - %d\n", pNpc->m_sNid);
                        delete pNpc;
                        pNpc = NULL;
                    }

                    //
                    if (g_arZone[pNpc->m_ZoneIndex]->m_byRoomEvent > 0 && pNpc->m_byDungeonFamily > 0) {
                        pRoom = NULL;
                        pRoom = g_arZone[pNpc->m_ZoneIndex]->m_arRoomEventArray.GetData(pNpc->m_byDungeonFamily);
                        if (!pRoom) {
                            TRACE("Error : CServerDlg,, Map Room Npc Fail!! : nid=%d, sid=%d, name=%s, fam=%d, "
                                  "zoneindex=%d\n",
                                  pNpc->m_sNid + NPC_BAND, pNpc->m_sSid, pNpc->m_strName, pNpc->m_byDungeonFamily,
                                  pNpc->m_ZoneIndex);
                            AfxMessageBox("Error : CServerDlg,, Map Room Npc Fail!!");
                            return FALSE;
                        }

                        int * pInt = NULL;
                        pInt = new int;
                        *pInt = pNpc->m_sNid;
                        if (!pRoom->m_mapRoomNpcArray.PutData(pNpc->m_sNid, pInt)) {
                            TRACE("### Map - Room Array MonsterNid Fail : nid=%d, sid=%d ###\n", pNpc->m_sNid,
                                  pNpc->m_sSid);
                        }
                    }

                    m_TotalNPC = nSerial;

                    if (--nNpcCount > 0) {
                        continue;
                    }

                    bMoveNext = TRUE;
                    nNpcCount = 0;
                }
            }

            nPathSerial = 1;
            NpcPosSet.MoveNext();
        }

        NpcPosSet.Close();
    }

    catch (CMemoryException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    } catch (CDBException * e) {
        e->ReportError();
        e->Delete();

        return FALSE;
    }

    int          step = 0;
    int          nThreadNumber = 0;
    CNpcThread * pNpcThread = NULL;

    for (int i = 0; i < m_arNpc.GetSize(); i++) {
        if (step == 0) {
            pNpcThread = new CNpcThread;
        }
        pNpcThread->m_pNpc[step] = m_arNpc.GetData(i);
        pNpcThread->m_pNpc[step]->m_sThreadNumber = nThreadNumber;
        pNpcThread->m_pNpc[step]->Init();

        ++step;

        if (step == NPC_NUM) {
            pNpcThread->m_sThreadNumber = nThreadNumber++;
            pNpcThread->pIOCP = &m_Iocport;
            pNpcThread->m_pThread =
                AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
            m_arNpcThread.push_back(pNpcThread);
            step = 0;
        }
    }
    if (step != 0) {
        pNpcThread->m_sThreadNumber = nThreadNumber++;
        pNpcThread->pIOCP = &m_Iocport;
        pNpcThread->m_pThread =
            AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        m_arNpcThread.push_back(pNpcThread);
    }

    // Event Npc Logic
    /*
    pNpcThread = NULL;
    pNpcThread = new CNpcThread;    
    pNpcThread->pIOCP = &m_Iocport;
    pNpcThread->m_sThreadNumber = 1000;

    for(int i = 0; i < NPC_NUM; i++) // �̸� �ִ� ��ȯ NPC_NUM���� �޸� �Ҵ�
    {
        CNpc*    pNpc = new CNpc;
        pNpc->m_sNid = nSerial++;
        pNpc->m_NpcState = NPC_DEAD;
        pNpc->m_lEventNpc = 0;
        pNpcThread->m_pNpc[i] = pNpc;
        if( !m_arNpc.PutData( pNpc->m_sNid, pNpc) )    {
            TRACE("Npc PutData Fail - %d\n", pNpc->m_sNid);
            delete pNpc;
            pNpc = NULL;
        }
    }

    pNpcThread->m_pThread = AfxBeginThread(NpcThreadProc, &(pNpcThread->m_ThreadInfo), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    m_arEventNpcThread.push_back( pNpcThread );
    */

    m_pZoneEventThread =
        AfxBeginThread(ZoneEventThreadProc, (LPVOID)(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

    //TRACE("m_TotalNPC = %d \n", m_TotalNPC);
    CString logstr;
    logstr.Format("[Monster Init - %d]", m_TotalNPC);
    m_StatusList.AddString(logstr);

    return TRUE;
}

//    NPC Thread ���� �۵���Ų��.
void CServerDlg::ResumeAI() {
    for (int i = 0; i < m_arNpcThread.size(); i++) {
        for (int j = 0; j < NPC_NUM; j++) {
            m_arNpcThread[i]->m_ThreadInfo.pNpc[j] = m_arNpcThread[i]->m_pNpc[j];
        }

        m_arNpcThread[i]->m_ThreadInfo.pIOCP = &m_Iocport;

        ::ResumeThread(m_arNpcThread[i]->m_pThread->m_hThread);
    }

    // Event Npc Logic
    /*    m_arEventNpcThread[0]->m_ThreadInfo.hWndMsg = this->GetSafeHwnd();
    for(int j = 0; j < NPC_NUM; j++)
    {
        m_arEventNpcThread[0]->m_ThreadInfo.pNpc[j] = NULL;    // �ʱ� ��ȯ ���� �翬�� �����Ƿ� NULL�� �۵��� �Ƚ�Ŵ
        m_arEventNpcThread[0]->m_ThreadInfo.m_byNpcUsed[j] = 0;
    }
    m_arEventNpcThread[0]->m_ThreadInfo.pIOCP = &m_Iocport;

    ::ResumeThread(m_arEventNpcThread[0]->m_pThread->m_hThread);    
    */

    ::ResumeThread(m_pZoneEventThread->m_hThread);
}

//    �޸� ����
BOOL CServerDlg::DestroyWindow() {
    // TODO: Add your specialized code here and/or call the base class
    KillTimer(CHECK_ALIVE);
    //KillTimer( REHP_TIME );

    g_bNpcExit = TRUE;

    if (m_UserLogFile.m_hFile != CFile::hFileNull) {
        m_UserLogFile.Close();
    }
    if (m_ItemLogFile.m_hFile != CFile::hFileNull) {
        m_ItemLogFile.Close();
    }

    for (int i = 0; i < m_arNpcThread.size(); i++) {
        WaitForSingleObject(m_arNpcThread[i]->m_pThread->m_hThread, INFINITE);
    }

    // Event Npc Logic
    /*    for(int i = 0; i < m_arEventNpcThread.size(); i++)
    {
        WaitForSingleObject(m_arEventNpcThread[i]->m_pThread->m_hThread, INFINITE);
    }    */

    WaitForSingleObject(m_pZoneEventThread, INFINITE);

    // DB���̺� ���� �κ�

    // Map(Zone) Array Delete...
    for (int i = 0; i < g_arZone.size(); i++) {
        delete g_arZone[i];
    }
    g_arZone.clear();

    // NpcTable Array Delete
    if (!m_arMonTable.IsEmpty()) {
        m_arMonTable.DeleteAllData();
    }

    // NpcTable Array Delete
    if (!m_arNpcTable.IsEmpty()) {
        m_arNpcTable.DeleteAllData();
    }

    // NpcThread Array Delete
    for (int i = 0; i < m_arNpcThread.size(); i++) {
        delete m_arNpcThread[i];
    }
    m_arNpcThread.clear();

    // Event Npc Logic
    // EventNpcThread Array Delete
    /*    for(int i = 0; i < m_arEventNpcThread.size(); i++)
        delete m_arEventNpcThread[i];
    m_arEventNpcThread.clear();        */

    // Item Array Delete
    if (m_NpcItem.m_ppItem) {
        for (int i = 0; i < m_NpcItem.m_nRow; i++) {
            delete[] m_NpcItem.m_ppItem[i];
            m_NpcItem.m_ppItem[i] = NULL;
        }
        delete[] m_NpcItem.m_ppItem;
        m_NpcItem.m_ppItem = NULL;
    }

    if (!m_MakeWeaponItemArray.IsEmpty()) {
        m_MakeWeaponItemArray.DeleteAllData();
    }

    if (!m_MakeDefensiveItemArray.IsEmpty()) {
        m_MakeDefensiveItemArray.DeleteAllData();
    }

    if (!m_MakeGradeItemArray.IsEmpty()) {
        m_MakeGradeItemArray.DeleteAllData();
    }

    if (!m_MakeLareItemArray.IsEmpty()) {
        m_MakeLareItemArray.DeleteAllData();
    }

    // MagicTable Array Delete
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

    // Npc Array Delete
    if (!m_arNpc.IsEmpty()) {
        m_arNpc.DeleteAllData();
    }

    // User Array Delete
    for (int i = 0; i < MAX_USER; i++) {
        if (m_pUser[i]) {
            delete m_pUser[i];
            m_pUser[i] = NULL;
        }
    }

    // Party Array Delete
    if (!m_arParty.IsEmpty()) {
        m_arParty.DeleteAllData();
    }

    while (!m_ZoneNpcList.empty()) {
        m_ZoneNpcList.pop_front();
    }

    DeleteCriticalSection(&g_User_critical);
    DeleteCriticalSection(&g_LogFileWrite);

    return CDialog::DestroyWindow();
}

void CServerDlg::DeleteUserList(int uid) {
    if (uid < 0 || uid >= MAX_USER) {
        TRACE("#### ServerDlg:DeleteUserList Uid Fail : uid=%d\n", uid);
        return;
    }

    EnterCriticalSection(&g_User_critical);

    CUser * pUser = NULL;
    pUser = m_pUser[uid];
    if (!pUser) {
        LeaveCriticalSection(&g_User_critical);
        TRACE("#### ServerDlg:DeleteUserList UserPtr Fail : uid=%d\n", uid);
        return;
    }

    if (pUser->m_iUserId == uid) {
        TRACE("*** UserLogOut���� ������ ��ȯ : uid=%d, %s ***\n", uid, pUser->m_strUserID);
        pUser->m_lUsed = 1;
        delete m_pUser[uid];
        m_pUser[uid] = NULL;
    } else {
        TRACE("#### ServerDlg:DeleteUserList Not Uid Fail : uid=%d\n", uid);
    }

    LeaveCriticalSection(&g_User_critical);
}

BOOL CServerDlg::MapFileLoad() {
    CFile   file;
    CString szFullPath, errormsg, sZoneName;
    MAP *   pMap = NULL;
    m_sTotalMap = 0;

    CZoneInfoSet ZoneInfoSet;

    if (!ZoneInfoSet.Open()) {
        AfxMessageBox(_T("ZoneInfoTable Open Fail!"));
        return FALSE;
    }

    if (ZoneInfoSet.IsBOF() || ZoneInfoSet.IsEOF()) {
        AfxMessageBox(_T("ZoneInfoTable Empty!"));
        return FALSE;
    }

    ZoneInfoSet.MoveFirst();

    while (!ZoneInfoSet.IsEOF()) {
        sZoneName = ZoneInfoSet.m_strZoneName;

        szFullPath.Format(".\\AIServer_MAP\\%s", sZoneName);

        if (!file.Open(szFullPath, CFile::modeRead)) {
            errormsg.Format("���� Open ���� - %s\n", szFullPath);
            AfxMessageBox(errormsg);
            return FALSE;
        }

        pMap = new MAP;
        pMap->m_nServerNo = ZoneInfoSet.m_ServerNo;
        pMap->m_nZoneNumber = ZoneInfoSet.m_ZoneNo;
        strcpy(pMap->m_MapName, (char *)(LPCTSTR)sZoneName);

        if (!pMap->LoadMap((HANDLE)file.m_hFile)) {
            errormsg.Format("Map Load ���� - %s\n", szFullPath);
            AfxMessageBox(errormsg);
            delete pMap;
            return FALSE;
        }

        // dungeon work
        if (ZoneInfoSet.m_RoomEvent > 0) {
            if (!pMap->LoadRoomEvent(ZoneInfoSet.m_RoomEvent)) {
                errormsg.Format("Map Room Event Load ���� - %s\n", szFullPath);
                AfxMessageBox(errormsg);
                delete pMap;
                return FALSE;
            }
            pMap->m_byRoomEvent = 1;
        }

        g_arZone.push_back(pMap);

        ZoneInfoSet.MoveNext();

        file.Close();
        m_sTotalMap++;
    }

    return TRUE;
}

// sungyong 2002.05.23
// game server�� ��� npc������ ����..
void CServerDlg::AllNpcInfo() {
    // server alive check
    CNpc * pNpc = NULL;
    MAP *  pMap = NULL;
    int    nZone = 0;
    int    size = m_arNpc.GetSize();

    int  send_index = 0, zone_index = 0, packet_size = 0;
    int  count = 0, send_count = 0, send_tot = 0;
    char send_buff[2048];
    ::ZeroMemory(send_buff, sizeof(send_buff));

    for (int i = 0; i < m_sTotalMap; i++) {
        pMap = g_arZone[i];
        if (pMap == NULL) {
            continue;
        }
        nZone = pMap->m_nZoneNumber;

        ::ZeroMemory(send_buff, sizeof(send_buff));
        send_index = 0;
        SetByte(send_buff, AG_SERVER_INFO, send_index);
        SetByte(send_buff, SERVER_INFO_START, send_index);
        SetByte(send_buff, nZone, send_index);
        packet_size = Send(send_buff, send_index, nZone);

        zone_index = GetZoneIndex(nZone);
        send_index = 2;
        count = 0;
        send_count = 0;
        m_CompCount = 0;
        m_iCompIndex = 0;
        ::ZeroMemory(send_buff, sizeof(send_buff));

        TRACE("****  allNpcInfo start = %d *****\n", nZone);

        for (int i = 0; i < size; i++) {
            pNpc = m_arNpc.GetData(i);
            if (pNpc == NULL) {
                TRACE("##### allNpcInfo Fail = %d\n", i);
                continue;
            }
            if (pNpc->m_sCurZone != nZone) {
                continue;
            }

            pNpc->SendNpcInfoAll(send_buff, send_index, count);
            count++;
            if (count == NPC_NUM) {
                SetByte(send_buff, NPC_INFO_ALL, send_count);
                SetByte(send_buff, (BYTE)count, send_count);
                m_CompCount++;
                //::CopyMemory(m_CompBuf+m_iCompIndex, send_buff, send_index);
                memset(m_CompBuf, 0x00, 10240);
                ::CopyMemory(m_CompBuf, send_buff, send_index);
                m_iCompIndex = send_index;
                SendCompressedData(nZone);
                send_index = 2;
                send_count = 0;
                count = 0;
                send_tot++;
                //TRACE("AllNpcInfo - send_count=%d, count=%d, zone=%d\n", send_tot, count, nZone);
                ::ZeroMemory(send_buff, sizeof(send_buff));
                Sleep(50);
            }
        }

        //if( count != 0 )    TRACE("--> AllNpcInfo - send_count=%d, count=%d, zone=%d\n", send_tot, count, nZone);
        if (count != 0 && count < NPC_NUM) {
            send_count = 0;
            SetByte(send_buff, NPC_INFO_ALL, send_count);
            SetByte(send_buff, (BYTE)count, send_count);
            Send(send_buff, send_index, nZone);
            send_tot++;
            //TRACE("AllNpcInfo - send_count=%d, count=%d, zone=%d\n", send_tot, count, nZone);
            Sleep(50);
        }

        send_index = 0;
        ::ZeroMemory(send_buff, sizeof(send_buff));
        SetByte(send_buff, AG_SERVER_INFO, send_index);
        SetByte(send_buff, SERVER_INFO_END, send_index);
        SetByte(send_buff, nZone, send_index);
        SetShort(send_buff, (short)m_TotalNPC, send_index);
        packet_size = Send(send_buff, send_index, nZone);

        TRACE("****  allNpcInfo end = %d *****\n", nZone);
    }

    Sleep(1000);
}
// ~sungyong 2002.05.23

CUser * CServerDlg::GetUserPtr(int nid) {
    CUser * pUser = NULL;

    if (nid < 0 || nid >= MAX_USER) {
        if (nid != -1) {
            TRACE("### GetUserPtr :: User Array Overflow [%d] ###\n", nid);
        }
        return NULL;
    }

    /*    if( !m_ppUserActive[nid] )
        return NULL;

    if( m_ppUserActive[nid]->m_lUsed == 1 ) return NULL;    // ������ ����� ���ġ ����.. (logout��)

    pUser = (CUser*)m_ppUserActive[nid];
*/
    pUser = m_pUser[nid];
    if (pUser == NULL) {
        return NULL;
    }
    if (pUser->m_lUsed == 1) {
        return NULL; // ������ ����� ���ġ ����.. (logout��)
    }
    if (pUser->m_iUserId < 0 || pUser->m_iUserId >= MAX_USER) {
        return NULL;
    }

    if (pUser->m_iUserId == nid) {
        return pUser;
    }

    return NULL;
}

void CServerDlg::OnTimer(UINT nIDEvent) {
    switch (nIDEvent) {
    case CHECK_ALIVE:
        CheckAliveTest();
        break;
    case REHP_TIME:
        //RechargeHp();
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

// sungyong 2002.05.23
void CServerDlg::CheckAliveTest() {
    int  send_index = 0;
    char send_buff[256];
    ::ZeroMemory(send_buff, sizeof(send_buff));
    int iErrorCode = 0;

    SetByte(send_buff, AG_CHECK_ALIVE_REQ, send_index);

    CGameSocket * pSocket = NULL;
    int           size = 0, count = 0;

    CString logstr;
    CTime   time = CTime::GetCurrentTime();

    for (int i = 0; i < MAX_SOCKET; i++) {
        pSocket = (CGameSocket *)m_Iocport.m_SockArray[i];
        if (pSocket == NULL) {
            continue;
        }
        size = pSocket->Send(send_buff, send_index);
        if (size > 0) {
            ++m_sErrorSocketCount;
            if (m_sErrorSocketCount == 10) {
                logstr.Format("*** All Socket Closed ***  %d-%d-%d, %d:%d]\r\n", time.GetYear(), time.GetMonth(),
                              time.GetDay(), time.GetHour(), time.GetMinute());
                //LogFileWrite( logstr );
            }
            count++;
        }
        //TRACE("size = %d, socket_num = %d, i=%d \n", size, pSocket->m_sSocketID, i);
    }

    if (count <= 0) {
        DeleteAllUserList(9999);
    }

    RegionCheck();
}

void CServerDlg::DeleteAllUserList(int zone) {
    if (zone < 0) {
        return;
    }

    CString logstr;

    if (zone == 9999 && m_bFirstServerFlag == TRUE) { // ��� ������ ������ ����...
        CUser * pUser = NULL;
        MAP *   pMap = NULL;

        TRACE("*** DeleteAllUserList - Start *** \n");

        for (int i = 0; i < m_sTotalMap; i++) {
            pMap = g_arZone[i];
            if (pMap == NULL) {
                continue;
            }
            for (int k = 0; k < pMap->m_sizeRegion.cx; k++) {
                for (int j = 0; j < pMap->m_sizeRegion.cy; j++) {
                    if (!pMap->m_ppRegion[k][j].m_RegionUserArray.IsEmpty()) {
                        pMap->m_ppRegion[k][j].m_RegionUserArray.DeleteAllData();
                    }
                }
            }
        }

        int size = g_arZone.size();

        EnterCriticalSection(&g_User_critical);
        // User Array Delete
        for (int i = 0; i < MAX_USER; i++) {
            pUser = m_pUser[i];
            if (pUser == NULL) {
                continue;
            }
            delete m_pUser[i];
            m_pUser[i] = NULL;
        }
        // ��Ƽ ���� ����..
        LeaveCriticalSection(&g_User_critical);

        // Party Array Delete
        if (!m_arParty.IsEmpty()) {
            m_arParty.DeleteAllData();
        }

        m_bFirstServerFlag = FALSE;
        TRACE("*** DeleteAllUserList - End *** \n");

        logstr.Format("[ DELETE All User List ]");
        m_StatusList.AddString(logstr);
    } else {
        if (zone != 9999) {
            logstr.Format("[GameServer DisConnect - zone = %d]", zone);
            m_StatusList.AddString(logstr);
        }
    }
}
// ~sungyong 2002.05.23

void CServerDlg::SendCompressedData(int nZone) {
    if (!m_CompCount || m_CompCount < 0 || !m_iCompIndex || m_iCompIndex < 0) {
        m_CompCount = 0;
        m_iCompIndex = 0;
        TRACE("#### SendCompressData Fail --> count=%d, index=%d\n", m_CompCount, m_iCompIndex);
        return;
    }

    m_CompMng.FlushAddData();
    m_CompMng.AddData(m_CompBuf, m_iCompIndex);
    m_CompMng.PreCompressWork();
    m_CompMng.Compress();

    int   comp_data_len = m_CompMng.GetCompressedDataCount();
    int   org_data_len = m_CompMng.GetUnCompressDataLength();
    DWORD crc_value = m_CompMng.GetCrcValue();

    int  send_index = 0, packet_size = 0;
    char send_buff[2048];
    ::ZeroMemory(send_buff, sizeof(send_buff));
    SetByte(send_buff, AG_COMPRESSED_DATA, send_index);
    SetShort(send_buff, (short)comp_data_len, send_index);
    SetShort(send_buff, (short)org_data_len, send_index);
    SetDWORD(send_buff, crc_value, send_index);
    SetShort(send_buff, (short)m_CompCount, send_index);

    char * packet = m_CompMng.GetExtractedBufferPtr();
    SetString(send_buff, packet, comp_data_len, send_index);

    if (packet == NULL) {
        m_CompCount = 0;
        m_iCompIndex = 0;
        m_CompMng.FlushAddData();
        TRACE("#### SendCompressData Fail packet==null\n");
        return;
    }

    packet_size = Send(send_buff, send_index, nZone);

    m_CompCount = 0;
    m_iCompIndex = 0;
    m_CompMng.FlushAddData();
}

BOOL CServerDlg::PreTranslateMessage(MSG * pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
            return TRUE;
        }
        if (pMsg->wParam == VK_F9) {
            SyncTest();
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

// sungyong 2002.05.23
int CServerDlg::Send(char * pData, int length, int nZone) {
    SEND_DATA * pNewData = NULL;
    pNewData = new SEND_DATA;
    if (pNewData == NULL) {
        return 0;
    }

    pNewData->sCurZone = nZone;
    pNewData->sLength = length;
    ::CopyMemory(pNewData->pBuf, pData, length);

    EnterCriticalSection(&(m_Iocport.m_critSendData));
    m_Iocport.m_SendDataList.push_back(pNewData);
    LeaveCriticalSection(&(m_Iocport.m_critSendData));

    PostQueuedCompletionStatus(m_Iocport.m_hSendIOCP, 0, 0, NULL);

    return 0;
}
// ~sungyong 2002.05.23

LRESULT CServerDlg::OnGameServerLogin(WPARAM wParam, LPARAM lParam) {
    /*    if( m_bNpcInfoDown ) {
        m_ZoneNpcList.push_back(wParam);
        return;
    }

    AllNpcInfo( wParam );    */

    return 0L;
}

void CServerDlg::GameServerAcceptThread() {
    ::ResumeThread(m_Iocport.m_hAcceptThread);
}

void CServerDlg::SyncTest() {
    FILE * stream = fopen("c:\\aiserver.txt", "w");

    fprintf(stream, "*****   Check ... List  *****\n");

    int  send_index = 0;
    char send_buff[256];
    ::ZeroMemory(send_buff, sizeof(send_buff));
    int iErrorCode = 0;

    SetByte(send_buff, AG_CHECK_ALIVE_REQ, send_index);

    CGameSocket * pSocket = NULL;
    int           size = 0;

    for (int i = 0; i < MAX_SOCKET; i++) {
        pSocket = (CGameSocket *)m_Iocport.m_SockArray[i];
        if (pSocket == NULL) {
            continue;
        }
        size = pSocket->Send(send_buff, send_index);

        fprintf(stream, " size = %d, socket_num = %d \n", size, pSocket->m_sSocketID);
    }

    /*
    int size = m_arNpc.GetSize();
    CNpc* pNpc = NULL;
    CUser* pUser = NULL;
    __Vector3 vUser;
    __Vector3 vNpc;
    __Vector3 vDistance;
    float fDis = 0.0f;
    int count = 0;

    fprintf(stream, "***** NPC List : %d *****\n", size);
    for(int i=0; i<size; i++)
    {
        pNpc = m_arNpc.GetData(i);
        if(pNpc == NULL)
        {
            TRACE("##### allNpcInfo Fail = %d\n", i);
            continue;
        }

        fprintf(stream, "nid=(%d, %s), zone=%d, x=%.2f, z=%.2f, rx=%d, rz=%d\n", pNpc->m_sNid+NPC_BAND, pNpc->m_strName,pNpc->m_sCurZone, pNpc->m_fCurX, pNpc->m_fCurZ, pNpc->m_iRegion_X, pNpc->m_iRegion_Z);

    /*    vNpc.Set(pNpc->m_fCurX, 0, pNpc->m_fCurZ);
        if(pNpc->m_byAttackPos)    {
            //EnterCriticalSection( &g_User_critical );
            pUser = m_arUser.GetData(pNpc->m_Target.id);
            if(pUser == NULL) {    
                fprintf(stream, "## Fail ## nid=(%d, %s), att_pos=%d, x=%.2f, z=%.2f\n", pNpc->m_sNid+NPC_BAND, pNpc->m_strName, pNpc->m_byAttackPos, pNpc->m_fCurX, pNpc->m_fCurZ);
                continue;
            }
            vUser.Set(pNpc->m_Target.x, 0, pNpc->m_Target.z);
            fDis = pNpc->GetDistance(vNpc, vUser);
            //fprintf(stream, "[ target : x=%.2f, z=%.2f ] [ user : x=%.2f, z=%.2f ] \n", pNpc->m_Target.x, pNpc->m_Target.z, pUser->m_curx, pUser->m_curz);
            fprintf(stream, "nid=(%d, %s), att_pos=%d, dis=%.2f, x=%.2f, z=%.2f\n", pNpc->m_sNid+NPC_BAND, pNpc->m_strName, pNpc->m_byAttackPos, fDis, pNpc->m_fCurX, pNpc->m_fCurZ);
            //LeaveCriticalSection( &g_User_critical );
        }    */
    //}
    /*
    fprintf(stream, "*****   User List  *****\n");

    for(int i=0; i<MAX_USER; i++)    {
        //pUser = m_ppUserActive[i];
        pUser = m_pUser[i];
        if(pUser == NULL)        continue;
        fprintf(stream, "nid=(%d, %s), zone=%d, x=%.2f, z=%.2f, rx=%d, rz=%d\n", pUser->m_iUserId, pUser->m_strUserID, pUser->m_curZone, pUser->m_curx, pUser->m_curz, pUser->m_sRegionX, pUser->m_sRegionZ);
    }    

    fprintf(stream, "*****   Region List  *****\n");
    for(int k=0; k<m_sTotalMap; k++)    {
        MAP* pMap = g_arZone[k];
        if(pMap == NULL)    continue;
        for( int i=0; i<pMap->m_sizeRegion.cx; i++ ) {
            for( int j=0; j<pMap->m_sizeRegion.cy; j++ ) {
                EnterCriticalSection( &g_User_critical );
                int total_user = pMap->m_ppRegion[i][j].m_RegionUserArray.GetSize();
                int total_mon = pMap->m_ppRegion[i][j].m_RegionNpcArray.GetSize();
                LeaveCriticalSection( &g_User_critical );

                if(total_user > 0 || total_mon > 0)    {
                    fprintf(stream, "rx=%d, rz=%d, user=%d, monster=%d\n", i, j, total_user, total_mon);
                }
            }
        }
    }    */

    fclose(stream);
}

CUser * CServerDlg::GetActiveUserPtr(int index) {
    CUser * pUser = NULL;

    /*    if(index < 0 || index > MAX_USER)    {
        TRACE("### Fail :: User Array Overflow[%d] ###\n", index );
        return NULL;
    }

    EnterCriticalSection( &g_User_critical );

    if ( m_ppUserActive[index] ) {
        LeaveCriticalSection( &g_User_critical );
        TRACE("### Fail : ActiveUser Array Invalid[%d] ###\n", index );
        return NULL;
    }
    else {
        pUser = (CUser *)m_ppUserInActive[index];
        if( !pUser ) {
            LeaveCriticalSection( &g_User_critical );
            TRACE("### Fail : InActiveUser Array Invalid[%d] ###\n", index );
            return NULL;
        }
    }

    m_ppUserActive[index] = pUser;
    m_ppUserInActive[index] = NULL;

    LeaveCriticalSection( &g_User_critical );    */

    return pUser;
}

CNpc * CServerDlg::GetNpcPtr(TCHAR * pNpcName) {
    CNpc * pNpc = NULL;

    int nSize = m_arNpc.GetSize();

    for (int i = 0; i < nSize; i++) {
        pNpc = m_arNpc.GetData(i);
        if (!pNpc) {
            continue;
        }

        if (_tcscmp(pNpc->m_strName, pNpcName) == 0) {
            return pNpc;
        }
    }

    return NULL;
}

//    �߰��� ��ȯ���� �޸𸮸� �����ϱ����� �÷��װ� 0�� ���°͸� �ѱ��.
CNpc * CServerDlg::GetEventNpcPtr() {
    CNpc * pNpc = NULL;
    for (int i = m_TotalNPC; i < m_arNpc.GetSize(); i++) {
        pNpc = m_arNpc.GetData(i);
        if (!pNpc) {
            continue;
        }

        if (pNpc->m_lEventNpc != 0) {
            continue;
        }

        pNpc->m_lEventNpc = 1;

        return pNpc;
    }
    return NULL;
}

int CServerDlg::MonsterSummon(TCHAR * pNpcName, int zone, float fx, float fz) {
    if (zone < 0 || zone > (g_arZone.size() + 1)) {
        TRACE("#### ��ȯ ���� : %s, zoneindex=%d #####\n", pNpcName, zone);
        return -1;
    }

    CNpc * pNpc = GetNpcPtr(pNpcName);
    if (pNpc == NULL) {
        TRACE("��ȯ��  ������ �̸�(%s)�� �߸��Ǿ����ϴ�.\n", pNpcName);
        return -1;
    }

    BOOL bFlag = FALSE;
    bFlag = SetSummonNpcData(pNpc, zone, fx, fz);

    return 1;
}

//    ��ȯ�� ���� ����Ÿ���� �����Ѵ�.
BOOL CServerDlg::SetSummonNpcData(CNpc * pNpc, int zone, float fx, float fz) {
    int    iCount = 0;
    CNpc * pEventNpc = GetEventNpcPtr();

    if (pEventNpc == NULL) {
        TRACE("��ȯ�Ҽ� �ִ� ���� �ִ� 20�����Դϴ�.\n");
        return FALSE;
    }

    CString strMsg = _T("");

    pEventNpc->m_sSid = pNpc->m_sSid; // MONSTER(NPC) Serial ID
    pEventNpc->m_byMoveType = 1;
    pEventNpc->m_byInitMoveType = 1;
    pEventNpc->m_byBattlePos = 0;
    _tcscpy(pEventNpc->m_strName, pNpc->m_strName); // MONSTER(NPC) Name
    pEventNpc->m_sPid = pNpc->m_sPid;               // MONSTER(NPC) Picture ID
    pEventNpc->m_sSize = pNpc->m_sSize;             // ĳ������ ����(100 �ۼ�Ʈ ����)
    pEventNpc->m_iWeapon_1 = pNpc->m_iWeapon_1;     // ���빫��
    pEventNpc->m_iWeapon_2 = pNpc->m_iWeapon_2;     // ���빫��
    pEventNpc->m_byGroup = pNpc->m_byGroup;         // �Ҽ�����
    pEventNpc->m_byActType = pNpc->m_byActType;     // �ൿ����
    pEventNpc->m_byRank = pNpc->m_byRank;           // ����
    pEventNpc->m_byTitle = pNpc->m_byTitle;         // ����
    pEventNpc->m_iSellingGroup = pNpc->m_iSellingGroup;
    pEventNpc->m_sLevel = pNpc->m_sLevel;             // level
    pEventNpc->m_iExp = pNpc->m_iExp;                 // ����ġ
    pEventNpc->m_iLoyalty = pNpc->m_iLoyalty;         // loyalty
    pEventNpc->m_iHP = pNpc->m_iMaxHP;                // �ִ� HP
    pEventNpc->m_iMaxHP = pNpc->m_iMaxHP;             // ���� HP
    pEventNpc->m_sMP = pNpc->m_sMaxMP;                // �ִ� MP
    pEventNpc->m_sMaxMP = pNpc->m_sMaxMP;             // ���� MP
    pEventNpc->m_sAttack = pNpc->m_sAttack;           // ���ݰ�
    pEventNpc->m_sDefense = pNpc->m_sDefense;         // ��
    pEventNpc->m_sHitRate = pNpc->m_sHitRate;         // Ÿ�ݼ�����
    pEventNpc->m_sEvadeRate = pNpc->m_sEvadeRate;     // ȸ�Ǽ�����
    pEventNpc->m_sDamage = pNpc->m_sDamage;           // �⺻ ������
    pEventNpc->m_sAttackDelay = pNpc->m_sAttackDelay; // ���ݵ�����
    pEventNpc->m_sSpeed = pNpc->m_sSpeed;             // �̵��ӵ�
    pEventNpc->m_fSpeed_1 = pNpc->m_fSpeed_1;         // �⺻ �̵� Ÿ��
    pEventNpc->m_fSpeed_2 = pNpc->m_fSpeed_2;         // �ٴ� �̵� Ÿ��..
    pEventNpc->m_fOldSpeed_1 = pNpc->m_fOldSpeed_1;   // �⺻ �̵� Ÿ��
    pEventNpc->m_fOldSpeed_2 = pNpc->m_fOldSpeed_2;   // �ٴ� �̵� Ÿ��..
    pEventNpc->m_fSecForMetor = 4.0f;                 // �ʴ� �� �� �ִ� �Ÿ�..
    pEventNpc->m_sStandTime = pNpc->m_sStandTime;     // ���ִ� �ð�
    pEventNpc->m_iMagic1 = pNpc->m_iMagic1;           // ��븶�� 1
    pEventNpc->m_iMagic2 = pNpc->m_iMagic2;           // ��븶�� 2
    pEventNpc->m_iMagic3 = pNpc->m_iMagic3;           // ��븶�� 3
    pEventNpc->m_sFireR = pNpc->m_sFireR;             // ȭ�� ���׷�
    pEventNpc->m_sColdR = pNpc->m_sColdR;             // �ñ� ���׷�
    pEventNpc->m_sLightningR = pNpc->m_sLightningR;   // ���� ���׷�
    pEventNpc->m_sMagicR = pNpc->m_sMagicR;           // ���� ���׷�
    pEventNpc->m_sDiseaseR = pNpc->m_sDiseaseR;       // ���� ���׷�
    pEventNpc->m_sPoisonR = pNpc->m_sPoisonR;         // �� ���׷�
    pEventNpc->m_sLightR = pNpc->m_sLightR;           // �� ���׷�
    pEventNpc->m_fBulk = pNpc->m_fBulk;
    pEventNpc->m_bySearchRange = pNpc->m_bySearchRange;   // �� Ž�� ����
    pEventNpc->m_byAttackRange = pNpc->m_byAttackRange;   // �����Ÿ�
    pEventNpc->m_byTracingRange = pNpc->m_byTracingRange; // �߰ݰŸ�
    pEventNpc->m_sAI = pNpc->m_sAI;                       // �ΰ����� �ε���
    pEventNpc->m_tNpcType = pNpc->m_tNpcType;             // NPC Type
    pEventNpc->m_byFamilyType = pNpc->m_byFamilyType;     // ������̿��� �������踦 �����Ѵ�.
    pEventNpc->m_iMoney = pNpc->m_iMoney;                 // �������� ��
    pEventNpc->m_iItem = pNpc->m_iItem;                   // �������� ������
    pEventNpc->m_tNpcLongType = pNpc->m_tNpcLongType;
    pEventNpc->m_byWhatAttackType = pNpc->m_byWhatAttackType;

    //////// MONSTER POS ////////////////////////////////////////
    pEventNpc->m_sCurZone = zone;
    pEventNpc->m_fCurX = fx;
    pEventNpc->m_fCurY = 0;
    pEventNpc->m_fCurZ = fz;
    pEventNpc->m_nInitMinX = pNpc->m_nInitMinX;
    pEventNpc->m_nInitMinY = pNpc->m_nInitMinY;
    pEventNpc->m_nInitMaxX = pNpc->m_nInitMaxX;
    pEventNpc->m_nInitMaxY = pNpc->m_nInitMaxY;
    pEventNpc->m_sRegenTime = pNpc->m_sRegenTime; // ��(DB)����-> �и��������
    pEventNpc->m_tItemPer = pNpc->m_tItemPer;     // NPC Type
    pEventNpc->m_tDnPer = pNpc->m_tDnPer;         // NPC Type

    pEventNpc->m_ZoneIndex = -1;

    pEventNpc->m_NpcState = NPC_DEAD; // ���´� ���������� �ؾ� �Ѵ�..
    pEventNpc->m_bFirstLive = 1;      // ó�� ��Ƴ� ���� ����� �Ѵ�..

    for (int i = 0; i < g_arZone.size(); i++) {
        if (g_arZone[i]->m_nZoneNumber == zone) {
            pEventNpc->m_ZoneIndex = i;
            break;
        }
    }

    if (pEventNpc->m_ZoneIndex == -1) {
        TRACE("Invaild zone Index!!\n");
        return FALSE;
    }

    pEventNpc->Init();

    BOOL bSuccess = FALSE;

    int test = 0;

    for (int i = 0; i < NPC_NUM; i++) {
        test = m_arEventNpcThread[0]->m_ThreadInfo.m_byNpcUsed[i];
        TRACE("setsummon == %d, used=%d\n", i, test);
        if (m_arEventNpcThread[0]->m_ThreadInfo.m_byNpcUsed[i] == 0) {
            m_arEventNpcThread[0]->m_ThreadInfo.m_byNpcUsed[i] = 1;
            bSuccess = TRUE;
            m_arEventNpcThread[0]->m_ThreadInfo.pNpc[i] = pEventNpc;
            break;
        }
    }

    if (!bSuccess) {
        pEventNpc->m_lEventNpc = 0;
        TRACE("### ��ȯ�� �����߽��ϴ�. ###\n");
        return FALSE;
    }

    TRACE("*** %d, %s �� ��ȯ�Ͽ����ϴ�. state = %d ***\n", pEventNpc->m_sNid + NPC_BAND, pEventNpc->m_strName,
          pEventNpc->m_NpcState);

    return TRUE;
}

void CServerDlg::TestCode() {
    //InitTrigonometricFunction();

    int random = 0, count_1 = 0, count_2 = 0, count_3 = 0;

    // TestCoding
    for (int i = 0; i < 100; i++) {
        random = myrand(1, 3);
        if (random == 1) {
            count_1++;
        } else if (random == 2) {
            count_2++;
        } else if (random == 3) {
            count_3++;
        }
    }

    //TRACE("$$$ random test == 1=%d, 2=%d, 3=%d,, %d,%s $$$\n", count_1, count_2, count_3, __FILE__, __LINE__);
}

BOOL CServerDlg::GetMagicType1Data() {
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

BOOL CServerDlg::GetMagicType2Data() {
    CMagicType2Set MagicType2Set;

    if (!MagicType2Set.Open()) {
        AfxMessageBox(_T("MagicType1 Open Fail!"));
        return FALSE;
    }
    if (MagicType2Set.IsBOF() || MagicType2Set.IsEOF()) {
        AfxMessageBox(_T("MagicType1 Empty!"));
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

BOOL CServerDlg::GetMagicType3Data() {
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
        //pType3Magic->bDistance = MagicType3Set.m_Distance;
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

BOOL CServerDlg::GetMagicType4Data() {
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

void CServerDlg::RegionCheck() {
    for (int k = 0; k < m_sTotalMap; k++) {
        MAP * pMap = g_arZone[k];
        if (pMap == NULL) {
            continue;
        }
        for (int i = 0; i < pMap->m_sizeRegion.cx; i++) {
            for (int j = 0; j < pMap->m_sizeRegion.cy; j++) {
                EnterCriticalSection(&g_User_critical);
                int total_user = pMap->m_ppRegion[i][j].m_RegionUserArray.GetSize();
                LeaveCriticalSection(&g_User_critical);
                if (total_user > 0) {
                    pMap->m_ppRegion[i][j].m_byMoving = 1;
                } else {
                    pMap->m_ppRegion[i][j].m_byMoving = 0;
                }
            }
        }
    }
}

BOOL CServerDlg::AddObjectEventNpc(_OBJECT_EVENT * pEvent, int zone_number) {
    int nServerNum = GetServerNumber(zone_number);
    //if(m_byZone != zone_number)     return FALSE;
    //if(m_byZone != UNIFY_ZONE)    {
    //    if(m_byZone != nServerNum)     return FALSE;
    //}

    //if( zone_number > 201 )    return FALSE;    // test
    CNpcTable * pNpcTable = m_arNpcTable.GetData(pEvent->sIndex);
    BOOL        bFindNpcTable = FALSE;
    if (pNpcTable == NULL) {
        bFindNpcTable = FALSE;
        TRACE("#### AddObjectEventNpc Fail : [sid = %d], zone=%d #####\n", pEvent->sIndex, zone_number);
        return FALSE;
    }

    bFindNpcTable = TRUE;

    CNpc * pNpc = new CNpc;

    pNpc->m_sNid = m_sMapEventNpc++;      // ���� �������� ���� ��ȣ
    pNpc->m_sSid = (short)pEvent->sIndex; // MONSTER(NPC) Serial ID

    pNpc->m_byMoveType = 100;
    pNpc->m_byInitMoveType = 100;
    bFindNpcTable = FALSE;

    pNpc->m_byMoveType = 0;
    pNpc->m_byInitMoveType = 0;

    pNpc->m_byBattlePos = 0;

    _tcscpy(pNpc->m_strName, pNpcTable->m_strName); // MONSTER(NPC) Name
    pNpc->m_sPid = pNpcTable->m_sPid;               // MONSTER(NPC) Picture ID
    pNpc->m_sSize = pNpcTable->m_sSize;             // ĳ������ ����(100 �ۼ�Ʈ ����)
    pNpc->m_iWeapon_1 = pNpcTable->m_iWeapon_1;     // ���빫��
    pNpc->m_iWeapon_2 = pNpcTable->m_iWeapon_2;     // ���빫��
    pNpc->m_byGroup = pNpcTable->m_byGroup;         // �Ҽ�����
    pNpc->m_byActType = pNpcTable->m_byActType;     // �ൿ����
    pNpc->m_byRank = pNpcTable->m_byRank;           // ����
    pNpc->m_byTitle = pNpcTable->m_byTitle;         // ����
    pNpc->m_iSellingGroup = pNpcTable->m_iSellingGroup;
    pNpc->m_sLevel = pNpcTable->m_sLevel;                // level
    pNpc->m_iExp = pNpcTable->m_iExp;                    // ����ġ
    pNpc->m_iLoyalty = pNpcTable->m_iLoyalty;            // loyalty
    pNpc->m_iHP = pNpcTable->m_iMaxHP;                   // �ִ� HP
    pNpc->m_iMaxHP = pNpcTable->m_iMaxHP;                // ���� HP
    pNpc->m_sMP = pNpcTable->m_sMaxMP;                   // �ִ� MP
    pNpc->m_sMaxMP = pNpcTable->m_sMaxMP;                // ���� MP
    pNpc->m_sAttack = pNpcTable->m_sAttack;              // ���ݰ�
    pNpc->m_sDefense = pNpcTable->m_sDefense;            // ��
    pNpc->m_sHitRate = pNpcTable->m_sHitRate;            // Ÿ�ݼ�����
    pNpc->m_sEvadeRate = pNpcTable->m_sEvadeRate;        // ȸ�Ǽ�����
    pNpc->m_sDamage = pNpcTable->m_sDamage;              // �⺻ ������
    pNpc->m_sAttackDelay = pNpcTable->m_sAttackDelay;    // ���ݵ�����
    pNpc->m_sSpeed = pNpcTable->m_sSpeed;                // �̵��ӵ�
    pNpc->m_fSpeed_1 = (float)pNpcTable->m_bySpeed_1;    // �⺻ �̵� Ÿ��
    pNpc->m_fSpeed_2 = (float)pNpcTable->m_bySpeed_2;    // �ٴ� �̵� Ÿ��..
    pNpc->m_fOldSpeed_1 = (float)pNpcTable->m_bySpeed_1; // �⺻ �̵� Ÿ��
    pNpc->m_fOldSpeed_2 = (float)pNpcTable->m_bySpeed_2; // �ٴ� �̵� Ÿ��..
    pNpc->m_fSecForMetor = 4.0f;                         // �ʴ� �� �� �ִ� �Ÿ�..
    pNpc->m_sStandTime = pNpcTable->m_sStandTime;        // ���ִ� �ð�
    pNpc->m_iMagic1 = pNpcTable->m_iMagic1;              // ��븶�� 1
    pNpc->m_iMagic2 = pNpcTable->m_iMagic2;              // ��븶�� 2
    pNpc->m_iMagic3 = pNpcTable->m_iMagic3;              // ��븶�� 3
    pNpc->m_sFireR = pNpcTable->m_sFireR;                // ȭ�� ���׷�
    pNpc->m_sColdR = pNpcTable->m_sColdR;                // �ñ� ���׷�
    pNpc->m_sLightningR = pNpcTable->m_sLightningR;      // ���� ���׷�
    pNpc->m_sMagicR = pNpcTable->m_sMagicR;              // ���� ���׷�
    pNpc->m_sDiseaseR = pNpcTable->m_sDiseaseR;          // ���� ���׷�
    pNpc->m_sPoisonR = pNpcTable->m_sPoisonR;            // �� ���׷�
    pNpc->m_sLightR = pNpcTable->m_sLightR;              // �� ���׷�
    pNpc->m_fBulk = (float)(((double)pNpcTable->m_sBulk / 100) * ((double)pNpcTable->m_sSize / 100));
    pNpc->m_bySearchRange = pNpcTable->m_bySearchRange;   // �� Ž�� ����
    pNpc->m_byAttackRange = pNpcTable->m_byAttackRange;   // �����Ÿ�
    pNpc->m_byTracingRange = pNpcTable->m_byTracingRange; // �߰ݰŸ�
    pNpc->m_sAI = pNpcTable->m_sAI;                       // �ΰ����� �ε���
    pNpc->m_tNpcType = pNpcTable->m_tNpcType;             // NPC Type
    pNpc->m_byFamilyType = pNpcTable->m_byFamilyType;     // ������̿��� �������踦 �����Ѵ�.
    pNpc->m_iMoney = pNpcTable->m_iMoney;                 // �������� ��
    pNpc->m_iItem = pNpcTable->m_iItem;                   // �������� ������
    pNpc->m_tNpcLongType = pNpcTable->m_byDirectAttack;
    pNpc->m_byWhatAttackType = pNpcTable->m_byDirectAttack;

    //////// MONSTER POS ////////////////////////////////////////

    pNpc->m_sCurZone = zone_number;

    pNpc->m_byGateOpen = pEvent->sStatus;
    pNpc->m_fCurX = pEvent->fPosX;
    pNpc->m_fCurY = pEvent->fPosY;
    pNpc->m_fCurZ = pEvent->fPosZ;

    pNpc->m_nInitMinX = pEvent->fPosX - 1;
    pNpc->m_nInitMinY = pEvent->fPosZ - 1;
    pNpc->m_nInitMaxX = pEvent->fPosX + 1;
    pNpc->m_nInitMaxY = pEvent->fPosZ + 1;

    pNpc->m_sRegenTime = 10000 * 1000; // ��(DB)����-> �и��������
    //pNpc->m_sRegenTime        = 30 * 1000;    // ��(DB)����-> �и��������
    pNpc->m_sMaxPathCount = 0;
    pNpc->m_tItemPer = pNpcTable->m_tItemPer; // NPC Type
    pNpc->m_tDnPer = pNpcTable->m_tDnPer;     // NPC Type

    pNpc->m_ZoneIndex = -1;
    pNpc->m_byObjectType = SPECIAL_OBJECT;
    pNpc->m_bFirstLive = 1; // ó�� ��Ƴ� ���� ����� �Ѵ�..
    //pNpc->m_ZoneIndex = GetZoneIndex(pNpc->m_sCurZone);
    /*
    if(pNpc->m_ZoneIndex == -1)    {
        AfxMessageBox("Invaild zone Index!!");
        return FALSE;
    }    */

    //pNpc->Init();
    if (!m_arNpc.PutData(pNpc->m_sNid, pNpc)) {
        TRACE("Npc PutData Fail - %d\n", pNpc->m_sNid);
        delete pNpc;
        pNpc = NULL;
    }

    m_TotalNPC = m_sMapEventNpc;

    return TRUE;
}

int CServerDlg::GetZoneIndex(int zonenumber) {
    int t_count = g_arZone.size();
    for (int i = 0; i < t_count; i++) {
        if (g_arZone[i] && (zonenumber == g_arZone[i]->m_nZoneNumber)) {
            return i;
        }
    }

    return -1;
}

int CServerDlg::GetServerNumber(int zonenumber) {
    int t_count = g_arZone.size();
    for (int i = 0; i < t_count; i++) {
        if (g_arZone[i] && (zonenumber == g_arZone[i]->m_nZoneNumber)) {
            return g_arZone[i]->m_nServerNo;
        }
    }

    return -1;
}

void CServerDlg::ClostSocket(int zonenumber) {
    CGameSocket * pSocket = NULL;

    for (int i = 0; i < MAX_SOCKET; i++) {
        pSocket = (CGameSocket *)m_Iocport.m_SockArray[i];
        if (pSocket == NULL) {
            continue;
        }
        if (pSocket->m_sSocketID == zonenumber) {
            //TRACE("size = %d, socket_num = %d, i=%d \n", size, pSocket->m_sSocketID, i);
            pSocket->CloseProcess();
            m_Iocport.RidIOCPSocket(pSocket->GetSocketID(), pSocket);
        }
    }
}

void CServerDlg::GetServerInfoIni() {
    CIni inifile;
    inifile.SetPath("server.ini");
    m_byZone = inifile.GetProfileInt("SERVER", "ZONE", 1);
    m_byTestMode = inifile.GetProfileInt("TEST", "MODE", 0);
}

void CServerDlg::SendSystemMsg(char * pMsg, int zone, int type, int who) {
    int  send_index = 0;
    char buff[256];
    memset(buff, 0x00, 256);
    short sLength = _tcslen(pMsg);

    SetByte(buff, AG_SYSTEM_MSG, send_index);
    SetByte(buff, type, send_index); // ä������
    SetShort(buff, who, send_index); // ��������
    SetShort(buff, sLength, send_index);
    SetString(buff, pMsg, sLength, send_index);

    Send(buff, send_index, zone);
}

void CServerDlg::ResetBattleZone() {
    TRACE("ServerDlg - ResetBattleZone() : start \n");
    for (int i = 0; i < g_arZone.size(); i++) {
        MAP * pMap = g_arZone[i];
        if (!pMap) {
            continue;
        }
        if (pMap->m_byRoomEvent == 0) {
            continue; // ������ ���� ��������ϴ� ���� �ƴϸ� ����..
        }
        //if( pMap->IsRoomStatusCheck() == TRUE )    continue;    // ��ü���� Ŭ���� �Ǿ��ٸ�
        pMap->InitializeRoom();
    }
    TRACE("ServerDlg - ResetBattleZone() : end \n");
}