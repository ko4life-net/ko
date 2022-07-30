#pragma once

// LevelUpTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLevelUpTableSet recordset

class CLevelUpTableSet : public CRecordset
{
public:
	CLevelUpTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CLevelUpTableSet)

// Field/Param Data
	//{{AFX_FIELD(CLevelUpTableSet, CRecordset)
	BYTE	m_level;
	long	m_Exp;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelUpTableSet)
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

