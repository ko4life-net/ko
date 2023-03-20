#pragma once

#include <string>
#include <dinput.h>
const int CURRENT_VERSION = 1097; // ���� ����

const float PACKET_INTERVAL_MOVE = 1.5f; // ���������� ������ ��Ŷ �ð� ����..
const float PACKET_INTERVAL_ROTATE = 4.0f;
const float PACKET_INTERVAL_REQUEST_TARGET_HP = 2.0f;

// ����Ű ������ ���� �κ�..
enum eKeyMap {
    KM_HOTKEY1 = DIK_1,
    KM_HOTKEY2 = DIK_2,
    KM_HOTKEY3 = DIK_3,
    KM_HOTKEY4 = DIK_4,
    KM_HOTKEY5 = DIK_5,
    KM_HOTKEY6 = DIK_6,
    KM_HOTKEY7 = DIK_7,
    KM_HOTKEY8 = DIK_8,
    KM_TOGGLE_RUN = DIK_T,
    KM_TOGGLE_MOVE_CONTINOUS = DIK_E,
    KM_TOGGLE_ATTACK = DIK_R,
    KM_TOGGLE_SITDOWN = DIK_C,
    KM_TOGGLE_INVENTORY = DIK_I,
    KM_TOGGLE_SKILL = DIK_K,
    KM_TOGGLE_STATE = DIK_U,
    KM_TOGGLE_MINIMAP = DIK_M,
    KM_TOGGLE_HELP = DIK_F1,
    KM_CAMERA_CHANGE = DIK_F9,
    KM_DROPPED_ITEM_OPEN = DIK_F,
    KM_MOVE_FOWARD = DIK_W,
    KM_MOVE_BACKWARD = DIK_S,
    KM_ROTATE_LEFT = DIK_A,
    KM_ROTATE_RIGHT = DIK_D,
    KM_TARGET_NEARST_ENEMY = DIK_Z,
    KM_TARGET_NEARST_PARTY = DIK_X,
    KM_TARGET_NEARST_FRIEND = DIK_V,
    KM_TARGET_PARTY1 = DIK_F1,
    KM_TARGET_PARTY2 = DIK_F2,
    KM_TARGET_PARTY3 = DIK_F3,
    KM_TARGET_PARTY4 = DIK_F4,
    KM_TARGET_PARTY5 = DIK_F5,
    KM_TARGET_PARTY6 = DIK_F6,
    KM_TARGET_PARTY7 = DIK_F7,
    KM_TARGET_PARTY8 = DIK_F8
};

enum e_PlayerType {
    PLAYER_BASE = 0,
    PLAYER_NPC = 1,
    PLAYER_OTHER = 2,
    PLAYER_MYSELF = 3
};

enum e_Race {
    RACE_ALL = 0,
    RACE_KA_ARKTUAREK = 1,
    RACE_KA_TUAREK = 2,
    RACE_KA_WRINKLETUAREK = 3,
    RACE_KA_PURITUAREK = 4,
    RACE_EL_BABARIAN = 11,
    RACE_EL_MAN = 12,
    RACE_EL_WOMEN = 13,
    //RACE_KA_NORMAL = 11, RACE_KA_WARRIOR = 12, RACE_KA_ROGUE = 13, RACE_KA_MAGE = 14,
    RACE_NPC = 100,
    RACE_UNKNOWN = 0xffffffff
};

enum e_Class {
    CLASS_KINDOF_WARRIOR = 1,
    CLASS_KINDOF_ROGUE,
    CLASS_KINDOF_WIZARD,
    CLASS_KINDOF_PRIEST,
    CLASS_KINDOF_ATTACK_WARRIOR,
    CLASS_KINDOF_DEFEND_WARRIOR,
    CLASS_KINDOF_ARCHER,
    CLASS_KINDOF_ASSASSIN,
    CLASS_KINDOF_ATTACK_WIZARD,
    CLASS_KINDOF_PET_WIZARD,
    CLASS_KINDOF_HEAL_PRIEST,
    CLASS_KINDOF_CURSE_PRIEST,

    CLASS_KA_WARRIOR = 101,
    CLASS_KA_ROGUE,
    CLASS_KA_WIZARD,
    CLASS_KA_PRIEST, // ������� �⺻ ����
    CLASS_KA_BERSERKER = 105,
    CLASS_KA_GUARDIAN,
    CLASS_KA_HUNTER = 107,
    CLASS_KA_PENETRATOR,
    CLASS_KA_SORCERER = 109,
    CLASS_KA_NECROMANCER,
    CLASS_KA_SHAMAN = 111,
    CLASS_KA_DARKPRIEST,

    CLASS_EL_WARRIOR = 201,
    CLASS_EL_ROGUE,
    CLASS_EL_WIZARD,
    CLASS_EL_PRIEST, // ������� �⺻ ����
    CLASS_EL_BLADE = 205,
    CLASS_EL_PROTECTOR,
    CLASS_EL_RANGER = 207,
    CLASS_EL_ASSASIN,
    CLASS_EL_MAGE = 209,
    CLASS_EL_ENCHANTER,
    CLASS_EL_CLERIC = 211,
    CLASS_EL_DRUID,

    CLASS_UNKNOWN = 0xffffffff
};

enum e_Class_Represent {
    CLASS_REPRESENT_WARRIOR = 0,
    CLASS_REPRESENT_ROGUE,
    CLASS_REPRESENT_WIZARD,
    CLASS_REPRESENT_PRIEST,
    CLASS_REPRESENT_GENERIC = 100,
    CLASS_REPRESENT_UNKNOWN = 0xffffffff
};

const float WEAPON_WEIGHT_STAND_SWORD = 5.0f; // ������ ���� ����...Į
const float WEAPON_WEIGHT_STAND_AXE = 5.0f;   // ������ ���� ����...����
const float WEAPON_WEIGHT_STAND_BLUNT = 8.0f; // ������ ���� ����...������ ����

enum e_Ani {
    ANI_BREATH = 0,
    ANI_WALK,
    ANI_RUN,
    ANI_WALK_BACKWARD,
    ANI_STRUCK0,
    ANI_STRUCK1,
    ANI_STRUCK2,
    ANI_GUARD,
    ANI_DEAD_NEATLY = 8,
    ANI_DEAD_KNOCKDOWN,
    ANI_DEAD_ROLL,
    ANI_SITDOWN,
    ANI_SITDOWN_BREATH,
    ANI_STANDUP,
    ANI_ATTACK_WITH_WEAPON_WHEN_MOVE = 14,
    ANI_ATTACK_WITH_NAKED_WHEN_MOVE,

    ANI_SPELLMAGIC0_A = 16,
    ANI_SPELLMAGIC0_B,
    ANI_SPELLMAGIC1_A = 18,
    ANI_SPELLMAGIC1_B,
    ANI_SPELLMAGIC2_A = 20,
    ANI_SPELLMAGIC2_B,
    ANI_SPELLMAGIC3_A = 22,
    ANI_SPELLMAGIC3_B,
    ANI_SPELLMAGIC4_A = 24,
    ANI_SPELLMAGIC4_B,

    ANI_SHOOT_ARROW_A = 26,
    ANI_SHOOT_ARROW_B,
    ANI_SHOOT_QUARREL_A = 28,
    ANI_SHOOT_QUARREL_B,
    ANI_SHOOT_JAVELIN_A = 30,
    ANI_SHOOT_JAVELIN_B,

    ANI_SWORD_BREATH_A = 32,
    ANI_SWORD_ATTACK_A0,
    ANI_SWORD_ATTACK_A1,
    ANI_SWORD_BREATH_B,
    ANI_SWORD_ATTACK_B0,
    ANI_SWORD_ATTACK_B1, // �Ѽհ�

    ANI_DAGGER_BREATH_A = 38,
    ANI_DAGGER_ATTACK_A0,
    ANI_DAGGER_ATTACK_A1,
    ANI_DAGGER_BREATH_B,
    ANI_DAGGER_ATTACK_B0,
    ANI_DAGGER_ATTACK_B1, // �ܵ�

    ANI_DUAL_BREATH_A = 44,
    ANI_DUAL_ATTACK_A0,
    ANI_DUAL_ATTACK_A1,
    ANI_DUAL_BREATH_B,
    ANI_DUAL_ATTACK_B0,
    ANI_DUAL_ATTACK_B1, // �̵���

    ANI_SWORD2H_BREATH_A = 50,
    ANI_SWORD2H_ATTACK_A0,
    ANI_SWORD2H_ATTACK_A1,
    ANI_SWORD2H_BREATH_B,
    ANI_SWORD2H_ATTACK_B0,
    ANI_SWORD2H_ATTACK_B1, // ��հ�

    ANI_BLUNT_BREATH_A = 56,
    ANI_BLUNT_ATTACK_A0,
    ANI_BLUNT_ATTACK_A1,
    ANI_BLUNT_BREATH_B,
    ANI_BLUNT_ATTACK_B0,
    ANI_BLUNT_ATTACK_B1, // �б� - ������ ?

    ANI_BLUNT2H_BREATH_A = 62,
    ANI_BLUNT2H_ATTACK_A0,
    ANI_BLUNT2H_ATTACK_A1,
    ANI_BLUNT2H_BREATH_B,
    ANI_BLUNT2H_ATTACK_B0,
    ANI_BLUNT2H_ATTACK_B1, // ������� ��� �б�. - ��� ������ ����.

