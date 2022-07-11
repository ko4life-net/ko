// PortalFactory.cpp: implementation of the CPortalFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N3Indoor.h"
#include "PortalFactory.h"

#include "PortalVolume.h"

#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortalFactory::CPortalFactory()
{

}

CPortalFactory::~CPortalFactory()
{

}

std::string	CPortalFactory::MakePvsVolString(int iIndex)
{
	std::string strRet;
	std::vector<char> buffer(32, NULL);

	sprintf(buffer.begin(), "Vol_%d", iIndex);
	strRet = buffer.begin();
	return strRet;
}

CPortalVolume* CPortalFactory::CreatePvsVol(int iIndex)
{
	CPortalVolume* pRet = NULL;
	pRet = new CPortalVolume;

	pRet->m_strID = MakePvsVolString(iIndex);
	pRet->m_iID	   = iIndex;	 

	return pRet;
}

