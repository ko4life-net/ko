//
// N3Terrain.cpp: implementation of the CLyTerrain class.
//    2001. 10. 22.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <stdio.h>
#include "N3Terrain.h"
#include "N3TerrainPatch.h"
#include "PlayerMySelf.h"
#include "GameBase.h"
#include "GameProcedure.h"
#include "UILoading.h"

#include "N3Base/N3River.h"
#include "N3Base/N3Pond.h"

const float COLLISION_BOX = 100.0f;

//
// Construction/Destruction
//
CN3Terrain::CN3Terrain() {
    m_Material.Init();
    m_ShadeMode = D3DSHADE_GOURAUD;
    m_FillMode = D3DFILL_SOLID;
    m_iLodLevel = MIN_LOD_LEVEL;

    m_ppPatch = NULL;

    m_pat_CenterPos.x = m_pat_CenterPos.y = 0;
    m_pat_LBPos.x = m_pat_LBPos.y = 0;
    m_pat_PrevLBPos.x = m_pat_PrevLBPos.y = 0;

    SetRectEmpty(&m_pat_BoundRect);
    m_iNumPatch = 0;

    m_pMapData = NULL;
    m_ti_MapSize = 0;
    m_pat_MapSize = 0;

    //m_ppIsLightMap = NULL;
    //m_pppLightMapTex = NULL;

    m_ti_CenterPos.x = m_ti_CenterPos.y = 0;
    m_ti_PrevCenterPos = m_ti_CenterPos;

    m_ppPatchRadius = NULL;
    m_ppPatchMiddleY = NULL;

    ZeroMemory(m_pGrassFileName, MAX_PATH);

    for (int i = 0; i < MAX_GRASS; i++) {
        ZeroMemory(m_pGrassTextureName[i], MAX_PATH);
    }
    m_iNumGrass = 0;

    m_NumTileTex = 0;
    m_pTileTex = NULL;
    m_ppColorMapTex = NULL;
    m_iNumColorMap = 0;

    float TileDirU[8][4] = {
        {0.0f, 1.0f, 0.0f, 1.0f}, //[up][LT, RT, LB, RB]
        {0.0f, 0.0f, 1.0f, 1.0f}, //[right][ // ]
        {1.0f, 0.0f, 1.0f, 0.0f}, //[left][ // ]
        {1.0f, 1.0f, 0.0f, 0.0f}, //[bottom][ // ]

        {1.0f, 0.0f, 1.0f, 0.0f}, //[up_mirr][LT, RT, LB, RB]
        {0.0f, 0.0f, 1.0f, 1.0f}, //[right_mirr][ // ]
        {0.0f, 1.0f, 0.0f, 1.0f}, //[left_mirr][ // ]
        {1.0f, 1.0f, 0.0f, 0.0f}  //[bottom_mirr][ // ]
    };
    memcpy(m_fTileDirU, TileDirU, sizeof(float) * 8 * 4);

    float TileDirV[8][4] = {
        {0.0f, 0.0f, 1.0f, 1.0f}, //[up][ // ]
        {1.0f, 0.0f, 1.0f, 0.0f}, //[right][ // ]
        {1.0f, 1.0f, 0.0f, 0.0f}, //[left][ // ]
        {0.0f, 1.0f, 0.0f, 1.0f}, //[bottom][ // ]

        {0.0f, 0.0f, 1.0f, 1.0f}, //[up_mirr][ // ]
        {0.0f, 1.0f, 0.0f, 1.0f}, //[right_mirr][ // ]
        {1.0f, 1.0f, 0.0f, 0.0f}, //[left_mirr][ // ]
        {1.0f, 0.0f, 1.0f, 0.0f}  //[bottom_mirr][ // ]
    };
    memcpy(m_fTileDirV, TileDirV, sizeof(float) * 8 * 4);

    MakeDistanceTable();

    m_pGrassAttr = NULL;
    m_pGrassNum = NULL;

    m_pRiver = NULL;
    m_pPond = NULL;
    m_pNormal = NULL;

    m_bAvailableTile = true;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m_LightMapPatch[i][j].clear();
        }
    }
}

CN3Terrain::~CN3Terrain() {
    Release();
}

//
//    MakeDistanceTable
//    �Ÿ��� ������� ���� ���̺��� ������ �� �ְ� �̸� ���̺� ����..
//    ���� ���� �Ÿ�..
//
void CN3Terrain::MakeDistanceTable() {
    for (int x = 0; x < DISTANCE_TABLE_SIZE; x++) {
        for (int z = 0; z < DISTANCE_TABLE_SIZE; z++) {
            double dist = sqrt((double)((x * x) + (z * z))) + 0.6;
            m_iDistanceTable[x][z] = (int)dist;
        }
    }
}

//
//    Release....
//
void CN3Terrain::Release() {
    if (m_pGrassAttr) {
        //free(m_pGrassAttr);
        GlobalFree(m_pGrassAttr);
        m_pGrassAttr = NULL;
    }

    if (m_pGrassNum) {
        //free(m_pGrassAttr);
        GlobalFree(m_pGrassNum);
        m_pGrassNum = NULL;
    }

    //    {
    //        for(int x=0;x<m_ti_MapSize;x++)
    //        {
    //            if(m_ppGrassAttr[x])
    //            {
    //                delete[] m_ppGrassAttr[x];
    //                m_ppGrassAttr[x] = NULL;
    //            }
    //        }
    //        delete[] m_ppGrassAttr;
    //        m_ppGrassAttr = NULL;
    //    }

    if (m_pRiver) {
        m_pRiver->Release();
        delete m_pRiver;
        m_pRiver = NULL;
    }

    if (m_pPond) {
        delete m_pPond;
        m_pPond = NULL;
    }

    if (m_pTileTex) {
        //        for(int x=0;x<m_NumTileTex;x++)
        //            m_pTileTex[x].Release();
        delete[] m_pTileTex;
        m_pTileTex = NULL;
    }

    if (m_ppColorMapTex) {
        for (int x = 0; x < m_iNumColorMap; x++) {
            //            for(int z=0;z<m_iNumColorMap;z++)
            //            {
            //                m_ppColorMapTex[x][z].Release();
            //            }
            delete[] m_ppColorMapTex[x];
            m_ppColorMapTex[x] = NULL;
        }
        delete[] m_ppColorMapTex;
        m_ppColorMapTex = NULL;
    }

    if (m_ppPatch) {
        for (int x = 0; x < m_iNumPatch; x++) {
            //            for(int z=0;z<m_iNumPatch;z++)
            //            {
            //                m_ppPatch[x][z].Release();
            //            }
            delete[] m_ppPatch[x];
            m_ppPatch[x] = NULL;
        }
        delete[] m_ppPatch;
        m_ppPatch = NULL;
    }

    if (m_pMapData) {
        //free(m_pMapData);
        GlobalFree(m_pMapData);
        m_pMapData = NULL;
    }

    if (m_pNormal) {
        GlobalFree(m_pNormal);
        m_pNormal = NULL;
    }

    if (m_ppPatchRadius) {
        for (int x = 0; x < m_pat_MapSize; x++) {
            delete[] m_ppPatchRadius[x];
            m_ppPatchRadius[x] = NULL;
        }
        delete[] m_ppPatchRadius;
        m_ppPatchRadius = NULL;
    }

    if (m_ppPatchMiddleY) {
        for (int x = 0; x < m_pat_MapSize; x++) {
            delete[] m_ppPatchMiddleY[x];
            m_ppPatchMiddleY[x] = NULL;
        }
        delete[] m_ppPatchMiddleY;
        m_ppPatchMiddleY = NULL;
    }

    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            stlMap_N3TexIt itBegin = m_LightMapPatch[x][z].begin();
            stlMap_N3TexIt itEnd = m_LightMapPatch[x][z].end();
            stlMap_N3TexIt it;

            for (it = itBegin; it != itEnd; it++) {
                CN3Texture * pTex = (*it).second;
                if (pTex) {
                    delete pTex;
                }
            }
            m_LightMapPatch[x][z].clear();
        }
    }

    CN3BaseFileAccess::Release();
}

//
//    Init...
//
void CN3Terrain::Init() {
    Release();

    TestAvailableTile();

    m_Material.Init();
    m_ShadeMode = D3DSHADE_GOURAUD;
    m_FillMode = D3DFILL_SOLID;
    //m_FillMode = D3DFILL_WIREFRAME;

    m_pat_CenterPos.x = m_pat_CenterPos.y = -100;
    m_pat_LBPos.x = m_pat_LBPos.y = -100;
    m_pat_PrevLBPos.x = m_pat_PrevLBPos.y = -100;

    SetRectEmpty(&m_pat_BoundRect);
    //m_pat_Center2Side = ((int)CN3Base::s_CameraData.fFP / (PATCH_TILE_SIZE * TILE_SIZE)) + 1;
    //    m_pat_Center2Side = 17;        // CN3Base::s_CameraData.fFP = 512 ��� �����Ҷ�...
    m_pat_Center2Side = 33; // CN3Base::s_CameraData.fFP = 1024 ��� �����Ҷ�...

    m_iNumPatch = (m_pat_Center2Side << 1) + 1;

    m_ppPatch = new CN3TerrainPatch *[m_iNumPatch];
    for (int x = 0; x < m_iNumPatch; x++) {
        m_ppPatch[x] = new CN3TerrainPatch[m_iNumPatch];
    }
    for (int x = 0; x < m_iNumPatch; x++) {
        for (int z = 0; z < m_iNumPatch; z++) {
            m_ppPatch[x][z].Init(this);
        }
    }

    m_pBaseTex.LoadFromFile("Misc\\Terrain_Base.bmp");

    m_iLodLevel = MIN_LOD_LEVEL;
    SetLODLevel(3);

    m_pMapData = NULL;
    m_pNormal = NULL;
    m_ti_MapSize = 0;
    m_pat_MapSize = 0;

    m_ti_CenterPos.x = m_ti_CenterPos.y = -100;
    m_ti_PrevCenterPos = m_ti_CenterPos;

    ZeroMemory(m_pGrassFileName, MAX_PATH);
    for (int i = 0; i < MAX_GRASS; i++) {
        ZeroMemory(m_pGrassTextureName[i], MAX_PATH);
    }
    m_iNumGrass = 0;

    m_NumTileTex = 0;
    m_pTileTex = NULL;
    m_ppColorMapTex = NULL;
    m_iNumColorMap = 0;

    m_pGrassAttr = NULL;
    m_pGrassNum = NULL;

    m_pRiver = new CN3River();
    m_pPond = new CN3Pond();
}