    ANI_AXE_BREATH_A = 68,
    ANI_AXE_ATTACK_A0,
    ANI_AXE_ATTACK_A1,
    ANI_AXE_BREATH_B,
    ANI_AXE_ATTACK_B0,
    ANI_AXE_ATTACK_B1, // �Ѽ� ����

    ANI_SPEAR_BREATH_A = 74,
    ANI_SPEAR_ATTACK_A0,
    ANI_SPEAR_ATTACK_A1,
    ANI_SPEAR_BREATH_B,
    ANI_SPEAR_ATTACK_B0,
    ANI_SPEAR_ATTACK_B1, // â - ���� ���� ���� �׳� â�̴�.

    ANI_POLEARM_BREATH_A = 80,
    ANI_POLEARM_ATTACK_A0,
    ANI_POLEARM_ATTACK_A1,
    ANI_POLEARM_BREATH_B,
    ANI_POLEARM_ATTACK_B0,
    ANI_POLEARM_ATTACK_B1, // ������� ��� ���ִ� â - û�浵 ??

    ANI_NAKED_BREATH_A = 86,
    ANI_NAKED_ATTACK_A0,
    ANI_NAKED_ATTACK_A1,
    ANI_NAKED_BREATH_B,
    ANI_NAKED_ATTACK_B0,
    ANI_NAKED_ATTACK_B1, // �Ǹ����� ??

    ANI_BOW_BREATH = 92,
    ANI_CROSS_BOW_BREATH,
    ANI_LAUNCHER_BREATH,
    ANI_BOW_BREATH_B,
    ANI_BOW_ATTACK_B0,
    ANI_BOW_ATTACK_B1, // Ȱ ����

    ANI_SHIELD_BREATH_A = 98,
    ANI_SHIELD_ATTACK_A0,
    ANI_SHIELD_ATTACK_A1,
    ANI_SHIELD_BREATH_B,
    ANI_SHIELD_ATTACK_B0,
    ANI_SHIELD_ATTACK_B1, // ���� ����

    ANI_GREETING0 = 104,
    ANI_GREETING1,
    ANI_GREETING2,
    ANI_WAR_CRY0 = 107,
    ANI_WAR_CRY1,
    ANI_WAR_CRY2,
    ANI_WAR_CRY3,
    ANI_WAR_CRY4,

    ANI_SKILL_AXE0 = 112,
    ANI_SKILL_AXE1,
    ANI_SKILL_AXE2,
    ANI_SKILL_AXE3,
    ANI_SKILL_DAGGER0 = 116,
    ANI_SKILL_DAGGER1,
    ANI_SKILL_DUAL0 = 118,
    ANI_SKILL_DUAL1,
    ANI_SKILL_BLUNT0 = 120,
    ANI_SKILL_BLUNT1,
    ANI_SKILL_BLUNT2,
    ANI_SKILL_BLUNT3,
    ANI_SKILL_POLEARM0 = 124,
    ANI_SKILL_POLEARM1,
    ANI_SKILL_SPEAR0 = 126,
    ANI_SKILL_SPEAR1,
    ANI_SKILL_SWORD0 = 128,
    ANI_SKILL_SWORD1,
    ANI_SKILL_SWORD2,
    ANI_SKILL_SWORD3,
    ANI_SKILL_AXE2H0 = 132,
    ANI_SKILL_AXE2H1,
    ANI_SKILL_SWORD2H0 = 134,
    ANI_SKILL_SWORD2H1,

    // ������ʹ� NPC Animation
    ANI_NPC_BREATH = 0,
    ANI_NPC_WALK,
    ANI_NPC_RUN,
    ANI_NPC_WALK_BACKWARD,
    ANI_NPC_ATTACK0 = 4,
    ANI_NPC_ATTACK1,
    ANI_NPC_STRUCK0,
    ANI_NPC_STRUCK1,
    ANI_NPC_STRUCK2,
    ANI_NPC_GUARD,
    ANI_NPC_DEAD0 = 10,
    ANI_NPC_DEAD1,
    ANI_NPC_TALK0,
    ANI_NPC_TALK1,
    ANI_NPC_TALK2,
    ANI_NPC_TALK3,
    ANI_NPC_SPELLMAGIC0 = 16,
    ANI_NPC_SPELLMAGIC1,

    ANI_UNKNOWN = 0xffffffff
};

//MAX_INCLINE_CLIMB = sqrt( 1 - sin(90-�ִ��簢)^2 )
//const    float MAX_INCLINE_CLIMB = 0.5f;       // �ö󰥼� �ִ� �ִ� ��簪 = 30 ��
const float MAX_INCLINE_CLIMB = 0.6430f; // �ö󰥼� �ִ� �ִ� ��簪 = 40 ��
//const    float MAX_INCLINE_CLIMB = 0.7071f; // �ö󰥼� �ִ� �ִ� ��簪 = 45 ��
//const    float MAX_INCLINE_CLIMB = 0.7660f; // �ö󰥼� �ִ� �ִ� ��簪 = 50 ��
//const    float MAX_INCLINE_CLIMB = 0.8660f; // �ö󰥼� �ִ� �ִ� ��簪 = 60 ��

enum e_MoveDirection {
    MD_STOP,
    MD_FOWARD,
    MD_BACKWARD,
    MD_UNKNOWN = 0xffffffff
};

const float MOVE_DELTA_WHEN_RUNNING = 3.0f; // �۶� �������� ����..
const float MOVE_SPEED_WHEN_WALK = 1.5f;    // Player ���� ������ ǥ�ؼӵ�

// ���� ����...
enum e_StateMove {
    PSM_STOP = 0,
    PSM_WALK,
    PSM_RUN,
    PSM_WALK_BACKWARD,
    PSM_COUNT
};

enum e_StateAction {
    PSA_BASIC = 0,  // �ƹ����� ���ϰ� ����...
    PSA_ATTACK,     // ���� ����..
    PSA_GUARD,      // ���� - ����..
    PSA_STRUCK,     // �������.
    PSA_DYING,      // �װ� �ִ���(����������)
    PSA_DEATH,      // �׾ ����..
    PSA_SPELLMAGIC, // ���� �ֹ� �ܿ�� ��..
    PSA_SITDOWN,    // �ɾ� �ִ���...
    PSA_COUNT
};

enum e_StateDying {
    PSD_DISJOINT = 0,  // ���صǾ� �״´�..
    PSD_KNOCK_DOWN,    // ���ư��ų� �ڷ� �и��鼭 �״´�.
    PSD_KEEP_POSITION, // �� �ڸ����� ����� �״´�..
    PSD_COUNT,

    PSD_UNKNOWN = 0xffffffff
};

enum e_StateParty {
    PSP_NORMAL = 0,
    PSP_POISONING = 1,
    PSP_CURSED = 2,
    PSP_MAGIC_TAKEN = 4,
    PSP_BLESSED = 8,
    PSP_UNKNOWN = 0xffffffff
};

enum e_PartPosition {
    PART_POS_UPPER = 0,
    PART_POS_LOWER,
    PART_POS_FACE,
    PART_POS_HANDS,
    PART_POS_FEET,
    PART_POS_HAIR_HELMET,
    PART_POS_COUNT,
    PART_POS_UNKNOWN = 0xffffffff
};

enum e_PlugPosition {
    PLUG_POS_RIGHTHAND = 0,
    PLUG_POS_LEFTHAND,
    PLUG_POS_BACK,
    PLUG_POS_KNIGHTS_GRADE,
    PLUG_POS_COUNT,
    PLUG_POS_UNKNOWN = 0xffffffff
};

