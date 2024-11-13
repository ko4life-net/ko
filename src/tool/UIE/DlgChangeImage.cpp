// DlgChangeImage.cpp : implementation file
//

#include "StdAfx.h"
#include "UIE.h"
#include "DlgChangeImage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeImage dialog

CDlgChangeImage::CDlgChangeImage(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgChangeImage::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgChangeImage)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgChangeImage::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgChangeImage)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgChangeImage, CDialog)
//{{AFX_MSG_MAP(CDlgChangeImage)
ON_BN_CLICKED(IDC_B_BROWSE_OLD_FILE, OnBBrowseOldFile)
ON_BN_CLICKED(IDC_B_BROWSE_NEW_FILE, OnBBrowseNewFile)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeImage message handlers

void CDlgChangeImage::OnBBrowseOldFile() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, "dxt", NULL, dwFlags, "Generic Image Files(*.dxt, *.bmp, *.tga)|*.dxt;*.bmp;*.tga;||", NULL);

    if (IDCANCEL == dlg.DoModal()) {
        return;
    }
    fs::path fsFile = dlg.GetPathName().GetString();
    CN3BaseFileAccess::ToRelative(fsFile);
    SetDlgItemText(IDC_E_PATH_OLD, fsFile.string().c_str());
}

void CDlgChangeImage::OnBBrowseNewFile() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, "dxt", NULL, dwFlags, "Generic Image Files(*.dxt, *.bmp, *.tga)|*.dxt;*.bmp;*.tga;||", NULL);

    if (IDCANCEL == dlg.DoModal()) {
        return;
    }
    fs::path fsFile = dlg.GetPathName().GetString();
    CN3BaseFileAccess::ToRelative(fsFile);
    SetDlgItemText(IDC_E_PATH_NEW, fsFile.string().c_str());
}

void CDlgChangeImage::OnOK() {
    // TODO: Add extra validation here
    CString szFile;
    GetDlgItemText(IDC_E_PATH_OLD, szFile);
    m_fsOldFile = szFile.GetString();

    GetDlgItemText(IDC_E_PATH_NEW, szFile);
    m_fsNewFile = szFile.GetString();

    CDialog::OnOK();
}
