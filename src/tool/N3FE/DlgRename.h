#if !defined(AFX_DLGRENAME_H__DC950706_D00B_468D_9F0C_4F4DBB6F0BD3__INCLUDED_)
#define AFX_DLGRENAME_H__DC950706_D00B_468D_9F0C_4F4DBB6F0BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog
#include "Resource.h"

class CDlgRename : public CDialog
{
// Construction
public:
	CDlgRename(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRename)
	enum { IDD = IDD_RENAME };
	CComboBox	m_CBDigit;
	CListBox	m_FileList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRename)
	afx_msg void OnBAdd();
	afx_msg void OnBConvert();
	afx_msg void OnBRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnBSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRENAME_H__DC950706_D00B_468D_9F0C_4F4DBB6F0BD3__INCLUDED_)
