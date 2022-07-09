#if !defined(AFX_PREFIX1TABLESET_H__52FDFC49_8160_4025_8F57_D96F1F5BE5F6__INCLUDED_)
#define AFX_PREFIX1TABLESET_H__52FDFC49_8160_4025_8F57_D96F1F5BE5F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreFix1TableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreFix1TableSet recordset

class CPreFix1TableSet : public CRecordset
{
public:
	CPreFix1TableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPreFix1TableSet)

// Field/Param Data
	//{{AFX_FIELD(CPreFix1TableSet, CRecordset)
	long	m_Num;
	CString	m_strCode;
	CString	m_strName;
	BYTE	m_Group;
	BYTE	m_Type;
	BYTE	m_Str;
	BYTE	m_Dex;
	BYTE	m_Wis;
	BYTE	m_Con;
	BYTE	m_Agi;
	BYTE	m_Aat;
	BYTE	m_Dat;
	BYTE	m_AMat;
	BYTE	m_DMat;
	int		m_Finat;
	int		m_Hpmax;
	int		m_Mpmax;
	BYTE	m_Mvs;
	BYTE	m_Casts;
	int		m_Ats;
	BYTE	m_Luck;
	int		m_Avo;
	int		m_Hr;
	BYTE	m_Sr;
	BYTE	m_Ac;
	BYTE	m_Amr;
	BYTE	m_Dmr;
	BYTE	m_Redmag;
	BYTE	m_Redatt;
	BYTE	m_Repdam;
	BYTE	m_GetHp;
	BYTE	m_GetMp;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreFix1TableSet)
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

#endif // !defined(AFX_PREFIX1TABLESET_H__52FDFC49_8160_4025_8F57_D96F1F5BE5F6__INCLUDED_)
