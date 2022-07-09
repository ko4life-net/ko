// ShapeTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "ShapeTypeDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapeTypeDlg dialog


CShapeTypeDlg::CShapeTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShapeTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bValidate = false;
	m_bPartial = true;
}


void CShapeTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapeTypeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapeTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CShapeTypeDlg)
	ON_BN_CLICKED(IDC_RADIO_PARTIAL, OnRadioPartial)
	ON_BN_CLICKED(IDC_RADIO_NONEPARTIAL, OnRadioNonepartial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapeTypeDlg message handlers

void CShapeTypeDlg::OnRadioPartial() 
{
	// TODO: Add your control notification handler code here
	m_bPartial = true;

	CButton* pBut = (CButton *)GetDlgItem(IDC_RADIO_PARTIAL);		  pBut->SetCheck(1);
	pBut = (CButton *)GetDlgItem(IDC_RADIO_NONEPARTIAL);			   pBut->SetCheck(0);	
}

void CShapeTypeDlg::OnRadioNonepartial() 
{
	// TODO: Add your control notification handler code here
	m_bPartial = false;

	CButton* pBut = (CButton *)GetDlgItem(IDC_RADIO_PARTIAL);		  pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_RADIO_NONEPARTIAL);			   pBut->SetCheck(1);		
}

void CShapeTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	CButton* pBut = (CButton *)GetDlgItem(IDC_RADIO_PARTIAL), *pBut2 = (CButton *)GetDlgItem(IDC_RADIO_NONEPARTIAL);

	if (!pBut->GetCheck() && !pBut2->GetCheck())
	{
		AfxMessageBox("먼저 Shape Type을 설정하세여..");
		return;
	}

	m_bValidate = true;
	
	CDialog::OnOK();
}

void CShapeTypeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bValidate = false;
	
	CDialog::OnCancel();
}

BOOL CShapeTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	RECT rect, rect2; GetWindowRect(&rect);
	int iWidth = (rect.right - rect.left)/2;
	int iHeight = (rect.bottom - rect.top)/2;
	::GetClientRect(pFrm->m_hWnd, &rect2);

	MoveWindow((rect2.left + rect2.right)/2 - iWidth, (rect2.bottom + rect2.top)/2 - iHeight, iWidth*2, iHeight*2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
