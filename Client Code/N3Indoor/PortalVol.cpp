// PortalVol.cpp: implementation of the CPortalVol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "n3indoor.h"

#include "PvsObjFactory.h"
#include "PortalVol.h"
#include "PortalWall.h"

#include "PvsMgr.h"

#include "MainFrm.h"
#include "OrganizeView.h"
#include "N3IndoorDoc.h"
#include "N3IndoorView.h"

#include "..\N3Base\N3Base.h"
#include "../N3Base/N3Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CPortalVol, CPvsBase )

CPortalVol::CPortalVol()
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

	m_strShapeFile		 = "";
	m_pShape			  = NULL;

	m_MtxShapeMove.Identity();	
	m_MtxShapeScale.Identity();	

	// ..
	__VPI vpi;
	viter vit =	m_viIndex.begin();
	
	while (vit != m_viIndex.end())
	{
		vpi = *vit++;
		vpi.m_ivVector.clear();
	}

	m_viIndex.clear();
}

CPortalVol::~CPortalVol()
{
	__VPI vpi;
	viter vit =	m_viIndex.begin();
	
	while (vit != m_viIndex.end())
	{
		vpi = *vit++;
		vpi.m_ivVector.clear();
	}

	m_viIndex.clear();
}

bool CPortalVol::CreatePvsObject()
{
	return true;
}

bool CPortalVol::DeletePvsObject()
{
	return true;
}

void CPortalVol::TickEdit()
{



}

void CPortalVol::TickCompile()
{



}

void CPortalVol::TickExecute()
{



}

void CPortalVol::RenderEdit()
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

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	if (pFrm->m_eShapeView == SHAPEVIEW_STATE_PARTIAL)
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			RenderShape();	

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			RenderCollision();	
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVol::RenderCompile()
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

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	if (pFrm->m_eShapeView == SHAPEVIEW_STATE_PARTIAL)
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			RenderShape();	

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			RenderCollision();	
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVol::RenderExecute()
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

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	if (pFrm->m_eShapeView == SHAPEVIEW_STATE_PARTIAL)
	{
		// Shape..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
			RenderShape();	

		// Collision..
		if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
			RenderCollision();	
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalVol::RenderShape()
{
	CN3Shape* pShape = ((CMainFrame*)AfxGetMainWnd())->m_pShapeBg;
	if (!pShape)	return;

	// PMesh에 전달할 Index Buffer를 만든다..
	viter vit =	m_viIndex.begin();
	__VPI vpi;
	
	while (vit != m_viIndex.end())
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
		pShape->PartialRender(vpi.m_iPartIndex, iSize, pIB);
		pIB->Release();
#else
		LPWORD pIndices;
		pIndices = new WORD[iSize]; memset(pIndices, 0, sizeof(WORD)*iSize);
		int iSizes = vpi.m_ivVector.size();
		for( int k = 0; k < iSizes; k++)
		{
			pIndices[k] = vpi.m_ivVector[k];
		}
		pShape->PartialRender(vpi.m_iPartIndex, iSize, pIndices);
		delete pIndices;
#endif
	}
}

void CPortalVol::RenderCollision()
{
	// 행렬 계산..
	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);

	CN3Shape* pShape = ((CMainFrame*)AfxGetMainWnd())->m_pShapeBg;
	if (!pShape)	return;

	LPDWORD pIndices;
	int iSize = m_ivColVector.size();
	pIndices = new DWORD[iSize]; memset(pIndices, 0, sizeof(DWORD)*iSize);
	int iSizes = m_ivColVector.size();
	for( int k = 0; k < iSizes; k++)
	{
		pIndices[k] = m_ivColVector[k];
	}
	pShape->PartialColRender(iSize, (int*)pIndices);
	delete pIndices;
}

