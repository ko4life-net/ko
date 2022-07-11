// ItemGroupSet.cpp : implementation file
//

#include "stdafx.h"
#include "ebenezer.h"
#include "ItemGroupSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet

IMPLEMENT_DYNAMIC(CItemGroupSet, CRecordset)

CItemGroupSet::CItemGroupSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CItemGroupSet)
	m_group = 0;
	m_name = _T("");
	m_item1 = 0;
	m_item2 = 0;
	m_item3 = 0;
	m_item4 = 0;
	m_item5 = 0;
	m_item6 = 0;
	m_item7 = 0;
	m_item8 = 0;
	m_item9 = 0;
	m_item10 = 0;
	m_item11 = 0;
	m_item12 = 0;
	m_item13 = 0;
	m_item14 = 0;
	m_item15 = 0;
	m_item16 = 0;
	m_item17 = 0;
	m_item18 = 0;
	m_item19 = 0;
	m_item20 = 0;
	m_item21 = 0;
	m_item22 = 0;
	m_item23 = 0;
	m_item24 = 0;
	m_item25 = 0;
	m_item26 = 0;
	m_item27 = 0;
	m_item28 = 0;
	m_item29 = 0;
	m_item30 = 0;
	m_nFields = 32;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CItemGroupSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=KN_Online;UID=knight;PWD=knight");
}

CString CItemGroupSet::GetDefaultSQL()
{
	return _T("[dbo].[ITEM_GROUP]");
}

void CItemGroupSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CItemGroupSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[group]"), m_group);
	RFX_Text(pFX, _T("[name]"), m_name);
	RFX_Long(pFX, _T("[item1]"), m_item1);
	RFX_Long(pFX, _T("[item2]"), m_item2);
	RFX_Long(pFX, _T("[item3]"), m_item3);
	RFX_Long(pFX, _T("[item4]"), m_item4);
	RFX_Long(pFX, _T("[item5]"), m_item5);
	RFX_Long(pFX, _T("[item6]"), m_item6);
	RFX_Long(pFX, _T("[item7]"), m_item7);
	RFX_Long(pFX, _T("[item8]"), m_item8);
	RFX_Long(pFX, _T("[item9]"), m_item9);
	RFX_Long(pFX, _T("[item10]"), m_item10);
	RFX_Long(pFX, _T("[item11]"), m_item11);
	RFX_Long(pFX, _T("[item12]"), m_item12);
	RFX_Long(pFX, _T("[item13]"), m_item13);
	RFX_Long(pFX, _T("[item14]"), m_item14);
	RFX_Long(pFX, _T("[item15]"), m_item15);
	RFX_Long(pFX, _T("[item16]"), m_item16);
	RFX_Long(pFX, _T("[item17]"), m_item17);
	RFX_Long(pFX, _T("[item18]"), m_item18);
	RFX_Long(pFX, _T("[item19]"), m_item19);
	RFX_Long(pFX, _T("[item20]"), m_item20);
	RFX_Long(pFX, _T("[item21]"), m_item21);
	RFX_Long(pFX, _T("[item22]"), m_item22);
	RFX_Long(pFX, _T("[item23]"), m_item23);
	RFX_Long(pFX, _T("[item24]"), m_item24);
	RFX_Long(pFX, _T("[item25]"), m_item25);
	RFX_Long(pFX, _T("[item26]"), m_item26);
	RFX_Long(pFX, _T("[item27]"), m_item27);
	RFX_Long(pFX, _T("[item28]"), m_item28);
	RFX_Long(pFX, _T("[item29]"), m_item29);
	RFX_Long(pFX, _T("[item30]"), m_item30);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet diagnostics

#ifdef _DEBUG
void CItemGroupSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CItemGroupSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
