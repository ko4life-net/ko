#ifndef __MY_3DSTRUCT_H_
#define __MY_3DSTRUCT_H_
#include <d3dx9.h>
#include <d3dx9math.h>
#include <string>

const DWORD FVF_VNT1 = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
const DWORD FVF_VNT2 = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
const DWORD FVF_CV = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD FVF_TRANSFORMED = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD FVF_TRANSFORMEDCOLOR = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

//..
const DWORD FVF_XYZT1				= D3DFVF_XYZ | D3DFVF_TEX1;
const DWORD FVF_XYZT2				= D3DFVF_XYZ | D3DFVF_TEX2;
const DWORD FVF_XYZNORMAL			= D3DFVF_XYZ | D3DFVF_NORMAL;
const DWORD FVF_XYZCOLORT1			= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD FVF_XYZCOLOR			= D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD FVF_XYZNORMALCOLOR		= D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_DIFFUSE;
const DWORD FVF_XYZNORMALCOLORT1	= D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct __Vector3 : public D3DXVECTOR3 // 3D Vertex
{
public:
	void	Normalize();
	float	Magnitude() const;
	float	Dot(const D3DXVECTOR3& vec) const;
	void	Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
	void	Absolute();

	void Zero();
	void Set(float fx, float fy, float fz);

	__Vector3() {};
	__Vector3(float fx, float fy, float fz);
	__Vector3(const _D3DVECTOR& vec);
	__Vector3(const D3DXVECTOR3& vec);

	const __Vector3& operator = (const __Vector3& vec);

	const __Vector3 operator * (const D3DXMATRIX& mtx) const;
	void operator *= (float fDelta);
	void operator *= (const D3DXMATRIX& mtx);
	__Vector3 operator + (const D3DXVECTOR3& vec) const;
	__Vector3 operator - (const D3DXVECTOR3& vec) const;
	__Vector3 operator * (const D3DXVECTOR3& vec) const;
	__Vector3 operator / (const D3DXVECTOR3& vec) const;

	void operator += (const D3DXVECTOR3& vec);
	void operator -= (const D3DXVECTOR3& vec);
	void operator *= (const D3DXVECTOR3& vec);
	void operator /= (const D3DXVECTOR3& vec);

	__Vector3 operator + (float fDelta) const;
	__Vector3 operator - (float fDelta) const;
	__Vector3 operator * (float fDelta) const;
	__Vector3 operator / (float fDelta) const;
};

struct __Material : public _D3DMATERIAL9
{
public:
	DWORD	dwColorOp, dwColorArg1, dwColorArg2;
	BOOL	nUseAlpha; // 알파블렌딩을 사용하는가 ?? 1-AlphaBlending | 2-안개랑 관계없음 | 4-낮에만 나타난다. | 8-??
	DWORD	dwSrcBlend; // 소스 블렌딩 방법
	DWORD	dwDestBlend; // 데스트 블렌딩 방법

public:
	void Init()
	{
		memset(this, 0, sizeof(__Material));
		_D3DCOLORVALUE white = { 1.0f, 1.0f, 1.0f, 1.0f };
//		Specular = Diffuse = Ambient = white; // 기본은 흰색..
		Diffuse = Ambient = white; // 기본은 흰색..
		
		dwColorOp = D3DTOP_MODULATE;
		dwColorArg1 = D3DTA_DIFFUSE;
		dwColorArg2 = D3DTA_TEXTURE;
		nUseAlpha = 0;
		dwSrcBlend = D3DBLEND_SRCALPHA;
		dwDestBlend = D3DBLEND_INVSRCALPHA;
	}

	void Init(_D3DCOLORVALUE& ambientColor, _D3DCOLORVALUE& diffuseColor, _D3DCOLORVALUE& specularColor)
	{
		this->Init();
		Ambient = ambientColor;
		Diffuse = diffuseColor;
		Specular = specularColor;
	}
};

struct __VertexColor
{
public:
	float x, y, z;
	D3DCOLOR color;

public:
	void Set(_D3DVECTOR& p, D3DCOLOR sColor) { x = p.x; y = p.y; z = p.z; color = sColor; }
	void Set(float sx, float sy, float sz, D3DCOLOR sColor) { x = sx; y = sy; z = sz; color = sColor; }
	__VertexColor() {}
	__VertexColor(_D3DVECTOR& p, D3DCOLOR sColor) { this->Set(p, sColor); }
	__VertexColor(float sx, float sy, float sz, D3DCOLOR sColor) { this->Set(sx, sy, sz, sColor); }
};

