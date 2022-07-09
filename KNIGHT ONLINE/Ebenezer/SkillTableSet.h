#if !defined(AFX_SKILLTABLESET_H__4C609A54_A619_4D02_87C8_DA6F9B6462DD__INCLUDED_)
#define AFX_SKILLTABLESET_H__4C609A54_A619_4D02_87C8_DA6F9B6462DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkillTableSet recordset

class CSkillTableSet : public CRecordset
{
public:
	CSkillTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSkillTableSet)

// Field/Param Data
	//{{AFX_FIELD(CSkillTableSet, CRecordset)
	long	m_Num;
	BYTE	m_Group;
	BYTE	m_Lv;
	int		m_Pic;
	CString	m_strName;
	int		m_Hp;
	int		m_Mp;
	int		m_HpMax;
	int		m_MpMax;
	BYTE	m_ResHp;
	BYTE	m_Soul;
	BYTE	m_Str;
	BYTE	m_Agi;
	BYTE	m_Dex;
	int		m_Hr;
	int		m_Avo;
	BYTE	m_totAddMin;
	BYTE	m_totAddMax;
	BYTE	m_totSubAddMin;
	BYTE	m_totSubAddMax;
	long	m_curMaxPoint;
	BYTE	m_ReqLv;
	BYTE	m_curAddMin;
	BYTE	m_curAddMax;
	long	m_curSubMaxPoint;
	int		m_curSubAddMin;
	int		m_curSubAddMax;
	BYTE	m_Success;
	BYTE	m_Type1;
	BYTE	m_Type2;
	BYTE	m_Type3;
	BYTE	m_Type4;
	BYTE	m_Type5;
	BYTE	m_Dist;
	int		m_DmgMin1;
	int		m_DmgMax1;
	int		m_DmgMin2;
	int		m_DmgMax2;
	int		m_Mvs;
	int		m_Ats;
	int		m_Contime;
	int		m_Interval;
	long	m_NeedStuff1;
	BYTE	m_Count1;
	long	m_NeedStuff2;
	BYTE	m_Count2;
	long	m_NeedStuff3;
	BYTE	m_Count3;
	long	m_NeedStuff4;
	BYTE	m_Count4;
	BYTE	m_NickName;
	CString	m_Explain;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillTableSet)
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

#endif // !defined(AFX_SKILLTABLESET_H__4C609A54_A619_4D02_87C8_DA6F9B6462DD__INCLUDED_)
