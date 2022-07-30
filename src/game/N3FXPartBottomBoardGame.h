// N3FXPartBottomBoardGame.h: interface for the CN3FXPartBottomBoard class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3FXPartBottomBoard.h"

class CN3FXPartBottomBoardGame : public CN3FXPartBottomBoard
{
public:
	float	GetGroundHeight(float x, float z);

public:
	CN3FXPartBottomBoardGame();
	virtual ~CN3FXPartBottomBoardGame();
};

