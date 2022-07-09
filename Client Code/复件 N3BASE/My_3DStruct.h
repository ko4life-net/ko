#ifndef __MY_3DSTRUCT_H_
#define __MY_3DSTRUCT_H_

#include "N3DUtil.h"
#include "N3VectorAndMatrix.h"

const DWORD FVF_VNT1 = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
const DWORD FVF_VNT2 = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
const DWORD FVF_CV = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD FVF_CSV = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR;
const DWORD FVF_TRANSFORMED = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD FVF_TRANSFORMEDT2 = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2;
const DWORD FVF_TRANSFORMEDCOLOR = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

//..
const DWORD FVF_XYZT1				= D3DFVF_XYZ | D3DFVF_TEX1;
const DWORD FVF_XYZT2				= D3DFVF_XYZ | D3DFVF_TEX2;
const DWORD FVF_XYZNORMAL			= D3DFVF_XYZ | D3DFVF_NORMAL;
const DWORD FVF_XYZCOLORT1			= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD FVF_XYZCOLORT2			= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
const DWORD FVF_XYZCOLORSPECULART1	= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;
const DWORD FVF_XYZCOLOR			= D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD FVF_XYZNORMALCOLOR		= D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_DIFFUSE;
const DWORD FVF_XYZNORMALCOLORT1	= D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_DIFFUSE | D3DFVF_TEX1;

const DWORD RF_NOTHING = 0;
const DWORD RF_ALPHABLENDING = 1;
const DWORD RF_NOTUSEFOG = 2;
const DWORD RF_DOUBLESIDED = 4;

struct __Material : public _D3DMATERIAL8
{
public:
	DWORD	dwColorOp, dwColorArg1, dwColorArg2;
	BOOL	nRenderFlags; // 1-AlphaBlending | 2-안개랑 관계없음 | 4-Double Side | 8- ??
	DWORD	dwSrcBlend; // 소스 블렌딩 방법
	DWORD	dwDestBlend; // 데스트 블렌딩 방법

public:
	void Init()
	{
		memset(this, 0, sizeof(__Material));
		_D3DCOLORVALUE white = { 1.0f, 1.0f, 1.0f, 1.0f };
//		Specular = Diffuse = Ambient = white; // 기본은 흰색..
		Diffuse = white; // 기본은 흰색..
		Ambient.a = Diffuse.a;
		Ambient.r = Diffuse.r * 0.8f;
		Ambient.g = Diffuse.g * 0.8f;
		Ambient.b = Diffuse.b * 0.8f;
		
		dwColorOp = D3DTOP_MODULATE;
		dwColorArg1 = D3DTA_DIFFUSE;
		dwColorArg2 = D3DTA_TEXTURE;
		nRenderFlags = RF_NOTHING;
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

	void ColorSet(_D3DCOLORVALUE crDiffuse)
	{
		Diffuse = crDiffuse; 
		Ambient.a = Diffuse.a;
		Ambient.r = Diffuse.r * 0.8f;
		Ambient.g = Diffuse.g * 0.8f;
		Ambient.b = Diffuse.b * 0.8f;
	}
};

struct __VertexColor
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
	D3DCOLOR color; // 필요 없다..
	float tu, tv;

public:
	void Set(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv)
		{ x = sx; y = sy; z = sz; rhw = srhw; color = sColor; tu = stu; tv = stv; }

	__VertexTransformed() {}
	__VertexTransformed(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv)
		{ this->Set(sx, sy, sz, srhw, sColor, stu, stv); }
};

struct __VertexTransformedT2 : public __VertexTransformed
{
public:
	float tu2, tv2;

public:
	void Set(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv, float stu2, float stv2)
	{ x = sx; y = sy; z = sz; rhw = srhw; color = sColor; tu = stu; tv = stv; tu2 = stu2; tv2 = stv2; }

	__VertexTransformedT2() {}
	__VertexTransformedT2(float sx, float sy, float sz, float srhw, D3DCOLOR sColor, float stu, float stv, float stu2, float stv2)
	{
		this->Set(sx, sy, sz, srhw, sColor, stu, stv, stu2, stv2);
	}
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
	void Set(float xx, float yy, float zz, float nxx, float nyy, float nzz) { x = xx; y = yy; z = zz; nx = nxx; ny = nyy; nz = nzz; }
	__VertexXyzNormal() {}
	__VertexXyzNormal(_D3DVECTOR& p, _D3DVECTOR& n) { this->Set(p, n); }
	__VertexXyzNormal(float sx, float sy, float sz, float xx, float yy, float zz) { this->Set(sx, sy, sz, xx, yy, zz); }
};


