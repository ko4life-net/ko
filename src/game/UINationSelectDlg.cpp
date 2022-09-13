// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UINationSelectDlg.h"
#include "GameProcNationSelect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINationSelectDlg::CUINationSelectDlg() {
    m_pProcNationSelectRef = NULL;
}

CUINationSelectDlg::~CUINationSelectDlg() {
    m_pBtnKarus = NULL;
    m_pBtnKarusNext = NULL;
    m_pBtnKarusExit = NULL;
    m_pBtnElmorad = NULL;
    m_pBtnElmoradNext = NULL;
    m_pBtnElmoradExit = NULL;
}

bool CUINationSelectDlg::Load(HANDLE hFile) {
    bool bSuccess = CN3UIBase::Load(hFile);
    // UI elements.

    m_pBtnKarus = GetChildByID("nation_karus")->GetChildByID("btn_karus_selection");
    __ASSERT(m_pBtnKarus, "NULL UI Component!!");
    m_pBtnKarusNext = GetChildByID("nation_karus")->GetChildByID("btn_karus_next");
    __ASSERT(m_pBtnKarusNext, "NULL UI Component!!");
    m_pBtnKarusExit = GetChildByID("nation_karus")->GetChildByID("btn_karus_close");
    __ASSERT(m_pBtnKarusExit, "NULL UI Component!!");
    m_pBtnElmorad = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_selection");
    __ASSERT(m_pBtnElmorad, "NULL UI Component!!");
    m_pBtnElmoradNext = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_next");
    __ASSERT(m_pBtnElmoradNext, "NULL UI Component!!");
    m_pBtnElmoradExit = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_close");
    __ASSERT(m_pBtnElmoradExit, "NULL UI Component!!");

    RECT rc = this->GetRegion();
    int  iX = ((int)s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
    int  iY = ((int)s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
    this->SetPos(iX, iY);

    return bSuccess;
}

bool CUINationSelectDlg::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtnKarus) // Karus
        {
            if (m_pProcNationSelectRef) {
                m_pProcNationSelectRef->MsgSendNationSelect(NATION_KARUS);
            }
        } else if (pSender == m_pBtnElmorad) // Elmorad
        {
            if (m_pProcNationSelectRef) {
                m_pProcNationSelectRef->MsgSendNationSelect(NATION_ELMORAD);
            }
        } else if (pSender == m_pBtnKarusNext) // Karus Next Page Button
        {
            GetChildByID("nation_karus")->SetVisible(false);
            GetChildByID("nation_elmo")->SetVisible(true);
        } else if (pSender == m_pBtnElmoradNext) // Elmorad Next Page Button
        {
            GetChildByID("nation_elmo")->SetVisible(false);
            GetChildByID("nation_karus")->SetVisible(true);
        }
        if (pSender == m_pBtnKarusExit || pSender == m_pBtnElmoradExit) // Karus Elmorad Exit button
        {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_CONFIRM_EXIT_GAME, szMsg);
            e_Behavior eBehavior = ((true) ? BEHAVIOR_EXIT : BEHAVIOR_NOTHING);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, eBehavior);
        }
    }

    return true;
}
