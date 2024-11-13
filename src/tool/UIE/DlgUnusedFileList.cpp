// DlgUnusedFileList.cpp : implementation file
//

#include "StdAfx.h"
#include "UIE.h"
#include "DlgUnusedFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFileList dialog

CDlgUnusedFileList::CDlgUnusedFileList(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgUnusedFileList::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgUnusedFileList)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgUnusedFileList::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgUnusedFileList)
    DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgUnusedFileList, CDialog)
//{{AFX_MSG_MAP(CDlgUnusedFileList)
ON_BN_CLICKED(IDC_B_DELETE_SELECT, OnBDeleteSelect)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFileList message handlers

void CDlgUnusedFileList::OnBDeleteSelect() {
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
}

BOOL CDlgUnusedFileList::OnInitDialog() {
    CDialog::OnInitDialog();

    for (const auto & fsFile : m_vFiles) {
        m_ListFiles.AddString(CString(fsFile.c_str()));
    }

    return TRUE;
}
