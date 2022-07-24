// GameBase.cpp: implementation of the CGameBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "GameBase.h"
#include "N3WorldManager.h"
#include "PlayerOtherMgr.h"
#include "PlayerMySelf.h"

#include "N3Base/N3ShapeMgr.h"

CN3TableBase<__TABLE_ZONE>*				CGameBase::s_pTbl_Zones = NULL;				// Zone ������ ���� Table
CN3TableBase<__TABLE_UI_RESRC>*			CGameBase::s_pTbl_UI = NULL;				// UI FileName Table
CN3TableBase<__TABLE_ITEM_BASIC>*		CGameBase::s_pTbl_Items_Basic = NULL;			// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ ���� ������ ���ҽ� ���̺�
CN3TableBase<__TABLE_ITEM_EXT>*			CGameBase::s_pTbl_Items_Exts[MAX_ITEM_EXTENSION] = 
											{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };			// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ ���� ������ ���ҽ� ���̺�
CN3TableBase<__TABLE_PLAYER_LOOKS>*		CGameBase::s_pTbl_UPC_Looks = NULL;			// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ �⺻ ��Ų
CN3TableBase<__TABLE_PLAYER_LOOKS>*		CGameBase::s_pTbl_NPC_Looks = NULL;			// NPC, Mob �⺻ ��� - 6���� ĳ���� ��Ʈ(), 2���� �÷���
CN3TableBase<__TABLE_UPC_SKILL>*		CGameBase::s_pTbl_Skill = NULL;				// Skill ������ ���� Table
CN3TableBase<__TABLE_EXCHANGE_QUEST>*	CGameBase::s_pTbl_Exchange_Quest = NULL;	// ��ȯ ����Ʈ�� ���� ���̺�..
CN3TableBase<__TABLE_FX>*				CGameBase::s_pTbl_FXSource;		// FX�ҽ� ���� ���̺�..
CN3TableBase<__TABLE_QUEST_MENU>*		CGameBase::s_pTbl_QuestMenu		= NULL;		// ����Ʈ ���� �޴�
CN3TableBase<__TABLE_QUEST_TALK>*		CGameBase::s_pTbl_QuestTalk		= NULL;		// ����Ʈ ����


CN3WorldManager*	CGameBase::s_pWorldMgr = NULL;		// ���� �Ŵ���..
CPlayerOtherMgr*	CGameBase::s_pOPMgr = NULL;				// Other Player Manager - �ٸ� ���� ���� Ŭ����..
CPlayerMySelf*		CGameBase::s_pPlayer = NULL;			// ���� Ŭ����..
	
CGameBase::CGameBase()
{
}

CGameBase::~CGameBase()
{
}

