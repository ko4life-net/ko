// N3FXPMesh.cpp: implementation of the CN3FXPMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXPMesh.h"
#include "N3PMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXPMesh::CN3FXPMesh() {
    m_fRadius = 0.0f;
    m_pColorVertices = NULL;
    m_pVertices = NULL;
    m_pVertices2 = NULL;
    m_pIndices = NULL;
}

CN3FXPMesh::~CN3FXPMesh() {
    Release();
}

HRESULT CN3FXPMesh::Create(int iNumVertices, int iNumIndices) {
    m_iMaxNumVertices = iNumVertices;
    m_iMaxNumIndices = iNumIndices;

    if (m_pVertices) {
        delete m_pVertices;
        m_pVertices = NULL;
    }
    if (m_pIndices) {
        delete m_pIndices;
        m_pIndices = NULL;
    }
    if (m_pColorVertices) {
        delete m_pColorVertices;
        m_pColorVertices = NULL;
    }

    if (m_iMaxNumVertices > 0) {
        m_pVertices = new __VertexT1[m_iMaxNumVertices];
    }
    if (m_iMaxNumIndices > 0) {
        m_pIndices = new WORD[m_iMaxNumIndices];
    }
    if (m_iMaxNumVertices > 0) {
        m_pColorVertices = new __VertexXyzColorT1[m_iMaxNumVertices];
    }

    return S_OK;
}

void CN3FXPMesh::operator=(const CN3FXPMesh & fxPMesh) {
    if (this == &fxPMesh) {
        return;
    }

    Release();

    FilePathSet(fxPMesh.FilePath());

    m_iMaxNumVertices = fxPMesh.m_iMaxNumVertices;
    m_iMaxNumIndices = fxPMesh.m_iMaxNumIndices;
    m_iMinNumVertices = fxPMesh.m_iMinNumVertices;
    m_iMinNumIndices = fxPMesh.m_iMinNumIndices;
    m_iNumCollapses = fxPMesh.m_iNumCollapses;

    m_fRadius = fxPMesh.m_fRadius;
    m_vMax = fxPMesh.m_vMax;
    m_vMin = fxPMesh.m_vMin;

    m_iLODCtrlValueCount = fxPMesh.m_iLODCtrlValueCount;
    m_iTotalIndexChanges = fxPMesh.m_iTotalIndexChanges;

    Create(m_iMaxNumVertices, m_iMaxNumIndices);

    memcpy(m_pColorVertices, fxPMesh.m_pColorVertices, sizeof(__VertexXyzColorT1) * m_iMaxNumVertices);
    memcpy(m_pIndices, fxPMesh.m_pIndices, sizeof(WORD) * m_iMaxNumIndices);

    if (m_iNumCollapses > 0) {
        m_pCollapses = new __EdgeCollapse
            [m_iNumCollapses +
             1]; // +1을 한 이유 : PMeshInstance::SplitOne() 함수에서 부득이하게 포인터가 경계선을 가르키게 해야 하는 경우가 있어서.
        memcpy(m_pCollapses, fxPMesh.m_pCollapses, sizeof(__EdgeCollapse) * (m_iMaxNumIndices + 1));
    }

    m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
    memcpy(m_pLODCtrlValues, fxPMesh.m_pLODCtrlValues, sizeof(__LODCtrlValue) * m_iLODCtrlValueCount);

    m_pAllIndexChanges = new int[m_iTotalIndexChanges];
    memcpy(m_pAllIndexChanges, fxPMesh.m_pAllIndexChanges, sizeof(int) * m_iTotalIndexChanges);
}

