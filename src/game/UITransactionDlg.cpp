// UITransactionDlg.cpp: implementation of the CUITransactionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "GameProcMain.h"

#include "UITransactionDlg.h"
#include "UIImageTooltipDlg.h"
#include "UIInventory.h"
#include "UIManager.h"
#include "PlayerMySelf.h"

#include "CountableItemEditDlg.h"

#include "UIHotKeyDlg.h"
#include "UISkillTreeDlg.h"

#include "N3Base/N3UIString.h"
#include "N3Base/N3UIEdit.h"
#include "N3Base/N3SndObj.h"
#include "Resource.h"

#include "KnightChrMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUITransactionDlg::CUITransactionDlg() {
    m_iCurPage = 0;
    for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
        for (int i = 0; i < MAX_ITEM_TRADE; i++) {
            m_pMyTrade[j][i] = NULL;
        }
    }
    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        m_pMyTradeInv[i] = NULL;
    }

    m_pUITooltipDlg = NULL;
    m_pStrMyGold = NULL;

    m_pUIInn = NULL;
    m_pUIBlackSmith = NULL;
    m_pUIStore = NULL;

    this->SetVisible(false);
}

CUITransactionDlg::~CUITransactionDlg() {
    Release();
}

void CUITransactionDlg::Release() {
    CN3UIBase::Release();

    for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
        for (int i = 0; i < MAX_ITEM_TRADE; i++) {
            if (m_pMyTrade[j][i] != NULL) {
                delete m_pMyTrade[j][i];
                m_pMyTrade[j][i] = NULL;
            }
        }
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (m_pMyTradeInv[i] != NULL) {
            delete m_pMyTradeInv[i];
            m_pMyTradeInv[i] = NULL;
        }
    }
}

void CUITransactionDlg::Render() {
    if (!m_bVisible) {
        return; // 보이지 않으면 자식들을 render하지 않는다.
    }

    POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
    m_pUITooltipDlg->DisplayTooltipsDisable();

    bool              bTooltipRender = false;
    __IconItemSkill * spItem = NULL;

    for (UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor) {
        CN3UIBase * pChild = (*itor);
        if ((GetState() == UI_STATE_ICON_MOVING) && (pChild->UIType() == UI_TYPE_ICON) &&
            (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) &&
            ((CN3UIIcon *)pChild == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon)) {
            continue;
        }
        pChild->Render();
        if ((GetState() == UI_STATE_COMMON_NONE) && (pChild->UIType() == UI_TYPE_ICON) &&
            (pChild->GetStyle() & UISTYLE_ICON_HIGHLIGHT)) {
            bTooltipRender = true;
            spItem = GetHighlightIconItem((CN3UIIcon *)pChild);
        }
    }

    // 갯수 표시되야 할 아이템 갯수 표시..
    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (m_pMyTradeInv[i] && ((m_pMyTradeInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                                 (m_pMyTradeInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL))) {
            // string 얻기..
            CN3UIString * pStr = GetChildStringByiOrder(i);
            if (pStr) {
                if ((GetState() == UI_STATE_ICON_MOVING) &&
                    (m_pMyTradeInv[i] == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect)) {
                    pStr->SetVisible(false);
                } else {
                    if (m_pMyTradeInv[i]->pUIIcon->IsVisible()) {
                        pStr->SetVisible(true);
                        pStr->SetStringAsInt(m_pMyTradeInv[i]->iCount);
                        pStr->Render();
                    } else {
                        pStr->SetVisible(false);
                    }
                }
            }
        } else {
            // string 얻기..
            CN3UIString * pStr = GetChildStringByiOrder(i);
            if (pStr) {
                pStr->SetVisible(false);
            }
        }
    }

    if ((GetState() == UI_STATE_ICON_MOVING) && (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect)) {
        CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->Render();
    }

    if (bTooltipRender && spItem) {
        e_UIWND_DISTRICT eUD = GetWndDistrict(spItem);
        switch (eUD) {
        case UIWND_DISTRICT_TRADE_NPC:
            m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, spItem, true, true);
            break;
        case UIWND_DISTRICT_TRADE_MY:
            m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, spItem, true, false);
            break;
        }
    }
}

void CUITransactionDlg::InitIconWnd(e_UIWND eWnd) {
    __TABLE_UI_RESRC * pTbl = CGameBase::s_pTbl_UI->Find(CGameBase::s_pPlayer->m_InfoBase.eNation);

    m_pUITooltipDlg = new CUIImageTooltipDlg();
    m_pUITooltipDlg->Init(this);
    m_pUITooltipDlg->LoadFromFile(pTbl->szItemInfo);
    m_pUITooltipDlg->InitPos();
    m_pUITooltipDlg->SetVisible(FALSE);

    CN3UIWndBase::InitIconWnd(eWnd);

    m_pStrMyGold = (CN3UIString *)GetChildByID("string_item_name");
    __ASSERT(m_pStrMyGold, "NULL UI Component!!");
    if (m_pStrMyGold) {
        m_pStrMyGold->SetString("0");
    }

    m_pUIInn = (CN3UIImage *)GetChildByID("img_inn");
    __ASSERT(m_pUIInn, "NULL UI Component!!");
    m_pUIBlackSmith = (CN3UIImage *)GetChildByID("img_blacksmith");
    __ASSERT(m_pUIBlackSmith, "NULL UI Component!!");
    m_pUIStore = (CN3UIImage *)GetChildByID("img_store");
    __ASSERT(m_pUIStore, "NULL UI Component!!");
}

