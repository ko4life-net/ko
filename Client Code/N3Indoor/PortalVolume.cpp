// PortalVolume.cpp: implementation of the CPortalVolume class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N3Indoor.h"
#include "PortalVolume.h"

#include "PvsManager.h"

#include "MainFrm.h"
#include "OrganizeView.h"

#include "..\N3Base\N3Base.h"
#include "../N3Base/N3Shape.h"
#include "../N3Base/N3Scene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortalVolume::CPortalVolume()
{
	m_pvVertex[0].Set(-fBaseVolumnSize, -fBaseVolumnSize,  fBaseVolumnSize, dwColorNone);
	m_pvVertex[1].Set( fBaseVolumnSize, -fBaseVolumnSize,  fBaseVolumnSize, dwColorNone);
	m_pvVertex[2].Set( fBaseVolumnSize, -fBaseVolumnSize, -fBaseVolumnSize, dwColorNone);
	m_pvVertex[3].Set(-fBaseVolumnSize, -fBaseVolumnSize, -fBaseVolumnSize, dwColorNone);
	m_pvVertex[4].Set(-fBaseVolumnSize,  fBaseVolumnSize,  fBaseVolumnSize, dwColorNone);
	m_pvVertex[5].Set( fBaseVolumnSize,  fBaseVolumnSize,  fBaseVolumnSize, dwColorNone);
	m_pvVertex[6].Set( fBaseVolumnSize,  fBaseVolumnSize, -fBaseVolumnSize, dwColorNone);
	m_pvVertex[7].Set(-fBaseVolumnSize,  fBaseVolumnSize, -fBaseVolumnSize, dwColorNone);

	unsigned short*		pIdx = m_pIndex;

	// 아랫면.
	*pIdx++ = 0;  *pIdx++ = 1;  *pIdx++ = 3;
	*pIdx++ = 2;  *pIdx++ = 3;  *pIdx++ = 1;

	// 앞면..
	*pIdx++ = 7;  *pIdx++ = 3;  *pIdx++ = 6;
	*pIdx++ = 2;  *pIdx++ = 6;  *pIdx++ = 3;

	// 왼쪽..
	*pIdx++ = 4;  *pIdx++ = 0;  *pIdx++ = 7;
	*pIdx++ = 3;  *pIdx++ = 7;  *pIdx++ = 0;

	// 오른쪽..
	*pIdx++ = 6;  *pIdx++ = 2;  *pIdx++ = 5;
	*pIdx++ = 1;  *pIdx++ = 5;  *pIdx++ = 2;

	// 뒷면..
	*pIdx++ = 5;  *pIdx++ = 1;  *pIdx++ = 4;
	*pIdx++ = 0;  *pIdx++ = 4;  *pIdx++ = 1;

	// 윗면..	
	*pIdx++ = 4;  *pIdx++ = 7;  *pIdx++ = 5;
	*pIdx++ = 6;  *pIdx++ = 5;  *pIdx++ = 7;

	m_eState = STATE_NONE;

	m_eRenderType = TYPE_UNKNOWN;
	m_iPriority = -1;											//.. 컴파일 모드에서 Portal의 우선순위..	-1로 먼저 클리어 한다음.. 0 순위는 자기 자신..
}

CPortalVolume::~CPortalVolume()
{
	m_VoltList.clear();
	m_piIDList.clear();
	m_piVisibleIDList.clear();

	ShapeInfo* pSI;
	siiter siit = m_plShapeInfoList.begin();
	while(siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;
		delete pSI;
	}
	m_plShapeInfoList.clear();

	ClearCompileData();
}

__Vector3 CPortalVolume::GetCentorPoint()
{
	__Vector3 vec[8], vec2;	vec2.Zero();
	for( int i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;
		vec2 += vec[i];
	}

	return (vec2/8);
}

void CPortalVolume::DeleteLinkByiOrder(int iOrder)
{
	witer wit = m_VoltList.begin();
	for( int i = 0; i < iOrder; i++)
		wit++;

	m_VoltList.erase(wit);
}

void CPortalVolume::DeleteLinkedShapeByiOrder(int iOrder)
{
	ShapeInfo* pSI;	
	int ith = 0;

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;
		if (iOrder == ith)
		{
			delete pSI;
			m_plShapeInfoList.erase(--siit);
			return;
		}
		ith++;
	}	
}

int	 CPortalVolume::GetLinkedVolumeCount()
{
	int iCount = 0;
	witer wit = m_VoltList.begin();
	while(wit != m_VoltList.end())
	{
		wit++;
		iCount++;
	}	

	return iCount;
}

int CPortalVolume::GetLinkedShapeCount()
{
	int iCount = 0;
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end())
	{
		siit++;
		iCount++;
	}	

	return iCount;
}

