// N3ShapeExtra.h: interface for the CN3ShapeExtra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3SHAPEEXTRA_H__F3D6E8DF_A20F_45F5_8729_2FD36796969C__INCLUDED_)
#define AFX_N3SHAPEEXTRA_H__F3D6E8DF_A20F_45F5_8729_2FD36796969C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Shape.h"
#include "N3Board.h"

class CN3ShapeExtra : public CN3Shape  
{
	class CN3ShapeExtraPart : public CN3Shape::CPart
	{
		friend CN3ShapeExtra;
	public:
		__Vector3	m_vRotationAxis; // 회전축..
		float		m_fRadian; // 현재 회전한 각도..
		float		m_fRotationPerSec; // 초당 회전.. Radian...

	public:
		void Tick(DWORD dwTime);
		void Render();

		CN3ShapeExtraPart();
		~CN3ShapeExtraPart();
	};

public:
	CN3LinkedList<CN3ShapeExtraPart*> m_PartExtras;
	CN3LinkedList<CN3Board*> m_Boards;

public:
	int PartExtraCount() { return m_PartExtras.Count(); }
	CN3ShapeExtraPart* PartExtra(int nPart) { if(nPart < 0 || nPart >= m_PartExtras.Count()) return NULL; return m_PartExtras.Get(nPart); }
	void PartExtraAdd();
	
	int BoardCount() { return m_Boards.Count(); }
	CN3Board* Board(int index) { if(index < 0 || index >= m_Boards.Count()) return NULL; return m_Boards.Get(index); }
	void BoardAdd();
	
	void Tick(float fFrm);
	void Render();
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	void Release();
	CN3ShapeExtra();
	virtual ~CN3ShapeExtra();

};

#endif // !defined(AFX_N3SHAPEEXTRA_H__F3D6E8DF_A20F_45F5_8729_2FD36796969C__INCLUDED_)
