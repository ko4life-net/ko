// N3Chr.cpp: implementation of the CN3Chr class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Chr.h"
#include "N3Mesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////
// Part, Plug....
CN3CPart::CN3CPart()
{
	m_dwType |= OBJ_CHARACTER_PART;
	m_pTexRef = NULL;

	this->Release();
};

CN3CPart::~CN3CPart()
{
	this->Release();
};

void CN3CPart::Release()
{
	m_Type = PART_UNKNOWN;
	lstrcpy(m_szID, "");
	m_Mtl.Init();

	s_MngTex.Delete(m_pTexRef);
	m_pTexRef = NULL;
	for(int i = 0; i < MAX_CHR_LOD; i++)
	{
		m_IMeshes[i].Release();
		m_Skins[i].Release();
	}
	CN3Base::Release();
}

void CN3CPart::TexSet(const char* szFN)
{
	s_MngTex.Delete(m_pTexRef);
	m_pTexRef = s_MngTex.Get(szFN, TRUE);
}

bool CN3CPart::Load(HANDLE hFile)
{
	CN3Base::Load(hFile);

	DWORD dwRWC = 0;
	int nL = 0;
	char szFN[512] = "";

	ReadFile(hFile, &m_Type, 4, &dwRWC, NULL);
	ReadFile(hFile, m_szID, sizeof(m_szID), &dwRWC, NULL);
	ReadFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL);

	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) 
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		this->TexSet(szFN);
	}

	for(int i = 0; i < MAX_CHR_LOD; i++)
	{
		m_IMeshes[i].Load(hFile);
		m_Skins[i].Load(hFile);
	}

	return true;
}

bool CN3CPart::Save(HANDLE hFile)
{
	CN3Base::Save(hFile);

	DWORD dwRWC = 0;
	int nL = 0;

	WriteFile(hFile, &m_Type, 4, &dwRWC, NULL);
	WriteFile(hFile, m_szID, sizeof(m_szID), &dwRWC, NULL);
	WriteFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL);
	
	if(m_pTexRef) nL = lstrlen(m_pTexRef->FileName());
	else nL = 0;
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) WriteFile(hFile, m_pTexRef->FileName(), nL, &dwRWC, NULL);

	for(int i = 0; i < MAX_CHR_LOD; i++)
	{
		m_IMeshes[i].Save(hFile);
		m_Skins[i].Save(hFile);
	}

	return true;
}

void CN3CPart::Render(int nLOD)
{
	if(nLOD < 0 || nLOD > MAX_CHR_LOD) return;
	if(m_IMeshes[nLOD].VertexCount() <= 0 || m_Skins[nLOD].VertexCount() <= 0) return;

	CN3Base::s_RenderInfo.nChrPart++;

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

	s_lpD3DDev->SetMaterial(&m_Mtl);
	LPDIRECT3DTEXTURE8 lpTex = NULL;
	if(m_pTexRef) lpTex = m_pTexRef->Get();
	if(lpTex)
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		s_lpD3DDev->SetTexture(0, lpTex);
		
		m_pTexRef->UpdateRenderInfo();
	}
	else
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		s_lpD3DDev->SetTexture(0, NULL);
	}

	m_IMeshes[nLOD].Render();

	if((m_Mtl.nRenderFlags & RF_ALPHABLENDING) && FALSE == dwAlpha)	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) 		s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // 안개 사용하지 않는다..
	if((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) 		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
}


CN3CPlug::CN3CPlug()
{
	m_dwType |= OBJ_CHARACTER_PLUG;

	m_pTexRef = NULL;
	Release();
};

CN3CPlug::~CN3CPlug()
{
	this->Release();
}

void CN3CPlug::Release()
{
	m_Type = PLUG_UNKNOWN;
	m_nJointIndex = 0;
	m_PMeshInst.Release();

	s_MngTex.Delete(m_pTexRef);
	m_pTexRef = NULL;

	m_Mtl.Init();
	m_vPosition.Zero();
	m_MtxRot.Identity();
	m_vScale.Set(1,1,1);
	m_Matrix.Identity();

	m_nTraceStep = 0; // 궤적 소멸 시간..
	m_crTrace = 0xffffffff; // 궤적 색깔.. 검은색이면 없다..
	m_fTrace0 = 0; // 궤적 위치..
	m_fTrace1 = 0; // 궤적 위치..
}

void CN3CPlug::RecalcMatrix()
{
	m_Matrix.Scale(m_vScale);
	m_Matrix *= m_MtxRot;
	m_Matrix.PosSet(m_vPosition);
}

void CN3CPlug::TexSet(const char* szFN)
{
	s_MngTex.Delete(m_pTexRef);
	m_pTexRef = s_MngTex.Get(szFN, TRUE);
}

void CN3CPlug::PMeshSet(const char* szFN)
{
	s_MngPMesh.Delete(m_PMeshInst.GetMesh());
	m_PMeshInst.Create(s_MngPMesh.Get(szFN));
}

void CN3CPlug::Render(__Matrix44& mtxParent, __Matrix44& mtxJoint)
{
	if(m_PMeshInst.GetNumVertices() <= 0) return;
	
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

	static __Matrix44 mtx;
	mtx = m_Matrix;
	mtx *= mtxJoint;
	mtx *= mtxParent;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtx);

	s_lpD3DDev->SetMaterial(&m_Mtl);
	LPDIRECT3DTEXTURE8 lpTex = NULL;
	if(m_pTexRef) lpTex = m_pTexRef->Get();
	if(lpTex)
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		s_lpD3DDev->SetTexture(0, lpTex);

		m_pTexRef->UpdateRenderInfo();
	}
	else
	{
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		s_lpD3DDev->SetTexture(0, NULL);
	}

	if((m_Mtl.nRenderFlags & RF_ALPHABLENDING) && FALSE == dwAlpha)	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) 		s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // 안개 사용하지 않는다..
	if((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) 		s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);

	m_PMeshInst.Render();
}

