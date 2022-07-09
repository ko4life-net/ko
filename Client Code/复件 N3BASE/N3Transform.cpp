// N3Obj.cpp: implementation of the CN3 class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Transform.h"
#include "N3Mesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CN3Transform::CN3Transform()
{
	m_dwType |= OBJ_TRANSFORM;

	CN3Transform::Release();
}

CN3Transform::~CN3Transform()
{
	CN3Transform::Release();
}

void CN3Transform::Release()
{
	m_vPos.Set(0,0,0); // 위치, 스케일, 회전 벡터. 
	m_vScale.Set(1,1,1);
	__Vector3 vv(1,1,1);
	m_qRot.Identity();

//	.x = m_qRot.y = m_qRot.z = m_qRot.w = 0;

	m_Matrix.Identity();

	// 에니메이션 키
	m_fFrmCur = 0;
	m_fFrmWhole = 0;

	m_KeyPos.Release();
	m_KeyRot.Release();
	m_KeyScale.Release();

	CN3Base::Release();
}

bool CN3Transform::Load(HANDLE hFile)
{
	CN3Base::Load(hFile);

	DWORD dwRWC = 0;
	ReadFile(hFile, &m_vPos, sizeof(__Vector3), &dwRWC, NULL); // 위치, 스케일, 회전 벡터. 
	ReadFile(hFile, &m_qRot, sizeof(__Quaternion), &dwRWC, NULL);
	ReadFile(hFile, &m_vScale, sizeof(__Vector3), &dwRWC, NULL);

	// 에니메이션 키
	m_KeyPos.Load(hFile);
	m_KeyRot.Load(hFile);
	m_KeyScale.Load(hFile);

	m_fFrmCur = 0;
	m_fFrmWhole = 0;

	float fFrmWhole = 0;
	
	fFrmWhole = m_KeyPos.Count() * m_KeyPos.SamplingRate() / 30.0f;
	if(fFrmWhole > m_fFrmWhole) m_fFrmWhole = fFrmWhole;
	fFrmWhole = m_KeyRot.Count() * m_KeyRot.SamplingRate() / 30.0f;
	if(fFrmWhole > m_fFrmWhole) m_fFrmWhole = fFrmWhole;
	fFrmWhole = m_KeyScale.Count() * m_KeyScale.SamplingRate() / 30.0f;
	if(fFrmWhole > m_fFrmWhole) m_fFrmWhole = fFrmWhole;

	this->ReCalcMatrix(); // 변환 행렬 계산..

	return true;
}

bool CN3Transform::Save(HANDLE hFile)
{
	CN3Base::Save(hFile);

	DWORD dwRWC = 0;
	WriteFile(hFile, &m_vPos, sizeof(__Vector3), &dwRWC, NULL); // 위치, 스케일, 회전 벡터. 
	WriteFile(hFile, &m_qRot, sizeof(__Quaternion), &dwRWC, NULL);
	WriteFile(hFile, &m_vScale, sizeof(__Vector3), &dwRWC, NULL);

	// 에니메이션 키
	m_KeyPos.Save(hFile);
	m_KeyRot.Save(hFile);
	m_KeyScale.Save(hFile);

	return true;
}

void CN3Transform::Tick(float fFrm)
{
	if(FRAME_SELFPLAY == fFrm)
	{
		m_fFrmCur += 30.0f / s_fFrmPerSec;
		if(m_fFrmCur < 0) m_fFrmCur = 0.0f;
		if(m_fFrmCur >= m_fFrmWhole) m_fFrmCur = 0.0f;
		fFrm = m_fFrmCur;
	}
	else
	{
		m_fFrmCur = fFrm;
	}

	bool bNdeedRecalcMatrix = this->TickAnimationKey(m_fFrmCur);
	
	if(m_dwType & OBJ_JOINT) return; // Joint 일 경우는 행렬을 계산하는 방법이 다르기 땜시 넘어간다..

	if(bNdeedRecalcMatrix) this->ReCalcMatrix();
}

void CN3Transform::ReCalcMatrix()
{
	m_Matrix.Scale(m_vScale);
	if(m_qRot.w != 0)
	{
		static __Matrix44 mtxRot;
		D3DXMatrixRotationQuaternion(&mtxRot, &m_qRot);
		m_Matrix *= mtxRot;
	}
	m_Matrix.PosSet(m_vPos);
}

