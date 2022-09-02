// UITradeBBSSelector.h: interface for the CUITradeBBSSelector class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"

#define TRADE_BBS_SELL   1
#define TRADE_BBS_BUY    2
#define TRADE_BBS_CANCEL 3

class CUITradeBBSSelector : public CN3UIBase {
  protected:
    CN3UIButton * m_pBtn_BBSSell;
    CN3UIButton * m_pBtn_BBSBuy;
    CN3UIButton * m_pBtn_BBSCancel;

  public:
    void SetVisible(bool bVisible);
    bool OnKeyPress(int iChar);
    void MsgSend_OpenTradeBuyBBS();
    void MsgSend_OpenTradeSellBBS();
    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUITradeBBSSelector();
    virtual ~CUITradeBBSSelector();
};
