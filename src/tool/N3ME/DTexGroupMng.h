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
    std::list<CDTexGroup *> m_Groups;     //List containing group data.
    CDlgDTexGroupView *     m_pGroupView; //A window that shows groups during actual work.

    DTEXATTR     m_SelectedDTex;     //Group and properties selected in the group window when working.
    DTEXTILEATTR m_SelectedDTexTile; //The actual texture tile selected during operation.

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
    bool         SaveToFile(CString RealFileName);
    bool         LoadFromFile(CString RealFileName);
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
