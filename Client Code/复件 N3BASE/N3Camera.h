// N3Camera.h: interface for the CN3Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Camera_h__INCLUDED_)
#define AFX_N3Camera_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"

class CN3Camera : public CN3Transform
{
public:
	__Vector3	m_vAt;
	__CameraData	m_Data;
	BOOL		m_bFogUse;
	float		m_fFogDensity;
	float		m_fFogStart;
	float		m_fFogEnd;
	D3DCOLOR	m_FogColor;

public:
	BOOL MoveByWindowMessage(MSG* pMsg);
	void Move(__Vector3& vDelta);
	void MovePlane(float fX, float fY);
	void MoveStraight(float fDistance);
	void LookAround(float fRadianX, float fRadianY);
	void Rotate(float fRadianX, float fRadianY);
	void Zoom(float fDelta);

	__Vector3 EyePos() { return m_vPos; }
	__Vector3 AtPos() { return m_vAt; }
	__Vector3 UpVector() { return m_vScale; } // Rotation 은 LookAt Position 처럼, Scale 은 UpVector 처럼 쓴다..
	__Vector3 Dir() {__Vector3 vDir = m_vAt - m_vPos; vDir.Normalize(); return vDir;} // 방향을 되돌린다.

	void EyePosSet(__Vector3& v) { m_vPos = v; }
	void EyePosSet(float x, float y, float z) { m_vPos.Set(x, y, z); }
	void AtPosSet(__Vector3& v) { m_vAt = v; } 
	void AtPosSet(float x, float y, float z) { m_vAt.Set(x, y, z); }
	void UpVectorSet(__Vector3& v) { m_vScale = v; } // Scale 은 UpVector 처럼 쓴다..
	void UpVectorSet(float x, float y, float z) { m_vScale.Set(x, y, z); } // Scale 은 UpVector 처럼 쓴다..

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	void Release();

	CN3Camera();
	virtual ~CN3Camera();

	void Tick(float fFrm = FRAME_SELFPLAY);
	void Render(float fUnitSize = 1.0f);
	void Apply();
	float			m_fRadianX;

private:
	__Vector3		m_v1, m_v2;
	__Quaternion	m_qt;
};

#endif // !defined(AFX_N3Camera_h__INCLUDED_)