#pragma once

// EnvironSoundPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvironSoundPage dialog

class CEnvironSoundPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEnvironSoundPage)

// Construction
public:
	CEnvironSoundPage();
	~CEnvironSoundPage();

// Dialog Data
	//{{AFX_DATA(CEnvironSoundPage)
	enum { IDD = IDD_ENVIRON_SOUND };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEnvironSoundPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEnvironSoundPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