void CGameBase::StaticMemberInit()
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// Resource Table �ε� �� �ʱ�ȭ...
	s_pTbl_Zones			= new CN3TableBase<__TABLE_ZONE>;			// Zone ������ ���� Table
	s_pTbl_UI				= new CN3TableBase<__TABLE_UI_RESRC>;		// UI Resource File Table loading
	s_pTbl_UPC_Looks		= new CN3TableBase<__TABLE_PLAYER_LOOKS>;	// �÷��̾���� �⺻ ����� �Ǵ� NPC Resource Table loading
	s_pTbl_Items_Basic		= new CN3TableBase<__TABLE_ITEM_BASIC>;		// Item Resource Table loading
	for(int i = 0; i < MAX_ITEM_EXTENSION; i++)
		s_pTbl_Items_Exts[i] = new CN3TableBase<__TABLE_ITEM_EXT>;
	s_pTbl_NPC_Looks		= new CN3TableBase<__TABLE_PLAYER_LOOKS>;	// NPC Resource Table loading
	s_pTbl_Skill			= new CN3TableBase<__TABLE_UPC_SKILL>;		// Skill ������ ���� Table
	s_pTbl_Exchange_Quest	= new CN3TableBase<__TABLE_EXCHANGE_QUEST>;	// ��ȯ ����Ʈ�� ���� ���̺�..
	s_pTbl_FXSource			= new CN3TableBase<__TABLE_FX>;				// FX Source�� ���� ���̺�..
	s_pTbl_QuestMenu		= new CN3TableBase<__TABLE_QUEST_MENU>;
	s_pTbl_QuestTalk		= new CN3TableBase<__TABLE_QUEST_TALK>;

	std::string szLangTail = ".tbl";
	int iLangID = ::GetUserDefaultLangID();
	if(0x0404 == iLangID) szLangTail = "_ch.tbl"; // Taiwan Language

	std::string szFN;
	szFN = "Data\\Zones.tbl";				s_pTbl_Zones->LoadFromFile(szFN.c_str());		// Zone ������ ���� Table
	szFN = "Data\\UIs" + szLangTail;		s_pTbl_UI->LoadFromFile(szFN.c_str());			// UI Resource File Table loading
	szFN = "Data\\UPC_DefaultLooks.tbl";	s_pTbl_UPC_Looks->LoadFromFile(szFN.c_str());	// �÷��̾���� �⺻ ����� �Ǵ� NPC Resource Table loading
	szFN = "Data\\Item_Org" + szLangTail;	s_pTbl_Items_Basic->LoadFromFile(szFN.c_str());	// Item Resource Table loading

	szFN = "Data\\Quest_Menu" + szLangTail;	s_pTbl_QuestMenu->LoadFromFile(szFN.c_str());	// ����Ʈ ���� ���ø޴�
	szFN = "Data\\Quest_Talk" + szLangTail;	s_pTbl_QuestTalk->LoadFromFile(szFN.c_str());	// ����Ʈ ���� ����

	for(i = 0; i < MAX_ITEM_EXTENSION; i++)
	{
		char szFNTmp[256] = "";
		sprintf(szFNTmp, "Data\\Item_Ext_%d", i);
		szFN = szFNTmp + szLangTail;
		s_pTbl_Items_Exts[i]->LoadFromFile(szFN.c_str());
	}

	szFN = "Data\\NPC_Looks.tbl";					s_pTbl_NPC_Looks->LoadFromFile(szFN.c_str());		// NPC Resource Table loading
	szFN = "Data\\skill_magic_main" + szLangTail;	s_pTbl_Skill->LoadFromFile(szFN.c_str());			// Skill ������ ���� Table
	szFN = "Data\\Exchange_Quest.tbl";				s_pTbl_Exchange_Quest->LoadFromFile(szFN.c_str());	// ��ȯ ����Ʈ�� ���� ���̺�..
	szFN = "Data\\fx.tbl";							s_pTbl_FXSource->LoadFromFile(szFN.c_str());


	s_pWorldMgr = new CN3WorldManager();
	s_pOPMgr = new CPlayerOtherMgr();
	s_pPlayer = new CPlayerMySelf(); // �⺻���� �� �÷��̾� ����..
}

void CGameBase::StaticMemberRelease()
{
	// Tables ....
	delete s_pTbl_Zones; s_pTbl_Zones = NULL;		// Zone ������ ���� Table
	delete s_pTbl_UI; s_pTbl_UI = NULL;					// UI Resource File Table loading
	delete s_pTbl_UPC_Looks; s_pTbl_UPC_Looks = NULL;	// �÷��̾���� �⺻ ����� �Ǵ� NPC Resource Table loading
	delete s_pTbl_Items_Basic; s_pTbl_Items_Basic = NULL;	// Item Resource Table loading
	for(int i = 0; i < MAX_ITEM_EXTENSION; i++)
	{
		delete s_pTbl_Items_Exts[i];
		s_pTbl_Items_Exts[i] = NULL;
	}
	delete s_pTbl_NPC_Looks; s_pTbl_NPC_Looks = NULL;			// NPC Resource Table loading
	delete s_pTbl_Skill; s_pTbl_Skill = NULL;					// Skill ������ ���� Table
	delete s_pTbl_Exchange_Quest; s_pTbl_Exchange_Quest = NULL;	// Skill ������ ���� Table
	delete s_pTbl_FXSource; s_pTbl_FXSource = NULL;				// FX Source�� ���� ���̺�..	
	delete s_pTbl_QuestMenu; s_pTbl_QuestMenu = NULL;			// ����Ʈ ���� ���ø޴�
	delete s_pTbl_QuestTalk; s_pTbl_QuestTalk = NULL;		// ����Ʈ ���� ����

	
	delete s_pPlayer;	s_pPlayer = NULL;		// Player Character
	delete s_pOPMgr;	s_pOPMgr = NULL;
	delete s_pWorldMgr;	s_pWorldMgr = NULL;
}

