// WarpMgr.h: interface for the CRegenUser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3BaseFileAccess.h"
#include <list>

class CDlgEditWarp;
class CMapMng;

typedef struct __WarpInfo {
    short sID;
    char  szName[32];
    char  szAgree[256];
    DWORD dwMoney;
    short sZoneID;
    float fX;
    float fY;
    float fZ;
    float fRadius;

    __WarpInfo() {
        ZeroMemory(szName, 32);
        ZeroMemory(szAgree, 256);
        sID = 0;
        dwMoney = 0;
        fX = fY = fZ = 0.0f;
        fRadius = 0.0f;
    }
} WARPINFO;

class CWarpMgr : public CN3BaseFileAccess {
  public:
    CDlgEditWarp * m_pDlg;

    CMapMng * m_pRefMapMng; // Terrain reference pointer..
    bool      m_bActive;

    std::list<WARPINFO *> m_ListWarpInfo;

    int m_iVersion;

  public:
    void       DelInfo(WARPINFO * pInfo);
    WARPINFO * GetInfoByName(char * pName);
    void       AddInfo(WARPINFO * pInfo) { m_ListWarpInfo.push_back(pInfo); }
    bool       IsActive() { return m_bActive; }
    void       SetActive(bool active);
    void       ClearList();

    bool Load(HANDLE hFile);
    bool Save(HANDLE hFile);
    void SaveServerData(HANDLE hFile);

    CWarpMgr();
    virtual ~CWarpMgr();
};
