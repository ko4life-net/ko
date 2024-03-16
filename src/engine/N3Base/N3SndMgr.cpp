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
//   Engine reset...
//
void CN3SndMgr::Init(HWND hWnd) {
    Release();
    m_bSndEnable = CN3SndObj::StaticInit(hWnd);
    m_Tbl_Source.LoadFromFile("Data\\sound.tbl");
}

CN3SndObj * CN3SndMgr::CreateObj(int iID, e_SndType eType) {
    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iID);
    if (pTbl == NULL) {
        return NULL;
    }

    return this->CreateObj(pTbl->szFN, eType);
}

CN3SndObj * CN3SndMgr::CreateObj(const std::string & szFN, e_SndType eType) {
    if (!m_bSndEnable) {
        return NULL;
    }

    CN3SndObj * pObjSrc = NULL;
    itm_Snd     it = m_SndObjSrcs.find(szFN);
    if (it == m_SndObjSrcs.end()) // I couldn't find it... let's create a new one...
    {
        pObjSrc = new CN3SndObj();
        if (false == pObjSrc->Create(szFN, eType)) // new loading...
        {
            delete pObjSrc;
            pObjSrc = NULL;
            return NULL;
        }
        m_SndObjSrcs.insert(val_Snd(szFN, pObjSrc)); //Add to map..
    } else {
        pObjSrc = it->second;
    }

    if (!m_bSndDuplicated) {
        return pObjSrc; // this_Snd
    }

    if (NULL == pObjSrc) {
        return NULL;
    }

    CN3SndObj * pObjNew = new CN3SndObj();
    if (false == pObjNew->Duplicate(pObjSrc, eType)) // Duplicate treatment...
    {
        delete pObjNew;
        pObjNew = NULL;
        return NULL;
    }

    if (pObjNew) {
        m_SndObjs_Duplicated.push_back(pObjNew); // add to the list...
    }
    return pObjNew;
}

CN3SndObjStream * CN3SndMgr::CreateStreamObj(const std::string & szFN) {
    CN3SndObjStream * pObj = new CN3SndObjStream();
    if (false == pObj->Create(szFN)) {
        delete pObj;
        pObj = NULL;
        return NULL;
    }

    m_SndObjStreams.push_back(pObj); // Add to list...

    return pObj;
}

CN3SndObjStream * CN3SndMgr::CreateStreamObj(int iID) {
    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iID);
    if (pTbl == NULL) {
        return NULL;
    }

    return this->CreateStreamObj(pTbl->szFN);
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
    std::string szFN = (*ppObj)->m_szFileName; // Remember the file name...

    itl_Snd it = m_SndObjs_Duplicated.begin(), itEnd = m_SndObjs_Duplicated.end();
    for (; it != itEnd; it++) {
        if (*ppObj == *it) {
            m_SndObjs_Duplicated.erase(it);
            delete *ppObj;
            *ppObj = NULL; // Clear object...
            return;
        }
    }

    it = m_SndObjs_PlayOnceAndRelease.begin();
    itEnd = m_SndObjs_PlayOnceAndRelease.end();
    for (; it != itEnd; it++) {
        if (*ppObj == *it) {
            m_SndObjs_PlayOnceAndRelease.erase(it);
            delete *ppObj;
            *ppObj = NULL; // Clear object...
            return;
        }
    }

    *ppObj = NULL; // Makes only the pointer null.

    /*    itm_Snd it = m_SndObjSrcs.find(szFN);
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

// This function plays once and calls ReleaseObj so that the pointer can be rewritten.
// Instead, the location can be specified only once, the first time.
bool CN3SndMgr::PlayOnceAndRelease(int iSndID, const _D3DVECTOR * pPos) {
    if (!m_bSndEnable) {
        return false;
    }

    TABLE_SOUND * pTbl = m_Tbl_Source.Find(iSndID);
    if (pTbl == NULL || pTbl->szFN.empty()) {
        return false;
    }

    CN3SndObj * pObjSrc = NULL;
    itm_Snd     it = m_SndObjSrcs.find(pTbl->szFN);
    if (it == m_SndObjSrcs.end()) // I couldn't find it... let's create a new one...
    {
        pObjSrc = new CN3SndObj();
        if (false == pObjSrc->Create(pTbl->szFN, (e_SndType)pTbl->iType)) // new loading...
        {
            delete pObjSrc;
            pObjSrc = NULL;
            return NULL;
        }
        m_SndObjSrcs.insert(val_Snd(pTbl->szFN, pObjSrc)); // Add to map..
        if (!m_bSndDuplicated) {
            pObjSrc->Play(pPos); // this_Snd
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
    if (false == pObj->Duplicate(pObjSrc, (e_SndType)pTbl->iType)) // Duplicate processing..
    {
        delete pObj;
        pObj = NULL;
        return NULL;
    }

    if (pObj) // put it on the list... noah
    {
        m_SndObjs_PlayOnceAndRelease.push_back(pObj);
        pObj->Play(pPos);
        return true;
    }
    return false;
    /*
    CN3SndObj* pObj = new CN3SndObj();
    if(false == pObj->Create(pTbl->szFN, (e_SndType)pTbl->iType))
    {
        delete pObj; pObj = NULL;
        return false;
    }
    pObj->Play(pPos);
    m_SndObjs_PlayOnceAndRelease.push_back(pObj);
    return true;
*/
}