ShapeInfo* CPortalVolume::GetNthLinkedShapeInfo(int iOrder)
{
	ShapeInfo* pSI;	
	int ith = 0;

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;
		if (iOrder == ith)
			return pSI;
		ith++;
	}	

	ASSERT(0);
	return NULL;
}

bool CPortalVolume::IsExistLinkedShapeByPointer(ShapeInfo* pSI)
{
	ShapeInfo* pSI2;	

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end())
	{
		pSI2 = *siit++;
		if (pSI2 == pSI)
			return true;
	}

	return false;
}

CPortalVolume* CPortalVolume::GetNthLinkedVolume(int iOrder)
{
	WVOL wvol;
	int ith = 0;

	witer wit = m_VoltList.begin();
	while(wit != m_VoltList.end())
	{
		wvol = *wit++;
		if (iOrder == ith)
			return wvol.pPvsVol;
		ith++;
	}	

	return NULL;
}

void CPortalVolume::SetShape(std::string szStr, CN3Shape* pShape, int iOrder)
{
	ShapeInfo* 	pSI = new ShapeInfo;
	pSI->m_iID = iOrder;
	pSI->m_strShapeFile = szStr;
	pSI->m_pShape = pShape;

	m_plShapeInfoList.push_back(pSI);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	pView->RefreshLinkedList();
}

void CPortalVolume::RenderEdit()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	float fPointSize;
	switch (m_eState)
	{
		case STATE_SELECTED:
			fPointSize = 12.0f;
			CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fPointSize));
			break;
		case STATE_LINKED:
		case STATE_VOLUME_ONLY:
			fPointSize = 8.0f;
			CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fPointSize));
			break;
	}

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	// Shape..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
		RenderShapeEdit();	

	// Collision..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
		RenderCollisionEdit();	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVolume::RenderCompile()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	float fPointSize;
	switch (m_eState)
	{
		case STATE_SELECTED:
			fPointSize = 12.0f;
			CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fPointSize));
			break;
		case STATE_LINKED:
			fPointSize = 8.0f;
			CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fPointSize));
			break;
	}

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	// Shape..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
		RenderShapeCompile();	

	// Collision..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
		RenderCollisionCompile();	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVolume::RenderExecute()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	// Shape..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
		RenderShapeExecute();	

	// Collision..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
		RenderCollisionExecute();	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVolume::SetState(e_PvsState ePS, e_PvsWallType ePWT)
{
	int i = 0;
	switch (ePS)
	{
		case STATE_NONE:
			for(i = 0; i < 8; i++)
				m_pvVertex[i].color = dwColorNone;
			break;
		case STATE_SELECTED:
			for(i = 0; i < 8; i++)
				m_pvVertex[i].color = dwColorSelected;
			break;
		case STATE_LINKED:
			for(i = 0; i < 8; i++)
			{
				switch (ePWT)
				{
					case PWALL_ZB:
						m_pvVertex[i].color = dwColorZB;
						break;
					case PWALL_ZF:
						m_pvVertex[i].color = dwColorZF;
						break;
					case PWALL_XL:
						m_pvVertex[i].color = dwColorXL;
						break;
					case PWALL_XR:
						m_pvVertex[i].color = dwColorXR;
						break;
					case PWALL_YT:
						m_pvVertex[i].color = dwColorYT;
						break;
					case PWALL_YB:
						m_pvVertex[i].color = dwColorYB;
						break;
				}
			}
			break;
		case STATE_VOLUME_ONLY:
			for(i = 0; i < 8; i++)
				m_pvVertex[i].color = dwColorSelectedVolOnly;
			break;
	}

	m_eState = ePS;
}

bool CPortalVolume::HasCollisionData()
{
	if (m_lpShapeColPartList.size() >= 1)
		return true;

	return false;
}