bool CN3Transform::TickAnimationKey(float fFrm)
{
	// 에니메이션 키
	int nKCP = m_KeyPos.Count();
	int nKCR = m_KeyRot.Count();
	int nKCS = m_KeyScale.Count();
	if(nKCP <= 0 && nKCR <= 0 && nKCS <= 0) return false;

	bool bNeedRecalcMatrix = false;
	int nFrm = (int)fFrm;
	if(m_KeyPos.DataGet(m_fFrmCur, m_vPos) == true) bNeedRecalcMatrix = true;
	if(m_KeyRot.DataGet(m_fFrmCur, m_qRot) == true) bNeedRecalcMatrix = true;
	if(m_KeyScale.DataGet(m_fFrmCur, m_vScale) == true) bNeedRecalcMatrix = true;

	return bNeedRecalcMatrix;
}

void CN3Transform::Render(const __Matrix44* pMtxParent, float fUnitSize)
{
	DWORD dwAlpha, dwFog, dwLight; // , dwZ;
//	s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZ);
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	
//	if(dwZ) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	static __Matrix44 stm;
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		stm.Identity();
		smtl.Init();
		bInit = true;
	}

	stm = m_Matrix;
	if(pMtxParent) m_Matrix *= *pMtxParent;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &stm);
	s_lpD3DDev->SetMaterial(&smtl);
	s_lpD3DDev->SetTexture(0, NULL);

	// 축 그리기..
	static __VertexColor vAxis[12];
	static bool bAxisCreated = false;
	if(false == bAxisCreated)
	{
		CN3Mesh mesh;
		mesh.Create_Axis(1.0f);
		__VertexT1* pVSrc = mesh.Vertices();
		D3DCOLOR crAxis;
		for(int i = 0; i < 12; i++)
		{
			if(i < 4) crAxis = 0xffff0000;
			else if(i < 8) crAxis = 0xff00ff00;
			else if(i < 12) crAxis = 0xff0000ff;

			vAxis[i].Set(pVSrc[i].x, pVSrc[i].y, pVSrc[i].z, crAxis);
		}

		bAxisCreated = true;
	}

	__Matrix44 mtxBox;
	mtxBox.Scale(fUnitSize, fUnitSize, fUnitSize); // 관절부 박스에 스케일 적용
	mtxBox *= m_Matrix;

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxBox);
	s_lpD3DDev->SetVertexShader(FVF_XYZCOLOR);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 6, vAxis, sizeof(__VertexColor)); // 선그리기..

//	if(dwZ) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZ);
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

