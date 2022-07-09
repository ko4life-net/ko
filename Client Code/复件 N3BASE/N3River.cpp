// N3River.cpp: implementation of the CN3River class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "N3River.h"
#include "N3Texture.h"
#include "N3RiverPatch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3River::CN3River()
{
	m_iRiverCount = 0;
	m_RiverInfos = NULL;
	ZeroMemory(m_pRiverPatches, sizeof(m_pRiverPatches));

#ifdef _LOAD_ALL_RIVERPATCH
	m_pAllRiverPatches = NULL;
#endif

	Release();
}

CN3River::~CN3River()
{
	Release();
}

void CN3River::Release()
{
	m_CurPatchPos.x = -1; m_CurPatchPos.y = -1;
	m_MaxPatchSize.cx = m_MaxPatchSize.cy = 0;
	for (int i=0; i<m_iRiverCount; ++i)	m_RiverInfos[i].DeleteTextures();
	m_iRiverCount = 0;
	if (m_RiverInfos) {delete [] m_RiverInfos; m_RiverInfos = NULL;}
	ReleaseRiverList();
	ReleaseAllRiverPatches();

#ifdef _LOAD_ALL_RIVERPATCH
	if (m_pAllRiverPatches)
	{
		int i, j;
		for (i=0; i<m_MaxPatchSize.cy; ++i)
		{
			for (j=0; j<m_MaxPatchSize.cx; ++j)
			{
				CN3RiverPatch* pRP = m_pAllRiverPatches[i*m_MaxPatchSize.cx + j];
				if (pRP) {delete pRP; m_pAllRiverPatches[i*m_MaxPatchSize.cx + j] = NULL;}
			}
		}
		delete [] m_pAllRiverPatches; m_pAllRiverPatches = NULL;
	}
#endif
}

void CN3River::ReleaseRiverList()
{
	for(void* pPos = m_RiverList.Position(0); pPos; m_RiverList.Next(pPos))
	{
		__RiverEx* pRiverEx = m_RiverList.Get(pPos);
		if (pRiverEx) delete pRiverEx;
	}
	m_RiverList.Release();

}

void CN3River::ReleaseAllRiverPatches()
{
	int i;
	for (i=0; i<9; ++i)
	{
#ifdef _LOAD_ALL_RIVERPATCH
		m_pRiverPatches[i] = NULL;
#else
		if (m_pRiverPatches[i]) {delete m_pRiverPatches[i]; m_pRiverPatches[i] = NULL;}
#endif
	}
}

bool CN3River::Load(HANDLE hFile)
{
	Release();
	int i;
	DWORD dwNum;
	// PatchBaseName 지정
#ifndef _LOAD_ALL_RIVERPATCH
	char szDir[_MAX_DIR];	char szFName[_MAX_FNAME];
	_splitpath(m_szFileName, NULL, szDir, szFName, NULL);
	_makepath(m_PatchBaseName, NULL, szDir, szFName, NULL);
#endif
	ReadFile(hFile, &m_MaxPatchSize, sizeof(m_MaxPatchSize), &dwNum, NULL);
	ReadFile(hFile, &m_iRiverCount, sizeof(m_iRiverCount), &dwNum, NULL);
	__RiverInfo* RiverInfos = CreateRiverInfo(m_iRiverCount);
	if (RiverInfos==NULL) return false;

	for(i=0; i<m_iRiverCount; ++i)
	{
		__RiverInfo* pRiverInfo = RiverInfos + i;
		ReadFile(hFile, &(pRiverInfo->iRiverID), sizeof(pRiverInfo->iRiverID), &dwNum, NULL);
		ReadFile(hFile, &(pRiverInfo->dwAlphaFactor), sizeof(pRiverInfo->dwAlphaFactor), &dwNum, NULL);
		ReadFile(hFile, &(pRiverInfo->fSpeed1), sizeof(pRiverInfo->fSpeed1), &dwNum, NULL);
		ReadFile(hFile, &(pRiverInfo->fSpeed2), sizeof(pRiverInfo->fSpeed2), &dwNum, NULL);
		int iLen;		char szTexName[_MAX_PATH];
		ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
		if (iLen>0) ReadFile(hFile, szTexName, iLen, &dwNum, NULL);
		szTexName[iLen] = NULL;
		pRiverInfo->SetTexName(szTexName);

		// animation texture 관련..
		ReadFile(hFile, &(pRiverInfo->fAnimTexFPS), sizeof(pRiverInfo->fAnimTexFPS), &dwNum, NULL);
		int iAnimTexCount;
		ReadFile(hFile, &iAnimTexCount, sizeof(iAnimTexCount), &dwNum, NULL);
		if (iAnimTexCount <=0 ) continue;
		pRiverInfo->SetAnimTexCount(iAnimTexCount);
		for (int j=0; j<iAnimTexCount; ++j)
		{
			ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
			if (iLen>0) ReadFile(hFile, szTexName, iLen, &dwNum, NULL);
			szTexName[iLen] = NULL;
			pRiverInfo->SetAnimTexName(j, szTexName);
		}
	}
	return true;
}

