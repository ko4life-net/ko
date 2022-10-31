// UIChat.cpp: implementation of the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIExitMenu.h"
#include "PlayerMySelf.h"

#include "APISocket.h"
#include "PacketDef.h"
#include "GameProcMain.h"
#include "GameProcedure.h"
#include "GameProcLogin.h"
#include "GameDef.h"

#include "UIMessageWnd.h"
#include "UIManager.h"
#include "UIPartyOrForce.h"
#include "SubProcPerTrade.h"

#include "Resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIExitMenu::CUIExitMenu() {
    m_pBtn_chr = NULL;
    m_pBtn_exit = NULL;
    m_pBtn_option = NULL;
    m_pBtn_cancel = NULL;
}

CUIExitMenu::~CUIExitMenu() {}

void CUIExitMenu::Release() {
    CN3UIBase::Release();
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

    if (CGameProcedure::s_pProcMain->m_pUIPartyOrForce) {
        CGameProcedure::s_pProcMain->m_pUIPartyOrForce->MemberDestroy();
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

        if (pSender->m_szID == "btn_chr") { // Select characeter

            if (CGameProcedure::s_pProcMain->m_pExitState == EXIT_STATE_ALLOW_LEAVE) {
                SelectCharacter();
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);
            CGameProcedure::s_pProcMain->m_pExitType = EXIT_TYPE_SELECTCHAR;
        }

        else if (pSender->m_szID == "btn_exit") {
            if (CGameProcedure::s_pProcMain->m_pExitState == EXIT_STATE_ALLOW_LEAVE) {
                ::PostQuitMessage(0);
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);

            CGameProcedure::s_pProcMain->m_pExitType = EXIT_TYPE_EXIT;
        } else if (pSender->m_szID == "btn_option") {
            if (CGameProcedure::s_pProcMain->m_pExitState == EXIT_STATE_ALLOW_LEAVE) {
                ::ShellExecute(NULL, "open", "Option.exe", NULL, NULL, SW_SHOWNORMAL);
                PostQuitMessage(0);
            }

            AddWarningMessage(IDS_EXIT_GAME_DURING_BATTLE_WARNING, false);
            CGameProcedure::s_pProcMain->m_pExitType = EXIT_TYPE_OPTION;
        } else if (pSender->m_szID == "btn_cancel") {
            SetVisible(false);
        }
    }
    return true;
}

bool CUIExitMenu::Load(HANDLE hFile) {
    if (false == CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtn_chr = (CN3UIButton *)GetChildByID("btn_chr");
    __ASSERT(m_pBtn_chr, "NULL UI Component!!");
    m_pBtn_exit = (CN3UIButton *)GetChildByID("btn_exit");
    __ASSERT(m_pBtn_exit, "NULL UI Component!!");
    m_pBtn_option = (CN3UIButton *)GetChildByID("btn_option");
    __ASSERT(m_pBtn_option, "NULL UI Component!!");
    m_pBtn_cancel = (CN3UIButton *)GetChildByID("btn_cancel");
    __ASSERT(m_pBtn_cancel, "NULL UI Component!!");

    return true;
}

void CUIExitMenu::Render() {
    if (!m_bVisible) {
        return;
    }

    CN3UIBase::Render();
}

void CUIExitMenu::ClearExitState() {
    CGameProcedure::s_pProcMain->m_pExitType = EXIT_TYPE_NONE;
    CGameProcedure::s_pProcMain->m_pExitState = EXIT_STATE_ALLOW_LEAVE;
    CGameProcedure::s_pProcMain->m_pExitSecondsElapsed = 0;
}

bool CUIExitMenu::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE: {
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