void CPortalVol::Translate()
{
	__Vector3 vec[8];

	vec[0].Set(-fBaseVolumnSize, -fBaseVolumnSize,  fBaseVolumnSize);
	vec[1].Set( fBaseVolumnSize, -fBaseVolumnSize,  fBaseVolumnSize);
	vec[2].Set( fBaseVolumnSize, -fBaseVolumnSize, -fBaseVolumnSize);
	vec[3].Set(-fBaseVolumnSize, -fBaseVolumnSize, -fBaseVolumnSize);
	vec[4].Set(-fBaseVolumnSize,  fBaseVolumnSize,  fBaseVolumnSize);
	vec[5].Set( fBaseVolumnSize,  fBaseVolumnSize,  fBaseVolumnSize);
	vec[6].Set( fBaseVolumnSize,  fBaseVolumnSize, -fBaseVolumnSize);
	vec[7].Set(-fBaseVolumnSize,  fBaseVolumnSize, -fBaseVolumnSize);

	for( int i = 0; i < 8; i++)
	{
		vec[i] *= m_MtxScale;
		vec[i] *= m_MtxMove;
		m_pvVertex[i].x = vec[i].x; m_pvVertex[i].y = vec[i].y; m_pvVertex[i].z = vec[i].z;
	}
}

void CPortalVol::SetState(e_PvsState ePS)
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
				m_pvVertex[i].color = dwColorLinked;
			break;
	}

	m_eState = ePS;
}

void CPortalVol::AddWallLinkByPointer(CPvsBase* pBase)
{
	if (!pBase)	return;	

	WallAdjust(pBase);

	m_pPvsList.push_back(pBase);
}

void CPortalVol::AddVolLinkByPointer(CPortalVol* pVol)
{
	if (!pVol)	return;	
	m_pPvsList.push_back(pVol);
}

void CPortalVol::WallAdjust(CPvsBase* pBase)
{
	CPortalWall* pWall = (CPortalWall* )pBase;
	float fOffset;
	__Matrix44 mat; mat.Identity();

	switch( pWall->m_eWallType )
	{
		case WALL_ZB:
			fOffset = m_pvVertex[0].z - pWall->m_pvVertex[0].z;
			mat.PosSet(0.0f, 0.0f, fOffset);
			pWall->m_MtxMove *= mat;
			break;

		case WALL_ZF:
			fOffset = m_pvVertex[2].z - pWall->m_pvVertex[0].z;
			mat.PosSet(0.0f, 0.0f, fOffset);
			pWall->m_MtxMove *= mat;
			break;

		case WALL_XL:
			fOffset = m_pvVertex[0].x - pWall->m_pvVertex[0].x;
			mat.PosSet(fOffset, 0.0f, 0.0f);
			pWall->m_MtxMove *= mat;
			break;

		case WALL_XR:
			fOffset = m_pvVertex[1].x - pWall->m_pvVertex[0].x;
			mat.PosSet(fOffset, 0.0f, 0.0f);
			pWall->m_MtxMove *= mat;
			break;

		case WALL_YT:
			fOffset = m_pvVertex[4].y - pWall->m_pvVertex[0].y;
			mat.PosSet(0.0f, fOffset, 0.0f);
			pWall->m_MtxMove *= mat;
			break;

		case WALL_YB:
			fOffset = m_pvVertex[0].y - pWall->m_pvVertex[0].y;
			mat.PosSet(0.0f, fOffset, 0.0f);
			pWall->m_MtxMove *= mat;
			break;
	}
	pWall->Translate();
}

void CPortalVol::DeleteLinkByiOrder(int iOrder)
{
	iter it = m_pPvsList.begin();
	for( int i = 0; i < iOrder; i++)
		it++;

	m_pPvsList.erase(it);
}

void CPortalVol::Load(FILE* stream)
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

	m_strShapeFile		 = "";
	m_pShape			  = NULL;

	m_MtxShapeMove.Identity();	
	m_MtxShapeScale.Identity();	

	// ..
	__VPI vpi;
	viter vit =	m_viIndex.begin();
	
	while (vit != m_viIndex.end())
	{
		vpi = *vit++;
		vpi.m_ivVector.clear();
	}

	m_viIndex.clear();

	CPvsBase::Load(stream);

	Translate();

	// 링크된 갯수를 로드..
	int iLinkedCount = 0;
	fread(&iLinkedCount, sizeof(int), 1, stream);
	
	// 링크된 아이디 로드..
	int iID;
	for( int i = 0; i < iLinkedCount; i++ )
	{
		fread(&iID, sizeof(int), 1, stream);
		m_piIDList.push_back(iID);
	}
}