//
//    ����ī�尡 Ÿ�ϸ��� �׸� �� �ִ��� ������ �˻�...
//
void CN3Terrain::TestAvailableTile() {
    m_bAvailableTile = true;

    if (CN3Base::s_DevCaps.MaxTextureBlendStages < 3 ||
        ((CN3Base::s_DevCaps.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) == 0)) {
        CLogWriter::Write("terrain tile not supported..");
        m_bAvailableTile = false;
    }

    return;

    DWORD ColorOP[3], ColorArg1[3], ColorArg2[3];

    CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &ColorOP[0]);
    CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &ColorArg1[0]);

    CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLOROP, &ColorOP[1]);
    CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG1, &ColorArg1[1]);
    CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG2, &ColorArg2[1]);

    CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLOROP, &ColorOP[2]);
    CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLORARG1, &ColorArg1[2]);
    CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLORARG2, &ColorArg2[2]);

    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE);
    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    DWORD   dwNumPasses;
    HRESULT hr;
    hr = CN3Base::s_lpD3DDev->ValidateDevice(&dwNumPasses);

    if (hr & D3DERR_TOOMANYOPERATIONS) {
        m_bAvailableTile = false;
    }
    if (hr & D3DERR_UNSUPPORTEDCOLORARG) {
        m_bAvailableTile = false;
    }
    if (hr & D3DERR_UNSUPPORTEDCOLOROPERATION) {
        m_bAvailableTile = false;
    }

    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, ColorOP[0]);
    CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, ColorArg1[0]);

    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, ColorOP[1]);
    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, ColorArg1[1]);
    CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG2, ColorArg2[1]);

    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLOROP, ColorOP[2]);
    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG1, ColorArg1[2]);
    CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG2, ColorArg2[2]);
}

//
//    Load...
//
bool CN3Terrain::Load(HANDLE hFile) {
    std::string szFNBackup = m_szFileName; // Init �� �ϰ� ���� ���� �̸��� ��������.... �׷���...

    Init();

    m_szFileName = szFNBackup;

    CUILoading * pUILoading = CGameProcedure::s_pUILoading; // �ε���..
    if (pUILoading) {
        pUILoading->Render("Allocating Terrain...", 0);
    }

    DWORD dwRWC;
    int   gtdversion = 0;
    //_________________________________________Loading of (MYKO) <1264 maps version 1 only
    ReadFile(hFile, &(gtdversion), sizeof(int), &dwRWC, NULL);
    if (gtdversion == 1){
        printf("MapVersion: %i 1264 detected\n", gtdversion);
        int iNameLength = 0;
        ReadFile(hFile, &iNameLength, sizeof(int), &dwRWC, NULL);
    if (iNameLength > 0){
        char szMapFName[25];
        ReadFile(hFile, szMapFName, iNameLength, &dwRWC, NULL); // Map name
        printf("Mapname: %s\n", szMapFName);
    }}
    if (gtdversion != 1) {
        DWORD nFilePos = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    }
    //_________________________________________Loading of (MYKO) <1264 maps version 1 only
    ReadFile(hFile, &(m_ti_MapSize), sizeof(int), &dwRWC, NULL);
    m_pat_MapSize = (m_ti_MapSize - 1) / PATCH_TILE_SIZE;

    //m_pMapData = (LPMAPDATA)malloc(sizeof(MAPDATA)*m_ti_MapSize*m_ti_MapSize);
    m_pMapData = (LPMAPDATA)GlobalAlloc(GMEM_FIXED, sizeof(MAPDATA) * m_ti_MapSize * m_ti_MapSize);
    if (m_pMapData == NULL) {
        CLogWriter::Write("Terrain Error : MapData Memory Allocation Failed..-.-");
    }
    __ASSERT(m_pMapData, "MapData Memory Allocation Failed..-.-");
    ReadFile(hFile, m_pMapData, sizeof(MAPDATA) * m_ti_MapSize * m_ti_MapSize, &dwRWC, NULL);

    m_pNormal = (__Vector3 *)GlobalAlloc(GMEM_FIXED, sizeof(__Vector3) * m_ti_MapSize * m_ti_MapSize);
    if (m_pNormal == NULL) {
        CLogWriter::Write("Terrain Error : Normal Vector Memory Allocation Failed..-.-");
    }
    __ASSERT(m_pNormal, "Normal Vector Memory Allocation Failed..-.-");
    SetNormals();

    if (pUILoading) {
        pUILoading->Render("", 100);
    }

    //patch middleY & radius...

    m_ppPatchRadius = new float *[m_pat_MapSize];
    m_ppPatchMiddleY = new float *[m_pat_MapSize];
    for (int x = 0; x < m_pat_MapSize; x++) {
        m_ppPatchMiddleY[x] = new float[m_pat_MapSize];
        m_ppPatchRadius[x] = new float[m_pat_MapSize];
    }

    if (pUILoading) {
        pUILoading->Render("Loading Terrain Patch Data...", 0);
    }

    char szLoadingBuff[128];
    for (int x = 0; x < m_pat_MapSize; x++) {
        for (int z = 0; z < m_pat_MapSize; z++) {
            ReadFile(hFile, &(m_ppPatchMiddleY[x][z]), sizeof(float), &dwRWC, NULL);
            ReadFile(hFile, &(m_ppPatchRadius[x][z]), sizeof(float), &dwRWC, NULL);
        }

        int iLoading = (x + 1) * 100 / m_pat_MapSize;
        sprintf(szLoadingBuff, "Loading Terrain Patch Data... %d %%", iLoading);

        if (pUILoading) {
            pUILoading->Render(szLoadingBuff, iLoading);
        }
    }

    //    m_ppGrassAttr = new unsigned char* [m_ti_MapSize];
    //    for(int x=0; x<m_ti_MapSize; x++)
    //    {
    //        m_ppGrassAttr[x] = new unsigned char[m_ti_MapSize];
    //        ReadFile(hFile, m_ppGrassAttr[x], sizeof(unsigned char)*m_ti_MapSize, &dwRWC, NULL);
    //
    //        if(!(x%256))
    //        {
    //            pUILoading->SetValue(20 + 7 * x / m_ti_MapSize);
    //            pUILoading->Render();
    //        }
    //
    //    }

    //m_pGrassAttr = (unsigned char*)malloc(sizeof(unsigned char)*m_ti_MapSize*m_ti_MapSize);
    m_pGrassAttr = (unsigned char *)GlobalAlloc(GMEM_FIXED, sizeof(unsigned char) * m_ti_MapSize * m_ti_MapSize);
    if (m_pGrassAttr == NULL) {
        CLogWriter::Write("Terrain Error : GrassAttr Data Memory Allocation Failed..-.-");
    }
    __ASSERT(m_pGrassAttr, "GrassAttr Data Memory Allocation Failed..-.-");
    ReadFile(hFile, m_pGrassAttr, sizeof(unsigned char) * m_ti_MapSize * m_ti_MapSize, &dwRWC, NULL);

    //^^vǮ���� ���� �ֱ�...(������ �־��..)
    m_pGrassNum = (unsigned char *)GlobalAlloc(GMEM_FIXED, sizeof(unsigned char) * m_ti_MapSize * m_ti_MapSize);
    if (m_pGrassNum == NULL) {
        CLogWriter::Write("Terrain Error : GrassNum Data Memory Allocation Failed..-.-");
    }
    __ASSERT(m_pGrassNum, "GrassNum Data Memory Allocation Failed..-.-");
    //ReadFile(hFile, m_pGrassNum, sizeof(unsigned char)*m_ti_MapSize*m_ti_MapSize, &dwRWC, NULL);
    memset(m_pGrassNum, 5, sizeof(unsigned char) * m_ti_MapSize * m_ti_MapSize);

    //load colormap....
    ReadFile(hFile, m_pGrassFileName, MAX_PATH, &dwRWC, NULL);
    LoadGrassInfo();

    LoadTileInfo(hFile);

    //load lightmap..
    if (pUILoading) {
        pUILoading->Render("Loading Lightmap Data...", 0);
    }

    int NumLightMap = 0;
    ReadFile(hFile, &NumLightMap, sizeof(int), &dwRWC, NULL);

    short        sx, sz;
    CN3Texture * pTmpTex = new CN3Texture;
    for (int i = 0; i < NumLightMap; i++) {
        ReadFile(hFile, &sx, sizeof(short), &dwRWC, NULL);
        ReadFile(hFile, &sz, sizeof(short), &dwRWC, NULL);
        pTmpTex->Load(hFile);

        //loading bar...
        int iLoading = (i + 1) * 100 / NumLightMap;
        sprintf(szLoadingBuff, "Loading Lightmap Data... %d %%", iLoading);
        if (CGameProcedure::s_pUILoading) {
            CGameProcedure::s_pUILoading->Render(szLoadingBuff, iLoading);
        }
    }
    delete pTmpTex;

    if (pUILoading) {
        pUILoading->Render("Loading River Data...", 0);
    }
    m_pRiver->Load(hFile); // �ʵ����� �ö������� ��ø� ����..2002.11.15
    m_pPond->Load(hFile);

    if (pUILoading) {
        pUILoading->Render("", 100);
    }
    return true;
}

//
//
//
void CN3Terrain::SetNormals() {
    return;
    /*
    __Vector3 vNormalTmp(0.0f, 0.0f, 0.0f);
    __Vector3 V1, V2;
    for(int x=0;x<m_ti_MapSize;x++)
    {
        for(int z=0;z<m_ti_MapSize;z++)
        {
            if( x==0 || z==0 || x==(m_ti_MapSize-1) || z==(m_ti_MapSize-1) )
            {
                m_pNormal[z + x*m_ti_MapSize].Set(0.0f, 1.0f, 0.0f);
            }
            else
            {
                int c = x*m_ti_MapSize + z;
                if ((x+z)%2==1)
                {
                    int u,v;
                    for(int i=0;i<4;i++)
                    {
                        if(i==0) { u = x*m_ti_MapSize + (z+1); v = (x+1)*m_ti_MapSize + z; }
                        else if(i==1) { u = (x+1)*m_ti_MapSize + z; v = x*m_ti_MapSize + (z-1); }
                        else if(i==2) { u = x*m_ti_MapSize + (z-1); v = (x-1)*m_ti_MapSize + z; }
                        else if(i==3) { u = (x-1)*m_ti_MapSize + z; v = x*m_ti_MapSize + (z+1); }                        
                    }

                    V1.Set(0.0f, m_pMapData[u].fHeight - m_pMapData[c].fHeight, 4.0f);
                    V2.Set(4.0f, m_pMapData[v].fHeight - m_pMapData[c].fHeight, 0.0f);
                    vNormalTmp.Cross(V1, V2);
                    vNormalTmp.Normalize();



                }
            }
        }
    }
*/
}

