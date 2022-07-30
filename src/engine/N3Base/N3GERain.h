// N3GERain.h: interface for the CN3GERain class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3GlobalEffect.h"

class CN3GERain : public CN3GlobalEffect  
{
public:
	CN3GERain();
	virtual ~CN3GERain();

// Attributes
public:
	void SetRainLength(float fLen) {m_fRainLength = fLen;}
	void SetVelocity(__Vector3& v) {m_vVelocity = v;}
protected:
	float		m_fWidth;
	float		m_fHeight;
	float		m_fRainLength;
	__Vector3	m_vVelocity;

// Operations
public:
	void Release();
	void Tick();
	void Render(__Vector3& vPos);

	void Create(float fDensity, float fWidth, float fHeight, float fRainLength, const __Vector3& vVelocity, float fTimeToFade = 3.0f);
protected:

};

