// RotDummy.h: interface for the CRotDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "TransDummy.h"

class CRotDummy : public CTransDummy  
{
public:
	CRotDummy();
	virtual ~CRotDummy();

public:
protected:

public:
	void SetSelObj(SelectElement Obj, bool bOne = true);
	virtual BOOL MouseMsgFilter(LPMSG pMsg);
protected:
};