void CUITransactionDlg::InitIconUpdate() {
    CN3UIArea * pArea;
    float       fUVAspect = (float)45.0f / (float)64.0f;

    for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
        for (int i = 0; i < MAX_ITEM_TRADE; i++) {
            if (m_pMyTrade[j][i] != NULL) {
                m_pMyTrade[j][i]->pUIIcon = new CN3UIIcon;
                m_pMyTrade[j][i]->pUIIcon->Init(this);
                m_pMyTrade[j][i]->pUIIcon->SetTex(m_pMyTrade[j][i]->fsIconFile);
                m_pMyTrade[j][i]->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
                m_pMyTrade[j][i]->pUIIcon->SetUIType(UI_TYPE_ICON);
                m_pMyTrade[j][i]->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
                pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, i);
                if (pArea) {
                    m_pMyTrade[j][i]->pUIIcon->SetRegion(pArea->GetRegion());
                    m_pMyTrade[j][i]->pUIIcon->SetMoveRect(pArea->GetRegion());
                }
            }
        }
    }
}

__IconItemSkill * CUITransactionDlg::GetHighlightIconItem(CN3UIIcon * pUIIcon) {
    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
        if ((m_pMyTrade[m_iCurPage][i] != NULL) && (m_pMyTrade[m_iCurPage][i]->pUIIcon == pUIIcon)) {
            return m_pMyTrade[m_iCurPage][i];
        }
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if ((m_pMyTradeInv[i] != NULL) && (m_pMyTradeInv[i]->pUIIcon == pUIIcon)) {
            return m_pMyTradeInv[i];
        }
    }

    return NULL;
}

void CUITransactionDlg::EnterTransactionState() {
    for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
        for (int i = 0; i < MAX_ITEM_TRADE; i++) {
            if (m_pMyTrade[j][i] != NULL) {
                if (m_pMyTrade[j][i]->pUIIcon) {
                    RemoveChild(m_pMyTrade[j][i]->pUIIcon);
                    m_pMyTrade[j][i]->pUIIcon->Release();
                    delete m_pMyTrade[j][i]->pUIIcon;
                    m_pMyTrade[j][i]->pUIIcon = NULL;
                }
                delete m_pMyTrade[j][i];
                m_pMyTrade[j][i] = NULL;
            }
        }
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (m_pMyTradeInv[i] != NULL) {
            if (m_pMyTradeInv[i]->pUIIcon) {
                RemoveChild(m_pMyTradeInv[i]->pUIIcon);
                m_pMyTradeInv[i]->pUIIcon->Release();
                delete m_pMyTradeInv[i]->pUIIcon;
                m_pMyTradeInv[i]->pUIIcon = NULL;
            }
            delete m_pMyTradeInv[i];
            m_pMyTradeInv[i] = NULL;
        }
    }

    fs::path             fsIconFile;
    __IconItemSkill *    spItem = NULL;
    __TABLE_ITEM_BASIC * pItem = NULL; // 아이템 테이블 구조체 포인터..
    __TABLE_ITEM_EXT *   pItemExt = NULL;

    int iOrg = m_iTradeID / 1000;
    int iExt = m_iTradeID % 1000;
    int iSize = CGameBase::s_pTbl_Items_Basic->GetSize();

    for (int i = 0, j = 0, k = 0; i < iSize; i++) {
        if (k >= MAX_ITEM_TRADE) {
            j++;
            k = 0;
        }

        if (j >= MAX_ITEM_TRADE_PAGE) {
            break;
        }

        pItem = CGameBase::s_pTbl_Items_Basic->GetIndexedData(i);
        if (NULL == pItem) // 아이템이 없으면..
        {
            __ASSERT(0, "아이템 포인터 테이블에 없음!!");
            CLogWriter::Write("CUITransactionDlg::EnterTransactionState - Invalid Item ID : %d, %d", iOrg, iExt);
            continue;
        }

        if (pItem->byExtIndex < 0 || pItem->byExtIndex >= MAX_ITEM_EXTENSION) {
            continue;
        }

        if (pItem->bySellGroup != iOrg) {
            continue;
        }

        pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iExt);
        if (NULL == pItemExt) // 아이템이 없으면..
        {
            __ASSERT(0, "아이템 포인터 테이블에 없음!!");
            CLogWriter::Write("CUITransactionDlg::EnterTransactionState - Invalid Item ID : %d, %d", iOrg, iExt);
            continue;
        }

        if (pItemExt->dwID != iExt) {
            continue;
        }

        e_PartPosition ePart;
        e_PlugPosition ePlug;
        e_ItemType     eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &fsIconFile, ePart,
                                                                       ePlug); // 아이템에 따른 파일 이름을 만들어서
        __ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item");

        spItem = new __IconItemSkill;
        spItem->pItemBasic = pItem;
        spItem->pItemExt = pItemExt;
        spItem->fsIconFile = fsIconFile; // 아이콘 파일 이름 복사..
        spItem->iCount = 1;
        spItem->iDurability = pItem->siMaxDurability + pItemExt->siMaxDurability;

        m_pMyTrade[j][k] = spItem;
        k++;
    }

    InitIconUpdate();
    m_iCurPage = 0;
    CN3UIString * pStr = (CN3UIString *)GetChildByID("string_page");
    if (pStr) {
        pStr->SetStringAsInt(m_iCurPage + 1);
    }

    for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
        if (j == m_iCurPage) {
            for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                if (m_pMyTrade[j][i] != NULL) {
                    m_pMyTrade[j][i]->pUIIcon->SetVisible(true);
                }
            }
        } else {
            for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                if (m_pMyTrade[j][i] != NULL) {
                    m_pMyTrade[j][i]->pUIIcon->SetVisible(false);
                }
            }
        }
    }

    ItemMoveFromInvToThis();

    if (m_pStrMyGold) {
        __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);
        m_pStrMyGold->SetString(::_FormatCoins(pInfoExt->iGold));
    }

    switch ((int)(m_iTradeID / 1000)) {
    case 122:
    case 222:
        ShowTitle(UI_BLACKSMITH);
        break;
    default:
        ShowTitle(UI_STORE);
        break;
    }

    CGameProcedure::s_pKnightChr->SendActionCommand(32);
}

