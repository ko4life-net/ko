// SoundCell.h: interface for the CSoundCell class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3Base.h"

class CLyTerrain;

const int MAX_CON = 5;

class CSoundCell : public CN3Base
{
protected:
	int					m_iVersion;
	__VertexXyzColor	m_TileVB[4];

public:
	CLyTerrain*			m_pRefTerrain;

	DWORD				m_dwSoundGroupID;

	RECT				m_Rect;
	POINT				m_BasePoint;

protected:
	void	MakeTileVB(int x, int z, DWORD color);
	
public:
	void	InitRect(__Vector3 v);
	void	AddRect(__Vector3 v);
	void	Render(DWORD color);
	void	Load(HANDLE hFile);
	void	Save(HANDLE hFile);

	CSoundCell();
	CSoundCell(CLyTerrain* pRefTerrain);
	virtual ~CSoundCell();
};

