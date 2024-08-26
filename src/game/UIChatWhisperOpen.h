// UIChat.h: interface for the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include <deque>

struct __WhisperMessage {
    std::string szChat;
    D3DCOLOR    color;

    __WhisperMessage(){};
    __WhisperMessage(const std::string & szChat_Arg, D3DCOLOR color_Arg) {
        szChat = szChat_Arg;
        color = color_Arg;
    }
};

typedef deque<__WhisperMessage *>    ChatListPm;
typedef ChatListPm::iterator         ChatListPmItor;
typedef ChatListPm::reverse_iterator ChatListPmReverseItor;

const int MAX_CHAT_LINES_PM = 100;

class CUIChatWhisperOpen : public CN3UIBase {
  protected:
    ChatListPm m_ChatBuffer;
    ChatListPm m_LineBuffer;
    ChatListPm m_ContinueMsg;
    int        m_iCurContinueMsg;

    CN3UIString *    m_pNoticeTitle;
    CN3UIString *    m_pChatOut;
    CN3UIScrollBar * m_pScrollbar;
    int              m_iChatLineCount;
    RECT             m_rcChatOutRegion;
    CN3UIString **   m_ppUILines;

    CN3UIEdit *   m_pMessage_Text;
    CN3UIString * m_pMessages_List;

    CN3UIEdit * m_pEdit;    //son, chat_in
    std::string m_szString; //son, chat_in

    CN3UIString * m_pText_UserName;
    CN3UIButton * m_pBtn_Close;
    CN3UIButton * m_pBtn_Hide;
    CN3UIBase *   m_pBtn_Chat;

    enum e_ChatMode m_eChatMode;

    bool m_bChatNormal;
    bool m_bChatPrivate;
    bool m_bChatClan;
    bool m_bChatShout;
    bool m_bChatParty;

    bool m_bKillFocus;

  protected:
    void SetTopLine(int iTopLine);
    void AddLineBuffer(const std::string & szString, D3DCOLOR color);
    void RecalcLineBuffers();
    void CreateLines();

    // Operations
  public:
    void         ShowContinueMsg();
    void         DeleteContinueMsg();
    bool         OnKeyPress(int iKey);
    bool         GetEnableKillFocus() { return m_bKillFocus; }
    void         SetEnableKillFocus(bool bKillFocus) { m_bKillFocus = bKillFocus; }
    virtual BOOL MoveOffset(int iOffsetX, int iOffsetY);
    virtual bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual void SetRegion(const RECT & Rect);

    void                SetString(const std::string & szChat);
    void                SetCaretPos(int iPos);
    const std::string & GetString() const { return m_szString; } //son, chat_in
    void                AddChatMsg(const std::string & szString, D3DCOLOR color = 0xffffffff);
    void                AddContinueMsg(e_ChatMode eCM, const std::string & szString, D3DCOLOR color = 0xffffffff);
    void                AdjustScroll();
    void                Open(string userName);
    void                Render();

    BOOL IsChatMode();
    void SetFocus();
    void KillFocus();

    CUIChatWhisperOpen();
    virtual ~CUIChatWhisperOpen();
};
