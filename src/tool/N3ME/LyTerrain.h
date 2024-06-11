// LyTerrain.h: interface for the CLyTerrain class.
// - 2001. 9.24.~ Converted for Map Editor...
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <list>

#include "LyTerrainDef.h"
#include "N3Base/N3BaseFileAccess.h"

class CN3Texture;
class CQTNode;

class CDTexMng;
class CDTexGroup;
class CDTexGroupMng;
class CDlgSetLightMap;

////////////////////////////////////////////////
// use multimap
//I will use it when saving gamedata..
//Also used when taking colormaps..^^
//
typedef std::multimap<short, short> MMap;
typedef MMap::value_type            MMValue;
typedef MMap::iterator              MMIter;

typedef std::list<short> LList;
typedef LList::iterator  LLIter;

typedef typename std::list<CQTNode *>::iterator it_QTNode;

class CLyTerrain : public CN3BaseFileAccess {
  protected:
    __Material  m_Material;
    D3DFILLMODE m_FillMode;

    //quadtree..
    CQTNode * m_pRoot;
    CQTNode * m_pRefCurrNode;

    //Related to terrain editing...
    __VertexXyzColor
         m_vBrushVertices[MAX_BRUSH_SIZE * MAX_BRUSH_SIZE * 9];    // vertex buffer representing the shape of the brush
    WORD m_wBrushIndices[MAX_BRUSH_SIZE * MAX_BRUSH_SIZE * 6 * 4]; // index buffer representing the shape of the brush
    int  m_iBrushIndexCount;                                       // Number of index indicating the shape of the brush
    int  m_iEditMode;                                              // Is it Edit mode?

    CDTexMng * m_pDTexMng;

    MMap m_ColorMapTileTree; //Tree with tiles before creating color map...

    int m_iDistanceTable[DISTANCE_TABLE_SIZE][DISTANCE_TABLE_SIZE];

  public:
    float m_fBrushIntensityMap[MAX_BRUSH_SIZE][MAX_BRUSH_SIZE]; // Map showing the rate of change of the brush
    float m_fHeightScale;
    float m_fFlatHeight;
    bool  m_bFlaten;

    int m_iZoneID;

    //LightMap related...
    CDlgSetLightMap *       m_pDlgSetLightMap;
    LPDIRECT3DVERTEXBUFFER9 m_LightMapVB;
    CN3Texture ***          m_ppLightMapTexture;
    bool **                 m_ppIsLightMap;
    int                     m_iNumLightMap;
    bool                    m_bDrawLineLightMap; //In the light map drawing option... is it drawn in line form?
    __VertexXyzColor        m_vLineLightMap
        [2]; //If you draw it in the form of a line, you have to display that line on the screen, right?...^^ That's the point...

    //LOD related..
    int m_iHeightLimit;
    int m_iDistLimit;

    std::list<CQTNode *> m_RenderNodes;

    LPDIRECT3DVERTEXBUFFER9 m_ColorMapVB; // When using a color map...
    LPDIRECT3DVERTEXBUFFER9 m_TileVB;     // One tile and one lightmap

    //common..
    MAPDATA ** m_ppMapData;
    BOOL **    m_ppRenderInfo;

    //texture..
    CN3Texture ** m_pColorTexture;

    RECT  m_VisibleRect; //Visible area per tile...
    POINT m_EyePos;      //Tile unit..

    int m_iHeightMapSize; // Similar concept to Cell Count -> You have to subtract 1 to get Cell Count.
    int m_iMaxLevel;

    int m_iNumTileMap; //Number of tile maps we currently have...

    int m_iColorMapTexSize;
    int m_iColorMapPixelPerUnitDistance;
    int m_iNumColorMap;

    //Related to the direction of tiling...
    float m_fTileDirU[8][4];
    float m_fTileDirV[8][4];
    float m_fTileDirUforColorMap[8][4];
    float m_fTileDirVforColorMap[8][4];

    // DTex related...
    LPDIRECT3DVERTEXBUFFER9 m_pDTexVB;
    __VertexT1 *            m_pDTexVertices;
    int                     m_DTexFaceCnt;
    int                     m_DTexBuff[5000];
    int                     m_DTexAttrTable[9][14];
    int                     m_DTexDirTable[9][14];

