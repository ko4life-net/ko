// N3FXBundle.cpp: implementation of the CN3FXBundle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXBundle.h"
#include "N3FXPartParticles.h"
#include "N3FXPartBillBoard.h"
#include "N3FXPartMesh.h"
#include "N3FXPartBottomBoard.h"

#include "N3SndMgr.h"
#include "N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float CN3FXBundle::m_fEffectSndDist = 48.0f;

CN3FXBundle::CN3FXBundle() {
    m_iVersion = 2;
    m_strName.erase();
    for (int i = 0; i < MAX_FX_PART; i++) {
        m_pPart[i] = NULL;
    }
    m_fLife0 = 0.0f;

    m_dwState = FX_BUNDLE_STATE_DEAD;
    m_fLife = 0.0f;

    m_vPos.Set(0.0f, 0.0f, 0.0f);
    m_vDir.Set(0.0f, 0.0f, 1.0f);
    m_vDestPos.Set(0.0f, 0.0f, 0.0f);

    m_iMoveType = FX_BUNDLE_MOVE_NONE;
    m_fVelocity = 0.0f;

    m_iSourceID = 0;
    m_iTargetID = 0;
    m_iTargetJoint = 0;
    m_iSourceJoint = 0;

    m_bDependScale = false;

    //m_vTargetScale.Set(1,1,1);
    m_fTargetScale = 1.0f;

    m_bStatic = false;

    m_pSndObj = NULL;
}

CN3FXBundle::~CN3FXBundle() {
    m_strName.erase();

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i]) {
            if (m_pPart[i]->pPart) {
                delete m_pPart[i]->pPart;
                m_pPart[i]->pPart = NULL;
            }
            delete m_pPart[i];
            m_pPart[i] = NULL;
        }
    }

#ifdef _N3GAME
    if (m_pSndObj) {
        CN3Base::s_SndMgr.ReleaseObj(&m_pSndObj);
    }
#endif
}

//
//    decode script file..
//    스크립트 파일 읽고 해석시킴...
//
#ifdef _N3TOOL
bool CN3FXBundle::DecodeScriptFile(const fs::path & fsFile) {
    FILE * stream = _wfopen(fsFile.c_str(), L"r");
    if (!stream) {
        return false;
    }

    fs::path fsFxbFile = fs::path(fsFile).replace_extension(".fxb");
    CN3BaseFileAccess::FilePathSet(fsFxbFile);

    char   szLine[512]{}, szCommand[80]{}, szBuf[4][80]{};
    char * pResult = fgets(szLine, sizeof(szLine), stream);
    sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

    if (!n3std::iequals(szCommand, "<n3fxbundle>")) {
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

        if (n3std::iequals(szCommand, "<name>")) {
            m_strName = szBuf[0];
            continue;
        }

        if (n3std::iequals(szCommand, "<part>")) {
            fs::path fsFile = CN3Base::PathGet() / szBuf[0];

            FXPARTWITHSTARTTIME * pPart = new FXPARTWITHSTARTTIME;
            pPart->fStartTime = atof(szBuf[1]);
            pPart->pPart = SetPart(fsFile);

            if (!(pPart->pPart)) {
                delete pPart;
                continue;
            }

            for (int i = 0; i < MAX_FX_PART; i++) {
                if (!m_pPart[i]) {
                    m_pPart[i] = pPart;
                    break;
                }
            }
            continue;
        }
        if (n3std::iequals(szCommand, "<velocity>")) {
            m_fVelocity = atof(szBuf[0]);
            continue;
        }
        if (n3std::iequals(szCommand, "<depend_scale>")) {
            if (n3std::iequals(szBuf[0], "true")) {
                m_bDependScale = true;
            } else {
                m_bDependScale = false;
            }
            continue;
        }
        if (n3std::iequals(szCommand, "<Static_Pos>")) {
            if (n3std::iequals(szBuf[0], "true")) {
                m_bStatic = true;
            } else {
                m_bStatic = false;
            }
            continue;
        }
    }
    fclose(stream);

    Init();

    return true;
}
#endif // end of _N3TOOL

