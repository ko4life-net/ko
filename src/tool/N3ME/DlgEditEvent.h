#pragma once

// DlgEditEvent.h : header file
//

class CEventMgr;
class CEventCell;

/////////////////////////////////////////////////////////////////////////////
// CDlgEditEvent dialog

class CDlgEditEvent : public CDialog
{
// Construction
public:
	void ResetAll();
	void SetCurrName(char* pName);
	CDlgEditEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditEvent)
	enum { IDD = IDD_EDIT_EVENT };
	CListBox	m_EventList;
	CString		m_CurrFileName;
	//}}AFX_DATA

// Variables..
	CEventMgr*	m_pRefEventMgr;
	CEventCell*	m_pSelEvent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditEvent)
	afx_msg void OnBtnEventDel();
	afx_msg void OnBtnEventEdit();
	afx_msg void OnBtnEventRecord();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEventList();
	afx_msg void OnBtnLoadEvtfile();
	afx_msg void OnBtnSaveEvtfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

