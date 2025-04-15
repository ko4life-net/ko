// LoginServerDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "LoginServer.h"
#include "LoginServerDlg.h"
#include "IOCPSocket2.h"
#include "VersionSet.h"
#include "SettingDlg.h"
#include "User.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIOCPort CLoginServerDlg::m_Iocport;

/////////////////////////////////////////////////////////////////////////////
// CLoginServerDlg dialog

CLoginServerDlg::CLoginServerDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CLoginServerDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CLoginServerDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_nLastVersion = 0;
    memset(m_ODBCName, 0, sizeof(m_ODBCName));
    memset(m_ODBCLogin, 0, sizeof(m_ODBCLogin));
    memset(m_ODBCPwd, 0, sizeof(m_ODBCPwd));
    memset(m_TableName, 0, sizeof(m_TableName));

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginServerDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLoginServerDlg)
    DDX_Control(pDX, IDC_LIST1, m_OutputList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginServerDlg, CDialog)
//{{AFX_MSG_MAP(CLoginServerDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_SETTING, OnVersionSetting)
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginServerDlg message handlers

BOOL CLoginServerDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    m_Iocport.Init(MAX_USER, CLIENT_SOCKSIZE, 1);

    for (int i = 0; i < MAX_USER; i++) {
        m_Iocport.m_SockArrayInActive[i] = new CUser;
    }

    if (!m_Iocport.Listen(_LISTEN_PORT)) {
        AfxMessageBox("FAIL TO CREATE LISTEN STATE");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    if (!GetInfoFromIni()) {
        AfxMessageBox("Ini File Info Error!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    char szConnectionString[256]{};
    sprintf(szConnectionString, "ODBC;DSN=%s;UID=%s;PWD=%s", m_ODBCName, m_ODBCLogin, m_ODBCPwd);
    if (!m_DBProcess.InitDatabase(szConnectionString)) {
        AfxMessageBox("Database Connection Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    if (!m_DBProcess.LoadVersionList()) {
        AfxMessageBox("Load Version List Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    if (!m_DBProcess.LoadNews()) {
        AfxMessageBox("Load News Data Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    StartNewsRefreshTimer();

    m_OutputList.AddString(szConnectionString);
    std::string szVersion = std::format("Latest Version : {:d}", m_nLastVersion);
    m_OutputList.AddString(szVersion.c_str());

    ::ResumeThread(m_Iocport.m_hAcceptThread);

    return TRUE; // return TRUE  unless you set the focus to a control
}

BOOL CLoginServerDlg::GetInfoFromIni() {
    std::string  szIniFile = (n3std::get_app_dir() / "Version.ini").string();
    const char * pszIniFile = szIniFile.c_str();

    DWORD dwAttrib = GetFileAttributesA(pszIniFile);
    if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
        WritePrivateProfileString("DOWNLOAD", "URL", "ftp.your-site.net", pszIniFile);
        WritePrivateProfileString("DOWNLOAD", "PATH", "/", pszIniFile);
        WritePrivateProfileString("ODBC", "DSN", "kodb", pszIniFile);
        WritePrivateProfileString("ODBC", "UID", "kodb_user", pszIniFile);
        WritePrivateProfileString("ODBC", "PWD", "kodb_user", pszIniFile);
        WritePrivateProfileString("ODBC", "TABLE", "VERSION", pszIniFile);
        WritePrivateProfileString("CONFIGURATION", "DEFAULT_PATH", "", pszIniFile);
        WritePrivateProfileString("CONFIGURATION", "NewsRefreshInterval", "30", pszIniFile);
        WritePrivateProfileString("SERVER_LIST", "COUNT", "1", pszIniFile);
        WritePrivateProfileString("SERVER_LIST", "SERVER_00", "127.0.0.1", pszIniFile);
        WritePrivateProfileString("SERVER_LIST", "NAME_00", "SERVER_01", pszIniFile);
    }

    char szBuff[500]{};
    GetPrivateProfileString("DOWNLOAD", "URL", "ftp.your-site.net", szBuff, sizeof(szBuff), pszIniFile);
    m_szFtpUrl = szBuff;

    memset(szBuff, 0, sizeof(szBuff));
    GetPrivateProfileString("DOWNLOAD", "PATH", "/", szBuff, sizeof(szBuff), pszIniFile);
    m_szFtpPath = szBuff;

    GetPrivateProfileString("ODBC", "DSN", "kodb", m_ODBCName, sizeof(m_ODBCName), pszIniFile);
    GetPrivateProfileString("ODBC", "UID", "kodb_user", m_ODBCLogin, sizeof(m_ODBCLogin), pszIniFile);
    GetPrivateProfileString("ODBC", "PWD", "kodb_user", m_ODBCPwd, sizeof(m_ODBCPwd), pszIniFile);
    GetPrivateProfileString("ODBC", "TABLE", "VERSION", m_TableName, sizeof(m_TableName), pszIniFile);

    memset(szBuff, 0, sizeof(szBuff));
    GetPrivateProfileString("CONFIGURATION", "DEFAULT_PATH", "", szBuff, sizeof(szBuff), pszIniFile);
    m_fsDefaultDir = szBuff;
    if (!m_fsDefaultDir.is_absolute()) {
        m_fsDefaultDir = n3std::get_app_dir().normalize();
    }

    m_nServerCount = GetPrivateProfileInt("SERVER_LIST", "COUNT", 1, pszIniFile);

    if (m_szFtpUrl.empty() || m_szFtpPath.empty()) {
        return FALSE;
    }
    if (!strlen(m_ODBCName) || !strlen(m_ODBCLogin) || !strlen(m_ODBCPwd) || !strlen(m_TableName)) {
        return FALSE;
    }
    if (m_nServerCount <= 0) {
        return FALSE;
    }

    m_NewsRefreshInterval = GetPrivateProfileInt("CONFIGURATION", "NewsRefreshInterval", -1, pszIniFile);
    if (m_NewsRefreshInterval <= 0) {
        m_NewsRefreshInterval = 15;
    }

    m_ServerList.reserve(20);
    for (int i = 0; i < m_nServerCount; i++) {
        _SERVER_INFO * pInfo = new _SERVER_INFO;
        std::string    szKeyIp = std::format("SERVER_{:02d}", i);
        GetPrivateProfileString("SERVER_LIST", szKeyIp.c_str(), "127.0.0.1", pInfo->strServerIP,
                                sizeof(pInfo->strServerIP), pszIniFile);

        std::string szKeyServerName = std::format("NAME_{:02d}", i);
        GetPrivateProfileString("SERVER_LIST", szKeyServerName.c_str(), szKeyIp.c_str(), pInfo->strServerName,
                                sizeof(pInfo->strServerName), pszIniFile);
        m_ServerList.emplace_back(pInfo);
    }

    return TRUE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginServerDlg::OnPaint() {
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
HCURSOR CLoginServerDlg::OnQueryDragIcon() {
    return (HCURSOR)m_hIcon;
}

BOOL CLoginServerDlg::PreTranslateMessage(MSG * pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLoginServerDlg::DestroyWindow() {
    if (!m_VersionList.IsEmpty()) {
        m_VersionList.DeleteAllData();
    }

    for (int i = 0; i < m_ServerList.size(); i++) {
        delete m_ServerList[i];
    }
    m_ServerList.clear();

    return CDialog::DestroyWindow();
}

void CLoginServerDlg::OnVersionSetting() {
    fs::path    fsIniPath = n3std::get_app_dir() / "Version.ini";
    CSettingDlg dlg(m_nLastVersion, this);
    dlg.m_fsDefaultDir = m_fsDefaultDir;
    if (dlg.DoModal() == IDOK) {
        m_fsDefaultDir = dlg.m_fsDefaultDir;
        WritePrivateProfileStringW(L"CONFIGURATION", L"DEFAULT_PATH", m_fsDefaultDir.c_str(), fsIniPath.c_str());
    }
}

void CLoginServerDlg::StartNewsRefreshTimer() {
    if (m_NewsRefreshTimerID != 0) {
        KillTimer(m_NewsRefreshTimerID);
    }

    m_NewsRefreshTimerID = SetTimer(1, m_NewsRefreshInterval * 60 * 1000, NULL);
    if (m_NewsRefreshTimerID == 0) {
        TRACE("Timer failed to set!");
    }
}

void CLoginServerDlg::RefreshNewsData() {
    m_ServerNews.clear();
    if (m_DBProcess.LoadNews()) {
        TRACE("News Data Refreshed Successfully!");
    } else {
        TRACE("Failed to Refresh News Data!");
    }
}

void CLoginServerDlg::OnTimer(UINT_PTR nIDEvent) {
    if (nIDEvent == m_NewsRefreshTimerID) {
        RefreshNewsData();
    }
    CDialog::OnTimer(nIDEvent);
}
