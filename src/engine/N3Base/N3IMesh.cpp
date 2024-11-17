// N3MeshIndexed: implementation of the CN3IMesh class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3IMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

__VertexT1 CN3IMesh::s_Vertices[MAX_IMESH_BUFFER];
__VertexT2 CN3IMesh::s_Vertices2[MAX_IMESH_BUFFER];

CN3IMesh::CN3IMesh() {
    m_dwType |= OBJ_MESH_INDEXED;

    m_nFC = 0;

    m_pwVtxIndices = NULL; // 점 인덱스 리스트.
    m_pwUVsIndices = NULL; // 텍스처 좌표 인덱스 리스트.

    m_nVC = m_nUVC = 0;
    m_pVertices = NULL;
    m_pfUVs = NULL;
    //    m_lpVB = NULL;
}

CN3IMesh::~CN3IMesh() {
    delete[] m_pVertices;
    m_pVertices = NULL;
    delete[] m_pfUVs;
    m_pfUVs = NULL;

    delete[] m_pwVtxIndices;
    m_pwVtxIndices = NULL; // 점 인덱스 리스트.
    delete[] m_pwUVsIndices;
    m_pwUVsIndices = NULL; // 텍스처 좌표 인덱스 리스트.

    //    if(m_lpVB) m_lpVB->Release();
}

void CN3IMesh::Release() {
    CN3BaseFileAccess::Release();

    m_nFC = 0;
    m_nVC = 0;
    m_nUVC = 0;

    delete[] m_pVertices;
    m_pVertices = NULL;
    delete[] m_pfUVs;
    m_pfUVs = NULL;

    delete[] m_pwVtxIndices;
    m_pwVtxIndices = NULL; // 점 인덱스 리스트.
    delete[] m_pwUVsIndices;
    m_pwUVsIndices = NULL; // 텍스처 좌표 인덱스 리스트.

    //    if(m_lpVB) m_lpVB->Release(); m_lpVB = NULL;

    m_vMin.Zero();
    m_vMax.Zero();
}

bool CN3IMesh::Create(int nFC, int nVC, int nUVC) {
    if (nFC <= 0 || nVC <= 0) {
        return false; // "Face count is less than 0 or vertex count is less than 0");
    }

    if (m_nFC > 0) {
        this->Release();
    }

#ifdef _N3GAME
    if (nFC > 32768) {
        CLogWriter::Write("CN3IMesh::Create - Too many faces. (more than 32768) (%s)", FilePath().string().c_str());
    }
#endif

    m_nFC = nFC;
    m_nVC = nVC;
    m_pVertices = new __VertexXyzNormal[nVC];
    memset(m_pVertices, 0, sizeof(__VertexXyzNormal) * nVC);
    m_pwVtxIndices = new WORD[nFC * 3];
    memset(m_pwVtxIndices, 0, 2 * nFC * 3); // unsigned short

    if (nUVC > 0) {
        m_nUVC = nUVC;
        m_pfUVs = new float[nUVC * 2];
        memset(m_pfUVs, 0, 8 * nUVC); // 사이즈가 8 인 이유는 float 2개라 그렇다..
        m_pwUVsIndices = new WORD[nFC * 3];
        memset(m_pwUVsIndices, 0, 2 * nFC * 3); // unsigned short
    }

    //    s_lpD3DDev->CreateVertexBuffer(nFC * 3 * sizeof(__VertexT1), 0, FVF_VNT1, D3DPOOL_MANAGED, &m_lpVB, NULL);
    //
    //    if (NULL == m_lpVB) {
    //#ifdef _N3GAME
    //        CLogWriter::Write("CN3IMesh::Create - Can't Create VertexBuffer (%s)", FilePath().string().c_str());
    //#endif
    //        this->Release();
    //        return false;
    //    }

    return true;
}

__VertexT1 * CN3IMesh::BuildVertexList() {
    if (m_nFC <= 0) {
        return NULL;
    }
    if (m_nFC >= MAX_IMESH_BUFFER / 3) {
        __ASSERT(0, "Vertex Buffer Overflow");
        return NULL;
    }

    int n = 0, nVI = 0, nUVI = 0;
    if (m_nUVC > 0) {
        for (int i = 0; i < m_nFC; i++) {
            n = i * 3 + 0;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                              m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);

            n = i * 3 + 1;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                              m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);

            n = i * 3 + 2;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                              m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);
        }
    } else {
        for (int i = 0; i < m_nFC; i++) {
            n = i * 3 + 0;
            nVI = m_pwVtxIndices[n];
            s_Vertices[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 0);

            n = i * 3 + 1;
            nVI = m_pwVtxIndices[n];
            s_Vertices[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 0);

            n = i * 3 + 2;
            nVI = m_pwVtxIndices[n];
            s_Vertices[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 0);
        }
    }

    return s_Vertices;
}

