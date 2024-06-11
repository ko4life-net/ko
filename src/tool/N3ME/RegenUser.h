// RegenUser.h: interface for the CRegenUser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include <list>

class CDlgRegenUser;
class CMapMng;

typedef struct ___VertexRect {
    __Vector3 m_vLB;
    __Vector3 m_vLT;
    __Vector3 m_vRT;
    __Vector3 m_vRB;
} VERTEXRECT;

class CRegenUser : public CN3Base {
  public:
    CDlgRegenUser * m_pDlg;

    CMapMng * m_pRefMapMng; // Terrain reference pointer..
    bool      m_bActive;

    std::list<VERTEXRECT *> m_vrListRegion;
    VERTEXRECT              m_vrCurrRegion;
    VERTEXRECT *            m_vrSelRegion;

    __Vector3 m_vCurrStart;
    __Vector3 m_vCurrEnd;

  public:
    void DeleteSel();
    void Render();
    void SetActive(bool active);
    BOOL MouseMsgFilter(LPMSG pMsg);
    void SetCurrRECT(__Vector3 vS, __Vector3 vE);

    void ClearList();
    void SaveServerData(HANDLE hFile);
    void LoadFromFile(LPCTSTR pFullFileName);
    void SaveToFile(LPCTSTR pFullFileName);

    CRegenUser();
    virtual ~CRegenUser();
};
