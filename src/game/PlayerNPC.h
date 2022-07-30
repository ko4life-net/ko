// PlayerNPC.h: interface for the CPlayerNPC class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "PlayerBase.h"
#include "GameBase.h"

//////////////////////////
// Other Player..
#include "PlayerBase.h"

class CPlayerNPC : public CPlayerBase
{
	friend class CPlayerOtherMgr;
public:
	void			MoveTo(float fPosX, float fPosY, float fPosZ, float fMoveSpeed, int iMoveMode); // 이 위치로 이동..
	virtual	void	Tick();

	CPlayerNPC();
	virtual ~CPlayerNPC();
};