bool CN3CPlug::Load(HANDLE hFile)
{
	CN3Base::Load(hFile);

	DWORD dwRWC = 0;
	int nL = 0;
	char szFN[512] = "";

	ReadFile(hFile, &m_Type, 4, &dwRWC, NULL); // Plug Type
	ReadFile(hFile, &m_nJointIndex, 4, &dwRWC, NULL); // Plug Joint Index

	ReadFile(hFile, &m_vPosition, sizeof(m_vPosition), &dwRWC, NULL);
	ReadFile(hFile, &m_MtxRot, sizeof(m_MtxRot), &dwRWC, NULL);
	ReadFile(hFile, &m_vScale, sizeof(m_vScale), &dwRWC, NULL);

	ReadFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // 재질

	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		this->PMeshSet(szFN);
	}

	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		this->TexSet(szFN);
	}

	ReadFile(hFile, &m_nTraceStep, 4, &dwRWC, NULL); // 궤적 갯수..
	if(m_nTraceStep > 0 && m_nTraceStep <= MAX_PLUG_TRACE_VERTEX/2 - 1)
	{
		ReadFile(hFile, &m_crTrace, 4, &dwRWC, NULL); // 궤적 색깔.. 검은색이면 없다..
		ReadFile(hFile, &m_fTrace0, 4, &dwRWC, NULL); // 궤적 위치..
		ReadFile(hFile, &m_fTrace1, 4, &dwRWC, NULL); // 궤적 위치..
	}
	else m_nTraceStep = 0;

	this->RecalcMatrix(); // 행렬 계산...

	return 0;
}

bool CN3CPlug::Save(HANDLE hFile)
{
	CN3Base::Save(hFile);

	DWORD dwRWC = 0;
	int nL = 0;

	WriteFile(hFile, &m_Type, 4, &dwRWC, NULL); // Plug Type
	WriteFile(hFile, &m_nJointIndex, 4, &dwRWC, NULL); // Plug Joint Index

	WriteFile(hFile, &m_vPosition, sizeof(m_vPosition), &dwRWC, NULL);
	WriteFile(hFile, &m_MtxRot, sizeof(m_MtxRot), &dwRWC, NULL);
	WriteFile(hFile, &m_vScale, sizeof(m_vScale), &dwRWC, NULL);

	WriteFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // 재질

	nL = 0;
	CN3PMesh* pPMesh = m_PMeshInst.GetMesh();
	if(pPMesh) nL = lstrlen(pPMesh->FileName());
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) WriteFile(hFile, pPMesh->FileName(), nL, &dwRWC, NULL);

	nL = 0;
	if(m_pTexRef) nL = lstrlen(m_pTexRef->FileName());
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) WriteFile(hFile, m_pTexRef->FileName(), nL, &dwRWC, NULL);

	WriteFile(hFile, &m_nTraceStep, 4, &dwRWC, NULL); // 궤적 갯수..
	if(m_nTraceStep > 0 && m_nTraceStep <= MAX_PLUG_TRACE_VERTEX/2 - 1)
	{
		WriteFile(hFile, &m_crTrace, 4, &dwRWC, NULL); // 궤적 색깔.. 검은색이면 없다..
		WriteFile(hFile, &m_fTrace0, 4, &dwRWC, NULL); // 궤적 위치..
		WriteFile(hFile, &m_fTrace1, 4, &dwRWC, NULL); // 궤적 위치..
	}
	else m_nTraceStep = 0;

	return 0;
}
// Part, Plug....
////////////////////////////////






//float CN3Chr::m_fDistLODs[MAX_CHR_LOD] = { 7.0f, 21.0f, 63.0f, 189.0f };
//float CN3Chr::s_fDistLODs[MAX_CHR_LOD] = { 4.0f, 15.0f, 34.0f, 95.0f, 150.0f };
float CN3Chr::s_fDistLODs[MAX_CHR_LOD] = { 4.0f, 12.0f, 26.0f, 80.0f };
CN3Mesh* CN3Chr::s_pBoxMesh = NULL;

CN3Chr::CN3Chr()
{
	m_dwType |= OBJ_CHARACTER;

	m_pJointRef = NULL;
	m_pMtxInverses = NULL;
	m_pMtxs = NULL;
//	m_pSkinCollision = NULL;
	
/*	m_vCollisionBoxes[0].Set(-0.3f, 1.8f, -0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[1].Set( 0.3f, 1.8f, -0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[2].Set( 0.3f, 0.0f, -0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[3].Set(-0.3f, 0.0f, -0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[4].Set( 0.3f, 1.8f,  0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[5].Set(-0.3f, 1.8f,  0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[6].Set(-0.3f, 0.0f,  0.15f); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[7].Set( 0.3f, 0.0f,  0.15f); // 충돌 체크를 위한 폴리곤

	m_wCollisionIndices[0] = 0; m_wCollisionIndices[1] = 1; m_wCollisionIndices[2] = 2; // 충돌 체크를 위한 폴리곤 인덱스 - 정면
	m_wCollisionIndices[3] = 0; m_wCollisionIndices[4] = 2; m_wCollisionIndices[5] = 3; // 충돌 체크를 위한 폴리곤 인덱스
	m_wCollisionIndices[6] = 1; m_wCollisionIndices[7] = 4; m_wCollisionIndices[8] = 7; // 충돌 체크를 위한 폴리곤 인덱스 - 오른쪽
	m_wCollisionIndices[9] = 1; m_wCollisionIndices[10] = 2; m_wCollisionIndices[11] = 2; // 충돌 체크를 위한 폴리곤 인덱스 
	m_wCollisionIndices[12] = 5; m_wCollisionIndices[13] = 0; m_wCollisionIndices[14] = 3; // 충돌 체크를 위한 폴리곤 인덱스 - 왼쪽
	m_wCollisionIndices[15] = 5; m_wCollisionIndices[16] = 3; m_wCollisionIndices[17] = 2; // 충돌 체크를 위한 폴리곤 인덱스
	m_wCollisionIndices[18] = 4; m_wCollisionIndices[19] = 5; m_wCollisionIndices[20] = 6; // 충돌 체크를 위한 폴리곤 인덱스 - 뒷쪽
	m_wCollisionIndices[21] = 4; m_wCollisionIndices[22] = 6; m_wCollisionIndices[23] = 7; // 충돌 체크를 위한 폴리곤 인덱스
	m_wCollisionIndices[24] = 5; m_wCollisionIndices[25] = 4; m_wCollisionIndices[26] = 1; // 충돌 체크를 위한 폴리곤 인덱스 - 위쪽
	m_wCollisionIndices[27] = 5; m_wCollisionIndices[28] = 1; m_wCollisionIndices[29] = 0; // 충돌 체크를 위한 폴리곤 인덱스
	m_wCollisionIndices[30] = 3; m_wCollisionIndices[31] = 2; m_wCollisionIndices[32] = 7; // 충돌 체크를 위한 폴리곤 인덱스 - 아래쪽
	m_wCollisionIndices[33] = 4; m_wCollisionIndices[34] = 7; m_wCollisionIndices[35] = 6; // 충돌 체크를 위한 폴리곤 인덱스
*/
	this->Release();
}

