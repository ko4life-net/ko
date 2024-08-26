// UIWhisperOpen.cpp: implementation of the CUIWhisperOpen class.
// GuTeK
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIChatWhisperOpen.h"
#include "UIChatWhisperHide.h"
#include "UIChat.h"
#include "PacketDef.h"
#include "GameProcMain.h"
#include "UIManager.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIScrollBar.h"
#include "N3Base/N3UIEdit.h" //son, chat_in
#include "N3Base/N3UIList.h" //son, chat_in

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIChatWhisperOpen::CUIChatWhisperOpen() {
    
	if (m_ppUILines) { delete[] m_ppUILines; m_ppUILines = NULL; }

	ChatListPmItor itor;
    for (itor = m_ChatBuffer.begin(); m_ChatBuffer.end() != itor; ++itor)
	{
        __WhisperMessage * pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
    m_ChatBuffer.clear();

	for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor)
	{
        __WhisperMessage * pChatInfo = (*itor);
		if (pChatInfo) delete pChatInfo;
	}
    m_LineBuffer.clear();


	DeleteContinueMsg();
	
}

CUIChatWhisperOpen::~CUIChatWhisperOpen() {
}

void CUIChatWhisperOpen::Release() {
    CN3UIBase::Release();

    //m_pText_UserName = NULL;
    m_pBtn_Close = NULL;
}

bool CUIChatWhisperOpen::Load(HANDLE hFile) {
    if (false == CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pText_UserName = (CN3UIString *)GetChildByID("exit_id");
    __ASSERT(m_pText_UserName, "NULL UI Component!!");

    m_pBtn_Close = (CN3UIButton *)GetChildByID("btn_close");
    __ASSERT(m_pBtn_Close, "NULL UI Component!!");
    m_pBtn_Hide = (CN3UIButton *)GetChildByID("btn_hide");
    __ASSERT(m_pBtn_Hide, "NULL UI Component!!");
    m_pBtn_Chat = GetChildByID("btn_chat");
    __ASSERT(m_pBtn_Chat, "NULL UI Component!!");

    m_pScrollbar = (CN3UIScrollBar *)GetChildByID("scroll");
    __ASSERT(m_pScrollbar, "NULL UI Component!!");

    m_pMessage_Text = (CN3UIEdit *)GetChildByID("edit_chat");
    __ASSERT(m_pMessage_Text, "NULL UI Component!!");
    m_pMessage_Text->SetMaxString(256);

    m_pMessages_List = (CN3UIString *)GetChildByID("exit_chat");
    __ASSERT(m_pMessages_List, "NULL UI Component!!");

    m_rcChatOutRegion = m_pMessages_List->GetRegion();

    CreateLines();

    return true;
}

void CUIChatWhisperOpen::Open(string userName) {
    m_pText_UserName->SetString(userName);
    m_pMessage_Text->SetMaxString(512);
    SetVisible(true);
    CGameProcedure::s_pProcMain->m_pUIChatDlg->ChangeChattingMode(N3_CHAT_PRIVATE);
    return;
}

bool CUIChatWhisperOpen::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_SCROLLBAR_POS) {
        int iCurLinePos = m_pScrollbar->GetCurrentPos();
        SetTopLine(iCurLinePos);
    } else if (dwMsg == UIMSG_BUTTON_CLICK) {
        POINT chatPosition = GetPos();

        if (pSender->m_szID == "btn_close") {
            SetVisible(false);
            KillFocus();
            CGameProcedure::s_pUIMgr->ReFocusUI();
            CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperTarget = "";
            CGameProcedure::s_pProcMain->m_pUIChatDlg->ChangeChattingMode(N3_CHAT_NORMAL);
            return CGameProcedure::s_pProcMain->m_pUIChatDlg->deleteWhisperWinodws(m_pText_UserName->GetString());

        } else if (pSender->m_szID == "btn_hide") {
           
            SetEnableKillFocus(false);
            KillFocus();
            CGameProcedure::s_pProcMain->m_pUIChatDlg->KillFocus();
            CGameProcedure::s_pProcMain->m_pUIChatDlg->ChangeChattingMode(N3_CHAT_NORMAL);

            CGameProcedure::s_pUIMgr->ReFocusUI();
            SetVisible(false);
            CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperTarget = "";
          
            auto it = CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperWindows.find(m_pText_UserName->GetString());
            if (it != CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperWindows.end()) {
                CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperWindows[m_pText_UserName->GetString()]
                    .messageNotRead = false;

            }

            CGameProcedure::s_pProcMain->WhisperHide(m_pText_UserName->GetString(), chatPosition);
        }

    } else if (dwMsg == UI_MOUSE_LBCLICK && (pSender->m_szID == "btn_bar" || pSender->m_szID == "exit_id")) {
        SetState(UI_STATE_COMMON_MOVE);
    } else if (dwMsg == UI_MOUSE_LBCLICK && pSender->m_szID == "edit_chat"){

        CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperTarget = m_pText_UserName->GetString();
        

    } else if (dwMsg == UIMSG_EDIT_RETURN) {
        CN3UIEdit * m_pBtn_message = (CN3UIEdit *)pSender;

        m_szString = m_pBtn_message->GetString();
        int iStrLen = m_szString.size();
        if (iStrLen > 0) {
            CGameProcedure::s_pProcMain->MsgSend_ChatSelectTarget(m_pText_UserName->GetString());
            CGameProcedure::s_pProcMain->m_pUIChatDlg->whisperTarget = m_pText_UserName->GetString();
            CGameProcedure::s_pProcMain->MsgSend_Chat(N3_CHAT_PRIVATE, m_szString.c_str());

            m_pBtn_message->SetString("");
        }

        SetEnableKillFocus(true);
    }

    return true;
}

