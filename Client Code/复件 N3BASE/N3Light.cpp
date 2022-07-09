// N3Light.cpp: implementation of the CN3Light class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Light::CN3Light()
{
	m_dwType |= OBJ_LIGHT;
	this->Release();
}

CN3Light::~CN3Light()
{
	CN3Light::Release();
}

void CN3Light::Release()
{
	memset(&m_Data, 0, sizeof(m_Data));

	CN3Transform::Release();
}

bool CN3Light::Load(HANDLE hFile)
{
	CN3Transform::Load(hFile);

	DWORD dwRWC;
	ReadFile(hFile, &m_Data, sizeof(m_Data), &dwRWC, NULL); // 라이트 세팅.

	__ASSERT(m_Data.nNumber >= 0 && m_Data.nNumber < 8, "Light Loading Warning - Light 번호가 범위를 벗어났습니다.");
	
	return true;
}

bool CN3Light::Save(HANDLE hFile)
{
	CN3Transform::Save(hFile);

	DWORD dwRWC;
	WriteFile(hFile, &m_Data, sizeof(m_Data), &dwRWC, NULL); // 라이트 세팅.

	return true;
}

void CN3Light::Tick(float fFrm)
{
	CN3Transform::Tick(fFrm);

	m_Data.Position = m_vPos;
}
