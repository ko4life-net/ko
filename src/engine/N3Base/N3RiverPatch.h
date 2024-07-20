// N3RiverPatch.h: interface for the CN3RiverPatch class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base.h"

struct __VertexRiver : public __VertexXyzT2 {
    WORD index; // 점의 Index
};

struct __River // 강의 점정보(정렬을 위한..)
{
    short           iRiverID;
    int             iVertexCount;
    __VertexRiver * pVertices;

    __River() { pVertices = NULL; }
    ~__River() {
        if (pVertices) {
            delete[] pVertices;
            pVertices = NULL;
        }
    }
};

struct __RiverEx // 강의 점정보(vertex buffer를 위한..)
{
    short           iRiverID;
    int             iVertexCount;
    __VertexXyzT2 * pVertices;

    int    iIndexCount;
    WORD * pIndices;

    __RiverEx() {
        pVertices = NULL;
        pIndices = NULL;
    }
    ~__RiverEx() {
        if (pVertices) {
            delete[] pVertices;
            pVertices = NULL;
        }
        if (pIndices) {
            delete[] pIndices;
            pIndices = NULL;
        }
    }
};

class CN3RiverPatch : public CN3Base // 각 patch에 속한 강들의 정보.
{
  public:
    CN3RiverPatch();
    virtual ~CN3RiverPatch();

    // Attributes
  public:
    int       GetRiverCount() const { return m_iRiverCount; }
    __River * GetRiver(int iIndex) {
        if (iIndex >= 0 && iIndex < m_iRiverCount) {
            return (m_pRivers + iIndex);
        }
        return NULL;
    }

  protected:
    int       m_iRiverCount;
    __River * m_pRivers;

    // Operations
  public:
    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual bool Save(HANDLE hFile);

    __River * CreateRiver(int iRiverCount);
    __River * GetRiverByID(int iRiverID);

  protected:
};
