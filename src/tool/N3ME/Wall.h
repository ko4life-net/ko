// Wall.h: interface for the CWall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include <list>

class CWall : public CN3Base {
  public:
    char                 m_Name[80];
    std::list<__Vector3> m_Wall;

  public:
    void Load(HANDLE hFile);
    void Save(HANDLE hFile);
    int  GetSize() { return m_Wall.size(); }
    bool GetVertex(int idx, __Vector3 * pPos); // Get the idxth point...
    void AddVertex(__Vector3 Vertex);          // Add path.
    void DelPrevVertex();

    CWall();
    virtual ~CWall();
};
