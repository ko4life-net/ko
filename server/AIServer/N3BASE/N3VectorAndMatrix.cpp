#include "N3VectorAndMatrix.h"

extern float __sfSs[3600]; // sine
extern float __sfCs[3600]; // consine
extern float __sfTs[3600]; // tangent

inline void InitTrigonometricFunction()
{
	float fRadian;
	for(int i = 0; i < 3600; i++)
	{
		fRadian = ((i/10.0f)/180.0f) * 3.141592654f;
		__sfSs[i] = sinf(fRadian);
		__sfCs[i] = cosf(fRadian);
		__sfTs[i] = tanf(fRadian);
	}
}

inline float __Sine(float fRadian) // 테이블에 의한 삼각함수 값구하기..
{
	if(fRadian > -10000.0f && fRadian < 10000.0f)
	{
		while(fRadian < 0) fRadian += __PI2;
		while(fRadian > __PI2) fRadian -= __PI2;
		
		int nIndex = (int)((fRadian / __PI2) * 3600)%3600;
		return __sfSs[nIndex];
	}
	else return 0.0f;
}

inline float __Cosine(float fRadian) // 테이블에 의한 삼각함수 값구하기..
{
	if(fRadian > -10000.0f && fRadian < 10000.0f)
	{
		while(fRadian < 0) fRadian += __PI2;
		while(fRadian > __PI2) fRadian -= __PI2;
		
		int nIndex = (int)((fRadian / __PI2) * 3600)%3600;
		return __sfCs[nIndex];
	}
	else return 1.0f;
}

inline float __Tangent(float fRadian) // 테이블에 의한 삼각함수 값구하기..
{
	if(fRadian > -10000.0f && fRadian < 10000.0f)
	{
		while(fRadian < 0) fRadian += __PI2;
		while(fRadian > __PI2) fRadian -= __PI2;
		
		int nIndex = (int)((fRadian / __PI2) * 3600)%3600;
		return __sfTs[nIndex];
	}
	else return 0.0f;
}

static __Vector3 __tempV;
static __Matrix44 __tempM;

inline void __Vector2::Zero()
{
	x = y = 0;
}

inline void __Vector2::Set(float fx, float fy)
{
	x = fx; 
	y = fy;
}

inline __Vector2::__Vector2(float fx, float fy)
{
	x = fx;
	y = fy;
}

inline void	__Vector3::Normalize()
{
	float fn = sqrtf(x*x + y*y + z*z);
	if(fn == 0) return;
	x /= fn; y /= fn; z /= fn;
}

inline float __Vector3::Magnitude()
{
	return sqrtf(x*x + y*y + z*z);
}

inline float __Vector3::Dot(const D3DXVECTOR3& vec)
{
	return x*vec.x + y*vec.y + z*vec.z;
}

inline void __Vector3::Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

inline void __Vector3::Absolute()
{
	if(x < 0) x *= -1.0f;
	if(y < 0) y *= -1.0f;
	if(z < 0) z *= -1.0f;
}

inline void __Vector3::Zero()
{
	x = y = z = 0;
}

inline void __Vector3::Set(float fx, float fy, float fz)
{
	x = fx; y = fy, z = fz;
}

//inline __Vector3::__Vector3()
//{
//}

inline __Vector3::__Vector3(float fx, float fy, float fz)
{
	x = fx; y = fy, z = fz;
}

inline __Vector3::__Vector3(const D3DXVECTOR3& vec)
{
	x = vec.x; y = vec.y; z = vec.z;
}

inline __Vector3::__Vector3(const _D3DVECTOR& vec)
{
	x = vec.x; y = vec.y; z = vec.z;
}

inline __Vector3 __Vector3::operator * (const D3DXMATRIX& mtx)
{
	__tempV.x = x*mtx._11 + y*mtx._21 + z*mtx._31 + mtx._41;
	__tempV.y = x*mtx._12 + y*mtx._22 + z*mtx._32 + mtx._42;
	__tempV.z = x*mtx._13 + y*mtx._23 + z*mtx._33 + mtx._43;

	return __tempV;
}

inline void __Vector3::operator *= (float fDelta)
{
	x *= fDelta;
	y *= fDelta;
	z *= fDelta;
}

