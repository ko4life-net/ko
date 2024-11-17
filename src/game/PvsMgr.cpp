// PvsMgr.cpp: implementation of the CPvsMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PvsMgr.h"
#include "GameBase.h"
#include "PlayerMySelf.h"
#include "GameProcedure.h"
#include "GameEng.h"

#include "N3Base/N3Camera.h"
#include "N3Base/N3ShapeMgr.h"

CN3Mng<class CN3Shape>      CPvsMgr::s_MngShape;
CN3Mng<class CN3ShapeExtra> CPvsMgr::s_MngShapeExt;
std::list<ShapeInfo *>      CPvsMgr::s_plShapeInfoList;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPvsMgr::CPvsMgr()
    : m_fsIndoorDir("N3Indoor")
    , m_fVolumeOffs(0.6f) //..
{
    s_plShapeInfoList.clear();
    m_pCurVol = NULL;
}

CPvsMgr::~CPvsMgr() {
    DeleteAllPvsObj();
}

void CPvsMgr::DeleteAllPvsObj() {
    iter it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        CPortalVolume * pvs = *it;
        if (pvs) {
            delete pvs;
        }
        it++;
    }
    m_pPvsList.clear();

    ShapeInfo * pSI;
    siiter      siit = s_plShapeInfoList.begin();
    while (siit != s_plShapeInfoList.end()) {
        pSI = *siit++;
        delete pSI;
    }
    s_plShapeInfoList.clear();
    s_MngShape.Release();
    s_MngShapeExt.Release();
}

ShapeInfo * CPvsMgr::GetShapeInfoByManager(int iID) {
    ShapeInfo * pSI;
    siiter      siit = s_plShapeInfoList.begin();
    while (siit != s_plShapeInfoList.end()) {
        pSI = *siit++;
        if (pSI->m_iID == iID) {
            return pSI;
        }
    }

    return NULL;
}

void CPvsMgr::Tick(bool bWarp, __Vector3 vPos) {
    CPortalVolume * pVol = NULL;
    __Vector3       vec = CGameBase::s_pPlayer->Position();
    if (bWarp) {
        vec = vPos;
    }

    vec.y += m_fVolumeOffs;
    m_pCurVol = NULL;

    iter it = m_pPvsList.begin();
    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if (pVol->IsInVolumn(vec)) {
            m_pCurVol = pVol;
            break;
        }
    }

    it = m_pPvsList.begin();
    while (it != m_pPvsList.end()) {
        pVol = *it++;
        pVol->m_eRenderType = TYPE_UNKNOWN;
    }

    if (!m_pCurVol) {
        return;
    }

    VisPortalPriority vPP;
    vppiter           vppit = m_pCurVol->m_pVisiblePvsList.begin();
    while (vppit != m_pCurVol->m_pVisiblePvsList.end()) {
        vPP = *vppit++;
        vPP.m_pVol->m_eRenderType = TYPE_TRUE;
    }
}

void CPvsMgr::Render() {
    CPortalVolume * pVol = NULL;
    iter            it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if (pVol->m_eRenderType == TYPE_TRUE) {
            pVol->Render();
        }
    }
}

bool CPvsMgr::LoadOldVersion(HANDLE hFile, int iVersionFromData) {
    //..

    return true;
}

