// UINationSelectDlg.h: interface for the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUINationSelectDlg : public CN3UIBase {
  public:
    CN3UIString * m_pTextNoticeName;
    CN3UIString * m_pTextNotice;

    CN3UIBase *   m_pBaseEl;
    CN3UIString * m_pTextElNotice;
    CN3UIButton * m_pBtnElSelection;
    CN3UIButton * m_pBtnElClose;
    CN3UIButton * m_pBtnElNext;

    CN3UIBase *   m_pBaseKa;
    CN3UIString * m_pTextKaNotice;
    CN3UIButton * m_pBtnKaSelection;
    CN3UIButton * m_pBtnKaClose;
    CN3UIButton * m_pBtnKaNext;

    enum e_Nation m_eCurNation;

    float m_fTextureFactor;

    bool m_bTransitionActive;
    bool m_bNationKa;

    CN3SndObj * m_pSndKa;
    CN3SndObj * m_pSndEl;

    class CUIMsgBoxOkCancel * m_pMsgBoxOkCancel;

    class CGameProcNationSelect * m_pProcNationSelectRef;

  public:
    void Render() override;
    void ChangeNation(bool bNationKa);
    void ButtonsSetEnable(bool bEnable);
    void InitResources();

    bool Load(HANDLE hFile) override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    void CallBackProc(int iID, DWORD dwFlag) override;

    CUINationSelectDlg();
    ~CUINationSelectDlg() override;
};
