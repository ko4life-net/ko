// UdpSocket.h: interface for the CUdpSocket class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CEbenezerDlg;
class CUdpSocket  
{
public:
	void RecvBattleZoneCurrentUsers( char* pBuf );
	void RecvDestroyKnights( char* pBuf );
	void RecvModifyFame( char* pBuf, BYTE command );
	void RecvJoinKnights( char* pBuf, BYTE command );
	void RecvCreateKnights( char* pBuf );
	void ReceiveKnightsProcess( char* pBuf );
	void RecvBattleEvent(char *pBuf);
	void ServerChat( char* pBuf );
	void Parsing( char* pBuf, int len );
	bool PacketProcess(int len);
	int  SendUDPPacket(char* strAddress, char* pBuf, int len);
	bool CreateSocket();
	CUdpSocket(CEbenezerDlg* pMain=NULL);
	virtual ~CUdpSocket();

	SOCKET m_hUDPSocket;
	SOCKADDR_IN m_SocketAddress;
	SOCKADDR_IN m_ReplyAddress;

	HANDLE m_hUdpThread;
	char	m_pRecvBuf[8192];
	CEbenezerDlg* m_pMain;
};

