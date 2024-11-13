// Ground.h: interface for the CN3FXPartParticles class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"

class CN3Texture;

class CGround : public CN3Base {
  public:
    int          m_iSize;     //한변의 길이..
    int          m_iTileSize; //타일의 길이..
    __VertexT1 * m_pVB;
    CN3Texture * m_pTile;

  public:
    void SetTex(const fs::path & fsFile);
    void Render();

    CGround();
    virtual ~CGround();
};
