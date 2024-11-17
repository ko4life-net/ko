#include "StdAfx.h"
#include "MakeItemGroupSet.h"
#include "AIServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemGroupSet

IMPLEMENT_DYNAMIC(CMakeItemGroupSet, CRecordset)

CMakeItemGroupSet::CMakeItemGroupSet(CDatabase * pdb)
    : CRecordset(pdb) {
    m_iItemGroupNum = 0;

    for (int i = 0; i < 30; i++) {
        m_iItem[i] = 0;
        m_sPersent[i] = 0;
    }

    m_nFields = 61;
    m_nDefaultType = snapshot;
}

CString CMakeItemGroupSet::GetDefaultConnect() {
    return CServerDlg::GetInstance()->GetGameDBConnectionString();
}

CString CMakeItemGroupSet::GetDefaultSQL() {
    return _T("[dbo].[MAKE_ITEM_GROUP]");
}

void CMakeItemGroupSet::DoFieldExchange(CFieldExchange * pFX) {
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Int(pFX, _T("[iItemGroupNum]"), m_iItemGroupNum);

    for (int i = 0; i < 30; i++) {
        CString itemField, persentField;
        itemField.Format(_T("[iItem_%02d]"), i + 1);
        persentField.Format(_T("[sPersent%02d]"), i + 1);

        RFX_Long(pFX, itemField, m_iItem[i]);
        RFX_Int(pFX, persentField, m_sPersent[i]);
    }
}

#ifdef _DEBUG
void CMakeItemGroupSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CMakeItemGroupSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
