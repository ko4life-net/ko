#pragma once

// ToolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolDlg dialog
class CN3Transform;
class CToolDlg : public CDialog
{
// Construction
public:
	CToolDlg(CWnd* pParent = NULL);   // standard constructor
	void SetObj(CN3Transform* pObj);

protected:
	CN3Transform* m_pSelectedObj;

public:

// Dialog Data
	//{{AFX_DATA(CToolDlg)
	enum { IDD = IDR_MAINFRAME };
	CSliderCtrl	m_RatateSlider;
	float	m_fRotY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderRotate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