//
//    GetPartType...
//    파트의 파일이름으로 타입을 알아내자..
//
#ifdef _N3TOOL
CN3FXPartBase * CN3FXBundle::SetPart(const fs::path & fsFile) {
    int PartType = FX_PART_TYPE_NONE;

    FILE * stream = _wfopen(fsFile.c_str(), L"r");
    if (!stream) {
        return NULL;
    }

    char   szLine[512]{}, szCommand[80]{}, szBuf[4][80]{};
    char * pResult = fgets(szLine, sizeof(szLine), stream);
    sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

    if (!n3std::iequals(szCommand, "<n3fxPart>")) {
        fclose(stream);
        return NULL;
    }

    while (!feof(stream)) {
        char * pResult = fgets(szLine, sizeof(szLine), stream);
        if (pResult == NULL) {
            continue;
        }

        memset(szCommand, 0, sizeof(szCommand));
        memset(szBuf, 0, sizeof(szBuf));

        sscanf(szLine, "%s %s %s %s %s", szCommand, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);

        if (n3std::iequals(szCommand, "<type>")) {
            if (n3std::iequals(szBuf[0], "particle")) {
                PartType = FX_PART_TYPE_PARTICLE;
            } else if (n3std::iequals(szBuf[0], "board")) {
                PartType = FX_PART_TYPE_BOARD;
            } else if (n3std::iequals(szBuf[0], "mesh")) {
                PartType = FX_PART_TYPE_MESH;
            } else if (n3std::iequals(szBuf[0], "ground")) {
                PartType = FX_PART_TYPE_BOTTOMBOARD;
            }
            //^^v 더 넣을꺼 있으면 넣어라..
        }
    }
    fclose(stream);

    CN3FXPartBase * pPart;
    if (PartType == FX_PART_TYPE_PARTICLE) {
        pPart = new CN3FXPartParticles;
        pPart->m_pRefBundle = this;
        pPart->m_pRefPrevPart = NULL;
        pPart->DecodeScriptFile(fsFile);
        return pPart;
    } else if (PartType == FX_PART_TYPE_BOARD) {
        pPart = new CN3FXPartBillBoard;
        pPart->m_pRefBundle = this;
        pPart->m_pRefPrevPart = NULL;
        pPart->DecodeScriptFile(fsFile);
        return pPart;
    } else if (PartType == FX_PART_TYPE_MESH) {
        pPart = new CN3FXPartMesh;
        pPart->m_pRefBundle = this;
        pPart->m_pRefPrevPart = NULL;
        pPart->DecodeScriptFile(fsFile);
        return pPart;
    } else if (PartType == FX_PART_TYPE_BOTTOMBOARD) {
        pPart = new CN3FXPartBottomBoard;
        pPart->m_pRefBundle = this;
        pPart->m_pRefPrevPart = NULL;
        pPart->DecodeScriptFile(fsFile);
        return pPart;
    }
    return NULL;
}
#endif // end of _N3TOOL

//
//    Init..
//
void CN3FXBundle::Init() {
    m_fLife = 0.0f;
    m_dwState = FX_BUNDLE_STATE_DEAD;

    m_vPos.Set(0.0f, 0.0f, 0.0f);
    m_vDir.Set(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            m_pPart[i]->pPart->Init();
        }
    }
}

