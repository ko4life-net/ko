// N3PMesh.cpp: implementation of the CN3PMesh class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3PMesh.h"
#include "N3PMeshInstance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3PMesh::CN3PMesh()
{
	m_dwType |= OBJ_MESH_PROGRESSIVE;

	m_pCollapses = NULL;
	m_pAllIndexChanges = NULL;

#ifdef _USE_VERTEXBUFFER
	m_pVB = NULL;
	m_pIB = NULL;
#else
	m_pVertices = NULL;
	m_pIndices = NULL;
#endif
	m_pLODCtrlValues = NULL;

	m_pWeaponTrack = NULL; // 무기의 궤적등...

	Release();
}

CN3PMesh::~CN3PMesh()
{
	CN3PMesh::Release();
}

void CN3PMesh::Release()
{

#ifdef _USE_VERTEXBUFFER
	if (m_pVB) { m_pVB->Release(); m_pVB = NULL;}
	if (m_pIB) { m_pIB->Release(); m_pIB = NULL;}
#else
	if (m_pVertices) { delete[] m_pVertices; m_pVertices = NULL;}
	if (m_pIndices) { delete[] m_pIndices; m_pIndices = NULL;}
#endif
	if (m_pCollapses) {	delete[] m_pCollapses; m_pCollapses = NULL;}
	if (m_pAllIndexChanges)	{ delete[] m_pAllIndexChanges; m_pAllIndexChanges = NULL;}
	if (m_pLODCtrlValues) {	delete [] m_pLODCtrlValues;	m_pLODCtrlValues = NULL;}

	m_iNumCollapses = 0;
	m_iTotalIndexChanges = 0;
	m_iMaxNumVertices = 0; m_iMaxNumIndices = 0;
	m_iMinNumVertices = 0; m_iMinNumIndices = 0;
	m_iLODCtrlValueCount = 0;

	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);
	m_fVolume = 0.0f;

	delete m_pWeaponTrack; m_pWeaponTrack = NULL;
}

