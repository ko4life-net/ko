// N3TransformCollision.h: interface for the CN3TransformCollision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3TRANSFORMCOLLISION_H__81088A50_9039_45F5_82D7_B0FF14C161F2__INCLUDED_)
#define AFX_N3TRANSFORMCOLLISION_H__81088A50_9039_45F5_82D7_B0FF14C161F2__INCLUDED_

#include "N3VectorAndMatrix.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"
#include "N3VMesh.h"

class CN3TransformCollision : public CN3Transform  
{
protected:
	float		m_fRadius; // 반지름..
	__Vector3	m_vMin, m_vMax; // 최대 최소점..
	CN3VMesh*	m_pMeshCollision; // 충돌 체크용 메시..
	CN3VMesh*	m_pMeshClimb; // 기어 올라가는 충돌 체크용 메시..

public:
	void FindMinMax();
	__Vector3 Min() { return m_vMin; }
	__Vector3 Max() { return m_vMax; }
	float Radius() { return m_fRadius; }
	
	void RenderCollisionMesh();
	void RenderClimbMesh();

//	By : Dino ( On 2001-08-27 오후 9:15:24 )
//	주어진 지점이 m_fRadius 범위안에 있는지 체크
	BOOL IsInRadius(__Vector3& vCheckPos)	{return ((vCheckPos - m_vPos).Magnitude() > m_fRadius ? FALSE : TRUE);}
	BOOL IsInRadiusXZ(float fX, float fZ) {fX -= m_vPos.x; fZ -= m_vPos.z; return (sqrtf(fX*fX + fZ*fZ) > m_fRadius ? FALSE : TRUE);}
//	~(By Dino On 2001-08-27 오후 9:15:24 )

	BOOL CheckCollision(int x, int y, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
	BOOL CheckCollision(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
	BOOL CheckClimb(int x, int y, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);
	BOOL CheckClimb(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL, __Vector3* pVPolygon = NULL);

	CN3VMesh* CollisionMesh() { return m_pMeshCollision; }
	CN3VMesh* ClimbMesh() { return m_pMeshClimb; }
	void CollisionMeshSet(const char* szFN);
	void ClimbMeshSet(const char* szFN);

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	void Release();
	CN3TransformCollision();
	virtual ~CN3TransformCollision();
};

#endif // !defined(AFX_N3TRANSFORMCOLLISION_H__81088A50_9039_45F5_82D7_B0FF14C161F2__INCLUDED_)
