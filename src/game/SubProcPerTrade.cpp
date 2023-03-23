// SubProcPerTrade.cpp: implementation of the CSubProcPerTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"

#include "SubProcPerTrade.h"
#include "GameProcedure.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"

#include "UIManager.h"
#include "UIMessageBox.h"

#include "UIInventory.h"
#include "UIDroppedItemDlg.h"

#include "PacketDef.h"
#include "APISocket.h"

#include "UIPerTradeDlg.h"
#include "UITradeEditDlg.h"

#include "N3UIIcon.h"
#include "UIHotKeyDlg.h"
#include "UISkillTreeDlg.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIEdit.h"
#include "N3Base/N3UIButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubProcPerTrade::CSubProcPerTrade() {
    m_pUIPerTradeDlg = NULL;
    m_pUITradeEditDlg = NULL;
    m_ePerTradeState = PER_TRADE_STATE_NONE;
    m_iOtherID = -1;
    m_iGoldOffsetBackup = 0;
    m_szMsg = ""; //MessagBox key
}

CSubProcPerTrade::~CSubProcPerTrade() {}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::Release() {
    if (m_pUIPerTradeDlg) {
        m_pUIPerTradeDlg->Release();
        delete m_pUIPerTradeDlg;
        m_pUIPerTradeDlg = NULL;
    }

    if (m_pUITradeEditDlg) {
        m_pUITradeEditDlg->Release();
        delete m_pUITradeEditDlg;
        m_pUITradeEditDlg = NULL;
    }

    m_szMsg = ""; //MessageBox key
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::InitPerTradeDlg(CUIManager * pUIManager) {
    int  iW = CN3Base::s_CameraData.vp.Width;
    RECT rc;

    e_Nation           eNation = s_pPlayer->m_InfoBase.eNation; // ����....
    __TABLE_UI_RESRC * pTbl = s_pTbl_UI->Find(eNation);

    // UIPerTradeDlg.. ^^
    m_pUIPerTradeDlg = new CUIPerTradeDlg();
    m_pUIPerTradeDlg->Init(pUIManager);
    m_pUIPerTradeDlg->m_pSubProcPerTrade = this;
    m_pUIPerTradeDlg->LoadFromFile(pTbl->szPersonalTrade);
    rc = m_pUIPerTradeDlg->GetRegion();
    m_pUIPerTradeDlg->SetPos(iW - (rc.right - rc.left), 10);
    m_pUIPerTradeDlg->SetVisible(false);
    m_pUIPerTradeDlg->InitIconWnd(UIWND_PER_TRADE);
    m_pUIPerTradeDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIPerTradeDlg->SetState(UI_STATE_COMMON_NONE);

    // UITradeEditDlg.. ^^
    m_pUITradeEditDlg = new CUITradeEditDlg();
    m_pUITradeEditDlg->Init(pUIManager);
    m_pUITradeEditDlg->m_pSubProcPerTrade = this;
    m_pUITradeEditDlg->LoadFromFile(pTbl->szPersonalTradeEdit);
    m_pUITradeEditDlg->SetStyle(UISTYLE_ALWAYSTOP);
    // ��ġ ��� ..
    int iXPos, iYPos;
    int iH = CN3Base::s_CameraData.vp.Height;
    iXPos = (iW / 2) - (m_pUITradeEditDlg->GetRegion().right - m_pUITradeEditDlg->GetRegion().left) / 2;
    iYPos = (iH / 2) - (m_pUITradeEditDlg->GetRegion().bottom - m_pUITradeEditDlg->GetRegion().top) / 2;
    m_pUITradeEditDlg->SetPos(iXPos, iYPos);
    m_pUITradeEditDlg->Close();
    m_pUITradeEditDlg->SetUIType(UI_TYPE_BASE);
    m_pUITradeEditDlg->SetState(UI_STATE_COMMON_NONE);

    // �ϴ��� �� ���������� �Ƚ�.. ^^
    m_pUITradeEditDlg->m_pArea = (CN3UIArea *)m_pUITradeEditDlg->GetChildByID("area_trade_icon");
    __ASSERT(m_pUITradeEditDlg->m_pArea, "NULL UI Component!!");

    m_pUITradeEditDlg->m_pImageOfIcon = new CN3UIImage;
    m_pUITradeEditDlg->m_pImageOfIcon->Init(m_pUITradeEditDlg);
    // �� ������ ���ڿ� ã��.. ���̵�� ã�� ��ɹۿ� ����.. ��.��
    __TABLE_ITEM_BASIC * pItem = NULL;        // ������ ���̺� ����ü ������..
    pItem = s_pTbl_Items_Basic->Find(dwGold); // �� ������ ���..

    std::string    szIconFN;
    e_PartPosition ePart;
    e_PlugPosition ePlug;
    CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����
    m_pUITradeEditDlg->m_pImageOfIcon->SetTex(szIconFN);
    float fUVAspect = (float)45.0f / (float)64.0f;
    m_pUITradeEditDlg->m_pImageOfIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
    m_pUITradeEditDlg->m_pImageOfIcon->SetRegion(m_pUITradeEditDlg->m_pArea->GetRegion());
    m_pUITradeEditDlg->m_pImageOfIcon->SetMoveRect(m_pUITradeEditDlg->m_pArea->GetRegion());
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::EnterWaitMsgFromServerStatePerTradeReq() {
    CPlayerOther * pTarget = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, false);
    if (NULL == pTarget) {
        return;
    }

    m_ePerTradeState = PER_TRADE_STATE_WAIT_FOR_REQ;

    // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
    char        szBuff[256] = "";
    std::string szFmt;
    ::_LoadStringFromResource(IDS_PERSONAL_TRADE_FMT_WAIT, szFmt);
    sprintf(szBuff, szFmt.c_str(), s_pPlayer->IDString().c_str(), pTarget->IDString().c_str());
    m_szMsg = CGameProcedure::MessageBoxPost(szBuff, "", MB_CANCEL, BEHAVIOR_PERSONAL_TRADE_FMT_WAIT);

    SecureCodeBegin();
}

void CSubProcPerTrade::EnterWaitMsgFromServerStatePerTradeReq(std::string szName) {
    m_ePerTradeState = PER_TRADE_STATE_WAIT_FOR_REQ;

    // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
    char        szBuff[256] = "";
    std::string szFmt;
    ::_LoadStringFromResource(IDS_PERSONAL_TRADE_FMT_WAIT, szFmt);
    sprintf(szBuff, szFmt.c_str(), s_pPlayer->IDString().c_str(), szName.c_str());
    m_szMsg = CGameProcedure::MessageBoxPost(szBuff, "", MB_CANCEL, BEHAVIOR_PERSONAL_TRADE_FMT_WAIT);

    SecureCodeBegin();
}

void CSubProcPerTrade::EnterWaitMyDecisionToPerTrade(int iOtherID) // ���� Ÿ�ο��Լ� ������ �ŷ��� ��û ���� ����..
{
    CPlayerOther * pTarget = s_pOPMgr->UPCGetByID(iOtherID, false);
    if (NULL == pTarget) {
        return;
    }

    m_iOtherID = iOtherID;
    m_ePerTradeState = PER_TRADE_STATE_WAIT_FOR_MY_DECISION_AGREE_OR_DISAGREE;

    // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
    char        szBuff[256] = "";
    std::string szFmt;
    ::_LoadStringFromResource(IDS_PERSONAL_TRADE_PERMIT, szFmt);
    sprintf(szBuff, szFmt.c_str(), s_pPlayer->IDString().c_str(), pTarget->IDString().c_str());
    m_szMsg = CGameProcedure::MessageBoxPost(szBuff, "", MB_YESNO, BEHAVIOR_PERSONAL_TRADE_PERMIT);

    SecureCodeBegin();
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::SecureCodeBegin() {
    // 1.��ŷ� ���̸� ���� �ŷ� ��û�� �ʵȴ�.. -> �ش� �κ�..    ok
    // 2.��ŷ� �߿� ���� �ŷ� ��û�� ������ ����.. -> �ش� �κ�.. ok

    // 3.������ �����̰� �ִ� ���̸� �����..
    if (s_pPlayer->IsMovingNow()) {
        s_pPlayer->ToggleMoveMode(); // �ڵ� ���� ���..
    }

    // 4.������ �Ŵ��� ������� ��� �ݴ´�..
    //   �κ��丮 �������̸�..
    if (CGameProcedure::s_pProcMain->m_pUIInventory->IsVisible()) {
        CGameProcedure::s_pProcMain->m_pUIInventory->Close();
    }

    //   Drop Item �������̸�..
    if (CGameProcedure::s_pProcMain->m_pUIDroppedItemDlg->IsVisible()) {
        CGameProcedure::s_pProcMain->m_pUIDroppedItemDlg->LeaveDroppedState();
    }

    // 5.��ǲ�� ���´�..    -> �ش� �κ�..    ok    (Ű�Է°� �޽���..)

    // 6.�ŷ�â�� ���� Control�� ���� Clear..
    CN3UIString * pStrMy = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_my");
    __ASSERT(pStrMy, "NULL UI Component!!");
    CN3UIString * pStrOther = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_other");
    __ASSERT(pStrOther, "NULL UI Component!!");
    CN3UIString * pStrMyName = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("text_id_my");
    __ASSERT(pStrMyName, "NULL UI Component!!");
    CN3UIString * pStrOtherName = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("text_id_other");
    __ASSERT(pStrOtherName, "NULL UI Component!!");
    pStrMy->SetString("0");
    pStrOther->SetString("0");
    pStrMyName->SetString(""); 
    pStrOtherName->SetString("");

    // 7.���� �ŷ� â�� óũ ��ư�� �������..
    CN3UIButton * pButtonMy = (CN3UIButton *)m_pUIPerTradeDlg->GetChildByID("btn_trade_my");
    __ASSERT(pButtonMy, "NULL UI Component!!");
    pButtonMy->SetState(UI_STATE_BUTTON_NORMAL);
    CN3UIButton * pButtonOther = (CN3UIButton *)m_pUIPerTradeDlg->GetChildByID("btn_trade_other");
    __ASSERT(pButtonOther, "NULL UI Component!!");
    pButtonOther->SetState(UI_STATE_BUTTON_NORMAL);

    if (s_pOPMgr->UPCGetByID(m_iOtherID, false) != NULL) {
       std::string Otherplayername = (s_pOPMgr->UPCGetByID(m_iOtherID, false))->IDString().c_str();
       pStrOtherName->SetString(Otherplayername);
       pStrMyName->SetString(s_pPlayer->IDString().c_str());
    } else {
       if (s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, false) != NULL) {
           std::string Otherplayername = (s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, false))->IDString().c_str();
           pStrOtherName->SetString(Otherplayername);
           pStrMyName->SetString(s_pPlayer->IDString().c_str());
       }
    }
    // 8.���� �ŷ� ��ư�� Click�� �� ����. uif ��ü ���..
}

