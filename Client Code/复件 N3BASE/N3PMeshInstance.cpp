// N3PMeshInstance.cpp: implementation of the CN3PMeshInstance class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3PMeshInstance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// an instance of a mesh. Each version of each mesh that is rendered at a 
// separate level of detail needs one of these.

// Each instance is tied to the original mesh that it represents.
CN3PMeshInstance::CN3PMeshInstance()
{
	m_pPMesh = NULL;
#ifdef _USE_VERTEXBUFFER
	m_pIB = NULL;
#else
	m_pIndices = NULL;
#endif
	m_iNumVertices = 0;
	m_iNumIndices  = 0;
	m_pCollapseUpTo = NULL;
}

CN3PMeshInstance::CN3PMeshInstance(CN3PMesh *pN3PMesh)
{
	m_pPMesh = NULL;

#ifdef _USE_VERTEXBUFFER
	m_pIB = NULL;
#else
	m_pIndices = NULL;
#endif
	m_iNumVertices = 0;
	m_iNumIndices  = 0;
	m_pCollapseUpTo = NULL;

	CN3PMeshInstance::Create(pN3PMesh);
}

int CN3PMeshInstance::Create(CN3PMesh* pN3PMesh)
{
	if(pN3PMesh == NULL)
	{
		CN3PMeshInstance::Release();
		return -1;
	}

	CN3PMesh* pOld = pN3PMesh;
	s_MngPMesh.Add(pN3PMesh);
	this->Release();
	pN3PMesh = pOld;
	
	m_pPMesh = pN3PMesh;

	this->NameSet(pN3PMesh->Name());

	// And setup my instance-specific data.
	// We start with the lowest level of detail.
	int iMaxNumIndices = m_pPMesh->GetMaxNumIndices();
	if (iMaxNumIndices>0)
	{
#ifdef _USE_VERTEXBUFFER
		HRESULT hr = s_lpD3DDev->CreateIndexBuffer(m_pPMesh->m_iMaxNumIndices*sizeof(WORD),
									D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
		__ASSERT(SUCCEEDED(hr), "Failed to create index Buffer");

		BYTE* pDestByte, *pSrcByte;
		hr = m_pIB->Lock(0, 0, &pDestByte, 0);
		hr = m_pPMesh->m_pIB->Lock(0, 0, &pSrcByte, 0);

		CopyMemory(pDestByte, pSrcByte, m_pPMesh->m_iMaxNumIndices * sizeof(WORD));
		m_pPMesh->m_pIB->Unlock();
		m_pIB->Unlock();

#else
		m_pIndices = new WORD[m_pPMesh->m_iMaxNumIndices];
		__ASSERT(m_pIndices, "Failed to create index buffer");
		CopyMemory(m_pIndices, m_pPMesh->m_pIndices, m_pPMesh->m_iMaxNumIndices * sizeof(WORD));
#endif
	}

	// lowest level of detail right now.
	m_iNumVertices = m_pPMesh->GetMinNumVertices();
	m_iNumIndices  = m_pPMesh->GetMinNumIndices();

	// So far, we're at the last collapse
	m_pCollapseUpTo = m_pPMesh->m_pCollapses;

	return true;
}

CN3PMeshInstance::~CN3PMeshInstance()
{
	CN3PMeshInstance::Release();
}

void CN3PMeshInstance::Release()
{
#ifdef _USE_VERTEXBUFFER
	if (m_pIB) {m_pIB->Release(); m_pIB = NULL;}
#else
	if (m_pIndices)	{ delete[] m_pIndices;m_pIndices = NULL;}
#endif

	if(m_pPMesh) s_MngPMesh.Delete(m_pPMesh);
	m_pPMesh = NULL;
	m_pCollapseUpTo = NULL;
	m_iNumVertices = 0;
	m_iNumIndices = 0;
}


void CN3PMeshInstance::SetLODByNumVertices(int iNumVertices)
{
	if(m_pCollapseUpTo == NULL) return;

	int iDiff = iNumVertices - m_iNumVertices;

	if(iDiff == 0)
	{
		return;
	}
	else if (iDiff > 0)
	{
		while(iNumVertices > m_iNumVertices)
		{
			if (m_pCollapseUpTo->NumVerticesToLose + m_iNumVertices > iNumVertices) break;		// 깜박임 방지 코드..
			if (SplitOne() == false) break;
		}
	}
	else if (iDiff < 0)
	{
		iDiff = -iDiff;
		while(iNumVertices < m_iNumVertices)
		{
			if (CollapseOne() == false) break;
		}
	}

	while(m_pCollapseUpTo->bShouldCollapse)
	{
		if (SplitOne() == false) break;
	}
}

void CN3PMeshInstance::SetLOD(float value)
{
#define _USE_LODCONTROL_VALUE
#ifdef _USE_LODCONTROL_VALUE
	// value는 distance * FOV이다.
	if (m_pPMesh == NULL ) return;

	if (m_pPMesh->m_iLODCtrlValueCount == 0)
	{	// LODCtrlValue가 없으면 모두 그린다.
		SetLODByNumVertices(0x7fffffff);
		return;
	}
	
	__ASSERT(m_pPMesh->m_pLODCtrlValues, "LOD control value is NULL!");

	CN3PMesh::__LODCtrlValue* pTmpLODCV = m_pPMesh->m_pLODCtrlValues + m_pPMesh->m_iLODCtrlValueCount-1;

	if (value < m_pPMesh->m_pLODCtrlValues[0].fDist)
	{		// 최소 기준치보다 가까우므로 가장 많은 면으로 그린다.
		SetLODByNumVertices(m_pPMesh->m_pLODCtrlValues[0].iNumVertices);
	}
	else if ( pTmpLODCV->fDist < value)
	{		// 최대 기준치보다 멀리 있으므로 가장 적은 면으로 그린다.
		SetLODByNumVertices(pTmpLODCV->iNumVertices);
	}
	else
	{		// 중간 값에 맞게 조정된 면 수로 그린다.
		for (int i=1; i< m_pPMesh->m_iLODCtrlValueCount; ++i)
		{
			if (value < m_pPMesh->m_pLODCtrlValues[i].fDist)
			{
				CN3PMesh::__LODCtrlValue* pHiValue = m_pPMesh->m_pLODCtrlValues + i;
				CN3PMesh::__LODCtrlValue* pLowValue = pHiValue - 1;
				float fVertices = (pHiValue->iNumVertices - pLowValue->iNumVertices)*
									(value - pLowValue->fDist)/(pHiValue->fDist - pLowValue->fDist);
				SetLODByNumVertices(pLowValue->iNumVertices + (int)fVertices);
				break;
			}
		}
	}
#else
	// value는 distance * FOV이다.
	if (m_pCollapseUpTo == NULL || m_pPMesh == NULL) return;

	const int iLODCtrlValueCount = 5;
	__PMLODCtrlValue LODCtrlValues[iLODCtrlValueCount];
	__PMLODCtrlValue* pTmpLODCV = &(LODCtrlValues[iLODCtrlValueCount-1]);

	int iMaxNumVertices = m_pPMesh->GetMaxNumVertices();
	int iMinNumVertices = m_pPMesh->GetMinNumVertices();
	int iDiff = iMaxNumVertices - iMinNumVertices;
	
	float fVolume = m_pPMesh->GetVolume();
	float fD = (sqrtf(fVolume)*3.0f) / (value * 1.0f);
	fD = 1.0f;
//	float fD = fVolume/(value*13.0f) * (400.0f/(float)iMaxNumVertices);
	if(fD > 1.0f) SetLODByNumVertices(iMaxNumVertices);
	else if(fD < 0.1f) SetLODByNumVertices(iMinNumVertices);
	else SetLODByNumVertices(iMinNumVertices + (int)(iDiff * fD));

#endif
}

bool CN3PMeshInstance::CollapseOne()
{
	if (m_pCollapseUpTo <= m_pPMesh->m_pCollapses) return false;

	m_pCollapseUpTo--;

	m_iNumIndices -= m_pCollapseUpTo->NumIndicesToLose;
	
#ifdef _USE_VERTEXBUFFER
	BYTE* pByte;
	WORD* pIndices;
	HRESULT hr = m_pIB->Lock(0, 0, &pByte, 0);
	pIndices = (WORD*)pByte;

	for (	int *i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
			i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
			i++)
	{
		pIndices[*i] = m_pCollapseUpTo->CollapseTo;
	}
	m_pIB->Unlock();
#else
	for (	int *i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
			i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
			i++)
	{
		m_pIndices[*i] = m_pCollapseUpTo->CollapseTo;
	}
#endif

	m_iNumVertices -= m_pCollapseUpTo->NumVerticesToLose;

	return true;
}

bool CN3PMeshInstance::SplitOne()
{
	if (m_pCollapseUpTo >= m_pPMesh->m_pCollapses + m_pPMesh->m_iNumCollapses) return false;

	m_iNumIndices  += m_pCollapseUpTo->NumIndicesToLose;
	m_iNumVertices += m_pCollapseUpTo->NumVerticesToLose;

#ifdef _USE_VERTEXBUFFER
	BYTE* pByte;
	WORD* pIndices;
	__ASSERT(m_pIB, "Index buffer pointer is NULL!");
	HRESULT hr = m_pIB->Lock(0, 0, &pByte, 0);
	pIndices = (WORD*)pByte;

	for (	int *i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
			i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
			i++)
	{
		pIndices[*i] = m_iNumVertices - 1;
	}
	m_pIB->Unlock();
#else
	for (	int *i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
			i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
			i++)
	{
		m_pIndices[*i] = m_iNumVertices - 1;
	}
#endif

	m_pCollapseUpTo++;

	return true;
}

void CN3PMeshInstance::Render()
{
	if (m_pPMesh == NULL) return;
	s_lpD3DDev->SetVertexShader(FVF_VNT1);

	const int iPCToRender = 1000;	// primitive count to render
#ifdef _USE_VERTEXBUFFER
	__ASSERT(m_pPMesh->m_pVB && m_pIB, "Progressive mesh's vertex buffer or index buffer is NULL!");
	s_lpD3DDev->SetStreamSource(0, m_pPMesh->m_pVB, sizeof(__VertexT1));
	s_lpD3DDev->SetIndices(m_pIB, 0);

	if(m_iNumIndices > 3)
	{
		int iPC = m_iNumIndices / 3;
		CN3Base::s_RenderInfo.nPolygon += iPC;

		int iLC = iPC / iPCToRender;
		int i;
		for (i=0; i<iLC; ++i)
		{
			s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_iNumVertices, i*iPCToRender*3, iPCToRender);
		}

		int iRPC = iPC%iPCToRender;
		if(iRPC > 0) s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_iNumVertices, i*iPCToRender*3, iRPC);
	}
#else
	if(m_iNumIndices > 3)
	{
		int iPC = m_iNumIndices / 3;
		CN3Base::s_RenderInfo.nPolygon += iPC;

		int iLC = iPC / iPCToRender;
		int i;
		for (i=0; i<iLC; ++i)
		{
			s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iPCToRender, m_pIndices + i*iPCToRender*3, D3DFMT_INDEX16, m_pPMesh->m_pVertices, sizeof(__VertexT1));
		}

		int iRPC = iPC%iPCToRender;
		if(iRPC > 0) s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iRPC, m_pIndices + i*iPCToRender*3, D3DFMT_INDEX16, m_pPMesh->m_pVertices, sizeof(__VertexT1));
	}
#endif
}

#ifdef _USE_VERTEXBUFFER
LPDIRECT3DVERTEXBUFFER8	CN3PMeshInstance::GetVertexBuffer() const
{
	if (m_pPMesh == NULL) return NULL;
	return m_pPMesh->GetVertexBuffer();
}

#else
__VertexT1*	CN3PMeshInstance::GetVertices() const
{
	if (m_pPMesh == NULL) return NULL;
	return m_pPMesh->m_pVertices;
}
#endif