__VertexT2 * CN3IMesh::BuildVertexListTwoUV() {
    if (m_nFC <= 0) {
        return NULL;
    }
    if (m_nFC >= MAX_IMESH_BUFFER / 3) {
        __ASSERT(0, "Vertex Buffer Overflow");
        return NULL;
    }

    int n = 0, nVI = 0, nUVI = 0;
    if (m_nUVC > 0) {
        for (int i = 0; i < m_nFC; i++) {
            n = i * 3 + 0;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices2[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                               m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1],
                               m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);

            n = i * 3 + 1;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices2[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                               m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1],
                               m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);

            n = i * 3 + 2;
            nVI = m_pwVtxIndices[n];
            nUVI = m_pwUVsIndices[n];
            //            s_Vertices[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            s_Vertices2[n].Set(m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z, m_pVertices[nVI].n.x,
                               m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1],
                               m_pfUVs[nUVI * 2], m_pfUVs[nUVI * 2 + 1]);
        }
    } else {
        for (int i = 0; i < m_nFC; i++) {
            n = i * 3 + 0;
            nVI = m_pwVtxIndices[n];
            s_Vertices2[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 1, 0, 1);

            n = i * 3 + 1;
            nVI = m_pwVtxIndices[n];
            s_Vertices2[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 1, 0, 1);

            n = i * 3 + 2;
            nVI = m_pwVtxIndices[n];
            s_Vertices2[n].Set(m_pVertices[nVI], m_pVertices[nVI].n, 0, 1, 0, 1);
        }
    }

    return s_Vertices2;
}

/*
__VertexT1* CN3IMesh::BuildVertexList()
{
    if(m_nFC <= 0 || NULL == m_lpVB) return NULL;

    __VertexT1* pVDests = NULL;
    m_lpVB->Lock(0, 0, (VOID**)(&pVDests), 0);

    int n = 0, nVI = 0, nUVI = 0;
    if(m_nUVC > 0)
    {
        for(int i = 0; i < m_nFC; i++)
        {
            n = i*3+0; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
//            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            pVDests[n].Set(    m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z,
                            m_pVertices[nVI].n.x, m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, 
                            m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            
            n = i*3+1; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
//            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            pVDests[n].Set(    m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z,
                            m_pVertices[nVI].n.x, m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, 
                            m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );

            n = i*3+2; nVI = m_pwVtxIndices[n]; nUVI = m_pwUVsIndices[n];
//            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
            pVDests[n].Set(    m_pVertices[nVI].x, m_pVertices[nVI].y, m_pVertices[nVI].z,
                            m_pVertices[nVI].n.x, m_pVertices[nVI].n.y, m_pVertices[nVI].n.z, 
                            m_pfUVs[nUVI*2], m_pfUVs[nUVI*2+1] );
        }
    }
    else
    {
        for(int i = 0; i < m_nFC; i++)
        {
            n = i*3+0; nVI = m_pwVtxIndices[n];
            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, 0, 0 );
            
            n = i*3+1; nVI = m_pwVtxIndices[n];
            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, 0, 0 );

            n = i*3+2; nVI = m_pwVtxIndices[n];
            pVDests[n].Set(    m_pVertices[nVI], m_pVertices[nVI].n, 0, 0 );
        }
    }

    m_lpVB->Unlock();
    return pVDests;
}
*/

void CN3IMesh::Render(bool bUseTwoUV) {
    if (s_lpD3DDev == NULL || m_nFC <= 0) {
        return;
    }

    if (bUseTwoUV) {
        __VertexT2 * pVs = this->BuildVertexListTwoUV();
        if (pVs) {
            s_lpD3DDev->SetFVF(FVF_VNT2);
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_nFC, pVs, sizeof(__VertexT2));
        }
    } else {
        __VertexT1 * pVs = this->BuildVertexList();
        if (pVs) {
            s_lpD3DDev->SetFVF(FVF_VNT1);
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_nFC, pVs, sizeof(__VertexT1));
        }
    }

    //    this->BuildVertexList();
    //    s_lpD3DDev->SetFVF(FVF_VNT1);
    //    s_lpD3DDev->SetStreamSource( 0, m_lpVB, 0, sizeof(__VertexT1) );
    //    s_lpD3DDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nFC);
}

#ifdef _N3TOOL
void CN3IMesh::RenderSelected() {
    if (s_lpD3DDev == NULL || m_nFC <= 0) {
        return;
    }
    __VertexT1 * pVs = this->BuildVertexList();
    if (NULL == pVs) {
        return;
    }

    __Vector3 vFace[4];
    for (int i = 0; i < m_nFC; i++) {
        vFace[0] = pVs[i * 3 + 0];
        vFace[1] = pVs[i * 3 + 1];
        vFace[2] = pVs[i * 3 + 2];
        vFace[3] = vFace[0];
        CN3Base::RenderLines(vFace, 3, 0xff00ff00); // 녹색으로 렌더링..
    }
}
#endif // end of _N3TOOL

