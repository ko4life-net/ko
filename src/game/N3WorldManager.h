// N3WorldManager.h: interface for the CN3WorldManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameBase.h"
#include "N3WorldBase.h"

class CN3Shape;
class CN3Sun;

class CN3WorldManager : public CGameBase {
    bool           m_bIndoor;
    CN3WorldBase * m_pActiveWorld;

  public:
    void InitWorld(int iZoneID, const __Vector3 & vPosPlayer);

    CN3WorldManager();
    virtual ~CN3WorldManager();

    void Tick();

    bool IsIndoor() { return m_bIndoor; }

    //..
    CN3WorldBase * GetActiveWorld() {
        //        __ASSERT(0, "No Acitive World!!");
        return m_pActiveWorld;
    }
};
