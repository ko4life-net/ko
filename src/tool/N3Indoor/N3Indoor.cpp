// N3Indoor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "N3Indoor.h"

#include "MainFrm.h"
#include "N3IndoorDoc.h"
#include "N3IndoorView.h"

#include <afxadv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorApp

BEGIN_MESSAGE_MAP(CN3IndoorApp, CWinApp)
	//{{AFX_MSG_MAP(CN3IndoorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorApp construction

CN3IndoorApp::CN3IndoorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bActive = true;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CN3IndoorApp object

CN3IndoorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorApp initialization

BOOL CN3IndoorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CN3IndoorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CN3IndoorView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CN3IndoorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorApp message handlers


BOOL CN3IndoorApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (CWinApp::OnIdle(lCount))
	  return TRUE;   

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	if ((lCount %100000) < 3)
	{
		if (pFrm && pFrm->GetIndoorView() )
			pFrm->GetIndoorView()->TickRender();
	}

	return TRUE;
}

void CN3IndoorApp::Write(const char* lpszFormat, ...)
{
	std::string s_szFileName = "log.txt";

	static char szFinal[1024];
	static SYSTEMTIME time;
	GetLocalTime(&time);
	szFinal[0] = NULL;

	DWORD dwRWC = 0;
	sprintf(szFinal, "    [%.2d시 %.2d분 %.2d초] ", time.wHour, time.wMinute, time.wSecond);

	static char szBuff[1024];
	szBuff[0] = NULL;
	va_list argList;
	va_start(argList, lpszFormat);
	vsprintf(szBuff, lpszFormat, argList);
	va_end(argList);

	lstrcat(szFinal, szBuff);
	lstrcat(szFinal, "\r\n");
	int iLength = lstrlen(szFinal);

	HANDLE hFile = CreateFile(s_szFileName.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		hFile = CreateFile(s_szFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hFile) hFile = NULL;
	}

	if(hFile)
	{
		::SetFilePointer(hFile, 0, NULL, FILE_END); // 추가 하기 위해서 파일의 끝으로 옮기고..

		WriteFile(hFile, szFinal, iLength, &dwRWC, NULL);
		CloseHandle(hFile);
	}
}

int CN3IndoorApp::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::Run();
}

std::string CN3IndoorApp::GetMRU1()
{
	m_pRecentFileList->ReadList();
	std::string str;
	CString cstr;
	if (m_pRecentFileList->GetSize() < 1)
		return str;

	cstr = (*(m_pRecentFileList))[0];
	str = cstr;

	return str;
}

std::string CN3IndoorApp::GetMRU2()
{
	m_pRecentFileList->ReadList();
	std::string str = "";
	CString cstr;
	if (m_pRecentFileList->GetSize() < 2)
		return str;

	cstr = (*(m_pRecentFileList))[1];
	str = cstr;

	return str;
}

std::string CN3IndoorApp::GetMRU3()
{
	m_pRecentFileList->ReadList();
	std::string str = "";
	CString cstr;
	if (m_pRecentFileList->GetSize() < 3)
		return str;

	cstr = (*(m_pRecentFileList))[2];
	str = cstr;

	return str;
}

std::string CN3IndoorApp::GetMRU4()
{
	m_pRecentFileList->ReadList();
	std::string str = "";
	CString cstr;
	if (m_pRecentFileList->GetSize() < 4)
		return str;

	cstr = (*(m_pRecentFileList))[3];
	str = cstr;

	return str;
}

void CN3IndoorApp::UpdateMRU()
{
	m_pRecentFileList->WriteList();
}