inline void __Vector3::operator *= (const D3DXMATRIX& mtx)
{
	__tempV.Set(x,y,z);
	x = __tempV.x*mtx._11 + __tempV.y*mtx._21 + __tempV.z*mtx._31 + mtx._41;
	y = __tempV.x*mtx._12 + __tempV.y*mtx._22 + __tempV.z*mtx._32 + mtx._42;
	z = __tempV.x*mtx._13 + __tempV.y*mtx._23 + __tempV.z*mtx._33 + mtx._43;
}

inline __Vector3 __Vector3::operator + (const D3DXVECTOR3& vec) 
{
	__tempV.x = x + vec.x;
	__tempV.y = y + vec.y;
	__tempV.z = z + vec.z;
	return __tempV;
}

inline __Vector3 __Vector3::operator - (const D3DXVECTOR3& vec) 
{
	__tempV.x = x - vec.x;
	__tempV.y = y - vec.y;
	__tempV.z = z - vec.z;
	return __tempV;
}

inline __Vector3 __Vector3::operator * (const D3DXVECTOR3& vec) 
{
	__tempV.x = x * vec.x;
	__tempV.y = y * vec.y;
	__tempV.z = z * vec.z;
	return __tempV;
}

inline __Vector3 __Vector3::operator / (const D3DXVECTOR3& vec) 
{
	__tempV.x = x / vec.x;
	__tempV.y = y / vec.y;
	__tempV.z = z / vec.z;
	return __tempV;
}

inline void __Vector3::operator += (const D3DXVECTOR3& vec) 
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

inline void __Vector3::operator -= (const D3DXVECTOR3& vec) 
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

inline void __Vector3::operator *= (const D3DXVECTOR3& vec) 
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
}

inline void __Vector3::operator /= (const D3DXVECTOR3& vec) 
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
}

inline __Vector3 __Vector3::operator + (float fDelta)
{ 
	__tempV.x = x + fDelta;
	__tempV.y = y + fDelta;
	__tempV.z = z + fDelta;
	return __tempV;
}

inline __Vector3 __Vector3::operator - (float fDelta)
{
	__tempV.x = x - fDelta;
	__tempV.y = y - fDelta;
	__tempV.z = z - fDelta;
	return __tempV;
}

inline __Vector3 __Vector3::operator * (float fDelta)
{
	__tempV.x = x * fDelta;
	__tempV.y = y * fDelta;
	__tempV.z = z * fDelta;
	return __tempV;
}

inline __Vector3 __Vector3::operator / (float fDelta)
{
	__tempV.x = x / fDelta;
	__tempV.y = y / fDelta;
	__tempV.z = z / fDelta;
	return __tempV;
}


inline void __Matrix44::Zero() 
{
	memset(this, 0, sizeof(_D3DMATRIX)); 
}

inline void __Matrix44::Identity()
{
	_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0;
	_11 = _22 = _33 = _44 = 1.0f;
}

inline __Vector3& __Matrix44::Pos() 
{
	__tempV.Set(_41, _42, _43);
	return __tempV;
}

inline void __Matrix44::PosSet(float x, float y, float z)
{
	_41 = x; _42 = y; _43 = z;
}

inline void __Matrix44::PosSet(const D3DXVECTOR3& v) 
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

inline void __Matrix44::RotationX(float fDelta)
{
	this->Identity();
	_22 = __Cosine(fDelta); _23 = -__Sine(fDelta); _32 = -_23; _33 = _22;
}

inline void __Matrix44::RotationY(float fDelta)
{
	this->Identity();
	_11 = __Cosine(fDelta); _13 = __Sine(fDelta); _31 = -_13; _33 = _11;
}

inline void __Matrix44::RotationZ(float fDelta)
{
	this->Identity();
	_11 = __Cosine(fDelta); _12 = -__Sine(fDelta); _21 = -_12; _22 = _11;
}

inline void __Matrix44::Rotation(float fX, float fY, float fZ)
{
	float SX = __Sine(fX), CX = __Cosine(fX);
	float SY = __Sine(fY), CY = __Cosine(fY);
	float SZ = __Sine(fZ), CZ = __Cosine(fZ);
	_11 = CY * CZ;
	_12 = CY * -SZ;
	_13 = SY;
	_14 = 0;
	
	_21 = SX * SY * CZ + CX * SZ;
	_22 = SX * SY * -SZ + CX * CZ;
	_23 = -SX * CY;
	_24 = 0;
	
	_31 = CX * -SY * CZ + SX * SZ;
	_32 = CX * -SY * -SZ + SX * CZ;
	_33 = CX * CY;
	_34 = 0;
	
	_41 = _42 = _43 = 0; _44 = 1;
}

