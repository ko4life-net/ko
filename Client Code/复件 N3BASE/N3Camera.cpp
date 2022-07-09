// N3Camera.cpp: implementation of the CN3Camera class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Camera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CN3Camera::CN3Camera()
{
	m_dwType |= OBJ_CAMERA;

	this->Release();
	m_fRadianX = 0.0f;
}

CN3Camera::~CN3Camera()
{
	CN3Camera::Release();
	m_fRadianX = 0.0f;
}

void CN3Camera::Release()
{
	CN3Transform::Release();

	memset(&m_Data, 0, sizeof(m_Data));

	m_Data.vEye = m_vPos = __Vector3(15,5,-15);
	m_Data.vAt = m_vAt = __Vector3(0,0,0);
	m_Data.vUp = m_vScale = __Vector3(0,1,0);

	m_Data.fFOV = D3DXToRadian(72.0f); // 기본값 72 도
	m_Data.fNP = 0.3f;
	m_Data.fFP = 256.0f;

	m_bFogUse = FALSE;
	m_fFogDensity = 0.008f * (256.0f / m_Data.fFP);
	m_fFogStart = m_Data.fFP / 2;
	m_fFogEnd = m_Data.fFP;
	m_FogColor = D3DCOLOR_ARGB(255,255,255,255);
}

bool CN3Camera::Load(HANDLE hFile)
{
	CN3Transform::Load(hFile);

	DWORD dwRWC = 0;
	ReadFile(hFile, &m_vAt, sizeof(__Vector3), &dwRWC, NULL); // At position
	ReadFile(hFile, &m_Data, sizeof(__CameraData), &dwRWC, NULL); // CameraData
	ReadFile(hFile, &m_bFogUse, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fFogDensity, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_FogColor, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fFogStart, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fFogEnd, 4, &dwRWC, NULL);

	return true;
}

bool CN3Camera::Save(HANDLE hFile)
{
	CN3Transform::Save(hFile);

	DWORD dwRWC = 0;
	WriteFile(hFile, &m_vAt, sizeof(__Vector3), &dwRWC, NULL); // At position
	WriteFile(hFile, &m_Data, sizeof(__CameraData), &dwRWC, NULL); // CameraData
	WriteFile(hFile, &m_bFogUse, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fFogDensity, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_FogColor, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fFogStart, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fFogEnd, 4, &dwRWC, NULL);

	return true;
}
void CN3Camera::Zoom(float fDelta)
{
	__Vector3 vD = m_vAt - m_vPos;

	float fD2 = sqrtf(vD.Magnitude());
	fDelta *= fD2 / 5.0f;

	m_vPos += vD * fDelta;
}

void CN3Camera::LookAround(float fRadianX, float fRadianY)		//At Postion을 중심으로 카메라가 돈다..고로 위치가 바뀐다..
{
	//static __Matrix44 mtx;
	//static __Vector3 v1, v2;
	//static __Quaternion qt;
	
	__Matrix44 mtx;		//by lynus...
	__Vector3 v1, v2;
	__Quaternion qt;

	v1 = m_vPos - m_vAt;
	mtx.RotationY(fRadianY);
	v1 *= mtx;
	m_vScale *= mtx;

	v2.Cross(v1, m_vScale);
	qt.RotationAxis(v2, fRadianX);
	mtx = qt;
	
	v1 *= mtx;
	m_vScale *= mtx;	//by lynus...
/*
#ifdef _N3_CLIENT_
	m_vScale *= mtx;
#endif
*/
	m_vPos = m_vAt + v1; // Rotation 은 LookAt Position 처럼, Scale 은 UpVector 처럼 쓴다..
}

