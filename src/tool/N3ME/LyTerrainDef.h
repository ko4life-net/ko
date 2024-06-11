////////////////////////////////////////////////////////////////////////////////////////
//
//    LyTerrainDef.h
//      - Data type definitions and constant definitions related to Terrain...
//      - Terrain using quadtree...^^
//
//    By Donghoon..
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// common include header...
#include "N3Base/N3Base.h"

// Constant Definitions..
const int N3ME_DATA_VERSION = 3;
//version info...
//Previous data does not have version information.
//1: Lightmap information is added to the initial data.
//2: Version 1 plus pool information (information about the relationship between the pool and the DTex group).
//3: Improved lightmap..^^

const int N3ME_DTEX_DATA_VERSION = 2;
//
//1: When reading dTex, the group information of each texture is read from each texture name.dif file.
//2: Read the group information of each texture from the dtexinfo.dtx file.

const int TERRAIN_CELL_SIZE = 4;
//const int MAX_COLORMAPTEXTURE = 32; // maximum #colormaptexture at one side = 4096/128..
//Currently being replaced with m_iNumColorMap...
const int PATCH_TILE_SIZE = 8; // One patch contains 8x8 tiles.

const int MAX_COLORMAPVB_SIZE = 1026; //Maximum size of VB taken as a color map.
const int MAX_TILEVB_SIZE = 16;       //Maximum size of tile VB.
const int MAX_LIGHTMAPVB_SIZE = 16;   //Maximum size of VB that uses lightmaps.

const int DTEX_SIZE = 1024; // Detail Texture Size
const int NUM_DTEXTILE = 8; // Detail Texture ºÐÇÒ ¼ö..

const int LIGHTMAP_TEX_SIZE = 8; //32;

const int DISTANCE_TABLE_SIZE = 64;

const int NOTMOVE_HEIGHT = 10; // Minimum value of tiles that cannot be reached due to a large height difference..
                               // If it is less than 10, you can move, if it is more than 10, you cannot move.

const int MAX_BRUSH_SIZE = 21;
const int BRUSH_CENTER = (MAX_BRUSH_SIZE / 2);
enum {
    BRUSHSHAPE_CIRCLE = 1,
    BRUSHSHAPE_SQUARE = 2
};
enum {
    TEM_NOT = 0,
    TEM_HEIGHTEN = 1,
    TEM_SMOOTH = 2,
    TEM_DTEX = 3,
    TEM_LIGHTMAP = 4,
    TEM_MAKENPCPATH = 5,
    TEM_FLAT = 6,
    TEM_BRUSH_SHOW = 7
};

enum {
    DTEX_FULL = 0,
    DTEX_1PER2,
    DTEX_1PER4,
    DTEX_3PER4,
    DTEX_1PER8,
    DTEX_7PER8,
    DTEX_5PER8,
    DTEX_3PER8,
    DTEX_MAX = 8
};

//macro..
#define DISTANCE3D(a, b)                                                                                               \
    (sqrt(((a).x - (b).x) * ((a).x - (b).x) + ((a).y - (b).y) * ((a).y - (b).y) + ((a).z - (b).z) * ((a).z - (b).z)))
#define DISTANCE2D(a, b) (sqrt(((a).x - (b).x) * ((a).x - (b).x) + ((a).z - (b).z) * ((a).z - (b).z)))

// Structure Definitions..

//Map editor..
typedef struct __DTexAttr {
    int Group;
    int Attr;

    __DTexAttr() {
        Group = 0;
        Attr = 0;
    }
} DTEXATTR, *LPDTEXATTR;

typedef struct __DTexTileAttr {
    int TexID;
    int TileX;
    int TileY;

    __DTexTileAttr() {
        TexID = -1;
        TileX = 0;
        TileY = 0;
    }
} DTEXTILEATTR, *LPDTEXTILEATTR;

typedef struct __DTexInfo {
    int          Dir;
    DTEXTILEATTR TexIdx;
    DTEXATTR     Attr;

    __DTexInfo() { Dir = 0; }
} DTEXINFO, *LPDTEXINFO;

typedef struct __MapData {
    float fHeight; //Terrain height value..
    //float    u1,v1;
    //__Vector3 Normal;
    DTEXINFO DTexInfo1;
    DTEXINFO DTexInfo2;

    __MapData() { fHeight = -FLT_MAX; }
} MAPDATA, *LPMAPDATA;

typedef struct __GameMapData {
    float fHeight;         //Terrain height value..
    DWORD bIsTileFull : 1; //true if the tile is full, false otherwise..
    DWORD Tex1Dir : 5;     //Not the index used by the tool..
    DWORD Tex2Dir : 5;
    DWORD Tex1Idx : 10; //Not the index used by the tool..
    DWORD Tex2Idx : 10;

    __GameMapData() {
        bIsTileFull = true;
        fHeight = -FLT_MAX;
        Tex1Idx = 0;
        Tex1Dir = 0;
        Tex2Idx = 0;
        Tex2Dir = 0;
    }

} GAMEMAPDATA, *LPGAMEMAPDATA;

typedef struct __SeedGroup {
    unsigned char Obj_Id : 4;
    unsigned char Seed_Count : 3;
    unsigned char sub_flage : 1;

    __SeedGroup * SeedGroup_Sub;

    __SeedGroup() {
        Obj_Id = 0;
        Seed_Count = 0;
        SeedGroup_Sub = NULL;
    }
} SEEDGROUP, *LPSEEDGROUP;
