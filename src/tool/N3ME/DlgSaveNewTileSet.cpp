// DlgSaveNewTileSet.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgSaveNewTileSet.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveNewTileSet dialog

CDlgSaveNewTileSet::CDlgSaveNewTileSet(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgSaveNewTileSet::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgSaveNewTileSet)
    m_NewTileSetName = _T("");
    //}}AFX_DATA_INIT
}

void CDlgSaveNewTileSet::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSaveNewTileSet)
    DDX_Control(pDX, IDC_LIST_TILESET, m_ListTileSet);
    DDX_Text(pDX, IDC_EDIT_NEWTILESETNAME, m_NewTileSetName);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSaveNewTileSet, CDialog)
//{{AFX_MSG_MAP(CDlgSaveNewTileSet)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveNewTileSet message handlers

BOOL CDlgSaveNewTileSet::OnInitDialog() {
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
