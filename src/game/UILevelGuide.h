#pragma once

#include "N3Base/N3UIBase.h"
#include <list>

const int MAX_GUIDE_PAGE = 3;
const int MAX_SEARCH_ABOVE_PLAYER_LEVEL = 5;

class CUILevelGuide : public CN3UIBase {
  public:
    CUILevelGuide();
    virtual ~CUILevelGuide();

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    bool isQuestIsAvailableForPlayer(int questRequiredClass);

    void Open();
    void ChangePage(int iIndex);
    void LoadQuestData(int sLevel = 0);

  public:
    CN3UIEdit *      m_pEdit_Level;
    CN3UIButton *    m_pBtn_Check;
    CN3UIButton *    m_pBtn_Cancel;
    CN3UIButton *    m_pBtn_Up;
    CN3UIButton *    m_pBtn_Down;
    CN3UIString *    m_pText_Titles[MAX_GUIDE_PAGE];
    CN3UIString *    m_pText_Guides[MAX_GUIDE_PAGE];
    CN3UIString *    m_pText_Page;
    CN3UIString *    m_pText_Level;
    CN3UIScrollBar * m_pScroll_Guides[MAX_GUIDE_PAGE];
    int              m_iCurrPage;
    int              m_iMaxPage;
    int              m_Quests[30][MAX_GUIDE_PAGE];
};
