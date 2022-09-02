// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : public CFrameWnd {

  protected: // create from serialization only
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

    // Generated message map functions
  protected:
    //{{AFX_MSG(CMainFrame)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
