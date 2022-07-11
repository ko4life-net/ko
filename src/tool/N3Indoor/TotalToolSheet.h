#if !defined(AFX_TOTALTOOLSHEET_H__D6003ABA_F24F_4AAB_9C63_29BD9FBE4D7A__INCLUDED_)
#define AFX_TOTALTOOLSHEET_H__D6003ABA_F24F_4AAB_9C63_29BD9FBE4D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TotalToolSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTotalToolSheet

#include "EnvironSoundPage.h"
#include "EventEditPage.h"
#include "MakeNpcPathPage.h"
#include "UserRegenPage.h"

class CTotalToolSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTotalToolSheet)

	//.. 
	CEnvironSoundPage	m_ESound;
	CEventEditPage			 m_EventEdit;
	CMakeNpcPathPage   m_MakeNpcPath;
	CUserRegenPage		 m_UserRegen;

// Construction
public:
	CTotalToolSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTotalToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTotalToolSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTotalToolSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTotalToolSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOTALTOOLSHEET_H__D6003ABA_F24F_4AAB_9C63_29BD9FBE4D7A__INCLUDED_)
