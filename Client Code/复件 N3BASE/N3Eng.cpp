// N3Eng.cpp: implementation of the CN3Eng class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Eng.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//int CN3Eng::m_nQtD3DDevice = 0;
float __sfSs[3600]; // sine
float __sfCs[3600]; // consine
float __sfTs[3600]; // tangent

__Vector3 __tempV; // Vector, Matrix 계산시 쓰는 임시 변수
__Matrix44 __tempM; // Vector, Matrix 계산시 쓰는 임시 변수

CN3Eng::CN3Eng()
{
	s_lpD3DDev = NULL;
	memset(&m_DeviceInfo, 0, sizeof(__D3DDEV_INFO));
	s_hWndDevice = NULL;

	// Direct3D 생성
	m_lpD3D = NULL;
	m_lpD3D = Direct3DCreate8(D3D_SDK_VERSION);
	if(NULL == m_lpD3D)
	{
		HWND hWnd = ::GetActiveWindow();
		MessageBox(hWnd, "Direct3D8 is not installed or incorrect.", "Initialization error", MB_OK);
		this->Release();
		exit(-1);
	}

	// 삼각 함수 테이블 초기화..
	::InitTrigonometricFunction();

	this->Release();
}

CN3Eng::~CN3Eng()
{
	CN3Base::ReleaseResrc(); 

	this->ReleaseEnv();
	CN3Eng::Release();

	if(m_lpD3D) if(0 == m_lpD3D->Release()) m_lpD3D = NULL;
}

void CN3Eng::ReleaseEnv()
{
	if(s_hWndDevice) ::DestroyWindow(s_hWndDevice);
	s_hWndDevice = NULL;
}

void CN3Eng::Release()
{
	m_nModeActive = -1;
	m_nAdapterCount = 1; // 그래픽 카드 갯수

	delete [] m_DeviceInfo.pModes;
	memset(&m_DeviceInfo, 0, sizeof(m_DeviceInfo));

	if(s_lpD3DDev)
	{
		if(0 == s_lpD3DDev->Release()) s_lpD3DDev = NULL;
		else
		{
			MessageBox(s_hWndDevice, "참조 계수가 0 이상입니다.", "Device Release 경고", MB_OK);
		}
	}
}

// Direct3D 생성 및 그래픽 카드 갯수, 모드 갯수 등 정보를 알아둔다.
void CN3Eng::InitEnv()
{
	//////////////////////////////////////////////////////////////////
	// 임시 윈도우 생성.. 없애진 않는다..
	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,  GetModuleHandle(NULL),
		NULL, NULL, NULL, NULL, "Noah D3D Device Window", NULL };
	RegisterClassEx( &wc );
	// Create the application's window
	s_hWndDevice = ::CreateWindow("Noah D3D Device Window", NULL,
		WS_POPUP, 0, 0, 1024, 768,
		NULL, NULL, wc.hInstance, 0L );
}

