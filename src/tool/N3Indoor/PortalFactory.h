// PortalFactory.h: interface for the CPortalFactory class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include <string>

class CPortalVolume;

class CPortalFactory {
    std::string MakePvsVolString(int iIndex);

  public:
    CPortalFactory();
    virtual ~CPortalFactory();

    CPortalVolume * CreatePvsVol(int iIndex);
};
