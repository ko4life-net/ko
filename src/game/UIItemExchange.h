// UIItemExchange.h: interface for the CUIItemRepairExchange class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"

#include "N3UIWndBase.h"

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIImage.h"

//////////////////////////////////////////////////////////////////////

class CUIItemExchange : public CN3UIWndBase {
    CN3UIImage * m_pImage_Repair;
    CN3UIImage * m_pImage_Exchange;

    __IconItemSkill *    m_pMyInvWnd[MAX_ITEM_INVENTORY];
    __IconItemSkill *    m_pMyNpcWnd[MAX_ITEM_EX_RE_NPC];
    int                  m_pMyNpcWndOriginIndex[MAX_ITEM_EX_RE_NPC];
    int                  m_iTotalPrice;
    CUIImageTooltipDlg * m_pUITooltipDlg;

  private:
    void Close();

  protected:
    e_UIWND_DISTRICT GetWndDistrict(__IconItemSkill * spItem);
    int              GetItemiOrder(__IconItemSkill * spItem, e_UIWND_DISTRICT eWndDist);
    RECT             GetSampleRect();

  public:
    void Release();

    CUIItemExchange();
    virtual ~CUIItemExchange();

    bool Load(HANDLE hFile);

    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    void Render();

    void Open();

    void ItemMoveFromInvToThis();
    void ItemMoveFromThisToInv();

    void InitIconWnd(e_UIWND eWnd);

    void              InitIconUpdate() {}
    __IconItemSkill * GetHighlightIconItem(CN3UIIcon * pUIIcon);

    void IconRestore();
    void CancelIconDrop(__IconItemSkill * spItem);
    bool ReceiveIconDrop(__IconItemSkill * spItem, POINT ptCur);

    int  CalcRepairGold(__IconItemSkill * spItem);
    void UpdateGoldValue();
    void UpdateUserTotalGold(int iGold);

    void UserPressCancel(); // And User Press Close.. ^^
    void UserPressOK();
    void ReceiveResultFromServer(int iResult, int iUserGold);
};