/*
enum e_ItemClass    {    ITEM_CLASS_DAGGER = 1, // �ܰ�(dagger)
                        ITEM_CLASS_SWORD, // 2 : �Ѽհ�(onehandsword)
                        ITEM_CLASS_SWORD_2H, // 3 : ��հ�(twohandsword)
                        ITEM_CLASS_AXE, // 4 : �Ѽյ���(onehandaxe)
                        ITEM_CLASS_AXE_2H, // 5 : �μյ���(twohandaxe)
                        ITEM_CLASS_MACE, // 6 : �Ѽ�Ÿ�ݹ���(mace)
                        ITEM_CLASS_MACE_2H, // 7 : �μ�Ÿ�ݹ���(twohandmace)
                        ITEM_CLASS_SPEAR, // 8 : â(spear)
                        ITEM_CLASS_POLEARM, // 9 : ����(polearm)
                        
                        ITEM_CLASS_SHIELD_SMALL = 11, // 11 : ��������(smallshield)
                        ITEM_CLASS_SHIELD_LARGE, // 12 : ���������(largeshield)
                        ITEM_CLASS_SHIELD_KITE, // 13 : ī��Ʈ����(kiteshield)
                        ITEM_CLASS_SHIELD_LARGETKITE, // 14 : �����ī��Ʈ(largekite)
                        ITEM_CLASS_SHIELD_PLATE, // 15 : �÷���Ʈ����(plateshield)
                        
                        ITEM_CLASS_BOW_SHORT = 21, // 21 : ��Ʈ����(Shortbow)
                        ITEM_CLASS_BOW_WRAPT, // 22 : ��Ʈ����(wraptbow)
                        ITEM_CLASS_BOW_COMPOSITE, // 23 : ������Ʈ����(compositebow)
                        ITEM_CLASS_BOW_IRON, // 24 : ���̾𺸿�(ironbow)
                        ITEM_CLASS_BOW_LONG, // 25 : �պ���(longbow)
                        ITEM_CLASS_BOW_CROSS, // 28 : ũ�ν�����(crossbow)
                        
                        ITEM_CLASS_STAFF = 31, // 31 : ������(staff)
                        ITEM_CLASS_ETC, // 32 : ��Ÿ ���� ��ǰ
                        
                        ITEM_CLASS_ARMOR_COTTON = 41, // 41 : õ��(cotton)
                        ITEM_CLASS_ARMOR_FUR, // 42 : �а���(Fur)
                        ITEM_CLASS_ARMOR_LEATHER, // 43 : ���׹�(leather)
                        ITEM_CLASS_ARMOR_HADLEATHER, // 44 : �ϵ巹����(hardleather)
                        ITEM_CLASS_ARMOR_RINGMAIL, // 45 : ����(ringmail)
                        ITEM_CLASS_ARMOR_SCALEMAIL, // 46 : ��ù�(scaledmail)
                        ITEM_CLASS_ARMOR_HALFPLATE, // 47 : ���� �÷���Ʈ ��
                        ITEM_CLASS_ARMOR_FULLPLATE, // 48 : ö�ǹ�(platemail)
                        ITEM_CLASS_ROBE, // 49 : ������κ�(robe)
                        
                        ITEM_CLASS_ARROW = 101,
                        
                        ITEM_CLASS_UNKNOWN = 0xffffffff }; // 101: ȭ��(arrow) 
*/
enum e_ItemAttrib {
    ITEM_ATTRIB_GENERAL = 0,
    ITEM_ATTRIB_MAGIC = 1,
    ITEM_ATTRIB_LAIR = 2,
    ITEM_ATTRIB_CRAFT = 3,
    ITEM_ATTRIB_UNIQUE = 4,
    ITEM_ATTRIB_UPGRADE = 5,
    ITEM_ATTRIB_UNKNOWN = 0xffffffff
};

enum e_ItemClass {
    ITEM_CLASS_DAGGER = 11,   // �ܰ�(dagger)
    ITEM_CLASS_SWORD = 21,    // �Ѽհ�(onehandsword)
    ITEM_CLASS_SWORD_2H = 22, // 3 : ��հ�(twohandsword)
    ITEM_CLASS_AXE = 31,      // �Ѽյ���(onehandaxe)
    ITEM_CLASS_AXE_2H = 32,   // �μյ���(twohandaxe)
    ITEM_CLASS_MACE = 41,     // �Ѽ�Ÿ�ݹ���(mace)
    ITEM_CLASS_MACE_2H = 42,  // �μ�Ÿ�ݹ���(twohandmace)
    ITEM_CLASS_SPEAR = 51,    // â(spear)
    ITEM_CLASS_POLEARM = 52,  // ����(polearm)

    ITEM_CLASS_SHIELD = 60, // ����(shield)

    ITEM_CLASS_BOW = 70,       //  ��Ʈ����(Shortbow)
    ITEM_CLASS_BOW_CROSS = 71, // ũ�ν�����(crossbow)
    ITEM_CLASS_BOW_LONG = 80,  // �պ���(longbow)

    ITEM_CLASS_EARRING = 91, // �Ͱ���
    ITEM_CLASS_AMULET = 92,  // �����
    ITEM_CLASS_RING = 93,    // ����
    ITEM_CLASS_BELT = 94,    // �㸮��
    ITEM_CLASS_CHARM = 95,   //�κ��丮�� ���ϰ� �ִ� ������
    ITEM_CLASS_JEWEL = 96,   //��������
    ITEM_CLASS_POTION = 97,  // ����
    ITEM_CLASS_SCROLL = 98,  // ��ũ��

    ITEM_CLASS_LAUNCHER = 100, // â������ ���� ������..

    ITEM_CLASS_STAFF = 110,   // ������(staff)
    ITEM_CLASS_ARROW = 120,   // ȭ��(Arrow)
    ITEM_CLASS_JAVELIN = 130, // ��â

    ITEM_CLASS_ARMOR_WARRIOR = 210, // ���� ��
    ITEM_CLASS_ARMOR_ROGUE = 220,   // �α� ��
    ITEM_CLASS_ARMOR_MAGE = 230,    // ������ ��
    ITEM_CLASS_ARMOR_PRIEST = 240,  // ���� ��

    ITEM_CLASS_ETC = 251, // ��Ÿ

    ITEM_CLASS_UNKNOWN = 0xffffffff
}; //

enum e_Nation {
    NATION_NOTSELECTED = 0,
    NATION_KARUS,
    NATION_ELMORAD,
    NATION_UNKNOWN = 0xffffffff
};

struct __TABLE_ITEM_BASIC;
struct __TABLE_ITEM_EXT;
struct __TABLE_PLAYER;

enum e_Authority {
    AUTHORITY_MANAGER = 0,
    AUTHORITY_USER,
    AUTHORITY_NOCHAT,
    AUTHORITY_NPC,
    AUTHORITY_BLOCK_USER = 0xff
};

struct __InfoPlayerBase {
    int         iID;     // ���� ID
    std::string szID;    // �̸�
    D3DCOLOR    crID;    // �̸� ����..
    e_Race      eRace;   // ĳ���� ��ݿ� ���� ����
    e_Nation    eNation; // �Ҽ� ����..
    e_Class     eClass;  // ����
    int         iLevel;  // ����
    int         iHPMax;
    int         iHP;
    int         iAuthority; // ���� - 0 ������, 1 - �Ϲ�����, 255 - ������ ����...

    bool bRenderID; // ȭ�鿡 ID �� �����..

    void Init() {
        iID = 0;                  // ���� ID
        szID = "";                // �̸�
        crID = 0;                 // �̸� ����..
        eRace = RACE_UNKNOWN;     // ĳ���� ��ݿ� ���� ����
        eNation = NATION_UNKNOWN; // �Ҽ� ����..
        eClass = CLASS_UNKNOWN;   // ����
        iLevel = 0;               // ����
        iHPMax = 0;
        iHP = 0;
        iAuthority = 1; // ���� - 0 ������, 1 - �Ϲ�����, 255 - ������ ����...
        bRenderID = true;
    }
};

struct __InfoPlayerOther {
    int iFace; // �� ���..
    int iHair; // �Ӹ�ī�� ���..

    int         iCity;         // �Ҽӵ���
    int         iKnightsID;    // �Ҽ� ����(���) ID
    std::string szKnights;     // �Ҽ� ����(���) �̸�
    int         iKnightsGrade; // �Ҽ� ����(���) ���
    int         iKnightsRank;  // �Ҽ� ����(���) ����

    int iRank;  // ���� - ����, ���� - ���ѿ� ���� �з�
    int iTitle; // ���� - ����, ���� - �ܼ��� �ź� -> �����...

    void Init() {
        iFace = 0;         // �� ���..
        iHair = 0;         // �Ӹ�ī�� ���..
        iCity;             // �Ҽӵ���
        iKnightsID = 0;    // �Ҽ� ����(���)
        szKnights = "";    // �Ҽ� ����(���)
        iKnightsGrade = 0; // �Ҽ� ����(���) ����
        iKnightsRank = 0;  // ���� - ����, ���� - ���ѿ� ���� �з�
        iTitle = 0;        // ���� - ����, ���� - �ܼ��� �ź� -> �����...
    }
};

// ���� ����..
enum e_KnightsDuty {
    KNIGHTS_DUTY_UNKNOWN = 0,   // ????? �Ѱܳ�??
    KNIGHTS_DUTY_CHIEF = 1,     // ����
    KNIGHTS_DUTY_VICECHIEF = 2, // �δ���
    KNIGHTS_DUTY_PUNISH = 3,    // ¡����.
    KNIGHTS_DUTY_TRAINEE = 4,   // �߽����
    KNIGHTS_DUTY_KNIGHT = 5,    // �Ϲݱ��
    KNIGHTS_DUTY_OFFICER = 6    // �屳
};

#define VICTORY_ABSENCE 0
#define VICTORY_KARUS   1
#define VICTORY_ELMORAD 2

struct __InfoPlayerMySelf : public __InfoPlayerOther {
    int iBonusPointRemain; // ���� ���ʽ� ����Ʈ...
    int iLevelPrev;        // ������ ����...

    int iMSPMax;
    int iMSP;

    int           iTargetHPPercent;
    int64_t       iGold;
    int           iExpNext;
    int           iExp;
    int           iRealmPoint;         // ���� �⿩��
    e_KnightsDuty eKnightsDuty;        // ���� ����
    int           iWeightMax;          // �������� �ѹ���
    int           iWeight;             // ��� �ִ� ������ ����..
    int           iStrength;           // ��
    int           iStrength_Delta;     // �������̳� ������ ���� �����Ȱ�.
    int           iStamina;            // ���׹̳�
    int           iStamina_Delta;      // �������̳� ������ ���� �����Ȱ�.
    int           iDexterity;          // ��÷��..
    int           iDexterity_Delta;    // �������̳� ������ ���� �����Ȱ�.
    int           iIntelligence;       // ����
    int           iIntelligence_Delta; // �������̳� ������ ���� �����Ȱ�.
    int           iMagicAttak;         // ����
    int           iMagicAttak_Delta;   // �������̳� ������ ���� �����Ȱ�.

