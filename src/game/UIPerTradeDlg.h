// UIPerTradeDlg.h: interface for the CUIPerTradeDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIArea.h"
#include "GameProcedure.h"

#include "GameDef.h"
#include "N3UIWndBase.h"

//////////////////////////////////////////////////////////////////////

class CGameProcMain;

class CUIPerTradeDlg : public CN3UIWndBase {
    friend class CUIInventory;

  public:
    CSubProcPerTrade * m_pSubProcPerTrade;

    // 직접 접근해야 할 객체 참조 포인터
    __IconItemSkill * m_pPerTradeMy[MAX_ITEM_PER_TRADE];
    __IconItemSkill * m_pPerTradeOther[MAX_ITEM_PER_TRADE];
    __IconItemSkill * m_pPerTradeInv[MAX_ITEM_INVENTORY];

    int m_iBackupiOrder[MAX_ITEM_PER_TRADE];

    CUIImageTooltipDlg * m_pUITooltipDlg;
    int                  m_iBackupiCount; // 전에 옮긴 화살등의 갯수..

    CN3UIString * m_pStrMyGold;

  protected:
    int  GetItemiOrder(__IconItemSkill * spItem, e_UIWND_DISTRICT eWndDist);
    RECT GetSampleRect();

    void ItemMoveFromInvToThis();
    void ItemMoveFromThisToInv();

  public:
    CUIPerTradeDlg();
    virtual ~CUIPerTradeDlg();
    void Release();

    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    void          Render();
    void          LeavePerTradeState();
    void          EnterPerTradeState();

    void InitIconWnd(e_UIWND eWnd);
    void InitIconUpdate();

    __IconItemSkill * GetHighlightIconItem(CN3UIIcon * pUIIcon);

    void IconRestore();

    bool ReceiveIconDrop(__IconItemSkill * spItem, POINT ptCur);

    void CancelIconDrop(__IconItemSkill * spItem);
    void AcceptIconDrop(__IconItemSkill * spItem);

    void SendToServerItemAddMsg(uint8_t pos, int itemID, int iCount);

    CN3UIBase * GetChildButtonByName(const std::string & szName);

    // Item Count OK..
    void ItemCountOK();
    void ItemCountCancel();

    void GoldUpdate();
};
