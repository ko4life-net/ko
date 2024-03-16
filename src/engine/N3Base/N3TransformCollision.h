// N3TransformCollision.h: interface for the CN3TransformCollision class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Transform.h"
#include "N3VMesh.h"

class CN3TransformCollision : public CN3Transform {
  protected:
    float      m_fRadius;        // radius..
    __Vector3  m_vMin, m_vMax;   // Maximum and minimum points..
    CN3VMesh * m_pMeshCollision; // Mesh for collision check...
    CN3VMesh * m_pMeshClimb;     // Climbing collision check mesh...

  public:
    virtual void FindMinMax();
    __Vector3    Min() { return m_vMin * m_Matrix; } // minimum value in world
    __Vector3    Max() { return m_vMax * m_Matrix; } // maximum value in the world
    __Vector3    RawMin() { return m_vMin; }         // minimum value in world
    __Vector3    RawMax() { return m_vMax; }         // maximum value in the world
    float        Radius() { return m_fRadius * m_vScale.y; }

    void SetRadius(float fRadius) { m_fRadius = fRadius; }
    void SetMin(__Vector3 vMin) { m_vMin = vMin; }
    void SetMax(__Vector3 vMax) { m_vMin = vMax; }
    void SetMeshCollision(const std::string & szFN) { m_pMeshCollision = s_MngVMesh.Get(szFN); }
    void SetMeshClimb(const std::string & szFN) { m_pMeshClimb = s_MngVMesh.Get(szFN); }

    // By: Dino (On 2001-08-27 9:15:24 PM)
    // Check whether the given point is within the range of m_fRadius
    bool IsInRadius(__Vector3 & vCheckPos) { return ((vCheckPos - m_vPos).Magnitude() > m_fRadius ? FALSE : TRUE); }
    bool IsInRadiusXZ(float fX, float fZ) {
        fX -= m_vPos.x;
        fZ -= m_vPos.z;
        return (sqrtf(fX * fX + fZ * fZ) > m_fRadius ? FALSE : TRUE);
    }
    //   ~(By Dino On 2001-08-27 9:15:24 PM)

    virtual int CheckCollisionPrecisely(bool bIgnoreBoxCheck, int ixScreen, int iyScreen, __Vector3 * pVCol = NULL,
                                        __Vector3 * pVNormal = NULL);
    //    BOOL CheckClimb(int x, int y, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
    //    BOOL CheckClimb(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
    void RenderCollisionMesh();
    void RenderClimbMesh();

    CN3VMesh * CollisionMesh() { return m_pMeshCollision; }
    CN3VMesh * ClimbMesh() { return m_pMeshClimb; }
    void       CollisionMeshSet(const std::string & szFN);
    void       ClimbMeshSet(const std::string & szFN);

    bool Load(HANDLE hFile);
#ifdef _N3TOOL
    bool Save(HANDLE hFile);
#endif // end of _N3TOOL

    void Release();
    CN3TransformCollision();
    virtual ~CN3TransformCollision();
};
