// ItemRepairMgr.h: interface for the CItemRepairMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameBase.h"

class CItemRepairMgr : CGameBase {
    struct __IconItemSkill * m_pspItemBack;
    int                      m_iArm;
    int                      m_iiOrder;

  public:
    CItemRepairMgr();
    virtual ~CItemRepairMgr();

    void Tick();
    void ReceiveResultFromServer(int iResult, int iUserGold);

    void UpdateUserTotalGold(int iGold);
    int  CalcRepairGold(struct __IconItemSkill * spItem);
};