/*
#if _DEBUG 
// 선택 상자 만들기.
void CN3Transform::GenerateSelectBox(__Vector3 &vMin, __Vector3 &vMax)
{
	// Bounding Box Vertex Buffer 생성
	if(m_lpVBBox != NULL) { m_lpVBBox->Release(); m_lpVBBox = NULL; }
	HRESULT rval = m_lpDevRef->CreateVertexBuffer(54 * sizeof(__VertexColor), 0, FVF_CV, D3DPOOL_MANAGED, &m_lpVBBox);
	if(rval != D3D_OK)
	{
		rval = m_lpDevRef->CreateVertexBuffer(54 * sizeof(__VertexColor), 0, FVF_CV, D3DPOOL_MANAGED, &m_lpVBBox);
		if(rval != D3D_OK)
		{
			char szDebug[256]; D3DXGetErrorString(rval, szDebug, 256);
			MessageBox(::GetActiveWindow(), szDebug, "VertexBuffer 생성 실패", MB_OK);
			return;
		}
	}

	float fW = (vMax.x - vMin.x)/5, fH = (vMax.y - vMin.y)/5, fL = (vMax.z - vMin.z)/5; // 길이 높이 너비 (x y z)

	__VertexColor* pVC;
	m_lpVBBox->Lock(0, 0, (BYTE**)&pVC, NULL);
	
	pVC[4] =  pVC[2] =  pVC[0] =  __VertexColor(vMin.x, vMax.y, vMin.z, 0xff7f7f7f);  // 정면 - 아래와 같은 선 리스트를 만든다.
	pVC[10] = pVC[8] =  pVC[6] =  __VertexColor(vMax.x, vMax.y, vMin.z, 0xff7f7f7f);  //  /          /
	pVC[16] = pVC[14] = pVC[12] = __VertexColor(vMax.x, vMin.y, vMin.z, 0xff7f7f7f);  // +--      --+
	pVC[22] = pVC[20] = pVC[18] = __VertexColor(vMin.x, vMin.y, vMin.z, 0xff7f7f7f);  // |          |
	pVC[28] = pVC[26] = pVC[24] = __VertexColor(vMax.x, vMax.y, vMax.z, 0xff7f7f7f);  // 
	pVC[34] = pVC[32] = pVC[30] = __VertexColor(vMin.x, vMax.y, vMax.z, 0xff7f7f7f);  // |/         |/
	pVC[40] = pVC[38] = pVC[36] = __VertexColor(vMin.x, vMin.y, vMax.z, 0xff7f7f7f);  // +--      --+
	pVC[46] = pVC[44] = pVC[42] = __VertexColor(vMax.x, vMin.y, vMax.z, 0xff7f7f7f);  // 

	pVC[1] =  pVC[0];  pVC[1].x  += fW;   pVC[3] =  pVC[2];  pVC[3].y  -= fH;   pVC[5] =  pVC[4];  pVC[5].z  += fL;
	pVC[7] =  pVC[6];  pVC[7].x  -= fW;   pVC[9] =  pVC[8];  pVC[9].y  -= fH;   pVC[11] = pVC[10]; pVC[11].z += fL;
	pVC[13] = pVC[12]; pVC[13].x -= fW;   pVC[15] = pVC[14]; pVC[15].y += fH;   pVC[17] = pVC[16]; pVC[17].z += fL;
	pVC[19] = pVC[18]; pVC[19].x += fW;   pVC[21] = pVC[20]; pVC[21].y += fH;   pVC[23] = pVC[22]; pVC[23].z += fL;
	
	pVC[25] = pVC[24]; pVC[25].x -= fW;   pVC[27] = pVC[26]; pVC[27].y -= fH;   pVC[29] = pVC[28]; pVC[29].z -= fL;
	pVC[31] = pVC[30]; pVC[31].x += fW;   pVC[33] = pVC[32]; pVC[33].y -= fH;   pVC[35] = pVC[34]; pVC[35].z -= fL;
	pVC[37] = pVC[36]; pVC[37].x += fW;   pVC[39] = pVC[38]; pVC[39].y += fH;   pVC[41] = pVC[40]; pVC[41].z -= fL;
	pVC[43] = pVC[42]; pVC[43].x -= fW;   pVC[45] = pVC[44]; pVC[45].y += fH;   pVC[47] = pVC[46]; pVC[47].z -= fL;

	__Vector3 vLength = (vMax - vMin);
	float fLength = vLength.Magnitude();
	__Vector3 vCenter = vMin + (vMax - vMin)/2.0f;
	pVC[48].Set(0, 0, 0, 0xffff0000); pVC[48].x -= vLength.x/2 + fLength/5 + 0.5f;// x 축 빨간색 -> 빨간색
	pVC[49].Set(0, 0, 0, 0xffff0000); pVC[49].x += vLength.x/2 + fLength/5 + 0.5f;// x 축 빨간색 -> 빨간색
	pVC[50].Set(0, 0, 0, 0xff00ff00); pVC[50].y -= vLength.y/2 + fLength/5 + 0.5f;// y 축 녹색 -> 녹색
	pVC[51].Set(0, 0, 0, 0xff00ff00); pVC[51].y += vLength.y/2 + fLength/5 + 0.5f;// y 축 녹색 -> 녹색
	pVC[52].Set(0, 0, 0, 0xff0000ff); pVC[52].z -= vLength.z/2 + fLength/5 + 0.5f;// z 축 파란색 -> 파란색
	pVC[53].Set(0, 0, 0, 0xff0000ff); pVC[53].z += vLength.z/2 + fLength/5 + 0.5f;// z 축 파란색 -> 파란색
	m_lpVBBox->Unlock();
}
#endif
*/
