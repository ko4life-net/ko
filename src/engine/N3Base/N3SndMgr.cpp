// N3SndMgr.cpp: implementation of the CN3SndMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3SndMgr.h"
#include "N3SndObj.h"
#include "N3SndObjStream.h"
#include "N3Base.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3SndMgr::CN3SndMgr() {
    m_bSndEnable = false;
    m_bSndDuplicated = false;
}

CN3SndMgr::~CN3SndMgr() {
    Release();
}

//
//    엔진 초기화..
//
void CN3SndMgr::Init(HWND hWnd) {
    Release();
    m_bSndEnable = CN3SndObj::StaticInit(hWnd);
    m_Tbl_Source.LoadFromFile(fs::path("Data") / "sound.tbl");
}

CN3SndObj * CN3SndMgr::CreateObj(int iID, e_SndType eType) {
    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iID);
    if (pTbl == NULL) {
        return NULL;
    }

    return this->CreateObj(pTbl->szFile, eType);
}

CN3SndObj * CN3SndMgr::CreateObj(const fs::path & fsFile, e_SndType eType) {
    if (!m_bSndEnable) {
        return NULL;
    }

    CN3SndObj * pObjSrc = NULL;
    itm_Snd     it = m_SndObjSrcs.find(fsFile);
    if (it == m_SndObjSrcs.end()) // 못 찾았다... 새로 만들자..
    {
        pObjSrc = new CN3SndObj();
        if (false == pObjSrc->Create(fsFile, eType)) // 새로 로딩..
        {
            delete pObjSrc;
            pObjSrc = NULL;
            return NULL;
        }
        m_SndObjSrcs.insert(val_Snd(fsFile, pObjSrc)); // 맵에 추가한다..
    } else {
        pObjSrc = it->second;
    }

    if (!m_bSndDuplicated) {
        return pObjSrc; //this_Snd
    }

    if (NULL == pObjSrc) {
        return NULL;
    }

    CN3SndObj * pObjNew = new CN3SndObj();
    if (false == pObjNew->Duplicate(pObjSrc, eType)) // Duplicate 처리..
    {
        delete pObjNew;
        pObjNew = NULL;
        return NULL;
    }

    if (pObjNew) {
        m_SndObjs_Duplicated.push_back(pObjNew); // 리스트에 넣는다...
    }
    return pObjNew;
}

CN3SndObjStream * CN3SndMgr::CreateStreamObj(const fs::path & fsFile) {
    CN3SndObjStream * pObj = new CN3SndObjStream();
    if (false == pObj->Create(fsFile)) {
        delete pObj;
        pObj = NULL;
        return NULL;
    }

    m_SndObjStreams.push_back(pObj); // 리스트에 넣기..

    return pObj;
}

CN3SndObjStream * CN3SndMgr::CreateStreamObj(int iID) {
    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iID);
    if (pTbl == NULL) {
        return NULL;
    }

    return this->CreateStreamObj(pTbl->szFile);
}

void CN3SndMgr::ReleaseStreamObj(CN3SndObjStream ** ppObj) {
    if (NULL == ppObj || NULL == *ppObj) {
        return;
    }

    itl_SndStream it = m_SndObjStreams.begin(), itEnd = m_SndObjStreams.end();
    for (; it != itEnd; it++) {
        if (*ppObj == *it) {
            delete *ppObj;
            *ppObj = NULL;
            m_SndObjStreams.erase(it);
            break;
        }
    }
}

