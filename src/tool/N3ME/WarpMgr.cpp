// WarpMgr.cpp: implementation of the CRegenUser class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "WarpMgr.h"
#include "DlgEditWarp.h"
#include "LyTerrainDef.h"
#include "LyTerrain.h"
#include "MapMng.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarpMgr::CWarpMgr() {
    m_pRefMapMng = NULL; // Terrain reference pointer..
    m_bActive = false;

    m_ListWarpInfo.clear();
    m_iVersion = 1;

    m_pDlg = new CDlgEditWarp;
    m_pDlg->Create(IDD_EDIT_WARP);
    m_pDlg->ShowWindow(FALSE);
    m_pDlg->m_pRefWarpMgr = this;
}

CWarpMgr::~CWarpMgr() {
    ClearList();

    if (m_pDlg) {
        m_pDlg->DestroyWindow();
        delete m_pDlg;
        m_pDlg = NULL;
    }
}

void CWarpMgr::SetActive(bool active) {
    if (m_bActive == active) {
        return;
    }
    m_bActive = active;

    if (active) {
        m_pDlg->ShowWindow(TRUE);
    } else {
        m_pDlg->ShowWindow(FALSE);
    }
}

void CWarpMgr::ClearList() {
    std::list<WARPINFO *>::iterator it, ite;

    ite = m_ListWarpInfo.end();
    for (it = m_ListWarpInfo.begin(); it != ite; it++) {
        WARPINFO * pWI = (*it);
        if (pWI) {
            delete pWI;
        }
    }
    m_ListWarpInfo.clear();
}

bool CWarpMgr::Load(HANDLE hFile) {
    ClearList();

    DWORD dwRWC;
    ReadFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    if (m_iVersion == 1) {
        int cnt;
        ReadFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);
        for (int i = 0; i < cnt; i++) {
            WARPINFO * pWI = new WARPINFO;
            ReadFile(hFile, pWI, sizeof(WARPINFO), &dwRWC, NULL);
            m_ListWarpInfo.push_back(pWI);
        }
    }

    m_pDlg->ResetAll();
    return true;
}

bool CWarpMgr::Save(HANDLE hFile) {
    DWORD dwRWC;
    WriteFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    int cnt = m_ListWarpInfo.size();
    WriteFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);

    std::list<WARPINFO *>::iterator it, ite;

    ite = m_ListWarpInfo.end();
    for (it = m_ListWarpInfo.begin(); it != ite; it++) {
        WARPINFO * pWI = (*it);
        WriteFile(hFile, pWI, sizeof(WARPINFO), &dwRWC, NULL);
    }
    return true;
}

void CWarpMgr::SaveServerData(HANDLE hFile) {
    DWORD dwRWC;
    int   cnt = m_ListWarpInfo.size();
    WriteFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);

    std::list<WARPINFO *>::iterator it, ite;

    ite = m_ListWarpInfo.end();
    for (it = m_ListWarpInfo.begin(); it != ite; it++) {
        WARPINFO * pWI = (*it);
        WriteFile(hFile, pWI, sizeof(WARPINFO), &dwRWC, NULL);
    }
}

WARPINFO * CWarpMgr::GetInfoByName(char * pName) {
    std::list<WARPINFO *>::iterator it, ite;

    ite = m_ListWarpInfo.end();
    for (it = m_ListWarpInfo.begin(); it != ite; it++) {
        WARPINFO * pWI = (*it);
        if (strcmp(pName, pWI->szName) == 0) {
            return pWI;
        }
    }
    return NULL;
}

void CWarpMgr::DelInfo(WARPINFO * pInfo) {
    std::list<WARPINFO *>::iterator it, ite;

    ite = m_ListWarpInfo.end();
    for (it = m_ListWarpInfo.begin(); it != ite; it++) {
        WARPINFO * pWI = (*it);
        if (pInfo == pWI) {
            m_ListWarpInfo.erase(it);
            it = ite;
        }
    }
}