///////////////////////////////////////////////////////////////////////
void CSubProcPerTrade::ResetMyTradeButtonState() {
    CN3UIButton * pButton = (CN3UIButton *)m_pUIPerTradeDlg->GetChildButtonByName("btn_trade_my");
    if (pButton) {
       pButton->SetState(UI_STATE_BUTTON_NORMAL);
    }
}
void CSubProcPerTrade::FinalizePerTrade() {
    m_ePerTradeState = PER_TRADE_STATE_NONE;

    if (m_pUIPerTradeDlg->IsVisible()) {
        m_pUIPerTradeDlg->SetVisible(false);
    }

    CGameProcedure::MessageBoxClose(m_szMsg);
    m_szMsg = "";

    if (m_pUITradeEditDlg->IsVisible()) {
        m_pUITradeEditDlg->SetQuantity(0);
        m_pUITradeEditDlg->Close();
    }
}

void CSubProcPerTrade::PerTradeCompleteSuccess() // ���� �ŷ� ���� ����..
{
    // ���� �ŷ� â�� �ִ� ������ ����..
    for (int i = 0; i < MAX_ITEM_PER_TRADE; i++) {
        if (m_pUIPerTradeDlg->m_pPerTradeMy[i] != NULL) {
            __IconItemSkill * spItem;
            spItem = m_pUIPerTradeDlg->m_pPerTradeMy[i];

            // �Ŵ������� ����..
            m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

            // ���ҽ� ����..
            spItem->pUIIcon->Release();
            delete spItem->pUIIcon;
            spItem->pUIIcon = NULL;
            delete spItem;
            spItem = NULL;
            m_pUIPerTradeDlg->m_pPerTradeMy[i] = NULL;
        }

        if (m_pUIPerTradeDlg->m_pPerTradeOther[i] != NULL) {
            __IconItemSkill * spItem;
            spItem = m_pUIPerTradeDlg->m_pPerTradeOther[i];

            // �Ŵ������� ����..
            m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

            // ���ҽ� ����..
            spItem->pUIIcon->Release();
            delete spItem->pUIIcon;
            spItem->pUIIcon = NULL;
            delete spItem;
            spItem = NULL;
            m_pUIPerTradeDlg->m_pPerTradeOther[i] = NULL;
        }
    }

    m_pUIPerTradeDlg->LeavePerTradeState();
}

