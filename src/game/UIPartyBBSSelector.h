// UIPartyBBSSelector.h: interface for the CUIPartyBBSSelector class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"

#define WANT_PARTY        0
#define WANT_PARTY_MEMBER 1

class CUIPartyBBSSelector : public CN3UIBase {
  protected:
    CN3UIButton * m_pBtn_WantPartyMember;
    CN3UIButton * m_pBtn_WantParty;

  public:
    void MsgSend_PartyBBSKind(BYTE byKind);
    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUIPartyBBSSelector();
    virtual ~CUIPartyBBSSelector();
};