//
//
//
unsigned short CN3Terrain::GetGrassAttr(int x, int z) {
    unsigned short Attr;
    if (x < 0 || x >= m_ti_MapSize || z < 0 || z >= m_ti_MapSize) {
        return 0;
    }
    if (m_pGrassAttr && m_pGrassNum) {
        Attr = (((unsigned short)m_pGrassAttr[x * m_ti_MapSize + z]) << 8) + m_pGrassNum[x * m_ti_MapSize + z];
        return Attr;
    }
    return 0;
}

//
//
//
MAPDATA CN3Terrain::GetMapData(int x, int z) {
    MAPDATA MapData;
    if (x < 0 || x >= m_ti_MapSize || z < 0 || z >= m_ti_MapSize) {
        return MapData;
    }
    MapData = m_pMapData[(x * m_ti_MapSize) + z];

    return MapData;
}

//
//
//
void CN3Terrain::LoadGrassInfo() {
    if (CGameProcedure::s_pUILoading) {
        CGameProcedure::s_pUILoading->Render("Loading Terrain Grass Data...", 0);
    }

    m_iNumGrass = 0;
    if (strcmp(m_pGrassFileName, "") == 0) {
        ZeroMemory(m_pGrassAttr, sizeof(unsigned char) * m_ti_MapSize * m_ti_MapSize);
        return;
    }

    char szDrive[_MAX_DRIVE];
    char szDir[_MAX_DIR];
    char szGrassDir[_MAX_DIR];
    char szModuleFilePath[_MAX_PATH];
    GetModuleFileName(NULL, szModuleFilePath, _MAX_PATH);
    _splitpath(szModuleFilePath, szDrive, szDir, NULL, NULL);
    char szDir2[MAX_PATH];
    sprintf(szDir2, "misc\\grass");
    sprintf(szGrassDir, "%s%s", szDir, szDir2);

    char szFullPath[MAX_PATH];
    _makepath(szFullPath, szDrive, szGrassDir, m_pGrassFileName, "grs");

    DWORD  dwRWC;
    HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    char Buff[80];
    if (!ReadFile(hFile, Buff, 80, &dwRWC, NULL)) {
        CloseHandle(hFile);
        return;
    }
    if (strcmp(Buff, "GrassInfoFile") != 0) {
        CloseHandle(hFile);
        return;
    }

    if (!ReadFile(hFile, &m_iNumGrass, sizeof(int), &dwRWC, NULL)) {
        CloseHandle(hFile);
        return;
    }

    int  id;
    char FileName[MAX_PATH];
    char szLoadingBuff[128];
    for (int i = 0; i < m_iNumGrass; i++) {
        if (!ReadFile(hFile, &id, sizeof(int), &dwRWC, NULL)) {
            CloseHandle(hFile);
            return;
        }
        if (!ReadFile(hFile, FileName, MAX_PATH, &dwRWC, NULL)) {
            CloseHandle(hFile);
            return;
        }

        char szDxtFullPath[_MAX_PATH];
        _makepath(szDxtFullPath, szDrive, szGrassDir, FileName, NULL);

        strcpy(m_pGrassTextureName[Log2(id)], szDxtFullPath);

        //loading bar...
        int iLoading = (i + 1) * 100 / m_iNumGrass;
        sprintf(szLoadingBuff, "Loading Terrain Grass Data... %d %%", iLoading);
        if (CGameProcedure::s_pUILoading) {
            CGameProcedure::s_pUILoading->Render(szLoadingBuff, iLoading);
        }
    }
    CloseHandle(hFile);
}

