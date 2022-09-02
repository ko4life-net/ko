// KscViewerDoc.h : interface of the CKscViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "N3JpegFile.h" // Added by ClassView

class CKscViewerDoc : public CDocument {
  protected: // create from serialization only
    CKscViewerDoc();
    DECLARE_DYNCREATE(CKscViewerDoc)

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CKscViewerDoc)
  public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive & ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    CString       m_szKscPath;
    CN3JpegFile * m_pJpegFile;
    CN3JpegFile * GetJpegFile();
    virtual ~CKscViewerDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

  protected:
    // Generated message map functions
  protected:
    //{{AFX_MSG(CKscViewerDoc)
    afx_msg void OnFileSave();
    afx_msg void OnFileSaveAs();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