bool CN3River::Save(HANDLE hFile)
{
	int i;
	DWORD dwNum;
	WriteFile(hFile, &m_MaxPatchSize, sizeof(m_MaxPatchSize), &dwNum, NULL);
	WriteFile(hFile, &m_iRiverCount, sizeof(m_iRiverCount), &dwNum, NULL);
	for(i=0; i<m_iRiverCount; ++i)
	{
		__RiverInfo* pRiverInfo = m_RiverInfos + i;
		WriteFile(hFile, &(pRiverInfo->iRiverID), sizeof(pRiverInfo->iRiverID), &dwNum, NULL);
		WriteFile(hFile, &(pRiverInfo->dwAlphaFactor), sizeof(pRiverInfo->dwAlphaFactor), &dwNum, NULL);
		WriteFile(hFile, &(pRiverInfo->fSpeed1), sizeof(pRiverInfo->fSpeed1), &dwNum, NULL);
		WriteFile(hFile, &(pRiverInfo->fSpeed2), sizeof(pRiverInfo->fSpeed2), &dwNum, NULL);
		int iLen = lstrlen(pRiverInfo->pszTexName);
		__ASSERT(iLen>0, "Texture file name length is less than 0");
		WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
		if (iLen>0) WriteFile(hFile, pRiverInfo->pszTexName, iLen, &dwNum, NULL);

		// animation texture 관련..
		WriteFile(hFile, &(pRiverInfo->fAnimTexFPS), sizeof(pRiverInfo->fAnimTexFPS), &dwNum, NULL);
		int iAnimTexCount = pRiverInfo->iAnimTexCount;
		WriteFile(hFile, &iAnimTexCount, sizeof(iAnimTexCount), &dwNum, NULL);
		for (int j=0; j<iAnimTexCount; ++j)
		{
			__ASSERT(pRiverInfo->pszAnimTexNames, "AnimTexNames가 메모리 할당되지 않았습니다.");
			iLen = lstrlen(pRiverInfo->pszAnimTexNames[j]);
			__ASSERT(iLen>0, "Texture file name length is less than 0");
			WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
			if (iLen>0) WriteFile(hFile, pRiverInfo->pszAnimTexNames[j], iLen, &dwNum, NULL);
		}
	}
	return true;
}

