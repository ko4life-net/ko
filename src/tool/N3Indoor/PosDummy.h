// PosDummy.h: interface for the CPosDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "TransDummy.h"

class CPosDummy : public CTransDummy  
{
public:
	CPosDummy();
	virtual ~CPosDummy();

public:
protected:

public:
	virtual void SetSelObj(SelectElement pObj, bool bOne = true);
	virtual BOOL MouseMsgFilter(LPMSG pMsg);
protected:

};

