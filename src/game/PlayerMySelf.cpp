// PlayerMgr.cpp: implementation of the CPlayerMySelf class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PlayerMySelf.h"

#include "Resource.h"
#include "PacketDef.h"
#include "PlayerOtherMgr.h"

#include "N3WorldManager.h"
#include "GameProcMain.h"
#include "UIInventory.h"
#include "MagicSkillMng.h"

#include "GameEng.h"

#include "N3Base/DFont.h"
#include "N3Base/N3Camera.h"
#include "N3Base/N3ShapeMgr.h"
#include "N3Base/N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayerMySelf::CPlayerMySelf() {
    m_ePlayerType = PLAYER_MYSELF; // Player Type ... Base, NPC, OTher, MySelf

    m_bRunning = false;         // �ٴ���..
    m_bMoveContinous = false;   // ��� �����̴���..
    m_bAttackContinous = false; // ��� �����ϴ� ��������..
    m_bSitDown = false;         // �ɾ��ִ� ��������....
    m_bRecruitParty = false;    // ��Ƽ������??

    m_bStun = false;    // ����..
    m_fStunTime = 0.0f; // ������ �ð�..

    m_fAttackTimeRecent = CN3Base::TimeGet(); // �ֱٿ� ������ �ð�..
    m_bTempMoveTurbo = false;

    m_InfoExt.Init();

    // Inventory..
    m_ChrInv.PartAlloc(PART_POS_COUNT);
    m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

    m_iSendRegeneration = 0; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���

    m_dwMagicID = 0xffffffff;
    m_fCastingTime = 0.0f;
    m_pObjectTarget = NULL; // Ÿ�� ������Ʈ ������..
}

