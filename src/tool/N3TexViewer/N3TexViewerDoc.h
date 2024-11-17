// N3TexViewerDoc.h : interface of the CN3TexViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Texture.h"

class CN3TexViewerDoc : public CDocument {
  protected: // create from serialization only
    CN3TexViewerDoc();
    DECLARE_DYNCREATE(CN3TexViewerDoc)

    // Attributes
  public:
    CN3Texture * m_pTex;
    CN3Texture * m_pTexAlpha;

    int                   m_nCurFile;
    fs::path              m_fsWorkDir;
    std::vector<fs::path> m_vDxtFiles;

    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CN3TexViewerDoc)
  public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void Serialize(CArchive & ar);
    virtual void SetTitle(LPCTSTR lpszTitle);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    void OpenFileAtIndex(int iIndex);
    void OpenLastFile();
    void OpenFirstFile();
    void OpenPrevFile();
    void OpenNextFile();
    void FindFiles();
    virtual ~CN3TexViewerDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

  protected:
    // Generated message map functions
  protected:
    //{{AFX_MSG(CN3TexViewerDoc)
    afx_msg void OnFileSaveAsBitmap();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
