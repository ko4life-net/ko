// SkyViewerDoc.h : interface of the CSkyViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3SkyMng.h"
#include "N3Base/N3GlobalEffectMng.h"

class CSkyViewerDoc : public CDocument
{
public:
	CN3SkyMng			m_Sky;
	CN3GlobalEffectMng	m_Weather;
protected: // create from serialization only
	CSkyViewerDoc();
	DECLARE_DYNCREATE(CSkyViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkyViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkyViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSkyViewerDoc)
	afx_msg void OnFileSaveAsGameFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

