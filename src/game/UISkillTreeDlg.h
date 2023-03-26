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

// MAX_SKILL_KIND_OF    :                         Base Skill 1��, ���� ��ų 4��..
// MAX_SKILL_IN_PAGE    :                         �� ������ ���� ������ ����..
// MAX_SKILL_PAGE_NUM    :                         ��ų �Ѱ��� ������ �ִ� ��������..

//////////////////////////////////////////////////////////////////////

class CUISkillTreeDlg : public CN3UIWndBase {
  protected:
    bool  m_bOpenningNow; // ������ �ִ�..
    bool  m_bClosingNow;  // ������ �ִ�..
    float m_fMoveDelta;   // �ε巴�� ������ ������ ����� ���ؼ� ������ġ ��꿡 �ε��Ҽ����� ����..

    int m_iRBtnDownOffs;

    CN3UIString * m_pStr_info;
    CN3UIString * m_pStr_skill_mp;
    CN3UIString * m_pStr_skill_item0;
    CN3UIString * m_pStr_skill_item1;

  public:
    int m_iCurKindOf;
    int m_iCurSkillPage;

    int               m_iSkillInfo[MAX_SKILL_FROM_SERVER]; // ������ �޴� ���� ����..
    __IconItemSkill * m_pMySkillTree[MAX_SKILL_KIND_OF][MAX_SKILL_PAGE_NUM][MAX_SKILL_IN_PAGE]; // �� ��ų ����..
    int               m_iCurInPageOffset[MAX_SKILL_KIND_OF]; // ��ų�� ���� ������ �ɼ�..

  protected:
    void AllClearImageByName(const std::string & szFN, bool bTrueOrNot);
    void AllClearImageByNametah(const std::string & szFN, bool bTrueOrNot);
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

    void SetPageInIconRegion(int iKindOf, int iPageNum); // ������ �������� ���� ������ ����..
    void SetPageInCharRegion();                          // ���� �������� ���� ������ ����..

    CN3UIImage *  GetChildImageByName(const std::string & szFN);
    CN3UIBase *   GetChildBaseByName(const std::string & szFN);
    CN3UIButton * GetChildButtonByName(const std::string & szFN);
    CN3UIString *   GetChildStringByName(const std::string & szFN);

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
