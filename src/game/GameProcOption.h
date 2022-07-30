// GameProcOption.h: interface for the CGameProcOption class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameProcedure.h"

class CGameProcOption : public CGameProcedure  
{
public:
	void Init();
	void Release();
	void Tick();
	void Render();
	
	CGameProcOption();
	virtual ~CGameProcOption();

};

