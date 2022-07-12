#if !defined(AFX_ENVIRONSOUNDPAGE_H__A141F244_3C2B_466C_AA4D_8E81A49FAA11__INCLUDED_)
#define AFX_ENVIRONSOUNDPAGE_H__A141F244_3C2B_466C_AA4D_8E81A49FAA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

#endif // !defined(AFX_ENVIRONSOUNDPAGE_H__A141F244_3C2B_466C_AA4D_8E81A49FAA11__INCLUDED_)
