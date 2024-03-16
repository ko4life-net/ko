// N3Eng.h: interface for the CN3Eng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"
#include <ddraw.h>
#include <functional>

struct CN3EngCallbacks {
    std::function<bool()> Init;
    std::function<void()> BeginScene;
    std::function<void()> EndScene;
    std::function<void()> Present;
};

class CN3Eng : public CN3Base {
  public:
    typedef struct __D3DDEV_INFO {
        char szDeviceName[128];
        char szDeviceDesc[128];
        int  nAdapter; // Which graphics card is it?
        int  nDevice;  // Which device is it?

        D3DDEVTYPE       DevType;    // Is it hardware acceleration?
        int              nModeCount; // Number of display modes
        D3DDISPLAYMODE * pModes;     // display mode
    } __D3DDevInfo;

    LPDIRECT3D9  m_lpD3D;
    LPDIRECTDRAW m_lpDD;

    static CN3EngCallbacks m_CB;

  protected:
    int           m_nModeActive;   // Currently selected Mode
    int           m_nAdapterCount; // Number of graphics cards
    __D3DDEV_INFO m_DeviceInfo;    //Device information

  public:
    HKEY RegistryOpen(const std::string & szKey);
    bool RegistryClose(HKEY & hKey);
    bool RegistryValueGet(HKEY hKey, const std::string & szName, std::string & szValue);
    bool RegistryValueGet(HKEY hKey, const std::string & szName, void * pValue, DWORD dwBytes);
    bool RegistryValueSet(HKEY hKey, const std::string & szName, std::string & szValue);
    bool RegistryValueSet(HKEY hKey, const std::string & szName, void * pValue, DWORD dwBytes);

    void        SetDefaultEnvironment();
    bool        Reset(BOOL bWindowed, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);
    static void Clear(D3DCOLOR crFill, RECT * pRC = NULL);
    static void ClearAuto(RECT * pRC = NULL);
    static void ClearZBuffer(const RECT * pRC = NULL);
    static void BeginScene();
    static void EndScene();
    static void Present(HWND hWnd, RECT * pRC = NULL);

    void LookAt(const __Vector3 & vEye, const __Vector3 & vAt, const __Vector3 & vUp);
    void SetProjection(float fNear, float fFar, float fLens, float fAspect);
    void SetViewPort(const RECT & pRC);

    void Release();

    // void InitEnv();
    bool Init(BOOL bWindowed, HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bUseHW);
    BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat,
                                D3DFORMAT * pDepthStencilFormat);

    static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    CN3Eng();
    virtual ~CN3Eng();
};
