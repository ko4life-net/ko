// CharacterDataSet.cpp : implementation file
//

#include "stdafx.h"
#include "Ebenezer.h"
#include "CharacterDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterDataSet

IMPLEMENT_DYNAMIC(CCharacterDataSet, CRecordset)

CCharacterDataSet::CCharacterDataSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CCharacterDataSet)
	m_Num = 0;
	m_Class = _T("");
	m_HP = 0.0;
	m_MP = 0.0;
	m_SP = 0.0;
	m_Hit = 0.0;
	m_AC1 = 0.0;
	m_AC2 = 0.0;
	m_AGI1 = 0.0;
	m_AGI2 = 0.0;
	m_RMP = 0;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CCharacterDataSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=KN_Online;UID=knight;PWD=knight");
}

CString CCharacterDataSet::GetDefaultSQL()
{
	return _T("[dbo].[CHARACTER_VAL]");
}

void CCharacterDataSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CCharacterDataSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[Num]"), m_Num);
	RFX_Text(pFX, _T("[Class]"), m_Class);
	RFX_Double(pFX, _T("[HP]"), m_HP);
	RFX_Double(pFX, _T("[MP]"), m_MP);
	RFX_Double(pFX, _T("[SP]"), m_SP);
	RFX_Double(pFX, _T("[Hit]"), m_Hit);
	RFX_Double(pFX, _T("[AC1]"), m_AC1);
	RFX_Double(pFX, _T("[AC2]"), m_AC2);
	RFX_Double(pFX, _T("[AGI1]"), m_AGI1);
	RFX_Double(pFX, _T("[AGI2]"), m_AGI2);
	RFX_Byte(pFX, _T("[RMP]"), m_RMP);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterDataSet diagnostics

#ifdef _DEBUG
void CCharacterDataSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CCharacterDataSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
