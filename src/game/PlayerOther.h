// PlayerOther.h: interface for the CPlayerOther class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameBase.h"
#include "PlayerNPC.h"

class CPlayerOther : public CPlayerNPC {
    friend class CPlayerOtherMgr;

  public:
    __InfoPlayerOther m_InfoExt; // 캐릭터 정보 확장..
    bool              m_bSit;

  public:
    void InitFace();
    void InitHair();
    void KnightsInfoSet(int iID, const std::string & szName, int iGrade, int iRank);
    void SetSoundAndInitFont();

    bool Init(enum e_Race eRace, int iFace, int iHair, DWORD * pdwItemIDs, int * piItenDurabilities);
    void Tick();

    CPlayerOther();
    virtual ~CPlayerOther();
};
