// RiverMesh.cpp: implementation of the CRiverMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "RiverMesh.h"
#include "N3Base/N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRiverMesh::CRiverMesh() {
    m_pTexture = NULL;
    m_iAnimTextureCount = 0;
    m_pAnimTextures = NULL;
    Release();
}

CRiverMesh::~CRiverMesh() {
    Release();
}

void CRiverMesh::Release() {
    m_iRiverID = -1;
    m_iVC = 0;
    m_iIC = 0;
    m_fSpeed1 = m_fSpeed2 = 1.0f;
    m_fMeterPerU = 1.0f;
    m_fMeterPerV = 1.0f;
    m_fMeterPerU2 = 1.0f;
    m_fMeterPerV2 = 1.0f;
    m_fAnimTexFPS = 30.0f;
    m_dwAlphaFactor = 0xff000000;

    // Index Buffer Initialize.
    static WORD wIndex[18] = {4, 0, 1, 4, 1, 5, 5, 1, 2, 5, 2, 6, 6, 2, 3, 6, 3, 7};
    for (int i = 0; i < MAX_RIVERMESH_INDEX / 18; i++) {
        for (int j = 0; j < 18; j++) {
            m_wIndex[i * 18 + j] = wIndex[j] + i * 4;
        }
    }
    ZeroMemory(m_pVertices, sizeof(m_pVertices));

    if (m_pTexture) {
        s_MngTex.Delete(&m_pTexture);
        m_pTexture = NULL;
    }
    ReleaseAnimTextures();
}

void CRiverMesh::ReleaseAnimTextures() {
    if (m_pAnimTextures == NULL) {
        m_iAnimTextureCount = 0;
        return;
    }
    for (int i = 0; i < m_iAnimTextureCount; ++i) {
        if (m_pAnimTextures[i]) {
            s_MngTex.Delete(&(m_pAnimTextures[i]));
            m_pAnimTextures[i] = NULL;
        }
    }
    delete[] m_pAnimTextures;
    m_pAnimTextures = NULL;
    m_iAnimTextureCount = 0;
}

bool CRiverMesh::Load(HANDLE hFile) {
    Release();
    DWORD dwNum;
    int   iLen;
    char  szTextueFName[_MAX_PATH];

    ReadFile(hFile, &m_iRiverID, sizeof(m_iRiverID), &dwNum, NULL); // river number
    ReadFile(hFile, &m_fSpeed1, sizeof(m_fSpeed1), &dwNum, NULL);   // flow rate
    ReadFile(hFile, &m_fSpeed2, sizeof(m_fSpeed2), &dwNum, NULL);   // flow rate 2
    ReadFile(hFile, &m_fMeterPerV, sizeof(m_fMeterPerV), &dwNum,
             NULL); // The length of the river corresponding to U-coordinate 1.0
    ReadFile(hFile, &m_fMeterPerU, sizeof(m_fMeterPerU), &dwNum,
             NULL); // The length of the river corresponding to V coordinate 1.0
    ReadFile(hFile, &m_fMeterPerV2, sizeof(m_fMeterPerV2), &dwNum,
             NULL); // The length of the river corresponding to U2 coordinate 1.0
    ReadFile(hFile, &m_fMeterPerU2, sizeof(m_fMeterPerU2), &dwNum,
             NULL); // The length of the river corresponding to V2 coordinate 1.0
    ReadFile(hFile, &m_dwAlphaFactor, sizeof(m_dwAlphaFactor), &dwNum,
             NULL); // Alpha value to make the river transparent

    ReadFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        ReadFile(hFile, m_pVertices, m_iVC * sizeof(__VertexXyzT2), &dwNum, NULL); // vertex buffer
    }
    ReadFile(hFile, &m_iIC, sizeof(m_iIC), &dwNum, NULL); // IndexBufferCount.
    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);   // texture name length
    if (iLen > 0) {
        ReadFile(hFile, szTextueFName, iLen, &dwNum, NULL); // texture name
        szTextueFName[iLen] = NULL;
        m_pTexture = s_MngTex.Get(szTextueFName, TRUE); // load texture
    }

    // Animation Texture Data
    ReadFile(hFile, &m_fAnimTexFPS, sizeof(m_fAnimTexFPS), &dwNum, NULL);             // Anim Tex frame/sec
    ReadFile(hFile, &m_iAnimTextureCount, sizeof(m_iAnimTextureCount), &dwNum, NULL); // AnimTexture Count

    __ASSERT(m_pAnimTextures == NULL, "���� �ؽ��� �����Ͱ� NULL�̾߾� �մϴ�.");

    if (m_iAnimTextureCount > 0) {
        m_pAnimTextures = new CN3Texture *[m_iAnimTextureCount];
    }

    for (int i = 0; i < m_iAnimTextureCount; ++i) {
        ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture name length
        if (iLen <= 0) {
            m_pAnimTextures[i] = NULL;
            __ASSERT(0, "�ؽ��İ� ����");
            continue;
        }
        ReadFile(hFile, szTextueFName, iLen, &dwNum, NULL); // texture name
        szTextueFName[iLen] = NULL;
        m_pAnimTextures[i] = s_MngTex.Get(szTextueFName, TRUE); // load texture
    }
    return 0;
}

