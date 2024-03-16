// N3Shape.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3TransformCollision.h"
#include "N3PMeshInstance.h"
#include "N3Texture.h"

#include <vector>

typedef std::vector<CN3Texture *> it_pTex;

class CN3SPart : public CN3BaseFileAccess {
    friend class CN3Shape;

  public:
    __Vector3  m_vPivot;            // Local axis
    __Matrix44 m_Matrix;            // World Matrix.. It is best to calculate it in advance when Shape Loading.
    BOOL       m_bOutOfCameraRange; // Outside Camera range...

    __Material m_Mtl;     // Material
    float      m_fTexFPS; // Texture Animation Interval;

    //    __Vector3    m_vWindFactorCur;        // Current wind blowing value.. Rotate with this value..
    //    __Vector3    m_vWindFactorToReach;    // Windy value...
    float m_fTimeToSetWind;     // Time to change the wind blowing value...
    float m_fWindFactorToReach; // Current wind blowing value.. Rotate with this value..
    float m_fWindFactorCur;     // Windy value...

  protected:
    std::vector<CN3Texture *> m_TexRefs; // Texture Reference Pointers
    CN3PMeshInstance          m_PMInst;  // Progressive Mesh Instance

    float
        m_fTexIndex; // Current Texture Index. This is the index required for animation. Texture animation is controlled using float.

  public:
    virtual bool Load(HANDLE hFile);
#ifdef _N3TOOL
    virtual bool Save(HANDLE hFile);
#endif // end of _N3TOOL

    int          TexCount() { return m_TexRefs.size(); }
    CN3Texture * Tex(int iIndex) {
        if (iIndex < 0 || iIndex >= m_TexRefs.size()) {
            return NULL;
        }
        return m_TexRefs[iIndex];
    }
    void         TexAlloc(int m_nCount);
    CN3Texture * TexSet(int iIndex, const std::string & szFN) {
        if (iIndex < 0 || iIndex >= m_TexRefs.size()) {
            return NULL;
        }
        s_MngTex.Delete(&m_TexRefs[iIndex]);
        m_TexRefs[iIndex] = s_MngTex.Get(szFN, true, s_Options.iTexLOD_Shape);
        return m_TexRefs[iIndex];
    }
    void TexSet(int iIndex, CN3Texture * pTex) {
        if (iIndex < 0 || iIndex >= m_TexRefs.size()) {
            return;
        }
        s_MngTex.Delete(&m_TexRefs[iIndex]);
        m_TexRefs[iIndex] = pTex;
    }

    CN3PMeshInstance * MeshInstance() { return &m_PMInst; }
    CN3PMesh *         Mesh() { return m_PMInst.GetMesh(); }
    void               MeshSet(const std::string & szFN);

    void ReCalcMatrix(const __Matrix44 & mtxParent) {
        m_Matrix.Identity();
        m_Matrix.PosSet(m_vPivot);
        m_Matrix *= mtxParent;
    }

    void Tick(const __Matrix44 & mtxParent, const __Quaternion & qRot,
              float fScale); // Enter the parent matrix, that is, the Shape matrix and the rotation quaternion.
    void Render();
#ifdef _N3TOOL
    void RenderSelected(bool bWireFrame);
    void RenderAxis();
#endif // end of _N3TOOL

    __Vector3 Min() {
        if (m_PMInst.GetMesh()) {
            return m_PMInst.GetMesh()->Min() * m_Matrix;
        } else {
            return __Vector3(0, 0, 0);
        }
    } // minimum value in world
    __Vector3 Max() {
        if (m_PMInst.GetMesh()) {
            return m_PMInst.GetMesh()->Max() * m_Matrix;
        } else {
            return __Vector3(0, 0, 0);
        }
    } // maximum value in the world
    float Radius() {
        if (m_PMInst.GetMesh()) {
            return m_PMInst.GetMesh()->Radius();
        } else {
            return 0.0f;
        }
    }

    virtual void Release();

