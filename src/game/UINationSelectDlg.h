// UINationSelectDlg.h: interface for the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUINationSelectDlg : public CN3UIBase {
  public:
    CN3UIBase * m_pBtnKarus;
    CN3UIBase * m_pBtnElmorad;

    class CGameProcNationSelect * m_pProcNationSelectRef;

  public:
    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUINationSelectDlg();
    virtual ~CUINationSelectDlg();
};
