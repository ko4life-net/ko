// UICmdListDlg.cpp: implementation of the CUICmdListDlg class.
//

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
#include "N3Base/N3UITooltip.h"
#include "N3Base/N3UIScrollBar.h"
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

CUICmdList::CUICmdList() {
    m_bOpenningNow = false;
    m_bClosingNow = false;
    m_fMoveDelta = 0.0f;

    m_pBtn_cancel = nullptr;
    m_pList_CmdCat = nullptr;
    m_pList_Cmds = nullptr;
    m_pUICmdEdit = nullptr;
    m_pScroll_Categories = nullptr;
    m_pScroll_Commands = nullptr;

    m_pList_CommandsPrivate = nullptr;
    m_pList_CommandsTrade = nullptr;
    m_pList_CommandsClan = nullptr;
    m_pList_CommandsKnights = nullptr;
    m_pList_CommandsParty = nullptr;
    m_pList_CommandsGuardianMonster = nullptr;
    m_pList_CommandsKing = nullptr;
    m_pList_CommandsGM = nullptr;
    m_pList_Cmds = nullptr;

}

CUICmdList::~CUICmdList() {
    Release();
}

bool CUICmdList::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_cancel = dynamic_cast<CN3UIButton *>(GetChildByID("btn_cancel"));
    if (!m_pBtn_cancel) {
        __ASSERT(m_pBtn_cancel, "NULL UI Component!!!");
        return false;
    }

    m_pList_CmdCat = dynamic_cast<CN3UIList *>(GetChildByID("list_curtailment"));
    if (!m_pList_CmdCat) {
        __ASSERT(m_pList_CmdCat, "NULL UI Component!!!");
        return false;
    }

    m_pList_Cmds = dynamic_cast<CN3UIList *>(GetChildByID("list_content"));
    if (!m_pList_Cmds) {
        __ASSERT(m_pList_Cmds, "NULL UI Component!!!");
        return false;
    }

    m_pScroll_Categories = dynamic_cast<CN3UIScrollBar *>(m_pList_CmdCat->GetChildByIndex(0));
    if (!m_pScroll_Categories) {
        __ASSERT(m_pScroll_Categories, "NULL UI Component!!");
        return false;
    }
    m_pScroll_Categories->SetVisible(true);

    m_pScroll_Commands = dynamic_cast<CN3UIScrollBar *>(m_pList_Cmds->GetChildByIndex(0));
    if (!m_pScroll_Commands) {
        __ASSERT(m_pScroll_Commands, "NULL UI Component!!");
        return false;
    }
    m_pScroll_Commands->SetVisible(true);

    m_pList_CmdCat->SetScrollBarRef(m_pScroll_Categories);
    m_pList_Cmds->SetScrollBarRef(m_pScroll_Commands);

    InitCommands();

    return true;
}

void CUICmdList::Release() {
    CN3UIBase::Release();
}

void CUICmdList::Open() {
    SetVisible(true);
    SetPos(CN3Base::s_CameraData.vp.Width, 10);
    m_fMoveDelta = 0;
    m_bOpenningNow = true;
    m_bClosingNow = false;

    if (CGameBase::s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER){
        if (m_pList_CmdCat->GetChildStrFromList(szGmCategory)) {
            m_pList_CmdCat->GetChildStrFromList(szGmCategory)->SetVisible(false);
        }
    } else {
        if (m_pList_CmdCat->GetChildStrFromList(szGmCategory)) {
            m_pList_CmdCat->GetChildStrFromList(szGmCategory)->SetVisible(true);
        }
    }

    m_iRBtnDownOffs = -1;
}

void CUICmdList::Close(bool bByKey) {
    SetVisible(false);
    m_fMoveDelta = 0;
    m_bOpenningNow = false;
    m_bClosingNow = true;
}

