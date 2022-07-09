#if !defined(AFX_DLGSHAPELIST_H__211F99A7_27C9_4341_A573_09BDBBA0A0A6__INCLUDED_)
#define AFX_DLGSHAPELIST_H__211F99A7_27C9_4341_A573_09BDBBA0A0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShapeList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShapeList dialog
#include "Resource.h"
#include "../N3Base/N3Base.h"

class CN3Scene;
class CN3Shape;

class CDlgShapeList : public CDialog
{
// Construction
public:
	CDlgShapeList(CWnd* pParent = NULL);   // standard constructor

	CN3Scene* m_pSceneRef;
	BOOL	m_IsSourceObj;		// Display용이냐? (툴에서 오브젝트 목록)
//	CMapMng* m_pMapMng;

// Dialog Data
	//{{AFX_DATA(CDlgShapeList)
	enum { IDD = IDD_SHAPE_LIST };
	CListBox	m_ListShape;
	//}}AFX_DATA

	void UpdateTree(CN3Scene* pScene = NULL);		 // Source와.. Output 공용..
	void SelectObject(void* pItemData);							 // Output용..
	int	GetTotalShapeInfoCount();									// Output용..
	CN3Shape* GetShapeByiOrder(int iOrder);				// Output용..

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShapeList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShapeList)
	afx_msg void OnBtnSort();
	afx_msg void OnSelchangeListObj();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHAPELIST_H__211F99A7_27C9_4341_A573_09BDBBA0A0A6__INCLUDED_)