bool CPortalVolume::QueryHeight(float fx, float fz, float& fy)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	VisPortalPriority vPP;
	CPortalVolume* pVol = NULL;

	vppiter vppit = m_pVisiblePvsList.begin();
	while(vppit != m_pVisiblePvsList.end())
	{
		vPP = *vppit++;	
		if (vPP.m_iPriority > 1)
			continue;	

		pVol = vPP.m_pVol;
		ShapeInfo* pSI = NULL;
		__ColIndex * pCI = NULL;

		int iCount = pVol->m_lpShapeColPartList.size();
		ciiter ciit = pVol->m_lpShapeColPartList.begin();
		while(ciit != pVol->m_lpShapeColPartList.end())
		{
			pCI = *ciit++;

			int iSize = pCI->m_ivVector.size();
			__Vector3 vA, vB, vC;
			for( int k = 0; k < iSize/3; k++)
			{
				pSI = pView->m_PVSMgr.GetShapeInfoByManager(pCI->m_iID);
				pSI->m_pShape->PosSet(pSI->Pos());
				pSI->m_pShape->RotSet(pSI->Rot());
				pSI->m_pShape->ScaleSet(pSI->Scale());
				pSI->m_pShape->PartialGetCollision(pCI->m_ivVector[k*3], vA);		vA *= pSI->m_Matrix;
				pSI->m_pShape->PartialGetCollision(pCI->m_ivVector[k*3+1], vB);		vB *= pSI->m_Matrix;
				pSI->m_pShape->PartialGetCollision(pCI->m_ivVector[k*3+2], vC);		vC *= pSI->m_Matrix;			

				float a, b, c;
				a = ((vB.x-vA.x)*(fz-vA.z)) - ((vB.z-vA.z)*(fx-vA.x));
				b = ((vC.x-vB.x)*(fz-vB.z)) - ((vC.z-vB.z)*(fx-vB.x));
				c = ((vA.x-vC.x)*(fz-vC.z)) - ((vA.z-vC.z)*(fx-vC.x));

				if((a<=0 && b<=0 && c<=0) || (a>=0 && b>=0 && c>=0))
				{
					__Vector3 vAB, vAC, vAxis;
					vAB.Set(vB.x - vA.x, vB.y - vA.y, vB.z - vA.z);
					vAC.Set(vC.x - vA.x, vC.y - vA.y, vC.z - vA.z);

					vAxis.Cross(vAB, vAC);
					if (vAxis.y <= 0.0f )
						continue;

					fy = (vAxis.x*(vA.x-fx)+vAxis.z*(vA.z-fz))/vAxis.y + vA.y;
					fy += 2.0f;
		
					return true;
				}
			}
		}
	}

	return false;
}

void CPortalVolume::ClearCompileData()
{
	ShapePart* pSP;
	spiter spit = m_lpShapePartList.begin();
	while(spit != m_lpShapePartList.end())
	{
		pSP = *spit++;
		pSP->Clear();
		delete pSP;
	}
	m_lpShapePartList.clear();

	__ColIndex* pCI;
	ciiter ciit = m_lpShapeColPartList.begin();
	while(ciit != m_lpShapeColPartList.end())
	{
		pCI = *ciit++;
		delete pCI;
	}
	m_lpShapeColPartList.clear();
	m_pVisiblePvsList.clear();
}

void CPortalVolume::RenderShapeEdit()
{
	ShapeInfo* pSI;	
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;

		pSI->m_pShape->PosSet(pSI->Pos());
		pSI->m_pShape->RotSet(pSI->Rot());
		pSI->m_pShape->ScaleSet(pSI->Scale());
		pSI->m_pShape->Tick(-1000);
		pSI->m_pShape->m_bDontRender = false;
		pSI->m_pShape->Render();
	}
}

void CPortalVolume::RenderCollisionEdit()
{
	ShapeInfo* pSI;
	siiter siit = m_plShapeInfoList.begin();
	while(siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;

		pSI->m_pShape->PosSet(pSI->Pos());
		pSI->m_pShape->RotSet(pSI->Rot());
		pSI->m_pShape->ScaleSet(pSI->Scale());
		pSI->m_pShape->Tick(-1000);
		pSI->m_pShape->m_bDontRender = false;

		// 로딩할때 미리 계산해 놓은 월드 행렬 적용..
		__Matrix44 mtxBackup;
		CN3Base::s_lpD3DDev->GetTransform(D3DTS_WORLD, &mtxBackup);
		CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &pSI->m_pShape->m_Matrix);

		if (pSI->m_pShape->CollisionMesh())
			pSI->m_pShape->CollisionMesh()->Render(0xffffffff);

		CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxBackup);
	}
}

