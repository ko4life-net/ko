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

#if _N3GAME // 게임이 아닌 툴에서는 필요없다...
#include "N3SndMgr.h"
#endif
#ifdef _N3UIE // ui 에디터일때는 필요하다.
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
const int   MAX_CAMERA_RADIUS = 512; // 2미터 단위로 128 개의 도트 프로덕트 미리 계산해 놓는다..

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

    float fFOV; // 카메라 렌즈 각 : Field Of View
    // float        fInverse_SineHalfOfFOV;
    float        fAspect; // 종횡비
    float        fNP;     // NearPlane
    float        fFP;     // FarPlane
    D3DVIEWPORT9 vp;      // ViewPort;
    __Matrix44   mtxView;
    __Matrix44   mtxViewInverse;
    __Matrix44   mtxProjection;

    float fFrustum[6][4];

    // fRadius - 물체의 반지름보다 약간 더 여유 있게 잡고 넣으면 그만큼 클리핑을 여유있게 한다..
    BOOL IsOutOfFrustum(const __Vector3 & vPosition, float fRadius)
    // 미리 계산된 카메라 평면의 도트 프로덕트 값을 기준으로 카메라 사면체 밖에 있으면  참을 돌려준다.
    {
        if ((vEye - vPosition).Magnitude() > fFP + fRadius) {
            return TRUE; // Far Plane 거리체크
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
    int nShape_Polygon; // 단순 폴리곤

    int nChr;
    int nChr_Part;
    int nChr_Polygon;      // 캐릭터 폴리곤
    int nChr_Plug;         // 캐릭터에 붙은 무기등..
    int nChr_Plug_Polygon; // 캐릭터에 붙은 무기등의 폴리곤..

    int nTerrain_Polygon;      // 타일 적용된 지형 폴리곤..
    int nTerrain_Tile_Polygon; // 타일 적용된 지형 폴리곤..

    int nAlpha_Polygon;

    int nTexture_32X32;     // 32 X 32 Texture
    int nTexture_64X64;     // 64 X 64 Texture
    int nTexture_128X128;   // 128 X 128 Texture
    int nTexture_256X256;   // 256 X 256 Texture
    int nTexture_512X512;   // 512 X 512 Texture
    int nTexture_Huge;      // 512 X 512 이상 size
    int nTexture_OtherSize; // Other size
};

struct __ResrcInfo {
    int nTexture_Loaded_32X32;     // 32 X 32 Texture
    int nTexture_Loaded_64X64;     // 64 X 64 Texture
    int nTexture_Loaded_128X128;   // 128 X 128 Texture
    int nTexture_Loaded_256X256;   // 256 X 256 Texture
    int nTexture_Loaded_512X512;   // 512 X 512 Texture
    int nTexture_Loaded_Huge;      // 512 X 512 이상 size
    int nTexture_Loaded_OtherSize; // Other size
};

struct __Options {
    int iUseShadow;
    int iTexLOD_Chr;     // 0 - 원래 크기.. 1 - 한단계 작게. 2 - 두단계 작게..
    int iTexLOD_Shape;   // 0 - 원래 크기.. 1 - 한단계 작게. 2 - 두단계 작게..
    int iTexLOD_Terrain; // 0 - 원래 크기.. 1 - 한단계 작게. 2 - 두단계 작게..
    int iViewWidth;
    int iViewHeight;
    int iViewColorDepth;
    int iViewDist;
    int iEffectSndDist; // 이펙트 사운드 거리

    bool bSndEnable;     // 0 - High, 1 - Low
    bool bSndDuplicated; // 중복된 음원 사용
    bool bWindowCursor;  // 0 - 게임에서 그려주는 커서 1 - 윈도우 커서 사용

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
    static LPDIRECT3DDEVICE9     s_lpD3DDev;      // Device 참조 포인터.. 멋대로 해제하면 안된다..
    static D3DPRESENT_PARAMETERS s_DevParam;      // Device 생성 Present Parameter
    static D3DCAPS9              s_DevCaps;       // Device 호환성...
    static DWORD                 s_dwTextureCaps; // Texture 지원.. DXT1 ~ DXT5, Square Only
    static HWND                  s_hWndBase;      // Init 할때 쓴 Window Handle
    static HWND                  s_hWndPresent;   // 최근에 Present 한 Window Handle

    static __CameraData s_CameraData; // 카메라 데이터 정적 변수..
    static __ResrcInfo  s_ResrcInfo;  // Rendering Information..
    static __Options    s_Options;    // 각종 옵션등...
#ifdef _DEBUG
    static __RenderInfo s_RenderInfo; // Rendering Information..
#endif
    static float s_fFrmPerSec; // Frame Per Second
    static float s_fSecPerFrm; // Second Per Frame = 1.0f/s_fFrmPerSec (Dino가 추가)

#ifdef _N3GAME // 게임이 아닌 툴에서는 필요없다...
    static CN3SndMgr s_SndMgr;
#endif
#ifdef _N3UIE // ui 에디터일때는 필요하다.
    static CN3SndMgr s_SndMgr;
#endif

    static CN3AlphaPrimitiveManager
        s_AlphaMgr; // Alpha blend 할 폴리곤들을 관리.. 추가했다가.. 카메라 거리에 씗추어 정렬하고 한꺼번에 그린다..

    static CN3Mng<class CN3Texture> s_MngTex;  // Texture Manager
    static CN3Mng<class CN3Mesh>    s_MngMesh; // Normal Mesh Manager
    static CN3Mng<class CN3VMesh> s_MngVMesh; // 단순히 폴리곤만 갖고 있는 메시 - 주로 충돌 체크에 쓴다..
    static CN3Mng<class CN3PMesh>       s_MngPMesh;   // Progressive Mesh Manager
    static CN3Mng<class CN3Joint>       s_MngJoint;   // Joint Manager
    static CN3Mng<class CN3CPartSkins>  s_MngSkins;   // Character Part Skin Manager
    static CN3Mng<class CN3AnimControl> s_MngAniCtrl; // Animation Manager
    static CN3Mng<class CN3FXPMesh> s_MngFXPMesh; // FX에서 쓰는 PMesh - 파일은 일반 PMesh를 쓰지만 속은 다르다.
    static CN3Mng<class CN3FXShape> s_MngFXShape; // FX에서 쓰는 Shape - 파일은 일반 shape를 쓰지만 속은 다르다.

  private:
    static fs::path s_fsBaseDir; // Application root path (aka working directory)

  protected:
    DWORD m_dwType; // "MESH", "CAMERA", "SCENE", "???" .... 등등등...

  public:
    std::string m_szName;

  public:
    static float            TimeGet();
    static const fs::path & PathGet() { return s_fsBaseDir; }
    static void             PathSet(const fs::path & fsBaseDir);

    static void RenderLines(const __Vector3 * pvLines, int nCount, D3DCOLOR color);
    static void RenderLines(const RECT & rc, D3DCOLOR color);

    static float TimerProcess(TIMER_COMMAND command);

    DWORD Type() { return m_dwType; } // 객체 종류..

    void ReleaseResrc();
    //#ifdef _N3TOOL
    void SaveResrc();
    //#endif // end of _N3TOOL
    virtual void Release();
    CN3Base();
    virtual ~CN3Base();
};
