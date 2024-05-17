// RiverMng.h: interface for the CRiverMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3BaseFileAccess.h"
#include "VtxPosDummy.h"
#include <afxtempl.h>

enum {
    RCM_NONE = 0,
    RCM_SELECT = 1,
    RCM_CREATE = 2
};

class CRiverMesh;
class CMainFrame;
class CDlgRiverProperty;

#include <list>
typedef typename std::list<CRiverMesh *>::iterator it_RiverMesh;

class CRiverMng : public CN3BaseFileAccess {
  public:
    CRiverMng(CMainFrame * pMainFrm);
    virtual ~CRiverMng();

    class __VertexRiver {
      public:
        float    x, y, z;
        float    nx, ny, nz;
        D3DCOLOR color;
        float    u, v, u2, v2;
        void     Set(float sx, float sy, float sz, float snx, float sny, float snz, D3DCOLOR scolor, float su, float sv,
                     float su2, float sv2) {
            x = sx, y = sy, z = sz;
            nx = snx, ny = sny, nz = snz;
            color = scolor;
            u = su, v = sv;
            u2 = su2, v2 = sv2;
        }
    };

    // Attributes
  public:
    CRiverMesh * GetSelRiver() const { return m_pSelRiver; }
    int          GetRCursorMode() const { return m_RCursorMode; }
    int          GetDrawRiverNum() { return m_RiverMeshes.size(); }
    it_RiverMesh GetDrawRiver();

  protected:
    std::list<CRiverMesh *>                    m_RiverMeshes; // River Mesh list
    CRiverMesh *                               m_pSelRiver;   // Selected RiverMesh
    CTypedPtrArray<CPtrArray, __VertexXyzT2 *> m_SelVtxArray; // selected points
    CVtxPosDummy                               m_VtxPosDummy; // dummy object that can move the point
    BOOL                                       m_bEditMode;   // Is this river editing mode?
    int                                        m_RCursorMode; // Adding a river or selecting a point?

    CMainFrame *        m_pMainFrm;     // mainframe pointer
    CDlgRiverProperty * m_pDlgProperty; // property dialog

    RECT             m_rcSelDrag;     // drag area
    __VertexXyzColor m_CreateLine[2]; // Line visible when the river is first created

    // Operations
  public:
    void         Tick();
    void         Render();
    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual bool Save(HANDLE hFile);

    void         MakeGameFiles(HANDLE hFile, float fSize); // Save as game file
    CRiverMesh * CreateNewRiverMesh(__Vector3 & vPos1, __Vector3 & vPos2, __Vector3 & vPos3,
                                    __Vector3 & vPos4);             // Add a new river.
    void         RemoveRiverMesh(int iRiverID);                     // Delete the selected river.
    BOOL         MouseMsgFilter(LPMSG pMsg);                        // Mouse message processing
    BOOL         SetRiverID(CRiverMesh * pRiverMesh, int iRiverID); // Set course ID
    void         SetEditMode(BOOL bEditMode);                       // Strong EditMode settings
    void         ExtrudeRiverEdge();
    CRiverMesh * GetRiverMesh(int iRiverID); // Get RiverMesh
    void         DeleteSelectedVertex();     // Clear selected points
    void         ReCalcUV();                 // Recalculate the UV coordinates of the selected river.
    void         ReCalcSelectedVertex();

    void GoRiver(int iRiverID); // Go to the selected river.
  protected:
    void SetSelRiver(CRiverMesh * pRiverMesh); // Select RiverMesh
    void SelectVtxByDragRect(RECT * pRect, BOOL bAdd);
};
