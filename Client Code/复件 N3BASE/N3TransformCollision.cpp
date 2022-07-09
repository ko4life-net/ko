// N3TransformCollision.cpp: implementation of the CN3TransformCollision class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3TransformCollision.h"
#include "N3VMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3TransformCollision::CN3TransformCollision()
{
	m_dwType |= OBJ_TRANSFORM_COLLISION;

	m_pMeshCollision = NULL;
	m_pMeshClimb = NULL;
}

CN3TransformCollision::~CN3TransformCollision()
{
	CN3TransformCollision::Release();
}

void CN3TransformCollision::Release()
{
	m_fRadius = 0;
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);

	if(m_pMeshCollision)
	{
		s_MngVMesh.Delete(m_pMeshCollision);
		m_pMeshCollision = NULL;
	}
	if(m_pMeshClimb)
	{
		s_MngVMesh.Delete(m_pMeshClimb);
		m_pMeshClimb = NULL;
	}

	CN3Transform::Release();
}

bool CN3TransformCollision::Load(HANDLE hFile)
{
	CN3Transform::Load(hFile);

	int nL = 0;
	char szFN[512] = "";

	DWORD dwRWC;
	ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 메시 파일 이름..
		m_pMeshCollision = s_MngVMesh.Get(szFN);
	}

	ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // 메시 파일 이름..
		m_pMeshClimb = s_MngVMesh.Get(szFN);
	}
	return true;
}

bool CN3TransformCollision::Save(HANDLE hFile)
{
	CN3Transform::Save(hFile);

	DWORD dwRWC;

	int nL = 0;
	if(NULL != m_pMeshCollision) nL = lstrlen(m_pMeshCollision->FileName());

	WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0) 
	{
		WriteFile(hFile, m_pMeshCollision->FileName(), nL, &dwRWC, NULL);
	}

	nL = 0;
	if(NULL != m_pMeshClimb) nL = lstrlen(m_pMeshClimb->FileName());
	
	WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0) 
	{
		WriteFile(hFile, m_pMeshClimb->FileName(), nL, &dwRWC, NULL);
	}
	return true;
}

void CN3TransformCollision::CollisionMeshSet(const char* szFN)
{
	s_MngVMesh.Delete(m_pMeshCollision);
	
	m_pMeshCollision = s_MngVMesh.Get(szFN);
	if(m_pMeshCollision) this->FindMinMax();
}

void CN3TransformCollision::ClimbMeshSet(const char* szFN)
{
	s_MngVMesh.Delete(m_pMeshClimb);
	
	m_pMeshClimb = s_MngVMesh.Get(szFN);
}

