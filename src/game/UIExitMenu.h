#pragma once

#include "N3Base/N3UIBase.h"

class CUIExitMenu : public CN3UIBase {
  private:
    CN3UIButton * m_pBtn_chr;
    CN3UIButton * m_pBtn_exit;
    CN3UIButton * m_pBtn_option;
    CN3UIButton * m_pBtn_cancel;

  public:
    void SetVisible(bool bVisible) override;
    bool OnKeyPress(int iKey) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    bool Load(HANDLE hFile) override;

    void SelectCharacter();
    void ClearExitState();
    void AddWarningMessage(DWORD idMessage, bool visbleMenu);

    CUIExitMenu();
    ~CUIExitMenu() override;
};
