#if !defined(AFX_RNPCPOSSET_H__A7F476F8_A63E_4734_8763_7DD5C5115D54__INCLUDED_)
#define AFX_RNPCPOSSET_H__A7F476F8_A63E_4734_8763_7DD5C5115D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RNpcPosSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRNpcPosSet recordset

class CRNpcPosSet : public CRecordset
{
public:
	CRNpcPosSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRNpcPosSet)

// Field/Param Data
	//{{AFX_FIELD(CRNpcPosSet, CRecordset)
	int		m_ZoneID;
	long	m_NpcID;
	BYTE	m_ActType;
	BYTE	m_RegenType;
	BYTE	m_DungeonFamily;
	BYTE	m_SpecialType;
	BYTE	m_TrapNumber;
	long	m_LeftX;
	long	m_TopZ;
	long	m_RightX;
	long	m_BottomZ;
	long	m_LimitMinX;
	long	m_LimitMinZ;
	long	m_LimitMaxX;
	long	m_LimitMaxZ;
	BYTE	m_NumNPC;
	int		m_RegTime;
	BYTE	m_DotCnt;
	CString	m_path;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRNpcPosSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RNPCPOSSET_H__A7F476F8_A63E_4734_8763_7DD5C5115D54__INCLUDED_)