CPlayerMySelf::~CPlayerMySelf() {}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CPlayerMySelf::Release() {
    m_bTargetOrPosMove = false;
    m_iMoveTarget = -1;

    m_bRunning = false;         // �ٴ���..
    m_bMoveContinous = false;   // ��� �����̴���..
    m_bAttackContinous = false; // ��� �����ϴ� ��������..
    m_bSitDown = false;         // �ɾ��ִ� ��������....
    m_bRecruitParty = false;    // ��Ƽ������??

    m_bStun = false;    // ����..
    m_fStunTime = 0.0f; // ������ �ð�..

    m_fAttackTimeRecent = CN3Base::TimeGet(); // �ֱٿ� ������ �ð�..
    m_bTempMoveTurbo = false;

    m_InfoExt.Init();

    // Inventory..
    m_ChrInv.Release();
    m_ChrInv.PartAlloc(PART_POS_COUNT);
    m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

    m_pObjectTarget = NULL; // Ÿ�� ������Ʈ ������..

    CPlayerBase::Release();
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CPlayerMySelf::SetMoveTargetID(int iID) {
    m_bTargetOrPosMove = true;
    m_iMoveTarget = iID;
}

void CPlayerMySelf::SetMoveTargetPos(__Vector3 vPos) {
    m_vTargetPos = vPos;
    m_bTargetOrPosMove = true;
    m_iMoveTarget = -1;
}

void CPlayerMySelf::Tick() {
    BOOL bAnim = TRUE;

    if (PSA_DEATH ==
        m_eState) // �״� �����̰�... �״� ���ϸ��̼��� ������.. // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    {
        if (0 == m_iSendRegeneration) {
            //            std::string szMsg; ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
            //            CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // �ٽ� ���� �޽��� ����..
            //            CLogWriter::Write("Dead!!!");
            m_iSendRegeneration = 1;
        }
        return;
    }

    if (m_fTimeAfterDeath > 0) // ������ �׿��� �Ѵ�..
    {
        if (m_fTimeAfterDeath > 3.0f) {
            this->Action(PSA_DYING, false); // 3 �ʰ� ������ �״´�.
        }
        CPlayerBase::Tick(); // ȸ��, ������ ���ϸ��̼� Tick �� ���� ���� ó��.. ���..
        return;
    }

    if (IsDead()) // ���� ���¸� ���ư���.
    {
        CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL);
        CPlayerBase::Tick(); // ȸ��, ������ ���ϸ��̼� Tick �� ���� ���� ó��.. ���..
        return;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // �� �÷��̾�� �����̴°� �ٸ��ѵ�� �ٸ��� ������ Ư���ϰ� ó��..
    if (PSM_WALK == m_eStateMove || PSM_WALK_BACKWARD == m_eStateMove ||
        PSM_RUN == m_eStateMove) // �յڷ� �ɾ�ų� �޷�����.
    {
        this->MoveSpeedCalculationAndCheckCollision(); // �����̴� �ӵ� �� �浹üũ...
        if (0 == m_fMoveSpeedPerSec)                   // �ӵ��� ������ �浹üũ ��� ������ �Ŵ�...
        {
            this->ActionMove(PSM_STOP);
            CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // ���� ��Ŷ ������..
        }

        __Vector3 vPos = this->Position();
        CPlayerBase::m_pSnd_MyMove = m_pSnd_Move;
        if (m_pSnd_Move && m_Chr.NeedPlaySound0()) {
            m_pSnd_Move->Play(&vPos);
        }
        if (m_pSnd_Move && m_Chr.NeedPlaySound1()) {
            m_pSnd_Move->Play(&vPos);
        }

        TargetOrPosMove();
    } else {
        CPlayerBase::m_pSnd_MyMove = NULL;
        m_fMoveSpeedPerSec = 0;
    }
    // �� �÷��̾�� �����̴°� �ٸ��ѵ�� �ٸ��� ������ Ư���ϰ� ó��..
    ////////////////////////////////////////////////////////////////////////////////

    CPlayerBase * pTarget = NULL;
    //    if(true == m_bAttackContinous || m_iSkillStep > 0)
    if (true == m_bAttackContinous && m_iSkillStep <= 0) {
        pTarget = TargetPointerCheck(false);
        if (NULL == pTarget) {
            CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL);
        } else {
            float fTime = CN3Base::TimeGet();

            // Ȱ���, ���� ��� �� ��ų�� ó���Ѵ�..
            if ((m_pItemPlugBasics[PLUG_POS_LEFTHAND] &&
                 ITEM_CLASS_BOW == m_pItemPlugBasics[PLUG_POS_LEFTHAND]->byClass) ||
                (m_pItemPlugBasics[PLUG_POS_LEFTHAND] &&
                 ITEM_CLASS_BOW_LONG == m_pItemPlugBasics[PLUG_POS_LEFTHAND]->byClass) ||
                (m_pItemPlugBasics[PLUG_POS_RIGHTHAND] &&
                 ITEM_CLASS_BOW_CROSS == m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->byClass)) {
                __TABLE_UPC_SKILL * pSkill = s_pTbl_Skill->Find(102003); // ��ų ���̺��� �⺻ Ȱ ��ų�� ã��
                if (pSkill) {
                    if (pTarget->IsAlive()) //�ӽ� ������ ��� �ִ� ���¿�����...
                    {
                        float fInterval = (pSkill->iCastTime / 10.f) + 0.15f;
                        if (m_fAttackDelta > 0) {
                            fInterval /= m_fAttackDelta;
                        }
                        if (fTime > m_fAttackTimeRecent + fInterval) // ���� ������ ������.. ����!
                        {
                            if (CGameProcedure::s_pProcMain->m_pMagicSkillMng->MsgSend_MagicProcess(
                                    m_iIDTarget, pSkill)) { // ��ų ��Ŷ �����⿡ �����ϸ�.
                                TRACE("%.2f\n", fTime - m_fAttackTimeRecent);
                            }
                            m_fAttackTimeRecent = fTime; // �ֱٿ� ������ �ð�..
                        }
                    }
                }
            } else if (m_pItemPlugBasics[PLUG_POS_RIGHTHAND] &&
                       ITEM_CLASS_LAUNCHER == m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->byClass) // ��â�� �������̸�..
            {
                __TABLE_UPC_SKILL * pSkill = s_pTbl_Skill->Find(102009); // ��ų ���̺��� �⺻ Ȱ ��ų�� ã��
                if (pSkill && fTime > m_fAttackTimeRecent + (pSkill->iCastTime / 10.f)) // ���� ������ ������.. ����!
                {
                    if (pTarget->IsAlive()) //�ӽ� ������ ��� �ִ� ���¿�����
                    {
                        CGameProcedure::s_pProcMain->m_pMagicSkillMng->MsgSend_MagicProcess(
                            m_iIDTarget, pSkill); // ��ų ��Ŷ �����⿡ �����ϸ�.
                        TRACE("%.2f\n", fTime - m_fAttackTimeRecent);
                        m_fAttackTimeRecent = fTime; // �ֱٿ� ������ �ð�..
                    }
                }
            } else // �� �����̸�..
            {
                float fIntervalTable = 1.0f;
                if (m_pItemPlugBasics[PLUG_POS_RIGHTHAND] && m_pItemPlugExts[PLUG_POS_RIGHTHAND]) // ���� ���� ����
                {
                    fIntervalTable = (m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->siAttackInterval / 100.0f) *
                                     (m_pItemPlugExts[PLUG_POS_RIGHTHAND]->siAttackIntervalPercentage /
                                      100.0f); // 100 �� 1�� 110 �̸� 1.1 �ʵ�.. �ۼ�Ʈ�� ����...
                }

                float fInterval = fIntervalTable;
                if (m_fAttackDelta > 0) {
                    fInterval /=
                        m_fAttackDelta; // ��ų�̳� ������ ���� ���ϴ� ���� �ӵ�.. 1.0 �� �⺻�̰� Ŭ���� �� ���� �����Ѵ�.
                }

                if (fTime > m_fAttackTimeRecent + fInterval) // ���� ������ ������.. ����!
                {
                    bool bCastingNow = CGameProcedure::s_pProcMain->m_pMagicSkillMng->IsCasting();
                    if (false == bCastingNow) // ĳ���� ���̸� ���� ��Ŷ�� ������ �ʴ´�.
                    {
                        bool bAttackable = IsAttackableTarget(pTarget);
                        if (bAttackable) // ���� ����..
                        {
                            float fDistance = s_pPlayer->DistanceExceptRadius(pTarget); // ���ݰŸ�

                            CGameProcedure::s_pProcMain->MsgSend_Attack(pTarget->IDNumber(), fIntervalTable, fDistance);
                            if (m_iSkillStep == 0 && PSA_ATTACK != m_eState &&
                                m_fFlickeringFactor == 1.0f) {           // ��ų�� ���°� �ƴѵ� �������� ������..
                                this->Action(PSA_ATTACK, true, pTarget); // ���� ���̾ƴϸ� �����Ѵ�..
                            }

                            m_fAttackTimeRecent = fTime; // �ֱٿ� ������ �ð�..
                        } else {
                            if (CGameProcedure::s_pEng->ViewPoint() == VP_THIRD_PERSON) {
                                CGameProcedure::s_pPlayer->RotateTo(pTarget); // ������ ������.

                                float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // ���� �Ÿ��� ���ϰ�..
                                float fDistLimit = this->AttackableDistance(pTarget);

                                if (fDist > fDistLimit && !m_bTargetOrPosMove) {
                                    CGameProcedure::s_pProcMain->CommandMove(MD_FOWARD, true);
                                    s_pPlayer->SetMoveTargetID(pTarget->IDNumber());
                                }
                            }
                            if (m_iSkillStep == 0 &&
                                PSA_SITDOWN != m_eState) {     // ��ų�� ���°� �ƴѵ� �ɾ��ִ� ���°� �ƴϸ�..
                                this->Action(PSA_BASIC, true); // �⺻�ڼ�..
                            }
                        }
                    }
                }
            }

            if (fTime == m_fAttackTimeRecent) { // ����������..
                CGameProcedure::s_pProcMain->CommandSitDown(false,
                                                            false); // ������ �����.. �ɾ��ִ� ���¿��� ���װ� �ִ�..
            }
        }
    }

    if (m_bStun) // ������ ������..
    {
        m_fStunTime -= s_fSecPerFrm; // ���� �ð� ó��..
        if (m_fStunTime < 0) {
            this->StunRelease(); // ���� Ǯ���ֱ�..
        }
    }

    if (PSA_ATTACK == m_eState || m_iSkillStep != 0) // ���� ���̰ų� ��ų ������̸�..
    {
        if (!pTarget) {
            pTarget = TargetPointerCheck(false); // Ÿ�� �����͸� ���´�..
        }
        CPlayerBase::ProcessAttack(
            pTarget); // ���ݿ� ���� ��ƾ ó��.. ���ϸ��̼� ���ð� �浹�� ó���һ� ��Ŷ�� ó�� ���Ѵ�..
    }

    if (m_dwMagicID != 0xffffffff) {
        m_fCastingTime += CN3Base::s_fSecPerFrm;
    }

    CPlayerBase::Tick(); // ȸ��, ������ ���ϸ��̼� Tick �� ���� ���� ó��.. ���..
}

