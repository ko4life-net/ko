// GameProcLogIn.h: interface for the CGameProcLogIn class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameProcedure.h"

class CGameProcLogIn : public CGameProcedure {
  public:
    class CN3Chr *     m_pChr;
    class CN3Texture * m_pTexBkg;
    class CUILogIn *   m_pUILogIn;

    class CN3Camera * m_pCamera;
    class CN3Light *  m_pLights[3];

    bool m_bLogIn; // 로그인 중복 방지..

    float m_fCurAudioFrm;

  public:
    void MsgRecv_GameServerGroupList(DataPack * pDataPack, int & iOffset);
    void MsgRecv_AccountLogIn(int iCmd, DataPack * pDataPack, int & iOffset);
    int  MsgRecv_VersionCheck(DataPack * pDataPack, int & iOffset);    // virtual
    int  MsgRecv_GameServerLogIn(DataPack * pDataPack, int & iOffset); // virtual - 국가 번호를 리턴한다.

    bool MsgSend_AccountLogIn(enum e_LogInClassification eLIC);

    void Release();
    void Init();
    void Tick();
    void Render();

  protected:
    virtual bool ProcessPacket(DataPack * pDataPack, int & iOffset);

  public:
    void ConnectToGameServer(); // 고른 게임 서버에 접속
    CGameProcLogIn();
    virtual ~CGameProcLogIn();
    //    void PacketSend_MGameLogin();
};
