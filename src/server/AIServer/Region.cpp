// Region.cpp: implementation of the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AIServer.h"
#include "Region.h"
#include "Npc.h"
#include "User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegion::CRegion() {}

CRegion::~CRegion() {
    if (!m_RegionUserArray.IsEmpty()) {
        m_RegionUserArray.DeleteAllData();
    }

    if (!m_RegionNpcArray.IsEmpty()) {
        m_RegionNpcArray.DeleteAllData();
    }
}
