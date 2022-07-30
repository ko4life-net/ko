// Wall.h: interface for the CWall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3Base.h"
#include <list>

class CWall : public CN3Base  
{
public:
	char	m_Name[80];
	std::list<__Vector3> m_Wall;

public:
	void	Load(HANDLE hFile);
	void	Save(HANDLE hFile);
	int		GetSize() { return m_Wall.size(); }
	bool	GetVertex(int idx, __Vector3* pPos);	// idx째 점 가져오기..
	void	AddVertex(__Vector3 Vertex);	//path더하기.
	void	DelPrevVertex();

	CWall();
	virtual ~CWall();
};

