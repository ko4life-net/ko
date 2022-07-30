// N3FXPartBillBoardGame.h: interface for the CN3FXPartBillBoard class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3FXPartBillBoard.h"

class CN3FXPartBillBoardGame : public CN3FXPartBillBoard
{
public:
	float	GetGroundHeight(float x, float z);

	CN3FXPartBillBoardGame() {}
	virtual ~CN3FXPartBillBoardGame() {}
};

