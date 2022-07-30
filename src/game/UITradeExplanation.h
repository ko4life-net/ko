// UITradeExplanation.h: interface for the CUITradeExplanation class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameDef.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "N3Base/N3UIBase.h"

class CUITradeExplanation : public CN3UIBase  
{
protected:
	class CN3UIButton*		m_pBtn_PageUp;
	class CN3UIButton*		m_pBtn_PageDown;
	class CN3UIButton*		m_pBtn_Close;
	class CN3UIString*		m_pText_Explanation;

	std::string m_szExplanation;
	int			m_iCurSel;
public:
	bool OnKeyPress(int iKey);
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void SetExplanation(int iCurSel, std::string szExplanation);

	CUITradeExplanation();
	virtual ~CUITradeExplanation();

};

