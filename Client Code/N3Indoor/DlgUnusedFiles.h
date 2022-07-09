#if !defined(AFX_DLGUNUSEDFILES_H__00D6A6CE_0993_4CE5_9796_7E8E00EE9350__INCLUDED_)
#define AFX_DLGUNUSEDFILES_H__00D6A6CE_0993_4CE5_9796_7E8E00EE9350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUnusedFiles.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFiles dialog

class CDlgUnusedFiles : public CDialog
{
public:
	CStringArray m_FileNames;
	CStringArray m_InvalidFileNames;

public:
	void UpdateAll();
	CDlgUnusedFiles(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUnusedFiles)
	enum { IDD = IDD_UNUSED_FILES };
	CListBox	m_ListInvalidObjects;
	CListBox	m_ListFiles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUnusedFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUnusedFiles)
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUNUSEDFILES_H__00D6A6CE_0993_4CE5_9796_7E8E00EE9350__INCLUDED_)
