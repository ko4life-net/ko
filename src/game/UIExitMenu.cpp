#include "StdAfx.h"
#include "UIExitMenu.h"

#include "Resource.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "SubProcPerTrade.h"

#include "UIManager.h"
#include "UIMessageWnd.h"
#include "UIPartyOrForce.h"

#include "N3Base/N3FXDef.h"
#include "N3Base/N3UIButton.h"

CUIExitMenu::CUIExitMenu() {
    m_pBtnChr = NULL;
    m_pBtnExit = NULL;
    m_pBtnOption = NULL;
    m_pBtnCancel = NULL;
}

CUIExitMenu::~CUIExitMenu() = default;

void CUIExitMenu::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisible(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI();
        if (m_pBtnChr) {
            m_pBtnChr->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pBtnExit) {
            m_pBtnExit->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pBtnOption) {
            m_pBtnOption->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pBtnCancel) {
            m_pBtnCancel->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }
}

bool CUIExitMenu::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender || !CGameProcedure::s_pProcMain) {
        return false;
    }

#define INITIATE_EXIT_GAME(eExitType)                                                                                  \
    if (!CGameProcedure::s_pProcMain->m_pUIChatDlg) {                                                                  \
        return true;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    std::string szMsg;                                                                                                 \
    ::_LoadStringFromResource(IDS_EXIT_GAME_DURING_BATTLE_WARNING, szMsg);                                             \
    CGameProcedure::s_pProcMain->m_pUIChatDlg->AddChatMsg(N3_CHAT_NORMAL, szMsg, 0xFFFF0000);                          \
    CGameProcedure::s_pProcMain->m_eExitType = eExitType;                                                              \
    SetVisible(false);

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnChr) {
            if (CGameProcedure::s_pProcMain->m_fExitCurCountDownToReach == -1.0f) {
                //CGameProcedure::s_pProcMain->m_bRecruitingParty = false; // TODO:
                //CGameProcedure::s_pProcMain->m_bPremiumSomething = true;
                SelectCharacter();
            } else {
                INITIATE_EXIT_GAME(EXIT_TYPE_SELECTCHAR);
            }
        } else if (pSender == (CN3UIBase *)m_pBtnExit) {
            if (CGameProcedure::s_pProcMain->m_fExitCurCountDownToReach == -1.0f) {
                ::PostQuitMessage(0);
            } else {
                INITIATE_EXIT_GAME(EXIT_TYPE_EXIT);
            }
        } else if (pSender == (CN3UIBase *)m_pBtnOption) {
            if (CGameProcedure::s_pProcMain->m_fExitCurCountDownToReach == -1.0f) {
                ::ShellExecute(NULL, "open", "Option.exe", NULL, NULL, SW_SHOWNORMAL);
                ::PostQuitMessage(0);
            } else {
                INITIATE_EXIT_GAME(EXIT_TYPE_OPTION);
            }
        } else if (pSender == (CN3UIBase *)m_pBtnCancel) {
            SetVisible(false);
        }
    }

    return true;
}

void CUIExitMenu::SelectCharacter() {
    // TODO: Trigger cursor effect
    //if (CGameBase::s_pPlayer && CGameProcedure::s_pFX) {
    //    CGameBase::s_pPlayer->StunRelease();
    //    CGameProcedure::s_pFX->TriggerCursor(CGameBase::s_pPlayer->m_InfoBase.iID, CGameBase::s_pPlayer->m_InfoBase.iID,
    //                                         30001, 30001, // fx/target_pointer.fxb for target curosr
    //                                         FX_BUNDLE_MOVE_DIR_FLEXABLETARGET);
    //}

    // TODO: Close merchant
    //if (CGameBase::s_pPlayer->m_bIsMerchantOpened) {
    //    CGameBase::s_pPlayer->InitOrDeleteMerchant(false, NULL);
    //    if (CGameProcedure::s_pProcMain->m_pUITradeInventory) {
    //        CGameProcedure::s_pProcMain->m_pUITradeInventory->m_bMerchantOpened = false;
    //        CGameProcedure::s_pProcMain->m_pUITradeInventory->MsgSend_MerchantClose();
    //    }
    //}

    std::string szIP = CGameProcedure::s_pSocket->GetCurrentIP();
    DWORD       dwPort = CGameProcedure::s_pSocket->GetCurrentPort();

    CGameProcedure::s_bNeedReportConnectionClosed = false;
    CGameProcedure::s_pSocket->Disconnect();
    Sleep(4000);
    CGameProcedure::s_pSocket->Connect(CGameProcedure::s_hWndBase, szIP.c_str(), dwPort);
    CGameProcedure::s_bNeedReportConnectionClosed = true;
    CGameProcedure::s_bNeedReportVersionCheck = true;
    CGameProcedure::ProcActiveSet((CGameProcedure *)CGameProcedure::s_pProcCharacterSelect);
}

bool CUIExitMenu::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtnChr = (CN3UIButton *)GetChildByID("btn_chr");
    __ASSERT(m_pBtnChr, "NULL UI Component!!");

    m_pBtnExit = (CN3UIButton *)GetChildByID("btn_exit");
    __ASSERT(m_pBtnExit, "NULL UI Component!!");

    m_pBtnOption = (CN3UIButton *)GetChildByID("btn_option");
    __ASSERT(m_pBtnOption, "NULL UI Component!!");

    m_pBtnCancel = (CN3UIButton *)GetChildByID("btn_cancel");
    __ASSERT(m_pBtnCancel, "NULL UI Component!!");

    return true;
}
