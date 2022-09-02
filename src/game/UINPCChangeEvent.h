// UINPCChangeEvent.h: interface for the CUINPCChangeEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "UIPointInitDlg.h"

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"

//////////////////////////////////////////////////////////////////////

class CUINPCChangeEvent : public CN3UIBase {
    CN3UIButton * m_pBtn_Change;
    CN3UIButton * m_pBtn_Repoint0;
    CN3UIButton * m_pBtn_Repoint1;
    CN3UIButton * m_pBtn_Close;

    CUIPointInitDlg * m_pDlg;

    bool m_bSendedAllPoint;

  public:
    bool OnKeyPress(int iKey);
    void SetVisible(bool bVisible);
    void Release();

    CUINPCChangeEvent();
    virtual ~CUINPCChangeEvent();

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    void Open();
    void Close();

    void ClassChange();
    void PointChangePriceQuery(bool bAllPoint);
    void ReceivePriceFromServer(int iGold);
};
