#if !defined(AFX_STOREINFOTABLESET_H__DD3326A4_C333_4457_AFCE_92B3FEB682FB__INCLUDED_)
#define AFX_STOREINFOTABLESET_H__DD3326A4_C333_4457_AFCE_92B3FEB682FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StoreInfoTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStoreInfoTableSet recordset

class CStoreInfoTableSet : public CRecordset
{
public:
	CStoreInfoTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStoreInfoTableSet)

// Field/Param Data
	//{{AFX_FIELD(CStoreInfoTableSet, CRecordset)
	long	m_store_id;
	BYTE	m_store_moral;
	CString	m_store_name;	
	int		m_item1;
	int		m_item2;
	int		m_item3;
	int		m_item4;
	int		m_item5;
	int		m_item6;
	int		m_item7;
	int		m_item8;
	int		m_item9;
	int		m_item10;
	int		m_item11;
	int		m_item12;
	int		m_item13;
	int		m_item14;
	int		m_item15;
	int		m_item16;
	int		m_item17;
	int		m_item18;
	int		m_item19;
	int		m_item20;
	int		m_item21;	
	int		m_item22;
	int		m_item23;
	int		m_item24;
	int		m_item25;
	int		m_item26;
	int		m_item27;
	int		m_item28;
	int		m_item29;
	int		m_item30;
	int		m_item31;
	int		m_item32;
	int		m_item33;
	int		m_item34;
	int		m_item35;
	int		m_item36;
	int		m_item37;
	int		m_item38;
	int		m_item39;
	int		m_item40;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStoreInfoTableSet)
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

#endif // !defined(AFX_STOREINFOTABLESET_H__DD3326A4_C333_4457_AFCE_92B3FEB682FB__INCLUDED_)