CN3Chr::~CN3Chr()
{
	CN3Chr::Release();
}

void CN3Chr::Release()
{
	m_nLOD = 0;

	s_MngJoint.Delete(m_pJointRef); m_pJointRef = NULL;
	delete [] m_pMtxInverses; m_pMtxInverses = NULL;
	delete [] m_pMtxs; m_pMtxs = NULL;

	for(void* pPos = m_Parts.Position(0);pPos;m_Parts.Next(pPos))
	{
		s_MngCPart.Delete(m_Parts.Get(pPos));
	}
	m_Parts.Release();

	pPos = m_Plugs.Position(0);
	for(;pPos;m_Plugs.Next(pPos)) delete m_Plugs.Get(pPos);
	m_Plugs.Release();

//	s_MngSkin.Delete(m_pSkinCollision);
//	m_pSkinCollision = NULL;

	// 무기 궤적
	memset(m_vPlugTraces, 0, sizeof(m_vPlugTraces));
	memset(m_bRenderPlugTraces, 0, sizeof(m_bRenderPlugTraces));
	memset(m_dwTickPlugPrevs, 0, sizeof(m_dwTickPlugPrevs));

	// Animation Control
	s_MngAniCtrl.Delete(m_pAniCtrlRef); m_pAniCtrlRef = NULL;
	m_pAniCur = NULL;

	m_fFrmCurUpper = 0; // 현재 프레임 0 - 상체
	m_fFrmCurLower = 0; // 현재 프레임 1 - 하체

	m_nAniCur = -1; // 현재 에니메이션
	m_nAniPrev = -1; // 전의 에니메이션..
	m_nAniLoopCountCur = 0; // 에니메이션 루핑 카운트..

	m_fFrmBlendUpper = 0.0f; // 블렌딩할 에니메이션 데이터 포인터..
	m_fFrmBlendLower = 0.0f; // 블렌딩할 에니메이션 데이터 포인터..
	m_fBlendTimeCur = 0.0f; // 현재 블렌딩 시간..
	m_fBlendTime = 0.0f; // 블렌딩할 시간. (초단위)

	// 충돌 메시는 캐릭터에서는 지운다..
	delete m_pMeshCollision; m_pMeshCollision = NULL;

	CN3TransformCollision::Release();
}

bool CN3Chr::Load(HANDLE hFile)
{
	if(m_pJointRef != NULL) CN3Chr::Release();

	CN3TransformCollision::Load(hFile);

	DWORD dwRWC = 0;
	int nL = 0;
	char szFN[512] = "";
	
	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
	m_pJointRef = s_MngJoint.Get(szFN);
	__ASSERT(m_pJointRef, "Joint pointer is NULL!");

	int nPartCount = 0;
	ReadFile(hFile, &nPartCount, 4, &dwRWC, NULL);

	for(int i = 0; i < nPartCount; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
			CN3CPart* pPart = s_MngCPart.Get(szFN);
			m_Parts.Add(pPart);
		}
	}
	
	int nPlugCount = 0;
	ReadFile(hFile, &nPlugCount, 4, &dwRWC, NULL);

	for(i = 0; i < nPlugCount; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
			CN3CPlug* pPlug = new CN3CPlug();
			if(false == pPlug->LoadFromFile(szFN))
			{
				delete pPlug;
				continue;
			}

			m_Plugs.Add(pPlug);
		}
	}

	this->RemakePlugTracePolygons(); // Plug Trace 에 쓰이는 폴리곤 초기화...

//	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
//	if(nL > 0) 
//	{
//		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
//		s_MngSkin.Delete(m_pSkinCollision);
//		m_pSkinCollision = s_MngSkin.Get(szFN);
//	}

	// Animation Control..
	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		this->AniCtrlSet(szFN);
		this->AniCurSet(0);
	}

	this->Init(); // 관절 및 Vertex 들을 초기화 시켜준다.. 그리고 최대 최소값을 찾고... 충돌 박스 메시를 초기화 시킨다..

	return 0;
}

bool CN3Chr::Save(HANDLE hFile)
{
	CN3TransformCollision::Save(hFile);

	DWORD dwRWC = 0;
	int nL = 0;
	
	if(m_pJointRef) nL = lstrlen(m_pJointRef->FileName());
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) WriteFile(hFile, m_pJointRef->FileName(), nL, &dwRWC, NULL);

	// 내용이 없는 Part Data는 걸러낸다..
	for(void* pPos = m_Parts.Position(0); pPos; )
	{
		bool bIsNullData = true;
		
		CN3CPart* pPart = m_Parts.Get(pPos);
		if(NULL != pPart)
		{
			for(int j = 0; j < MAX_CHR_LOD; j++)
			{
				if(pPart->Skin(j)->VertexCount() > 0 && pPart->Mesh(j)->VertexCount() > 0)
				{
					bIsNullData = false;
					break;
				}
			}
		}

		if(bIsNullData)
		{
			s_MngCPart.Delete(pPart);
			m_Parts.Delete(pPos);
		}
		else
		{
			m_Parts.Next(pPos);
		}
	}

	int nPartCount = m_Parts.Count();
	WriteFile(hFile, &nPartCount, 4, &dwRWC, NULL);

	pPos = m_Parts.Position(0);
	for(; pPos; m_Parts.Next(pPos))
	{
		CN3CPart* pPart = m_Parts.Get(pPos);
		nL = lstrlen(pPart->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			WriteFile(hFile, pPart->FileName(), nL, &dwRWC, NULL);
			pPart->SaveToFile();
		}
	}
	
	int nPlugCount = m_Plugs.Count();
	WriteFile(hFile, &nPlugCount, 4, &dwRWC, NULL);

	for(int i = 0; i < nPlugCount; i++)
	{
		CN3CPlug* pPlug = m_Plugs.Get(i);

		nL = lstrlen(pPlug->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 0)
		{
			WriteFile(hFile, pPlug->FileName(), nL, &dwRWC, NULL);
			pPlug->SaveToFile();
		}
		else
		{
			__ASSERT(0, "Plug 이름을 정해 주세요");
		}
	}
	
