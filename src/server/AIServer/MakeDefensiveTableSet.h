#pragma once

// MakeDefensiveTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeDefensiveTableSet recordset

class CMakeDefensiveTableSet : public CRecordset
{
public:
	CMakeDefensiveTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMakeDefensiveTableSet)

// Field/Param Data
	//{{AFX_FIELD(CMakeDefensiveTableSet, CRecordset)
	BYTE	m_byLevel;
	int		m_sClass_1;
	int		m_sClass_2;
	int		m_sClass_3;
	int		m_sClass_4;
	int		m_sClass_5;
	int		m_sClass_6;
	int		m_sClass_7;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeDefensiveTableSet)
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