void CPortalVolume::RenderShapeCompile()
{
	RenderShapeEdit();

	ShapeInfo* pSI = NULL;
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	ShapePart* pSP = NULL;
	spiter spit = m_lpShapePartList.begin();
	while( spit != m_lpShapePartList.end())
	{
		pSP = *spit++;

		viter vit =	pSP->m_viIndex.begin();
		__VPI vpi;

		while (vit != pSP->m_viIndex.end())
		{
			vpi = *vit++;
			int iSize = vpi.m_ivVector.size();
		
	#ifdef _USE_VERTEXBUFFER
			LPDIRECT3DINDEXBUFFER8	pIB;
			HRESULT hr = CN3Base::s_lpD3DDev->CreateIndexBuffer(iSize*sizeof(WORD),
											D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIB);
			if (FAILED(hr)) return hr;

			BYTE* pByte;
			hr = pIB->Lock(0, 0, &pByte, 0);
			WORD* pIndices = (WORD*)pByte;
			
			Iit = vpi.m_ivVector.begin();
			while(Iit != vpi.m_ivVector.end())
			{
				int iOrder = *Iit++;
				*pIndices++ = iOrder;
			}

			pIB->Unlock();
			
			// Rendering..
			pSI = pView->m_PVSMgr.GetShapeInfoByManager(pSP->m_iID);
			pSI->m_pShape->PosSet(pSI->Pos());
			pSI->m_pShape->RotSet(pSI->Rot());
			pSI->m_pShape->ScaleSet(pSI->Scale());
			pSI->m_pShape->m_bDontRender = false;
			pSI->m_pShape->PartialRender(vpi.m_iPartIndex, iSize, pIB);
			pIB->Release();
	#else
			LPWORD pIndices;
			pIndices = new WORD[iSize]; memset(pIndices, 0, sizeof(WORD)*iSize);
			int iSizes = vpi.m_ivVector.size();
			for( int k = 0; k < iSizes; k++)
			{
				pIndices[k] = vpi.m_ivVector[k];
			}
			pSI = pView->m_PVSMgr.GetShapeInfoByManager(pSP->m_iID);
			pSI->m_pShape->PosSet(pSI->Pos());
			pSI->m_pShape->RotSet(pSI->Rot());
			pSI->m_pShape->ScaleSet(pSI->Scale());
			pSI->m_pShape->m_bDontRender = false;
			pSI->m_pShape->PartialRender(vpi.m_iPartIndex, iSize, pIndices);
			delete pIndices;
	#endif
		}
	}
}

void CPortalVolume::RenderCollisionCompile()
{
	RenderCollisionEdit();

	ShapeInfo* pSI = NULL;
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	__ColIndex * pCI = NULL;

	ciiter ciit = m_lpShapeColPartList.begin();
	while(ciit != m_lpShapeColPartList.end())
	{
		pCI = *ciit++;

		// 행렬 계산..
		__Matrix44 mtxWorld;
		mtxWorld.Identity();

		CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);

		LPDWORD pIndices;
		int iSize = pCI->m_ivVector.size();
		pIndices = new DWORD[iSize]; memset(pIndices, 0, sizeof(DWORD)*iSize);
		int iSizes = pCI->m_ivVector.size();
		for( int k = 0; k < iSizes; k++)
		{
			pIndices[k] = pCI->m_ivVector[k];
		}

		__Matrix44 mtxBackup;
		CN3Base::s_lpD3DDev->GetTransform(D3DTS_WORLD, &mtxBackup);
		pSI = pView->m_PVSMgr.GetShapeInfoByManager(pCI->m_iID);
		CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &pSI->m_Matrix);

		pSI->m_pShape->PartialColRender(iSize, (int*)pIndices);
		delete pIndices;

		CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxBackup);
	}
}

void CPortalVolume::RenderShapeExecute()
{
	RenderShapeCompile();
}

void CPortalVolume::RenderCollisionExecute()
{
	RenderCollisionCompile();
}

bool CPortalVolume::Load(HANDLE hFile, bool bGameData)
{
	CN3Transform::Load(hFile);

	// 자신의 데이터 로드..
	DWORD dwNum;
	std::string strSrc;

	// 링크된 갯수를 로드..
	int iLinkedCount = 0;

	ReadFile(hFile, &iLinkedCount, sizeof(int), &dwNum, NULL);
	
	// 링크된 아이디 로드..
	WVID wvid;
	for( int i = 0; i < iLinkedCount; i++ )
	{
		ReadFile(hFile, &wvid.iID, sizeof(int), &dwNum, NULL);
		ReadFile(hFile, &wvid.ePWT, sizeof(int), &dwNum, NULL);
		m_piIDList.push_back(wvid);
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	// 링크된 Shape 갯수 로드..
	int iCount = 0; int iSize = 0;
	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);
	for (i = 0; i < iCount; i++)
	{
		ShapeInfo*	pSI = new ShapeInfo;
		ReadFile(hFile, &pSI->m_iID, sizeof(int), &dwNum, NULL);
		
		// 문자열 길이..
		strSrc = CPVSManager::ReadDecryptString(hFile);
		pSI->m_strShapeFile = strSrc;

		// SourceList에서.. Shape의 Pointer를 연결한다..
		pSI->m_pShape = pFrm->m_pSceneSource->ShapeGetByFileName(strSrc);
		ASSERT(pSI->m_pShape);

		ReadFile(hFile, &pSI->m_iBelong, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iEventID, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iEventType, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iNPC_ID, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iNPC_Status, sizeof(int), &dwNum, NULL);	
		pSI->Load(hFile);

		m_plShapeInfoList.push_back(pSI);
	}

	if (!bGameData)
		return true;

	LoadGameData(hFile);

	return true;
}

