// UIRepairTooltipDlg.h: interface for the UIRepairTooltipDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIString.h"

#include "GameProcedure.h"
#include "N3UIWndBase.h"

#define MAX_REPAIR_TOOLTIP_COUNT 4

//////////////////////////////////////////////////////////////////////

class CUIRepairTooltipDlg : public CN3UIBase {
    CN3UIString *     m_pStr[MAX_REPAIR_TOOLTIP_COUNT];      // 스트륑.. ^^
    D3DCOLOR          m_pStrColor[MAX_REPAIR_TOOLTIP_COUNT]; // 스트륑 색깔.. ^^
    int               m_iPosXBack, m_iPosYBack;
    __IconItemSkill * m_spItemBack;

  public:
    bool              m_bBRender;
    int               m_iBxpos;
    int               m_iBypos;
    __IconItemSkill * m_pBspItem;
    int               m_iBRequiredGold;
    bool              m_bBHaveEnough;

    int m_iBRequiredGoldBackup;

  protected:
    void SetTooltipTextColor(bool bHaveEnough);

  public:
    CUIRepairTooltipDlg();
    virtual ~CUIRepairTooltipDlg();
    void Release();
    void InitPos();
    void BackupStrColor();

    void DisplayTooltipsEnable(int xpos, int ypos, __IconItemSkill * spItem, int iRequiredGold, bool bHaveEnough);
    void DisplayTooltipsDisable();

    void Render();
};
