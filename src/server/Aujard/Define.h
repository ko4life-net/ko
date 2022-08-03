#pragma once

#define MAX_USER			3000
#define MAX_ID_SIZE			30
#define MAX_PW_SIZE			12
#define MAX_ITEM			28

////////////////////////////////////////////////////////////
// Socket Define
////////////////////////////////////////////////////////////
#define SOCKET_BUFF_SIZE	(1024*8)
#define MAX_PACKET_SIZE		(1024*2)

#define PACKET_START1				0XAA
#define PACKET_START2				0X55
#define PACKET_END1					0X55
#define PACKET_END2					0XAA
//#define PROTOCOL_VER				0X01

// status
#define STATE_CONNECTED			0X01
#define STATE_DISCONNECTED		0X02
#define STATE_GAMESTART			0x03

/////////////////////////////////////////////////////
// ITEM_SLOT DEFINE
#define RIGHTEAR			0
#define HEAD				1
#define LEFTEAR				2
#define NECK				3
#define BREAST				4
#define SHOULDER			5
#define RIGHTHAND			6
#define WAIST				7
#define LEFTHAND			8
#define RIGHTRING			9
#define LEG					10
#define LEFTRING			11
#define GLOVE				12
#define FOOT				13
/////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

typedef union{
	short int	i;
	BYTE		b[2];
} MYSHORT;

typedef union{
	int			i;
	BYTE		b[4];
} MYINT;

typedef union{
	DWORD		w;
	BYTE		b[4];
} MYDWORD;


// DEFINE MACRO PART...
#define BufInc(x) (x)++;(x) %= SOCKET_BUF_SIZE;

// DEFINE Shared Memory Queue Flag

#define E	0x00
#define R	0x01
#define W	0x02
#define WR	0x03

// DEFINE Shared Memory Queue Return VALUE

#define SMQ_BROKEN		10000
#define SMQ_FULL		10001
#define SMQ_EMPTY		10002
#define SMQ_PKTSIZEOVER	10003
#define SMQ_WRITING		10004
#define SMQ_READING		10005

// DEFINE Shared Memory Costumizing

#define MAX_PKTSIZE		512
#define MAX_COUNT		4096
#define SMQ_LOGGERSEND	"KNIGHT_SEND"
#define SMQ_LOGGERRECV	"KNIGHT_RECV"

// Packet Define...
#define WIZ_LOGIN				0x01	// Account Login
#define WIZ_NEW_CHAR			0x02	// Create Character DB
#define WIZ_DEL_CHAR			0x03	// Delete Character DB
#define WIZ_SEL_CHAR			0x04	// Select Character
#define WIZ_SEL_NATION			0x05	// Select Nation
#define WIZ_ALLCHAR_INFO_REQ	0x0C	// Account All Character Info Request
#define WIZ_LOGOUT				0x0F	// Request Logout
#define WIZ_DATASAVE			0x37	// User GameData DB Save Request
#define WIZ_KNIGHTS_PROCESS		0x3C	// Knights Related Packet..
#define WIZ_CLAN_PROCESS		0x4E	// Clans Related Packet..
#define WIZ_LOGIN_INFO			0x50	// define for DBAgent Communication
#define WIZ_KICKOUT				0x51	// Account ID forbid duplicate connection
#define WIZ_BATTLE_EVENT		0x57	// Battle Event Result

#define DB_COUPON_EVENT			0x10	// coupon event
		#define CHECK_COUPON_EVENT		0x01
		#define UPDATE_COUPON_EVENT		0x02

