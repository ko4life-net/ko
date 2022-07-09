// N3GlobalEffectMng.h: interface for the CN3GlobalEffectMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_)
#define AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CN3GERain;
class CN3GESnow;
class CN3GlobalEffectMng : public CN3Base  
{
public:
	CN3GlobalEffectMng();
	virtual ~CN3GlobalEffectMng();

// Attributes
public:
protected:
	float			m_fCellSize;	// ³»ºÎ ¼¿ Å©±â
	POINT			m_CurCellPos;	// ÇöÀç ³»ºÎ¼¿ ÁÂÇ¥
	__Vector3		m_vPos[9];		// ÁÖº¯ ¼¿ 9Ä­ÀÇ Áß½ÉÁÂÇ¥µé

	// È¿°úµé..
	CN3GERain*		m_pGERain;		// ºñ
	CN3GESnow*		m_pGESnow;		// ´«

// Operations
public:
	virtual void	Release();
	void			Tick();
	void			Render();
	void			SetWeather(int iWeather);	// ³¯¾¾ ÁöÁ¤ÇÏ±â
protected:

};

#endif // !defined(AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_)
