#pragma once

// UserRegenPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserRegenPage dialog

class CUserRegenPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserRegenPage)

// Construction
public:
	CUserRegenPage();
	~CUserRegenPage();

// Dialog Data
	//{{AFX_DATA(CUserRegenPage)
	enum { IDD = IDD_REGEN_USER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserRegenPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserRegenPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

