// UICmdListDlg.h: interface for the CUICmdListDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIList.h"
#include "N3Base/N3UIEdit.h"

//////////////////////////////////////////////////////////////////////
const int MAX_VISIBLE_ITEMS = 15;
const int COMMANDS_CATEGORY_NAMES_INDEX    = 7800;
const int COMMANDS_CATEGORY_TOOLTIP_INDEX  = 7900;
const int COMMANDS_CATEGORY_PRIVATE_INDEX  = 8000;
const int COMMANDS_CATEGORY_TRADE_INDEX    = 8200;
const int COMMANDS_CATEGORY_PARTY_INDEX    = 8400;
const int COMMANDS_CATEGORY_CLAN_INDEX     = 8600;
const int COMMANDS_CATEGORY_KNIGHTS_INDEX  = 8800;
const int COMMANDS_CATEGORY_GUARDIAN_INDEX = 9200;
const int COMMANDS_CATEGORY_KING_INDEX     = 9400;
const int COMMANDS_CATEGORY_GM_INDEX       = 9000;
const int COMMANDS_TOOLTIP_INDEX = 100;

enum iCommandListCategory {
    CMD_LIST_PRIVATE = IDS_CMD_CATEGORY_PRIVATE,
    CMD_LIST_TRADE = IDS_CMD_CATEGORY_TRADE,
    CMD_LIST_PARTY = IDS_CMD_CATEGORY_PARTY,
    CMD_LIST_CLAN = IDS_CMD_CATEGORY_CLAN,
    CMD_LIST_KNIGHTS = IDS_CMD_CATEGORY_KNIGHTS,
    CMD_LIST_GUARDIAN = IDS_CMD_CATEGORY_GUARDIAN,
    CMD_LIST_KING = IDS_CMD_CATEGORY_KING,
    CMD_LIST_GM = IDS_CMD_CATEGORY_GM,
    CMD_LIST_COUNT = 8
};

enum iPrivateCommand {
    CMD_WHISPER = IDS_CMD_WHISPER,
    CMD_TOWN = IDS_CMD_TOWN,
    CMD_EXIT = IDS_CMD_EXIT,
    CMD_GREETING = IDS_CMD_GREETING,
    CMD_GREETING2 = IDS_CMD_GREETING2,
    CMD_GREETING3 = IDS_CMD_GREETING3,
    CMD_PROVOKE = IDS_CMD_PROVOKE,
    CMD_PROVOKE2 = IDS_CMD_PROVOKE2,
    CMD_PROVOKE3 = IDS_CMD_PROVOKE3,
    CMD_GAME_SAVE = IDS_CMD_GAME_SAVE,
    CMD_RECOMMEND = IDS_CMD_RECOMMEND,
    CMD_INDIVIDUAL_BATTLE = IDS_CMD_INDIVIDUAL_BATTLE,
    CMD_PRIVATE_COUNT = 12
};

enum iTradeCommand {
    CMD_TRADE = IDS_CMD_TRADE,
    CMD_FORBIDTRADE = IDS_CMD_FORBIDTRADE,
    CMD_PERMITTRADE = IDS_CMD_PERMITTRADE,
    CMD_MERCHANT = IDS_CMD_MERCHANT,
    CMD_TRADE_COUNT = 5
};

enum iPartyCommand {
    CMD_PARTY = IDS_CMD_PARTY,
    CMD_LEAVEPARTY = IDS_CMD_LEAVEPARTY,
    CMD_RECRUITPARTY = IDS_CMD_RECRUITPARTY,
    CMD_SEEKING_PARTY = IDS_CMD_SEEKING_PARTY,
    CMD_FORBIDPARTY = IDS_CMD_FORBIDPARTY,
    CMD_PERMITPARTY = IDS_CMD_PERMITPARTY,
    CMD_PARTY_COUNT = 7
};

enum iClanCommand {
    CMD_JOINCLAN = IDS_CMD_JOINCLAN,
    CMD_WITHDRAWCLAN = IDS_CMD_WITHDRAWCLAN,
    CMD_FIRECLAN = IDS_CMD_FIRECLAN,
    CMD_COMMAND = IDS_CMD_COMMAND,
    CMD_CLAN_WAR = IDS_CMD_CLAN_WAR,
    CMD_SURRENDER = IDS_CMD_SURRENDER,
    CMD_APPOINTVICECHIEF = IDS_CMD_APPOINTVICECHIEF,
    CMD_CLAN_CHAT = IDS_CMD_CLAN_CHAT,
    CMD_CLAN_BATTLE = IDS_CMD_CLAN_BATTLE,
    CMD_CLAN_COUNT = 10
};

enum iKnightsCommand {
    CMD_CONFEDERACY = IDS_CMD_CONFEDERACY,
    CMD_BAN_KNIGHTS = IDS_CMD_BAN_KNIGHTS,
    CMD_QUIT_KNIGHTS = IDS_CMD_QUIT_KNIGHTS,
    CMD_BASE = IDS_CMD_BASE,
    CMD_DECLARATION = IDS_CMD_DECLARATION,
    CMD_KNGIHTS_COUNT = 6
};

