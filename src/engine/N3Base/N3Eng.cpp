// N3Eng.cpp: implementation of the CN3Eng class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3Eng.h"
#include "N3Light.h"
#include "LogWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3EngCallbacks CN3Eng::m_CB;

CN3Eng::CN3Eng() {
    m_lpDD = NULL;
    s_lpD3DDev = NULL;
    memset(&m_DeviceInfo, 0, sizeof(__D3DDEV_INFO));

    m_nModeActive = -1;
    m_nAdapterCount = 1; // Number of graphics cards

    delete[] m_DeviceInfo.pModes;
    memset(&m_DeviceInfo, 0, sizeof(m_DeviceInfo));

    // Create Direct3D
    m_lpD3D = NULL;
    m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (NULL == m_lpD3D) {
        MessageBox(::GetActiveWindow(), "Direct3D9 is not installed or lower version.", "Initialization", MB_OK);
        // for (int iii = 0; iii < 1; iii++) { // Beep several times~
        //     Beep(2000, 200);
        //     Sleep(300);
        // }
#ifdef _N3GAME
        CLogWriter::Write("Direct3D9 is not installed or lower version");
#endif
        this->Release();
        exit(-1);
    }

    // Path where the program was executed..
    if (s_szPath.empty()) {
        char szPath[256];
        char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
        ::GetModuleFileName(NULL, szPath, 256);
        _splitpath(szPath, szDrive, szDir, NULL, NULL);
        sprintf(szPath, "%s%s", szDrive, szDir);
        this->PathSet(szPath); //Path settings..
    }

#ifdef _N3GAME
    CLogWriter::Open("Log.txt");
#endif
}

CN3Eng::~CN3Eng() {
    CN3Base::ReleaseResrc();

    delete[] m_DeviceInfo.pModes;

    if (s_lpD3DDev) {
        int nRefCount = s_lpD3DDev->Release();
        if (0 == nRefCount) {
            s_lpD3DDev = NULL;
        } else {
#ifdef _N3GAME
            CLogWriter::Write("CNEng::~CN3Eng - Device reference count is bigger than 0");
#endif
        }
    }
    if (m_lpD3D) {
        if (0 == m_lpD3D->Release()) {
            m_lpD3D = NULL;
        }
    }
    if (m_lpDD) {
        m_lpDD->Release();
    }
    m_lpDD = NULL;

#ifdef _N3GAME
    CLogWriter::Close();
#endif
}

void CN3Eng::Release() {
    m_nModeActive = -1;
    m_nAdapterCount = 1; // Number of graphics cards

    delete[] m_DeviceInfo.pModes;
    memset(&m_DeviceInfo, 0, sizeof(m_DeviceInfo));

    if (s_lpD3DDev) {
        int nRefCount = s_lpD3DDev->Release();
        if (0 == nRefCount) {
            s_lpD3DDev = NULL;
        } else {
#ifdef _N3GAME
            CLogWriter::Write("CNEng::Release Device reference count is bigger than 0");
#endif
        }
    }

    if (m_lpDD) {
        m_lpDD->Release();
    }
    m_lpDD = NULL;
}

