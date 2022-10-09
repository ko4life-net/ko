// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UINationSelectDlg.h"
#include "GameProcNationSelect.h"
#include "GameProcLogIn.h"

#include "N3Base/N3SndObjStream.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIButton.h"

#include "APISocket.h"
#include "GameBase.h"
#include "UIMsgBoxOkCancel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINationSelectDlg::CUINationSelectDlg() {
    m_pBaseEl = NULL;
    m_pTextNoticeEl = NULL;
    m_pBtnSelectionEl = NULL;
    m_pBtnCloseEl = NULL;
    m_pBtnNextEl = NULL;

    m_pBaseKa = NULL;
    m_pTextNoticeKa = NULL;
    m_pBtnSelectionKa = NULL;
    m_pBtnCloseKa = NULL;
    m_pBtnNextKa = NULL;

    m_pProcNationSelectRef = NULL;

    m_pTextNoticeName = NULL;
    m_pTextNotice = NULL;

    m_pMsgBoxOkCancel = NULL;

    m_eCurNation = NATION_NOTSELECTED;

    m_pSndKa = NULL;
    m_pSndEl = NULL;

    m_bTransitionActive = false;
    m_bNationKa = false;

    m_fTextureFactor = 1.0f;
}

CUINationSelectDlg::~CUINationSelectDlg() {
    if (m_pMsgBoxOkCancel) {
        m_pMsgBoxOkCancel->Release();
    }
    m_pMsgBoxOkCancel = NULL;

    if (m_pSndKa) {
        CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSndKa);
    }
    if (m_pSndEl) {
        CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSndEl);
    }
}

bool CUINationSelectDlg::Load(HANDLE hFile) {
    if (!CN3UIBase::Load(hFile)) {
        return false;
    }

    m_fTextureFactor = 1.0f;
    m_bTransitionActive = false;
    m_bNationKa = false;

    m_pTextNoticeName = (CN3UIString *)GetChildByID("text_notice_name");
    __ASSERT(m_pTextNoticeName, "NULL UI Component!!");
    if (m_pTextNoticeName) {
        m_pTextNoticeName->SetVisible(false);
    }

    m_pTextNotice = (CN3UIString *)GetChildByID("text_notice");
    __ASSERT(m_pTextNotice, "NULL UI Component!!");
    if (m_pTextNotice) {
        m_pTextNotice->SetVisible(false);
    }

    m_pBaseEl = (CN3UIBase *)GetChildByID("nation_elmo");
    __ASSERT(m_pBaseEl, "NULL UI Component!!");
    if (m_pBaseEl) {
        m_pTextNoticeEl = (CN3UIString *)m_pBaseEl->GetChildByID("text_elmo_notice");
        __ASSERT(m_pTextNoticeEl, "NULL UI Component!!");

        m_pBtnSelectionEl = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_selection");
        __ASSERT(m_pBtnSelectionEl, "NULL UI Component!!");

        m_pBtnCloseEl = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_close");
        __ASSERT(m_pBtnCloseEl, "NULL UI Component!!");

        m_pBtnNextEl = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_next");
        __ASSERT(m_pBtnNextEl, "NULL UI Component!!");
    }

    m_pBaseKa = (CN3UIBase *)GetChildByID("nation_karus");
    __ASSERT(m_pBaseKa, "NULL UI Component!!");
    if (m_pBaseKa) {
        m_pTextNoticeKa = (CN3UIString *)m_pBaseKa->GetChildByID("text_karus_notice");
        __ASSERT(m_pTextNoticeKa, "NULL UI Component!!");

        m_pBtnSelectionKa = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_selection");
        __ASSERT(m_pBtnSelectionKa, "NULL UI Component!!");

        m_pBtnCloseKa = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_close");
        __ASSERT(m_pBtnCloseKa, "NULL UI Component!!");

        m_pBtnNextKa = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_next");
        __ASSERT(m_pBtnNextKa, "NULL UI Component!!");
    }

    InitResources();

    RECT rc = this->GetRegion();
    int  iX = ((int)s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
    int  iY = ((int)s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
    this->SetPos(iX, iY);

    __TABLE_UI_RESRC * pTbl = CGameBase::s_pTbl_UI->Find(NATION_KARUS);
    if (pTbl) {
        if (m_pMsgBoxOkCancel) {
            delete m_pMsgBoxOkCancel;
        }
        m_pMsgBoxOkCancel = new CUIMsgBoxOkCancel();
        if (!m_pMsgBoxOkCancel->LoadFromFile(pTbl->szMsgBoxOkCancel)) {
            delete m_pMsgBoxOkCancel;
            m_pMsgBoxOkCancel = NULL;
            __ASSERT(false, "NULL UI Component!!");
        }
        m_pMsgBoxOkCancel->SetPosCenter();
    }

    return true;
}

void CUINationSelectDlg::Render() {
    if (!m_bTransitionActive) {
        CN3UIBase::Render();
        return;
    }

    m_fTextureFactor -= s_fSecPerFrm * 0.3333333f;
    if (m_fTextureFactor < 0.30f) {
        m_fTextureFactor = 1.0f;
        m_bTransitionActive = false;
        if (m_bNationKa) {
            if (m_pBaseEl) {
                m_pBaseEl->SetVisible(false);
            }
            if (m_pBaseKa) {
                m_pBaseKa->SetVisible(true);
            }
        } else {
            if (m_pBaseEl) {
                m_pBaseEl->SetVisible(true);
            }
            if (m_pBaseKa) {
                m_pBaseKa->SetVisible(false);
            }
        }

        CN3UIBase::Render();
        ButtonsSetEnable(true);
    } else {
        DWORD dwAlpha, dwSrcBlend, dwDestBlend, dwTexFact;
        DWORD dwTexAlphaOp, dwTexAlpha1, dwTexAlpha2;
        CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
        CN3Base::s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
        CN3Base::s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
        CN3Base::s_lpD3DDev->GetRenderState(D3DRS_TEXTUREFACTOR, &dwTexFact);
        CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwTexAlphaOp);
        CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTexAlpha1);
        CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwTexAlpha2);

        DWORD dwFactorToApply = ((DWORD)(255.0f * m_fTextureFactor)) << 24;

        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, dwFactorToApply);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTOP_SELECTARG2);

        if (m_bNationKa) {
            if (m_pBaseEl) {
                m_pBaseEl->Render();
            }
            if (m_pBaseKa) {
                m_pBaseKa->Render();
            }
        } else {
            if (m_pBaseKa) {
                m_pBaseKa->Render();
            }
            if (m_pBaseEl) {
                m_pBaseEl->Render();
            }
        }

        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
        CN3Base::s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, dwTexFact);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwTexAlphaOp);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwTexAlpha1);
        CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwTexAlpha2);
    }
}

