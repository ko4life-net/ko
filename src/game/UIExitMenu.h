// UIChat.h: interface for the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "GameDef.h"
#include <deque>

class CUIExitMenu : public CN3UIBase {
  protected:
    CN3UIButton * m_pBtn_chr;
    CN3UIButton * m_pBtn_exit;
    CN3UIButton * m_pBtn_option;
    CN3UIButton * m_pBtn_cancel;

  protected:
    // Operations
  public:
    bool OnKeyPress(int iKey) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    void Release() override;
    bool Load(HANDLE hFile) override;

    void Render() override;
    void SelectCharacter();
    void ClearExitState();
    void AddWarningMessage(DWORD idMessage, bool visbleMenu);

    CUIExitMenu();
    ~CUIExitMenu() override;
};
