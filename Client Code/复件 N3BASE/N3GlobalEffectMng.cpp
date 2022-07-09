// N3GlobalEffectMng.cpp: implementation of the CN3GlobalEffectMng class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "N3GlobalEffectMng.h"
#include "N3GERain.h"
#include "N3GESnow.h"


//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////


enum {	GEW_CLEAR=0,			// ¸¼À½
		GEW_DRIZZLE_RAIN=1,		// °¡¶ûºñ
		GEW_RAINY=2,			// Àû´ç·®ÀÇ ºñ
		GEW_HEAVY_RAIN=3,		// Æø¿ì
		GEW_SNOW1=11,			// Æ÷±ÙÇÑ ´«
		GEW_SNOW2=12,			// Àû´çÇÑ ´«
		GEW_HEAVY_SNOW=13		// Æø¼³
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3GlobalEffectMng::CN3GlobalEffectMng()
{
	m_pGERain = NULL;
	m_pGESnow = NULL;
	Release();
}

CN3GlobalEffectMng::~CN3GlobalEffectMng()
{
	Release();
}

void CN3GlobalEffectMng::Release()
{
	m_fCellSize = 0.0f;
	m_CurCellPos.x = m_CurCellPos.y = -1;

	if (m_pGERain) {delete m_pGERain; m_pGERain = NULL;}
	if (m_pGESnow) {delete m_pGESnow; m_pGESnow = NULL;}
}

void CN3GlobalEffectMng::Tick()
{
	if (m_fCellSize<=0.0f) return;

	POINT NewCellPos;	
	NewCellPos.x = int(s_CameraData.vEye.x/m_fCellSize);
	NewCellPos.y = int(s_CameraData.vEye.z/m_fCellSize);
	if (NewCellPos.x != m_CurCellPos.x || NewCellPos.y != m_CurCellPos.y)
	{
		m_CurCellPos = NewCellPos;
		int i, j;
		for(i=0; i<3; ++i)
			for(j=0; j<3; ++j)
				m_vPos[j*3+i].Set( (m_CurCellPos.x+i-0.5f)*m_fCellSize, 0, (m_CurCellPos.y+j-0.5f)*m_fCellSize);
	}

	if (m_pGERain) m_pGERain->Tick();
	if (m_pGESnow) m_pGESnow->Tick();

}

void CN3GlobalEffectMng::Render()
{
	int i, j;
	for (i=0; i<3; ++i)
	{
		for(j=0; j<3; ++j)
		{
			if (m_pGERain)
			{
				m_pGERain->Render(m_vPos[j*3+i]);
			}
			if (m_pGESnow)
			{
				m_pGESnow->Render(m_vPos[j*3+i]);
			}
		}
	}
}

void CN3GlobalEffectMng::SetWeather(int iWeather)
{
	const float fHeight = 20.0f;
	BOOL	bRainy;
	float fDensity;
	__Vector3 vVelocity;
	float fRainLength;
	float fSnowSize;

	switch(iWeather)
	{
	case GEW_CLEAR:
		if (m_pGERain) {delete m_pGERain; m_pGERain = NULL;}
		if (m_pGESnow) {delete m_pGESnow; m_pGESnow = NULL;}
		return;
		break;
	case GEW_DRIZZLE_RAIN:
		bRainy = TRUE;
		fDensity = 0.03f;
		vVelocity.Set(0.3f, -7.0f, 0);
		fRainLength = 0.1f;
		break;
	case GEW_RAINY:
		bRainy = TRUE;
		fDensity = 0.08f;
		vVelocity.Set(0, -10.0f, 1);
		fRainLength= 0.2f;
		break;
	case GEW_HEAVY_RAIN:
		bRainy = TRUE;
		fDensity = 0.12f;
		vVelocity.Set(0.3f, -10.0f, -1.5f);
		fRainLength= 0.25f;
		break;
	case GEW_SNOW1:
		bRainy = FALSE;
		fDensity = 0.08f;
		vVelocity.Set(0.0f, -0.7f, 0.0f);
		fSnowSize = 0.05f;
		break;
	case GEW_SNOW2:
		bRainy = FALSE;
		fDensity = 0.1f;
		vVelocity.Set(0.5f, -1.0f, 0.0f);
		fSnowSize = 0.08f;
		break;
	case GEW_HEAVY_SNOW:
		bRainy = FALSE;
		fDensity = 0.1f;
		vVelocity.Set(-9.0f, -3.0f, 0.0f);
		fSnowSize = 0.1f;
		break;
	}
	if (bRainy)
	{
		if (m_pGERain == NULL) m_pGERain = new CN3GERain;

		m_fCellSize = 20.0f;
		m_pGERain->Create(fDensity, m_fCellSize, fHeight, fRainLength, vVelocity);	// ºñ
		m_pGERain->SetActive(TRUE);
		if (m_pGESnow) {delete m_pGESnow; m_pGESnow = NULL;}
	}
	else
	{
		if (m_pGESnow == NULL) m_pGESnow = new CN3GESnow;

		m_fCellSize = 10.0f;
		m_pGESnow->Create(fDensity, m_fCellSize, fHeight, fSnowSize, vVelocity);	// ´«
		m_pGESnow->SetActive(TRUE);
		if (m_pGERain) {delete m_pGERain; m_pGERain = NULL;}
	}
}