// N3Shape.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////
// CN3Shape Part ....
CN3Shape::CPart::CPart()
{
	m_dwType |= OBJ_SHAPE_PART;

	m_nTexCount = 0;
	m_ppTexRefs = NULL; // Texture Reference Pointers
	this->Release();
}

CN3Shape::CPart::~CPart()
{
	this->Release();
}

void CN3Shape::CPart::Release()
{
	m_vPivot.Set(0,0,0); // Local 축
	m_Matrix.Identity(); // World Matrix.. Shape Loading 때 미리 계산해야 좋다..		
	m_bOutOfCameraRange = TRUE; // Camera 범위 바깥에 있음...
	m_Mtl.Init(); // Material
	m_fTexFPS = 10.0f; // Texture Animation Interval;
	m_nTexCount = 0; // Texture Count
	m_nTexIndex = 0; // Current Texture Index.. Animation 시킬때 필요한 인덱스이다..
	m_dwTickPrev = timeGetTime(); // 전의 Tick Count...

	m_vMin.Zero();
	m_vMax.Zero();
	m_fRadius = 0.0f;

	for(int i = 0; i < m_nTexCount; i++) { s_MngTex.Delete(m_ppTexRefs[i]); } 
	delete [] m_ppTexRefs;
	m_ppTexRefs = NULL; // Texture Reference Pointers
	m_PMInst.Release();
}

void CN3Shape::CPart::MeshSet(const char* szFN)
{
	m_PMInst.Create(s_MngPMesh.Get(szFN));
	this->FindMinMax();
}

void CN3Shape::CPart::TexAlloc(int nCount)
{
	if(nCount <= 0) return;

	for(int i = 0; i < m_nTexCount; i++) { s_MngTex.Delete(m_ppTexRefs[i]); }
	delete [] m_ppTexRefs;

	m_nTexCount = nCount;
	m_ppTexRefs = new CN3Texture*[nCount];
	memset(m_ppTexRefs, 0, 4 * nCount);
}
void CN3Shape::CPart::TexSet(int index, const char* szFN)
{
	if(index < 0 || index >= m_nTexCount) return;
	s_MngTex.Delete(m_ppTexRefs[index]);
	m_ppTexRefs[index] = s_MngTex.Get(szFN);
}

void CN3Shape::CPart::Tick(DWORD dwTime) // timeGetTime 으로 얻은 값을 넣으면 Texture Animation 을 컨트롤 한다..
{
	// 카메라와 멀리 떨어지면 지나간다..
	float fDist = (m_Matrix.Pos() - s_CameraData.vEye).Magnitude();
	if(fDist > s_CameraData.fFP + m_fRadius)
	{
		m_bOutOfCameraRange = TRUE;
		return;
	}

	if(s_CameraData.IsOutOfFrustum(this->m_Matrix.Pos(), m_fRadius * 2.4f)) // 카메라 사면체 바깥이면 지나간다..
	{
		m_bOutOfCameraRange = TRUE;
		return;
	}
	
	m_bOutOfCameraRange = FALSE;
	float fLOD = fDist * s_CameraData.fFOV;

	// 카메라 거리에 따라 LOD 수준을 조절한다.
//	fLOD *= 256.0f / s_CameraData.fFP;
	m_PMInst.SetLOD(fLOD);
	if(m_nTexCount > 1 && (dwTime - m_dwTickPrev) > (1000.0f / m_fTexFPS))
	{
		m_nTexIndex++;
		if(m_nTexIndex >= m_nTexCount) m_nTexIndex = 0;
		m_dwTickPrev = dwTime;
	}


	m_bOutOfCameraRange = FALSE; // 임시로 강제 세팅
}