void CSubProcPerTrade::PerTradeCompleteCancel() // ���� �ŷ� ���..
{
    int64_t iGold;    // �ŷ�â�� ��..
    int64_t iMyMoney; // �κ��丮�� ��..

    if ((int)m_ePerTradeState >= (int)PER_TRADE_STATE_NORMAL) {
        // ���� ���� �˻� �Ѵ�..
        // �ŷ� â�� �� ���� ���� ��� �´�..
        CN3UIString * pStrMy = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_my");
        __ASSERT(pStrMy, "NULL UI Component!!");
        iGold = pStrMy->GetStringAsInt({','});

        // ���� ���� ���� ���� ��� �´�..
        iMyMoney = s_pPlayer->m_InfoExt.iGold;
        // ���� ���� �Ѵ�..
        iMyMoney += iGold;
        s_pPlayer->m_InfoExt.iGold = iMyMoney;

        // �״��� �������� �˻��Ѵ�..
        // �����۵��� ���� ���..
        for (int i = 0; i < MAX_ITEM_PER_TRADE; i++) {
            if (m_pUIPerTradeDlg->m_pPerTradeMy[i] != NULL) {
                if ((m_pUIPerTradeDlg->m_pPerTradeMy[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                    (m_pUIPerTradeDlg->m_pPerTradeMy[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                    // Ȱ�̳� ����� �������� ���..
                    __IconItemSkill * spItem;
                    spItem = m_pUIPerTradeDlg->m_pPerTradeMy[i];

                    // ������ �������� �ִٸ�..
                    if (m_pUIPerTradeDlg->m_pPerTradeInv[m_pUIPerTradeDlg->m_iBackupiOrder[i]]) {
                        // ���� ������Ʈ �ϰ�..
                        m_pUIPerTradeDlg->m_pPerTradeInv[m_pUIPerTradeDlg->m_iBackupiOrder[i]]->iCount +=
                            m_pUIPerTradeDlg->m_pPerTradeMy[i]->iCount;

                        // �Ŵ������� ����..
                        m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

                        // ���ҽ� ����..
                        spItem->pUIIcon->Release();
                        delete spItem->pUIIcon;
                        spItem->pUIIcon = NULL;
                        delete spItem;
                        spItem = NULL;
                        m_pUIPerTradeDlg->m_pPerTradeMy[i] = NULL;
                    }
                    // ���ٸ� �״�� �̵��Ѵ�..
                    else {
                        __IconItemSkill * spItem;
                        spItem = m_pUIPerTradeDlg->m_pPerTradeMy[i];

                        spItem->pUIIcon->SetParent(m_pUIPerTradeDlg);

                        // �κ��丮 �����쿡 �����ϰ�..
                        m_pUIPerTradeDlg->m_pPerTradeInv[m_pUIPerTradeDlg->m_iBackupiOrder[i]] = spItem;

                        // �� �ŷ� �����쿡�� Ŭ����..
                        m_pUIPerTradeDlg->m_pPerTradeMy[i] = NULL;

                        CN3UIArea * pArea;

                        pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(UI_AREA_TYPE_PER_TRADE_INV,
                                                                       m_pUIPerTradeDlg->m_iBackupiOrder[i]);
                        if (pArea) {
                            spItem->pUIIcon->SetRegion(pArea->GetRegion());
                            spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
                        }
                    }
                } else {
                    __IconItemSkill * spItem;
                    spItem = m_pUIPerTradeDlg->m_pPerTradeMy[i];

                    spItem->pUIIcon->SetParent(m_pUIPerTradeDlg);

                    // �κ��丮 �����쿡 �����ϰ�..
                    m_pUIPerTradeDlg->m_pPerTradeInv[m_pUIPerTradeDlg->m_iBackupiOrder[i]] = spItem;

                    // �� �ŷ� �����쿡�� Ŭ����..
                    m_pUIPerTradeDlg->m_pPerTradeMy[i] = NULL;

                    CN3UIArea * pArea;

                    pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(UI_AREA_TYPE_PER_TRADE_INV,
                                                                   m_pUIPerTradeDlg->m_iBackupiOrder[i]);
                    if (pArea) {
                        spItem->pUIIcon->SetRegion(pArea->GetRegion());
                        spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
                    }
                }
            }

            if (m_pUIPerTradeDlg->m_pPerTradeOther[i] != NULL) {
                __IconItemSkill * spItem;
                spItem = m_pUIPerTradeDlg->m_pPerTradeOther[i];

                // �Ŵ������� ����..
                m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

                // ���ҽ� ����..
                spItem->pUIIcon->Release();
                delete spItem->pUIIcon;
                spItem->pUIIcon = NULL;
                delete spItem;
                spItem = NULL;
                m_pUIPerTradeDlg->m_pPerTradeOther[i] = NULL;
            }
        }

        m_pUIPerTradeDlg->LeavePerTradeState();
    }
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::LeavePerTradeState(e_PerTradeResultCode ePTRC) // ������ �ŷ��� ��ҵǴ� ���¸� ����..
{
    BYTE byBuff[4];   // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    char        szBuf[256] = "";
    std::string szMsg;

    switch (ePTRC) {
    case PER_TRADE_RESULT_MY_CANCEL: // �ŷ��� ��û�� ���� �ŷ� ��û�� ���..
        // �������� ��Ŷ ���� ����..
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_CANCEL);
        CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ����..

        // �� ������..
        PerTradeCompleteCancel();
        FinalizePerTrade();
        TRACE("���� �ŷ��� ��� ����� ���� ��ҵ�.. �������� ����..\n");
        break;

    case PER_TRADE_RESULT_MY_DISAGREE: // �ŷ��� ��û���� ���� �ŷ� ��û�� ���..
        // �������� ��Ŷ ���� ����..
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_AGREE);
        CAPISocket::MP_AddByte(byBuff, iOffset, 0x00);

        CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ����..

        // �� ������..
        FinalizePerTrade();
        TRACE("���� �ŷ��� ����.. ����� ���� ��ҵ� �������� ����..\n");
        break;

    case PER_TRADE_RESULT_OTHER_DISAGREE: // �ŷ��� ��û���� ������ �ŷ� ��û�� ���..
        TRACE("������ �ŷ��� ����.. \n");
        //this_ui
        // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
        ::_LoadStringFromResource(IDS_OTHER_PER_TRADE_ID_NO, szMsg);
        CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
        //            ::_LoadStringFromResource(IDS_OTHER_PER_TRADE_NO, szMsg);
        //            sprintf(szBuf, szMsg.c_str(),
        //                (s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, false))->IDString().c_str());
        //            CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        // �� ������..
        FinalizePerTrade();
        break;
    }
}

void CSubProcPerTrade::ProcessProceed(e_PerTradeProceedCode ePTPC) // ������ �ŷ��� ��ӵǴ� ���¸� ����..
{
    BYTE byBuff[4];   // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    switch (ePTPC) {
    case PER_TRADE_RESULT_MY_AGREE: // �ŷ��� ��û���� ���� �ŷ� ��û�� ���..
        // �������� ��Ŷ ���� ����..
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_AGREE);
        CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);

        CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ����..

        CGameProcedure::MessageBoxClose(m_szMsg);
        m_szMsg = "";

        // Trade Core Start..
        PerTradeCoreStart();
        break;

    case PER_TRADE_RESULT_OTHER_AGREE: // �ŷ��� ��û���� ������ �ŷ� ��û�� ���..
        CGameProcedure::MessageBoxClose(m_szMsg);
        m_szMsg = "";

        // Trade Core Start..
        PerTradeCoreStart();
        break;
    }
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::PerTradeCoreStart() {
    m_ePerTradeState = PER_TRADE_STATE_NORMAL;

    m_pUIPerTradeDlg->SetVisible(true);
    m_pUIPerTradeDlg->EnterPerTradeState();

    if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->IsVisible()) {
        CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->Close();
    }
}

