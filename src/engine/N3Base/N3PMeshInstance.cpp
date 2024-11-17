// N3PMeshInstance.cpp: implementation of the CN3PMeshInstance class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3PMeshInstance.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// an instance of a mesh. Each version of each mesh that is rendered at a
// separate level of detail needs one of these.

// Each instance is tied to the original mesh that it represents.
CN3PMeshInstance::CN3PMeshInstance() {
    m_pPMesh = NULL;
#ifdef _USE_VERTEXBUFFER
    m_pIB = NULL;
#else
    m_pIndices = NULL;
#endif
    m_iNumVertices = 0;
    m_iNumIndices = 0;
    m_pCollapseUpTo = NULL;
}

CN3PMeshInstance::CN3PMeshInstance(CN3PMesh * pN3PMesh) {
    m_pPMesh = NULL;

#ifdef _USE_VERTEXBUFFER
    m_pIB = NULL;
#else
    m_pIndices = NULL;
#endif
    m_iNumVertices = 0;
    m_iNumIndices = 0;
    m_pCollapseUpTo = NULL;

    CN3PMeshInstance::Create(pN3PMesh);
}

CN3PMeshInstance::CN3PMeshInstance(const fs::path & fsFile) {
    m_pPMesh = NULL;

#ifdef _USE_VERTEXBUFFER
    m_pIB = NULL;
#else
    m_pIndices = NULL;
#endif
    m_iNumVertices = 0;
    m_iNumIndices = 0;
    m_pCollapseUpTo = NULL;

    this->Create(fsFile);
}

CN3PMeshInstance::~CN3PMeshInstance() {
#ifdef _USE_VERTEXBUFFER
    if (m_pIB && m_pIB->Release() == 0) {
        m_pIB = NULL;
    }
#else
    delete[] m_pIndices;
    m_pIndices = NULL;
#endif
    s_MngPMesh.Delete(&m_pPMesh);
}

void CN3PMeshInstance::Release() {
#ifdef _USE_VERTEXBUFFER
    if (m_pIB) {
        m_pIB->Release();
        m_pIB = NULL;
    }
#else
    if (m_pIndices) {
        delete[] m_pIndices;
        m_pIndices = NULL;
    }
#endif

    s_MngPMesh.Delete(&m_pPMesh);
    m_pCollapseUpTo = NULL;
    m_iNumVertices = 0;
    m_iNumIndices = 0;
}

bool CN3PMeshInstance::Create(CN3PMesh * pN3PMesh) {
    if (pN3PMesh == NULL) {
        CN3PMeshInstance::Release();
        return false;
    }

    m_pPMesh = pN3PMesh;
    m_szName = pN3PMesh->m_szName;

    // And setup my instance-specific data.
    // We start with the lowest level of detail.
    int iMaxNumIndices = m_pPMesh->GetMaxNumIndices();
    if (iMaxNumIndices > 0) {
#ifdef _USE_VERTEXBUFFER
        HRESULT hr = s_lpD3DDev->CreateIndexBuffer(m_pPMesh->m_iMaxNumIndices * sizeof(WORD), D3DUSAGE_DYNAMIC,
                                                   D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB);
        __ASSERT(SUCCEEDED(hr), "Failed to create index Buffer");

        BYTE *pDestByte, *pSrcByte;
        hr = m_pIB->Lock(0, 0, (VOID **)&pDestByte, 0);
        hr = m_pPMesh->m_pIB->Lock(0, 0, (VOID **)&pSrcByte, 0);

        CopyMemory(pDestByte, pSrcByte, m_pPMesh->m_iMaxNumIndices * sizeof(WORD));
        m_pPMesh->m_pIB->Unlock();
        m_pIB->Unlock();

#else
        if (m_pIndices) {
            delete[] m_pIndices;
        }
        m_pIndices = NULL;
        m_pIndices = new WORD[m_pPMesh->m_iMaxNumIndices];
        __ASSERT(m_pIndices, "Failed to create index buffer");
        CopyMemory(m_pIndices, m_pPMesh->m_pIndices, m_pPMesh->m_iMaxNumIndices * sizeof(WORD));
#endif
    }

    // lowest level of detail right now.
    m_iNumVertices = m_pPMesh->GetMinNumVertices();
    m_iNumIndices = m_pPMesh->GetMinNumIndices();

    // So far, we're at the last collapse
    m_pCollapseUpTo = m_pPMesh->m_pCollapses;

    return true;
}

