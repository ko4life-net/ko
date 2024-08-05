// UICmd.cpp: implementation of the CUICmd class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UICmd.h"
#include "GameProcMain.h"
#include "PlayerOtherMgr.h"
//#include "GameProcLogIn.h"
#include "PlayerMyself.h"
#include "UITransactionDlg.h"
#include "N3Base/N3UIButton.h"
#include "UIManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUICmd::CUICmd() {
    m_pBtn_Exit = NULL; //나가기

    m_pBtn_Act = NULL;        //행동
    m_pBtn_Act_Walk = NULL;   //걷기
    m_pBtn_Act_Run = NULL;    //달리기
    m_pBtn_Act_Stop = NULL;   //서기
    m_pBtn_Act_Attack = NULL; //공격

    m_pBtn_Act_StandUp = NULL; // 일어서기.
    m_pBtn_Act_SitDown = NULL; // 앉기

    m_pBtn_Camera = NULL;        //카메라
    m_pBtn_Inventory = NULL;     //아이템 창
    m_pBtn_Party_Invite = NULL;  //파티 초대
    m_pBtn_Party_Disband = NULL; //파티 탈퇴
    m_pBtn_Option = NULL;        //옵션
    m_pBtn_Quest = NULL;         //퀘스트
    m_pBtn_Character = NULL;     //자기 정보창
    m_pBtn_Skill = NULL;         //스킬트리 또는 마법창
    m_pBtn_Belong = NULL;        //소속 기사단
    m_pBtn_Map = NULL;           // 미니맵
}

CUICmd::~CUICmd() {
    DestroyTooltip();
}

bool CUICmd::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_Act = GetChildByID("btn_control");       //__ASSERT(m_pBtn_Act, "NULL UI Component!!");
    m_pBtn_Act_Walk = GetChildByID("btn_walk");     //__ASSERT(m_pBtn_Act_Walk, "NULL UI Component!!");
    m_pBtn_Act_Run = GetChildByID("btn_run");       //__ASSERT(m_pBtn_Act_Run, "NULL UI Component!!");
    m_pBtn_Act_Stop = GetChildByID("btn_stop");     //__ASSERT(m_pBtn_Act_Stop, "NULL UI Component!!");
    m_pBtn_Act_Attack = GetChildByID("btn_attack"); //__ASSERT(m_pBtn_Act_Attack, "NULL UI Component!!");
    m_pBtn_Act_StandUp = GetChildByID("btn_stand"); //__ASSERT(m_pBtn_Act_StandUp, "NULL UI Component!!");
    m_pBtn_Act_SitDown = GetChildByID("btn_sit");   //__ASSERT(m_pBtn_Act_SitDown, "NULL UI Component!!");

    if (m_pBtn_Act_StandUp) {
        m_pBtn_Act_StandUp->SetVisible(false); // 일어서기 버튼은 미리 죽여놓는다..
    }

    m_pBtn_Character = GetChildByID("btn_character"); //__ASSERT(m_pBtn_Character, "NULL UI Component!!");
    m_pBtn_Inventory = GetChildByID("btn_inventory"); //__ASSERT(m_pBtn_Inventory, "NULL UI Component!!");

    m_pBtn_Option = GetChildByID("btn_option");         //__ASSERT(m_pBtn_Option, "NULL UI Component!!");
    m_pBtn_Camera = GetChildByID("btn_camera");         //__ASSERT(m_pBtn_Camera, "NULL UI Component!!");
    m_pBtn_Party_Invite = GetChildByID("btn_invite");   //__ASSERT(m_pBtn_Party_Invite, "NULL UI Component!!");
    m_pBtn_Party_Disband = GetChildByID("btn_disband"); //__ASSERT(m_pBtn_Party_Disband, "NULL UI Component!!");
    m_pBtn_Skill = GetChildByID("btn_skill");           //__ASSERT(m_pBtn_Skill, "NULL UI Component!!");
    m_pBtn_Exit = GetChildByID("btn_Exit");             //__ASSERT(m_pBtn_Exit, "NULL UI Component!!");

    m_pBtn_Quest = GetChildByID("btn_quest");   //__ASSERT(m_pBtn_Quest, "NULL UI Component!!");
    m_pBtn_Belong = GetChildByID("btn_knight"); //__ASSERT(m_pBtn_Belong, "NULL UI Component!!");

    m_pBtn_Map = GetChildByID("btn_map"); //__ASSERT(m_pBtn_Map, "NULL UI Component!!");

    //    this->SetVisibleActButtons(true);
    //    this->SetVisibleOptButtons(false);

    return true;
}