void CPortalVolume::LoadGameData(HANDLE hFile)
{
	DWORD dwNum;
	std::string strSrc;

	//..
	int iCount = 0; IDAndPriority IDAP;
	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	for( int i = 0; i < iCount; i++ )
	{
		ReadFile(hFile, &IDAP.m_iID, sizeof(int), &dwNum, NULL);
		ReadFile(hFile, &IDAP.m_iPriority, sizeof(int), &dwNum, NULL);
		ASSERT(IDAP.m_iPriority != -1);
		m_piVisibleIDList.push_back(IDAP);
	}

	//..
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	CN3Shape* pShape = NULL;

	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);
		
	int iSize = 0, iSize_2 = 0, iSize_3 = 0;
	for( i = 0; i < iCount; i++ )
	{
		ShapePart* pSP = new ShapePart;
		ReadFile(hFile, &pSP->m_iID, sizeof(int), &dwNum, NULL);
		
		ReadFile(hFile, &iSize_2, sizeof(int), &dwNum, NULL);
		for( int j = 0; j <iSize_2; j++ )
		{
			__VPI vpi;
			ReadFile(hFile, &vpi.m_iPartIndex, sizeof(int), &dwNum, NULL);

			ReadFile(hFile, &iSize_3, sizeof(int), &dwNum, NULL);
			for( int k = 0; k < iSize_3; k++ )
			{
				int iV = 0;
				ReadFile(hFile, &iV, sizeof(int), &dwNum, NULL);
				vpi.m_ivVector.push_back(iV);
			}

			pSP->m_viIndex.push_back(vpi);	
		}

		m_lpShapePartList.push_back(pSP);
	}

	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	COrganizeView* pView = pFrm->GetOrganizeView();

	for( i = 0; i < iCount; i++ )
	{
		__ColIndex* pCI = new __ColIndex;
		ReadFile(hFile, &pCI->m_iID, sizeof(int), &dwNum, NULL);		

		ReadFile(hFile, &iSize_2, sizeof(int), &dwNum, NULL);
		for( int j = 0; j <iSize_2; j++ )
		{
			int iV = 0;			
			ReadFile(hFile, &iV, sizeof(int), &dwNum, NULL);
			pCI->m_ivVector.push_back(iV);
		}

		m_lpShapeColPartList.push_back(pCI);
	}
}

bool CPortalVolume::Save(HANDLE hFile, bool bGameData)
{
	DWORD dwNum;
	std::string strSrc;

	// 자신의 아이디를 저장..
	WriteFile(hFile, &m_iID, sizeof(int), &dwNum, NULL);

	// 자신의 데이터 저장..
	CN3Transform::Save(hFile);

	// 링크된 갯수를 저장..
	int iCount = m_VoltList.size();
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	//링크된 아이디 저장..
	WVOL wvol;
	witer wit = m_VoltList.begin();
	while (wit != m_VoltList.end())
	{
		wvol = *wit++;
		WriteFile(hFile, &wvol.pPvsVol->m_iID, sizeof(int), &dwNum, NULL);
		WriteFile(hFile, &wvol.ePWT, sizeof(int), &dwNum, NULL);
	}

	// 링크된 Shape 갯수 저장..
	iCount = m_plShapeInfoList.size();
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	ShapeInfo*	pSI = NULL;	
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;
		WriteFile(hFile, &pSI->m_iID, sizeof(int), &dwNum, NULL);

		CPVSManager::WriteCryptographString(hFile, pSI->m_strShapeFile);

		// Shape의 데이터 저장..
		WriteFile(hFile, &pSI->m_iBelong, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iEventID, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iEventType, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iNPC_ID, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iNPC_Status, sizeof(int), &dwNum, NULL);	
		pSI->Save(hFile);
	}

	if (!bGameData)
		return true;

	SaveGameData(hFile);

	return true;
}