bool CN3PMeshInstance::Create(const fs::path & fsFile) {
    if (m_pPMesh && m_pPMesh->FilePath() == fsFile) {
        return true; // 파일 이름이 같으면 새로 만들지 않고 리턴하자
    }
    this->Release();

    CN3PMesh * pN3PMesh = s_MngPMesh.Get(fsFile);
    return this->Create(pN3PMesh);
}

void CN3PMeshInstance::SetLODByNumVertices(int iNumVertices) {
    if (m_pCollapseUpTo == NULL) {
        return;
    }

    int iDiff = iNumVertices - m_iNumVertices;

    if (iDiff == 0) {
        return;
    } else if (iDiff > 0) {
        while (iNumVertices > m_iNumVertices) {
            if (m_pCollapseUpTo->NumVerticesToLose + m_iNumVertices > iNumVertices) {
                break; // 깜박임 방지 코드..
            }
            if (SplitOne() == false) {
                break;
            }
        }
    } else if (iDiff < 0) {
        iDiff = -iDiff;
        while (iNumVertices < m_iNumVertices) {
            if (CollapseOne() == false) {
                break;
            }
        }
    }

    while (m_pCollapseUpTo->bShouldCollapse) {
        if (SplitOne() == false) {
            break;
        }
    }
}

void CN3PMeshInstance::SetLOD(float value) {
#define _USE_LODCONTROL_VALUE
#ifdef _USE_LODCONTROL_VALUE
    // value는 distance * FOV이다.
    if (m_pPMesh == NULL) {
        return;
    }

    if (m_pPMesh->m_iLODCtrlValueCount == 0) { // LODCtrlValue가 없으면 모두 그린다.
        SetLODByNumVertices(0x7fffffff);
        return;
    }

    __ASSERT(m_pPMesh->m_pLODCtrlValues, "LOD control value is NULL!");

    CN3PMesh::__LODCtrlValue * pTmpLODCV = m_pPMesh->m_pLODCtrlValues + m_pPMesh->m_iLODCtrlValueCount - 1;

    if (value < m_pPMesh->m_pLODCtrlValues[0].fDist) { // 최소 기준치보다 가까우므로 가장 많은 면으로 그린다.
        SetLODByNumVertices(m_pPMesh->m_pLODCtrlValues[0].iNumVertices);
    } else if (pTmpLODCV->fDist < value) { // 최대 기준치보다 멀리 있으므로 가장 적은 면으로 그린다.
        SetLODByNumVertices(pTmpLODCV->iNumVertices);
    } else { // 중간 값에 맞게 조정된 면 수로 그린다.
        for (int i = 1; i < m_pPMesh->m_iLODCtrlValueCount; ++i) {
            if (value < m_pPMesh->m_pLODCtrlValues[i].fDist) {
                CN3PMesh::__LODCtrlValue * pHiValue = m_pPMesh->m_pLODCtrlValues + i;
                CN3PMesh::__LODCtrlValue * pLowValue = pHiValue - 1;
                float fVertices = (pHiValue->iNumVertices - pLowValue->iNumVertices) * (value - pLowValue->fDist) /
                                  (pHiValue->fDist - pLowValue->fDist);
                SetLODByNumVertices(pLowValue->iNumVertices + (int)fVertices);
                break;
            }
        }
    }
#else
    // value는 distance * FOV이다.
    if (m_pCollapseUpTo == NULL || m_pPMesh == NULL) {
        return;
    }

    const int          iLODCtrlValueCount = 5;
    __PMLODCtrlValue   LODCtrlValues[iLODCtrlValueCount];
    __PMLODCtrlValue * pTmpLODCV = &(LODCtrlValues[iLODCtrlValueCount - 1]);

    int iMaxNumVertices = m_pPMesh->GetMaxNumVertices();
    int iMinNumVertices = m_pPMesh->GetMinNumVertices();
    int iDiff = iMaxNumVertices - iMinNumVertices;

    float fVolume = m_pPMesh->GetVolume();
    float fD = (sqrtf(fVolume) * 3.0f) / (value * 1.0f);
    fD = 1.0f;
    //    float fD = fVolume/(value*13.0f) * (400.0f/(float)iMaxNumVertices);
    if (fD > 1.0f) {
        SetLODByNumVertices(iMaxNumVertices);
    } else if (fD < 0.1f) {
        SetLODByNumVertices(iMinNumVertices);
    } else {
        SetLODByNumVertices(iMinNumVertices + (int)(iDiff * fD));
    }

#endif
}

