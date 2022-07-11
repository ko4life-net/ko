// ServerInfoViewer.h : main header file for the SERVERINFOVIEWER application
//

#if !defined(AFX_SERVERINFOVIEWER_H__452D9531_1BB2_4A1B_BD75_8B72FD84AF51__INCLUDED_)
#define AFX_SERVERINFOVIEWER_H__452D9531_1BB2_4A1B_BD75_8B72FD84AF51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerApp:
// See ServerInfoViewer.cpp for the implementation of this class
//

class CServerInfoViewerApp : public CWinApp
{
public:
	CServerInfoViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerInfoViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CServerInfoViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERINFOVIEWER_H__452D9531_1BB2_4A1B_BD75_8B72FD84AF51__INCLUDED_)