//	nL = 0;
//	if(m_pSkinCollision) nL = lstrlen(m_pSkinCollision->Name());
//	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
//	if(nL > 0) 
//	{
//		WriteFile(hFile, m_pSkinCollision->Name(), nL, &dwRWC, NULL);
//	}
	
	// Animation Control..
	nL = 0;
	if(m_pAniCtrlRef) nL = lstrlen(m_pAniCtrlRef->FileName());
	WriteFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0)
	{
		WriteFile(hFile, m_pAniCtrlRef->FileName(), nL, &dwRWC, NULL);
	}

	return true;
}

void CN3Chr::Tick(float fFrm)
{
	if(NULL == m_pJointRef)
	{
		m_nLOD = -1;
		return;
	}

	// 카메라와 멀리 떨어지면 지나간다..
	float fDist = (m_vPos - s_CameraData.vEye).Magnitude();
	if(fDist > s_CameraData.fFP + m_fRadius)
	{
		m_nLOD = -1;
		return;
	}

	static __Vector3 vPos2;
	vPos2 = m_vPos;
	vPos2.y += 1.8f;
	float fOffset = m_fRadius * 1.2f + 2.0f;
	if(	s_CameraData.IsOutOfFrustum(m_vPos, fOffset) &&
		s_CameraData.IsOutOfFrustum(vPos2, fOffset) ) // 카메라 사면체 바깥이면 지나간다.. 현 지점과 머리 끝점을 조사..
	{
		m_nLOD = -1;
		return;
	}

	// LOD 결정...
	m_nLOD = MAX_CHR_LOD;
	float fLOD = fDist * s_CameraData.fFOV;
	bool bDecideLOD = false;
	for(int i = 0; i < MAX_CHR_LOD; i++)
	{
		if(fLOD < s_fDistLODs[i])
		{
			CN3CPart* pPart = m_Parts.Get(0);
			m_nLOD = i;
			bDecideLOD = true;
			break;
		}
	}

	if(m_nLOD >= 0 && m_nLOD < MAX_CHR_LOD)
	{
		if(m_pAniCur && fFrm == FRAME_SELFPLAY) // 현재 설정된 에니메이션 대로 자동 플레이
		{
			bool bBlendTick = false;

			if(0.0f != m_fBlendTime) // Blending 할 에니메이션이 있으면..
			{
				// 가속 적용하는 부분.. 블렌딩 시작 시점엔 느리게.. 중간점일수록 빠르게 끝시점엔 느리게..
				float fBlendHalf = m_fBlendTime / 2.0f;
				float fBlendDelta = 1.0f;
				if(m_fBlendTimeCur <= fBlendHalf)
				{
					fBlendDelta = (m_fBlendTimeCur / fBlendHalf) + 0.7f;
				}
				else
				{
					fBlendDelta += ((m_fBlendTime - m_fBlendTimeCur) / fBlendHalf) + 0.7f;
				}
				
				m_fBlendTimeCur += fBlendDelta / s_fFrmPerSec;

				if(m_fBlendTimeCur > m_fBlendTime) // Blending 이 끝났다.
				{
					m_fBlendTime = 0.0f;
					m_fBlendTimeCur = 0.0f;
					m_fFrmCurUpper = m_pAniCur->fFrmStartUpper;
				}
				else
				{
					bBlendTick = true;
				}
			}
			else
			{
				m_fFrmCurUpper += m_pAniCur->fFrmPerSecUpper / s_fFrmPerSec;
				if(m_fFrmCurUpper < m_pAniCur->fFrmStartUpper) m_fFrmCurUpper = m_pAniCur->fFrmStartUpper;
				if(m_fFrmCurUpper >= m_pAniCur->fFrmEndUpper)
				{
					m_fFrmCurUpper = m_pAniCur->fFrmStartUpper + (m_fFrmCurUpper - m_pAniCur->fFrmEndUpper);
					if(m_nAniLoopCountCur > 0) // Animation Loop Count 가 지정되어 있으면..
					{
						m_nAniLoopCountCur--;
						if(m_nAniLoopCountCur <= 0) // 지정된 수 만큼 루핑이 끝나면..
						{
							float fFrmPrev = m_pAniCur->fFrmEndUpper;
							this->AniCurSet(m_nAniPrev, 0, 0.3f); // 전의 에니메이션으로 돌린다..
							m_fFrmBlendUpper = fFrmPrev;
							bBlendTick = true;
						}
					}
				}

				m_fFrmCurLower += m_pAniCur->fFrmPerSecLower / s_fFrmPerSec;
				if(m_fFrmCurLower <= m_pAniCur->fFrmStartLower) m_fFrmCurLower = m_pAniCur->fFrmStartLower;
				if(m_fFrmCurLower >= m_pAniCur->fFrmEndLower)
					m_fFrmCurLower = m_pAniCur->fFrmStartLower + (m_fFrmCurLower - m_pAniCur->fFrmEndUpper);
			}

			if(bBlendTick)
			{
				float fBlendFactor = 1.0f - m_fBlendTimeCur / m_fBlendTime;
				int nJI = 0;
				m_pJointRef->MatricesBlendGet(m_pMtxs, nJI, m_fFrmBlendUpper, m_fFrmCurUpper, fBlendFactor); // Joint Animation Blending...
			}
			else
			{
				m_pJointRef->Tick(m_fFrmCurUpper); // Joint Animation...
				int nJI = 0;
				m_pJointRef->MatricesGet(m_pMtxs, nJI); // Matrix 계산..
				
				fFrm = m_fFrmCurUpper; // 전의 프레임을 기억해놓는다..
			}
		}
		else
		{
			m_pJointRef->Tick(fFrm); // Joint Animation...
			int nJI = 0;
			m_pJointRef->MatricesGet(m_pMtxs, nJI); // Matrix 계산..
		}
		

		int nPlugTrace = 0;
		__Vector3 vTrace;
		CN3CPlug* pPlug = NULL;
		for(void* pPos = m_Plugs.Position(0); pPos; m_Plugs.Next(pPos))
		{
			pPlug = m_Plugs.Get(pPos);
			pPlug->m_PMeshInst.SetLOD(fLOD);

			if(NULL == m_pAniCur) continue;
			if(pPlug->m_nTraceStep <= 0) continue;
			if(nPlugTrace >= MAX_PLUG_TRACE) continue;

			if(fFrm >= m_pAniCur->fFrmPlugTraceStart0 && fFrm <= m_pAniCur->fFrmPlugTraceEnd0) // 궤적 프레임의 안에 있으면..
			{
				m_bRenderPlugTraces[nPlugTrace] = true;
				if(m_fFrmPrev <= m_pAniCur->fFrmPlugTraceStart0) // 시작점에 오면..
				{
					vTrace.Set(0, pPlug->m_fTrace0, 0);
					vTrace *= pPlug->m_Matrix;
					vTrace *= m_pMtxs[pPlug->m_nJointIndex];
					m_vPlugTraces[nPlugTrace][0].v = vTrace;
					
					vTrace.Set(0, pPlug->m_fTrace1, 0);
					vTrace *= pPlug->m_Matrix;
					vTrace *= m_pMtxs[pPlug->m_nJointIndex];
					m_vPlugTraces[nPlugTrace][1].v = vTrace;

					// 무기 궤적 계산..
					for(i = 1; i <= pPlug->m_nTraceStep; i++)
					{
						m_vPlugTraces[nPlugTrace][i*2].v = m_vPlugTraces[nPlugTrace][0].v;
						m_vPlugTraces[nPlugTrace][i*2+1].v = m_vPlugTraces[nPlugTrace][1].v;
					}
				}
				else
				{
					DWORD dwTick = timeGetTime();
					if(dwTick > m_dwTickPlugPrevs[nPlugTrace] + 20)
					{
						// 무기 궤적 계산.. 하나씩 뒤로 미룬다..
						for(i = pPlug->m_nTraceStep; i > 0; i--)
						{
							m_vPlugTraces[nPlugTrace][i*2].v = m_vPlugTraces[nPlugTrace][(i-1)*2].v;
							m_vPlugTraces[nPlugTrace][i*2+1].v = m_vPlugTraces[nPlugTrace][(i-1)*2+1].v;
						}

						m_dwTickPlugPrevs[nPlugTrace] = dwTick;
					}
					
					vTrace.Set(0, pPlug->m_fTrace0, 0);
					vTrace *= pPlug->m_Matrix;
					vTrace *= m_pMtxs[pPlug->m_nJointIndex];
					m_vPlugTraces[nPlugTrace][0].v = vTrace;
					
					vTrace.Set(0, pPlug->m_fTrace1, 0);
					vTrace *= pPlug->m_Matrix;
					vTrace *= m_pMtxs[pPlug->m_nJointIndex];
					m_vPlugTraces[nPlugTrace][1].v = vTrace;
				}
			}
			else
			{
				m_bRenderPlugTraces[nPlugTrace] = false;
			}

			nPlugTrace++;
		}
	}

	m_fFrmPrev = fFrm; // 전의 프레임을 기억해놓는다..
}

