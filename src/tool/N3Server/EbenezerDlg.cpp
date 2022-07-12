// EbenezerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ebenezer.h"
#include "EbenezerDlg.h"
#include "Define.h"
#include "User.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIOCPort	CEbenezerDlg::m_Iocport;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
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
// CEbenezerDlg dialog

CEbenezerDlg::CEbenezerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEbenezerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEbenezerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nCount = 0;
}

void CEbenezerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEbenezerDlg)
	DDX_Control(pDX, IDC_COUNT_EDIT, m_CountCtrl);
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

BOOL CEbenezerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_Iocport.Init( MAX_USER, 1 );
	
	for(int i=0; i<MAX_USER; i++) {
		m_Iocport.m_SockArrayInActive[i] = new CUser;
	}

	if ( !m_Iocport.Listen( _LISTEN_PORT ) )
	{
		AfxMessageBox("FAIL TO CREATE LISTEN STATE", MB_OK);
	}

	CFile file;
	CString szFullPath, errormsg;
	szFullPath.Format(".\\MAP\\%s", "test.smd");
	if (!file.Open(szFullPath, CFile::modeRead))
	{
		errormsg.Format( "파일 Open 실패 - %s\n", szFullPath );
		AfxMessageBox(errormsg);
		return FALSE;
	}

	if( m_Map.LoadMap( (HANDLE)file.m_hFile ) == FALSE ) {
		errormsg.Format( "Map Load 실패 - %s\n", szFullPath );
		AfxMessageBox(errormsg);
		return FALSE;
	}

	file.Close();

	::ResumeThread( m_Iocport.m_hAcceptThread );

	SetTimer( 1, 5000, NULL );

	for(i=0; i<5; i++) {
		m_pInitPos[i].initx = 640.0f + 5*i;
		m_pInitPos[i].initz = 590.0f + 5*i;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEbenezerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEbenezerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEbenezerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CEbenezerDlg::DestroyWindow() 
{
	KillTimer( 1 );
	
	return CDialog::DestroyWindow();
}

CUser* CEbenezerDlg::GetUserPtr(char *userid)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser ) {
			if( !_strnicmp( pUser->m_UserId, userid, MAX_ID_SIZE ) )
				break;
		}
	}

	return pUser;
}

void CEbenezerDlg::OnTimer(UINT nIDEvent) 
{
	CString count;
	count.Format("%d", m_nCount);
	m_CountCtrl.SetWindowText(count);
	m_CountCtrl.UpdateWindow();

	CDialog::OnTimer(nIDEvent);
}

void CEbenezerDlg::Send_All(char *pBuf, int len, BOOL tome)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() == STATE_GAMESTART) ) {
			pUser->Send( pBuf, len );
		}
	}
}

void CEbenezerDlg::Send_Region(char *pBuf, int len, int x, int z, BOOL tome)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() == STATE_GAMESTART) ) {
			if( pUser->m_RegionX == x && pUser->m_RegionZ == z )
				pUser->Send( pBuf, len );
		}
	}
}

int CEbenezerDlg::GetRegionCount(int x, int z)
{
	CUser* pUser = NULL;
	int count = 0;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() != STATE_GAMESTART) ) {
			if( pUser->m_RegionX == x && pUser->m_RegionZ == z )
				count++;
		}
	}

	return count;
}

void CEbenezerDlg::UserInOutForMe(CUser *pSendUser)
{
	int send_index = 0;
	CUser* pUser = NULL;
	char buff[1024];
	memset( buff, 0x00, 1024 );

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && pUser->GetSocketID() == pSendUser->GetSocketID() )
			continue;
		if( pUser && pUser->GetState() != STATE_DISCONNECTED ) {
			SetByte( buff, WIZ_USERINOUT, send_index );
			SetByte( buff, 0x01, send_index );
			SetShort( buff, pUser->GetSocketID(), send_index );
			SetShort( buff, strlen(pUser->m_UserId), send_index );
			SetString( buff, pUser->m_UserId, strlen(pUser->m_UserId), send_index );
			Setfloat( buff, pUser->m_curx, send_index );
			Setfloat( buff, pUser->m_curz, send_index );

			pSendUser->Send( buff, send_index );
		}
	}
}
