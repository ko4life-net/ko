// N3Mesh.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Mesh.h"
#include "N3IMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Mesh::CN3Mesh()
{
	m_dwType |= OBJ_MESH;

	m_pVertices = NULL; // 점 버퍼
	m_psnIndices = NULL; // 인덱스 버퍼

	this->Release();
}

CN3Mesh::~CN3Mesh()
{
	this->Release();
}

void CN3Mesh::Release()
{
	this->ReleaseVertices();
	this->ReleaseIndices();
}

void CN3Mesh::ReleaseVertices()
{
	delete [] m_pVertices; m_pVertices = NULL;
	
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);

	m_nVC = 0;
}

void CN3Mesh::ReleaseIndices()
{
	delete [] m_psnIndices; m_psnIndices = NULL;
	m_nIC = 0;
}

bool CN3Mesh::Load(HANDLE hFile)
{
	if(m_pVertices != NULL || m_psnIndices != NULL) this->Release();

	DWORD dwRWC = 0;

	ReadFile(hFile, &m_nVC, 4, &dwRWC, NULL); // 점갯수 읽기..
	if(m_nVC > 0)
	{
		this->Create(m_nVC, 0); // Vertex Buffer 생성 및 데이터 채우기
		ReadFile(hFile, m_pVertices, m_nVC * sizeof(__VertexT1), &dwRWC, NULL);
		this->FindMinMax(); // 최대, 최소값을 찾는다.
	}
	
	ReadFile(hFile, &m_nIC, 4, &dwRWC, NULL); // 인덱스 갯수 읽기..
	if(m_nIC > 0)
	{
		this->Create(0, m_nIC); // 인덱스 버퍼 생성 및 데이터 채우기
		ReadFile(hFile, m_psnIndices, m_nIC * 2, &dwRWC, NULL);
	}

	return true;
}

bool CN3Mesh::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	WriteFile(hFile, &m_nVC, 4, &dwRWC, NULL); // 점갯수 읽기..
	if(m_nVC > 0) 
	{
		WriteFile(hFile, m_pVertices, m_nVC * sizeof(__VertexT1), &dwRWC, NULL);
	}
	
	WriteFile(hFile, &m_nIC, 4, &dwRWC, NULL); // 인덱스 갯수 읽기..
	if(m_nIC > 0)
	{
		WriteFile(hFile, m_psnIndices, m_nIC * 2, &dwRWC, NULL);
	}

	return true;
}

void CN3Mesh::MakeIndexed()
{
	if(m_nVC <= 0 || m_nIC > 0) return;

	short wIs[65536]; memset(wIs, -1, sizeof(wIs));

	__VertexT1* pVs = new __VertexT1[m_nVC]; memset(pVs, -1, sizeof(__VertexT1) * m_nVC);
	int nICount = 0, nVCount = 0;
	for(int i = 0; i < m_nVC; i++)
	{
		BOOL bAccord = FALSE;
		for(int j = 0; j < nVCount; j++)
		{
			if(	m_pVertices[i].x == pVs[j].x && 
				m_pVertices[i].y == pVs[j].y && 
				m_pVertices[i].z == pVs[j].z && 
				m_pVertices[i].nx == pVs[j].nx && 
				m_pVertices[i].ny == pVs[j].ny && 
				m_pVertices[i].nz == pVs[j].nz && 
				m_pVertices[i].tu == pVs[j].tu && 
				m_pVertices[i].tv == pVs[j].tv )
			{
				bAccord = TRUE;
				break;
			}
		}

		if(TRUE == bAccord)
		{
			wIs[nICount] = j;
		}
		else
		{
			wIs[nICount] = nVCount;
			pVs[nVCount] = m_pVertices[i];
			nVCount++;
		}
		
		nICount++;
	}

	this->ReleaseIndices();
	this->ReleaseVertices();

	this->Create(nVCount, nICount);
	memcpy(m_pVertices, pVs, sizeof(__VertexT1)*nVCount);
	memcpy(m_psnIndices, wIs, 2*nICount);

	delete [] pVs; pVs = NULL;
}

