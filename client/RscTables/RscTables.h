// RscTables.h : main header file for the RSCTABLES application
//

#if !defined(AFX_RSCTABLES_H__EEFEB68F_9ACB_4EA2_B135_849FBB7E1546__INCLUDED_)
#define AFX_RSCTABLES_H__EEFEB68F_9ACB_4EA2_B135_849FBB7E1546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_RSCTABLES_H__EEFEB68F_9ACB_4EA2_B135_849FBB7E1546__INCLUDED_)
