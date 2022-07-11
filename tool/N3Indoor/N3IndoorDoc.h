// N3IndoorDoc.h : interface of the CN3IndoorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3INDOORDOC_H__4AC24108_B961_4DD5_9BEF_D132AA550DC7__INCLUDED_)
#define AFX_N3INDOORDOC_H__4AC24108_B961_4DD5_9BEF_D132AA550DC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CN3IndoorDoc : public CDocument
{
protected: // create from serialization only
	CN3IndoorDoc();
	DECLARE_DYNCREATE(CN3IndoorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CN3IndoorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CN3IndoorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CN3IndoorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_N3INDOORDOC_H__4AC24108_B961_4DD5_9BEF_D132AA550DC7__INCLUDED_)
