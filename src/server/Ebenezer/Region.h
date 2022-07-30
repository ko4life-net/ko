// Region.h: interface for the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "define.h"
#include "GameDefine.h"
#include "STLMap.h"

typedef CSTLMap <_ZONE_ITEM>	ZoneItemArray;
typedef CSTLMap <int>			ZoneUserArray;
typedef CSTLMap <int>			ZoneNpcArray;

class CRegion  
{
public:
	CRegion();
	virtual ~CRegion();

	ZoneItemArray	m_RegionItemArray;
	ZoneUserArray	m_RegionUserArray;
	ZoneNpcArray	m_RegionNpcArray;

};

