// TransDummy.h: interface for the CTransDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Transform.h"
#include <afxtempl.h>

enum {
    DUMMY_CENTER = 0,
    DUMMY_X = 1,
    DUMMY_Y = 2,
    DUMMY_Z = 3,
    NUM_DUMMY = 4,
    DUMMY_NO = 5
};
const int NUM_CUBEVERTEX = 36;

struct __DUMMYCUBE {
    int                    iType;                    // Classification of Center, X, Y, Z types.
    __VertexXyzNormalColor Vertices[NUM_CUBEVERTEX]; // Constituting points of the cube
    __Vector3              vCenterPos;               // center point of cube
    float                  fDistance;                // Distance from camera
};

class CLyTerrain;

class CTransDummy : public CN3Transform {
  public:
    CLyTerrain * m_pTerrainRef;

    CTransDummy();
    virtual ~CTransDummy();

    // Attributes
  public:
  protected:
    __DUMMYCUBE                               m_DummyCubes[NUM_DUMMY]; // 4 cubes (center, x, y, z);
    __VertexXyzColor                          m_LineVertices[6]; // Points that make up the line connecting the 4 cubes
    CTypedPtrArray<CPtrArray, CN3Transform *> m_SelObjArray;     // Objects selected in MapMng
    __DUMMYCUBE *                             m_pSortedCubes[NUM_DUMMY]; // Pointers sorted according to Cube distance
    __DUMMYCUBE *                             m_pSelectedCube;           // selected cube
    __Vector3                                 m_vPrevPos; // Position rotation zoom value before this mouse drag
    __Quaternion                              m_qPrevRot; // Position rotation zoom value before this mouse drag
    __Vector3 *
        m_vPrevScaleArray; // Zoom values before this mouse drag (array because it must contain the values of all selected objects)

    // Operations
  public:
    virtual void Tick();
    virtual void Render();
    virtual void Release();
    virtual void SetSelObj(CN3Transform * pObj); // change selected object
    virtual void AddSelObj(CN3Transform * pObj); // add selected object
    virtual BOOL MouseMsgFilter(LPMSG pMsg);     // Process mouse messages
    void         GetPickRay(POINT point, __Vector3 & vDir,
                            __Vector3 & vOrig); // Function to get a line extending in the direction the mouse pointer points
  protected:
    __DUMMYCUBE * Pick(int x, int y);                                                             // cube picking
    void InitDummyCube(int iType, __DUMMYCUBE * pDummyCube, __Vector3 & vOffset, D3DCOLOR color); // initialize cube
    static int   SortCube(const void * pArg1, const void * pArg2); // Sorting function for cube camera distance
    virtual void TransDiff(__Vector3 * pvDiffPos, __Quaternion * pvDiffRot,
                           __Vector3 * pvDiffScale); // Transform the selected objects by the difference.
};