    int iAttack;       // ���ݷ�
    int iAttack_Delta; // ������ ���� ������ ��..
    int iGuard;        // ����
    int iGuard_Delta;  // ������ ���� ������ ��..

    int iRegistFire;         // ���׷�
    int iRegistFire_Delta;   // ������ ���� ���׷� ��ȭ��..
    int iRegistCold;         // ���׷�
    int iRegistCold_Delta;   // ������ ���� ���׷� ��ȭ��..
    int iRegistLight;        // ���׷�
    int iRegistLight_Delta;  // ������ ���� ���׷� ��ȭ��..
    int iRegistMagic;        // ���׷�
    int iRegistMagic_Delta;  // ������ ���� ���׷� ��ȭ��..
    int iRegistCurse;        // ���׷�
    int iRegistCurse_Delta;  // ������ ���� ���׷� ��ȭ��..
    int iRegistPoison;       // ���׷�
    int iRegistPoison_Delta; // ������ ���� ���׷� ��ȭ��..

    int iZoneInit;      // �������� ó���� ���� ����ȣ
    int iZoneCur;       // ���� ��..
    int iVictoryNation; // 0: ���º� 1:������ �¸� 2:ī�罺 �¸�

    void Init() {
        __InfoPlayerOther::Init();

        iBonusPointRemain = 0; // ���� ���ʽ� ����Ʈ...
        iLevelPrev = 0;        // ������ ����...

        iMSPMax = 0;
        iMSP = 0;

        iTargetHPPercent = 0;
        iGold = 0;
        iExpNext = 0;
        iExp = 0;
        iRealmPoint = 0;                     // ���� �⿩��
        eKnightsDuty = KNIGHTS_DUTY_UNKNOWN; // ���� ����
        iWeightMax = 0;                      // �������� �ѹ���
        iWeight = 0;                         // ��� �ִ� ������ ����..
        iStrength = 0;                       // ��
        iStrength_Delta = 0;                 // �������̳� ������ ���� �����Ȱ�.
        iStamina = 0;                        // ���׹̳�
        iStamina_Delta = 0;                  // �������̳� ������ ���� �����Ȱ�.
        iDexterity = 0;                      // ��÷��..
        iDexterity_Delta = 0;                // �������̳� ������ ���� �����Ȱ�.
        iIntelligence = 0;                   // ����
        iIntelligence_Delta = 0;             // �������̳� ������ ���� �����Ȱ�.
        iMagicAttak = 0;                     // ����
        iMagicAttak_Delta = 0;               // �������̳� ������ ���� �����Ȱ�.

        iAttack = 0;       // ���ݷ�
        iAttack_Delta = 0; // ������ ���� ������ ��..
        iGuard = 0;        // ����
        iGuard_Delta = 0;  // ������ ���� ������ ��..

        iRegistFire = 0;         // ���׷�
        iRegistFire_Delta = 0;   // ������ ���� ���׷� ��ȭ��..
        iRegistCold = 0;         // ���׷�
        iRegistCold_Delta = 0;   // ������ ���� ���׷� ��ȭ��..
        iRegistLight = 0;        // ���׷�
        iRegistLight_Delta = 0;  // ������ ���� ���׷� ��ȭ��..
        iRegistMagic = 0;        // ���׷�
        iRegistMagic_Delta = 0;  // ������ ���� ���׷� ��ȭ��..
        iRegistCurse = 0;        // ���׷�
        iRegistCurse_Delta = 0;  // ������ ���� ���׷� ��ȭ��..
        iRegistPoison = 0;       // ���׷�
        iRegistPoison_Delta = 0; // ������ ���� ���׷� ��ȭ��..

        iZoneInit = 0x01;    // �������� ó���� ���� ����ȣ
        iZoneCur = 0;        // ���� ��..
        iVictoryNation = -1; // ���￡�� �̱� ����
    }
};

const int MAX_PARTY_OR_FORCE = 8;

struct __InfoPartyOrForce {
    int         iID;             // ��Ƽ�� ID
    int         iLevel;          // Level
    e_Class     eClass;          // ����
    int         iHP;             // Hit Point
    int         iHPMax;          // Hit Point Max
    bool        bSufferDown_HP;  // Status - HP ������...
    bool        bSufferDown_Etc; // Status - ���ְ迭 ������ ������
    std::string szID;            // ��Ƽ �̸� ���ڿ�

    void Init() {
        iID = -1;
        iLevel = 0;
        eClass = CLASS_UNKNOWN;
        iHP = 0;
        iHPMax = 0;
        szID = "";

        bSufferDown_HP = false;  // Status - HP ������...
        bSufferDown_Etc = false; // Status - ���ְ迭 ������ ������
    };

    __InfoPartyOrForce() { this->Init(); }
};

enum e_PartyStatus {
    PARTY_STATUS_DOWN_HP = 1,
    PARTY_STATUS_DOWN_ETC = 2
};

struct __InfoPartyBBS // ��Ƽ ���� �Խ��� ����ü..
{
    std::string szID;   // ��Ƽ �̸� ���ڿ�
    int         iID;    // ��Ƽ�� ID
    int         iLevel; // Level
    e_Class     eClass; // ����
    int         iMemberCount;

    void Init() {
        szID = "";
        iID = -1;
        iLevel = 0;
        eClass = CLASS_UNKNOWN;
        iMemberCount = 0;
    };

    __InfoPartyBBS() { this->Init(); }
};

enum {
    RESRC_UI_LOGIN = 0,
    RESRC_UI_CHARACTERSELECT,
    RESRC_UI_CHARACTERCREATE,
    RESRC_UI_INVENTORY,
    RESRC_UI_CHAT,
    RESRC_UI_MYINFORMATION,
    RESRC_UI_TARGET,
    RESRC_UI_HOTKEY,
    RESRC_UI_FUNCTION
};

typedef struct __TABLE_ZONE {
    DWORD       dwID;        // zone ID
    std::string szTerrainFN; // Terrain FileName - Terrain, Tile Map, ColorMap....
    std::string szColorMapFN;
    std::string szLightMapFN;
    std::string szObjectPostDataFN; // Object ��ġ ���� - �̾ȿ� �浹 üũ �����͵� ��� �ִ�.
    std::string szMiniMapFN;
    std::string szSkySetting;         // �ϴð� ������ ������ ���� ���� ����..
    BOOL        bIndicateEnemyPlayer; // ���� �÷��̾ ǥ���ϳ�??
    int         iFixedSundDirection;  // ���� ������ ������Ű����..
    std::string szLightObjFN;         // ������ ��ġ�Ǿ� �ִ� ����Ʈ��������..

    std::string szGevFN;         // TODO: implement
    uint32_t    iAbyssSomething; // TODO: implement
    std::string szEnsFN;         // TODO: implement
    float       fFOV;            // TODO: implement
    std::string szFlagFN;        // TODO: implement
} TABLE_ZONE;

