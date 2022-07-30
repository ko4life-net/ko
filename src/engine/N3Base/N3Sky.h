// N3Sky.h: interface for the CN3Sky class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base.h"
#include "N3ColorChange.h"

//class CN3Texture;
class CN3Sky : public CN3Base  
{
	friend class CN3SkyMng;
public:
	CN3Sky();
	virtual ~CN3Sky();

// Attributes
public:
protected:
	CN3ColorChange		m_SkyColor;
	CN3ColorChange		m_FogColor;

	__VertexXyzColor	m_vFronts[4];
	__VertexXyzColor	m_Bottom[4];
//	CN3Texture*			m_pTex;

// Operations
public:
	void				Init();
	void				Render();
	virtual				void Release();
	virtual				void Tick();
protected:
};