bool CGameBase::GetTextByClass(e_Class eClass, std::string& szText)
{
	switch(eClass)
	{
		case CLASS_KINDOF_WARRIOR: ::_LoadStringFromResource(IDS_CLASS_KINDOF_WARRIOR, szText); break;
		case CLASS_KINDOF_ROGUE: ::_LoadStringFromResource(IDS_CLASS_KINDOF_ROGUE, szText); break;
		case CLASS_KINDOF_WIZARD: ::_LoadStringFromResource(IDS_CLASS_KINDOF_WIZARD, szText); break;
		case CLASS_KINDOF_PRIEST: ::_LoadStringFromResource(IDS_CLASS_KINDOF_PRIEST, szText); break;
		case CLASS_KINDOF_ATTACK_WARRIOR: ::_LoadStringFromResource(IDS_CLASS_KINDOF_ATTACK_WARRIOR, szText); break;
		case CLASS_KINDOF_DEFEND_WARRIOR: ::_LoadStringFromResource(IDS_CLASS_KINDOF_DEFEND_WARRIOR, szText); break;
		case CLASS_KINDOF_ARCHER: ::_LoadStringFromResource(IDS_CLASS_KINDOF_ARCHER, szText); break;
		case CLASS_KINDOF_ASSASSIN: ::_LoadStringFromResource(IDS_CLASS_KINDOF_ASSASSIN, szText); break;
		case CLASS_KINDOF_ATTACK_WIZARD: ::_LoadStringFromResource(IDS_CLASS_KINDOF_ATTACK_WIZARD, szText); break;
		case CLASS_KINDOF_PET_WIZARD: ::_LoadStringFromResource(IDS_CLASS_KINDOF_PET_WIZARD, szText); break;
		case CLASS_KINDOF_HEAL_PRIEST: ::_LoadStringFromResource(IDS_CLASS_KINDOF_HEAL_PRIEST, szText); break;
		case CLASS_KINDOF_CURSE_PRIEST: ::_LoadStringFromResource(IDS_CLASS_KINDOF_CURSE_PRIEST, szText); break;

		case CLASS_EL_WARRIOR:	case CLASS_KA_WARRIOR:	::_LoadStringFromResource(IDS_CLASS_WARRIOR, szText); break;
		case CLASS_EL_ROGUE:	case CLASS_KA_ROGUE:	::_LoadStringFromResource(IDS_CLASS_ROGUE, szText); break;
		case CLASS_EL_WIZARD:	case CLASS_KA_WIZARD:	::_LoadStringFromResource(IDS_CLASS_WIZARD, szText); break;
		case CLASS_EL_PRIEST:	case CLASS_KA_PRIEST:	::_LoadStringFromResource(IDS_CLASS_PRIEST, szText); break;
		
		case CLASS_KA_BERSERKER:	::_LoadStringFromResource(IDS_CLASS_KA_BERSERKER, szText); break;
		case CLASS_KA_GUARDIAN:		::_LoadStringFromResource(IDS_CLASS_KA_GUARDIAN, szText); break;
		case CLASS_KA_HUNTER:		::_LoadStringFromResource(IDS_CLASS_KA_HUNTER, szText); break;
		case CLASS_KA_PENETRATOR:	::_LoadStringFromResource(IDS_CLASS_KA_PENETRATOR, szText); break;
		case CLASS_KA_SORCERER:		::_LoadStringFromResource(IDS_CLASS_KA_SORCERER, szText); break;
		case CLASS_KA_NECROMANCER:	::_LoadStringFromResource(IDS_CLASS_KA_NECROMANCER, szText); break;
		case CLASS_KA_SHAMAN:		::_LoadStringFromResource(IDS_CLASS_KA_SHAMAN, szText); break;
		case CLASS_KA_DARKPRIEST:	::_LoadStringFromResource(IDS_CLASS_KA_DARKPRIEST, szText); break;
		
		case CLASS_EL_BLADE:		::_LoadStringFromResource(IDS_CLASS_EL_BLADE, szText); break;
		case CLASS_EL_PROTECTOR:	::_LoadStringFromResource(IDS_CLASS_EL_PROTECTOR, szText); break;
		case CLASS_EL_RANGER:		::_LoadStringFromResource(IDS_CLASS_EL_RANGER, szText); break;
		case CLASS_EL_ASSASIN:		::_LoadStringFromResource(IDS_CLASS_EL_ASSASIN, szText); break;
		case CLASS_EL_MAGE:			::_LoadStringFromResource(IDS_CLASS_EL_MAGE, szText); break;
		case CLASS_EL_ENCHANTER:	::_LoadStringFromResource(IDS_CLASS_EL_ENCHANTER, szText); break;
		case CLASS_EL_CLERIC:		::_LoadStringFromResource(IDS_CLASS_EL_CLERIC, szText); break;
		case CLASS_EL_DRUID:		::_LoadStringFromResource(IDS_CLASS_EL_DRUID, szText); break;
		
		default: __ASSERT(0, "Invalid Class"); szText = "Unknown Class"; return false;
	}

	return true;
}

