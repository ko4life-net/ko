// N3SubScene.cpp: implementation of the CN3SubScene class.
//
//////////////////////////////////////////////////////////////////////
#include "N3SubScene.h"
#include "N3Shape.h"
#include "N3Chr.h"
#include <crtdbg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3SubScene::CN3SubScene()
{
	m_dwType = OBJ_SUBSCENE;

	memset(m_pShapes, 0, sizeof(m_pShapes));
	memset(m_pChrs, 0, sizeof(m_pChrs));

	Release();
}

CN3SubScene::~CN3SubScene()
{
	Release();
}

void CN3SubScene::Release()
{
#ifdef _MAPTOOL	// 맵툴에서는 추가된 object의 포인터가 참조포인터이기 때문에 delete 않게 하기 위해서..
	memset(m_pShapes, 0, sizeof(m_pShapes));
	memset(m_pChrs, 0, sizeof(m_pChrs));
#else
	int i;
	for(i = 0; i < MAX_SUBSCENE_SHAPE; i++) { if(m_pShapes[i]) { delete m_pShapes[i]; m_pShapes[i] = NULL; } }
	for(i = 0; i < MAX_SUBSCENE_CHARACTER; i++) { if(m_pChrs[i]) { delete m_pChrs[i]; m_pChrs[i] = NULL; } }
#endif
	m_nShapeCount = 0;
	m_nChrCount = 0;
	m_fFrmCur = 0.0f; // Animation Frame;
}

