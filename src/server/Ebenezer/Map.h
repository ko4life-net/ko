// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3ShapeMgr.h"
#include "Region.h"
#include "GameEvent.h"
#include "STLMap.h"

typedef CSTLMap<CGameEvent>                EventArray;
typedef CSTLMap<_OBJECT_EVENT>             ObjectEventArray;
typedef CSTLMap<_REGENE_EVENT>             ObjectRegeneArray;
typedef CSTLMap<_WARP_INFO>                WarpArray;
typedef typename map<int, int *>::iterator Iterator;

class CUser;
class CEbenezerDlg;

class C3DMap {
  private:
    ObjectEventArray  m_ObjectEventArray;
    ObjectRegeneArray m_ObjectRegeneArray;

    EventArray  m_EventArray;
    CN3ShapeMgr m_N3ShapeMgr;
    //    int            m_nMapSize;        // Grid Unit ex) 4m
    //    float        m_fUnitDist;    // i Grid Distance

    float ** m_fHeight;

    int m_nXRegion;
    int m_nZRegion;

  public:
    //
    int   m_nMapSize;  // Grid Unit ex) 4m
    float m_fUnitDist; // i Grid Distance
                       //
    void            LoadWarpList(HANDLE hFile);
    void            LoadRegeneEvent(HANDLE hFile);
    BOOL            IsValidPosition(float x, float z, float y);
    _OBJECT_EVENT * GetObjectEvent(int objectindex) { return m_ObjectEventArray.GetData(objectindex); };
    _REGENE_EVENT * GetRegeneEvent(int objectindex) { return m_ObjectRegeneArray.GetData(objectindex); };
    void            LoadObjectEvent(HANDLE hFile);
    BOOL            LoadEvent();
    BOOL            CheckEvent(float x, float z, CUser * pUser = NULL);
    Iterator        RegionNpcRemove(int rx, int rz, int nid);
    void            RegionNpcAdd(int rx, int rz, int nid);
    Iterator        RegionUserRemove(int rx, int rz, int uid);
    void            RegionUserAdd(int rx, int rz, int uid);
    BOOL            RegionItemRemove(int rx, int rz, int bundle_index, int itemid, int count);
    BOOL            RegionItemAdd(int rx, int rz, _ZONE_ITEM * pItem);
    BOOL            ObjectCollision(float x1, float z1, float y1, float x2, float z2, float y2);
    float           GetHeight(float x, float y, float z);
    void            LoadMapTile(HANDLE hFile);
    BOOL            LoadMap(HANDLE hFile);
    void            LoadTerrain(HANDLE hFile);
    int             GetXRegionMax() { return m_nXRegion - 1; };
    int             GetZRegionMax() { return m_nZRegion - 1; };

    C3DMap();
    virtual ~C3DMap();

    fs::path m_fsSmdFileName;
    int      m_nServerNo;
    int      m_nZoneNumber;
    float    m_fInitX;
    float    m_fInitZ;
    float    m_fInitY;
    BYTE     m_bType; // Zone Type : 1 -> common zone,  2 -> battle zone, 3 -> 24 hour open battle zone
    short    m_sMaxUser;

    CRegion ** m_ppRegion;
    short **   m_ppnEvent;

    WarpArray m_WarpArray;

    CEbenezerDlg * m_pMain;

    DWORD m_wBundle; // Zone Item Max Count
};
