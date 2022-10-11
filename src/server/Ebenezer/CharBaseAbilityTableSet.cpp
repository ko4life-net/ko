// CharBaseAbilityTableSet.cpp : implementation file
//

#include "StdAfx.h"
#include "ebenezer.h"
#include "CharBaseAbilityTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharBaseAbilityTableSet

IMPLEMENT_DYNAMIC(CCharBaseAbilityTableSet, CRecordset)

CCharBaseAbilityTableSet::CCharBaseAbilityTableSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CCharBaseAbilityTableSet)
    m_Num = 0;
    m_Class = 0;
    m_Tribe = 0;
    m_Gen = 0;
    m_Str = 0;
    m_Dex = 0;
    m_Wis = 0;
    m_Con = 0;
    m_Agi = 0;
    m_UpHpMin = 0;
    m_UpHpMax = 0;
    m_UpMpMin = 0;
    m_UpMpMax = 0;
    m_UpStMin = 0;
    m_UpStMax = 0;
    m_LvUpSt = 0;
    m_ReHp = 0;
    m_ReMp = 0;
    m_ReHpSec = 0;
    m_ReMpSec = 0;
    m_Wgt = 0;
    m_Hp = 0;
    m_Mp = 0;
    m_St = 0;
    m_Wp = 0;
    m_DecSt = 0;
    m_ReWalkSt = 0;
    m_ReStandSt = 0;
    m_ReStSec = 0;
    m_Mr = 0;
    m_UpMr = 0;
    m_LvUpMr = 0;
    m_Ats = 0;
    m_Mvs = 0;
    m_Cats = 0;
    m_Sr = 0;
    m_nFields = 36;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CCharBaseAbilityTableSet::GetDefaultConnect() {
    return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CCharBaseAbilityTableSet::GetDefaultSQL() {
    return _T("[dbo].[CHAR_BASE_ABILITY]");
}

void CCharBaseAbilityTableSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CCharBaseAbilityTableSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("[Num]"), m_Num);
    RFX_Byte(pFX, _T("[Class]"), m_Class);
    RFX_Byte(pFX, _T("[Tribe]"), m_Tribe);
    RFX_Byte(pFX, _T("[Gen]"), m_Gen);
    RFX_Byte(pFX, _T("[Str]"), m_Str);
    RFX_Byte(pFX, _T("[Dex]"), m_Dex);
    RFX_Byte(pFX, _T("[Wis]"), m_Wis);
    RFX_Byte(pFX, _T("[Con]"), m_Con);
    RFX_Byte(pFX, _T("[Agi]"), m_Agi);
    RFX_Byte(pFX, _T("[UpHpMin]"), m_UpHpMin);
    RFX_Byte(pFX, _T("[UpHpMax]"), m_UpHpMax);
    RFX_Byte(pFX, _T("[UpMpMin]"), m_UpMpMin);
    RFX_Byte(pFX, _T("[UpMpMax]"), m_UpMpMax);
    RFX_Byte(pFX, _T("[UpStMin]"), m_UpStMin);
    RFX_Byte(pFX, _T("[UpStMax]"), m_UpStMax);
    RFX_Byte(pFX, _T("[LvUpSt]"), m_LvUpSt);
    RFX_Byte(pFX, _T("[ReHp]"), m_ReHp);
    RFX_Byte(pFX, _T("[ReMp]"), m_ReMp);
    RFX_Long(pFX, _T("[ReHpSec]"), m_ReHpSec);
    RFX_Long(pFX, _T("[ReMpSec]"), m_ReMpSec);
    RFX_Int(pFX, _T("[Wgt]"), m_Wgt);
    RFX_Byte(pFX, _T("[Hp]"), m_Hp);
    RFX_Byte(pFX, _T("[Mp]"), m_Mp);
    RFX_Byte(pFX, _T("[St]"), m_St);
    RFX_Byte(pFX, _T("[Wp]"), m_Wp);
    RFX_Byte(pFX, _T("[DecSt]"), m_DecSt);
    RFX_Byte(pFX, _T("[ReWalkSt]"), m_ReWalkSt);
    RFX_Byte(pFX, _T("[ReStandSt]"), m_ReStandSt);
    RFX_Long(pFX, _T("[ReStSec]"), m_ReStSec);
    RFX_Byte(pFX, _T("[Mr]"), m_Mr);
    RFX_Byte(pFX, _T("[UpMr]"), m_UpMr);
    RFX_Byte(pFX, _T("[LvUpMr]"), m_LvUpMr);
    RFX_Byte(pFX, _T("[Ats]"), m_Ats);
    RFX_Byte(pFX, _T("[Mvs]"), m_Mvs);
    RFX_Byte(pFX, _T("[Cats]"), m_Cats);
    RFX_Byte(pFX, _T("[Sr]"), m_Sr);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCharBaseAbilityTableSet diagnostics

#ifdef _DEBUG
void CCharBaseAbilityTableSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CCharBaseAbilityTableSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
