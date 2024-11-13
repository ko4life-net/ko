// DlgLoadNPCPath.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgLoadNPCPath.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadNPCPath dialog

CDlgLoadNPCPath::CDlgLoadNPCPath(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgLoadNPCPath::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgLoadNPCPath)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgLoadNPCPath::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLoadNPCPath)
    DDX_Control(pDX, IDC_LIST_NPCPATH, m_NPCPathFileList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgLoadNPCPath, CDialog)
//{{AFX_MSG_MAP(CDlgLoadNPCPath)
ON_LBN_SELCHANGE(IDC_LIST_NPCPATH, OnSelchangeListNpcpath)
ON_LBN_DBLCLK(IDC_LIST_NPCPATH, OnDblclkListNpcpath)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadNPCPath message handlers

void CDlgLoadNPCPath::OnSelchangeListNpcpath() {
    int CurrSel = m_NPCPathFileList.GetCurSel();
    m_NPCPathFileList.GetText(CurrSel, m_SelFileName); // TODO: Add your control notification handler code here
}

void CDlgLoadNPCPath::OnDblclkListNpcpath() {
    OnOK();
}

BOOL CDlgLoadNPCPath::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("NPCPath") / "*.npi").string();
    m_NPCPathFileList.Dir(DDL_READONLY, szSearchPath.c_str());

    int count = m_NPCPathFileList.GetCount();

    CString str;
    for (int i = 0; i < count; i++) {
        m_NPCPathFileList.GetText(0, str);
        str = fs::path(str.GetString()).stem().c_str();
        m_NPCPathFileList.InsertString(count, str);
        m_NPCPathFileList.DeleteString(0);
    }

    fs::current_path(fsPrevPath);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
