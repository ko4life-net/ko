#pragma once

#include "N3Base/N3UIBase.h"
#include <list>

const int MAX_GUIDE_PAGE = 3;

class CUILevelGuide : public CN3UIBase {
  public:
    CUILevelGuide();
    virtual ~CUILevelGuide();

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    void Check();
    void MovePage(int iIndex);

  public:
    CN3UIEdit *            m_pEdit_Level;
    CN3UIButton *          m_pBtn_Check;
    CN3UIButton *          m_pBtn_Cancel;
    CN3UIButton *          m_pBtn_Up;
    CN3UIButton *          m_pBtn_Down;
    CN3UIString *          m_pText_Titles[MAX_GUIDE_PAGE];
    CN3UIString *          m_pText_Guides[MAX_GUIDE_PAGE];
    CN3UIString *          m_pText_Page;
    CN3UIString *          m_pText_Level;
    CN3UIScrollBar *       m_pScroll_Guides[MAX_GUIDE_PAGE];
    std::list<std::string> m_QuestList;
    int                    m_iCurrPage;
};