////////////////////////////////////////////////////////////////
// Knights Packet sub define 
////////////////////////////////////////////////////////////////
#define KNIGHTS_CREATE			0x11		// ����
#define KNIGHTS_JOIN			0x12		// ����
#define KNIGHTS_WITHDRAW		0x13		// Ż��
#define KNIGHTS_REMOVE			0x14		// ��� ����
#define KNIGHTS_DESTROY			0x15		// �ǰ���
#define KNIGHTS_ADMIT			0x16		// ��� ���� �㰡
#define KNIGHTS_REJECT			0x17		// ��� ���� ����
#define KNIGHTS_PUNISH			0x18		// ��� ¡��
#define KNIGHTS_CHIEF			0x19		// ���� �Ӹ�
#define KNIGHTS_VICECHIEF		0x1A		// �δ��� �Ӹ�
#define KNIGHTS_OFFICER			0x1B		// �屳�Ӹ�
#define KNIGHTS_ALLLIST_REQ		0x1C		// ����Ʈ�� 10�� ������ Page ��û
#define KNIGHTS_MEMBER_REQ		0x1D		// ��� ��� ��û
#define KNIGHTS_CURRENT_REQ		0x1E		// ���� ���� ����Ʈ
#define KNIGHTS_STASH			0x1F		// ���� â��
#define KNIGHTS_MODIFY_FAME		0x20		// ����� ���� ����.. �ش� ������� ����
#define KNIGHTS_JOIN_REQ		0x21		// �ش������� ���Կ�û�� �Ѵ�
#define KNIGHTS_LIST_REQ		0x22		// ���� ����Ʈ��  ��û ( index �˻� )

////////////////////////////////////////////////////////////////
// Clan Packet sub define
////////////////////////////////////////////////////////////////
#define CLAN_CREATE				0x01
#define CLAN_JOIN				0x02

////////////////////////////////////////////////////////////////
// Update User Data type define
////////////////////////////////////////////////////////////////
#define UPDATE_LOGOUT			0x01
#define UPDATE_ALL_SAVE			0x02
#define UPDATE_PACKET_SAVE		0x03


// Reply packet define...

#define SEND_ME					0x01
#define SEND_REGION				0x02
#define SEND_ALL				0x03

#define SLOT_MAX			14		// ���� ���� MAX
#define HAVE_MAX			28		// ���� ���� MAX (�κ��丮â)	
#define ITEMCOUNT_MAX		9999	// �Ҹ� ������ ���� �Ѱ谪
#define WAREHOUSE_MAX		196		// â�� ������ MAX
/////////////////////////////////////////////////////////////////////////////////
// Structure Define
/////////////////////////////////////////////////////////////////////////////////
struct	_ITEM_DATA
{
	int		nNum;		// item ��ȣ
	short	sDuration;	// item ������
	short	sCount;		// item ���� or item �ູ �Ӽ��� �ش� ��
	__int64	nSerialNum;	// item serial code
};

