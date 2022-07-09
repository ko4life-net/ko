#ifndef __N3D_UTIL_H_2001_08_08
#define __N3D_UTIL_H_2001_08_08

#include "N3VectorAndMatrix.h"
#include "CrtDbg.h"

#ifndef _DEBUG
#define __ASSERT(expr, expMessage) void(0)
#else
#define __ASSERT(expr, expMessage) \
if(!(expr)) { _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, "N3 Custom Assert Functon", expMessage); _CrtDbgBreak(); }
#endif


D3DCOLOR _RGB_To_D3DCOLOR(COLORREF cr, float fAlpha);
COLORREF _D3DCOLOR_To_RGB(D3DCOLOR& cr);
COLORREF _D3DCOLORVALUE_To_RGB(D3DCOLORVALUE& cr);
D3DCOLORVALUE _RGB_To_D3DCOLORVALUE(COLORREF cr, float fAlpha);
bool IntersectTriangle(__Vector3& vOrig, __Vector3& vDir , __Vector3& v0, __Vector3& v1, __Vector3& v2, float& fT, float& fU, float& fV, __Vector3* pVCol = NULL);
bool IntersectTriangle(__Vector3& vOrig, __Vector3& vDir, __Vector3& v0, __Vector3& v1, __Vector3& v2);
POINT Convert3D_To_2DCoordinate(__Vector3 &vPos, __Matrix44& mtxView, __Matrix44& mtxProjection, int nVPW, int nVPH);
#endif // __N3D_UTIL_H_2001_08_08