void CUIChatWhisperOpen::AddLineBuffer(const std::string & szString, D3DCOLOR color) {
    if (szString.empty()) {
        return;
    }

    const int iStrLen = szString.size();
    SIZE size;
    if (FALSE == m_pMessages_List->GetTextExtent(szString, iStrLen, &size)) {
        __ASSERT(0, "no device context");
        return;
    }

    const int iRegionWidth = m_rcChatOutRegion.right - m_rcChatOutRegion.left;

    int iCX = 0;
    int iCount = 0;
    int iLineStart = 0;

    while (iCount < iStrLen) {
        if ('\n' == szString[iCount]) // \n
        {
            __WhisperMessage * pLineInfo = new __WhisperMessage;
            m_LineBuffer.push_back(pLineInfo);

            pLineInfo->color = color;
            if ((iCount - iLineStart) > 0) {
                int         iLineLength = iCount - iLineStart + 1;
                std::string szLine;
                pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
            }

            ++iCount;
            iLineStart = iCount;
            iCX = 0;
        } else {
            int iCC = 0;
            if (0x80 & szString[iCount]) {
                iCC = 2; // 2BYTE 
            } else {
                iCC = 1; // 1BYTE
            }

            BOOL bFlag = m_pMessages_List->GetTextExtent(&(szString[iCount]), iCC, &size);
            __ASSERT(bFlag, "cannot get size of dfont");
            if ((iCX + size.cx) > iRegionWidth) 
            {
                int iLineLength = iCount - iLineStart;
                if (iLineLength > 0) {
                    __WhisperMessage * pLineInfo = new __WhisperMessage;
                    m_LineBuffer.push_back(pLineInfo);

                    pLineInfo->color = color;
                    pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
                } else {
                    __ASSERT(iRegionWidth > 15, "The region is too narrow to print even a single character");
                    break;
                }
                iLineStart = iCount;
                iCX = 0;
            }
            iCount += iCC;
            iCX += size.cx;
        }
    }

    int iLineLength = iStrLen - iLineStart;
    if (iLineLength > 0) {
        __WhisperMessage * pLineInfo = new __WhisperMessage;
        m_LineBuffer.push_back(pLineInfo);

        pLineInfo->color = color;
        pLineInfo->szChat = szString.substr(iLineStart, iLineLength);
    }
}
void CUIChatWhisperOpen::AddChatMsg(const std::string & szString, D3DCOLOR color) {
    if (szString.empty()) {
        return;
    }
    __WhisperMessage * pChatInfo = new __WhisperMessage(szString, color);
    m_ChatBuffer.push_back(pChatInfo);

    if (m_ChatBuffer.size() > 255) {
        __WhisperMessage * pTemp = m_ChatBuffer.front();
        if (pTemp) {
            delete pTemp;
        }

        m_ChatBuffer.pop_front();
    }

    this->AddLineBuffer(szString, color);
    this->AdjustScroll();
}