    CN3SPart();
    virtual ~CN3SPart();

#ifdef _USE_VERTEXBUFFER
    void PartialRender(int iCount, LPDIRECT3DINDEXBUFFER9 pIB);
#else
    void PartialRender(int iCount, WORD * pIndices);
#endif
};

typedef typename std::vector<CN3SPart *>::iterator it_SPart;

class CN3Shape : public CN3TransformCollision {
  public:
    int m_iBelong;    // Affiliation - 0:No affiliation 1:Elmorad 2:Carus 3:?? ....
    int m_iEventID;    //Event ID
    int m_iEventType;  // Event Type
    int m_iNPC_ID;     // NPC ID if the object is used as an NPC
    int m_iNPC_Status; // Default Status for objects used as NPCs

    bool m_bDontRender; // If this flag is set according to the camera distance, it will not be rendered.
    bool m_bVisible;    //..

    std::vector<CN3SPart *> m_Parts; // Part Data Pointer Linked List

  public:
#ifdef _N3TOOL
    bool SaveToSameFolderAndMore(const std::string & szFullPath, const std::string & szRelativePath);
    bool SaveToSameFolder(const std::string & szFullPath);
    void RemoveRenderFlags(int nFlags = -1);
    void MakeDefaultMaterial();
#endif // end of _N3TOOL
    int CheckCollisionPrecisely(
        bool bIgnoreBoxCheck, int ixScreen, int iyScreen, __Vector3 * pVCol = NULL,
        __Vector3 * pVNormal = NULL); // Precisely check by polygon - first check the box and then check again precisely...
    int CheckCollisionPrecisely(
        bool bIgnoreBoxCheck, const __Vector3 & vPos, const __Vector3 & vDir, __Vector3 * pVCol = NULL,
        __Vector3 * pVNormal = NULL);      // Precisely check by polygon - first check the box and then check again precisely...
    bool MakeCollisionMeshByParts();       // Recreate the collision mesh as a box...
    bool MakeCollisionMeshByPartsDetail(); // As it is... create a collision mesh...

    void         FindMinMax();
    virtual void ReCalcMatrix();
    void         ReCalcPartMatrix();
    virtual void Tick(float fFrm = FRAME_SELFPLAY);
    virtual void Render();

    CN3SPart * Part(int iIndex) {
        if (iIndex < 0 || iIndex >= m_Parts.size()) {
            return NULL;
        }
        return m_Parts[iIndex];
    }
    CN3SPart * PartAdd() {
        CN3SPart * pPart = new CN3SPart();
        m_Parts.push_back(pPart);
        return pPart;
    }
    int  PartCount() { return m_Parts.size(); }
    void PartDelete(int iIndex);

    bool Load(HANDLE hFile);
#ifdef _N3TOOL
    bool Save(HANDLE hFile);
    void RenderSelected(bool bWireFrame);
    void RenderSelected(int iPart, bool bWireFrame);
    bool IsPMeshProcessed();
#endif // end of _N3TOOL

    virtual void Release();
    CN3Shape();
    virtual ~CN3Shape();

    //   By: Ecli666 (On 2002-08-06 4:33:04 PM)
    //
    void       SetMaxLOD();
    __Matrix44 GetPartMatrix(int iPartIndex);
#ifdef _USE_VERTEXBUFFER
    void PartialRender(int iPartIndex, int iCount, LPDIRECT3DINDEXBUFFER9 pIB);
#else
    void PartialRender(int iPartIndex, int iCount, WORD * pIndices);
#endif
    int       GetIndexbufferCount(int iPartIndex);
    int       GetIndexByiOrder(int iPartIndex, int iOrder);
    __Vector3 GetVertexByIndex(int iPartIndex, int iIndex);
    int       GetColIndexbufferCount();
    int       GetColIndexByiOrder(int iOrder);
    __Vector3 GetColVertexByIndex(int iIndex);
    void      PartialColRender(int iCount, int * piIndices);
    void      PartialGetCollision(int iIndex, __Vector3 & vec);
    bool      LoadTransformOnly(HANDLE hFile);
    //    ~(By Ecli666 On 2002-08-06 ¿ÀÈÄ 4:33:04 )
};
