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

    memset(m_strFtpUrl, NULL, 256);
    memset(m_strFilePath, NULL, 256);
    memset(m_strDefaultPath, NULL, _MAX_PATH);
    m_nLastVersion = 0;
    memset(m_ODBCName, NULL, 32);
    memset(m_ODBCLogin, NULL, 32);
    memset(m_ODBCPwd, NULL, 32);
    memset(m_TableName, NULL, 32);

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

    char strconnection[256];
    memset(strconnection, NULL, 256);
    sprintf(strconnection, "ODBC;DSN=%s;UID=%s;PWD=%s", m_ODBCName, m_ODBCLogin, m_ODBCPwd);
    if (!m_DBProcess.InitDatabase(strconnection)) {
        AfxMessageBox("Database Connection Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }
    if (!m_DBProcess.LoadVersionList()) {
        AfxMessageBox("Load Version List Fail!!");
        AfxPostQuitMessage(0);
        return FALSE;
    }

    m_OutputList.AddString(strconnection);
    CString version;
    version.Format("Latest Version : %d", m_nLastVersion);
    m_OutputList.AddString(version);

    ::ResumeThread(m_Iocport.m_hAcceptThread);

    return TRUE; // return TRUE  unless you set the focus to a control
}

BOOL CLoginServerDlg::GetInfoFromIni() {
    int     errorcode = 0;
    CString errorstr, inipath;

    inipath.Format("%s\\Version.ini", GetProgPath());
    GetPrivateProfileString("DOWNLOAD", "URL", "ftp.your-site.net", m_strFtpUrl, 256, inipath);
    GetPrivateProfileString("DOWNLOAD", "PATH", "/", m_strFilePath, 256, inipath);

    GetPrivateProfileString("ODBC", "DSN", "kodb", m_ODBCName, 32, inipath);
    GetPrivateProfileString("ODBC", "UID", "kodb_user", m_ODBCLogin, 32, inipath);
    GetPrivateProfileString("ODBC", "PWD", "kodb_user", m_ODBCPwd, 32, inipath);
    GetPrivateProfileString("ODBC", "TABLE", "VERSION", m_TableName, 32, inipath);
    GetPrivateProfileString("CONFIGURATION", "DEFAULT_PATH", "", m_strDefaultPath, 256, inipath);

    m_nServerCount = GetPrivateProfileInt("SERVER_LIST", "COUNT", 1, inipath);

    if (!strlen(m_strFtpUrl) || !strlen(m_strFilePath)) {
        return FALSE;
    }
    if (!strlen(m_ODBCName) || !strlen(m_ODBCLogin) || !strlen(m_ODBCPwd) || !strlen(m_TableName)) {
        return FALSE;
    }
    if (m_nServerCount <= 0) {
        return FALSE;
    }

    char ipkey[20];
    memset(ipkey, 0x00, 20);
    char namekey[20];
    memset(namekey, 0x00, 20);
    _SERVER_INFO * pInfo = NULL;

    m_ServerList.reserve(20);
    for (int i = 0; i < m_nServerCount; i++) {
        pInfo = new _SERVER_INFO;
        sprintf(ipkey, "SERVER_%02d", i);
        sprintf(namekey, "NAME_%02d", i);
        GetPrivateProfileString("SERVER_LIST", ipkey, "127.0.0.1", pInfo->strServerIP, 32, inipath);
        GetPrivateProfileString("SERVER_LIST", namekey, ipkey, pInfo->strServerName, 32, inipath);
        m_ServerList.push_back(pInfo);
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
    CString errorstr, inipath;
    inipath.Format("%s\\Version.ini", GetProgPath());
    CSettingDlg setdlg(m_nLastVersion, this);

    strcpy(setdlg.m_strDefaultPath, m_strDefaultPath);
    if (setdlg.DoModal() == IDOK) {
        strcpy(m_strDefaultPath, setdlg.m_strDefaultPath);
        WritePrivateProfileString("CONFIGURATION", "DEFAULT_PATH", m_strDefaultPath, inipath);
    }
}