void CN3Chr::Render(float fFrm)
{
	if(NULL == m_pJointRef) return;
	if(m_nLOD < 0 || m_nLOD > MAX_CHR_LOD) return;

	CN3Base::s_RenderInfo.nChr++;

	if(m_nLOD == MAX_CHR_LOD)
	{
		static bool bCubeCreated = false;
		if(false == bCubeCreated)
		{
			s_pBoxMesh = new CN3Mesh();
			s_pBoxMesh->Create_Cube(__Vector3(-0.25f, 0, -0.15f), __Vector3(0.25f, 1.8f, 0.15f));
			bCubeCreated = true;
		}

		// 일부러 재질, 텍스처들을 적용하지 않았다..
		CN3CPart* pPart = m_Parts.Get(2);
		if(NULL == pPart) pPart = m_Parts.Get(0);
		if(NULL != pPart)
		{
			LPDIRECT3DTEXTURE8 lpTex = NULL;
			CN3Texture* pTex = pPart->Tex();
			if(pTex) lpTex = pTex->Get();
			if(lpTex)
			{
				s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
				s_lpD3DDev->SetTexture(0, lpTex);
				
				pTex->UpdateRenderInfo();
			}
			else
			{
				s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				s_lpD3DDev->SetTexture(0, NULL);
			}
			
			s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

			int nVC = s_pBoxMesh->VertexCount();
			__VertexT1* pVs = s_pBoxMesh->Vertices();
			
			s_lpD3DDev->SetVertexShader(FVF_VNT1);
			s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, nVC/3, pVs, sizeof(__VertexT1));
			s_RenderInfo.nPolygon += nVC / 3;
		}

		return;
	}

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
	this->BuildMesh(); // 행렬에 따라 점위치 계산..

	CN3CPart* pPart = NULL;
	void* pPos = NULL;

	for(pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPart = m_Parts.Get(pPos);
		pPart->Render(m_nLOD);
	}

	// Plug - 붙이는 부분 Rendering
	CN3CPlug* pPlug = NULL;
	int nPlugTrace = 0;
	for(pPos = m_Plugs.Position(0); pPos; m_Plugs.Next(pPos))
	{
		pPlug = m_Plugs.Get(pPos);
		pPlug->Render(m_Matrix, m_pMtxs[pPlug->m_nJointIndex]);

		if(pPlug->m_nTraceStep <= 0) continue;
		if(nPlugTrace >= MAX_PLUG_TRACE) continue;
		if(false == m_bRenderPlugTraces[nPlugTrace]) continue;

		// 궤적 그리기..
		DWORD dwPrevAlpha, dwCull, dwLgt, dwPrevAlphaOp, dwA1;
		s_lpD3DDev->GetRenderState( D3DRS_CULLMODE, &dwCull);
		s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwPrevAlpha);
		s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwLgt);
		s_lpD3DDev->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwPrevAlphaOp);
		s_lpD3DDev->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwA1 );

		
		s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		s_lpD3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE);

		s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCCOLOR);
		s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_ONE);

		
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		
		//alpha = select texture alpha
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

		
		s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
		s_lpD3DDev->SetVertexShader(FVF_CV);
		
		s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, pPlug->m_nTraceStep * 2, m_vPlugTraces[nPlugTrace], sizeof(__VertexColor));
			
		s_lpD3DDev->SetRenderState( D3DRS_CULLMODE, dwCull);
		s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwPrevAlpha);
		s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwLgt);
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwPrevAlphaOp);
		s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwA1 );

		nPlugTrace++;
	}
}

