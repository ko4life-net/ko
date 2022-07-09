// N3AnimControl.cpp: implementation of the CN3AnimControl class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3AnimControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3AnimControl::CN3AnimControl()
{
	m_dwType |= OBJ_ANIM_CONTROL;
}

CN3AnimControl::~CN3AnimControl()
{
	this->Release();
}

void CN3AnimControl::Release()
{
	CN3Base::Release();

	for(void* pPos = m_AniList.Position(0); pPos; m_AniList.Next(pPos))
	{
		delete m_AniList.Get(pPos);
	}

	m_AniList.Release(); // animation Data List
}

bool CN3AnimControl::Load(HANDLE hFile)
{
	if(m_AniList.Count() > 0) this->Release();

	DWORD dwRWC = 0;
	int nCount = 0;
	ReadFile(hFile, &nCount, 4, &dwRWC, NULL);

	int nL = 0;
	char szTmp[256];
	for(int i = 0; i < nCount; i++)
	{
		__AnimData* pData = new __AnimData;
		m_AniList.Add(pData);
		ReadFile(hFile, pData, sizeof(__AnimData), &dwRWC, NULL);
		pData->szName = NULL;
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 1)
		{
			ReadFile(hFile, szTmp, nL, &dwRWC, NULL);
			pData->NameSet(szTmp);
		}
	}

	return true;
}

bool CN3AnimControl::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	int nL = 0;
	int nCount = m_AniList.Count();
	WriteFile(hFile, &nCount, 4, &dwRWC, NULL);

	for(void* pPos = m_AniList.Position(0); pPos; m_AniList.Next(pPos))
	{
		__AnimData* pData = m_AniList.Get(pPos);

		WriteFile(hFile, pData, sizeof(__AnimData), &dwRWC, NULL);
		nL = lstrlen(pData->Name()) + 1;
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL > 1)
		{
			WriteFile(hFile, pData->Name(), nL, &dwRWC, NULL);
		}
	}

	return true;
}