void CN3Shape::CPart::Render()
{
	if(m_bOutOfCameraRange || m_PMInst.GetNumVertices() <= 0) return;

	// 로딩할때 미리 계산해 놓은 월드 행렬 적용..
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	CN3Base::s_RenderInfo.nShapePart++;

	static DWORD dwAlpha, dwFog, dwCull;
	if(m_Mtl.nRenderFlags & RF_ALPHABLENDING) // Alpha 사용
	{
		s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
		if(TRUE != dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   m_Mtl.dwSrcBlend);
		s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  m_Mtl.dwDestBlend);
	}
	if(m_Mtl.nRenderFlags & RF_NOTUSEFOG) // Fog 무시..
	{
		s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
		if(TRUE == dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
	if(m_Mtl.nRenderFlags & RF_DOUBLESIDED) // Render Flags - 
	{
		s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	s_lpD3DDev->SetMaterial(&m_Mtl); // 재질 설정..

	LPDIRECT3DTEXTURE8 lpTex = NULL;
	if(m_nTexCount > 0)
	{
		if(m_ppTexRefs[m_nTexIndex]) lpTex = m_ppTexRefs[m_nTexIndex]->Get();
	}

	s_lpD3DDev->SetTexture(0, lpTex);
	if(NULL != lpTex)
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP,   m_Mtl.dwColorOp);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, m_Mtl.dwColorArg1);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, m_Mtl.dwColorArg2);
	}
	else
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	}

	m_PMInst.Render();

	if((m_Mtl.nRenderFlags & RF_ALPHABLENDING) && FALSE == dwAlpha)	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) 		s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // 안개 사용하지 않는다..
	if((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) 		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
}

void CN3Shape::CPart::RenderSelected()
{
//	DWORD dwPrevAlpha;
//	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwPrevAlpha);
	
//	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_ONE);
//	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_ONE);

//	DWORD dwPrevFog = FALSE;
//	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwPrevFog);
//	s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	LPDIRECT3DTEXTURE8 lpTex = NULL;
	if(m_nTexCount > 0 && m_ppTexRefs[0])
	{
		m_ppTexRefs[0]->UpdateRenderInfo();
		lpTex = m_ppTexRefs[0]->Get();
	}

	s_lpD3DDev->SetTexture(0, lpTex);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	if(lpTex != NULL) s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	else s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
	m_PMInst.Render();

//	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwPrevAlpha);
//	s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwPrevFog); // 안개 사용하지 않는다..
}

void CN3Shape::CPart::RenderAxis()
{
	// 축그리기..
	static CN3Transform TTmp;
	TTmp.m_Matrix = m_Matrix;
	TTmp.Render(&m_Matrix, m_fRadius * 1.5f);

}

void CN3Shape::CPart::FindMinMax()
{
	m_vMin.Zero();
	m_vMax.Zero();
	m_fRadius = 0;

	// 반지름 계산..
	CN3PMesh* pPMesh = m_PMInst.GetMesh();
	if(NULL == pPMesh) return;

	m_vMin = pPMesh->Min();
	m_vMax = pPMesh->Max();
	__Vector3 vRad = m_vMax;
	if(-m_vMin.x > m_vMax.x) vRad.x = -m_vMin.x;
	if(-m_vMin.y > m_vMax.y) vRad.y = -m_vMin.y;
	if(-m_vMin.z > m_vMax.z) vRad.z = -m_vMin.z;

	m_fRadius = vRad.Magnitude();
}

bool CN3Shape::CPart::Load(HANDLE hFile)
{
	DWORD dwRWC;
	int nL = 0;
	char szFN[256];

	ReadFile(hFile, &m_vPivot, sizeof(__Vector3), &dwRWC, NULL);

	ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 메시 파일 이름..
	this->MeshSet(szFN);

	ReadFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // 재질

	int nTC = 0;
	ReadFile(hFile, &nTC, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
	delete [] m_ppTexRefs; m_ppTexRefs = NULL;
	this->TexAlloc(nTC); // Texture Pointer Pointer 할당..
	for(int j = 0; j < nTC; j++) // Texture Count 만큼 파일 이름 읽어서 텍스처 부르기..
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 텍스처 파일 이름..
			m_ppTexRefs[j] = s_MngTex.Get(szFN);
		}
	}

	return true;
}