inline void __Matrix44::Rotation(const D3DXVECTOR3& v)
{
	float SX = __Sine(v.x), CX = __Cosine(v.x);
	float SY = __Sine(v.y), CY = __Cosine(v.y);
	float SZ = __Sine(v.z), CZ = __Cosine(v.z);
	_11 = CY * CZ;
	_12 = CY * -SZ;
	_13 = SY;
	_14 = 0;
	
	_21 = SX * SY * CZ + CX * SZ;
	_22 = SX * SY * -SZ + CX * CZ;
	_23 = -SX * CY;
	_24 = 0;
	
	_31 = CX * -SY * CZ + SX * SZ;
	_32 = CX * -SY * -SZ + SX * CZ;
	_33 = CX * CY;
	_34 = 0;
	
	_41 = _42 = _43 = 0; _44 = 1;
}

inline void __Matrix44::Scale(float sx, float sy, float sz) 
{
	this->Identity();
	_11 = sx; _22 = sy; _33 = sz;
}

inline void __Matrix44::Scale(const D3DXVECTOR3& v) 
{
	this->Identity();
	_11 = v.x; _22 = v.y; _33 = v.z;
}

inline __Matrix44::__Matrix44()
{
};

inline __Matrix44::__Matrix44(const _D3DMATRIX& mtx)
{
	memcpy(this, &mtx, sizeof(_D3DMATRIX));
}

inline __Matrix44::__Matrix44(const D3DXMATRIX& mtx)
{
	memcpy(this, &mtx, sizeof(D3DXMATRIX));
}

inline __Matrix44::__Matrix44(const D3DXQUATERNION& qt)
{
	D3DXMatrixRotationQuaternion(this, &qt);
}

inline __Matrix44 __Matrix44::operator * (const D3DXMATRIX& mtx)
{
//	__tempM._11 = _11 * mtx._11 + _12 * mtx._21 + _13 * mtx._31 + _14 * mtx._41;
//	__tempM._12 = _11 * mtx._12 + _12 * mtx._22 + _13 * mtx._32 + _14 * mtx._42;
//	__tempM._13 = _11 * mtx._13 + _12 * mtx._23 + _13 * mtx._33 + _14 * mtx._43;
//	__tempM._14 = _11 * mtx._14 + _12 * mtx._24 + _13 * mtx._34 + _14 * mtx._44;

//	__tempM._21 = _21 * mtx._11 + _22 * mtx._21 + _23 * mtx._31 + _24 * mtx._41;
//	__tempM._22 = _21 * mtx._12 + _22 * mtx._22 + _23 * mtx._32 + _24 * mtx._42;
//	__tempM._23 = _21 * mtx._13 + _22 * mtx._23 + _23 * mtx._33 + _24 * mtx._43;
//	__tempM._24 = _21 * mtx._14 + _22 * mtx._24 + _23 * mtx._34 + _24 * mtx._44;

//	__tempM._31 = _31 * mtx._11 + _32 * mtx._21 + _33 * mtx._31 + _34 * mtx._41;
//	__tempM._32 = _31 * mtx._12 + _32 * mtx._22 + _33 * mtx._32 + _34 * mtx._42;
//	__tempM._33 = _31 * mtx._13 + _32 * mtx._23 + _33 * mtx._33 + _34 * mtx._43;
//	__tempM._34 = _31 * mtx._14 + _32 * mtx._24 + _33 * mtx._34 + _34 * mtx._44;

//	__tempM._41 = _41 * mtx._11 + _42 * mtx._21 + _43 * mtx._31 + _44 * mtx._41;
//	__tempM._42 = _41 * mtx._12 + _42 * mtx._22 + _43 * mtx._32 + _44 * mtx._42;
//	__tempM._43 = _41 * mtx._13 + _42 * mtx._23 + _43 * mtx._33 + _44 * mtx._43;
//	__tempM._44 = _41 * mtx._14 + _42 * mtx._24 + _43 * mtx._34 + _44 * mtx._44;

	// 최적화 된 코드..
	__tempM._11 = _11 * mtx._11 + _12 * mtx._21 + _13 * mtx._31;
	__tempM._12 = _11 * mtx._12 + _12 * mtx._22 + _13 * mtx._32;
	__tempM._13 = _11 * mtx._13 + _12 * mtx._23 + _13 * mtx._33;
	__tempM._14 = 0;

	__tempM._21 = _21 * mtx._11 + _22 * mtx._21 + _23 * mtx._31;
	__tempM._22 = _21 * mtx._12 + _22 * mtx._22 + _23 * mtx._32;
	__tempM._23 = _21 * mtx._13 + _22 * mtx._23 + _23 * mtx._33;
	__tempM._24 = 0;

	__tempM._31 = _31 * mtx._11 + _32 * mtx._21 + _33 * mtx._31;
	__tempM._32 = _31 * mtx._12 + _32 * mtx._22 + _33 * mtx._32;
	__tempM._33 = _31 * mtx._13 + _32 * mtx._23 + _33 * mtx._33;
	__tempM._34 = 0;

	__tempM._41 = _41 * mtx._11 + _42 * mtx._21 + _43 * mtx._31 + mtx._41;
	__tempM._42 = _41 * mtx._12 + _42 * mtx._22 + _43 * mtx._32 + mtx._42;
	__tempM._43 = _41 * mtx._13 + _42 * mtx._23 + _43 * mtx._33 + mtx._43;
	__tempM._44 = 1.0f;

	return __tempM;
}