typedef struct __TABLE_UI_RESRC {
    DWORD dwID; // �÷��̾��� �Ҽ� ������ ���� Ű�� ����..

    std::string szLogIn;     // 0
    std::string szCmd;       // 1
    std::string szChat;      // 2
    std::string szMsgOutput; // 3
    std::string szStateBar;  // 4
    std::string szVarious;   // 5 - ĳ���� ����, ���� ����, ����Ʈ, ģ��.. ��� ���������� �Ѱ��� �������.
    std::string szState;     // 6 - ĳ���� ����
    std::string szKnights;   // 7 - ���� ����
    std::string szQuest;     // 8 - ����Ʈ
    std::string szFriends;   // 9 - ģ�� ����

    std::string szInventory;   // 10
    std::string szTransaction; // 11
    std::string szDroppedItem; // 12

    std::string szTargetBar;         // 13
    std::string szTargetSymbolShape; // 14
    std::string szSkillTree;         // 15
    std::string szHotKey;            // 16

    std::string szMiniMap;      // 17
    std::string szPartyOrForce; // 18
    std::string szPartyBBS;     // 19

    std::string szHelp;   // 20
    std::string szNotice; // 21

    std::string szCharacterCreate; // 22
    std::string szCharacterSelect; // 23

    std::string szToolTip;           // 24
    std::string szMessageBox;        // 25
    std::string szLoading;           // 26
    std::string szItemInfo;          // 27
    std::string szPersonalTrade;     // 28
    std::string szPersonalTradeEdit; // 29

    std::string szNpcEvent;         // 30
    std::string szZoneChangeOrWarp; // 31
    std::string szExchangeRepair;   // 32

    std::string szRepairTooltip;   // 33
    std::string szNpcTalk;         // 34
    std::string szNpcExchangeList; // 35

    std::string szKnightsOperation; // 36
    std::string szClassChange;      // 37

    std::string szEndingDisplay; // 38
    std::string szWareHouse;     // 39

    std::string szChangeClassInit; // 40
    std::string szChangeInitBill;  // 41

    std::string szInn;              // 42
    std::string szInputClanName;    // 43
    std::string szTradeBBS;         // 44
    std::string szTradeBBSSelector; // 45
    std::string szTradeExplanation; // 46
    std::string szTradeMemolist;    // 47
    std::string szQuestMenu;        // 48
    std::string szQuestTalk;        // 49
    std::string szQuestEdit;        // 50 TODO: implement
    std::string szDead;             // 51
    std::string szElLoading;        // 52
    std::string szKaLoading;        // 53
    std::string szNationSelect;     // 54

    std::string szChatSmall;                // 55 TODO: implement
    std::string szMsgOutputSmall;           // 56 TODO: implement
    std::string szItemUpgrade;              // 57 TODO: implement
    std::string szDuelCreate;               // 58 TODO: implement
    std::string szDuelList;                 // 59 TODO: implement
    std::string szDuelMessageBox;           // 60 TODO: implement
    std::string szDuelEditMsg;              // 61 TODO: implement
    std::string szDuelLobby;                // 62 TODO: implement
    std::string szQuestContent;             // 63 TODO: implement
    std::string szDuelItemCount;            // 64 TODO: implement
    std::string szTradeInventory;           // 65 TODO: implement
    std::string szTradeBuyInventory;        // 66 TODO: implement
    std::string szTradeItemDisplay;         // 67 TODO: implement
    std::string szTradePrice;               // 68 TODO: implement
    std::string szTradeCount;               // 69 TODO: implement
    std::string szTradeMessageBox;          // 70 TODO: implement
    std::string szKaPageKnights;            // 71 TODO: implement
    std::string szKaPageKnightsUnionMain;   // 72 TODO: implement
    std::string szKaPageKnightsUnionSub;    // 73 TODO: implement
    std::string szKaCmdList;                // 74 TODO: implement
    std::string szCmdEdit;                  // 75 TODO: implement
    std::string szKnightsCrest;             // 76 TODO: implement
    std::string szShoppingMall;             // 77 TODO: implement
    std::string szLevelGuide;               // 78
    std::string szWarfareNpc;               // 79 TODO: implement
    std::string szWarfarePetition;          // 80 TODO: implement
    std::string szCastleUnion;              // 81 TODO: implement
    std::string szWarfareSchedule;          // 82 TODO: implement
    std::string szExitMenu;                 // 83 TODO: implement
    std::string szResurrection;             // 84 TODO: implement
    std::string szIdChange;                 // 85 TODO: implement
    std::string szId;                       // 86 TODO: implement
    std::string szIdCheck;                  // 87 TODO: implement
    std::string szWarfareAdministrationNpc; // 88 TODO: implement
    std::string szWarfareTaxTariff;         // 89 TODO: implement
    std::string szKaWarfareMantleList;      // 90 TODO: implement
    std::string szKnightsMantleShop;        // 91 TODO: implement
    std::string szWarfareTaxCollection;     // 92 TODO: implement
    std::string szWarfareTaxRate;           // 93 TODO: implement
    std::string szWarfareTaxMsg;            // 94 TODO: implement
    std::string szKaCatapult;               // 95 TODO: implement
    std::string szDisguiseRing;             // 96 TODO: implement
    std::string szMsgBoxOk;                 // 97 TODO: implement
    std::string szMsgBoxOkCancel;           // 98
    std::string szWhisper_open;             // 99 TODO: implement
    std::string szWhisper_close;            // 100 TODO: implement
    std::string szKnights_crest_chr;        // 101 TODO: implement
    std::string szWarning;                  // 102 TODO: implement
    std::string szConversation;             // 103 TODO: implement
    std::string szBlog;                     // 104 TODO: implement
    std::string szInn_password;             // 105 TODO: implement
    std::string szRookieTip;                // 106
    std::string szWebPage;                  // 107 TODO: implement
    std::string szPartyBoardMessage;        // 108 TODO: implement
    std::string szSymbol;                   // 109 TODO: implement
    std::string szRentalNpc;                // 110 TODO: implement
    std::string szRentalTransaction;        // 111 TODO: implement
    std::string szRentalTry;                // 112 TODO: implement
    std::string szRentalItem;               // 113 TODO: implement
    std::string szRentalMessage;            // 114 TODO: implement
    std::string szRentalCount;              // 115 TODO: implement
} TABLE_UI_RESRC;

typedef struct __TABLE_ITEM_BASIC // ���� �����ۿ� ���� ���ҽ� ���ڵ�...
{
    DWORD       dwID;       // 00 �ڵ�ȭ�� �����۹�ȣ
    BYTE        byExtIndex; // 01 Ȯ�� �ε���
    std::string szName;     // 02 �̸�
    std::string szRemark;   // 03 ������ ����
    DWORD       dwTODO1;    // TODO: implement introduced in 1264
    BYTE        byTODO2;    // TODO: implement introduced in 1264
    DWORD       dwIDResrc;  // 04 �ڵ�ȭ�� ������ ���ҽ�    d
    DWORD       dwIDIcon;   // 05 �ڵ�ȭ�� ������ ������ ���ҽ�    d
    DWORD       dwSoundID0; // 06 Sound ID - 0 �̸� ���� ����~..
    DWORD       dwSoundID1; // 07 Sound ID - 0 �̸� ���� ����~..

    BYTE byClass;       // 08 ������ ����    b enum e_ItemClass ����....
    BYTE byIsRobeType;  // 09 ���Ʒ��� ��°�� �� �κ�Ÿ���� ������.....
    BYTE byAttachPoint; // 10 ���� ��ġ    b
    BYTE byNeedRace;    // 11 ����    b
    BYTE byNeedClass;   // 12 ����    b

    short siDamage;         // 13 ����Ÿ��    b
    short siAttackInterval; // 14 ���� �ð� 100 �� 1 ��
    short siAttackRange;    // 15 ��ȿ �Ÿ�    .1 meter ����
    short siWeight;         // 16 ���� .1 �߷�����
    short siMaxDurability;  // 17 ������
    int   iPrice;           // 18 �ż���
    int   iPriceSale;       // 19 �ŵ���
    short siDefense;        // 20 ����
    BYTE  byContable;       // 21 ���ڰ� �ִ� �������ΰ�??

    DWORD dwEffectID1; // 22 ���� ȿ�� ID1
    DWORD dwEffectID2; // 23 ���� ȿ�� ID1

    char cNeedLevel; // 24 �䱸 ����        �÷��̾��� iLeve.. - �������� �������� �ִ�..

    BYTE byTODO3; // TODO: implement introduced in 1264

    BYTE byNeedRank;        // 25 �䱸 ����        �÷��̾��� iRank..
    BYTE byNeedTitle;       // 26 �䱸 Ÿ��Ʋ     �÷��̾��� iTitle..
    BYTE byNeedStrength;    // 27 �䱸 ��         �÷��̾��� iStrength..
    BYTE byNeedStamina;     // 28 �䱸 ����     �÷��̾��� iStamina..
    BYTE byNeedDexterity;   // 29 �䱸 ��ø        �÷��̾��� iDexterity..
    BYTE byNeedInteli;      // 30 �䱸 ����        �÷��̾��� iIntelligence..
    BYTE byNeedMagicAttack; // 31 �䱸 ����        �÷��̾��� iMagicAttak..

    BYTE bySellGroup; // 32 ������ �Ĵµ��� ���� �׷�..
} TABLE_ITEM_BASIC;

const int MAX_ITEM_EXTENSION = 24; // Ȯ�� ������ ���̺� ����.
const int LIMIT_FX_DAMAGE = 64;
const int ITEM_UNIQUE = 4;
const int ITEM_LIMITED_EXHAUST = 17;