bool CN3Eng::Init(BOOL bWindowed, HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bUseHW)
{
	memset(&s_ResrcInfo, 0, sizeof(CN3Base::__ResrcInfo)); // Rendering Information 초기화..

	if(NULL == s_hWndDevice) return false;

	s_hWndBase = hWnd;

	int nAMC = m_lpD3D->GetAdapterModeCount(0); // 디스플레이 모드 카운트
	m_DeviceInfo.nAdapter = 0;
	m_DeviceInfo.DevType = D3DDEVTYPE_HAL;
	m_DeviceInfo.nDevice = 0;
	m_DeviceInfo.nModeCount = nAMC;
	m_DeviceInfo.pModes = new D3DDISPLAYMODE[nAMC];
	for(int i = 0; i < nAMC; i++)
	{
		m_lpD3D->EnumAdapterModes(0, i, &m_DeviceInfo.pModes[i]); // 디스플레이 모드 가져오기..
	}

	D3DDEVTYPE DevType = D3DDEVTYPE_REF;
	if(TRUE == bUseHW) DevType = D3DDEVTYPE_HAL;

	memset(&s_DevParam, 0, sizeof(s_DevParam));
	s_DevParam.Windowed = bWindowed;
	s_DevParam.hDeviceWindow = s_hWndDevice;
	s_DevParam.EnableAutoDepthStencil = TRUE;
	s_DevParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	s_DevParam.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
	if(TRUE == bWindowed) // 윈도우 모드일 경우
	{
		D3DDISPLAYMODE dm;
		m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
		s_DevParam.BackBufferCount = 1;
		if(dwWidth <= 0) dwWidth = dm.Width;
		if(dwHeight <= 0) dwHeight = dm.Height;
		BBFormat = dm.Format;
	}
	else
	{
		s_DevParam.BackBufferCount = 1;
		s_DevParam.AutoDepthStencilFormat = D3DFMT_D16; // 자동 생성이면 무시된다.
		if(16 == dwBPP) BBFormat = D3DFMT_R5G6B5;
		else if(24 == dwBPP) BBFormat = D3DFMT_R8G8B8;
		else if(32 == dwBPP) BBFormat = D3DFMT_X8R8G8B8;
	}

	s_DevParam.BackBufferWidth = dwWidth;
	s_DevParam.BackBufferHeight = dwHeight;
	s_DevParam.BackBufferFormat = BBFormat;
	s_DevParam.MultiSampleType = D3DMULTISAMPLE_NONE; // Swap Effect 가 Discard 형태가 아니면 반드시 이런 식이어야 한다.
	s_DevParam.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	int nMC = m_DeviceInfo.nModeCount;
	for(i = 0; i < nMC; i++)
	{
//		if(	m_DeviceInfo.pModes[i].Width == dwWidth && 
//			m_DeviceInfo.pModes[i].Height == dwHeight && 
		if(	m_DeviceInfo.pModes[i].Format == BBFormat) // 모드가 일치하면
		{
			this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format, &s_DevParam.AutoDepthStencilFormat); // 깊이와 스텐실 버퍼를 찾는다.
			m_nModeActive = i;
			break;
		}
	}

	HRESULT rval = 0;
	rval = m_lpD3D->CreateDevice(0, DevType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
	if(rval != D3D_OK)
	{
		rval = m_lpD3D->CreateDevice(0, DevType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
		if(rval != D3D_OK)
		{
			char szDebug[256];
			D3DXGetErrorString(rval, szDebug, 256);
			MessageBox(hWnd, szDebug, "Direct3D Device 생성 실패", MB_OK);
			this->Release();
			return false;
		}
#if _DEBUG
		OutputDebugString("HardWare TnL 이 지원되지 않습니다. SoftWare TnL 로 진행 합니다.\n");
#endif
	}

	// Device 지원 항목은??
	s_lpD3DDev->GetDeviceCaps(&s_DevCaps);
	// DXT 지원 여부..
	s_dwTextureCaps = 0;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, s_DevCaps.DeviceType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1)) s_dwTextureCaps |= TEX_CAPS_DXT1;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, s_DevCaps.DeviceType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT2)) s_dwTextureCaps |= TEX_CAPS_DXT2;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, s_DevCaps.DeviceType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3)) s_dwTextureCaps |= TEX_CAPS_DXT3;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, s_DevCaps.DeviceType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT4)) s_dwTextureCaps |= TEX_CAPS_DXT4;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, s_DevCaps.DeviceType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5)) s_dwTextureCaps |= TEX_CAPS_DXT5;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) s_dwTextureCaps |= TEX_CAPS_SQUAREONLY;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) s_dwTextureCaps |= TEX_CAPS_MIPMAP;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_POW2) s_dwTextureCaps |= TEX_CAPS_POW2;

	// Device 윈도우 크기 지정.
	::SetWindowPos(s_hWndDevice, NULL, 0, 0, dwWidth, dwHeight, SWP_NOZORDER);

	// 기본 라이트 정보 지정..
	for(i = 0; i < 8; i++)
	{
		CN3Light::__Light Lgt;
		_D3DCOLORVALUE LgtColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Lgt.InitPoint(__Vector3(0,0,0), LgtColor);
		s_lpD3DDev->SetLight(i, &Lgt);
	}

	// 기본 뷰와 프로젝션 설정.
	this->LookAt(__Vector3(5,5,-10), __Vector3(0,0,0), __Vector3(0,1,0));
	this->SetProjection(0.1f, 256.0f, D3DXToRadian(45.0f), (float)dwHeight/dwWidth);
	
	RECT rcView = { 0, 0, dwWidth, dwHeight };
	this->SetViewPort(rcView);

	this->SetDefaultEnvironment(); // 기본 상태로 설정..

	return true;
}