void CSubProcPerTrade::PerTradeCoreInvDisable() {
    RECT rect = {0, 0, 0, 0};

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (m_pUIPerTradeDlg->m_pPerTradeInv[i] != NULL) {
            m_pUIPerTradeDlg->m_pPerTradeInv[i]->pUIIcon->SetMoveRect(rect);
        }
    }
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::RequestItemCountEdit() {
    if (m_ePerTradeState != PER_TRADE_STATE_NORMAL) {
        return;
    }
    m_ePerTradeState = PER_TRARE_STATE_EDITTING;

    m_pUITradeEditDlg->Open(true);
    m_pUIPerTradeDlg->PlayGoldSound();
}

void CSubProcPerTrade::ItemCountEditOK() {
    std::string szGold;
    int64_t     iGold;       // �ŷ�â�� ��..
    int64_t     iGoldOffset; // ����â�� ��..
    int64_t     iMyMoney;    // �κ��丮�� ��..

    // �ŷ� â�� �� ���� ���� ��� �´�..
    CN3UIString * pStrMy = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_my");
    __ASSERT(pStrMy, "NULL UI Component!!");
    iGold = pStrMy->GetStringAsInt({','});

    //  �Է� â�� ���� ��
    iGoldOffset = m_pUITradeEditDlg->GetQuantity();

    // Gold Offset Backup..
    m_iGoldOffsetBackup = iGoldOffset;

    // ���� ���� ���� ���� ��� �´�..
    iMyMoney = s_pPlayer->m_InfoExt.iGold;

    if (iGoldOffset <= 0) {
        return;
    }
    if (iGoldOffset > iMyMoney) {
        return;
    }

    // ���� ���� ��Ų��..
    iMyMoney -= iGoldOffset;
    s_pPlayer->m_InfoExt.iGold = iMyMoney;

    // �� ǥ��.. �κ��丮..
    szGold = ::_FormatCoins(iMyMoney);
    CN3UIString * pString = NULL;
    pString = (CN3UIString *)CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold");
    __ASSERT(pString, "NULL UI Component!!");
    if (pString) {
        pString->SetString(szGold);
    }
    if (m_pUIPerTradeDlg->m_pStrMyGold) {
        m_pUIPerTradeDlg->m_pStrMyGold->SetString(szGold);
    }

    // �� ǥ��.. ���� �ŷ� â..
    iGold += iGoldOffset;
    pStrMy->SetString(::_FormatCoins(iGold));

    // �������� �����Ѵ�..
    BYTE byBuff[16];  // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    // �������� ��Ŷ ���� ����..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_ADD);
    CAPISocket::MP_AddByte(byBuff, iOffset, 0xff);
    CAPISocket::MP_AddDword(byBuff, iOffset, dwGold);
    CAPISocket::MP_AddInt64(byBuff, iOffset, iGoldOffset);

    CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ����..

    m_pUITradeEditDlg->SetQuantity(0);

    // ���¸� ��ȭ��Ű��.. â�� �ݰ�..
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;
    m_ePerTradeState = PER_TRADE_STATE_NORMAL;
    m_ePerTradeItemKindBackup = PER_TRADE_ITEM_MONEY;
    m_pUITradeEditDlg->Close();

    m_pUIPerTradeDlg->PlayGoldSound();
}