void CN3Camera::Rotate(float fRadianX, float fRadianY)
{
	//static __Matrix44 mtx;
	__Matrix44 mtx;		//by lynus...

#ifdef _N3_CLIENT_
	m_fRadianX += fRadianX;
	if ( m_fRadianX < -D3DXToRadian(60.0f) )
	{
		m_fRadianX  -= fRadianX;
		return;
	}
	if ( m_fRadianX > D3DXToRadian(80.0f) )
	{
		m_fRadianX  -= fRadianX;
		return;
	}
#endif

	mtx.RotationY(fRadianY);

	m_v1 = m_vAt - m_vPos; // Rotation 은 LookAt Position 처럼, Scale 은 UpVector 처럼 쓴다..
	m_v1 *= mtx;
	m_vScale *= mtx;

	m_v2.Cross(m_v1, m_vScale);
	m_qt.RotationAxis(m_v2, fRadianX);
	mtx = m_qt;
	
	m_v1 *= mtx;
	m_vScale *= mtx;	//by lynus...
/*
#ifndef _N3_CLIENT_
	m_vScale *= mtx;
#endif
*/
	m_vAt = m_vPos + m_v1;
}

void CN3Camera::Move(__Vector3& vDelta)
{
	m_vPos += vDelta;
	m_vAt += vDelta;
}

void CN3Camera::MoveStraight(float fDistance)
{
	__Vector3 vDelta = m_vAt - m_vPos;
	vDelta.Normalize();
	m_vPos += vDelta * fDistance;
}

void CN3Camera::MovePlane(float fX, float fY)
{
	__Vector3 vDir = m_vAt - m_vPos; // Rotation 은 LookAt Position 처럼, Scale 은 UpVector 처럼 쓴다..
	vDir.Normalize();

	__Vector3 vHoriz;
	vHoriz.Cross(m_vScale, vDir);
	vHoriz.Normalize();
	
	__Vector3 vDown;
	vDown.Cross(vHoriz, vDir);
	m_vScale = vDown*(-1);

	__Vector3 vMove = vHoriz*fX + vDown*fY;
	m_vPos += vMove;
	m_vAt += vMove; // Rotation 은 LookAt Position 처럼, Scale 은 UpVector 처럼 쓴다..
}

void CN3Camera::Apply()
{
	s_lpD3DDev->SetTransform(D3DTS_VIEW, &m_Data.mtxView);
	s_lpD3DDev->SetTransform(D3DTS_PROJECTION, &m_Data.mtxProjection); // Projection Matrix Setting
	memcpy(&(CN3Base::s_CameraData), &m_Data, sizeof(CN3Base::__CameraData)); // Static Data Update...

	// 안개 색깔 맞추기..
	s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, m_bFogUse);
	s_lpD3DDev->SetRenderState( D3DRS_FOGCOLOR,  m_FogColor);
	s_lpD3DDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_EXP2);
	s_lpD3DDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_EXP2);
	s_lpD3DDev->SetRenderState( D3DRS_FOGSTART,   *(DWORD*)&m_fFogStart);
	s_lpD3DDev->SetRenderState( D3DRS_FOGEND,     *(DWORD*)&m_fFogEnd);
	s_lpD3DDev->SetRenderState( D3DRS_FOGDENSITY, *(DWORD*)&m_fFogDensity);
}

void CN3Camera::Render(float fUnitSize)
{
	static __VertexColor vFrustums[8];
	static WORD wIndices[24] = {	0, 4, 1, 5, 2, 6, 3, 7, // Frustum
									0, 1, 1, 2, 2, 3, 3, 0, // Near
									4, 5, 5, 6, 6, 7, 7, 4 }; // Far
	
	__Vector3 vN;
	D3DCOLOR crLine = 0xff00ff00;

	for(int i = 0; i < 4; i++)
	{
		vN = m_Data.vFarPoints[i] - m_vPos; vN.Normalize();
		vFrustums[i].Set(m_vPos + (vN * m_Data.fNP), crLine);
		vFrustums[i+4].Set(m_Data.vFarPoints[i], crLine);
	}


	DWORD dwAlpha, dwFog, dwLight; // , dwZ;
//	s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZ);
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	
//	if(dwZ) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	__Matrix44 mtxWorld; mtxWorld.Identity();
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	__Material mtl; mtl.Init(); // 흰색..
	s_lpD3DDev->SetMaterial(&mtl);
	s_lpD3DDev->SetTexture(0, NULL);

	s_lpD3DDev->SetVertexShader(FVF_XYZCOLOR);
	s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 24, 12, wIndices, D3DFMT_INDEX16, vFrustums, sizeof(__VertexColor)); // 선그리기..

