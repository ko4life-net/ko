// N3Skin.cpp: implementation of the CN3Skin class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Skin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Skin::CN3Skin()
{
	m_dwType |= OBJ_SKIN;

	m_pVertices = NULL;
	this->Release();
}

CN3Skin::~CN3Skin()
{
	CN3Skin::Release();
}

void CN3Skin::Release()
{
	m_nVertexCount = 0;
	delete [] m_pVertices; m_pVertices = NULL;
}

bool CN3Skin::Load(HANDLE hFile)
{
	DWORD dwRWC = 0;
	ReadFile(hFile, &m_nVertexCount, 4, &dwRWC, NULL); // 헤더 읽고..
	
	this->Alloc(m_nVertexCount);

	for(int i = 0; i < m_nVertexCount; i++)
	{
		ReadFile(hFile, &m_pVertices[i], sizeof(__VertexSkinned), &dwRWC, NULL);
		m_pVertices[i].pnJoints = NULL;
		m_pVertices[i].pfWeights = NULL;
		
		int nAffect = m_pVertices[i].nAffect;
		if(nAffect > 1)
		{
			m_pVertices[i].pnJoints = new int[nAffect];
			m_pVertices[i].pfWeights = new float[nAffect];

			ReadFile(hFile, m_pVertices[i].pnJoints, 4 * nAffect, &dwRWC, NULL);
			ReadFile(hFile, m_pVertices[i].pfWeights, 4 * nAffect, &dwRWC, NULL);
		}
		else if(nAffect == 1)
		{
			m_pVertices[i].pnJoints = new int[1];
			ReadFile(hFile, m_pVertices[i].pnJoints, 4, &dwRWC, NULL);
		}
	}

	return true;
}

bool CN3Skin::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;
	WriteFile(hFile, &m_nVertexCount, 4, &dwRWC, NULL); // 헤더 읽고..
	for(int i = 0; i < m_nVertexCount; i++)
	{
		WriteFile(hFile, &m_pVertices[i], sizeof(__VertexSkinned), &dwRWC, NULL);
		int nAffect = m_pVertices[i].nAffect;
		if(nAffect > 1)
		{
			WriteFile(hFile, m_pVertices[i].pnJoints, 4 * nAffect, &dwRWC, NULL);
			WriteFile(hFile, m_pVertices[i].pfWeights, 4 * nAffect, &dwRWC, NULL);
		}
		else if(nAffect == 1)
		{
			WriteFile(hFile, m_pVertices[i].pnJoints, 4, &dwRWC, NULL);
		}
	}

	return true;
}

void CN3Skin::Alloc(int nCount)
{
	if(nCount <= 0) return;

	if(m_pVertices) this->Release();
	m_pVertices = new __VertexSkinned[nCount]; 
	memset(m_pVertices, 0, sizeof(__VertexSkinned)*nCount);
	m_nVertexCount = nCount;
}

int CN3Skin::SortWeightsProc(const void *pArg1, const void *pArg2)
{
	__Weight* pW1 = (__Weight*)pArg1;
	__Weight* pW2 = (__Weight*)pArg2;

	if(pW1->fWeight < pW2->fWeight) return 1;
	else return -1;
}
