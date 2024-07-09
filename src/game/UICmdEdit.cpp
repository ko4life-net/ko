// UICmdEditDlg.cpp: implementation of the CUICmdEditDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h";
#include "resource.h"
#include "UICmdEdit.h"

#include "GameProcMain.h"
#include "GameProcedure.h"
#include "PacketDef.h"
#include "APISocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUICmdEdit::CUICmdEdit() {}

CUICmdEdit::~CUICmdEdit() {}

bool CUICmdEdit::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pText_Title = (CN3UIString *)GetChildByID("Text_cmd");
    __ASSERT(m_pText_Title, "NULL UI Component!!");
    m_pBtn_Ok = (CN3UIButton *)GetChildByID("btn_ok");
    __ASSERT(m_pBtn_Ok, "NULL UI Component!!");
    m_pBtn_Cancel = (CN3UIButton *)GetChildByID("btn_cancel");
    __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
    m_pEdit_Box = (CN3UIEdit *)GetChildByID("edit_cmd");
    __ASSERT(m_pEdit_Box, "NULL UI Component!!");
    return true;
}

bool CUICmdEdit::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender->m_szID == "btn_ok") {
            m_szArg1 = m_pEdit_Box->GetString();
            std::string tempCmdStr = m_pText_Title->GetString() + " " + m_szArg1;
            CGameProcedure::s_pProcMain->ParseChattingCommand(tempCmdStr);

            SetVisible(false);
            return true;
        }

        if (pSender->m_szID == "btn_cancel") {
            SetVisible(false);
            return true;
        }
    }
    return true;
}

void CUICmdEdit::Open(std::string msg) {
    if (!msg.empty()) {
        m_pText_Title->SetString("/"+ msg);
        m_pEdit_Box->SetString("");
    }

    m_pEdit_Box->SetFocus();
    SetVisible(true);
}

void CUICmdEdit::SetVisible(bool bVisible) {
    if (bVisible == this->IsVisible()) {
        return;
    }

    if (!bVisible) {
        m_pEdit_Box->KillFocus();
    }

    CN3UIBase::SetVisible(bVisible);
}
