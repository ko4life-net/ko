// SwappedDummy.h: interface for the CSwappedDummy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWAPPEDDUMMY_H__0D514EE5_E85D_493D_864F_D9E4BEB68F59__INCLUDED_)
#define AFX_SWAPPEDDUMMY_H__0D514EE5_E85D_493D_864F_D9E4BEB68F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TransDummy.h"

class CSwappedDummy : public CTransDummy  
{
public:
	CSwappedDummy();
	virtual ~CSwappedDummy();

public:
	virtual BOOL MouseMsgFilter(LPMSG pMsg);
};

#endif // !defined(AFX_SWAPPEDDUMMY_H__0D514EE5_E85D_493D_864F_D9E4BEB68F59__INCLUDED_)