void CPortalVol::Save(FILE* stream)
{
	// 먼저 자신의 타입을 저장..
	fwrite(&_iTypeVolumn, sizeof(int), 1, stream);	

	// 자신의 아이디를 저장..
	fwrite(&m_iID, sizeof(int), 1, stream);	

	// 자신의 데이터 저장..
	CPvsBase::Save(stream);

	// 링크된 갯수를 저장..
	int iCount = 0;
	iter it = m_pPvsList.begin();
	while (it != m_pPvsList.end())
	{
		it++;	iCount++;
	}
	fwrite(&iCount, sizeof(int), 1, stream);	

	//링크된 아이디 저장..
	CPvsBase* pBase = NULL;
	it = m_pPvsList.begin();
	while (it != m_pPvsList.end())
	{
		pBase = *it++;
		fwrite(&pBase->m_iID, sizeof(int), 1, stream);	
	}
}

void CPortalVol::AddVisibleIDList(CPortalVol* pVol)
{
	m_pVisibleiIDList.push_back(pVol->m_iID);
}

void CPortalVol::SetShape(std::string szStr, CN3Shape* pShape)
{
	m_strShapeFile = szStr;
	m_pShape = pShape;
	m_pShape->PosSet(0.0f, 0.0f, 0.0f);

	m_MtxShapeMove = m_MtxMove;	
	m_MtxShapeScale = m_MtxScale;
}

void CPortalVol::SplitAndMakeShape(CN3Shape* pShape)
{
	__Vector3 vec;
	viter vit;

	vit =	m_viIndex.begin();
	
	while (vit != m_viIndex.end())
	{
		__VPI	vpi = *vit++;
		vpi.m_ivVector.clear();
	}

	m_viIndex.clear();

	int iPartCount = pShape->PartCount();
	GAPP->Write("Part Count %d", iPartCount);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 기존 Index Buffer 에서.. Vertex Buffer를 참조해서.. 버텍스를 얻어낸다음.. 
	//  폴리곤 단위(3개의 버텍스가 전부)로 보이지 않는 다면.. 인덱스 버퍼에서 제거한다..

	for ( int i = 0; i < iPartCount; i++ )
	{
		int iIndexCount = pShape->GetIndexbufferCount(i);
		if (!iIndexCount) continue;

		__VPI vpi;
		vpi.m_iPartIndex = i;
		
		for ( int j = 0; j < iIndexCount; j += 3 )
		{
			int iIndex1, iIndex2, iIndex3;
			__Vector3 vec1, vec2, vec3;

			iIndex1		= pShape->GetIndexByiOrder(i, j);						
			vec1		 = pShape->GetVertexByIndex(i, iIndex1); 
			vec1		*= pShape->GetPartMatrix(i);	
			iIndex2		= pShape->GetIndexByiOrder(i, j+1);						
			vec2		 = pShape->GetVertexByIndex(i, iIndex2); 		
			vec2		*= pShape->GetPartMatrix(i);	
			iIndex3		= pShape->GetIndexByiOrder(i, j+2);						
			vec3		 = pShape->GetVertexByIndex(i, iIndex3); 		
			vec3		*= pShape->GetPartMatrix(i);	

			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;
			
			vpi.m_ivVector.push_back(iIndex1);	
			vpi.m_ivVector.push_back(iIndex2);	
			vpi.m_ivVector.push_back(iIndex3);	
		}

		m_viIndex.push_back(vpi);
	}

	SplitAndMakeCollision(pShape);
}

