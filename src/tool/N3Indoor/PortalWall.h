// PortalWall.h: interface for the CPortalWall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "PvsBase.h"
#include "N3Base/My_3DStruct.h"

class CPvsObjFactory;
class CPvsMgr;
class CPortalVol;

class CPortalWall : public CPvsBase {
    friend class CPvsObjFactory;
    friend class CPvsMgr;

    DECLARE_DYNAMIC(CPortalWall)

    e_WallType     m_eWallType;
    __VertexColor  m_pvVertex[4];
    unsigned short m_pIndex[6];

    void CopyToData(CPortalWall * pLinked, CPortalVol * pVol);

    CPortalWall(e_WallType eWT, CPortalVol * pVol);
    virtual ~CPortalWall();

    bool CreatePvsObject();
    bool DeletePvsObject();

    void SetState(e_PvsState ePS);

    void Load(FILE * stream);
    void Save(FILE * stream);

    void Translate();

    void TickEdit();
    void TickCompile();
    void TickExecute();

    void RenderEdit();
    void RenderCompile();
    void RenderExecute();
};
