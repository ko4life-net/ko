// N3PMeshInstance.h: interface for the CN3PMeshInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3PMeshInstance_h__INCLUDED_)
#define AFX_N3PMeshInstance_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3PMesh.h"

class CN3PMesh;
struct CN3PMesh::__EdgeCollapse;

// Every instance of the mesh in the game needs one of these structures. They
// encode the level of detail being used to render at the moment.
class CN3PMeshInstance : public CN3Base  
{
public:
	CN3PMeshInstance();
	CN3PMeshInstance(CN3PMesh *pN3PMesh);
	virtual ~CN3PMeshInstance();

protected:
#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#else
	WORD* m_pIndices;
#endif

	int m_iNumIndices, m_iNumVertices; // number of triangles/vertices to use at this moment.

	CN3PMesh::__EdgeCollapse*	m_pCollapseUpTo;	// 참조 포인터

	// A pointer to the original mesh. This is where you'll find the vertex data (which
	// is invariant between PMRInstance's)
	CN3PMesh *m_pPMesh;	// 참조 포인터

public:
	BOOL IsLOD() { if(NULL == m_pCollapseUpTo) return FALSE; return TRUE; }
	int Create(CN3PMesh* pN3Mesh);
	void Release();
	void Render();
	// The value you pass into this could be based on distance from camera, frame rate, etc
	void SetLOD(float value); // value = Camera Distance / Camera FOV;

	// Or you can choose to collapse/split one step at a time. These return false if there was
	// no more collapsing/splitting to do.
	bool CollapseOne();
	bool    SplitOne();

	void SetLODByNumVertices(int iNumVertices);
	int GetNumVertices() const {return m_iNumVertices;};
	int GetNumIndices() const {return m_iNumIndices;};
	CN3PMesh* GetMesh() {return m_pPMesh;};

#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DVERTEXBUFFER8	GetVertexBuffer() const;
	LPDIRECT3DINDEXBUFFER8 GetIndexBuffer() const {return m_pIB;}
#else
	__VertexT1*		GetVertices() const;
	WORD*			GetIndices() const { return m_pIndices;};
#endif
};

#endif // !defined(AFX_N3PMeshInstance_h__INCLUDED_)