void CPlayerMySelf::Render(float fSunAngle) {
    // ������ LOD ���� + 1
    m_Chr.m_nLOD--;
    if (m_Chr.m_nLOD < 0) {
        m_Chr.m_nLOD = 0;
    }

    int iLODDeltaPrev = CN3Chr::LODDelta(); // ��ĳ���� ���� �����ϰ� �׸���..
    CN3Chr::LODDeltaSet(0);
    CPlayerBase::Render(fSunAngle);
    CN3Chr::LODDeltaSet(iLODDeltaPrev);
}

bool CPlayerMySelf::ToggleAttackContinous() {
    if (!IsAlive()) {
        return false;
    }

    if (false == m_bAttackContinous) // Ÿ���� �ִ��� ����..
    {
        CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(m_iIDTarget, true);
        if (pTarget) //  && !IsOutOfAttackRange(pTarget)) // Ÿ���� �ְ� ���� ������ ������ ��������
        {
            this->m_bAttackContinous = true;
        }
    } else {
        m_bAttackContinous = false;
    }

    return m_bAttackContinous;
}

bool CPlayerMySelf::ToggleRunMode() {
    m_bRunning = !m_bRunning; // �ٴ���..
    if (true == m_bRunning) {
        if (PSM_WALK == m_eStateMove) {
            m_eStateMove = PSM_RUN;
            m_Chr.AniCurSet(ANI_RUN);
        }
    } else {
        if (PSM_RUN == m_eStateMove) {
            m_eStateMove = PSM_WALK;
            m_Chr.AniCurSet(ANI_WALK);
        }
    }

    return m_bRunning;
}

void CPlayerMySelf::ToggleMoveMode() {
    m_bMoveContinous = !m_bMoveContinous; // ��� �����̴���..

    int nAni = ANI_BREATH;
    if (m_bMoveContinous) // ��� �������� �ϸ�..
    {
        if (m_bRunning) // �ٸ�..
        {
            m_eStateMove = PSM_RUN;
            nAni = ANI_RUN;
        } else {
            m_eStateMove = PSM_WALK;
            nAni = ANI_WALK;
        }

        float fSpeed = this->MoveSpeedCalculationAndCheckCollision(); // �����̴� �ӵ� �� �浹üũ...
        if (0 == fSpeed)                                              // ���� ������...
        {
            m_bMoveContinous = false;
            m_eStateMove = PSM_STOP;
            m_eState = PSA_BASIC;
            nAni =
                this->JudgeAnimationBreath(); // Ÿ���� ������. ��� �ִ� ���⿡ ���� �⺻ ���ϸ��̼� �Ǵ�.. ������ �� �⺻ ���ϸ��̼�
        }
    } else {
        m_eStateMove = PSM_STOP;
        m_eState = PSA_BASIC;
        nAni =
            this->JudgeAnimationBreath(); // Ÿ���� ������. ��� �ִ� ���⿡ ���� �⺻ ���ϸ��̼� �Ǵ�.. ������ �� �⺻ ���ϸ��̼�
    }

    this->AnimationClear();
    m_Chr.AniCurSet(nAni); // ��°�� �Ȱ� �ڱ�, ���߱� ����
}

__Vector3 CPlayerMySelf::NextPos(float fTimeAfter) {
    __Matrix44 mtxRot = m_Chr.Rot();
    __Vector3  vDir(0, 0, 1);
    __Vector3  vNextPos = m_Chr.Pos() + ((vDir * mtxRot) * m_fMoveSpeedPerSec * fTimeAfter);

    return vNextPos;
}

void CPlayerMySelf::RotAdd(const float fRotRadianPerSec) // y ���� �������� �ʴ� ȸ���ϴ� �ӵ��� �־��ش�.
{
    m_fYawCur += fRotRadianPerSec * s_fSecPerFrm;

    if (m_fYawCur >= D3DXToRadian(180.0f) * 2) {
        m_fYawCur -= D3DXToRadian(180.0f) * 2;
    }
    if (m_fYawCur <= -D3DXToRadian(180.0f) * 2) {
        m_fYawCur += D3DXToRadian(180.0f) * 2;
    }
    m_fYawToReach = m_fYawCur; // �ٷ� ������..
}

