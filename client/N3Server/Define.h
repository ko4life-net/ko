#ifndef _DEFINE_H
#define _DEFINE_H

#define _LISTEN_PORT		10000
#define MAX_USER			3000

#define MAX_ID_SIZE			20
#define MAX_ITEM			28
#define VIEW_DIST			64		// 가시거리

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

// Socket type
#define TYPE_ACCEPT				0x01
#define TYPE_CONNECT			0x02

// Overlapped flag
#define OVL_RECEIVE				0X01
#define OVL_SEND				0X02
#define OVL_CLOSE				0X03
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

// Packet Define...

#define WIZ_LOGIN				0x01
#define WIZ_GAMESTART			0x02
#define WIZ_MOVE				0x03
#define WIZ_USERINOUT			0x04

// Reply packet define...

#define RE_USER_LOGIN			0x01
#define SERVER_ALIVE_CHECK		0x02
#define RE_CHANGE_ZONE			0x03
#define RE_USER_CLOSE			0X04

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	Global Function Define
//

void	GetString( char* tBuf, char* sBuf, int len, int& index );
BYTE	GetByte  ( char* sBuf, int& index );
int 	GetShort ( char* sBuf, int& index );
DWORD	GetDWORD ( char* sBuf, int& index );
float	Getfloat ( char* sBuf, int& index );

void	SetString( char* tBuf, char* sBuf, int len, int& index );
void	SetByte  ( char* tBuf, BYTE sByte, int& index );
void	SetShort ( char* tBuf, int sShort, int& index );
void	SetDWORD ( char* tBuf, DWORD sWORD, int& index);
void	Setfloat ( char* tBuf, float sFloat, int& index );
int		ParseSpace( char* tBuf, char* sBuf );

CString GetProgPath();

#endif