void CPortalVolume::SaveGameData(HANDLE hFile)
{
	DWORD dwNum;
	std::string strSrc;

	int iCount = 0;
	CPortalVolume* pVol = NULL;

	// Visible Volume의 ID..
	iCount = m_pVisiblePvsList.size();
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	VisPortalPriority vPP;
	vppiter vppit = m_pVisiblePvsList.begin();
	while( vppit != m_pVisiblePvsList.end())
	{
		vPP = *vppit++;
		WriteFile(hFile, &vPP.m_pVol->m_iID, sizeof(int), &dwNum, NULL);
		WriteFile(hFile, &vPP.m_iPriority, sizeof(int), &dwNum, NULL);
		ASSERT(vPP.m_iPriority != -1);
	}

	iCount = m_lpShapePartList.size(); 
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	// Shape의 부분 정보..
	ShapePart* pSP = NULL;
	spiter spit = m_lpShapePartList.begin();
	while( spit != m_lpShapePartList.end())
	{
		pSP = *spit++;	
		WriteFile(hFile, &pSP->m_iID, sizeof(int), &dwNum, NULL);
		
		int iSize = pSP->m_viIndex.size();
		WriteFile(hFile, &iSize, sizeof(int), &dwNum, NULL);
		
		__VPI vpi;
		viter vit = pSP->m_viIndex.begin();
		while ( vit != pSP->m_viIndex.end())
		{
			vpi = *vit++;
			WriteFile(hFile, &vpi.m_iPartIndex, sizeof(int), &dwNum, NULL);

			iSize = vpi.m_ivVector.size();
			WriteFile(hFile, &iSize, sizeof(int), &dwNum, NULL);

			for( int i = 0; i < iSize; i++)
				WriteFile(hFile, &vpi.m_ivVector[i], sizeof(int), &dwNum, NULL);
		}
	}

	iCount = m_lpShapeColPartList.size(); 
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	__ColIndex* pCI = NULL;
	ciiter ciit = m_lpShapeColPartList.begin();
	while( ciit != m_lpShapeColPartList.end() )
	{
		pCI = *ciit++;
		WriteFile(hFile, &pCI->m_iID, sizeof(int), &dwNum, NULL);

		int iSize = pCI->m_ivVector.size();
		WriteFile(hFile, &iSize, sizeof(int), &dwNum, NULL);

		for( int i = 0; i < iSize; i++)
			WriteFile(hFile, &pCI->m_ivVector[i], sizeof(int), &dwNum, NULL);		
	}
}

void CPortalVolume::SplitAndMakeShape(ShapeInfo* pSI)
{
	ShapePart* pSP = new ShapePart;
	pSP->m_iID = pSI->m_iID;

	int iPartCount = pSI->m_pShape->PartCount();

	pSI->m_pShape->PosSet(pSI->Pos());
	pSI->m_pShape->RotSet(pSI->Rot());
	pSI->m_pShape->ScaleSet(pSI->Scale());

	__Matrix44 mtx; mtx = pSI->m_pShape->m_Matrix;
	
	for ( int i = 0; i < iPartCount; i++ )
	{
		int iIndexCount = pSI->m_pShape->GetIndexbufferCount(i);
		if (!iIndexCount) continue;
	
		__VPI vpi;
		vpi.m_iPartIndex = i;

		for ( int j = 0; j < iIndexCount; j += 3 )
		{
			int iIndex1, iIndex2, iIndex3;
			__Vector3 vec1, vec2, vec3;

			iIndex1		= pSI->m_pShape->GetIndexByiOrder(i, j);						
			vec1		 = pSI->m_pShape->GetVertexByIndex(i, iIndex1);
			vec1		*= pSI->m_pShape->GetPartMatrix(i);	
			iIndex2		= pSI->m_pShape->GetIndexByiOrder(i, j+1);						
			vec2		 = pSI->m_pShape->GetVertexByIndex(i, iIndex2); 		
			vec2		*= pSI->m_pShape->GetPartMatrix(i);	
			iIndex3		= pSI->m_pShape->GetIndexByiOrder(i, j+2);						
			vec3		 = pSI->m_pShape->GetVertexByIndex(i, iIndex3); 		
			vec3		*= pSI->m_pShape->GetPartMatrix(i);	

			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;
			
			vpi.m_ivVector.push_back(iIndex1);	
			vpi.m_ivVector.push_back(iIndex2);	
			vpi.m_ivVector.push_back(iIndex3);	
		}

		pSP->m_viIndex.push_back(vpi);
	}
	
	m_lpShapePartList.push_back(pSP);
	RenderShapeCompile();
}

