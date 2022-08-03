#pragma once

const int SOCKET_PORT_GAME = 15001; // ������ ���� ��Ʈ ��ȣ..
const int SOCKET_PORT_LOGIN = 15100; // ������ ���� ��Ʈ ��ȣ..

#define N3_ACCOUNT_LOGIN			0xf3
#define N3_ACCOUNT_LOGIN_MGAME		0xf4	// !!! LOGIN_REQ !!! �� ����
#define N3_GAMESERVER_GROUP_LIST	0xf5	// Recv - b1(ServerCount) loop(	s1(IP����), str1(IP���ڿ�), s1(�����̸�����), str1(�����̸����ڿ�), s1(���������ο�) )

#define	N3_GAME_SERVER_LOGIN	0x01	// Game server �� Log in..
#define	N3_NEW_CHARACTER		0x02	// ĳ���� ���� �����..
#define	N3_DELETE_CHARACTER		0x03	// ����� ���� ĳ���� �����..
#define	N3_CHARACTER_SELECT		0x04	// Game start..
#define	N3_NATION_SELECT		0x05	// ���� ���� - 0 �̸� ����..
#define	N3_MOVE					0x06	// Move..
#define	N3_ROTATE				0x09	// Rotate..
#define	N3_USER_INOUT			0x07	// User inout..
#define	N3_NPC_INOUT			0x0A	// NPC inout..
#define	N3_ATTACK				0x08	// Attack..
#define	N3_NPC_MOVE				0x0B	// NPC move..
#define	N3_ALL_CHARACTER_INFO_REQUEST	0x0C	// ��� ĳ���� ���� ��û�ϱ�..
#define	N3_GAMESTART			0x0D	// Game Start..
#define	N3_MYINFO				0x0E	// NPC moveedge..
#define	N3_CHAT					0x10	// Chat..
	enum e_ChatMode {	N3_CHAT_NORMAL = 1,
						N3_CHAT_PRIVATE,
						N3_CHAT_PARTY,
						N3_CHAT_FORCE,
						N3_CHAT_SHOUT,
						N3_CHAT_CLAN,
						N3_CHAT_PUBLIC,
						N3_CHAT_WAR,
						N3_CHAT_TITLE,						//�����ٿ� �� ����
						N3_CHAT_TITLE_DELETE,				//�����ٿ� �ִ� ���� ����
						N3_CHAT_CONTINUE,					//���� ����
						N3_CHAT_CONTINUE_DELETE,			//���� ���� ����
						N3_CHAT_UNKNOWN = 0xffffffff };

#define	N3_DEAD					0x11	// Player Dead..
#define	N3_REGENE				0x12	// Player Regene..
#define N3_TIME					0x13	// ���ӻ��� �ð�
#define N3_WEATHER				0x14	// ���ӻ��� ������ȭ
#define	N3_UPDATE_REGION_UPC	0x15	// Player Regene..
#define	N3_UPDATE_REGION_NPC	0x1C	// NPC Region Update
#define	N3_REQUEST_NPC_IN		0x1D	// NPC ���� ��û
#define N3_WARP					0x1E	// Warp !!!
#define	N3_ITEM_MOVE			0x1F	// Item Move..
#define N3_HP_CHANGE			0x17	// Player stats Update
#define N3_MSP_CHANGE			0x18	// Player stats Update
#define N3_EXP_CHANGE			0x1A	// Player stats Update
#define N3_LEVEL_CHANGE			0x1B	// Player stats Update
#define N3_REALM_POINT_CHANGE	0x2A	// Player stats Update // ���� �⿩��
#define N3_REQUEST_USER_IN		0x16	// Update Region ������ �ʿ��� User In ��û
#define N3_NPC_EVENT			0x20	// NPC Event ���� ��� ��ŷ�..
#define	N3_ITEM_TRADE_START		0x25	// ��ŷ� ��Ŷ.. ������ ��..
#define N3_TARGET_HP			0x22	// Target Character HP Percentage ��û
#define N3_ITEM_BUNDLE_DROP		0x23	// �����鼭 Item ���
#define N3_ITEM_BUNDLE_OPEN_REQUEST 0x24	// �����ۻ��ڸ� ���ų� ��ü�� ������.. ��û�� ������ �ޱ� ����..
#define	N3_ITEM_TRADE			0x21	// ��ŷ� ��Ŷ.. ������ ��..
	enum e_SubPacket_Trade		{	N3_SP_TRADE_BUY  = 0x01,
									N3_SP_TRADE_SELL = 0x02,
									N3_SP_TRADE_MOVE = 0x03	};

