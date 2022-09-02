// N3FXPMesh.h: interface for the CN3FXPMesh class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"
#include "N3PMesh.h"

class CN3FXPMesh : public CN3PMesh {
    friend class CN3FXPMeshInstance;

  private:
    __VertexXyzColorT1 * m_pColorVertices;

  public:
    void                 Render();
    void                 Release();
    void                 FindMinMax();
    bool                 Load(HANDLE hFile);
    void                 SetColor(DWORD dwColor = 0xffffffff);
    HRESULT              Create(int iNumVertices, int iNumIndices);
    void                 operator=(const CN3FXPMesh & fxPMesh);
    __VertexXyzColorT1 * GetColorVertices() { return m_pColorVertices; }

  public:
    CN3FXPMesh();
    virtual ~CN3FXPMesh();
};
