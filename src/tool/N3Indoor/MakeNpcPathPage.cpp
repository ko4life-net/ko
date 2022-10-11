// MakeNpcPathPage.cpp : implementation file
//

#include "StdAfx.h"
#include "N3Indoor.h"
#include "MakeNpcPathPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeNpcPathPage property page

IMPLEMENT_DYNCREATE(CMakeNpcPathPage, CPropertyPage)

CMakeNpcPathPage::CMakeNpcPathPage()
    : CPropertyPage(CMakeNpcPathPage::IDD) {
    //{{AFX_DATA_INIT(CMakeNpcPathPage)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CMakeNpcPathPage::~CMakeNpcPathPage() {}

void CMakeNpcPathPage::DoDataExchange(CDataExchange * pDX) {
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMakeNpcPathPage)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakeNpcPathPage, CPropertyPage)
//{{AFX_MSG_MAP(CMakeNpcPathPage)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeNpcPathPage message handlers