bool CN3Eng::Init(BOOL bWindowed, HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bUseHW) {
    memset(&s_ResrcInfo, 0, sizeof(__ResrcInfo)); // Initialize Rendering Information..

    s_hWndBase = hWnd;

    int nAMC = m_lpD3D->GetAdapterModeCount(0, D3DFMT_X8R8G8B8); // display mode count
    if (nAMC <= 0) {
        MessageBox(hWnd, "Can't create D3D - Invalid display mode property.", "initialization", MB_OK);
//        { for(int iii = 0; iii < 2; iii++) Beep(2000, 200); Sleep(300); } // Beep several times~
#ifdef _N3GAME
        CLogWriter::Write("Can't create D3D - Invalid display mode property.");
#endif
        this->Release();
        return false;
    }

    m_DeviceInfo.nAdapter = 0;
    m_DeviceInfo.DevType = D3DDEVTYPE_HAL;
    m_DeviceInfo.nDevice = 0;
    m_DeviceInfo.nModeCount = nAMC;
    delete[] m_DeviceInfo.pModes;
    m_DeviceInfo.pModes = new D3DDISPLAYMODE[nAMC];
    for (int i = 0; i < nAMC; i++) {
        m_lpD3D->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &m_DeviceInfo.pModes[i]); // Get display mode..
    }

    D3DDEVTYPE DevType = D3DDEVTYPE_REF;
    if (TRUE == bUseHW) {
        DevType = D3DDEVTYPE_HAL;
    }

    memset(&s_DevParam, 0, sizeof(s_DevParam));
    s_DevParam.Windowed = bWindowed;
    s_DevParam.EnableAutoDepthStencil = TRUE;
    s_DevParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    s_DevParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    s_DevParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
    if (TRUE == bWindowed) // In case of window mode
    {
        D3DDISPLAYMODE dm;
        m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
        s_DevParam.BackBufferCount = 1;
        if (dwWidth <= 0) {
            dwWidth = dm.Width;
        }
        if (dwHeight <= 0) {
            dwHeight = dm.Height;
        }
        BBFormat = dm.Format;
        s_DevParam.hDeviceWindow = hWnd;
    } else {
        s_DevParam.BackBufferCount = 1;
        s_DevParam.AutoDepthStencilFormat = D3DFMT_D16; // Ignored if automatically generated.
        if (16 == dwBPP) {
            BBFormat = D3DFMT_R5G6B5;
        } else if (24 == dwBPP) {
            BBFormat = D3DFMT_R8G8B8;
        } else if (32 == dwBPP) {
            BBFormat = D3DFMT_X8R8G8B8;
        }
        s_DevParam.hDeviceWindow = hWnd;
    }

    s_DevParam.BackBufferWidth = dwWidth;
    s_DevParam.BackBufferHeight = dwHeight;
    s_DevParam.BackBufferFormat = BBFormat;
    s_DevParam.MultiSampleType = D3DMULTISAMPLE_NONE; // If the Swap Effect is not Discard type, it must be like this.
    s_DevParam.Flags = 0;
    //#ifdef _N3TOOL
    s_DevParam.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    //#endif // end of _N3TOOL

    int nMC = m_DeviceInfo.nModeCount;
    for (int i = 0; i < nMC; i++) {
        // if (m_DeviceInfo.pModes[i].Width == dwWidth && m_DeviceInfo.pModes[i].Height == dwHeight &&
        if (m_DeviceInfo.pModes[i].Format == BBFormat) // If the modes match
        {
            this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format,
                                         &s_DevParam.AutoDepthStencilFormat); // Find the depth and stencil buffer.
            m_nModeActive = i;
            break;
        }
    }

    HRESULT rval =
        m_lpD3D->CreateDevice(0, DevType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
    if (rval != D3D_OK) {
        rval = m_lpD3D->CreateDevice(0, DevType, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
        if (rval != D3D_OK) {
            char * pszDebug = NULL;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, rval, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pszDebug, 0, NULL);
            std::stringstream errMsg;
            errMsg << "Can't create D3D Device - please, check DirectX or display card driver: [";
            errMsg << "0x" << std::uppercase << std::hex << rval;
            if (pszDebug) {
                errMsg << ":" << pszDebug;
            }
            errMsg << "]";
            MessageBox(hWnd, errMsg.str().c_str(), "Initialization", MB_OK);
#ifdef _N3GAME
            CLogWriter::Write(errMsg.str().c_str());
#endif
            LocalFree(pszDebug);
            // for (int iii = 0; iii < 3; iii++) { // Beep several times~
            //     Beep(2000, 200);
            //     Sleep(300);
            // }

            this->Release();
            return false;
        }
#ifdef _N3GAME
        CLogWriter::Write("CNEng::Init - Not supported HardWare TnL");
#endif
    }

    // What device support items??
    // Whether DXT is supported...
    s_dwTextureCaps = 0;
    s_DevCaps.DeviceType = DevType;

    s_lpD3DDev->GetDeviceCaps(&s_DevCaps);
    if (s_DevCaps.MaxTextureWidth < 256 ||
        s_DevCaps.MaxTextureHeight < 256) // If the texture support size is 256 or less, give up completely.
    {
        MessageBox(::GetActiveWindow(), "Can't support this graphic card : Texture size is too small",
                   "Initialization error", MB_OK);
#ifdef _N3GAME
        CLogWriter::Write("Can't support this graphic card : Texture size is too small");
#endif
        // for (int iii = 0; iii < 4; iii++) { // Beep several times~
        //     Beep(2000, 200);
        //     Sleep(300);
        // }

        this->Release();
        return false;
    }

    if (D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1)) {
        s_dwTextureCaps |= TEX_CAPS_DXT1;
    }
    if (D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT2)) {
        s_dwTextureCaps |= TEX_CAPS_DXT2;
    }
    if (D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3)) {
        s_dwTextureCaps |= TEX_CAPS_DXT3;
    }
    if (D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT4)) {
        s_dwTextureCaps |= TEX_CAPS_DXT4;
    }
    if (D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5)) {
        s_dwTextureCaps |= TEX_CAPS_DXT5;
    }
    if (s_DevCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) {
        s_dwTextureCaps |= TEX_CAPS_SQUAREONLY;
    }
    if (s_DevCaps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) {
        s_dwTextureCaps |= TEX_CAPS_MIPMAP;
    }
    if (s_DevCaps.TextureCaps & D3DPTEXTURECAPS_POW2) {
        s_dwTextureCaps |= TEX_CAPS_POW2;
    }

    // Specify basic light information..
    for (int i = 0; i < 8; i++) {
        CN3Light::__Light Lgt;
        _D3DCOLORVALUE    LgtColor = {1.0f, 1.0f, 1.0f, 1.0f};
        Lgt.InitPoint(i, __Vector3(0, 0, 0), LgtColor);
        s_lpD3DDev->SetLight(i, &Lgt);
    }

    // Default view and projection settings.
    this->LookAt(__Vector3(5, 5, -10), __Vector3(0, 0, 0), __Vector3(0, 1, 0));
    this->SetProjection(0.1f, 256.0f, D3DXToRadian(45.0f), (float)dwHeight / dwWidth);

    RECT rcView = {0, 0, dwWidth, dwHeight};
    this->SetViewPort(rcView);
    this->SetDefaultEnvironment(); // Set to default state..

    if (m_CB.Init) {
        if (!m_CB.Init()) {
            return false;
        }
    }

    return true;
}

