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
    void ReceiveResultFromServer(int iResult, int64_t iUserGold);

    void UpdateUserTotalGold(int64_t iGold);
    int  CalcRepairGold(struct __IconItemSkill * spItem);
};
