#pragma once

#include "N3Base/N3UIBase.h"
#include <list>

const int MAX_QUESTS_PER_PAGE = 3;

class CUILevelGuide : public CN3UIBase {
  public:
    CUILevelGuide();
    ~CUILevelGuide() override;

    bool Load(HANDLE hFile) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    void SetVisible(bool bVisible) override;

    void UpdateQuestsByLevel(int iLevel);
    void ChangePage(int iIndex);
    void CheckAvailableQuests();

  public:
    CN3UIEdit * m_pEdit_Level;

    CN3UIButton * m_pBtn_Check;
    CN3UIButton * m_pBtn_Cancel;
    CN3UIButton * m_pBtn_Up;
    CN3UIButton * m_pBtn_Down;

    CN3UIString * m_pText_Titles[MAX_QUESTS_PER_PAGE];
    CN3UIString * m_pText_Guides[MAX_QUESTS_PER_PAGE];
    CN3UIString * m_pText_Page;
    CN3UIString * m_pText_Level;

    CN3UIScrollBar * m_pScroll_Guides[MAX_QUESTS_PER_PAGE];

    std::list<struct __TABLE_HELP *> m_QuestList;

    int m_iCurrPage;
};
