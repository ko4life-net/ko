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
	virtual void SetVertex(__Vector3* pVertex);
	virtual void SetSelObj(CN3Transform* pObj);
	virtual BOOL MouseMsgFilter(LPMSG pMsg);
protected:

};