void CSubProcPerTrade::ItemCountEditCancel() {
    m_pUITradeEditDlg->SetQuantity(0);

    // ���¸� ��ȭ��Ű��.. â�� �ݰ�..
    m_ePerTradeState = PER_TRADE_STATE_NORMAL;
    m_pUITradeEditDlg->Close();

    m_pUIPerTradeDlg->PlayGoldSound();
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::PerTradeMyDecision() // ���� �ŷ��� ���� �ߴ�..
{
    // �������� �����Ѵ�..
    BYTE byBuff[4];   // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    // �������� ��Ŷ ���� ����..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_DECIDE);

    CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ����..

    // �� ���� ��ư Disable..
    CN3UIButton * pButton = (CN3UIButton *)m_pUIPerTradeDlg->GetChildButtonByName("btn_trade_my");
    if (pButton) {
        pButton->SetState(UI_STATE_BUTTON_DISABLE);
    }
    SecureJobStuffByMyDecision();
}

void CSubProcPerTrade::SecureJobStuffByMyDecision() {
    m_ePerTradeState = PER_TRADE_STATE_MY_TRADE_DECISION_DONE;

    // 1. �κ��丮 ���� Icon ������ ����..
    PerTradeCoreInvDisable();

    // 2. �� �Է�â Ŭ�� ����..        �ش� �κ�..

    // 3. �÷����� ��ư ����..        �ش� �κ�..
}

void CSubProcPerTrade::PerTradeOtherDecision() // �ٸ� ����� �ŷ��� ���� �ߴ�..
{
    CN3UIButton * pButtonOther = (CN3UIButton *)m_pUIPerTradeDlg->GetChildByID("btn_trade_other");
    __ASSERT(pButtonOther, "NULL UI Component!!");
    pButtonOther->SetState(UI_STATE_BUTTON_DISABLE);
}

///////////////////////////////////////////////////////////////////////

void CSubProcPerTrade::ReceiveMsgPerTradeReq(int iOtherID) {
    EnterWaitMyDecisionToPerTrade(iOtherID); // ���� Ÿ�ο��Լ� ������ �ŷ��� ��û ���� ����..
}

void CSubProcPerTrade::ReceiveMsgPerTradeAgree(BYTE bResult) {
    switch (bResult) {
    case 0x01: // ����..
        ProcessProceed(PER_TRADE_RESULT_OTHER_AGREE);
        break;

    case 0x00: // ����..
        LeavePerTradeState(PER_TRADE_RESULT_OTHER_DISAGREE);
        break;
    }
}

void CSubProcPerTrade::ReceiveMsgPerTradeAdd(BYTE bResult) {
    // ���¸� ��ȭ��Ű��.. â�� �ݰ�..
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;

    std::string szGold;
    int64_t     iGold;    // �ŷ�â�� ��..
    int64_t     iMyMoney; // �κ��丮�� ��..

    switch (bResult) {
    case 0x01:
        break;

    case 0x00: // ���� ���� ���.. �����Ѵ�..
        // ���� �۾��� ������ ������ ���� ���..
        switch (m_ePerTradeItemKindBackup) {
        case PER_TRADE_ITEM_MONEY: {
            // �ŷ� â�� �� ���� ���� ��� �´�..
            CN3UIString * pStrMy = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_my");
            __ASSERT(pStrMy, "NULL UI Component!!");
            iGold = pStrMy->GetStringAsInt({','});

            // ���� ���� ���� ���� ��� �´�..
            iMyMoney = s_pPlayer->m_InfoExt.iGold;
            // ���� ���� ��Ų��..
            iMyMoney += m_iGoldOffsetBackup;
            s_pPlayer->m_InfoExt.iGold = iMyMoney;

            // �� ǥ��.. �κ��丮..
            szGold = ::_FormatCoins(iMyMoney);
            CN3UIString * pString = NULL;
            pString = (CN3UIString *)CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold");
            __ASSERT(pString, "NULL UI Component!!");
            if (pString) {
                pString->SetString(szGold);
            }
            if (m_pUIPerTradeDlg->m_pStrMyGold) {
                m_pUIPerTradeDlg->m_pStrMyGold->SetString(szGold);
            }

            // �� ǥ��.. ���� �ŷ� â..
            iGold -= m_iGoldOffsetBackup;
            pStrMy->SetString(::_FormatCoins(iGold));
        } break;

        case PER_TRADE_ITEM_OTHER: {
            // ���� �۾��� ������ ������ �������� ���..
            if ((m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]
                     ->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                (m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]
                     ->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                // Ȱ�̳� ����� �������� ���..
                bool bFound = false;

                // �κ��丮�� �������� ������ ���.. �����..
                if (!m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]) {
                    bFound = true;
                    // �κ��丮�� ����� �������� ������ �����ش�..
                    __IconItemSkill *spItem, *spItemNew = NULL;
                    spItem = m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

                    // �������� ������ �������� ����� ������ 0����..
                    spItemNew = new __IconItemSkill;
                    spItemNew->pItemBasic = spItem->pItemBasic;
                    spItemNew->pItemExt = spItem->pItemExt;
                    spItemNew->szIconFN = spItem->szIconFN; // ������ ���� �̸� ����..
                    spItemNew->iCount = m_pUIPerTradeDlg->m_iBackupiCount;
                    spItemNew->iDurability = spItem->iDurability;

                    // ������ ���ҽ� �����..
                    spItemNew->pUIIcon = new CN3UIIcon;
                    float fUVAspect = (float)45.0f / (float)64.0f;
                    spItemNew->pUIIcon->Init(m_pUIPerTradeDlg);
                    spItemNew->pUIIcon->SetTex(spItemNew->szIconFN);
                    spItemNew->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
                    spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
                    spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
                    spItemNew->pUIIcon->SetVisible(true);
                    CN3UIArea * pArea;
                    pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(
                        UI_AREA_TYPE_PER_TRADE_INV, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
                    if (pArea) {
                        spItemNew->pUIIcon->SetRegion(pArea->GetRegion());
                        spItemNew->pUIIcon->SetMoveRect(pArea->GetRegion());
                    }

                    m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] =
                        spItemNew;

                    // �� �ŷ�â�� �������� �ִ� ��� .. ���� ����..
                    if (m_pUIPerTradeDlg->m_iBackupiCount !=
                        m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]
                            ->iCount) {
                        m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]
                            ->iCount -= m_pUIPerTradeDlg->m_iBackupiCount;
                    }
                }

                // �������� �� �ŷ�â�� ���Կ� ������.. �����Ѵ�..
                if (m_pUIPerTradeDlg->m_iBackupiCount ==
                    m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount) {
                    bFound = true;

                    __IconItemSkill * spItem;
                    spItem = m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

                    // �Ŵ������� ����..
                    m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

                    // ���ҽ� ����..
                    spItem->pUIIcon->Release();
                    delete spItem->pUIIcon;
                    spItem->pUIIcon = NULL;
                    delete spItem;
                    spItem = NULL;
                    m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

                    // �κ��丮�� �������� �ִ� ���.. ���� ����..
                    if (m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]) {
                        m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]
                            ->iCount += m_pUIPerTradeDlg->m_iBackupiCount;
                    }
                }

                // ������ ���� �����Ѱ� �ƴϸ�.. ������ �����Ѵ�..
                if (!bFound) {
                    // �� �ŷ�â�� �κ��丮�� �������� ������ �����ش�..
                    m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]
                        ->iCount += m_pUIPerTradeDlg->m_iBackupiCount;

                    m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount -=
                        m_pUIPerTradeDlg->m_iBackupiCount;
                }
            } else {
                __IconItemSkill * spItem;
                spItem = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;
                spItem->pUIIcon->SetParent(m_pUIPerTradeDlg);

                // �κ��丮 �����쿡 �����ϰ�..
                m_pUIPerTradeDlg->m_pPerTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItem;

                // �� �ŷ� �����쿡�� Ŭ����..
                m_pUIPerTradeDlg->m_pPerTradeMy[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

                CN3UIArea * pArea;
                pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(
                    UI_AREA_TYPE_PER_TRADE_INV, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
                if (pArea) {
                    spItem->pUIIcon->SetRegion(pArea->GetRegion());
                    spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
                }
            }
        } break;
        }
        break;
    }
}

