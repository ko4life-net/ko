// VersionSet.cpp : implementation file
//

#include "StdAfx.h"
#include "LoginServer.h"
#include "VersionSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVersionSet

IMPLEMENT_DYNAMIC(CVersionSet, CRecordset)

CVersionSet::CVersionSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CVersionSet)
    m_sVersion = 0;
    m_sHistoryVersion = 0;
    m_nFields = 4;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CVersionSet::GetDefaultConnect() {
    return _T("ODBC;DSN=kodb;UID=kodb_user;PWD=kodb_user");
}

CString CVersionSet::GetDefaultSQL() {
    return _T("[dbo].[VERSION]");
}

void CVersionSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CVersionSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Int(pFX, _T("[sVersion]"), m_sVersion);
    RFX_Text(pFX, _T("[strFile]"), m_strFile);
    RFX_Text(pFX, _T("[strPatchFileName]"), m_strPatchFileName);
    RFX_Int(pFX, _T("[sHistoryVersion]"), m_sHistoryVersion);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CVersionSet diagnostics

#ifdef _DEBUG
void CVersionSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CVersionSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
