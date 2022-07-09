// ItemLevelTableSet.cpp : implementation file
//

#include "stdafx.h"
#include "ebenezer.h"
#include "ItemLevelTableSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemLevelTableSet

IMPLEMENT_DYNAMIC(CItemLevelTableSet, CRecordset)

CItemLevelTableSet::CItemLevelTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CItemLevelTableSet)
	m_Effect = 0;
	m_Swd = 0;
	m_Spe = 0;
	m_Blt = 0;
	m_Bow = 0;
	m_Thr = 0;
	m_Sta = 0;
	m_Amr = 0;
	m_Glv = 0;
	m_Boot = 0;
	m_Shld = 0;
	m_Hlm = 0;
	m_Gaiter = 0;
	m_Wrist = 0;
	m_Str = 0;
	m_Dex = 0;
	m_Con = 0;
	m_Wis = 0;
	m_Agi = 0;
	m_Hpmax = 0;
	m_Mpmax = 0;
	m_Price = 0;
	m_nFields = 22;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CItemLevelTableSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=Repent;UID=Repent;PWD=Repent");
}

CString CItemLevelTableSet::GetDefaultSQL()
{
	return _T("[dbo].[ITEM_LEVEL]");
}

void CItemLevelTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CItemLevelTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Effect]"), m_Effect);
	RFX_Int(pFX, _T("[Swd]"), m_Swd);
	RFX_Int(pFX, _T("[Spe]"), m_Spe);
	RFX_Int(pFX, _T("[Blt]"), m_Blt);
	RFX_Int(pFX, _T("[Bow]"), m_Bow);
	RFX_Int(pFX, _T("[Thr]"), m_Thr);
	RFX_Int(pFX, _T("[Sta]"), m_Sta);
	RFX_Int(pFX, _T("[Amr]"), m_Amr);
	RFX_Int(pFX, _T("[Glv]"), m_Glv);
	RFX_Int(pFX, _T("[Boot]"), m_Boot);
	RFX_Int(pFX, _T("[Shld]"), m_Shld);
	RFX_Int(pFX, _T("[Hlm]"), m_Hlm);
	RFX_Int(pFX, _T("[Gaiter]"), m_Gaiter);
	RFX_Int(pFX, _T("[Wrist]"), m_Wrist);
	RFX_Int(pFX, _T("[Str]"), m_Str);
	RFX_Int(pFX, _T("[Dex]"), m_Dex);
	RFX_Int(pFX, _T("[Con]"), m_Con);
	RFX_Int(pFX, _T("[Wis]"), m_Wis);
	RFX_Int(pFX, _T("[Agi]"), m_Agi);
	RFX_Long(pFX, _T("[Hpmax]"), m_Hpmax);
	RFX_Long(pFX, _T("[Mpmax]"), m_Mpmax);
	RFX_Long(pFX, _T("[Price]"), m_Price);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CItemLevelTableSet diagnostics

#ifdef _DEBUG
void CItemLevelTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CItemLevelTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
