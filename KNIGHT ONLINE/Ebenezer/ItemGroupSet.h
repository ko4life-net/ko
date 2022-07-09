#if !defined(AFX_ITEMGROUPSET_H__973066D1_3776_422A_9D36_CE0C94B5E256__INCLUDED_)
#define AFX_ITEMGROUPSET_H__973066D1_3776_422A_9D36_CE0C94B5E256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemGroupSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet recordset

class CItemGroupSet : public CRecordset
{
public:
	CItemGroupSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CItemGroupSet)

// Field/Param Data
	//{{AFX_FIELD(CItemGroupSet, CRecordset)
	int		m_group;
	CString	m_name;
	long	m_item1;
	long	m_item2;
	long	m_item3;
	long	m_item4;
	long	m_item5;
	long	m_item6;
	long	m_item7;
	long	m_item8;
	long	m_item9;
	long	m_item10;
	long	m_item11;
	long	m_item12;
	long	m_item13;
	long	m_item14;
	long	m_item15;
	long	m_item16;
	long	m_item17;
	long	m_item18;
	long	m_item19;
	long	m_item20;
	long	m_item21;
	long	m_item22;
	long	m_item23;
	long	m_item24;
	long	m_item25;
	long	m_item26;
	long	m_item27;
	long	m_item28;
	long	m_item29;
	long	m_item30;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemGroupSet)
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

#endif // !defined(AFX_ITEMGROUPSET_H__973066D1_3776_422A_9D36_CE0C94B5E256__INCLUDED_)