#define	N3_ITEM_DROPPED_GET		0x26	// ������ ȹ���� �����ۿ� ���� ��..
#define N3_ZONE_CHANGE			0x27	// ��ü����.. Recv - b1 ����ȣ f3 ��ǥ X, Z, Y | Recv
#define N3_POINT_CHANGE			0x28	// ���� ü���� -	 ������ b1(1�� 2ü�� 3��ø 4���� 5����) s(-1 +1)  // ������ b1(1�� 2ü�� 3��ø 4���� 5�ŷ�) s(�����ġ)
#define N3_STATE_CHANGE			0x29	// ���º�ȭ Send - b2(Type, State) | Recv S1(ID) b2(Type, State) - // Type 1 �ɱ⼭��, 2 ��Ƽ����...
	enum e_SubPacket_State {	N3_SP_STATE_CHANGE_SITDOWN = 0x01,
								N3_SP_STATE_CHANGE_RECRUIT_PARTY = 0x02,
								N3_SP_STATE_CHANGE_SIZE = 0x03,
								N3_SP_STATE_CHANGE_ACTION = 0x04,			// 1 - �λ�, 11 - ����
								N3_SP_STATE_CHANGE_VISIBLE = 0x05 };		// ���� 0 ~ 255

#define N3_VERSION_CHECK		0x2B	// ���� üũ... s1 - Version
//#define N3_CRYPTION_PUBLIC_KEY	0x2C	// ��ȣȭ 64 ��Ʈ ����Ű Recv 64i
#define N3_USER_LOOK_CHANGE		0x2D	// �÷��̾��� �����̳� ���Ⱑ �ٲ��..
#define N3_NOTICE				0x2E	// ��������..

#define N3_PARTY_OR_FORCE		0x2F	// Party Or Force Packet... Send - b1(Party:1, Force:2)
	// Sub Packet
	enum e_SubPacket_Party {	N3_SP_PARTY_OR_FORCE_CREATE			= 0x01,	// Send - s1(ID)	| Recv b1(YesNoErr)
							N3_SP_PARTY_OR_FORCE_PERMIT			= 0x02,	// Send - b1(YesNo) | Recv - s1(ID)
							N3_SP_PARTY_OR_FORCE_INSERT			= 0x03,	// Send - s1(ID) | Recv - s3(ID, HPMax, HP) b2(Level, Class) - ���ڿ��� ID �� �˾Ƴ���.. ID �� -1 �̸�.. ��Ƽ�� �����°��� ������ �����ѰŴ�..
							N3_SP_PARTY_OR_FORCE_REMOVE			= 0x04,	// Send - s1(ID) | Recv - s1(ID) - �ڱ� �ڽ��̸� ��Ƽ�� ���� �Ѵ�..
							N3_SP_PARTY_OR_FORCE_DESTROY			= 0x05,	// Send
							N3_SP_PARTY_OR_FORCE_HP_CHANGE		= 0x06,	// Recv - s3(ID, HPMax, HP)
							N3_SP_PARTY_OR_FORCE_LEVEL_CHANGE	= 0x07,	// Recv - s1(ID), b1(Level)
							N3_SP_PARTY_OR_FORCE_CLASS_CHANGE	= 0x08,	// Recv - s1(ID), b1(Class)�幰���� �����Ҷ�...
							N3_SP_PARTY_OR_FORCE_STATUS_CHANGE	= 0x09 };	// Recv - s1(ID), b1(Status)��, ����, ���Ӽ�����, �ູ

#define N3_PER_TRADE			0x30	// Personal Trade..
	// Sub Packet
	enum e_SubPacket_PerTrade {	N3_SP_PER_TRADE_REQ =			0x01, 
								N3_SP_PER_TRADE_AGREE =			0x02,
								N3_SP_PER_TRADE_ADD =			0x03,
								N3_SP_PER_TRADE_OTHER_ADD =		0x04,
								N3_SP_PER_TRADE_DECIDE	=		0x05,
								N3_SP_PER_TRADE_OTHER_DECIDE =	0x06,
								N3_SP_PER_TRADE_DONE =			0x07,
								N3_SP_PER_TRADE_CANCEL =		0x08 };	

