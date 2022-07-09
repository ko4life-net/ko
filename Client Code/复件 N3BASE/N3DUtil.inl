#include "N3DUtil.h"

inline D3DCOLOR _RGB_To_D3DCOLOR(COLORREF cr, DWORD dwAlpha)
{
	D3DCOLOR cr2 =	(dwAlpha << 24) | 
					((cr & 0x000000ff)<<16) | // R
					(cr & 0x0000ff00) | // G
					((cr & 0x00ff0000)>>16); // B
	return cr2;
};

inline COLORREF _D3DCOLOR_To_RGB(D3DCOLOR& cr)
{
	COLORREF cr2 =	((cr & 0x00ff0000)>>16) | // R
					(cr & 0x0000ff00) | // G
					((cr & 0x000000ff)<<16); // B
	return cr2;
};

inline COLORREF _D3DCOLORVALUE_To_RGB(D3DCOLORVALUE& cr)
{
	COLORREF cr2 =	(((DWORD)(cr.r*255.0f))) | // R
					(((DWORD)(cr.g*255.0f))<<8) | // G
					(((DWORD)(cr.b*255.0f))<<16); // B
	return cr2;
};

inline D3DCOLOR _D3DCOLORVALUE_To_D3DCOLOR(D3DCOLORVALUE& cr)
{
	COLORREF cr2 =	(((DWORD)(cr.a*255.0f))<<24) | // A
					(((DWORD)(cr.r*255.0f))<<16) | // R
					(((DWORD)(cr.g*255.0f))<<8) | // G
					(((DWORD)(cr.b*255.0f))); // B
	return cr2;
};

inline D3DCOLORVALUE _RGB_To_D3DCOLORVALUE(COLORREF cr, float fAlpha)
{
	D3DCOLORVALUE cr2;
	cr2.a =	fAlpha; // Alpha
	cr2.r = (cr & 0x000000ff)/255.0f;
	cr2.g = ((cr & 0x0000ff00)>>8)/255.0f;
	cr2.b = ((cr & 0x00ff0000)>>16)/255.0f;
	return cr2;
};

inline D3DCOLORVALUE _D3DCOLOR_To_D3DCOLORVALUE(D3DCOLOR cr)
{
	D3DCOLORVALUE cr2;
	cr2.a =	((cr & 0xff000000)>>24)/255.0f;
	cr2.r = ((cr & 0x00ff0000)>>16)/255.0f;
	cr2.g = ((cr & 0x0000ff00)>>8)/255.0f;
	cr2.b = (cr & 0x000000ff)/255.0f; // Alpha
	return cr2;
};

inline bool IntersectTriangle(__Vector3& vOrig, __Vector3& vDir , __Vector3& v0, __Vector3& v1, __Vector3& v2, float& fT, float& fU, float& fV, __Vector3* pVCol)
{
    // Find vectors for two edges sharing vert0
    static __Vector3 vEdge1, vEdge2;
	
	vEdge1 = v1 - v0;
    vEdge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    __Vector3 pVec;	float fDet;
	
//	By : Ecli666 ( On 2001-09-12 오전 10:39:01 )

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return FALSE;

//	~(By Ecli666 On 2001-09-12 오전 10:39:01 )

    pVec.Cross(vDir, vEdge2);

    // If determinant is near zero, ray lies in plane of triangle
    fDet = vEdge1.Dot(pVec);
    if( fDet < 0.0001f )		// 거의 0에 가까우면 삼각형 평면과 지나가는 선이 평행하다.
        return FALSE;

    // Calculate distance from vert0 to ray origin
    __Vector3 tVec = vOrig - v0;

    // Calculate U parameter and test bounds
    fU = tVec.Dot(pVec);
    if( fU < 0.0f || fU > fDet )
        return FALSE;

    // Prepare to test V parameter
    __Vector3 qVec;
    qVec.Cross(tVec, vEdge1);

    // Calculate V parameter and test bounds
    fV = D3DXVec3Dot( &vDir, &qVec );
    if( fV < 0.0f || fU + fV > fDet )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    fT = D3DXVec3Dot( &vEdge2, &qVec );
    float fInvDet = 1.0f / fDet;
    fT *= fInvDet;
    fU *= fInvDet;
    fV *= fInvDet;

	// t가 클수록 멀리 직선과 평면과 만나는 점이 멀다.
	// t*dir + orig 를 구하면 만나는 점을 구할 수 있다.
	// u와 v의 의미는 무엇일까?
	// 추측 : v0 (0,0), v1(1,0), v2(0,1) <괄호안은 (U, V)좌표> 이런식으로 어느 점에 가깝나 나타낸 것 같음
	//

	if(pVCol) *pVCol = vOrig + (vDir * fT);	// 접점을 계산..

	// *t < 0 이면 뒤쪽...
	if ( fT < 0.0f )
		return FALSE;

	return TRUE;
}

inline bool IntersectTriangle(__Vector3& vOrig, __Vector3& vDir, __Vector3& v0, __Vector3& v1, __Vector3& v2)
{
    // Find vectors for two edges sharing vert0
    // Begin calculating determinant - also used to calculate U parameter
    static float fDet, fT, fU, fV;
    static __Vector3 vEdge1, vEdge2, tVec, pVec, qVec;
	
	vEdge1 = v1 - v0;
    vEdge2 = v2 - v0;

	
//	By : Ecli666 ( On 2001-09-12 오전 10:39:01 )

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return FALSE;

//	~(By Ecli666 On 2001-09-12 오전 10:39:01 )

    pVec.Cross(vDir, vEdge2);

    // If determinant is near zero, ray lies in plane of triangle
    fDet = vEdge1.Dot(pVec);
    if( fDet < 0.0001f )		// 거의 0에 가까우면 삼각형 평면과 지나가는 선이 평행하다.
        return FALSE;

    // Calculate distance from vert0 to ray origin
    tVec = vOrig - v0;

    // Calculate U parameter and test bounds
    fU = tVec.Dot(pVec);
    if( fU < 0.0f || fU > fDet )
        return FALSE;

    // Prepare to test V parameter
    qVec.Cross(tVec, vEdge1);

    // Calculate V parameter and test bounds
    fV = D3DXVec3Dot( &vDir, &qVec );
    if( fV < 0.0f || fU + fV > fDet )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    fT = D3DXVec3Dot( &vEdge2, &qVec ) / fDet;

	// *t < 0 이면 뒤쪽...
	if ( fT < 0.0f )
		return FALSE;

	return TRUE;
}

inline POINT Convert3D_To_2DCoordinate(__Vector3 &vPos, __Matrix44& mtxView, __Matrix44& mtxProjection, int nVPW, int nVPH)
{
	__Matrix44 matVP;
	D3DXMatrixMultiply(&matVP, &mtxView, &mtxProjection);
	D3DXVECTOR4 v;
	D3DXVec3Transform(&v, (D3DXVECTOR3*)(&vPos), &matVP);

	POINT pt;
	float fScreenZ = (v.z/v.w);
	if (fScreenZ>1.0 || fScreenZ<0.0)
	{
		pt.x = -1;
		pt.y = -1;
		return pt;
	}

	pt.x = int(((v.x/v.w)+1.0f)*(nVPW)/2.0f);
	pt.y = int((1.0f-(v.y/v.w))*(nVPH)/2.0f);

	return pt;
}