bool CRiverMesh::Save(HANDLE hFile) {
    DWORD dwNum;

    WriteFile(hFile, &m_iRiverID, sizeof(m_iRiverID), &dwNum, NULL); // river number
    WriteFile(hFile, &m_fSpeed1, sizeof(m_fSpeed1), &dwNum, NULL);   // flow rate
    WriteFile(hFile, &m_fSpeed2, sizeof(m_fSpeed2), &dwNum, NULL);   // flow rate 2
    WriteFile(hFile, &m_fMeterPerV, sizeof(m_fMeterPerV), &dwNum,
              NULL); // The length of the river corresponding to U-coordinate 1.0
    WriteFile(hFile, &m_fMeterPerU, sizeof(m_fMeterPerU), &dwNum,
              NULL); // The length of the river corresponding to V coordinate 1.0
    WriteFile(hFile, &m_fMeterPerV2, sizeof(m_fMeterPerV2), &dwNum,
              NULL); // The length of the river corresponding to U2 coordinate 1.0
    WriteFile(hFile, &m_fMeterPerU2, sizeof(m_fMeterPerU2), &dwNum,
              NULL); // The length of the river corresponding to V2 coordinate 1.0
    WriteFile(hFile, &m_dwAlphaFactor, sizeof(m_dwAlphaFactor), &dwNum,
              NULL); // Alpha value to make the river transparent

    WriteFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        WriteFile(hFile, m_pVertices, m_iVC * sizeof(__VertexXyzT2), &dwNum, NULL); // vertex buffer
    }
    WriteFile(hFile, &m_iIC, sizeof(m_iIC), &dwNum, NULL); // IndexBuffer Count.

    int iLen = 0;
    if (m_pTexture) {
        iLen = m_pTexture->FileName().size();
    }
    WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture file name length
    if (iLen > 0) {
        WriteFile(hFile, m_pTexture->FileName().c_str(), iLen, &dwNum, NULL); // texture file name
    }

    // Animation Texture Data
    WriteFile(hFile, &m_fAnimTexFPS, sizeof(m_fAnimTexFPS), &dwNum, NULL);             // Anim Tex frame/sec
    WriteFile(hFile, &m_iAnimTextureCount, sizeof(m_iAnimTextureCount), &dwNum, NULL); // AnimTexture Count

    for (int i = 0; i < m_iAnimTextureCount; ++i) {
        __ASSERT(m_pAnimTextures[i], "The river texture pointer is NULL.");
        int iLen = m_pAnimTextures[i]->FileName().size();
        WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture name length
        if (iLen > 0) {
            WriteFile(hFile, m_pAnimTextures[i]->FileName().c_str(), iLen, &dwNum, NULL); // texture name
        }
    }
    return 0;
}

