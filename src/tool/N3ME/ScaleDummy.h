// ScaleDummy.h: interface for the CScaleDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "TransDummy.h"

class CScaleDummy : public CTransDummy {
  public:
    CScaleDummy();
    virtual ~CScaleDummy();

  public:
  protected:
  public:
    virtual BOOL MouseMsgFilter(LPMSG pMsg);

  protected:
};
