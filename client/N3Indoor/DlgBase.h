#if !defined(AFX_DLGBASE_H__66F49FA8_BC6C_4136_A7A0_AA23CA80A3FB__INCLUDED_)
#define AFX_DLGBASE_H__66F49FA8_BC6C_4136_A7A0_AA23CA80A3FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBase dialog
#include "../Common Control/PropertyList.h"
#include "Resource.h"

class CDlgBase : public CDialog
{
// Construction
public:
	bool m_bFirst;

	void OneTimeInit();
	void UpdateInfo();
	CDlgBase(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBase)
	enum { IDD = IDD_BASE };
	CPropertyList	m_LPTransform;
	CPropertyList	m_LPShape;
	CPropertyList	m_LPMaterial;
	CPropertyList	m_LPLight;
	CPropertyList	m_LPCPlug;
	CPropertyList	m_LPCPart;
	CPropertyList	m_LPChr;
	CPropertyList	m_LPCamera;
	CComboBox	m_CBChrPlug;
	CComboBox	m_CBShapePart;
	CComboBox	m_CBChrPart;
	CComboBox	m_CBChrLOD;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBase)
	afx_msg void OnChangeEName();
	afx_msg void OnSelchangeCbPartShape();
	afx_msg void OnSelchangeCbPartCharacte();
	afx_msg void OnSelchangeCbLod();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbPlugCharacter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBASE_H__66F49FA8_BC6C_4136_A7A0_AA23CA80A3FB__INCLUDED_)