void CN3Chr::Render2(float fFrm)
{
	if(m_nLOD < 0 || m_nLOD >= MAX_CHR_LOD) return;
	__ASSERT(m_pJointRef, "Joint pointer is NULL!");

	CN3Base::s_RenderInfo.nChr++;

	this->BuildMesh(); // 행렬에 따라 점위치 계산..

	// Plug - 붙이는 부분 Rendering
	CN3CPlug* pPlug = NULL;
	void* pPos = m_Plugs.Position(0);
	pPlug = m_Plugs.Get(pPos);
	if(pPlug) pPlug->Render(m_Matrix, m_pMtxs[pPlug->m_nJointIndex]);
}

void CN3Chr::BuildMesh()
{
	if(m_nLOD < 0 || m_nLOD >= MAX_CHR_LOD) return;
	__ASSERT(m_pJointRef, "Joint pointer is NULL!");

	float fWeight = 0;
	int nJIndex = 0, nAffect = 0;

	int j = 0, k = 0, nVC = 0;
	CN3CPart* pPart = NULL;
	CN3IMesh* pIMesh = NULL;
	CN3Skin* pSkin = NULL;

	for(void* pPos = m_Parts.Position(0); pPos; m_Parts.Next(pPos))
	{
		pPart = m_Parts.Get(pPos);

		pIMesh = pPart->Mesh(m_nLOD);
		pSkin = pPart->Skin(m_nLOD);

		if(NULL == pIMesh || NULL == pSkin) continue;

		__VertexXyzNormal* pVDest = pIMesh->Vertices();
		__VertexSkinned* pVSrc = pSkin->Vertices();

		nVC = pSkin->VertexCount();
		__Vector3 vFinal;
		int nAffect = 0;
		float* pfWeights = NULL;
		for(j = 0; j < nVC; j++) // j < m_nBoneVertices 와 같다..
		{
			nAffect = pVSrc[j].nAffect;
			if(1 == nAffect)
			{
				// 단일 뼈대...
				nJIndex = pVSrc[j].pnJoints[0];
				pVDest[j].v = (pVSrc[j].vOrigin * m_pMtxInverses[nJIndex]) * m_pMtxs[nJIndex];
			}
			else if(nAffect > 1) 
			{
				vFinal.Set(0,0,0);
				pfWeights = pVSrc[j].pfWeights;
				for(k = 0; k < nAffect; k++)
				{
					if(pfWeights[k] <= 0) continue;

					nJIndex = pVSrc[j].pnJoints[k];
					vFinal += ((pVSrc[j].vOrigin * m_pMtxInverses[nJIndex]) * m_pMtxs[nJIndex]) * pfWeights[k];
				}
				pVDest[j].v = vFinal;
			}
		}
	}

/*	if(m_pMeshCollision && m_pSkinCollision)
	{
		__ASSERT(m_pMeshCollision->IndexCount() > 0, "Indices count is less than 0");

		__Vector3* pVDest = m_pMeshCollision->Vertices();
		__VertexSkinned* pVSrc = m_pSkinCollision->Vertices();

		nVC = m_pMeshCollision->VertexCount();
		int nSVC = m_pSkinCollision->VertexCount();
		if(nSVC != nVC)
		{
			__ASSERT(0, "Mesh Vertices count is different to Skin vertices count");
		}
		else
		{
			int nAffect = 0;
			float* pfWeights = NULL;
			for(j = 0; j < nVC; j++) // j < m_nBoneVertices 와 같다..
			{
				nAffect = pVSrc[j].nAffect;
				if(1 == nAffect)
				{
					// 단일 뼈대...
					nJIndex = pVSrc[j].pnJoints[0];
					pVDest[j] = (pVSrc[j].vOrigin * m_pMtxInverses[nJIndex]) * m_pMtxs[nJIndex];
				}
				else if(nAffect > 1) 
				{
					pVDest[j].Zero();
					pfWeights = pVSrc[j].pfWeights;
					for(k = 0; k < nAffect; k++)
					{
						if(pfWeights[k] <= 0) continue;

						nJIndex = pVSrc[j].pnJoints[k];
						pVDest[j] += ((pVSrc[j].vOrigin * m_pMtxInverses[nJIndex]) * m_pMtxs[nJIndex]) * pfWeights[k];
					}
				}
			}
		}
	}
*/

}