bool CN3Shape::CPart::Save(HANDLE hFile)
{
	DWORD dwRWC;
	int nL = 0;

	WriteFile(hFile, &m_vPivot, sizeof(__Vector3), &dwRWC, NULL);

	CN3PMesh* pPMesh = m_PMInst.GetMesh();
	__ASSERT(pPMesh, "Progressive mesh pointer is NULL!");

	nL = lstrlen(pPMesh->FileName());
	WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0) WriteFile(hFile, pPMesh->FileName(), nL, &dwRWC, NULL); // 메시 파일 이름..

	WriteFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // 재질

	WriteFile(hFile, &m_nTexCount, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
	for(int j = 0; j < m_nTexCount; j++) // Texture File 이름 쓰기...
	{
		if(m_ppTexRefs[j]) nL = lstrlen(m_ppTexRefs[j]->FileName());
		else nL = 0;

		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0) WriteFile(hFile, m_ppTexRefs[j]->FileName(), nL, &dwRWC, NULL); // 택스처 파일 이름..
	}

	return true;
}

// CN3Shape Part ....
///////////////////////////////////

///////////////////////////////////
// CN3Shape
CN3Shape::CN3Shape()
{
	m_dwType |= OBJ_SHAPE;
	this->Release();
}

CN3Shape::~CN3Shape()
{
	CN3Shape::Release();
}

void CN3Shape::Release()
{
	m_bDontRender = FALSE;
	
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		delete m_Parts.Get(pPos);
	}
	m_Parts.Release();
	
	m_nBelong = 0;

	CN3TransformCollision::Release();
}

void CN3Shape::Tick(float fFrm)
{
	// 카메라와 멀리 떨어지면 지나간다..
	float fDist = (m_vPos - s_CameraData.vEye).Magnitude();
	if(fDist > s_CameraData.fFP + m_fRadius)
	{
		m_bDontRender = TRUE;
		return;
	}

	if(s_CameraData.IsOutOfFrustum(m_vPos, m_fRadius * 2.4f))
	{
		m_bDontRender = TRUE;
		return;
	}

	m_bDontRender = FALSE;

	CN3TransformCollision::Tick(fFrm);

	CPart* pPD = NULL;
	DWORD dwTime = timeGetTime();
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPD = m_Parts.Get(pPos);

		__ASSERT(pPD, "Part Data is NULL");
		pPD->Tick(dwTime);
	}
}

// 카메라 위치, 카메라 평면(관찰 절두체 평면) -> 12개의 벡터 배열로 되어 있다.
// [0][1]:카메라 위치와 벡터, [2][3]:카메라 범위 위치와 방향 벡터, [4][5] ~ [10][11]:상하좌우평면벡터
void CN3Shape::Render()
{
	if(TRUE == m_bDontRender) return;
	CN3Base::s_RenderInfo.nShape++;

	static dwTick = 0;
	dwTick = timeGetTime(); // Time...

	CPart* pPD = NULL;
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPD = m_Parts.Get(pPos);
		pPD->Render();
	}

#ifdef _DEBUG
#ifdef _N3_CLIENT_
//	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
//	if(m_pMeshCollision) m_pMeshCollision->Render(0xff00ff00);
//	if(m_pMeshClimb) m_pMeshClimb->Render(0xff0000ff);
#endif
#endif
}

void CN3Shape::RenderSelected()
{
	// 축그리기..
	CN3Transform::Render(NULL, m_fRadius * 1.5f);

	CPart* pPD = NULL;
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPD = m_Parts.Get(pPos);
		pPD->RenderSelected();
	}
}

bool CN3Shape::Load(HANDLE hFile)
{
	int nPC = m_Parts.Count();
	if(nPC > 0) CN3Shape::Release();

	CN3TransformCollision::Load(hFile); // 기본정보 읽기...

	DWORD dwRWC = 0;
	
	int nL = 0;
	char szFN[512] = "";

	ReadFile(hFile, &nPC, 4, &dwRWC, NULL); // Part Count
	for(int i = 0; i < nPC; i++) PartAdd();

	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		CPart* pPD = m_Parts.Get(pPos);

		pPD->Load(hFile);
		pPD->RecalcMatrix(m_Matrix); // Part Matrix 계산
	}
		
	ReadFile(hFile, &m_nBelong, 4, &dwRWC, NULL); // 소속
	this->FindMinMax();
	return true;
}

