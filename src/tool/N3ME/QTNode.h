// QTNode.h: interface for the CQTNode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum eQTNODESTATE {
    QTNODESTATE_CLIPPED = 0,
    QTNODESTATE_NOTRENDER,
    QTNODESTATE_RENDER,
    QTNODESTATE_UNKNOWN
}; // Status properties of quadtree nodes..
enum eDIR {
    DIR_LEFT = 0,
    DIR_TOP,
    DIR_RIGHT,
    DIR_BOTTOM,
    DIR_NUM
};

class CLyTerrain;

class CQTNode {
  protected:
    int m_CenterX; //Coordinates of the center point in the height field array.
    int m_CenterZ; //Coordinates of the center point in the height field array.

    int          m_Level; //Depth value on the quadtree...
    eQTNODESTATE m_State; //

    CQTNode * m_pFriend[DIR_NUM];
    CQTNode * m_pChild[DIR_NUM];

    CLyTerrain * m_pRefTerrain;

  public:
    float m_MinY; // lowest point
    float m_MaxY; // lowest point
    int   GetCenterX() { return m_CenterX; }
    int   GetCenterZ() { return m_CenterZ; }

  private:
    bool       IsRender(POINT * pEye);
    inline int Distance(int sx, int sz, int tx, int tz);
    void       RenderMaxLevel();
    void       RenderNormalLevel();

  public:
    int  GetMaxLevel(eDIR dir);
    void Render();
    void Tick();
    void ClearMinMaxY();
    void SetWholeClipped();
    void SetMinMaxY();
    void SetCenterPoint(int x, int z);
    void Init(int level = 0, CLyTerrain * pTerrain = NULL);
    void Release();
    void LinkFriend();

    CQTNode();
    virtual ~CQTNode();
};
