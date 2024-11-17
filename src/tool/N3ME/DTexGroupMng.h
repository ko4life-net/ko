// DTexGroupMng.h: interface for the CDTexGroupMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include "LyTerrainDef.h"
#include <list>

class CDTexGroup;
class CDlgDTexGroupView;
class CMainFrame;

typedef typename std::list<CDTexGroup *>::iterator it_DTexGroup;

class CDTexGroupMng : public CN3Base {
  public:
    std::list<CDTexGroup *> m_Groups;     //그룹 테이터들을 가지고 있는 리스트.
    CDlgDTexGroupView *     m_pGroupView; //실제 작업할때 그룹들을 보여주는 창.

    DTEXATTR     m_SelectedDTex;     //작업시 그룹창에서 선택한 그룹과 속성.
    DTEXTILEATTR m_SelectedDTexTile; //작업시 선택한 실제 텍스쳐 타일.

    CMainFrame * m_pMainFrm;

  private:
    static int CompareIdx(const void * arg1, const void * arg2);

  public:
    void         DelTileByDTexID(int DTexID);
    int          GetBrushSize();
    DTEXTILEATTR GetTileAttr(int groupID, int attr);
    DTEXTILEATTR GetTileAttrManuel(int groupID, int attr, int AttrIdx);
    int          GetIndex2ID(int idx);
    int          GetID2Index(int id);
    char *       GetGroupName(int id);
    bool         SaveToFile(const fs::path & fsFileName);
    bool         LoadFromFile(const fs::path & fsFileName);
    void         Init(CWnd * pWndParent);
    void         Release();
    void         SetGroup(const char * pName);
    void         SetGroupID(const char * pName, int id);
    void         DelGroup(int ID);
    void         SetTile(int ID, int attr, DTEXTILEATTR tile);
    void         DelTile(int ID, int attr, DTEXTILEATTR tile);
    CDTexGroupMng();
    virtual ~CDTexGroupMng();
};
