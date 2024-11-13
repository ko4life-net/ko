// DlgUnusedFiles.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgUnusedFiles.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFiles dialog

CDlgUnusedFiles::CDlgUnusedFiles(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgUnusedFiles::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgUnusedFiles)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgUnusedFiles::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgUnusedFiles)
    DDX_Control(pDX, IDC_LIST_INVALID_OBJECTS, m_ListInvalidObjects);
    DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgUnusedFiles, CDialog)
//{{AFX_MSG_MAP(CDlgUnusedFiles)
ON_BN_CLICKED(IDC_DELETE, OnDelete)
ON_BN_CLICKED(ID_CANCEL, OnCancel)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFiles message handlers

void CDlgUnusedFiles::OnDelete() {
    int iSC = m_ListFiles.GetSelCount();
    if (iSC <= 0) {
        return;
    }

    if (IDYES != MessageBox("Would you like to erase it?", "Check", MB_YESNO)) {
        return;
    }

    std::vector<int> vSels(iSC);
    m_ListFiles.GetSelItems(iSC, vSels.data());

    for (const auto & iSelIndex : vSels) {
        CString szFile;
        m_ListFiles.GetText(iSelIndex, szFile);
        fs::remove(szFile.GetString());
    }

    CDialog::OnOK();
}

void CDlgUnusedFiles::OnCancel() {
    CDialog::OnCancel();
}

BOOL CDlgUnusedFiles::OnInitDialog() {
    CDialog::OnInitDialog();

    this->UpdateAll();

    return TRUE;
}

void CDlgUnusedFiles::UpdateAll() {
    for (const auto & fsFile : m_vFiles) {
        m_ListFiles.AddString(CString(fsFile.c_str()));
    }

    for (const auto & szError : m_vErroredFiles) {
        m_ListInvalidObjects.AddString(CString(szError.c_str()));
    }
}
