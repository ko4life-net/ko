// N3Base.h: interface for the CN3Base class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "My_3DStruct.h"
#include "N3Mng.h"
#include "N3AlphaPrimitiveManager.h"
#ifdef _N3GAME
#include "LogWriter.h"
#endif

#include <string>

#if _N3GAME // In non-game tools, you don't need it...
#include "N3SndMgr.h"
#endif
#ifdef _N3UIE // Required for ui editor.
#include "N3SndMgr.h"
#endif

const DWORD TEX_CAPS_DXT1 = 0x00000001;
const DWORD TEX_CAPS_DXT2 = 0x00000002;
const DWORD TEX_CAPS_DXT3 = 0x00000004;
const DWORD TEX_CAPS_DXT4 = 0x00000008;
const DWORD TEX_CAPS_DXT5 = 0x00000010;
const DWORD TEX_CAPS_SQUAREONLY = 0x00000020;
const DWORD TEX_CAPS_MIPMAP = 0x00000040;
const DWORD TEX_CAPS_POW2 = 0x00000080;

const float CAMERA_RADIUS_UNIT = 2.0f;
const int   MAX_CAMERA_RADIUS = 512; // Pre-calculate 128 dot products in units of 2 meters.

enum TIMER_COMMAND {
    TIMER_RESET,
    TIMER_START,
    TIMER_STOP,
    TIMER_ADVANCE,
    TIMER_GETABSOLUTETIME,
    TIMER_GETAPPTIME,
    TIMER_GETELAPSEDTIME
};

struct __CameraData {
    __CameraData() { Release(); }
    void Release() {
        memset(this, 0, sizeof(__CameraData));
        mtxView.Identity();
        mtxViewInverse.Identity();
        mtxProjection.Identity();
    }
    __Vector3 vEye; // Camera Position Vector
    __Vector3 vAt;  // Camera At Vector
    __Vector3 vUp;  // Camera Up Vector

    float fFOV; // camera lens angle : Field Of View
    // float        fInverse_SineHalfOfFOV;
    float        fAspect; // aspect ratio
    float        fNP;     // NearPlane
    float        fFP;     // FarPlane
    D3DVIEWPORT9 vp;      // ViewPort;
    __Matrix44   mtxView;
    __Matrix44   mtxViewInverse;
    __Matrix44   mtxProjection;

    float fFrustum[6][4];

    // fRadius - If you hold it a little more than the radius of the object and insert it, you will have enough room for clipping.
    BOOL IsOutOfFrustum(const __Vector3 & vPosition, float fRadius)
    // Returns true if it is outside the camera tetrahedron based on the precomputed dot product of the camera plane.
    {
        if ((vEye - vPosition).Magnitude() > fFP + fRadius) {
            return TRUE; // Far Plane - distance check
        }

        int p;
        for (p = 0; p < 6; p++) {
            if (fFrustum[p][0] * vPosition.x + fFrustum[p][1] * vPosition.y + fFrustum[p][2] * vPosition.z +
                    fFrustum[p][3] <=
                -fRadius) {
                return TRUE;
            }
        }
        return FALSE;
    }
};

struct __RenderInfo {
    int nShape;
    int nShape_Part;
    int nShape_Polygon; // Simple Polygon

    int nChr;
    int nChr_Part;
    int nChr_Polygon;      // character polygon
    int nChr_Plug;         // Weapons attached to the character, etc.
    int nChr_Plug_Polygon; // Polygons such as weapons attached to characters..

    int nTerrain_Polygon;      // Tiled terrain polygons..
    int nTerrain_Tile_Polygon; // Tiled terrain polygons..

    int nAlpha_Polygon;

    int nTexture_32X32;     // 32 X 32 Texture
    int nTexture_64X64;     // 64 X 64 Texture
    int nTexture_128X128;   // 128 X 128 Texture
    int nTexture_256X256;   // 256 X 256 Texture
    int nTexture_512X512;   // 512 X 512 Texture
    int nTexture_Huge;      // 512 X 512 or larger size
    int nTexture_OtherSize; // Other size
};

struct __ResrcInfo {
    int nTexture_Loaded_32X32;     // 32 X 32 Texture
    int nTexture_Loaded_64X64;     // 64 X 64 Texture
    int nTexture_Loaded_128X128;   // 128 X 128 Texture
    int nTexture_Loaded_256X256;   // 256 X 256 Texture
    int nTexture_Loaded_512X512;   // 512 X 512 Texture
    int nTexture_Loaded_Huge;      // 512 X 512 or larger size
    int nTexture_Loaded_OtherSize; // Other size
};

