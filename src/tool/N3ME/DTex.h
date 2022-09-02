// DTex.h: interface for the CDTexAttr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CN3Texture;
#include "LyTerrainDef.h"

class CDTex {
  private:
  public:
    DTEXATTR     m_Attr[NUM_DTEXTILE][NUM_DTEXTILE];
    CN3Texture * m_pTex;
    int          m_ID;

  public:
    void Init();
    void Release();
    void SetAttr(int x, int y, DTEXATTR attr);

    CDTex();
    virtual ~CDTex();
};
