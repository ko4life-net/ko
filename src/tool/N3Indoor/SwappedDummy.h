// SwappedDummy.h: interface for the CSwappedDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "TransDummy.h"

class CSwappedDummy : public CTransDummy  
{
public:
	CSwappedDummy();
	virtual ~CSwappedDummy();

public:
	virtual BOOL MouseMsgFilter(LPMSG pMsg);
};