struct _ITEM_TABLE
{
	int   m_iNum;				// item num
	char  m_strName[50];		// item Name
	BYTE  m_bKind;				// item ����
	BYTE  m_bSlot;				// ������ġ
	BYTE  m_bRace;				// ��� ������ ����
	BYTE  m_bClass;				// ��� ������ Class
	short m_sDamage;			// �ִ� Ÿ��ġ
	short m_sDelay;				// ���ݽð�
	short m_sRange;				// ����(��ȿ�Ÿ�)
	short m_sWeight;			// ����
	short m_sDuration;			// ������
	int	  m_iBuyPrice;			// ������ ��°���
	int	  m_iSellPrice;			// ������ �Ĵ°���
	short m_sAc;				// ����
	BYTE  m_bCountable;			// ���� ���� ������
	int	  m_iEffect1;			// ���� ����Ʈ1
	int	  m_iEffect2;			// ���� ����Ʈ2
	BYTE  m_bReqLevel;			// �䱸 ����
	BYTE  m_bReqRank;			// �䱸 ����
	BYTE  m_bReqTitle;			// �䱸 ����
	BYTE  m_bReqStr;			// �䱸 ��
	BYTE  m_bReqSta;			// �䱸 ü��
	BYTE  m_bReqDex;			// �䱸 ��ø
	BYTE  m_bReqIntel;			// �䱸 ����
	BYTE  m_bReqCha;			// �䱸 �ŷ�
	BYTE  m_bSellingGroup;		// ���� ��� ��ǰ
	BYTE  m_ItemType;			// ���������� �Ǵ� ���������
	short m_sHitrate;			// Ÿ�ݷ�
	short m_sEvarate;			// ȸ����
	short m_sDaggerAc;			// ����1
	short m_sSwordAc;			// ����2
	short m_sMaceAc;			// ����3
	short m_sAxeAc;				// ����4
	short m_sSpearAc;			// ����5
	short m_sBowAc;				// ����6
	BYTE  m_bFireDamage;		// �� �Ӽ�
	BYTE  m_bIceDamage;			// �ñ� �Ӽ�
	BYTE  m_bLightningDamage;	// ���� �Ӽ�
	BYTE  m_bPoisonDamage;		// �� �Ӽ�
	BYTE  m_bHPDrain;			// HP ���
	BYTE  m_bMPDamage;			// MP Ÿ��
	BYTE  m_bMPDrain;			// MP ���
	BYTE  m_bMirrorDamage;		// �ݻ� Ÿ��
	BYTE  m_bDroprate;			// ��� ����
	BYTE  m_bStrB;				// �� ���ʽ�
	BYTE  m_bStaB;				// ü�� ���ʽ�
	BYTE  m_bDexB;				// ��ø�� ���ʽ�
	BYTE  m_bIntelB;			// ���� ���ʽ�
	BYTE  m_bChaB;				// �ŷ� ���ʽ�
	short m_MaxHpB;				// MaxHP add
	short m_MaxMpB;				// MaxMP add
	BYTE  m_bFireR;				// �� ���� ���׷�
	BYTE  m_bColdR;				// ���� ���� ���׷�
	BYTE  m_bLightningR;		// ���� ���� ���׷�
	BYTE  m_bMagicR;			// ��Ÿ ���� ���׷�
	BYTE  m_bPoisonR;			// �� ���� ���׷�
	BYTE  m_bCurseR;			// ���� ���� ���׷�
};

struct _USER_DATA
{
	char	m_id[MAX_ID_SIZE+1];			// ���� ID
	char	m_Accountid[MAX_ID_SIZE+1];		// ���� ID

	BYTE	m_bZone;						// ���� Zone
	float	m_curx;							// ���� X ��ǥ
	float	m_curz;							// ���� Z ��ǥ
	float	m_cury;							// ���� Y ��ǥ

	BYTE	m_bNation;						// �Ҽӱ���
	BYTE	m_bRace;						// ����
	short	m_sClass;						// ����
	BYTE	m_bHairColor;					// ����
	BYTE	m_bRank;						// ����
	BYTE	m_bTitle;						// ����
	BYTE	m_bLevel;						// ����
	int		m_iExp;							// ����ġ
	int		m_iLoyalty;						// �ο�Ƽ
	BYTE	m_bFace;					// ����
	BYTE	m_bCity;						// �Ҽӵ���
	short	m_bKnights;						// �Ҽ� ����
	//short	m_sClan;						// �Ҽ� Clan
	BYTE	m_bFame;						// ��
	short	m_sHp;							// HP
	short	m_sMp;							// MP
	short	m_sSp;							// SP
	BYTE	m_bStr;							// ��
	BYTE	m_bSta;							// �����
	BYTE	m_bDex;							// ����, ȸ����
	BYTE	m_bIntel;						// ����(?), ĳ���� ������ ����
	BYTE	m_bCha;							// ���� ������, ���� ���� ����(?)
	BYTE	m_bAuthority;					// ���� ����
	BYTE	m_bPoints;						// ���ʽ� ����Ʈ
	int		m_iGold;						// ĳ���� ���� ��(21��)
	short	m_sBind;						// Saved Bind Point
	int		m_iBank;						// â���� ��(21��)
	
