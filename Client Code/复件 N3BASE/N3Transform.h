// N3Transform.h: interface for the CN3Transform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Transform_h__INCLUDED_)
#define AFX_N3Transform_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3AnimKey.h"

const float FRAME_SELFPLAY = -10000.0f;

class CN3Transform : public CN3Base
{
protected:
	__Vector3 m_vPos;
	__Quaternion m_qRot;
	__Vector3 m_vScale;


public:
	__Matrix44	m_Matrix; // 변환 행렬

	CN3AnimKey m_KeyPos; // 에니메이션 키
	CN3AnimKey m_KeyRot;
	CN3AnimKey m_KeyScale;

	float m_fFrmWhole; // 전체 프레임수
	float m_fFrmCur; // 현재 프레임

public:
	virtual void Render(const __Matrix44* pMtxParent = NULL, float fUnitSize = 1.0f);
	virtual void Tick(float fFrm = FRAME_SELFPLAY);
	bool TickAnimationKey(float fFrm); // Animation Key Tick... Animation Key 가 있어 움직이면 true, 아니면 false 를 return;
	bool Save(HANDLE hFile);
	bool Load(HANDLE hFile);
	void Release();

	__Vector3 Pos() { return m_vPos; }
	__Quaternion Rot() { return m_qRot; }
	__Vector3 Scale() { return m_vScale; }

	void PosSet(__Vector3& v) { m_vPos = v; ReCalcMatrix(); }
	void PosSet(float x, float y, float z) { m_vPos.Set(x, y, z); ReCalcMatrix(); }
	void RotSet(__Quaternion& q) { m_qRot = q; ReCalcMatrix(); }
	void RotSet(float x, float y, float z, float w) { m_qRot.x = x, m_qRot.y = y, m_qRot.z = z, m_qRot.w = w; ReCalcMatrix(); }
	void ScaleSet(__Vector3& v) { m_vScale = v; ReCalcMatrix(); }
	void ScaleSet(float x, float y, float z) { m_vScale.Set(x, y, z); ReCalcMatrix(); }
	virtual void ReCalcMatrix();

	CN3Transform();
	virtual ~CN3Transform();
};

#endif // !defined(AFX_N3Transform_h__INCLUDED_)