void CN3Eng::LookAt(const __Vector3 & vEye, const __Vector3 & vAt, const __Vector3 & vUp) {
    __Matrix44 matView;
    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    s_lpD3DDev->SetTransform(D3DTS_VIEW, &matView);
}

void CN3Eng::SetProjection(float fNear, float fFar, float fLens, float fAspect) {
    __Matrix44 matProjection;
    ::D3DXMatrixPerspectiveFovLH(&matProjection, fLens, fAspect, fNear, fFar);
    s_lpD3DDev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void CN3Eng::SetViewPort(const RECT & rc) {
    if (NULL == s_lpD3DDev) {
        return;
    }
    D3DVIEWPORT9 vp;

    vp.X = rc.left;
    vp.Y = rc.top;
    vp.Width = rc.right - rc.left;
    vp.Height = rc.bottom - rc.top;
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;

    s_lpD3DDev->SetViewport(&vp);
    memcpy(&s_CameraData.vp, &vp, sizeof(D3DVIEWPORT9));
}

LRESULT WINAPI CN3Eng::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        // PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        // Render();
        // ValidateRect(hWnd, NULL);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CN3Eng::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat,
                                    D3DFORMAT * pDepthStencilFormat) {
    int       nDSC = 6;
    D3DFORMAT DepthFmts[] = {D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D16, D3DFMT_D15S1};

    HRESULT rval = 0;
    for (int i = 0; i < nDSC; i++) {
        rval = m_lpD3D->CheckDeviceFormat(iAdapter, DeviceType, TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,
                                          DepthFmts[i]);
        if (D3D_OK == rval) {
            rval = m_lpD3D->CheckDepthStencilMatch(iAdapter, DeviceType, TargetFormat, TargetFormat, DepthFmts[i]);
            if (D3D_OK == rval) {
                *pDepthStencilFormat = DepthFmts[i];
                return TRUE;
            }
        }
    }

    return FALSE;
}

