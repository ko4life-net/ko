// N3FXPartBase.cpp: implementation of the CN3EffectPartBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXPartBase.h"
#include "N3FXBundle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXPartBase::CN3FXPartBase() {
    m_iVersion = 0;
    m_iBaseVersion = 2;

    m_pRefBundle = NULL;
    m_pRefPrevPart = NULL;

    m_iType = FX_PART_TYPE_NONE;

    m_fLife = m_fCurrLife = 0.0f;

    m_strName.erase();

    m_vVelocity.Set(0, 0, 0);
    m_vAcceleration.Set(0, 0, 0);
    m_vRotVelocity.Set(0, 0, 0);
    m_vCurrVelocity = m_vVelocity;

    m_dwState = FX_PART_STATE_READY;
    m_vPos.Set(0, 0, 0);
    m_vCurrPos = m_vPos;

    m_ppRefTex = NULL;
    m_iNumTex = 0;
    m_fTexFPS = 30.0f;

    m_dwSrcBlend = D3DBLEND_ONE;
    m_dwDestBlend = D3DBLEND_ONE;
    m_bAlpha = TRUE;

    m_fFadeOut = 0.0f;
    m_fFadeIn = 0.0f;

    m_bOnGround = false;

    m_dwRenderFlag = RF_ALPHABLENDING | RF_NOTUSEFOG | RF_DIFFUSEALPHA | RF_NOTUSELIGHT | RF_DOUBLESIDED;

    m_dwZEnable = D3DZB_TRUE;
    m_dwZWrite = TRUE;
    m_dwLight = FALSE;
    m_dwDoubleSide = D3DCULL_NONE;
    /*
const DWORD RF_NOTHING            = 0x0;
const DWORD RF_ALPHABLENDING    = 0x1;        // Alpha blending
const DWORD RF_NOTUSEFOG        = 0x2;        // 안개 무시
const DWORD RF_DOUBLESIDED        = 0x4;        // 양면 - D3DCULL_NONE
const DWORD RF_BOARD_Y            = 0x8;        // Y 축으로 해서.. 카메라를 본다.
const DWORD RF_POINTSAMPLING    = 0x10;        // MipMap 에서.. PointSampling 으로 한다..
const DWORD RF_WINDY            = 0x20;        // 바람에 날린다.. 바람의 값은 CN3Base::s_vWindFactor 를 참조 한다..
const DWORD RF_NOTUSELIGHT        = 0x40;        // Light Off
const DWORD RF_DIFFUSEALPHA        = 0x80;        // Diffuse 값을 갖고 투명하게 Alpha blending
const DWORD RF_NOTZWRITE        = 0x100;    // ZBuffer 에 안쓴다.
const DWORD RF_UV_CLAMP            = 0x200;    // texture UV적용을 Clamp로 한다..default는 wrap이다..
const DWORD RF_NOTZBUFFER        = 0x400;    // ZBuffer 무시.
*/
}

CN3FXPartBase::~CN3FXPartBase() {
    if (m_ppRefTex) {
        for (int i = 0; i < m_iNumTex; i++) {
            CN3Base::s_MngTex.Delete(&m_ppRefTex[i]);
        }
        delete[] m_ppRefTex;
        m_ppRefTex = NULL;
    }
}

