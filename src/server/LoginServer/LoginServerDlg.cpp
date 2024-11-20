// LoginServerDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "LoginServer.h"
#include "LoginServerDlg.h"
#include "IOCPSocket2.h"
#include "VersionSet.h"
#include "SettingDlg.h"
#include "User.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIOCPort CLoginServerDlg::m_Iocport;

/////////////////////////////////////////////////////////////////////////////
// CLoginServerDlg dialog

CLoginServerDlg * CLoginServerDlg::s_pInstance = nullptr;

CLoginServerDlg::CLoginServerDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CLoginServerDlg::IDD, pParent) {
    s_pInstance = this;

    m_nLastVersion = 0;

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

    CString szConnStr = ConnectionStringLogin();
    if (!m_DBProcess.InitDatabase(szConnStr)) {
        AfxMessageBox("Database Connection Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    if (!m_DBProcess.LoadVersionList()) {
        AfxMessageBox("Load Version List Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    m_OutputList.AddString(szConnStr);
    std::string szVersion = std::format("Latest Version : {:d}", m_nLastVersion);
    m_OutputList.AddString(szVersion.c_str());

    ::ResumeThread(m_Iocport.m_hAcceptThread);

    return TRUE; // return TRUE  unless you set the focus to a control
}

BOOL CLoginServerDlg::GetInfoFromIni() {
    CIni ini("Server.ini");

    m_szFtpUrl = ini.GetString("DOWNLOAD", "URL", "ftp.your-site.net");
    m_szFtpPath = ini.GetString("DOWNLOAD", "PATH", "/");

    m_szOdbcLogDsn = ini.GetString("ODBC", "LOG_DSN", "kodb");
    m_szOdbcLogUid = ini.GetString("ODBC", "LOG_UID", "kodb_user");
    m_szOdbcLogPwd = ini.GetString("ODBC", "LOG_PWD", "kodb_user");
    m_szVersionTable = ini.GetString("ODBC", "TABLE", "VERSION");

    m_fsDefaultDir = ini.GetString("CONFIGURATION", "DEFAULT_PATH", "");
    if (!m_fsDefaultDir.is_absolute()) {
        m_fsDefaultDir = n3std::get_app_dir().normalize();
    }

    m_nServerCount = ini.GetInt("SERVER_LIST", "COUNT", 1);

    if (m_szFtpUrl.empty() || m_szFtpPath.empty()) {
        return FALSE;
    }
    if (m_szOdbcLogDsn.empty() || m_szOdbcLogUid.empty() || m_szOdbcLogPwd.empty() || m_szVersionTable.empty()) {
        return FALSE;
    }
    if (m_nServerCount <= 0) {
        return FALSE;
    }

    m_ServerList.reserve(20);
    for (int i = 0; i < m_nServerCount; i++) {
        _SERVER_INFO * pInfo = new _SERVER_INFO;
        std::string    szKeyIp = std::format("SERVER_{:02d}", i);
        ini.GetString("SERVER_LIST", szKeyIp, "127.0.0.1").copy(pInfo->strServerIP, sizeof(pInfo->strServerIP) - 1);

        std::string szKeyServerName = std::format("NAME_{:02d}", i);
        ini.GetString("SERVER_LIST", szKeyServerName, szKeyIp)
            .copy(pInfo->strServerName, sizeof(pInfo->strServerName) - 1);
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
    CSettingDlg dlg(m_nLastVersion, this);
    dlg.m_fsDefaultDir = m_fsDefaultDir;
    if (dlg.DoModal() == IDOK) {
        m_fsDefaultDir = dlg.m_fsDefaultDir;

        CIni ini("Server.ini");
        ini.SetString("CONFIGURATION", "DEFAULT_PATH", m_fsDefaultDir.string().c_str());
    }
}

CString CLoginServerDlg::ConnectionStringLogin() const {
    static CString szConnStr =
        std::format("ODBC;DSN={:s};UID={:s};PWD={:s}", m_szOdbcLogDsn, m_szOdbcLogUid, m_szOdbcLogPwd).c_str();

    return szConnStr;
}