void CN3Eng::LookAt(__Vector3 &vEye, __Vector3 &vAt, __Vector3 &vUp)
{
	__Matrix44 matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	s_lpD3DDev->SetTransform(D3DTS_VIEW, &matView);
}

void CN3Eng::SetProjection(float fNear, float fFar, float fLens, float fAspect)
{
	__Matrix44 matProjection;
	::D3DXMatrixPerspectiveFovLH(&matProjection, fLens, fAspect, fNear, fFar);
	s_lpD3DDev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void CN3Eng::SetViewPort(RECT& rc)
{
	if(NULL == s_lpD3DDev) return;
	D3DVIEWPORT8 vp;

	vp.X = rc.left;
	vp.Y = rc.top;
	vp.Width = rc.right - rc.left;
	vp.Height = rc.bottom - rc.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	s_lpD3DDev->SetViewport(&vp);
	memcpy(&s_CameraData.vp, &vp, sizeof(D3DVIEWPORT8));
}

LRESULT WINAPI CN3Eng::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
//          PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
//          Render();
//          ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL CN3Eng::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat)
{
	int nDSC = 6;
	D3DFORMAT DepthFmts[] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D16, D3DFMT_D15S1};

	HRESULT rval = 0;
	for(int i = 0; i < nDSC; i++)
	{
		rval = m_lpD3D->CheckDeviceFormat(iAdapter, DeviceType, TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, DepthFmts[i]);
		if(D3D_OK == rval)
		{
			rval = m_lpD3D->CheckDepthStencilMatch(iAdapter, DeviceType, TargetFormat, TargetFormat, DepthFmts[i]);
			if(D3D_OK == rval)
			{
				*pDepthStencilFormat = DepthFmts[i];
				return TRUE;
			}
		}
	}

    return FALSE;
}
void CN3Eng::Present(HWND hWnd, RECT* pRC)
{
	CN3Base::TimerProcess();

//	HRESULT rval = s_lpD3DDev->TestCooperativeLevel();
//	if(D3D_OK != rval)
//	{
//#if _DEBUG
//		char szDebug[256];
//		D3DXGetErrorString(rval, szDebug, 256);
//#endif
//		return;
//	}

	if(this->s_DevParam.Windowed) // 윈도우 모드면...
	{
		RECT rc;
		GetClientRect(s_hWndBase, &rc);
		pRC = &rc;
	}

	HRESULT rval = s_lpD3DDev->Present(pRC, pRC, hWnd, NULL);
	if(D3D_OK == rval)
	{
		s_hWndPresent = hWnd; // Present window handle 을 저장해 놓는다.
	}
//	else if(D3DERR_DEVICELOST == rval)
//	{
//		Beep(2000, 200);
//		rval = s_lpD3DDev->Reset(&s_DevParam);
//		return;
//	}
//	else if(D3DERR_DEVICENOTRESET == rval) 
//	{
//		Beep(3000, 100);
//		Sleep(100);
//		Beep(3000, 100);
//		rval = s_lpD3DDev->Reset(&s_DevParam);
//	}
#if _DEBUG
	else
	{
		char szDebug[256];
		D3DXGetErrorString(rval, szDebug, 256);
		Beep(5000, 50);
		Sleep(50);
		Beep(5000, 50);
		Sleep(50);
		Beep(5000, 50);
		Sleep(50);
	}
#endif
}

