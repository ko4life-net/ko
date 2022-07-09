// N3SMesh.h: interface for the CN3SMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3SMESH_H__41504828_1D08_4790_87D0_54026F972419__INCLUDED_)
#define AFX_N3SMESH_H__41504828_1D08_4790_87D0_54026F972419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

// Stream Mesh 흐르는 강물등을 나타낸다.

class CN3SMesh : public CN3Base  
{
public:
	CN3SMesh();
	virtual ~CN3SMesh();

// Attributes
public:
protected:
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;

// Operations
public:
	virtual void	Release();
	virtual int		Load(HANDLE hFile);
	virtual int		Save(HANDLE hFile);
	void			Tick();
	HRESULT			Render();
	
protected:

};

#endif // !defined(AFX_N3SMESH_H__41504828_1D08_4790_87D0_54026F972419__INCLUDED_)
