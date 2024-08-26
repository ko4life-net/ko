// UIChat.cpp: implementation of the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIChatWhisperHide.h"
#include "UIChatWhisperOpen.h"
#include "PacketDef.h"
#include "GameProcMain.h"
#include "UIMessageWnd.h"
#include "UIManager.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIEdit.h" //son, chat_in

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIChatWhisperHide::CUIChatWhisperHide() {
}

CUIChatWhisperHide::~CUIChatWhisperHide() {}

void CUIChatWhisperHide::Release() {
    CN3UIBase::Release();

    m_pText_UserName = NULL;
    m_pBtn_close = NULL;
}

void CUIChatWhisperHide::Open(string userName) {
    SetVisible(true);
    m_pText_UserName->SetString(userName);

    return;
}

bool CUIChatWhisperHide::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    else if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender->m_szID == "btn_open") {
            SetVisible(false);
            POINT chatPosition = GetPos();
            m_messageNotRead = false;
            CGameProcedure::s_pProcMain->WhisperOpen(m_pText_UserName->GetString(), chatPosition);
        }

    }
    //son, chat_in
    else if (dwMsg == UIMSG_EDIT_RETURN) {
        CN3UIEdit * pEdit = (CN3UIEdit *)pSender;

    } else {
        SetState(UI_STATE_COMMON_MOVE);
    }
    //son, chat_in

    return true;
}

bool CUIChatWhisperHide::Load(HANDLE hFile) {
    if (false == CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pText_UserName = (CN3UIString *)GetChildByID("exit_id");
    __ASSERT(m_pText_UserName, "NULL UI Component!!");

    m_pBtn_open = (CN3UIButton *)GetChildByID("btn_open");
    __ASSERT(m_pBtn_open, "NULL UI Component!!");

    m_pBtn_bar = (CN3UIButton *)GetChildByID("btn_bar");
    __ASSERT(m_pBtn_bar, "NULL UI Component!!");

    return true;
}


void CUIChatWhisperHide::Render() {
    if (!m_bVisible) {
        return;
    }

    DWORD dwTime = GetTickCount();

    if (m_messageNotRead) {
        if (((dwTime / 1000) & 1) == 1) {
            GetChildByID("btn_bar")->SetState(UI_STATE_BUTTON_ON);
        } else {
            GetChildByID("btn_bar")->SetState(UI_STATE_BUTTON_NORMAL);
        }

    } else {
        GetChildByID("btn_bar")->SetState(UI_STATE_BUTTON_NORMAL);
    }

    CN3UIBase::Render();
}

//son, chat_in
void CUIChatWhisperHide::SetFocus() {
    SetEnableKillFocus(false);
    this->SetString("");
    if (m_pEdit) {
        m_pEdit->SetFocus();
    }
}

void CUIChatWhisperHide::KillFocus() {
    this->SetString("");
    if (m_pEdit) {
        m_pEdit->KillFocus();
    }
}

void CUIChatWhisperHide::SetString(const std::string & szChat) {
    m_szString = szChat;
    if (m_pEdit) {
        m_pEdit->SetString(m_szString);
        m_pEdit->SetCaretPos(m_szString.size());
    }
}

void CUIChatWhisperHide::SetCaretPos(int iPos) {
    /*
	if (IsChatMode() && m_pEdit)
	{
		m_pEdit->SetCaretPos(iPos);
	}
	*/
}

BOOL CUIChatWhisperHide::MoveOffset(int iOffsetX, int iOffsetY) {
    if (0 == iOffsetX && 0 == iOffsetY) {
        return FALSE;
    }
    // ui żµżŞ
    m_rcRegion.left += iOffsetX;
    m_rcRegion.top += iOffsetY;
    m_rcRegion.right += iOffsetX;
    m_rcRegion.bottom += iOffsetY;

    // movable żµżŞ
    m_rcMovable.left += iOffsetX;
    m_rcMovable.top += iOffsetY;
    m_rcMovable.right += iOffsetX;
    m_rcMovable.bottom += iOffsetY;

    // children ÁÂÇĄ °»˝Ĺ
    CN3UIBase * pCUI = NULL; // Child UI...
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        pCUI = (*itor);
        __ASSERT(pCUI, "child UI pointer is NULL!");
        pCUI->MoveOffset(iOffsetX, iOffsetY);
    }

    return TRUE;
}

void CUIChatWhisperHide::SetRegion(const RECT & Rect) {
    CN3UIBase::SetRegion(Rect);
}

void CUIChatWhisperHide::SetNewMessageAnimation(bool animationActivated) {
    if (animationActivated) {
        static float fTime;
        static float fTimePrevFps = CN3Base::TimeGet();
        if (fTime > fTimePrevFps + 0.5f) {
            fTimePrevFps = fTime;
        }
    }
}

bool CUIChatWhisperHide::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE: { //hotkey°ˇ Ć÷Äż˝ş ŔâÇôŔÖŔ»¶§´Â ´Ů¸Ą ui¸¦ ´ÝŔ»Ľö ľřŔ¸ąÇ·Î DIK_ESCAPE°ˇ µéľîżŔ¸é Ć÷Äż˝ş¸¦ ´Ů˝ĂŔâ°í
        //ż­·ÁŔÖ´Â ´Ů¸Ą ŔŻľĆŔĚ¸¦ ´ÝľĆÁŘ´Ů.
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
