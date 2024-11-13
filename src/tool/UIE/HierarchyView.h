// HierarchyView.h : interface of the CHierarchyView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CUIEDoc;
class CN3UIBase;
class CHierarchyView : public CTreeView {
  protected: // create from serialization only
    CHierarchyView();
    DECLARE_DYNCREATE(CHierarchyView)

    // Attributes
  public:
    CUIEDoc * GetDocument();

    // Operations
  public:
  protected:
    void UpdateAllInfo();
    void UpdateTreeItem(HTREEITEM hParent, CN3UIBase * pUIBase);
    void SelectObject(HTREEITEM hItem, CN3UIBase * pUIBase);

  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHierarchyView)
  public:
    virtual void OnDraw(CDC * pDC); // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT & cs);

  protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual void OnUpdate(CView * pSender, LPARAM lHint, CObject * pHint);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CHierarchyView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

  protected:
    // Generated message map functions
  protected:
    //{{AFX_MSG(CHierarchyView)
    afx_msg void OnRclick(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnKillFocus(CWnd * pNewWnd);
    afx_msg void OnSetFocus(CWnd * pOldWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // debug version in HierarchyView.cpp
inline CUIEDoc * CHierarchyView::GetDocument() {
    return (CUIEDoc *)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
