// BaseItemTableSet.cpp : implementation file
//

#include "StdAfx.h"
#include "ebenezer.h"
#include "BaseItemTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseItemTableSet

IMPLEMENT_DYNAMIC(CBaseItemTableSet, CRecordset)

CBaseItemTableSet::CBaseItemTableSet(CDatabase * pdb)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CBaseItemTableSet)
    m_Num = 0;
    m_strCode = _T("");
    m_strName = _T("");
    m_Group = 0;
    m_Type = 0;
    m_Gen = 0;
    m_Class = 0;
    m_ItemPos = 0;
    m_Price = 0;
    m_PotType = 0;
    m_Pot = 0;
    m_AatMin = 0;
    m_AatMax = 0;
    m_DatMin = 0;
    m_DatMax = 0;
    m_Dis = 0;
    m_AMat = 0;
    m_DMat = 0;
    m_Ac = 0;
    m_Amr = 0;
    m_Dmr = 0;
    m_Hr = 0;
    m_Avo = 0;
    m_SpecialMac = 0;
    m_Contime = 0;
    m_Mac = 0;
    m_Wgt = 0;
    m_nFields = 27;
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CBaseItemTableSet::GetDefaultConnect() {
    return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CBaseItemTableSet::GetDefaultSQL() {
    return _T("[dbo].[BASE_ITEM]");
}

void CBaseItemTableSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CBaseItemTableSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("[Num]"), m_Num);
    RFX_Text(pFX, _T("[strCode]"), m_strCode);
    RFX_Text(pFX, _T("[strName]"), m_strName);
    RFX_Byte(pFX, _T("[Group]"), m_Group);
    RFX_Byte(pFX, _T("[Type]"), m_Type);
    RFX_Byte(pFX, _T("[Gen]"), m_Gen);
    RFX_Byte(pFX, _T("[Class]"), m_Class);
    RFX_Byte(pFX, _T("[ItemPos]"), m_ItemPos);
    RFX_Long(pFX, _T("[Price]"), m_Price);
    RFX_Byte(pFX, _T("[PotType]"), m_PotType);
    RFX_Long(pFX, _T("[Pot]"), m_Pot);
    RFX_Byte(pFX, _T("[AatMin]"), m_AatMin);
    RFX_Byte(pFX, _T("[AatMax]"), m_AatMax);
    RFX_Byte(pFX, _T("[DatMin]"), m_DatMin);
    RFX_Byte(pFX, _T("[DatMax]"), m_DatMax);
    RFX_Byte(pFX, _T("[Dis]"), m_Dis);
    RFX_Byte(pFX, _T("[AMat]"), m_AMat);
    RFX_Byte(pFX, _T("[DMat]"), m_DMat);
    RFX_Byte(pFX, _T("[Ac]"), m_Ac);
    RFX_Byte(pFX, _T("[Amr]"), m_Amr);
    RFX_Byte(pFX, _T("[Dmr]"), m_Dmr);
    RFX_Int(pFX, _T("[Hr]"), m_Hr);
    RFX_Int(pFX, _T("[Avo]"), m_Avo);
    RFX_Long(pFX, _T("[SpecialMac]"), m_SpecialMac);
    RFX_Int(pFX, _T("[Contime]"), m_Contime);
    RFX_Int(pFX, _T("[Mac]"), m_Mac);
    RFX_Int(pFX, _T("[Wgt]"), m_Wgt);
    //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CBaseItemTableSet diagnostics

#ifdef _DEBUG
void CBaseItemTableSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CBaseItemTableSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif //_DEBUG
