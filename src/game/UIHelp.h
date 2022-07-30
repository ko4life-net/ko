// UIHelp.h: interface for the CUIHelp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3UIBase.h"

const int MAX_HELP_PAGE = 4;

class CUIHelp : public CN3UIBase  
{
public:
	CN3UIBase* m_pPages[MAX_HELP_PAGE];

	CN3UIBase* m_pBtn_Prev;
	CN3UIBase* m_pBtn_Next;
	CN3UIBase* m_pBtn_Close;

public:
	void SetVisible(bool bVisible);
	bool	OnKeyPress(int iKey);
	void	Release();
	bool	Load(HANDLE hFile);
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	CUIHelp();
	virtual ~CUIHelp();
};