typedef struct __TABLE_ITEM_EXT // ���� �����ۿ� ���� ���ҽ� ���ڵ�...
{
    DWORD       dwID;          // 00 �ڵ�ȭ�� �����۹�ȣ
    std::string szHeader;      // 01 ���λ�
    DWORD       dwTODO1;       // TODO: implement introduced in 1264 BaseID
    std::string szRemark;      // 02 ������ ����
    DWORD       dwTODO2;       // TODO: implement introduced in 1097
    DWORD       dwTODO3;       // TODO: implement introduced in 1264 ResrcID
    DWORD       dwTODO4;       // TODO: implement introduced in 1264 IconID
    BYTE        byMagicOrRare; // 03 ���� Ȥ�� ���� ����������...

    short siDamage;                   // 04 ����Ÿ��
    short siAttackIntervalPercentage; // 05 ���� �ð� ����
    short siHitRate;                  // 06 Ÿ�ݷ�    - �ۼ�Ʈ
    short siEvationRate;              // 07 ȸ����    -   "

    short siMaxDurability; // 08 ������
    short siPriceMultiply; // 09 �ż��� ����
    short siDefense;       // 10 ����

    short siDefenseRateDagger; // 11 �ܰ˹��    - �ۼ�Ʈ
    short siDefenseRateSword;  // 12 �˹��    - �ۼ�Ʈ
    short siDefenseRateBlow;   // 13 Ÿ�ݹ��    - �ۼ�Ʈ
    short siDefenseRateAxe;    // 14 �������    - �ۼ�Ʈ
    short siDefenseRateSpear;  // 15 â���    - �ۼ�Ʈ
    short siDefenseRateArrow;  // 16 ȭ����    - �ۼ�Ʈ

    BYTE byDamageFire;   // 17 �߰������� - ��
    BYTE byDamageIce;    // 18 �߰������� - ����
    BYTE byDamageThuner; // 19 �߰������� - ����
    BYTE byDamagePoison; // 20 �߰������� - ��

    BYTE byStillHP;              // 21 HP ���
    BYTE byDamageMP;             // 22 MP Damage
    BYTE byStillMP;              // 23 MP ���
    BYTE byReturnPhysicalDamage; // 24 ����Ÿ�� �ݻ�

    BYTE bySoulBind; // 25�ҿ� ���ε� - �ϴ��Ͻ� �������� ������ ���� - �ۼ�Ʈ - ����� �Ⱦ���.

    short siBonusStr;        // 26 �� ���ʽ�
    short siBonusSta;        // 27 ü�� ���ʽ�
    short siBonusDex;        // 28 ��ø�� ���ʽ�
    short siBonusInt;        // 29 ���� ���ʽ�
    short siBonusMagicAttak; // 30 ���� ���ʽ�
    short siBonusHP;         // 31 HP ���ʽ�
    short siBonusMSP;        // 32 MSP ���ʽ�

    short siRegistFire;   // 33 �� ����
    short siRegistIce;    // 34 �ñ� ����
    short siRegistElec;   // 35 ���� ����
    short siRegistMagic;  // 36 ���� ����
    short siRegistPoison; // 37 �� ����
    short siRegistCurse;  // 38 ���� ����

    DWORD dwEffectID1; // 39 ���� ȿ�� ID1
    DWORD dwEffectID2; // 40 ���� ȿ�� ID2

    short siNeedLevel;       // 41 �䱸 ����        �÷��̾��� iLeve..
    short siNeedRank;        // 42 �䱸 ����        �÷��̾��� iRank..
    short siNeedTitle;       // 43 �䱸 Ÿ��Ʋ     �÷��̾��� iTitle..
    short siNeedStrength;    // 44 �䱸 ��         �÷��̾��� iStrength..
    short siNeedStamina;     // 45 �䱸 ����     �÷��̾��� iStamina..
    short siNeedDexterity;   // 46 �䱸 ��ø        �÷��̾��� iDexterity..
    short siNeedInteli;      // 47 �䱸 ����        �÷��̾��� iIntelligence..
    short siNeedMagicAttack; // 48 �䱸 ����        �÷��̾��� iMagicAttak..
} TABLE_ITEM_EXT;

const int MAX_NPC_SHOP_ITEM = 30;
typedef struct __TABLE_NPC_SHOP {
    DWORD       dwNPCID;
    std::string szName;
    DWORD       dwItems[MAX_NPC_SHOP_ITEM];
} TABLE_NPC_SHOP;

enum e_ItemType {
    ITEM_TYPE_PLUG = 1,
    ITEM_TYPE_PART,
    ITEM_TYPE_ICONONLY,
    ITEM_TYPE_GOLD = 9,
    ITEM_TYPE_SONGPYUN = 10,
    ITEM_TYPE_UNKNOWN = 0xffffffff
};

enum e_ItemPosition {
    ITEM_POS_DUAL = 0,
    ITEM_POS_RIGHTHAND,
    ITEM_POS_LEFTHAND,
    ITEM_POS_TWOHANDRIGHT,
    ITEM_POS_TWOHANDLEFT,
    ITEM_POS_UPPER = 5,
    ITEM_POS_LOWER,
    ITEM_POS_HEAD,
    ITEM_POS_GLOVES,
    ITEM_POS_SHOES,
    ITEM_POS_EAR = 10,
    ITEM_POS_NECK,
    ITEM_POS_FINGER,
    ITEM_POS_SHOULDER,
    ITEM_POS_BELT,
    ITEM_POS_INVENTORY = 15,
    ITEM_POS_GOLD = 16,
    ITEM_POS_SONGPYUN = 17,
    ITEM_POS_UNKNOWN = 0xffffffff
};

enum e_ItemSlot {
    ITEM_SLOT_EAR_RIGHT = 0,
    ITEM_SLOT_HEAD = 1,
    ITEM_SLOT_EAR_LEFT = 2,
    ITEM_SLOT_NECK = 3,
    ITEM_SLOT_UPPER = 4,
    ITEM_SLOT_SHOULDER = 5,
    ITEM_SLOT_HAND_RIGHT = 6,
    ITEM_SLOT_BELT = 7,
    ITEM_SLOT_HAND_LEFT = 8,
    ITEM_SLOT_RING_RIGHT = 9,
    ITEM_SLOT_LOWER = 10,
    ITEM_SLOT_RING_LEFT = 11,
    ITEM_SLOT_GLOVES = 12,
    ITEM_SLOT_SHOES = 13,
    ITEM_SLOT_COUNT = 14,
    ITEM_SLOT_UNKNOWN = 0xffffffff
};

typedef struct __TABLE_PLAYER_LOOKS // NPC, Mob ��� ���� ���ҽ� ���ڵ�...
{
    DWORD       dwID;         // NPC ���� ID
    std::string szName;       // ĳ���� �̸�
    std::string szJointFN;    // ���� ���� �̸�
    std::string szAniFN;      // ���ϸ��̼� ���� �̸�
    std::string szPartFNs[7]; // �� Character Part - ��ü, ��ü, �Ӹ�, ��, �ٸ�, �Ӹ�ī��, ����

    int iJointRH;    // ������ �� ������ȣ
    int iJointLH;    // �޼� �� ������ȣ
    int iJointLH2;   // �޼� �ȶ� ������ȣ
    int iJointCloak; // ���� ���� ���� ��ȣ

    int iSndID_Move;
    int iSndID_Attack0;
    int iSndID_Attack1;
    int iSndID_Struck0;
    int iSndID_Struck1;
    int iSndID_Dead0;
    int iSndID_Dead1;
    int iSndID_Breathe0;
    int iSndID_Breathe1;
    int iSndID_Reserved0;
    int iSndID_Reserved1;
} TABLE_PLAYER;

typedef struct __TABLE_EXCHANGE_QUEST {
    DWORD       dwID;              // ����Ʈ ��ȣ..    0
    DWORD       dwNpcNum;          // npc ��ȣ..    1
    std::string szDesc;            // ����..        2
    int         iCondition0;       // ���� 1..        3
    int         iCondition1;       // ���� 2..        4
    int         iCondition2;       // ���� 3..        5
    int         iCondition3;       // ���� 4..        6
    int         iNeedGold;         // �䱸 ���..    7
    BYTE        bNeedLevel;        // �䱸 ����    8
    BYTE        bNeedClass;        // �䱸 ����    9
    BYTE        bNeedRank;         // �䱸 ����    10
    BYTE        bNeedExtra1;       // �䱸 ����1    11
    BYTE        bNeedExtra2;       // �䱸 ����2    12
    BYTE        bCreatePercentage; // ���� Ȯ��    13
    int         iArkTuarek;        // ��ũ ���Ʒ�..    14
    int         iTuarek;           // ���Ʒ�..            15
    int         iRinkleTuarek;     // ��Ŭ ���Ʒ�..    16
    int         iBabarian;         // �ٹٸ���..        17
    int         iMan;              // ����..            18
    int         iWoman;            // ����..            19
} TABLE_EXCHANGE_QUEST;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������̺�...

typedef struct __TABLE_UPC_SKILL {
    DWORD       dwID;         // SKILL ���� ID
    std::string szEngName;    // ��ų ���� �̸�
    std::string szName;       // ��ų �ѱ� �̸�
    std::string szDesc;       // ��ų �̸�
    int         iSelfAnimID1; // ������ ���۽���
    int         iSelfAnimID2; // ������ ���۳�

    int idwTargetAnimID; // Ÿ�� ����
    int iSelfFX1;        // ������ ȿ�� 1
    int iSelfPart1;      // ������ ȿ�� �Ͼ�� ��ġ 1
    int iSelfFX2;        // ������ ȿ�� 2
    int iSelfPart2;      // ������ ȿ�� �Ͼ�� ��ġ 2
    int iFlyingFX;       // ���� ȿ��
    int iTargetFX;       // Ÿ�� ȿ��

    int iTargetPart; // ȿ���� �Ͼ ��ġ.
    int iTarget;     // Ÿ��
    int iNeedLevel;  // �䱸 ����
    int iNeedSkill;  // �䱸 ��ų
    int iExhaustMSP; // �Ҹ� MSP

    int   iExhaustHP; // �Ҹ� HP
    DWORD dwNeedItem; // �䱸 ������ e_ItemClass ����.. 10�� �������̴�..
    DWORD dwExhaustItem;
    int   iCastTime;   // ĳ���� �ð�
    int   iReCastTime; // �ٽ� ĳ�����Ҷ����� �ɸ��� �ð�.

    float fUnkown1;        // TODO: implement
    float fUnkown2;        // TODO: implement 1264
    int   iPercentSuccess; // ������
    DWORD dw1stTableType;  // ù��° Ÿ��.
    DWORD dw2ndTableType;  // �ι�° Ÿ��.
    int   iValidDist;      // ��ȿ�Ÿ�
    int   iEtc;            // TODO: implement 1264

} TABLE_UPC_ATTACK_B;

