// RNpcPosSet.cpp : implementation file
//

#include "StdAfx.h"
#include "AIServer.h"
#include "RNpcPosSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRNpcPosSet

IMPLEMENT_DYNAMIC(CRNpcPosSet, CRecordset)

CRNpcPosSet::CRNpcPosSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CRNpcPosSet)
    m_ZoneID = 0;
    m_NpcID = 0;
    m_ActType = 0;
    m_RegenType = 0;
    m_DungeonFamily = 0;
    m_SpecialType = 0;
    m_TrapNumber = 0;
    m_LeftX = 0;
    m_TopZ = 0;
    m_RightX = 0;
    m_BottomZ = 0;
    m_LimitMinX = 0;
    m_LimitMinZ = 0;
    m_LimitMaxX = 0;
    m_LimitMaxZ = 0;
    m_NumNPC = 0;
    m_RegTime = 0;
    m_DotCnt = 0;
    m_path = _T("");
    m_nFields = 19;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CRNpcPosSet::GetDefaultConnect() {
    return _T("ODBC;DSN=kodb;UID=kodb_user;PWD=kodb_user");
}

CString CRNpcPosSet::GetDefaultSQL() {
    //return _T("[dbo].[REPENT_NPC_POS]");
    return _T("[dbo].[NPC_POS]");
}

void CRNpcPosSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CRNpcPosSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Int(pFX, _T("[ZoneID]"), m_ZoneID);
    RFX_Long(pFX, _T("[NpcID]"), m_NpcID);
    RFX_Byte(pFX, _T("[ActType]"), m_ActType);
    RFX_Byte(pFX, _T("[RegenType]"), m_RegenType);
    RFX_Byte(pFX, _T("[DungeonFamily]"), m_DungeonFamily);
    RFX_Byte(pFX, _T("[SpecialType]"), m_SpecialType);
    RFX_Byte(pFX, _T("[TrapNumber]"), m_TrapNumber);
    RFX_Long(pFX, _T("[LeftX]"), m_LeftX);
    RFX_Long(pFX, _T("[TopZ]"), m_TopZ);
    RFX_Long(pFX, _T("[RightX]"), m_RightX);
    RFX_Long(pFX, _T("[BottomZ]"), m_BottomZ);
    RFX_Long(pFX, _T("[LimitMinX]"), m_LimitMinX);
    RFX_Long(pFX, _T("[LimitMinZ]"), m_LimitMinZ);
    RFX_Long(pFX, _T("[LimitMaxX]"), m_LimitMaxX);
    RFX_Long(pFX, _T("[LimitMaxZ]"), m_LimitMaxZ);
    RFX_Byte(pFX, _T("[NumNPC]"), m_NumNPC);
    RFX_Int(pFX, _T("[RegTime]"), m_RegTime);
    RFX_Byte(pFX, _T("[DotCnt]"), m_DotCnt);
    RFX_Text(pFX, _T("[path]"), m_path);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRNpcPosSet diagnostics

#ifdef _DEBUG
void CRNpcPosSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CRNpcPosSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
