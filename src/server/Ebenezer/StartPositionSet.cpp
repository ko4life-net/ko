// StartPositionSet.cpp : implementation file
//

#include "stdafx.h"
#include "ebenezer.h"
#include "StartPositionSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartPositionSet

IMPLEMENT_DYNAMIC(CStartPositionSet, CRecordset)

CStartPositionSet::CStartPositionSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CStartPositionSet)
    m_ZoneID = 0;
    m_sKarusX = 0;
    m_sKarusZ = 0;
    m_sElmoradX = 0;
    m_sElmoradZ = 0;
    m_bRangeX = 0;
    m_bRangeZ = 0;
    m_sKarusGateX = 0;
    m_sKarusGateZ = 0;
    m_sElmoGateX = 0;
    m_sElmoGateZ = 0;
    m_nFields = 11;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CStartPositionSet::GetDefaultConnect() {
    return _T("ODBC;DSN=kodb;UID=kodb_user;PWD=kodb_user");
}

CString CStartPositionSet::GetDefaultSQL() {
    return _T("[dbo].[START_POSITION]");
}

void CStartPositionSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CStartPositionSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("[ZoneID]"), m_ZoneID);
    RFX_Long(pFX, _T("[sKarusX]"), m_sKarusX);
    RFX_Long(pFX, _T("[sKarusZ]"), m_sKarusZ);
    RFX_Long(pFX, _T("[sElmoradX]"), m_sElmoradX);
    RFX_Long(pFX, _T("[sElmoradZ]"), m_sElmoradZ);
    RFX_Byte(pFX, _T("[bRangeX]"), m_bRangeX);
    RFX_Byte(pFX, _T("[bRangeZ]"), m_bRangeZ);
    RFX_Long(pFX, _T("[sKarusGateX]"), m_sKarusGateX);
    RFX_Long(pFX, _T("[sKarusGateZ]"), m_sKarusGateZ);
    RFX_Long(pFX, _T("[sElmoGateX]"), m_sElmoGateX);
    RFX_Long(pFX, _T("[sElmoGateZ]"), m_sElmoGateZ);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStartPositionSet diagnostics

#ifdef _DEBUG
void CStartPositionSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CStartPositionSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