void CN3Eng::BeginScene() {
    s_lpD3DDev->BeginScene();
    if (m_CB.BeginScene) {
        m_CB.BeginScene();
    }
}

void CN3Eng::EndScene() {
    if (m_CB.EndScene) {
        m_CB.EndScene();
    }
    s_lpD3DDev->EndScene();
}

void CN3Eng::Present(HWND hWnd, RECT * pRC) {
    //     HRESULT rval = s_lpD3DDev->TestCooperativeLevel();
    //     if (D3D_OK != rval) {
    // #if _DEBUG
    //         char szDebug[256];
    //         D3DXGetErrorString(rval, szDebug, 256);
    // #endif
    //         return;
    //     }

    RECT rc;
    if (s_DevParam.Windowed) // In Windows mode...
    {
        GetClientRect(s_hWndBase, &rc);
        pRC = &rc;
    }

    if (m_CB.Present) {
        m_CB.Present();
    }

    HRESULT rval = s_lpD3DDev->Present(pRC, pRC, hWnd, NULL);
    if (D3D_OK == rval) {
        s_hWndPresent = hWnd; // Save the Present window handle.
    } else if (D3DERR_DEVICELOST == rval || D3DERR_DEVICENOTRESET == rval) {
        rval = s_lpD3DDev->Reset(&s_DevParam);
        if (D3D_OK != rval) {
#ifdef _N3GAME
            // char szErr[256];
            // D3DXGetErrorString(rval, szErr, 256);
            // CLogWriter::Write("CNEng::Present - device present failed (%s)", szErr);
            // Beep(2000, 50);
#endif
        } else {
            rval = s_lpD3DDev->Present(pRC, pRC, hWnd, NULL);
        }
        return;
    } else {
#ifdef _N3GAME
        // char szErr[256];
        // D3DXGetErrorString(rval, szErr, 256);
        // CLogWriter::Write("CNEng::Present - device present failed (%s)", szErr);
        // Beep(2000, 50);
#endif
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Frame rate measurement...
    // float        fTime = CN3Base::TimerProcess(TIMER_GETABSOLUTETIME);
    // static float fTimePrev = fTime - 0.03333f;
    // static DWORD dwFrm = 0;
    // dwFrm++;
    // if (fTime - fTimePrev > 1.0f) { // Only after more than 1 second passes to measure the frame. Otherwise, jagged figures will appear.
    //     s_fFrmPerSec = (float)dwFrm / (fTime - fTimePrev);
    //     dwFrm = 0;
    //     fTimePrev = fTime;
    // }

    s_fSecPerFrm = CN3Base::TimerProcess(TIMER_GETELAPSEDTIME);
    if (s_fSecPerFrm <= 0.001f || s_fSecPerFrm >= 1.0f) {
        s_fSecPerFrm = 0.033333f; // If it doesn't come out too well, set it to the default value of 30 frames.
    }
    s_fFrmPerSec = 1.0f / s_fSecPerFrm; // Measures the number of frames per second.

    // fTimePrev = fTime;
    // Frame rate measurement...
    ////////////////////////////////////////////////////////////////////////////////
}

void CN3Eng::Clear(D3DCOLOR crFill, RECT * pRC) {
    RECT rc;
    if (NULL == pRC && s_DevParam.Windowed) // In Windows mode...
    {
        GetClientRect(s_hWndBase, &rc);
        pRC = &rc;
    }

    if (pRC) {
        _D3DRECT rc3D = {pRC->left, pRC->top, pRC->right, pRC->bottom};
        s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, crFill, 1.0f, 0);
    } else {
        s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, crFill, 1.0f, 0);
    }

#ifdef _DEBUG
    memset(&s_RenderInfo, 0, sizeof(__RenderInfo));
#endif
}