void CUITransactionDlg::GoldUpdate() {
    if (m_pStrMyGold) {
        m_pStrMyGold->SetString(::_FormatCoins(CGameBase::s_pPlayer->m_InfoExt.iGold));
    }
}

void CUITransactionDlg::ItemMoveFromInvToThis() {
    CUIInventory * pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
    if (!pInven) {
        return;
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        m_pMyTradeInv[i] = NULL;
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (pInven->m_pMyInvWnd[i]) {
            __IconItemSkill * spItem = pInven->m_pMyInvWnd[i];
            spItem->pUIIcon->SetParent(this);

            pInven->m_pMyInvWnd[i] = NULL;
            CN3UIArea * pArea;

            pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, i);
            if (pArea) {
                spItem->pUIIcon->SetRegion(pArea->GetRegion());
                spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
            }

            m_pMyTradeInv[i] = spItem;
        }
    }
}

void CUITransactionDlg::LeaveTransactionState() {
    if (IsVisible()) {
        SetVisible(false);
    }

    if (GetState() == UI_STATE_ICON_MOVING) {
        IconRestore();
    }
    SetState(UI_STATE_COMMON_NONE);
    CN3UIWndBase::AllHighLightIconFree();

    // 이 윈도우의 inv 영역의 아이템을 이 인벤토리 윈도우의 inv영역으로 옮긴다..
    ItemMoveFromThisToInv();

    if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) {
        CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
    }
    if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) {
        CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
    }
}

void CUITransactionDlg::ItemMoveFromThisToInv() {
    CUIInventory * pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
    if (!pInven) {
        return;
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if (m_pMyTradeInv[i]) {
            __IconItemSkill * spItem = m_pMyTradeInv[i];
            spItem->pUIIcon->SetParent(pInven);

            m_pMyTradeInv[i] = NULL;

            CN3UIArea * pArea;

            pArea = pInven->GetChildAreaByiOrder(UI_AREA_TYPE_INV, i);
            if (pArea) {
                spItem->pUIIcon->SetRegion(pArea->GetRegion());
                spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
            }

            pInven->m_pMyInvWnd[i] = spItem;
        }
    }
}

void CUITransactionDlg::ItemCountOK() {
    int64_t              iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();
    __IconItemSkill *    spItem, *spItemNew = NULL;
    __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);
    int                  iWeight;

    switch (CN3UIWndBase::m_pCountableItemEdit->GetCallerWndDistrict()) {
    case UIWND_DISTRICT_TRADE_NPC: // 사는 경우..
        spItem = m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];

        switch (spItem->pItemBasic->byContable) {
        case UIITEM_TYPE_ONLYONE:
        case UIITEM_TYPE_SOMOONE:
            iWeight = spItem->pItemBasic->siWeight;

            // 무게 체크..
            if ((pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }
            break;

        case UIITEM_TYPE_COUNTABLE:
            if (iGold <= 0) {
                return;
            }
            // short 범위이상은 살수 없다..
            if (iGold > UIITEM_COUNT_MANY) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_BUY_FAIL, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            if (spItem->iCount + iGold > UIITEM_COUNT_MANY) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_GET_MANY, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            // 매수가 X 갯수가 내가 가진 돈보다 많으면.. 그냥 리턴..
            if ((iGold * spItem->pItemBasic->iPrice) > pInfoExt->iGold) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_COUNTABLE_ITEM_BUY_NOT_ENOUGH_MONEY, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            iWeight = iGold * spItem->pItemBasic->siWeight;

            // 무게 체크..
            if ((pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }
            break;

        case UIITEM_TYPE_COUNTABLE_SMALL:
            if (iGold <= 0) {
                return;
            }
            // short 범위이상은 살수 없다..
            if (iGold > UIITEM_COUNT_FEW) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_BUY_FAIL, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            if (spItem->iCount + iGold > UIITEM_COUNT_FEW) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_GET_MANY, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            // 매수가 X 갯수가 내가 가진 돈보다 많으면.. 그냥 리턴..
            if ((iGold * spItem->pItemBasic->iPrice) > pInfoExt->iGold) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_COUNTABLE_ITEM_BUY_NOT_ENOUGH_MONEY, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }

            iWeight = iGold * spItem->pItemBasic->siWeight;

            // 무게 체크..
            if ((pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                return;
            }
            break;
        }

        CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;

        if (m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]) // 해당 위치에 아이콘이 있으면..
        {
            //  숫자 업데이트..
            m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount += iGold;

            // 표시는 아이콘 렌더링할때.. Inventory의 Render에서..
            // 서버에게 보냄..
            SendToServerBuyMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                   CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                               CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder, iGold);
        } else {
            spItem = m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
            CN3UIArea * pArea;
            spItemNew = new __IconItemSkill;
            spItemNew->pItemBasic = spItem->pItemBasic;
            spItemNew->pItemExt = spItem->pItemExt;
            spItemNew->fsIconFile = spItem->fsIconFile; // 아이콘 파일 이름 복사..
            spItemNew->iCount = iGold;
            spItemNew->iDurability = spItem->pItemBasic->siMaxDurability + spItem->pItemExt->siMaxDurability;

            // 아이콘 리소스 만들기..
            spItemNew->pUIIcon = new CN3UIIcon;
            float fUVAspect = (float)45.0f / (float)64.0f;
            spItemNew->pUIIcon->Init(this);
            spItemNew->pUIIcon->SetTex(spItemNew->fsIconFile);
            spItemNew->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
            spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
            spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
            spItemNew->pUIIcon->SetVisible(true);
            pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY,
                                                       CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
            if (pArea) {
                spItemNew->pUIIcon->SetRegion(pArea->GetRegion());
                spItemNew->pUIIcon->SetMoveRect(pArea->GetRegion());
            }

            m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemNew;

            // 서버에게 보냄..
            SendToServerBuyMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                   CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                               CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder, iGold);
        }
        // Sound..
        if (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic) {
            PlayItemSound(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic);
        }
        break;

    case UIWND_DISTRICT_TRADE_MY: //  파는 경우..
        spItem = m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];

        if (iGold <= 0) {
            return;
        }
        if (iGold > spItem->iCount) {
            return;
        }

        CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;

        if ((spItem->iCount - iGold) > 0) {
            //  숫자 업데이트..
            spItem->iCount -= iGold;
        } else {
            spItem->pUIIcon->SetVisible(false);
        }

        // 서버에게 보냄..
        SendToServerSellMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                            CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, iGold);
        break;
    }

    CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUITransactionDlg::ItemCountCancel() {
    // Sound..
    if (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic) {
        PlayItemSound(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic);
    }

    // 취소..
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
    CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;

    CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUITransactionDlg::SendToServerSellMsg(int itemID, uint8_t pos, int iCount) {
    BYTE byBuff[32];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_TRADE_SELL);
    CAPISocket::MP_AddDword(byBuff, iOffset, itemID);
    CAPISocket::MP_AddByte(byBuff, iOffset, pos);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iCount);

    CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUITransactionDlg::SendToServerBuyMsg(int itemID, uint8_t pos, int iCount) {
    BYTE byBuff[32];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_TRADE_BUY);
    CAPISocket::MP_AddDword(byBuff, iOffset, m_iTradeID);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_iNpcID);
    CAPISocket::MP_AddDword(byBuff, iOffset, itemID);
    CAPISocket::MP_AddByte(byBuff, iOffset, pos);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iCount);

    CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUITransactionDlg::SendToServerMoveMsg(int itemID, uint8_t startpos, uint8_t destpos) {
    BYTE byBuff[32];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_TRADE_MOVE);
    CAPISocket::MP_AddDword(byBuff, iOffset, itemID);
    CAPISocket::MP_AddByte(byBuff, iOffset, startpos);
    CAPISocket::MP_AddByte(byBuff, iOffset, destpos);

    CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUITransactionDlg::ReceiveResultTradeMoveSuccess() {
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
    CN3UIWndBase::AllHighLightIconFree();
    SetState(UI_STATE_COMMON_NONE);
}