void CN3Chr::Init()
{
	if(NULL == m_pJointRef) return;

	// 관절이 몇개 있는지 계산하고.. 포인터 할당..
	int nJC = 0;
	m_pJointRef->NodeCount(nJC);
	delete [] m_pMtxInverses; m_pMtxInverses = new __Matrix44[nJC];
	delete [] m_pMtxs; m_pMtxs = new __Matrix44[nJC];
	
	int nJI = 0;
	m_pJointRef->Tick(0); // 초기에 관절 위치 계산..
	m_pJointRef->MatricesGet(m_pMtxs, nJI);
	
	if(nJC > 0)
	{
		m_vMin.Set(100000.0f, 100000.0f, 100000.0f);
		m_vMax.Set(-100000.0f, -100000.0f, -100000.0f);

		__Vector3 vTmp;
		for(int i = 0; i < nJC; i++) // 관절 갯수 만큼 각 관절의 참조 포인터와 역행렬을 얻어놓는다..
		{
			float fTmp = 1.0f;
			::D3DXMatrixInverse(&m_pMtxInverses[i], &fTmp, &m_pMtxs[i]); // 로컬 축에 대한 역행렬을 구한다..

			// 각 조인트의 관절 위치 값에 대해 최대 최소값을 찾는다.
			vTmp = m_pMtxs[i].Pos();
			if(vTmp.x < m_vMin.x) m_vMin.x = vTmp.x;
			if(vTmp.y < m_vMin.y) m_vMin.y = vTmp.y;
			if(vTmp.z < m_vMin.z) m_vMin.z = vTmp.z;
			if(vTmp.x > m_vMax.x) m_vMax.x = vTmp.x;
			if(vTmp.y > m_vMax.y) m_vMax.y = vTmp.y;
			if(vTmp.z > m_vMax.z) m_vMax.z = vTmp.z;
		}

		m_fRadius = (m_vMax - m_vMin).Magnitude(); // 반지름 정하기..
	}
	else
	{
		m_vMin.Zero();
		m_vMax.Zero();
		m_fRadius = 0.0f;
	}

	// 충돌 체크를 위한 폴리곤.. 크기에 맞게 변환..
	if(NULL == m_pMeshCollision) m_pMeshCollision = new CN3VMesh();
	m_pMeshCollision->CreateCube(m_vMin, m_vMax);

/*	m_vCollisionBoxes[0].Set(vMin.x, vMax.y, vMin.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[1].Set(vMax.x, vMax.y, vMin.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[2].Set(vMax.x, vMin.y, vMin.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[3].Set(vMin.x, vMin.y, vMin.z); // 충돌 체크를 위한 폴리곤
	
	m_vCollisionBoxes[4].Set(vMax.x, vMax.y, vMax.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[5].Set(vMin.x, vMax.y, vMax.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[6].Set(vMin.x, vMin.y, vMax.z); // 충돌 체크를 위한 폴리곤
	m_vCollisionBoxes[7].Set(vMax.x, vMin.y, vMax.z); // 충돌 체크를 위한 폴리곤
*/
}

void CN3Chr::JointSet(const char* szFN)
{
	if(m_pJointRef) s_MngJoint.Delete(m_pJointRef);
	m_pJointRef = s_MngJoint.Get(szFN);

	this->Init(); // 초기화...
}

int CN3Chr::PartAdd()
{
	CN3CPart* pPD = new CN3CPart();
	
	char szPart[256] = "";
	wsprintf(szPart, "Default_%d", m_Parts.Count());
	pPD->NameSet(szPart);
	wsprintf(szPart, "chr\\Default_%d.N3CPart", m_Parts.Count());
	pPD->FileNameSet(szPart);

	s_MngCPart.Add(pPD);
	return m_Parts.Add(pPD);
}

void CN3Chr::PartDelete(int nPart)
{
	void* pPos = m_Parts.Position(nPart);
	CN3CPart* pPart = m_Parts.Get(pPos);

	s_MngCPart.Delete(pPart);
	m_Parts.Delete(pPos);
}

void CN3Chr::PartSet(int nPart, const char *szFN)
{
	if(NULL == szFN) return;

	CN3CPart* pPart = m_Parts.Get(nPart);
	if(pPart == NULL || pPart->Name() == NULL) return;

	if(strcmp(szFN, pPart->Name()) == 0) return; // 같은 데이터면 넘어간다..

	s_MngCPart.Delete(pPart);
	pPart = s_MngCPart.Get(szFN);
	m_Parts.Set(nPart, pPart);
}

int CN3Chr::PlugAdd()
{
	CN3CPlug* pPlug = new CN3CPlug();
	return m_Plugs.Add(pPlug);
}

void CN3Chr::PlugDelete(int nPlug)
{
	void* pPos = m_Plugs.Position(nPlug);
	delete m_Plugs.Get(pPos);
	m_Plugs.Delete(pPos);
}

void CN3Chr::PlugSet(int nPlug, const char *szFN)
{
	if(NULL == szFN) return;
	
	CN3CPlug* pPlug = m_Plugs.Get(nPlug);
	if(pPlug == NULL || pPlug->Name() == NULL) return;
	if(strcmp(szFN, pPlug->Name()) == 0) return; // 같은 데이터면 넘어간다..

	pPlug->Release();
	if(lstrlen(szFN) == 0) return;

	pPlug->LoadFromFile(szFN);

	this->RemakePlugTracePolygons();
}

/*
void CN3Chr::CollisionSkinSet(const char *szFN)
{
	s_MngSkin.Delete(m_pSkinCollision);
	m_pSkinCollision = s_MngSkin.Get(szFN);
}
*/

void CN3Chr::RemakePlugTracePolygons()
{
	int nPlugTrace = 0; // 몇번째에 넣어야 하나...
	CN3CPlug* pPlug = NULL;
	for(void* pPos = m_Plugs.Position(0); pPos; m_Plugs.Next(pPos))
	{
		pPlug = m_Plugs.Get(pPos);
		if(pPlug->m_nTraceStep <= 0) continue;
			
		if(nPlugTrace >= MAX_PLUG_TRACE) return;
		if(pPlug->m_nTraceStep > MAX_PLUG_TRACE_VERTEX/2-2) return;

		DWORD dwA = (pPlug->m_crTrace & 0xff000000) >> 24;
		DWORD dwR = (pPlug->m_crTrace & 0x00ff0000) >> 16;
		DWORD dwG = (pPlug->m_crTrace & 0x0000ff00) >> 8;
		DWORD dwB = (pPlug->m_crTrace & 0x000000ff);
		for(int i = 0; i < pPlug->m_nTraceStep + 1; i++) // 단계적으로 색을 정해준다..
		{
			m_vPlugTraces[nPlugTrace][i*2+1].color = m_vPlugTraces[nPlugTrace][i*2].color =	
				((dwA * (pPlug->m_nTraceStep - i) / pPlug->m_nTraceStep) << 24) | 
				((dwR * (pPlug->m_nTraceStep - i) / pPlug->m_nTraceStep) << 16) | 
				((dwG * (pPlug->m_nTraceStep - i) / pPlug->m_nTraceStep) << 8) | 
				(dwB * (pPlug->m_nTraceStep - i) / pPlug->m_nTraceStep);
		}
	}
}