void CN3Eng::ClearAuto(RECT * pRC) {
    DWORD dwFillColor = D3DCOLOR_ARGB(255, 192, 192, 192); // Basic color
    DWORD dwUseFog = FALSE;
    s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwUseFog); // When using fog, the fog color is applied as the background color.
    if (dwUseFog != 0) {
        s_lpD3DDev->GetRenderState(D3DRS_FOGCOLOR, &dwFillColor);
    } else {
        CN3Light::__Light Lgt;

        BOOL bEnable;
        s_lpD3DDev->GetLightEnable(0, &bEnable);
        if (bEnable) {
            s_lpD3DDev->GetLight(0, &Lgt);
            dwFillColor = D3DCOLOR_ARGB((BYTE)(Lgt.Diffuse.a * 255.0f), (BYTE)(Lgt.Diffuse.r * 255.0f),
                                        (BYTE)(Lgt.Diffuse.g * 255.0f), (BYTE)(Lgt.Diffuse.b * 255.0f));
        }
    }

    CN3Eng::Clear(dwFillColor, pRC);
}

void CN3Eng::ClearZBuffer(const RECT * pRC) {
    RECT rc;
    if (NULL == pRC && s_DevParam.Windowed) // In Windows mode...
    {
        GetClientRect(s_hWndBase, &rc);
        pRC = &rc;
    }

    if (pRC) {
        D3DRECT rc3D = {pRC->left, pRC->top, pRC->right, pRC->bottom};
        s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    } else {
        s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    }
}

bool CN3Eng::Reset(BOOL bWindowed, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP) {
    if (NULL == s_lpD3DDev) {
        return false;
    }
    if (dwWidth <= 0 || dwHeight <= 0) {
        return false;
    }
    if (dwWidth == s_DevParam.BackBufferWidth && dwHeight == s_DevParam.BackBufferHeight) // When the width and height are the same...
    {
        if (0 == dwBPP) {
            return false;
        }
        if (16 == dwBPP && D3DFMT_R5G6B5 == s_DevParam.BackBufferFormat) {
            return false;
        }
        if (24 == dwBPP && D3DFMT_R8G8B8 == s_DevParam.BackBufferFormat) {
            return false;
        }
        if (32 == dwBPP && D3DFMT_X8R8G8B8 == s_DevParam.BackBufferFormat) {
            return false;
        }
    }

    D3DPRESENT_PARAMETERS DevParamBackUp; // Baclup
    memcpy(&DevParamBackUp, &s_DevParam, sizeof(D3DPRESENT_PARAMETERS));

    D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
    if (bWindowed) {
        D3DDISPLAYMODE dm;
        m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
        BBFormat = dm.Format;
    } else {
        if (16 == dwBPP) {
            BBFormat = D3DFMT_R5G6B5;
        } else if (24 == dwBPP) {
            BBFormat = D3DFMT_R8G8B8;
        } else if (32 == dwBPP) {
            BBFormat = D3DFMT_X8R8G8B8;
        }
    }

    s_DevParam.Windowed = bWindowed;
    s_DevParam.BackBufferWidth = dwWidth;
    s_DevParam.BackBufferHeight = dwHeight;
    s_DevParam.BackBufferFormat = BBFormat;

    int nMC = m_DeviceInfo.nModeCount;
    for (int i = 0; i < nMC; i++) {
        // if (m_DeviceInfo.pModes[i].Width == dwWidth && m_DeviceInfo.pModes[i].Height == dwHeight &&
        if (m_DeviceInfo.pModes[i].Format == s_DevParam.BackBufferFormat) // If the modes match
        {
            this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format,
                                         &s_DevParam.AutoDepthStencilFormat); // Find the depth and stencil buffer.
            m_nModeActive = i;
            break;
        }
    }

    if (D3D_OK != s_lpD3DDev->Reset(&s_DevParam)) {
#ifdef _N3GAME
        CLogWriter::Write("CNEng::Reset - Insufficient video memory");
#endif
        memcpy(&s_DevParam, &DevParamBackUp, sizeof(D3DPRESENT_PARAMETERS));
        if (D3D_OK != s_lpD3DDev->Reset(&s_DevParam)) {
#ifdef _N3GAME
            CLogWriter::Write("CNEng::Reset - Insufficient video memory");
#endif
        }
        return false;
    }

    RECT rcView = {0, 0, dwWidth, dwHeight};
    this->SetViewPort(rcView);

    this->SetDefaultEnvironment();

    return true;
}