void CN3Eng::Clear(D3DCOLOR crFill, RECT* pRC)
{
	if(NULL == pRC && s_DevParam.Windowed) // 윈도우 모드면...
	{
		RECT rc;
		GetClientRect(s_hWndBase, &rc);
		pRC = &rc;
	}

	if(pRC)
	{
		_D3DRECT rc3D = { pRC->left, pRC->top, pRC->right, pRC->bottom };
		s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_TARGET, crFill, 1.0f, 0);
	}
	else
	{
		s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		s_lpD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, crFill, 1.0f, 0);
	}

	memset(&s_RenderInfo, 0, sizeof(CN3Base::__RenderInfo));
}

void CN3Eng::ClearAuto(RECT* pRC)
{
	DWORD dwFillColor = D3DCOLOR_ARGB(255,192,192,192); // 기본색
	DWORD dwUseFog = FALSE;
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwUseFog); // 안개를 쓰면 바탕색을 안개색을 깔아준다..
	if(dwUseFog != 0) s_lpD3DDev->GetRenderState(D3DRS_FOGCOLOR, &dwFillColor);
	else
	{
		CN3Light::__Light Lgt;

		BOOL bEnable;
		s_lpD3DDev->GetLightEnable(0, &bEnable);
		if(bEnable)
		{
			s_lpD3DDev->GetLight(0, &Lgt);
			dwFillColor = D3DCOLOR_ARGB((BYTE)(Lgt.Diffuse.a * 255.0f), (BYTE)(Lgt.Diffuse.r * 255.0f), (BYTE)(Lgt.Diffuse.g * 255.0f), (BYTE)(Lgt.Diffuse.b * 255.0f));
		}
	}

	this->Clear(dwFillColor, pRC);
}

void CN3Eng::ClearZBuffer(RECT* pRC)
{
	if(this->s_DevParam.Windowed) // 윈도우 모드면...
	{
		RECT rc;
		GetClientRect(s_hWndBase, &rc);
		pRC = &rc;
	}

	if(pRC) 
	{
		D3DRECT rc3D = { pRC->left, pRC->top, pRC->right, pRC->bottom };
		s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	}
	else
	{
		s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	}
}

bool CN3Eng::Reset(BOOL bWindowed, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
	if(NULL == s_lpD3DDev) return false;
	if(dwWidth <= 0 || dwHeight <= 0) return false;

	D3DPRESENT_PARAMETERS DevParamBackUp; // Baclup
	memcpy(&DevParamBackUp, &s_DevParam, sizeof(D3DPRESENT_PARAMETERS));

	D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
	if(bWindowed)
	{
		D3DDISPLAYMODE dm;
		m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
		BBFormat = dm.Format;
	}
	else
	{
		if(16 == dwBPP) BBFormat = D3DFMT_R5G6B5;
		else if(24 == dwBPP) BBFormat = D3DFMT_R8G8B8;
		else if(32 == dwBPP) BBFormat = D3DFMT_X8R8G8B8;
	}

	s_DevParam.Windowed = bWindowed;
	s_DevParam.BackBufferWidth = dwWidth;
	s_DevParam.BackBufferHeight = dwHeight;
	s_DevParam.BackBufferFormat = BBFormat;

	int nMC = m_DeviceInfo.nModeCount;
	for(int i = 0; i < nMC; i++)
	{
//		if(	m_DeviceInfo.pModes[i].Width == dwWidth && 
//			m_DeviceInfo.pModes[i].Height == dwHeight && 
		if(	m_DeviceInfo.pModes[i].Format == s_DevParam.BackBufferFormat) // 모드가 일치하면
		{
			this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format, &s_DevParam.AutoDepthStencilFormat); // 깊이와 스텐실 버퍼를 찾는다.
			m_nModeActive = i;
			break;
		}
	}

	if(D3D_OK != s_lpD3DDev->Reset(&s_DevParam))
	{
		memcpy(&s_DevParam, &DevParamBackUp, sizeof(D3DPRESENT_PARAMETERS));
		if(D3D_OK != s_lpD3DDev->Reset(&s_DevParam))
			exit(-1); // 끝장낸다..

		MessageBox(s_hWndBase, "Insufficient video memory", "Device reset failed.", MB_OK); 
		return false;
	}

	RECT rcView = { 0, 0, dwWidth, dwHeight };
	this->SetViewPort(rcView);

	this->SetDefaultEnvironment();

	return true;
}

