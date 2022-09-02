// DTexGroup.h: interface for the CDTexGroup class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LyTerrainDef.h"
#include <list>

typedef typename std::list<LPDTEXTILEATTR>::iterator it_DTexTileAttr;

class CDTexGroup {
  public:
    char m_Name[40];
    int  m_ID;

    std::list<LPDTEXTILEATTR> m_Attributes[DTEX_MAX];

  public:
    void Init();
    void Release();
    void DelAttrByDTexID(int DTexIndx);
    void SetAttr(int attr, __DTexTileAttr tile);
    void DelAttr(int attr, __DTexTileAttr tile);
    void ClearDTex();

    CDTexGroup();
    virtual ~CDTexGroup();
};
