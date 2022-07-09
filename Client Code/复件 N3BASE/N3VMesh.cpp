// N3VMesh.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"

#include "N3VMesh.h"
#include "N3IMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3VMesh::CN3VMesh()
{
	m_dwType |= OBJ_MESH_VECTOR3;
	
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

bool CN3VMesh::Load(HANDLE hFile)
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

	return true;
}

bool CN3VMesh::Save(HANDLE hFile)
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

	return true;
}

void CN3VMesh::CreateVertices(int nVC)
{
	__ASSERT(nVC > 0, "Vertex count is less than 0");
	
	delete [] m_pVertices;
	m_pVertices = new __Vector3[nVC];
	memset(m_pVertices, 0, nVC * sizeof(__Vector3)); // Vertex Buffer 생성
	m_nVC = nVC;
}

void CN3VMesh::CreateIndex(int nIC)
{
	__ASSERT(nIC > 0, "Index count is less than 0");
	
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

void CN3VMesh::Render(D3DCOLOR crLine)
{
	if(m_nVC <= 0) return;

	DWORD dwLight, dwShade;
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	s_lpD3DDev->GetRenderState(D3DRS_FILLMODE, &dwShade);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(D3DFILL_WIREFRAME != dwShade) s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	s_lpD3DDev->SetTexture(0, NULL);
 	s_lpD3DDev->SetVertexShader(FVF_CV);

	__VertexColor vTs[3];
	if(m_nIC)
	{
		int nFC = m_nIC / 3;
		for(int i = 0; i < nFC; i++)
		{
			vTs[0].Set(m_pVertices[m_pwIndices[i*3+0]], crLine);
			vTs[1].Set(m_pVertices[m_pwIndices[i*3+1]], crLine);
			vTs[2].Set(m_pVertices[m_pwIndices[i*3+2]], crLine);

			s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vTs, sizeof(__VertexColor));
		}
	}
	else
	{
		int nFC = m_nVC / 3;
		for(int i = 0; i < nFC; i++)
		{
			vTs[0].Set(m_pVertices[i*3+0], crLine);
			vTs[1].Set(m_pVertices[i*3+1], crLine);
			vTs[2].Set(m_pVertices[i*3+2], crLine);

			s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vTs, sizeof(__VertexColor));
		}
	}

	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
	if(D3DFILL_WIREFRAME != dwShade) s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, dwShade);
}

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

