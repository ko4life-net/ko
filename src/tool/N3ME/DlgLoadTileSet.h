#pragma once

// DlgLoadTileSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadTileSet dialog

class CDlgLoadTileSet : public CDialog
{
// Construction
public:
	CDlgLoadTileSet(CWnd* pParent = NULL);   // standard constructor

	CString		m_SelFileName;

// Dialog Data
	//{{AFX_DATA(CDlgLoadTileSet)
	enum { IDD = IDD_LOAD_TILESET };
	CListBox	m_ListTileSet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoadTileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoadTileSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLoadTileset();
	afx_msg void OnDblclkListLoadTileset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

