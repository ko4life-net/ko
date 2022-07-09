// N3VMesh.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3VMesh_h__INCLUDED_)
#define AFX_N3VMesh_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "My_3DStruct.h"	// Added by ClassView

class CN3VMesh// : public CN3Base
{
protected:
	__Vector3*	m_pVertices; // 점 버퍼
	int		m_nVC; // 점 갯수

	WORD*	m_pwIndices; // Index...
	int		m_nIC; // Index Count

	__Vector3 m_vCenter; // Mesh Vertices 의 중간점..
	float m_fRadius; // 반지름

	int		m_nRegionX;
	int		m_nRegionZ;
public:
	void SetRegion( int dimx, int dimz );
	float Radius() { return m_fRadius; }
	__Vector3 Center() { return m_vCenter; }
	void CalcRadiusAndCenter();
//	void Render(D3DCOLOR crLine);
	void CreateVertices(int nVC);
	void CreateIndex(int nIC);
	void CreateCube(const __Vector3& vMin, const __Vector3& vMax);
	
	int			VertexCount() { return m_nVC; }
	__Vector3*	Vertices() { return m_pVertices; }

	int		IndexCount() { return m_nIC; }
	WORD*	Indices() { return m_pwIndices; }

	int	Load(HANDLE hFile);
	int	Save(HANDLE hFile);
	
	void	Release();

	CN3VMesh();
	virtual ~CN3VMesh();
};

#endif // !defined(AFX_N3VMesh_h__INCLUDED_)
