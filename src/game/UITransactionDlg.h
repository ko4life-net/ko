// UITransactionDlg.h: interface for the CUITransactionDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIArea.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIImage.h"
#include "N3Base/N3UIButton.h"
#include "GameProcedure.h"

#include "GameDef.h"
#include "N3UIWndBase.h"

enum e_NpcTrade {
    UI_BLACKSMITH = 1,
    UI_STORE,
    UI_INN
};

//////////////////////////////////////////////////////////////////////

class CUITransactionDlg : public CN3UIWndBase {
    friend class CUIInventory;

  public:
    // 직접 접근해야 할 객체 참조 포인터
    __IconItemSkill * m_pMyTrade[MAX_ITEM_TRADE_PAGE][MAX_ITEM_TRADE];
    __IconItemSkill * m_pMyTradeInv[MAX_ITEM_INVENTORY];
    CN3UIString *     m_pStrMyGold;

    int                  m_iCurPage;
    int                  m_iTradeID;
    int                  m_iNpcID;
    CUIImageTooltipDlg * m_pUITooltipDlg;

    CN3UIImage * m_pUIInn;
    CN3UIImage * m_pUIBlackSmith;
    CN3UIImage * m_pUIStore;

    //this_ui_add_start
    CN3UIButton * m_pBtnClose;
    CN3UIButton * m_pBtnPageUp;
    CN3UIButton * m_pBtnPageDown;
    //this_ui_add_end

  protected:
    int  GetItemiOrder(__IconItemSkill * spItem, e_UIWND_DISTRICT eWndDist);
    RECT GetSampleRect();

    e_UIWND_DISTRICT GetWndDistrict(__IconItemSkill * spItem);

  public:
    CUITransactionDlg();
    virtual ~CUITransactionDlg();
    void Release();

    //this_ui_add_start
    bool OnKeyPress(int iKey);
    bool Load(HANDLE hFile);
    void SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
    void SetVisible(bool bVisible);
    //this_ui_add_end

    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    void          Render();
    void          LeaveTransactionState();
    void          EnterTransactionState();

    void InitIconWnd(e_UIWND eWnd);
    void InitIconUpdate();

    __IconItemSkill * GetHighlightIconItem(CN3UIIcon * pUIIcon);

    void IconRestore();

    bool ReceiveIconDrop(__IconItemSkill * spItem, POINT ptCur);

    void CancelIconDrop(__IconItemSkill * spItem);
    void AcceptIconDrop(__IconItemSkill * spItem);

    void SendToServerSellMsg(int itemID, uint8_t pos, int iCount);
    void SendToServerBuyMsg(int itemID, uint8_t pos, int iCount);
    void SendToServerMoveMsg(int itemID, uint8_t startpos, uint8_t destpos);

    // 물건 구입이 서버에게 보내기전 성공..
    void ReceiveItemDropByTradeSuccess();

    CN3UIBase * GetChildButtonByName(const std::string & szName);

    // Item Count OK..
    void ItemCountOK();
    void ItemCountCancel();

    void ItemMoveFromInvToThis();
    void ItemMoveFromThisToInv();

    void ReceiveResultTradeFromServer(uint8_t bResult, uint8_t bType, int iMoney);
    void ReceiveResultTradeMoveSuccess();
    void ReceiveResultTradeMoveFail();

    // 타이틀 표시..
    void ShowTitle(e_NpcTrade eNT);

    void GoldUpdate();
};