bool CGameBase::GetTextByKnightsDuty(e_KnightsDuty eDuty, std::string& szText)
{
	switch(eDuty)
	{
		case KNIGHTS_DUTY_UNKNOWN:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_UNKNOWN, szText); break; // ????? �Ѱܳ�??
		case KNIGHTS_DUTY_PUNISH:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_PUNISH, szText); break; // ¡����.
		case KNIGHTS_DUTY_TRAINEE:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_TRAINEE, szText); break; // �߽����
		case KNIGHTS_DUTY_KNIGHT:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_KNIGHT, szText); break; // �Ϲݱ��
		case KNIGHTS_DUTY_OFFICER:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_OFFICER, szText); break; // �屳
		case KNIGHTS_DUTY_VICECHIEF:	::_LoadStringFromResource(IDS_KNIGHTS_DUTY_VICECHIEF, szText); break; // �δ���.
		case KNIGHTS_DUTY_CHIEF:		::_LoadStringFromResource(IDS_KNIGHTS_DUTY_CHIEF, szText); break; // ������ ����..
		default: __ASSERT(0, "Invalid Knights Duty"); szText = "Unknown Duty"; return false;
	}

	return true;
}

bool CGameBase::GetTextByItemClass(e_ItemClass eItemClass, std::string& szText)
{
	switch(eItemClass)
	{
		case ITEM_CLASS_DAGGER:			::_LoadStringFromResource(IDS_ITEM_CLASS_DAGGER, szText); break; // �ܰ�(dagger)
		case ITEM_CLASS_SWORD:			::_LoadStringFromResource(IDS_ITEM_CLASS_SWORD, szText); break; // �Ѽհ�(onehandsword)
		case ITEM_CLASS_SWORD_2H:		::_LoadStringFromResource(IDS_ITEM_CLASS_SWORD_2H, szText); break; // 3 : ��հ�(twohandsword)
		case ITEM_CLASS_AXE:			::_LoadStringFromResource(IDS_ITEM_CLASS_AXE, szText); break; // �Ѽյ���(onehandaxe)
		case ITEM_CLASS_AXE_2H:			::_LoadStringFromResource(IDS_ITEM_CLASS_AXE_2H, szText); break; // �μյ���(twohandaxe)
		case ITEM_CLASS_MACE:			::_LoadStringFromResource(IDS_ITEM_CLASS_MACE, szText); break; // �Ѽ�Ÿ�ݹ���(mace)
		case ITEM_CLASS_MACE_2H:		::_LoadStringFromResource(IDS_ITEM_CLASS_MACE_2H, szText); break; // �μ�Ÿ�ݹ���(twohandmace)
		case ITEM_CLASS_SPEAR:			::_LoadStringFromResource(IDS_ITEM_CLASS_SPEAR, szText); break; // â(spear)
		case ITEM_CLASS_POLEARM:		::_LoadStringFromResource(IDS_ITEM_CLASS_POLEARM, szText); break; // ����(polearm)

		case ITEM_CLASS_SHIELD:			::_LoadStringFromResource(IDS_ITEM_CLASS_SHIELD, szText); break; // ����(shield)

		case ITEM_CLASS_BOW:			::_LoadStringFromResource(IDS_ITEM_CLASS_BOW, szText); break; //  ��Ʈ����(Shortbow)
		case ITEM_CLASS_BOW_CROSS:		::_LoadStringFromResource(IDS_ITEM_CLASS_BOW_CROSS, szText); break; // ũ�ν�����(crossbow)
		case ITEM_CLASS_BOW_LONG:		::_LoadStringFromResource(IDS_ITEM_CLASS_BOW_LONG, szText); break; // �պ���(longbow)

		case ITEM_CLASS_EARRING:		::_LoadStringFromResource(IDS_ITEM_CLASS_EARRING, szText); break; // �Ͱ���
		case ITEM_CLASS_AMULET:			::_LoadStringFromResource(IDS_ITEM_CLASS_AMULET, szText); break; // �����
		case ITEM_CLASS_RING:			::_LoadStringFromResource(IDS_ITEM_CLASS_RING, szText); break; // ����
		case ITEM_CLASS_BELT:			::_LoadStringFromResource(IDS_ITEM_CLASS_BELT, szText); break; // �㸮��
		case ITEM_CLASS_CHARM:			::_LoadStringFromResource(IDS_ITEM_CLASS_CHARM, szText); break; //�κ��丮�� ���ϰ� �ִ� ������
		case ITEM_CLASS_JEWEL:			::_LoadStringFromResource(IDS_ITEM_CLASS_JEWEL, szText); break; //��������
		case ITEM_CLASS_POTION:			::_LoadStringFromResource(IDS_ITEM_CLASS_POTION, szText); break; // ����
		case ITEM_CLASS_SCROLL:			::_LoadStringFromResource(IDS_ITEM_CLASS_SCROLL, szText); break; // ��ũ��



		case ITEM_CLASS_LAUNCHER:		::_LoadStringFromResource(IDS_ITEM_CLASS_LAUNCHER, szText); break; 
						
		case ITEM_CLASS_STAFF:			::_LoadStringFromResource(IDS_ITEM_CLASS_STAFF, szText); break; // ������(staff)
		case ITEM_CLASS_ARROW:			::_LoadStringFromResource(IDS_ITEM_CLASS_ARROW, szText); break; // ȭ��(Arrow)
		case ITEM_CLASS_JAVELIN:		::_LoadStringFromResource(IDS_ITEM_CLASS_JAVELIN, szText); break; // ��â
		
		case ITEM_CLASS_ARMOR_WARRIOR:	::_LoadStringFromResource(IDS_ITEM_CLASS_ARMOR_WARRIOR, szText); break; // ���� ��
		case ITEM_CLASS_ARMOR_ROGUE:	::_LoadStringFromResource(IDS_ITEM_CLASS_ARMOR_ROGUE, szText); break; // �α� ��
		case ITEM_CLASS_ARMOR_MAGE:		::_LoadStringFromResource(IDS_ITEM_CLASS_ARMOR_MAGE, szText); break; // ������ ��
		case ITEM_CLASS_ARMOR_PRIEST:	::_LoadStringFromResource(IDS_ITEM_CLASS_ARMOR_PRIEST, szText); break; // ���� ��
		default:
//			__ASSERT(0, "Invalid Item Class"); szText = "Unknonw Item Class";
			return false;
	}

	return true;
}

