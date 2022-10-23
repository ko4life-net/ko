#pragma once

#include "N3Base/N3UIBase.h"

class CUIRookieTip : public CN3UIBase {
  public:
    CUIRookieTip();
    ~CUIRookieTip() override;

    void Release() override;
    bool Load(HANDLE hFile) override;
    void Tick() override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    void SetVisible(bool bVisible) override;

  public:
    class CN3UIButton * m_pBtnClose;
    class CN3UIButton * m_pBtnDontDisplayAgain;

    float m_fMoveDelta;
    bool  m_bOpenningNow;
};
