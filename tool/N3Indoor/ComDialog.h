#if !defined(AFX_COMDIALOG_H__742979CE_A9B8_4F71_8956_6B02C385BE73__INCLUDED_)
#define AFX_COMDIALOG_H__742979CE_A9B8_4F71_8956_6B02C385BE73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComDialog dialog

class CComDialog : public CDialog
{
// Construction
public:
	CComDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComDialog)
	enum { IDD = IDD_COMPUTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMDIALOG_H__742979CE_A9B8_4F71_8956_6B02C385BE73__INCLUDED_)