void CN3River::Render()
{
	if(m_RiverList.Count() <= 0) return;

	// transform
	__Matrix44 matWorld, matOld;	matWorld.Identity();
	s_lpD3DDev->GetTransform(D3DTS_WORLD, &matOld);
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

	// backup state
	DWORD dwAlphaBlend, dwDestAlpha, dwSrcAlpha, dwTextureFactor;
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
	s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_TEXTUREFACTOR, &dwTextureFactor);
	DWORD dwAlphaOP, dwAlphaArg1;
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOP);
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);

	for(void* pPos = m_RiverList.Position(0); pPos; m_RiverList.Next(pPos))
	{
		__RiverEx* pRiverEx = m_RiverList.Get(pPos);

		if (pRiverEx == NULL) continue;
		int iPrimitiveCount = pRiverEx->iIndexCount/3;
		if (iPrimitiveCount<=0) continue;

		__RiverInfo* pRiverInfo = GetRiverInfoByID(pRiverEx->iRiverID);
		__ASSERT(pRiverInfo, "River information pointer is NULL!");

		// alpha factor 있으면 투명 설정하기
		if ( (pRiverInfo->dwAlphaFactor & 0xff000000) != 0xff000000)
		{
			// render state 세팅
			s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, pRiverInfo->dwAlphaFactor);	// alpha factor
			// texture state 세팅 (alpha)
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			s_lpD3DDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		}
		else
		{
			// render state 세팅
			s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			// texture state 세팅 (alpha)
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		}

		// texture 세팅하기 (color)
		__ASSERT(pRiverInfo->pTexture, "Texture pointer is NULL!");
		s_lpD3DDev->SetTexture(0, pRiverInfo->pTexture->Get());
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		//
		__ASSERT(pRiverInfo->pAnimTextures, "Animation Texture가 지정되어 있지 않습니다.");

		LPDIRECT3DTEXTURE8 pD3DTex = NULL;
		if (pRiverInfo->pAnimTextures && pRiverInfo->pAnimTextures[pRiverInfo->iCurAnimTexFrm])
			pD3DTex = pRiverInfo->pAnimTextures[pRiverInfo->iCurAnimTexFrm]->Get();
		if (pD3DTex)
		{
			s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			s_lpD3DDev->SetTexture(1, pD3DTex);
		}
		else
		{
			s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			s_lpD3DDev->SetTexture(1, NULL);
		}

		// render
		s_lpD3DDev->SetVertexShader(FVF_XYZT2);
		HRESULT hr = s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, pRiverEx->iVertexCount, iPrimitiveCount, 
			pRiverEx->pIndices, D3DFMT_INDEX16, pRiverEx->pVertices, sizeof(__VertexXyzT2));
	}

	// restore
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &matOld);
	s_lpD3DDev->SetTexture(1, NULL);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOP);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);
	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestAlpha);
	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcAlpha);
	s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, dwTextureFactor);
}

void CN3River::Tick()
{
	// UV좌표 계산
	if (m_RiverList.Count() <= 0) return;
	int j = 0;
	float fSecPerFrm = 1.0f/s_fFrmPerSec;

	DWORD dwCurTick = timeGetTime();

	for(void* pPos = m_RiverList.Position(0); pPos; m_RiverList.Next(pPos))
	{
		__RiverEx* pRiverEx = m_RiverList.Get(pPos);
		if (pRiverEx == NULL) continue;

		__RiverInfo* pRiverInfo = GetRiverInfoByID(pRiverEx->iRiverID);
		__ASSERT(pRiverInfo, "River information pointer is NULL");

		// uv좌표 변화
		if (pRiverInfo->fSpeed1 != 0.0f)
		{
			float fDiffV1 = pRiverInfo->fSpeed1 * fSecPerFrm;
			for (j=0; j<pRiverEx->iVertexCount; ++j) pRiverEx->pVertices[j].tv += fDiffV1;
			pRiverInfo->fCurDiffV1 += fDiffV1;

			if (pRiverInfo->fCurDiffV1 > 10.0f)	// 1.0f는 너무 작아서 자주 실행할 것같다.
			{
				for (j=0; j<pRiverEx->iVertexCount; ++j) pRiverEx->pVertices[j].tv -= 10.0f;
				pRiverInfo->fCurDiffV1 -= 10.0f;
			}
		}

		// uv2좌표 변화
		if (pRiverInfo->fSpeed2 != 0.0f)
		{
			float fDiffV2 = pRiverInfo->fSpeed2 * fSecPerFrm;
			for (j=0; j<pRiverEx->iVertexCount; ++j) pRiverEx->pVertices[j].tv2 += fDiffV2;
			pRiverInfo->fCurDiffV2 += fDiffV2;

			if (pRiverInfo->fCurDiffV2 > 10.0f)	// 1.0f는 너무 작아서 자주 실행할 것같다.
			{
				for (j=0; j<pRiverEx->iVertexCount; ++j) pRiverEx->pVertices[j].tv2 -= 10.0f;
				pRiverInfo->fCurDiffV2 -= 10.0f;
			}
		}

		// animation texture
		if (dwCurTick - pRiverInfo->dwPrevTick > 1000/pRiverInfo->fAnimTexFPS)
		{
			pRiverInfo->dwPrevTick = dwCurTick;
			++(pRiverInfo->iCurAnimTexFrm);
			if (pRiverInfo->iCurAnimTexFrm >= pRiverInfo->iAnimTexCount) pRiverInfo->iCurAnimTexFrm = 0;
		}
	}
}

