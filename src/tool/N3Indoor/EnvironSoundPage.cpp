// EnvironSoundPage.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "EnvironSoundPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvironSoundPage property page

IMPLEMENT_DYNCREATE(CEnvironSoundPage, CPropertyPage)

CEnvironSoundPage::CEnvironSoundPage() : CPropertyPage(CEnvironSoundPage::IDD)
{
	//{{AFX_DATA_INIT(CEnvironSoundPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEnvironSoundPage::~CEnvironSoundPage()
{
}

void CEnvironSoundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvironSoundPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvironSoundPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEnvironSoundPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvironSoundPage message handlers
