// PortalFactory.h: interface for the CPortalFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTALFACTORY_H__8561BB9B_D293_49FC_832D_F94AF4B8BEF9__INCLUDED_)
#define AFX_PORTALFACTORY_H__8561BB9B_D293_49FC_832D_F94AF4B8BEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <string>

class CPortalVolume;

class CPortalFactory  
{
	std::string	MakePvsVolString(int iIndex);	

public:
	CPortalFactory();
	virtual ~CPortalFactory();

	CPortalVolume*	CreatePvsVol(int iIndex);
};

#endif // !defined(AFX_PORTALFACTORY_H__8561BB9B_D293_49FC_832D_F94AF4B8BEF9__INCLUDED_)
