// WallMgr.h: interface for the CWallMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3BaseFileAccess.h"
#include "N3Base/N3ShapeMgr.h"
#include <list>

class CWall;
class CMapMng;
class CDlgMakeWall;

class CWallMgr : public CN3BaseFileAccess {
  protected:
    __Vector3        m_BaseCube[8];
    __VertexXyzColor m_CubeVB[36];
    __VertexXyzColor m_LineVB[2];
    __VertexXyzColor m_BoardVB[4];

  public:
    CMapMng *          m_pRefMapMng; // Terrain reference pointer..
    std::list<CWall *> m_pWalls;     // Walls
    CDlgMakeWall *     m_pDlg;       // path make dialog..
    bool               m_bActive;    // Is this function activated? 1: Enabled, 0: Disabled..
    CWall * m_pCurrWall; // The wall that is currently being built.. or the buffer that is being prepared to be built..

  protected:
    void MakeLine(__Vector3 sv, __Vector3 ev, D3DCOLOR color);
    void MakeCube(__Vector3 cv, D3DCOLOR color);
    void MakeBoard(__Vector3 sv, __Vector3 ev, D3DCOLOR color);

  public:
    void    AddWall2Coll(CN3ShapeMgr * pShapeMgr);
    void    SetFocus(CWall * pWall);
    void    SetCurrWall(CWall * pWall);
    void    DelWall(CWall * pWall);
    int     GetSize() { return m_pWalls.size(); }
    CWall * GetpWall(int idx);

    void Render();
    void UpdateWall();
    void SetActive(bool active);
    BOOL MouseMsgFilter(LPMSG pMsg);

    virtual bool Load(HANDLE hFile);
    virtual bool Save(HANDLE hFile);

    CWallMgr();
    virtual ~CWallMgr();
};
