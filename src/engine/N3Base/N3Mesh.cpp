// N3Mesh.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3Mesh.h"
#include "N3IMesh.h"
#include "N3PMesh.h"
#include "N3PMeshInstance.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Mesh::CN3Mesh() {
    m_dwType |= OBJ_MESH;

    m_vMin.Set(0, 0, 0);
    m_vMax.Set(0, 0, 0);

    m_nVC = 0;
    m_nIC = 0;

    m_pVertices = NULL;  // dot buffer
    m_psnIndices = NULL; // index buffer
}

CN3Mesh::~CN3Mesh() {
    delete[] m_pVertices;
    m_pVertices = NULL;
    delete[] m_psnIndices;
    m_psnIndices = NULL;
}

void CN3Mesh::Release() {
    this->ReleaseVertices();
    this->ReleaseIndices();

    CN3BaseFileAccess::Release();
}

void CN3Mesh::ReleaseVertices() {
    delete[] m_pVertices;
    m_pVertices = NULL;

    m_vMin.Set(0, 0, 0);
    m_vMax.Set(0, 0, 0);

    m_nVC = 0;
}

void CN3Mesh::ReleaseIndices() {
    delete[] m_psnIndices;
    m_psnIndices = NULL;
    m_nIC = 0;
}

bool CN3Mesh::Load(HANDLE hFile) {
    if (m_pVertices != NULL || m_psnIndices != NULL) {
        this->Release();
    }

    DWORD dwRWC = 0;

    ReadFile(hFile, &m_nVC, 4, &dwRWC, NULL); // Read the number of dots...
    if (m_nVC > 0) {
        this->Create(m_nVC, 0); // Create Vertex Buffer and fill data
        ReadFile(hFile, m_pVertices, m_nVC * sizeof(__VertexT1), &dwRWC, NULL);
        this->FindMinMax(); // Find the maximum and minimum values.
    }

    ReadFile(hFile, &m_nIC, 4, &dwRWC, NULL); // Read the number of indexes...
    if (m_nIC > 0) {
        this->Create(0, m_nIC); // Create index buffer and fill data
        ReadFile(hFile, m_psnIndices, m_nIC * 2, &dwRWC, NULL);
    }

    return true;
}

#ifdef _N3TOOL
bool CN3Mesh::Save(HANDLE hFile) {
    DWORD dwRWC = 0;

    WriteFile(hFile, &m_nVC, 4, &dwRWC, NULL); // Read the number of dots...
    if (m_nVC > 0) {
        WriteFile(hFile, m_pVertices, m_nVC * sizeof(__VertexT1), &dwRWC, NULL);
    }

    WriteFile(hFile, &m_nIC, 4, &dwRWC, NULL); // Read the number of indexes...
    if (m_nIC > 0) {
        WriteFile(hFile, m_psnIndices, m_nIC * 2, &dwRWC, NULL);
    }

    return true;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
void CN3Mesh::MakeIndexed() {
    if (m_nVC <= 0 || m_nIC > 0) {
        return;
    }

    int nIs[100000];
    memset(nIs, -1, sizeof(nIs));

    __VertexT1 * pVs = new __VertexT1[m_nVC];
    memcpy(pVs, m_pVertices, sizeof(__VertexT1) * m_nVC);
    int nICount = 0, nVCount = 0;
    for (int i = 0; i < m_nVC; i++) {
        BOOL bAccord = FALSE;
        int  j;
        for (j = 0; j < nVCount; j++) {
            if (m_pVertices[i].x == pVs[j].x && m_pVertices[i].y == pVs[j].y && m_pVertices[i].z == pVs[j].z &&
                //                m_pVertices[i].nx == pVs[j].nx &&
                //                m_pVertices[i].ny == pVs[j].ny &&
                //                m_pVertices[i].nz == pVs[j].nz &&
                m_pVertices[i].tu == pVs[j].tu && m_pVertices[i].tv == pVs[j].tv) {
                bAccord = TRUE;
                break;
            }
        }

        if (TRUE == bAccord) {
            nIs[i] = j;
        } else {
            nIs[i] = nVCount;
            pVs[nVCount] = m_pVertices[i];
            nVCount++;
        }
    }

    this->Create(nVCount, m_nVC);
    memcpy(m_pVertices, pVs, sizeof(__VertexT1) * nVCount);
    for (int i = 0; i < m_nIC; i++) {
        m_psnIndices[i] = nIs[i];
    }

    delete[] pVs;
    pVs = NULL;
}
#endif // end of _N3TOOL

void CN3Mesh::Create(int nVC, int nIC) {
    if (nVC > 0) // If there is a point...
    {
#ifdef _N3GAME
        if (nVC > 32768) {
            CLogWriter::Write("CN3IMesh::Create - Too many vertices. (more than 32768) (%s)", m_szFileName.c_str());
        }
#endif
        if (m_pVertices) {
            this->ReleaseVertices();
        }
        m_pVertices = new __VertexT1[nVC];
        memset(m_pVertices, 0, nVC * sizeof(__VertexT1)); // Create Vertex Buffer
        m_nVC = nVC;
    }

    if (nIC > 0) // If mesh loading is successful and there is an index...
    {
#ifdef _N3GAME
        if (nIC > 32768) {
            CLogWriter::Write("CN3IMesh::Create - Too many indices. (more than 32768) (%s)", m_szFileName.c_str());
        }
#endif
        if (m_psnIndices) {
            this->ReleaseIndices();
        }
        m_psnIndices = new WORD[nIC];
        memset(m_psnIndices, 0, nIC * 2); // Create Index Buffer
        m_nIC = nIC;
    }
}

void CN3Mesh::FindMinMax() {
    m_vMin.Zero();
    m_vMax.Zero();

    if (m_nVC <= 0) {
        return;
    }

    // Find the minimum and maximum points.
    m_vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
    m_vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < m_nVC; i++) {
        if (m_pVertices[i].x < m_vMin.x) {
            m_vMin.x = m_pVertices[i].x;
        }
        if (m_pVertices[i].y < m_vMin.y) {
            m_vMin.y = m_pVertices[i].y;
        }
        if (m_pVertices[i].z < m_vMin.z) {
            m_vMin.z = m_pVertices[i].z;
        }

        if (m_pVertices[i].x > m_vMax.x) {
            m_vMax.x = m_pVertices[i].x;
        }
        if (m_pVertices[i].y > m_vMax.y) {
            m_vMax.y = m_pVertices[i].y;
        }
        if (m_pVertices[i].z > m_vMax.z) {
            m_vMax.z = m_pVertices[i].z;
        }
    }
}

