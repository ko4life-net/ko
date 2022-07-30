// UIPointInitDlg.h: interface for the UIPointInitDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "GameDef.h"

#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIString.h"

//////////////////////////////////////////////////////////////////////


class CUIPointInitDlg     : public CN3UIBase  
{
	CN3UIButton*		m_pBtn_Ok;
	CN3UIButton*		m_pBtn_Cancel;

	CN3UIString*		m_pText_NeedGold;
	bool				m_bAllpoint;

public:
	CUIPointInitDlg();
	virtual ~CUIPointInitDlg();

	void	InitDlg(bool bAllpoint, int iGold);
	bool	OnKeyPress(int iKey);
	void	Release();

	bool	Load(HANDLE hFile);
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void	Close();

	void	PushOkButton();
};

