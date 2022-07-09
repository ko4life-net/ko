// N3Board.h: interface for the CN3Board class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3BOARD_H__A6E96276_7491_4722_82C9_EA06561D1D41__INCLUDED_)
#define AFX_N3BOARD_H__A6E96276_7491_4722_82C9_EA06561D1D41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"
#include "N3Texture.h"

const DWORD BOARD_X				= 0;
const DWORD BOARD_Y				= 1;
const DWORD BOARD_Z				= 2;
const DWORD BOARD_XYZ			= 3;

class CN3Board : public CN3Transform
{
protected:
	__VertexT1 m_vRects[6];

	int				m_nTexIndex;
	int				m_nTexCount;
	CN3Texture**	m_ppTexs; // 텍스처 포인터 리스트..

public:
	DWORD m_dwBoardType; // Board Type

	float	m_fFPS; // Frame Per Second
	DWORD	m_dwTickPrev;

	__Material m_Mtl; // 재질..

public:
	void TexSet(int index, const char* szFN);
	void TexAlloc(int nCount);
	int TexCount() { return m_nTexCount; }
	CN3Texture* Tex(int index) { if(index < 0 || index >= m_nTexCount) return NULL; return m_ppTexs[index]; }

	void Init(__Vector3 vPos, DWORD dwBoardType, float fW, float fH);
	void Tick(float fFrm);
	void Render();

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	void LoadFromText(LPCTSTR szFName);
	
	void Release();

	CN3Board();
	virtual ~CN3Board();
};

#endif // !defined(AFX_N3BOARD_H__A6E96276_7491_4722_82C9_EA06561D1D41__INCLUDED_)
