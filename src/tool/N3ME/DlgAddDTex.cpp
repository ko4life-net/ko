// DlgAddDTex.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgAddDTex.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDTex dialog

CDlgAddDTex::CDlgAddDTex(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgAddDTex::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgAddDTex)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgAddDTex::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAddDTex)
    DDX_Control(pDX, IDC_ADDDTEXLIST, m_TexList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAddDTex, CDialog)
//{{AFX_MSG_MAP(CDlgAddDTex)
ON_LBN_SELCHANGE(IDC_ADDDTEXLIST, OnSelchangeAdddtexlist)
ON_LBN_DBLCLK(IDC_ADDDTEXLIST, OnDblclkAdddtexlist)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDTex message handlers

void CDlgAddDTex::OnSelchangeAdddtexlist() {
    int CurrSel = m_TexList.GetCurSel();
    //m_TexList.SetCurSel(CurrSel);
    m_TexList.GetText(CurrSel, m_TexName);
}

void CDlgAddDTex::OnDblclkAdddtexlist() {
    OnOK();
}

BOOL CDlgAddDTex::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("DTex") / "*.bmp").string();
    m_TexList.Dir(DDL_READONLY, szSearchPath.c_str());

    fs::current_path(fsPrevPath);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
