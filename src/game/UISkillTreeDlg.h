// UISkillTreeDlg.h: interface for the CUISkillTreeDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "N3UIWndBase.h"

#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIImage.h"
#include "N3Base/N3UIString.h"

const int SKILL_DEF_BASIC = 0;
const int SKILL_DEF_SPECIAL0 = 1;
const int SKILL_DEF_SPECIAL1 = 2;
const int SKILL_DEF_SPECIAL2 = 3;
const int SKILL_DEF_SPECIAL3 = 4;

// MAX_SKILL_KIND_OF    :                         Base Skill 1개, 전문 스킬 4개..
// MAX_SKILL_IN_PAGE    :                         한 페이지 내의 아이콘 갯수..
// MAX_SKILL_PAGE_NUM    :                         스킬 한개가 가지는 최대 페이지수..

//////////////////////////////////////////////////////////////////////

class CUISkillTreeDlg : public CN3UIWndBase {
  protected:
    bool m_bOpenningNow; // 열리고 있다..
    bool m_bClosingNow;  // 닫히고 있다..
    float m_fMoveDelta; // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..

    int m_iRBtnDownOffs;

    CN3UIString * m_pStr_info;
    CN3UIString * m_pStr_skill_mp;
    CN3UIString * m_pStr_skill_item0;
    CN3UIString * m_pStr_skill_item1;

  public:
    int m_iCurKindOf;
    int m_iCurSkillPage;

    int               m_iSkillInfo[MAX_SKILL_FROM_SERVER]; // 서버로 받는 슬롯 정보..
    __IconItemSkill * m_pMySkillTree[MAX_SKILL_KIND_OF][MAX_SKILL_PAGE_NUM][MAX_SKILL_IN_PAGE]; // 총 스킬 정보..
    int               m_iCurInPageOffset[MAX_SKILL_KIND_OF]; // 스킬당 현재 페이지 옵셋..

  protected:
    void AllClearImageByName(const std::string & szName, bool bTrueOrNot);
    RECT GetSampleRect();
    void PageButtonInitialize();

  public:
    void SetVisible(bool bVisible);
    CUISkillTreeDlg();
    virtual ~CUISkillTreeDlg();

    void          SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
    bool          OnKeyPress(int iKey);
    virtual void  Release();
    virtual void  Tick();
    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    void          Render();
    void          Open();
    void          Close();

    void InitIconWnd(e_UIWND eWnd);
    void InitIconUpdate();

    __IconItemSkill * GetHighlightIconItem(CN3UIIcon * pUIIcon);
    int               GetSkilliOrder(__IconItemSkill * spSkill);

    void AddSkillToPage(__TABLE_UPC_SKILL * pUSkill, int iOffset = 0);

    void SetPageInIconRegion(int iKindOf, int iPageNum); // 아이콘 역역에서 현재 페이지 설정..
    void SetPageInCharRegion();                          // 문자 역역에서 현재 페이지 설정..

    CN3UIImage *  GetChildImageByName(const std::string & szName);
    CN3UIBase *   GetChildBaseByName(const std::string & szName);
    CN3UIButton * GetChildButtonByName(const std::string & szName);

    void PageLeft();
    void PageRight();

    void PointPushUpButton(int iValue);

    bool HasIDSkill(int iID);
    void ButtonVisibleStateSet();

    void TooltipRenderEnable(__IconItemSkill * spSkill);
    void TooltipRenderDisable();
    void CheckButtonTooltipRenderEnable();
    void ButtonTooltipRender(int iIndex);

    void UpdateDisableCheck();
    int  GetIndexInArea(POINT pt);
};
