// N3FE.h : main header file for the N3FE application
//

#if !defined(AFX_N3FE_H__445B9F95_C8C2_4931_9630_B83923E89450__INCLUDED_)
#define AFX_N3FE_H__445B9F95_C8C2_4931_9630_B83923E89450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CN3FEApp:
// See N3FE.cpp for the implementation of this class
//

class CN3FEApp : public CWinApp
{
public:
	CN3FEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CN3FEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CN3FEApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_N3FE_H__445B9F95_C8C2_4931_9630_B83923E89450__INCLUDED_)