__RiverInfo* CN3River::CreateRiverInfo(int iRiverCount)	// m_RiverInfos 버퍼 생성
{
	if (iRiverCount <= 0) return NULL;
	m_iRiverCount = iRiverCount;
	m_RiverInfos = new __RiverInfo[iRiverCount];
	return m_RiverInfos;
}

void CN3River::SetPatchPos(int iX, int iZ)
{
	if (m_iRiverCount<=0) return;	// 이 존에는 강이 없다.

	int i, j;
	int iDX = m_CurPatchPos.x-iX;
	int iDZ = m_CurPatchPos.y-iZ;

	if (iDX==0 && iDZ == 0) return;
	ReleaseRiverList();

	if ( m_CurPatchPos.x == -1 || m_CurPatchPos.y == -1 || abs(iDX)>2 || abs(iDZ)>2)
	{
		// 처음 Load하거나 2칸을 넘게 좌표 이동하였을때
		ReleaseAllRiverPatches();
		for(i=0; i<3; ++i)
		{
			for(j=0; j<3; ++j)
			{
				m_pRiverPatches[j*3 + i] = LoadPatch(iX-1+i, iZ-1+j);
			}
		}
	}
	else
	{
		CN3RiverPatch* pOldRiverPatches[9];
		CopyMemory(pOldRiverPatches, m_pRiverPatches, sizeof(pOldRiverPatches));	// 현재 patch 포인터들 버퍼에 저장

		int iNewX, iNewZ;
		for(i=0; i<3; ++i)
		{
			for(j=0; j<3; ++j)
			{
				iNewX = iX-1+i;		iNewZ = iZ-1+j;
				int iTmpX = iNewX-m_CurPatchPos.x+1;
				int iTmpZ = iNewZ-m_CurPatchPos.y+1;
				if (iTmpX>=0 && iTmpX<3 && iTmpZ>=0 && iTmpZ<3)
				{
					// 기존에 Load된 것이 새로운9패치중에도 쓰이면 포인터 옮기기
					m_pRiverPatches[j*3 + i] = pOldRiverPatches[iTmpZ*3 + iTmpX];
					pOldRiverPatches[iTmpZ*3 + iTmpX] = NULL;
				}
				else
				{
					// 새로 Load해야 하는 것들
					m_pRiverPatches[j*3 + i] = LoadPatch(iNewX, iNewZ);
				}
			}
		}
#ifndef _LOAD_ALL_RIVERPATCH
		// 더이상 필요없는 것은 메모리 해제
		for(i=0; i<3; ++i)
		{
			for(j=0; j<3; ++j)
			{
				if (pOldRiverPatches[j*3 + i]) delete pOldRiverPatches[j*3 + i];
			}
		}
#endif
	}

	m_CurPatchPos.x = iX; m_CurPatchPos.y = iZ;

	// 강의 Vertex들을 순서에 맞게 정렬후 Index buffer 생성하기
	SortRiver();
}