//
//    TickTick...^^
//
void CN3SndMgr::Tick() {
    if (!m_bSndEnable) {
        return;
    }

    //    m_Eng.SetListenerPos(&(CN3Base::s_CameraData.vEye));
    //    __Vector3 vUP(0.0f, 1.0f, 0.0f);
    //    __Vector3 vDir = CN3Base::s_CameraData.vAt - CN3Base::s_CameraData.vEye;
    //
    //    if(vDir.Magnitude() <= FLT_MIN) vDir.Set(0.0f, 0.0f, 1.0f);
    //
    //    m_Eng.SetListenerOrientation(&vDir, &vUP);
    //

    /*
    CN3SndObj* pObj = NULL;
    itl_Snd it = m_SndObjs_Duplicated.begin(), itEnd = m_SndObjs_Duplicated.end();
    for(; it != itEnd; it++)
    {
        pObj = *it;
        pObj->Tick();
    }
*/
    itl_Snd     it, itEnd; //this_Snd
    CN3SndObj * pObj = NULL;
    if (!m_bSndDuplicated) {
        itm_Snd it_m = m_SndObjSrcs.begin(), itEnd_m = m_SndObjSrcs.end();
        for (; it_m != itEnd_m; it_m++) {
            pObj = it_m->second;
            pObj->Tick();
        }
    } else {
        it = m_SndObjs_Duplicated.begin();
        itEnd = m_SndObjs_Duplicated.end();
        for (; it != itEnd; it++) {
            pObj = *it;
            pObj->Tick();
        }
    }

    it = m_SndObjs_PlayOnceAndRelease.begin();
    itEnd = m_SndObjs_PlayOnceAndRelease.end();
    for (; it != itEnd;) {
        pObj = *it;
        pObj->Tick();
        if (false == pObj->IsPlaying()) {
            it = m_SndObjs_PlayOnceAndRelease.erase(it);
            delete pObj;
            pObj = NULL;
        } else {
            it++;
        }
    }

    CN3SndObjStream * pObj2 = NULL;
    itl_SndStream     it2 = m_SndObjStreams.begin(), itEnd2 = m_SndObjStreams.end();
    for (; it2 != itEnd2; it2++) {
        pObj2 = *it2;
        if (pObj2) {
            pObj2->Tick();
        }
    }

    //    itm_Snd it2 = m_SndObjSrcs.begin();
    //    for(; it2 != m_SndObjSrcs.end(); it2++)
    //    {
    //        pObj = it2->second.pSndObj;
    //        if(pObj) pObj->Tick();
    //    }

    CN3SndObj::StaticTick(); // CommitDeferredSetting...
}

//
//    Obj하나 무효화..
void CN3SndMgr::ReleaseObj(CN3SndObj ** ppObj) {
    if (NULL == ppObj || NULL == *ppObj) {
        return;
    }
    fs::path fsFile = (*ppObj)->m_fsFile; // 파일 이름을 기억하고..

    itl_Snd it = m_SndObjs_Duplicated.begin(), itEnd = m_SndObjs_Duplicated.end();
    for (; it != itEnd; it++) {
        if (*ppObj == *it) {
            m_SndObjs_Duplicated.erase(it);
            delete *ppObj;
            *ppObj = NULL; // 객체 지우기..
            return;
        }
    }

    it = m_SndObjs_PlayOnceAndRelease.begin();
    itEnd = m_SndObjs_PlayOnceAndRelease.end();
    for (; it != itEnd; it++) {
        if (*ppObj == *it) {
            m_SndObjs_PlayOnceAndRelease.erase(it);
            delete *ppObj;
            *ppObj = NULL; // 객체 지우기..
            return;
        }
    }

    *ppObj = NULL; // 포인터만 널로 만들어 준다..

    /*    itm_Snd it = m_SndObjSrcs.find(fsFile);
    if(it != m_SndObjSrcs.end()) // 찾았다..
    {
        CN3SndObj* pObj = it->second;
        delete pObj;
        m_SndObjSrcs.erase(it);
    }
    else
    {
        itl_Snd it2 = m_SndObjs_PlayOnceAndRelease.begin();
        for(; it2 != m_SndObjs_PlayOnceAndRelease.end(); it2++)
        {
            CN3SndObj* pObj = *it2;
            if(pObj == *ppObj)
            {
                delete pObj;
                m_SndObjs_PlayOnceAndRelease.erase(it2);
                break;
            }        
        }
    }
*/
}

