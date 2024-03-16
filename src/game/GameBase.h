// N3GameBase.h: interface for the CN3GameBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include "N3Base/N3TableBase.h"
#include "GameDef.h"

class CGameBase : public CN3Base {
#define ACT_WORLD s_pWorldMgr->GetActiveWorld()

  public:
    static CN3TableBase<struct __TABLE_ZONE> *     s_pTbl_Zones; // Table for Zone information..
    static CN3TableBase<struct __TABLE_UI_RESRC> * s_pTbl_UI;    // UI FileName Table
    static CN3TableBase<struct __TABLE_ITEM_BASIC> *
        s_pTbl_Items_Basic; // Equipped item resource table for each class of each user (myself and others around me)
    static CN3TableBase<struct __TABLE_ITEM_EXT> *
        s_pTbl_Items_Exts[MAX_ITEM_EXTENSION]; // Equipped item resource table for each class of each user (myself and others around me)
    static CN3TableBase<struct __TABLE_PLAYER_LOOKS> *
        s_pTbl_UPC_Looks; // Default skin for each class of each user (myself and others around me)
    static CN3TableBase<struct __TABLE_PLAYER_LOOKS> *
        s_pTbl_NPC_Looks; // Basic appearance of NPC, Mob - 6 character parts(), 2 plugs
    static CN3TableBase<struct __TABLE_UPC_SKILL> *      s_pTbl_Skill;          // Table for skill information
    static CN3TableBase<struct __TABLE_EXCHANGE_QUEST> * s_pTbl_Exchange_Quest; // Table of exchange quests..
    static CN3TableBase<struct __TABLE_FX> *             s_pTbl_FXSource;       // FX Source Information Table..
    static CN3TableBase<struct __TABLE_QUEST_MENU> *     s_pTbl_QuestMenu;      // Quest Select Menu
    static CN3TableBase<struct __TABLE_QUEST_TALK> *     s_pTbl_QuestTalk;      // Quest fingerprint
    static CN3TableBase<struct __TABLE_TEXTS> *          s_pTbl_Texts;          // Localized strings
    static CN3TableBase<struct __TABLE_HELP> *           s_pTbl_Help;           // Leveling Guide

    static class CN3WorldManager * s_pWorldMgr; // World manager class..
    static class CPlayerOtherMgr * s_pOPMgr;    // Other Player Manager - Another user management class..
    static class CPlayerMySelf *   s_pPlayer;   // user class...

  protected:
    static void StaticMemberInit();    // Initialization of tables, basic terrain, objects, characters, etc.
    static void StaticMemberRelease(); // Initialization of tables, basic terrain, objects, characters, etc.

  public:
    static bool              GetTextByAttrib(e_ItemAttrib eAttrib, std::string & szAttrib);
    static bool              GetTextByClass(e_Class eClass, std::string & szText);
    static bool              GetTextByItemClass(e_ItemClass eItemClass, std::string & szText);
    static bool              GetTextByKnightsDuty(e_KnightsDuty eDuty, std::string & szText);
    static bool              GetTextByNation(e_Nation eNation, std::string & szText);
    static bool              GetTextByRace(e_Race eRace, std::string & szText);
    static D3DCOLOR          GetIDColorByLevelDifference(int iLevelDiff); // ID color according to level difference is returned.
    static e_Class_Represent GetRepresentClass(e_Class eClass); // If you enter a detailed job, it returns the job represented.
    static e_ItemType        MakeResrcFileNameForUPC(__TABLE_ITEM_BASIC * pItem,         // item data...
                                                     std::string *        szResrcFN,     // Resource FileName
                                                     std::string *        szIconFN,      // Icon FileName
                                                     e_PartPosition &     ePartPosition, // Index in case of Part
                                                     e_PlugPosition &     ePlugPosition);    // Index in case of Plug

    class CPlayerBase * CharacterGetByID(int iID, bool bFromAlive);
    bool                IsValidCharacter(CPlayerBase * pCharacter);

    CGameBase();
    virtual ~CGameBase();
};