void CSubProcPerTrade::ReceiveMsgPerTradeOtherAdd(int iItemID, int iCount, int iDurability) {
    int64_t iGold;
    int     iDestiOrder; // �ŷ�â�� ��..

    if (iItemID == dwGold) {
        // �ŷ� â�� �ٸ� ����� ���� ���� ��� �´�..
        CN3UIString * pStrOther = (CN3UIString *)m_pUIPerTradeDlg->GetChildByID("string_money_other");
        __ASSERT(pStrOther, "NULL UI Component!!");
        iGold = pStrOther->GetStringAsInt({','});

        // ���� ���� ����..
        iGold += iCount;

        // �� ǥ��.. ���� �ŷ� â..
        pStrOther->SetString(::_FormatCoins(iGold));
    } else {
        // �������� �� �� �ִ��� Ȯ��, �������� ��� ���� �ڸ� ���..
        bool bFound = false;

        // ������ ���� �ֱ�..
        __TABLE_ITEM_BASIC * pItem = NULL;
        __TABLE_ITEM_EXT *   pItemExt = NULL;
        pItem = s_pTbl_Items_Basic->Find(iItemID / 1000 * 1000); // �� ������ ���..
        if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
            pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemID % 1000);
        }
        if (NULL == pItem || NULL == pItemExt) {
            __ASSERT(0, "������ ������ ���̺� ����!!");
            return;
        }

        int i;
        if ((pItem->byContable == UIITEM_TYPE_COUNTABLE) || (pItem->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
            for (i = 0; i < MAX_ITEM_PER_TRADE; i++) {
                if ((m_pUIPerTradeDlg->m_pPerTradeOther[i]) &&
                    (m_pUIPerTradeDlg->m_pPerTradeOther[i]->pItemBasic->dwID == pItem->dwID)) {
                    bFound = true;
                    iDestiOrder = i;
                    break;
                }
            }

            // ��ã������..
            if (!bFound) {
                // �κ��丮 �󽽷��� ã�� ����..
                for (i = 0; i < MAX_ITEM_PER_TRADE; i++) {
                    if (!m_pUIPerTradeDlg->m_pPerTradeOther[i]) {
                        bFound = true;
                        iDestiOrder = i;
                        break;
                    }
                }

                if (!bFound) { // �� ������ ã�� ��������..
                    return;
                }
            }

            if (m_pUIPerTradeDlg->m_pPerTradeOther[iDestiOrder]) // �ش� ��ġ�� �������� ������..
            {
                m_pUIPerTradeDlg->m_pPerTradeOther[iDestiOrder]->iCount += iCount;
            } else {
                std::string    szIconFN;
                e_PartPosition ePart;
                e_PlugPosition ePlug;
                CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart,
                                                        ePlug); // �����ۿ� ���� ���� �̸��� ����

                __IconItemSkill * spItem;

                spItem = new __IconItemSkill;
                spItem->pItemBasic = pItem;
                spItem->pItemExt = pItemExt;
                spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
                spItem->iCount = iCount;
                spItem->iDurability = iDurability;
                float fUVAspect = (float)45.0f / (float)64.0f;
                spItem->pUIIcon = new CN3UIIcon;
                spItem->pUIIcon->Init(m_pUIPerTradeDlg);
                spItem->pUIIcon->SetTex(szIconFN);
                spItem->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
                spItem->pUIIcon->SetUIType(UI_TYPE_ICON);
                spItem->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
                spItem->pUIIcon->SetVisible(true);

                CN3UIArea * pArea = NULL;
                pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(UI_AREA_TYPE_PER_TRADE_OTHER, i);
                if (pArea) {
                    spItem->pUIIcon->SetRegion(pArea->GetRegion());
                    // ������ �� ����..
                    RECT rect = {0, 0, 0, 0};
                    spItem->pUIIcon->SetMoveRect(rect);
                }

                m_pUIPerTradeDlg->m_pPerTradeOther[i] = spItem;
            }
        } else {
            for (i = 0; i < MAX_ITEM_PER_TRADE; i++) {
                if (m_pUIPerTradeDlg->m_pPerTradeOther[i] == NULL) {
                    bFound = true;
                    break;
                }
            }

            if (!bFound) {
                return; // �� ã�����Ƿ�.. ����..
            }

            std::string    szIconFN;
            e_PartPosition ePart;
            e_PlugPosition ePlug;
            CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart,
                                                    ePlug); // �����ۿ� ���� ���� �̸��� ����

            __IconItemSkill * spItem;

            spItem = new __IconItemSkill;
            spItem->pItemBasic = pItem;
            spItem->pItemExt = pItemExt;
            spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
            spItem->iCount = 1;
            spItem->iDurability = iDurability;
            float fUVAspect = (float)45.0f / (float)64.0f;
            spItem->pUIIcon = new CN3UIIcon;
            spItem->pUIIcon->Init(m_pUIPerTradeDlg);
            spItem->pUIIcon->SetTex(szIconFN);
            spItem->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
            spItem->pUIIcon->SetUIType(UI_TYPE_ICON);
            spItem->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
            spItem->pUIIcon->SetVisible(true);

            CN3UIArea * pArea = NULL;
            pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(UI_AREA_TYPE_PER_TRADE_OTHER, i);
            if (pArea) {
                spItem->pUIIcon->SetRegion(pArea->GetRegion());
                // ������ �� ����..
                RECT rect = {0, 0, 0, 0};
                spItem->pUIIcon->SetMoveRect(rect);
            }

            if (spItem->iDurability == 0) {
                spItem->pUIIcon->SetStyle(spItem->pUIIcon->GetStyle() | UISTYLE_DURABILITY_EXHAUST);
            }

            m_pUIPerTradeDlg->m_pPerTradeOther[i] = spItem;
        }
    }
}