//
//
//
bool CN3FXBundle::Load(HANDLE hFile) {
    DWORD dwRWC = 0;

    ReadFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    ReadFile(hFile, &m_fLife0, sizeof(float), &dwRWC, NULL);
    if (m_fLife0 > 10.0f) {
        m_fLife0 = 10.0f;
    }
    ReadFile(hFile, &m_fVelocity, sizeof(float), &dwRWC, NULL);

    ReadFile(hFile, &m_bDependScale, sizeof(bool), &dwRWC, NULL);

    if (m_iVersion == 0) {
        for (int i = 0; i < 8; i++) {
            int iType;

            ReadFile(hFile, &iType, sizeof(int), &dwRWC, NULL);

            if (iType == FX_PART_TYPE_NONE) {
                continue;
            }

            else if (iType == FX_PART_TYPE_PARTICLE) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartParticles;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_PARTICLE;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_BOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBillBoard;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOARD;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_MESH) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartMesh;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_MESH;
                m_pPart[i]->pPart->Load(hFile);
            } else if (iType == FX_PART_TYPE_BOTTOMBOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBottomBoard;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;
                m_pPart[i]->pPart->Load(hFile);
            }
        }
    }
    if (m_iVersion >= 1) {
        for (int i = 0; i < MAX_FX_PART; i++) {
            int iType;

            ReadFile(hFile, &iType, sizeof(int), &dwRWC, NULL);

            if (iType == FX_PART_TYPE_NONE) {
                continue;
            }

            else if (iType == FX_PART_TYPE_PARTICLE) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartParticles;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_PARTICLE;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_BOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBillBoard;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOARD;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_MESH) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartMesh;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_MESH;
                m_pPart[i]->pPart->Load(hFile);
            } else if (iType == FX_PART_TYPE_BOTTOMBOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBottomBoard;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;
                m_pPart[i]->pPart->Load(hFile);
            }
        }
    }

    if (m_iVersion >= 2) {
        ReadFile(hFile, &m_bStatic, sizeof(bool), &dwRWC, NULL);
    }

    if (m_iVersion == 3) {
        ReadFile(hFile, &m_bEarthQuake, sizeof(bool), &dwRWC, NULL);
        if (m_bEarthQuake) {
            ReadFile(hFile, &m_fEarthQuakeStartTime, sizeof(float), &dwRWC, NULL);
        } else {
            m_fEarthQuakeStartTime = 0;
        }
    }

    return true;
}

//
//
//
bool CN3FXBundle::Save(HANDLE hFile) {
    DWORD dwRWC = 0;
    WriteFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);
    WriteFile(hFile, &m_fLife0, sizeof(float), &dwRWC, NULL);
    WriteFile(hFile, &m_fVelocity, sizeof(float), &dwRWC, NULL);

    WriteFile(hFile, &m_bDependScale, sizeof(bool), &dwRWC, NULL);

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            WriteFile(hFile, &(m_pPart[i]->pPart->m_iType), sizeof(int), &dwRWC, NULL);
            WriteFile(hFile, &(m_pPart[i]->fStartTime), sizeof(float), &dwRWC, NULL);
            m_pPart[i]->pPart->Save(hFile);
        } else {
            int Type = FX_PART_TYPE_NONE;
            WriteFile(hFile, &Type, sizeof(int), &dwRWC, NULL);
        }
    }

    WriteFile(hFile, &m_bStatic, sizeof(bool), &dwRWC, NULL);

    return true;
}

//
//    Tick...
//
bool CN3FXBundle::Tick() {
    if (m_dwState == FX_BUNDLE_STATE_DEAD) {
        return false;
    }

    m_fLife += CN3Base::s_fSecPerFrm;

    if (m_dwState == FX_BUNDLE_STATE_DYING || m_dwState == FX_BUNDLE_STATE_LIVE) {
        if (CheckAllPartsDead() || (m_fLife0 != 0.0f && m_fLife > m_fLife0)) {
            m_dwState = FX_BUNDLE_STATE_DEAD;
            Init();
            return false;
        }
    }

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            if (m_pPart[i]->fStartTime <= m_fLife && m_pPart[i]->pPart->m_dwState == FX_PART_STATE_READY) {
                m_pPart[i]->pPart->Start();
            }
            m_pPart[i]->pPart->Tick();
        }
    }
    return true;
}

//
//    Render...
//
void CN3FXBundle::Render() {
    if (m_dwState == FX_BUNDLE_STATE_DEAD) {
        return;
    }

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart && (m_pPart[i]->pPart->m_dwState != FX_PART_STATE_DEAD) &&
            (m_pPart[i]->pPart->m_dwState != FX_PART_STATE_READY)) {
            m_pPart[i]->pPart->Render();
        }
    }
}

