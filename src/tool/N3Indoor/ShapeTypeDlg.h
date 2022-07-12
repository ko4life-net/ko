#if !defined(AFX_SHAPETYPEDLG_H__BD715689_4AFC_437E_8842_ADFC11D39911__INCLUDED_)
#define AFX_SHAPETYPEDLG_H__BD715689_4AFC_437E_8842_ADFC11D39911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShapeTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShapeTypeDlg dialog

class CShapeTypeDlg : public CDialog
{
public:
	bool	m_bValidate;
	bool	m_bPartial;

// Construction
public:
	CShapeTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShapeTypeDlg)
	enum { IDD = IDD_SHAPE_TYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapeTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShapeTypeDlg)
	afx_msg void OnRadioPartial();
	afx_msg void OnRadioNonepartial();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPETYPEDLG_H__BD715689_4AFC_437E_8842_ADFC11D39911__INCLUDED_)
