// Region.h: interface for the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "STLMap.h"

class CUser;
class CNpc;

typedef CSTLMap<int> ZoneUserArray;
typedef CSTLMap<int> ZoneNpcArray;

//typedef CTypedPtrArray <CPtrArray, int*>    ZoneUserArray;
//typedef CTypedPtrArray <CPtrArray, int*>    ZoneNpcArray;

class CRegion {
  public:
    ZoneUserArray m_RegionUserArray;
    ZoneNpcArray  m_RegionNpcArray;
    BYTE          m_byMoving; // move : 1, not moving : 0

  protected:
    int m_nIndex;

  public:
    CRegion();
    virtual ~CRegion();
};
