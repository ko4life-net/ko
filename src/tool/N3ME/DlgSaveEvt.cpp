// DlgSaveEvt.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgSaveEvt.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveEvt dialog

CDlgSaveEvt::CDlgSaveEvt(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgSaveEvt::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgSaveEvt)
    m_FileName = _T("");
    //}}AFX_DATA_INIT
}

void CDlgSaveEvt::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSaveEvt)
    DDX_Control(pDX, IDC_LIST_SAVED_EVT_FILE, m_SavedFileList);
    DDX_Text(pDX, IDC_EDIT_NEW_EVT_FILE, m_FileName);
    DDV_MaxChars(pDX, m_FileName, 255);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSaveEvt, CDialog)
//{{AFX_MSG_MAP(CDlgSaveEvt)
ON_LBN_SELCHANGE(IDC_LIST_SAVED_EVT_FILE, OnSelchangeListSavedEvtFile)
ON_LBN_DBLCLK(IDC_LIST_SAVED_EVT_FILE, OnDblclkListSavedEvtFile)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveEvt message handlers

void CDlgSaveEvt::OnSelchangeListSavedEvtFile() {
    int CurrSel = m_SavedFileList.GetCurSel();
    m_SavedFileList.GetText(CurrSel, m_FileName);
    UpdateData(FALSE);
}

void CDlgSaveEvt::OnDblclkListSavedEvtFile() {
    OnOK();
}

BOOL CDlgSaveEvt::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("event") / "*.evt").string();
    m_SavedFileList.Dir(DDL_READONLY, szSearchPath.c_str());

    int count = m_SavedFileList.GetCount();

    CString str;
    for (int i = 0; i < count; i++) {
        m_SavedFileList.GetText(0, str);
        str = fs::path(str.GetString()).stem().c_str();
        m_SavedFileList.InsertString(count, str);
        m_SavedFileList.DeleteString(0);
    }

    fs::current_path(fsPrevPath);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