typedef struct __TABLE_UPC_SKILL_TYPE_1 {
    DWORD dwID;          // SKILL ���� ID
    int   iSuccessType;  // ����Ÿ��.
    int   iSuccessRatio; // ������
    int   iPower;        // ���ݷ�
    int   iDelay;        // ������
    int   iComboType;    // �޺�Ÿ��
    int   iNumCombo;     // �޺�ȸ��
    int   iComboDamage;  // �޺������
    int   iValidAngle;   // ���ݹݰ�
    int   iAct[3];
} TABLE_UPC_SKILL_TYPE_1;

typedef struct __TABLE_UPC_SKILL_TYPE_2 {
    DWORD dwID;         // SKILL ���� ID
    int   iSuccessType; // ����Ÿ��.
    int   iPower;       // ���ݷ�
    int   iAddDist;     // �Ÿ�����
    int   iNumArrow;    // ȭ��䱸��
} TABLE_UPC_SKILL_TYPE_2;

typedef struct __TABLE_UPC_SKILL_TYPE_3 {
    DWORD dwID; // SKILL ���� ID
    int   iDDType;
    int   iStartDamage;
    int   iDuraDamage;
    int   iDurationTime; // ���ӽð�
    int   iAttribute;
} TABLE_UPC_SKILL_TYPE_3;

typedef struct __TABLE_UPC_SKILL_TYPE_4 {
    DWORD dwID;      // �Ϸù�ȣ
    int   iBuffType; // ����Ÿ��
    int   iDuration;
    int   iAttackSpeed;     // ���ݼӵ�
    int   iMoveSpeed;       // �̵��ӵ�
    int   iAC;              // ����
    int   iAttack;          // ���ݷ�
    int   iMaxHP;           // MAXHP
    int   iMaxHPPct;        // TODO: implement
    int   iMaxMP;           // TODO: implement
    int   iStr;             // ��
    int   iSta;             // ü��
    int   iDex;             // ��ø
    int   iInt;             // ����
    int   iMAP;             // ����
    int   iFireResist;      // ������
    int   iColdResist;      // �ñ�����
    int   iLightningResist; // ��������
    int   iMagicResist;     // ��������
    int   iDeseaseResist;   // ��������
    int   iPoisonResist;    // ������
} TABLE_UPC_SKILL_TYPE_4;

typedef struct __TABLE_UPC_SKILL_TYPE_5 {
    DWORD dwID;          // �Ϸù�ȣ
    DWORD dwTarget;      // Ÿ��
    int   iSuccessRatio; // ������
    int   iValidDist;    // ��ȿ�Ÿ�
    int   iRadius;       // �ݰ�
    float fCastTime;     // ĳ����Ÿ��
    float fRecastTime;   // ��ĳ����Ÿ��
    int   iDurationTime; // ���ӽð�
    DWORD dwExhaustItem; // �Ҹ������
    DWORD dwFX;          // ����ȿ��
} TABLE_UPC_SKILL_TYPE_5;

typedef struct __TABLE_UPC_SKILL_TYPE_6 {
    DWORD dwID;          // �Ϸù�ȣ
    DWORD dwTarget;      // Ÿ��
    int   iSuccessRatio; // ������
    int   iValidDist;    // ��ȿ�Ÿ�
    int   iRadius;       // �ݰ�
    float fCastTime;     // ĳ����Ÿ��
    float fRecastTime;   // ��ĳ����Ÿ��
    int   iDurationTime; // ���ӽð�
    DWORD dwExhaustItem; // �Ҹ������
    DWORD dwFX;          // ����ȿ��
    DWORD dwTranform;    // ����
} TABLE_UPC_SKILL_TYPE_6;

typedef struct __TABLE_UPC_SKILL_TYPE_7 {
    DWORD dwID;          // �Ϸù�ȣ
    DWORD dwTarget;      // Ÿ��
    DWORD dwValidGroup;  // ��ȿ�׷�
    int   iSuccessRatio; // ������
    int   iValidDist;    // ��ȿ�Ÿ�
    int   iRadius;       // �ݰ�
    float fCastTime;     // ĳ����Ÿ��
    float fRecastTime;   // ��ĳ����Ÿ��
    int   iDurationTime; // ���ӽð�
    DWORD dwExhaustItem; // �Ҹ������
    DWORD dwFX;          // ����ȿ��
} TABLE_UPC_SKILL_TYPE_7;

typedef struct __TABLE_UPC_SKILL_TYPE_8 {
    DWORD dwID;       // �Ϸù�ȣ
    DWORD dwTarget;   // Ÿ��
    int   iRadius;    // �ݰ�
    DWORD dwWarpType; // �����̵�Ÿ��
    float fRefillEXP; // ����ġȸ��
    DWORD dwZone1;    // ���ѹ�1
    DWORD dwZone2;    // ���ѹ�2
    DWORD dwZone3;    // ���ѹ�3
    DWORD dwZone4;    // ���ѹ�4
    DWORD dwZone5;    // ���ѹ�5
} TABLE_UPC_SKILL_TYPE_8;

typedef struct __TABLE_UPC_SKILL_TYPE_9 {
    DWORD dwID;          // �Ϸù�ȣ
    DWORD dwTarget;      // Ÿ��
    int   iSuccessRatio; // ������
    int   iValidDist;    // ��ȿ�Ÿ�
    int   iRadius;       // �ݰ�
    float fCastTime;     // ĳ����Ÿ��
    float fRecastTime;   // ��ĳ����Ÿ��
    int   iDurationTime; // ���ӽð�
    DWORD dwExhaustItem; // �Ҹ������
    DWORD dwAttr;        // �Ӽ�
    int   iDamage;       // �����
} TABLE_UPC_SKILL_TYPE_9;

typedef struct __TABLE_UPC_SKILL_TYPE_10 {
    DWORD dwID;          // �Ϸù�ȣ
    DWORD dwTarget;      // Ÿ��
    int   iSuccessRatio; // ������
    int   iValidDist;    // ��ȿ�Ÿ�
    int   iRadius;       // �ݰ�
    float fCastTime;     // ĳ����Ÿ��
    float fRecastTime;   // ��ĳ����Ÿ��
    DWORD dwExhaustItem; // �Ҹ������
    DWORD dwRemoveAttr;  // ���żӼ�
} TABLE_UPC_SKILL_TYPE_10;

//�������̺�...
///////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __TABLE_QUEST_MENU {
    DWORD       dwID;   // �Ϸù�ȣ
    std::string szMenu; // ���� �޴�
} TABLE_QUEST_MENU;

typedef struct __TABLE_QUEST_TALK {
    DWORD       dwID;   // �Ϸù�ȣ
    std::string szTalk; // ����Ʈ ����
} TABLE_QUEST_TALK;

typedef struct __TABLE_HELP {
    int               iID;
    int               iLevelMin;
    int               iLevelMax;
    e_Class_Represent eRepresentClass;
    std::string       szTitle;
    std::string       szDescription;
} TABLE_HELP;

typedef struct __TABLE_TEXTS {
    DWORD       dwID;
    std::string szText;
} TABLE_TEXTS;

const int MAX_ITEM_SLOT_OPC =
    8; // ���� ������ - �ٸ� �÷��̾�(NPC ����) 0 ~ 4 ��ü,��ü,���,��,�� 5 ���� 6 ������ 7 �޼�
const int MAX_ITEM_INVENTORY = 28; // ���� ���� MAX (�κ��丮â)
const int MAX_ITEM_TRADE = 24;     // ���ΰ� �ŷ�..
const int MAX_ITEM_TRADE_PAGE = 12;
const int MAX_ITEM_WARE_PAGE = 8;
const int MAX_ITEM_PER_TRADE = 12; // ���ΰ� �ŷ�..
const int MAX_ITEM_BUNDLE_DROP_PIECE = 6;
const int MAX_ITEM_EX_RE_NPC = 4; // ��ȯ, ����â NPC ����..

const int MAX_SKILL_FROM_SERVER = 9; // �������Լ� �޴� ���� ���� ����..

const int MAX_SKILL_KIND_OF = 5;  // Base Skill 1��, ���� ��ų 4��..
const int MAX_SKILL_IN_PAGE = 6;  // �� ������ ���� ������ ����..
const int MAX_SKILL_PAGE_NUM = 3; // ��ų �Ѱ��� ������ �ִ� ��������..

const int MAX_SKILL_HOTKEY_PAGE = 8; // Hot Key�� �� ������ ��..
const int MAX_SKILL_IN_HOTKEY = 8;   // Hot Key�� �� ���������� ����..

const int MAX_AVAILABLE_CHARACTER = 3; // �� ������ �����Ҽ� �ִ� �ִ� ĳ���� ��..

// �ο.... By ecli666
const int ID_SOUND_ITEM_ETC_IN_INVENTORY = 2000;
const int ID_SOUND_ITEM_IN_REPAIR = 2001;
const int ID_SOUND_ITEM_WEAPON_IN_INVENTORY = 2002;
const int ID_SOUND_ITEM_ARMOR_IN_INVENTORY = 2003;
const int ID_SOUND_GOLD_IN_INVENTORY = 3000;
const int ID_SOUND_SKILL_THROW_ARROW = 5500;
const int ID_SOUND_BGM_LOGIN = 35;
const int ID_SOUND_BGM_TOWN = 20000;
const int ID_SOUND_BGM_KA_BATTLE = 20002;
const int ID_SOUND_BGM_EL_BATTLE = 20003;
const int ID_SOUND_CHR_SELECT_ROTATE = 2501;