void CUICmdList::Tick() {
    if (m_bOpenningNow) {
        POINT ptCur = GetPos();
        RECT  rc = GetRegion();
        float fWidth = static_cast<float>(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width - static_cast<int>(fWidth);
        ptCur.x = CN3Base::s_CameraData.vp.Width - static_cast<int>(m_fMoveDelta);
        if (ptCur.x <= iXLimit) {
            ptCur.x = iXLimit;
            m_bOpenningNow = false;
        }

        SetPos(ptCur.x, ptCur.y);
    } else if (m_bClosingNow) {
        POINT ptCur = GetPos();
        RECT  rc = GetRegion();
        float fWidth = static_cast<float>(rc.right - rc.left);

        float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
        fDelta *= (fWidth - m_fMoveDelta) / fWidth;
        if (fDelta < 2.0f) {
            fDelta = 2.0f;
        }
        m_fMoveDelta += fDelta;

        int iXLimit = CN3Base::s_CameraData.vp.Width;
        ptCur.x = CN3Base::s_CameraData.vp.Width - static_cast<int>(fWidth - m_fMoveDelta);
        if (ptCur.x >= iXLimit) {
            ptCur.x = iXLimit;
            m_bClosingNow = false;

            SetVisibleWithNoSound(false, false, true);
        }

        SetPos(ptCur.x, ptCur.y);
    }

    CN3UIBase::Tick();
}

void CUICmdList::InitCommands() {
    if (!m_pList_CmdCat) {
        return;
    }

    for (int i = 0; i < CMD_LIST_COUNT; ++i) {

        if (i == CMD_LIST_KNIGHTS - COMMANDS_CATEGORY_NAMES_INDEX ||
            i == CMD_LIST_GUARDIAN - COMMANDS_CATEGORY_NAMES_INDEX ||
            i == CMD_LIST_KING - COMMANDS_CATEGORY_NAMES_INDEX) { // -> in ver 1298 remove  this condition
            continue;
        }

        std::string szCategory;
        _LoadStringFromResource(i + COMMANDS_CATEGORY_NAMES_INDEX, szCategory); 

        if (i + COMMANDS_CATEGORY_NAMES_INDEX == CMD_LIST_GM) {
            szGmCategory = szCategory;
        }

        std::string cmdCategoryTip;
        _LoadStringFromResource(i + COMMANDS_CATEGORY_TOOLTIP_INDEX,cmdCategoryTip); 
        m_pList_CmdCat->AddStringTooltip(szCategory, 0xFFC6C6FB, cmdCategoryTip.c_str(), 0xFF80FF80);
    }

    int rangeMax = m_pList_CmdCat->GetCount() - MAX_VISIBLE_ITEMS;
    m_pScroll_Categories->SetVisibleWithNoSound(rangeMax >= 0);
    m_pScroll_Categories->SetRange(0, rangeMax);

    m_pList_CmdCat->SetFontColor(0xffffff00); // yellow

    LoadCommandMap(m_mCmd_Private, CMD_WHISPER, CMD_INDIVIDUAL_BATTLE);
    LoadCommandMap(m_mCmd_Trade, CMD_TRADE, CMD_MERCHANT);
    LoadCommandMap(m_mCmd_Party, CMD_PARTY, CMD_PERMITPARTY);
    LoadCommandMap(m_mCmd_Clan, CMD_JOINCLAN, CMD_CLAN_BATTLE);
//    LoadCommandMap(m_mCmd_Knights, CMD_CONFEDERACY, CMD_DECLARATION); -> in ver 1298 remove  comment and enable
//    LoadCommandMap(m_mCmd_Guard, CMD_GUARD_HIDE, CMD_GUARD_DESTROY);
//    LoadCommandMap(m_mCmd_King, CMD_KING_ROYALORDER, CMD_KING_REWARD);
    LoadCommandMap(m_mCmd_GM, CMD_VISIBLE, CMD_PLC);

    UpdateCommandList(CMD_LIST_PRIVATE);
}

void CUICmdList::LoadCommandMap(std::map<int, CommandInfo> & cmdMap, int startId, int endId) {
    int i = 0;
    for (int indexResource = startId; indexResource <= endId; ++indexResource) {
    CommandInfo info;
        _LoadStringFromResource(indexResource, info.name);
        _LoadStringFromResource(indexResource + COMMANDS_TOOLTIP_INDEX, info.tooltip);
        cmdMap[i] = info;
        std::transform(info.name.begin(), info.name.end(), info.name.begin(), ::toupper);
        g_commandMappings[info.name] = indexResource;
        i++;
    }
}

bool CUICmdList::UpdateCommandList(iCommandListCategory cmdCat) {
    m_pList_Cmds->ResetContent();
    const std::map<int, CommandInfo> * cmdMap = nullptr;


    switch (cmdCat) {
    case CMD_LIST_PRIVATE:
        cmdMap = &m_mCmd_Private;
        break;
    case CMD_LIST_TRADE:
        cmdMap = &m_mCmd_Trade;
        break;
    case CMD_LIST_PARTY:
        cmdMap = &m_mCmd_Party;
        break;
    case CMD_LIST_CLAN:
        cmdMap = &m_mCmd_Clan;
        break;
/* in ver 1298 enable this
    case CMD_LIST_KNIGHTS:
        cmdMap = &m_mCmd_Knights;
        break;
    case CMD_LIST_GUARDIAN:
        cmdMap = &m_mCmd_Guard;
        break;
    case CMD_LIST_KING:
        cmdMap = &m_mCmd_King;
        break;
*/
    case CMD_LIST_GM - 3: // - 3 to disable CMD_LIST_KNIGHTS, CMD_LIST_GUARDIAN, CMD_LIST_KING -> in ver 1298 remove -3
        if (CGameBase::s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) {
            cmdMap = &m_mCmd_GM;
        }
        break;
    }

    if (cmdMap) {
        for (const auto & cmdPair : *cmdMap) {
            const CommandInfo & info = cmdPair.second;
            char                tooltipBuffer[256];
            sprintf_s(tooltipBuffer, sizeof(tooltipBuffer), info.tooltip.c_str(), info.name.c_str());
            m_pList_Cmds->AddStringTooltip(info.name, 0xFFC6C6FB, tooltipBuffer, 0xFF80FF80);
        }
    }

    int rangeMax = m_pList_Cmds->GetCount() - MAX_VISIBLE_ITEMS;
    m_pScroll_Commands->SetVisibleWithNoSound(rangeMax >= 0);
    m_pScroll_Commands->SetRange(0, rangeMax);

    return true;
}

bool CUICmdList::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (!pSender) {
        return false;
    }

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_cancel) {
            SetVisible(false);
            return true;
        }
    } else if (dwMsg == UIMSG_LIST_SELCHANGE) {
        if (pSender == m_pList_CmdCat) {
            iCommandListCategory iSel =
                static_cast<iCommandListCategory>(m_pList_CmdCat->GetCurSel() + COMMANDS_CATEGORY_NAMES_INDEX);
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

void CUICmdList::Render() {
    if (!m_bVisible) {
        return;
    }

    CN3UIBase::Render();
}

bool CUICmdList::OnKeyPress(int iKey) {
    return CN3UIBase::OnKeyPress(iKey);
}

void CUICmdList::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);
    if (bVisible) {
        CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
    } else {
        CGameProcedure::s_pUIMgr->ReFocusUI(); // this_ui
    }
}

