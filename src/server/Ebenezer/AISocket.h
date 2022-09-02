// AISocket.h: interface for the CAISocket class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "IOCPSocket2.h"
#include "MagicProcess.h"

class CEbenezerDlg;
class CAISocket : public CIOCPSocket2 {
  private:
    CEbenezerDlg * m_pMain;

    BOOL          m_bAllNpcInfoRecv;
    CMagicProcess m_MagicProcess;

  public:
    int m_iZoneNum;

    CAISocket(int zonenum);
    virtual ~CAISocket();

    void Initialize();
    int  GetZoneNumber() { return m_iZoneNum; };
    void Parsing(int len, char * pData);
    void CloseProcess();

    void InitEventMonster(int index);
    // Packet recv
    void LoginProcess(char * pBuf);
    void RecvCheckAlive(char * pBuf);
    void RecvServerInfo(char * pBuf);
    void RecvNpcInfoAll(char * pBuf);
    void RecvNpcMoveResult(char * pBuf);
    void RecvNpcAttack(char * pBuf);
    void RecvMagicAttackResult(char * pBuf);
    void RecvNpcInfo(char * pBuf);
    void RecvUserHP(char * pBuf);
    void RecvUserExp(char * pBuf);
    void RecvSystemMsg(char * pBuf);
    void RecvNpcGiveItem(char * pBuf);
    void RecvUserFail(char * pBuf);
    void RecvCompressedData(char * pBuf);
    void RecvGateDestory(char * pBuf);
    void RecvNpcDead(char * pBuf);
    void RecvNpcInOut(char * pBuf);
    void RecvBattleEvent(char * pBuf);
    void RecvNpcEventItem(char * pBuf);
    void RecvGateOpen(char * pBuf);
};