    // ColorMap related...
    LPDIRECT3DVERTEXBUFFER9 m_pColorMapTmpVB;
    __VertexTransformedT2 * m_pColorMapTmpVertices;

    //Brush related..
    BOOL m_bBrushFlat; //Are you going to flatten the brush that raises and lowers the terrain?

  protected:
    int  DetectRealLightMap(int sx, int sz, int range);
    void MakeDistanceTable();
    void FillLightMap(int x, int z, int lcx, int lcz, DWORD Color, float alpha);
    void SetLightMap(int x, int z, int lcx, int lcz);

    void PutColorMapTile(
        int x, int z); //Insert tiles to create a color map... (not created yet... put those tiles in the queue...)
    void VerifyTexInfo(LPDTEXINFO pTex);
    void SetColorMap(int x, int y);
    void SetPertinentTile(CDTexGroupMng * pDTexGroupMng, LPDTEXINFO pDest1, LPDTEXINFO pDest2);

    inline int  Log2(int x);
    inline void ClearRenderInfo();
    void        SetVisibleRect(); //Tile unit..

    //map editor related..
    //
    void  SetDTexManual(int x, int y); // Manual shooting
    void  SetDTex(int x, int y);       // Auto capture.
    BOOL  IntersectTriangle(const D3DXVECTOR3 orig, const D3DXVECTOR3 dir, D3DXVECTOR3 & v0, D3DXVECTOR3 & v1,
                            D3DXVECTOR3 & v2, float * t, float * u, float * v);
    void  Heighten(POINT ptCenter, float fHeight); // Function to raise or lower the terrain
    void  Smooth(POINT ptCenter); // Smooth the terrain by comparing it to the height value of the surrounding map.
    void  Flaten(POINT ptCenter);
    void  SetApexHeight(int x, int z, float fHeight); // Specify the height value of the height map
    float GetApexHeight(int x, int z) const;          // Get the height value of the height map
    float GetFallOffValue(float fFallOff, float x);   // Expression used when calculating BrushIntensity.

    //Improved light map related...(2002. 11. 13..)
    void ConvertLightMapToolDataV2toV3();
    //

  public:
    void UpdateBrushArea(POINT ptCenter); // Update brush area display vertex
    void RenderBrushArea();               // Draw brush area display

    void ExportHeightBMP(const char * szPathName);
    void ImportHeightBMP(const char * szPathName);

    void MakeMoveTable(short ** ppEvent);
    void Init(int HeightMapSize = 5);
    bool LoadFromFile(const char * lpszPath);
    bool SaveToFile(const char * lpszPath);
    bool SaveToFilePartition(const char * lpszPath, float psx, float psz, float width);
    void Tick();
    void Render();
    void Release();

    void MakeGameLightMap(char * szFullPathName);
    void MakeGameColorMap(char * szFullPathName);
    void GeneraterColorMap(bool bIsAll = false);
    void TilingAll(); //Lay the entire terrain with the selected tiles..

    void UpDateDistLimit();

    CN3Texture * GetTileTex(int id);
    void         SetFillMode(D3DFILLMODE fillmode);
    float        GetHeight(float x, float z);

    //map editor related..
    //
    SIZE GetPatchNum(float fSize);
    BOOL MouseMsgFilter(LPMSG pMsg); // Handle mouse messages when modifying terrain
    bool Pick(int x, int y, __Vector3 * vec, POINT * pHeightMapPos = NULL);
    void Import(LPCTSTR pFileName, float fSize);
    void ImportHeight(LPCTSTR pFileName);
    void SaveServerData(HANDLE hFile);
    void SetEditMode(int iEditMode); // Change to terrain edit mode
    void UpdateBrushIntensityMap(
        int iShape, int iSize,
        float fFallOff); // Reconfigure the IntensityMap according to the shape and size of the brush.
    void SaveGameData(HANDLE hFile);
    void ColorMapImport(LPCTSTR lpszPathName);
    void ColorMapExport(LPCTSTR lpszPathName);
    void GenerateMiniMap(LPCTSTR lpszPathName, int size);
    //

    CLyTerrain();
    virtual ~CLyTerrain();
};