void CUITransactionDlg::ReceiveResultTradeMoveFail() {
    CN3UIArea * pArea;
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;

    __IconItemSkill *spItemSource = NULL, *spItemTarget = NULL;
    spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;
    spItemTarget = CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget;

    if (spItemSource) {
        pArea = NULL;
        pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
        if (pArea) {
            spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
            spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
        }

        m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemSource;
    }

    if (spItemTarget) {
        pArea = NULL;
        pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
        if (pArea) {
            spItemTarget->pUIIcon->SetRegion(pArea->GetRegion());
            spItemTarget->pUIIcon->SetMoveRect(pArea->GetRegion());
        }

        m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemTarget;
    } else {
        m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;
    }

    CN3UIWndBase::AllHighLightIconFree();
    SetState(UI_STATE_COMMON_NONE);
}

void CUITransactionDlg::ReceiveItemDropByTradeSuccess() {
    // 원래 아이템을 삭제해야 하지만.. 되살릴 방법이 없기 때문에 원래 위치로 옮기고..
    __IconItemSkill * spItem;
    spItem = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;

    CN3UIArea * pArea;

    pArea = CGameProcedure::s_pProcMain->m_pUIInventory->GetChildAreaByiOrder(
        UI_AREA_TYPE_INV, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
    if (pArea) {
        spItem->pUIIcon->SetRegion(pArea->GetRegion());
        spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
    }

    // Invisible로 하고 삭제는 서버가 성공을 줄때 한다..
    spItem->pUIIcon->SetVisible(false);

    if ((spItem->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
        (spItem->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
        // 활이나 물약등 아이템인 경우..
        spItem->pUIIcon->SetVisible(true);
    }
}

bool CUITransactionDlg::ReceiveIconDrop(__IconItemSkill * spItem, POINT ptCur) {
// Temp Define
#define FAIL_RETURN                                                                                                    \
    {                                                                                                                  \
        CN3UIWndBase::AllHighLightIconFree();                                                                          \
        SetState(UI_STATE_COMMON_NONE);                                                                                \
        return false;                                                                                                  \
    }

    CN3UIArea *      pArea;
    e_UIWND_DISTRICT eUIWnd = UIWND_DISTRICT_UNKNOWN;
    if (!m_bVisible) {
        return false;
    }

    // 내가 가졌던 아이콘이 아니면..
    if (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd != m_eUIWnd) {
        FAIL_RETURN
    }
    if ((CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict != UIWND_DISTRICT_TRADE_NPC) &&
        (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict != UIWND_DISTRICT_TRADE_MY)) {
        FAIL_RETURN
    }

    // 내가 가졌던 아이콘이면.. npc영역인지 검사한다..
    int  i, iDestiOrder = -1;
    bool bFound = false;
    for (i = 0; i < MAX_ITEM_TRADE; i++) {
        pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, i);
        if ((pArea) && (pArea->IsIn(ptCur.x, ptCur.y))) {
            bFound = true;
            eUIWnd = UIWND_DISTRICT_TRADE_NPC;
            break;
        }
    }

    if (!bFound) {
        for (i = 0; i < MAX_ITEM_INVENTORY; i++) {
            pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, i);
            if ((pArea) && (pArea->IsIn(ptCur.x, ptCur.y))) {
                bFound = true;
                eUIWnd = UIWND_DISTRICT_TRADE_MY;
                break;
            }
        }
    }

    if (!bFound) {
        FAIL_RETURN
    }

    // 같은 윈도우 내에서의 움직임은 fail!!!!!
    if ((eUIWnd == CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict) &&
        (eUIWnd != UIWND_DISTRICT_TRADE_MY)) {
        FAIL_RETURN
    }

    // 본격적으로 Recovery Info를 활용하기 시작한다..
    // 먼저 WaitFromServer를 On으로 하고.. Select Info를 Recovery Info로 복사.. 이때 Dest는 팰요없다..
    if (spItem != CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) {
        CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;
    }

    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;
    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = CN3UIWndBase::m_sSelectedIconInfo.pItemSelect;
    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWnd = CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd;
    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWndDistrict =
        CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict;
    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder = CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder;
    CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;

    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWnd = UIWND_TRANSACTION;
    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWndDistrict = eUIWnd;

    for (i = 0; i < MAX_ITEM_INVENTORY; i++) {
        pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, i);
        if (pArea && pArea->IsIn(ptCur.x, ptCur.y)) {
            iDestiOrder = i;
            break;
        }
    }

    switch (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict) {
    case UIWND_DISTRICT_TRADE_NPC:
        if (eUIWnd == UIWND_DISTRICT_TRADE_MY) // 사는 경우..
        {
            if ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                // 활이나 물약등 아이템인 경우..
                // 면저 인벤토리에 해당 아이콘이 있는지 알아본다..
                bFound = false;

                for (i = 0; i < MAX_ITEM_INVENTORY; i++) {
                    if (bFound) {
                        CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iDestiOrder;
                        break;
                    }

                    if ((m_pMyTradeInv[i]) &&
                        (m_pMyTradeInv[i]->pItemBasic->dwID ==
                         CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->dwID) &&
                        (m_pMyTradeInv[i]->pItemExt->dwID ==
                         CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemExt->dwID)) {
                        bFound = true;
                        iDestiOrder = i;
                    }
                }

                // 못찾았으면..
                if (!bFound) {
                    if (m_pMyTradeInv[iDestiOrder]) // 해당 위치에 아이콘이 있으면..
                    {
                        // 인벤토리 빈슬롯을 찾아 들어간다..
                        for (i = 0; i < MAX_ITEM_INVENTORY; i++) {
                            if (!m_pMyTradeInv[i]) {
                                bFound = true;
                                iDestiOrder = i;
                                break;
                            }
                        }

                        if (!bFound) // 빈 슬롯을 찾지 못했으면..
                        {
                            CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                            CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
                            CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
                            FAIL_RETURN
                        }
                    }
                }

                CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iDestiOrder;

                CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                CN3UIWndBase::m_pCountableItemEdit->Open(
                    UIWND_TRANSACTION, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict, false);
                FAIL_RETURN
            } else {
                __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

                // 매수가 X 갯수가 내가 가진 돈보다 많으면.. 그냥 리턴..
                if ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->iPrice) > pInfoExt->iGold) {
                    std::string szMsg;
                    ::_LoadStringFromResource(IDS_COUNTABLE_ITEM_BUY_NOT_ENOUGH_MONEY, szMsg);
                    CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
                    CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
                    FAIL_RETURN
                }

                // 무게 체크..
                if ((pInfoExt->iWeight + CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->siWeight) >
                    pInfoExt->iWeightMax) {
                    std::string szMsg;
                    ::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);
                    CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
                    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
                    CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
                    FAIL_RETURN
                }

                // 일반 아이템인 경우..
                if (m_pMyTradeInv[iDestiOrder]) // 해당 위치에 아이콘이 있으면..
                {
                    // 인벤토리 빈슬롯을 찾아 들어간다..
                    bFound = false;
                    for (i = 0; i < MAX_ITEM_INVENTORY; i++) {
                        if (!m_pMyTradeInv[i]) {
                            bFound = true;
                            iDestiOrder = i;
                            break;
                        }
                    }

                    if (!bFound) // 빈 슬롯을 찾지 못했으면..
                    {
                        CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                        CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
                        CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
                        FAIL_RETURN
                    }

                    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iDestiOrder;
                } else {
                    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iDestiOrder;
                }

                SendToServerBuyMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                       CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                                   iDestiOrder, CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->iCount);

                fs::path       fsIconFile;
                e_PartPosition ePart;
                e_PlugPosition ePlug;
                CGameProcedure::MakeResrcFileNameForUPC(
                    m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pItemBasic, NULL,
                    &fsIconFile, ePart, ePlug); // 아이템에 따른 파일 이름을 만들어서

                __IconItemSkill * spItemNew;
                spItemNew = new __IconItemSkill;
                spItemNew->pItemBasic =
                    m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pItemBasic;
                spItemNew->pItemExt =
                    m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pItemExt;
                spItemNew->fsIconFile = fsIconFile; // 아이콘 파일 이름 복사..
                spItemNew->iCount = 1;
                spItemNew->iDurability =
                    m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]
                        ->pItemBasic->siMaxDurability +
                    m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]
                        ->pItemExt->siMaxDurability;

                // 아이콘 리소스 만들기..
                spItemNew->pUIIcon = new CN3UIIcon;
                float fUVAspect = (float)45.0f / (float)64.0f;
                spItemNew->pUIIcon->Init(this);
                spItemNew->pUIIcon->SetTex(fsIconFile);
                spItemNew->pUIIcon->SetUVRect(0, 0, fUVAspect, fUVAspect);
                spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
                spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
                spItemNew->pUIIcon->SetVisible(true);
                pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, iDestiOrder);
                if (pArea) {
                    spItemNew->pUIIcon->SetRegion(pArea->GetRegion());
                    spItemNew->pUIIcon->SetMoveRect(pArea->GetRegion());
                }

                m_pMyTradeInv[iDestiOrder] = spItemNew;
                FAIL_RETURN
            }
        } else {
            CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
            CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = NULL;
            CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
            FAIL_RETURN
        }
        break;

    case UIWND_DISTRICT_TRADE_MY:
        if (eUIWnd == UIWND_DISTRICT_TRADE_NPC) // 파는 경우..
        {
            if ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                // 활이나 물약등 아이템인 경우..
                CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
                CN3UIWndBase::m_pCountableItemEdit->Open(
                    UIWND_TRANSACTION, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict, false);
            } else {
                // Server에게 보낸다..
                SendToServerSellMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                        CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                                    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder,
                                    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->iCount);

                // 원래 아이템을 삭제해야 하지만.. 되살릴 방법이 없기 때문에 원래 위치로 옮기고..
                pArea = NULL;
                pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY,
                                             CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
                if (pArea) {
                    spItem->pUIIcon->SetRegion(pArea->GetRegion());
                    spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
                }

                // Invisible로 하고 삭제는 서버가 성공을 줄때 한다..
                spItem->pUIIcon->SetVisible(false);
            }
            FAIL_RETURN
        } else {
            // 이동..
            __IconItemSkill *spItemSource, *spItemTarget = NULL;
            spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;

            pArea = NULL;
            pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, iDestiOrder);
            if (pArea) {
                spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
                spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
            }

            CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iDestiOrder;
            if (m_pMyTradeInv[iDestiOrder]) // 해당 위치에 아이콘이 있으면..
            {
                CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = m_pMyTradeInv[iDestiOrder];
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.UIWnd = UIWND_TRANSACTION;
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.UIWndDistrict = UIWND_DISTRICT_TRADE_MY;
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.iOrder = iDestiOrder;
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.UIWnd = UIWND_TRANSACTION;
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.UIWndDistrict = UIWND_DISTRICT_TRADE_MY;
                CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.iOrder =
                    CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder;

                spItemTarget = CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget;

                pArea = NULL;
                pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY,
                                             CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
                if (pArea) {
                    spItemTarget->pUIIcon->SetRegion(pArea->GetRegion());
                    spItemTarget->pUIIcon->SetMoveRect(pArea->GetRegion());
                }
            } else {
                CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget = NULL;
            }

            m_pMyTradeInv[iDestiOrder] = spItemSource;
            m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemTarget;

            SendToServerMoveMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID +
                                    CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
                                CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, iDestiOrder);
            TRACE("Source %d, Target %d \n", CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, iDestiOrder);
            FAIL_RETURN
        }
        break;
    }

    CN3UIWndBase::AllHighLightIconFree();
    SetState(UI_STATE_COMMON_NONE);

    return false;
}

