#include "StdAfx.h"
#include "UIRookieTip.h"
#include "GameProcedure.h"

#include "N3Base/N3UIButton.h"

CUIRookieTip::CUIRookieTip() {
    m_pBtnClose = NULL;
    m_pBtnDontDisplayAgain = NULL;

    m_fMoveDelta = 0.0f;
    m_bOpenningNow = false;
}

CUIRookieTip::~CUIRookieTip() = default;

void CUIRookieTip::Release() {
    CN3UIBase::Release();
    m_pBtnClose = NULL;
    m_pBtnDontDisplayAgain = NULL;
}

bool CUIRookieTip::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtnClose = (CN3UIButton *)GetChildByID("btn_close");
    __ASSERT(m_pBtnClose, "NULL UI Component!!");

    m_pBtnDontDisplayAgain = (CN3UIButton *)GetChildByID("btn_dontdisplayagain");
    __ASSERT(m_pBtnDontDisplayAgain, "NULL UI Component!!");

    if (m_pBtnDontDisplayAgain) {
        __WndInfo WI{};
        if (CGameProcedure::RegGetSetting(UI_POST_WND_ROOKIETIP, &WI, sizeof(__WndInfo)) && !WI.bVisible) {
            m_pBtnDontDisplayAgain->SetState(UI_STATE_BUTTON_DOWN);
        }
    }

    return true;
}

bool CUIRookieTip::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnClose) {
            SetVisible(false);
        } else if (pSender == (CN3UIBase *)m_pBtnDontDisplayAgain) {
            __WndInfo WI{};
            sprintf(WI.szName, UI_POST_WND_ROOKIETIP);
            WI.ptPosition = GetPos();
            WI.bVisible = m_pBtnDontDisplayAgain->GetState() != UI_STATE_BUTTON_DOWN;
            CGameProcedure::RegPutSetting(WI.szName, &WI, sizeof(__WndInfo));
        }
    }

    return true;
}

void CUIRookieTip::Tick() {
    if (m_bOpenningNow) {
        POINT ptCur = GetPos();
        RECT  rc = GetRegion();
        float fWidth = (float)(rc.right - rc.left);

        float fDelta = 2500.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 30.0f) {
            fDelta = 30.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = (CN3Base::s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
        ptCur.x = (int)(m_fMoveDelta - fWidth);
        if (ptCur.x >= iXLimit) {
            ptCur.x = iXLimit;
            m_bOpenningNow = false;
            m_fMoveDelta = 0.0f;
        }

        SetPos(ptCur.x, ptCur.y);
    }

    CN3UIBase::Tick();
}

void CUIRookieTip::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        // Set the position hidden to the left side, so that when Tick starts,
        // it will do a transition to the right towards the center.
        RECT rc = GetRegion();
        int  iX = -(rc.right - rc.left);
        int  iY = (CN3Base::s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
        SetPos(iX, iY);
        m_bOpenningNow = true;
        m_fMoveDelta = 0.0f;
    }
}