struct __Options {
    int iUseShadow;
    int iTexLOD_Chr;     // 0 - original size. 1 - one size smaller. 2 - two steps smaller..
    int iTexLOD_Shape;   // 0 - original size. 1 - one size smaller. 2 - two steps smaller..
    int iTexLOD_Terrain; // 0 - original size. 1 - one size smaller. 2 - two steps smaller..
    int iViewWidth;
    int iViewHeight;
    int iViewColorDepth;
    int iViewDist;
    int iEffectSndDist; // effect sound distance

    bool bSndEnable;     // 0 - High, 1 - Low
    bool bSndDuplicated; // Use of duplicate sound sources
    bool bWindowCursor;  // 0 - Cursor drawn by game 1 - Use window cursor

    void InitDefault() {
        iUseShadow = true;
        iTexLOD_Chr = 0;
        iTexLOD_Shape = 0;
        iTexLOD_Terrain = 0;
        iViewColorDepth = 16;
        iViewWidth = 1024;
        iViewHeight = 768;
        iViewDist = 512;
        iEffectSndDist = 48;
        bSndEnable = 0;
        bSndDuplicated = false;
        bWindowCursor = true;
    }
    __Options() { InitDefault(); }
};

class CN3Base {
  public:
    static LPDIRECT3DDEVICE9     s_lpD3DDev;      // Device reference pointer.. You must not release it arbitrarily..
    static D3DPRESENT_PARAMETERS s_DevParam;      // Device Creation Present Parameter
    static D3DCAPS9              s_DevCaps;       // Device Compatibility...
    static DWORD                 s_dwTextureCaps; // Texture support.. DXT1 ~ DXT5, Square Only
    static HWND                  s_hWndBase;      // Window handle used at init
    static HWND                  s_hWndPresent;   // Window Handle recently presented

    static __CameraData s_CameraData; // Camera data static variable..
    static __ResrcInfo  s_ResrcInfo;  // Rendering Information..
    static __Options    s_Options;    // Various options...
#ifdef _DEBUG
    static __RenderInfo s_RenderInfo; // Rendering Information..
#endif
    static float s_fFrmPerSec; // Frame Per Second
    static float s_fSecPerFrm; // Second Per Frame = 1.0f/s_fFrmPerSec (added by Dino)

#ifdef _N3GAME // In non-game tools, you don't need it...
    static CN3SndMgr s_SndMgr;
#endif
#ifdef _N3UIE // Required for ui editor.
    static CN3SndMgr s_SndMgr;
#endif

    static CN3AlphaPrimitiveManager
        s_AlphaMgr; // Manages the polygons to be alpha blended.. Adds them.. Washes them at the camera distance, arranges them, and draws them all at once..

    static CN3Mng<class CN3Texture>     s_MngTex;     // Texture Manager
    static CN3Mng<class CN3Mesh>        s_MngMesh;    // Normal Mesh Manager
    static CN3Mng<class CN3VMesh>       s_MngVMesh;   // A mesh with only polygons - mainly used for collision checking.
    static CN3Mng<class CN3PMesh>       s_MngPMesh;   // Progressive Mesh Manager
    static CN3Mng<class CN3Joint>       s_MngJoint;   // Joint Manager
    static CN3Mng<class CN3CPartSkins>  s_MngSkins;   // Character Part Skin Manager
    static CN3Mng<class CN3AnimControl> s_MngAniCtrl; // Animation Manager
    static CN3Mng<class CN3FXPMesh>     s_MngFXPMesh; // PMesh used in FX - The file uses general PMesh, but the contents are different.
    static CN3Mng<class CN3FXShape>     s_MngFXShape; // Shape used in FX - The file uses a general shape, but the inside is different.

  protected:
    static std::string s_szPath; // path where the program was executed.

  protected:
    DWORD m_dwType; // "MESH", "CAMERA", "SCENE", "???" .... and so on...

  public:
    std::string m_szName;

  public:
    static float               TimeGet();
    static const std::string & PathGet() { return s_szPath; }
    static void                PathSet(const std::string & szPath);

    static void RenderLines(const __Vector3 * pvLines, int nCount, D3DCOLOR color);
    static void RenderLines(const RECT & rc, D3DCOLOR color);

    static float TimerProcess(TIMER_COMMAND command);

    DWORD Type() { return m_dwType; } // object type...

    void ReleaseResrc();
    //#ifdef _N3TOOL
    void SaveResrc();
    //#endif // end of _N3TOOL
    virtual void Release();
    CN3Base();
    virtual ~CN3Base();
};
