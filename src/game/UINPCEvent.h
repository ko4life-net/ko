// UINPCEvent.h: interface for the UINPCEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameDef.h"

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIString.h"

enum e_NpcEvent	{ NPC_EVENT_ITEM_TRADE = 0, NPC_EVENT_TRADE_REPAIR = 1, NPC_EVENT_KNIGHTS = 2 };

//////////////////////////////////////////////////////////////////////


class CUINPCEvent   : public CN3UIBase  
{
	int					m_iTradeID;
	int					m_iIDTarget;
	CN3UIButton*		m_pBtn_Repair;
	CN3UIString*		m_pText_Repair;
	CN3UIString*		m_pText_Title;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	void Release();

	CUINPCEvent();
	virtual ~CUINPCEvent();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void Open(e_NpcEvent eNpcEvent, int iTradeId = -1, int iIDTarget = -1);
	void Close();
};

