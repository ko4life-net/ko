// WarMessage.h: interface for the CWarMessage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CDFont;

class CWarMessage  
{
private:
	float	m_fTime;
	CDFont*	m_pMessageFont;
	POINT	m_ptMessage;
public:
	void Tick();
	void	RenderMessage();
	void	SetMessage(const std::string& szText, DWORD dwFlags = D3DFONT_BOLD,DWORD dwColor = 0xffffffff);
	void	Release();
	void	InitFont();
	CWarMessage();
	virtual ~CWarMessage();

};

