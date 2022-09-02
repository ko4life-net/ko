// UINpcTalk.h: interface for the CUINpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIString.h"

//////////////////////////////////////////////////////////////////////

class CUINpcTalk : public CN3UIBase {
    int m_iIDTarget;

  public:
    CUINpcTalk();
    virtual ~CUINpcTalk();

    void Release();

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    void Open(int iIDTarget = -1);
    void Close();
};