bool CGameBase::GetTextByAttrib(e_ItemAttrib eAttrib, std::string& szAttrib)
{
	switch(eAttrib)
	{
		case ITEM_ATTRIB_GENERAL:		::_LoadStringFromResource(IDS_ITEM_ATTRIB_GENERAL, szAttrib); break; // �ܰ�(dagger)
		case ITEM_ATTRIB_MAGIC:			::_LoadStringFromResource(IDS_ITEM_ATTRIB_MAGIC, szAttrib); break; // �Ѽհ�(onehandsword)
		case ITEM_ATTRIB_LAIR:			::_LoadStringFromResource(IDS_ITEM_ATTRIB_LAIR, szAttrib); break; // 3 : ��հ�(twohandsword)
		case ITEM_ATTRIB_CRAFT:			::_LoadStringFromResource(IDS_ITEM_ATTRIB_CRAFT, szAttrib); break; // �Ѽյ���(onehandaxe)
		case ITEM_ATTRIB_UNIQUE:		::_LoadStringFromResource(IDS_ITEM_ATTRIB_UNIQUE, szAttrib); break; // �μյ���(twohandaxe)
		case ITEM_ATTRIB_UPGRADE:		::_LoadStringFromResource(IDS_ITEM_ATTRIB_UPGRADE, szAttrib); break; // �Ѽ�Ÿ�ݹ���(mace)
		default:
			return false;
	}

	return true;
}

