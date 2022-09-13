// UIChat.cpp: implementation of the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIExitMenu.h"
#include "PacketDef.h"
#include "GameProcMain.h"
#include "UIMessageWnd.h"
#include "UIManager.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIExitMenu::CUIExitMenu() {}

CUIExitMenu::~CUIExitMenu() {}

void CUIExitMenu::Release() {
    CN3UIBase::Release();

    m_pBtn_chr = NULL;
    m_pBtn_exit = NULL;
    m_pBtn_option = NULL;
    m_pBtn_cancel = NULL;
}

void CUIExitMenu::Open(string userName) {
    SetVisible(true);

    return;
}

bool CUIExitMenu::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    else if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender->m_szID == "btn_chr") {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_EXIT, szMsg);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_RESTART_GAME);
        } else if (pSender->m_szID == "btn_exit") {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_EXIT, szMsg);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_EXIT);
        } else if (pSender->m_szID == "btn_option") {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_CONFIRM_EXECUTE_OPTION, szMsg);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_EXECUTE_OPTION);
        } else if (pSender->m_szID == "btn_cancel") {
            SetVisible(false);
        }
    } else if (dwMsg == UIMSG_EDIT_RETURN) {
        CN3UIEdit * pEdit = (CN3UIEdit *)pSender;
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

bool CUIExitMenu::OnKeyPress(int iKey) {

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