	BYTE    m_bstrSkill[9];				// ������ ��ų
	_ITEM_DATA m_sItemArray[HAVE_MAX+SLOT_MAX];		// 42*8 bytes
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];	// â�� ������	196*8 bytes

	BYTE	m_bLogout;						// �α׾ƿ� �÷���
	BYTE	m_bWarehouse;					// â�� �ŷ� �߾���?
	DWORD	m_dwTime;						// �÷���Ÿ��...
};


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	Global Function Define
//

inline void GetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf, sBuf+index, len);
	index += len;
};

inline BYTE GetByte(char* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (BYTE)(*(sBuf+t_index));
};

inline int GetShort(char* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf+index-2);
};

inline DWORD GetDWORD(char* sBuf, int& index)
{
	index += 4;
	return *(DWORD*)(sBuf+index-4);
};

inline float Getfloat(char* sBuf, int& index)
{
	index += 4;
	return *(float*)(sBuf+index-4);
};

inline __int64 GetInt64(char* sBuf, int& index)
{
	index += 8;
	return *(__int64*)(sBuf+index-8);
};

inline void SetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf+index, sBuf, len);
	index += len;
};

inline void SetByte(char* tBuf, BYTE sByte, int& index)
{
	*(tBuf+index) = (char)sByte;
	index++;
};

inline void SetShort(char* tBuf, int sShort, int& index)
{
	short temp = (short)sShort;

	CopyMemory( tBuf+index, &temp, 2);
	index += 2;
};

inline void SetDWORD(char* tBuf, DWORD sDWORD, int& index)
{
	CopyMemory( tBuf+index, &sDWORD, 4);
	index += 4;
};

inline void Setfloat ( char* tBuf, float sFloat, int& index )
{
	CopyMemory( tBuf+index, &sFloat, 4);
	index += 4;
};

inline void SetInt64 ( char* tBuf, __int64 nInt64, int& index )
{
	CopyMemory( tBuf+index, &nInt64, 8);
	index += 8;
};
// sungyong 2001.11.06
inline int GetVarString(TCHAR* tBuf, TCHAR* sBuf, int nSize, int& index)
{
	int nLen = 0;
	
	if(nSize == sizeof(BYTE))	nLen = GetByte(sBuf, index);
	else nLen = GetShort(sBuf, index);

	GetString(tBuf, sBuf, nLen, index);
	*(tBuf + nLen) = 0;

	return nLen;
};

inline void SetVarString(TCHAR *tBuf, TCHAR* sBuf, int len, int &index)
{
	*(tBuf+index) = (BYTE)len;
	index ++;

	CopyMemory(tBuf+index, sBuf, len);
	index += len;
};
// ~sungyong 2001.11.06
inline int ParseSpace( char* tBuf, char* sBuf)
{
	int i = 0, index = 0;
	BOOL flag = FALSE;
	
	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	while(sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] !=(BYTE) 0){
		tBuf[i++] = sBuf[index++];
		flag = TRUE;
	}
	tBuf[i] = 0;

	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	if(!flag) return 0;	
	return index;
};

inline CString GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString _Path = Path;
	return _Path;
};

inline void LogFileWrite( LPCTSTR logstr )
{
	CString ProgPath, LogFileName;
	CFile file;
	int loglength;

	ProgPath = GetProgPath();
	loglength = strlen( logstr );

	LogFileName.Format("%s\\Aujard.log", ProgPath);
	
	file.Open( LogFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );

	file.SeekToEnd();
	file.Write(logstr, loglength);
	file.Close();
};

inline int DisplayErrorMsg(SQLHANDLE hstmt)
{
	SQLCHAR       SqlState[6], Msg[1024];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc2;
	char		  logstr[512];
	memset( logstr, NULL, 512 );

	i = 1;
	while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
	{
		sprintf( logstr, "*** %s, %d, %s, %d ***\r\n", SqlState,NativeError,Msg,MsgLen );
		LogFileWrite( logstr );

		i++;
	}

	if( strcmp((char *)SqlState, "08S01") == 0 )
		return -1;
	else
		return 0;
};

