// IOCPort.h: interface for the CIOCPort class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#pragma warning(disable : 4786)

#include <list>
using namespace std;

typedef  list<int>  SidList;

class CIOCPSocket2;

class CIOCPort  
{
public:
	int GetClientSid();
	void CreateAcceptThread();
	void CreateClientWorkerThread();
	void RidIOCPSocket(int index, CIOCPSocket2 *pSock);
	CIOCPSocket2* GetIOCPSocket(int index);
	void CreateReceiveWorkerThread(int workernum);
	void PutOldSid(int sid);
	int GetNewSid();
	BOOL Associate(CIOCPSocket2 *pIocpSock, HANDLE hPort);
	BOOL Listen( int port );
	void Init( int serversocksize, int clientsocksize, int workernum = 0 );
	void DeleteAllArray();
	CIOCPort();
	virtual ~CIOCPort();

	SOCKET m_ListenSocket;
	HANDLE m_hListenEvent;
	HANDLE m_hServerIOCPort;
	HANDLE m_hClientIOCPort;
	HANDLE m_hAcceptThread;

	int m_SocketArraySize;
	int m_ClientSockSize;

	SidList m_SidList;
	CIOCPSocket2 **m_SockArray;
	CIOCPSocket2 **m_SockArrayInActive;

	CIOCPSocket2 **m_ClientSockArray;		// Connect¿ë ¼ÒÄÏ

protected:
	DWORD m_dwNumberOfWorkers;
	DWORD m_dwConcurrency;
};