void CPlayerMySelf::InventoryChrRender(const RECT & Rect) {
    /*
    DWORD dwUsefog = TRUE;
    CN3Base::s_lpD3DDev->GetRenderState( D3DRS_FOGENABLE , &dwUsefog );

    DWORD dwUseLighting=TRUE;
    CN3Base::s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwUseLighting );

    int    bLight[8];
    for ( int i = 0; i < 8; i++ )    CN3Base::s_lpD3DDev->GetLightEnable(i, &bLight[i]);

    if (dwUseLighting) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, TRUE );
    if (dwUsefog) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
    // set render states
    for ( int i = 1; i < 8; i++ )    CN3Base::s_lpD3DDev->LightEnable(i, FALSE);
    CN3Base::s_lpD3DDev->LightEnable(0, TRUE);

    D3DLIGHT9 lgt0;
    
    memset(&lgt0, 0, sizeof(D3DLIGHT9));
    lgt0.Type = D3DLIGHT_POINT;
    lgt0.Attenuation0 = 0.5f;
    lgt0.Range = 100.0f;
    lgt0.Position = __Vector3(0.0f, 2.0f, -10.0f);
    lgt0.Diffuse.r = 220/255.0f; lgt0.Diffuse.g = 255/255.0f; lgt0.Diffuse.b = 220/255.0f;
    CN3Base::s_lpD3DDev->SetLight(0, &lgt0);

    //ĳ���� ���..
    //
    __Matrix44 mtxproj, mtxview, mtxworld, mtxprojback, mtxviewback, mtxworldback;

    CN3Base::s_lpD3DDev->GetTransform( D3DTS_PROJECTION, &mtxprojback );
    CN3Base::s_lpD3DDev->GetTransform( D3DTS_VIEW, &mtxviewback );
    CN3Base::s_lpD3DDev->GetTransform( D3DTS_WORLD, &mtxworldback );

    D3DXMatrixOrthoLH(&mtxproj, 12.0f, 9.0f, 0, 100);  
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_PROJECTION, &mtxproj );

    D3DXMatrixLookAtLH( &mtxview, &D3DXVECTOR3( 0.0f, 2.0f,-10.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_VIEW, &mtxview );

    mtxworld.Identity();
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_WORLD, &mtxworld );


///////////////////////////////////////////////////////////////
// 2d ��ǥ ����..

    // 2d -> 3d..
    __Vector3 vPos;
    vPos.x = (float)((Rect.right - Rect.left)/2.0f + Rect.left);
    vPos.y = (float)(Rect.bottom) - (Rect.bottom - Rect.top)/16.0f;
    vPos.z = 0.11f;

    float fWidth, fHeight;
    fWidth = s_CameraData.vp.Width;    fHeight = s_CameraData.vp.Height;    fWidth *= 0.5f;    fHeight *= 0.5f;

    vPos.x = (vPos.x - fWidth)/fWidth;
    vPos.y = (fHeight - vPos.y)/fHeight;

    __Matrix44 mtxProjInv, mtxViewInv;
    D3DXMatrixInverse(&mtxProjInv, NULL, &mtxproj);
    D3DXMatrixInverse(&mtxViewInv, NULL, &mtxview);

    vPos *= mtxProjInv;
    vPos *= mtxViewInv;

    m_ChrInv.PosSet(vPos.x, vPos.y, 1.0f);
    __Quaternion qt;
    qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(198.0f));
    m_ChrInv.RotSet(qt);

    CGameProcedure::s_pEng->ClearZBuffer(NULL);
    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    m_ChrInv.m_nLOD = 0;
    m_ChrInv.Render();

    CN3Base::s_lpD3DDev->SetTransform( D3DTS_PROJECTION, &mtxprojback );
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_VIEW, &mtxviewback );
    CN3Base::s_lpD3DDev->SetTransform( D3DTS_WORLD, &mtxworldback );

    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00000000);

    CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwUseLighting );
    CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE , dwUsefog );
    for ( int i = 0; i < 8; i++ )    CN3Base::s_lpD3DDev->LightEnable(i, bLight[i]);
*/
    // �Ʒ��� dino����
    // backup render state
    DWORD     dwLighting;
    D3DLIGHT9 BackupLight0;

    s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    BOOL bLight[8];
    for (int i = 0; i < 8; ++i) {
        s_lpD3DDev->GetLightEnable(i, &(bLight[i]));
    }
    s_lpD3DDev->GetLight(0, &BackupLight0);

    // set render state
    if (TRUE != dwLighting) {
        s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    }
    for (int i = 1; i < 8; ++i) {
        s_lpD3DDev->LightEnable(i, FALSE);
    }
    s_lpD3DDev->LightEnable(0, TRUE);

    // 0�� light ����
    D3DLIGHT9 Light0;
    memset(&Light0, 0, sizeof(D3DLIGHT9));
    Light0.Type = D3DLIGHT_POINT;
    Light0.Attenuation0 = 0.5f;
    Light0.Range = 100.0f;
    Light0.Position = __Vector3(0.0f, 2.0f, 10.0f);
    Light0.Diffuse.r = 220 / 255.0f;
    Light0.Diffuse.g = 255 / 255.0f;
    Light0.Diffuse.b = 220 / 255.0f;
    s_lpD3DDev->SetLight(0, &Light0);

    // ĳ���� ��ġ�� ���� ����
    m_ChrInv.PosSet(__Vector3(0, 0, 0));
    __Quaternion qt;
    qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(18.0f)); // �ణ �񽺵��ϰ� ���ְ�..
    m_ChrInv.RotSet(qt);

    // render
    RenderChrInRect(&m_ChrInv, Rect);

    // restore
    if (TRUE != dwLighting) {
        s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
    }
    for (int i = 0; i < 8; ++i) {
        s_lpD3DDev->LightEnable(i, bLight[i]);
    }
    s_lpD3DDev->SetLight(0, &BackupLight0);
}

void CPlayerMySelf::InventoryChrTick() {
    // Inventory Animation Tick..
    m_ChrInv.PosSet(m_Chr.Pos());
    m_ChrInv.Tick();
}

void CPlayerMySelf::InventoryChrAnimationInitialize() {
    int iAniTmp = m_ChrInv.AniCtrl()->Count() - 1;
    m_ChrInv.AniCurSet(iAniTmp, false, 0);
    m_ChrInv.AniCurSet(ANI_BREATH, false, 0);
    m_ChrInv.Tick(); // �ѹ� ���ش�..
}

