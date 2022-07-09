// EventEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "EventEditPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventEditPage property page

IMPLEMENT_DYNCREATE(CEventEditPage, CPropertyPage)

CEventEditPage::CEventEditPage() : CPropertyPage(CEventEditPage::IDD)
{
	//{{AFX_DATA_INIT(CEventEditPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEventEditPage::~CEventEditPage()
{
}

void CEventEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventEditPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEventEditPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventEditPage message handlers