//
//
//
void CN3Terrain::LoadTileInfo(HANDLE hFile) {
    if (CGameProcedure::s_pUILoading) {
        CGameProcedure::s_pUILoading->Render("Loading Terrain Tile Data...", 0);
    }

    DWORD dwRWC;
    ReadFile(hFile, &m_NumTileTex, sizeof(int), &dwRWC, NULL);
    if (m_NumTileTex == 0) {
        return;
    }

    m_pTileTex = new CN3Texture[m_NumTileTex];

    int NumTileTexSrc;
    ReadFile(hFile, &NumTileTexSrc, sizeof(int), &dwRWC, NULL);
    if (NumTileTexSrc == 0) {
        return;
    }

    char ** SrcName = new char *[NumTileTexSrc];
    for (int i = 0; i < NumTileTexSrc; i++) {
        SrcName[i] = new char[MAX_PATH];
        ReadFile(hFile, SrcName[i], MAX_PATH, &dwRWC, NULL);
    }

    short  SrcIdx, TileIdx;
    HANDLE hTTGFile;
    char   szLoadingBuff[128];
    for (int i = 0; i < m_NumTileTex; i++) {
        ReadFile(hFile, &SrcIdx, sizeof(short), &dwRWC, NULL);
        ReadFile(hFile, &TileIdx, sizeof(short), &dwRWC, NULL);

        hTTGFile = CreateFile(SrcName[SrcIdx], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        for (int j = 0; j < TileIdx; j++) {
            //            m_pTileTex[i].m_iLOD = s_Options.iTexLOD_Terrain; // LOD ������ �б�..
            //            m_pTileTex[i].Load(hTTGFile);// �տ� �ִ� ���� ���� �͵�...
            m_pTileTex[i].SkipFileHandle(hTTGFile); // �տ� �ִ� ���� ���� �͵�...
        }
        m_pTileTex[i].m_iLOD = s_Options.iTexLOD_Terrain; // LOD ������ �б�..
        m_pTileTex[i].Load(hTTGFile);                     // ��¥ Ÿ��...

        //loading bar...
        int iLoading = (i + 1) * 100 / m_NumTileTex;
        sprintf(szLoadingBuff, "Loading Terrain Tile Data... %d %%", iLoading);
        if (CGameProcedure::s_pUILoading) {
            CGameProcedure::s_pUILoading->Render(szLoadingBuff, iLoading);
        }

        CloseHandle(hTTGFile);
    }

    for (int i = 0; i < NumTileTexSrc; i++) {
        delete[] SrcName[i];
        SrcName[i] = NULL;
    }

    delete[] SrcName;
}

//
//    lod level ����..
//    default�� 3...
//    min = 0, max = 10..
//
bool CN3Terrain::SetLODLevel(int level) {
    if (level == m_iLodLevel) {
        return false;
    }
    m_iLodLevel = level;

    if (m_iLodLevel < 2) {
        m_iLodLevel = 2;
    }

    for (int x = 0; x < m_iNumPatch; x++) {
        for (int z = 0; z < m_iNumPatch; z++) {
            int dist = m_iDistanceTable[T_Abs(m_pat_Center2Side - x)][T_Abs(m_pat_Center2Side - z)];
            if (dist <= m_iLodLevel) {
                m_ppPatch[x][z].SetLevel(1);
            } else if (dist <= m_iLodLevel + 3) {
                m_ppPatch[x][z].SetLevel(2);
            } else {
                m_ppPatch[x][z].SetLevel(3);
            }
        }
    }

    SetBlunt();

    return true;
}

//
//    SetBlunt...
//    �� ��ġ�� �׸���� ���ϱ�..������� ����� �Ұ�����..
//
void CN3Terrain::SetBlunt() {
    for (int x = 0; x < m_iNumPatch; x++) {
        for (int z = 0; z < m_iNumPatch; z++) {
            m_ppPatch[x][z].m_IsBlunt[0] = true;
            m_ppPatch[x][z].m_IsBlunt[1] = true;
            m_ppPatch[x][z].m_IsBlunt[2] = true;
            m_ppPatch[x][z].m_IsBlunt[3] = true;
            if (m_ppPatch[x][z].GetLevel() == 1) {
                m_ppPatch[x][z].m_IsBlunt[0] = false;
                m_ppPatch[x][z].m_IsBlunt[1] = false;
                m_ppPatch[x][z].m_IsBlunt[2] = false;
                m_ppPatch[x][z].m_IsBlunt[3] = false;
                continue;
            }

            if (m_ppPatch[x][z].GetLevel() == 2) {
                if (x > 0) {
                    if (m_ppPatch[x][z].GetLevel() > m_ppPatch[x - 1][z].GetLevel()) {
                        m_ppPatch[x][z].m_IsBlunt[0] = false;
                    }
                }
                if ((z + 1) < m_iNumPatch) {
                    if (m_ppPatch[x][z].GetLevel() > m_ppPatch[x][z + 1].GetLevel()) {
                        m_ppPatch[x][z].m_IsBlunt[1] = false;
                    }
                }
                if ((x + 1) < m_iNumPatch) {
                    if (m_ppPatch[x][z].GetLevel() > m_ppPatch[x + 1][z].GetLevel()) {
                        m_ppPatch[x][z].m_IsBlunt[2] = false;
                    }
                }
                if (z > 0) {
                    if (m_ppPatch[x][z].GetLevel() > m_ppPatch[x][z - 1].GetLevel()) {
                        m_ppPatch[x][z].m_IsBlunt[3] = false;
                    }
                }
                continue;
            }

            if (x > 0) {
                if (m_ppPatch[x][z].GetLevel() >= m_ppPatch[x - 1][z].GetLevel()) {
                    m_ppPatch[x][z].m_IsBlunt[0] = false;
                }
            }
            if ((z + 1) < m_iNumPatch) {
                if (m_ppPatch[x][z].GetLevel() >= m_ppPatch[x][z + 1].GetLevel()) {
                    m_ppPatch[x][z].m_IsBlunt[1] = false;
                }
            }
            if ((x + 1) < m_iNumPatch) {
                if (m_ppPatch[x][z].GetLevel() >= m_ppPatch[x + 1][z].GetLevel()) {
                    m_ppPatch[x][z].m_IsBlunt[2] = false;
                }
            }
            if (z > 0) {
                if (m_ppPatch[x][z].GetLevel() >= m_ppPatch[x][z - 1].GetLevel()) {
                    m_ppPatch[x][z].m_IsBlunt[3] = false;
                }
            }
        }
    }
}

//
//    Tick..
//
void CN3Terrain::Tick() {
    int iLOD = 0; // LOD ���� ���.. ���߿� ������ �ٲپ�� �Ѵ�.
    iLOD = (int)(3.0f * s_CameraData.fFP / 512.0f);
    bool ChangeLOD = this->SetLODLevel(iLOD);

    m_pat_PrevLBPos = m_pat_LBPos;
    m_ti_PrevCenterPos = m_ti_CenterPos;

    bool bMovePatch = CheckMovePatch();
    if (bMovePatch || ChangeLOD) {
        DispositionPatch();
    }

    bool bChangeBound = CheckBound();

    if ((bMovePatch) || (bChangeBound) || ChangeLOD) {
        for (int x = m_pat_BoundRect.left; x <= m_pat_BoundRect.right; x++) {
            for (int z = m_pat_BoundRect.top; z <= m_pat_BoundRect.bottom; z++) {
                if (x < 0 || z < 0) {
                    continue;
                }
                m_ppPatch[x][z].Tick();
            }
        }
    }
    m_pRiver->Tick();
    m_pPond->Tick();
}

//
//    CheckMovePatch
//    ��ġ������ �̵��� �̷�� ������...
//
bool CN3Terrain::CheckMovePatch() {
    m_ti_CenterPos.x = Real2Tile(CN3Base::s_CameraData.vEye.x);
    m_ti_CenterPos.y = Real2Tile(CN3Base::s_CameraData.vEye.z);

    m_pat_LBPos.x = Tile2Patch(m_ti_CenterPos.x) - m_pat_Center2Side;
    m_pat_LBPos.y = Tile2Patch(m_ti_CenterPos.y) - m_pat_Center2Side;

    if (m_pat_PrevLBPos.x == m_pat_LBPos.x && m_pat_PrevLBPos.y == m_pat_LBPos.y) {
        return false;
    }

    return true;
}

//
//    DispositionPatch
//
void CN3Terrain::DispositionPatch() {
    for (int x = 0; x < m_iNumPatch; x++) {
        for (int z = 0; z < m_iNumPatch; z++) {
            int px = m_pat_LBPos.x + x;
            int pz = m_pat_LBPos.y + z;

            if (px < 0 || pz < 0 || px >= m_pat_MapSize || pz >= m_pat_MapSize) {
                continue;
            }

            m_ppPatch[x][z].m_ti_LBPoint.x = px * PATCH_TILE_SIZE;
            m_ppPatch[x][z].m_ti_LBPoint.y = pz * PATCH_TILE_SIZE;

            int cx = px * PATCH_PIXEL_SIZE / COLORMAPTEX_SIZE;
            int cz = pz * PATCH_PIXEL_SIZE / COLORMAPTEX_SIZE;
            if (cx < 0 || cz < 0 || cx >= m_iNumColorMap || cz >= m_iNumColorMap) {
                m_ppPatch[x][z].m_pRefColorTex = NULL;
            } else {
                m_ppPatch[x][z].m_pRefColorTex = &(m_ppColorMapTex[cx][cz]);
            }
        }
    }

    //lightmap�о ��ġ�ϰ�...
    //�ִ��� �����...
    POINT PrevCenter = m_pat_CenterPos;
    m_pat_CenterPos.x = m_pat_LBPos.x + (m_iNumPatch / 2);
    m_pat_CenterPos.y = m_pat_LBPos.y + (m_iNumPatch / 2);

    if (PrevCenter.x == m_pat_CenterPos.x) {
        if (PrevCenter.y == m_pat_CenterPos.y) {
            SetLightMap(DIR_CM);
        } else if (PrevCenter.y == (m_pat_CenterPos.y + 1)) {
            SetLightMap(DIR_CB);
        } else if (PrevCenter.y == (m_pat_CenterPos.y - 1)) {
            SetLightMap(DIR_CT);
        } else {
            SetLightMap(DIR_WARP);
        }
    } else if (PrevCenter.x == (m_pat_CenterPos.x - 1)) {
        if (PrevCenter.y == m_pat_CenterPos.y) {
            SetLightMap(DIR_RM);
        } else if (PrevCenter.y == (m_pat_CenterPos.y + 1)) {
            SetLightMap(DIR_RB);
        } else if (PrevCenter.y == (m_pat_CenterPos.y - 1)) {
            SetLightMap(DIR_RT);
        } else {
            SetLightMap(DIR_WARP);
        }
    } else if (PrevCenter.x == (m_pat_CenterPos.x + 1)) {
        if (PrevCenter.y == m_pat_CenterPos.y) {
            SetLightMap(DIR_LM);
        } else if (PrevCenter.y == (m_pat_CenterPos.y + 1)) {
            SetLightMap(DIR_LB);
        } else if (PrevCenter.y == (m_pat_CenterPos.y - 1)) {
            SetLightMap(DIR_LT);
        } else {
            SetLightMap(DIR_WARP);
        }
    } else {
        SetLightMap(DIR_WARP);
    }

    //m_pRiver->SetPatchPos(m_pat_LBPos.x+m_pat_Center2Side, m_pat_LBPos.y+m_pat_Center2Side);
}

//
//
//
void CN3Terrain::SetLightMap(int dir) {
    return; // TODO: I suppose this needs to be reworked.
    __TABLE_ZONE * pZoneData = CGameBase::s_pTbl_Zones->Find(CGameBase::s_pPlayer->m_InfoExt.iZoneCur);
    if (!pZoneData) {
        return;
    }

    HANDLE hFile =
        CreateFile(pZoneData->szLightMapFN.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFile) {
        return;
    }

    DWORD dwRWC;
    int * Addr = new int[m_pat_MapSize * m_pat_MapSize];
    int   iVersion;
    BOOL  b = ReadFile(hFile, &(iVersion), sizeof(int), &dwRWC, NULL);
    b = ReadFile(hFile, &(Addr[0]), sizeof(int) * m_pat_MapSize * m_pat_MapSize, &dwRWC, NULL);

    //DIR_LT = 0, DIR_CT = 1, DIR_RT = 2,
    //DIR_LM = 3, DIR_CM = 4, DIR_RM = 5,
    //DIR_LB = 6, DIR_CB = 7, DIR_RB = 8,
    //DIR_WARP = 9
    switch (dir) {
    case DIR_LT: {
        SetLightMapPatch(0, 0, hFile, Addr);
        ReplaceLightMapPatch(1, 0, m_LightMapPatch[0][1]);
        ReplaceLightMapPatch(2, 0, m_LightMapPatch[1][1]);

        SetLightMapPatch(0, 1, hFile, Addr);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[0][2]);
        ReplaceLightMapPatch(2, 1, m_LightMapPatch[1][2]);

        SetLightMapPatch(0, 2, hFile, Addr);
        SetLightMapPatch(1, 2, hFile, Addr);
        SetLightMapPatch(2, 2, hFile, Addr);
        break;
    }
    case DIR_CT: {
        ReplaceLightMapPatch(0, 0, m_LightMapPatch[0][1]);
        ReplaceLightMapPatch(1, 0, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(2, 0, m_LightMapPatch[2][1]);

        ReplaceLightMapPatch(0, 1, m_LightMapPatch[0][2]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[1][2]);
        ReplaceLightMapPatch(2, 1, m_LightMapPatch[2][2]);

        SetLightMapPatch(0, 2, hFile, Addr);
        SetLightMapPatch(1, 2, hFile, Addr);
        SetLightMapPatch(2, 2, hFile, Addr);
        break;
    }
    case DIR_RT: {
        ReplaceLightMapPatch(0, 0, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(1, 0, m_LightMapPatch[2][1]);
        SetLightMapPatch(2, 0, hFile, Addr);

        ReplaceLightMapPatch(0, 1, m_LightMapPatch[1][2]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[2][2]);
        SetLightMapPatch(2, 1, hFile, Addr);

        SetLightMapPatch(0, 2, hFile, Addr);
        SetLightMapPatch(1, 2, hFile, Addr);
        SetLightMapPatch(2, 2, hFile, Addr);
        break;
    }
    case DIR_LM: {
        ReplaceLightMapPatch(2, 0, m_LightMapPatch[1][0]);
        ReplaceLightMapPatch(2, 1, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(2, 2, m_LightMapPatch[1][2]);

        ReplaceLightMapPatch(1, 0, m_LightMapPatch[0][0]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[0][1]);
        ReplaceLightMapPatch(1, 2, m_LightMapPatch[0][2]);

        SetLightMapPatch(0, 0, hFile, Addr);
        SetLightMapPatch(0, 1, hFile, Addr);
        SetLightMapPatch(0, 2, hFile, Addr);
        break;
    }
    case DIR_WARP: {
        SetLightMapPatch(0, 0, hFile, Addr);
        SetLightMapPatch(1, 0, hFile, Addr);
        SetLightMapPatch(2, 0, hFile, Addr);

        SetLightMapPatch(0, 1, hFile, Addr);
        SetLightMapPatch(1, 1, hFile, Addr);
        SetLightMapPatch(2, 1, hFile, Addr);

        SetLightMapPatch(0, 2, hFile, Addr);
        SetLightMapPatch(1, 2, hFile, Addr);
        SetLightMapPatch(2, 2, hFile, Addr);
        break;
    }
    case DIR_RM: {
        ReplaceLightMapPatch(0, 0, m_LightMapPatch[1][0]);
        ReplaceLightMapPatch(0, 1, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(0, 2, m_LightMapPatch[1][2]);

        ReplaceLightMapPatch(1, 0, m_LightMapPatch[2][0]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[2][1]);
        ReplaceLightMapPatch(1, 2, m_LightMapPatch[2][2]);

        SetLightMapPatch(2, 0, hFile, Addr);
        SetLightMapPatch(2, 1, hFile, Addr);
        SetLightMapPatch(2, 2, hFile, Addr);
        break;
    }
    case DIR_LB: {
        ReplaceLightMapPatch(2, 2, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(1, 2, m_LightMapPatch[0][1]);
        SetLightMapPatch(0, 2, hFile, Addr);

        ReplaceLightMapPatch(2, 1, m_LightMapPatch[1][0]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[0][0]);
        SetLightMapPatch(0, 1, hFile, Addr);

        SetLightMapPatch(0, 0, hFile, Addr);
        SetLightMapPatch(1, 0, hFile, Addr);
        SetLightMapPatch(2, 0, hFile, Addr);
        break;
    }
    case DIR_CB: {
        ReplaceLightMapPatch(0, 2, m_LightMapPatch[0][1]);
        ReplaceLightMapPatch(1, 2, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(2, 2, m_LightMapPatch[2][1]);

        ReplaceLightMapPatch(0, 1, m_LightMapPatch[0][0]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[1][0]);
        ReplaceLightMapPatch(2, 1, m_LightMapPatch[2][0]);

        SetLightMapPatch(0, 0, hFile, Addr);
        SetLightMapPatch(1, 0, hFile, Addr);
        SetLightMapPatch(2, 0, hFile, Addr);
        break;
    }
    case DIR_RB: {
        ReplaceLightMapPatch(0, 2, m_LightMapPatch[1][1]);
        ReplaceLightMapPatch(1, 2, m_LightMapPatch[2][1]);
        SetLightMapPatch(2, 2, hFile, Addr);

        ReplaceLightMapPatch(0, 1, m_LightMapPatch[1][0]);
        ReplaceLightMapPatch(1, 1, m_LightMapPatch[2][0]);
        SetLightMapPatch(2, 1, hFile, Addr);

        SetLightMapPatch(0, 0, hFile, Addr);
        SetLightMapPatch(1, 0, hFile, Addr);
        SetLightMapPatch(2, 0, hFile, Addr);
        break;
    }
    }

    delete[] Addr;
    CloseHandle(hFile);
}

//
//
//
void CN3Terrain::ReplaceLightMapPatch(int x, int z, stlMap_N3Tex & LightMapPatch) {
    stlMap_N3TexIt itBegin = m_LightMapPatch[x][z].begin();
    stlMap_N3TexIt itEnd = m_LightMapPatch[x][z].end();
    stlMap_N3TexIt it;

    for (it = itBegin; it != itEnd; it++) {
        CN3Texture * pTex = (*it).second;
        if (pTex) {
            delete pTex;
        }
    }
    m_LightMapPatch[x][z].clear();
    m_LightMapPatch[x][z] = LightMapPatch;
    LightMapPatch.clear();
}

//
//
//
void CN3Terrain::SetLightMapPatch(int x, int z, HANDLE hFile, int * pAddr) {
    stlMap_N3TexIt itBegin = m_LightMapPatch[x][z].begin();
    stlMap_N3TexIt itEnd = m_LightMapPatch[x][z].end();
    stlMap_N3TexIt it;

    for (it = itBegin; it != itEnd; it++) {
        CN3Texture * pTex = (*it).second;
        if (pTex) {
            delete pTex;
        }
    }
    m_LightMapPatch[x][z].clear();

    DWORD dwRWC;
    int   px, pz;
    px = m_pat_CenterPos.x - 1 + x;
    pz = m_pat_CenterPos.y - 1 + z;

    if (px < 0 || px >= m_pat_MapSize || pz < 0 || pz >= m_pat_MapSize) {
        return;
    }

    int jump = pAddr[px + (m_pat_MapSize * pz)];
    if (jump <= 0) {
        return;
    }
    DWORD dwPtr = SetFilePointer(hFile, jump, NULL, FILE_BEGIN);

    int TexCount;
    ReadFile(hFile, &TexCount, sizeof(int), &dwRWC, NULL);

    for (int i = 0; i < TexCount; i++) {
        int tx, tz;
        ReadFile(hFile, &tx, sizeof(int), &dwRWC, NULL);
        ReadFile(hFile, &tz, sizeof(int), &dwRWC, NULL);

        CN3Texture * pTex = new CN3Texture;
        pTex->Load(hFile);
        int rtx = px * PATCH_TILE_SIZE + tx;
        int rtz = pz * PATCH_TILE_SIZE + tz;

        DWORD key = rtx * 10000 + rtz;
        m_LightMapPatch[x][z].insert(stlMap_N3TexValue(key, pTex));
    }
}

//
//
//
CN3Texture * CN3Terrain::GetLightMap(int tx, int tz) {
    int px = tx / PATCH_TILE_SIZE;
    int pz = tz / PATCH_TILE_SIZE;

    px -= (m_pat_CenterPos.x - 1);
    pz -= (m_pat_CenterPos.y - 1);
    if (px < 0 || px > 2 || pz < 0 || pz > 2) {
        return NULL;
    }

    DWORD          key = tx * 10000 + tz;
    stlMap_N3TexIt it = m_LightMapPatch[px][pz].find(key);
    if (it != m_LightMapPatch[px][pz].end()) {
        return (*it).second;
    }
    return NULL;
}

//
//    CheckBounce...
//    ��ġ������ ���ÿ��� �˻�..
//    �������� return true...
//
bool CN3Terrain::CheckBound() {
    RECT prevPatRc = m_pat_BoundRect;

    RECT rc;
    rc.left = rc.right = Real2Patch(CN3Base::s_CameraData.vEye.x);
    rc.top = rc.bottom = Real2Patch(CN3Base::s_CameraData.vEye.z);

    // ���ü�� ���� ���Ϳ� Far �� ������ ��ġ ���..
    float fS = sinf(CN3Base::s_CameraData.fFOV / 2.0f);
    float fPL = CN3Base::s_CameraData.fFP;
    float fAspect = CN3Base::s_CameraData.fAspect; // ��Ⱦ��

    // Far Plane �� �� ������ ��ġ ���
    __Vector3 vFPs[4] = {__Vector3(fPL * -fS * fAspect, fPL * fS, fPL),   // LeftTop
                         __Vector3(fPL * fS * fAspect, fPL * fS, fPL),    // rightTop
                         __Vector3(fPL * fS * fAspect, fPL * -fS, fPL),   // RightBottom
                         __Vector3(fPL * -fS * fAspect, fPL * -fS, fPL)}; // LeftBottom
    // ������ ��ġ�� ȸ�� ����� �����Ѵ�..
    for (int i = 0; i < 4; i++) {
        vFPs[i] = vFPs[i] * CN3Base::s_CameraData.mtxViewInverse;
    }

    for (int i = 0; i < 4; i++) {
        POINT FarPoint;
        FarPoint.x = Real2Patch(vFPs[i].x);
        FarPoint.y = Real2Patch(vFPs[i].z);

        if (FarPoint.x < rc.left) {
            rc.left = FarPoint.x;
        }
        if (FarPoint.x > rc.right) {
            rc.right = FarPoint.x;
        }
        if (FarPoint.y < rc.top) {
            rc.top = FarPoint.y;
        }
        if (FarPoint.y > rc.bottom) {
            rc.bottom = FarPoint.y;
        }
    }

    rc.left = rc.left - m_pat_LBPos.x - 1;
    rc.right = rc.right - m_pat_LBPos.x + 1;
    rc.top = rc.top - m_pat_LBPos.y - 1;
    rc.bottom = rc.bottom - m_pat_LBPos.y + 1;

    if (rc.left < 0) {
        rc.left = 0;
    }
    if (rc.left >= m_iNumPatch) {
        rc.left = m_iNumPatch - 1;
    }
    if (rc.right < 0) {
        rc.right = 0;
    }
    if (rc.right >= m_iNumPatch) {
        rc.right = m_iNumPatch - 1;
    }
    if (rc.top < 0) {
        rc.top = 0;
    }
    if (rc.top >= m_iNumPatch) {
        rc.top = m_iNumPatch - 1;
    }
    if (rc.bottom < 0) {
        rc.bottom = 0;
    }
    if (rc.bottom >= m_iNumPatch) {
        rc.bottom = m_iNumPatch - 1;
    }

    m_pat_BoundRect = rc;

    bool bChangeRender = CheckRenderablePatch();

    if (!bChangeRender && EqualRect(&m_pat_BoundRect, &prevPatRc)) {
        return false;
    }

    return true;
}

//
//
//
bool CN3Terrain::CheckRenderablePatch() {
    bool      bChange = false;
    __Vector3 CenterPoint;
    BOOL      PrevState;
    for (int x = m_pat_BoundRect.left; x <= m_pat_BoundRect.right; x++) {
        for (int z = m_pat_BoundRect.top; z <= m_pat_BoundRect.bottom; z++) {
            if (x < 0 || z < 0) {
                continue;
            }

            PrevState = m_ppPatch[x][z].m_bIsRender;
            m_ppPatch[x][z].m_bIsRender = TRUE;

            int px = m_pat_LBPos.x + x;
            int pz = m_pat_LBPos.y + z;

            if (px < 0 || pz < 0 || px >= m_pat_MapSize || pz >= m_pat_MapSize) {
                m_ppPatch[x][z].m_bIsRender = FALSE;
                continue;
            }

            CenterPoint.Set((float)((m_ppPatch[x][z].m_ti_LBPoint.x + (PATCH_TILE_SIZE >> 1)) * TILE_SIZE),
                            m_ppPatchMiddleY[px][pz],
                            (float)((m_ppPatch[x][z].m_ti_LBPoint.y + (PATCH_TILE_SIZE >> 1)) * TILE_SIZE));

            m_ppPatch[x][z].m_bIsRender =
                (!(CN3Base::s_CameraData.IsOutOfFrustum(CenterPoint, m_ppPatchRadius[px][pz] * 2.0f)));
            if (m_ppPatch[x][z].m_bIsRender != PrevState) {
                bChange = true;
            }
        }
    }
    return bChange;
}

//
//
//
void CN3Terrain::Render() {
    __Matrix44 WorldMtx;
    WorldMtx.Identity();
    CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &WorldMtx);

    __Material mtl;
    mtl.Init();
    CN3Base::s_lpD3DDev->SetMaterial(&mtl);

    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, m_FillMode);
    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SHADEMODE, m_ShadeMode);

    DWORD CullMode, ZEnable;
    CN3Base::s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &CullMode);
    CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &ZEnable);

    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    //CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZBIAS, 1);

    DWORD ColorOP0, ColorOP1, ColorOP2;
    DWORD ColorArg01, ColorArg02, ColorArg11, ColorArg12, ColorArg21, ColorArg22;

    HRESULT hr;
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &ColorOP0);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &ColorArg01);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG2, &ColorArg02);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLOROP, &ColorOP1);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG1, &ColorArg11);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG2, &ColorArg12);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLOROP, &ColorOP2);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLORARG1, &ColorArg21);
    hr = CN3Base::s_lpD3DDev->GetTextureStageState(2, D3DTSS_COLORARG2, &ColorArg22);

    DWORD AddressU1, AddressV1, AddressU2, AddressV2;
    hr = s_lpD3DDev->GetSamplerState(0, D3DSAMP_ADDRESSU, &AddressU1);
    hr = s_lpD3DDev->GetSamplerState(0, D3DSAMP_ADDRESSV, &AddressV1);
    hr = s_lpD3DDev->GetSamplerState(1, D3DSAMP_ADDRESSU, &AddressU2);
    hr = s_lpD3DDev->GetSamplerState(1, D3DSAMP_ADDRESSV, &AddressV2);

    // ������ �ؽ��ĵ��� ���������� ��輱�� ���� �� �ִ�..^^
    hr = s_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
    hr = s_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
    hr = s_lpD3DDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
    hr = s_lpD3DDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

    for (int x = m_pat_BoundRect.left; x <= m_pat_BoundRect.right; x++) {
        for (int z = m_pat_BoundRect.top; z <= m_pat_BoundRect.bottom; z++) {
            if (x < 0 || z < 0) {
                continue;
            }
            m_ppPatch[x][z].Render();
        }
    }

    hr = s_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, AddressU1);
    hr = s_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, AddressV1);
    hr = s_lpD3DDev->SetSamplerState(1, D3DSAMP_ADDRESSU, AddressU2);
    hr = s_lpD3DDev->SetSamplerState(1, D3DSAMP_ADDRESSV, AddressV2);

    // restor texture stage state settings...
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, ColorOP0);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, ColorArg01);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, ColorArg02);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, ColorOP1);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, ColorArg11);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG2, ColorArg12);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLOROP, ColorOP2);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG1, ColorArg21);
    hr = CN3Base::s_lpD3DDev->SetTextureStageState(2, D3DTSS_COLORARG2, ColorArg22);
    hr = CN3Base::s_lpD3DDev->SetTexture(0, NULL);
    hr = CN3Base::s_lpD3DDev->SetTexture(1, NULL);
    hr = CN3Base::s_lpD3DDev->SetTexture(2, NULL);

    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, CullMode);
    CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, ZEnable);

    m_pRiver->Render();
    m_pPond->Render();
}