void CUITransactionDlg::ReceiveResultTradeFromServer(uint8_t bResult, uint8_t bType, int iMoney) {
    CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;
    CN3UIString *        pStatic = NULL;
    __IconItemSkill *    spItem = NULL;
    __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

    // 소스 영역이 UIWND_DISTRICT_TRADE_NPC 이면 아이템 사는거..
    switch (CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWndDistrict) {
    case UIWND_DISTRICT_TRADE_NPC:
        if (bResult != 0x01) // 실패라면..
        {
            if ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                int64_t iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

                if ((m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount - iGold) > 0) {
                    //  숫자 업데이트..
                    m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount -= iGold;
                } else {
                    // 아이템 삭제.. 현재 인벤토리 윈도우만..
                    __IconItemSkill * spItem;
                    spItem = m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

                    // 인벤토리에서도 지운다..
                    m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

                    // iOrder로 내 매니저의 아이템을 리스트에서 삭제한다..
                    RemoveChild(spItem->pUIIcon);

                    // 아이콘 리소스 삭제...
                    spItem->pUIIcon->Release();
                    delete spItem->pUIIcon;
                    spItem->pUIIcon = NULL;
                    delete spItem;
                    spItem = NULL;
                }
            } else {
                // 아이템 삭제.. 현재 인벤토리 윈도우만..
                __IconItemSkill * spItem;
                spItem = m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

                // 인벤토리에서도 지운다..
                m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

                // iOrder로 내 매니저의 아이템을 리스트에서 삭제한다..
                RemoveChild(spItem->pUIIcon);

                // 아이콘 리소스 삭제...
                spItem->pUIIcon->Release();
                delete spItem->pUIIcon;
                spItem->pUIIcon = NULL;
                delete spItem;
                spItem = NULL;
            }

            if (bType == 0x04) {
                // 메시지 박스 텍스트 표시..
                std::string szMsg;
                ::_LoadStringFromResource(IDS_ITEM_TOOMANY_OR_HEAVY, szMsg);
                CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
            }
        } else {
            // 성공이면.. 돈 업데이트.. 먼저 인벤토리..
            pInfoExt->iGold = iMoney;
            pStatic = (CN3UIString *)CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold");
            __ASSERT(pStatic, "NULL UI Component!!");
            if (pStatic) {
                pStatic->SetString(::_FormatCoins(pInfoExt->iGold));
            }
            if (m_pStrMyGold) {
                m_pStrMyGold->SetString(::_FormatCoins(pInfoExt->iGold)); // 상거래창..
            }
        }

        CN3UIWndBase::AllHighLightIconFree();
        SetState(UI_STATE_COMMON_NONE);
        break;

    case UIWND_DISTRICT_TRADE_MY:
        if (bResult != 0x01) // 실패라면..
        {
            if ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)) {
                int64_t iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

                if (m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]
                        ->pUIIcon->IsVisible()) // 기존 아이콘이 보인다면..
                {
                    // 숫자만 바꿔준다..
                    m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount += iGold;
                } else {
                    // 기존 아이콘이 안 보인다면..
                    m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount = iGold;

                    // 아이콘이 보이게..
                    m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetVisible(true);
                }
            } else {
                // Invisible로 쎗팅했던 Icon Visible로..
                spItem = m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
                spItem->pUIIcon->SetVisible(true);
            }
        } else {
            // 활이나 물약등 아이템인 경우 기존 아이콘이 안보인다면.. 아이템 삭제..
            if ((((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
                  (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable ==
                   UIITEM_TYPE_COUNTABLE_SMALL)) &&
                 !m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->IsVisible()) ||
                ((CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable != UIITEM_TYPE_COUNTABLE) &&
                 (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable !=
                  UIITEM_TYPE_COUNTABLE_SMALL))) {
                // 아이템 삭제.. 현재 내 영역 윈도우만..
                __IconItemSkill * spItem;
                spItem = m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];

                // 내 영역에서도 지운다..
                m_pMyTradeInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

                // iOrder로 내 매니저의 아이템을 리스트에서 삭제한다..
                RemoveChild(spItem->pUIIcon);

                // 아이콘 리소스 삭제...
                spItem->pUIIcon->Release();
                delete spItem->pUIIcon;
                spItem->pUIIcon = NULL;
                delete spItem;
                spItem = NULL;
            }

            // 성공이면.. 돈 업데이트..
            pInfoExt->iGold = iMoney;
            pStatic = (CN3UIString *)CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold");
            __ASSERT(pStatic, "NULL UI Component!!");
            if (pStatic) {
                pStatic->SetString(::_FormatCoins(pInfoExt->iGold));
            }
            if (m_pStrMyGold) {
                m_pStrMyGold->SetString(::_FormatCoins(pInfoExt->iGold)); // 상거래창..
            }
        }

        CN3UIWndBase::AllHighLightIconFree();
        SetState(UI_STATE_COMMON_NONE);
        break;
    }
}

