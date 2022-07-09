// FXMgr.cpp: implementation of the CFXMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "FXMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFXMgr::CFXMgr()
{
	m_nEffectCount = 0;
	memset(m_pEffects, 0, sizeof(m_pEffects));
}

CFXMgr::~CFXMgr()
{
	this->Release();
}

void CFXMgr::ReleaseInstances()
{
	CN3EffectInstance* pInst = NULL;
	for(void* pPos = m_EffectInstances.Position(0); pPos; m_EffectInstances.Next(pPos))
	{
		pInst = m_EffectInstances.Get(pPos);
		delete pInst;
	}
	m_EffectInstances.Release();
}

void CFXMgr::Release()
{
	this->ReleaseInstances();

	for(int i = 0; i < m_nEffectCount; i++)
	{
		delete m_pEffects[i];
		m_pEffects[i] = NULL;
	}
	m_nEffectCount = 0;
}

void CFXMgr::Trigger(const char* szEffectName, __Vector3& vPos, __Vector3& vDir, float fVelocity, CN3Shape* pShape)
{
	CN3EffectInstance* pInst = NULL;
	for(int i = 0; i < m_nEffectCount; i++)
	{
		if(lstrcmpi(m_pEffects[i]->Name(), szEffectName) == 0)
		{
			pInst = new CN3EffectInstance();
			
			pInst->Create(m_pEffects[i]);
			
			pInst->m_vStart = pInst->m_vPosCur = pInst->m_vPosPrev = vPos;
			pInst->m_vDir = vDir;
			m_EffectInstances.Add(pInst);

			break;
		}
	}
}


void CFXMgr::Tick()
{
	DWORD dwTick = timeGetTime();
	
	CN3EffectInstance* pInst;
	
	for(void* pPos = m_EffectInstances.Position(0); pPos; )
	{
		pInst = m_EffectInstances.Get(pPos);

		pInst->Tick(dwTick);

/*		if( eState == ES_FB_PREPARE)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_FIREBALLREADY1 + g_Facade.m_pSndMgr->m_IdxFireballReady, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_FIREBALLREADY1 + g_Facade.m_pSndMgr->m_IdxFireballReady, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_FIREBALLREADY1 + g_Facade.m_pSndMgr->m_IdxFireballReady);
			g_Facade.m_pSndMgr->m_IdxFireballReady++;
			if(g_Facade.m_pSndMgr->m_IdxFireballReady == g_Facade.m_pSndMgr->m_NumFireballReady) g_Facade.m_pSndMgr->m_IdxFireballReady = 0;
		}
		if( eState == ES_FB_FLY)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_FIREBALLFLY1 + g_Facade.m_pSndMgr->m_IdxFireballFly, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_FIREBALLFLY1 + g_Facade.m_pSndMgr->m_IdxFireballFly, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_FIREBALLFLY1 + g_Facade.m_pSndMgr->m_IdxFireballFly);
			g_Facade.m_pSndMgr->m_IdxFireballFly++;
			if(g_Facade.m_pSndMgr->m_IdxFireballFly == g_Facade.m_pSndMgr->m_NumFireballFly) g_Facade.m_pSndMgr->m_IdxFireballFly = 0;
		}
		if( eState == ES_FB_EXPLODE)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_FIREBALLEXPL1 + g_Facade.m_pSndMgr->m_IdxFireballExpl, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_FIREBALLEXPL1 + g_Facade.m_pSndMgr->m_IdxFireballExpl, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_FIREBALLEXPL1 + g_Facade.m_pSndMgr->m_IdxFireballExpl);
			g_Facade.m_pSndMgr->m_IdxFireballExpl++;
			if(g_Facade.m_pSndMgr->m_IdxFireballExpl == g_Facade.m_pSndMgr->m_NumFireballExpl) g_Facade.m_pSndMgr->m_IdxFireballExpl = 0;
		}
		if( eState == ES_IC_PREPARE)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_ICECOMETREADY1 + g_Facade.m_pSndMgr->m_IdxIcecometReady, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_ICECOMETREADY1 + g_Facade.m_pSndMgr->m_IdxIcecometReady, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_ICECOMETREADY1 + g_Facade.m_pSndMgr->m_IdxIcecometReady);
			g_Facade.m_pSndMgr->m_IdxIcecometReady++;
			if(g_Facade.m_pSndMgr->m_IdxIcecometReady == g_Facade.m_pSndMgr->m_NumIcecometReady) g_Facade.m_pSndMgr->m_IdxIcecometReady = 0;
		}
		if( eState == ES_IC_FLY)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_ICECOMETFLY1 + g_Facade.m_pSndMgr->m_IdxIcecometFly, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_ICECOMETFLY1 + g_Facade.m_pSndMgr->m_IdxIcecometFly, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_ICECOMETFLY1 + g_Facade.m_pSndMgr->m_IdxIcecometFly);
			g_Facade.m_pSndMgr->m_IdxIcecometFly++;
			if(g_Facade.m_pSndMgr->m_IdxIcecometFly == g_Facade.m_pSndMgr->m_NumIcecometFly) g_Facade.m_pSndMgr->m_IdxIcecometFly = 0;
		}
		if( eState == ES_IC_EXPLODE)
		{
			g_Facade.m_pSndMgr->Looping(SND3DOBJ_ICECOMETEXPL1 + g_Facade.m_pSndMgr->m_IdxIcecometExpl, false);
			g_Facade.m_pSndMgr->SetPos(SND3DOBJ_ICECOMETEXPL1 + g_Facade.m_pSndMgr->m_IdxIcecometExpl, (D3DVECTOR)pInst->m_vPosCur);
			g_Facade.m_pSndMgr->Play(SND3DOBJ_ICECOMETEXPL1 + g_Facade.m_pSndMgr->m_IdxIcecometExpl);
			g_Facade.m_pSndMgr->m_IdxIcecometExpl++;
			if(g_Facade.m_pSndMgr->m_IdxIcecometExpl == g_Facade.m_pSndMgr->m_NumIcecometExpl) g_Facade.m_pSndMgr->m_IdxIcecometExpl = 0;
		}*/

		if(pInst->m_pEffectRef && pInst->TickStep() >= pInst->m_pEffectRef->TickStepCount())
		{
			delete pInst; pInst = NULL;
			m_EffectInstances.Delete(pPos);
		}

		if(NULL != pInst) m_EffectInstances.Next(pPos);
	}
}



