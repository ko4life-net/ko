// SkillTableSet.cpp : implementation file
//

#include "stdafx.h"
#include "ebenezer.h"
#include "SkillTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillTableSet

IMPLEMENT_DYNAMIC(CSkillTableSet, CRecordset)

CSkillTableSet::CSkillTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSkillTableSet)
	m_Num = 0;
	m_Group = 0;
	m_Lv = 0;
	m_Pic = 0;
	m_strName = _T("");
	m_Hp = 0;
	m_Mp = 0;
	m_HpMax = 0;
	m_MpMax = 0;
	m_ResHp = 0;
	m_Soul = 0;
	m_Str = 0;
	m_Agi = 0;
	m_Dex = 0;
	m_Hr = 0;
	m_Avo = 0;
	m_totAddMin = 0;
	m_totAddMax = 0;
	m_totSubAddMin = 0;
	m_totSubAddMax = 0;
	m_curMaxPoint = 0;
	m_ReqLv = 0;
	m_curAddMin = 0;
	m_curAddMax = 0;
	m_curSubMaxPoint = 0;
	m_curSubAddMin = 0;
	m_curSubAddMax = 0;
	m_Success = 0;
	m_Type1 = 0;
	m_Type2 = 0;
	m_Type3 = 0;
	m_Type4 = 0;
	m_Type5 = 0;
	m_Dist = 0;
	m_DmgMin1 = 0;
	m_DmgMax1 = 0;
	m_DmgMin2 = 0;
	m_DmgMax2 = 0;
	m_Mvs = 0;
	m_Ats = 0;
	m_Contime = 0;
	m_Interval = 0;
	m_NeedStuff1 = 0;
	m_Count1 = 0;
	m_NeedStuff2 = 0;
	m_Count2 = 0;
	m_NeedStuff3 = 0;
	m_Count3 = 0;
	m_NeedStuff4 = 0;
	m_Count4 = 0;
	m_NickName = 0;
	m_Explain = _T("");
	m_nFields = 52;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSkillTableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CSkillTableSet::GetDefaultSQL()
{
	return _T("[dbo].[SKILL]");
}

void CSkillTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSkillTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Byte(pFX, _T("[Group]"), m_Group);
	RFX_Byte(pFX, _T("[Lv]"), m_Lv);
	RFX_Int(pFX, _T("[Pic]"), m_Pic);
	RFX_Text(pFX, _T("[strName]"), m_strName);
	RFX_Int(pFX, _T("[Hp]"), m_Hp);
	RFX_Int(pFX, _T("[Mp]"), m_Mp);
	RFX_Int(pFX, _T("[HpMax]"), m_HpMax);
	RFX_Int(pFX, _T("[MpMax]"), m_MpMax);
	RFX_Byte(pFX, _T("[ResHp]"), m_ResHp);
	RFX_Byte(pFX, _T("[Soul]"), m_Soul);
	RFX_Byte(pFX, _T("[Str]"), m_Str);
	RFX_Byte(pFX, _T("[Agi]"), m_Agi);
	RFX_Byte(pFX, _T("[Dex]"), m_Dex);
	RFX_Int(pFX, _T("[Hr]"), m_Hr);
	RFX_Int(pFX, _T("[Avo]"), m_Avo);
	RFX_Byte(pFX, _T("[totAddMin]"), m_totAddMin);
	RFX_Byte(pFX, _T("[totAddMax]"), m_totAddMax);
	RFX_Byte(pFX, _T("[totSubAddMin]"), m_totSubAddMin);
	RFX_Byte(pFX, _T("[totSubAddMax]"), m_totSubAddMax);
	RFX_Long(pFX, _T("[curMaxPoint]"), m_curMaxPoint);
	RFX_Byte(pFX, _T("[ReqLv]"), m_ReqLv);
	RFX_Byte(pFX, _T("[curAddMin]"), m_curAddMin);
	RFX_Byte(pFX, _T("[curAddMax]"), m_curAddMax);
	RFX_Long(pFX, _T("[curSubMaxPoint]"), m_curSubMaxPoint);
	RFX_Int(pFX, _T("[curSubAddMin]"), m_curSubAddMin);
	RFX_Int(pFX, _T("[curSubAddMax]"), m_curSubAddMax);
	RFX_Byte(pFX, _T("[Success]"), m_Success);
	RFX_Byte(pFX, _T("[Type1]"), m_Type1);
	RFX_Byte(pFX, _T("[Type2]"), m_Type2);
	RFX_Byte(pFX, _T("[Type3]"), m_Type3);
	RFX_Byte(pFX, _T("[Type4]"), m_Type4);
	RFX_Byte(pFX, _T("[Type5]"), m_Type5);
	RFX_Byte(pFX, _T("[Dist]"), m_Dist);
	RFX_Int(pFX, _T("[DmgMin1]"), m_DmgMin1);
	RFX_Int(pFX, _T("[DmgMax1]"), m_DmgMax1);
	RFX_Int(pFX, _T("[DmgMin2]"), m_DmgMin2);
	RFX_Int(pFX, _T("[DmgMax2]"), m_DmgMax2);
	RFX_Int(pFX, _T("[Mvs]"), m_Mvs);
	RFX_Int(pFX, _T("[Ats]"), m_Ats);
	RFX_Int(pFX, _T("[Contime]"), m_Contime);
	RFX_Int(pFX, _T("[Interval]"), m_Interval);
	RFX_Long(pFX, _T("[NeedStuff1]"), m_NeedStuff1);
	RFX_Byte(pFX, _T("[Count1]"), m_Count1);
	RFX_Long(pFX, _T("[NeedStuff2]"), m_NeedStuff2);
	RFX_Byte(pFX, _T("[Count2]"), m_Count2);
	RFX_Long(pFX, _T("[NeedStuff3]"), m_NeedStuff3);
	RFX_Byte(pFX, _T("[Count3]"), m_Count3);
	RFX_Long(pFX, _T("[NeedStuff4]"), m_NeedStuff4);
	RFX_Byte(pFX, _T("[Count4]"), m_Count4);
	RFX_Byte(pFX, _T("[NickName]"), m_NickName);
	RFX_Text(pFX, _T("[Explain]"), m_Explain);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSkillTableSet diagnostics

#ifdef _DEBUG
void CSkillTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSkillTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
