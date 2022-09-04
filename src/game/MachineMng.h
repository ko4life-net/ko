// MachineMng.h: interface for the CMachineMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMachineBase;

#include "GameBase.h"

typedef typename std::list<CMachineBase *>::iterator it_Machine;

class CMachineMng : public CGameBase {
  public:
    CMachineMng();
    virtual ~CMachineMng();

    // Attributes
  public:
  protected:
    std::list<CMachineBase *> m_Machines;

    // Operations
  public:
    //    void            ReceiveReplyMsg(ReplyMsg& RPMsg);
    virtual void Release();
    void         Render();
    void         Tick();

  protected:
    //    void            AddMachine(ReplyMsg& RPMsg);
    CMachineBase * GetMachine(const std::string & szID);
};
