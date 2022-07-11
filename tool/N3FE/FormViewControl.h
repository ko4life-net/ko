#if !defined(AFX_FORMVIEWCONTROL_H__5AE56F53_4EC9_4C1D_AEFD_824BDDD97CC2__INCLUDED_)
#define AFX_FORMVIEWCONTROL_H__5AE56F53_4EC9_4C1D_AEFD_824BDDD97CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormViewControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormViewControl form view

class CN3FEDoc;

class CFormViewControl : public CFormView
{
protected:
	CFormViewControl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormViewControl)

// Form Data
public:
	//{{AFX_DATA(CFormViewControl)
	enum { IDD = IDD_CONTROL };
	CListBox	m_PartList;
	CListBox	m_BundleList;
	//}}AFX_DATA

// Attributes
public:
	CN3FEDoc* GetDocument();

// Operations
public:
	void UpdateInfo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormViewControl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormViewControl)
	afx_msg void OnBAdd();
	afx_msg void OnBDelete();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBUpdateEffect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBTrigger();
	afx_msg void OnBStop();
	afx_msg void OnChangeEPartScript();
	afx_msg void OnChangeEBundleScript();
	afx_msg void OnSelchangeListBundle();
	afx_msg void OnSelchangeListPart();
	afx_msg void OnBBundleAdd();
	afx_msg void OnBBundleInsert();
	afx_msg void OnBBundleDelete();
	afx_msg void OnBPartAdd();
	afx_msg void OnBPartDelete();
	afx_msg void OnBPartInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG  // debug version in N3FEView.cpp
inline CN3FEDoc* CFormViewControl::GetDocument()
   { return (CN3FEDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWCONTROL_H__5AE56F53_4EC9_4C1D_AEFD_824BDDD97CC2__INCLUDED_)