e_Class_Represent CGameBase::GetRepresentClass(e_Class eClass)
{
	switch(eClass)
	{
		case CLASS_KA_WARRIOR:
		case CLASS_KA_BERSERKER:
		case CLASS_KA_GUARDIAN:
		case CLASS_EL_WARRIOR:
		case CLASS_EL_BLADE:
		case CLASS_EL_PROTECTOR:
			return CLASS_REPRESENT_WARRIOR;

		case CLASS_KA_ROGUE:
		case CLASS_KA_HUNTER:
		case CLASS_KA_PENETRATOR:
		case CLASS_EL_ROGUE:
		case CLASS_EL_RANGER:
		case CLASS_EL_ASSASIN:
			return CLASS_REPRESENT_ROGUE;

		case CLASS_KA_WIZARD:
		case CLASS_KA_SORCERER:
		case CLASS_KA_NECROMANCER:
		case CLASS_EL_WIZARD:
		case CLASS_EL_MAGE:
		case CLASS_EL_ENCHANTER:
			return CLASS_REPRESENT_WIZARD;

		case CLASS_KA_PRIEST:
		case CLASS_KA_SHAMAN:
		case CLASS_KA_DARKPRIEST:
		case CLASS_EL_PRIEST:
		case CLASS_EL_CLERIC:
		case CLASS_EL_DRUID:
			return CLASS_REPRESENT_PRIEST;
	}

	return CLASS_REPRESENT_UNKNOWN;
}


bool CGameBase::GetTextByNation(e_Nation eNation, std::string& szText)
{
	switch(eNation)
	{
		case NATION_ELMORAD:	::_LoadStringFromResource(IDS_NATION_ELMORAD, szText); break;
		case NATION_KARUS:		::_LoadStringFromResource(IDS_NATION_KARUS, szText); break;
		default: ::_LoadStringFromResource(IDS_NATION_UNKNOWN, szText); return false;
	}

	return true;
}

