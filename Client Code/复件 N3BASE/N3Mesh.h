// N3MeshNormal.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Mesh_h__INCLUDED_)
#define AFX_N3Mesh_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CN3IMesh;

class CN3Mesh : public CN3Base
{
protected:
	__VertexT1*	m_pVertices; // Á¡ ¹öÆÛ
	WORD*		m_psnIndices; // ÀÎµ¦½º ¹öÆÛ

	int		m_nVC; // Á¡ °¹¼ö
	int		m_nIC; // Á¡ °¹¼ö
	__Vector3 m_vMin, m_vMax;

public:
	bool Import(CN3IMesh* pIMesh);
	__Vector3 Min() { return m_vMin; }
	__Vector3 Max() { return m_vMax; }
	void	FindMinMax();
	
	int			VertexCount() { return m_nVC; }
	__VertexT1*	Vertices() { return m_pVertices; }

	int		IndexCount() { return m_nIC; }
	WORD*	Indices() { return m_psnIndices; } // ÀÎµ¦½º ¹öÆÛ

	bool	Load(HANDLE hFile);
	bool	Save(HANDLE hFile);
	
	void	Create(int nVC, int nIC);
	void	Create_Cube(__Vector3 &vMin, __Vector3 &vMax);
	void	Create_Axis(float fLength);
	void	MakeIndexed();
	void	ReGenerateSmoothNormal();

	void	Release();
	void	ReleaseVertices();
	void	ReleaseIndices();

	CN3Mesh();
	virtual ~CN3Mesh();
};

#endif // !defined(AFX_N3Mesh_h__INCLUDED_)