void CPortalVol::SplitAndMakeCollision(CN3Shape* pShape)
{
	m_ivColVector.clear();

	int iIndexCount = pShape->GetColIndexbufferCount();
	if (!iIndexCount)
	{
		for ( int j = 0; j < pShape->CollisionMesh()->VertexCount(); j += 3 )
		{
			__Vector3 vec1, vec2, vec3;

			vec1		 = pShape->GetColVertexByIndex(j); 
			vec2		 = pShape->GetColVertexByIndex(j+1); 		
			vec3		 = pShape->GetColVertexByIndex(j+2); 		
		
			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;

			m_ivColVector.push_back(j);
			m_ivColVector.push_back(j+1);
			m_ivColVector.push_back(j+2);
		}	
	}
	else
	{
		for ( int j = 0; j < iIndexCount; j += 3 )
		{
			int iIndex1, iIndex2, iIndex3;
			__Vector3 vec1, vec2, vec3;

			iIndex1		= pShape->GetColIndexByiOrder(j);						
			vec1		 = pShape->GetColVertexByIndex(iIndex1); 
			iIndex2		= pShape->GetColIndexByiOrder(j+1);						
			vec2		 = pShape->GetColVertexByIndex(iIndex2); 		
			iIndex3		= pShape->GetColIndexByiOrder(j+2);						
			vec3		 = pShape->GetColVertexByIndex(iIndex3); 		
		
			if ( (!IsInVolumn(vec1) && !IsInVolumn(vec2) && !IsInVolumn(vec3)) && !IsInVolumnEx(vec1, vec2, vec3) ) 
				continue;

			m_ivColVector.push_back(iIndex1);
			m_ivColVector.push_back(iIndex2);
			m_ivColVector.push_back(iIndex3);
		}	
	}
}

bool CPortalVol::IsInVolumn(__Vector3 vec)
{
	if (vec.x >= m_pvVertex[0].x && vec.x <= m_pvVertex[1].x && vec.y >= m_pvVertex[0].y && vec.y <= m_pvVertex[4].y && 
		vec.z >=  m_pvVertex[2].z && vec.z <= m_pvVertex[0].z )
		return true;

	return false;
}

bool CPortalVol::IsInVolumnEx(__Vector3 vec1, __Vector3 vec2, __Vector3 vec3)
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

bool CPortalVol::IsInVolumnExEx(__Vector3 vec1, __Vector3 vec2)
{
	__Vector3 vPick;

	float fMag2, fMag1 = (vec2 - vec1).Magnitude();	
	__Vector3 vDir; vDir = vec2 - vec1; vDir.Normalize();

	if (IsInVolumnExExEx(vec1, vDir, WALL_ZB, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, WALL_ZF, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, WALL_XL, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, WALL_XR, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, WALL_YT, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	if (IsInVolumnExExEx(vec1, vDir, WALL_YB, vPick))
	{
		fMag2 = (vPick - vec1).Magnitude();
		if ( fMag2 <= fMag1 )
			return true;
	}

	return false;
}

bool CPortalVol::IsInVolumnExExEx(__Vector3 vOrig, __Vector3 vDir, e_WallType eWT, __Vector3 &vPick)
{
	float ft, fu, fv;
	int i;

	switch (eWT)
	{
		case WALL_ZB:
			i = 6;
			break;
		case WALL_ZF:
			i = 24;
			break;
		case WALL_XL:
			i = 12;
			break;
		case WALL_XR:
			i = 18;
			break;
		case WALL_YT:
			i = 30;
			break;
		case WALL_YB:
			i = 0;
			break;
	}

	if(IntersectTriangle(vOrig, vDir, m_pvVertex[m_pIndex[i]], m_pvVertex[m_pIndex[i+1]], m_pvVertex[m_pIndex[i+2]], ft, fu, fv, &vPick))
		return true;
	if(IntersectTriangle(vOrig, vDir, m_pvVertex[m_pIndex[i+3]], m_pvVertex[m_pIndex[i+4]],	m_pvVertex[m_pIndex[i+5]], ft, fu, fv, &vPick))
		return true;

	return false;
}

bool CPortalVol::IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir,
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
