// StoreInfoTableSet.cpp : implementation file
//

#include "StdAfx.h"
#include "ebenezer.h"
#include "StoreInfoTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreInfoTableSet

IMPLEMENT_DYNAMIC(CStoreInfoTableSet, CRecordset)

CStoreInfoTableSet::CStoreInfoTableSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CStoreInfoTableSet)
    m_store_id = 0;
    m_store_moral = 0;
    m_store_name = _T("");
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
    m_item31 = 0;
    m_item32 = 0;
    m_item33 = 0;
    m_item34 = 0;
    m_item35 = 0;
    m_item36 = 0;
    m_item37 = 0;
    m_item38 = 0;
    m_item39 = 0;
    m_item40 = 0;
    m_nFields = 43;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CStoreInfoTableSet::GetDefaultConnect() {
    return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CStoreInfoTableSet::GetDefaultSQL() {
    return _T("[dbo].[STORE_INFO]");
}

void CStoreInfoTableSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CStoreInfoTableSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("[store_id]"), m_store_id);
    RFX_Byte(pFX, _T("[store_moral]"), m_store_moral);
    RFX_Text(pFX, _T("[store_name]"), m_store_name);
    RFX_Int(pFX, _T("[item1]"), m_item1);
    RFX_Int(pFX, _T("[item2]"), m_item2);
    RFX_Int(pFX, _T("[item3]"), m_item3);
    RFX_Int(pFX, _T("[item4]"), m_item4);
    RFX_Int(pFX, _T("[item5]"), m_item5);
    RFX_Int(pFX, _T("[item6]"), m_item6);
    RFX_Int(pFX, _T("[item7]"), m_item7);
    RFX_Int(pFX, _T("[item8]"), m_item8);
    RFX_Int(pFX, _T("[item9]"), m_item9);
    RFX_Int(pFX, _T("[item10]"), m_item10);
    RFX_Int(pFX, _T("[item11]"), m_item11);
    RFX_Int(pFX, _T("[item12]"), m_item12);
    RFX_Int(pFX, _T("[item13]"), m_item13);
    RFX_Int(pFX, _T("[item14]"), m_item14);
    RFX_Int(pFX, _T("[item15]"), m_item15);
    RFX_Int(pFX, _T("[item16]"), m_item16);
    RFX_Int(pFX, _T("[item17]"), m_item17);
    RFX_Int(pFX, _T("[item18]"), m_item18);
    RFX_Int(pFX, _T("[item19]"), m_item19);
    RFX_Int(pFX, _T("[item20]"), m_item20);
    RFX_Int(pFX, _T("[item21]"), m_item21);
    RFX_Int(pFX, _T("[item22]"), m_item22);
    RFX_Int(pFX, _T("[item23]"), m_item23);
    RFX_Int(pFX, _T("[item24]"), m_item24);
    RFX_Int(pFX, _T("[item25]"), m_item25);
    RFX_Int(pFX, _T("[item26]"), m_item26);
    RFX_Int(pFX, _T("[item27]"), m_item27);
    RFX_Int(pFX, _T("[item28]"), m_item28);
    RFX_Int(pFX, _T("[item29]"), m_item29);
    RFX_Int(pFX, _T("[item30]"), m_item30);
    RFX_Int(pFX, _T("[item31]"), m_item31);
    RFX_Int(pFX, _T("[item32]"), m_item32);
    RFX_Int(pFX, _T("[item33]"), m_item33);
    RFX_Int(pFX, _T("[item34]"), m_item34);
    RFX_Int(pFX, _T("[item35]"), m_item35);
    RFX_Int(pFX, _T("[item36]"), m_item36);
    RFX_Int(pFX, _T("[item37]"), m_item37);
    RFX_Int(pFX, _T("[item38]"), m_item38);
    RFX_Int(pFX, _T("[item39]"), m_item39);
    RFX_Int(pFX, _T("[item40]"), m_item40);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStoreInfoTableSet diagnostics

#ifdef _DEBUG
void CStoreInfoTableSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CStoreInfoTableSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
