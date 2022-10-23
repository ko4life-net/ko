// UINotice.cpp: implementation of the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UINotice.h"
#include "Resource.h"
#include "GameProcMain.h"
#include "GameProcedure.h"
#include "PlayerMySelf.h"
#include "UIManager.h"
#include "UIRookieTip.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINotice::CUINotice() {
    m_pTextNotice = NULL;
    m_pTextEvent = NULL;
    m_pTextEventName = NULL;
    m_pTextTip = NULL;
    m_pBtnQuit = NULL;

    m_fMoveDelta = 0.0f;
    m_bOpenningNow = false;
    m_bClosingNow = false;

    m_Texts.clear();
    m_TextsEvent.clear();
}

CUINotice::~CUINotice() = default;

void CUINotice::Release() {
    m_Texts.clear();
    m_TextsEvent.clear();

    CN3UIBase::Release();
}

void CUINotice::Open() {

    SetState(UI_STATE_COMMON_NONE);

    SetVisible(true);
    this->SetPos(CN3Base::s_CameraData.vp.Width, 10);
    m_fMoveDelta = 0;
    m_bOpenningNow = true;
    m_bClosingNow = false;

    if (m_pSnd_OpenUI) {
        m_pSnd_OpenUI->Play();
    }
}

void CUINotice::Close() {

    SetState(UI_STATE_COMMON_NONE);

    RECT rc = this->GetRegion();
    this->SetPos(CN3Base::s_CameraData.vp.Width - (rc.right - rc.left), 10);

    m_fMoveDelta = 0;
    m_bOpenningNow = false;
    m_bClosingNow = true;

    if (m_pSnd_CloseUI) {
        m_pSnd_CloseUI->Play();
    }
}

bool CUINotice::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pTextNotice = (CN3UIString *)GetChildByID("Text_Notice");
    __ASSERT(m_pTextNotice, "NULL UI Component!!");

    m_pBtnQuit = (CN3UIButton *)GetChildByID("btn_quit");
    __ASSERT(m_pBtnQuit, "NULL UI Component!!");

    m_pTextEventName = (CN3UIString *)GetChildByID("text_event_name");
    __ASSERT(m_pTextEventName, "NULL UI Component!!");

    m_pTextTip = (CN3UIString *)GetChildByID("text_tip");
    __ASSERT(m_pTextTip, "NULL UI Component!!");

    m_pTextEvent = (CN3UIString *)GetChildByID("text_event");
    __ASSERT(m_pTextEvent, "NULL UI Component!!");

    return true;
}

bool CUINotice::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnQuit && !m_bClosingNow) {
            m_fMoveDelta = 0.0f;
            m_bClosingNow = true;

            __WndInfo WIHelp{};
            if (CGameProcedure::RegGetSetting(UI_POST_WND_HELP, &WIHelp, sizeof(__WndInfo)) && !WIHelp.bVisible) {
                if (CGameProcedure::s_pPlayer->m_InfoBase.iLevel <= 20 /* && !s_pPlayer->m_InfoBase.bIsChicken */) {
                    __WndInfo WIRookie{};
                    WIRookie.bVisible = true;
                    CGameProcedure::RegGetSetting(UI_POST_WND_ROOKIETIP, &WIRookie, sizeof(__WndInfo));
                    if (WIRookie.bVisible) {
                        if (CGameProcedure::s_pProcMain->m_pUIRookieTip) {
                            CGameProcedure::s_pProcMain->m_pUIRookieTip->SetVisible(true);
                        }
                    }
                }
            }
        }
    }

    return true;
}

void CUINotice::GenerateText() {
    if (!m_pTextNotice) {
        return;
    }

    int iTextLen = 0;
    for (const auto & text : m_Texts) {
        iTextLen += text.size() + 3; // LineFeed, Carriage return
    }
    if (iTextLen <= 0) {
        return;
    }

    std::vector<char> szTextNotice(iTextLen * 2, 0);
    for (const auto & text : m_Texts) {
        lstrcat(szTextNotice.data(), text.c_str());
        lstrcat(szTextNotice.data(), "\n");
    }
    m_pTextNotice->SetString(szTextNotice.data());

    // Event messages or tip of the day
    if (m_TextsEvent.size() > 0) {
        iTextLen = 0;
        for (const auto & text : m_TextsEvent) {
            iTextLen += text.size() + 3; // LineFeed, Carriage return
        }
        if (iTextLen <= 0) {
            return;
        }

        std::vector<char> szTextEvent(iTextLen * 2, 0);
        for (const auto & text : m_TextsEvent) {
            lstrcat(szTextEvent.data(), text.c_str());
            lstrcat(szTextEvent.data(), "\n");
        }
        if (m_pTextEvent) {
            m_pTextEvent->SetString(szTextEvent.data());
        }
        if (m_pTextTip) {
            m_pTextTip->SetVisible(false);
        }
    } else {
        srand((uint32_t)time(NULL));

        std::string szStrMsg;
        ::_LoadStringFromResource(IDS_HELP_TIP_ALL, szStrMsg);
        m_pTextEventName->SetString(szStrMsg);

        ::_LoadStringFromResource(IDS_HELP_TIP1 + (rand() % 32), szStrMsg);
        m_pTextTip->SetString(szStrMsg);
    }
}

void CUINotice::Tick() {
    if (!m_bVisible) {
        return;
    }

    if (m_bOpenningNow) {
        POINT ptCur = this->GetPos();
        RECT  rc = this->GetRegion();
        float fWidth = (float)(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width - (int)fWidth;
        ptCur.x = CN3Base::s_CameraData.vp.Width - (int)m_fMoveDelta;
        if (ptCur.x <= iXLimit) {
            ptCur.x = iXLimit;
            m_bOpenningNow = false;
        }

        this->SetPos(ptCur.x, ptCur.y);
    } else if (m_bClosingNow) {
        POINT ptCur = this->GetPos();
        RECT  rc = this->GetRegion();
        float fWidth = (float)(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width;
        ptCur.x = CN3Base::s_CameraData.vp.Width - (int)(fWidth - m_fMoveDelta);
        if (ptCur.x >= iXLimit) {
            ptCur.x = iXLimit;
            m_bClosingNow = false;

            this->SetVisibleWithNoSound(false, false, true);
        }

        this->SetPos(ptCur.x, ptCur.y);
    }
    CN3UIBase::Tick();
}

bool CUINotice::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE:
    case DIK_RETURN:
    case DIK_NUMPADENTER:
        if (!m_bOpenningNow) {
            ReceiveMessage(m_pBtnQuit, UIMSG_BUTTON_CLICK);
        }
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}

void CUINotice::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
        m_bOpenningNow = true;
    } else {
        m_bClosingNow = true;
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
    }
    m_fMoveDelta = 0.0f;
}

void CUINotice::RemoveNotice() {
    m_Texts.clear();
    m_TextsEvent.clear();
}
