// NpcPosTable.cpp: implementation of the CNpcPosTable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AIServer.h"
#include "NpcPosTable.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcPosTable::CNpcPosTable() {}

CNpcPosTable::~CNpcPosTable() {}

void CNpcPosTable::Initialize() {
    CIni ini("mon_pos1.ini");
    m_sSid = ini.GetInt("MAIN", "sSid", 0);
    memset(m_strName, 0, sizeof(m_strName));
    ini.GetString("MAIN", "strName", "monster").copy(m_strName, sizeof(m_strName) - 1);
    m_byType = ini.GetInt("MAIN", "type", 0);
    m_sZone = ini.GetInt("MAIN", "zone", 0);
    m_sX = ini.GetInt("MAIN", "xpos", 0);
    m_sY = ini.GetInt("MAIN", "ypos", 0);
    m_sZ = ini.GetInt("MAIN", "zpos", 0);
    m_sMinX = ini.GetInt("MAIN", "minx", 0);
    m_sMinY = ini.GetInt("MAIN", "miny", 0);
    m_sMaxX = ini.GetInt("MAIN", "maxx", 0);
    m_sMaxY = ini.GetInt("MAIN", "maxy", 0);
    m_sArea = ini.GetInt("MAIN", "area", 0);
    m_sRegenTime = ini.GetInt("MAIN", "regentime", 2000);
    m_sCount = ini.GetInt("MAIN", "count", 0);
}
