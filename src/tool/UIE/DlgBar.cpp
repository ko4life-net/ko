// DlgBar.cpp : implementation file
//

#include "StdAfx.h"
#include "UIE.h"
#include "DlgBar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBar dialog

CDlgBar::CDlgBar(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgBar::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgBar)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgBar::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBar)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgBar, CDialog)
//{{AFX_MSG_MAP(CDlgBar)
ON_BN_CLICKED(IDC_BUTTON_BASEPATH, OnButtonBasepath)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBar message handlers

void CDlgBar::OnButtonBasepath() {
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (NULL == pFrm) {
        return;
    }

    std::string szCurDir = CN3Base::PathGet();

    CFolderPickerDialog dlg;
    dlg.m_ofn.lpstrTitle = "Please select the resource base path.";
    dlg.m_ofn.lpstrInitialDir = szCurDir.c_str();
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    std::string szDir = dlg.GetPathName().GetString();
    pFrm->SetBasePath(szDir.c_str());

    CWinApp * pApp = AfxGetApp();
    ASSERT(pApp);
    pApp->WriteProfileString("Work", "Path", szDir.c_str());
}
