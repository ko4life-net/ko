// PortalWall.cpp: implementation of the CPortalWall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "n3indoor.h"

#include "PvsObjFactory.h"
#include "PortalWall.h"
#include "PortalVol.h"

#include "PvsMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CPortalWall, CPvsBase )

CPortalWall::CPortalWall(e_WallType eWT, CPortalVol* pVol)
{
	m_eWallType = eWT;
	unsigned short* pIdx = m_pIndex;
	int i;

	if (pVol)
	{
		switch (m_eWallType)
		{
			case WALL_ZB:
				m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[1].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
				break;

			case WALL_ZF:
				m_pvVertex[0].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[2].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
				m_pvVertex[3].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
				break;

			case WALL_XL:
				m_pvVertex[0].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[1].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[2].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[3].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				break;

			case WALL_XR:
				m_pvVertex[0].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[1].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);			
				m_pvVertex[2].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[3].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				break;

			case WALL_YB:
				m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[2].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				break;

			case WALL_YT:
				m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[1].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
				m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
				break;
		}

		m_MtxScale = pVol->m_MtxScale;
		m_MtxMove = pVol->m_MtxMove;
	}
	else
	{
		for ( i = 0; i < 4; i++ )
			m_pvVertex[i].Set(0.0f, 0.0f, 0.0f, dwColorNoneAlpha);
	}

	*pIdx++ = 0;  *pIdx++ = 1;  *pIdx++ = 2;
	*pIdx++ = 3;  *pIdx++ = 2;  *pIdx++ = 1;

	if (pVol)
		Translate();
}

void CPortalWall::Translate()
{
	__Vector3 vec[4];

	switch (m_eWallType)
	{
		case WALL_ZB:
			vec[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize);
			vec[1].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize);
			vec[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize);
			vec[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize);
			break;

		case WALL_ZF:
			vec[0].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize);
			vec[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize);
			vec[2].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize);
			vec[3].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize);
			break;

		case WALL_XL:
			vec[0].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, fBaseWallSize-fWallSizeOffset);
			vec[1].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, fBaseWallSize-fWallSizeOffset);
			vec[2].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset);
			vec[3].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset);
			break;

		case WALL_XR:
			vec[0].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset);
			vec[1].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset);			
			vec[2].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset);
			vec[3].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset);
			break;

		case WALL_YB:
			vec[0].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset);
			vec[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset);
			vec[2].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset);
			vec[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset);
			break;

		case WALL_YT:
			vec[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset);
			vec[1].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset);
			vec[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset);
			vec[3].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset);
			break;
	}

	for( int i = 0; i < 4; i++)
	{
		vec[i] *= m_MtxScale;
		vec[i] *= m_MtxMove;
		m_pvVertex[i].x = vec[i].x; m_pvVertex[i].y = vec[i].y; m_pvVertex[i].z = vec[i].z; m_pvVertex[i].color = dwColorNoneAlpha;
	}
}

CPortalWall::~CPortalWall()
{
}

bool CPortalWall::CreatePvsObject()
{
	return true;
}

bool CPortalWall::DeletePvsObject()
{
	return true;
}

void CPortalWall::TickEdit()
{


}

void CPortalWall::TickCompile()
{


}

void CPortalWall::TickExecute()
{


}

void CPortalWall::RenderEdit()
{
	DWORD dwAlpha, dwFog, dwLight, dwCullMode, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(dwCullMode != D3DCULL_NONE) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
    CN3Base::s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
    CN3Base::s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX, TRUE);
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

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
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	CN3Base::s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, m_pIndex, D3DFMT_INDEX16, m_pvVertex, sizeof(__VertexColor) );
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPortalWall::RenderCompile()
{


}

void CPortalWall::RenderExecute()
{


}

void CPortalWall::SetState(e_PvsState ePS)
{
	int i = 0;
	switch (ePS)
	{
		case STATE_NONE:
			for(i = 0; i < 4; i++)
				m_pvVertex[i].color = dwColorNoneAlpha;
			break;
		case STATE_SELECTED:
			for(i = 0; i < 4; i++)
				m_pvVertex[i].color = dwColorSelectedAlpha;
			break;
		case STATE_LINKED:
			for(i = 0; i < 4; i++)
				m_pvVertex[i].color = dwColorLinkedAlpha;
			break;
	}

	m_eState = ePS;
}

void CPortalWall::Load(FILE* stream)
{
	switch (m_eWallType)
	{
		case WALL_ZB:
			m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[1].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize, dwColorNoneAlpha);
			break;

		case WALL_ZF:
			m_pvVertex[0].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[2].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
			m_pvVertex[3].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize, dwColorNoneAlpha);
			break;

		case WALL_XL:
			m_pvVertex[0].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[1].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[2].Set(-fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[3].Set(-fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			break;

		case WALL_XR:
			m_pvVertex[0].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[1].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);			
			m_pvVertex[2].Set( fBaseWallSize,  fBaseWallSize-fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[3].Set( fBaseWallSize, -fBaseWallSize+fWallSizeOffset,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			break;

		case WALL_YB:
			m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[1].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[2].Set(-fBaseWallSize+fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset, -fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			break;

		case WALL_YT:
			m_pvVertex[0].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[1].Set(-fBaseWallSize+fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[2].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize,  fBaseWallSize-fWallSizeOffset, dwColorNoneAlpha);
			m_pvVertex[3].Set( fBaseWallSize-fWallSizeOffset,  fBaseWallSize, -fBaseWallSize+fWallSizeOffset, dwColorNoneAlpha);
			break;
	}

	CPvsBase::Load(stream);

	unsigned short* pIdx = m_pIndex;

	*pIdx++ = 0;  *pIdx++ = 1;  *pIdx++ = 2;
	*pIdx++ = 3;  *pIdx++ = 2;  *pIdx++ = 1;
}

void CPortalWall::Save(FILE* stream)
{
	// 먼저 자신의 타입을 저장..
	fwrite(&_iTypeWall, sizeof(int), 1, stream);
	fwrite(&m_eWallType, sizeof(int), 1, stream);

	// 자신의 아이디를 저장..
	fwrite(&m_iID, sizeof(int), 1, stream);	

	// 자신의 데이터 저장..
	CPvsBase::Save(stream);
}

void CPortalWall::CopyToData(CPortalWall* pLinked, CPortalVol* pVol)
{
	m_MtxScale = pLinked->m_MtxScale;
	m_MtxMove = pVol->m_MtxMove;

	Translate();
}





