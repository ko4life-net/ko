// DlgDataCount.cpp : implementation file
//

#include "stdafx.h"
#include "RscTables.h"
#include "DlgDataCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDataCount dialog


CDlgDataCount::CDlgDataCount(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataCount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDataCount)
	m_iCount = 0;
	//}}AFX_DATA_INIT
}


void CDlgDataCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataCount)
	DDX_Text(pDX, IDC_EDIT_COUNT, m_iCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataCount, CDialog)
	//{{AFX_MSG_MAP(CDlgDataCount)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDataCount message handlers
