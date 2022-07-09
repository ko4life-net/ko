#if !defined(AFX_CHARACTERDATASET_H__092DF295_1C05_4D8C_B724_78272903037A__INCLUDED_)
#define AFX_CHARACTERDATASET_H__092DF295_1C05_4D8C_B724_78272903037A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterDataSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterDataSet recordset

class CCharacterDataSet : public CRecordset
{
public:
	CCharacterDataSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCharacterDataSet)

// Field/Param Data
	//{{AFX_FIELD(CCharacterDataSet, CRecordset)
	BYTE	m_Num;
	CString	m_Class;
	double	m_HP;
	double	m_MP;
	double	m_SP;
	double	m_Hit;
	double	m_AC1;
	double	m_AC2;
	double	m_AGI1;
	double	m_AGI2;
	BYTE	m_RMP;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterDataSet)
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

#endif // !defined(AFX_CHARACTERDATASET_H__092DF295_1C05_4D8C_B724_78272903037A__INCLUDED_)