//
//    Log2(x) = l..
//    2�� �¼��� ���ؼ��� ����� �۵�...(x>0)
//
inline int CN3Terrain::Log2(int x) {
    int l = 0;
    while (x != 1) {
        x = x >> 1;
        l++;
    }
    return l;
}

//
//
//
float CN3Terrain::GetHeight(float x, float z) {
    int ix = ((int)x) / TILE_SIZE;
    int iz = ((int)z) / TILE_SIZE;

    if (ix < 0 || ix > (m_ti_MapSize - 2)) {
        return -FLT_MAX;
    }
    if (iz < 0 || iz > (m_ti_MapSize - 2)) {
        return -FLT_MAX;
    }

    float dX = (x - (ix * TILE_SIZE)) / TILE_SIZE;
    float dZ = (z - (iz * TILE_SIZE)) / TILE_SIZE;

    float y;
    float h1, h2, h3, h12, h13;

    if ((ix + iz) % 2 == 0) //�簢���� / ���..
    {
        h1 = m_pMapData[ix * m_ti_MapSize + iz].fHeight;
        h3 = m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight;
        if (dZ > dX) //���� �ﰢ��..
        {
            h2 = m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight;

            h12 = h1 + (h2 - h1) * dZ; // h1�� h2������ ���̰�
            h13 = h1 + (h3 - h1) * dZ; // h1�� h3������ ���̰�

            y = h12 + ((h13 - h12) * (dX / dZ)); // ã���� �ϴ� ���̰�
            return y;
        } else //�Ʒ��� �ﰢ��..
        {
            if (dX == 0.0f) {
                return h1;
            }

            h2 = m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight;

            h12 = h1 + (h2 - h1) * dX; // h1�� h2������ ���̰�
            h13 = h1 + (h3 - h1) * dX; // h1�� h3������ ���̰�

            y = h12 + ((h13 - h12) * (dZ / dX)); // ã���� �ϴ� ���̰�
            return y;
        }
    }

    else if ((ix + iz) % 2 == 1) //�簢���� �������� ���..
    {
        h1 = m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight;
        h3 = m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight;

        if ((dX + dZ) > 1.0f) //���� �ﰢ��..
        {
            if (dZ == 0.0f) {
                return h1;
            }
            h2 = m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight;

            h12 = h1 + (h2 - h1) * dZ;
            h13 = h1 + (h3 - h1) * dZ;

            y = h12 + ((h13 - h12) * ((1.0f - dX) / dZ));
            return y;
        } else //�Ʒ��� �ﰢ��..
        {
            if (dX == 1.0f) {
                return h1;
            }
            h2 = m_pMapData[ix * m_ti_MapSize + iz].fHeight;

            h12 = h2 + (h1 - h2) * dX; // h1�� h2������ ���̰�
            h13 = h3 + (h1 - h3) * dX; // h1�� h3������ ���̰�

            y = h12 + ((h13 - h12) * (dZ / (1.0f - dX)));
            return y;
        }
    }

    return -FLT_MAX;
}

