#include "StdAfx.h"

#include "UILevelGuide.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"

CUILevelGuide::CUILevelGuide() {
    for (size_t i = 0; i < MAX_GUIDE_PAGE; ++i) {
        m_pText_Titles[i] = NULL;
        m_pText_Guides[i] = NULL;
        m_pScroll_Guides[i] = NULL;
    }
    m_pBtn_Check = NULL;
    m_pEdit_Level = NULL;
    m_pText_Page = NULL;
    m_pText_Level = NULL;

    m_iCurrPage = 0;
}

CUILevelGuide::~CUILevelGuide() {
    m_QuestList.clear();
}

bool CUILevelGuide::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtn_Check = (CN3UIButton *)(this->GetChildByID("btn_check"));
    __ASSERT(m_pBtn_Check, "NULL UI Component!!");

    m_pBtn_Cancel = (CN3UIButton *)(this->GetChildByID("btn_cancel"));
    __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");

    m_pBtn_Up = (CN3UIButton *)(this->GetChildByID("btn_up"));
    __ASSERT(m_pBtn_Up, "NULL UI Component!!");

    m_pBtn_Down = (CN3UIButton *)(this->GetChildByID("btn_down"));
    __ASSERT(m_pBtn_Down, "NULL UI Component!!");

    m_pEdit_Level = (CN3UIEdit *)(this->GetChildByID("edit_level"));
    __ASSERT(m_pEdit_Level, "NULL UI Component!!");

    m_pText_Page = (CN3UIString *)(this->GetChildByID("text_page"));
    __ASSERT(m_pText_Page, "NULL UI Component!!");

    m_pText_Level = (CN3UIString *)(this->GetChildByID("text_level"));
    __ASSERT(m_pText_Level, "NULL UI Component!!");

    for (size_t i = 0; i < MAX_GUIDE_PAGE; ++i) {
        std::string indexStr = std::to_string(i);

        m_pText_Titles[i] = (CN3UIString *)(this->GetChildByID("text_title" + indexStr));
        __ASSERT(m_pText_Titles[i], "NULL UI Component!!");
        if (m_pText_Titles[i]) {
            m_pText_Titles[i]->SetString("");
        }

        m_pText_Guides[i] = (CN3UIString *)(this->GetChildByID("text_guide" + indexStr));
        __ASSERT(m_pText_Guides[i], "NULL UI Component!!");
        if (m_pText_Guides[i]) {
            m_pText_Guides[i]->SetString("");
        }

        m_pScroll_Guides[i] = (CN3UIScrollBar *)(this->GetChildByID("scroll_guide" + indexStr));
        __ASSERT(m_pScroll_Guides[i], "NULL UI Component!!");
        if (m_pScroll_Guides[i]) {
            m_pScroll_Guides[i]->SetVisible(false);
        }
    }

    if (m_pText_Page) {
        m_pText_Page->SetStringAsInt(m_iCurrPage + 1);
    }

    return true;
}

bool CUILevelGuide::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtn_Check) {
            Check();
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Cancel) {
            SetVisible(false);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Up) {
            MovePage(m_iCurrPage + 1);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Down) {
            MovePage(m_iCurrPage - 1);
            return true;
        }
    } else if (dwMsg == UIMSG_SCROLLBAR_POS) {
        int iIndex = -1;
        for (size_t i = 0; i < MAX_GUIDE_PAGE; ++i) {
            if (pSender == (CN3UIBase *)m_pScroll_Guides[i]) {
                iIndex = i;
                break;
            }
        }
        if (iIndex == -1 || !m_pScroll_Guides[iIndex] || !m_pText_Guides[iIndex]) {
            return true;
        }

        m_pText_Guides[iIndex]->SetStartLine(m_pScroll_Guides[iIndex]->GetCurrentPos());
    }

    return true;
}

void CUILevelGuide::Check() {
    N3_WARN("[CUILevelGuide::Check] TODO: needs implementing.");
}

void CUILevelGuide::MovePage(int iIndex) {
    N3_WARN("[CUILevelGuide::MovePage] TODO: needs implementing.");
}
