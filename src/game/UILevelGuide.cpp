#include "StdAfx.h"
#include "UILevelGuide.h"

#include "Resource.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"
#include "UIManager.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIEdit.h"

const int MAX_SEARCH_ABOVE_LEVEL = 5;

CUILevelGuide::CUILevelGuide() {
    m_pEdit_Level = NULL;

    m_pBtn_Check = NULL;
    m_pBtn_Cancel = NULL;
    m_pBtn_Up = NULL;
    m_pBtn_Down = NULL;

    m_pText_Page = NULL;
    m_pText_Level = NULL;

    m_iCurrPage = 0;

    for (size_t i = 0; i < MAX_QUESTS_PER_PAGE; ++i) {
        m_pText_Titles[i] = NULL;
        m_pText_Guides[i] = NULL;
        m_pScroll_Guides[i] = NULL;
    }
}

CUILevelGuide::~CUILevelGuide() = default;

bool CUILevelGuide::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtn_Check = (CN3UIButton *)GetChildByID("btn_check");
    __ASSERT(m_pBtn_Check, "NULL UI Component!!");

    m_pBtn_Cancel = (CN3UIButton *)GetChildByID("btn_cancel");
    __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");

    m_pBtn_Up = (CN3UIButton *)GetChildByID("btn_up");
    __ASSERT(m_pBtn_Up, "NULL UI Component!!");

    m_pBtn_Down = (CN3UIButton *)GetChildByID("btn_down");
    __ASSERT(m_pBtn_Down, "NULL UI Component!!");

    m_pEdit_Level = (CN3UIEdit *)GetChildByID("edit_level");
    __ASSERT(m_pEdit_Level, "NULL UI Component!!");

    m_pText_Page = (CN3UIString *)GetChildByID("text_page");
    __ASSERT(m_pText_Page, "NULL UI Component!!");

    m_pText_Level = (CN3UIString *)GetChildByID("text_level");
    __ASSERT(m_pText_Level, "NULL UI Component!!");

    for (size_t i = 0; i < MAX_QUESTS_PER_PAGE; ++i) {
        std::string indexStr = std::to_string(i);

        m_pText_Titles[i] = (CN3UIString *)GetChildByID("text_title" + indexStr);
        __ASSERT(m_pText_Titles[i], "NULL UI Component!!");
        if (m_pText_Titles[i]) {
            m_pText_Titles[i]->SetString("");
        }

        m_pText_Guides[i] = (CN3UIString *)GetChildByID("text_guide" + indexStr);
        __ASSERT(m_pText_Guides[i], "NULL UI Component!!");
        if (m_pText_Guides[i]) {
            m_pText_Guides[i]->SetString("");
        }

        m_pScroll_Guides[i] = (CN3UIScrollBar *)GetChildByID("scroll_guide" + indexStr);
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
            CheckAvailableQuests();
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Cancel) {
            SetVisible(false);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Up) {
            ChangePage(m_iCurrPage + 1);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Down) {
            ChangePage(m_iCurrPage - 1);
            return true;
        }
    } else if (dwMsg == UIMSG_SCROLLBAR_POS) {
        int iIndex = -1;
        for (size_t i = 0; i < MAX_QUESTS_PER_PAGE; ++i) {
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

void CUILevelGuide::SetVisible(bool bVisible) {
    if (m_bVisible == bVisible) {
        return;
    }

    if (bVisible) {
        UpdateQuestsByLevel(CGameBase::s_pPlayer->m_InfoBase.iLevel);
        if (m_pEdit_Level) {
            m_pEdit_Level->SetString("");
            m_pEdit_Level->SetFocus();
        }
    } else {
        if (m_pEdit_Level) {
            m_pEdit_Level->SetString("");
            m_pEdit_Level->KillFocus();
        }
    }

    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisible(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI();
    }
}

void CUILevelGuide::UpdateQuestsByLevel(int iLevel) {
    if (iLevel >= 0 && CGameBase::s_pPlayer->m_InfoBase.iLevel + MAX_SEARCH_ABOVE_LEVEL >= iLevel) {
        m_QuestList.clear();
        e_Class_Represent eRepClass = CGameBase::GetRepresentClass(CGameBase::s_pPlayer->m_InfoBase.eClass);

        size_t iQuestsCount = CGameBase::s_pTbl_Help->GetSize();
        if (iQuestsCount > 0) {
            for (size_t i = 0; i < iQuestsCount; ++i) {
                __TABLE_HELP * pQuest = CGameBase::s_pTbl_Help->Find(i);
                if (!pQuest) {
                    continue;
                }

                if (pQuest->eRepresentClass == CLASS_REPRESENT_GENERIC || pQuest->eRepresentClass == eRepClass) {
                    if (pQuest->iLevelMin == iLevel && pQuest->iLevelMax == iLevel) {
                        m_QuestList.push_front(pQuest);
                    } else if (pQuest->iLevelMin <= iLevel && pQuest->iLevelMax >= iLevel) {
                        m_QuestList.push_back(pQuest);
                    }
                }
            }
        }
    }

    if (m_pText_Level) {
        m_pText_Level->SetStringAsInt(iLevel);
    }
    ChangePage(0);
}

void CUILevelGuide::ChangePage(int iIndex) {
    if (iIndex < 0) {
        return;
    }

    if (m_QuestList.size() <= 0 || iIndex * MAX_QUESTS_PER_PAGE < m_QuestList.size()) {
        for (size_t i = 0; i < MAX_QUESTS_PER_PAGE; ++i) {
            if (m_pText_Titles[i]) {
                m_pText_Titles[i]->SetString("");
            }
            if (m_pText_Guides[i]) {
                m_pText_Guides[i]->SetString("");
            }
            if (m_pScroll_Guides[i]) {
                m_pScroll_Guides[i]->SetVisible(false);
            }
        }
    }

    if (m_QuestList.size() > 0 && iIndex * MAX_QUESTS_PER_PAGE < m_QuestList.size()) {
        auto itr = std::next(m_QuestList.begin(), iIndex * MAX_QUESTS_PER_PAGE);
        for (size_t i = 0; i < MAX_QUESTS_PER_PAGE && itr != m_QuestList.end(); ++i, ++itr) {
            __TABLE_HELP * pQuest = (*itr);
            if (!pQuest) {
                continue;
            }

            if (m_pText_Titles[i]) {
                m_pText_Titles[i]->SetString(pQuest->szTitle);
            }
            if (m_pText_Guides[i]) {
                m_pText_Guides[i]->SetString(pQuest->szDescription);
                if (m_pText_Guides[i]->GetLineCount() > m_pText_Guides[i]->GetStringRealWidthRect()) {
                    if (m_pScroll_Guides[i]) {
                        m_pScroll_Guides[i]->SetVisible(true);
                        int iScroll = m_pText_Guides[i]->GetLineCount() - m_pText_Guides[i]->GetStringRealWidthRect();
                        if (iScroll < 0) {
                            iScroll = 0;
                        }
                        m_pScroll_Guides[i]->SetRange(0, iScroll);
                        m_pScroll_Guides[i]->SetCurrentPos(0);
                    }
                } else if (m_pScroll_Guides[i]) {
                    m_pScroll_Guides[i]->SetVisible(false);
                }
            }
        }
        m_iCurrPage = iIndex;
        if (m_pText_Page) {
            m_pText_Page->SetStringAsInt(iIndex + 1);
        }
    }
}

void CUILevelGuide::CheckAvailableQuests() {
    int iLevelFilter = std::atol(m_pEdit_Level->GetString().c_str());
    if (iLevelFilter <= 0) {
        return;
    }

    if (CGameBase::s_pPlayer->m_InfoBase.iLevel + MAX_SEARCH_ABOVE_LEVEL < iLevelFilter) {
        std::string szMsgFormat;
        ::_LoadStringFromResource(IDS_LEVELGUIDE_SEARCH_WARNING, szMsgFormat);

        char szMsg[70]{};
        sprintf(szMsg, szMsgFormat.c_str(), MAX_SEARCH_ABOVE_LEVEL);
        CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_NOTHING);

        iLevelFilter = CGameBase::s_pPlayer->m_InfoBase.iLevel + MAX_SEARCH_ABOVE_LEVEL;
    }

    UpdateQuestsByLevel(iLevelFilter);
    m_pEdit_Level->SetString("");
    m_pEdit_Level->SetFocus();
}