void CN3Mesh::Create(int nVC, int nIC)
{
	__ASSERT(nVC < 65536, "점의 갯수가 너무 많습니다.");
	__ASSERT(nIC < 65536, "점의 갯수가 너무 많습니다.");

	if(nVC > 0) // 점이 있으면...
	{
		if(m_pVertices) this->ReleaseVertices();
		m_pVertices = new __VertexT1[nVC]; memset(m_pVertices, 0, nVC * sizeof(__VertexT1)); // Vertex Buffer 생성
		m_nVC = nVC;
	}

	if(nIC > 0) // Mesh 로딩에 성공하고, 인덱스가 있으면..
	{
		if(m_psnIndices) this->ReleaseIndices();
		m_psnIndices = new WORD[nIC]; memset(m_psnIndices, 0, nIC * 2); // Index Buffer 생성
		m_nIC = nIC;
	}
}

void CN3Mesh::FindMinMax()
{
	// 최소, 최대 점을 찾는다.
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

void CN3Mesh::Create_Cube(__Vector3 &vMin, __Vector3 &vMax)
{
	this->Create(36, 0);

	__Vector3 vPs[6];
	__Vector3 vN;
	float fTUVs[6][2] = { 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1 };

	int i = 0;

	// z 축 음의 면
	vN.Set(0,0,-1);
	vPs[0].Set(vMin.x, vMax.y, vMin.z); vPs[1].Set(vMax.x, vMax.y, vMin.z); vPs[2].Set(vMax.x, vMin.y, vMin.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMin.x, vMin.y, vMin.z);
	for(i = 0; i < 6; i++) m_pVertices[0+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	// x 축 양의 면
	vN.Set(1, 0, 0);
	vPs[0].Set(vMax.x, vMax.y, vMin.z); vPs[1].Set(vMax.x, vMax.y, vMax.z); vPs[2].Set(vMax.x, vMin.y, vMax.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMax.x, vMin.y, vMin.z);
	for(i = 0; i < 6; i++) m_pVertices[6+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	// z 축 양의 면
	vN.Set(0, 0, 1);
	vPs[0].Set(vMax.x, vMax.y, vMax.z); vPs[1].Set(vMin.x, vMax.y, vMax.z); vPs[2].Set(vMin.x, vMin.y, vMax.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMax.x, vMin.y, vMax.z);
	for(i = 0; i < 6; i++) m_pVertices[12+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	// x 축 음의 면
	vN.Set(-1, 0, 0);
	vPs[0].Set(vMin.x, vMax.y, vMax.z); vPs[1].Set(vMin.x, vMax.y, vMin.z); vPs[2].Set(vMin.x, vMin.y, vMin.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMin.x, vMin.y, vMax.z);
	for(i = 0; i < 6; i++) m_pVertices[18+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	// y 축 양의 면
	vN.Set(0, 1, 0);
	vPs[0].Set(vMin.x, vMax.y, vMax.z); vPs[1].Set(vMax.x, vMax.y, vMax.z); vPs[2].Set(vMax.x, vMax.y, vMin.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMin.x, vMax.y, vMin.z);
	for(i = 0; i < 6; i++) m_pVertices[24+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	// y 축 음의 면
	vN.Set(0, -1, 0);
	vPs[0].Set(vMin.x, vMin.y, vMin.z); vPs[1].Set(vMax.x, vMin.y, vMin.z); vPs[2].Set(vMax.x, vMin.y, vMax.z);
	vPs[3] = vPs[0]; vPs[4] = vPs[2]; vPs[5].Set(vMin.x, vMin.y, vMax.z);
	for(i = 0; i < 6; i++) m_pVertices[30+i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);

	this->FindMinMax();
}

void CN3Mesh::Create_Axis(float fLength)
{
	this->Create(12, 0);

	this->FindMinMax();

	// x 축
//	m_pVertices[0].Set(-fLength/2.0f, 0, 0, 0, 1, 0, 0, 0);
//	m_pVertices[1].Set( fLength/2.0f, 0, 0, 0, 1, 0, 0, 0);
//	m_pVertices[2] = m_pVertices[1];
//	m_pVertices[3] = m_pVertices[1];
//	m_pVertices[3].x -= fLength / 20.0f; 
//	m_pVertices[3].y += fLength / 20.0f;

	m_pVertices[0].Set(0, 0, 0, 0, 1, 0, 0, 0);
	m_pVertices[1].Set(fLength, 0, 0, 0, 1, 0, 0, 0);
	m_pVertices[2] = m_pVertices[1];
	m_pVertices[3] = m_pVertices[1];
	m_pVertices[3].x -= fLength / 10.0f; 
	m_pVertices[3].y += fLength / 10.0f;

	__Matrix44 mtx;
	__Vector3 v, n;
	
	// y 축
	mtx.RotationZ(D3DX_PI / -2.0f);
	for(int i = 0; i < 4; i++) 
	{
		v = m_pVertices[i].v;
		n = m_pVertices[i].n;
		v *= mtx;
		n *= mtx;

		m_pVertices[4+i].Set(v, n, 0, 0);
	}

	// z 축
	mtx.RotationY(D3DX_PI / -2.0f);
	for(i = 0; i < 4; i++) 
	{
		v = m_pVertices[i].v;
		n = m_pVertices[i].n;
		v *= mtx;
		n *= mtx;

		m_pVertices[8+i].Set(v, n, 0, 0);
	}
}

bool CN3Mesh::Import(CN3IMesh *pIMesh)
{
	int nFC = pIMesh->FaceCount();
	if(NULL == pIMesh || nFC <= 0) return false;
	
	this->Release();
	this->Create(nFC * 3, 0);

	__VertexT1* pvSrc = pIMesh->BuildVertexList();
	memcpy(m_pVertices, pvSrc, sizeof(__VertexT1) * nFC * 3);

	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int i = 0; i < nFC; i++) // Normal 값 다시 세팅..
	{
		v0 = m_pVertices[i*3+0].v;
		v1 = m_pVertices[i*3+1].v;
		v2 = m_pVertices[i*3+2].v;

		vN.Cross(v1 - v0, v2 - v1); // Normal 값을 계산하고...
		vN.Normalize();

		m_pVertices[i*3+0].n = vN;
		m_pVertices[i*3+1].n = vN;
		m_pVertices[i*3+2].n = vN;
	}

	this->NameSet(pIMesh->Name()); // 이름..
	return true;
}

void CN3Mesh::ReGenerateSmoothNormal()
{
	if(m_nVC <= 0) return;

	int* pnNs = new int[m_nVC];
	memset(pnNs, 0, 4 * m_nVC);
	__Vector3* pvNs = new __Vector3[m_nVC];
	memset(pvNs, 0, sizeof(__Vector3)*m_nVC);

	int nFC = 0;
	if(m_nIC > 0) nFC = m_nIC / 3;
	else nFC = m_nVC / 3;

	__Vector3 v0, v1, v2, vN(0,0,0);
	for(int i = 0; i < m_nVC; i++)
	{
		for(int j = 0; j < nFC; j++)
		{
			if(m_nIC > 0)
			{
				v0 = m_pVertices[m_psnIndices[j*3+0]].v;
				v1 = m_pVertices[m_psnIndices[j*3+1]].v;
				v2 = m_pVertices[m_psnIndices[j*3+2]].v;
			}
			else
			{
				v0 = m_pVertices[j*3+0].v;
				v1 = m_pVertices[j*3+1].v;
				v2 = m_pVertices[j*3+2].v;
			}

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