void CRiverMesh::Render() {
    if (0 > m_iVC) {
        return;
    }

    __Matrix44 matWorld;
    matWorld.Identity();
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);
    s_lpD3DDev->SetFVF(FVF_XYZT2);

    // Texture state
    if (m_pTexture && m_iVC > 4) {
        // backup state
        DWORD dwAlphaBlend, dwDestAlpha, dwSrcAlpha, dwTextureFactor;
        s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
        s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestAlpha);
        s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcAlpha);
        s_lpD3DDev->GetRenderState(D3DRS_TEXTUREFACTOR, &dwTextureFactor);
        DWORD dwAlphaOP, dwAlphaArg1;
        s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOP);
        s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);

        if ((m_dwAlphaFactor & 0xff000000) != 0xff000000) // Set alpha factor
        {
            // render state settings
            s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwAlphaFactor); // alpha factor settings
            // texture state setting (alpha)
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }

        // set texture state(color)
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        s_lpD3DDev->SetTexture(0, m_pTexture->Get());
        if (m_pAnimTextures && m_pAnimTextures[0]) {
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
            s_lpD3DDev->SetTexture(1, m_pAnimTextures[0]->Get());
        } else {
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
            s_lpD3DDev->SetTexture(1, NULL);
        }

        // render
        //s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVC-2, m_pVertices, sizeof(__VertexXyzT2));
        s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iVC, m_iIC / 3, m_wIndex, D3DFMT_INDEX16,
                                           m_pVertices, sizeof(__VertexXyzT2));

        // restore
        s_lpD3DDev->SetTexture(1, NULL);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOP);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);
        s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
        s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestAlpha);
        s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcAlpha);
        s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, dwTextureFactor);
    } else {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        s_lpD3DDev->SetTexture(0, NULL);

        // backup state
        __Material BackupMtrl;
        s_lpD3DDev->GetMaterial(&BackupMtrl);

        // set material
        D3DCOLORVALUE color;
        color.a = 1.0f;
        color.r = color.g = 0.0f;
        color.b = 1.0f;
        __Material mtrl;
        mtrl.Init(color);
        s_lpD3DDev->SetMaterial(&mtrl);

        // render
        s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 3, m_pVertices, sizeof(__VertexXyzT2));

        // restore
        s_lpD3DDev->SetMaterial(&BackupMtrl);
    }
}

