// N3MeshIndexed.h: interface for the CN3IMesh class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"

const int MAX_IMESH_BUFFER = 10000;

class CN3IMesh : public CN3BaseFileAccess {
  protected:
    static __VertexT1 s_Vertices[MAX_IMESH_BUFFER];
    static __VertexT2 s_Vertices2[MAX_IMESH_BUFFER];

    int m_nFC; // Number of triangles

    int    m_nVC;          // Vertex Count
    WORD * m_pwVtxIndices; // List of point indices.
    int    m_nUVC;         // UV coordinate Count
    WORD * m_pwUVsIndices; // Texture coordinate index list.

    __VertexXyzNormal * m_pVertices; // Coordinate Data
    float *             m_pfUVs;     // UV Data - Allocate as much as m_nUVC * 2.
                                     // LPDIRECT3DVERTEXBUFFER9 m_lpVB;

    __Vector3 m_vMin, m_vMax; // Minimum and maximum points... they may change, but roughly...

  public:
#ifdef _N3TOOL
    void ApplyOffset(__Vector3 vOffset);
    void ReGenerateSmoothNormal();
    void RenderSelected();
#endif // end of _N3TOOL
    __Vector3 Min() { return m_vMin; }
    __Vector3 Max() { return m_vMax; }
    void      FindMinMax();
    void      Render(bool bUseTwoUV = false);
    __VertexT1 *
    BuildVertexList(); // Create a point in the Vertex Buffer. Be careful not to use it twice at the same time.
    __VertexT2 *
    BuildVertexListTwoUV(); // Create a point in the Vertex Buffer. Be careful not to use it twice at the same time.

    int                 FaceCount() { return m_nFC; }
    int                 VertexCount() { return m_nVC; }
    __VertexXyzNormal * Vertices() { return m_pVertices; }
    WORD *              VertexInices() { return m_pwVtxIndices; }
    int                 UVCount() { return m_nUVC; }
#ifdef _N3TOOL
    void VertexIndexSet(int index, int nVI) {
        if (index >= 0 && index < m_nFC * 3) {
            m_pwVtxIndices[index] = nVI;
        }
    }
    void UVSet(int index, float fU, float fV) {
        if (index >= 0 && index < m_nUVC) {
            m_pfUVs[index * 2 + 0] = fU;
            m_pfUVs[index * 2 + 1] = fV;
        }
    }
    void UVIndexSet(int index, int nUVI) {
        if (index >= 0 && index < m_nFC * 3) {
            m_pwUVsIndices[index] = nUVI;
        }
    }
#endif // end of _N3TOOL

    virtual bool Create(int nFC, int nVC, int nUVC); // In order, Face Count, VertexCount, UV Count
    bool         Load(HANDLE hFile);
#ifdef _N3TOOL
    bool Save(HANDLE hFile);
#endif // end of _N3TOOL

    void Release();
    CN3IMesh();
    virtual ~CN3IMesh();

//   By: Ecli666 (On 2002-07-23 5:31:41 PM)
//
#ifdef _N3GAME
    void        TickForShadow(bool bUseTwoUV = false);
    int         GetVertexCount();
    __Vector3 * GetVertexData();

  private:
    __Vector3 * m_pVertexT1;
#endif

    //    ~(By Ecli666 On 2002-07-23 5:31:41 PM)
};
