// DTexMng.h: interface for the CDTexMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DTexGroup.h"
#include <list>

class CDTex;
class CMainFrame;

typedef typename std::list<CDTex *>::iterator it_DTex;

class CDTexMng : public CN3Base {
  protected:
    int m_NextID;

  public:
    std::list<CDTex *> m_pDTex;
    CMainFrame *       m_pMainFrm;
    //int                    m_NumDTex;    //dtex를 담고 있는 마지막 배열 + 1...^^
    //CDTex*            m_pDTex[MAX_TILETEXTURE];

  public:
    int     GetMaxID() { return m_NextID - 1; }
    CDTex * GetDTexByID(int id);
    CDTex * GetDTexByName(const fs::path & fsFileName);
    bool    IsInDTex(const fs::path & fsFileName);
    int     GetNumDTex() { return m_pDTex.size(); }
    bool    AddDTex(const fs::path & fsFileName);
    void    DelDTexByID(int id);
    void    Init(CMainFrame * pFrm = NULL);
    void    Release();

    void SaveToFile(const fs::path & fsFileName);
    void LoadFromFile(const fs::path & fsFileName);
    void SaveGameTile();

    CDTexMng();
    virtual ~CDTexMng();
};