BOOL CN3TransformCollision::CheckCollision(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshCollision) return FALSE;
	
	int nIC = m_pMeshCollision->IndexCount();
	int nFC = 0;
	if(nIC > 0)
	{
		nFC = nIC / 3; // Face Count
	}
	else
	{
		nFC = m_pMeshCollision->VertexCount() / 3;
	}
	if(nFC <= 0) return FALSE;

	static float t,u,v;
	static __Vector3 vSrc[3];

	if(nIC > 0)
	{
		WORD* pwIndices = m_pMeshCollision->Indices();
		__Vector3* pVSrc = m_pMeshCollision->Vertices();
		for(int i = 0; i < nFC; i++)
		{
			vSrc[0] = pVSrc[pwIndices[i*3+0]] * m_Matrix;
			vSrc[1] = pVSrc[pwIndices[i*3+1]] * m_Matrix;
			vSrc[2] = pVSrc[pwIndices[i*3+2]] * m_Matrix;

			if(true == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}
	else
	{
		__Vector3* pVSrc = m_pMeshCollision->Vertices();
		for(int i = 0; i < nFC; i++, pVSrc += 3)
		{
			vSrc[0] = pVSrc[0] * m_Matrix;
			vSrc[1] = pVSrc[1] * m_Matrix;
			vSrc[2] = pVSrc[2] * m_Matrix;

			if(true == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CN3TransformCollision::CheckCollision(int x, int y, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshCollision || NULL == m_pMeshCollision->Vertices()) return FALSE;

	// Compute the vector of the pick ray in screen space
	__Vector3 vTmp;
	vTmp.x =  ( ( ( 2.0f * x ) / (CN3Base::s_CameraData.vp.Width) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._11;
	vTmp.y = -( ( ( 2.0f * y ) / (CN3Base::s_CameraData.vp.Height) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._22;
	vTmp.z =  1.0f;

	// Transform the screen space pick ray into 3D space
	__Matrix44* pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
	__Vector3 vPos, vDir;
	vDir.x  = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
	vDir.y  = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
	vDir.z  = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
	vPos = pMtxVI->Pos();

	return CheckCollision(vPos, vDir, pVCol, pVNormal, pVPolygon);
}

void CN3TransformCollision::RenderCollisionMesh()
{
	if(NULL == m_pMeshCollision) return;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	m_pMeshCollision->Render(0xff00ff00);
}

void CN3TransformCollision::RenderClimbMesh()
{
	if(NULL == m_pMeshClimb) return;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	m_pMeshClimb->Render(0xff0000ff);
}

BOOL CN3TransformCollision::CheckClimb(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshClimb) return FALSE;
	
	int nIC = m_pMeshClimb->IndexCount();
	int nFC = 0;
	if(nIC > 0)
	{
		nFC = nIC / 3; // Face Count
	}
	else
	{
		nFC = m_pMeshClimb->VertexCount() / 3;
	}
	if(nFC <= 0) return FALSE;

	float t,u,v;
	__Vector3 vSrc[3];

	if(nIC > 0)
	{
		WORD* pwIndices = m_pMeshClimb->Indices();
		__Vector3* pVSrc = m_pMeshClimb->Vertices();
		for(int i = 0; i < nFC; i++)
		{
			vSrc[0] = pVSrc[pwIndices[i*3+0]] * m_Matrix;
			vSrc[1] = pVSrc[pwIndices[i*3+1]] * m_Matrix;
			vSrc[2] = pVSrc[pwIndices[i*3+2]] * m_Matrix;

			if(TRUE == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}
	else
	{
		__Vector3* pVSrc = m_pMeshClimb->Vertices();
		for(int i = 0; i < nFC; i++, pVSrc += 3)
		{
			vSrc[0] = pVSrc[0] * m_Matrix;
			vSrc[1] = pVSrc[1] * m_Matrix;
			vSrc[2] = pVSrc[2] * m_Matrix;

			if(TRUE == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CN3TransformCollision::CheckClimb(int x, int y, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshClimb || NULL == m_pMeshClimb->Vertices()) return FALSE;

	// Compute the vector of the pick ray in screen space
	__Vector3 vTmp;
	vTmp.x =  ( ( ( 2.0f * x ) / (CN3Base::s_CameraData.vp.Width) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._11;
	vTmp.y = -( ( ( 2.0f * y ) / (CN3Base::s_CameraData.vp.Height) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._22;
	vTmp.z =  1.0f;

	// Transform the screen space pick ray into 3D space
	__Matrix44* pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
	__Vector3 vPos, vDir;
	vDir.x  = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
	vDir.y  = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
	vDir.z  = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
	vPos = pMtxVI->Pos();

	return CheckClimb(vPos, vDir, pVCol, pVNormal, pVPolygon);
}

void CN3TransformCollision::FindMinMax()
{
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);
	m_fRadius = 0.0f;

	if(NULL == m_pMeshCollision || m_pMeshCollision->VertexCount() <= 0) return;

	__Vector3 vMax(-100000.0f, -100000.0f, -100000.0f);
	__Vector3 vMin(100000.0f, 100000.0f, 100000.0f);
	
	int nVC = m_pMeshCollision->VertexCount();
	__Vector3* pVs = m_pMeshCollision->Vertices();
	for(int i = 0; i < nVC; i++)
	{
		if(pVs[i].x < vMin.x) vMin.x = pVs[i].x;
		if(pVs[i].y < vMin.y) vMin.y = pVs[i].y;
		if(pVs[i].z < vMin.z) vMin.z = pVs[i].z;
		if(pVs[i].x > vMax.x) vMax.x = pVs[i].x;
		if(pVs[i].y > vMax.y) vMax.y = pVs[i].y;
		if(pVs[i].z > vMax.z) vMax.z = pVs[i].z;
	}

	// 최대 최소값을 저장
	m_vMin = vMin;
	m_vMax = vMax;

	// 최대 최소값을 갖고 반지름 계산한다..
	__Vector3 vRad;
	vMin.Absolute();
	vMax.Absolute();
	
	if(vMin.x > vMax.x) vRad.x = vMin.x; else vRad.x = vMax.x;
	if(vMin.y > vMax.y) vRad.y = vMin.y; else vRad.y = vMax.y;
	if(vMin.z > vMax.z) vRad.z = vMin.z; else vRad.z = vMax.z;
	
	m_fRadius = vRad.Magnitude();
}
