// BirdMng.h: interface for the CBirdMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"

typedef struct __TABLE_BIRD {
    DWORD  dwID;         // zone ID
    char * pszTerrainFN; // Terrain FileName - Terrain, Tile Map, ColorMap....
    char * pszColorMapFN;
    char * pszObjectPostDataFN; // Object 위치 정보 - 이안에 충돌 체크 데이터도 들어 있다.
} TABLE_BIRD;

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
