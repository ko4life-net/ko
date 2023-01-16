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
        if (m_pBtnOption) {
            m_pBtnOption->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pBtnExit) {
            m_pBtnExit->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pBtnCancel) {
            m_pBtnCancel->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }
}

void CUIExitMenu::SelectCharacter() {

    /*
	
  v0 = s_pPlayer;
    if (s_pPlayer && s_pFX) {
        s_pPlayer->m_bMoveUnk = 0;
        s_pPlayer->field_9EC = 0;
        CN3FXMgr::TriggerCursor((_DWORD *)s_pFX, s_pPlayer->m_InfoBase_iID, s_pPlayer->m_InfoBase_iID,
                                30001, // fx/target_pointer.fxb
                                30001, FX_BUNDLE_MOVE_DIR_FLEXABLETARGET);
        v0 = s_pPlayer;
    }
    
    */
    // TODO: CN3FXMgr::TriggerCursor
    CGameBase::s_pPlayer->StunRelease();

    if (CGameProcedure::s_pProcMain->m_pSubProcPerTrade) {

        // TODO: MERCHANT CLEAR MODEL

        CGameProcedure::s_pProcMain->m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_CANCEL);
    }

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

void CUIExitMenu::AddWarningMessage(DWORD idMessage, bool visbleMenu) {

    if (!CGameProcedure::s_pProcMain->m_pUIChatDlg) {
        return;
    }

    std::string szMsg;
    ::_LoadStringFromResource(idMessage, szMsg);

    CGameProcedure::s_pProcMain->m_pUIChatDlg->AddChatMsg(N3_CHAT_NORMAL, szMsg, 0xFFFF0000);

    SetVisible(visbleMenu);
}

bool CUIExitMenu::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnChr) { // Select characeter
            if (CGameProcedure::s_pProcMain->m_eExitState == EXIT_STATE_ALLOW_LEAVE) {
                // CGameProcedure::s_pProcMain->m_bIsSeekingParty = false; // TODO: Reset seeking party.
                SelectCharacter();
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);
            CGameProcedure::s_pProcMain->m_eExitType = EXIT_TYPE_SELECTCHAR;
        } else if (pSender == (CN3UIBase *)m_pBtnExit) {
            if (CGameProcedure::s_pProcMain->m_eExitState == EXIT_STATE_ALLOW_LEAVE) {
                ::PostQuitMessage(0);
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);
            CGameProcedure::s_pProcMain->m_eExitType = EXIT_TYPE_EXIT;
        } else if (pSender == (CN3UIBase *)m_pBtnOption) {
            if (CGameProcedure::s_pProcMain->m_eExitState == EXIT_STATE_ALLOW_LEAVE) {
                ::ShellExecute(NULL, "open", "Option.exe", NULL, NULL, SW_SHOWNORMAL);
                PostQuitMessage(0);
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);
            CGameProcedure::s_pProcMain->m_eExitType = EXIT_TYPE_OPTION;
        } else if (pSender == (CN3UIBase *)m_pBtnCancel) {
            SetVisible(false);
        }
    }

    return true;
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

void CUIExitMenu::ClearExitState() {
    CGameProcedure::s_pProcMain->m_eExitType = EXIT_TYPE_NONE;
    CGameProcedure::s_pProcMain->m_eExitState = EXIT_STATE_ALLOW_LEAVE;
    CGameProcedure::s_pProcMain->m_iExitSecondsElapsed = 0;
}
