// UINotice.cpp: implementation of the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UINotice.h"
#include "GameProcedure.h"
#include "UIManager.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINotice::CUINotice() {
    m_pText_Notice = NULL;
    m_pTextEvent = NULL;
    m_pBtn_Quit = NULL;
    m_pTextTip = NULL;
    m_pTextEvent_Name = NULL;
    sEventName.clear();
    sTextTip.clear();

    m_bOpenningNow = false;
    m_bClosingNow = false;
    m_fMoveDelta = 0;
}

CUINotice::~CUINotice() {
    m_Texts.clear();
    m_TextsNotices.clear();
}

void CUINotice::Release() {
    m_Texts.clear();
    m_TextsNotices.clear();
    sEventName.clear();
    sTextTip.clear();
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
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pText_Notice = (CN3UIString *)GetChildByID("Text_Notice");
    m_pBtn_Quit = (CN3UIButton *)GetChildByID("btn_quit");
    m_pTextEvent_Name = (CN3UIString *)GetChildByID("text_event_name");
    m_pTextTip = (CN3UIString *)GetChildByID("text_tip");
    m_pTextEvent = (CN3UIString *)GetChildByID("text_event");

    return true;
}

bool CUINotice::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_Quit) {
            if (m_pText_Notice) {
                m_pText_Notice->SetString("");
            }
            Close();
        }
    }

    return true;
}

void CUINotice::GenerateText() {
    if (NULL == m_pText_Notice) {
        return;
    }

    int iTextLen = 0;

    for (auto & itString : m_TextsNotices) {
        iTextLen += itString.size() + 3; // LineFeed, Carriage return
    }

    if (iTextLen <= 0) {
        return;
    }

    std::vector<char> szBuff(iTextLen * 2, 0);
    for (auto & itString : m_TextsNotices) {
        lstrcat(&szBuff[0], itString.c_str());
        lstrcat(&szBuff[0], "\n");
    }

    m_pText_Notice->SetString(&(szBuff[0]));
}

void CUINotice::TipOfTheDay() {
    if (NULL == m_pText_Notice) {
        return;
    }

    int  iTextLen = 0;
    bool tipOfTheDayEnabled = false;

    for (auto & itString : m_Texts) {
        iTextLen += itString.size() + 3; // LineFeed, Carriage return
    }

    if (iTextLen <= 0) {
        tipOfTheDayEnabled = true;
    }

    if (!tipOfTheDayEnabled) {
        std::vector<char> szBuff(iTextLen * 2, 0);

        for (auto & itString : m_Texts) {
            lstrcat(&szBuff[0], itString.c_str());
            lstrcat(&szBuff[0], "\n");
        }

        m_pTextEvent->SetString(&(szBuff[0])); //Set Event text

    } else {

        srand((unsigned)time(NULL));
        int randomTip = 5001 + rand() % 32;

        ::_LoadStringFromResource(5034, sEventName);
        m_pTextEvent_Name->SetString(sEventName);

        ::_LoadStringFromResource(randomTip, sTextTip);
        m_pTextTip->SetString(sTextTip);
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
        ReceiveMessage(m_pBtn_Quit, UIMSG_BUTTON_CLICK);
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}

void CUINotice::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
    }
}

void CUINotice::RemoveNotice() {
    m_Texts.clear();
    m_TextsNotices.clear();
}
