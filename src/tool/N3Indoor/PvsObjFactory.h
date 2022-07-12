#if !defined(AFX_PVSOBJFACTORY_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_)
#define AFX_PVSOBJFACTORY_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <string>
#include "PvsBase.h"

class CPortalVol;


class CPvsObjFactory
{
	HICON		GetPvsWallIcon();
	std::string	MakePvsWallString(int iIndex, e_WallType eWT);
	HICON		GetPvsVolIcon();
	std::string	MakePvsVolString(int iIndex);	

public:
	CPvsObjFactory();
	virtual ~CPvsObjFactory();

	CPvsBase*	CreatePvsWall(int iIndex, e_WallType eWT, CPortalVol* pVol = NULL);
	CPvsBase*	CreatePvsVol(int iIndex);
	void		DestroyPvsObj(CPvsBase* pBase);
};


#endif // !defined(AFX_PVSOBJFACTORY_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_)
