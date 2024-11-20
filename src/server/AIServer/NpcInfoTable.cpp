// NpcInfoTable.cpp: implementation of the CNpcInfoTable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AIServer.h"
#include "NpcInfoTable.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcInfoTable::CNpcInfoTable() {}

CNpcInfoTable::~CNpcInfoTable() {}

void CNpcInfoTable::Initialize() {
    CIni ini("mon1.ini");
    m_sSid = ini.GetInt("MAIN", "sSid", 0);
    memset(m_strName, 0, sizeof(m_strName));
    ini.GetString("MAIN", "strName", "monster").copy(m_strName, sizeof(m_strName) - 1);
    m_sMaxHP = ini.GetInt("MAIN", "최대HP", 0);
    m_sMaxMP = ini.GetInt("MAIN", "최대PP", 0);
    m_byClass = ini.GetInt("MAIN", "무기계열", 0);
    m_iExp = ini.GetInt("MAIN", "경험치", 0);
    m_sDefense = ini.GetInt("MAIN", "방어값", 0);
    m_byAttackRange = ini.GetInt("MAIN", "사정거리", 0);
    m_sAI = ini.GetInt("MAIN", "인공지능인덱스", 0);
    m_sAttackDelay = ini.GetInt("MAIN", "공격딜레이", 0);
    m_bySearchRange = ini.GetInt("MAIN", "적탐지범위", 0);
    m_sSpeed = ini.GetInt("MAIN", "이동속도", 0);
    m_sStandTime = ini.GetInt("MAIN", "StandTime", 0);
    m_tNpcType = ini.GetInt("MAIN", "NpcType", 0);
    m_sFamilyType = ini.GetInt("MAIN", "FamilyType", 0);
    m_tItemPer = ini.GetInt("MAIN", "ItemPer", 0);
    m_tDnPer = ini.GetInt("MAIN", "DnPer", 0);
}
