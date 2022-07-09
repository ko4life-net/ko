// N3River.h: interface for the CN3River class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3RIVER_H__D0171C53_F631_4EC3_9D42_B4B754093FAC__INCLUDED_)
#define AFX_N3RIVER_H__D0171C53_F631_4EC3_9D42_B4B754093FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3LinkedList.h"

struct __RiverInfo
{
	// 고정된 정보
	short		iRiverID;
	DWORD		dwAlphaFactor;
	float		fSpeed1;
	float		fSpeed2;
	char*		pszTexName;

	// animation texture 정보
	float		fAnimTexFPS;	// 초당 프레임수
	int			iAnimTexCount;	// Animation texture 갯수
	char**		pszAnimTexNames;// animation texture 이름들

	// 실시간으로 변하는 정보
	float		fCurDiffV1;		// 원본과 현재 V좌표의 차이
	float		fCurDiffV2;		// 원본과 현재 V2좌표의 차이
	CN3Texture*	pTexture;		// Texture 참조포인터 (NULL이 아니면 Load되어 있는 것이다.
	int			iCurAnimTexFrm;	// 현재 animation texture의 프에임 번호..
	CN3Texture** pAnimTextures;	// animation texture포인터들..(NULL이 아니면 Load되어 있는 것이다.
	DWORD		dwPrevTick;		// 이전 iCurAnimTexFrm을 증가시킨 tickcount

	__RiverInfo () {iRiverID = -1; dwAlphaFactor = 0x00000000; fSpeed1 = fSpeed2 = 0; pszTexName = NULL;
					fAnimTexFPS = 30.0f; iAnimTexCount = 0; pszAnimTexNames = NULL;
					fCurDiffV1 = fCurDiffV2 = 0; pTexture = NULL; iCurAnimTexFrm = 0; pAnimTextures = NULL; dwPrevTick = 0;}
	~__RiverInfo () { if (pszTexName) delete [] pszTexName;
						__ASSERT(pTexture==NULL, "Texture 참조 포인터가 해제 되지 않았습니다.");
						for(int i=0; i<iAnimTexCount; ++i)
						{	__ASSERT(pszAnimTexNames, "");
							if (pszAnimTexNames[i]) {delete [] pszAnimTexNames[i]; pszAnimTexNames[i] = NULL;}
							__ASSERT(pAnimTextures, ""); __ASSERT(pAnimTextures[i]==NULL, "Texture 참조 포인터가 해제 되지 않았습니다.");
						}
						if (pszAnimTexNames) {delete [] pszAnimTexNames; pszAnimTexNames = NULL;}
						if (pAnimTextures) {delete [] pAnimTextures; pAnimTextures = NULL;}
					}
	void		SetTexName(const char* pszName)
	{
		int iLen = lstrlen(pszName); if (iLen==0) return;
		if(pszTexName) delete [] pszTexName;
		pszTexName = new char[iLen+1];
		lstrcpy(pszTexName, pszName);
	}
	void		SetAnimTexName(int iIndex, const char* pszName)
	{
		__ASSERT(pAnimTextures && pszAnimTexNames, "SetAnimTexCount를 한번 호출해야 한다.");
		int iLen = lstrlen(pszName); if (iLen==0) return;
		if(pszAnimTexNames[iIndex]) delete [] pszAnimTexNames[iIndex];
		pszAnimTexNames[iIndex] = new char[iLen+1];
		lstrcpy(pszAnimTexNames[iIndex], pszName);
	}
	void		SetAnimTexCount(int iCount)	// 이함수는 처음에 한번만 불러줘야 한다.
	{
		__ASSERT(pAnimTextures == NULL && iAnimTexCount == 0 && pszAnimTexNames == NULL && iCount>0,"이함수는 처음에 한번만 불러줘야 한다.");
		iAnimTexCount = iCount;
		pszAnimTexNames = new char*[iAnimTexCount];
		pAnimTextures = new CN3Texture*[iAnimTexCount];
		ZeroMemory(pszAnimTexNames, sizeof(char*)*iAnimTexCount);
		ZeroMemory(pAnimTextures, sizeof(CN3Texture*)*iAnimTexCount);
	}
	void		DeleteTextures()
	{
		if (pTexture) { CN3Base::s_MngTex.Delete(pTexture); pTexture = NULL;}
		for (int i=0; i < iAnimTexCount; ++i)	{	__ASSERT(pAnimTextures, "pAnimTextures is NULL");
			CN3Base::s_MngTex.Delete(pAnimTextures[i]);		pAnimTextures[i] = NULL;}
	}
	void		GetTextures()
	{
		char szFN[_MAX_PATH] = "";
		if (pTexture == NULL)
		{
			wsprintf(szFN, "Terrain\\River\\%s", pszTexName);
			pTexture = CN3Base::s_MngTex.Get(szFN);
		}
		__ASSERT(pTexture, "pTexture is NULL");
		for (int i=0; i < iAnimTexCount; ++i)	{	__ASSERT(pAnimTextures, "pAnimTextures is NULL");
			__ASSERT(pszAnimTexNames, "pAnimTextures is NULL");
			if (pAnimTextures[i] != NULL) continue;
			wsprintf(szFN, "Terrain\\River\\%s", pszAnimTexNames[i]);
			pAnimTextures[i] = CN3Base::s_MngTex.Get(szFN);
			__ASSERT(pAnimTextures[i], "pAnimTextures[i] is NULL");}
	}
};

