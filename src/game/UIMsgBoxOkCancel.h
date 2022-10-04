#pragma once

#include "N3Base/N3UIBase.h"

#define UI_MSGBOX_OK_MSG     1
#define UI_MSGBOX_CANCEL_MSG 2

class CUIMsgBoxOkCancel : public CN3UIBase {
  public:
    CUIMsgBoxOkCancel();
    ~CUIMsgBoxOkCancel() override;

    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    bool OnKeyPress(int iKey) override;
    void SetVisible(bool bVisible) override;
    bool Load(HANDLE hFile) override;
    void SetMsg(const std::string & szMsg);

  public:
    CN3UIButton * m_pBtnOk;
    CN3UIButton * m_pBtnCancel;
    CN3UIString * m_pTextMsg;
};