//
//    parse script...
//    스크립트 해서 & 셋팅.
//
#ifdef _N3TOOL
bool CN3FXPartBase::ParseScript(char * szCommand, char * szBuff0, char * szBuff1, char * szBuff2, char * szBuff3) {
    //    이름.
    if (n3std::iequals(szCommand, "<name>")) {
        m_strName = szBuff0;
        return true;
    }

    //    타입..
    if (n3std::iequals(szCommand, "<type>")) {
        if (n3std::iequals(szBuff0, "particle")) {
            m_iType = FX_PART_TYPE_PARTICLE;
        }
        if (n3std::iequals(szBuff0, "board")) {
            m_iType = FX_PART_TYPE_BOARD;
        }
        if (n3std::iequals(szBuff0, "mesh")) {
            m_iType = FX_PART_TYPE_MESH;
        }
        if (n3std::iequals(szBuff0, "ground")) {
            m_iType = FX_PART_TYPE_BOTTOMBOARD;
        }
        //^^v 더 넣을꺼 있으면 넣어라..
        return true;
    }

    //    지속시간.(0이면 무한대...)
    if (n3std::iequals(szCommand, "<life>")) {
        m_fLife = atof(szBuff0);
        return true;
    }

    //    texture 이름과 개수 읽기.
    if (n3std::iequals(szCommand, "<texture>")) {
        m_iNumTex = atoi(szBuff1);
        m_ppRefTex = new CN3Texture *[m_iNumTex];

        fs::path fsFile = szBuff0;
        m_fsTexFileBase = fsFile.parent_path() / fsFile.stem();

        std::string szExt = fsFile.extension().string();
        for (int i = 0; i < m_iNumTex; i++) {
            m_ppRefTex[i] = CN3Base::s_MngTex.Get(m_fsTexFileBase + std::format("{:04d}{:s}", i, szExt));
        }
        return true;
    }

    //    texture animation speed 설정..
    if (n3std::iequals(szCommand, "<texture_animation_speed>")) {
        m_fTexFPS = atof(szBuff0);
        return true;
    }

    //    상대위치...
    if (n3std::iequals(szCommand, "<position0>")) {
        m_vPos.x = atof(szBuff0);
        m_vPos.y = atof(szBuff1);
        m_vPos.z = atof(szBuff2);
        return true;
    }

    //    속도..
    if (n3std::iequals(szCommand, "<velocity>")) {
        __Vector3 v;
        m_vVelocity.x = atof(szBuff0);
        m_vVelocity.y = atof(szBuff1);
        m_vVelocity.z = atof(szBuff2);
        return true;
    }

    //    가속도..
    if (n3std::iequals(szCommand, "<acceleration>")) {
        __Vector3 v;
        m_vAcceleration.x = atof(szBuff0);
        m_vAcceleration.y = atof(szBuff1);
        m_vAcceleration.z = atof(szBuff2);
        return true;
    }

    //    회전 각속도..
    if (n3std::iequals(szCommand, "<rot_velocity>")) {
        __Vector3 v;
        m_vRotVelocity.x = atof(szBuff0);
        m_vRotVelocity.y = atof(szBuff1);
        m_vRotVelocity.z = atof(szBuff2);
        return true;
    }

    //    SRCBLEND..
    if (n3std::iequals(szCommand, "<src_blend>")) {
        if (n3std::iequals(szBuff0, "ONE")) {
            m_dwSrcBlend = D3DBLEND_ONE;
        } else if (n3std::iequals(szBuff0, "ZERO")) {
            m_dwSrcBlend = D3DBLEND_ZERO;
        } else if (n3std::iequals(szBuff0, "SRCCOLOR")) {
            m_dwSrcBlend = D3DBLEND_SRCCOLOR;
        } else if (n3std::iequals(szBuff0, "INVSRCCOLOR")) {
            m_dwSrcBlend = D3DBLEND_INVSRCCOLOR;
        } else if (n3std::iequals(szBuff0, "SRCALPHA")) {
            m_dwSrcBlend = D3DBLEND_SRCALPHA;
        } else if (n3std::iequals(szBuff0, "INVSRCALPHA")) {
            m_dwSrcBlend = D3DBLEND_INVSRCALPHA;
        }
        return true;
    }

    //    SRCBLEND..
    if (n3std::iequals(szCommand, "<dest_blend>")) {
        if (n3std::iequals(szBuff0, "ONE")) {
            m_dwDestBlend = D3DBLEND_ONE;
        } else if (n3std::iequals(szBuff0, "ZERO")) {
            m_dwDestBlend = D3DBLEND_ZERO;
        } else if (n3std::iequals(szBuff0, "SRCCOLOR")) {
            m_dwDestBlend = D3DBLEND_SRCCOLOR;
        } else if (n3std::iequals(szBuff0, "INVSRCCOLOR")) {
            m_dwDestBlend = D3DBLEND_INVSRCCOLOR;
        } else if (n3std::iequals(szBuff0, "SRCALPHA")) {
            m_dwDestBlend = D3DBLEND_SRCALPHA;
        } else if (n3std::iequals(szBuff0, "INVSRCALPHA")) {
            m_dwDestBlend = D3DBLEND_INVSRCALPHA;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<alpha>")) {
        m_dwRenderFlag |= RF_ALPHABLENDING;
        if (n3std::iequals(szBuff0, "TRUE")) {
            m_bAlpha = TRUE;
        } else if (n3std::iequals(szBuff0, "FALSE")) {
            m_bAlpha = FALSE;
            m_dwRenderFlag ^= RF_ALPHABLENDING;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<fadeout>")) {
        m_fFadeOut = atof(szBuff0);
        return true;
    }

    if (n3std::iequals(szCommand, "<fadein>")) {
        m_fFadeIn = atof(szBuff0);
        return true;
    }

    if (n3std::iequals(szCommand, "<on_ground>")) {
        if (n3std::iequals(szBuff0, "TRUE")) {
            m_bOnGround = true;
        } else if (n3std::iequals(szBuff0, "FALSE")) {
            m_bOnGround = false;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<doubleside>")) {
        m_dwRenderFlag |= RF_DOUBLESIDED;
        if (n3std::iequals(szBuff0, "true")) {
            m_dwDoubleSide = D3DCULL_NONE;
        }
        if (n3std::iequals(szBuff0, "false")) {
            m_dwDoubleSide = D3DCULL_CCW;
            m_dwRenderFlag ^= RF_DOUBLESIDED;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<light>")) {
        m_dwRenderFlag |= RF_NOTUSELIGHT;
        if (n3std::iequals(szBuff0, "true")) {
            m_dwLight = TRUE;
            m_dwRenderFlag ^= RF_NOTUSELIGHT;
        }
        if (n3std::iequals(szBuff0, "false")) {
            m_dwLight = FALSE;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<zbuffer>")) {
        m_dwRenderFlag |= RF_NOTZBUFFER;
        if (n3std::iequals(szBuff0, "true")) {
            m_dwZEnable = D3DZB_TRUE;
            m_dwRenderFlag ^= RF_NOTZBUFFER;
        }
        if (n3std::iequals(szBuff0, "false")) {
            m_dwZEnable = D3DZB_FALSE;
        }
        return true;
    }

    if (n3std::iequals(szCommand, "<zwrite>")) {
        m_dwRenderFlag |= RF_NOTZWRITE;
        if (n3std::iequals(szBuff0, "true")) {
            m_dwZWrite = TRUE;
            m_dwRenderFlag ^= RF_NOTZWRITE;
        }
        if (n3std::iequals(szBuff0, "false")) {
            m_dwZWrite = FALSE;
        }
        return true;
    }

    return false;
}
#endif // end of _N3TOOL

//
//    Decode Script File
//    스크립트 파일 읽고 해석.(call parse script..)
//
#ifdef _N3TOOL
bool CN3FXPartBase::DecodeScriptFile(const fs::path & fsFile) {
    // fs::path fsFxpPath = fs::path(fsFile).replace_extension(".fxp");
    // CN3BaseFileAccess::FilePathSet(fsFxpPath);
    CN3BaseFileAccess::FilePathSet(fsFile);

    FILE * stream = _wfopen(fsFile.c_str(), L"r");
    if (!stream) {
        return false;
    }

    char   szLine[512]{}, szCommand[80]{}, szBuf[4][80]{};
    char * pResult = fgets(szLine, sizeof(szLine), stream);
    sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

    if (!n3std::iequals(szCommand, "<n3fxpart>")) {
        fclose(stream);
        return false;
    }

    while (!feof(stream)) {
        char * pResult = fgets(szLine, sizeof(szLine), stream);
        if (pResult == NULL) {
            continue;
        }

        memset(szCommand, 0, sizeof(szCommand));
        memset(szBuf, 0, sizeof(szBuf));
        sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
        ParseScript(szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
    }
    fclose(stream);
    Init();

    return true;
}
#endif // end of _N3TOOL

//
//    init...변수 초기화..
//
void CN3FXPartBase::Init() {
    m_fCurrLife = 0.0f;
    m_vCurrVelocity = m_vVelocity;
    m_vCurrPos = m_vPos;
}

//
//    start...파트 구동 시작...
//
void CN3FXPartBase::Start() {
    m_dwState = FX_PART_STATE_LIVE;
}

//
//    stop..
//    파트 멈춤시도...
//    이함수 호출한다고 파트가 바로 끝나는건 아니다..끝내는 과정을 시작하는 거다..
//    실질적인 끝맺음은 tick에서 할껄...^^
//
void CN3FXPartBase::Stop() {
    m_dwState = FX_PART_STATE_DYING;
    m_fCurrLife = m_fLife + m_fFadeIn;
}

//
//    Tick...
//
bool CN3FXPartBase::Tick() {
    if (m_dwState == FX_PART_STATE_DEAD || m_dwState == FX_PART_STATE_READY) {
        return false;
    }

    m_fCurrLife += CN3Base::s_fSecPerFrm;
    if (m_fLife > 0.0f && m_dwState == FX_PART_STATE_LIVE) {
        if (m_fCurrLife >= (m_fLife + m_fFadeIn)) {
            Stop();
        }
    }
    if (m_dwState == FX_PART_STATE_DYING) {
        if (IsDead()) {
            m_dwState = FX_PART_STATE_DEAD;
            Init();
            return false;
        }
    }
    return true;
}

//
//
//
bool CN3FXPartBase::IsDead() {
    return true;
}

//
//    Render..
//
void CN3FXPartBase::Render() {}

//
//    load...
//
bool CN3FXPartBase::Load(HANDLE hFile) {
    unsigned char cTmp;
    DWORD         dwRWC = 0;

    ReadFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);
    m_iVersion = (int)cTmp;

    ReadFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);
    m_iBaseVersion = (int)cTmp;

    ReadFile(hFile, &m_fLife, sizeof(float), &dwRWC, NULL);
    if (m_fLife > 10.0f) {
        m_fLife = 10.0f;
    }

    ReadFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);
    m_iType = (int)cTmp;

    ReadFile(hFile, &m_vVelocity, sizeof(__Vector3), &dwRWC, NULL);
    ReadFile(hFile, &m_vAcceleration, sizeof(__Vector3), &dwRWC, NULL);
    ReadFile(hFile, &m_vRotVelocity, sizeof(__Vector3), &dwRWC, NULL);

    ReadFile(hFile, &m_bOnGround, sizeof(bool), &dwRWC, NULL);

    ReadFile(hFile, &m_vPos, sizeof(__Vector3), &dwRWC, NULL);

    ReadFile(hFile, &m_iNumTex, sizeof(int), &dwRWC, NULL);
    ReadFile(hFile, &m_fTexFPS, sizeof(float), &dwRWC, NULL);

    char szTexFileBase[260]{};
    ReadFile(hFile, &szTexFileBase, sizeof(szTexFileBase), &dwRWC, NULL);
    m_fsTexFileBase = fs::path(szTexFileBase).generic_string();

    if (m_iBaseVersion < 2) {
        ReadFile(hFile, &m_bAlpha, sizeof(BOOL), &dwRWC, NULL);
        ReadFile(hFile, &m_dwSrcBlend, sizeof(DWORD), &dwRWC, NULL);
        ReadFile(hFile, &m_dwDestBlend, sizeof(DWORD), &dwRWC, NULL);

        ReadFile(hFile, &m_fFadeOut, sizeof(float), &dwRWC, NULL);
        ReadFile(hFile, &m_fFadeIn, sizeof(float), &dwRWC, NULL);
    }
    if (m_iBaseVersion >= 2) {
        ReadFile(hFile, &m_dwSrcBlend, sizeof(DWORD), &dwRWC, NULL);
        ReadFile(hFile, &m_dwDestBlend, sizeof(DWORD), &dwRWC, NULL);

        ReadFile(hFile, &m_fFadeOut, sizeof(float), &dwRWC, NULL);
        ReadFile(hFile, &m_fFadeIn, sizeof(float), &dwRWC, NULL);

        ReadFile(hFile, &m_dwRenderFlag, sizeof(DWORD), &dwRWC, NULL);

        if (m_dwRenderFlag & RF_NOTZBUFFER) {
            m_dwZEnable = D3DZB_FALSE;
        } else {
            m_dwZEnable = D3DZB_TRUE;
        }
        if (m_dwRenderFlag & RF_NOTZWRITE) {
            m_dwZWrite = FALSE;
        } else {
            m_dwZWrite = TRUE;
        }
        if (m_dwRenderFlag & RF_DOUBLESIDED) {
            m_dwDoubleSide = D3DCULL_NONE;
        } else {
            m_dwDoubleSide = D3DCULL_CCW;
        }
        if (m_dwRenderFlag & RF_NOTUSELIGHT) {
            m_dwLight = FALSE;
        } else {
            m_dwLight = TRUE;
        }
        if (m_dwRenderFlag & RF_ALPHABLENDING) {
            m_bAlpha = TRUE;
        } else {
            m_bAlpha = FALSE;
        }
    }

    m_ppRefTex = new CN3Texture *[m_iNumTex];
    for (int i = 0; i < m_iNumTex; i++) {
        m_ppRefTex[i] = CN3Base::s_MngTex.Get(m_fsTexFileBase + std::format("{:04d}.dxt", i));
    }

    return true;
}

//
//
//
bool CN3FXPartBase::Save(HANDLE hFile) {
    unsigned char cTmp;
    DWORD         dwRWC = 0;

    cTmp = (unsigned char)m_iVersion;
    WriteFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);

    cTmp = (unsigned char)m_iBaseVersion;
    WriteFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);

    WriteFile(hFile, &m_fLife, sizeof(float), &dwRWC, NULL);

    cTmp = (unsigned char)m_iType;
    WriteFile(hFile, &cTmp, sizeof(unsigned char), &dwRWC, NULL);

    WriteFile(hFile, &m_vVelocity, sizeof(__Vector3), &dwRWC, NULL);
    WriteFile(hFile, &m_vAcceleration, sizeof(__Vector3), &dwRWC, NULL);
    WriteFile(hFile, &m_vRotVelocity, sizeof(__Vector3), &dwRWC, NULL);

    WriteFile(hFile, &m_bOnGround, sizeof(bool), &dwRWC, NULL);

    WriteFile(hFile, &m_vPos, sizeof(__Vector3), &dwRWC, NULL);

    WriteFile(hFile, &m_iNumTex, sizeof(int), &dwRWC, NULL);

    WriteFile(hFile, &m_fTexFPS, sizeof(float), &dwRWC, NULL);

    char szTexFileBase[260]{};
    fs::path(m_fsTexFileBase).normalize('/', '\\').string().copy(szTexFileBase, sizeof(szTexFileBase) - 1);
    WriteFile(hFile, szTexFileBase, sizeof(szTexFileBase), &dwRWC, NULL);

    /* 
    if(m_iBaseVersion<2)
    {
        WriteFile(hFile, &m_bAlpha, sizeof(BOOL), &dwRWC, NULL);
        WriteFile(hFile, &m_dwSrcBlend, sizeof(DWORD), &dwRWC, NULL);
        WriteFile(hFile, &m_dwDestBlend, sizeof(DWORD), &dwRWC, NULL);

        WriteFile(hFile, &m_fFadeOut, sizeof(float), &dwRWC, NULL);
        WriteFile(hFile, &m_fFadeIn, sizeof(float), &dwRWC, NULL);
    }
    //*/

    // m_iBaseVersion >= 2
    //
    WriteFile(hFile, &m_dwSrcBlend, sizeof(DWORD), &dwRWC, NULL);
    WriteFile(hFile, &m_dwDestBlend, sizeof(DWORD), &dwRWC, NULL);

    WriteFile(hFile, &m_fFadeOut, sizeof(float), &dwRWC, NULL);
    WriteFile(hFile, &m_fFadeIn, sizeof(float), &dwRWC, NULL);

    m_dwRenderFlag = RF_ALPHABLENDING | RF_NOTUSEFOG | RF_DIFFUSEALPHA | RF_NOTUSELIGHT | RF_DOUBLESIDED |
                     RF_NOTZWRITE | RF_NOTZBUFFER;
    if (m_dwZEnable == D3DZB_TRUE) {
        m_dwRenderFlag ^= RF_NOTZBUFFER;
    }
    if (m_dwZWrite == TRUE) {
        m_dwRenderFlag ^= RF_NOTZWRITE;
    }
    if (m_dwDoubleSide != D3DCULL_NONE) {
        m_dwRenderFlag ^= RF_DOUBLESIDED;
    }
    if (m_dwLight == TRUE) {
        m_dwRenderFlag ^= RF_NOTUSELIGHT;
    }
    if (m_bAlpha != TRUE) {
        m_dwRenderFlag ^= RF_ALPHABLENDING;
    }

    WriteFile(hFile, &m_dwRenderFlag, sizeof(DWORD), &dwRWC, NULL);

    return true;
}

void CN3FXPartBase::Duplicate(CN3FXPartBase * pSrc) {
    m_iVersion = pSrc->m_iVersion;
    m_fLife = pSrc->m_fLife;
    m_iType = pSrc->m_iType;

    m_vVelocity = pSrc->m_vVelocity;
    m_vAcceleration = pSrc->m_vAcceleration;
    m_vRotVelocity = pSrc->m_vRotVelocity;

    m_bOnGround = pSrc->m_bOnGround;

    m_vPos = pSrc->m_vPos;
    m_iNumTex = pSrc->m_iNumTex;
    m_fTexFPS = pSrc->m_fTexFPS;
    m_fsTexFileBase = pSrc->m_fsTexFileBase;

    m_dwZEnable = pSrc->m_dwZEnable;
    m_dwZWrite = pSrc->m_dwZWrite;
    m_dwLight = pSrc->m_dwLight;
    m_dwDoubleSide = pSrc->m_dwDoubleSide;

    m_bAlpha = pSrc->m_bAlpha;
    m_dwSrcBlend = pSrc->m_dwSrcBlend;
    m_dwDestBlend = pSrc->m_dwDestBlend;

    m_fFadeOut = pSrc->m_fFadeOut;
    m_fFadeIn = pSrc->m_fFadeIn;

    m_ppRefTex = new CN3Texture *[m_iNumTex];

    for (int i = 0; i < m_iNumTex; i++) {
        m_ppRefTex[i] = CN3Base::s_MngTex.Get(m_fsTexFileBase + std::format("{:04d}.dxt", i));
    }
}