void CN3Eng::SetDefaultEnvironment() {
    // Specify default rendering state

    __Matrix44 matWorld;
    matWorld.Identity();
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);
    // s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW); // Z buffer available
    s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); // Z buffer available
    s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    s_lpD3DDev->SetRenderState(D3DRS_DITHERENABLE, TRUE);
    s_lpD3DDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    // s_lpD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00444444);

    s_lpD3DDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    s_lpD3DDev->SetRenderState(D3DRS_ALPHAFUNC,
                               D3DCMP_GREATER); // Basic alpha function - If you don't do it, alpha textures may break.

    s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    // Specifies a default texture filter.
    float fMipMapLODBias = -1.0f;
    for (int i = 0; i < 8; i++) {
        s_lpD3DDev->SetTexture(i, NULL);
        s_lpD3DDev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        s_lpD3DDev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        s_lpD3DDev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        s_lpD3DDev->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&fMipMapLODBias)));
    }

    // Specify clipping status
    D3DCLIPSTATUS9 cs;
    cs.ClipUnion = cs.ClipIntersection = D3DCS_ALL;
    s_lpD3DDev->SetClipStatus(&cs);
}

HKEY CN3Eng::RegistryOpen(const std::string & szKey) {
    HKEY hKey = NULL;
    long lStatus = RegOpenKey(HKEY_CURRENT_USER, szKey.c_str(), &hKey);

    return hKey;
}

bool CN3Eng::RegistryClose(HKEY & hKey) {
    if (NULL == hKey) {
        return false;
    }

    long lStatus = RegCloseKey(hKey);
    hKey = NULL;

    if (ERROR_SUCCESS == lStatus) {
        return true;
    }
    return false;
}

bool CN3Eng::RegistryValueGet(HKEY hKey, const std::string & szName, std::string & szValue) {
    if (NULL == hKey || szName.empty() || szValue.empty()) {
        return false;
    }

    DWORD dwType = REG_SZ;
    DWORD dwBytes = 0;

    char buffer[MAX_PATH]{};
    long lStatus = RegQueryValueEx(hKey, szName.c_str(), NULL, &dwType, (BYTE *)buffer, &dwBytes);
    szValue = buffer;

    if (ERROR_SUCCESS == lStatus) {
        return true;
    }
    return false;
}

bool CN3Eng::RegistryValueGet(HKEY hKey, const std::string & szName, void * pValue, DWORD dwBytes) {
    if (NULL == hKey || szName.empty() || NULL == pValue || 0 == dwBytes) {
        return false;
    }

    DWORD dwType = REG_BINARY;
    long  lStatus = RegQueryValueEx(hKey, szName.c_str(), NULL, &dwType, (BYTE *)pValue, &dwBytes);

    if (ERROR_SUCCESS == lStatus) {
        return true;
    }
    return false;
}

bool CN3Eng::RegistryValueSet(HKEY hKey, const std::string & szName, std::string & szValue) {
    if (NULL == hKey || szName.empty() || szValue.empty()) {
        return false;
    }

    DWORD dwBytes = szValue.size();
    long  lStatus = RegSetValueEx(hKey, szName.c_str(), NULL, REG_SZ, (BYTE *)(szValue.c_str()), dwBytes);

    if (ERROR_SUCCESS == lStatus) {
        return true;
    }
    return false;
}

bool CN3Eng::RegistryValueSet(HKEY hKey, const std::string & szName, void * pValue, DWORD dwBytes) {
    if (NULL == hKey || szName.empty() || NULL == pValue || 0 == dwBytes) {
        return false;
    }

    long lStatus = RegSetValueEx(hKey, szName.c_str(), NULL, REG_BINARY, (BYTE *)pValue, dwBytes);

    if (ERROR_SUCCESS == lStatus) {
        return true;
    }
    return false;
}