//
//    Trigger...
//
void CN3FXBundle::Trigger(int iSourceID, int iTargetID, int iTargetJoint, int iSndID) {
    m_dwState = FX_BUNDLE_STATE_LIVE;

    m_iSourceID = iSourceID;
    m_iTargetID = iTargetID;
    m_iTargetJoint = iTargetJoint;

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            m_pPart[i]->pPart->m_dwState = FX_PART_STATE_READY;
        }
    }

#ifdef _N3GAME
    if (iSndID >= 0) {
        float fDist = (s_CameraData.vEye - m_vPos).Magnitude();
        //        if(fDist < 48.0f)
        if (fDist < m_fEffectSndDist) { //this_Snd
            CN3Base::s_SndMgr.PlayOnceAndRelease(iSndID, &m_vPos);
        }
    }

    //    if(iSndID >= 0 && NULL == m_pSndObj)
    //        m_pSndObj = CN3Base::s_SndMgr.CreateObj(iSndID);
    //    if(m_pSndObj) m_pSndObj->Play(&m_vPos);

#endif
}

//
//    Stop...
//    멈추는 단계를 시작한다..
//    정말 멈추는 기능은 tick에서 모든 파트가 다 죽었다고 판단될때 tick에서 수행한다.
//
void CN3FXBundle::Stop(bool immediately) {
    if (m_dwState == FX_BUNDLE_STATE_DEAD) {
        return;
    }

    if (!immediately) {
        m_dwState = FX_BUNDLE_STATE_DYING;
        for (int i = 0; i < MAX_FX_PART; i++) {
            if (m_pPart[i] && m_pPart[i]->pPart) {
                m_pPart[i]->pPart->Stop();
            }
        }
    } else {
        m_dwState = FX_BUNDLE_STATE_DYING;
        Init();
    }
}

//
//    check all parts are dead..
//    if dead return true..
//
bool CN3FXBundle::CheckAllPartsDead() {
    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            if (m_pPart[i]->pPart->m_dwState != FX_PART_STATE_DEAD) {
                return false;
            }
        }
    }
    return true;
}

CN3FXPartBase * CN3FXBundle::GetPart(int i) {
    if (i < 0 || i >= MAX_FX_PART) {
        return NULL;
    }

    if (m_pPart[i]) {
        return m_pPart[i]->pPart;
    }

    return NULL;
}

float CN3FXBundle::GetPartSTime(int i) {
    if (i < 0 || i >= MAX_FX_PART) {
        return -1.0f;
    }

    if (m_pPart[i]) {
        return m_pPart[i]->fStartTime;
    }

    return -1.0f;
}

void CN3FXBundle::SetPartSTime(int i, float time) {
    if (i < 0 || i >= MAX_FX_PART) {
        return;
    }

    if (m_pPart[i]) {
        m_pPart[i]->fStartTime = time;
    }
    return;
}

void CN3FXBundle::Duplicate(CN3FXBundle * pDestBundle) {
    pDestBundle->FilePathSet(FilePath());

    pDestBundle->m_iVersion = m_iVersion;
    pDestBundle->m_fLife0 = m_fLife0;

    pDestBundle->m_fVelocity = m_fVelocity;
    pDestBundle->m_bDependScale = m_bDependScale;
    pDestBundle->m_bStatic = pDestBundle->m_bStatic;

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i]) {
            if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_PARTICLE) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartParticles;
                CN3FXPartParticles * pPart = (CN3FXPartParticles *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_PARTICLE;

                pPart->Duplicate((CN3FXPartParticles *)m_pPart[i]->pPart);
            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_BOARD) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartBillBoard;
                CN3FXPartBillBoard * pPart = (CN3FXPartBillBoard *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_BOARD;

                pPart->Duplicate((CN3FXPartBillBoard *)m_pPart[i]->pPart);
            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_MESH) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartMesh;
                CN3FXPartMesh * pPart = (CN3FXPartMesh *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_MESH;

                pPart->Duplicate((CN3FXPartMesh *)m_pPart[i]->pPart);

            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_BOTTOMBOARD) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartBottomBoard;
                CN3FXPartBottomBoard * pPart = (CN3FXPartBottomBoard *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;

                pPart->Duplicate((CN3FXPartBottomBoard *)m_pPart[i]->pPart);
            }
        }
    }
}
