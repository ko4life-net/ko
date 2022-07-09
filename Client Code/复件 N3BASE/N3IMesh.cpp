// N3MeshIndexed: implementation of the CN3IMesh class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3IMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

__VertexT1 CN3IMesh::s_Vertices[65536];
//__VertexXyzT1 CN3IMesh::m_StaticVertices[32767];

CN3IMesh::CN3IMesh()
{
	m_dwType |= OBJ_MESH_INDEXED;

	m_nFC = 0;

	m_pwVtxIndices = NULL; // 점 인덱스 리스트. 
	m_pwUVsIndices = NULL; // 텍스처 좌표 인덱스 리스트.

	m_nVC = m_nUVC = 0;
	m_pVertices = NULL;
	m_pfUVs = NULL;
}

CN3IMesh::~CN3IMesh()
{
	CN3IMesh::Release();
}

void CN3IMesh::Release()
{
	m_nFC = 0;
	m_nVC = 0;
	m_nUVC = 0;

	delete [] m_pVertices; m_pVertices = NULL;
	delete [] m_pfUVs; m_pfUVs = NULL;

	delete [] m_pwVtxIndices; m_pwVtxIndices = NULL; // 점 인덱스 리스트. 
	delete [] m_pwUVsIndices; m_pwUVsIndices = NULL; // 텍스처 좌표 인덱스 리스트.

	m_vMin.Zero();
	m_vMax.Zero();
}

void CN3IMesh::Create(int nFC, int nVC, int nUVC)
{
	if(nFC <= 0 || nVC <= 0) return;
	if(m_nFC > 0) this->Release();

	m_nFC = nFC;
	m_nVC = nVC;
	m_pVertices = new __VertexXyzNormal[nVC]; memset(m_pVertices, 0, sizeof(__VertexXyzNormal) * nVC);
	m_pwVtxIndices = new WORD[nFC*3]; memset(m_pwVtxIndices, 0, 2 * nFC * 3); // unsigned short
	
	if(nUVC > 0)
	{
		m_nUVC = nUVC; m_pfUVs = new float[nUVC*2]; memset(m_pfUVs, 0, 8 * nUVC); // 사이즈가 8 인 이유는 float 2개라 그렇다..
		m_pwUVsIndices = new WORD[nFC*3]; memset(m_pwUVsIndices, 0, 2 * nFC * 3); // unsigned short
	} 
}

__VertexT1* CN3IMesh::BuildVertexList()
{
	int n = 0, nVI = 0, nUVI = 0;
	if(m_nUVC > 0)
	{
		for(int i = 0; i < m_nFC; i++)
		{
			n = i*3+0; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1]);
			
			n = i*3+1; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1]);

			n = i*3+2; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1]);
		}
	}
	else
	{
		for(int i = 0; i < m_nFC; i++)
		{
			n = i*3+0; nVI = m_pwVtxIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, 0, 0);
			
			n = i*3+1; nVI = m_pwVtxIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, 0, 0);

			n = i*3+2; nVI = m_pwVtxIndices[n];
			s_Vertices[n].Set(m_pVertices[nVI].v, m_pVertices[nVI].n, 0, 0);
		}
	}

	return s_Vertices;
}

void CN3IMesh::Render()
{
	if(s_lpD3DDev == NULL || m_nFC <= 0) return;

	this->BuildVertexList();
	
	s_lpD3DDev->SetVertexShader(FVF_VNT1);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_nFC, s_Vertices, sizeof(__VertexT1));

	CN3Base::s_RenderInfo.nPolygonCharacter += m_nFC;
}

