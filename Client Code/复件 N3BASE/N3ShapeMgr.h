// N3ShapeMgr.h: interface for the CN3ShapeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3SHAPEMGR_H__36456F66_1D60_4589_A5D9_70B94C2C3127__INCLUDED_)
#define AFX_N3SHAPEMGR_H__36456F66_1D60_4589_A5D9_70B94C2C3127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Shape.h"

const int CELL_MAIN_DEVIDE = 4; // 기본 셀에 대해서 4 X 4 로 나눈다..
const int CELL_SUB_SIZE = 8; // 8 Meter 가 레벨1 셀의 사이즈이다..
const int CELL_MAIN_SIZE = CELL_MAIN_DEVIDE * CELL_SUB_SIZE; // 8 Meter 가 레벨1 셀의 사이즈이다..
const int MAX_CELL_MAIN = 4096 / (CELL_MAIN_DEVIDE * CELL_SUB_SIZE); // 최대 너비, 길이...
const int MAX_MGR_SHAPE = 4096;

class CN3Camera;

class CN3ShapeMgr : public CN3Base  
{
public:
	struct __CellSub // 하위 셀 데이터
	{
		int 	nCCPolyCount; // Collision Check Polygon Count
		WORD*	pwCCVertIndices; // Collision Check Polygon Vertex Indices - wCCPolyCount * 3 만큼 생성된다.

		void Load(HANDLE hFile)
		{
			DWORD dwRWC = 0;
			ReadFile(hFile, &nCCPolyCount, 4, &dwRWC, NULL);
			if(nCCPolyCount > 0)
			{
				if(pwCCVertIndices) delete [] pwCCVertIndices;
				pwCCVertIndices = new WORD[nCCPolyCount * 3];
				ReadFile(hFile, pwCCVertIndices, nCCPolyCount * 3 * 2, &dwRWC, NULL);
//#if _DEBUG				
//				static char szTrace[256];
//				wsprintf(szTrace, "CollisionCheckPolygon : %d\n", nCCPolyCount);
//				OutputDebugString(szTrace);
//#endif
			}
		}
		void Save(HANDLE hFile)
		{
			DWORD dwRWC = 0;
			WriteFile(hFile, &nCCPolyCount, 4, &dwRWC, NULL);
			if(nCCPolyCount > 0)
				WriteFile(hFile, pwCCVertIndices, nCCPolyCount * 3 * 2, &dwRWC, NULL);
		}

		__CellSub() { memset(this, 0, sizeof(__CellSub)); }
		~__CellSub() { delete [] pwCCVertIndices; }
	};

	struct __CellMain // 기본 셀 데이터
	{
		int		nShapeCount; // Shape Count;
		WORD*	pwShapeIndices; // Shape Indices
		__CellSub SubCells[CELL_MAIN_DEVIDE][CELL_MAIN_DEVIDE];

		void Load(HANDLE hFile)
		{
			DWORD dwRWC = 0;
			ReadFile(hFile, &nShapeCount, 4, &dwRWC, NULL);
			if(nShapeCount > 0)
			{
				if(pwShapeIndices) delete [] pwShapeIndices;
				pwShapeIndices = new WORD[nShapeCount];
				ReadFile(hFile, pwShapeIndices, nShapeCount * 2, &dwRWC, NULL);
			}
			for(int z = 0; z < CELL_MAIN_DEVIDE; z++)
			{
				for(int x = 0; x < CELL_MAIN_DEVIDE; x++)
				{
					SubCells[x][z].Load(hFile);
				}
			}
		}

		void Save(HANDLE hFile)
		{
			DWORD dwRWC = 0;
			WriteFile(hFile, &nShapeCount, 4, &dwRWC, NULL);
			if(nShapeCount > 0) WriteFile(hFile, pwShapeIndices, nShapeCount * 2, &dwRWC, NULL);
			for(int z = 0; z < CELL_MAIN_DEVIDE; z++)
			{
				for(int x = 0; x < CELL_MAIN_DEVIDE; x++)
				{
					SubCells[x][z].Save(hFile);
				}
			}
		}
		
		__CellMain() { nShapeCount = 0; pwShapeIndices = NULL; }
		~__CellMain() { delete [] pwShapeIndices; }
	};

protected:
	int			m_nShapeCount;
	CN3Shape*	m_pShapes[MAX_MGR_SHAPE];
	
	int			m_nCollisionFaceCount;
	__Vector3* 	m_pvCollisions;

	__CellMain* m_pCells[MAX_CELL_MAIN][MAX_CELL_MAIN];

	CN3LinkedList<CN3Shape*> m_ShapesToRender; // Tick 을 호출하면 렌더링할 것만 추린다..

public:
	__CellSub* SubCell(__Vector3& vPos) // 해당 위치의 셀 포인터를 돌려준다.
	{
		int x = (int)(vPos.x / CELL_MAIN_SIZE);
		int z = (int)(vPos.z / CELL_MAIN_SIZE);
		
		__ASSERT(x >= 0 && x < MAX_CELL_MAIN && z >= 0 && z < MAX_CELL_MAIN, "Invalid cell number");

		if(NULL == m_pCells[x][z]) return NULL;

		int xx = (((int)vPos.x)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;
		int zz = (((int)vPos.z)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;

		return &(m_pCells[x][z]->SubCells[xx][zz]);
	}

	void RenderCollision(__Vector3& vPos);
	bool CheckCollision(__Vector3& vPos, __Vector3& vPosNext, __Vector3* pvCol, __Vector3* pvNormal);
	void Render();
	void Tick();

	void ReleaseShapes();
	int Add(CN3Shape* pShape);
	void GenerateCollisionData();
	void Create(int nCellWidthCount, int nCellLengthCount); // Cell 갯수를 넣는다. 
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	void Release();
	CN3ShapeMgr();
	virtual ~CN3ShapeMgr();

};

#endif // !defined(AFX_N3SHAPEMGR_H__36456F66_1D60_4589_A5D9_70B94C2C3127__INCLUDED_)