void CSubProcPerTrade::ReceiveMsgPerTradeOtherDecide() {
    PerTradeOtherDecision();
}

void CSubProcPerTrade::ReceiveMsgPerTradeDoneSuccessBegin(int64_t iTotalGold) {
    CN3UIString * pString = NULL;
    pString = (CN3UIString *)CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold");
    __ASSERT(pString, "NULL UI Component!!");
    pString->SetString(::_FormatCoins(iTotalGold));
    s_pPlayer->m_InfoExt.iGold = iTotalGold;
}

void CSubProcPerTrade::ReceiveMsgPerTradeDoneItemMove(BYTE bItemPos, int iItemID, int iCount, int iDurability) {
    __IconItemSkill * spItem;

    // ������ �κ��丮 INV ������ �߰�..
    // �������� �� �� �ִ��� Ȯ��..
    if (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos] != NULL) {
        if (iItemID != (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->pItemBasic->dwID / 1000 * 1000) +
                           (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->pItemExt->dwID % 1000)) {
            // ���� �������� Ŭ����..
            spItem = m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos];

            // �Ŵ������� ����..
            m_pUIPerTradeDlg->RemoveChild(spItem->pUIIcon);

            // ���ҽ� ����..
            spItem->pUIIcon->Release();
            delete spItem->pUIIcon;
            spItem->pUIIcon = NULL;
            delete spItem;
            spItem = NULL;
            m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos] = NULL;

            // ������ �� ���̵�� �������� �����..
            goto Make_Icon;
        }

        switch (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->pItemBasic->byContable) {
        case UIITEM_TYPE_COUNTABLE: // ȭ�� ���� ����..
            m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount += iCount;
            if (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount > UIITEM_COUNT_MANY) {
                m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount = UIITEM_COUNT_MANY;
            }
            return;
            break;

        case UIITEM_TYPE_COUNTABLE_SMALL: // ���ర�� ����..
            m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount += iCount;
            if (m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount > UIITEM_COUNT_FEW) {
                m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos]->iCount = UIITEM_COUNT_FEW;
            }
            return;
            break;
        }
    }