CN3CPlugBase * CPlayerMySelf::PlugSet(e_PlugPosition ePos, const std::string & szFN, __TABLE_ITEM_BASIC * pItemBasic,
                                      __TABLE_ITEM_EXT * pItemExt) {
    int iJoint = 0;
    if (PLUG_POS_RIGHTHAND == ePos) {
        m_pItemPlugBasics[ePos] = pItemBasic;
        m_pItemPlugExts[ePos] = pItemExt;
        iJoint = m_pLooksRef->iJointRH;
    } else if (PLUG_POS_LEFTHAND == ePos) {
        m_pItemPlugBasics[ePos] = pItemBasic;
        m_pItemPlugExts[ePos] = pItemExt;
        if (pItemBasic) {
            if (pItemBasic->byClass == ITEM_CLASS_SHIELD) {
                iJoint = m_pLooksRef->iJointLH2; // ������ ���
            } else {
                iJoint = m_pLooksRef->iJointLH; // �޼� ��..
            }
        }
    } else if (PLUG_POS_KNIGHTS_GRADE == ePos) {
        m_pItemPlugBasics[ePos] = pItemBasic;
        m_pItemPlugExts[ePos] = pItemExt;
        iJoint = m_pLooksRef->iJointRH - 2; // ������ �ȸ�
    } else if (PLUG_POS_BACK == ePos) {
        //m_pItemBasicPlugRefs[PLUG_POS_BACK] = pItem;
    } else {
        __ASSERT(0, "Invalid Plug Item position");
    }

    CN3CPlugBase * pPlug = m_ChrInv.PlugSet(ePos, szFN);
    if (NULL == pPlug) {
        return NULL;
    }

    if (PLUG_POS_LEFTHAND == ePos || PLUG_POS_RIGHTHAND == ePos) // Ű�� ����ؼ� ũ�� Ű���. �⺻Ű�� 1.8 �����̴�.
    {
        float fScale = m_Chr.Height() / 1.8f;
        fScale *= pPlug->Scale().y / m_Chr.Scale().y;
        pPlug->ScaleSet(__Vector3(fScale, fScale, fScale));
        pPlug->m_nJointIndex = iJoint; // ���� ��ȣ ����..
    }
    //    else if(PLUG_POS_BACK == ePos)
    //    {
    //        CN3CPlug_Cloak *pPlugCloak = (CN3CPlug_Cloak*)pPlug;
    //        pPlugCloak->GetCloak()->SetPlayerBase(this);
    //    }

    this->SetSoundPlug(pItemBasic);
    return CPlayerBase::PlugSet(ePos, szFN, pItemBasic, pItemExt);
}

CN3CPart * CPlayerMySelf::PartSet(e_PartPosition ePos, const std::string & szFN, __TABLE_ITEM_BASIC * pItemBasic,
                                  __TABLE_ITEM_EXT * pItemExt) {
    if (ePos < PART_POS_UPPER || ePos >= PART_POS_COUNT) {
        __ASSERT(0, "Invalid Item Position");
        return NULL;
    }

    if (PART_POS_UPPER == ePos) // ��ü�� ��� Ư���� ó���� �ʿ�..
    {
        if (pItemBasic) // ������ ���
        {
            if (pItemBasic->byIsRobeType &&
                m_Chr.Part(PART_POS_LOWER)) // �κ� Ÿ���� ��¥ �����̰� �Ʒ��� ���� �԰� ������..
            {
                m_ChrInv.PartSet(PART_POS_LOWER, ""); // �Ʒ��� ����ش�..
                m_Chr.PartSet(PART_POS_LOWER, "");
            }
        } else // ��ü�� ���� ���
        {
            if (m_pItemPartBasics[ePos] && m_pItemPartBasics[ePos]->byIsRobeType) // ���� �����ߴ��� �κ��..
            {
                if (m_pItemPartBasics[PART_POS_LOWER]) // ��ü�� �������� ����������..
                {
                    std::string    szFN2;
                    e_PartPosition ePartPos2 = PART_POS_UNKNOWN;
                    e_PlugPosition ePlugPos2 = PLUG_POS_UNKNOWN;
                    CGameProcedure::MakeResrcFileNameForUPC(m_pItemPartBasics[PART_POS_LOWER], &szFN2, NULL, ePartPos2,
                                                            ePlugPos2, m_InfoBase.eRace);

                    m_ChrInv.PartSet(PART_POS_LOWER, szFN2); // ��ü�� ���� ���� ������..
                    m_Chr.PartSet(PART_POS_LOWER, szFN2);
                } else // ��ü�� �԰� �־��� �������� ���ٸ�..
                {
                    __TABLE_PLAYER_LOOKS * pLooks =
                        s_pTbl_UPC_Looks->Find(m_InfoBase.eRace); // User Player Character Skin ����ü ������..
                    m_ChrInv.PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER]); // ��ü�� �⺻���� ������.
                    m_Chr.PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER]);    // ��ü�� �⺻���� ������.
                }
            }
        }
    } else if (PART_POS_LOWER == ePos) // ��ü�� ���..
    {
        // �������� �����ϰ� ���ǰ���...
        if (m_pItemPartBasics[PART_POS_UPPER] &&
            m_pItemPartBasics[PART_POS_UPPER]->byIsRobeType) // ���� ��ü�� �����ߴ��� �κ��..
        {
            m_pItemPartBasics[ePos] = pItemBasic; // ������ ����
            m_pItemPartExts[ePos] = pItemExt;
            m_ChrInv.PartSet(ePos, "");
            m_Chr.PartSet(ePos, ""); // ��ü�� �����(?)..
            return NULL;             // ���ư���.
        }
    }

    CN3CPart * pPart = NULL;
    if (szFN.empty()) // ���� �̸��� ���°Ÿ�.. �⺻ ����..
    {
        if (PART_POS_HAIR_HELMET == ePos) {
            this->InitHair();
            pPart = m_Chr.Part(ePos);
        } else if (PART_POS_FACE == ePos) {
            this->InitFace();
            pPart = m_Chr.Part(ePos);
        } else {
            __TABLE_PLAYER_LOOKS * pLooks =
                s_pTbl_UPC_Looks->Find(m_InfoBase.eRace); // Player Character Skin ����ü ������..
            if (pLooks) {
                m_ChrInv.PartSet(ePos, pLooks->szPartFNs[ePos]);
                pPart = m_Chr.PartSet(ePos, pLooks->szPartFNs[ePos]);
                if (pPart) {
                    pPart->TexOverlapSet("");
                }
            }
        }
    } else {
        m_ChrInv.PartSet(ePos, szFN);
        pPart = m_Chr.PartSet(ePos, szFN);
    }

    m_pItemPartBasics[ePos] = pItemBasic; // ������ ����
    m_pItemPartExts[ePos] = pItemExt;

    return pPart;
}

