// N3PMesh.h: interface for the CN3PMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3PMesh_h__INCLUDED_)
#define AFX_N3PMesh_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _USE_VERTEXBUFFER			// 선언하면 VertexBuffer와 IndexBuffer를 사용한다.

#include "N3Base.h"

class CN3PMesh : public CN3Base
{
friend class CN3PMeshInstance;
friend class CN3PMeshCreate;
friend class CPMeshEditor;			// N3Viewer의 클래스

public:
	struct  __LODCtrlValue
	{
		float	fDist;
		int		iNumVertices;
	};

	// A single edge collapse / vertex split operation.
	struct __EdgeCollapse
	{
	//	float Value;
		int NumIndicesToLose, NumIndicesToChange, NumVerticesToLose;
		//int *IndexChanges;
		int iIndexChanges;	// 포인터 대신 n번째 저장 (0, 1, 2,...)
		int CollapseTo;
		bool	bShouldCollapse;	// 여기서 병합/나누기를 멈추면 구멍이 뚤린다. 다음단계를 더 진행해야함.
	};

	struct __WeaponTrack
	{
		__Vector3	m_vTrack0; // 궤적 0 ... 무기의 검기(?) 를 표현할때 쓴다..
		__Vector3	m_vTrack1; // 궤적 1 ...
		D3DCOLOR 	m_crTrack; // 궤적 색상..
		float		m_fTrackTime; // 궤적 없어지는 시간..
	};

protected:
	__WeaponTrack* m_pWeaponTrack;

	//int m_iNumMaterials;
	int m_iNumCollapses;			// 삼각형 병합 리스트
	int m_iTotalIndexChanges;
	int *m_pAllIndexChanges; // All the index changes

	// The mesh is an array of materials, and an edge collapse list. The mesh
	// is saved in its lowest level of detail

	__EdgeCollapse	*m_pCollapses;

	// Mesh 정보
#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DVERTEXBUFFER8 m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#else
	__VertexT1*		m_pVertices;
	WORD*			m_pIndices;
#endif
	int m_iMaxNumVertices, m_iMaxNumIndices;
	int m_iMinNumVertices, m_iMinNumIndices;

	__Vector3	m_vMin, m_vMax; // 최대 최소값..
	float		m_fVolume;

	// LOD 조정 정보..(거리에 따라서 Vertex의 수를 조정할 수 있다.)
	int					m_iLODCtrlValueCount;
	__LODCtrlValue*		m_pLODCtrlValues;
protected:
	static int SortByDistance(const void *pArg1, const void *pArg2);

public:
	void ReGenerateSharpNormal();
	void ReGenerateSmoothNormal();
	void CreateLODByDefault();
	int LODCtrlCount() { return m_iLODCtrlValueCount; }
	void LODCtrlSet(__LODCtrlValue* pLODCtrls, int nCount);
	__LODCtrlValue* LODCtrlGet(int index) { if(index < 0 || index >= m_iLODCtrlValueCount) return NULL; return &m_pLODCtrlValues[index]; }

//	int					GetLODCtrlValueCount() const {return m_iLODCtrlValueCount;}
//	__PMLODCtrlValue*	GetLODCtrlValue(int iIndex) const {if (iIndex>=0&&m_iLODCtrlValueCount>iIndex)return m_pLODCtrlValues+iIndex; return NULL;}
//	void				SetLODCtrlValues(__PMLODCtrlValue* pValues) {m_pLODCtrlValues = pValues;}

	__Vector3 Min() { return m_vMin; }
	__Vector3 Max() { return m_vMax; }

	void Release();
	HRESULT Create(int iNumVertices, int iNumIndices);
	
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	void FindMinMax();

	float GetVolume() const {return m_fVolume;};
	int	GetMinNumVertices() const {return m_iMinNumVertices;};
	int	GetMaxNumVertices() const {return m_iMaxNumVertices;};
	int GetMaxNumIndices() const {return m_iMaxNumIndices;};
	int GetMinNumIndices() const {return m_iMinNumIndices;};

#ifdef _USE_VERTEXBUFFER
	LPDIRECT3DVERTEXBUFFER8	GetVertexBuffer() const {return m_pVB;}
	LPDIRECT3DINDEXBUFFER8	GetIndexBuffer() const {return m_pIB;}
#else
	__VertexT1*		GetVertices() const { return m_pVertices;};
	WORD*			GetIndices() const { return m_pIndices;};		// 제대로된 Index가 아님 
	// (제대로 된 인덱스를 얻으려면 N3PMeshInstance로 만든후 LOD조정후 인덱스값을 얻으면 된다.)
#endif

	void CopyMesh(CN3PMesh* pSrcPMesh);

	CN3PMesh();
	virtual ~CN3PMesh();

};

#endif // !defined(AFX_N3PMesh_h__INCLUDED_)
