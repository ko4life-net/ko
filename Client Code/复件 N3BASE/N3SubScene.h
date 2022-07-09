// N3SubScene.h: interface for the CN3SubScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3SUBSCENE_H__A82F1D15_F10E_49BB_A975_F0B6926F3673__INCLUDED_)
#define AFX_N3SUBSCENE_H__A82F1D15_F10E_49BB_A975_F0B6926F3673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

const int MAX_SUBSCENE_SHAPE = 1024;
const int MAX_SUBSCENE_CHARACTER = 256;

class CN3Shape;
class CN3Chr;
class CN3SubScene : public CN3Base  
{
public:
	CN3SubScene();
	virtual ~CN3SubScene();

	struct __ObjectInfo				// N3Shape의 위치 회전 확대/축소 정보
	{
		__Vector3		vPos;
		__Vector3		vRot;
		__Vector3		vScale;
	};

protected:
	int m_nShapeCount;
	int m_nChrCount;
	float m_fFrmCur;

	CN3Shape*		m_pShapes[MAX_SUBSCENE_SHAPE];
	CN3Chr*			m_pChrs[MAX_SUBSCENE_CHARACTER];

public:
	void	Tick(float fFrm = -100000.0f);
	void	Render();

	int		ShapeAdd(CN3Shape* pShape);
	void	ShapeDelete(CN3Shape* pShape);
	void	ShapeDelete(int index);
	int		ShapeCount() { return m_nShapeCount; }
	CN3Shape* ShapeGet(int index) { if(index < 0 || index >= m_nShapeCount) return NULL; return m_pShapes[index]; }

	int		ChrAdd(CN3Chr* pChr);
	void	ChrDelete(int index);
	void	ChrDelete(CN3Chr* pChr);
	int		ChrCount() { return m_nChrCount; }
	CN3Chr* ChrGet(int index) { if(index < 0 || index >= m_nChrCount) return NULL; return m_pChrs[index]; }

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	void Release();
};

#endif // !defined(AFX_N3SUBSCENE_H__A82F1D15_F10E_49BB_A975_F0B6926F3673__INCLUDED_)