void CUIChatWhisperOpen::CreateLines() {
    if (m_ppUILines) {
        for (int i = 0; i < m_iChatLineCount; ++i) {
            if (m_ppUILines[i]) {
                delete m_ppUILines[i];
                m_ppUILines[i] = NULL;
            }
        }
        delete[] m_ppUILines;
        m_ppUILines = NULL;
    }
    SIZE size;
    if (m_pMessages_List && m_pMessages_List->GetTextExtent("M", lstrlen("M"), &size) && size.cy > 0) { // The letter 'M' is used for measuring text width because it's typically the widest character, providing a better estimate of the maximum text width.
        m_iChatLineCount = (m_rcChatOutRegion.bottom - m_rcChatOutRegion.top) / size.cy;
    } else {
        return;
    }
    const std::string & szFontName = m_pMessages_List->GetFontName();
    DWORD               dwFontHeight = m_pMessages_List->GetFontHeight();
    DWORD               dwFontFlag = m_pMessages_List->GetFontFlags();

    if (m_iChatLineCount <= 0 || szFontName.empty()) {
        return;
    }

    m_ppUILines = new CN3UIString *[m_iChatLineCount];
    for (int i = 0; i < m_iChatLineCount; ++i) {
        RECT rc;
        SetRect(&rc, m_rcChatOutRegion.left, m_rcChatOutRegion.top + (i * size.cy), m_rcChatOutRegion.right,
                m_rcChatOutRegion.top + ((i + 1) * size.cy));
        m_ppUILines[i] = new CN3UIString();
        m_ppUILines[i]->Init(this);
        m_ppUILines[i]->SetRegion(rc);
        m_ppUILines[i]->SetStyle(UISTYLE_STRING_SINGLELINE | UISTYLE_STRING_ALIGNLEFT | UISTYLE_STRING_ALIGNTOP);
        m_ppUILines[i]->SetFont(szFontName, dwFontHeight, dwFontFlag & D3DFONT_BOLD, dwFontFlag & D3DFONT_ITALIC);
    }
}

void CUIChatWhisperOpen::AdjustScroll() {
    int  iCurLinePos = m_pScrollbar->GetCurrentPos(); 
    BOOL bAutoScroll = (m_pScrollbar->GetMaxPos() == iCurLinePos) ? TRUE : FALSE;

    while (
        m_LineBuffer.size() > MAX_CHAT_LINES_PM &&
        0 < iCurLinePos) 
    {
        __WhisperMessage * pTemp = m_LineBuffer.front();
        if (pTemp) {
            delete pTemp;
        }
        m_LineBuffer.pop_front();
        --iCurLinePos;
    }

    int iLineBufferSize = m_LineBuffer.size();
    int iMaxScrollPos = iLineBufferSize - m_iChatLineCount;
    if (iMaxScrollPos < 0) {
        iMaxScrollPos = 0;
    }
    m_pScrollbar->SetRange(0, iMaxScrollPos);

    if (bAutoScroll) {
        iCurLinePos = iMaxScrollPos;
    }
    if (iCurLinePos < 0) {
        iCurLinePos = 0;
    }

    m_pScrollbar->SetCurrentPos(iCurLinePos);

    SetTopLine(iCurLinePos);
}

void CUIChatWhisperOpen::Render() {
    if (m_pMessage_Text->HaveFocus()) {
        m_pBtn_Chat->SetState(UI_STATE_BUTTON_DOWN);
    } else {
        m_pBtn_Chat->SetState(UI_STATE_BUTTON_NORMAL);
    }

    if (!m_bVisible) {
        return;
    }

    CN3UIBase::Render();
}

void CUIChatWhisperOpen::SetTopLine(int iTopLine) {
    if (m_iChatLineCount <= 0) {
        return;
    }

    const int iLineBufferSize = m_LineBuffer.size();
    if (iTopLine < 0) {
        iTopLine = 0;
    } else if (iTopLine > iLineBufferSize) {
        iTopLine = iLineBufferSize;
    }

    __WhisperMessage ** ppLineInfos = new __WhisperMessage *[m_iChatLineCount];
    ZeroMemory(ppLineInfos, sizeof(__WhisperMessage *) * m_iChatLineCount);

    int i, iCurLine = 0;
    for (i = 0; i < m_iChatLineCount; ++i) {
        iCurLine = iTopLine + i;
        if (iLineBufferSize <= iCurLine) {
            break;
        }
        ppLineInfos[i] = m_LineBuffer[iCurLine];
    }

    __ASSERT(m_ppUILines, "null pointer");
    int iRealLine = i; 
    int iRealLineCount = 0;
    for (int i = 0; i < iRealLine; ++i) {
        ++iRealLineCount;
        if (NULL == m_ppUILines[i]) {
            continue;
        }
        m_ppUILines[i]->SetColor(ppLineInfos[i]->color);
        m_ppUILines[i]->SetString(ppLineInfos[i]->szChat);
    }
    for (int i = iRealLineCount; i < m_iChatLineCount; ++i) {
        if (NULL == m_ppUILines[i]) {
            continue;
        }
        m_ppUILines[i]->SetString("");
    }
    delete[] ppLineInfos;
}

