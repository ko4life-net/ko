#pragma once

#include "N3Base/N3UIBase.h"

class CUIExitMenu : public CN3UIBase {
  private:
    CN3UIButton * m_pBtnChr;
    CN3UIButton * m_pBtnExit;
    CN3UIButton * m_pBtnOption;
    CN3UIButton * m_pBtnCancel;

  public:
    void SetVisible(bool bVisible) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    bool Load(HANDLE hFile) override;

    void SelectCharacter();
    void ClearExitState();
    void AddWarningMessage(DWORD idMessage, bool visbleMenu);

    CUIExitMenu();
    ~CUIExitMenu() override;
};