bool CPlayerMySelf::InitChr(__TABLE_PLAYER_LOOKS * pTbl) {
    CPlayerBase::InitChr(pTbl);

    m_ChrInv.JointSet(pTbl->szJointFN);
    m_ChrInv.AniCtrlSet(pTbl->szAniFN);

    float fScale = 2.1f / m_Chr.Height();
    m_ChrInv.ScaleSet(fScale, fScale, fScale); // �κ��丮 â�� ���� ũ�⸦ �ٿ��ش�..

    return true;
}

float CPlayerMySelf::AttackableDistance(CPlayerBase * pTarget) {
    if (NULL == pTarget) {
        return 0;
    }

    float fDistLimit = (m_Chr.Radius() + pTarget->Radius()) / 2.0f; // ���� �Ÿ�����..
    if (m_pItemPlugBasics[0]) {
        fDistLimit += m_pItemPlugBasics[0]->siAttackRange / 10.0f; // �������� ��� ������..
    } else if (m_pItemPlugBasics[1] &&
               ITEM_POS_TWOHANDLEFT ==
                   m_pItemPlugBasics[1]->byAttachPoint) { // �޼տ� ���ڵ巹��Ʈ(Ȱ) �������� ��� ������..
        fDistLimit += m_pItemPlugBasics[1]->siAttackRange / 10.0f;
    }

    return fDistLimit;
}

float CPlayerMySelf::DistanceExceptRadius(CPlayerBase * pTarget) {
    if (NULL == pTarget) {
        return 0;
    }

    float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude();
    float fDistRadius = (m_Chr.Radius() + pTarget->Radius()) / 2.0f; // ���� �Ÿ�����..

    return fDist - fDistRadius;
}

bool CPlayerMySelf::IsAttackableTarget(CPlayerBase * pTarget, bool bMesureAngle) {
    if (m_fFlickeringFactor != 1.0f) {
        return false; //��Ȱ�ؼ� �����̴� ���� �����Ҽ� ����.
    }
    if (NULL == pTarget || pTarget->IsDead()) {
        return false; //���� ������ ĳ���� �������� ���ϰ� - �� �ױ� ������ ĳ���� �����Ѵ�..
    }
    if (pTarget->m_InfoBase.eNation == m_InfoBase.eNation) {
        return false; // ���� �����̴�..
    }

    float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // ���� �Ÿ��� ���ϰ�..
    float fDistLimit = this->AttackableDistance(pTarget);
    if (fDist > fDistLimit) {
        return false; // �Ÿ��� �����̻� ������ ������ ���ư���.
    }

    if (bMesureAngle) // ������ ����..
    {
        __Vector3 vDir = this->Direction();
        vDir.y = 0;
        vDir.Normalize();
        __Vector3 vDirTarget = pTarget->Position() - m_Chr.Pos();
        vDirTarget.y = 0;
        vDirTarget.Normalize();
        float fDot = vDir.Dot(vDirTarget);
        if (fDot > 0.7f) {
            return true;
        } else {
            return false;
        }
    }

    return true;
}

