// KnightChrMgr.h: interface for the CKnightChrMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "SharedMem.h"

class CKnightChrMgr  
{
	char	m_sIDAndPW[32];
	int		m_iActionNo;

public:
	CKnightChrMgr(HWND hWnd);
	virtual ~CKnightChrMgr();

public:
	CSharedMemQueue	Smq;

	void RCommand(char rbuf[], int leng);
	void TCommand(char);

	LONG OnReceiveSmq(UINT WParam, LONG LParam);

	void SendActionCommand(int iAc);
};

