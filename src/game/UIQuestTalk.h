// UIQuestTalk.h: interface for the CUIQuestTalk class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3UIBase.h"
#include "PacketDef.h"
#include "APISocket.h"

#define MAX_STRING_TALK		10

class CUIQuestTalk   : public CN3UIBase
{
protected:
	class CN3UIString*		m_pTextTalk;
	class CN3UIButton*		m_pBtnOk;

	std::string				m_szTalk[MAX_STRING_TALK];
	int						m_iNumTalk;
	int						m_iCurTalk;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void Open(class DataPack* pDataPack, int& iOffset);
	CUIQuestTalk();
	virtual ~CUIQuestTalk();

};