void CN3River::SortRiver()
{
	int i, j, k, l;
	// 강별로..
	for (i=0; i<m_iRiverCount; ++i)
	{
		int iVC = 0;
		__VertexRiver* pVRs[2048];
		ZeroMemory(pVRs, sizeof(pVRs));

		// 특정 ID의 강을 찾는다.
		for(j=0; j<3; ++j)
		{
			for (k=0; k<3; ++k)
			{
				if (m_pRiverPatches[k*3+j] == NULL) continue;
				__River* pRiver = m_pRiverPatches[k*3+j]->GetRiverByID(m_RiverInfos[i].iRiverID);
				if (pRiver == NULL) continue;
				// __VertexRiver 포인터를 array에 카피
				for (l=0; l<pRiver->iVertexCount; ++l) pVRs[iVC++] = pRiver->pVertices + l;
			}
		}
		if (iVC==0)	// m_RiverInfos[i].iRiverID를 가진 강이 주변9칸에 포함되어 있지 않다.
		{
			m_RiverInfos[i].DeleteTextures();
			continue;
		}
		__ASSERT(lstrlen(m_RiverInfos[i].pszTexName), "Texture filename length less than 0");
		m_RiverInfos[i].GetTextures();

		// index순으로.. __VertexRiver 정렬 
		qsort(pVRs, iVC, sizeof(__VertexRiver*), CompareVertexRiver);

		// __RiverEx 에 복사
		float fCurDiffV1 = m_RiverInfos[i].fCurDiffV1;
		float fCurDiffV2 = m_RiverInfos[i].fCurDiffV2;
		__RiverEx* pRiverEx = new __RiverEx;
		pRiverEx->iRiverID = m_RiverInfos[i].iRiverID;
		pRiverEx->iVertexCount = iVC;
		__VertexXyzT2* pNewBuff = pRiverEx->pVertices = new __VertexXyzT2[iVC];
		for (j=0; j<iVC; ++j)
		{
			pNewBuff[j].Set(pVRs[j]->x, pVRs[j]->y, pVRs[j]->z, pVRs[j]->tu, pVRs[j]->tv, pVRs[j]->tu2, pVRs[j]->tv2);	// 복사
			pNewBuff[j].tv += fCurDiffV1;	// UV좌표중 V좌표 현재 시간에 맞게 수정
			pNewBuff[j].tv2 += fCurDiffV2;	// UV좌표중 V2좌표 현재 시간에 맞게 수정
		}

		// __RiverEx의 인덱스 버퍼 만들기
		int iIC = 0;

		struct __Sequence{ WORD iStart; WORD iEnd;};
		__Sequence TmpSequence[2048];
		int iSequenceCount = 0;

		WORD iFirstIndex = pVRs[0]->index;
		for (j=1; j<iVC; ++j)
		{
			if ((pVRs[j-1]->index + 1) == pVRs[j]->index)	continue;	// 계속 이어지는 Index다..

			// 이어지지 않는다.
			TmpSequence[iSequenceCount].iStart = iFirstIndex;
			TmpSequence[iSequenceCount].iEnd = pVRs[j-1]->index;
			iFirstIndex = pVRs[j]->index;

			// iIC 계산.
			int iDiff = TmpSequence[iSequenceCount].iEnd - TmpSequence[iSequenceCount].iStart + 1;
			if (iDiff>2) iIC += ((iDiff-2)*3);

			++iSequenceCount;
		}
		// 마지막 점 처리
		TmpSequence[iSequenceCount].iStart = iFirstIndex;
		TmpSequence[iSequenceCount].iEnd = pVRs[j-1]->index;
		// iIC 계산.
		int iDiff = TmpSequence[iSequenceCount].iEnd - TmpSequence[iSequenceCount].iStart + 1;
		if (iDiff>2) iIC += ((iDiff-2)*3);
		++iSequenceCount;
/*
		WORD iFirstIndex = pNewBuff[0].index;
		for (j=1; j<iVC; ++j)
		{
			if ((pNewBuff[j-1].index + 1) == pNewBuff[j].index)	continue;	// 계속 이어지는 Index다..

			// 이어지지 않는다.
			TmpSequence[iSequenceCount].iStart = iFirstIndex;
			TmpSequence[iSequenceCount].iEnd = pNewBuff[j-1].index;
			iFirstIndex = pNewBuff[j].index;

			// iIC 계산.
			int iDiff = TmpSequence[iSequenceCount].iEnd - TmpSequence[iSequenceCount].iStart + 1;
			if (iDiff>2) iIC += ((iDiff-2)*3);

			++iSequenceCount;
		}
		// 마지막 점 처리
		TmpSequence[iSequenceCount].iStart = iFirstIndex;
		TmpSequence[iSequenceCount].iEnd = pNewBuff[j-1].index;
		// iIC 계산.
		int iDiff = TmpSequence[iSequenceCount].iEnd - TmpSequence[iSequenceCount].iStart + 1;
		if (iDiff>2) iIC += ((iDiff-2)*3);
		++iSequenceCount;
*/

		// TmpSequence 정보를 토대로 Index 버퍼 만들기
		int iCount = 0;
		int iVertexIndex = 0;
		pRiverEx->iIndexCount = iIC;
		WORD* pIndices = pRiverEx->pIndices = new WORD[iIC];
		for (j=0; j<iSequenceCount; ++j)
		{
			int iDiff = TmpSequence[j].iEnd - TmpSequence[j].iStart + 1;
			if (iDiff>2)
			{
				for (k=0; k<iDiff-2; ++k)
				{
					if ( (TmpSequence[j].iStart+k)%2)
					{	// 홀
						pIndices[iCount+0] = iVertexIndex+k;
						pIndices[iCount+1] = iVertexIndex+k+2;
						pIndices[iCount+2] = iVertexIndex+k+1;
					}
					else
					{	// 짝
						pIndices[iCount+0] = iVertexIndex+k;
						pIndices[iCount+1] = iVertexIndex+k+1;
						pIndices[iCount+2] = iVertexIndex+k+2;
					}
					iCount += 3;
				}
			}
			iVertexIndex += iDiff;
		}
		__ASSERT(iCount == iIC, "count different");

		// 리스트에 추가하기
		m_RiverList.Add(pRiverEx);
	}
}

