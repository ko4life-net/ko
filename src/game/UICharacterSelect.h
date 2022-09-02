// UICharacterSelect.h: interface for the UICharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "GameProcCharacterSelect.h"

class CGameProcedure;

class CUICharacterSelect : public CN3UIBase {
  protected:
    CN3UIBase * m_pBtnLeft;
    CN3UIBase * m_pBtnRight;
    CN3UIBase * m_pBtnExit;
    CN3UIBase * m_pBtnDelete;

  public:
    DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    bool  OnKeyPress(int iKey);
    CUICharacterSelect();
    virtual ~CUICharacterSelect();

    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual void Tick();
    virtual bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    void DisplayChrInfo(__CharacterSelectInfo * pCSInfo);
    void DontDisplayInfo();
};