//	����ȿ�� ��Ŷ ����(��� ����)
//	(BYTE)	N3_MAGIC
//	(BYTE)	N3_MAGIC_CASTING or flying or effecting or fail
//	(DWORD)	Magic ID
//	(short)	������ ID
//	(short) Ÿ��ID
//	(short) data1
//	(short) data2
//	(short) data3
//	(short) data4
//	(short) data5
//	(short) data6
#define N3_MAGIC				0x31	//����ȿ��..
	// Sub Packet
	enum e_SubPacket_Magic {	N3_SP_MAGIC_CASTING =	0x01,
								N3_SP_MAGIC_FLYING =	0x02,
								N3_SP_MAGIC_EFFECTING =	0x03,
								N3_SP_MAGIC_FAIL = 		0x04,
								N3_SP_MAGIC_TYPE4BUFFTYPE = 0x05,
								N3_SP_MAGIC_CANCEL = 0x06								
	};

#define N3_SKILL_CHANGE			0x32	// Skill Point Change..
#define N3_OBJECT_EVENT			0x33	// Send - s4(Object Event ID, Parameter, ������ NPC ID, ������ NPC Command) | Recv 

#define N3_CLASS_CHANGE			0x34	// Class Change..
	// Sub Packet..
	enum e_SubPacket_ClassChange_Main {
		N3_SP_CLASS_CHANGE_PURE = 0x01,		// Class Change Real..
		N3_SP_CLASS_CHANGE_REQ  = 0x02,		// Class Change Req..
		N3_SP_CLASS_ALL_POINT   = 0x03,		// Class Change All Point..
		N3_SP_CLASS_SKILL_POINT = 0x04,
		N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY = 0x05	};	// Class Change Skill Point..

	enum e_SubPacket_ClassChange {	N3_SP_CLASS_CHANGE_SUCCESS	= 0x01,		// Success..
									N3_SP_CLASS_CHANGE_NOT_YET	= 0x02,		// Not yet..
									N3_SP_CLASS_CHANGE_ALREADY = 0x03,		// Already..
									N3_SP_CLASS_CHANGE_ITEM_IN_SLOT = 0x04,		// Item in Slot..
									N3_SP_CLASS_CHANGE_FAILURE	= 0x00 };	// Failure..

#define N3_CHAT_SELECT_TARGET		0x35	// Send - s1(ID String Length) str1(ID String) | Recv - s1(ID String Length) str1(ID String) ���ڿ� ���̰� 0�̸� ����..
#define N3_CONCURRENT_USER_COUNT	0x36	// Send - ... | Recv - s1(���� ���� �����ڼ�)
#define N3_REQUEST_GAME_SAVE		0x37	// ������ ���� ��û.. �޴°� ����..

#define N3_DURABILITY_CHANGE		0x38	// ������ ����..

#define N3_TIME_NOTIFY				0x39	// ��Ŷ���� �ƹ��͵� �Ⱥ����� 2�� ������.. �̰� ������..

#define N3_ITEM_TRADE_REPAIR		0x3a	// ������ �Ÿ�, ��ȯ, ����..
#define N3_ITEM_REPAIR_REQUEST		0x3b	// ������ ���� ����..


