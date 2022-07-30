// GameProcNationSelect.h: interface for the CGameProcNationSelect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameProcedure.h"

class CGameProcNationSelect : public CGameProcedure   
{
public:
	class CUINationSelectDlg* m_pUINationSelectDlg;	

protected:
	bool ProcessPacket(DataPack* pDataPack, int& iOffset);

public:
	void MsgSendNationSelect(e_Nation eNation);
	
	void Release();
	void Init();
	void Tick();
	void Render();

	CGameProcNationSelect();
	virtual ~CGameProcNationSelect();
};