bool CUINationSelectDlg::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (!m_bTransitionActive && dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnCloseKa || pSender == (CN3UIBase *)m_pBtnCloseEl) {
            CGameProcedure::s_pSocket->Disconnect();
            CGameProcedure::ProcActiveSet((CGameProcedure *)CGameProcedure::s_pProcLogIn);
            CGameProcedure::s_pProcLogIn->m_bLogIn = true;
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtnNextKa || pSender == (CN3UIBase *)m_pBtnNextEl) {
            ChangeNation((pSender != (CN3UIBase *)m_pBtnNextKa));
            ButtonsSetEnable(false);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtnSelectionKa || pSender == (CN3UIBase *)m_pBtnSelectionEl) {
            m_eCurNation = (pSender == (CN3UIBase *)m_pBtnSelectionKa) ? NATION_KARUS : NATION_ELMORAD;
            m_pProcNationSelectRef->MsgSendNationSelect(m_eCurNation);
            return true;
        }
    }

    return true;
}

void CUINationSelectDlg::InitResources() {
    std::string szBuff;
    if (m_pTextNoticeEl) {
        // El Morad is a nation that was formed by King Manes...
        ::_LoadStringFromResource(IDS_NATIONSELECT_DESCRIPTION_EL, szBuff);
        m_pTextNoticeEl->SetString(szBuff);
    }
    if (m_pTextNoticeKa) {
        // Karus is a new nation that was established in Adonis...
        ::_LoadStringFromResource(IDS_NATIONSELECT_DESCRIPTION_KA, szBuff);
        m_pTextNoticeKa->SetString(szBuff);
    }

    if (m_pBaseEl) {
        m_pBaseEl->SetVisible(false);
    }
    if (m_pBaseKa) {
        m_pBaseKa->SetVisible(true);
    }

    if (m_pSndEl) {
        CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSndEl);
    }
    if (m_pSndKa) {
        CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSndKa);
    }
    m_pSndEl = CN3Base::s_SndMgr.CreateStreamObj(ID_SOUND_BGM_EL_BATTLE);
    m_pSndKa = CN3Base::s_SndMgr.CreateStreamObj(ID_SOUND_BGM_KA_BATTLE);
    if (m_pSndEl) {
        m_pSndEl->Looping(true);
    }
    if (m_pSndKa) {
        m_pSndKa->Looping(true);
        m_pSndKa->Play(NULL, 0.0f, 3.0f);
    }

    m_bNationKa = true;
}

void CUINationSelectDlg::ChangeNation(bool bNationKa) {
    if (bNationKa) {
        m_bNationKa = true;
        if (m_pSndEl) {
            m_pSndEl->Stop(3.0f);
        }
        if (m_pSndKa) {
            m_pSndKa->Play(NULL, 0.0f, 6.0f);
        }
    } else {
        m_bNationKa = false;
        if (m_pSndKa) {
            m_pSndKa->Stop(3.0f);
        }
        if (m_pSndEl) {
            m_pSndEl->Play(NULL, 0.0f, 6.0f);
        }
    }

    m_bTransitionActive = true;
}

void CUINationSelectDlg::ButtonsSetEnable(bool bEnable) {
    eUI_STATE eState = bEnable ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE;
    for (const auto & pBtn : {
             m_pBtnSelectionEl,
             m_pBtnCloseEl,
             m_pBtnNextEl,
             m_pBtnSelectionKa,
             m_pBtnCloseKa,
             m_pBtnNextKa,
         }) {
        pBtn->SetState(eState);
    }
}