const float SOUND_RANGE_TO_SET = 10.0f;
const float SOUND_RANGE_TO_RELEASE = 20.0f;

const float STUN_TIME = 3.0f;

enum e_Behavior {
    BEHAVIOR_NOTHING = 0,
    BEHAVIOR_EXIT,         // ���α׷� ������..
    BEHAVIOR_RESTART_GAME, // ���� �ٽ� �����ϱ�(ĳ���� ����)
    BEHAVIOR_REGENERATION, // ��Ȱ
    BEHAVIOR_CANCEL,       // ���.. ����� ������ �ŷ� ��û�� ������ ��Ҹ� ��������..

    BEHAVIOR_PARTY_PERMIT,  // ������ ��Ƽ���� ��û�� ���°��� ���.
    BEHAVIOR_PARTY_DISBAND, // ��Ƽ Ż��..
    BEHAVIOR_FORCE_PERMIT,  // ������ �δ밡�� ��û�� ���°��� ���.
    BEHAVIOR_FORCE_DISBAND, // �δ� Ż��..

    BEHAVIOR_REQUEST_BINDPOINT, // ���ε� ����Ʈ�� ...

    BEHAVIOR_DELETE_CHR,

    BEHAVIOR_KNIGHTS_CREATE,
    BEHAVIOR_KNIGHTS_DESTROY,  // ���� ��ü..
    BEHAVIOR_KNIGHTS_WITHDRAW, // ���� ��ü..

    BEHAVIOR_PERSONAL_TRADE_FMT_WAIT, // ���ΰŷ�.. ���� ��û�� ���..
    BEHAVIOR_PERSONAL_TRADE_PERMIT,   // ���ΰŷ�.. ���� ��û���� ���..

    BEHAVIOR_MGAME_LOGIN,

    BEHAVIOR_CLAN_JOIN,
    BEHAVIOR_PARTY_BBS_REGISTER,        // ��Ƽ �Խ��ǿ� ���
    BEHAVIOR_PARTY_BBS_REGISTER_CANCEL, // ��Ƽ �Խ��ǿ� ��� ����

    BEHAVIOR_EXECUTE_OPTION, // ���� �����ϰ� �ɼ� ����..

    BEHAVIOR_UNKNOWN = 0xffffffff
};

enum e_SkillMagicTaget {
    SKILLMAGIC_TARGET_SELF = 1,          // �� �ڽ�..
    SKILLMAGIC_TARGET_FRIEND_WITHME = 2, // ���� ������ �츮��(����) �� �ϳ� ..
    SKILLMAGIC_TARGET_FRIEND_ONLY = 3,   // ���� �� �츮�� �� �ϳ�
    SKILLMAGIC_TARGET_PARTY = 4,         // ���� ������ �츮��Ƽ �� �ϳ�..
    SKILLMAGIC_TARGET_NPC_ONLY = 5,      // NPC�� �ϳ�.
    SKILLMAGIC_TARGET_PARTY_ALL = 6,     // ���� ȣ���� �츮��Ƽ ���..
    SKILLMAGIC_TARGET_ENEMY_ONLY = 7,    // ������ ������ ��� ���� �ϳ�(NPC����)
    SKILLMAGIC_TARGET_ALL = 8,           // �׻� �����ϴ� ��� ���� �ϳ�.

    SKILLMAGIC_TARGET_AREA_ENEMY = 10,       // �� ������ �ִ� ����...
    SKILLMAGIC_TARGET_AREA_FRIEND = 11,      // �� ������ �ִ� ����.
    SKILLMAGIC_TARGET_AREA_ALL = 12,         // �� ������ �ִ� ��ε�.
    SKILLMAGIC_TARGET_AREA = 13,             // ���� �߽����� �� ����
    SKILLMAGIC_TARGET_DEAD_FRIEND_ONLY = 25, //���� ����� ���� �� �츮�� �� �ϳ�.

    SKILLMAGIC_TARGET_UNKNOWN = 0xffffffff
};

//define fx...
typedef struct __TABLE_FX // FX ���ҽ� ���ڵ�...
{
    DWORD       dwID;      // ���� ID
    std::string szName;    // TODO: implement
    std::string szFN;      // file name
    DWORD       dwSoundID; // ȿ���� ���� ���� �Ƶ�.
} TABLE_FX;

const int MAX_COMBO = 3;

const int FXID_BLOOD = 10002;
const int FXID_LEVELUP_KARUS = 10012;
const int FXID_LEVELUP_ELMORAD = 10018;
const int FXID_REGEN_ELMORAD = 10019;
const int FXID_REGEN_KARUS = 10020;
const int FXID_SWORD_FIRE_MAIN = 10021;
const int FXID_SWORD_FIRE_TAIL = 10022;
const int FXID_SWORD_FIRE_TARGET = 10031;
const int FXID_SWORD_ICE_MAIN = 10023;
const int FXID_SWORD_ICE_TAIL = 10024;
const int FXID_SWORD_ICE_TARGET = 10032;
const int FXID_SWORD_LIGHTNING_MAIN = 10025;
const int FXID_SWORD_LIGHTNING_TAIL = 10026;
const int FXID_SWORD_LIGHTNING_TARGET = 10033;
const int FXID_SWORD_POISON_MAIN = 10027;
const int FXID_SWORD_POISON_TAIL = 10028;
const int FXID_SWORD_POISON_TARGET = 10034;
//const int    FXID_GROUND_TARGET = 10035;
const int FXID_REGION_TARGET_EL_ROGUE = 10035;
const int FXID_REGION_TARGET_EL_WIZARD = 10036;
const int FXID_REGION_TARGET_EL_PRIEST = 10037;
const int FXID_REGION_TARGET_KA_ROGUE = 10038;
const int FXID_REGION_TARGET_KA_WIZARD = 10039;
const int FXID_REGION_TARGET_KA_PRIEST = 10040;
const int FXID_CLAN_RANK_1 = 10041;
const int FXID_REGION_POISON = 10100;

//define skillmagic_type4_bufftype
enum e_SkillMagicType4 {
    BUFFTYPE_MAXHP = 1,             //MaxHP��ȭ..
    BUFFTYPE_AC = 2,                //AC��ȭ..
    BUFFTYPE_RESIZE = 3,            //ĳ���� ũ�� ����..
    BUFFTYPE_ATTACK = 4,            //���ݷ�..
    BUFFTYPE_ATTACKSPEED = 5,       //���ݽ��ǵ�..
    BUFFTYPE_SPEED = 6,             //�̵��ӵ�..
    BUFFTYPE_ABILITY = 7,           //�ټ����� �ɷ�(str, sta, cha, dex int)
    BUFFTYPE_RESIST = 8,            //�ټ����� ���׷�..
    BUFFTYPE_HITRATE_AVOIDRATE = 9, //hitrate n avoidrate
    BUFFTYPE_TRANS = 10,            //����, ����..
    BUFFTYPE_SLEEP = 11,            //������..
    BUFFTYPE_EYE = 12               //�÷°���...
};

enum e_SkillMagicType3 {
    DDTYPE_TYPE3_DUR_OUR = 100,
    DDTYPE_TYPE3_DUR_ENEMY = 200
};

enum e_SkillMagicFailMsg {
    SKILLMAGIC_FAIL_CASTING = -100,    //ĳ���� ����..
    SKILLMAGIC_FAIL_KILLFLYING = -101, //Flying��ü �׿�..
    SKILLMAGIC_FAIL_ENDCOMBO = -102,   //combo ���� ������.
    SKILLMAGIC_FAIL_NOEFFECT = -103,   //Effecting ����...
    SKILLMAGIC_FAIL_ATTACKZERO = -104, //Ÿ��ġ 0...(mp�� �Ҹ��ϰ� Ÿ�ݽ���..)

    SKILLMAGIC_FAIL_UNKNOWN = 0xffffffff
};

enum e_ObjectType {
    OBJECT_TYPE_BINDPOINT,
    OBJECT_TYPE_DOOR_LEFTRIGHT,
    OBJECT_TYPE_DOOR_TOPDOWN,
    OBJECT_TYPE_LEVER_TOPDOWN,
    OBJECT_TYPE_FLAG,
    OBJECT_TYPE_WARP_POINT,
    OBJECT_TYPE_UNKNOWN = 0xffffffff
};

//definitions related clan....
const int   CLAN_LEVEL_LIMIT = 20;
const int   CLAN_COST = 500000;
const DWORD KNIGHTS_FONT_COLOR = 0xffff0000; // ����(Ŭ��)�̸� ��Ʈ �÷�..

enum e_Cursor {
    CURSOR_ATTACK,
    CURSOR_EL_NORMAL,
    CURSOR_EL_CLICK,
    CURSOR_KA_NORMAL,
    CURSOR_KA_CLICK,
    CURSOR_PRE_REPAIR,
    CURSOR_NOW_REPAIR,
    CURSOR_COUNT,
    CURSOR_UNKNOWN = 0xffffffff
};

// Seconds a player can exit or select character after attacking or being attacked
const int SECONDS_TO_EXIT_GAME_AFTER_ATTACK = 10;

enum e_GameExitType {
    EXIT_TYPE_NONE,
    EXIT_TYPE_SELECTCHAR,
    EXIT_TYPE_EXIT,
    EXIT_TYPE_OPTION,
};
