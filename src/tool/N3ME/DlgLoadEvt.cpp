// DlgLoadEvt.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgLoadEvt.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadEvt dialog

CDlgLoadEvt::CDlgLoadEvt(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgLoadEvt::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgLoadEvt)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgLoadEvt::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLoadEvt)
    DDX_Control(pDX, IDC_LIST_SAVED_EVT_FILE, m_EvtFileList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgLoadEvt, CDialog)
//{{AFX_MSG_MAP(CDlgLoadEvt)
ON_LBN_DBLCLK(IDC_LIST_SAVED_EVT_FILE, OnDblclkListSavedEvtFile)
ON_LBN_SELCHANGE(IDC_LIST_SAVED_EVT_FILE, OnSelchangeListSavedEvtFile)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadEvt message handlers

BOOL CDlgLoadEvt::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("Event") / "*.evt").string();
    m_EvtFileList.Dir(DDL_READONLY, szSearchPath.c_str());

    int count = m_EvtFileList.GetCount();

    CString str;
    for (int i = 0; i < count; i++) {
        m_EvtFileList.GetText(0, str);
        str = fs::path(str.GetString()).stem().c_str();
        m_EvtFileList.InsertString(count, str);
        m_EvtFileList.DeleteString(0);
    }

    fs::current_path(fsPrevPath);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoadEvt::OnDblclkListSavedEvtFile() {
    OnOK();
}

void CDlgLoadEvt::OnSelchangeListSavedEvtFile() {
    int CurrSel = m_EvtFileList.GetCurSel();
    m_EvtFileList.GetText(CurrSel, m_SelFileName);
}