bool CPvsMgr::Load(HANDLE hFile) {
    DWORD dwNum;
    int   iT;

    ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);
    if (iT != ciVersion) {
        return LoadOldVersion(hFile, iT);
    }

    // N3Scene 화일.. 안쓴다.. -.-;
    fs::path fsSrcFile = ReadDecryptString(hFile);

    // 전체 이동값.. 안슨다.. -.-;
    ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);
    ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);
    ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);

    int iCount;
    ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

    for (int i = 0; i < iCount; i++) {
        ShapeInfo * pSI = new ShapeInfo;
        ReadFile(hFile, &pSI->m_iID, sizeof(int), &dwNum, NULL);

        // 문자열 길이..
        fsSrcFile = ReadDecryptString(hFile);
        pSI->m_fsShapeFile = m_fsIndoorDir / fsSrcFile.filename();
        pSI->m_pShape = s_MngShape.Get(pSI->m_fsShapeFile);
        __ASSERT(pSI->m_pShape, "Shape Not Found");

        ReadFile(hFile, &pSI->m_iBelong, sizeof(int), &dwNum, NULL);
        ReadFile(hFile, &pSI->m_iEventID, sizeof(int), &dwNum, NULL);
        ReadFile(hFile, &pSI->m_iEventType, sizeof(int), &dwNum, NULL);
        ReadFile(hFile, &pSI->m_iNPC_ID, sizeof(int), &dwNum, NULL);
        ReadFile(hFile, &pSI->m_iNPC_Status, sizeof(int), &dwNum, NULL);
        pSI->Load(hFile);
        s_plShapeInfoList.push_back(pSI);
    }

    // Total Count..
    ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

    CPortalVolume *pVol = NULL, *pVolTo = NULL;
    int            iID;

    for (int i = 0; i < iCount; i++) {
        ReadFile(hFile, &iID, sizeof(int), &dwNum, NULL);
        pVol = new CPortalVolume;
        pVol->m_pManager = this;
        pVol->m_iID = iID;
        pVol->Load(hFile);
        m_pPvsList.push_back(pVol);
    }

    iter              it = m_pPvsList.begin();
    idapiter          idapit;
    IDAndPriority     IDAP;
    VisPortalPriority vPP;

    while (it != m_pPvsList.end()) {
        pVol = *it++;

        idapit = pVol->m_piVisibleIDList.begin();
        while (idapit != pVol->m_piVisibleIDList.end()) {
            IDAP = *idapit++;
            pVolTo = GetPortalVolPointerByID(IDAP.m_iID);
            vPP.m_pVol = pVolTo;
            vPP.m_iPriority = IDAP.m_iPriority;
            pVol->m_pVisiblePvsList.push_back(vPP);
        }

        pVol->m_piVisibleIDList.clear();
    }

    return true;
}

CPortalVolume * CPvsMgr::GetPortalVolPointerByID(int iID) {
    CPortalVolume * pVol = NULL;

    iter it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        // 자신의 데이터 저장..
        pVol = *it++;
        if (pVol->m_iID == iID) {
            return pVol;
        }
    }

    return NULL;
}

#define CRY_KEY 0x0816

std::string CPvsMgr::ReadDecryptString(HANDLE hFile) {
    DWORD dwNum;

    int iLen = 0;
    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);

    std::string szStr;
    if (iLen > 0) {
        szStr.assign(iLen, '\0');
        ReadFile(hFile, szStr.data(), iLen, &dwNum, NULL);
        for (int i = 0; i < iLen; i++) {
            szStr[i] ^= CRY_KEY;
        }
    }

    return szStr;
}

//////////////////////////////////////////////////////////////////////////////////

bool CPvsMgr::CheckCollisionCameraWithTerrain(__Vector3 & vEyeResult, const __Vector3 & vAt, float fNP) {
    fNP = (vAt - vEyeResult).Magnitude();
    bool  bCollision = false;
    float fNPMin = fNP, fTemp = 0.0f;

    CPortalVolume * pVol = NULL;
    iter            it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if ((pVol->m_eRenderType == TYPE_TRUE) && pVol->CheckCollisionCameraWithTerrain(vEyeResult, vAt, fNP)) {
            bCollision = true;
            fTemp = (vEyeResult - vAt).Magnitude();
            if (fTemp < fNPMin) {
                fNPMin = fTemp;
            }
        }
    }

    if (bCollision && (fNPMin < fNP)) {
        __Vector3 vT;
        vT.Zero();
        vT = vEyeResult - vAt;
        vT.Normalize();
        vT *= fNPMin;
        vEyeResult = vT + vAt;
    }

    return bCollision;
}

bool CPvsMgr::CheckCollisionCameraWithShape(__Vector3 & vEyeResult, const __Vector3 & vAt, float fNP) {
    fNP = (vAt - vEyeResult).Magnitude();
    bool  bCollision = false;
    float fNPMin = fNP, fTemp = 0.0f;

    CPortalVolume * pVol = NULL;
    iter            it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if ((pVol->m_eRenderType == TYPE_TRUE) && pVol->CheckCollisionCameraWithShape(vEyeResult, vAt, fNP)) {
            bCollision = true;
            fTemp = (vEyeResult - vAt).Magnitude();
            if (fTemp < fNPMin) {
                fNPMin = fTemp;
            }
        }
    }

    if (bCollision && (fNPMin < fNP)) {
        __Vector3 vT;
        vT.Zero();
        vT = vEyeResult - vAt;
        vT.Normalize();
        vT *= fNPMin;
        vEyeResult = vT + vAt;
    }

    return bCollision;
}

