// N3FXPartMesh.cpp: implementation of the CN3FXPartMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXBundle.h"
#include "N3FXShape.h"
#include "N3FXPartMesh.h"
#include "N3AnimKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXPartMesh::CN3FXPartMesh() {
    m_iVersion = 5;

    m_pShape = NULL;
    m_pRefShape = NULL;

    m_dwCurrColor = 0xffffffff;

    m_cTextureMoveDir = 0;
    m_fv = 0.0f;
    m_fu = 0.0f;

    m_vScaleVel.Set(0, 0, 0);
    m_vDir.Set(0, 0, 1);
    m_vUnitScale.Set(1, 1, 1);
    m_vCurrScaleVel.Set(0, 0, 0);
    m_vScaleAccel.Set(0, 0, 0);

    m_bTexLoop = false;
    m_fMeshFPS = 30.0f;
}

CN3FXPartMesh::~CN3FXPartMesh() {
    CN3Base::s_MngFXShape.Delete(&m_pRefShape);
    if (m_pShape) {
        m_pShape->Release();
        delete m_pShape;
        m_pShape = NULL;
    }
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

//
//
//
#ifdef _N3TOOL
bool CN3FXPartMesh::ParseScript(char * szCommand, char * szBuff0, char * szBuff1, char * szBuff2, char * szBuff3) {
    if (CN3FXPartBase::ParseScript(szCommand, szBuff0, szBuff1, szBuff2, szBuff3)) {
        return true;
    }

    if (n3std::iequals(szCommand, "<shape_name>")) {
        fs::path fsShapeFile = szBuff0;
        m_pShape = new CN3FXShape;

        m_pRefShape = s_MngFXShape.Get(fsShapeFile);
        m_pShape->Duplicate(m_pRefShape);

        __Vector3 vScale;
        if (m_pShape->m_KeyScale.DataGet(0, vScale)) {
            m_vUnitScale = vScale;
        } else {
            m_vUnitScale = m_pShape->Scale();
        }

        return true;
    }
    if (n3std::iequals(szCommand, "<texture_move>")) {
        if (n3std::iequals(szBuff0, "up")) {
            m_cTextureMoveDir = 1;
        } else if (n3std::iequals(szBuff0, "down")) {
            m_cTextureMoveDir = 2;
        } else if (n3std::iequals(szBuff0, "left")) {
            m_cTextureMoveDir = 3;
        } else if (n3std::iequals(szBuff0, "right")) {
            m_cTextureMoveDir = 4;
        }

        float uv = atof(szBuff1);

        if (m_cTextureMoveDir == 1) {
            m_fu = 0.0f;
            m_fv = uv;
        } else if (m_cTextureMoveDir == 2) {
            m_fu = 0.0f;
            m_fv = -uv;
        } else if (m_cTextureMoveDir == 3) {
            m_fu = uv;
            m_fv = 0.0f;
        } else if (m_cTextureMoveDir == 4) {
            m_fu = -uv;
            m_fv = 0.0f;
        }

        return true;
    }
    if (n3std::iequals(szCommand, "<scale_velocity>")) {
        m_vScaleVel.Set(atof(szBuff0), atof(szBuff1), atof(szBuff2));
        m_vCurrScaleVel = m_vScaleVel;
        return true;
    }
    if (n3std::iequals(szCommand, "<scale_accelerate>")) {
        m_vScaleAccel.Set(atof(szBuff0), atof(szBuff1), atof(szBuff2));
        return true;
    }
    if (n3std::iequals(szCommand, "<scale>")) {
        m_vUnitScale.Set(atof(szBuff0), atof(szBuff1), atof(szBuff2));
        return true;
    }
    if (n3std::iequals(szCommand, "<tex_fps>")) {
        m_fTexFPS = atof(szBuff0);
        if (!m_pShape) {
            return false;
        }
        for (int i = 0; i < m_pShape->PartCount(); i++) {
            m_pShape->Part(i)->m_fTexFPS = atof(szBuff0);
        }
        return true;
    }
    if (n3std::iequals(szCommand, "<tex_loop>")) {
        m_bTexLoop = true;
        if (n3std::iequals(szBuff0, "false")) {
            m_bTexLoop = false;
        }

        if (!m_pShape) {
            return false;
        }
        for (int i = 0; i < m_pShape->PartCount(); i++) {
            m_pShape->Part(i)->m_bTexLoop = m_bTexLoop;
        }
        return true;
    }
    if (n3std::iequals(szCommand, "<mesh_fps>")) {
        m_fMeshFPS = atof(szBuff0);
        return true;
    }

    if (n3std::iequals(szCommand, "<end>")) {
        Init();
        return true;
    }

    return false;
}
#endif // end of _N3TOOL

//
//    init...
//
void CN3FXPartMesh::Init() {
    CN3FXPartBase::Init();

    m_dwCurrColor = 0xffffffff;
    m_vCurrVelocity = m_vVelocity;
    m_vCurrPos = m_vPos;
    m_vCurrScaleVel = m_vScaleVel;

    m_vDir.Set(0, 0, 1);

    if (m_pRefBundle) {
        if (m_pShape) {
            m_pShape->PosSet(m_vPos + m_pRefBundle->m_vPos);
        }
    } else if (m_pShape) {
        m_pShape->PosSet(m_vPos);
    }

    if (m_pShape) {
        m_pShape->SetPartsMtl(m_bAlpha, m_dwSrcBlend, m_dwDestBlend, m_dwZEnable, m_dwZWrite, m_dwLight,
                              m_dwDoubleSide);
    }
}

//
//
//
bool CN3FXPartMesh::Load(HANDLE hFile) {
    if (!CN3FXPartBase::Load(hFile)) {
        return false;
    }

    DWORD dwRWC = 0;

    char szShapeFile[260]{};
    ReadFile(hFile, szShapeFile, sizeof(szShapeFile), &dwRWC, NULL);

    if (m_pShape) {
        delete m_pShape;
    }
    m_pShape = new CN3FXShape;

    m_pRefShape = s_MngFXShape.Get(szShapeFile);
    m_pShape->Duplicate(m_pRefShape);
    m_pShape->SetPartsMtl(m_bAlpha, m_dwSrcBlend, m_dwDestBlend, m_dwZEnable, m_dwZWrite, m_dwLight, m_dwDoubleSide);
    //m_pShape->LoadFromFile(szShapeFile);
    __Vector3 vScale;
    if (m_pShape->m_KeyScale.DataGet(0, vScale)) {
        m_vUnitScale = vScale;
    } else {
        m_vUnitScale = m_pShape->Scale();
    }

    ReadFile(hFile, &m_cTextureMoveDir, sizeof(char), &dwRWC, NULL);
    ReadFile(hFile, &m_fu, sizeof(float), &dwRWC, NULL);
    ReadFile(hFile, &m_fv, sizeof(float), &dwRWC, NULL);
    ReadFile(hFile, &m_vScaleVel, sizeof(__Vector3), &dwRWC, NULL);
    m_vCurrScaleVel = m_vScaleVel;

    if (m_iVersion >= 2) {
        ReadFile(hFile, &m_bTexLoop, sizeof(bool), &dwRWC, NULL);
    }
    if (m_iVersion >= 3) {
        ReadFile(hFile, &m_vScaleAccel, sizeof(__Vector3), &dwRWC, NULL);
    }
    if (m_iVersion >= 4) {
        ReadFile(hFile, &m_fMeshFPS, sizeof(float), &dwRWC, NULL);
    }
    if (m_iVersion >= 5) {
        ReadFile(hFile, &m_vUnitScale, sizeof(__Vector3), &dwRWC, NULL);
    }

    if (m_iVersion == 6) {
        ReadFile(hFile, &m_bShapeLoop, sizeof(bool), &dwRWC, NULL); //?<shape_loop> true <view_fix> true <usefadeshowlife> false
    }

    if (m_pShape) {
        for (int i = 0; i < m_pShape->PartCount(); i++) {
            m_pShape->Part(i)->m_fTexFPS = m_fTexFPS;
            m_pShape->Part(i)->m_bTexLoop = m_bTexLoop;
        }
    }

    Init();

    return true;
}

//
//
//
bool CN3FXPartMesh::Save(HANDLE hFile) {
    if (!CN3FXPartBase::Save(hFile)) {
        return false;
    }

    DWORD dwRWC = 0;

    char szShapeFile[260]{};
    m_pShape->FilePathWin().string().copy(szShapeFile, sizeof(szShapeFile) - 1);
    WriteFile(hFile, szShapeFile, sizeof(szShapeFile), &dwRWC, NULL);

    WriteFile(hFile, &m_cTextureMoveDir, sizeof(char), &dwRWC, NULL);
    WriteFile(hFile, &m_fu, sizeof(float), &dwRWC, NULL);
    WriteFile(hFile, &m_fv, sizeof(float), &dwRWC, NULL);

    WriteFile(hFile, &m_vScaleVel, sizeof(__Vector3), &dwRWC, NULL);

    if (m_iVersion >= 2) {
        WriteFile(hFile, &m_bTexLoop, sizeof(bool), &dwRWC, NULL);
    }
    if (m_iVersion >= 3) {
        WriteFile(hFile, &m_vScaleAccel, sizeof(__Vector3), &dwRWC, NULL);
    }
    if (m_iVersion >= 4) {
        WriteFile(hFile, &m_fMeshFPS, sizeof(float), &dwRWC, NULL);
    }
    if (m_iVersion >= 5) {
        WriteFile(hFile, &m_vUnitScale, sizeof(__Vector3), &dwRWC, NULL);
    }

    return true;
}

//
//
//
void CN3FXPartMesh::Start() {
    m_dwCurrColor = 0xffffffff;
    if (!m_pShape) {
        return;
    }

    int PartCount = m_pShape->PartCount();
    for (int i = 0; i < PartCount; i++) {
        CN3FXSPart * pPart = m_pShape->Part(i);

        pPart->m_Mtl.dwSrcBlend = m_dwSrcBlend;
        pPart->m_Mtl.dwDestBlend = m_dwDestBlend;

        if ((pPart->m_Mtl.nRenderFlags & RF_ALPHABLENDING) && !m_bAlpha) {
            pPart->m_Mtl.nRenderFlags -= RF_ALPHABLENDING;
        } else if (!(pPart->m_Mtl.nRenderFlags & RF_ALPHABLENDING) && m_bAlpha) {
            pPart->m_Mtl.nRenderFlags += RF_ALPHABLENDING;
        }

        __VertexXyzColorT1 * pVertices;
        pVertices = pPart->GetColorVertices();
        if (pVertices) {
            for (int j = 0; j < pPart->Mesh()->GetMaxNumVertices(); j++) {
                pVertices[j].color = m_dwCurrColor;
            }
        }
    }

    CN3FXPartBase::Start();
}

//
//
//
void CN3FXPartMesh::Stop() {
    CN3FXPartBase::Stop();
}

//
//
//
bool CN3FXPartMesh::Tick() {
    if (!CN3FXPartBase::Tick()) {
        return false;
    }

    if (!m_pShape) {
        return false;
    }

    if (m_fCurrLife <= m_fFadeIn) {
        DWORD Alpha = (DWORD)(255.0f * m_fCurrLife / m_fFadeIn);
        m_dwCurrColor = (Alpha << 24) + 0x00ffffff;

        int PartCount = m_pShape->PartCount();
        for (int i = 0; i < PartCount; i++) {
            CN3FXSPart * pPart = m_pShape->Part(i);
            if (pPart) {
                pPart->SetColor(m_dwCurrColor);
            }
        }
    } else if (m_dwCurrColor != 0xffffffff && m_fCurrLife < (m_fFadeIn + m_fLife)) {
        m_dwCurrColor = 0xffffffff;

        int PartCount = m_pShape->PartCount();
        for (int i = 0; i < PartCount; i++) {
            CN3FXSPart * pPart = m_pShape->Part(i);
            if (pPart) {
                pPart->SetColor(m_dwCurrColor);
            }
        }
    }

    if (m_dwState == FX_PART_STATE_DYING) {
        float TotalLife = m_fFadeIn + m_fLife + m_fFadeOut;
        if (m_fCurrLife >= TotalLife) {
            m_dwCurrColor = 0x00ffffff;
        } else {
            DWORD Alpha = (DWORD)(255.0f * (TotalLife - m_fCurrLife) / m_fFadeOut);
            m_dwCurrColor = (Alpha << 24) + 0x00ffffff;
        }

        int PartCount = m_pShape->PartCount();
        for (int i = 0; i < PartCount; i++) {
            CN3FXSPart * pPart = m_pShape->Part(i);
            if (pPart) {
                pPart->SetColor(m_dwCurrColor);
            }
        }
    }

    float fFrm = m_fCurrLife * m_fMeshFPS;
    if (fFrm > m_pShape->GetWholeFrm() - 1.0f) {
        fFrm = m_pShape->GetWholeFrm() - 1.0f;
    }

    m_pShape->SetCurrFrm(fFrm);

    if (m_cTextureMoveDir > 0) {
        MoveTexUV();
    }

    Rotate();
    Scaling();
    Move();

    m_pShape->Tick();

    return true;

    /*    
    //회전과 이동..
    __Matrix44 mtx;
    mtx.Identity();]
    mtx.Rotation(m_fCurrLife*m_vRotVelocity);
    __Quaternion qtLocalRot(mtx);
    
    //mesh방향과 bundle방향을 맞춰라...
    __Quaternion qtBundle;
    __Vector3 vDirAxis;
    float fDirAng;
    
    vDirAxis.Cross(m_vDir, m_pRefBundle->m_vDir);
    int tmp;
    tmp = vDirAxis.x*10000.0f;
    vDirAxis.x = (float)(tmp)/10000.0f;
    tmp = vDirAxis.y*10000.0f;
    vDirAxis.y = (float)(tmp)/10000.0f;
    tmp = vDirAxis.z*10000.0f;
    vDirAxis.z = (float)(tmp)/10000.0f;

    if(vDirAxis.x==0.0f && vDirAxis.y==0.0f && vDirAxis.z==0.0f) vDirAxis.Set(0,1,0);

    fDirAng = acos((double)m_vDir.Dot(m_pRefBundle->m_vDir));
    
    qtBundle.RotationAxis(vDirAxis, fDirAng);

    __Quaternion qt = qtLocalRot * qtBundle;
    m_pShape->RotSet(qt);
            
    m_vCurrVelocity += m_vAcceleration*CN3Base::s_fSecPerFrm;
    m_vCurrPos += m_vCurrVelocity*CN3Base::s_fSecPerFrm;
    m_pShape->PosSet(m_vCurrPos+m_pRefBundle->m_vPos);

    
    m_vCurrScaleVel += m_vScaleAccel*m_fCurrLife;
    
    __Vector3 vScale = m_vCurrScaleVel*m_fCurrLife;
    vScale += m_vUnitScale;
    if(m_pRefBundle->m_bDependScale)
    {
        vScale.x *= m_pRefBundle->m_vTargetScale.x;
        vScale.y *= m_pRefBundle->m_vTargetScale.y;
        vScale.z *= m_pRefBundle->m_vTargetScale.z;
    }
    if(vScale.x < 0) vScale.x = 0;
    if(vScale.y < 0) vScale.y = 0;
    if(vScale.z < 0) vScale.z = 0;

    //m_pShape->ScaleSet(m_vUnitScale.x+vScale.x, m_vUnitScale.y+vScale.y, m_vUnitScale.z+vScale.z);
    m_pShape->ScaleSet(vScale.x, vScale.y, vScale.z);

    //텍스쳐 이동..
    if(m_cTextureMoveDir>0)
    {
        int cnt = m_pShape->PartCount();
        for(int i=0;i<cnt;i++)
        {
            int VertexCount = m_pShape->Part(i)->Mesh()->GetNumVertices();

            LPDIRECT3DVERTEXBUFFER9 pVB = m_pShape->Part(i)->Mesh()->GetVertexBuffer();
            
            __VertexXyzColorT1* pVertex;
            HRESULT hr = pVB->Lock(0, 0, (VOID**)&pVertex, 0);
            if (FAILED(hr)) continue;

            for(int j=0;j<VertexCount;j++)
            {
                pVertex[j].tu += m_fu*CN3Base::s_fSecPerFrm;
                pVertex[j].tv += m_fv*CN3Base::s_fSecPerFrm;
            }
            pVB->Unlock();
        }
    }
    m_pShape->Tick(fFrm);
    return true;
*/
}

//
//
//
void CN3FXPartMesh::Rotate() {
    __Matrix44 mtx;
    mtx.Identity();
    //mtx.Rotation(m_fCurrLife*m_vRotVelocity);
    //__Quaternion qtLocalRot(mtx);

    m_pShape->m_mtxParent.Rotation(m_fCurrLife * m_vRotVelocity);

    //mesh방향과 bundle방향을 맞춰라...
    __Quaternion qtBundle;
    __Vector3    vDirAxis;
    float        fDirAng;

    vDirAxis.Cross(m_vDir, m_pRefBundle->m_vDir);
    int tmp;
    tmp = vDirAxis.x * 10000.0f;
    vDirAxis.x = (float)(tmp) / 10000.0f;
    tmp = vDirAxis.y * 10000.0f;
    vDirAxis.y = (float)(tmp) / 10000.0f;
    tmp = vDirAxis.z * 10000.0f;
    vDirAxis.z = (float)(tmp) / 10000.0f;

    if (vDirAxis.x == 0.0f && vDirAxis.y == 0.0f && vDirAxis.z == 0.0f) {
        vDirAxis.Set(0, 1, 0);
    }
    fDirAng = acos((double)m_vDir.Dot(m_pRefBundle->m_vDir));
    qtBundle.RotationAxis(vDirAxis, fDirAng);
    mtx = qtBundle;

    m_pShape->m_mtxParent *= mtx;
}

//
//
//
void CN3FXPartMesh::Move() {
    m_vCurrVelocity += m_vAcceleration * CN3Base::s_fSecPerFrm;
    m_vCurrPos += m_vCurrVelocity * CN3Base::s_fSecPerFrm;

    __Quaternion qtBundle;
    __Vector3    vDirAxis;
    float        fDirAng;

    vDirAxis.Cross(m_vDir, m_pRefBundle->m_vDir);
    int tmp;
    tmp = vDirAxis.x * 10000.0f;
    vDirAxis.x = (float)(tmp) / 10000.0f;
    tmp = vDirAxis.y * 10000.0f;
    vDirAxis.y = (float)(tmp) / 10000.0f;
    tmp = vDirAxis.z * 10000.0f;
    vDirAxis.z = (float)(tmp) / 10000.0f;

    if (vDirAxis.x == 0.0f && vDirAxis.y == 0.0f && vDirAxis.z == 0.0f) {
        vDirAxis.Set(0, 1, 0);
    }

    fDirAng = acos((double)m_vDir.Dot(m_pRefBundle->m_vDir));
    qtBundle.RotationAxis(vDirAxis, fDirAng);

    __Matrix44 mtx = qtBundle;
    __Vector3  vRealPos = m_vCurrPos * mtx;

    //__Vector3 vPos = m_vCurrPos+m_pRefBundle->m_vPos;
    __Vector3 vPos = vRealPos + m_pRefBundle->m_vPos;

    m_pShape->m_mtxParent.PosSet(vPos);
}

//
//
//
void CN3FXPartMesh::Scaling() {
    m_vCurrScaleVel += m_vScaleAccel * m_fCurrLife;
    __Vector3 vScale = m_vCurrScaleVel * m_fCurrLife;
    vScale += m_vUnitScale;

    if (m_pRefBundle->m_bDependScale) {
        vScale *= m_pRefBundle->m_fTargetScale;
    }

    if (vScale.x < 0.0f) {
        vScale.x = 0.0f;
    }
    if (vScale.y < 0.0f) {
        vScale.y = 0.0f;
    }
    if (vScale.z < 0.0f) {
        vScale.z = 0.0f;
    }

    __Matrix44 mtx;
    mtx.Identity();
    mtx.Scale(vScale);
    m_pShape->m_mtxParent *= mtx;
    //m_pShape->m_mtxParent.Scale(vScale);
}

//
//
//
void CN3FXPartMesh::MoveTexUV() {
    int cnt = m_pShape->PartCount();
    for (int i = 0; i < cnt; i++) {
        CN3FXSPart * pPart = m_pShape->Part(i);
        if (!pPart) {
            continue;
        }

        __VertexXyzColorT1 * pVertices;
        pVertices = pPart->GetColorVertices();
        if (pVertices) {
            for (int j = 0; j < pPart->Mesh()->GetMaxNumVertices(); j++) {
                pVertices[j].tu += m_fu * CN3Base::s_fSecPerFrm;
                pVertices[j].tv += m_fv * CN3Base::s_fSecPerFrm;
            }
        }
    }
}

//
//
//
int CN3FXPartMesh::NumPart() {
    if (!m_pShape) {
        return 0;
    }

    return m_pShape->PartCount();
}

//
//
//
int CN3FXPartMesh::NumVertices(int Part) {
    if (!m_pShape) {
        return 0;
    }
    //    return m_pShape->Part(Part)->Mesh()->GetNumVertices(); //this_fx
    return m_pShape->Part(Part)->Mesh()->GetMaxNumVertices();
}

//
//
//
LPDIRECT3DVERTEXBUFFER9 CN3FXPartMesh::GetVB(int Part) {
    if (!m_pShape) {
        return NULL;
    }
    return NULL;
    //    return m_pShape->Part(Part)->Mesh()->GetVertexBuffer();    //this_fx
}

//
//
//
bool CN3FXPartMesh::IsDead() {
    float TotalLife = m_fFadeIn + m_fLife + m_fFadeOut;
    if (m_fCurrLife >= TotalLife) {
        return true;
    }
    return false;
}

//
//    render...
//    일단은 파티클 하나씩 그리고....
//    나중에는 같은 텍스쳐 쓰는 것들끼리 묶어서 그리자...
//
void CN3FXPartMesh::Render() {
    // render state 세팅
    if (!m_pShape) {
        return;
    }

    DWORD dwAlpha;
    s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);

    s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, m_bAlpha);
    if (m_dwState == FX_PART_STATE_DYING || m_fCurrLife < m_fFadeIn) {
        s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    }

    s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, m_dwSrcBlend);
    s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, m_dwDestBlend);

    s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    s_lpD3DDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    m_pShape->Render();

    s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
}

