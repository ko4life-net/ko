// UISkillTreeDlg.cpp: implementation of the CUISkillTreeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "LocalInput.h"
#include "Resource.h"
#include "UICmdList.h"

#include "GameProcedure.h"
#include "GameProcMain.h"
#include "UIHotKeyDlg.h"
#include "UIManager.h"

#include "N3Base/N3UIArea.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3SndObj.h"

#include "APISocket.h"
#include "PacketDef.h"
#include "PlayerMySelf.h"
#include "UIManager.h"

#include "N3UIDBCLButton.h"

#include "GameDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUICmdListDlg::CUICmdListDlg() {
    m_bOpenningNow = false; // 열리고 있다..
    m_bClosingNow = false;  // 닫히고 있다..
    m_fMoveDelta = 0.0f;    // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..

    m_pBtn_cancel = NULL;
    m_pList_CmdCat = NULL;
    m_pList_Cmds = NULL;
    m_pUICmdEdit = NULL;
}

CUICmdListDlg::~CUICmdListDlg() {
    Release();
}

bool CUICmdListDlg::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_cancel = (CN3UIButton *)this->GetChildByID("btn_cancel");
    __ASSERT(m_pBtn_cancel, "NULL UI Component!!!");
    m_pList_CmdCat = (CN3UIList *)this->GetChildByID("list_curtailment");
    __ASSERT(m_pList_CmdCat, "NULL UI Component!!!");
    m_pList_Cmds = (CN3UIList *)this->GetChildByID("list_content");
    __ASSERT(m_pList_Cmds, "NULL UI Component!!!");

    CreateCategoryList();

    return true;
}

void CUICmdListDlg::Release() {
    CN3UIBase::Release();
}

void CUICmdListDlg::Open() {
    // 스르륵 열린다!!
    SetVisible(true);
    this->SetPos(CN3Base::s_CameraData.vp.Width, 10);
    m_fMoveDelta = 0;
    m_bOpenningNow = true;
    m_bClosingNow = false;

    m_iRBtnDownOffs = -1;
}
void CUICmdListDlg::Close(bool bByKey) {
    SetVisible(false);
    m_fMoveDelta = 0;
    m_bOpenningNow = false;
    m_bClosingNow = true;
}

void CUICmdListDlg::Tick() {
    if (m_bOpenningNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
    {
        POINT ptCur = this->GetPos();
        RECT  rc = this->GetRegion();
        float fWidth = (float)(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width - (int)fWidth;
        ptCur.x = CN3Base::s_CameraData.vp.Width - (int)m_fMoveDelta;
        if (ptCur.x <= iXLimit) // 다열렸다!!
        {
            ptCur.x = iXLimit;
            m_bOpenningNow = false;
        }

        this->SetPos(ptCur.x, ptCur.y);
    } else if (m_bClosingNow) // 오른쪽에서 왼쪽으로 스르륵...열려야 한다면..
    {
        POINT ptCur = this->GetPos();
        RECT  rc = this->GetRegion();
        float fWidth = (float)(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width;
        ptCur.x = CN3Base::s_CameraData.vp.Width - (int)(fWidth - m_fMoveDelta);
        if (ptCur.x >= iXLimit) // 다 닫혔다..!!
        {
            ptCur.x = iXLimit;
            m_bClosingNow = false;

            this->SetVisibleWithNoSound(false, false, true); // 다 닫혔으니 눈에서 안보이게 한다.
        }

        this->SetPos(ptCur.x, ptCur.y);
    }

    CN3UIBase::Tick();
}

bool CUICmdListDlg::CreateCategoryList() {

    if (m_pList_CmdCat == NULL || m_pList_Cmds == NULL) {
        return false;
    }

    std::string szCategory;
    int         idStart = IDS_PRIVATE_CMD_CAT;

    for (int i = 0; i < 8; i++) {
        ::_LoadStringFromResource(i + 7800, szCategory); //load command categories

        if (CGameBase::s_pPlayer->m_InfoBase.iAuthority == 0 && szCategory == "GM") { // if not gm hide this category
            continue;
        }

        m_pList_CmdCat->AddString(szCategory);
        idStart++;
    }

    m_pList_CmdCat->SetFontColor(0xffffff00); //green

    int idCur = 8000; //Command list strings start at this index
    int idEnd = 9600; //Command list strings end at this index

    std::string szCommand;
    //create map of commands
    for (int i = idCur; idCur < idEnd; idCur++, i++) {
        if (idCur == 9000) {
            i += 400; // offset and put gm cmds at end of map
        } else if (idCur == 9100) {
            i -= 500;
            idCur = 9200;
        }
        szCommand.clear();
        ::_LoadStringFromResource(idCur, szCommand);
        if (!szCommand.empty() && (i / 100) % 2 == 0) {
            m_mapCmds[i] = szCommand;
        }
    }

    UpdateCommandList(CMD_LIST_PRIVATE); //initialize a cmd list for viewing when opening cmd window

    return true;
}
bool CUICmdListDlg::UpdateCommandList(uint8_t cmdCat) {

    if (m_pList_Cmds == NULL) {
        return false;
    }

    m_pList_Cmds->ResetContent();

    int indexStart = cmdCat * 200 + 8000; //start index for correct loc in map
    int indexEnd = indexStart + 100;      //where to stop iterating
    int i = 0;

    for (auto itr = m_mapCmds.begin(); itr != m_mapCmds.end(); ++itr) {
        if (itr->first >= indexStart && itr->first < indexEnd) {
            m_pList_Cmds->AddString(itr->second);

            CN3UIString * pChild = m_pList_Cmds->GetChildStrFromList(itr->second);
            std::string   cmdTip;
            ::_LoadStringFromResource(itr->first + 100, cmdTip);
            if (pChild != NULL) {
                pChild->SetTooltipText(cmdTip.c_str());
            }
            //SavvyNik tooltip is being loaded in but the rectangle
            //that it shows on is too small. Need to figure out where
            //this is being set.
        }
    }

    return true;
}
bool CUICmdListDlg::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_cancel) {
            SetVisible(false);
            return true;
        }
    } else if (dwMsg == UIMSG_LIST_SELCHANGE) {
        if (pSender == m_pList_CmdCat) {
            uint8_t iSel = m_pList_CmdCat->GetCurSel();
            UpdateCommandList(iSel);
            return true;
        }
    } else if (dwMsg == UIMSG_LIST_DBLCLK) {
        if (pSender == m_pList_Cmds) {
            uint8_t iSel = m_pList_Cmds->GetCurSel();
            ExecuteCommand(iSel);
            return true;
        }
    }

    return false;
}

void CUICmdListDlg::Render() {
    if (!m_bVisible) {
        return;
    }

    CN3UIBase::Render();
}

//this_ui_add_start
bool CUICmdListDlg::OnKeyPress(int iKey) {

    return CN3UIBase::OnKeyPress(iKey);
}

void CUICmdListDlg::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI(); //this_ui
    }
}

bool CUICmdListDlg::ExecuteCommand(uint8_t cmdSel) {

    std::string command;
    m_pList_Cmds->GetString(cmdSel, command);

    if (command == "PM") {
        CGameProcedure::s_pProcMain->OpenCmdEdit(command);
    }

    command = '/' + command;
    CGameProcedure::s_pProcMain->ParseChattingCommand(command);

    return true;
}