inline void __Matrix44::operator *= (const D3DXMATRIX& mtx)
{
	memcpy(&__tempM, this, sizeof(__Matrix44));

//	_11 = __tempM._11 * mtx._11 + __tempM._12 * mtx._21 + __tempM._13 * mtx._31 + __tempM._14 * mtx._41;
//	_12 = __tempM._11 * mtx._12 + __tempM._12 * mtx._22 + __tempM._13 * mtx._32 + __tempM._14 * mtx._42;
//	_13 = __tempM._11 * mtx._13 + __tempM._12 * mtx._23 + __tempM._13 * mtx._33 + __tempM._14 * mtx._43;
//	_14 = __tempM._11 * mtx._14 + __tempM._12 * mtx._24 + __tempM._13 * mtx._34 + __tempM._14 * mtx._44;

//	_21 = __tempM._21 * mtx._11 + __tempM._22 * mtx._21 + __tempM._23 * mtx._31 + __tempM._24 * mtx._41;
//	_22 = __tempM._21 * mtx._12 + __tempM._22 * mtx._22 + __tempM._23 * mtx._32 + __tempM._24 * mtx._42;
//	_23 = __tempM._21 * mtx._13 + __tempM._22 * mtx._23 + __tempM._23 * mtx._33 + __tempM._24 * mtx._43;
//	_24 = __tempM._21 * mtx._14 + __tempM._22 * mtx._24 + __tempM._23 * mtx._34 + __tempM._24 * mtx._44;

//	_31 = __tempM._31 * mtx._11 + __tempM._32 * mtx._21 + __tempM._33 * mtx._31 + __tempM._34 * mtx._41;
//	_32 = __tempM._31 * mtx._12 + __tempM._32 * mtx._22 + __tempM._33 * mtx._32 + __tempM._34 * mtx._42;
//	_33 = __tempM._31 * mtx._13 + __tempM._32 * mtx._23 + __tempM._33 * mtx._33 + __tempM._34 * mtx._43;
//	_34 = __tempM._31 * mtx._14 + __tempM._32 * mtx._24 + __tempM._33 * mtx._34 + __tempM._34 * mtx._44;

//	_41 = __tempM._41 * mtx._11 + __tempM._42 * mtx._21 + __tempM._43 * mtx._31 + __tempM._44 * mtx._41;
//	_42 = __tempM._41 * mtx._12 + __tempM._42 * mtx._22 + __tempM._43 * mtx._32 + __tempM._44 * mtx._42;
//	_43 = __tempM._41 * mtx._13 + __tempM._42 * mtx._23 + __tempM._43 * mtx._33 + __tempM._44 * mtx._43;
//	_44 = __tempM._41 * mtx._14 + __tempM._42 * mtx._24 + __tempM._43 * mtx._34 + __tempM._44 * mtx._44;

	_11 = __tempM._11 * mtx._11 + __tempM._12 * mtx._21 + __tempM._13 * mtx._31;
	_12 = __tempM._11 * mtx._12 + __tempM._12 * mtx._22 + __tempM._13 * mtx._32;
	_13 = __tempM._11 * mtx._13 + __tempM._12 * mtx._23 + __tempM._13 * mtx._33;
	_14 = 0;

	_21 = __tempM._21 * mtx._11 + __tempM._22 * mtx._21 + __tempM._23 * mtx._31 + __tempM._24 * mtx._41;
	_22 = __tempM._21 * mtx._12 + __tempM._22 * mtx._22 + __tempM._23 * mtx._32 + __tempM._24 * mtx._42;
	_23 = __tempM._21 * mtx._13 + __tempM._22 * mtx._23 + __tempM._23 * mtx._33 + __tempM._24 * mtx._43;
	_24 = 0;

	_31 = __tempM._31 * mtx._11 + __tempM._32 * mtx._21 + __tempM._33 * mtx._31 + __tempM._34 * mtx._41;
	_32 = __tempM._31 * mtx._12 + __tempM._32 * mtx._22 + __tempM._33 * mtx._32 + __tempM._34 * mtx._42;
	_33 = __tempM._31 * mtx._13 + __tempM._32 * mtx._23 + __tempM._33 * mtx._33 + __tempM._34 * mtx._43;
	_34 = 0;

	_41 = __tempM._41 * mtx._11 + __tempM._42 * mtx._21 + __tempM._43 * mtx._31 + mtx._41;
	_42 = __tempM._41 * mtx._12 + __tempM._42 * mtx._22 + __tempM._43 * mtx._32 + mtx._42;
	_43 = __tempM._41 * mtx._13 + __tempM._42 * mtx._23 + __tempM._43 * mtx._33 + mtx._43;
	_44 = 1;
}

