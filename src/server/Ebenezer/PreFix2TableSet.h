#if !defined(AFX_PREFIX2TABLESET_H__201B34F4_71F4_40F0_86D5_4F1A60D86701__INCLUDED_)
#define AFX_PREFIX2TABLESET_H__201B34F4_71F4_40F0_86D5_4F1A60D86701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreFix2TableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreFix2TableSet recordset

class CPreFix2TableSet : public CRecordset
{
public:
	CPreFix2TableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPreFix2TableSet)

// Field/Param Data
	//{{AFX_FIELD(CPreFix2TableSet, CRecordset)
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
	//{{AFX_VIRTUAL(CPreFix2TableSet)
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

#endif // !defined(AFX_PREFIX2TABLESET_H__201B34F4_71F4_40F0_86D5_4F1A60D86701__INCLUDED_)
