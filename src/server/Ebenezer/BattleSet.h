#pragma once

// BattleSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBattleSet recordset

class CBattleSet : public CRecordset
{
public:
	CBattleSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBattleSet)

// Field/Param Data
	//{{AFX_FIELD(CBattleSet, CRecordset)
	int		m_sIndex;
	BYTE	m_byNation;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBattleSet)
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

