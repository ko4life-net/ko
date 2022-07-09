// N3MeshIndexed.h: interface for the CN3IMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3IMesh_h__INCLUDED_)
#define AFX_N3IMesh_h__INCLUDED_

#include "N3Base.h"

class CN3IMesh : public CN3Base  
{
protected:
	static __VertexT1 s_Vertices[65536];
//	static __VertexXyzT1 m_StaticVertices[32767];

	int			m_nFC; // 삼각형 갯수

	int				m_nVC; // Vertex Count
	WORD*	m_pwVtxIndices; // 점 인덱스 리스트. 
	int				m_nUVC; // UV 좌표 Count
	WORD*	m_pwUVsIndices; // 텍스처 좌표 인덱스 리스트.

	__VertexXyzNormal*	m_pVertices; // 좌표 Data
	float*		m_pfUVs; // UV Data - m_nUVC * 2 만큼 할당한다.

	__Vector3 m_vMin, m_vMax; // 최소, 최대점.. 변하긴 하지만 대략적으로...

public:
	void ApplyOffset(__Vector3 vOffset);
	void ReGenerateSmoothNormal();
	__Vector3 Min() { return m_vMin; }
	__Vector3 Max() { return m_vMax; }
	void FindMinMax();
	void Render();
	__VertexT1* BuildVertexList();
	
	int FaceCount() { return m_nFC; }

	int	VertexCount() { return m_nVC; }
	__VertexXyzNormal* Vertices() { return m_pVertices; }
	WORD* VertexInices() { return m_pwVtxIndices; }
	void VertexIndexSet(int index, int nVI) { if(index >= 0 && index < m_nFC*3) m_pwVtxIndices[index] = nVI; }

	int UVCount() { return m_nUVC; }
	void UVSet(int index, float fU, float fV) { if(index >= 0 && index < m_nUVC) { m_pfUVs[index*2+0] = fU; m_pfUVs[index*2+1] = fV; } }
	void UVIndexSet(int index, int nUVI) { if(index >= 0 && index < m_nFC*3) m_pwUVsIndices[index] = nUVI; }

	void Create(int nFC, int nVC, int nUVC); // 차례대로  Face Count, VertexCount, UV Count
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	void Release();
	CN3IMesh();
	virtual ~CN3IMesh();
};

#endif // !defined(AFX_N3IMesh_h__INCLUDED_)
