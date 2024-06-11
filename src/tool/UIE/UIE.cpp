// UIE.cpp : Defines the class behaviors for the application.
//

#include "StdAfx.h"
#include "UIE.h"

#include "MainFrm.h"
#include "UIEDoc.h"
#include "HierarchyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIEApp

BEGIN_MESSAGE_MAP(CUIEApp, CWinApp)
//{{AFX_MSG_MAP(CUIEApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIEApp construction

CUIEApp::CUIEApp() {
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUIEApp object

CUIEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUIEApp initialization

static BOOL NEAR PASCAL SetRegKey(LPCSTR lpszKey, LPCSTR lpszValue) {
    if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ, lpszValue, lstrlen(lpszValue)) != ERROR_SUCCESS) {
        TRACE1("Warning: registration database update failed for key '%Fs'\n", lpszKey);
        return FALSE;
    }

    return TRUE;
}

BOOL CUIEApp::InitInstance() {
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    //    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    SetRegistryKey(_T("NoahSystem"));

    LoadStdProfileSettings(); // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    CSingleDocTemplate * pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CUIEDoc),
                                          RUNTIME_CLASS(CMainFrame), // main SDI frame window
                                          RUNTIME_CLASS(CHierarchyView));
    AddDocTemplate(pDocTemplate);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo)) {
        return FALSE;
    }

    EnableShellOpen();
    RegisterShellFileTypes();

    CString strFileTypeName;

    pDocTemplate->GetDocString(strFileTypeName, CDocTemplate::regFileTypeId);
    SetRegKey(".uif", strFileTypeName);

    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
    // No message handlers
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

// App command to run the dialog
void CUIEApp::OnAppAbout() {
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CUIEApp message handlers

// global function
BOOL SelectTexture(char * pszBuff) {
    if (NULL == pszBuff) {
        return FALSE;
    }
    pszBuff[0] = NULL;
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    DWORD        dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog  dlg(TRUE, "dxt", NULL, dwFlags, "Generic Image Files(*.bmp, *.tga, *.dxt)|*.bmp;*.tga;*.dxt||", NULL);
    dlg.m_ofn.lpstrInitialDir = pFrm->m_Eng.PathGet().c_str();
    dlg.m_ofn.lpstrTitle = "Select texture file";
    if (IDCANCEL == dlg.DoModal()) {
        return FALSE;
    }
    lstrcpy(pszBuff, dlg.GetPathName());

    //    CN3BaseFileAccess tmpBase;
    //    tmpBase.FileNameSet(pszBuff);    // Base��ο� ���ؼ� ����� ��θ� �Ѱ��ش�.
    //    const std::string strFN(tmpBase.FileName());
    //    strcpy(pszBuff, strFN.c_str());
    return TRUE;
}
