// UITradeBBSEditDlg.h: interface for the CUITradeBBSEditDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUITradeBBSEditDlg : public CN3UIBase {
  protected:
    class CN3UIEdit *   m_pEditTitle;
    class CN3UIEdit *   m_pEditPrice;
    class CN3UIEdit *   m_pEditExplanation;
    class CN3UIButton * m_pBtn_Ok;
    class CN3UIButton * m_pBtn_Cancel;

  public:
    bool OnKeyPress(int iChar);
    void SetVisible(bool bVisible);
    void ShowWindow(int iID = -1, CN3UIBase * pParent = NULL);
    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    std::string GetTradeTitle();
    std::string GetTradeExplanation();
    int         GetPrice();

    CUITradeBBSEditDlg();
    virtual ~CUITradeBBSEditDlg();
};