int CN3River::CompareVertexRiver( const void *arg1, const void *arg2 )
{
	int iReturn = (*((__VertexRiver**)arg1))->index - (*((__VertexRiver**)arg2))->index;
	__ASSERT(iReturn != 0, "return value is 0");
	return iReturn;
}

CN3RiverPatch* CN3River::LoadPatch(int iX, int iZ)
{
	if (iX<0 || iZ<0 || iX>=m_MaxPatchSize.cx || iZ>=m_MaxPatchSize.cy) return NULL;
#ifdef _LOAD_ALL_RIVERPATCH
	if (m_pAllRiverPatches == NULL) return NULL;
	return m_pAllRiverPatches[iZ*m_MaxPatchSize.cx + iX];
#else
	CN3RiverPatch* pRP = new CN3RiverPatch();
	char szName[_MAX_PATH];
	wsprintf(szName, "%s_%02d%02d.grp", m_PatchBaseName, iX, iZ);
	pRP->Load(szName);
	return pRP;
#endif
}

__RiverInfo* CN3River::GetRiverInfoByID(int iRiverID)
{
	int i;
	for (i=0; i<m_iRiverCount; ++i)
	{
		if(m_RiverInfos[i].iRiverID == iRiverID) return (m_RiverInfos + i);
	}
	return NULL;
}

#ifdef _LOAD_ALL_RIVERPATCH
void CN3River::LoadAllPatches(HANDLE hFile)
{
	__ASSERT(m_pAllRiverPatches == NULL &&
			m_MaxPatchSize.cx > 0 && m_MaxPatchSize.cy > 0, "load error");
	m_pAllRiverPatches = new CN3RiverPatch*[m_MaxPatchSize.cx * m_MaxPatchSize.cy];
	ZeroMemory(m_pAllRiverPatches, sizeof(CN3RiverPatch*)*m_MaxPatchSize.cx * m_MaxPatchSize.cy);
	int i, j;
	for(i=0; i<m_MaxPatchSize.cy; ++i)
	{
		for(j=0; j<m_MaxPatchSize.cx; ++j)
		{
			m_pAllRiverPatches[i*m_MaxPatchSize.cx + j] = new CN3RiverPatch();
			m_pAllRiverPatches[i*m_MaxPatchSize.cx + j]->Load(hFile);
		}
	}
}
#endif