bool CPlayerMySelf::CheckCollision() {
    float fSpeed =
        m_fMoveSpeedPerSec * CN3Base::s_fSecPerFrm; // ���� �����̴� �ӵ�..�����ӿ� ���� ���Ǿ� ������ �ӵ��̴�.
    if (0 == fSpeed) {
        return false; // �������� ������ �浹üũ ���� �ʴ´�.
    }

    __Vector3 vPos = this->Position();

    __Vector3  vDir(0, 0, 1); // ȸ������ ���ϰ�..
    __Matrix44 mtxRot = this->Rotation();
    vDir *= mtxRot; // ȸ���� ���� ������ ���ϰ�.
    if (fSpeed < 0) {
        fSpeed *= -1.0f;
        vDir *= -1.0f;
    }
    __Vector3 vPosNext = vPos + (vDir * fSpeed); // ���� ��ġ ���..
    if (false == ACT_WORLD->IsInTerrainWithTerrain(vPosNext.x, vPosNext.z, vPos)) {
        return true; // ��� �ȿ� ���� ������..
    }

    //////////////////////////////////
    // �ٸ� �÷��̾�� üũ..
    CPlayerOther * pUPC = NULL;
    float          fHeightSum, fMag;
    it_UPC         it = s_pOPMgr->m_UPCs.begin(), itEnd = s_pOPMgr->m_UPCs.end();
    for (; it != itEnd; it++) {
        pUPC = it->second;

        if (pUPC->IsDead()) {
            continue; //�׾� �ִ� ������ ĳ���ʹ� �浹üũ�� ���� �ʴ´�.
        }
        if (m_InfoBase.eNation == pUPC->m_InfoBase.eNation) {
            continue; // ���� ����...
        }

        fMag = (pUPC->Position() - vPos).Magnitude();
        if (fMag < 32.0f) {
            fHeightSum = (pUPC->Height() + m_Chr.Height()) / 2.5f;
            if (fMag < fHeightSum) // �Ÿ��� Ű�� �պ��� ������..
            {
                float fMag2 = (pUPC->Position() - vPosNext).Magnitude(); // ������ġ�� �� ������.
                if (fMag2 < fMag) {
                    return true;
                }
            }
        }
    }
    // �ٸ� �÷��̾�� üũ..
    //////////////////////////////////

    //    __TABLE_ZONE* pZoneInfo = s_pTbl_Zones->Find(m_InfoExt.iZoneCur);
    //    if(pZoneInfo && pZoneInfo->bNPCCollisionCheck) //this_zone

    //���� ���Ǿ��� �浹 üũ�� �Ѵ�.
    CPlayerNPC * pNPC = NULL;
    it_NPC       it_N = s_pOPMgr->m_NPCs.begin(), it_NEnd = s_pOPMgr->m_NPCs.end();
    for (; it_N != it_NEnd; it_N++) {
        pNPC = it_N->second;

        if (pNPC->m_pShapeExtraRef) {
            continue; // �������� �����̸� �浹üũ�� ���� �ʴ´�..
        }
        if (m_InfoBase.eNation == pNPC->m_InfoBase.eNation) {
            continue; // ���� ����...
        }
        if (m_InfoBase.eNation == NATION_KARUS && pNPC->m_InfoBase.eNation != NATION_ELMORAD) {
            continue; // ���� ���Ǿ��� �浹 üũ�� �Ѵ�.
        }
        if (m_InfoBase.eNation == NATION_ELMORAD && pNPC->m_InfoBase.eNation != NATION_KARUS) {
            continue; //
        }

        fMag = (pNPC->Position() - vPos).Magnitude();
        if (fMag < 32.0f) {
            fHeightSum = (pNPC->Height() + m_Chr.Height()) / 2.5f;
            if (fMag < fHeightSum) // �Ÿ��� Ű�� �պ��� ������..
            {
                float fMag2 = (pNPC->Position() - vPosNext).Magnitude(); // ������ġ�� �� ������.
                if (fMag2 < fMag) {
                    return true;
                }
            }
        }
    } //for(

    // ������Ʈ�� �浹üũ..
    __Vector3 vPos2 = vPos, vCol, vNormal;
    if (!s_pWorldMgr->IsIndoor()) {
        vPos2.y += 0.5f; // ĳ���� �߳��̿��� 0.5 ���� ���� ������ �浹üũ�Ѵ�.
    } else {
        vPos2.y +=
            0.6f; // ĳ���� �߳��̿��� 0.6 ���� ���� ������ �浹üũ�Ѵ�.    �� �Լ� ������ ���� 0.6�� PvsMgr�� m_fVolumeOffs.. ^^
    }
    bool bColShape = ACT_WORLD->CheckCollisionWithShape(vPos2, vDir, fSpeed, &vCol, &vNormal);
    if (bColShape) {
        return true; // ������Ʈ�� �浹���� ������
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ����� ������Ʈ�� ���̰� ���ϱ�..
    float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPosNext.x, vPosNext.z); // ������ ���̰�..
    float fYClimb = ACT_WORLD->GetHeightNearstPosWithShape(vPosNext, CN3Base::s_fSecPerFrm * 30.0f,
                                                           &vNormal); // �浹 üũ ������Ʈ�� ���̰�..
    vNormal.y = 0;                                                    // �̷��� �������� ��縦 ������ �ִ�..

    if (!s_pWorldMgr->IsIndoor()) {
        if (fYClimb > fYTerrain &&
            fYClimb < vPosNext.y + ((30.0f / CN3Base::s_fFrmPerSec) *
                                    0.5f)) // �浹 üũ ������Ʈ ���̰��� �ְ� �������� ���� ��츸 ���̰� ����
        {
            if (vNormal.Magnitude() > MAX_INCLINE_CLIMB && vNormal.Dot(vDir) <= 0.0f) // ��� üũ..
            {
                return true;
            }
            m_fYNext = fYClimb;
        } else {
            // ������ ��簡 45 �� �������� üũ
            if (true == ACT_WORLD->CheckInclineWithTerrain(vPosNext, vDir, MAX_INCLINE_CLIMB)) {
                return true;
            }
            m_fYNext = fYTerrain; // ���� ��ġ�� ���߰�..
        }
    } else // �ϴ�..
    {
        if ((fYClimb > fYTerrain) && (fYClimb < vPosNext.y + 0.6f)) {
            m_fYNext = fYClimb;
        } else {
            m_fYNext = fYTerrain; // ���� ��ġ�� ���߰�..
        }

        if ((m_fYNext > vPos.y + 0.6f) || (m_fYNext < vPos.y - 0.6f * 2.0f)) {
            m_fYNext = vPos.y;
            return true;
        }
    }

    //    else // �ö󰥼� ���� ���̸� �������� ���� üũ..
    //    {
    //        // ������ ���ؼ�.. ���⿡ ���� �ٸ� �ӵ��� ����
    //        s_pTerrain->GetNormal(vPos.x, vPos.z, vNormal);
    //        vNormal.Normalize();
    //        vNormal.y    = 0.0f;
    //        float fM = vNormal.Magnitude();
    //        float fD = vNormal.Dot(vDir);
    //        if(fSpeed < 0) fD *= -1.0f;
    //        if(fD < 0) fSpeed *= 1.0f - (fM / 0.7071f); // ���⿡ ���� ���� ����
    //
    //        vPosNext = vPos + (vDir * fSpeed); // ���� ��ġ ���..
    //        m_fYNext = s_pTerrain->GetHeight(vPosNext.x, vPosNext.z);
    //    }

    this->PositionSet(vPosNext, false);

    ///////////////////////////////////////////////////////////////
    // ĳ���� �浹 üũ..
    //    int iSize = s_pOPMgr->m_OPCs.size();
    //    it_UPC it = s_pOPMgr->m_OPCs.begin();
    //    for( int i = 0; i < iSize; i++, it++ )
    //    {
    //        if ( ((*it)->Position() - vPosAfter).Magnitude() < 1.2f )
    //            return vPosBefore;
    //    }

    return false;
}

void CPlayerMySelf::InitFace() {
    __TABLE_PLAYER_LOOKS * pLooks = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pLooks && !pLooks->szPartFNs[PART_POS_FACE].empty()) // �������� �ְ� �� �̸��� ������..
    {
        char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
        ::_splitpath(pLooks->szPartFNs[PART_POS_FACE].c_str(), NULL, szDir, szFName, szExt);
        sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iFace, szExt);
        this->PartSet(PART_POS_FACE, szBuff, NULL, NULL);
    }
}

void CPlayerMySelf::InitHair() {
    __TABLE_PLAYER_LOOKS * pLooks = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pLooks && !pLooks->szPartFNs[PART_POS_HAIR_HELMET].empty()) // �������� �ְ� �� �̸��� ������..
    {
        char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
        ::_splitpath(pLooks->szPartFNs[PART_POS_HAIR_HELMET].c_str(), NULL, szDir, szFName, szExt);
        sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iHair, szExt);
        this->PartSet(PART_POS_HAIR_HELMET, szBuff, NULL, NULL);
    } else {
        m_Chr.PartSet(PART_POS_HAIR_HELMET, "");
        m_ChrInv.PartSet(PART_POS_HAIR_HELMET, "");
    }
}