struct __VertexXyzColorSpecularT1
{
public:
	float x, y, z;
	D3DCOLOR color;
	D3DCOLOR specular;
	float tu, tv;

public:
	void Set(_D3DVECTOR& p, D3DCOLOR sColor, D3DCOLOR sSpecular, float u, float v) { x = p.x; y = p.y; z = p.z; color = sColor; specular = sSpecular, tu = u; tv = v; }
	void Set(float sx, float sy, float sz, D3DCOLOR sColor, D3DCOLOR sSpecular, float u, float v) { x = sx; y = sy; z = sz; color = sColor; specular = sSpecular, tu = u; tv = v;	}
	__VertexXyzColorSpecularT1() {}
	__VertexXyzColorSpecularT1(_D3DVECTOR& p, D3DCOLOR sColor, D3DCOLOR sSpecular, float u, float v) { this->Set(p, sColor, sSpecular, u, v); }
	__VertexXyzColorSpecularT1(float sx, float sy, float sz, D3DCOLOR sColor, D3DCOLOR sSpecular, float u, float v) { this->Set(sx, sy, sz, sColor, sSpecular, u, v); }
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

struct __VertexXyzColorT2 : public __VertexXyzColorT1
{
public:
	float tu2, tv2;
public:
	void Set(_D3DVECTOR& p, D3DCOLOR sColor, float u, float v, float u2, float v2) { x = p.x; y = p.y; z = p.z; color = sColor; tu = u; tv = v; tu2 = u2; tv2 = v2;}
	void Set(float sx, float sy, float sz, D3DCOLOR sColor, float u, float v, float u2, float v2) { x = sx; y = sy; z = sz; color = sColor; tu = u2; tv = v2;	}
	__VertexXyzColorT2() {}
	__VertexXyzColorT2(_D3DVECTOR& p, D3DCOLOR sColor, float u, float v, float u2, float v2) { this->Set(p, sColor, u, v, u2, v2); }
	__VertexXyzColorT2(float sx, float sy, float sz, D3DCOLOR sColor, float u, float v, float u2, float v2) { this->Set(sx, sy, sz, sColor, u, v, u2, v2); }
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



const int MAX_MIPMAP_COUNT = 10; // 1024 * 1024 단계까지 생성

const DWORD OBJ_UNKNOWN				= 0;
const DWORD OBJ_BASE				= 0x1;
const DWORD OBJ_TEXTURE				= 0x2;
const DWORD OBJ_TRANSFORM 			= 0x4;
const DWORD OBJ_TRANSFORM_COLLISION = 0x8;
const DWORD OBJ_SCENE				= 0x10;

const DWORD OBJ_CAMERA				= 0x100;
const DWORD OBJ_LIGHT				= 0x200;
const DWORD OBJ_SHAPE				= 0x400;
const DWORD OBJ_SHAPE_PART			= 0x800;
const DWORD OBJ_SHAPE_EXTRA			= 0x1000;
const DWORD OBJ_CHARACTER			= 0x2000;
const DWORD OBJ_CHARACTER_PART		= 0x4000;
const DWORD OBJ_CHARACTER_PLUG		= 0x8000;
const DWORD OBJ_BOARD				= 0x1000;

const DWORD OBJ_MESH				= 0x100000;
const DWORD OBJ_MESH_PROGRESSIVE	= 0x200000;
const DWORD OBJ_MESH_INDEXED		= 0x400000;
const DWORD OBJ_MESH_VECTOR3		= 0x800000;
const DWORD OBJ_JOINT				= 0x1000000;
const DWORD OBJ_SKIN				= 0x2000000;

const DWORD OBJ_DUMMY				= 0x10000000;
const DWORD OBJ_EFFECT				= 0x20000000;
const DWORD OBJ_ANIM_CONTROL		= 0x40000000;
const DWORD OBJ_SUBSCENE			= 0x80000000;


#define g_EPSILON (1.0e-5f)

#endif // __MY_3DSTRUCT_H_