//
//    GetNormal(float x, float z, __Vector3& vNormal)
//
void CN3Terrain::GetNormal(float x, float z, __Vector3 & vNormal) {
    if (x < 10.0f || x > ((m_ti_MapSize - 1) * TILE_SIZE - 10.0f) || z < 10.0f ||
        z > ((m_ti_MapSize - 1) * TILE_SIZE - 10.0f)) {
        vNormal.Set(0.0f, 1.0f, 0.0f);
        return;
    }

    int ix = ((int)x) / TILE_SIZE;
    int iz = ((int)z) / TILE_SIZE;

    float dX = (x - ix * TILE_SIZE) / TILE_SIZE;
    float dZ = (z - iz * TILE_SIZE) / TILE_SIZE;

    __Vector3 v1, v2;
    vNormal.Set(0, 1, 0);
    float Height;
    if ((ix + iz) % 2 == 1) {
        if ((dX + dZ) < 1.0f) {
            Height = m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight - m_pMapData[ix * m_ti_MapSize + iz].fHeight;
            v1.Set(0, Height, TILE_SIZE);

            Height = m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight - m_pMapData[ix * m_ti_MapSize + iz].fHeight;
            v2.Set(TILE_SIZE, Height, 0);

            vNormal.Cross(v1, v2);
            return;
        } else {
            Height = m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight -
                     m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight;
            v1.Set(0.0f, Height, (-1) * TILE_SIZE);

            Height = m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight -
                     m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight;
            v2.Set((-1) * TILE_SIZE, Height, 0.0f);

            vNormal.Cross(v1, v2);
            return;
        }
    } else {
        if (dZ > dX) {
            Height = m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight -
                     m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight;
            v1.Set(TILE_SIZE, Height, 0.0f);

            Height = m_pMapData[ix * m_ti_MapSize + iz].fHeight - m_pMapData[ix * m_ti_MapSize + (iz + 1)].fHeight;
            v2.Set(0.0f, Height, (-1) * TILE_SIZE);

            vNormal.Cross(v1, v2);
            return;
        } else {
            Height = m_pMapData[ix * m_ti_MapSize + iz].fHeight - m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight;
            v1.Set((-1) * TILE_SIZE, Height, 0.0f);

            Height = m_pMapData[(ix + 1) * m_ti_MapSize + (iz + 1)].fHeight -
                     m_pMapData[(ix + 1) * m_ti_MapSize + iz].fHeight;
            v2.Set(0.0f, Height, TILE_SIZE);

            vNormal.Cross(v1, v2);
            return;
        }
    }
    return;
}

//
//
//
bool CN3Terrain::IsInTerrain(float x, float z) {
    if (x < 30.0f || x > ((m_ti_MapSize - 1) * TILE_SIZE - 30.0f) || z < 30.0f ||
        z > ((m_ti_MapSize - 1) * TILE_SIZE - 30.0f)) {
        return false;
    }
    return true;
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////////
//

#define OFFSET_COLLISION_TERRAIN 0.5f

BOOL CN3Terrain::Pick(int x, int y, __Vector3 & vPick) {
    // Compute the vector of the pick ray in screen space
    __Vector3 vTmp;
    vTmp.x = (((2.0f * x) / (CN3Base::s_CameraData.vp.Width)) - 1) / CN3Base::s_CameraData.mtxProjection._11;
    vTmp.y = -(((2.0f * y) / (CN3Base::s_CameraData.vp.Height)) - 1) / CN3Base::s_CameraData.mtxProjection._22;
    vTmp.z = 1.0f;

    // Transform the screen space pick ray into 3D space
    __Matrix44 * pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
    __Vector3    vDir;
    vDir.x = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
    vDir.y = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
    vDir.z = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
    __Vector3 vPos = pMtxVI->Pos();
    __Vector3 vPosCur = vPos;

    bool      bCollision = FALSE;
    __Vector3 A, B, C;
    float     t, u, v;

    int ix = ((int)vPosCur.x) / TILE_SIZE;
    int iz = ((int)vPosCur.z) / TILE_SIZE;

    if ((ix + iz) % 2 == 1) // ���.. �޼� ���ε�...
    {
        A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE), (float)(iz + 1) * TILE_SIZE);
        A.y += OFFSET_COLLISION_TERRAIN;
        C.y += OFFSET_COLLISION_TERRAIN;
        B.y += OFFSET_COLLISION_TERRAIN;
    }
    if ((ix + iz) % 2 == 0) {
        A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE), (float)(iz + 1) * TILE_SIZE);
        C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
              (float)(iz + 1) * TILE_SIZE);
        A.y += OFFSET_COLLISION_TERRAIN;
        C.y += OFFSET_COLLISION_TERRAIN;
        B.y += OFFSET_COLLISION_TERRAIN;
    }
    bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);

    if (FALSE == bCollision) // �浹���� ���� ���....
    {
        vPick.Set(0, 0, 0); // �ϴ� �浹 ���� ����..

        // ��....        !!����!!  ���ؽ� ���ۿ� �ε��� ���� �����..
        __Vector3 AA[8];      // ���� ���ؽ� ����..
        int       pIndex[36]; // ���� �ε��� ����..
        int *     pIdx = pIndex;

        AA[0] = __Vector3(vPos.x - COLLISION_BOX, vPos.y - COLLISION_BOX, vPos.z + COLLISION_BOX);
        AA[1] = __Vector3(vPos.x + COLLISION_BOX, vPos.y - COLLISION_BOX, vPos.z + COLLISION_BOX);
        AA[2] = __Vector3(vPos.x + COLLISION_BOX, vPos.y - COLLISION_BOX, vPos.z - COLLISION_BOX);
        AA[3] = __Vector3(vPos.x - COLLISION_BOX, vPos.y - COLLISION_BOX, vPos.z - COLLISION_BOX);
        AA[4] = __Vector3(vPos.x - COLLISION_BOX, vPos.y + COLLISION_BOX, vPos.z + COLLISION_BOX);
        AA[5] = __Vector3(vPos.x + COLLISION_BOX, vPos.y + COLLISION_BOX, vPos.z + COLLISION_BOX);
        AA[6] = __Vector3(vPos.x + COLLISION_BOX, vPos.y + COLLISION_BOX, vPos.z - COLLISION_BOX);
        AA[7] = __Vector3(vPos.x - COLLISION_BOX, vPos.y + COLLISION_BOX, vPos.z - COLLISION_BOX);

        // ����.
        *pIdx++ = 0;
        *pIdx++ = 1;
        *pIdx++ = 3;
        *pIdx++ = 2;
        *pIdx++ = 3;
        *pIdx++ = 1;

        // �ո�..
        *pIdx++ = 7;
        *pIdx++ = 3;
        *pIdx++ = 6;
        *pIdx++ = 2;
        *pIdx++ = 6;
        *pIdx++ = 3;

        // ����..
        *pIdx++ = 4;
        *pIdx++ = 0;
        *pIdx++ = 7;
        *pIdx++ = 3;
        *pIdx++ = 7;
        *pIdx++ = 0;

        // ������..
        *pIdx++ = 6;
        *pIdx++ = 2;
        *pIdx++ = 5;
        *pIdx++ = 1;
        *pIdx++ = 5;
        *pIdx++ = 2;

        // �޸�..
        *pIdx++ = 5;
        *pIdx++ = 1;
        *pIdx++ = 4;
        *pIdx++ = 0;
        *pIdx++ = 4;
        *pIdx++ = 1;

        // �ظ�..
        *pIdx++ = 7;
        *pIdx++ = 6;
        *pIdx++ = 4;
        *pIdx++ = 5;
        *pIdx++ = 4;
        *pIdx++ = 6;

        for (int i = 0; FALSE == bCollision && i < 36; i += 3) {
            float t, u, v;
            bCollision =
                ::_IntersectTriangle(vPos, vDir, AA[pIndex[i]], AA[pIndex[i + 1]], AA[pIndex[i + 2]], t, u, v, &vPick);
        }
    }

    return bCollision;
}

