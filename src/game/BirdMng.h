// BirdMng.h: interface for the CBirdMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"

class CBird;

class CBirdMng : public CN3Base {
  public:
    CBirdMng();
    virtual ~CBirdMng();

    // Attributes
  protected:
    CBird * m_pBird;
    int     m_iBirdCount;

    // Operations
  public:
    virtual void Release();
    void         Tick();
    void         Render();
    void         LoadFromFile(const fs::path & fsFile);

  protected:
};
