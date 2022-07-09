////////////////////////////////////////////////////////////////////
// __ 가 붙은 구조체는 상속을 받은 내가 만든 구조체이다. 이름을 구별하려고 이렇게 했다.
#ifndef __N3VECTORANDMATRIX_H_
#define __N3VECTORANDMATRIX_H_

#include "D3DX8.h"
#include "D3DX8Math.h"

const float __PI2 = 6.283185308f;

void InitTrigonometricFunction(); // 삼각 함수 테이블 초기화
float __Sine(float fRadian); // 테이블에 의한 삼각함수 값구하기..
float __Cosine(float fRadian); // 테이블에 의한 삼각함수 값구하기..
float __Tangent(float fRadian); // 테이블에 의한 삼각함수 값구하기..

struct __Vector2 : public D3DXVECTOR2 // 2D Vertex
{
public:
	void Zero();
	void Set(float fx, float fy);
	__Vector2() {};
	__Vector2(float fx, float fy);
};

struct __Matrix44;

struct __Vector3 : public D3DXVECTOR3 // 3D Vertex
{
public:
	void	Normalize();
	float	Magnitude();
	float	Dot(const D3DXVECTOR3& vec);
	void	Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
	void	Absolute();

	void Zero();
	void Set(float fx, float fy, float fz);

	__Vector3() {};
	__Vector3(float fx, float fy, float fz);
	__Vector3(const _D3DVECTOR& vec);
	__Vector3(const D3DXVECTOR3& vec);

	__Vector3 operator * (const D3DXMATRIX& mtx);
	void operator *= (float fDelta);
	void operator *= (const D3DXMATRIX& mtx);
	__Vector3 operator + (const D3DXVECTOR3& vec);
	__Vector3 operator - (const D3DXVECTOR3& vec);
	__Vector3 operator * (const D3DXVECTOR3& vec);
	__Vector3 operator / (const D3DXVECTOR3& vec);

	void operator += (const D3DXVECTOR3& vec);
	void operator -= (const D3DXVECTOR3& vec);
	void operator *= (const D3DXVECTOR3& vec);
	void operator /= (const D3DXVECTOR3& vec);

	__Vector3 operator + (float fDelta);
	__Vector3 operator - (float fDelta);
	__Vector3 operator * (float fDelta);
	__Vector3 operator / (float fDelta);
};

struct __Matrix44 : public D3DXMATRIX // 4x4 Matrix
{
public:
	void Zero();
	void Identity();
	__Vector3& Pos();
	void PosSet(float x, float y, float z);
	void PosSet(const D3DXVECTOR3& v);
	void RotationX(float fDelta);
	void RotationY(float fDelta);
	void RotationZ(float fDelta);
	void Rotation(float fX, float fY, float fZ);
	void Rotation(const D3DXVECTOR3& v);
	void Scale(float sx, float sy, float sz);
	void Scale(const D3DXVECTOR3& v);

	void LookAt(const __Vector3& vPos, const __Vector3& vAt, const __Vector3& vUp);

	__Matrix44 operator * (const D3DXMATRIX& mtx);
	void operator *= (const D3DXMATRIX& mtx);

	void operator = (const D3DXQUATERNION& qt);

	__Matrix44();
	__Matrix44(const _D3DMATRIX& mtx);
	__Matrix44(const D3DXMATRIX& mtx);
	__Matrix44(const D3DXQUATERNION& qt);
};

struct __Quaternion : public D3DXQUATERNION
{
public:
	void Identity();
	void Set(float fX, float fY, float fZ, float fW);

	void RotationAxis(const __Vector3& v, float fRadian);
	void RotationAxis(float fX, float fY, float fZ, float fRadian);
	void operator = (const D3DXMATRIX& mtx);

	void AxisAngle(__Vector3& vAxisResult, float& fRadianResult);
	void Slerp(const D3DXQUATERNION& qt1, const D3DXQUATERNION& qt2, float fDelta);

	__Quaternion();
	__Quaternion(const D3DXMATRIX& mtx);
};

#endif // __N3VECTORANDMATRIX_H_