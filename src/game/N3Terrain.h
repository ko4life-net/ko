//    N3Terrain.h: interface for the CLyTerrain class.
//    2001. 10. 22.
//
//////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////
//    변수앞에 ti_가 붙은 건 타일단위..
//    변수앞에 pat_가 붙은 건 패치단위..
//    변수앞에 re_가 붙은 건 미터단위..
//

#include "N3Base/N3BaseFileAccess.h"
#include "N3Base/N3Texture.h"
#include "N3TerrainDef.h"
#include "N3TerrainPatch.h"

#include <string>

//class CN3Terrain : public CGameBase
class CN3Terrain : public CN3BaseFileAccess {
  public:
    //common..
    __Material   m_Material;
    D3DSHADEMODE m_ShadeMode;
    D3DFILLMODE  m_FillMode;
    int          m_iLodLevel;

    //Patch...
    CN3TerrainPatch ** m_ppPatch;
    POINT              m_pat_LBPos;
    POINT              m_pat_PrevLBPos;
    RECT               m_pat_BoundRect; //LBPos에 대한 상대좌표..
    int                m_iNumPatch;
    int                m_pat_Center2Side; //중심점에서 한쪽변까지의 길이..
    int                m_iDistanceTable[DISTANCE_TABLE_SIZE][DISTANCE_TABLE_SIZE];

    //MapInfo..
    MAPDATA *   m_pMapData;
    int         m_ti_MapSize;  // 셀이 몇개 들어가나.. 4096Meter -> 1024 + 1
    int         m_pat_MapSize; // 패치 갯수.. 사이즈에 따라 틀리다..
    __Vector3 * m_pNormal;

    //LightMap Info..
    POINT        m_pat_CenterPos;
    stlMap_N3Tex m_LightMapPatch[3][3];

    //bool**            m_ppIsLightMap;
    //CN3Texture***        m_pppLightMapTex;

    //Patch
    float ** m_ppPatchRadius;
    float ** m_ppPatchMiddleY;

    //Tile..
    POINT m_ti_CenterPos;
    POINT m_ti_PrevCenterPos;

    //Texture...
    int          m_NumTileTex; // Tile Texture 갯수
    CN3Texture * m_pTileTex;

    //ColorMap..
    CN3Texture ** m_ppColorMapTex;
    int           m_iNumColorMap; // 컬러맵은 분할 저장되어 있다.. 갯수 =

    //컬러맵위에 덧 씌우는 무늬맵..
    CN3Texture m_pBaseTex;

    //타일방향..
    float m_fTileDirU[8][4];
    float m_fTileDirV[8][4];

    std::string m_szZoneName;

    class CN3River * m_pRiver;
    class CN3Pond *  m_pPond;

    bool m_bAvailableTile;

  protected:
    void SetNormals();
    void SetLightMap(int dir);
    void ReplaceLightMapPatch(int x, int z, stlMap_N3Tex & LightMapPatch);
    void SetLightMapPatch(int x, int z, HANDLE hFile, int * pAddr);

    void TestAvailableTile();
    void MakeDistanceTable();

    inline int Log2(int x);                                        //2의 승수 전용....
    int        Real2Tile(float x) { return ((int)x / TILE_SIZE); } // 실좌표 -> 타일좌표...(절대좌표)
    int        Tile2Patch(int x) { return (x / PATCH_TILE_SIZE); } // 타일좌표 -> 패치좌표...(절대좌표계)
    int Real2Patch(float fX) { return (((int)fX / TILE_SIZE) / PATCH_TILE_SIZE); } // 실좌표 -> 패치좌표..(절대좌표계)

    void LoadTileInfo(HANDLE hFile);
    bool CheckRenderablePatch();
    bool CheckMovePatch();
    bool CheckBound();
    void DispositionPatch();
    void SetBlunt();

  public:
    CN3Texture * GetLightMap(int tx, int tz);
    float        GetWidthByMeter() { return (m_ti_MapSize - 1) * TILE_SIZE; }
    int          GetLODLevel() { return m_iLodLevel; }
    bool         SetLODLevel(int level);
    float        GetHeight(float x, float z);
    void         Release();
    void         Init();
    bool         Load(HANDLE hFile);
    void         Tick();
    void         Render();
    void         SetFillMode(D3DFILLMODE FillMode) { m_FillMode = FillMode; }
    void         SetShadeMode(D3DSHADEMODE ShadeMode) { m_ShadeMode = ShadeMode; }

    CN3Terrain();
    virtual ~CN3Terrain();

  public: //additional........
    bool         GetTileTexInfo(float x, float z, TERRAINTILETEXINFO & TexInfo1, TERRAINTILETEXINFO & TexInfo2);
    CN3Texture * GetTileTex(int x, int z);
    MAPDATA      GetMapData(int x, int z);
    bool         LoadColorMap(const fs::path & fsFile);
    void         GetNormal(float x, float z, __Vector3 & vNormal);
    bool         IsInTerrain(float x, float z);
    //..
    BOOL Pick(int x, int y, __Vector3 & vPick);
    BOOL PickWide(int x, int y, __Vector3 & vPick);
    void CalcCollisionTerrainByOTPlayer(__Vector3, __Vector3, __Vector3 &);

    bool CheckIncline(
        const __Vector3 & vPos, const __Vector3 & vDir,
        float fIncline); // 현재 위치와 방향에서의 경사값이 인수로 들어온것보다 크면(못올라갈 곳이면) true, 작으면 false
    bool CheckCollisionCamera(__Vector3 & vEye, const __Vector3 & vAt,
                              float fNP); // vEye 에 계산된 값도 들어온다.. 카메라 Near Plane을 넣으면 계산.
    BOOL CheckCollisionByHeight(const __Vector3 & vPos, float fUnitSize, float & fHeight) {
        fHeight = this->GetHeight(vPos.x, vPos.z);
        if (vPos.y < fHeight + fUnitSize) {
            return TRUE; // 현재 높이가 지형높이 + 크기 보다 작다면
        }
        return FALSE;
    }
    bool CheckCollision(__Vector3 & vPos, __Vector3 & vDir, float fVelocity, __Vector3 * vpCol);
};
