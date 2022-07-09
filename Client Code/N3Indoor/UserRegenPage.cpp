// UserRegenPage.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "UserRegenPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserRegenPage property page

IMPLEMENT_DYNCREATE(CUserRegenPage, CPropertyPage)

CUserRegenPage::CUserRegenPage() : CPropertyPage(CUserRegenPage::IDD)
{
	//{{AFX_DATA_INIT(CUserRegenPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserRegenPage::~CUserRegenPage()
{
}

void CUserRegenPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserRegenPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserRegenPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUserRegenPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserRegenPage message handlers