bool CUICmdList::ExecuteCommand(uint8_t cmdSel) {
    std::string command;
    m_pList_Cmds->GetString(cmdSel, command);

    switch (m_pList_CmdCat->GetCurSel() + COMMANDS_CATEGORY_NAMES_INDEX) {
    case CMD_LIST_PRIVATE:
        ProcessCommand_Private(command);
        break;
    case CMD_LIST_TRADE:
    case CMD_LIST_PARTY:
    case CMD_LIST_CLAN:
//    case CMD_LIST_KNIGHTS: // -> in ver 1298 enable: CMD_LIST_KNIGHTS, CMD_LIST_GUARDIAN, CMD_LIST_KING
//    case CMD_LIST_GUARDIAN:
        command = '/' + command;
        CGameProcedure::s_pProcMain->ParseChattingCommand(command);
        break;
//    case CMD_LIST_KING:

//       break;
    case CMD_LIST_GM - 3: // - 3 to disable CMD_LIST_KNIGHTS, CMD_LIST_GUARDIAN, CMD_LIST_KING -> in ver 1298 remove -3
        ProcessCommand_GM(command);
        break;
    default:
        break;
    }

    return true;
}


void CUICmdList::ProcessCommand_Private(std::string command) {

    int cmdSel = m_pList_Cmds->GetCurSel();

    switch (cmdSel + COMMANDS_CATEGORY_PRIVATE_INDEX) {
    case CMD_WHISPER:
        CGameProcedure::s_pProcMain->OpenCmdEdit(command);
        break;
    case CMD_TOWN:
    case CMD_EXIT:
    case CMD_GREETING:
    case CMD_GREETING2:
    case CMD_GREETING3:
    case CMD_PROVOKE:
    case CMD_PROVOKE2:
    case CMD_PROVOKE3:
    case CMD_GAME_SAVE:
    case CMD_RECOMMEND:
    case CMD_INDIVIDUAL_BATTLE:
        CGameProcedure::s_pProcMain->ParseChattingCommand('/' + command);
        break;
    }
}
void CUICmdList::ProcessCommand_GM(std::string command) {

    int cmdSel = m_pList_Cmds->GetCurSel();

    switch (cmdSel + COMMANDS_CATEGORY_GM_INDEX) {
    case CMD_VISIBLE:
    case CMD_CLEAN:
    case CMD_RAINING:
    case CMD_SNOWING:
    case CMD_TIME:
    case CMD_NOTICE:
    case CMD_ARREST:
    case CMD_FORBIDCONNECT:
    case CMD_FORBIDCHAT:
    case CMD_PERMITCHAT:
    case CMD_NOTICEALL:
    case CMD_FORBIDUSER:
    case CMD_SUMMONUSER:
    case CMD_ATTACKDISABLE:
    case CMD_ATTACKENABLE:
        CGameProcedure::s_pProcMain->OpenCmdEdit(command);
        break;
    case CMD_INVISIBLE:
    case CMD_CU_COUNT:
    case CMD_CUTOFF:
    case CMD_VIEW:
    case CMD_PLC:
        CGameProcedure::s_pProcMain->ParseChattingCommand('/' + command);
        break;
    }
}
