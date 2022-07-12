// ServerInfoViewerDoc.h : interface of the CServerInfoViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERINFOVIEWERDOC_H__B0C14D87_7585_4D1D_984E_79F85FF53FD4__INCLUDED_)
#define AFX_SERVERINFOVIEWERDOC_H__B0C14D87_7585_4D1D_984E_79F85FF53FD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct __CharacterSymbol
{
	__Vector3		vBoxes[5]; // 상자를 LineStrip 으로 그린다. 5
	__Vector3		vDirections[3];
	__Matrix44		mtxWorld;
	D3DCOLOR		color; // 색..
	char			szID[64]; // ID
	char			szRemark[128]; // 주석???
	
	__CharacterSymbol()
	{
		__Matrix44 mtxTmp;
		mtxWorld.Identity();
		color = 0xffffffff; // 기본은 흰색..
		szID[0] = NULL; // ID
		szRemark[0] = NULL; // 주석???

		float fRadius = 2.0f;
		vBoxes[0].Set(-fRadius, 0, fRadius);
		vBoxes[1].Set( fRadius, 0, fRadius);
		vBoxes[2].Set( fRadius, 0, -fRadius);
		vBoxes[3].Set(-fRadius, 0, -fRadius);
		vBoxes[4] = vBoxes[0];

		vDirections[0].Zero();
		vDirections[1].Set(0, 0, fRadius*2);
		vDirections[2].Set(0, (fRadius/5.0f), fRadius*2 - (fRadius/5.0f));
	}
};

#include <list>

typedef std::list<__CharacterSymbol>::iterator it_CSymbol;

class CServerInfoViewerDoc : public CDocument
{
public:
	std::list<__CharacterSymbol>	m_Players;
	std::list<__CharacterSymbol>	m_NPCs;
	std::list<__CharacterSymbol>	m_Monsters;

protected: // create from serialization only
	CServerInfoViewerDoc();
	DECLARE_DYNCREATE(CServerInfoViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerInfoViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CServerInfoViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CServerInfoViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERINFOVIEWERDOC_H__B0C14D87_7585_4D1D_984E_79F85FF53FD4__INCLUDED_)
