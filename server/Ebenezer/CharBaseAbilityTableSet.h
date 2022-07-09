#if !defined(AFX_CHARBASEABILITYTABLESET_H__B30A5230_FA2B_4094_8B84_AF306ED89C3D__INCLUDED_)
#define AFX_CHARBASEABILITYTABLESET_H__B30A5230_FA2B_4094_8B84_AF306ED89C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharBaseAbilityTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharBaseAbilityTableSet recordset

class CCharBaseAbilityTableSet : public CRecordset
{
public:
	CCharBaseAbilityTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCharBaseAbilityTableSet)

// Field/Param Data
	//{{AFX_FIELD(CCharBaseAbilityTableSet, CRecordset)
	long	m_Num;
	BYTE	m_Class;
	BYTE	m_Tribe;
	BYTE	m_Gen;
	BYTE	m_Str;
	BYTE	m_Dex;
	BYTE	m_Wis;
	BYTE	m_Con;
	BYTE	m_Agi;
	BYTE	m_UpHpMin;
	BYTE	m_UpHpMax;
	BYTE	m_UpMpMin;
	BYTE	m_UpMpMax;
	BYTE	m_UpStMin;
	BYTE	m_UpStMax;
	BYTE	m_LvUpSt;
	BYTE	m_ReHp;
	BYTE	m_ReMp;
	long	m_ReHpSec;
	long	m_ReMpSec;
	int		m_Wgt;
	BYTE	m_Hp;
	BYTE	m_Mp;
	BYTE	m_St;
	BYTE	m_Wp;
	BYTE	m_DecSt;
	BYTE	m_ReWalkSt;
	BYTE	m_ReStandSt;
	long	m_ReStSec;
	BYTE	m_Mr;
	BYTE	m_UpMr;
	BYTE	m_LvUpMr;
	BYTE	m_Ats;
	BYTE	m_Mvs;
	BYTE	m_Cats;
	BYTE	m_Sr;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharBaseAbilityTableSet)
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

#endif // !defined(AFX_CHARBASEABILITYTABLESET_H__B30A5230_FA2B_4094_8B84_AF306ED89C3D__INCLUDED_)
