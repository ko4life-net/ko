// N3UIIcon.h: interface for the CN3UIIcon class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3UIImage.h"

class CN3UIDBCLButton  : public CN3UIImage
{
public:
	CN3UIDBCLButton();
	virtual ~CN3UIDBCLButton();

	virtual DWORD	MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
};

