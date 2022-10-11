// TotalToolSheet.cpp : implementation file
//

#include "StdAfx.h"
#include "N3Indoor.h"
#include "TotalToolSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTotalToolSheet

IMPLEMENT_DYNAMIC(CTotalToolSheet, CPropertySheet)

CTotalToolSheet::CTotalToolSheet(UINT nIDCaption, CWnd * pParentWnd, UINT iSelectPage)
    : CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {
    AddPage(&m_ESound);
    AddPage(&m_EventEdit);
    AddPage(&m_MakeNpcPath);
    AddPage(&m_UserRegen);
}

CTotalToolSheet::CTotalToolSheet(LPCTSTR pszCaption, CWnd * pParentWnd, UINT iSelectPage)
    : CPropertySheet(pszCaption, pParentWnd, iSelectPage) {
    AddPage(&m_ESound);
    AddPage(&m_EventEdit);
    AddPage(&m_MakeNpcPath);
    AddPage(&m_UserRegen);
}

CTotalToolSheet::~CTotalToolSheet() {}

BEGIN_MESSAGE_MAP(CTotalToolSheet, CPropertySheet)
//{{AFX_MSG_MAP(CTotalToolSheet)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTotalToolSheet message handlers
