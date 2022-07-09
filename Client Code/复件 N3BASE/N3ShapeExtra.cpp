// N3ShapeExtra.cpp: implementation of the CN3ShapeExtra class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3ShapeExtra.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CN3ShapeExtraPart
CN3ShapeExtra::CN3ShapeExtraPart::CN3ShapeExtraPart()
{
	m_vRotationAxis.Set(0,1,0); // 회전축..
	m_fRotationPerSec = 0.0f; // 초당 회전.. Radian...
}

CN3ShapeExtra::CN3ShapeExtraPart::~CN3ShapeExtraPart()
{
}

void CN3ShapeExtra::CN3ShapeExtraPart::Tick(DWORD dwTime)
{
	CPart::Tick(dwTime);
	if(m_bOutOfCameraRange) return;

	// 회전...
	if(m_fRotationPerSec != 0.0f)
	{
		m_fRadian += m_fRotationPerSec / s_fFrmPerSec;
	}
}

void CN3ShapeExtra::CN3ShapeExtraPart::Render()
{
}
// CN3ShapeExtraPart
//////////////////////////////////////////////////////////////////////


CN3ShapeExtra::CN3ShapeExtra()
{
	m_dwType |= OBJ_SHAPE_EXTRA;
}

CN3ShapeExtra::~CN3ShapeExtra()
{
	CN3ShapeExtra::Release();
}

void CN3ShapeExtra::Release()
{
	CN3Shape::Release();

	void* pPos= m_PartExtras.Position(0);
	for(; pPos; m_PartExtras.Next(pPos))
	{
		delete m_PartExtras.Get(pPos);
	}
	m_PartExtras.Release();

	pPos = m_Boards.Position(0);
	for(; pPos; m_Boards.Next(pPos))
	{
		delete m_Boards.Get(pPos);
	}
	m_Boards.Release();
}

bool CN3ShapeExtra::Load(HANDLE hFile)
{
/*	CN3Shape::Load(hFile);

	if(m_Parts.Count() > 0 || m_PartExtras.Count() > 0) CN3ShapeExtra::Release();

	DWORD dwRWC = 0;
	
	int nL = 0;
	char szFN[512] = "";

	int nPCE = 0;
	ReadFile(hFile, &nPCE, 4, &dwRWC, NULL); // Part Count
	for(int i = 0; i < nPCE; i++) PartExtraAdd();

	for(void* pPos = m_PartExtras.Position(0); pPos; m_PartExtras.Next(pPos))
	{
		CN3ShapeExtraPart* pPDE = m_PartExtras.Get(pPos);

		CN3PMeshInstance* pPMITmp = pPDE->m_pPMInst; // 임시로 포인터 보관..
		ReadFile(hFile, pPDE, sizeof(CN3ShapeExtraPart), &dwRWC, NULL); // Part Extra Data 읽기..
		pPDE->m_pPMInst = pPMITmp;

		ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
		__ASSERT(nL > 0, "FileName length is less than 0");
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 메시 파일 이름..
		pPDE->m_pPMInst->Create(s_MngPMesh.Get(szFN));

		pPDE->TexAlloc(pPDE->m_nTexCount); // Texture Pointer Pointer 할당..
		for(int j = 0; j < pPDE->m_nTexCount; j++) // Texture Count 만큼 파일 이름 읽어서 텍스처 부르기..
		{
			ReadFile(hFile, &nL, 4, &dwRWC, NULL);
			if(nL > 0)
			{
				ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 텍스처 파일 이름..
				pPDE->m_ppTexRefs[j] = s_MngTex.Get(szFN);
			}
		}
	}

	pPos = m_Boards.Position(0);
	for(; pPos; m_Boards.Next(pPos))
	{
		CN3ShapeExtraPart* pPDE = m_PartExtras.Get(pPos);

		CN3PMeshInstance* pPMITmp = pPDE->m_pPMInst; // 임시로 포인터 보관..
		ReadFile(hFile, pPDE, sizeof(CN3ShapeExtraPart), &dwRWC, NULL); // Part Extra Data 읽기..
		pPDE->m_pPMInst = pPMITmp;

		ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
		__ASSERT(nL > 0, "FileName length is less than 0");
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 메시 파일 이름..
		pPDE->m_pPMInst->Create(s_MngPMesh.Get(szFN));
	}
*/
	return true;
}