#define N3_KNIGHTS					0x3C	// Knights Related Packet..
	enum e_SubPacket_Knights {	N3_SP_KNIGHTS_CREATE =				0x01, // ���� Send - s1(Name Length) str1 | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_JOIN =				0x02, // ���� Send - s1(Knights ID) | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_WITHDRAW =			0x03, // Ż�� Send - | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_MEMBER_REMOVE =		0x04, // ��� ���� - 
								N3_SP_KNIGHTS_DESTROY	=			0x05, // �ǰ��� Send - | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_MEMBER_JOIN_ADMIT =	0x06, // ��� ���� �㰡 Send - s1(Knights ID) | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_MEMBER_JOIN_REJECT =	0x07, // ��� ���� ���� Send - s1(Knights ID) | Recv - b1(1:���� 0:����)
								N3_SP_KNIGHTS_MEMBER_PUNISH =		0x08, // ��� ¡�� - ���԰� ����
								N3_SP_KNIGHTS_APPOINT_CHIEF =		0x09, // ���� �Ӹ� - ���԰� ����
								N3_SP_KNIGHTS_APPOINT_VICECHIEF =	0x0A, // �δ��� �Ӹ� - ���԰� ����
								N3_SP_KNIGHTS_APPOINT_OFFICER =		0x0B, // �屳�Ӹ� - ���԰� ����
								N3_SP_KNIGHTS_GRADE_CHANGE_ALL =	0x0C, // �� ������ ��� �� ���� ���� Recv - s1(count) Loop { s1(Knights ID) b2(Grade Rank) }
								N3_SP_KNIGHTS_MEMBER_INFO_ALL =		0x0D, // ��� ��� ��û Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
								N3_SP_KNIGHTS_MEMBER_INFO_ONLINE =	0x0E, // ���� ���� ����Ʈ Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
								N3_SP_KNIGHTS_STASH =				0x0F, // ���� â��
								N3_SP_KNIGHTS_DUTY_CHANGE =			0x10, // ����� ���� ����.. �ش� ������� ����.. Recv - s1(Knights ID) b1(����);
								N3_SP_KNIGHTS_JOIN_REQ =			0x11, // ���� �ε���
								N3_SP_KNIGHTS_UNKNOWN };

	enum e_SubPacket_KNights_Create {	N3_SP_KNIGHTS_CREATE_FAIL_DBFAIL =			0x00,
										N3_SP_KNIGHTS_CREATE_SUCCESS =				0x01,
										N3_SP_KNIGHTS_CREATE_FAIL_LOWLEVEL =		0x02,
										N3_SP_KNIGHTS_CREATE_FAIL_DUPLICATEDNAME =	0x03,
										N3_SP_KNIGHTS_CREATE_FAIL_LOWMONEY =		0x04,
										N3_SP_KNIGHTS_CREATE_FAIL_ALREADYJOINED =	0x05,
										N3_SP_KNIGHTS_CREATE_FAIL_UNKNOWN =			0x06,
										N3_SP_KNIGHTS_CREATE_FAIL_INVALIDDAY =		0x07,
										N3_SP_KNIGHTS_CREATE_FAIL_INVALIDSERVER =	0x08 };

	enum e_SubPacket_KNights_Common {	N3_SP_KNIGHTS_COMMON_DBFAIL =				0x00,	//DB�˻� ����..
										N3_SP_KNIGHTS_COMMON_SUCCESS =				0x01,	//����
										N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER =		0x02,	//���� ����..
										N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER =		0x03,	//��������� �׾� ����..
										N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER =		0x04,	//��������� ������ �ٸ�..
										N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER =	0x05,	//��������� �̹� �ٸ� Ŭ���̳� ���ܿ� ���ԵǾ� ����..
										N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT =	0x06,	//������ ����..
										N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN =		0x07,	//�������� �ʴ� ����..									
										N3_SP_KNIGHTS_COMMON_FAIL_FULL =			0x08,	//�ο��� Ǯ..
										N3_SP_KNIGHTS_COMMON_FAIL_ME =				0x09,	//�ڱ��ڽ��� ������ ���..
										N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED =		0x0A,	//�����̳� Ŭ���� ���ԵǾ� ���� ����...
										N3_SP_KNIGHTS_COMMON_FAIL_REJECT =			0x0B,	//������� ����...
										N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE =		0x0C	// ������������ �ź�..
								};	// join, appoint, leave, withdraw ��� ����.....
	
#define N3_ITEM_COUNT_CHANGE		0x3d
#define N3_KNIGHTS_LIST_BASIC		0x3e // ���� ���� ��û
	enum e_SubPacket_KnightsList {	N3_SP_KNIGHTS_LIST_BASIC_ALL = 0x01,  // Receive - s1(knights Count) { s21(id, �̸�����), str1(�̸�) }
									N3_SP_KNIGHTS_LIST_BASIC_INSERT = 0x02, // Receive - s2(id, �̸�����), str1(�̸�)
									N3_SP_KNIGHTS_LIST_BASIC_REMOVE = 0x03 }; // Receive - s1(id)
