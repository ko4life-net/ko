#pragma once

#pragma warning(disable : 4786)

#include <string>
#include "PvsBase.h"

class CPortalVol;

class CPvsObjFactory {
    HICON       GetPvsWallIcon();
    std::string MakePvsWallString(int iIndex, e_WallType eWT);
    HICON       GetPvsVolIcon();
    std::string MakePvsVolString(int iIndex);

  public:
    CPvsObjFactory();
    virtual ~CPvsObjFactory();

    CPvsBase * CreatePvsWall(int iIndex, e_WallType eWT, CPortalVol * pVol = NULL);
    CPvsBase * CreatePvsVol(int iIndex);
    void       DestroyPvsObj(CPvsBase * pBase);
};
