#if !defined(AFX_EVENTEDITPAGE_H__19C24F82_D014_4325_9CF4_9469B3E63265__INCLUDED_)
#define AFX_EVENTEDITPAGE_H__19C24F82_D014_4325_9CF4_9469B3E63265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventEditPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventEditPage dialog

class CEventEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEventEditPage)

// Construction
public:
	CEventEditPage();
	~CEventEditPage();

// Dialog Data
	//{{AFX_DATA(CEventEditPage)
	enum { IDD = IDD_EDIT_EVENT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEventEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEventEditPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTEDITPAGE_H__19C24F82_D014_4325_9CF4_9469B3E63265__INCLUDED_)
