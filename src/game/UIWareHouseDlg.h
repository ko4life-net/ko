// UIWareHouseDlg.h: interface for the UIWareHouseDlg class.
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

class CUIWareHouseDlg : public CN3UIWndBase {
    friend class CUIInventory;

  public:
    // 직접 접근해야 할 객체 참조 포인터
    __IconItemSkill * m_pMyWare[MAX_ITEM_WARE_PAGE][MAX_ITEM_TRADE];
    __IconItemSkill * m_pMyWareInv[MAX_ITEM_INVENTORY];
    CN3UIString *     m_pStrMyGold;
    CN3UIString *     m_pStrWareGold;

    //this_ui_add_start
    CN3UIButton * m_pBtnGold;
    CN3UIButton * m_pBtnGoldWareHouse;
    CN3UIButton * m_pBtnClose;
    CN3UIButton * m_pBtnPageUp;
    CN3UIButton * m_pBtnPageDown;
    //this_ui_add_end

    bool    m_bSendedItemGold;
    int64_t m_iGoldOffsetBackup;

    int                  m_iCurPage;
    CUIImageTooltipDlg * m_pUITooltipDlg;

  protected:
    int  GetItemiOrder(__IconItemSkill * spItem, e_UIWND_DISTRICT eWndDist);
    RECT GetSampleRect();

    e_UIWND_DISTRICT GetWndDistrict(__IconItemSkill * spItem);

  public:
    CUIWareHouseDlg();
    virtual ~CUIWareHouseDlg();
    void Release();

    //this_ui_add_start
    void SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
    void SetVisible(bool bVisible);
    bool Load(HANDLE hFile);
    bool OnKeyPress(int iKey);
    //this_ui_add_end

    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    void          Render();
    void          LeaveWareHouseState();
    void          EnterWareHouseStateStart(int64_t iWareGold);
    void          AddItemInWare(int iItem, int iDurability, int iCount, int iIndex);
    void          EnterWareHouseStateEnd();

    void InitIconWnd(e_UIWND eWnd);
    void InitIconUpdate();

    __IconItemSkill * GetHighlightIconItem(CN3UIIcon * pUIIcon);

    void IconRestore();

    bool ReceiveIconDrop(__IconItemSkill * spItem, POINT ptCur);

    void CancelIconDrop(__IconItemSkill * spItem);
    void AcceptIconDrop(__IconItemSkill * spItem);

    void SendToServerToWareMsg(int iItemID, uint8_t page, uint8_t startpos, uint8_t pos, int64_t iCount);
    void SendToServerFromWareMsg(int iItemID, uint8_t page, uint8_t startpos, uint8_t pos, int64_t iCount);
    void SendToServerWareToWareMsg(int iItemID, uint8_t page, uint8_t startpos, uint8_t destpos);
    void SendToServerInvToInvMsg(int iItemID, uint8_t page, uint8_t startpos, uint8_t destpos);
    void ReceiveResultToWareMsg(BYTE bResult);
    void ReceiveResultFromWareMsg(BYTE bResult);
    void ReceiveResultWareToWareMsg(BYTE bResult);
    void ReceiveResultInvToInvMsg(BYTE bResult);
    void ReceiveResultGoldToWareFail();
    void ReceiveResultGoldFromWareFail();

    // Item Count OK..
    void ItemCountOK();
    void ItemCountCancel();

    void GoldCountToWareOK();
    void GoldCountFromWareOK();
    void GoldCountToWareCancel();
    void GoldCountFromWareCancel();

    void ItemMoveFromInvToThis();
    void ItemMoveFromThisToInv();
};
