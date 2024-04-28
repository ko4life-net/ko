// N3MeshNormal.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"

class CN3IMesh;
class CN3PMesh;

class CN3Mesh : public CN3BaseFileAccess {
  protected:
    __VertexT1 * m_pVertices;  // dot buffer
    WORD *       m_psnIndices; // index buffer

    int       m_nVC; // number of points
    int       m_nIC; // number of points
    __Vector3 m_vMin, m_vMax;

  public:
    __Vector3 Min() { return m_vMin; }
    __Vector3 Max() { return m_vMax; }
    void      FindMinMax();

    int          VertexCount() { return m_nVC; }
    __VertexT1 * Vertices() { return m_pVertices; }
    int          IndexCount() { return m_nIC; }
    WORD *       Indices() { return m_psnIndices; } // index buffer

    bool Load(HANDLE hFile);

    void Create(int nVC, int nIC);
    void Create_Cube(const __Vector3 & vMin, const __Vector3 & vMax);

    bool Import(CN3PMesh * pPMesh);

#ifdef _N3TOOL
    bool Import(CN3IMesh * pIMesh);
    bool Save(HANDLE hFile);
    void Create_Axis(float fLength);
    void MakeIndexed();
    void ReGenerateSmoothNormal();
#endif // end of _N3TOOL

    void Release();
    void ReleaseVertices();
    void ReleaseIndices();

    CN3Mesh();
    virtual ~CN3Mesh();
};