bool CUICmd::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (CGameProcedure::s_pProcMain->m_pUITransactionDlg->IsVisible()) {
        return true;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        //        if(pSender == m_pBtn_Option)
        //        {
        //            this->SetVisibleActButtons(false);
        //            this->SetVisibleOptButtons(true);
        //        }

        if (pSender == m_pBtn_Act) {
            //            this->SetVisibleActButtons(true);
            //            this->SetVisibleOptButtons(false);
        }

        else if (pSender == m_pBtn_Act_Walk) {
            CGameProcedure::s_pProcMain->CommandToggleWalkRun();
        }

        else if (pSender == m_pBtn_Act_Run) {
            CGameProcedure::s_pProcMain->CommandToggleWalkRun();
        }

        else if (pSender == m_pBtn_Act_Attack) {
            CGameProcedure::s_pProcMain->CommandToggleAttackContinous();
        }

        else if (pSender == m_pBtn_Inventory) {
            CGameProcedure::s_pProcMain->CommandToggleUIInventory();
        }

        else if (pSender == m_pBtn_Character) {
            CGameProcedure::s_pProcMain->CommandToggleUIState();
        }

        else if (pSender == m_pBtn_Option) {
            CGameProcedure::s_pProcMain->CommandToggleCmdList();
        } else if (pSender == m_pBtn_Exit) {
            CGameProcedure::s_pProcMain->CommandExitMenu();
        }

        else if (pSender == m_pBtn_Camera) {
            CGameProcedure::s_pProcMain->CommandCameraChange(); // 카메라 시점 바꾸기..
        }

        else if (pSender == m_pBtn_Party_Invite) {
            CPlayerOther * pUPC = CGameProcedure::s_pOPMgr->UPCGetByID(CGameProcedure::s_pPlayer->m_iIDTarget, true);
            if (pUPC && pUPC->m_InfoBase.eNation == CGameProcedure::s_pPlayer->m_InfoBase.eNation) // 국가 체크
            {
                CGameProcedure::s_pProcMain->MsgSend_PartyOrForceCreate(0, pUPC->IDString()); // 파티 초대하기..
            }
        }

        else if (pSender == m_pBtn_Party_Disband) {
            CGameProcMain * pMain = CGameProcedure::s_pProcMain;
            CPlayerMySelf * pPlayer = CGameBase::s_pPlayer;

            bool          bIAmLeader = false, bIAmMemberOfParty = false;
            int           iMemberIndex = -1;
            CPlayerBase * pTarget = NULL;
            pMain->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex,
                                            pTarget); // 파티의 상황을 보고..

            std::string szMsg;
            if (bIAmLeader) // 내가 리더면..
            {
                if (iMemberIndex > 0) {
                    ::_LoadStringFromResource(IDS_PARTY_CONFIRM_DISCHARGE, szMsg); // 파티에서 쫓아낼까여?
                    szMsg = pTarget->IDString() + szMsg;
                } else {
                    ::_LoadStringFromResource(IDS_PARTY_CONFIRM_DESTROY, szMsg); // 파티 뽀갤까여?
                }
            } else if (bIAmMemberOfParty) {
                ::_LoadStringFromResource(IDS_PARTY_CONFIRM_LEAVE, szMsg); // 파티에서 나갈까여?
            }

            if (!szMsg.empty()) {
                CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO,
                                               BEHAVIOR_PARTY_DISBAND); // 파티 해체,축출,탈퇴하기..확인
            }
        }

        else if (pSender == m_pBtn_Act_SitDown) {
            CGameProcedure::s_pProcMain->CommandSitDown(true, true);
        }

        else if (pSender == m_pBtn_Act_StandUp) {
            CGameProcedure::s_pProcMain->CommandSitDown(true, false);
        }

        else if (pSender == m_pBtn_Skill) {
            CGameProcedure::s_pProcMain->CommandToggleUISkillTree();
        }

        else if (pSender == m_pBtn_Map) {
            CGameProcedure::s_pProcMain->CommandToggleUIMiniMap();
        }
    }

    return true;
}

/*
void CUICmd::SetVisibleActButtons(bool bVisible)
{
    //행동
    if(m_pBtn_Act_Walk) m_pBtn_Act_Walk->SetVisible(bVisible); 
    if(m_pBtn_Act_Run) m_pBtn_Act_Run->SetVisible(bVisible);
    if(m_pBtn_Act_Stop) m_pBtn_Act_Stop->SetVisible(bVisible);
    if(m_pBtn_Act_StandUp) m_pBtn_Act_StandUp->SetVisible(bVisible);
    if(m_pBtn_Act_SitDown) m_pBtn_Act_SitDown->SetVisible(bVisible);
    if(m_pBtn_Act_Attack) m_pBtn_Act_Attack->SetVisible(bVisible);
}
*/

/*
void CUICmd::SetVisibleOptButtons(bool bVisible)
{
    //옵션
    if(m_pBtn_Opt_Quest) m_pBtn_Opt_Quest->SetVisible(bVisible);
    if(m_pBtn_Character) m_pBtn_Character->SetVisible(bVisible);    
    if(m_pBtn_Skill) m_pBtn_Skill->SetVisible(bVisible);
    if(m_pBtn_Opt_Knight) m_pBtn_Opt_Knight->SetVisible(bVisible);
    if(m_pBtn_Inventory) m_pBtn_Inventory->SetVisible(bVisible);    
}
*/

void CUICmd::UpdatePartyButtons(bool bIAmLeader, bool bIAmMemberOfParty, int iMemberIndex,
                                const CPlayerBase * pTarget) {
    bool bInvite = true;
    if (bIAmLeader) // 내가 리더이면..
    {
        if (pTarget) // 타겟이 있고..
        {
            if (iMemberIndex > 0) {
                bInvite = false; // 타겟이 파티원이면..축출 가능하게.
            } else {
                bInvite = true;
            }
        } else {
            bInvite = false; // 리더도 나갈수 있다..
        }
    } else {
        if (bIAmMemberOfParty) {
            bInvite = false; // 리더는 아니지만 파티에 들어있는 상태이면.. 탈퇴가능..
        } else {
            bInvite = true; // 파티에 안들어 있다면 초대 가능...
        }
    }

    if (m_pBtn_Party_Invite) {
        m_pBtn_Party_Invite->SetVisible(bInvite);
    }
    if (m_pBtn_Party_Disband) {
        m_pBtn_Party_Disband->SetVisible(!bInvite);
    }
}

bool CUICmd::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE: { //hotkey가 포커스 잡혀있을때는 다른 ui를 닫을수 없으므로 DIK_ESCAPE가 들어오면 포커스를 다시잡고
        //열려있는 다른 유아이를 닫아준다.
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
        CN3UIBase * pFocus = CGameProcedure::s_pUIMgr->GetFocusedUI();
        if (pFocus && pFocus != this) {
            pFocus->OnKeyPress(iKey);
        }
    }
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}
