#if !defined(AFX_DLGBROWSEPATH_H__149585E3_52EB_41ED_8BFA_BB5982371255__INCLUDED_)
#define AFX_DLGBROWSEPATH_H__149585E3_52EB_41ED_8BFA_BB5982371255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "ShellTree.h"

// DlgBrowsePath.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowsePath dialog

class CDlgBrowsePath : public CDialog
{
// Construction
public:
	CString m_szPath;
	CString GetPath() { return m_szPath; }
	CDlgBrowsePath(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBrowsePath)
	enum { IDD = IDD_BROWSE_PATH };
	CComboBox	m_CBPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBrowsePath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CShellTree* m_TreeCtl;
	// Generated message map functions
	//{{AFX_MSG(CDlgBrowsePath)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSynchFolders();
	afx_msg void OnSelchangedBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBROWSEPATH_H__149585E3_52EB_41ED_8BFA_BB5982371255__INCLUDED_)
