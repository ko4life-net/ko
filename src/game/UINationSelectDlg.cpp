// UINationSelectDlg.cpp: implementation of the CUINationSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
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
    m_pBtnElmorad = NULL;
}

bool CUINationSelectDlg::Load(HANDLE hFile) {
    bool bSuccess = CN3UIBase::Load(hFile);

    // TODO: Just to get things loading for now, but this needs to be fixed and properly load and implement all
    // UI elements.
    m_pBtnKarus = GetChildByID("nation_karus")->GetChildByID("btn_karus_selection");
    __ASSERT(m_pBtnKarus, "NULL UI Component!!");
    m_pBtnElmorad = GetChildByID("nation_elmo")->GetChildByID("btn_elmo_selection");
    __ASSERT(m_pBtnElmorad, "NULL UI Component!!");

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
        }
    }

    return true;
}