void CPortalVolume::SplitAndMakeCollision(ShapeInfo* pSI)
{
	__ColIndex* pCI = new __ColIndex;
	pCI->m_iID = pSI->m_iID;

	int iIndexCount = pSI->m_pShape->GetColIndexbufferCount();
	if (iIndexCount == -1) return; 
	if (!iIndexCount)
	{
		for ( int j = 0; j < pSI->m_pShape->CollisionMesh()->VertexCount(); j += 3 )
		{
			__Vector3 vec1, vec2, vec3;

			vec1		 = pSI->m_pShape->GetColVertexByIndex(j); 
			vec1		*= pSI->m_Matrix;
			vec2		 = pSI->m_pShape->GetColVertexByIndex(j+1); 		
			vec2		 *= pSI->m_Matrix;
			vec3		 = pSI->m_pShape->GetColVertexByIndex(j+2); 		
			vec3		 *= pSI->m_Matrix;
		
			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;

			pCI->m_ivVector.push_back(j);
			pCI->m_ivVector.push_back(j+1);
			pCI->m_ivVector.push_back(j+2);
		}	
	}
	else
	{
		for ( int j = 0; j < iIndexCount; j += 3 )
		{
			int iIndex1, iIndex2, iIndex3;
			__Vector3 vec1, vec2, vec3;

			iIndex1		= pSI->m_pShape->GetColIndexByiOrder(j);						
			vec1		 = pSI->m_pShape->GetColVertexByIndex(iIndex1); 
			vec1		 *= pSI->m_pShape->m_Matrix;
			iIndex2		= pSI->m_pShape->GetColIndexByiOrder(j+1);						
			vec2		 = pSI->m_pShape->GetColVertexByIndex(iIndex2); 		
			vec2		 *= pSI->m_pShape->m_Matrix;
			iIndex3		= pSI->m_pShape->GetColIndexByiOrder(j+2);						
			vec3		 = pSI->m_pShape->GetColVertexByIndex(iIndex3); 		
			vec3		 *= pSI->m_pShape->m_Matrix;
		
			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;

			pCI->m_ivVector.push_back(iIndex1);
			pCI->m_ivVector.push_back(iIndex2);
			pCI->m_ivVector.push_back(iIndex3);
		}	
	}

	m_lpShapeColPartList.push_back(pCI);
	RenderCollisionCompile();
}

bool CPortalVolume::IsInVolumn(__Vector3 vec)
{
	__Vector3 vec2[8];
	for( int i = 0; i < 8; i++)
	{
		vec2[i] = m_pvVertex[i];
		vec2[i] *= m_Matrix;
	}

	if (vec.x >= vec2[0].x && vec.x <= vec2[1].x && vec.y >= vec2[0].y && vec.y <= vec2[4].y && vec.z >=  vec2[2].z && vec.z <= vec2[0].z )
		return true;

	return false;
}

bool CPortalVolume::IsInVolumn2d(int ix, int iz)
{
	__Vector3 vec2[8];
	for( int i = 0; i < 8; i++)
	{
		vec2[i] = m_pvVertex[i];
		vec2[i] *= m_Matrix;
	}

	if (ix >= vec2[0].x && ix <= vec2[1].x && iz >=  vec2[2].z && iz <= vec2[0].z )
		return true;

	return false;
}

bool CPortalVolume::IsInVolumnEx(__Vector3 vec1, __Vector3 vec2, __Vector3 vec3)
{
	if (IsInVolumnExEx(vec1, vec2))
		return true;

	if (IsInVolumnExEx(vec2, vec1))
		return true;

	if (IsInVolumnExEx(vec1, vec3))
		return true;

	if (IsInVolumnExEx(vec3, vec1))
		return true;

	if (IsInVolumnExEx(vec2, vec3))
		return true;

	if (IsInVolumnExEx(vec3, vec2))
		return true;

	return false;
}

