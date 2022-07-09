#if !defined(AFX_BASEITEMTABLESET_H__509C714B_8EF6_406B_91D1_DFB516A5B807__INCLUDED_)
#define AFX_BASEITEMTABLESET_H__509C714B_8EF6_406B_91D1_DFB516A5B807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseItemTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseItemTableSet recordset

class CBaseItemTableSet : public CRecordset
{
public:
	CBaseItemTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBaseItemTableSet)

// Field/Param Data
	//{{AFX_FIELD(CBaseItemTableSet, CRecordset)
	long	m_Num;
	CString	m_strCode;
	CString	m_strName;
	BYTE	m_Group;
	BYTE	m_Type;
	BYTE	m_Gen;
	BYTE	m_Class;
	BYTE	m_ItemPos;
	long	m_Price;
	BYTE	m_PotType;
	long	m_Pot;
	BYTE	m_AatMin;
	BYTE	m_AatMax;
	BYTE	m_DatMin;
	BYTE	m_DatMax;
	BYTE	m_Dis;
	BYTE	m_AMat;
	BYTE	m_DMat;
	BYTE	m_Ac;
	BYTE	m_Amr;
	BYTE	m_Dmr;
	int		m_Hr;
	int		m_Avo;
	long	m_SpecialMac;
	int		m_Contime;
	int		m_Mac;
	int		m_Wgt;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseItemTableSet)
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

#endif // !defined(AFX_BASEITEMTABLESET_H__509C714B_8EF6_406B_91D1_DFB516A5B807__INCLUDED_)
