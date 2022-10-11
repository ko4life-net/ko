// PreFix2TableSet.cpp : implementation file
//

#include "StdAfx.h"
#include "ebenezer.h"
#include "PreFix2TableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreFix2TableSet

IMPLEMENT_DYNAMIC(CPreFix2TableSet, CRecordset)

CPreFix2TableSet::CPreFix2TableSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CPreFix2TableSet)
    m_Num = 0;
    m_strCode = _T("");
    m_strName = _T("");
    m_Group = 0;
    m_Type = 0;
    m_Str = 0;
    m_Dex = 0;
    m_Wis = 0;
    m_Con = 0;
    m_Agi = 0;
    m_Aat = 0;
    m_Dat = 0;
    m_AMat = 0;
    m_DMat = 0;
    m_Finat = 0;
    m_Hpmax = 0;
    m_Mpmax = 0;
    m_Mvs = 0;
    m_Casts = 0;
    m_Ats = 0;
    m_Luck = 0;
    m_Avo = 0;
    m_Hr = 0;
    m_Sr = 0;
    m_Ac = 0;
    m_Amr = 0;
    m_Dmr = 0;
    m_Redmag = 0;
    m_Redatt = 0;
    m_Repdam = 0;
    m_GetHp = 0;
    m_GetMp = 0;
    m_nFields = 32;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CPreFix2TableSet::GetDefaultConnect() {
    return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CPreFix2TableSet::GetDefaultSQL() {
    return _T("[dbo].[PREFIX2]");
}

void CPreFix2TableSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CPreFix2TableSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("[Num]"), m_Num);
    RFX_Text(pFX, _T("[strCode]"), m_strCode);
    RFX_Text(pFX, _T("[strName]"), m_strName);
    RFX_Byte(pFX, _T("[Group]"), m_Group);
    RFX_Byte(pFX, _T("[Type]"), m_Type);
    RFX_Byte(pFX, _T("[Str]"), m_Str);
    RFX_Byte(pFX, _T("[Dex]"), m_Dex);
    RFX_Byte(pFX, _T("[Wis]"), m_Wis);
    RFX_Byte(pFX, _T("[Con]"), m_Con);
    RFX_Byte(pFX, _T("[Agi]"), m_Agi);
    RFX_Byte(pFX, _T("[Aat]"), m_Aat);
    RFX_Byte(pFX, _T("[Dat]"), m_Dat);
    RFX_Byte(pFX, _T("[AMat]"), m_AMat);
    RFX_Byte(pFX, _T("[DMat]"), m_DMat);
    RFX_Int(pFX, _T("[Finat]"), m_Finat);
    RFX_Int(pFX, _T("[Hpmax]"), m_Hpmax);
    RFX_Int(pFX, _T("[Mpmax]"), m_Mpmax);
    RFX_Byte(pFX, _T("[Mvs]"), m_Mvs);
    RFX_Byte(pFX, _T("[Casts]"), m_Casts);
    RFX_Int(pFX, _T("[Ats]"), m_Ats);
    RFX_Byte(pFX, _T("[Luck]"), m_Luck);
    RFX_Int(pFX, _T("[Avo]"), m_Avo);
    RFX_Int(pFX, _T("[Hr]"), m_Hr);
    RFX_Byte(pFX, _T("[Sr]"), m_Sr);
    RFX_Byte(pFX, _T("[Ac]"), m_Ac);
    RFX_Byte(pFX, _T("[Amr]"), m_Amr);
    RFX_Byte(pFX, _T("[Dmr]"), m_Dmr);
    RFX_Byte(pFX, _T("[Redmag]"), m_Redmag);
    RFX_Byte(pFX, _T("[Redatt]"), m_Redatt);
    RFX_Byte(pFX, _T("[Repdam]"), m_Repdam);
    RFX_Byte(pFX, _T("[GetHp]"), m_GetHp);
    RFX_Byte(pFX, _T("[GetMp]"), m_GetMp);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPreFix2TableSet diagnostics

#ifdef _DEBUG
void CPreFix2TableSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CPreFix2TableSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
