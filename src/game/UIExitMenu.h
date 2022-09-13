// UIChat.h: interface for the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
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
    bool         OnKeyPress(int iKey);
    virtual bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    virtual void Release();
    virtual bool Load(HANDLE hFile);

    void Open(string userName);
    void Render();

    CUIExitMenu();
    virtual ~CUIExitMenu();
};