bool CN3IMesh::Load(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);
    DWORD dwRWC = 0;

    int nFC = 0, nVC = 0, nUVC = 0;

    ReadFile(hFile, &nFC, 4, &dwRWC, NULL);
    ReadFile(hFile, &nVC, 4, &dwRWC, NULL);
    ReadFile(hFile, &nUVC, 4, &dwRWC, NULL);

    if (nFC > 0 && nVC > 0) {
        this->Create(nFC, nVC, nUVC);
        ReadFile(hFile, m_pVertices, sizeof(__VertexXyzNormal) * nVC, &dwRWC, NULL);
        ReadFile(hFile, m_pwVtxIndices, 2 * nFC * 3, &dwRWC, NULL); // unsigned short
    } else {
        Release();
    }

    if (m_nUVC > 0) {
        ReadFile(hFile, m_pfUVs, 8 * nUVC, &dwRWC, NULL);
        ReadFile(hFile, m_pwUVsIndices, 2 * nFC * 3, &dwRWC, NULL); // unsigned short
    }

    this->FindMinMax(); // 최소 최대값을 찾는다..

    return true;
}

#ifdef _N3TOOL
bool CN3IMesh::Save(HANDLE hFile) {
    CN3BaseFileAccess::Save(hFile);

    DWORD dwRWC = 0;

    WriteFile(hFile, &m_nFC, 4, &dwRWC, NULL);
    WriteFile(hFile, &m_nVC, 4, &dwRWC, NULL);
    WriteFile(hFile, &m_nUVC, 4, &dwRWC, NULL);

    if (m_nFC > 0 && m_nVC > 0) {
        WriteFile(hFile, m_pVertices, sizeof(__VertexXyzNormal) * m_nVC, &dwRWC, NULL);
        WriteFile(hFile, m_pwVtxIndices, 2 * m_nFC * 3, &dwRWC, NULL); // unsigned short
    }

    if (m_nUVC > 0) {
        WriteFile(hFile, m_pfUVs, 8 * m_nUVC, &dwRWC, NULL);
        WriteFile(hFile, m_pwUVsIndices, 2 * m_nFC * 3, &dwRWC, NULL); // unsigned short
    }

    return true;
}
#endif // end of _N3TOOL

void CN3IMesh::FindMinMax() {
    m_vMin.Zero();
    m_vMax.Zero();

    if (m_pVertices == NULL || m_nVC < 0) {
        return;
    }

    // 최소, 최대 점을 찾는다.
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

#ifdef _N3TOOL
void CN3IMesh::ReGenerateSmoothNormal() {
    if (m_nFC <= 0) {
        return;
    }
    if (NULL == this->BuildVertexList()) {
        return;
    }

    int * pnNs = new int[m_nVC];
    memset(pnNs, 0, 4 * m_nVC);
    __Vector3 * pvNs = new __Vector3[m_nVC];
    memset(pvNs, 0, sizeof(__Vector3) * m_nVC);

    __Vector3 v0, v1, v2, vN(0, 0, 0);
    for (int i = 0; i < m_nVC; i++) {
        for (int j = 0; j < m_nFC; j++) {
            v0 = m_pVertices[m_pwVtxIndices[j * 3 + 0]];
            v1 = m_pVertices[m_pwVtxIndices[j * 3 + 1]];
            v2 = m_pVertices[m_pwVtxIndices[j * 3 + 2]];

            if ((m_pVertices[i].x == v0.x && m_pVertices[i].y == v0.y && m_pVertices[i].z == v0.z) ||
                (m_pVertices[i].x == v1.x && m_pVertices[i].y == v1.y && m_pVertices[i].z == v1.z) ||
                (m_pVertices[i].x == v2.x && m_pVertices[i].y == v2.y && m_pVertices[i].z == v2.z)) {
                vN.Cross(v1 - v0, v2 - v1);
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

#ifdef _N3TOOL
void CN3IMesh::ApplyOffset(__Vector3 vOffset) {
    __Vector3 v;
    for (int i = 0; i < m_nVC; i++) {
        m_pVertices[i].x += vOffset.x;
        m_pVertices[i].y += vOffset.y;
        m_pVertices[i].z += vOffset.z;
    }
}
#endif // end of _N3TOOL

#ifdef _N3GAME
void CN3IMesh::TickForShadow(bool bUseTwoUV) {
    if (s_lpD3DDev == NULL || m_nFC <= 0) {
        m_pVertexT1 = NULL;
        return;
    }

    if (bUseTwoUV) {
        __Vector3 * pVs = (__Vector3 *)this->BuildVertexListTwoUV();
        if (pVs) {
            m_pVertexT1 = pVs;
        } else {
            m_pVertexT1 = NULL;
        }
    } else {
        __Vector3 * pVs = (__Vector3 *)this->BuildVertexList();
        if (pVs) {
            m_pVertexT1 = pVs;
        } else {
            m_pVertexT1 = NULL;
        }
    }
}

int CN3IMesh::GetVertexCount() {
    return m_nFC;
}

__Vector3 * CN3IMesh::GetVertexData() {
    return m_pVertexT1;
}
#endif