bool CN3Shape::Save(HANDLE hFile)
{
	CN3TransformCollision::Save(hFile); // 기본정보 읽기...
	
	DWORD dwRWC = 0;
	
	int nL = 0;
	
	CPart* pPD = NULL;
	int nPC = m_Parts.Count();
	WriteFile(hFile, &nPC, 4, &dwRWC, NULL); // Mesh FileName

	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPD = m_Parts.Get(pPos);
		pPD->Save(hFile);
	}

	WriteFile(hFile, &m_nBelong, 4, &dwRWC, NULL); // 소속

	return true;
}

int CN3Shape::PartAdd()
{
	CPart* pPD = new CPart();
	return m_Parts.Add(pPD);
}

void CN3Shape::PartDelete(int nPart)
{
	void* pPos = m_Parts.Position(nPart);
	CPart* pPD = m_Parts.Get(pPos);
	delete pPD;

	m_Parts.Delete(pPos);
}

void CN3Shape::RenderSelected(int nPart)
{
	CPart* pPD = m_Parts.Get(nPart);
	if(NULL == pPD) return;
	pPD->RenderSelected();
}

void CN3Shape::ReCalcMatrix()
{
	CN3Transform::ReCalcMatrix(); // Transfomr Matrix 를 계산 해주고..

	// 각 파트의 매트릭스를 다시 계산해 준다..
	ReCalcPartMatrix();
}

void CN3Shape::ReCalcPartMatrix()
{
	// 각 파트의 매트릭스를 다시 계산해 준다..
	CN3Shape::CPart* pPD = NULL;
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPD = m_Parts.Get(pPos);

		pPD->RecalcMatrix(m_Matrix);
	}
}

void CN3Shape::FindMinMax()
{
	__Vector3 vMax(-100000.0f, -100000.0f, -100000.0f);
	__Vector3 vMin(100000.0f, 100000.0f, 100000.0f);
	__Vector3 vMinTmp(0,0,0);
	__Vector3 vMaxTmp(0,0,0);

	// 가장 큰 지점찾기..
	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		CPart* pPD = m_Parts.Get(pPos);
		pPD->RecalcMatrix(m_Matrix);

		CN3PMesh* pPMesh = pPD->m_PMInst.GetMesh();
		if(pPMesh)
		{
			vMinTmp = (pPMesh->Min() * pPD->m_Matrix) - m_vPos;
			vMaxTmp = (pPMesh->Max() * pPD->m_Matrix) - m_vPos;
		}
		else
		{
			vMinTmp.Set(0,0,0);
			vMaxTmp.Set(0,0,0);
		}
	
		if(vMinTmp.x < vMin.x) vMin.x = vMinTmp.x;
		if(vMinTmp.y < vMin.y) vMin.y = vMinTmp.y;
		if(vMinTmp.z < vMin.z) vMin.z = vMinTmp.z;
		if(vMaxTmp.x > vMax.x) vMax.x = vMaxTmp.x;
		if(vMaxTmp.y > vMax.y) vMax.y = vMaxTmp.y;
		if(vMaxTmp.z > vMax.z) vMax.z = vMaxTmp.z;
	}

	// 최대 최소값을 저장
	m_vMin = vMin;
	m_vMax = vMax;

	// 최대 최소값을 갖고 반지름 계산한다..
	__Vector3 vRad;
	vMin.Absolute();
	vMax.Absolute();
	
	if(vMin.x > vMax.x) vRad.x = vMin.x; else vRad.x = vMax.x;
	if(vMin.y > vMax.y) vRad.y = vMin.y; else vRad.y = vMax.y;
	if(vMin.z > vMax.z) vRad.z = vMin.z; else vRad.z = vMax.z;
	
	m_fRadius = vRad.Magnitude();
}


bool CN3Shape::CheckPreciseCollision(__Vector3 &vPos, __Vector3 &vDir)
{
	return false;
}
