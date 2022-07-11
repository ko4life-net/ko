// PvsBase.cpp: implementation of the CPvsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "n3indoor.h"

#include "PvsBase.h"
#include "PortalWall.h"
#include "PortalVol.h"

#include "PvsObjFactory.h"

#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPvsObjFactory::CPvsObjFactory()
{
}

CPvsObjFactory::~CPvsObjFactory()
{

}

//.......
CPvsBase* CPvsObjFactory::CreatePvsWall(int iIndex, e_WallType eWT, CPortalVol* pVol)
{
	CPvsBase* pRet = NULL;
	pRet = new CPortalWall(eWT, pVol);

#ifdef _N3TOOL
	pRet->m_strID = MakePvsWallString(iIndex, eWT);
#endif
	pRet->m_iID	  = iIndex;	 

	if (!pRet->CreatePvsObject())
		return NULL;

	return pRet;
}

CPvsBase* CPvsObjFactory::CreatePvsVol(int iIndex)
{
	CPvsBase* pRet = NULL;
	pRet = new CPortalVol;

#ifdef _N3TOOL
	pRet->m_strID = MakePvsVolString(iIndex);
#endif
	pRet->m_iID	  = iIndex;	 

	if (!pRet->CreatePvsObject())
		return NULL;

	return pRet;
}

void CPvsObjFactory::DestroyPvsObj(CPvsBase* pBase)
{


}

//..
HICON CPvsObjFactory::GetPvsWallIcon()
{
	return AfxGetApp()->LoadIcon(IDI_PORTAL);
}

std::string	CPvsObjFactory::MakePvsWallString(int iIndex, e_WallType eWT)
{
	int iWT = (int)eWT; iWT++;
	std::string strRet;
	std::vector<char> buffer(32, NULL);

	sprintf(buffer.begin(), "Wall_%d_%d", iIndex, iWT);
	strRet = buffer.begin();
	return strRet;
}

HICON CPvsObjFactory::GetPvsVolIcon()
{
	return AfxGetApp()->LoadIcon(IDI_PVOLUMN);
}

std::string	CPvsObjFactory::MakePvsVolString(int iIndex)
{
	std::string strRet;
	std::vector<char> buffer(32, NULL);

	sprintf(buffer.begin(), "Vol_%d", iIndex);
	strRet = buffer.begin();
	return strRet;
}







