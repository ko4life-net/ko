// PvsMgr.h: interface for the CPvsMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Base/N3BaseFileAccess.h"
#include "N3Base/N3ShapeExtra.h"

#include "PortalVolume.h"
#include <string>

const int ciVersion = 1;

typedef typename std::list<CPortalVolume *>::iterator iter;

class CPvsMgr : public CN3BaseFileAccess {
    friend class CDungeonManager;
    friend class CPortalVolume;

    const fs::path m_fsIndoorDir;
    const float    m_fVolumeOffs; // Volume 체크 높이..

    std::list<CPortalVolume *> m_pPvsList;

    //.. Current Portal Volume.. ^^
    CPortalVolume * m_pCurVol;

  public:
    static CN3Mng<class CN3Shape>      s_MngShape;
    static CN3Mng<class CN3ShapeExtra> s_MngShapeExt;
    static std::list<ShapeInfo *>      s_plShapeInfoList;
    static ShapeInfo *                 GetShapeInfoByManager(int iID);

  private:
    void Tick(bool bWarp = false, __Vector3 vPos = __Vector3());
    void Render();

    void DeleteAllPvsObj();

    // String Cryptograph.. ^^
    static std::string ReadDecryptString(HANDLE hFile);
    const fs::path &   GetIndoorFolderPath() { return m_fsIndoorDir; }

    CPortalVolume * GetPortalVolPointerByID(int iID);

    ////////////////////////////////////////////////////////////////
    bool       CheckCollisionCameraWithTerrain(__Vector3 & vEyeResult, const __Vector3 & vAt, float fNP);
    bool       CheckCollisionCameraWithShape(__Vector3 & vEyeResult, const __Vector3 & vAt, float fNP);
    float      GetHeightWithTerrain(float x, float z, bool bWarp = false);
    float      GetHeightNearstPosWithShape(const __Vector3 & vPos, float fDist, __Vector3 * pvNormal = NULL);
    bool       IsInTerrainWithTerrain(float x, float z, __Vector3 vPosBefore = __Vector3());
    float      GetHeightWithShape(float fX, float fZ, __Vector3 * pvNormal = NULL);
    BOOL       PickWideWithTerrain(int x, int y, __Vector3 & vPick);
    CN3Shape * PickWithShape(int iXScreen, int iYScreen, bool bMustHaveEvent, __Vector3 * pvPick = NULL);
    CN3Shape * ShapeGetByIDWithShape(int iID);
    bool       CheckCollisionWithShape(const __Vector3 & vPos,         // 충돌 위치
                                       const __Vector3 & vDir,         // 방향 벡터
                                       float             fSpeedPerSec, // 초당 움직이는 속도
                                       __Vector3 *       pvCol,        // 충돌 지점
                                       __Vector3 *       pvNormal,     // 충돌한면의 법선벡터
                                       __Vector3 *       pVec);              // 충돌한 면 의 폴리곤 __Vector3[3]

  public:
    CPvsMgr();
    virtual ~CPvsMgr();

    //..
    bool Load(HANDLE hFile);
    bool LoadOldVersion(HANDLE hFile, int iVersionFromData);
};
