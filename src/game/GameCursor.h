// GameCursor.h: interface for the CGameCursor class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameDef.h"
#include "N3Base/N3UIBase.h"

class CGameCursor   : public CN3UIBase  
{
private:
	class		CN3UIImage* m_pImageCursor[CURSOR_COUNT];
	bool		m_bCursorLocked;
	e_Cursor	m_ePrevGameCursor;
	e_Cursor	m_eCurGameCursor;
	HCURSOR		m_hCursor;

public:
	void Tick();
	void Render();
	void RestoreGameCursor();
	void SetGameCursor(e_Cursor eCursor, bool bLocked = false);
	bool Load(HANDLE hFile);
	CGameCursor();
	virtual ~CGameCursor();

};