void CFXMgr::Render()
{
	// Effect..
	int iCount = m_EffectInstances.Count();

	if(iCount <= 0) return;

	// Turn off culling
	DWORD dwCull, dwZWrite, dwLgt, dwAlpha, dwFog; // , dwCV, dwSC
	s_lpD3DDev->GetRenderState( D3DRS_CULLMODE, &dwCull);
	s_lpD3DDev->GetRenderState( D3DRS_ZWRITEENABLE,	&dwZWrite);
//	s_lpD3DDev->GetRenderState( D3DRS_COLORVERTEX, &dwCV);
//	s_lpD3DDev->GetRenderState( D3DRS_SPECULARENABLE, &dwSC);
	s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwLgt );
	s_lpD3DDev->GetRenderState( D3DRS_ALPHABLENDENABLE,	&dwAlpha );
	s_lpD3DDev->GetRenderState( D3DRS_FOGENABLE, &dwFog );
	
	if(D3DCULL_NONE != dwCull) s_lpD3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	if(FALSE != dwZWrite) s_lpD3DDev->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );
//	if(TRUE != dwCV) s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX, TRUE);
//	s_lpD3DDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE);
	if(FALSE != dwLgt) s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE); // Turn off D3D lighting
	if(TRUE != dwAlpha) s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	if(FALSE != dwFog) s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	for(void* pPos = m_EffectInstances.Position(0); pPos; m_EffectInstances.Next(pPos))
	{
		CN3EffectInstance* pInst = m_EffectInstances.Get(pPos);
		pInst->Render();
	}

	s_lpD3DDev->SetRenderState( D3DRS_CULLMODE, dwCull );
	s_lpD3DDev->SetRenderState( D3DRS_ZWRITEENABLE, dwZWrite );
//	s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX, dwCV); // Turn off D3D lighting
//	s_lpD3DDev->SetRenderState( D3DRS_SPECULARENABLE, dwSC);
	s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwLgt );
	s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE,	dwAlpha );
	s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, dwFog );
}

CN3Effect* CFXMgr::EffectAddFromFile(const char* szFN)
{
	m_pEffects[m_nEffectCount] = new CN3Effect();
	m_pEffects[m_nEffectCount]->Parsing(szFN); // Script Parsing
	m_nEffectCount++;

	return m_pEffects[m_nEffectCount-1];
}

CN3Effect* CFXMgr::EffectAdd()
{
	m_pEffects[m_nEffectCount] = new CN3Effect();
	m_nEffectCount++;

	return m_pEffects[m_nEffectCount-1];
}

bool CFXMgr::Load(HANDLE hFile)
{
	this->Release();

	DWORD dwRWC = 0;
	ReadFile(hFile, &m_nEffectCount, 4, &dwRWC, NULL); // 헤더 읽고..
	
	int nFN = 0;
	char szFN[256] = "";
	for(int i = 0; i < m_nEffectCount; i++)
	{
		ReadFile(hFile, &nFN, 4, &dwRWC, NULL); // 헤더 읽고..
		if(nFN > 0)
		{
			ReadFile(hFile, szFN, nFN, &dwRWC, NULL); // 헤더 읽고..
			szFN[nFN] = NULL;

			this->EffectAddFromFile(szFN); // Effect Add
		}
		else
		{
			MessageBox(s_hWndBase, "Effect filename is null", "Effect loading error", MB_OK);
		}
	}

	return true;
}

bool CFXMgr::Save(HANDLE hFile)
{
	return true;
}

