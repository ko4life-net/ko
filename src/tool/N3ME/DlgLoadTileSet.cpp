// DlgLoadTileSet.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgLoadTileSet.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadTileSet dialog

CDlgLoadTileSet::CDlgLoadTileSet(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgLoadTileSet::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgLoadTileSet)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgLoadTileSet::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLoadTileSet)
    DDX_Control(pDX, IDC_LIST_LOAD_TILESET, m_ListTileSet);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgLoadTileSet, CDialog)
//{{AFX_MSG_MAP(CDlgLoadTileSet)
ON_LBN_SELCHANGE(IDC_LIST_LOAD_TILESET, OnSelchangeListLoadTileset)
ON_LBN_DBLCLK(IDC_LIST_LOAD_TILESET, OnDblclkListLoadTileset)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadTileSet message handlers

BOOL CDlgLoadTileSet::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("DTex") / "*.dtx").string();
    m_ListTileSet.Dir(DDL_READONLY, szSearchPath.c_str());

    int count = m_ListTileSet.GetCount();

    CString str;
    for (int i = 0; i < count; i++) {
        m_ListTileSet.GetText(0, str);
        str = fs::path(str.GetString()).stem().c_str();
        m_ListTileSet.InsertString(count, str);
        m_ListTileSet.DeleteString(0);
    }

    fs::current_path(fsPrevPath);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoadTileSet::OnSelchangeListLoadTileset() {
    int CurrSel = m_ListTileSet.GetCurSel();
    m_ListTileSet.GetText(CurrSel, m_SelFileName); // TODO: Add your control notification handler code here
    m_SelFileName = fs::path(m_SelFileName.GetString()).stem().c_str();
}

void CDlgLoadTileSet::OnDblclkListLoadTileset() {
    OnOK();
}