void CN3Eng::SetDefaultEnvironment()
{
	// 기본 렌더링 상태 지정
//	s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW); // Z버퍼 사용가능
	s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); // Z버퍼 사용가능
	s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    s_lpD3DDev->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
    s_lpD3DDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
//    s_lpD3DDev->SetRenderState( D3DRS_AMBIENT,        0x00444444 );

	s_lpD3DDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	s_lpD3DDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기본 알파 펑션 - 안해주면 알파 텍스처들이 빵꾸나기도 한다.
	
	s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 기본 텍스처 필터 지정.
	float fMipMapLODBias = -1.0f;
	for(int i = 0; i < 8; i++)
	{
		s_lpD3DDev->SetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fMipMapLODBias)));
	}

	// 클리핑 상태 지정
	D3DCLIPSTATUS8 cs; cs.ClipUnion = cs.ClipIntersection = D3DCS_ALL;
	s_lpD3DDev->SetClipStatus(&cs);	
}

HKEY CN3Eng::RegistryOpen(const char *szKey)
{
    HKEY hKey = NULL;
	long lStatus = RegOpenKey(HKEY_CURRENT_USER, szKey, &hKey);

	return hKey;
}

bool CN3Eng::RegistryClose(HKEY& hKey)
{
	if(NULL == hKey) return false;

	long lStatus = RegCloseKey(hKey);
	hKey = NULL;

	if(ERROR_SUCCESS == lStatus) return true;
	return false;
}

bool CN3Eng::RegistryValueGet(HKEY hKey, const char* szName, char* szValue)
{
	if(NULL == hKey || lstrlen(szName) <= 0 || lstrlen(szValue) <= 0) return false;

	DWORD dwType = REG_SZ;
	DWORD dwBytes = 0;
	long lStatus = RegQueryValueEx(hKey, szName, NULL, &dwType, (BYTE*)szValue, &dwBytes);

	if(ERROR_SUCCESS == lStatus) return true;
	return false;
}

bool CN3Eng::RegistryValueGet(HKEY hKey, const char* szName, void* pValue, DWORD dwBytes)
{
	if(NULL == hKey || lstrlen(szName) <= 0 || NULL == pValue || 0 == dwBytes) return false;

	DWORD dwType = REG_BINARY;
	long lStatus = RegQueryValueEx(hKey, szName, NULL, &dwType, (BYTE*)pValue, &dwBytes);

	if(ERROR_SUCCESS == lStatus) return true;
	return false;
}

bool CN3Eng::RegistryValueSet(HKEY hKey, const char* szName, char* szValue)
{
	if(NULL == hKey || lstrlen(szName) <= 0 || lstrlen(szValue) <= 0) return false;

	DWORD dwBytes = lstrlen(szValue);
	long lStatus = RegSetValueEx(hKey, szName, NULL, REG_SZ, (BYTE*)szValue, dwBytes);

	if(ERROR_SUCCESS == lStatus) return true;
	return false;
}

bool CN3Eng::RegistryValueSet(HKEY hKey, const char* szName, void* pValue, DWORD dwBytes)
{
	if(NULL == hKey || lstrlen(szName) <= 0 || NULL == pValue || 0 == dwBytes) return false;

	long lStatus = RegSetValueEx(hKey, szName, NULL, REG_BINARY, (BYTE*)pValue, dwBytes);

	if(ERROR_SUCCESS == lStatus) return true;
	return false;
}

