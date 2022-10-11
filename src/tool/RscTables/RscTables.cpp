// RscTables.cpp : Defines the class behaviors for the application.
//

#include "StdAfx.h"
#include "RscTables.h"
#include "RscTablesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRscTablesApp

BEGIN_MESSAGE_MAP(CRscTablesApp, CWinApp)
//{{AFX_MSG_MAP(CRscTablesApp)
//}}AFX_MSG_MAP
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRscTablesApp construction

CRscTablesApp::CRscTablesApp() {
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRscTablesApp object

CRscTablesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRscTablesApp initialization

BOOL CRscTablesApp::InitInstance() {
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    CRscTablesDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
