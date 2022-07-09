#if !defined(AFX_MAKENPCPATHPAGE_H__AA10B3D7_9242_4275_AB25_0F041ABEBF76__INCLUDED_)
#define AFX_MAKENPCPATHPAGE_H__AA10B3D7_9242_4275_AB25_0F041ABEBF76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeNpcPathPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeNpcPathPage dialog

class CMakeNpcPathPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMakeNpcPathPage)

// Construction
public:
	CMakeNpcPathPage();
	~CMakeNpcPathPage();

// Dialog Data
	//{{AFX_DATA(CMakeNpcPathPage)
	enum { IDD = IDD_MAKE_NPCPATH };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMakeNpcPathPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMakeNpcPathPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKENPCPATHPAGE_H__AA10B3D7_9242_4275_AB25_0F041ABEBF76__INCLUDED_)