void CPlayerMySelf::KnightsInfoSet(int iID, const std::string & szName, int iGrade, int iRank) {
    CPlayerBase::KnightsInfoSet(iID, szName, iGrade, iRank);

    m_InfoExt.iKnightsID = iID;
    m_InfoExt.szKnights = szName;
    m_InfoExt.iKnightsGrade = iGrade;
    m_InfoExt.iKnightsRank = iRank;

    if (m_InfoExt.szKnights.empty()) {
        if (m_pClanFont) {
            delete m_pClanFont;
        }
        m_pClanFont = NULL;
    } else {
        if (!m_pClanFont) {
            std::string szFontID;
            ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
            m_pClanFont = new CDFont(szFontID, 12);
            m_pClanFont->InitDeviceObjects(s_lpD3DDev);
            m_pClanFont->RestoreDeviceObjects();
        }

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // ��Ʈ�� �ؽ�Ʈ ����.
        m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
    }
}

void CPlayerMySelf::SetSoundAndInitFont() {
    CPlayerBase::SetSoundAndInitFont();

    if (m_InfoExt.szKnights.empty()) {
        delete m_pClanFont;
        m_pClanFont = NULL;
    } else {
        if (!m_pClanFont) {
            std::string szFontID;
            ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
            m_pClanFont = new CDFont(szFontID, 12);
            m_pClanFont->InitDeviceObjects(s_lpD3DDev);
            m_pClanFont->RestoreDeviceObjects();
        }

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // ��Ʈ�� �ؽ�Ʈ ����.
        m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
    }
}

float CPlayerMySelf::
    MoveSpeedCalculationAndCheckCollision() // �ӵ��� ���ϰ� �� �ӵ��� �浹 üũ�� �Ѵ�. ���ϰ��� 0 �̸� �浹�̴�..
{
    m_fMoveSpeedPerSec = MOVE_SPEED_WHEN_WALK;
    if (PSM_RUN == m_eStateMove) {
        m_fMoveSpeedPerSec *= MOVE_DELTA_WHEN_RUNNING;
    } else if (PSM_STOP == m_eStateMove) {
        m_fMoveSpeedPerSec = 0.0f;
    } else if (PSM_WALK_BACKWARD == m_eStateMove) {
        m_fMoveSpeedPerSec *= -1.0f;
    }
    m_fMoveSpeedPerSec *= m_fMoveDelta; // �̵� �ӵ� ���ϱ⿡ ����... �����ش�.

    if (m_bTempMoveTurbo) // ���߿� �÷���...
    {
        m_fMoveSpeedPerSec *= 10.0f;
    }

    if (this->CheckCollision()) // �浹 üũ��..
    {
        if (CGameProcedure::s_pProcMain) {
            CGameProcedure::s_pProcMain->CommandMove(MD_STOP, true);
        }
        //        m_fMoveSpeedPerSec = 0;
    }

    // ������ ���ؼ�.. ���⿡ ���� �ٸ� �ӵ��� ����
    /*    __Vector3 vDir = this->Direction();
    __Vector3 vPos = m_Chr.Pos(), vNormal(0,1,0);
    s_pTerrain->GetNormal(vPos.x, vPos.z, vNormal);
    vNormal.Normalize();
    vNormal.y    = 0.0f;
    float fM = vNormal.Magnitude();
    float fD = vNormal.Dot(vDir);
    if(fSpeed < 0) fD *= -1.0f;
//    if (fM > MAX_INCLINE_CLIMB && fD <= 0.0f )
//    {
//    }
    if(fD < 0) fSpeed *= 1.0f - (fM / 0.7071f); // ���⿡ ���� ���� ����
*/
    return m_fMoveSpeedPerSec;
}

void CPlayerMySelf::Stun(float fTime) // ������ �ð����� ���� ��Ű��.
{
    m_bStun = true;      // ����..
    m_fStunTime = fTime; // ������ �ð�..

    CGameProcedure::s_pProcMain->CommandEnableAttackContinous(false, NULL); // ���� ���߰�..
    CGameProcedure::s_pProcMain->CommandMove(MD_STOP, true);                // �������� ���߰� �ϰ�..
}

void CPlayerMySelf::StunRelease() // ���� Ǯ��..
{
    m_bStun = false;    // ����..
    m_fStunTime = 0.0f; // ������ �ð�..
}

void CPlayerMySelf::TargetOrPosMove() {
    if (!m_bTargetOrPosMove) {
        return;
    }

    m_vTargetPos.y = m_Chr.Pos().y;

    if (m_iMoveTarget >= 0) {
        CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(m_iMoveTarget, true);
        if (pTarget) {
            m_vTargetPos = pTarget->Position();
        } else {
            this->ActionMove(PSM_STOP);
            CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // ���� ��Ŷ ������..
        }
    }

    __Vector3 vDir = m_vTargetPos - s_pPlayer->Position();
    vDir.y = 0.0f;
    vDir.Normalize();

    float fYaw = ::_Yaw2D(vDir.x, vDir.z);
    this->RotateTo(fYaw, true); // ������ ������

    if (m_iMoveTarget >= 0) {
        CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(m_iMoveTarget, true);

        float fDist = (pTarget->Position() - m_Chr.Pos()).Magnitude(); // ���� �Ÿ��� ���ϰ�..
        float fDistLimit = this->AttackableDistance(pTarget);

        if (fDist < fDistLimit) {
            CGameProcedure::s_pProcMain->CommandEnableAttackContinous(true, pTarget);
            this->ActionMove(PSM_STOP);
            CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // ���� ��Ŷ ������..
        }
    } else {
        float fDist = (m_vTargetPos - Position()).Magnitude();
        if (fDist < 0.5f) {
            this->ActionMove(PSM_STOP);
            CGameProcedure::s_pProcMain->MsgSend_Move(false, false); // ���� ��Ŷ ������..
        }
    }
}
