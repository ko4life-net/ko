// UIChatWhisperHide.h: interface for the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include <deque>

class CUIChatWhisperHide : public CN3UIBase {
  public:
    static class CGameProcCharacterSelect * s_pProcCharacterSelect;

  protected:
    int              m_iCurContinueMsg;
    CN3UIString *    m_pNoticeTitle;
    CN3UIString *    m_pChatOut;
    CN3UIScrollBar * m_pScrollbar;
    int              m_iChatLineCount;
    RECT             m_rcChatOutRegion;
    CN3UIString **   m_ppUILines;

    CN3UIEdit * m_pEdit;    //son, chat_in
    std::string m_szString; //son, chat_in

    CN3UIString * m_pText_UserName;
    CN3UIButton * m_pBtn_close;
    CN3UIButton * m_pBtn_open;
    CN3UIButton * m_pBtn_bar;

    bool m_bKillFocus;

    // Operations
  public:
    void         ShowContinueMsg();
    void         DeleteContinueMsg();
    bool         OnKeyPress(int iKey);
    bool         GetEnableKillFocus() { return m_bKillFocus; }
    void         SetEnableKillFocus(bool bKillFocus) { m_bKillFocus = bKillFocus; }
    void         ChatListenEnable();
    void         SetNewMessageAnimation(bool animationActivated = true);
    virtual BOOL MoveOffset(int iOffsetX, int iOffsetY);
    virtual bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual void SetRegion(const RECT & Rect);

    void                SetString(const std::string & szChat);
    void                SetCaretPos(int iPos);
    const std::string & GetString() const { return m_szString; }
    void                Open(string userName);
    void                Render();

    bool m_messageNotRead;

    BOOL IsChatMode();
    void SetFocus();
    void KillFocus();

    CUIChatWhisperHide();
    virtual ~CUIChatWhisperHide();
};
