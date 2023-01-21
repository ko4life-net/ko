#pragma once

#include "N3Base/N3UIBase.h"

class CUIExitMenu : public CN3UIBase {
  public:
    CUIExitMenu();
    ~CUIExitMenu() override;

    void SetVisible(bool bVisible) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    bool Load(HANDLE hFile) override;
    void SelectCharacter();

  private:
    CN3UIButton * m_pBtnChr;
    CN3UIButton * m_pBtnExit;
    CN3UIButton * m_pBtnOption;
    CN3UIButton * m_pBtnCancel;
};
