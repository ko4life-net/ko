// DlgSaveNPCPath.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgSaveNPCPath.h"
#include "N3Base/N3Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveNPCPath dialog

CDlgSaveNPCPath::CDlgSaveNPCPath(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgSaveNPCPath::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgSaveNPCPath)
    m_NewFileName = _T("");
    //}}AFX_DATA_INIT
}

void CDlgSaveNPCPath::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSaveNPCPath)
    DDX_Control(pDX, IDC_LIST_NPCPATHFILENAME, m_SavedFileList);
    DDX_Text(pDX, IDC_EDIT_NPCPATHFILENAME, m_NewFileName);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSaveNPCPath, CDialog)
//{{AFX_MSG_MAP(CDlgSaveNPCPath)
ON_LBN_SELCHANGE(IDC_LIST_NPCPATHFILENAME, OnSelchangeListNpcpathfilename)
ON_LBN_DBLCLK(IDC_LIST_NPCPATHFILENAME, OnDblclkListNpcpathfilename)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveNPCPath message handlers

void CDlgSaveNPCPath::OnSelchangeListNpcpathfilename() {
    int CurrSel = m_SavedFileList.GetCurSel();
    m_SavedFileList.GetText(CurrSel, m_NewFileName);
    UpdateData(FALSE);
}

void CDlgSaveNPCPath::OnDblclkListNpcpathfilename() {
    OnOK();
}

BOOL CDlgSaveNPCPath::OnInitDialog() {
    CDialog::OnInitDialog();

    fs::path fsPrevPath = fs::current_path();
    fs::current_path(CN3Base::PathGet());

    std::string szSearchPath = (fs::path("npcpath") / "*.npi").string();
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