//	if(dwZ) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZ);
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CN3Camera::Tick(float fFrm)
{
	CN3Transform::Tick(fFrm);

	////////////////////////////////////////////////////////////////////////
	// View Matrix 및 Projection Matrix Setting
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 	__Vector3 m_vEye, m_vUp ->> m_vPos, m_vScale 로 대신한다.. 중요!!
	m_Data.vEye = m_vPos;
	m_Data.vAt  = m_vAt;
	m_Data.vUp  = m_vScale; // Up Vector 처럼 쓴다.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	::D3DXMatrixLookAtLH(&m_Data.mtxView, &m_Data.vEye, &m_Data.vAt, &m_Data.vUp); // Look At 적용
	::D3DXMatrixInverse(&m_Data.mtxViewInverse, NULL, &m_Data.mtxView); // View Inverse 행렬 구하기..
	CN3Base::s_lpD3DDev->GetViewport(&m_Data.vp); // View port 가져오기...
	m_Data.fAspect = (float)m_Data.vp.Width / (float)m_Data.vp.Height; // 종횡비
	::D3DXMatrixPerspectiveFovLH(&m_Data.mtxProjection, m_Data.fFOV, m_Data.fAspect, m_Data.fNP, m_Data.fFP); // Projection Matrix Setting

	// 카메라 회전각 행렬 구하기..
	static __Matrix44 mtxRot;
	mtxRot = m_Data.mtxViewInverse;
	mtxRot.PosSet(0,0,0);

	// 사면체의 법선 벡터와 Far 네 귀퉁이 위치 계산..
	float fC = __Cosine(m_Data.fFOV / 2.0f);
	float fS = __Sine(m_Data.fFOV / 2.0f);
	float fPL = m_Data.fFP;
	
	// Far Plane 의 네 귀퉁이 위치 계산
	__Vector3 vFPs[4] = {	__Vector3(fPL * -fS * m_Data.fAspect, fPL * fS, fPL),	// LeftTop
							__Vector3(fPL * fS * m_Data.fAspect, fPL * fS, fPL),		// rightTop
							__Vector3(fPL * fS * m_Data.fAspect, fPL * -fS, fPL),	// RightBottom
							__Vector3(fPL * -fS * m_Data.fAspect, fPL * -fS, fPL) }; // LeftBottom
//	__Vector3 vFPs[4] = {	__Vector3(fPL * -fS, fPL * fS / m_Data.fAspect, fPL),	// LeftTop
//							__Vector3(fPL * fS, fPL * fS / m_Data.fAspect, fPL),		// rightTop
//							__Vector3(fPL * fS, fPL * -fS / m_Data.fAspect, fPL),	// RightBottom
//							__Vector3(fPL * -fS, fPL * -fS / m_Data.fAspect, fPL) }; // LeftBottom
	// Frustom 의 Front, Left, Top, Right, Bottom 를 제외한 4면의 법선 벡터 계산..
	__Vector3 vFNs[4] = {	__Vector3( fS,   0, fC),		// Left
							__Vector3(-fS,   0, fC),		// Right
							__Vector3( 0, -fC,  fS),		// Top
							__Vector3( 0,  fC,  fS) };		// Bottom

	//

	/*
	// 사면체의 법선 벡터와 Far 네 귀퉁이 위치 계산.. by lynus...2001. 9.20
	//
	float fT = tanf(m_Data.fFOV / 2.0f);
	float fPL = m_Data.fFP;

	float fHalfWidth = fT * fPL;
	float fHalfHeight = fHalfWidth / m_Data.fAspect;
	
	// Far Plane 의 네 귀퉁이 위치 계산
	__Vector3 vFPs[4] = {	__Vector3( -fHalfWidth, fHalfHeight, fPL),	// LeftTop
							__Vector3( fHalfWidth, fHalfHeight, fPL),		// rightTop
							__Vector3( fHalfWidth, -fHalfHeight, fPL),	// RightBottom
							__Vector3( -fHalfWidth, -fHalfHeight, fPL)
						}; // LeftBottom
	// Frustom 의 Front, Left, Top, Right, Bottom 를 제외한 4면의 법선 벡터 계산..
	float l = 1.0f / fT;	// game programming gems (p492..)
	float a = 1.0f / m_Data.fAspect;
	float l2plus1 = 2.0f / ( 1.0f - cosf(m_Data.fFOV)); // l^2 + 1..... ( l = 1/tan(FOV/2) )

	float e1 = 1.0f / (float)sqrt(l2plus1) ; // 1 / root(l^2 + 1)
	float e2 = e1 * l;
	float e3 = l / (float)sqrt((l2plus1-1+pow(a, 2)));	//( l / root(l^2 + a^2) )
	float e4 = a / (float)sqrt((l2plus1-1+pow(a, 2)));	//( a / root(l^2 + a^2) )
	
	__Vector3 vFNs[4] = {	__Vector3( e2, 0, e1),		// Left
							__Vector3( -e2, 0, e1),		// Right
							__Vector3( 0, -e3, e4),		// Top
							__Vector3( 0,  e3, e4) };		// Bottom

	//
	////////////////////////////////////////////////// end of lynus's codes...
	*/

	// 법선벡터와 귀퉁이 위치에 회전 행렬을 적용한다..
	m_Data.vFrustumNormals[0] = m_Data.vAt - m_Data.vEye;
	m_Data.vFrustumNormals[0].Normalize();
	for(int i = 0; i < 4; i++)
	{
		m_Data.vFrustumNormals[i+1] = vFNs[i] * mtxRot;
		m_Data.vFarPoints[i] = vFPs[i] * m_Data.mtxViewInverse;
	}

	static __Vector3 vEyeTmp;
	for(i = 0; i < MAX_CAMERA_RADIUS; i++)
	{
		vEyeTmp = m_vPos;
		vEyeTmp -= (m_Data.vFrustumNormals[0]) * (i * 3.0f); // 카메라 위치를 10.0 m 뒤로 뺀다. -> Shape 클리핑에 쓴다..

		m_Data.fEyeDotValues[i][0] = vEyeTmp.Dot(m_Data.vFrustumNormals[0]);
		m_Data.fEyeDotValues[i][1] = vEyeTmp.Dot(m_Data.vFrustumNormals[1]);
		m_Data.fEyeDotValues[i][2] = vEyeTmp.Dot(m_Data.vFrustumNormals[2]);
		m_Data.fEyeDotValues[i][3] = vEyeTmp.Dot(m_Data.vFrustumNormals[3]);
		m_Data.fEyeDotValues[i][4] = vEyeTmp.Dot(m_Data.vFrustumNormals[4]);
	}
	// Static Camera Data 계산...
	////////////////////////////////////////////////////////////////////////
}