bool CN3PMeshInstance::CollapseOne() {
    if (m_pCollapseUpTo <= m_pPMesh->m_pCollapses) {
        return false;
    }

    m_pCollapseUpTo--;

    m_iNumIndices -= m_pCollapseUpTo->NumIndicesToLose;

#ifdef _USE_VERTEXBUFFER
    BYTE *  pByte;
    WORD *  pIndices;
    HRESULT hr = m_pIB->Lock(0, 0, (VOID **)&pByte, 0);
    pIndices = (WORD *)pByte;

    for (int * i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
         i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange; i++) {
        pIndices[*i] = m_pCollapseUpTo->CollapseTo;
    }
    m_pIB->Unlock();
#else
    for (int * i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
         i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange; i++) {
        m_pIndices[*i] = m_pCollapseUpTo->CollapseTo;
    }
#endif

    m_iNumVertices -= m_pCollapseUpTo->NumVerticesToLose;

    return true;
}

bool CN3PMeshInstance::SplitOne() {
    if (m_pCollapseUpTo >= m_pPMesh->m_pCollapses + m_pPMesh->m_iNumCollapses) {
        return false; // 이렇게 하면 포인터 하나가 삐져 나오게 된다..
    }
    // 하지만 이렇게 다시 하는 이유는 아래 코드로 하면 마지막 폴리곤이 절대 그려지지 않는다.
    // 이렇게 해도 괜찮을 수 있도록 방어코드를 넣었다. m_pPMesh->m_pCollapses 를 할당할때 1개 더 할당하고 마지막 데이터를 초기값으로 넣었다.
    //    if (m_pCollapseUpTo >= m_pPMesh->m_pCollapses + m_pPMesh->m_iNumCollapses - 1) return false; // 이게 정상이다..

    m_iNumIndices += m_pCollapseUpTo->NumIndicesToLose;
    m_iNumVertices += m_pCollapseUpTo->NumVerticesToLose;

    if (m_pPMesh->m_pAllIndexChanges) {
#ifdef _USE_VERTEXBUFFER
        BYTE * pByte;
        WORD * pIndices;
        __ASSERT(m_pIB, "Index buffer pointer is NULL!");
        HRESULT hr = m_pIB->Lock(0, 0, (VOID **)&pByte, 0);
        pIndices = (WORD *)pByte;

        for (int * i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
             i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
             i++) {
            pIndices[*i] = m_iNumVertices - 1;
        }
        m_pIB->Unlock();
#else
        for (int * i = m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges;
             i < m_pPMesh->m_pAllIndexChanges + m_pCollapseUpTo->iIndexChanges + m_pCollapseUpTo->NumIndicesToChange;
             i++) {
            m_pIndices[*i] = m_iNumVertices - 1;
        }
#endif
    }

    m_pCollapseUpTo++;
    return true;
}

void CN3PMeshInstance::Render() {
    if (m_pPMesh == NULL) {
        return;
    }
    s_lpD3DDev->SetFVF(FVF_VNT1);

    const int iPCToRender = 1000; // primitive count to render
#ifdef _USE_VERTEXBUFFER
    __ASSERT(m_pPMesh->m_pVB && m_pIB, "Progressive mesh's vertex buffer or index buffer is NULL!");
    s_lpD3DDev->SetStreamSource(0, m_pPMesh->m_pVB, 0, sizeof(__VertexT1));
    s_lpD3DDev->SetIndices(m_pIB);

    if (m_iNumIndices > 3) {
        int iPC = m_iNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3,
                                             iPCToRender);
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3, iRPC);
        }
    }
#else
    if (m_iNumIndices > 3) {
        int iPC = m_iNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iPCToRender,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices,
                                               sizeof(__VertexT1));
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iRPC,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices,
                                               sizeof(__VertexT1));
        }
    }
#endif
}

void CN3PMeshInstance::RenderTwoUV() {
    if (NULL == m_pPMesh) {
        return;
    }
    if (NULL == m_pPMesh->GetVertices2()) {
        m_pPMesh->GenerateSecondUV(); // 두번째 UV 가 없음 새로 만든다..
    }
    if (NULL == m_pPMesh->GetVertices2()) {
        return;
    }

    s_lpD3DDev->SetFVF(FVF_VNT2);

    const int iPCToRender = 1000; // primitive count to render
#ifdef _USE_VERTEXBUFFER
    __ASSERT(m_pPMesh->m_pVB && m_pIB, "Progressive mesh's vertex buffer or index buffer is NULL!");
    s_lpD3DDev->SetStreamSource(0, m_pPMesh->m_pVB, 0, sizeof(__VertexT2));
    s_lpD3DDev->SetIndices(m_pIB);

    if (m_iNumIndices > 3) {
        int iPC = m_iNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3,
                                             iPCToRender);
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3, iRPC);
        }
    }