void CN3FXPartMesh::Duplicate(CN3FXPartMesh * pSrc) {
    if (!pSrc) {
        return;
    }

    CN3FXPartBase::Duplicate(pSrc);
    if (m_pShape) {
        delete m_pShape;
        m_pShape = NULL;
    }

    m_pShape = new CN3FXShape;

    m_pRefShape = s_MngFXShape.Get(pSrc->m_pRefShape->FilePath());
    m_pShape->Duplicate(m_pRefShape);
    m_pShape->SetPartsMtl(m_bAlpha, m_dwSrcBlend, m_dwDestBlend, m_dwZEnable, m_dwZWrite, m_dwLight, m_dwDoubleSide);

    m_cTextureMoveDir = pSrc->m_cTextureMoveDir;
    m_fu = pSrc->m_fu;
    m_fv = pSrc->m_fv;
    m_vScaleVel = pSrc->m_vScaleVel;
    m_vScaleAccel = pSrc->m_vScaleAccel;
    m_fMeshFPS = pSrc->m_fMeshFPS;
    m_bTexLoop = pSrc->m_bTexLoop;
    m_vUnitScale = pSrc->m_vUnitScale;

    if (m_pShape) {
        for (int i = 0; i < m_pShape->PartCount(); i++) {
            m_pShape->Part(i)->m_fTexFPS = m_fTexFPS;
            m_pShape->Part(i)->m_bTexLoop = m_bTexLoop;
        }
    }

    Init();
    return;
}
