#include "StdAfx.h"
#include "UIMsgBoxOkCancel.h"

#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIString.h"

CUIMsgBoxOkCancel::CUIMsgBoxOkCancel() {
    m_pBtnOk = NULL;
    m_pBtnCancel = NULL;
    m_pTextMsg = NULL;
}

CUIMsgBoxOkCancel::~CUIMsgBoxOkCancel() = default;

bool CUIMsgBoxOkCancel::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnOk) {
            if (m_pParentUI) {
                m_pParentUI->CallBackProc(m_iChildID, UI_MSGBOX_OK_MSG);
            }
            SetVisible(false);
        } else if (pSender == (CN3UIBase *)m_pBtnCancel) {
            if (m_pParentUI) {
                m_pParentUI->CallBackProc(m_iChildID, UI_MSGBOX_CANCEL_MSG);
            }
            SetVisible(false);
        }

        return true;
    }

    return true;
}

bool CUIMsgBoxOkCancel::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE:
        ReceiveMessage(m_pBtnCancel, UIMSG_BUTTON_CLICK);
        return true;
    case DIK_NUMPADENTER:
    case DIK_RETURN:
        ReceiveMessage(m_pBtnOk, UIMSG_BUTTON_CLICK);
        return true;
    }

    return false;
}

void CUIMsgBoxOkCancel::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (m_pTextMsg) {
        m_pTextMsg->SetString("");
    }
}

bool CUIMsgBoxOkCancel::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_pBtnOk = (CN3UIButton *)GetChildByID("btn_ok");
    __ASSERT(m_pBtnOk, "NULL UI Component!!!");

    m_pBtnCancel = (CN3UIButton *)GetChildByID("btn_cancel");
    __ASSERT(m_pBtnCancel, "NULL UI Component!!!");

    m_pTextMsg = (CN3UIString *)GetChildByID("text_msg");
    __ASSERT(m_pTextMsg, "NULL UI Component!!!");

    return true;
}

void CUIMsgBoxOkCancel::SetMsg(const std::string & szMsg) {
    if (m_pTextMsg) {
        m_pTextMsg->SetString(szMsg);
    }
}
