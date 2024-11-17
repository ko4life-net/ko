// N3TransformCollision.h: interface for the CN3TransformCollision class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Transform.h"
#include "N3VMesh.h"

class CN3TransformCollision : public CN3Transform {
  protected:
    float      m_fRadius;        // 반지름..
    __Vector3  m_vMin, m_vMax;   // 최대 최소점..
    CN3VMesh * m_pMeshCollision; // 충돌 체크용 메시..
    CN3VMesh * m_pMeshClimb;     // 기어 올라가는 충돌 체크용 메시..

  public:
    virtual void FindMinMax();
    __Vector3    Min() { return m_vMin * m_Matrix; } // 월드 상의 최소값
    __Vector3    Max() { return m_vMax * m_Matrix; } // 월드 상의 최대값
    __Vector3    RawMin() { return m_vMin; }         // 월드 상의 최소값
    __Vector3    RawMax() { return m_vMax; }         // 월드 상의 최대값
    float        Radius() { return m_fRadius * m_vScale.y; }

    void SetRadius(float fRadius) { m_fRadius = fRadius; }
    void SetMin(__Vector3 vMin) { m_vMin = vMin; }
    void SetMax(__Vector3 vMax) { m_vMin = vMax; }
    void SetMeshCollision(const fs::path & fsFile) { m_pMeshCollision = s_MngVMesh.Get(fsFile); }
    void SetMeshClimb(const fs::path & fsFile) { m_pMeshClimb = s_MngVMesh.Get(fsFile); }

    //    By : Dino ( On 2001-08-27 오후 9:15:24 )
    //    주어진 지점이 m_fRadius 범위안에 있는지 체크
    bool IsInRadius(__Vector3 & vCheckPos) { return ((vCheckPos - m_vPos).Magnitude() > m_fRadius ? FALSE : TRUE); }
    bool IsInRadiusXZ(float fX, float fZ) {
        fX -= m_vPos.x;
        fZ -= m_vPos.z;
        return (sqrtf(fX * fX + fZ * fZ) > m_fRadius ? FALSE : TRUE);
    }
    //    ~(By Dino On 2001-08-27 오후 9:15:24 )

    virtual int CheckCollisionPrecisely(bool bIgnoreBoxCheck, int ixScreen, int iyScreen, __Vector3 * pVCol = NULL,
                                        __Vector3 * pVNormal = NULL);
    //    BOOL CheckClimb(int x, int y, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
    //    BOOL CheckClimb(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
    void RenderCollisionMesh();
    void RenderClimbMesh();

    CN3VMesh * CollisionMesh() { return m_pMeshCollision; }
    CN3VMesh * ClimbMesh() { return m_pMeshClimb; }
    void       CollisionMeshSet(const fs::path & fsFile);
    void       ClimbMeshSet(const fs::path & fsFile);

    bool Load(HANDLE hFile);
#ifdef _N3TOOL
    bool Save(HANDLE hFile);
#endif // end of _N3TOOL

    void Release();
    CN3TransformCollision();
    virtual ~CN3TransformCollision();
};