void CRiverMesh::RenderVertexPoint() // Redraw only the dots so they are visible
{
    if (m_iVC == 0 || m_pVertices == NULL) {
        return;
    }
    HRESULT hr;

    // backup state
    DWORD dwZEnable;
    hr = s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);

    // set state
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

    // set texture
    hr = s_lpD3DDev->SetTexture(0, NULL);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    // transform
    __Matrix44 matView, matProj, matVP;
    s_lpD3DDev->GetTransform(D3DTS_VIEW, &matView);
    s_lpD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixMultiply(&matVP, &matView, &matProj);
    D3DVIEWPORT9 vp = s_CameraData.vp;

    __VertexTransformedColor Vertices[4];
    D3DCOLOR                 clr = D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00);
    s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);

    for (int i = 0; i < m_iVC; ++i) {
        D3DXVECTOR4 v;
        D3DXVec3Transform(&v, &(m_pVertices[i]), &matVP);

        float fScreenZ = (v.z / v.w);
        if (fScreenZ > 1.0 || fScreenZ < 0.0) {
            continue;
        }

        int iScreenX = int(((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f);
        int iScreenY = int((1.0f - (v.y / v.w)) * (vp.Height) / 2.0f);
        if (iScreenX >= (int)vp.X && iScreenX <= (int)vp.Width && iScreenY >= (int)vp.Y && iScreenY <= (int)vp.Height) {
            // set X (Since only 1 pixel is drawn when a dot is placed, draw an X mark.)
            Vertices[0].Set(float(iScreenX - 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[1].Set(float(iScreenX + 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            Vertices[2].Set(float(iScreenX + 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[3].Set(float(iScreenX - 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            // render
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, Vertices, sizeof(__VertexTransformedColor));
        }
    }

    // restore
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
}

int CRiverMesh::AddVertex(__Vector3 & vPos1, __Vector3 & vPos2, __Vector3 & vPos3, __Vector3 & vPos4) {
    ASSERT(MAX_RIVERMESH_VERTEX > m_iVC + 4 && (m_iVC % 4) == 0);
    //if (MAX_RIVERMESH_VERTEX<=m_iVC+4) return m_iVC;
    ASSERT(m_iVC < 4);
    m_pVertices[m_iVC + 0].Set(vPos1, 0, 0, 0, 0);
    m_pVertices[m_iVC + 1].Set(vPos2, 0, 0, 0, 0);
    m_pVertices[m_iVC + 2].Set(vPos3, 0, 0, 0, 0);
    m_pVertices[m_iVC + 3].Set(vPos4, 0, 0, 0, 0);
    m_iVC += 4;
    ReCalcUV();

    return m_iVC;
}

int CRiverMesh::AddVertex() {
    ASSERT(MAX_RIVERMESH_VERTEX > m_iVC + 4 && (m_iVC % 4) == 0);
    if (MAX_RIVERMESH_VERTEX <= m_iVC + 4) {
        return m_iVC;
    }
    ASSERT(m_iVC >= 4);

    if (m_iVC < 4) {
        m_pVertices[m_iVC + 0].Set(0, 0, 0, 0, 0, 0, 0);
        m_pVertices[m_iVC + 1].Set(0, 0, 1, 0, 0, 0, 0);
        m_pVertices[m_iVC + 2].Set(0, 0, 2, 0, 0, 0, 0);
        m_pVertices[m_iVC + 3].Set(0, 0, 3, 0, 0, 0, 0);
        return m_iVC;
    }

    // Find the direction perpendicular to the last two existing points (offset value varies depending on the distance from the camera.)
    __Vector3 v1, v2, v3, vDir, vDiff, v4, v5;
    v1 = m_pVertices[m_iVC - 4];
    v2 = m_pVertices[m_iVC - 3];
    v3 = m_pVertices[m_iVC - 2];
    v4 = m_pVertices[m_iVC - 1];

    v5 = v4 - v1;
    __Matrix44 mat;
    mat.RotationY(D3DXToRadian(-90.0f));
    vDir = v5 * mat;
    vDir.Normalize();
    vDiff = vDir * (s_CameraData.vEye - (v1 + (v5 / 2))).Magnitude() * 0.1f;

    m_pVertices[m_iVC + 0] = v1 - vDiff;
    m_pVertices[m_iVC + 1] = v2 - vDiff;
    m_pVertices[m_iVC + 2] = v3 - vDiff;
    m_pVertices[m_iVC + 3] = v4 - vDiff;
    m_iVC += 4;

    // IndexBuffer Setting.
    m_iIC += INDEXBUFFER_PERLINE;
    ReCalcUV();

    return m_iVC;
}

int CRiverMesh::DeleteVertex(int iIndex) {
    if (iIndex < 0 || iIndex >= m_iVC) {
        return m_iVC;
    }
    int iStart = (iIndex / 4);

    // Vertext Buffer delete
    for (int i = iStart * 4; i < m_iVC - 4; ++i) {
        m_pVertices[i] = m_pVertices[i + 4];
    }
    m_iVC -= 4;

    // need not indexing Index Buffer again. ^^
    m_iIC -= INDEXBUFFER_PERLINE;

    return m_iVC;
}

BOOL CRiverMesh::SetTextureName(LPCTSTR pszFName) {
    if (m_pTexture) {
        if (lstrcmpi(pszFName, m_pTexture->FileName().c_str()) == 0) {
            return TRUE;
        }
        s_MngTex.Delete(&m_pTexture);
    }
    m_pTexture = s_MngTex.Get(pszFName, TRUE);
    return m_pTexture ? TRUE : FALSE;
}

BOOL CRiverMesh::SetAnimTextureName(LPCTSTR pszFName, LPCTSTR pszExt, int iCount) {
    ReleaseAnimTextures();
    if (lstrlen(pszFName) == 0 || iCount <= 0) {
        return FALSE;
    }
    __ASSERT(iCount < 100, "There are too many river water animation textures.");
    m_iAnimTextureCount = iCount;
    m_pAnimTextures = new CN3Texture *[m_iAnimTextureCount];

    char szTemp[_MAX_PATH];
    for (int i = 0; i < m_iAnimTextureCount; ++i) {
        wsprintf(szTemp, "%s%02d%s", pszFName, i, pszExt);
        m_pAnimTextures[i] = s_MngTex.Get(szTemp);
    }
    return TRUE;
}

void CRiverMesh::ReCalcUV() {
    if (m_iVC < 4) {
        return;
    }

    float x = 0.0f;
    float y = 0.0f;
    for (int i = 0; i < m_iVC / 4; i++) {
        for (int j = 0; j < 4; j++) {
            m_pVertices[i * 4 + j].tu = x;
            m_pVertices[i * 4 + j].tv = y;
            m_pVertices[i * 4 + j].tu2 = x;
            m_pVertices[i * 4 + j].tv2 = y;
            x += 1.0f;
        }
        y += 1.0f;
        x = 0.0f;
    }

    /*
    int iCount = m_iVC/2;
    float fUPerMeter = 1.0f/m_fMeterPerU;
    float fVPerMeter = 1.0f/m_fMeterPerV;
    float fUPerMeter2 = 1.0f/m_fMeterPerU2;
    float fVPerMeter2 = 1.0f/m_fMeterPerV2;

    // set the first two points
    __Vector3 vDiff; float fDiff;
    vDiff = (__Vector3)m_pVertices[0].v - (__Vector3)m_pVertices[1].v;
    fDiff = vDiff.Magnitude()*fUPerMeter/2;
    m_pVertices[0].tu = 0.5f - fDiff;    m_pVertices[1].tu = 0.5f + fDiff;
    m_pVertices[0].tv = 0.0f;            m_pVertices[1].tv = 0.0f;
    fDiff = vDiff.Magnitude()*fUPerMeter2/2;
    m_pVertices[0].tu2 = 0.5f - fDiff;    m_pVertices[1].tu2 = 0.5f + fDiff;
    m_pVertices[0].tv2 = 0.0f;            m_pVertices[1].tv2 = 0.0f;

    // Calculate the remaining points
    for (int i=1; i<iCount; ++i)
    {
        // U
        vDiff = (__Vector3)m_pVertices[i*2+0].v - (__Vector3)m_pVertices[i*2+1].v;
        fDiff = vDiff.Magnitude()*fUPerMeter/2;
        m_pVertices[i*2+0].tu = 0.5f - fDiff;    m_pVertices[i*2+1].tu = 0.5f + fDiff;
        // U2
        fDiff = vDiff.Magnitude()*fUPerMeter2/2;
        m_pVertices[i*2+0].tu2 = 0.5f - fDiff;    m_pVertices[i*2+1].tu2 = 0.5f + fDiff;

        // V
        vDiff = (__Vector3)m_pVertices[i*2+0].v - (__Vector3)m_pVertices[i*2-2].v;
        float fTmp = vDiff.Magnitude();
        vDiff = (__Vector3)m_pVertices[i*2+1].v - (__Vector3)m_pVertices[i*2-1].v;
        fDiff = (fTmp + vDiff.Magnitude())*fVPerMeter/2;
        m_pVertices[i*2+0].tv = m_pVertices[i*2+1].tv = m_pVertices[i*2-2].tv + fDiff;

        // V2
        fDiff = (fTmp + vDiff.Magnitude())*fVPerMeter2/2;
        m_pVertices[i*2+0].tv2 = m_pVertices[i*2+1].tv2 = m_pVertices[i*2-2].tv2 + fDiff;
    }
*/
}

__Vector3 CRiverMesh::GetCenter() {
    int       iCenter = m_iVC >> 1;
    __Vector3 vCenter;

    vCenter.x = m_pVertices[iCenter].x;
    vCenter.y = m_pVertices[iCenter].y;
    vCenter.z = m_pVertices[iCenter].z;

    return vCenter;
}