void CUITransactionDlg::CancelIconDrop(__IconItemSkill * spItem) {
    CN3UIWndBase::AllHighLightIconFree();
    SetState(UI_STATE_COMMON_NONE);
}

void CUITransactionDlg::AcceptIconDrop(__IconItemSkill * spItem) {
    CN3UIWndBase::AllHighLightIconFree();
    SetState(UI_STATE_COMMON_NONE);
}

void CUITransactionDlg::IconRestore() {
    CN3UIArea * pArea;

    switch (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict) {
    case UIWND_DISTRICT_TRADE_NPC:
        if (m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder] != NULL) {
            pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC,
                                                       CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder);
            if (pArea) {
                m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetRegion(
                    pArea->GetRegion());
                m_pMyTrade[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetMoveRect(
                    pArea->GetRegion());
            }
        }
        break;

    case UIWND_DISTRICT_TRADE_MY:
        if (m_pMyTradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder] != NULL) {
            pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY,
                                                       CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder);
            if (pArea) {
                m_pMyTradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetRegion(
                    pArea->GetRegion());
                m_pMyTradeInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetMoveRect(
                    pArea->GetRegion());
            }
        }
        break;
    }
}

DWORD CUITransactionDlg::MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld) {
    DWORD dwRet = UI_MOUSEPROC_NONE;
    if (!m_bVisible) {
        return dwRet;
    }
    if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) {
        dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
        return dwRet;
    }

    // 드래그 되는 아이콘 갱신..
    if ((GetState() == UI_STATE_ICON_MOVING) &&
        (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd == UIWND_TRANSACTION)) {
        CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
        CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
    }

    return CN3UIWndBase::MouseProc(dwFlags, ptCur, ptOld);
}

