// PlayerMgr.h: interface for the CPlayerMySelf class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include "PlayerBase.h"

//////////////////////////////////////////////////////////////////////
class CPlayerMySelf : public CPlayerBase {
  protected:
    bool  m_bRunning;          // 뛰는지..
    float m_fAttackTimeRecent; // 최근에 공격한 시간..

    __Vector3 m_vCollisionOffsets[3]; // 허리 부분 2개의 충돌 체크 + 머리 부분 1개의 충돌 체크..

  public:
    __InfoPlayerMySelf m_InfoExt;
    CN3Chr             m_ChrInv;           // 캐릭터 기본 객체...
    bool               m_bMoveContinous;   // 계속 움직이는지..
    bool               m_bAttackContinous; // 계속 공격하는 상태인지..
    bool               m_bSitDown;         // 앉아있는 상태인지....
    bool               m_bRecruitParty;    // 파티 모집중??

    bool  m_bStun;     // 기절..
    float m_fStunTime; // 기절한 시간..

    int m_iSendRegeneration; // 한번 보내면 다시 죽을때까지 안보내는 플래그 0 일때는 메시지 박스를 띄운다..
    bool m_bTempMoveTurbo; // 개발을 위해 임시로 넣은 코드.. 무지 빠르게 움직이게 한다.. Tick 참조.

    DWORD m_dwMagicID;    // 쓰고 있는 마법..
    float m_fCastingTime; // 마법 주문 거는 시간..

    class CN3Shape * m_pObjectTarget; // 타겟 오브젝트 포인터..

    bool      m_bTargetOrPosMove; // 타겟이나 지점 이동 여부
    int       m_iMoveTarget;      // 타겟 아이디
    __Vector3 m_vTargetPos;       // 이동할 지점 위치
    void      SetMoveTargetID(int iID);
    void      SetMoveTargetPos(__Vector3 vPos);

  public:
    void TargetOrPosMove();
    void Stun(float fTime); // 일정한 시간동안 기절 시키기.
    void StunRelease();     // 기절 풀기..

    float
    MoveSpeedCalculationAndCheckCollision(); // 속도를 구하고 그 속도로 충돌 체크를 한다. 리턴값이 0 이면 충돌이다..

    void InitFace(); // 갖고 있는 정보로 얼굴을 초기화 한다..
    void InitHair(); // 갖고 있는 정보로 머리카락을 초기화 한다..
    void KnightsInfoSet(int iID, const std::string & szName, int iGrade, int iRank);
    void SetSoundAndInitFont();

    float AttackableDistance(CPlayerBase * pTarget);
    float DistanceExceptRadius(CPlayerBase * pTarget);
    bool  IsAttackableTarget(CPlayerBase * pTarget, bool bMesureAngle = true); // 공격 가능한 범위에 있는지..
    bool  IsRunning() { return m_bRunning; }
    bool CheckCollision(); // 움직이는 처리와 충돌체크를 한다. 충돌되는게 있으면 움직이지 않는다.
    //..
    bool           InitChr(__TABLE_PLAYER_LOOKS * pTblUPC);
    CN3CPart *     PartSet(e_PartPosition ePos, const fs::path & fsFile, __TABLE_ITEM_BASIC * pItemBasic,
                           __TABLE_ITEM_EXT * pItemExt);
    CN3CPlugBase * PlugSet(e_PlugPosition ePos, const fs::path & fsFile, __TABLE_ITEM_BASIC * pItemBasic,
                           __TABLE_ITEM_EXT * pItemExt);

    void Tick();
    void Render(float fSunAngle);

    __Vector3 NextPos(float fTimeAfter);
    void      RotAdd(const float fRotRadianPerSec); // y 축을 기준으로 초당 회전하는 속도를 넣어준다.

    bool ToggleAttackContinous();
    bool ToggleRunMode();
    void ToggleMoveMode();

    // Inventory..
    void InventoryChrRender(const RECT & Rect);
    void ChrCreateChrRender(const RECT & Rect);
    void InventoryChrTick();
    void InventoryChrAnimationInitialize();

    void Release();
    CPlayerMySelf();
    virtual ~CPlayerMySelf();
};
