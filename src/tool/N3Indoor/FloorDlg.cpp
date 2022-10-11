// FloorDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "N3Indoor.h"
#include "FloorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloorDlg dialog

CFloorDlg::CFloorDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CFloorDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CFloorDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_iCurFloor = 0;
}

void CFloorDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFloorDlg)
    DDX_Control(pDX, IDC_FLOOR_NUMBER, m_NumFloor);
    //}}AFX_DATA_MAP
}

int CFloorDlg::GetFloor() {
    return m_iCurFloor;
}

BEGIN_MESSAGE_MAP(CFloorDlg, CDialog)
//{{AFX_MSG_MAP(CFloorDlg)
ON_EN_CHANGE(IDC_FLOOR_NUMBER, OnChangeFloorNumber)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloorDlg message handlers

void CFloorDlg::OnChangeFloorNumber() {
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO: Add your control notification handler code here
    m_iCurFloor = GetDlgItemInt(IDC_FLOOR_NUMBER);
}

void CFloorDlg::OnOK() {
    // TODO: Add extra validation here
    int iCur = GetFloor();
    if (iCur < 1) {
        AfxMessageBox("0보다 큰 수를 입력하십시오....");
        return;
    }

    CDialog::OnOK();
}