bool CN3SubScene::Load(HANDLE hFile)
{
#ifdef _MAPTOOL
	_ASSERT(0);
#endif
	Release();

	int i, nL = 0;
	char szName[_MAX_PATH] = "";
	DWORD dwRWC;

	int nSC = 0;
	ReadFile(hFile, &nSC, sizeof(nSC), &dwRWC, NULL); // Shapes..
	__ObjectInfo	ObjInfo;
	for(i = 0; i < nSC; i++)
	{
		ReadFile(hFile, &nL, sizeof(nL), &dwRWC, NULL);
		if(nL < 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Shape* pShape = new CN3Shape();
		__IS_MEM_IN_BLACKLIST(pShape);
		pShape->NameSet(szName);
		pShape->LoadFromFile();

		// 위치 회전 스케일 정보 load
		ReadFile(hFile, &ObjInfo, sizeof(ObjInfo), &dwRWC, NULL);
		pShape->PosSet(ObjInfo.vPos);
		pShape->RotSet(ObjInfo.vRot);
		pShape->ScaleSet(ObjInfo.vScale);
		
		ShapeAdd(pShape);
	}

	int nChrC = 0;
	ReadFile(hFile, &nChrC, sizeof(nChrC), &dwRWC, NULL); // 캐릭터
	for(i = 0; i < nChrC; i++)
	{
		ReadFile(hFile, &nL, sizeof(nL), &dwRWC, NULL);
		if(nL < 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Chr* pChr = new CN3Chr();
		__IS_MEM_IN_BLACKLIST(pChr);
		pChr->NameSet(szName);
		pChr->LoadFromFile();
		ChrAdd(pChr);
	}

	return true;
}

bool CN3SubScene::Save(HANDLE hFile)
{
	int i, nL = 0;
	const char* szName = NULL;
	DWORD dwRWC;
	__ObjectInfo	ObjInfo;

	WriteFile(hFile, &m_nShapeCount, sizeof(m_nShapeCount), &dwRWC, NULL); // Shapes..
	for(i = 0; i < m_nShapeCount; i++)
	{
#ifdef _MAPTOOL
		// 원본 파일 이름 추출해서 저장하기..
		char szSrcName[_MAX_PATH];
		char szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
		char szNewFName[_MAX_FNAME];
		_splitpath(m_pShapes[i]->Name(), NULL, szDir, szFName, szExt);
		int iLen = lstrlen(szFName);
		__ASSERT(iLen>4, "원본파일 이름을 알 수 없다.");
		lstrcpyn(szNewFName, szFName, iLen-4+1);
		_makepath(szSrcName, NULL, szDir, szNewFName, szExt);
		szName = szSrcName;
#else
		szName = m_pShapes[i]->Name();
#endif
		nL = lstrlen(szName);
		WriteFile(hFile, &nL, sizeof(nL), &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, szName, nL, &dwRWC, NULL);
		//m_pShapes[i]->SaveToFile();

		// 위치 회전 스케일 정보 save
		ObjInfo.vPos = m_pShapes[i]->PosGet();
		ObjInfo.vRot = m_pShapes[i]->RotGet();
		ObjInfo.vScale = m_pShapes[i]->ScaleGet();

		WriteFile(hFile, &ObjInfo, sizeof(ObjInfo), &dwRWC, NULL);
	}

	WriteFile(hFile, &m_nChrCount, sizeof(m_nChrCount), &dwRWC, NULL); // 캐릭터
	for(i = 0; i < m_nChrCount; i++)
	{
		szName = m_pChrs[i]->Name();
		nL = lstrlen(szName);
		WriteFile(hFile, &nL, sizeof(nL), &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, szName, nL, &dwRWC, NULL);
		m_pChrs[i]->SaveToFile();
	}
	return true;
}

void CN3SubScene::Render()
{
#ifdef _MAPTOOL
	_ASSERT(0);
#endif
	int i;
	for(i = 0; i < m_nShapeCount; i++)
	{
		__ASSERT(m_pShapes[i], "Shape is NULL pointer!");
		m_pShapes[i]->Render();
	}

	for(i = 0; i < m_nChrCount; i++)
	{
		__ASSERT(m_pChrs[i], "Character is NULL pointer!");
		m_pChrs[i]->Render(m_fFrmCur);
	}
}

void CN3SubScene::Tick(float fFrm)
{
#ifdef _MAPTOOL
	_ASSERT(0);
#endif
	m_fFrmCur = fFrm;

//	if(m_fFrmPerSec > 2.0f)
//	{
//		if(fFrm <  m_fFrmStart || fFrm > m_fFrmEnd)
//		{
//			m_fFrmCur += 20.0f / CN3Base::m_fFrmPerSec; // 일정하게 움직이도록 시간에 따라 움직이는 양을 조절..
//			if(m_fFrmCur > m_fFrmEnd) m_fFrmCur = m_fFrmStart;
//		}
//		else
//		{
//			m_fFrmCur = fFrm;
//		}
//	}

	int i;
	for(i = 0; i < m_nShapeCount; i++)
	{
		m_pShapes[i]->Tick(fFrm);
	}

	for(i = 0; i < m_nChrCount; i++)
	{
		m_pChrs[i]->Tick(fFrm);
	}
}

int CN3SubScene::ShapeAdd(CN3Shape *pShape)
{
	if(pShape == NULL) return -1;

	for(int i = 0; i < m_nShapeCount; i++)
	{
		if(pShape == m_pShapes[i]) return -1;
	}

	if(m_pShapes[m_nShapeCount])
#ifdef _MAPTOOL
		_ASSERT(0);
#else
		delete m_pShapes[m_nShapeCount];
#endif
	m_pShapes[m_nShapeCount] = pShape;

	m_nShapeCount++;
	return m_nShapeCount;
}

void CN3SubScene::ShapeDelete(int index)
{
#ifdef _MAPTOOL
		_ASSERT(0);
#endif
	if(index < 0 || index >= m_nShapeCount) return;

	delete m_pShapes[index];
	m_pShapes[index] = NULL;
	
	m_nShapeCount--;
	for(int i = index; i < m_nShapeCount; i++) m_pShapes[i] = m_pShapes[i+1];
	m_pShapes[m_nShapeCount] = NULL;
}

void CN3SubScene::ShapeDelete(CN3Shape *pShape)
{
#ifdef _MAPTOOL
		_ASSERT(0);
#endif
	for(int i = 0; i < m_nShapeCount; i++) 
	{
		if(m_pShapes[i] == pShape) 
		{
			this->ShapeDelete(i);
			break;
		}
	}
}

int CN3SubScene::ChrAdd(CN3Chr *pChr)
{
	if(pChr == NULL) return -1;

	for(int i = 0; i < m_nChrCount; i++)
	{
		if(pChr == m_pChrs[i]) return -1;
	}

	if(m_pChrs[m_nChrCount])
#ifdef _MAPTOOL
		_ASSERT(0);
#else
		delete m_pChrs[m_nChrCount];
#endif
	m_pChrs[m_nChrCount] = pChr;

	m_nChrCount++;
	return m_nChrCount;
}

void CN3SubScene::ChrDelete(int index)
{
#ifdef _MAPTOOL
		_ASSERT(0);
#endif
	if(index < 0 || index >= m_nChrCount) return;

	delete m_pChrs[index];
	m_pChrs[index] = NULL;
	
	m_nChrCount--;
	for(int i = index; i < m_nChrCount; i++) m_pChrs[i] = m_pChrs[i+1];
	m_pChrs[m_nChrCount] = NULL;
}

void CN3SubScene::ChrDelete(CN3Chr *pChr)
{
#ifdef _MAPTOOL
		_ASSERT(0);
#endif
	for(int i = 0; i < m_nChrCount; i++) 
	{
		if(m_pChrs[i] == pChr) 
		{
			this->ChrDelete(i);
			break;
		}
	}
}