int CUITransactionDlg::GetItemiOrder(__IconItemSkill * spItem, e_UIWND_DISTRICT eWndDist) {
    switch (eWndDist) {
    case UIWND_DISTRICT_TRADE_NPC:
        for (int i = 0; i < MAX_ITEM_TRADE; i++) {
            if ((m_pMyTrade[m_iCurPage][i] != NULL) && (m_pMyTrade[m_iCurPage][i] == spItem)) {
                return i;
            }
        }
        break;

    case UIWND_DISTRICT_TRADE_MY:
        for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
            if ((m_pMyTradeInv[i] != NULL) && (m_pMyTradeInv[i] == spItem)) {
                return i;
            }
        }
        break;
    }

    return -1;
}

RECT CUITransactionDlg::GetSampleRect() {
    RECT        rect;
    CN3UIArea * pArea;
    POINT       ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
    pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, 0);
    rect = pArea->GetRegion();
    float fWidth = rect.right - rect.left;
    float fHeight = rect.bottom - rect.top;
    fWidth *= 0.5f;
    fHeight *= 0.5f;
    rect.left = ptCur.x - (int)fWidth;
    rect.right = ptCur.x + (int)fWidth;
    rect.top = ptCur.y - (int)fHeight;
    rect.bottom = ptCur.y + (int)fHeight;
    return rect;
}

e_UIWND_DISTRICT CUITransactionDlg::GetWndDistrict(__IconItemSkill * spItem) {
    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
        if ((m_pMyTrade[m_iCurPage][i] != NULL) && (m_pMyTrade[m_iCurPage][i] == spItem)) {
            return UIWND_DISTRICT_TRADE_NPC;
        }
    }

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) {
        if ((m_pMyTradeInv[i] != NULL) && (m_pMyTradeInv[i] == spItem)) {
            return UIWND_DISTRICT_TRADE_MY;
        }
    }
    return UIWND_DISTRICT_UNKNOWN;
}

