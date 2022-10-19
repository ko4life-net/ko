#include "StdAfx.h"

#include "UILevelGuide.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIEdit.h"
#include "PlayerMySelf.h"
#include "Resource.h"
#include "GameProcMain.h"

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
}

void CUILevelGuide::LoadQuestData(int sLevel) {
    m_iCurrPage = 1;

    int lvlMin = 0;
    int pLevel = CGameBase::s_pPlayer->m_InfoBase.iLevel;

    m_pText_Level->SetStringAsInt(pLevel);
    m_pText_Page->SetStringAsInt(1);

    if (sLevel - pLevel > MAX_SEARCH_ABOVE_PLAYER_LEVEL) {
        m_pText_Level->SetStringAsInt(pLevel + MAX_SEARCH_ABOVE_PLAYER_LEVEL);

        char searchText[70];

        std::string szMsg;
        ::_LoadStringFromResource(IDS_QUEST_MENU_SEARCH_LEVEL_MAX_INFO, szMsg);
        sprintf(searchText, szMsg.c_str(), MAX_SEARCH_ABOVE_PLAYER_LEVEL);

        CGameProcedure::MessageBoxPost(searchText, "", MB_OK, BEHAVIOR_NOTHING);
    }

    int page = 1;
    int questIndex = 0;

    for (int i = 1; i < CGameProcedure::s_pProcMain->s_pTbl_Help->GetSize(); i++) {
        __TABLE_HELP * selectedQuest = CGameProcedure::s_pProcMain->s_pTbl_Help->Find(i);

        if (sLevel >= selectedQuest->pLevelMin && isQuestIsAvailableForPlayer(selectedQuest->pClass)) {

            m_Quests[page][questIndex] = i;

            if (i % 3 == 0) {
                page++;
                questIndex = 0;
            } else {
                questIndex++;
            }
        }
    }

    m_iMaxPage = page;

    for (int i = 0; i < 3; i++) {
        __TABLE_HELP * questObj = CGameProcedure::s_pProcMain->s_pTbl_Help->Find(m_Quests[1][i]);

        m_pText_Titles[i]->SetString(questObj->qTitle);
        m_pText_Guides[i]->SetString(questObj->qDesc);

        int maxLineCount = m_pText_Guides[i]->GetLineCount();

        m_pScroll_Guides[i]->SetRange(0, maxLineCount);
        m_pScroll_Guides[i]->SetCurrentPos(0);
    }
}

bool CUILevelGuide::isQuestIsAvailableForPlayer(int questRequiredClass) {

    int pClass = CGameBase::s_pPlayer->m_InfoBase.eClass;

    if (questRequiredClass == 100) { // for all
        return true;
    }

    if (questRequiredClass == 0 &&
        (pClass == CLASS_KA_WARRIOR || pClass == CLASS_KA_BERSERKER || pClass == CLASS_KA_GUARDIAN ||
         pClass == CLASS_EL_WARRIOR || pClass == CLASS_EL_BLADE || pClass == CLASS_EL_PROTECTOR)) { // warrior
        return true;
    }

    if (questRequiredClass == 1 &&
        (pClass == CLASS_KA_ROGUE || pClass == CLASS_KA_HUNTER || pClass == CLASS_KA_PENETRATOR ||
         pClass == CLASS_EL_ROGUE || pClass == CLASS_EL_RANGER || pClass == CLASS_EL_ASSASIN)) { // rouge
        return true;
    }

    if (questRequiredClass == 2 &&
        (pClass == CLASS_KA_WIZARD || pClass == CLASS_KA_SORCERER || pClass == CLASS_KA_NECROMANCER ||
         pClass == CLASS_EL_WIZARD || pClass == CLASS_EL_MAGE || pClass == CLASS_EL_ENCHANTER)) { // mage
        return true;
    }

    if (questRequiredClass == 3 &&
        (pClass == CLASS_KA_PRIEST || pClass == CLASS_KA_SHAMAN || pClass == CLASS_KA_DARKPRIEST ||
         pClass == CLASS_EL_PRIEST || pClass == CLASS_EL_CLERIC || pClass == CLASS_EL_DRUID)) { // priest
        return true;
    }

    return false;
}

void CUILevelGuide::Open() {
    m_iCurrPage = 1;
    SetVisible(true);

    LoadQuestData(CGameBase::s_pPlayer->m_InfoBase.iLevel);

    return;
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
        m_pScroll_Guides[i]->SetRange(0, 0);
    }

    if (m_pText_Page) {
        m_pText_Page->SetStringAsInt(m_iCurrPage + 1);
    }

    return true;
}

void CUILevelGuide::ChangePage(int iIndex) {
    __TABLE_HELP * questObj = NULL;

    m_pText_Page->SetStringAsInt(iIndex);

    for (int i = 0; i < 3; i++) {
        questObj = CGameProcedure::s_pProcMain->s_pTbl_Help->Find(m_Quests[iIndex][i]);
        m_pText_Titles[i]->SetString(questObj->qTitle);
        m_pText_Guides[i]->SetString(questObj->qDesc);

        int maxLineCount = m_pText_Guides[i]->GetLineCount();

        m_pScroll_Guides[i]->SetRange(0, maxLineCount);
        m_pScroll_Guides[i]->SetCurrentPos(0);
    }
}

bool CUILevelGuide::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtn_Check) {
            int checkLevel = std::stoi(m_pEdit_Level->GetString());
            LoadQuestData(checkLevel);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Cancel) {
            SetVisible(false);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Up) {

            if (m_iCurrPage >= m_iMaxPage) {
                return true;
            }
            ChangePage(++m_iCurrPage);

            return true;
        } else if (pSender == (CN3UIBase *)m_pBtn_Down) {

            if (m_iCurrPage <= 1) {
                return true;
            }
            ChangePage(--m_iCurrPage);

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

        if (m_pText_Guides[iIndex]->GetString().length() > 0) {
            m_pText_Guides[iIndex]->SetStartLine(m_pScroll_Guides[iIndex]->GetCurrentPos());
        }
    }

    return true;
}
