// UINotice.h: interface for the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include <list>
#include <string>

typedef typename std::list<std::string>::iterator it_String;

class CUINotice : public CN3UIBase {
  public:
    class CN3UIString * m_pTextNotice;
    class CN3UIString * m_pTextEvent;
    class CN3UIString * m_pTextEventName;
    class CN3UIString * m_pTextTip;
    class CN3UIButton * m_pBtnQuit;

    float m_fMoveDelta;
    bool  m_bOpenningNow;
    bool  m_bClosingNow;

    std::list<std::string> m_Texts;
    std::list<std::string> m_TextsEvent;

  public:
    void RemoveNotice();
    void SetVisible(bool bVisible);
    bool OnKeyPress(int iKey);
    void GenerateText();
    void Release();
    void Open();
    void Close();
    void Tick();

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUINotice();
    virtual ~CUINotice();
};
