// RscTables.h : main header file for the RSCTABLES application
//

#pragma once


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#pragma warning(disable:4786)

#include "resource.h"		// main symbols
#include "TableGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CRscTablesApp:
// See RscTables.cpp for the implementation of this class
//

class CRscTablesApp : public CWinApp
{
public:
	CRscTablesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRscTablesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRscTablesApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