bool CN3FXPMesh::Load(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);

    DWORD dwNum;
    ReadFile(hFile, &m_iNumCollapses, sizeof(m_iNumCollapses), &dwNum, NULL);
    ReadFile(hFile, &m_iTotalIndexChanges, sizeof(m_iTotalIndexChanges), &dwNum, NULL);

    ReadFile(hFile, &m_iMaxNumVertices, sizeof(int), &dwNum, NULL);
    ReadFile(hFile, &m_iMaxNumIndices, sizeof(int), &dwNum, NULL);
    ReadFile(hFile, &m_iMinNumVertices, sizeof(int), &dwNum, NULL);
    ReadFile(hFile, &m_iMinNumIndices, sizeof(int), &dwNum, NULL);

    HRESULT hr = Create(m_iMaxNumVertices, m_iMaxNumIndices);
    __ASSERT(SUCCEEDED(hr), "Failed to create progressive mesh");

    if (m_iMaxNumVertices > 0) {
        ReadFile(hFile, m_pVertices, m_iMaxNumVertices * sizeof(__VertexT1), &dwNum, NULL);
        for (int i = 0; i < m_iMaxNumVertices; i++) {
            m_pColorVertices[i].x = m_pVertices[i].x;
            m_pColorVertices[i].y = m_pVertices[i].y;
            m_pColorVertices[i].z = m_pVertices[i].z;
            m_pColorVertices[i].color = 0xffffffff;
            m_pColorVertices[i].tu = m_pVertices[i].tu;
            m_pColorVertices[i].tv = m_pVertices[i].tv;
        }

        delete[] m_pVertices;
        m_pVertices = NULL;
    }

    if (m_iMaxNumIndices > 0) {
        ReadFile(hFile, m_pIndices, m_iMaxNumIndices * sizeof(WORD), &dwNum, NULL);
    }

    if (m_iNumCollapses > 0) {
        m_pCollapses = new __EdgeCollapse
            [m_iNumCollapses +
             1]; // +1을 한 이유 : PMeshInstance::SplitOne() 함수에서 부득이하게 포인터가 경계선을 가르키게 해야 하는 경우가 있어서.
        ReadFile(hFile, m_pCollapses, m_iNumCollapses * sizeof(__EdgeCollapse), &dwNum, NULL);
        ZeroMemory(m_pCollapses + m_iNumCollapses,
                   sizeof(__EdgeCollapse)); // 위의 +1을 한이유와 같음. 만약을 대비해 마지막 데이타를 초기화 해둠

        bool bFixed = false;
        for (int i = 0; i < m_iNumCollapses; i++) {
            if (m_pCollapses[i].iIndexChanges < 0) {
                m_pCollapses[i].iIndexChanges = 0;
                bFixed = true;
            }
        }
#ifdef _DEBUG
        if (bFixed) {
            ::MessageBox(s_hWndBase, "잘못된 Progressive Mesh 수정", m_szName.c_str(), MB_OK);
        }
#endif
    }
    if (m_iTotalIndexChanges > 0) {
        m_pAllIndexChanges = new int[m_iTotalIndexChanges];
        ReadFile(hFile, m_pAllIndexChanges, m_iTotalIndexChanges * sizeof(int), &dwNum, NULL);
    }

    __ASSERT(m_pLODCtrlValues == NULL && m_iLODCtrlValueCount == 0, "Invalid Level of detail control value");
    ReadFile(hFile, &m_iLODCtrlValueCount, sizeof(m_iLODCtrlValueCount), &dwNum, NULL);
    if (m_iLODCtrlValueCount > 0) {
        m_pLODCtrlValues = new __LODCtrlValue[m_iLODCtrlValueCount];
        ReadFile(hFile, m_pLODCtrlValues, m_iLODCtrlValueCount * sizeof(__LODCtrlValue), &dwNum, NULL);
    }

    FindMinMax();

    return true;
}

void CN3FXPMesh::Release() {
    CN3PMesh::Release();

    if (m_pVertices) {
        delete m_pVertices;
        m_pVertices = NULL;
    }
    if (m_pIndices) {
        delete m_pIndices;
        m_pIndices = NULL;
    }
    if (m_pColorVertices) {
        delete m_pColorVertices;
        m_pColorVertices = NULL;
    }
}

void CN3FXPMesh::Render() {
    s_lpD3DDev->SetFVF(FVF_XYZCOLORT1);

    const int iPCToRender = 1000; // primitive count to render
    if (m_iMaxNumIndices > 3) {
        int iPC = m_iMaxNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iMaxNumVertices, iPCToRender,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pColorVertices,
                                               sizeof(__VertexXyzColorT1));
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iMaxNumVertices, iRPC,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pColorVertices,
                                               sizeof(__VertexXyzColorT1));
        }
    }
}

void CN3FXPMesh::FindMinMax() {
    if (m_iMaxNumVertices <= 0) {
        m_vMin.Zero();
        m_vMax.Zero();
        m_fRadius = 0;
        return;
    }

    // 최소, 최대 점을 찾는다.
    m_vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
    m_vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < m_iMaxNumVertices; i++) {
        if (m_pColorVertices[i].x < m_vMin.x) {
            m_vMin.x = m_pColorVertices[i].x;
        }
        if (m_pColorVertices[i].y < m_vMin.y) {
            m_vMin.y = m_pColorVertices[i].y;
        }
        if (m_pColorVertices[i].z < m_vMin.z) {
            m_vMin.z = m_pColorVertices[i].z;
        }

        if (m_pColorVertices[i].x > m_vMax.x) {
            m_vMax.x = m_pColorVertices[i].x;
        }
        if (m_pColorVertices[i].y > m_vMax.y) {
            m_vMax.y = m_pColorVertices[i].y;
        }
        if (m_pColorVertices[i].z > m_vMax.z) {
            m_vMax.z = m_pColorVertices[i].z;
        }
    }

    // 최대 최소값을 갖고 반지름 계산한다..
    m_fRadius = (m_vMax - m_vMin).Magnitude() * 0.5f;
}

void CN3FXPMesh::SetColor(DWORD dwColor) {
    if (m_pColorVertices == NULL) {
        return;
    }
    if (m_iMaxNumVertices <= 0) {
        return;
    }

    for (int i = 0; i < m_iMaxNumVertices; i++) {
        m_pColorVertices[i].color = dwColor;
    }
}