float CPvsMgr::GetHeightWithTerrain(float x, float z, bool bWarp) {
    if (!m_pCurVol) {
        Tick();
    }

    float fHeight = FLT_MIN;
    if (!m_pCurVol) {
        return fHeight;
    }

    m_pCurVol->GetHeightWithTerrain(x, z, fHeight);
    return fHeight;
}

float CPvsMgr::GetHeightNearstPosWithShape(const __Vector3 & vPos, float fDist, __Vector3 * pvNormal) {
    float fHeightMax = FLT_MIN, fHeight;

    CPortalVolume * pVol = NULL;
    iter            it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if (pVol->m_eRenderType == TYPE_TRUE) {
            fHeight = pVol->GetHeightNearstPosWithShape(vPos, fDist, pvNormal);
            if (fHeightMax < fHeight) {
                fHeightMax = fHeight;
            }
        }
    }

    return fHeightMax;
}

bool CPvsMgr::IsInTerrainWithTerrain(float x, float z, __Vector3 vPosBefore) {
    if (!m_pCurVol) {
        Tick();
    }

    float fHeight = FLT_MIN;
    if (!m_pCurVol) {
        return false;
    }

    return m_pCurVol->GetHeightWithTerrain(x, z, fHeight);
}

float CPvsMgr::GetHeightWithShape(float fX, float fZ, __Vector3 * pvNormal) {
    float fHeightMax = FLT_MIN, fHeight;

    CPortalVolume *pVol = NULL, *pVolNe = NULL;
    iter           it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        fHeight = pVol->GetHeightNearstPosWithShape(__Vector3(fX, FLT_MIN, fZ), 0.0f);
        if (fHeightMax < fHeight) {
            fHeightMax = fHeight;
            pVolNe = pVol;
        }
    }

    __Vector3 vPos;
    vPos.Set(fX, fHeightMax, fZ);
    if (pVolNe && pVolNe->m_eRenderType != TYPE_TRUE) {
        Tick(true, vPos);
    }

    return fHeightMax;
}

BOOL CPvsMgr::PickWideWithTerrain(int x, int y, __Vector3 & vPick) {
    BOOL      bColl = FALSE;
    __Vector3 vCamPo = CGameProcedure::s_pEng->CameraGetActive()->EyePos(), vPT;
    float     fDistMax = FLT_MAX, fDT;

    CPortalVolume *pVol = NULL, *pVolNe = NULL;
    iter           it = m_pPvsList.begin();

    while (it != m_pPvsList.end()) {
        pVol = *it++;
        if ((pVol->m_eRenderType == TYPE_TRUE) && (pVol->PickWideWithTerrain(x, y, vPT))) {
            fDT = (vPT - vCamPo).Magnitude();
            if (fDT <= fDistMax) {
                fDistMax = fDT;
                vPick = vPT;
                bColl = TRUE;
            }
        }
    }

    return bColl;
}

CN3Shape * CPvsMgr::PickWithShape(int iXScreen, int iYScreen, bool bMustHaveEvent, __Vector3 * pvPick) {
    if (!m_pCurVol) {
        Tick();
    }

    if (!m_pCurVol) {
        return NULL;
    }

    return m_pCurVol->PickWithShape(iXScreen, iYScreen, bMustHaveEvent, pvPick);
}

bool CPvsMgr::CheckCollisionWithShape(const __Vector3 & vPos,         // 충돌 위치
                                      const __Vector3 & vDir,         // 방향 벡터
                                      float             fSpeedPerSec, // 초당 움직이는 속도
                                      __Vector3 *       pvCol,        // 충돌 지점
                                      __Vector3 *       pvNormal,     // 충돌한면의 법선벡터
                                      __Vector3 *       pVec)               // 충돌한 면 의 폴리곤 __Vector3[3]
{
    if (!m_pCurVol) {
        Tick();
    }

    if (!m_pCurVol) {
        return NULL;
    }

    return m_pCurVol->CheckCollisionWithShape(vPos, vDir, fSpeedPerSec, pvCol, pvNormal, pVec);
}

CN3Shape * CPvsMgr::ShapeGetByIDWithShape(int iID) {
    if (!m_pCurVol) {
        Tick();
    }

    if (!m_pCurVol) {
        return NULL;
    }

    return m_pCurVol->ShapeGetByIDWithShape(iID);
}
