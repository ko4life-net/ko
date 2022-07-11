// ServerInfoViewerView.h : interface of the CServerInfoViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERINFOVIEWERVIEW_H__3E780FC2_6609_4A9D_AF31_52605C4F7513__INCLUDED_)
#define AFX_SERVERINFOVIEWERVIEW_H__3E780FC2_6609_4A9D_AF31_52605C4F7513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CServerInfoViewerView : public CView
{
protected: // create from serialization only
	CServerInfoViewerView();
	DECLARE_DYNCREATE(CServerInfoViewerView)

// Attributes
public:
	CServerInfoViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerInfoViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void FocusGround();
	virtual ~CServerInfoViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CServerInfoViewerView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ServerInfoViewerView.cpp
inline CServerInfoViewerDoc* CServerInfoViewerView::GetDocument()
   { return (CServerInfoViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERINFOVIEWERVIEW_H__3E780FC2_6609_4A9D_AF31_52605C4F7513__INCLUDED_)