bool CN3ShapeExtra::Save(HANDLE hFile)
{
/*	CN3Shape::Save(hFile);

	DWORD dwRWC = 0;
	
	int nL = 0;

	int nPCE = m_PartExtras.Count();
	WriteFile(hFile, &nPCE, 4, &dwRWC, NULL); // Part Count

	for(void* pPos = m_PartExtras.Position(0); pPos; m_PartExtras.Next(pPos))
	{
		CN3ShapeExtraPart* pPDE = m_PartExtras.Get(pPos);

		WriteFile(hFile, pPDE, sizeof(CN3ShapeExtraPart), &dwRWC, NULL); // Part Extra Data 읽기..

		CN3PMesh* pPMesh = pPDE->Mesh();
		if(pPMesh) nL = lstrlen(pPMesh->Name());
		else nL = 0;
		
		WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
		if(nL > 0) WriteFile(hFile, pPMesh->Name(), nL, &dwRWC, NULL); // 메시 파일 이름..

		for(int j = 0; j < pPDE->m_nTexCount; j++) // Texture Count 만큼 파일 이름 읽어서 텍스처 부르기..
		{
			CN3Texture* pTex = pPDE->Tex(j);
			if(pTex) nL = lstrlen(pTex->Name());
			else nL = 0;

			WriteFile(hFile, &nL, 4, &dwRWC, NULL);
			if(nL > 0) WriteFile(hFile, pTex->Name(), nL, &dwRWC, NULL); // 텍스처 파일 이름..
		}
	}
*/
	return true;
}

void CN3ShapeExtra::Tick(float fFrm)
{
	CN3Shape::Tick(fFrm);

	CN3Board* pBoard = NULL;

	for(void* pPos = m_Boards.Position(0); pPos; m_Boards.Next(pPos)) 
	{
		pBoard = m_Boards.Get(pPos);

		__ASSERT(pBoard, "Board Pointer is NULL!");
		pBoard->Tick(fFrm);
	}


	DWORD dwTime = timeGetTime();
	CN3ShapeExtraPart* pPED = NULL;
	
	pPos = m_PartExtras.Position(0);
	for(; pPos; m_PartExtras.Next(pPos)) 
	{
		pPED = m_PartExtras.Get(pPos);

		__ASSERT(pPED, "part extra pointer is NULL!");
		pPED->Tick(dwTime);
	}
}

void CN3ShapeExtra::Render()
{
	CN3Shape::Render();

	CN3Board* pBoard = NULL;
	
	for(void* pPos = m_Boards.Position(0); pPos; m_Boards.Next(pPos)) 
	{
		pBoard = m_Boards.Get(pPos);

		__ASSERT(pBoard, "Board pointer is NULL!");
		pBoard->Render();
	}

	CN3ShapeExtraPart* pPED = NULL;

	pPos = m_PartExtras.Position(0);
	for(; pPos; m_PartExtras.Next(pPos))
	{
		pPED = m_PartExtras.Get(pPos);

		__ASSERT(pPED, "Part extra pointer is NULL!");
		pPED->Render();
	}
}

void CN3ShapeExtra::PartExtraAdd()
{
	CN3ShapeExtraPart* pPDE = new CN3ShapeExtraPart();
	m_PartExtras.Add(pPDE);
}

void CN3ShapeExtra::BoardAdd()
{
	CN3Board* pBoard = new CN3Board();
	m_Boards.Add(pBoard);
}

