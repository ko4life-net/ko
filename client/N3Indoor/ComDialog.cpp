// ComDialog.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "ComDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComDialog dialog


CComDialog::CComDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CComDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComDialog, CDialog)
	//{{AFX_MSG_MAP(CComDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComDialog message handlers

