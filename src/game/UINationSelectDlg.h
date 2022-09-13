// UINationSelectDlg.h: interface for the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUINationSelectDlg : public CN3UIBase {
  public:
    CN3UIBase * m_pBtnKarus;
    CN3UIBase * m_pBtnKarusNext;
    CN3UIBase * m_pBtnKarusExit;
    CN3UIBase * m_pBtnElmorad;
    CN3UIBase * m_pBtnElmoradNext;
    CN3UIBase * m_pBtnElmoradExit;

    class CGameProcNationSelect * m_pProcNationSelectRef;

  public:
    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUINationSelectDlg();
    virtual ~CUINationSelectDlg();
};
