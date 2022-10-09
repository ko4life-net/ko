// UINationSelectDlg.h: interface for the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUINationSelectDlg : public CN3UIBase {
  public:
    CUINationSelectDlg();
    ~CUINationSelectDlg() override;

    bool Load(HANDLE hFile) override;
    void Render() override;
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;

    void InitResources();
    void ChangeNation(bool bNationKa);
    void ButtonsSetEnable(bool bEnable);

  public:
    CN3UIString * m_pTextNoticeName;
    CN3UIString * m_pTextNotice;

    CN3UIBase *   m_pBaseEl;
    CN3UIString * m_pTextNoticeEl;
    CN3UIButton * m_pBtnSelectionEl;
    CN3UIButton * m_pBtnCloseEl;
    CN3UIButton * m_pBtnNextEl;

    CN3UIBase *   m_pBaseKa;
    CN3UIString * m_pTextNoticeKa;
    CN3UIButton * m_pBtnSelectionKa;
    CN3UIButton * m_pBtnCloseKa;
    CN3UIButton * m_pBtnNextKa;

    enum e_Nation m_eCurNation;

    float m_fTextureFactor;

    bool m_bTransitionActive;
    bool m_bNationKa;

    CN3SndObjStream * m_pSndKa;
    CN3SndObjStream * m_pSndEl;

    class CUIMsgBoxOkCancel * m_pMsgBoxOkCancel;

    class CGameProcNationSelect * m_pProcNationSelectRef;
};
