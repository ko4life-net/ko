// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UINationSelectDlg.h"
#include "GameProcNationSelect.h"

#include "N3Base/N3SndObj.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIButton.h"

#include "APISocket.h"
#include "GameBase.h"
#include "UIMsgBoxOkCancel.h"

#define CHILD_UI_SELNATION_MSG 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINationSelectDlg::CUINationSelectDlg() {
    m_pBaseEl = NULL;
    m_pTextElNotice = NULL;
    m_pBtnElSelection = NULL;
    m_pBtnElClose = NULL;
    m_pBtnElNext = NULL;

    m_pBaseKa = NULL;
    m_pTextKaNotice = NULL;
    m_pBtnKaSelection = NULL;
    m_pBtnKaClose = NULL;
    m_pBtnKaNext = NULL;

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
        CN3Base::s_SndMgr.ReleaseObj(&m_pSndKa);
    }
    if (m_pSndEl) {
        CN3Base::s_SndMgr.ReleaseObj(&m_pSndEl);
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
        m_pTextElNotice = (CN3UIString *)m_pBaseEl->GetChildByID("text_elmo_notice");
        __ASSERT(m_pTextElNotice, "NULL UI Component!!");

        m_pBtnElSelection = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_selection");
        __ASSERT(m_pBtnElSelection, "NULL UI Component!!");

        m_pBtnElClose = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_close");
        __ASSERT(m_pBtnElClose, "NULL UI Component!!");

        m_pBtnElNext = (CN3UIButton *)m_pBaseEl->GetChildByID("btn_elmo_next");
        __ASSERT(m_pBtnElNext, "NULL UI Component!!");
    }

    m_pBaseKa = (CN3UIBase *)GetChildByID("nation_karus");
    __ASSERT(m_pBaseKa, "NULL UI Component!!");
    if (m_pBaseKa) {
        m_pTextKaNotice = (CN3UIString *)m_pBaseKa->GetChildByID("text_karus_notice");
        __ASSERT(m_pTextKaNotice, "NULL UI Component!!");

        m_pBtnKaSelection = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_selection");
        __ASSERT(m_pBtnKaSelection, "NULL UI Component!!");

        m_pBtnKaClose = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_close");
        __ASSERT(m_pBtnKaClose, "NULL UI Component!!");

        m_pBtnKaNext = (CN3UIButton *)m_pBaseKa->GetChildByID("btn_karus_next");
        __ASSERT(m_pBtnKaNext, "NULL UI Component!!");
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

bool CUINationSelectDlg::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (!m_bTransitionActive && dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == (CN3UIBase *)m_pBtnKaClose || pSender == (CN3UIBase *)m_pBtnElClose) {
            CGameProcedure::s_pSocket->Disconnect();
            CGameProcedure::ProcActiveSet((CGameProcedure *)CGameProcedure::s_pProcCharacterSelect);
            //*(_BYTE *)(s_pProcCharacterSelect + 53) = 1;
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtnKaNext || pSender == (CN3UIBase *)m_pBtnElNext) {
            ChangeNation((pSender != (CN3UIBase *)m_pBtnKaNext));
            ButtonsSetEnable(false);
            return true;
        } else if (pSender == (CN3UIBase *)m_pBtnKaSelection || pSender == (CN3UIBase *)m_pBtnElSelection) {
            m_eCurNation = (pSender == (CN3UIBase *)m_pBtnKaSelection) ? NATION_KARUS : NATION_ELMORAD;
            if (m_pMsgBoxOkCancel) {
                // 10420: You must delete all your characters in this nation if you wish to create a character
                std::string szBuff;
                ::_LoadStringFromResource(10420, szBuff);
                m_pMsgBoxOkCancel->ShowWindow(CHILD_UI_SELNATION_MSG, this);
                m_pMsgBoxOkCancel->SetMsg(szBuff);
            }
            return true;
        }
    }

    return true;
}

void CUINationSelectDlg::CallBackProc(int iID, DWORD dwFlag) {
    if (iID == CHILD_UI_SELNATION_MSG && dwFlag == UI_MSGBOX_OK_MSG) {
        if (m_pProcNationSelectRef) {
            m_pProcNationSelectRef->MsgSendNationSelect(m_eCurNation);
        }
    }
}

void CUINationSelectDlg::ButtonsSetEnable(bool bEnable) {
    std::vector<CN3UIBase *> pBtns = {
        m_pBtnElSelection, m_pBtnElClose, m_pBtnElNext, m_pBtnKaSelection, m_pBtnKaClose, m_pBtnKaNext,
    };

    eUI_STATE eState = bEnable ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE;
    for (const auto & pBtn : pBtns) {
        pBtn->SetState(eState);
    }
}

void CUINationSelectDlg::InitResources() {
    std::string szBuff;
    if (m_pTextElNotice) {
        // 10422: El Morad is a nation that was formed by King Manes...
        ::_LoadStringFromResource(10422, szBuff);
        m_pTextElNotice->SetString(szBuff);
    }
    if (m_pTextKaNotice) {
        // 10421: Karus is a new nation that was established in Adonis...
        ::_LoadStringFromResource(10421, szBuff);
        m_pTextKaNotice->SetString(szBuff);
    }

    if (m_pSndKa) {
        CN3Base::s_SndMgr.ReleaseObj(&m_pSndKa);
    }
    if (m_pSndEl) {
        CN3Base::s_SndMgr.ReleaseObj(&m_pSndEl);
    }

    if (m_pBaseEl) {
        m_pBaseEl->SetVisible(false);
    }
    if (m_pBaseKa) {
        m_pBaseKa->SetVisible(true);
    }
    m_bNationKa = true;

    m_pSndKa = CN3Base::s_SndMgr.CreateObj(20009, SNDTYPE_STREAM);
    if (m_pSndKa) {
        m_pSndKa->Looping(true);
        m_pSndKa->Play(NULL, 0.0f, 0.0f);
    }
}

void CUINationSelectDlg::ChangeNation(bool bNationKa) {
    if (bNationKa) {
        m_bNationKa = true;
        if (m_pSndEl) {
            CN3Base::s_SndMgr.ReleaseObj(&m_pSndEl);
        }
        m_pSndKa = CN3Base::s_SndMgr.CreateObj(20009, SNDTYPE_STREAM);
        if (m_pSndKa) {
            m_pSndKa->Looping(true);
            m_pSndKa->Play(NULL, 0.0f, 0.0f);
        }
    } else {
        m_bNationKa = false;
        if (m_pSndKa) {
            CN3Base::s_SndMgr.ReleaseObj(&m_pSndKa);
        }
        m_pSndEl = CN3Base::s_SndMgr.CreateObj(20000, SNDTYPE_STREAM);
        if (m_pSndEl) {
            m_pSndEl->Looping(true);
            m_pSndEl->Play(NULL, 0.0f, 0.0f);
        }
    }
    if (m_pBaseEl) {
        m_pBaseEl->SetVisible(true);
    }
    if (m_pBaseKa) {
        m_pBaseKa->SetVisible(true);
    }
    m_bTransitionActive = true;
}

void CUINationSelectDlg::Render() {
    if (!m_bTransitionActive) {
        CN3UIBase::Render();
        return;
    }

    m_fTextureFactor -= s_fSecPerFrm * 0.3333333f;
    if (m_fTextureFactor < 0.0f) {
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
