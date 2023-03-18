// UINPCChangeEvent.cpp: implementation of the CUINPCChangeEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UINPCChangeEvent.h"

#include "PacketDef.h"

#include "GameProcMain.h"
#include "PlayerMySelf.h"

#include "GameProcedure.h"
#include "APISocket.h"

#include "N3UIWndBase.h"
#include "UIInventory.h"
#include "UIManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINPCChangeEvent::CUINPCChangeEvent() {
    m_pBtn_Change = NULL;
    m_pBtn_Repoint0 = NULL;
    m_pBtn_Repoint1 = NULL;
    m_pBtn_Close = NULL;

    m_pDlg = NULL;
}

CUINPCChangeEvent::~CUINPCChangeEvent() {
    if (m_pDlg) {
        delete m_pDlg;
    }
    m_pDlg = NULL;
}

void CUINPCChangeEvent::Release() {
    if (m_pDlg) {
        m_pDlg->Release();
        delete m_pDlg;
        m_pDlg = NULL;
    }

    CN3UIBase::Release();
}

bool CUINPCChangeEvent::Load(HANDLE hFile) {
    int iW = CN3Base::s_CameraData.vp.Width;
    int iH = CN3Base::s_CameraData.vp.Height;

    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_Change        = (CN3UIButton*)GetChildByID("Btn_change");        __ASSERT(m_pBtn_Change,    "NULL UI Component!!");
    m_pBtn_Repoint0        = (CN3UIButton*)GetChildByID("Btn_repoint0");    __ASSERT(m_pBtn_Repoint0, "NULL UI Component!!");
    m_pBtn_Repoint1        = (CN3UIButton*)GetChildByID("Btn_repoint1");    __ASSERT(m_pBtn_Repoint1,    "NULL UI Component!!");
    m_pBtn_Close = (CN3UIButton *)GetChildByID("Btn_close");
    __ASSERT(m_pBtn_Close, "NULL UI Component!!");

    // UIPointInitDlg.. ^^
    e_Nation           eNation = CGameProcedure::s_pPlayer->m_InfoBase.eNation; // ����....
    __TABLE_UI_RESRC * pTbl = CGameProcedure::s_pTbl_UI->Find(eNation);

    m_pDlg = new CUIPointInitDlg();
    m_pDlg->LoadFromFile(pTbl->szChangeInitBill);

    // ��ġ ��� ..
    int iXPos, iYPos;
    iXPos = (iW / 2) - (m_pDlg->GetRegion().right - m_pDlg->GetRegion().left) / 2;
    iYPos = (iH / 2) - (m_pDlg->GetRegion().bottom - m_pDlg->GetRegion().top) / 2;
    m_pDlg->SetPos(iXPos, iYPos);
    m_pDlg->Close();

    return true;
}

bool CUINPCChangeEvent::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_Change) {
            Close();
            ClassChange();
        } else if (pSender == m_pBtn_Close) {
            Close();
        }

        else if (pSender == m_pBtn_Repoint0 && !CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) {
            CUIInventory * pInv = ((CGameProcMain *)CGameProcedure::s_pProcMain)->m_pUIInventory;
            if (!pInv) {
                Close();
                return true;
            }

            if (!pInv->HasAnyItemInSlot()) {
                PointChangePriceQuery(true);
            } else {
                Close();
                char        szBuf[256] = "";
                std::string szMsg;

                ::_LoadStringFromResource(IDS_MSG_HASITEMINSLOT, szMsg);
                sprintf(szBuf, szMsg.c_str());
                CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
            }
        }

        else if (pSender == m_pBtn_Repoint1 && !CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) {
            PointChangePriceQuery(false);
        }
    }

    return true;
}

void CUINPCChangeEvent::Open() {
    SetVisible(true);
}

void CUINPCChangeEvent::Close() {
    SetVisible(false);
    if (m_pDlg && m_pDlg->IsVisible()) {
        m_pDlg->Close();
    }
}

void CUINPCChangeEvent::ClassChange() {
    BYTE byBuff[32];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_CLASS_CHANGE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_CLASS_CHANGE_PURE);
    CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUINPCChangeEvent::PointChangePriceQuery(bool bAllPoint) {
    BYTE byBuff[32];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_CLASS_CHANGE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY);
    if (bAllPoint) {
        CAPISocket::MP_AddByte(byBuff, iOffset, 1);
    } else {
        CAPISocket::MP_AddByte(byBuff, iOffset, 2);
    }

    CGameProcedure::s_pSocket->Send(byBuff, iOffset);
    m_bSendedAllPoint = bAllPoint;
}

void CUINPCChangeEvent::ReceivePriceFromServer(int iGold) {
    if (m_pDlg) {
        m_pDlg->ShowWindow(-1, this);
        m_pDlg->InitDlg(m_bSendedAllPoint, iGold);
    }
}

//this_ui_add_start
void CUINPCChangeEvent::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
    }
}

bool CUINPCChangeEvent::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_ESCAPE:
        ReceiveMessage(m_pBtn_Close, UIMSG_BUTTON_CLICK);
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}
//this_ui_add_end
