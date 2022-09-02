// GameEvent.h: interface for the CGameEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CUser;
class CGameEvent {
  public:
    void RunEvent(CUser * pUser = NULL);
    CGameEvent();
    virtual ~CGameEvent();

    short m_sIndex;
    BYTE  m_bType;

    int m_iCond[5];

    int  m_iExec[5];
    char m_strExec[5][255];
};