struct __VertexTransformedColor
{
public:
	float x, y, z, rhw;
	D3DCOLOR color;

public:
	void Set(float sx, float sy, float sz, float srhw, D3DCOLOR sColor) { x = sx; y = sy; z = sz; rhw = srhw; color = sColor; }
	__VertexTransformedColor() {}
	__VertexTransformedColor(float sx, float sy, float sz, float srhw, D3DCOLOR sColor) { this->Set(sx, sy, sz, srhw, sColor); }
};

struct __VertexT1
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	union
	{
		struct
		{
			float nx, ny, nz;
		};
		D3DVECTOR n;
	};
	float tu, tv;

public:
	void Set(_D3DVECTOR& p, _D3DVECTOR& n, float u, float v)
	{
		x = p.x; y = p.y; z = p.z;
		nx = n.x; ny = n.y; nz = n.z;
		tu = u; tv = v;
	}
	void Set(float sx, float sy, float sz, float snx, float sny, float snz, float stu, float stv)
	{
		x = sx; y = sy; z = sz;
		nx = snx; ny = sny; nz = snz;
		tu = stu; tv = stv;
	}

	__VertexT1() {}
	__VertexT1(_D3DVECTOR& p, _D3DVECTOR& n, float u, float v) { this->Set(p, n, u, v); }
	__VertexT1(float sx, float sy, float sz, float snx, float sny, float snz, float stu, float stv) 
		{ this->Set(sx, sy, sz, snx, sny, snz, stu, stv); } 
};

struct __VertexT2 : public __VertexT1
{
public:
	float tu2, tv2;
public:
	void Set(_D3DVECTOR& p, _D3DVECTOR& n, float u, float v, float u2, float v2)
	{
		x = p.x; y = p.y; z = p.z;
		nx = n.x; ny = n.y; nz = n.z;
		tu = u; tv = v; tu2 = u2; tv2 = v2;
	}
	void Set(float sx, float sy, float sz, float snx, float sny, float snz, float stu, float stv, float stu2, float stv2)
	{
		x = sx; y = sy; z = sz;
		nx = snx; ny = sny; nz = snz;
		tu = stu; tv = stv;
		tu2 = stu2; tv2 = stv2;
	}
	__VertexT2() {}
	__VertexT2(_D3DVECTOR& p, _D3DVECTOR& n, float u, float v, float u2, float v2) { this->Set(p, n, u, v, u2, v2); }
	__VertexT2(float sx, float sy, float sz, float snx, float sny, float snz, float stu, float stv, float stu2, float stv2)
		{ this->Set(sx, sy, sz, snx, sny, snz, stu, stv, stu2, stv2); } 
};

struct __VertexTransformed
{
public:
	union
	{
		struct 
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	float rhw;
	D3DCOLOR color;
	float tu, tv;

public:
	void Set(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv)
		{ x = sx; y = sy; z = sz; rhw = srhw; color = sColor; tu = stu; tv = stv; }