BOOL CN3Terrain::PickWide(int x, int y, __Vector3 & vPick) {
    // Compute the vector of the pick ray in screen space
    __Vector3 vTmp;
    vTmp.x = (((2.0f * x) / (CN3Base::s_CameraData.vp.Width)) - 1) / CN3Base::s_CameraData.mtxProjection._11;
    vTmp.y = -(((2.0f * y) / (CN3Base::s_CameraData.vp.Height)) - 1) / CN3Base::s_CameraData.mtxProjection._22;
    vTmp.z = 1.0f;

    // Transform the screen space pick ray into 3D space
    __Matrix44 * pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
    __Vector3    vDir;
    vDir.x = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
    vDir.y = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
    vDir.z = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
    __Vector3 vPos = pMtxVI->Pos();
    __Vector3 vPosCur = vPos;

    vDir.Normalize();

    bool      bCollision = FALSE;
    __Vector3 A, B, C;
    float     t, u, v;

    while ((vPosCur.x >= 0.0f) && (vPosCur.z >= 0.0f) && (IsInTerrain(vPosCur.x, vPosCur.z))) {
        if (bCollision) {
            return bCollision;
        }

        int ix = ((int)vPosCur.x) / TILE_SIZE;
        int iz = ((int)vPosCur.z) / TILE_SIZE;

        for (int i = 0; i < 10; i++) {
            switch (i) {
            case 0: //  0, 0
                break;
            case 1:
                ix--; // -1, 0
                break;
            case 2: //  0, -1
                ix++;
                iz--;
                break;
            case 3: // -1, -1
                ix--;
                break;
            case 4: //  1, -1
                ix++;
                ix++;
                break;
            case 5: // -1, 1
                ix--;
                ix--;
                iz++;
                iz++;
                break;
            case 6: //  0, 1
                ix++;
                break;
            case 7: //  1, 1
                ix++;
                break;
            case 8: //  1, 0
                iz--;
                break;
            case 9: //  0, 0
                ix--;
                break;
            };

            if ((ix + iz) % 2 == 1) // ���.. �޼� ���ε�...
            {
                A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    break;
                }

                A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);
                B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    break;
                }
            }
            if ((ix + iz) % 2 == 0) {
                A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);
                C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    break;
                }

                A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    break;
                }
            }
        }
        vPosCur += (vDir * TILE_SIZE);
        //vDir ũ�Ⱑ �۱� ������ Nomalize�ϰ� TILE_SIZE��ŭ ���ؼ� ���� üũ�� ��ġ�� �ٲ��ش�.
        //�̷��� ���� ������ üũ�� �κ��� ������ üũ�ϱ� ������ ���ϰ� Ŀ����.
    }

    if (FALSE == bCollision) // �浹���� ���� ���....
    {
        vPick.Set(0, 0, 0); // �ϴ� �浹 ���� ����..

        // ��....        !!����!!  ���ؽ� ���ۿ� �ε��� ���� �����..
        __Vector3 AA[8];      // ���� ���ؽ� ����..
        int       pIndex[36]; // ���� �ε��� ����..
        int *     pIdx = pIndex;

#define COL_BOX_OFF 2000

        AA[0] = __Vector3(vPos.x - COL_BOX_OFF, vPos.y - COL_BOX_OFF, vPos.z + COL_BOX_OFF);
        AA[1] = __Vector3(vPos.x + COL_BOX_OFF, vPos.y - COL_BOX_OFF, vPos.z + COL_BOX_OFF);
        AA[2] = __Vector3(vPos.x + COL_BOX_OFF, vPos.y - COL_BOX_OFF, vPos.z - COL_BOX_OFF);
        AA[3] = __Vector3(vPos.x - COL_BOX_OFF, vPos.y - COL_BOX_OFF, vPos.z - COL_BOX_OFF);
        AA[4] = __Vector3(vPos.x - COL_BOX_OFF, vPos.y + COL_BOX_OFF, vPos.z + COL_BOX_OFF);
        AA[5] = __Vector3(vPos.x + COL_BOX_OFF, vPos.y + COL_BOX_OFF, vPos.z + COL_BOX_OFF);
        AA[6] = __Vector3(vPos.x + COL_BOX_OFF, vPos.y + COL_BOX_OFF, vPos.z - COL_BOX_OFF);
        AA[7] = __Vector3(vPos.x - COL_BOX_OFF, vPos.y + COL_BOX_OFF, vPos.z - COL_BOX_OFF);

        // ����.
        *pIdx++ = 0;
        *pIdx++ = 1;
        *pIdx++ = 3;
        *pIdx++ = 2;
        *pIdx++ = 3;
        *pIdx++ = 1;

        // �ո�..
        *pIdx++ = 7;
        *pIdx++ = 3;
        *pIdx++ = 6;
        *pIdx++ = 2;
        *pIdx++ = 6;
        *pIdx++ = 3;

        // ����..
        *pIdx++ = 4;
        *pIdx++ = 0;
        *pIdx++ = 7;
        *pIdx++ = 3;
        *pIdx++ = 7;
        *pIdx++ = 0;

        // ������..
        *pIdx++ = 6;
        *pIdx++ = 2;
        *pIdx++ = 5;
        *pIdx++ = 1;
        *pIdx++ = 5;
        *pIdx++ = 2;

        // �޸�..
        *pIdx++ = 5;
        *pIdx++ = 1;
        *pIdx++ = 4;
        *pIdx++ = 0;
        *pIdx++ = 4;
        *pIdx++ = 1;

        // �ظ�..
        *pIdx++ = 7;
        *pIdx++ = 6;
        *pIdx++ = 4;
        *pIdx++ = 5;
        *pIdx++ = 4;
        *pIdx++ = 6;

        for (int i = 0; FALSE == bCollision && i < 36; i += 3) {
            float t, u, v;
            bCollision =
                ::_IntersectTriangle(vPos, vDir, AA[pIndex[i]], AA[pIndex[i + 1]], AA[pIndex[i + 2]], t, u, v, &vPick);
        }
    }

    return bCollision;
}

#define __MAX_DISTANCE 6000

//
//
//
void CN3Terrain::CalcCollisionTerrainByOTPlayer(__Vector3 vOrig, __Vector3 vAt, __Vector3 & Vec) {
    bool      boo = FALSE;
    __Vector3 vec2, vec3, vec4, vDir; // vec1 & vec2 is 2D..  vec3 & vec4 is 3D..
    __Vector3 A, B, C, AB, AC;
    float     ftx, fty, ftz;
    vec2 = vOrig;
    vec3 = vAt;
    vOrig = vec2;
    vec3.Normalize();
    vec3 *= TILE_SIZE;
    vDir = vec3;
    vec4 = vec2;

    int ix = ((int)vec2.x) / TILE_SIZE;
    int iz = ((int)vec2.z) / TILE_SIZE;

    if ((ix + iz) % 2 == 1) // ���.. �޼� ���ε�...
    {
        A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE), (float)(iz + 1) * TILE_SIZE);

        boo = ::_IntersectTriangle(vOrig, vDir, A, B, C, ftx, fty, ftz);
        if (boo == TRUE) {
            Vec = vOrig + ftx * vDir;
            return;
        }

        A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
              (float)(iz + 1) * TILE_SIZE);
        B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE), (float)(iz + 1) * TILE_SIZE);

        boo = ::_IntersectTriangle(vOrig, vDir, A, B, C, ftx, fty, ftz);
        if (boo == TRUE) {
            Vec = vOrig + ftx * vDir;
            return;
        }
    }
    if ((ix + iz) % 2 == 0) {
        A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE), (float)(iz + 1) * TILE_SIZE);
        C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
              (float)(iz + 1) * TILE_SIZE);

        boo = ::_IntersectTriangle(vOrig, vDir, A, B, C, ftx, fty, ftz);
        if (boo == TRUE) {
            Vec = vOrig + ftx * vDir;
            return;
        }

        A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
        C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
              (float)(iz + 1) * TILE_SIZE);

        boo = ::_IntersectTriangle(vOrig, vDir, A, B, C, ftx, fty, ftz);
        if (boo == TRUE) {
            Vec = vOrig + ftx * vDir;
            return;
        }
    }

    //#define COLLISION_BOX 6000

    // ��....        !!����!!  ���ؽ� ���ۿ� �ε��� ���� �����..
    __Vector3 AA[8];      // ���� ���ؽ� ����..
    int       pIndex[36]; // ���� �ε��� ����..
    int *     pIdx = pIndex;

    AA[0] = __Vector3(vec4.x - COLLISION_BOX, vec4.y - COLLISION_BOX, vec4.z + COLLISION_BOX);
    AA[1] = __Vector3(vec4.x + COLLISION_BOX, vec4.y - COLLISION_BOX, vec4.z + COLLISION_BOX);
    AA[2] = __Vector3(vec4.x + COLLISION_BOX, vec4.y - COLLISION_BOX, vec4.z - COLLISION_BOX);
    AA[3] = __Vector3(vec4.x - COLLISION_BOX, vec4.y - COLLISION_BOX, vec4.z - COLLISION_BOX);
    AA[4] = __Vector3(vec4.x - COLLISION_BOX, vec4.y + COLLISION_BOX, vec4.z + COLLISION_BOX);
    AA[5] = __Vector3(vec4.x + COLLISION_BOX, vec4.y + COLLISION_BOX, vec4.z + COLLISION_BOX);
    AA[6] = __Vector3(vec4.x + COLLISION_BOX, vec4.y + COLLISION_BOX, vec4.z - COLLISION_BOX);
    AA[7] = __Vector3(vec4.x - COLLISION_BOX, vec4.y + COLLISION_BOX, vec4.z - COLLISION_BOX);

    // ����.
    *pIdx++ = 0;
    *pIdx++ = 1;
    *pIdx++ = 3;
    *pIdx++ = 2;
    *pIdx++ = 3;
    *pIdx++ = 1;

    // �ո�..
    *pIdx++ = 7;
    *pIdx++ = 3;
    *pIdx++ = 6;
    *pIdx++ = 2;
    *pIdx++ = 6;
    *pIdx++ = 3;

    // ����..
    *pIdx++ = 4;
    *pIdx++ = 0;
    *pIdx++ = 7;
    *pIdx++ = 3;
    *pIdx++ = 7;
    *pIdx++ = 0;

    // ������..
    *pIdx++ = 6;
    *pIdx++ = 2;
    *pIdx++ = 5;
    *pIdx++ = 1;
    *pIdx++ = 5;
    *pIdx++ = 2;

    // �޸�..
    *pIdx++ = 5;
    *pIdx++ = 1;
    *pIdx++ = 4;
    *pIdx++ = 0;
    *pIdx++ = 4;
    *pIdx++ = 1;

    // �ظ�..
    *pIdx++ = 7;
    *pIdx++ = 6;
    *pIdx++ = 4;
    *pIdx++ = 5;
    *pIdx++ = 4;
    *pIdx++ = 6;

    for (int i = 0; i < 36; i += 3) {
        boo = ::_IntersectTriangle(vOrig, vDir, AA[pIndex[i]], AA[pIndex[i + 1]], AA[pIndex[i + 2]], ftx, fty, ftz);
        if (boo == TRUE) {
            break;
        }
    }
    Vec = vOrig + ftx * vDir;
}