#define N3_ITEM_DESTROY				0x3f
#define N3_ADMINISTRATOR			0x40 // ������ ���� ��Ŷ..
	enum e_SubPacket_Administrator { N3_SP_ADMINISTRATOR_ARREST = 0x01, // �� ���� �ִ� ������ ��ü����.. Send b1(Type) s1(�����̸�����), str1(�����̸�)
									N3_SP_ADMINISTRATOR_FORBID_CONNECT = 0x02, // ���ӱ��� �� �ѾƳ���.. Send b1(Type) s1(�����̸�����), str1(�����̸�)
									N3_SP_ADMINISTRATOR_CHAT_FORBID = 0x03, // ä�ñ��� Send b1(Type) s1(�����̸�����), str1(�����̸�)
									N3_SP_ADMINISTRATOR_CHAT_PERMIT = 0x04 }; // ä���㰡 Send b1(Type) s1(�����̸�����), str1(�����̸�)
									
#define N3_CHECK_SPEEDHACK			0x41 // ���ǵ��� üũ�� ��Ŷ..

#define N3_COMPRESSED_PACKET		0x42 // ����� ��Ŷ,.. Ǯ� �ѹ��� �Ľ��ؾ� �Ѵ�..
#define N3_CONTINOUS_PACKET			0x44 // �پ ���� ��Ŷ.. ������ ���鼭 �Ľ��ؾ� �Ѵ�..

#define N3_WAREHOUSE				0X45 // ������ ���� ��Ŷ..
	enum e_SubPacket_WareHouse {	N3_SP_WARE_OPEN = 				0x01,	// Open..
									N3_SP_WARE_GET_IN =				0x02,	// �̱�..
									N3_SP_WARE_GET_OUT =			0X03,	// ����..
									N3_SP_WARE_WARE_MOVE =			0x04,	// �̵�..
									N3_SP_WARE_INV_MOVE =			0x05,	// �̵�..
									N3_SP_WARE_INN =				0x10,	// ��ó��..������������ ����..
									N3_SP_WARE_UNKNOWN };

#define N3_SERVER_CHANGE			0x46
#define N3_REPORT_DEBUG_STRING		0x47
#define N3_HOME						0x48	// ������ ����.. �������� ������ �޴´�.
#define N3_FRIEND_INFO				0x49	// ģ�� ����.. Send s1(����ī��Ʈ) s1(�̸�����), str1(�����̸�) | Receive s1(����ī��Ʈ) ��Loop { s1(�̸�����), str1(�����̸�), b1(����, ��Ƽ or ����) }
#define N3_NOAH_CHANGE	0x4a	// ��� Change..
	enum e_SubPacket_NoahChange {	N3_SP_NOAH_GET = 0x01,	// ��� ����,
									N3_SP_NOAH_LOST	= 0x02,	// ��� �н�..
									N3_SP_NOAH_SPEND = 0x03	// ��� �Һ�..
								};		
#define	N3_WARP_LIST					0x4b	// ���� ����Ʈ Recv -
//#define	N3_SERVER_CHECK					0x4c	// ���� �ӽ� ���� üũ... Recv - s1, str1(IP), s1(dwPort) | Send s1(Zone ID), s3(x*10,z*10,y*10)
//#define	N3_SERVER_CONCURRENT_CONNECT	0x4d	// ���������ڼ�...  Send - b1(����) | Recv - s1(�����ڼ�)
#define N3_CORPSE_CHAR					0x4e	// ĳ���Ͱ� �ٽ� regen�� �Ͽ� ��ü�� �����.
#define N3_PARTY_BBS					0x4f	// ��Ƽ �Խ��ǿ� ����ϱ�
	enum e_Party_BBS {	N3_SP_PARTY_REGISTER						= 0x01,		// ��Ƽ �Խ��ǿ� ���
						N3_SP_PARTY_REGISTER_CANCEL					= 0x02,		// ��Ƽ �Խ��ǿ��� ��� ���
						N3_SP_PARTY_BBS_DATA						= 0x03};	// ��Ƽ �Խ� ���� �䱸

#define N3_TRADE_BBS					0x50	// ��ŷ� �Խ���
	enum e_Trade_BBS_Kind { N3_SP_TRADE_BBS_BUY						= 0x01,		// ��� ���� ���
							N3_SP_TRADE_BBS_SELL					= 0x02};		// �Ĵ� ���� ���

	enum e_Trade_BBS_Sub  { N3_SP_TYPE_REGISTER						= 0x01,		// ���� ����ϱ�
							N3_SP_TYPE_REGISTER_CANCEL				= 0x02,		// ��� �����ϱ�
							N3_SP_TYPE_BBS_DATA						= 0x03,		// �Խ��� ���� �䱸
							N3_SP_TYPE_BBS_OPEN						= 0x04,		// ��ŷ� �Խ��� ����
							N3_SP_TYPE_BBS_TRADE					= 0X05};	// �Խ��ǿ��� �ŷ� ��û�ϱ�
