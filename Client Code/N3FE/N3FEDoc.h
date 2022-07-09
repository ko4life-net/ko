// N3FEDoc.h : interface of the CN3FEDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3FEDOC_H__0FA60B98_AF25_435B_9FEF_33730FA3AAFC__INCLUDED_)
#define AFX_N3FEDOC_H__0FA60B98_AF25_435B_9FEF_33730FA3AAFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../N3Base/N3EffectMgr.h"
#include "../N3Base/N3GlobalEffectMng.h"
class CN3FEDoc : public CDocument
{
public:
	CN3EffectMgr		m_EffectMgr;
	CStringArray		m_szBundleScripts;
	CStringArray		m_szPartScripts;

protected: // create from serialization only
	CN3FEDoc();
	DECLARE_DYNCREATE(CN3FEDoc)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CN3FEDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateEffectData(BOOL bSaveAndUpdateEffectData);
	virtual ~CN3FEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CN3FEDoc)
	afx_msg void OnFileImportBundle();
	afx_msg void OnFileImportParts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_N3FEDOC_H__0FA60B98_AF25_435B_9FEF_33730FA3AAFC__INCLUDED_)