bool CUITransactionDlg::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
// Temp Define
#define FAIL_CODE                                                                                                      \
    {                                                                                                                  \
        SetState(UI_STATE_COMMON_NONE);                                                                                \
        return false;                                                                                                  \
    }

    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtnClose) {
            LeaveTransactionState();
        }

        CN3UIString * pStr;

        if (pSender == m_pBtnPageUp) {
            m_iCurPage--;
            if (m_iCurPage < 0) {
                m_iCurPage = 0;
            }

            pStr = (CN3UIString *)GetChildByID("string_page");
            if (pStr) {
                pStr->SetStringAsInt(m_iCurPage + 1);
            }

            for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
                if (j == m_iCurPage) {
                    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                        if (m_pMyTrade[j][i] != NULL) {
                            m_pMyTrade[j][i]->pUIIcon->SetVisible(true);
                        }
                    }
                } else {
                    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                        if (m_pMyTrade[j][i] != NULL) {
                            m_pMyTrade[j][i]->pUIIcon->SetVisible(false);
                        }
                    }
                }
            }
        }

        if (pSender == m_pBtnPageDown) {
            m_iCurPage++;
            if (m_iCurPage >= MAX_ITEM_TRADE_PAGE) {
                m_iCurPage = MAX_ITEM_TRADE_PAGE - 1;
            }

            pStr = (CN3UIString *)GetChildByID("string_page");
            if (pStr) {
                pStr->SetStringAsInt(m_iCurPage + 1);
            }

            for (int j = 0; j < MAX_ITEM_TRADE_PAGE; j++) {
                if (j == m_iCurPage) {
                    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                        if (m_pMyTrade[j][i] != NULL) {
                            m_pMyTrade[j][i]->pUIIcon->SetVisible(true);
                        }
                    }
                } else {
                    for (int i = 0; i < MAX_ITEM_TRADE; i++) {
                        if (m_pMyTrade[j][i] != NULL) {
                            m_pMyTrade[j][i]->pUIIcon->SetVisible(false);
                        }
                    }
                }
            }
        }
    }

    __IconItemSkill * spItem = NULL;
    e_UIWND_DISTRICT  eUIWnd;
    int               iOrder;

    DWORD dwBitMask = 0x000f0000;

    switch (dwMsg & dwBitMask) {
    case UIMSG_ICON_DOWN_FIRST:
        CN3UIWndBase::AllHighLightIconFree();

        // Get Item..
        spItem = GetHighlightIconItem((CN3UIIcon *)pSender);

        // Save Select Info..
        CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_TRANSACTION;
        eUIWnd = GetWndDistrict(spItem);
        if (eUIWnd == UIWND_DISTRICT_UNKNOWN) {
            FAIL_CODE
        }
        CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict = eUIWnd;
        iOrder = GetItemiOrder(spItem, eUIWnd);
        if (iOrder == -1) {
            FAIL_CODE
        }
        CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder = iOrder;
        CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;
        // Do Ops..
        ((CN3UIIcon *)pSender)->SetRegion(GetSampleRect());
        ((CN3UIIcon *)pSender)->SetMoveRect(GetSampleRect());
        // Sound..
        if (spItem) {
            PlayItemSound(spItem->pItemBasic);
        }
        break;

    case UIMSG_ICON_UP:
        // 아이콘 매니저 윈도우들을 돌아 다니면서 검사..
        if (!CGameProcedure::s_pUIMgr->BroadcastIconDropMsg(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect)) {
            // 아이콘 위치 원래대로..
            IconRestore();
        }
        // Sound..
        if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) {
            PlayItemSound(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic);
        }
        break;

    case UIMSG_ICON_DOWN:
        if (GetState() == UI_STATE_ICON_MOVING) {
            CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
            CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
        }
        break;
    }

    return true;
}

CN3UIBase * CUITransactionDlg::GetChildButtonByName(const std::string & szName) {
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        CN3UIBase * pChild = (CN3UIBase *)(*itor);
        if ((pChild->UIType() == UI_TYPE_BUTTON) && (szName.compare(pChild->m_szID) == 0)) {
            return pChild;
        }
    }

    return NULL;
}

void CUITransactionDlg::ShowTitle(e_NpcTrade eNT) {
    m_pUIInn->SetVisible(false);
    m_pUIBlackSmith->SetVisible(false);
    m_pUIStore->SetVisible(false);

    switch (eNT) {
    case UI_BLACKSMITH:
        m_pUIBlackSmith->SetVisible(true);
        break;
    case UI_STORE:
        m_pUIStore->SetVisible(true);
        break;
    case UI_INN:
        m_pUIInn->SetVisible(true);
        break;
    }
}

//this_ui_add_start
void CUITransactionDlg::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
    } else {
        if (CN3UIWndBase::m_pCountableItemEdit && CN3UIWndBase::m_pCountableItemEdit->IsVisible()) {
            ItemCountCancel();
        }

        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
    }
}

void CUITransactionDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus) {
    CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

    if (bWork && !bVisible) {
        if (CN3UIWndBase::m_pCountableItemEdit && CN3UIWndBase::m_pCountableItemEdit->IsVisible()) {
            ItemCountCancel();
        }

        if (GetState() == UI_STATE_ICON_MOVING) {
            IconRestore();
        }
        SetState(UI_STATE_COMMON_NONE);
        CN3UIWndBase::AllHighLightIconFree();

        // 이 윈도우의 inv 영역의 아이템을 이 인벤토리 윈도우의 inv영역으로 옮긴다..
        ItemMoveFromThisToInv();

        if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) {
            CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
        }
        if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) {
            CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
        }
        if (m_pUITooltipDlg) {
            m_pUITooltipDlg->DisplayTooltipsDisable();
        }
    }
}

bool CUITransactionDlg::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtnClose = (CN3UIButton *)(this->GetChildByID("btn_close"));
    __ASSERT(m_pBtnClose, "NULL UI Component!!");
    m_pBtnPageUp = (CN3UIButton *)(this->GetChildByID("btn_page_up"));
    __ASSERT(m_pBtnPageUp, "NULL UI Component!!");
    m_pBtnPageDown = (CN3UIButton *)(this->GetChildByID("btn_page_down"));
    __ASSERT(m_pBtnPageDown, "NULL UI Component!!");

    return true;
}

bool CUITransactionDlg::OnKeyPress(int iKey) {
    switch (iKey) {
    case DIK_PRIOR:
        ReceiveMessage(m_pBtnPageUp, UIMSG_BUTTON_CLICK);
        return true;
    case DIK_NEXT:
        ReceiveMessage(m_pBtnPageDown, UIMSG_BUTTON_CLICK);
        return true;
    case DIK_ESCAPE:
        ReceiveMessage(m_pBtnClose, UIMSG_BUTTON_CLICK);
        if (m_pUITooltipDlg) {
            m_pUITooltipDlg->DisplayTooltipsDisable();
        }
        return true;
    }

    return CN3UIBase::OnKeyPress(iKey);
}
//this_ui_add_end