bool CN3VMesh::CheckCollision(const __Matrix44* pMtxWorld, int xScreen, int yScreen)
{
	if(m_nVC <= 0) return false;
	// Compute the vector of the pick ray in screen space
	static __Vector3 vTmp, vPos, vDir;
	vTmp.x =  ( ( ( 2.0f * xScreen ) / (CN3Base::s_CameraData.vp.Width) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._11;
	vTmp.y = -( ( ( 2.0f * yScreen ) / (CN3Base::s_CameraData.vp.Height) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._22;
	vTmp.z =  1.0f;

	// Transform the screen space pick ray into 3D space
	__Matrix44* pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
	vDir.x  = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
	vDir.y  = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
	vDir.z  = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
	vPos = pMtxVI->Pos();

	if(pMtxWorld)
	{
		static __Matrix44 mtxWI;
		D3DXMatrixInverse(&mtxWI, NULL, pMtxWorld); // World Matrix Inverse

		vPos *= mtxWI;
		mtxWI.PosSet(0,0,0);
		vDir *= mtxWI; // 역행렬로 회전..
	}

	if(m_nIC > 0)  // 인덱스가 있는 경우
	{
		int nFC = m_nIC / 3;
		for(int i = 0; i < nFC; i++)
		{
			if(::IntersectTriangle(vPos, vDir, m_pVertices[m_pwIndices[i*3+0]], m_pVertices[m_pwIndices[i*3+1]], m_pVertices[m_pwIndices[i*3+2]])) // 첫째 벡터가 걸치면..
			{
				return true;
			}
		}
	}
	else // 인덱스 없이 점만 있는 경우
	{
		int nFC = m_nVC / 3;
		for(int i = 0; i < nFC; i++)
		{
			if(::IntersectTriangle(vPos, vDir, m_pVertices[i*3+0], m_pVertices[i*3+1], m_pVertices[i*3+2])) // 첫째 벡터가 걸치면..
			{
				return true;
			}
		}
	}

	return false;
}

bool CN3VMesh::CheckCollision(const __Matrix44* pMtxWorld, __Vector3& v0, __Vector3& v1, __Vector3 &vCollision, __Vector3 &vNormal)
{
	if(m_nVC <= 0) return false;

	static __Vector3 vPos0, vPos1, vDir;
	vPos0 = v0;
	vPos1 = v1;

	if(pMtxWorld)
	{
		static __Matrix44 mtxWI;
		D3DXMatrixInverse(&mtxWI, NULL, pMtxWorld); // World Matrix Inverse

		vPos0 *= mtxWI;
		vPos1 *= mtxWI;
	}
	else
	{
	}
	vDir = vPos1 - vPos0;

	if(m_nIC > 0) // 인덱스가 있는 경우
	{
		int nFC = m_nIC / 3;
		int nCI0, nCI1, nCI2; // Collision polygon index
		for(int i = 0; i < nFC; i++)
		{
			nCI0 = m_pwIndices[i*3+0];
			nCI1 = m_pwIndices[i*3+1];
			nCI2 = m_pwIndices[i*3+2];

			if(::IntersectTriangle(vPos0, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2])) // 첫째 벡터가 걸치면..
			{
				if(false == ::IntersectTriangle(vPos1, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2])) // 둘째는 안 걸치면..
				{
					// 충돌이다..
					float fT, fU, fV;
					::IntersectTriangle(vPos0, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2], fT, fU, fV, &vCollision);
					if(pMtxWorld) vCollision *= (*pMtxWorld);
					// 법선 벡터 구하기..
					vNormal.Cross(m_pVertices[nCI1] - m_pVertices[nCI0], m_pVertices[nCI2] - m_pVertices[nCI1]);
					vNormal.Normalize();
					return true;
				}
			}
		}
	}
	else // 인덱스 없이 점만 있는 경우
	{
		int nFC = m_nIC / 3;
		int nCI0, nCI1, nCI2; // Collision polygon index
		for(int i = 0; i < nFC; i++)
		{
			nCI0 = m_pwIndices[i*3+0];
			nCI1 = m_pwIndices[i*3+1];
			nCI2 = m_pwIndices[i*3+2];

			if(::IntersectTriangle(v0, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2])) // 첫째 벡터가 걸치면..
			{
				if(false == ::IntersectTriangle(v1, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2])) // 둘째는 안 걸치면..
				{
					// 충돌이다..
					float fT, fU, fV;
					::IntersectTriangle(v0, vDir, m_pVertices[nCI0], m_pVertices[nCI1], m_pVertices[nCI2], fT, fU, fV, &vCollision);
					// 법선 벡터 구하기..
					vNormal.Cross(m_pVertices[nCI1] - m_pVertices[nCI0], m_pVertices[nCI2] - m_pVertices[nCI1]);
					return true;
				}
			}
		}
	}

	return false;
}

bool CN3VMesh::Import(CN3IMesh *pIMesh)
{
	int nFC = pIMesh->FaceCount();
	if(NULL == pIMesh || nFC <= 0) return false;
	
	this->Release();
	this->CreateVertices(nFC * 3);

	__VertexT1* pvSrc = pIMesh->BuildVertexList();

	for(int i = 0; i < nFC; i++) // Normal 값 다시 세팅..
	{
		m_pVertices[i*3+0] = pvSrc[i*3+0].v;
		m_pVertices[i*3+1] = pvSrc[i*3+1].v;
		m_pVertices[i*3+2] = pvSrc[i*3+2].v;
	}

	this->NameSet(pIMesh->Name()); // 이름..
	return true;
}