bool CN3IMesh::Load(HANDLE hFile)
{
	DWORD dwRWC = 0;
	
	int nFC = 0, nVC = 0, nUVC = 0;

	ReadFile(hFile, &nFC, 4, &dwRWC, NULL);
	ReadFile(hFile, &nVC, 4, &dwRWC, NULL);
	ReadFile(hFile, &nUVC, 4, &dwRWC, NULL);

	if(nFC > 0 && nVC > 0)
	{
		this->Create(nFC, nVC, nUVC);
		ReadFile(hFile, m_pVertices, sizeof(__VertexXyzNormal) * nVC, &dwRWC, NULL);
		ReadFile(hFile, m_pwVtxIndices, 2 * nFC * 3, &dwRWC, NULL); // unsigned short
	}
	else
	{
		Release();
	}
	
	if(m_nUVC > 0)
	{
		ReadFile(hFile, m_pfUVs, 8 * nUVC, &dwRWC, NULL);
		ReadFile(hFile, m_pwUVsIndices, 2 * nFC * 3, &dwRWC, NULL); // unsigned short
	}

	this->FindMinMax(); // 최소 최대값을 찾는다..

	return true;
}

bool CN3IMesh::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;
	
	WriteFile(hFile, &m_nFC, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_nVC, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_nUVC, 4, &dwRWC, NULL);

	if(m_nFC > 0 && m_nVC > 0)
	{
		WriteFile(hFile, m_pVertices, sizeof(__VertexXyzNormal) * m_nVC, &dwRWC, NULL);
		WriteFile(hFile, m_pwVtxIndices, 2 * m_nFC * 3, &dwRWC, NULL); // unsigned short
	}
	
	if(m_nUVC > 0)
	{
		WriteFile(hFile, m_pfUVs, 8 * m_nUVC, &dwRWC, NULL);
		WriteFile(hFile, m_pwUVsIndices, 2 * m_nFC * 3, &dwRWC, NULL); // unsigned short
	}

	return true;
}

void CN3IMesh::FindMinMax()
{
	if(m_pVertices == NULL) return;

	// 최소, 최대 점을 찾는다.
	if(m_nVC > 0)
	{
		m_vMin.Set(1000000.0f, 1000000.0f, 1000000.0f);
		m_vMax.Set(-1000000.0f, -1000000.0f, -1000000.0f);
		for(int i = 0; i < m_nVC; i++)
		{
			if(m_pVertices[i].x < m_vMin.x) m_vMin.x = m_pVertices[i].x;
			if(m_pVertices[i].y < m_vMin.y) m_vMin.y = m_pVertices[i].y;
			if(m_pVertices[i].z < m_vMin.z) m_vMin.z = m_pVertices[i].z;

			if(m_pVertices[i].x > m_vMax.x) m_vMax.x = m_pVertices[i].x;
			if(m_pVertices[i].y > m_vMax.y) m_vMax.y = m_pVertices[i].y;
			if(m_pVertices[i].z > m_vMax.z) m_vMax.z = m_pVertices[i].z;
		}
	}
	else
	{
		m_vMin.Zero();
		m_vMax.Zero();
	}
}

void CN3IMesh::ReGenerateSmoothNormal()
{
	if(m_nFC <= 0) return;

	this->BuildVertexList();

	int* pnNs = new int[m_nVC];
	memset(pnNs, 0, 4 * m_nVC);
	__Vector3* pvNs = new __Vector3[m_nVC];
	memset(pvNs, 0, sizeof(__Vector3)*m_nVC);

	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int i = 0; i < m_nVC; i++)
	{
		for(int j = 0; j < m_nFC; j++)
		{
			v0 = m_pVertices[m_pwVtxIndices[j*3+0]].v;
			v1 = m_pVertices[m_pwVtxIndices[j*3+1]].v;
			v2 = m_pVertices[m_pwVtxIndices[j*3+2]].v;

			if(	(m_pVertices[i].x == v0.x && m_pVertices[i].y == v0.y && m_pVertices[i].z == v0.z) ||
				(m_pVertices[i].x == v1.x && m_pVertices[i].y == v1.y && m_pVertices[i].z == v1.z) ||
				(m_pVertices[i].x == v2.x && m_pVertices[i].y == v2.y && m_pVertices[i].z == v2.z) )
			{
				vN.Cross(v1 - v0, v2 - v1);
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

void CN3IMesh::ApplyOffset(__Vector3 vOffset)
{
	__Vector3 v;
	for(int i = 0; i < m_nVC; i++)
	{
		m_pVertices[i].x += vOffset.x;
		m_pVertices[i].y += vOffset.y;
		m_pVertices[i].z += vOffset.z;
	}
}
