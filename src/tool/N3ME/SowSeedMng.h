// SowSeedMng.h: interface for the CSowSeedMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>

typedef struct __GRASS {
    int       Tile_x;
    int       Tile_z;
    __Vector3 Pos;
    float     Detx;
    float     Detz;
    float     roll;
    int       Sub_Grass_Count;
    __GRASS * Sub_Grass;

    __GRASS() {
        Tile_x = -1;
        Tile_z = -1;
        Pos = __Vector3(0.0f, 0.0f, 0.0f);
        Sub_Grass = NULL;
        Sub_Grass_Count = 0;
        roll = 0;
    };

} GRASS, *LPGRASS;

typedef struct __GRASS_GROUP {
    int       Group_id;
    int       Obj_ID;
    int       b_size;
    char      FileName[256];
    __Vector3 Pos;

    std::list<LPGRASS> grass;
    __GRASS_GROUP() {
        Pos = __Vector3(0.0f, 0.0f, 0.0f);
        memset(FileName, 0, 256);
        Group_id = -1;
        grass.clear();
    };

} GRASS_GROUP, *LPGRASS_GROUP;

typedef struct __OBJ_NAME {
    int  Id;
    char FileName[256];
} OBJ_NAME, *LPOBJ_NAME;

typedef typename std::list<LPGRASS>::iterator       it_Grass;
typedef typename std::list<LPGRASS_GROUP>::iterator it_Grass_Group;
typedef typename std::list<LPOBJ_NAME>::iterator    it_Obj_Name;

class CSowSeedMng {
  public:
    std::list<LPGRASS_GROUP> Grass_Group;
    std::list<LPOBJ_NAME>    Obj_Name;
    LPGRASS                  Grass;
    int                      Grass_Count;

    int Add_List_Count;

    __VertexColor * m_pVertices; // dot buffer
    int             m_nVC;       // number of points

    bool  bActive;
    BOOL  Render_Grass;
    POINT ptSelHeightMapPos;
    int   Select_Group_Id;

    CSowSeedMng();
    virtual ~CSowSeedMng();
    void SetActive(BOOL flage);

    void Render(LPDIRECT3DDEVICE9 lpD3DDevice);
    // Mouse message processing
    BOOL MouseMessage(LPMSG pMsg);
    void Release();
    void Create_Grass(int NUm);
    void Random_Grass(void);
    void Add_Grass(void);
    int  SelectSeed(POINT point);
    void SetListPos(void);
    void CreateBox(__Vector3 Max, __Vector3 Min);
    void Render_Box(LPDIRECT3DDEVICE9 lpD3DDevice, __Vector3 Pos);
    void SaveData(void);
    void LoadData(void);
    void SaveDataGame(void);

    void Test_GameDataLoad(void);
    void Test_GameDataSave(void);
};
