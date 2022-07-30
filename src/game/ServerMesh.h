// ServerMesh.h: interface for the CServerMesh class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3Base.h"

class CN3Terrain;

class CServerMesh   : public CN3Base  
{
	__Vector3		m_vSMesh[48];
	void	AutoConcMesh(D3DCOLOR color, float left, float right, float bottom, float top, float low, float high, int iStart );

public:
	CServerMesh();
	virtual ~CServerMesh();

	void	Tick(CN3Terrain* pTerrain, const __Vector3& vPosPlayer);
	void	Render();
};

