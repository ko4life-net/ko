// N3VMesh.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N3VMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3VMesh::CN3VMesh()
{
//	m_dwType |= OBJ_MESH_VECTOR3;
	
	m_pVertices = NULL; // 점 버퍼
	m_pwIndices = NULL; // Index...

	this->Release();
}

CN3VMesh::~CN3VMesh()
{
	this->Release();
}

void CN3VMesh::Release()
{
	delete [] m_pVertices; m_pVertices = NULL;
	m_nVC = 0;

	delete [] m_pwIndices; m_pwIndices = NULL;
	m_nIC = 0;

	m_vCenter.Set(0,0,0); // Mesh Vertices 의 중간점..
	m_fRadius = 0.0f; // 반지름
}

int CN3VMesh::Load(HANDLE hFile)
{
	if(NULL != m_pVertices) this->Release();

	DWORD dwRWC = 0;

	int nVC;
	ReadFile(hFile, &nVC, 4, &dwRWC, NULL); // 점갯수 읽기..
	if(nVC > 0)
	{
		this->CreateVertices(nVC); // Vertex Buffer 생성 및 데이터 채우기
		ReadFile(hFile, m_pVertices, nVC * sizeof(__Vector3), &dwRWC, NULL);
	}

	int nIC;
	ReadFile(hFile, &nIC, 4, &dwRWC, NULL); // Index Count..
	if(nIC > 0)
	{
		this->CreateIndex(nIC); // Vertex Buffer 생성 및 데이터 채우기
		ReadFile(hFile, m_pwIndices, nIC * 2, &dwRWC, NULL);
	}

	this->CalcRadiusAndCenter(); // 중심점과 반지름을 계산해 준다..

	return 0;
}

int CN3VMesh::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	WriteFile(hFile, &m_nVC, 4, &dwRWC, NULL); // 점갯수 읽기..
	if(m_nVC > 0) 
	{
		WriteFile(hFile, m_pVertices, m_nVC * sizeof(__Vector3), &dwRWC, NULL);
	}

	WriteFile(hFile, &m_nIC, 4, &dwRWC, NULL); // Index Count..
	if(m_nIC > 0)
	{
		WriteFile(hFile, m_pwIndices, m_nIC * 2, &dwRWC, NULL); // Index Buffer 데이터 쓰기..
	}

	return 0;
}

void CN3VMesh::CreateVertices(int nVC)
{
	_ASSERT(nVC > 0);
	
	delete [] m_pVertices;
	m_pVertices = new __Vector3[nVC];
	memset(m_pVertices, 0, nVC * sizeof(__Vector3)); // Vertex Buffer 생성
	m_nVC = nVC;
}

void CN3VMesh::CreateIndex(int nIC)
{
	_ASSERT(nIC > 0);
	
	delete [] m_pwIndices;
	m_pwIndices = new WORD[nIC];
	memset(m_pwIndices, 0, nIC * 2); // Index Buffer 생성
	m_nIC = nIC;
}

void CN3VMesh::CreateCube(const __Vector3 &vMin, const __Vector3 &vMax)
{
	this->CreateVertices(8);
	this->CreateIndex(36);

	m_pVertices[0].Set(vMin.x, vMax.y, vMin.z); 
	m_pVertices[1].Set(vMax.x, vMax.y, vMin.z); 
	m_pVertices[2].Set(vMax.x, vMin.y, vMin.z); 
	m_pVertices[3].Set(vMin.x, vMin.y, vMin.z); 

	m_pVertices[4].Set(vMax.x, vMax.y, vMax.z); 
	m_pVertices[5].Set(vMin.x, vMax.y, vMax.z); 
	m_pVertices[6].Set(vMin.x, vMin.y, vMax.z); 
	m_pVertices[7].Set(vMax.x, vMin.y, vMax.z); 

	m_pwIndices[0] = 0; m_pwIndices[1] = 1; m_pwIndices[2] = 2; 
	m_pwIndices[3] = 0; m_pwIndices[4] = 2; m_pwIndices[5] = 3; 
	m_pwIndices[6] = 1; m_pwIndices[7] = 4; m_pwIndices[8] = 7;
	m_pwIndices[9] = 1; m_pwIndices[10] = 7; m_pwIndices[11] = 2;
	m_pwIndices[12] = 4; m_pwIndices[13] = 5; m_pwIndices[14] = 6;
	m_pwIndices[15] = 4; m_pwIndices[16] = 6; m_pwIndices[17] = 7;
	m_pwIndices[18] = 5; m_pwIndices[19] = 0; m_pwIndices[20] = 3;
	m_pwIndices[21] = 5; m_pwIndices[22] = 3; m_pwIndices[23] = 6;
	m_pwIndices[24] = 5; m_pwIndices[25] = 4; m_pwIndices[26] = 1;
	m_pwIndices[27] = 5; m_pwIndices[28] = 1; m_pwIndices[29] = 0;
	m_pwIndices[30] = 3; m_pwIndices[31] = 2; m_pwIndices[32] = 7;
	m_pwIndices[33] = 3; m_pwIndices[34] = 7; m_pwIndices[35] = 6;

	this->CalcRadiusAndCenter(); // 중심점과 반지름을 계산해 준다..
}
/*
void CN3VMesh::Render(D3DCOLOR crLine)
{
	if(m_nVC <= 0) return;

	DWORD dwLight, dwShade;
	m_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	m_lpD3DDev->GetRenderState(D3DRS_FILLMODE, &dwShade);
	if(dwLight) m_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(D3DFILL_WIREFRAME != dwShade) m_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_lpD3DDev->SetTexture(0, NULL);
	m_lpD3DDev->SetVertexShader(FVF_CV);

	__VertexColor vTs[3];
	if(m_nIC)
	{
		int nFC = m_nIC / 3;
		for(int i = 0; i < nFC; i++)
		{
			vTs[0].Set(m_pVertices[m_pwIndices[i*3+0]], crLine);
			vTs[1].Set(m_pVertices[m_pwIndices[i*3+1]], crLine);
			vTs[2].Set(m_pVertices[m_pwIndices[i*3+2]], crLine);

			m_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vTs, sizeof(__VertexColor));
		}
	}

	if(dwLight) m_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
	if(D3DFILL_WIREFRAME != dwShade) m_lpD3DDev->SetRenderState(D3DRS_FILLMODE, dwShade);
}
*/
void CN3VMesh::CalcRadiusAndCenter()
{
	if(m_nVC <= 0) return;

	__Vector3 vMin(100000.0f, 100000.0f, 100000.0f), vMax(-100000.0f, -100000.0f, -100000.0f);
	for(int i = 0; i < m_nVC; i++)
	{
		if(m_pVertices[i].x < vMin.x) vMin.x = m_pVertices[i].x;
		if(m_pVertices[i].y < vMin.y) vMin.y = m_pVertices[i].y;
		if(m_pVertices[i].z < vMin.z) vMin.z = m_pVertices[i].z;
		if(m_pVertices[i].x > vMax.x) vMax.x = m_pVertices[i].x;
		if(m_pVertices[i].y > vMax.y) vMax.y = m_pVertices[i].y;
		if(m_pVertices[i].z > vMax.z) vMax.z = m_pVertices[i].z;
	}

	m_vCenter = vMin + (vMax - vMin) / 2.0f;
	m_fRadius = (vMax - vMin).Magnitude() / 2.0f;
}

void CN3VMesh::SetRegion(int dimx, int dimz)
{
	m_nRegionX = dimx;
	m_nRegionZ = dimz;
}