void CN3Chr::AniCtrlSet(const char* szFN)
{
	s_MngAniCtrl.Delete(m_pAniCtrlRef);
	m_pAniCtrlRef = s_MngAniCtrl.Get(szFN);
	
	m_nAniCur = m_nAniPrev = 0;
	m_pAniCur = NULL;
	if(m_pAniCtrlRef) m_pAniCur = m_pAniCtrlRef->DataGet(m_nAniCur);
	m_fFrmCurUpper = 0;
	m_fFrmCurLower = 0;
}

int CN3Chr::AniCurSet(int nAni, int nLoopCount, float fBlendTime)
{
	if(NULL == m_pAniCtrlRef) return -1;
	if(nAni == m_nAniCur) return nAni;

	int nAniPrev = m_nAniCur;
	int nCount = m_pAniCtrlRef->m_AniList.Count();
	if(nAni >= 0 && nAni < nCount)
	{
		m_nAniPrev = m_nAniCur;
		m_nAniCur = nAni;
		m_nAniLoopCountCur = nLoopCount;
		
		m_pAniCur = m_pAniCtrlRef->m_AniList.Get(nAni);
		
		m_fBlendTime = fBlendTime;
		m_fBlendTimeCur = 0.0f;

		if(fBlendTime > 0)
		{
			m_fFrmBlendUpper = m_fFrmCurUpper; // Blending 할 에니메이션 Frame..
		}
		else
		{
			m_fFrmBlendUpper = m_pAniCur->fFrmStartUpper; // Blending 할 에니메이션 Frame..
		}

		m_fFrmCurUpper = m_pAniCur->fFrmStartUpper;
		m_fFrmCurLower = m_pAniCur->fFrmStartLower;
	}

	return nAniPrev;
}
/*
int  CN3Chr::AniCurSet(const char *szName, int nLoopCount)
{
	if(NULL == m_pAniCtrlRef) return -1;
	if(m_pAniCur && 0 == lstrcmpi(szName, m_pAniCur->Name())) return m_nAniCur;

	int nAniPrev = m_nAniCur;

	CN3AnimControl::__AnimData* pData;

	int nAni = 0;
	for(void* pPos = m_pAniCtrlRef->m_AniList.Position(0); pPos; m_pAniCtrlRef->m_AniList.Next(pPos))
	{
		pData = m_pAniCtrlRef->m_AniList.Get(pPos);

		if(lstrcmpi(szName, pData->Name()) == 0)
		{
			return this->AniCurSet(nAni, nLoopCount);
		}
		nAni++;
	}

	return -1;
}
*/

void CN3Chr::AniDefaultSet()
{
	// 기본적인 Animation Control 만들기..
	if(s_MngAniCtrl.IsExist("Chr\\Default.N3Anim") == false)
	{
		CN3AnimControl* pAniCtrlDefault = new CN3AnimControl();
		pAniCtrlDefault->NameSet("Default");
		pAniCtrlDefault->FileNameSet("Chr\\Default.N3Anim");
		s_MngAniCtrl.Add(pAniCtrlDefault);
	}
	
	s_MngAniCtrl.Delete(m_pAniCtrlRef);
	m_pAniCtrlRef = s_MngAniCtrl.Get("Chr\\Default.N3Anim");
}

bool CN3Chr::CheckCollisionByBox(int xScreen, int yScreen)
{
	if(NULL == m_pMeshCollision) return false;
	return m_pMeshCollision->CheckCollision(&m_Matrix, xScreen, yScreen);

/*	// Compute the vector of the pick ray in screen space
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

	static __Vector3 svColls[8];
	for(int i = 0; i < 8; i++)
	{
		svColls[i] = m_vCollisionBoxes[i] * m_Matrix; // 로컬 충돌 박스를 월드 행렬로 곱해주고..
	}

	int nCI0, nCI1, nCI2; // Collision polygon index
	for(i = 0; i < 12; i++)
	{
		nCI0 = m_wCollisionIndices[i*3+0];
		nCI1 = m_wCollisionIndices[i*3+1];
		nCI2 = m_wCollisionIndices[i*3+2];

		if(::IntersectTriangle(vPos, vDir, svColls[nCI0], svColls[nCI1], svColls[nCI2])) // 첫째 벡터가 걸치면..
		{
			return true;
		}
	}

	return false;
*/
}

bool CN3Chr::CheckCollisionByBox(__Vector3& v0, __Vector3& v1, __Vector3 &vCollision, __Vector3 &vNormal)
{
	if(NULL == m_pMeshCollision) return false;
	return m_pMeshCollision->CheckCollision(&m_Matrix, v0, v1, vCollision, vNormal);

/*	__Vector3 vDir = v1 - v0;
	static __Vector3 svColls[8];
	for(int i = 0; i < 8; i++)
	{
		svColls[i] = m_vCollisionBoxes[i] * m_Matrix; // 로컬 충돌 박스를 월드 행렬로 곱해주고..
	}

	int nCI0, nCI1, nCI2; // Collision polygon index
	for(i = 0; i < 12; i++)
	{
		nCI0 = m_wCollisionIndices[i*3+0];
		nCI1 = m_wCollisionIndices[i*3+1];
		nCI2 = m_wCollisionIndices[i*3+2];

		if(::IntersectTriangle(v0, vDir, svColls[nCI0], svColls[nCI1], svColls[nCI2])) // 첫째 벡터가 걸치면..
		{
			if(false == ::IntersectTriangle(v1, vDir, svColls[nCI0], svColls[nCI1], svColls[nCI2])) // 둘째는 안 걸치면..
			{
				// 충돌이다..
				float fT, fU, fV;
				::IntersectTriangle(v0, vDir, svColls[nCI0], svColls[nCI1], svColls[nCI2], fT, fU, fV, &vCollision);
				// 법선 벡터 구하기..
				vNormal.Cross(svColls[nCI1] - svColls[nCI0], svColls[nCI2] - svColls[nCI1]);
				return true;
			}
		}

	}

	return false;
*/
}