void CUIChatWhisperOpen::RecalcLineBuffers() {
    int iMaxScrollPos = 0;
    ChatListPmItor itor;
    for (itor = m_LineBuffer.begin(); m_LineBuffer.end() != itor; ++itor) {
        __WhisperMessage * pLineBuff = (*itor);
        if (pLineBuff) {
            delete pLineBuff;
        }
    }

    m_LineBuffer.clear();

    for (itor = m_ChatBuffer.begin(); m_ChatBuffer.end() != itor; ++itor) {
        __WhisperMessage * pChatBuff = (*itor);
        if (pChatBuff) {
            AddLineBuffer(pChatBuff->szChat, pChatBuff->color);
        }
    }

    while (m_LineBuffer.size() > MAX_CHAT_LINES_PM)
    {
        __WhisperMessage * pLineBuff = m_ChatBuffer.front();
        if (pLineBuff) {
            delete pLineBuff;
        }

        m_LineBuffer.pop_front();
    }

    int iLineBufferSize = m_LineBuffer.size();
    iMaxScrollPos = iLineBufferSize - m_iChatLineCount;

    if (iMaxScrollPos < 0) {
        iMaxScrollPos = 0;
    }
    m_pScrollbar->SetRange(0, iMaxScrollPos); 
    m_pScrollbar->SetCurrentPos(iMaxScrollPos);

    SetTopLine(iMaxScrollPos);
}

//son, chat_in
void CUIChatWhisperOpen::SetFocus() {
    SetEnableKillFocus(false);
    this->SetString("");
    if (m_pMessage_Text) {
        m_pMessage_Text->SetFocus();
    }
}

void CUIChatWhisperOpen::KillFocus() {
    this->SetString("");
    if (m_pMessage_Text) {
        m_pMessage_Text->KillFocus();
    }
}

BOOL CUIChatWhisperOpen::IsChatMode() {
    return ((m_pMessage_Text && GetFocusedEdit() == m_pMessage_Text) ? TRUE : FALSE);
}
//son, chat_in

void CUIChatWhisperOpen::SetString(const std::string & szChat) {
    m_szString = szChat;
    if (m_pMessage_Text) {
        m_pMessage_Text->SetString(m_szString);
        m_pMessage_Text->SetCaretPos(m_szString.size());
    }
}

void CUIChatWhisperOpen::SetCaretPos(int iPos) {
    if (IsChatMode() && m_pMessage_Text) {
        m_pMessage_Text->SetCaretPos(iPos);
    }
}

BOOL CUIChatWhisperOpen::MoveOffset(int iOffsetX, int iOffsetY) {
    if (0 == iOffsetX && 0 == iOffsetY) {
        return FALSE;
    }
    m_rcRegion.left += iOffsetX;
    m_rcRegion.top += iOffsetY;
    m_rcRegion.right += iOffsetX;
    m_rcRegion.bottom += iOffsetY;

    m_rcMovable.left += iOffsetX;
    m_rcMovable.top += iOffsetY;
    m_rcMovable.right += iOffsetX;
    m_rcMovable.bottom += iOffsetY;

    CN3UIBase * pCUI = NULL;
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        pCUI = (*itor);
        __ASSERT(pCUI, "child UI pointer is NULL!");
        pCUI->MoveOffset(iOffsetX, iOffsetY);
    }


    return TRUE;
}

void CUIChatWhisperOpen::SetRegion(const RECT & Rect) {
    CN3UIBase::SetRegion(Rect);
}

void CUIChatWhisperOpen::DeleteContinueMsg() {
    m_iCurContinueMsg = 0;

    ChatListPmItor itor;
    for (itor = m_ContinueMsg.begin(); m_ContinueMsg.end() != itor; ++itor) {
        __WhisperMessage * pChatInfo = (*itor);
        if (pChatInfo) {
            delete pChatInfo;
        }
    }
    m_ContinueMsg.clear();
}

void CUIChatWhisperOpen::ShowContinueMsg() {
    int iSize = m_ContinueMsg.size();
    if (m_iCurContinueMsg > iSize) {
        m_iCurContinueMsg = 0;
    }

    int           iCnt = 0;
    ChatListPmItor itor;
    for (itor = m_ContinueMsg.begin(); m_ContinueMsg.end() != itor; ++itor) {
        if (iCnt == m_iCurContinueMsg) {
            m_iCurContinueMsg++;
            __WhisperMessage * pChatInfo = (*itor);
            if (pChatInfo) {
                AddChatMsg(pChatInfo->szChat, pChatInfo->color);
            }
        }
    }
}

bool CUIChatWhisperOpen::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE: {
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
        CN3UIBase * pFocus = CGameProcedure::s_pUIMgr->GetFocusedUI();
        if (pFocus && pFocus != this) {
            pFocus->OnKeyPress(iKey);
        }
    }
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}