enum iGuardianCommand {
    CMD_GUARD_HIDE = IDS_CMD_GUARD_HIDE,
    CMD_GUARD = IDS_CMD_GUARD,
    CMD_GUARD_DEFEND = IDS_CMD_GUARD_DEFEND,
    CMD_GUARD_LOOK_OUT = IDS_CMD_GUARD_LOOK_OUT,
    CMD_GUARD_FORMATION = IDS_CMD_GUARD_FORMATION,
    CMD_GUARD_REST = IDS_CMD_GUARD_REST,
    CMD_GUARD_DESTROY = IDS_CMD_GUARD_DESTROY,
    CMD_GUARD_COUNT = 8
};
enum iKingCommand {
    CMD_KING_ROYALORDER = IDS_CMD_KING_ROYALORDER,
    CMD_KING_PRIZE = IDS_CMD_KING_PRIZE,
    CMD_KING_EXPRATE = IDS_CMD_KING_EXPRATE,
    CMD_KING_DROPRATE = IDS_CMD_KING_DROPRATE,
    CMD_KING_RAIN = IDS_CMD_KING_RAIN,
    CMD_KING_SNOW = IDS_CMD_KING_SNOW,
    CMD_KING_CLEAR = IDS_CMD_KING_CLEAR,
    CMD_KING_REWARD = IDS_CMD_KING_REWARD,
    CMD_KING_COUNT = 9
};

enum iGmCommand {
    CMD_VISIBLE = IDS_CMD_VISIBLE,
    CMD_INVISIBLE = IDS_CMD_INVISIBLE,
    CMD_CLEAN = IDS_CMD_CLEAN,
    CMD_RAINING = IDS_CMD_RAINING,
    CMD_SNOWING = IDS_CMD_SNOWING,
    CMD_TIME = IDS_CMD_TIME,
    CMD_CU_COUNT = IDS_CMD_CU_COUNT,
    CMD_NOTICE = IDS_CMD_NOTICE,
    CMD_ARREST = IDS_CMD_ARREST,
    CMD_FORBIDCONNECT = IDS_CMD_FORBIDCONNECT,
    CMD_FORBIDCHAT = IDS_CMD_FORBIDCHAT,
    CMD_PERMITCHAT = IDS_CMD_PERMITCHAT,
    CMD_NOTICEALL = IDS_CMD_NOTICEALL,
    CMD_CUTOFF = IDS_CMD_CUTOFF,
    CMD_VIEW = IDS_CMD_VIEW,
    CMD_UNVIEW = IDS_CMD_UNVIEW,
    CMD_FORBIDUSER = IDS_CMD_FORBIDUSER,
    CMD_SUMMONUSER = IDS_CMD_SUMMONUSER,
    CMD_ATTACKDISABLE = IDS_CMD_ATTACKDISABLE,
    CMD_ATTACKENABLE = IDS_CMD_ATTACKENABLE,
    CMD_PLC = IDS_CMD_PLC,
    CMD_GM_COUNT = 22
};

class CUICmdList : public CN3UIBase {
    bool               m_bDestoyDlgAlive;
    class CUICmdEdit * m_pUICmdEdit;
    CN3UIButton *      m_pBtn_cancel;
    CN3UIList *        m_pList_CmdCat;
    CN3UIList *        m_pList_CommandsPrivate;
    CN3UIList *        m_pList_CommandsTrade;
    CN3UIList *        m_pList_CommandsClan;
    CN3UIList *        m_pList_CommandsKnights;
    CN3UIList *        m_pList_CommandsParty;
    CN3UIList *        m_pList_CommandsGuardianMonster;
    CN3UIList *        m_pList_CommandsKing;
    CN3UIList *        m_pList_CommandsGM;
    CN3UIList *        m_pList_Cmds;
    CN3UIScrollBar *   m_pScroll_Categories;
    CN3UIScrollBar *   m_pScroll_Commands;

  public:
    struct CommandInfo {
        std::string name;
        std::string tooltip;
    };

    std::map<int, CommandInfo> m_mCmd_Private;
    std::map<int, CommandInfo> m_mCmd_Trade;
    std::map<int, CommandInfo> m_mCmd_Party;
    std::map<int, CommandInfo> m_mCmd_Clan;
    std::map<int, CommandInfo> m_mCmd_Knights;
    std::map<int, CommandInfo> m_mCmd_Guard;
    std::map<int, CommandInfo> m_mCmd_King;
    std::map<int, CommandInfo> m_mCmd_GM;
    std::map<std::string, int> g_commandMappings;

  protected:
    bool  m_bOpenningNow;
    bool  m_bClosingNow;
    float m_fMoveDelta;
    int   m_iRBtnDownOffs;
    bool  m_bRBtnProcessing;
    std::string                szGmCategory;
    std::map<int, CommandInfo> m_CommandMap;

  public:
    void SetVisible(bool bVisible) override;

    bool         OnKeyPress(int iKey) override;
    void         Release() override;
    void         Tick() override;
    bool         ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) override;
    void         Render();
    void         Open();
    void         Close(bool bByKey = false);
    bool         Load(HANDLE hFile);
    void         InitCommands();
    bool         CreateCategoryList();
    bool         UpdateCommandList(iCommandListCategory cmd);
    void         LoadCommandMap(std::map<int, CommandInfo> & cmdMap, int startId, int endId);
    bool         ExecuteCommand(uint8_t cmdSel);
    void         ProcessCommand_Private(std::string command);
    void         ProcessCommand_GM(std::string command);

    CUICmdList();
    virtual ~CUICmdList();
};
