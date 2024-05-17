// UICmdListDlg.h: interface for the CUICmdListDlg class.
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
    CN3UIScrollBar *   m_pScroll_Categories;
    CN3UIScrollBar *   m_pScroll_Commands;

  protected:
    bool  m_bOpenningNow;
    bool  m_bClosingNow;
    float m_fMoveDelta;
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