bool CGameBase::GetTextByRace(e_Race eRace, std::string& szText)
{
	switch(eRace)
	{
		case RACE_EL_BABARIAN:		::_LoadStringFromResource(IDS_RACE_EL_BABARIAN, szText); break;
		case RACE_EL_MAN:			::_LoadStringFromResource(IDS_RACE_EL_MAN, szText); break;
		case RACE_EL_WOMEN:			::_LoadStringFromResource(IDS_RACE_EL_WOMEN, szText); break;

		case RACE_KA_ARKTUAREK:		::_LoadStringFromResource(IDS_RACE_KA_ARKTUAREK, szText); break;
		case RACE_KA_TUAREK:		::_LoadStringFromResource(IDS_RACE_KA_TUAREK, szText); break;
		case RACE_KA_WRINKLETUAREK:	::_LoadStringFromResource(IDS_RACE_KA_WRINKLETUAREK, szText); break;
		case RACE_KA_PURITUAREK:	::_LoadStringFromResource(IDS_RACE_KA_PURITUAREK, szText); break;
			
		default: ::_LoadStringFromResource(IDS_NATION_UNKNOWN, szText); return false;
	}

	return true;
}

D3DCOLOR CGameBase::GetIDColorByLevelDifference(int iLevelDiff)
{
	// ���� ���̿� ���� ����...
	// ����� : �÷��̾�� +8, 
	// ������: �÷��̾�� +5, +6, +7
	// ����� : �÷��̾��� +2, +3, +4
	// ��� : -1 ? �÷��̾�  ? 1
	// �Ķ��� : �÷��̾�� 2���� ���� -2, -3, -4   
	// �ʷϻ� : �÷��̾�� -5, -6, -7
	// �ϴû� : �÷��̾�� -8, ��(����ġ�� ���� ����)
	
	D3DCOLOR crID = 0xffffffff;
	if(iLevelDiff >= 8)			crID = D3DCOLOR_ARGB(255, 255, 0, 255);
	else if(iLevelDiff >= 5)	crID = D3DCOLOR_ARGB(255, 255, 0, 0);
	else if(iLevelDiff >= 2)	crID = D3DCOLOR_ARGB(255, 255, 255, 0);
	else if(iLevelDiff >= -1)	crID = D3DCOLOR_ARGB(255, 255, 255, 255);
	else if(iLevelDiff >= -4)	crID = D3DCOLOR_ARGB(255, 0, 0, 255);
	else if(iLevelDiff >= -7)	crID = D3DCOLOR_ARGB(255, 0, 255, 0);
	else crID = D3DCOLOR_ARGB(255, 0, 255, 255);

	return crID;
}