inline void __Matrix44::operator = (const D3DXQUATERNION& qt)
{
	D3DXMatrixRotationQuaternion(this, &qt);
}

inline void __Matrix44::LookAt(const __Vector3& vPos, const __Vector3& vAt, const __Vector3& vUp)
{
/*	__Vector3 F = vAt - vPos;
	F.Normalize();

	__Vector3 S;
	S.Cross(F, vUp);
	S.Normalize();

	__Vector3 U;
	U.Cross(S, F);
	U.Normalize();

	_11 = S.x; _12 = U.x; _13 = F.x; _14 = 0;
	_21 = S.y; _22 = U.y; _23 = F.y; _24 = 0;
	_31 = S.z; _32 = U.z; _33 = F.z; _34 = 0;
	_41 = -vPos.x;
	_42 = -vPos.y;
	_43 = -vPos.z;
	_44 = 1;
*/
}

inline void __Quaternion::Identity()
{
	x = y = z = 0; w = 1.0f;
}


inline void __Quaternion::Set(float fX, float fY, float fZ, float fW)
{
	x = fX; y = fY; z = fZ; w = fW;
}

inline void __Quaternion::RotationAxis(const __Vector3& v, float fRadian)
{
	D3DXQuaternionRotationAxis(this, &v, fRadian);
}

inline void __Quaternion::RotationAxis(float fX, float fY, float fZ, float fRadian)
{
	__Vector3 v(fX, fY, fZ);
	D3DXQuaternionRotationAxis(this, &v, fRadian);
}

inline void __Quaternion::operator = (const D3DXMATRIX& mtx)
{
	D3DXQuaternionRotationMatrix(this, &mtx);
}

inline void __Quaternion::AxisAngle(__Vector3& vAxisResult, float& fRadianResult)
{
	D3DXQuaternionToAxisAngle(this, &vAxisResult, &fRadianResult);
}

inline void __Quaternion::Slerp(const D3DXQUATERNION& qt1, const D3DXQUATERNION& qt2, float fDelta)
{
	D3DXQuaternionSlerp(this, &qt1, &qt2, fDelta);
}

inline __Quaternion::__Quaternion()
{
}

inline __Quaternion::__Quaternion(const D3DXMATRIX& mtx)
{
	D3DXQuaternionRotationMatrix(this, &mtx);
}