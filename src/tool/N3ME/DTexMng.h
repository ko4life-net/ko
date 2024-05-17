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
    //int                    m_NumDTex;    //last array containing dtex + 1...^^
    //CDTex*            m_pDTex[MAX_TILETEXTURE];

  public:
    bool    IsInDTex(CString FileName);
    int     GetMaxID() { return m_NextID - 1; }
    CDTex * GetDTexByID(int id);
    CDTex * GetDTexByName(CString FileName);
    int     GetNumDTex() { return m_pDTex.size(); }
    bool    AddDTex(CString FileName);
    void    DelDTexByID(int id);
    void    Init(CMainFrame * pFrm = NULL);
    void    Release();

    void SaveToFile(CString RealFileName);
    void LoadFromFile(CString RealFileName);
    void SaveGameTile();

    CDTexMng();
    virtual ~CDTexMng();
};
