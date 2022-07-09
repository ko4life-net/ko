#if !defined(AFX_DLGDATACOUNT_H__49ED5914_1366_43F5_B899_7FF8D0D00380__INCLUDED_)
#define AFX_DLGDATACOUNT_H__49ED5914_1366_43F5_B899_7FF8D0D00380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDataCount dialog

class CDlgDataCount : public CDialog
{
// Construction
public:
	CDlgDataCount(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDataCount)
	enum { IDD = IDD_DIALOG_DATACOUNT };
	int		m_iCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDataCount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDataCount)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATACOUNT_H__49ED5914_1366_43F5_B899_7FF8D0D00380__INCLUDED_)
