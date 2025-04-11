// ItemBundleMgr.cpp: implementation of the CItemBundleMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBundleMgr.h"
#include "GameProcMain.h"
#include "N3Terrain.h"
#include "APISocket.h"
#include "PacketDef.h"
#include "N3TerrainManager.h"
#include "N3WorldManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemBundle::CItemBundle(int iBundleID, float fTimeToLive) {
    m_iBundleID = iBundleID;
    m_fTimeToLive = fTimeToLive;
}

CItemBundle::~CItemBundle() {}

void CItemBundle::Tick() {
    m_fTimeToLive -= s_fSecPerFrm;
    if (IsAlive()) {
        CN3Shape::Tick();
    }
}

CItemBundleMgr::CItemBundleMgr() {}

CItemBundleMgr::~CItemBundleMgr() {
    ReleaseBundles();
}

void CItemBundleMgr::Release() {
    ReleaseBundles();
}

void CItemBundleMgr::ReleaseBundles() {
    CItemBundle * pItem = NULL;
    it_ItemBundle it = m_Bundles.begin(), itEnd = m_Bundles.end();
    for (; it != itEnd; it++) {
        pItem = (*it).second;
        delete pItem;
    }
    m_Bundles.clear();
}

void CItemBundleMgr::Tick() {
    CItemBundle * pItem = NULL;
    it_ItemBundle it = m_Bundles.begin(), itEnd = m_Bundles.end();
    for (; it != itEnd;) {
        pItem = (*it).second;

        pItem->Tick();
        if (pItem->IsAlive()) {
            it++;
        } else {
            delete pItem;
            it = m_Bundles.erase(it);
        }
    }
}

void CItemBundleMgr::Render() {
    CItemBundle * pItem = NULL;
    it_ItemBundle it = m_Bundles.begin(), itEnd = m_Bundles.end();
    for (; it != itEnd; it++) {
        pItem = (*it).second;
        pItem->Render();
    }
}

CItemBundle * CItemBundleMgr::Pick(int ixScreen, int iyScreen) {
    CItemBundle * pItemBundle = NULL;
    CN3VMesh *    pVMesh = NULL;
    it_ItemBundle it = m_Bundles.begin(), itEnd = m_Bundles.end();

    __Vector3 vPos, vDir;
    ::_Convert2D_To_3DCoordinate(ixScreen, iyScreen, s_CameraData.mtxView, s_CameraData.mtxProjection, s_CameraData.vp,
                                 vPos, vDir);

    for (; it != itEnd; it++) {
        pItemBundle = (*it).second;

        CN3VMesh * pvMesh = pItemBundle->CollisionMesh();
        if (NULL != pvMesh && pvMesh->Pick(pItemBundle->m_Matrix, vPos, vDir)) {
            return pItemBundle;
        }
    }

    return NULL;
}

bool CItemBundleMgr::Add(__Vector3 vPos, int iBundleID, float fTimeToLive) {
    if (m_Bundles.end() != m_Bundles.find(iBundleID)) {
        return false;
    }
    if (fTimeToLive <= 0.0f) {
        return false;
    }

    CItemBundle * pIB = new CItemBundle(iBundleID, fTimeToLive);
    pIB->LoadFromFile("Misc\\itembox.n3shape");

    vPos.y = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);
    pIB->PosSet(vPos.x, vPos.y + 0.01, vPos.z);

    m_Bundles.insert(std::map<int, CItemBundle *>::value_type(iBundleID, pIB));
    return true;
}
