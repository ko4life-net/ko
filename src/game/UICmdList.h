// UISkillTreeDlg.h: interface for the CUISkillTreeDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIList.h"
#include "N3Base/N3UIEdit.h"

//////////////////////////////////////////////////////////////////////

class CUICmdListDlg : public CN3UIBase {
    bool               m_bDestoyDlgAlive;
    class CUICmdEdit * m_pUICmdEdit;
    CN3UIButton *      m_pBtn_cancel;
    CN3UIList *        m_pList_CmdCat;
    CN3UIList *        m_pList_Cmds;

  protected:
    bool  m_bOpenningNow; // 열리고 있다..
    bool  m_bClosingNow;  // 닫히고 있다..
    float m_fMoveDelta;   // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..
    int   m_iRBtnDownOffs;
    bool  m_bRBtnProcessing;

    enum iCmd {
        CMD_LIST_PRIVATE,
        CMD_LIST_TRADE,
        CMD_LIST_PARTY,
        CMD_LIST_CLAN,
        CMD_LIST_KNIGHTS,
        CMD_LIST_GUARDIAN,
        CMD_LIST_KING,
        CMD_LIST_GM
    };

    std::map<uint16_t, std::string> m_mapCmds;

  public:
    void SetVisible(bool bVisible);

    bool         OnKeyPress(int iKey);
    virtual void Release();
    virtual void Tick();
    virtual bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    void         Render();
    void         Open();
    void         Close(bool bByKey = false);
    bool         Load(HANDLE hFile);
    bool         CreateCategoryList();
    bool         UpdateCommandList(uint8_t cmd);
    bool         ExecuteCommand(uint8_t cmdSel);

    CUICmdListDlg();
    virtual ~CUICmdListDlg();
};