#define N3_KICK_OUT						0x51	// Recv s1, str1(IP) s1(port) | Send s1, str1(ID)
#define N3_NPC_QUEST					0X52	// npc ����Ʈ
#define N3_ALIVE_CHECK					0x53	// �ܼ� ����
#define N3_ITEM_WEIGHT_CHANGE			0x54
#define N3_QUEST_SELECT					0x55	// ����Ʈ �޴� ����
#define N3_QUEST_TALK					0x56	// ����Ʈ ��ȭ









#define N3_TEMP_TEST 0xff // �ӽ� �׽�Ʈ..


#ifdef _DEBUG
#include <map>
static std::map<int, const char *> PacketToString = {
	{N3_ACCOUNT_LOGIN, "WIZ_ACCOUNT_LOGIN"},
	{N3_ACCOUNT_LOGIN_MGAME, "WIZ_ACCOUNT_LOGIN_MGAME"},
	{N3_GAMESERVER_GROUP_LIST, "WIZ_GAMESERVER_GROUP_LIST"},
	{N3_GAME_SERVER_LOGIN, "WIZ_GAME_SERVER_LOGIN"},
	{N3_NEW_CHARACTER, "WIZ_NEW_CHARACTER"},
	{N3_DELETE_CHARACTER, "WIZ_DELETE_CHARACTER"},
	{N3_CHARACTER_SELECT, "WIZ_CHARACTER_SELECT"},
	{N3_NATION_SELECT, "WIZ_NATION_SELECT"},
	{N3_MOVE, "WIZ_MOVE"},
	{N3_ROTATE, "WIZ_ROTATE"},
	{N3_USER_INOUT, "WIZ_USER_INOUT"},
	{N3_NPC_INOUT, "WIZ_NPC_INOUT"},
	{N3_ATTACK, "WIZ_ATTACK"},
	{N3_NPC_MOVE, "WIZ_NPC_MOVE"},
	{N3_ALL_CHARACTER_INFO_REQUEST, "WIZ_ALL_CHARACTER_INFO_REQUEST"},
	{N3_GAMESTART, "WIZ_GAMESTART"},
	{N3_MYINFO, "WIZ_MYINFO"},
	{N3_CHAT, "WIZ_CHAT"},
	{N3_DEAD, "WIZ_DEAD"},
	{N3_REGENE, "WIZ_REGENE"},
	{N3_TIME, "WIZ_TIME"},
	{N3_WEATHER, "WIZ_WEATHER"},
	{N3_UPDATE_REGION_UPC, "WIZ_UPDATE_REGION_UPC"},
	{N3_UPDATE_REGION_NPC, "WIZ_UPDATE_REGION_NPC"},
	{N3_REQUEST_NPC_IN, "WIZ_REQUEST_NPC_IN"},
	{N3_WARP, "WIZ_WARP"},
	{N3_ITEM_MOVE, "WIZ_ITEM_MOVE"},
	{N3_HP_CHANGE, "WIZ_HP_CHANGE"},
	{N3_MSP_CHANGE, "WIZ_MSP_CHANGE"},
	{N3_EXP_CHANGE, "WIZ_EXP_CHANGE"},
	{N3_LEVEL_CHANGE, "WIZ_LEVEL_CHANGE"},
	{N3_REALM_POINT_CHANGE, "WIZ_REALM_POINT_CHANGE"},
	{N3_REQUEST_USER_IN, "WIZ_REQUEST_USER_IN"},
	{N3_NPC_EVENT, "WIZ_NPC_EVENT"},
	{N3_ITEM_TRADE_START, "WIZ_ITEM_TRADE_START"},
	{N3_TARGET_HP, "WIZ_TARGET_HP"},
	{N3_ITEM_BUNDLE_DROP, "WIZ_ITEM_BUNDLE_DROP"},
	{N3_ITEM_BUNDLE_OPEN_REQUEST, "WIZ_ITEM_BUNDLE_OPEN_REQUEST"},
	{N3_ITEM_TRADE, "WIZ_ITEM_TRADE"},
	{N3_ITEM_DROPPED_GET, "WIZ_ITEM_DROPPED_GET"},
	{N3_ZONE_CHANGE, "WIZ_ZONE_CHANGE"},
	{N3_POINT_CHANGE, "WIZ_POINT_CHANGE"},
	{N3_STATE_CHANGE, "WIZ_STATE_CHANGE"},
	{N3_VERSION_CHECK, "WIZ_VERSION_CHECK"},
	// {N3_CRYPTION_PUBLIC_KEY, "WIZ_CRYPTION_PUBLIC_KEY"},
	{N3_USER_LOOK_CHANGE, "WIZ_USER_LOOK_CHANGE"},
	{N3_NOTICE, "WIZ_NOTICE"},
	{N3_PARTY_OR_FORCE, "WIZ_PARTY_OR_FORCE"},
	{N3_PER_TRADE, "WIZ_PER_TRADE"},
	{N3_MAGIC, "WIZ_MAGIC"},
	{N3_SKILL_CHANGE, "WIZ_SKILL_CHANGE"},
	{N3_OBJECT_EVENT, "WIZ_OBJECT_EVENT"},
	{N3_CLASS_CHANGE, "WIZ_CLASS_CHANGE"},
	{N3_CHAT_SELECT_TARGET, "WIZ_CHAT_SELECT_TARGET"},
	{N3_CONCURRENT_USER_COUNT, "WIZ_CONCURRENT_USER_COUNT"},
	{N3_REQUEST_GAME_SAVE, "WIZ_REQUEST_GAME_SAVE"},
	{N3_DURABILITY_CHANGE, "WIZ_DURABILITY_CHANGE"},
	{N3_TIME_NOTIFY, "WIZ_TIME_NOTIFY"},
	{N3_ITEM_TRADE_REPAIR, "WIZ_ITEM_TRADE_REPAIR"},
	{N3_ITEM_REPAIR_REQUEST, "WIZ_ITEM_REPAIR_REQUEST"},
	{N3_KNIGHTS, "WIZ_KNIGHTS"},
	{N3_ITEM_COUNT_CHANGE, "WIZ_ITEM_COUNT_CHANGE"},
	{N3_KNIGHTS_LIST_BASIC, "WIZ_KNIGHTS_LIST_BASIC"},
	{N3_ITEM_DESTROY, "WIZ_ITEM_DESTROY"},
	{N3_ADMINISTRATOR, "WIZ_ADMINISTRATOR"},
	{N3_CHECK_SPEEDHACK, "WIZ_CHECK_SPEEDHACK"},
	{N3_COMPRESSED_PACKET, "WIZ_COMPRESSED_PACKET"},
	{N3_CONTINOUS_PACKET, "WIZ_CONTINOUS_PACKET"},
	{N3_WAREHOUSE, "WIZ_WAREHOUSE"},
	{N3_SERVER_CHANGE, "WIZ_SERVER_CHANGE"},
	{N3_REPORT_DEBUG_STRING, "WIZ_REPORT_DEBUG_STRING"},
	{N3_HOME, "WIZ_HOME"},
	{N3_FRIEND_INFO, "WIZ_FRIEND_INFO"},
	{N3_NOAH_CHANGE, "WIZ_NOAH_CHANGE"},
	{N3_WARP_LIST, "WIZ_WARP_LIST"},
	// {N3_SERVER_CHECK, "WIZ_SERVER_CHECK"},
	// {N3_SERVER_CONCURRENT_CONNECT, "WIZ_SERVER_CONCURRENT_CONNECT"},
	{N3_CORPSE_CHAR, "WIZ_CORPSE_CHAR"},
	{N3_PARTY_BBS, "WIZ_PARTY_BBS"},
	{N3_TRADE_BBS, "WIZ_TRADE_BBS"},
	{N3_KICK_OUT, "WIZ_KICK_OUT"},
	{N3_NPC_QUEST, "WIZ_NPC_QUEST"},
	{N3_ALIVE_CHECK, "WIZ_ALIVE_CHECK"},
	{N3_ITEM_WEIGHT_CHANGE, "WIZ_ITEM_WEIGHT_CHANGE"},
	{N3_QUEST_SELECT, "WIZ_QUEST_SELECT"},
	{N3_QUEST_TALK, "WIZ_QUEST_TALK"},
	{N3_TEMP_TEST, "WIZ_TEMP_TEST"}
};
#endif