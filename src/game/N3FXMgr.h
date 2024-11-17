// N3FXMgr.h: interface for the CN3FXMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Base/N3BaseFileAccess.h"
#include "N3Base/N3FXDef.h"
#include "N3Base/N3TableBase.h"

#include <list>
#include <map>
#include "N3UIWndBase.h"

class CN3FXBundleGame;

typedef struct __FXBundleOrigin // 번들에서 파트들 관리할때..
{
    class CN3FXBundleGame * pBundle;
    int                     iNum;
    float                   fLimitedTime;

    __FXBundleOrigin() {
        pBundle = NULL;
        iNum = 0;
        fLimitedTime = 0.0f;
    }
} FXBUNDLEORIGIN, *LPFXBUNDLEORIGIN;

typedef std::map<fs::path, LPFXBUNDLEORIGIN> stlMAP_BUNDLEORIGIN;
typedef stlMAP_BUNDLEORIGIN::value_type      stlMAP_BUNDLEORIGIN_VALUE;
typedef stlMAP_BUNDLEORIGIN::iterator        stlMAP_BUNDLEORIGIN_IT;

typedef std::list<CN3FXBundleGame *> stlLIST_BUNDLEGAME;
//typedef stlLIST_BUNDLEGAME::value_type stlLIST_BUNDLEGAME_VALUE;
typedef stlLIST_BUNDLEGAME::iterator stlLIST_BUNDLEGAME_IT;

class CN3FXMgr : public CGameBase {
  public:
    //std::list<CN3FXBundleGame*>        m_ListBundle;
    stlLIST_BUNDLEGAME m_ListBundle;

    stlMAP_BUNDLEORIGIN m_OriginBundle;

    float m_fOriginLimitedTime;

  public:
    void TriggerBundle(int SourceID, int SourceJoint, int FXID, int TargetID, int TargetJoint, int idx = 0,
                       int MoveType = FX_BUNDLE_MOVE_NONE);
    void TriggerBundle(int SourceID, int SourceJoint, int FXID, __Vector3 TargetPos, int idx = 0,
                       int MoveType = FX_BUNDLE_MOVE_NONE);
    void Stop(int SourceID, int TargetID, int FXID = -1, int idx = 0, bool immediately = false);
    void StopMine();
    void Tick();
    void Render();
    bool CheckCollisionSphere(__Vector3 vSp, __Vector3 vEp, __Vector3 vCp, float fRadius, __Vector3 * vCol);

    void SetBundlePos(int FXID, int idx, __Vector3 & vPos);

    void ClearAll();
    CN3FXMgr();
    virtual ~CN3FXMgr();
};
