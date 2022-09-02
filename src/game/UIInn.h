// UIInn.h: interface for the UIInn class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "N3Base/N3UIBase.h"

//////////////////////////////////////////////////////////////////////

class CUIInn : public CN3UIBase {
  public:
    bool OnKeyPress(int iChar);
    void SetVisible(bool bVisible);
    void Message(int iMessageID);

    void MsgSend_OpenWareHouse();

    CUIInn();
    virtual ~CUIInn();

    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
};
