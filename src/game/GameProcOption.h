// GameProcOption.h: interface for the CGameProcOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCOPTION_H__98F2C543_CFC5_45AA_84DA_B93B34A29C46__INCLUDED_)
#define AFX_GAMEPROCOPTION_H__98F2C543_CFC5_45AA_84DA_B93B34A29C46__INCLUDED_


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

#endif // !defined(AFX_GAMEPROCOPTION_H__98F2C543_CFC5_45AA_84DA_B93B34A29C46__INCLUDED_)