#define _LOAD_ALL_RIVERPATCH		// 모든 patch를 한꺼번에 Load하여 놓고 쓸때 선언하자
// 선언하지 않으면 주변 9칸의 patch정보 파일만 load하여 사용한다.

class CN3RiverPatch;
class CN3River : public CN3Base  // CN3RiverPatch를 관리하는 클래스
{
public:
	CN3River();
	virtual ~CN3River();

// structures
protected:

// Attributes
public:
	void			SetMaxPatchSize(int iX, int iZ) {m_MaxPatchSize.cx = iX; m_MaxPatchSize.cy = iZ;};
protected:
	int				m_iRiverCount;		// 모든 강의 수
	__RiverInfo*	m_RiverInfos;		// 모든 강의 정보
	CN3RiverPatch*	m_pRiverPatches[9];	// 주변 9칸의 패치정보
	POINT			m_CurPatchPos;		//현재 있는 위치의 Patch좌표
	SIZE			m_MaxPatchSize;		// Patch가 가로 세로 몇개가 있는지..
	CN3LinkedList<struct __RiverEx*> m_RiverList;	// 실제로 그려질 수 있는 강들의 리스트

#ifndef _LOAD_ALL_RIVERPATCH
	char			m_PatchBaseName[_MAX_PATH];	//Patch파일의 기본적인 이름 (맨뒤의 XXXX의 위치좌표를 뺀 이름)
#else
	CN3RiverPatch**	m_pAllRiverPatches;	// 이 존의 모든 patch정보

public:
	void LoadAllPatches(HANDLE hFile);	// 모든 강 패치정보를 한꺼번에 Load
#endif

// Operations
public:
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual bool	Save(HANDLE hFile);
	void			Render();
	void			Tick();

	__RiverInfo*	CreateRiverInfo(int iRiverCount);	// m_RiverInfos 버퍼 생성
	void			SetPatchPos(int iX, int iZ);		// 현재 있는 위치 patch좌표 설정
protected:
	__RiverInfo*	GetRiverInfoByID(int iRiverID);	
	void			ReleaseRiverList();					
	void			ReleaseAllRiverPatches();			// RiverPatch를 모두 Release
	CN3RiverPatch*	LoadPatch(int iX, int iZ);			// 해당 위치의 patch를 new하여 load한다.
	void			SortRiver();						// 강의 점들을 다시 정렬한다.
	static int		CompareVertexRiver( const void *arg1, const void *arg2 );	// __VertexRiver점을 정렬할때 비교하는 함수
};

#endif // !defined(AFX_N3RIVER_H__D0171C53_F631_4EC3_9D42_B4B754093FAC__INCLUDED_)