bool CPortalVolume::IsInVolumnExEx(__Vector3 vec1, __Vector3 vec2)
{
	__Vector3 vPick;

	float fMag2, fMag1 = (vec2 - vec1).Magnitude();	
	__Vector3 vDir; vDir = vec2 - vec1; vDir.Normalize();

	if (IsInVolumnExExEx(vec1, vDir, PWALL_ZB, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, PWALL_ZF, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, PWALL_XL, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, PWALL_XR, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, PWALL_YT, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, PWALL_YB, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	return false;
}

bool CPortalVolume::IsInVolumnExExEx(__Vector3 vOrig, __Vector3 vDir, e_PvsWallType ePWT, __Vector3 &vPick)
{
	float ft, fu, fv;
	int i;

	switch (ePWT)
	{
		case PWALL_ZB:
			i = 6;
			break;
		case PWALL_ZF:
			i = 24;
			break;
		case PWALL_XL:
			i = 12;
			break;
		case PWALL_XR:
			i = 18;
			break;
		case PWALL_YT:
			i = 30;
			break;
		case PWALL_YB:
			i = 0;
			break;
	}

	__Vector3 vec[8];
	for( i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;
	}

	if(IntersectTriangle(vOrig, vDir, vec[m_pIndex[i]], vec[m_pIndex[i+1]], vec[m_pIndex[i+2]], ft, fu, fv, &vPick))
		return true;
	if(IntersectTriangle(vOrig, vDir, vec[m_pIndex[i+3]], vec[m_pIndex[i+4]], vec[m_pIndex[i+5]], ft, fu, fv, &vPick))
		return true;

	return false;
}

bool CPortalVolume::PickWithVolume(__Vector3 &vPos, __Vector3& vDir, __Vector3* pVec)
{
	__Vector3 vec[8];
	for( int i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;
	}

	float ft, fu, fv;
	for (i = 0; i < 36; i += 6)
	{
		if(IntersectTriangle(vPos, vDir, vec[m_pIndex[i]], vec[m_pIndex[i+1]], vec[m_pIndex[i+2]], ft, fu, fv, pVec))
			return true;
		if(IntersectTriangle(vPos, vDir, vec[m_pIndex[i+3]], vec[m_pIndex[i+4]], vec[m_pIndex[i+5]], ft, fu, fv, pVec))
			return true;
	}

	return false;
}

bool CPortalVolume::GetHeight(int iX, int iZ, float &fHeight)
{
	if (IsInVolumn2d(iX, iZ))
	{
		fHeight = GetTopVolumeHeight() - 0.01f;
		return true;
	}

	return false;
}

float CPortalVolume::GetTopVolumeHeight()
{
	float fHeight = FLT_MIN;

	__Vector3 vec[8];
	for( int i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;
		if (fHeight < vec[i].y)
			fHeight = vec[i].y;
	}
	return fHeight;
}

__Vector3 CPortalVolume::Min()
{
	__Vector3 vec[8], vMin; 
	vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	for( int i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;

		if(vec[i].x < vMin.x) vMin.x = vec[i].x;
		if(vec[i].y < vMin.y) vMin.y = vec[i].y;
		if(vec[i].z < vMin.z) vMin.z = vec[i].z;
	}
	return vMin;
}

__Vector3 CPortalVolume::Max()
{
	__Vector3 vec[8], vMax; vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for( int i = 0; i < 8; i++)
	{
		vec[i] = m_pvVertex[i];
		vec[i] *= m_Matrix;

		if(vec[i].x > vMax.x) vMax.x = vec[i].x;
		if(vec[i].y > vMax.y) vMax.y = vec[i].y;
		if(vec[i].z > vMax.z) vMax.z = vec[i].z;
	}
	return vMax;
}

void CPortalVolume::UpdatePosAll(float fx, float fy, float fz)
{
	PosSet(fx, fy, fz);

	ShapeInfo*	pSI = NULL;	
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;	
		pSI->PosSet(fx, fy, fz);
	}
}

bool CPortalVolume::IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir,
							  const __Vector3& v0, const __Vector3& v1, const __Vector3& v2,
							  float& fT, float& fU, float& fV, __Vector3* pVCol)
{
    // Find vectors for two edges sharing vert0
    static __Vector3 vEdge1, vEdge2;
	
	vEdge1 = v1 - v0;
    vEdge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    __Vector3 pVec;	float fDet;
	
//	By : Ecli666 ( On 2001-09-12 오전 10:39:01 )

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return false;

//	~(By Ecli666 On 2001-09-12 오전 10:39:01 )

    pVec.Cross(vDir, vEdge2);

    // If determinant is near zero, ray lies in plane of triangle
    fDet = vEdge1.Dot(pVec);
    if( fDet < 0.0001f )		// 거의 0에 가까우면 삼각형 평면과 지나가는 선이 평행하다.
        return false;

    // Calculate distance from vert0 to ray origin
    __Vector3 tVec = vOrig - v0;

    // Calculate U parameter and test bounds
    fU = tVec.Dot(pVec);
    if( fU < 0.0f || fU > fDet )
        return false;

    // Prepare to test V parameter
    __Vector3 qVec;
    qVec.Cross(tVec, vEdge1);

    // Calculate V parameter and test bounds
    fV = D3DXVec3Dot( &vDir, &qVec );
    if( fV < 0.0f || fU + fV > fDet )
        return false;

    // Calculate t, scale parameters, ray intersects triangle
    fT = D3DXVec3Dot( &vEdge2, &qVec );
    float fInvDet = 1.0f / fDet;
    fT *= fInvDet;
    fU *= fInvDet;
    fV *= fInvDet;

	// t가 클수록 멀리 직선과 평면과 만나는 점이 멀다.
	// t*dir + orig 를 구하면 만나는 점을 구할 수 있다.
	// u와 v의 의미는 무엇일까?
	// 추측 : v0 (0,0), v1(1,0), v2(0,1) <괄호안은 (U, V)좌표> 이런식으로 어느 점에 가깝나 나타낸 것 같음
	//

	if(pVCol) (*pVCol) = vOrig + (vDir * fT);	// 접점을 계산..

	// *t < 0 이면 뒤쪽...
	if ( fT < 0.0f )
		return false;

	return true;	
}