//
//    Release Whole Objects & Sound Sources & Sound Engine..
//
void CN3SndMgr::Release() {
    if (!m_bSndEnable) {
        return;
    }

    CN3SndObj * pObj = NULL;
    itm_Snd     it = m_SndObjSrcs.begin(), itEnd = m_SndObjSrcs.end();
    for (; it != itEnd; it++) {
        pObj = it->second;
        if (pObj) {
            delete pObj;
        }
    }
    m_SndObjSrcs.clear();

    itl_Snd it2 = m_SndObjs_Duplicated.begin(), itEnd2 = m_SndObjs_Duplicated.end();
    for (; it2 != itEnd2; it2++) {
        pObj = *it2;
        if (pObj) {
            delete pObj;
        }
    }
    m_SndObjs_Duplicated.clear();

    it2 = m_SndObjs_PlayOnceAndRelease.begin();
    itEnd2 = m_SndObjs_PlayOnceAndRelease.end();
    for (; it2 != itEnd2; it2++) {
        pObj = *it2;
        if (pObj) {
            delete pObj;
        }
    }
    m_SndObjs_PlayOnceAndRelease.clear();

    CN3SndObjStream * pObj2 = NULL;
    itl_SndStream     it3 = m_SndObjStreams.begin(), itEnd3 = m_SndObjStreams.end();
    for (; it3 != itEnd3; it3++) {
        pObj2 = *it3;
        if (pObj2) {
            delete pObj2;
        }
    }
    m_SndObjStreams.clear();

    CN3SndObj::StaticRelease();
}

// 이 함수는 한번 플레이 하고 그 포인터를 다시 쓸수있게 ReleaseObj를 호출한다.
// 대신 위치는 처음 한번밖에 지정할 수 없다.
bool CN3SndMgr::PlayOnceAndRelease(int iSndID, const _D3DVECTOR * pPos) {
    if (!m_bSndEnable) {
        return false;
    }

    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iSndID);
    if (pTbl == NULL || pTbl->szFile.empty()) {
        return false;
    }

    CN3SndObj * pObjSrc = NULL;
    itm_Snd     it = m_SndObjSrcs.find(pTbl->szFile);
    if (it == m_SndObjSrcs.end()) // 못 찾았다... 새로 만들자..
    {
        pObjSrc = new CN3SndObj();
        if (false == pObjSrc->Create(pTbl->szFile, (e_SndType)pTbl->iType)) // 새로 로딩..
        {
            delete pObjSrc;
            pObjSrc = NULL;
            return NULL;
        }
        m_SndObjSrcs.insert(val_Snd(pTbl->szFile, pObjSrc)); // 맵에 추가한다..
        if (!m_bSndDuplicated) {
            pObjSrc->Play(pPos); //this_Snd
        }
    } else {
        pObjSrc = it->second;
    }

    if (NULL == pObjSrc) {
        return false;
    }

    if (!m_bSndDuplicated) {
        pObjSrc->Play(pPos); //this_Snd
        return true;
    }

    CN3SndObj * pObj = new CN3SndObj();
    if (false == pObj->Duplicate(pObjSrc, (e_SndType)pTbl->iType)) // Duplicate 처리..
    {
        delete pObj;
        pObj = NULL;
        return NULL;
    }

    if (pObj) // 리스트에 넣는다...noah
    {
        m_SndObjs_PlayOnceAndRelease.push_back(pObj);
        pObj->Play(pPos);
        return true;
    }
    return false;
    /*
    CN3SndObj* pObj = new CN3SndObj();
    if(false == pObj->Create(pTbl->fsFile, (e_SndType)pTbl->iType))
    {
        delete pObj; pObj = NULL;
        return false;
    }
    pObj->Play(pPos);
    m_SndObjs_PlayOnceAndRelease.push_back(pObj);
    return true;
*/
}
