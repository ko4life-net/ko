// N3IndoorView.h : interface of the CN3IndoorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3INDOORVIEW_H__5405C965_E4CC_402B_AAD6_AF8481584AEB__INCLUDED_)
#define AFX_N3INDOORVIEW_H__5405C965_E4CC_402B_AAD6_AF8481584AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CN3IndoorView : public CView
{
	bool	m_bSwapped;
	bool   m_bValid;
	RECT m_rcSelDrag;					// 드래그 영역

	BOOL CustomCameraMove(MSG* pMsg);
	BOOL ExecuteModeMsgFilter(MSG* pMsg);
	void RenderDragRect(RECT* rc);

public:
	bool	m_bExecuteCameraTop;

protected: // create from serialization only
	CN3IndoorView();
	DECLARE_DYNCREATE(CN3IndoorView)

// Attributes
public:
	CN3IndoorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CN3IndoorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void TickRender();
	virtual ~CN3IndoorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CN3IndoorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in N3IndoorView.cpp
inline CN3IndoorDoc* CN3IndoorView::GetDocument()
   { return (CN3IndoorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_N3INDOORVIEW_H__5405C965_E4CC_402B_AAD6_AF8481584AEB__INCLUDED_)