BOOL CN3Camera::MoveByWindowMessage(MSG* pMsg)
{
	static int iButtonDownCount = 0;
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		//short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		short zDelta = (short)((pMsg->wParam>>16)&0x0000ffff);
		float fD = (m_vPos - m_vAt).Magnitude();
		this->MoveStraight(fD * zDelta * 0.001f);
		this->Apply();
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_UP: { __Vector3 vDir = m_vAt - m_vPos; vDir.Normalize(); this->Move(vDir*10.0f); this->Apply(); return TRUE; }
			break;
		case VK_DOWN: { __Vector3 vDir = m_vAt - m_vPos; vDir.Normalize(); this->Move(vDir*-10.0f); this->Apply(); return TRUE; }
			break;
		case VK_RIGHT: { this->Rotate(0, -0.1f); this->Apply(); return TRUE; }
			break;
		case VK_LEFT: { this->Rotate(0, 0.1f); this->Apply(); return TRUE; }
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			{
				static __Vector3 vEyes[10] = { m_vPos, m_vPos, m_vPos, m_vPos, m_vPos, m_vPos, m_vPos, m_vPos, m_vPos, m_vPos };
				static __Vector3 vAts[10] = { m_vAt, m_vAt, m_vAt, m_vAt, m_vAt, m_vAt, m_vAt, m_vAt, m_vAt, m_vAt};
				static __Vector3 vUps[10] = { m_vScale, m_vScale, m_vScale, m_vScale, m_vScale, m_vScale, m_vScale, m_vScale, m_vScale, m_vScale };
				
				int nCamIndex = pMsg->wParam - '0';

				if (GetAsyncKeyState(VK_CONTROL) & 0xff00)
				{
					vEyes[nCamIndex] = m_vPos;
					vAts[nCamIndex] = m_vAt;
					vUps[nCamIndex] = m_vScale;
					return FALSE;
				}
				else
				{
					this->EyePosSet(vEyes[nCamIndex]);
					this->AtPosSet(vAts[nCamIndex]);
					this->UpVectorSet(vUps[nCamIndex]);
					this->Apply();
					return TRUE;
				}
			}
			break;
		}
	}
	else if(GetAsyncKeyState(VK_MENU) & 0xff00)
	{
		static POINT ptPrev;
	
		POINT point = { LOWORD(pMsg->lParam), HIWORD(pMsg->lParam) };
		POINT ptDelta = { point.x - ptPrev.x, point.y - ptPrev.y };

		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
//				iButtonDownCount++;
				::SetCapture(pMsg->hwnd);
				ptPrev.x = short(LOWORD(pMsg->lParam));
				ptPrev.y = short(HIWORD(pMsg->lParam));
				return TRUE;
			}
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			{
				iButtonDownCount--;
//				if (iButtonDownCount <= 0) 
					ReleaseCapture();
				return TRUE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				DWORD nFlags = pMsg->wParam;
				ptPrev = point;

				if(	(nFlags & MK_LBUTTON) && (nFlags & MK_MBUTTON) ) // Alt + LB + MB
				{
					float fZoom = (float)(ptDelta.x)/1000.0f;
					this->Zoom(fZoom);
					this->Apply();
					return TRUE;
				}
				else if((nFlags & MK_LBUTTON) ) // Alt + LB
				{
					float fRX = (float)(ptDelta.y)/200.0f;
					float fRY = -(float)(ptDelta.x)/200.0f;
					this->LookAround(fRX, fRY);
					this->Apply();
					return TRUE;
				}
				else if((nFlags & MK_RBUTTON) ) // Alt + RB
				{
					float fRX = (float)(ptDelta.y)/200.0f;
					float fRY = (float)(ptDelta.x)/200.0f;
					this->Rotate(fRX, -fRY);
					this->Apply();
					return TRUE;
				}
				else if((nFlags & MK_MBUTTON) ) // Alt + MB
				{
					__Vector3 vDelta = m_vPos - m_vAt;
					float fDelta = vDelta.Magnitude() * 0.002f / m_Data.fFOV;
					__Vector3 vD1;
					vD1.Set(-(float)(ptDelta.x), (float)(ptDelta.y), 0 );

					vD1 *= fDelta;

					this->MovePlane(vD1.x, -vD1.y);
					this->Apply();
					return TRUE;
				}
			}
		default:	// 마우스 메세지가 아닐경우 카메라 움직임이 아니다.
			return FALSE;
		}
	}

	return FALSE;
}