// Item Data �� ������ �����̸��� �����..
e_ItemType CGameBase::MakeResrcFileNameForUPC(	__TABLE_ITEM_BASIC* pItem,		// ������ ������...
												std::string* pszResrcFN,			// Resource FileName
												std::string* pszIconFN,			// Icon FileName
												e_PartPosition& ePartPosition,	// Part �ϰ�� Index
												e_PlugPosition& ePlugPosition)	// Plug �ϰ�� Index
{	
	ePartPosition = PART_POS_UNKNOWN;
	ePlugPosition = PLUG_POS_UNKNOWN;
	if(pszResrcFN) *pszResrcFN = "";
	if(pszIconFN) *pszIconFN = "";

	if(NULL == pItem) return ITEM_TYPE_UNKNOWN;
	
	// �� 8 �ڸ��̴�.
	e_ItemType eType	= ITEM_TYPE_UNKNOWN;
	e_ItemPosition ePos	= (e_ItemPosition)pItem->byAttachPoint;	// ������ġ...

	int iPos = 0;
	std::string szExt; // Ȯ����..
	
	if(ePos >= ITEM_POS_DUAL && ePos <= ITEM_POS_TWOHANDLEFT)
	{
		if(ITEM_POS_DUAL == ePos || ITEM_POS_RIGHTHAND == ePos || ITEM_POS_TWOHANDRIGHT == ePos) ePlugPosition = PLUG_POS_RIGHTHAND;
		else if(ITEM_POS_LEFTHAND == ePos || ITEM_POS_TWOHANDLEFT == ePos) ePlugPosition = PLUG_POS_LEFTHAND;

		eType = ITEM_TYPE_PLUG;
		szExt = ".n3cplug";
	}
	else if(ePos >= ITEM_POS_UPPER && ePos <= ITEM_POS_SHOES)
	{
		if(ITEM_POS_UPPER == ePos)			ePartPosition = PART_POS_UPPER; 		// ��ü
		else if(ITEM_POS_LOWER == ePos)		ePartPosition = PART_POS_LOWER; 		// ��ü
		else if(ITEM_POS_HEAD == ePos)		ePartPosition = PART_POS_HAIR_HELMET; 	// ����
		else if(ITEM_POS_GLOVES == ePos)	ePartPosition = PART_POS_HANDS; 		// ��
		else if(ITEM_POS_SHOES == ePos)		ePartPosition = PART_POS_FEET;			// ��
		else { __ASSERT(0, "lll"); }
		
		eType = ITEM_TYPE_PART;
		szExt = ".n3cpart";
		iPos = ePartPosition + 1;
	}
	else if(ePos >= ITEM_POS_EAR && ePos <= ITEM_POS_INVENTORY)
	{
		eType = ITEM_TYPE_ICONONLY;
		szExt = ".dxt";
	}
	else if(ePos == ITEM_POS_GOLD)
	{
		eType = ITEM_TYPE_GOLD;
		szExt = ".dxt";
	}
	else if(ePos == ITEM_POS_SONGPYUN)
	{
		eType = ITEM_TYPE_SONGPYUN;
		szExt = ".dxt";
	}
	else
	{
		__ASSERT(0, "Invalid Item Position");
	}

	std::vector<char> buffer(256, NULL);
	if(pszResrcFN)
	{
		if(pItem->dwIDResrc) 
		{
			sprintf(&buffer[0],	"Item\\%.1d_%.4d_%.2d_%.1d%s",
				(pItem->dwIDResrc / 10000000), 
				(pItem->dwIDResrc / 1000) % 10000, 
				(pItem->dwIDResrc / 10) % 100, 
				pItem->dwIDResrc % 10,
				szExt.c_str());
			*pszResrcFN = &buffer[0];
		}
		else // �����ܸ� �ִ� �÷��׳� ��Ʈ �ϼ��� �ִ�...
		{
			*pszResrcFN = "";
		}
	}
	if(pszIconFN)
	{
//		sprintf(&buffer[0],	"UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt", eType, iIndex, eRace, iPos);
		sprintf(&buffer[0],	"UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt",
			(pItem->dwIDIcon / 10000000), 
			(pItem->dwIDIcon / 1000) % 10000, 
			(pItem->dwIDIcon / 10) % 100, 
			pItem->dwIDIcon % 10);
		*pszIconFN = &buffer[0];
	}
	
	return eType;
}


bool CGameBase::IsValidCharacter(CPlayerBase* pCharacter)
{
	if(NULL == pCharacter) return false;
	if(pCharacter == s_pPlayer) return true; // �÷��̾��̴�.
	return s_pOPMgr->IsValidCharacter(pCharacter); //  �ϴ� ����ִ� �ѵ��߿��� �����ͺ���.. 
}

CPlayerBase* CGameBase::CharacterGetByID(int iID, bool bFromAlive)
{
	if(iID < 0) return NULL;
	if(iID == s_pPlayer->IDNumber()) return s_pPlayer;
	return s_pOPMgr->CharacterGetByID(iID, bFromAlive);
}