bool CN3PMesh::Load(HANDLE hFile)
{
	CN3Base::Load(hFile);

	DWORD dwNum;
	ReadFile(hFile, &m_iNumCollapses, sizeof(m_iNumCollapses), &dwNum, NULL);
	ReadFile(hFile, &m_iTotalIndexChanges, sizeof(m_iTotalIndexChanges), &dwNum, NULL);

	ReadFile(hFile, &m_iMaxNumVertices, sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMaxNumIndices , sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMinNumVertices, sizeof(int), &dwNum, NULL);
	ReadFile(hFile, &m_iMinNumIndices , sizeof(int), &dwNum, NULL);

	HRESULT hr = Create(m_iMaxNumVertices, m_iMaxNumIndices);
	__ASSERT(SUCCEEDED(hr), "Failed to create progressive mesh");
#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
//		HRESULT hr = m_lpD3DDev->CreateVertexBuffer(m_iMaxNumVertices*sizeof(__VertexT1),
//										D3DUSAGE_DYNAMIC, FVF_VNT1, D3DPOOL_MANAGED, &m_pVB);
//		if (FAILED(hr)) return hr;
		BYTE* pByte;
		hr = m_pVB->Lock(0, 0, &pByte, 0);
		if (FAILED(hr)) return false;
		ReadFile(hFile, pByte, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
//		HRESULT hr = m_lpD3DDev->CreateIndexBuffer(m_iMaxNumIndices*sizeof(WORD),
//										D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
//		if (FAILED(hr)) return hr;
		BYTE* pByte;
		hr = m_pIB->Lock(0, 0, &pByte, 0);
		if (FAILED(hr)) return false;
		ReadFile(hFile, pByte, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0)
	{
//		m_pVertices = new __VertexT1[m_iMaxNumVertices];
		ReadFile(hFile, m_pVertices, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
	}

	if (m_iMaxNumIndices>0)
	{
//		m_pIndices = new WORD[m_iMaxNumIndices];
		ReadFile(hFile, m_pIndices, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
	}
#endif

	if (m_iNumCollapses>0)
	{
		m_pCollapses = new __EdgeCollapse[m_iNumCollapses];
		ReadFile(hFile, m_pCollapses, m_iNumCollapses*sizeof(__EdgeCollapse), &dwNum, NULL);
	}
	if (m_iTotalIndexChanges>0)
	{
		m_pAllIndexChanges = new int[m_iTotalIndexChanges];
		ReadFile(hFile, m_pAllIndexChanges, m_iTotalIndexChanges*sizeof(int), &dwNum, NULL);
	}

	__ASSERT(m_pLODCtrlValues == NULL && m_iLODCtrlValueCount == 0, "Invalid Level of detail control value");
	ReadFile(hFile, &m_iLODCtrlValueCount, sizeof(m_iLODCtrlValueCount), &dwNum, NULL);
	if (m_iLODCtrlValueCount>0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
		ReadFile(hFile, m_pLODCtrlValues, m_iLODCtrlValueCount*sizeof(__LODCtrlValue), &dwNum, NULL);
	}

	BOOL bHaveWeaponTrack = FALSE;
	ReadFile(hFile, &bHaveWeaponTrack, 4, &dwNum, NULL); // 궤적이 있는지...
	if(bHaveWeaponTrack)
	{
		if(NULL == m_pWeaponTrack) m_pWeaponTrack = new __WeaponTrack;
		ReadFile(hFile, m_pWeaponTrack, sizeof(__WeaponTrack), &dwNum, NULL); // 궤적 데이터..
	}
	
	FindMinMax();

	return true;
}

bool CN3PMesh::Save(HANDLE hFile)
{
	CN3Base::Save(hFile);

	DWORD dwNum;
	WriteFile(hFile, &m_iNumCollapses, sizeof(m_iNumCollapses), &dwNum, NULL);
	WriteFile(hFile, &m_iTotalIndexChanges, sizeof(m_iTotalIndexChanges), &dwNum, NULL);

	WriteFile(hFile, &(m_iMaxNumVertices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMaxNumIndices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMinNumVertices), sizeof(int), &dwNum, NULL);
	WriteFile(hFile, &(m_iMinNumIndices), sizeof(int), &dwNum, NULL);

#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
		HRESULT hr;
		BYTE* pByte;
		hr = m_pVB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
		if (FAILED(hr)) return false;
		WriteFile(hFile, pByte, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
		HRESULT hr;
		BYTE* pByte;
		hr = m_pIB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
		if (FAILED(hr)) return false;
		WriteFile(hFile, pByte, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0) WriteFile(hFile, m_pVertices, m_iMaxNumVertices*sizeof(__VertexT1), &dwNum, NULL);
	if (m_iMaxNumIndices>0) WriteFile(hFile, m_pIndices, m_iMaxNumIndices*sizeof(WORD), &dwNum, NULL);
#endif

	if (m_iNumCollapses>0) WriteFile(hFile, m_pCollapses, m_iNumCollapses*sizeof(__EdgeCollapse), &dwNum, NULL);
	if (m_iTotalIndexChanges>0) WriteFile(hFile, m_pAllIndexChanges, m_iTotalIndexChanges*sizeof(m_pAllIndexChanges[0]), &dwNum, NULL);

	WriteFile(hFile, &m_iLODCtrlValueCount, sizeof(m_iLODCtrlValueCount), &dwNum, NULL);
	if (m_iLODCtrlValueCount>0) WriteFile(hFile, m_pLODCtrlValues, m_iLODCtrlValueCount*sizeof(__LODCtrlValue), &dwNum, NULL);

	BOOL bHaveWeaponTrack = FALSE;
	if(m_pWeaponTrack) bHaveWeaponTrack = TRUE;
	WriteFile(hFile, &bHaveWeaponTrack, 4, &dwNum, NULL); // 궤적이 있는지...
	if(bHaveWeaponTrack)
	{
		WriteFile(hFile, m_pWeaponTrack, sizeof(__WeaponTrack), &dwNum, NULL); // 궤적 데이터..
	}

	return true;
}

void CN3PMesh::FindMinMax()
{
	// 최소, 최대 점을 찾는다.
	m_vMin.Set(1000000.0f, 1000000.0f, 1000000.0f);
	m_vMax.Set(-1000000.0f, -1000000.0f, -1000000.0f);

#ifdef _USE_VERTEXBUFFER
	__ASSERT(m_pVB && m_pIB, "Vertex buffer is NULL or Index buffer is NULL!");
	HRESULT hr;
	BYTE* pByte;
	hr = m_pVB->Lock(0, 0, &pByte, D3DLOCK_READONLY);
	__VertexT1* pVertices = (__VertexT1*)pByte;
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		if(pVertices[i].x < m_vMin.x) m_vMin.x = pVertices[i].x;
		if(pVertices[i].y < m_vMin.y) m_vMin.y = pVertices[i].y;
		if(pVertices[i].z < m_vMin.z) m_vMin.z = pVertices[i].z;
		if(pVertices[i].x > m_vMax.x) m_vMax.x = pVertices[i].x;
		if(pVertices[i].y > m_vMax.y) m_vMax.y = pVertices[i].y;
		if(pVertices[i].z > m_vMax.z) m_vMax.z = pVertices[i].z;
	}
	m_pVB->Unlock();
#else
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		if(m_pVertices[i].x < m_vMin.x) m_vMin.x = m_pVertices[i].x;
		if(m_pVertices[i].y < m_vMin.y) m_vMin.y = m_pVertices[i].y;
		if(m_pVertices[i].z < m_vMin.z) m_vMin.z = m_pVertices[i].z;

		if(m_pVertices[i].x > m_vMax.x) m_vMax.x = m_pVertices[i].x;
		if(m_pVertices[i].y > m_vMax.y) m_vMax.y = m_pVertices[i].y;
		if(m_pVertices[i].z > m_vMax.z) m_vMax.z = m_pVertices[i].z;
	}
#endif
	// 부피 계산
	__Vector3 vDiff = m_vMax - m_vMin;
	m_fVolume = vDiff.x * vDiff.y + vDiff.y * vDiff.z + vDiff.z * vDiff.x;
}

void CN3PMesh::CopyMesh(CN3PMesh* pSrcPMesh)
{
	Release();

	HRESULT hr;
	
	__ASSERT(pSrcPMesh, "Source progressive mesh pointer is NULL");
	
	m_iNumCollapses			= pSrcPMesh->m_iNumCollapses;
	m_iTotalIndexChanges	= pSrcPMesh->m_iTotalIndexChanges;
	m_iMaxNumVertices		= pSrcPMesh->m_iMaxNumVertices;
	m_iMaxNumIndices		= pSrcPMesh->m_iMaxNumIndices;
	m_iMinNumVertices		= pSrcPMesh->m_iMinNumVertices;
	m_iMinNumIndices		= pSrcPMesh->m_iMinNumIndices;
	m_vMin					= pSrcPMesh->m_vMin;
	m_vMax					= pSrcPMesh->m_vMax;
	m_fVolume				= pSrcPMesh->m_fVolume;
	m_iLODCtrlValueCount	= pSrcPMesh->m_iLODCtrlValueCount;

	if (m_iTotalIndexChanges>0)
	{
		m_pAllIndexChanges = new int[m_iTotalIndexChanges];

		CopyMemory(m_pAllIndexChanges, pSrcPMesh->m_pAllIndexChanges, sizeof(int)*m_iTotalIndexChanges);
	}

	if (m_iNumCollapses>0)
	{
		m_pCollapses = new __EdgeCollapse[m_iNumCollapses];
		CopyMemory(m_pCollapses, pSrcPMesh->m_pCollapses, sizeof(__EdgeCollapse)*m_iNumCollapses);
	}

	hr = Create(m_iMaxNumVertices, m_iMaxNumIndices);
	__ASSERT(SUCCEEDED(hr), "Failed to create progressive mesh");
#ifdef _USE_VERTEXBUFFER
	if (m_iMaxNumVertices>0)
	{
		BYTE* pDestByte, *pSrcByte;
		hr = m_pVB->Lock(0, 0, &pDestByte, 0);
		hr = pSrcPMesh->m_pVB->Lock(0, 0, &pSrcByte, D3DLOCK_READONLY);
		CopyMemory(pDestByte, pSrcByte, m_iMaxNumVertices*sizeof(__VertexT1));
		hr = pSrcPMesh->m_pVB->Unlock();
		m_pVB->Unlock();
	}
	if (m_iMaxNumIndices>0)
	{
		BYTE* pDestByte, *pSrcByte;
		hr = m_pIB->Lock(0, 0, &pDestByte, 0);
		hr = pSrcPMesh->m_pIB->Lock(0, 0, &pSrcByte, D3DLOCK_READONLY);
		CopyMemory(pDestByte, pSrcByte, m_iMaxNumIndices*sizeof(WORD));
		pSrcPMesh->m_pIB->Unlock();
		m_pIB->Unlock();
	}
#else
	if (m_iMaxNumVertices>0)
	{
//		m_pVertices = new __VertexT1[m_iMaxNumVertices];
		CopyMemory(m_pVertices, pSrcPMesh->m_pVertices, sizeof(__VertexT1)*m_iMaxNumVertices);
	}

	if (m_iMaxNumIndices>0)
	{
//		m_pIndices = new WORD[m_iMaxNumIndices];
		CopyMemory(m_pIndices, pSrcPMesh->m_pIndices, sizeof(WORD)*m_iMaxNumIndices);
	}
#endif
	if (m_iLODCtrlValueCount>0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
		CopyMemory(m_pLODCtrlValues, pSrcPMesh->m_pLODCtrlValues, sizeof(__LODCtrlValue)*m_iLODCtrlValueCount);
	}

	NameSet(pSrcPMesh->Name());
}

HRESULT CN3PMesh::Create(int iNumVertices, int iNumIndices)
{
	m_iMaxNumVertices = iNumVertices;
	m_iMaxNumIndices = iNumIndices;

#ifdef _USE_VERTEXBUFFER
	HRESULT hr;
	if (m_pVB){ m_pVB->Release(); m_pVB = NULL;}
	if (m_pIB){ m_pIB->Release(); m_pIB = NULL;}

	if (m_iMaxNumVertices>0)
	{
		hr = m_lpD3DDev->CreateVertexBuffer(m_iMaxNumVertices*sizeof(__VertexT1),
										D3DUSAGE_DYNAMIC, FVF_VNT1, D3DPOOL_MANAGED, &m_pVB);
		if (FAILED(hr)) return hr;
	}

	if (m_iMaxNumIndices>0)
	{
		hr = m_lpD3DDev->CreateIndexBuffer(m_iMaxNumIndices*sizeof(WORD),
										D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
		if (FAILED(hr)) return hr;
	}
#else
	if (m_pVertices) {delete m_pVertices; m_pVertices = NULL;}
	if (m_pIndices) {delete m_pIndices; m_pIndices = NULL;}

	if (m_iMaxNumVertices>0)
	{
		m_pVertices = new __VertexT1[m_iMaxNumVertices];
	}
	if (m_iMaxNumIndices>0)
	{
		m_pIndices = new WORD[m_iMaxNumIndices];
	}
#endif
	return S_OK;
}

void CN3PMesh::LODCtrlSet(__LODCtrlValue *pLODCtrls, int nCount)
{
	m_iLODCtrlValueCount = 0;
	delete [] m_pLODCtrlValues; m_pLODCtrlValues = NULL;
	if(NULL == pLODCtrls || nCount <= 0) return;

	m_iLODCtrlValueCount = nCount;
	if(nCount > 0)
	{
		m_pLODCtrlValues = new __LODCtrlValue[nCount];
		memcpy(m_pLODCtrlValues, pLODCtrls, sizeof(__LODCtrlValue) * nCount);

		// 거리에 따라 정렬
		qsort(m_pLODCtrlValues, m_iLODCtrlValueCount, sizeof(__LODCtrlValue), SortByDistance);
	}
}


int CN3PMesh::SortByDistance(const void *pArg1, const void *pArg2)
{
	__LODCtrlValue *pObj1 = (__LODCtrlValue*)pArg1;
	__LODCtrlValue *pObj2 = (__LODCtrlValue*)pArg2;

	if(pObj1->fDist < pObj2->fDist) return -1;
	else if(pObj1->fDist > pObj2->fDist) return 1;
	else return 0;
}


void CN3PMesh::CreateLODByDefault()
{
	///////////////////////////////////////////////////////
	// tigger 기본적인 LOD 값을 넣는다..
	float fMaxDist = s_CameraData.fFP * s_CameraData.fFOV;
	if(m_iMaxNumVertices <= 0 || fMaxDist < 0.0f) return;

	this->FindMinMax();
	float fRad = (m_vMax - m_vMin).Magnitude() * 1.5f;

	CN3PMesh::__LODCtrlValue LODs[2];
	int iVC = m_iMaxNumVertices - m_iMinNumVertices;
	
	LODs[0].fDist = fRad;
	LODs[0].iNumVertices = m_iMaxNumVertices;

	LODs[1].fDist = fMaxDist; // + (m_vMax - m_vMin).Magnitude()/3.0f);
	LODs[1].iNumVertices = m_iMinNumVertices;

	this->LODCtrlSet(LODs, 2);
	// tigger 기본적인 LOD 값을 넣는다..
	///////////////////////////////////////////////////////

}

void CN3PMesh::ReGenerateSmoothNormal()
{
	if(m_iMaxNumVertices <= 0) return;

	CN3PMeshInstance PMI(this);
	PMI.SetLODByNumVertices(m_iMaxNumVertices); // 최대 점으로 세팅하고..
	int nIC = PMI.GetNumIndices(); // Index Count...
	WORD* pwIndices = PMI.GetIndices(); // Index ...

	int* pnNs = new int[m_iMaxNumVertices];
	memset(pnNs, 0, 4 * m_iMaxNumVertices);
	__Vector3* pvNs = new __Vector3[m_iMaxNumVertices];
	memset(pvNs, 0, sizeof(__Vector3)*m_iMaxNumVertices);

	int nFC = nIC / 3;

	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int i = 0; i < m_iMaxNumVertices; i++)
	{
		for(int j = 0; j < nFC; j++)
		{
			v0 = m_pVertices[pwIndices[j*3+0]].v;
			v1 = m_pVertices[pwIndices[j*3+1]].v;
			v2 = m_pVertices[pwIndices[j*3+2]].v;

			if(	(m_pVertices[i].x == v0.x && m_pVertices[i].y == v0.y && m_pVertices[i].z == v0.z) ||
				(m_pVertices[i].x == v1.x && m_pVertices[i].y == v1.y && m_pVertices[i].z == v1.z) ||
				(m_pVertices[i].x == v2.x && m_pVertices[i].y == v2.y && m_pVertices[i].z == v2.z) )
			{
				vN.Cross(v1 - v0, v2 - v1); // Normal 값을 계산하고...
				vN.Normalize();

				pnNs[i]++;
				pvNs[i] += vN;
			}
		}

		m_pVertices[i].n = pvNs[i] / (float)pnNs[i];
	}

	delete [] pnNs;
	delete [] pvNs;
}

void CN3PMesh::ReGenerateSharpNormal()
{
	if(m_iMaxNumVertices <= 0) return;

	CN3PMeshInstance PMI(this);
	PMI.SetLODByNumVertices(m_iMaxNumVertices); // 최대 점으로 세팅하고..
	int nIC = PMI.GetNumIndices(); // Index Count...
	WORD* pwIndices = PMI.GetIndices(); // Index ...

	int nFC = nIC / 3;
	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int j = 0; j < nFC; j++)
	{
		v0 = m_pVertices[pwIndices[j*3+0]].v;
		v1 = m_pVertices[pwIndices[j*3+1]].v;
		v2 = m_pVertices[pwIndices[j*3+2]].v;

		vN.Cross(v1 - v0, v2 - v1); // Normal 값을 계산하고...
		vN.Normalize();

		m_pVertices[pwIndices[j*3+0]].n = vN;
		m_pVertices[pwIndices[j*3+1]].n = vN;
		m_pVertices[pwIndices[j*3+2]].n = vN;
	}

}