bool CN3Terrain::CheckIncline(const __Vector3 & vPos, const __Vector3 & vDir, float fIncline) {
    __Vector3 vNormal;
    this->GetNormal(vPos.x, vPos.z, vNormal);
    vNormal.Normalize();
    vNormal.y = 0.0f;
    if (vNormal.Magnitude() > fIncline && vNormal.Dot(vDir) <= 0.0f) {
        return true;
    }
    return false;
}

bool CN3Terrain::CheckCollisionCamera(__Vector3 & vEyeResult, const __Vector3 & vAt, float fNP) {
    float fHeight = this->GetHeight(vEyeResult.x, vEyeResult.z);
    float fDelta = fHeight - vEyeResult.y + fNP;
    if (fDelta < 0) {
        return false;
    }

    __Vector3 vDir = vAt - vEyeResult;
    vDir.Normalize();
    vDir.Set(0, 1, 0);

    vEyeResult += vDir * fDelta;
    return true;
}

bool CN3Terrain::CheckCollision(__Vector3 & vPos, __Vector3 & vDir, float fVelocity, __Vector3 * vCol) {
    float fHeight1, fHeight2;
    vDir.Normalize();

    fHeight1 = vPos.y - this->GetHeight(vPos.x, vPos.z);
    __Vector3 vNextPos = vPos + (fVelocity * CN3Base::s_fSecPerFrm * vDir);
    fHeight2 = vNextPos.y - this->GetHeight(vNextPos.x, vNextPos.z);
    if (fHeight1 <= 0) {
        (*vCol) = vPos;
        (*vCol).y = this->GetHeight(vPos.x, vPos.z) + 0.1f;
        return true;
    }
    if (fHeight1 * fHeight2 > 0) {
        return false;
    }

    /////////////////////////////////////////////
    //�� �� �����ϰ� �Ϸ���..����...
    //
    (*vCol) = vPos;
    (*vCol).y = this->GetHeight(vPos.x, vPos.z) + 0.1f;
    return true;
    //
    /////////////////////////////////////////////

    //�����ϰ� �Ϸ��� ������ ������...
    //�浹�ߴ�...
    RECT rt;
    rt.left = Real2Tile(vPos.x);
    rt.bottom = Real2Tile(vPos.z);
    rt.right = Real2Tile(vNextPos.x);
    rt.top = Real2Tile(vNextPos.z);

    if (rt.left > rt.right) {
        int tmp = rt.left;
        rt.left = rt.right;
        rt.right = rt.left;
    }
    if (rt.bottom > rt.top) {
        int tmp = rt.top;
        rt.top = rt.bottom;
        rt.bottom = rt.top;
    }

    __Vector3 A, B, C;
    BOOL      bCollision;
    __Vector3 vPick;
    float     t, u, v;
    for (int ix = rt.left; ix <= rt.right; ix++) {
        for (int iz = rt.bottom; iz <= rt.top; iz++) {
            if ((ix + iz) % 2 == 1) // ���.. �޼� ���ε�...
            {
                A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    (*vCol) = vPick;
                    return true;
                }

                A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);
                B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    (*vCol) = vPick;
                    return true;
                }
            }
            if ((ix + iz) % 2 == 0) {
                A.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);
                C.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                B.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    (*vCol) = vPick;
                    return true;
                }

                A.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, iz * TILE_SIZE),
                      (float)iz * TILE_SIZE);
                B.Set((float)ix * TILE_SIZE, GetHeight(ix * TILE_SIZE, iz * TILE_SIZE), (float)iz * TILE_SIZE);
                C.Set((float)(ix + 1) * TILE_SIZE, GetHeight((ix + 1) * TILE_SIZE, (iz + 1) * TILE_SIZE),
                      (float)(iz + 1) * TILE_SIZE);

                bCollision = ::_IntersectTriangle(vPos, vDir, A, B, C, t, u, v, &vPick);
                if (bCollision == TRUE) {
                    (*vCol) = vPick;
                    return true;
                }
            }
        }
    }

    (*vCol) = vPos;
    (*vCol).y = this->GetHeight(vPos.x, vPos.z);
    return true;
}

bool CN3Terrain::LoadColorMap(const std::string & szFN) {
    CUILoading * pUILoading = CGameProcedure::s_pUILoading; // �ε���..

    m_iNumColorMap = (m_pat_MapSize * PATCH_PIXEL_SIZE) / COLORMAPTEX_SIZE;
    m_ppColorMapTex = new CN3Texture *[m_iNumColorMap];
    for (int x = 0; x < m_iNumColorMap; x++) {
        m_ppColorMapTex[x] = new CN3Texture[m_iNumColorMap];
    }

    HANDLE hColorMapFile = CreateFile(szFN.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hColorMapFile) {
        CLogWriter::Write("Failed to load ColorMap - %s", szFN.c_str());
        return false;
    }

    char szBuff[128];
    for (int x = 0; x < m_iNumColorMap; x++) {
        for (int z = 0; z < m_iNumColorMap; z++) {
            m_ppColorMapTex[x][z].m_iLOD = s_Options.iTexLOD_Terrain; // LOD ������ �д´�..
            m_ppColorMapTex[x][z].Load(hColorMapFile);
        }

        sprintf(szBuff, "Loading colormap %d %%", x * 100 / m_iNumColorMap);
        if (pUILoading) {
            pUILoading->Render(szBuff, 60 + 15 * x / m_iNumColorMap);
        }
    }

    CloseHandle(hColorMapFile);
    return true;
}

CN3Texture * CN3Terrain::GetTileTex(int x, int z) {
    if (x < 0 || x >= m_ti_MapSize || z < 0 || z >= m_ti_MapSize) {
        return NULL;
    }

    MAPDATA MapData;
    MapData = m_pMapData[(x * m_ti_MapSize) + z];
    return &(m_pTileTex[MapData.Tex1Idx]);
}

bool CN3Terrain::GetTileTexInfo(float x, float z, TERRAINTILETEXINFO & TexInfo1, TERRAINTILETEXINFO & TexInfo2) {
    int tx = x / TILE_SIZE;
    int tz = z / TILE_SIZE;

    if (tx < 0 || tx >= m_ti_MapSize || tz < 0 || tz >= m_ti_MapSize) {
        return false;
    }

    MAPDATA MapData = m_pMapData[(tx * m_ti_MapSize) + tz];

    if (MapData.Tex1Idx < 0 || MapData.Tex1Idx >= m_NumTileTex) {
        TexInfo1.pTex = NULL;
        TexInfo1.u = TexInfo1.v = 0.0f;
    } else {
        TexInfo1.pTex = &(m_pTileTex[MapData.Tex1Idx]);
        TexInfo1.u = TexInfo1.v = 0.0f;
        //u1[0] = m_pRefTerrain->m_fTileDirU[dir1][2];
        //u1[1] = m_pRefTerrain->m_fTileDirU[dir1][0];
        //u1[2] = m_pRefTerrain->m_fTileDirU[dir1][1];
        //u1[3] = m_pRefTerrain->m_fTileDirU[dir1][3];

        //v1[0] = m_pRefTerrain->m_fTileDirV[dir1][2];
        //v1[1] = m_pRefTerrain->m_fTileDirV[dir1][0];
        //v1[2] = m_pRefTerrain->m_fTileDirV[dir1][1];
        //v1[3] = m_pRefTerrain->m_fTileDirV[dir1][3];

        //u2[0] = m_pRefTerrain->m_fTileDirU[dir2][2];
        //u2[1] = m_pRefTerrain->m_fTileDirU[dir2][0];
        //u2[2] = m_pRefTerrain->m_fTileDirU[dir2][1];
        //u2[3] = m_pRefTerrain->m_fTileDirU[dir2][3];

        //v2[0] = m_pRefTerrain->m_fTileDirV[dir2][2];
        //v2[1] = m_pRefTerrain->m_fTileDirV[dir2][0];
        //v2[2] = m_pRefTerrain->m_fTileDirV[dir2][1];
        //v2[3] = m_pRefTerrain->m_fTileDirV[dir2][3];
    }

    if (MapData.Tex2Idx < 0 || MapData.Tex2Idx >= m_NumTileTex) {
        TexInfo2.pTex = NULL;
        TexInfo2.u = TexInfo2.v = 0.0f;
    } else {
        TexInfo2.pTex = &(m_pTileTex[MapData.Tex2Idx]);
        TexInfo2.u = TexInfo2.v = 0.0f;
        //u1[0] = m_pRefTerrain->m_fTileDirU[dir1][2];
        //u1[1] = m_pRefTerrain->m_fTileDirU[dir1][0];
        //u1[2] = m_pRefTerrain->m_fTileDirU[dir1][1];
        //u1[3] = m_pRefTerrain->m_fTileDirU[dir1][3];

        //v1[0] = m_pRefTerrain->m_fTileDirV[dir1][2];
        //v1[1] = m_pRefTerrain->m_fTileDirV[dir1][0];
        //v1[2] = m_pRefTerrain->m_fTileDirV[dir1][1];
        //v1[3] = m_pRefTerrain->m_fTileDirV[dir1][3];

        //u2[0] = m_pRefTerrain->m_fTileDirU[dir2][2];
        //u2[1] = m_pRefTerrain->m_fTileDirU[dir2][0];
        //u2[2] = m_pRefTerrain->m_fTileDirU[dir2][1];
        //u2[3] = m_pRefTerrain->m_fTileDirU[dir2][3];

        //v2[0] = m_pRefTerrain->m_fTileDirV[dir2][2];
        //v2[1] = m_pRefTerrain->m_fTileDirV[dir2][0];
        //v2[2] = m_pRefTerrain->m_fTileDirV[dir2][1];
        //v2[3] = m_pRefTerrain->m_fTileDirV[dir2][3];
    }

    return true;
}