void CN3Mesh::Create_Cube(const __Vector3 & vMin, const __Vector3 & vMax) {
    this->Create(36, 0);

    __Vector3 vPs[6];
    __Vector3 vN;
    float     fTUVs[6][2] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

    // z-axis negative side
    vN.Set(0, 0, -1);
    vPs[0].Set(vMin.x, vMax.y, vMin.z);
    vPs[1].Set(vMax.x, vMax.y, vMin.z);
    vPs[2].Set(vMax.x, vMin.y, vMin.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMin.x, vMin.y, vMin.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[0 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    // x-axis positive side
    vN.Set(1, 0, 0);
    vPs[0].Set(vMax.x, vMax.y, vMin.z);
    vPs[1].Set(vMax.x, vMax.y, vMax.z);
    vPs[2].Set(vMax.x, vMin.y, vMax.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMax.x, vMin.y, vMin.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[6 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    // z axis positive side
    vN.Set(0, 0, 1);
    vPs[0].Set(vMax.x, vMax.y, vMax.z);
    vPs[1].Set(vMin.x, vMax.y, vMax.z);
    vPs[2].Set(vMin.x, vMin.y, vMax.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMax.x, vMin.y, vMax.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[12 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    // x-axis negative side
    vN.Set(-1, 0, 0);
    vPs[0].Set(vMin.x, vMax.y, vMax.z);
    vPs[1].Set(vMin.x, vMax.y, vMin.z);
    vPs[2].Set(vMin.x, vMin.y, vMin.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMin.x, vMin.y, vMax.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[18 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    //y axis positive side
    vN.Set(0, 1, 0);
    vPs[0].Set(vMin.x, vMax.y, vMax.z);
    vPs[1].Set(vMax.x, vMax.y, vMax.z);
    vPs[2].Set(vMax.x, vMax.y, vMin.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMin.x, vMax.y, vMin.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[24 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    // y axis negative side
    vN.Set(0, -1, 0);
    vPs[0].Set(vMin.x, vMin.y, vMin.z);
    vPs[1].Set(vMax.x, vMin.y, vMin.z);
    vPs[2].Set(vMax.x, vMin.y, vMax.z);
    vPs[3] = vPs[0];
    vPs[4] = vPs[2];
    vPs[5].Set(vMin.x, vMin.y, vMax.z);
    for (int i = 0; i < 6; i++) {
        m_pVertices[30 + i].Set(vPs[i], vN, fTUVs[i][0], fTUVs[i][1]);
    }

    this->FindMinMax();
}

#ifdef _N3TOOL
void CN3Mesh::Create_Axis(float fLength) {
    this->Create(12, 0);

    this->FindMinMax();

    // x รเ
    //    m_pVertices[0].Set(-fLength/2.0f, 0, 0, 0, 1, 0, 0, 0);
    //    m_pVertices[1].Set( fLength/2.0f, 0, 0, 0, 1, 0, 0, 0);
    //    m_pVertices[2] = m_pVertices[1];
    //    m_pVertices[3] = m_pVertices[1];
    //    m_pVertices[3].x -= fLength / 20.0f;
    //    m_pVertices[3].y += fLength / 20.0f;

    m_pVertices[0].Set(0, 0, 0, 0, 1, 0, 0, 0);
    m_pVertices[1].Set(fLength, 0, 0, 0, 1, 0, 0, 0);
    m_pVertices[2] = m_pVertices[1];
    m_pVertices[3] = m_pVertices[1];
    m_pVertices[3].x -= fLength / 10.0f;
    m_pVertices[3].y += fLength / 10.0f;

    __Matrix44 mtx;

    //y axis
    mtx.RotationZ(D3DX_PI / -2.0f);
    for (int i = 0; i < 4; i++) {
        m_pVertices[4 + i].Set(m_pVertices[i] * mtx, m_pVertices[i].n * mtx, 0, 0);
    }

    //z axis
    mtx.RotationY(D3DX_PI / -2.0f);
    for (int i = 0; i < 4; i++) {
        m_pVertices[8 + i].Set(m_pVertices[i] * mtx, m_pVertices[i].n * mtx, 0, 0);
    }
}
#endif // end of _N3TOOL

bool CN3Mesh::Import(CN3PMesh * pPMesh) {
    if (NULL == pPMesh) {
        return false;
    }
    int iNumIndices = pPMesh->GetMaxNumIndices();
    if (0 >= iNumIndices) {
        return false;
    }
    Release(); // reset

    // Create pmesh instance and adjust lod to highest state
    CN3PMeshInstance PMeshInstance;
    PMeshInstance.Create(pPMesh);
    PMeshInstance.SetLODByNumVertices(pPMesh->GetMaxNumVertices());

    // Create vertex, index buffer
    Create(PMeshInstance.GetNumVertices(), PMeshInstance.GetNumIndices());

    // Copy vertex index buffer
    __VertexT1 * pVertices = PMeshInstance.GetVertices();
    WORD *       pIndices = PMeshInstance.GetIndices();
    memcpy(m_pVertices, pVertices, sizeof(__VertexT1) * m_nVC);
    memcpy(m_psnIndices, pIndices, sizeof(WORD) * m_nIC);

    m_szName = pPMesh->m_szName; // name..
    return true;
}

#ifdef _N3TOOL
bool CN3Mesh::Import(CN3IMesh * pIMesh) {
    if (NULL == pIMesh) {
        return false;
    }
    __VertexT1 * pvSrc = pIMesh->BuildVertexList();
    if (NULL == pvSrc) {
        return false;
    }

    int nFC = pIMesh->FaceCount();
    this->Release();
    this->Create(nFC * 3, 0);

    memcpy(m_pVertices, pvSrc, sizeof(__VertexT1) * nFC * 3);

    __Vector3 v0, v1, v2, vN(0, 0, 0);
    for (int i = 0; i < nFC; i++) // Reset normal value..
    {
        v0 = m_pVertices[i * 3 + 0];
        v1 = m_pVertices[i * 3 + 1];
        v2 = m_pVertices[i * 3 + 2];

        vN.Cross(v1 - v0, v2 - v1); // Calculate the normal value....
        vN.Normalize();

        m_pVertices[i * 3 + 0].n = vN;
        m_pVertices[i * 3 + 1].n = vN;
        m_pVertices[i * 3 + 2].n = vN;
    }

    m_szName = pIMesh->m_szName; // name..
    return true;
}

void CN3Mesh::ReGenerateSmoothNormal() {
    if (m_nVC <= 0) {
        return;
    }

    int * pnNs = new int[m_nVC];
    memset(pnNs, 0, 4 * m_nVC);
    __Vector3 * pvNs = new __Vector3[m_nVC];
    memset(pvNs, 0, sizeof(__Vector3) * m_nVC);

    int nFC = 0;
    if (m_nIC > 0) {
        nFC = m_nIC / 3;
    } else {
        nFC = m_nVC / 3;
    }

    __Vector3 v0, v1, v2, vN(0, 0, 0);
    for (int i = 0; i < m_nVC; i++) {
        for (int j = 0; j < nFC; j++) {
            if (m_nIC > 0) {
                v0 = m_pVertices[m_psnIndices[j * 3 + 0]];
                v1 = m_pVertices[m_psnIndices[j * 3 + 1]];
                v2 = m_pVertices[m_psnIndices[j * 3 + 2]];
            } else {
                v0 = m_pVertices[j * 3 + 0];
                v1 = m_pVertices[j * 3 + 1];
                v2 = m_pVertices[j * 3 + 2];
            }

            if (m_pVertices[i] == v0 || m_pVertices[i] == v1 || m_pVertices[i] == v2) {
                vN.Cross(v1 - v0, v2 - v1); // Calculate the Normal value...
                vN.Normalize();

                pnNs[i]++;
                pvNs[i] += vN;
            }
        }

        m_pVertices[i].n = pvNs[i] / (float)pnNs[i];
    }

    delete[] pnNs;
    delete[] pvNs;
}
#endif // end of _N3TOOL
