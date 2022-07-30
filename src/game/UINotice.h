// UINotice.h: interface for the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3UIBase.h"
#include <list>
#include <string>

typedef typename std::list<std::string>::iterator it_String;

class CUINotice : public CN3UIBase  
{
public:
	class CN3UIString*		m_pText_Notice;
	class CN3UIScrollBar*	m_pScrollBar;
	class CN3UIButton*		m_pBtn_OK;

	std::list<std::string>	m_Texts;

public:
	void RemoveNotice();
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	void GenerateText();
	void Release();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	CUINotice();
	virtual ~CUINotice();

};