	__VertexTransformed() {}
	__VertexTransformed(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv)
		{ this->Set(sx, sy, sz, srhw, sColor, stu, stv); }
};

//..
struct __VertexXyzT1
{
public:
	union
	{
		struct 
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	float tu, tv;	

public:
	void Set(_D3DVECTOR& p, float u, float v) { x = p.x; y = p.y; z = p.z; tu = u; tv = v; }
	void Set(float sx, float sy, float sz, float u, float v) { x = sx; y = sy; z = sz; tu = u; tv = v; }
	__VertexXyzT1() {}
	__VertexXyzT1(_D3DVECTOR& p, float u, float v) { this->Set(p, u, v); }
	__VertexXyzT1(float sx, float sy, float sz, float u, float v) { this->Set(sx, sy, sz, u, v); }
};


struct __VertexXyzT2 : public __VertexXyzT1
{
public:
	float tu2, tv2;	

public:
	void Set(_D3DVECTOR& p, float u, float v, float u2, float v2) { x = p.x; y = p.y; z = p.z; tu = u; tv = v; tu2 = u2; tv2 = v2;}
	void Set(float sx, float sy, float sz, float u, float v, float u2, float v2) { x = sx; y = sy; z = sz; tu = u; tv = v; tu2 = u2; tv2 = v2;}
	__VertexXyzT2() {}
	__VertexXyzT2(_D3DVECTOR& p, float u, float v, float u2, float v2) { this->Set(p, u, v, u2, v2); }
	__VertexXyzT2(float sx, float sy, float sz, float u, float v, float u2, float v2) { this->Set(sx, sy, sz, u, v, u2, v2); }
};



struct __VertexXyzNormal
{
public:
	union
	{
		struct 
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	union
	{
		struct 
		{
			float nx, ny, nz;
		};
		D3DVECTOR n;
	};

public:
	void Set(_D3DVECTOR& p, _D3DVECTOR& n) { x = p.x; y = p.y; z = p.z; nx = n.x; ny = n.y; nz = n.z; }
	void Set(float sx, float sy, float sz, float xx, float yy, float zz) { x = sx; y = sy; z = sz; nx = xx; ny = yy; nz = zz; }
	__VertexXyzNormal() {}
	__VertexXyzNormal(_D3DVECTOR& p, _D3DVECTOR& n) { this->Set(p, n); }
	__VertexXyzNormal(float sx, float sy, float sz, float xx, float yy, float zz) { this->Set(sx, sy, sz, xx, yy, zz); }
};



struct __VertexXyzColorT1
{
public:
	float x, y, z;
	D3DCOLOR color;
	float tu, tv;

public:
	void Set(_D3DVECTOR& p, D3DCOLOR sColor, float u, float v) { x = p.x; y = p.y; z = p.z; color = sColor; tu = u; tv = v; }
	void Set(float sx, float sy, float sz, D3DCOLOR sColor, float u, float v) { x = sx; y = sy; z = sz; color = sColor; tu = u; tv = v;	}
	__VertexXyzColorT1() {}
	__VertexXyzColorT1(_D3DVECTOR& p, D3DCOLOR sColor, float u, float v) { this->Set(p, sColor, u, v); }
	__VertexXyzColorT1(float sx, float sy, float sz, D3DCOLOR sColor, float u, float v) { this->Set(sx, sy, sz, sColor, u, v); }
};



struct __VertexXyzColor
{
public:
	union
	{
		struct 
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	D3DCOLOR color;

public:
	void Set(_D3DVECTOR& p, D3DCOLOR sColor) { x = p.x; y = p.y; z = p.z; color = sColor; }
	void Set(float sx, float sy, float sz, D3DCOLOR sColor) { x = sx; y = sy; z = sz; color = sColor; }
	__VertexXyzColor() {}
	__VertexXyzColor(_D3DVECTOR& p, D3DCOLOR sColor) { this->Set(p, sColor); }
	__VertexXyzColor(float sx, float sy, float sz, D3DCOLOR sColor) { this->Set(sx, sy, sz, sColor); }
};



struct __VertexXyzNormalColor
{
public:
	union
	{
		struct 
		{
			float x, y, z;
		};
		D3DVECTOR v;
	};
	union
	{
		struct 
		{
			float nx, ny, nz;
		};
		D3DVECTOR n;
	};
	D3DCOLOR color;

public:
	void Set(_D3DVECTOR& p, _D3DVECTOR& n, D3DCOLOR sColor) { x = p.x; y = p.y; z = p.z; nx = n.x; ny = n.y; nz = n.z; color = sColor; }
	void Set(float sx, float sy, float sz, float xx, float yy, float zz, D3DCOLOR sColor) { x = sx; y = sy; z = sz; nx = xx; ny = yy; nz = zz; color = sColor; }
	__VertexXyzNormalColor() {}
	__VertexXyzNormalColor(_D3DVECTOR& p, _D3DVECTOR& n, D3DCOLOR sColor) { this->Set(p, n, sColor); }
	__VertexXyzNormalColor(float sx, float sy, float sz, float xx, float yy, float zz, D3DCOLOR sColor) { this->Set(sx, sy, sz, xx, yy, zz, sColor); }
};


inline void	__Vector3::Normalize()
{
	float fn = sqrtf(x * x + y * y + z * z);
	if (fn == 0) return;
	x /= fn; y /= fn; z /= fn;
}

inline float __Vector3::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z);
}

inline float __Vector3::Dot(const D3DXVECTOR3& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

inline void __Vector3::Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

inline void __Vector3::Absolute()
{
	if (x < 0) x *= -1.0f;
	if (y < 0) y *= -1.0f;
	if (z < 0) z *= -1.0f;
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

inline const __Vector3& __Vector3::operator = (const __Vector3& vec)
{
	x = vec.x; y = vec.y; z = vec.z;
	return *this;
}

inline const __Vector3 __Vector3::operator * (const D3DXMATRIX& mtx) const
{
	static __Vector3 vTmp;

	vTmp.x = x * mtx._11 + y * mtx._21 + z * mtx._31 + mtx._41;
	vTmp.y = x * mtx._12 + y * mtx._22 + z * mtx._32 + mtx._42;
	vTmp.z = x * mtx._13 + y * mtx._23 + z * mtx._33 + mtx._43;

	return vTmp;
}

inline void __Vector3::operator *= (float fDelta)
{
	x *= fDelta;
	y *= fDelta;
	z *= fDelta;
}

inline void __Vector3::operator *= (const D3DXMATRIX& mtx)
{
	static __Vector3 vTmp;

	vTmp.Set(x, y, z);
	x = vTmp.x * mtx._11 + vTmp.y * mtx._21 + vTmp.z * mtx._31 + mtx._41;
	y = vTmp.x * mtx._12 + vTmp.y * mtx._22 + vTmp.z * mtx._32 + mtx._42;
	z = vTmp.x * mtx._13 + vTmp.y * mtx._23 + vTmp.z * mtx._33 + mtx._43;
}

inline __Vector3 __Vector3::operator + (const D3DXVECTOR3& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x + vec.x;
	vTmp.y = y + vec.y;
	vTmp.z = z + vec.z;
	return vTmp;
}

inline __Vector3 __Vector3::operator - (const D3DXVECTOR3& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x - vec.x;
	vTmp.y = y - vec.y;
	vTmp.z = z - vec.z;
	return vTmp;
}

inline __Vector3 __Vector3::operator * (const D3DXVECTOR3& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x * vec.x;
	vTmp.y = y * vec.y;
	vTmp.z = z * vec.z;
	return vTmp;
}

inline __Vector3 __Vector3::operator / (const D3DXVECTOR3& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x / vec.x;
	vTmp.y = y / vec.y;
	vTmp.z = z / vec.z;
	return vTmp;
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

inline __Vector3 __Vector3::operator + (float fDelta) const
{
	static __Vector3 vTmp;

	vTmp.x = x + fDelta;
	vTmp.y = y + fDelta;
	vTmp.z = z + fDelta;
	return vTmp;
}

inline __Vector3 __Vector3::operator - (float fDelta) const
{
	static __Vector3 vTmp;

	vTmp.x = x - fDelta;
	vTmp.y = y - fDelta;
	vTmp.z = z - fDelta;
	return vTmp;
}

inline __Vector3 __Vector3::operator * (float fDelta) const
{
	static __Vector3 vTmp;

	vTmp.x = x * fDelta;
	vTmp.y = y * fDelta;
	vTmp.z = z * fDelta;
	return vTmp;
}

inline __Vector3 __Vector3::operator / (float fDelta) const
{
	static __Vector3 vTmp;

	vTmp.x = x / fDelta;
	vTmp.y = y / fDelta;
	vTmp.z = z / fDelta;
	return vTmp;
}


const int MAX_MIPMAP_COUNT = 10; // 1024 * 1024 단계까지 생성

enum DXT_PF 
{
	DXT_PF_1555 = 0x1555,
	DXT_PF_4444 = 0x4444,
	DXT_PF_8888 = 0x8888,
};

enum ANIMATION_KEY_TYPE { 
	KEY_POSITION_TCB = 1,
	KEY_POSITION_BEZIER = 2,
	KEY_POSITION_LINEAR = 3,
	KEY_POSITION_UNKNOWN = 0x1fffffff,

	KEY_ROTATION_TCB = 11,
	KEY_ROTATION_BEZIER = 12,
	KEY_ROTATION_LINEAR = 13,
	KEY_ROTATION_UNKNOWN = 0x2fffffff,

	KEY_SCALE_TCB = 21,
	KEY_SCALE_BEZIER = 22,
	KEY_SCALE_LINEAR = 23,
	KEY_SCALE_UNKNOWN = 0x3fffffff,
	
	KEY_UNKNOWN = 0xffffffff,
}; 


// By lynus..
typedef struct _LyCameraData
{
	__Vector3 vPos;
	__Vector3 vUp;
	__Vector3 vDir;
	__Vector3 vFarPoint[4];

	float fFov;
	float fAspect;
	float fFP;
	float fNP;
} LYCAMERADATA, *LPLYCAMERADATA;


const DWORD OBJ_UNKNOWN				= 0;
const DWORD OBJ_BASE				= 0x1;
const DWORD OBJ_TEXTURE				= 0x2;
const DWORD OBJ_TRANSFORM 			= 0x4;
const DWORD OBJ_TRANSFORM_COLLISION = 0x8;
const DWORD OBJ_SCENE				= 0x10;

const DWORD OBJ_CAMERA				= 0x100;
const DWORD OBJ_LIGHT				= 0x200;
const DWORD OBJ_SHAPE				= 0x400;
const DWORD OBJ_CHARACTER			= 0x800;

const DWORD OBJ_MESH				= 0x10000;
const DWORD OBJ_MESH_PROGRESSIVE	= 0x20000;
const DWORD OBJ_MESH_INDEXED		= 0x40000;
const DWORD OBJ_MESH_VECTOR3		= 0x80000;
const DWORD OBJ_JOINT				= 0x100000;
const DWORD OBJ_SKIN				= 0x200000;

const DWORD OBJ_DUMMY				= 0x1000000;
const DWORD OBJ_EFFECT				= 0x2000000;
const DWORD OBJ_ANIM_CONTROL		= 0x4000000;
const DWORD OBJ_SUBSCENE			= 0x8000000;

#endif // __MY_3DSTRUCT_H_

