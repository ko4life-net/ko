// N3FEView.h : interface of the CN3FEView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3FEVIEW_H__0258AD96_93F0_43F2_B9EB_71E3B81CB19D__INCLUDED_)
#define AFX_N3FEVIEW_H__0258AD96_93F0_43F2_B9EB_71E3B81CB19D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CN3FEDoc;

class CN3FEView : public CView
{
protected: // create from serialization only
	CN3FEView();
	DECLARE_DYNCREATE(CN3FEView)

// Attributes
public:
	CN3FEDoc* GetDocument();

// Operations
public:
	CPoint		m_ptPrev;
	__Vector3	m_vTarget;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CN3FEView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CN3FEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CN3FEView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in N3FEView.cpp
inline CN3FEDoc* CN3FEView::GetDocument()
   { return (CN3FEDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_N3FEVIEW_H__0258AD96_93F0_43F2_B9EB_71E3B81CB19D__INCLUDED_)
