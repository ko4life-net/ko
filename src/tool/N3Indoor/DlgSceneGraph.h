#if !defined(AFX_DLGSCENEGRAPH_H__C0B705DA_3306_4993_B4A8_77E63DE2CB9D__INCLUDED_)
#define AFX_DLGSCENEGRAPH_H__C0B705DA_3306_4993_B4A8_77E63DE2CB9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSceneGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSceneGraph dialog
#include "Resource.h"
#include "../N3Base/N3Base.h"
#include "../N3Base/N3TransformCollision.h"

class CN3Scene;

class CDlgSceneGraph : public CDialog
{
public:
	CN3Scene*		m_pSceneRef;
	CImageList		m_ImgList;

protected:
	void UpdateTreeItem(HTREEITEM hParent, CN3Base* pBase);
	DWORD	m_dwFlag;	// 목록에 표시할 것들을 나타낸 플래그

// Construction
public:
	void ExpandTree(HTREEITEM hItem);
	void UpdateTree(CN3Scene* pScene); 

// standard constructor
	CDlgSceneGraph(CWnd* pParent = NULL, DWORD dwFlag = 
		OBJ_CAMERA|OBJ_LIGHT|OBJ_SHAPE|OBJ_CHARACTER|OBJ_MESH|OBJ_MESH_PROGRESSIVE|
		OBJ_MESH_INDEXED|OBJ_JOINT|OBJ_SKIN);

	void RenderObj(CN3TransformCollision* pObj);


// Dialog Data
	//{{AFX_DATA(CDlgSceneGraph)
	enum { IDD = IDD_SCENE_GRAPH };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSceneGraph)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSceneGraph)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkTreeObj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTreeObj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeObj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCENEGRAPH_H__C0B705DA_3306_4993_B4A8_77E63DE2CB9D__INCLUDED_)
