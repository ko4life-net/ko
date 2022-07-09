// N3GlobalEffect.cpp: implementation of the CN3GlobalEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "N3GlobalEffect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3GlobalEffect::CN3GlobalEffect()
{
	m_pVB = NULL;
	m_pIB = NULL;
}

CN3GlobalEffect::~CN3GlobalEffect()
{
	Release();
}

void CN3GlobalEffect::Release()
{
	if (m_pVB) { m_pVB->Release(); m_pVB = NULL;}
	if (m_pIB) { m_pIB->Release(); m_pIB = NULL;}
	m_bActive = FALSE;
	m_iVC = 0;
	m_iIC = 0;

	CN3Transform::Release();
}

void CN3GlobalEffect::Tick()
{
	CN3Transform::Tick(-1);
}

void CN3GlobalEffect::Render(__Vector3& vPos)
{
	PosSet(vPos);
}