Make_Icon:

    // ������ ���� �ֱ�..
    __TABLE_ITEM_BASIC * pItem = NULL;
    __TABLE_ITEM_EXT *   pItemExt = NULL;
    pItem = s_pTbl_Items_Basic->Find(iItemID / 1000 * 1000); // �� ������ ���..
    if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
        pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemID % 1000);
    }
    if (NULL == pItem || NULL == pItemExt) {
        __ASSERT(0, "������ ������ ���̺� ����!!");
        return;
    }

    std::string    szIconFN;
    e_PartPosition ePart;
    e_PlugPosition ePlug;
    CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����

    spItem = new __IconItemSkill;
    spItem->pItemBasic = pItem;
    spItem->pItemExt = pItemExt;
    spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
    spItem->iCount = iCount;
    spItem->iDurability = iDurability;

    float fUVAspect = (float)45.0f / (float)64.0f;
    spItem->pUIIcon = new CN3UIIcon;
    spItem->pUIIcon->Init(m_pUIPerTradeDlg);
    spItem->pUIIcon->SetTex(szIconFN);
    spItem->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
    spItem->pUIIcon->SetUIType(UI_TYPE_ICON);
    spItem->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
    spItem->pUIIcon->SetVisible(true);

    CN3UIArea * pArea = NULL;
    pArea = m_pUIPerTradeDlg->GetChildAreaByiOrder(UI_AREA_TYPE_PER_TRADE_INV, bItemPos);
    if (pArea) {
        spItem->pUIIcon->SetRegion(pArea->GetRegion());
        spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
    }

    if (spItem->iDurability == 0) {
        spItem->pUIIcon->SetStyle(spItem->pUIIcon->GetStyle() | UISTYLE_DURABILITY_EXHAUST);
    }

    m_pUIPerTradeDlg->m_pPerTradeInv[bItemPos] = spItem;
}

void CSubProcPerTrade::ReceiveMsgPerTradeDoneSuccessEnd() {
    // �� ������..
    PerTradeCompleteSuccess();
    FinalizePerTrade();

    if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) {
        CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
    }
    if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) {
        CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
    }
}

void CSubProcPerTrade::ReceiveMsgPerTradeDoneFail() {
    // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
    if (s_pOPMgr->UPCGetByID(m_iOtherID, false) != NULL) {
        std::string szMsg;
        ::_LoadStringFromResource(IDS_PER_TRADE_FAIL, szMsg);
        CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffffffff);

        // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
        ::_LoadStringFromResource(IDS_ITEM_TOOMANY_OR_HEAVY, szMsg);
        CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
    }

    // �� ������..
    PerTradeCompleteCancel();
    FinalizePerTrade();
}

void CSubProcPerTrade::ReceiveMsgPerTradeCancel() {
    // �޽��� �ڽ� �ؽ�Ʈ ǥ��..
    if (s_pOPMgr->UPCGetByID(m_iOtherID, false) != NULL) {
        std::string szFmt;
        ::_LoadStringFromResource(IDS_OTHER_PER_TRADE_CANCEL, szFmt);
        char szBuf[256]{};
        sprintf(szBuf, szFmt.c_str(), (s_pOPMgr->UPCGetByID(m_iOtherID, false))->IDString().c_str());
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
    }

    // �� ������..
    PerTradeCompleteCancel();
    FinalizePerTrade();
}

void CSubProcPerTrade::ItemCountOK() {
    m_pUIPerTradeDlg->ItemCountOK();
}

void CSubProcPerTrade::ItemCountCancel() {
    m_pUIPerTradeDlg->ItemCountCancel();
}
