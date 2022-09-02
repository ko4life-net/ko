// N3CEDoc.h : interface of the CN3CEDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Scene.h"

class CN3CEDoc : public CDocument {
  public:
    CN3Scene m_Scene;
    BOOL     m_bLoadingNow;

  protected: // create from serialization only
    CN3CEDoc();
    DECLARE_DYNCREATE(CN3CEDoc)

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CN3CEDoc)
  public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive & ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void OnCloseDocument();
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CN3CEDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

  protected:
    // Generated message map functions
  protected:
    //{{AFX_MSG(CN3CEDoc)
    afx_msg void OnFileSaveAsOneFolder();
    afx_msg void OnToolOptimizeAnimationData();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