#else
    if (m_iNumIndices > 3) {
        int iPC = m_iNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iPCToRender,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices2,
                                               sizeof(__VertexT2));
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iRPC,
                                               m_pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices2,
                                               sizeof(__VertexT2));
        }
    }
#endif
}

#ifdef _USE_VERTEXBUFFER
LPDIRECT3DVERTEXBUFFER9 CN3PMeshInstance::GetVertexBuffer() const {
    if (m_pPMesh == NULL) {
        return NULL;
    }
    return m_pPMesh->GetVertexBuffer();
}

#else
__VertexT1 * CN3PMeshInstance::GetVertices() const {
    if (m_pPMesh == NULL) {
        return NULL;
    }
    return m_pPMesh->m_pVertices;
}
#endif

//    By : Ecli666 ( On 2002-08-06 오후 4:33:04 )
//
#ifdef _USE_VERTEXBUFFER
void CN3PMeshInstance::PartialRender(int iCount, LPDIRECT3DINDEXBUFFER9 pIB) {
    if (m_pPMesh == NULL) {
        return;
    }
    s_lpD3DDev->SetFVF(FVF_VNT1);
    const int iPCToRender = 1000; // primitive count to render

    __ASSERT(m_pPMesh->m_pVB && pIB, "Progressive mesh's vertex buffer or index buffer is NULL!");
    s_lpD3DDev->SetStreamSource(0, m_pPMesh->m_pVB, 0, sizeof(__VertexT1));
    s_lpD3DDev->SetIndices(pIB);

    if (iCount > 3) {
        int iPC = iCount / 3;
        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3,
                                             iPCToRender);
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, i * iPCToRender * 3, iRPC);
        }
    }
#else
void CN3PMeshInstance::PartialRender(int iCount, WORD * pIndices) {
    if (m_pPMesh == NULL) {
        return;
    }
    s_lpD3DDev->SetFVF(FVF_VNT1);
    const int iPCToRender = 1000; // primitive count to render

    /*    if(m_iNumIndices > 3)
    {
        int iPC = m_iNumIndices / 3;

        int iLC = iPC / iPCToRender;
        int i;
        for (i=0; i<iLC; ++i)
        {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iPCToRender, m_pIndices + i*iPCToRender*3, D3DFMT_INDEX16, m_pPMesh->m_pVertices, sizeof(__VertexT1));
        }

        int iRPC = iPC%iPCToRender;
        if(iRPC > 0) s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iRPC, m_pIndices + i*iPCToRender*3, D3DFMT_INDEX16, m_pPMesh->m_pVertices, sizeof(__VertexT1));
    }
*/

    if (iCount > 3) {
        int iPC = iCount / 3;
        int iLC = iPC / iPCToRender;
        int i;
        for (i = 0; i < iLC; ++i) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iPCToRender,
                                               pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices,
                                               sizeof(__VertexT1));
        }

        int iRPC = iPC % iPCToRender;
        if (iRPC > 0) {
            s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertices, iRPC,
                                               pIndices + i * iPCToRender * 3, D3DFMT_INDEX16, m_pPMesh->m_pVertices,
                                               sizeof(__VertexT1));
        }
    }
}
#endif

    int CN3PMeshInstance::GetIndexByiOrder(int iOrder) {
        if (iOrder >= GetNumIndices()) {
            return 0;
        }

#ifdef _USE_VERTEXBUFFER
        int    iIndex = 0;
        BYTE * pByte;
        hr = m_pIB->Lock(0, 0, &pByte, 0);
        LPWORD pIndices = (LPWORD)pByte;
        iIndex = pIndices[iOrder];
        m_pIB->Unlock();
        return iIndex;
#else
    return m_pIndices[iOrder];
#endif
    }

    __Vector3 CN3PMeshInstance::GetVertexByIndex(int iIndex) {
        __Vector3 vec;
        vec.Zero();
        if (iIndex > GetNumVertices()) {
            return vec;
        }

#ifdef _USE_VERTEXBUFFER
        HRESULT      hr;
        __VertexT1 * pVB;
        hr = GetMesh()->m_pVB->Lock(0, 0, (VOID **)&pVB, 0);
        if (FAILED(hr)) {
            return vec;
        }
        vec = (__Vector3)pVB[iIndex];
        GetMesh()->m_pVB->Unlock();
        return vec;
#else
    return GetMesh()->m_pVertices[iIndex];
#endif
    }

    //    ~(By Ecli666 On 2002-08-06 오후 4:33:04 )
