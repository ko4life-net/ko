// PvsBase.cpp: implementation of the CPvsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3indoor.h"

#include "PvsBase.h"
#include "PortalWall.h"
#include "PortalVol.h"

#include "PvsObjFactory.h"

#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPvsObjFactory::CPvsObjFactory() {}

CPvsObjFactory::~CPvsObjFactory() {}

//.......
CPvsBase * CPvsObjFactory::CreatePvsWall(int iIndex, e_WallType eWT, CPortalVol * pVol) {
    CPvsBase * pRet = NULL;
    pRet = new CPortalWall(eWT, pVol);

#ifdef _N3TOOL
    pRet->m_strID = MakePvsWallString(iIndex, eWT);
#endif
    pRet->m_iID = iIndex;

    if (!pRet->CreatePvsObject()) {
        return NULL;
    }

    return pRet;
}

CPvsBase * CPvsObjFactory::CreatePvsVol(int iIndex) {
    CPvsBase * pRet = NULL;
    pRet = new CPortalVol;

#ifdef _N3TOOL
    pRet->m_strID = MakePvsVolString(iIndex);
#endif
    pRet->m_iID = iIndex;

    if (!pRet->CreatePvsObject()) {
        return NULL;
    }

    return pRet;
}

void CPvsObjFactory::DestroyPvsObj(CPvsBase * pBase) {}

//..
HICON CPvsObjFactory::GetPvsWallIcon() {
    return AfxGetApp()->LoadIcon(IDI_PORTAL);
}

std::string CPvsObjFactory::MakePvsWallString(int iIndex, e_WallType eWT) {
    int iWT = (int)eWT;
    iWT++;
    char buffer[32]{};
    sprintf(buffer, "Wall_%d_%d", iIndex, iWT);
    return std::string(buffer);
}

HICON CPvsObjFactory::GetPvsVolIcon() {
    return AfxGetApp()->LoadIcon(IDI_PVOLUMN);
}

std::string CPvsObjFactory::MakePvsVolString(int iIndex) {
    char buffer[32]{};
    sprintf(buffer, "Vol_%d", iIndex);
    return std::string(buffer);
}
