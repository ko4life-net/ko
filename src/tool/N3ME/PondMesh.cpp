// PondMesh.cpp: implementation of the CPondMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "PondMesh.h"
#include "LyTerrain.h"
#include "N3Base/N3Texture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPondMesh::CPondMesh() {
    m_pdwIndex = NULL;
    m_pTexture = NULL;

    Release();
}

CPondMesh::~CPondMesh() {
    Release();
}

void CPondMesh::Release() {
    m_iPondID = -1;
    m_fWaterHeight = 0.0f;   //    water level
    m_iWaterScaleWidth = 4;  //    Number of horizontal points
    m_iWaterScaleHeight = 6; //    Number of dots in the vertical direction
    m_iBackUpWidht = 0;
    m_iBackUpHeight = 0;
    m_dwPondAlpha = 0x88ffffff; //    alpha of water
    m_fTU = 50.0f;
    m_fTV = 50.0f;

    m_bUVState = TRUE;
    m_pTerrain = NULL;

    m_iRectVC = 0;
    ZeroMemory(m_pRectVts, 100 * sizeof(__VertexXyzT2));

    m_iVC = 0;
    m_iIC = 0;
    m_fWaterScaleX = 1.0f, m_fWaterScaleZ = 1.0f;

    ZeroMemory(m_pVertices, MAX_PONDMESH_VERTEX * sizeof(__Vector3));
    ZeroMemory(m_pViewVts, MAX_PONDMESH_VERTEX * sizeof(__VertexXyzT2));

    ClearSelectPos();

    if (m_pTexture) {
        s_MngTex.Delete(&m_pTexture);
        m_pTexture = NULL;
    }
    if (m_pdwIndex) {
        delete[] m_pdwIndex;
        m_pdwIndex = NULL;
    }
}

void CPondMesh::Render() {
    if (0 > m_iVC) {
        return;
    }

    __Matrix44 matWorld;
    matWorld.Identity();
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

    //   Draw area box
    {
        s_lpD3DDev->SetTexture(0, NULL);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        s_lpD3DDev->SetFVF(FVF_XYZCOLOR);

        s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, m_ViewRect, sizeof(__VertexXyzColor));
    }

    s_lpD3DDev->SetFVF(FVF_XYZT2);
    // Texture state
    if (m_pTexture && m_iVC > 2 && m_iIC > 0) {
        // backup state
        DWORD dwAlphaBlend, dwDestAlpha, dwSrcAlpha, dwTextureFactor;
        s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
        s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestAlpha);
        s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcAlpha);
        s_lpD3DDev->GetRenderState(D3DRS_TEXTUREFACTOR, &dwTextureFactor);
        DWORD dwAlphaOP, dwAlphaArg1;
        s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOP);
        s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);

        if ((m_dwPondAlpha & 0xff000000) != 0xff000000) // Set alpha factor
        {
            // render state settings
            s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            s_lpD3DDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwPondAlpha); // alpha factor settings
            // Texture state setting (alpha)
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
            s_lpD3DDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        }

        // set texture state(color)
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        s_lpD3DDev->SetTexture(0, m_pTexture->Get());
        s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        s_lpD3DDev->SetTexture(1, NULL);

        // render
        s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iVC, m_iIC, m_pdwIndex, D3DFMT_INDEX16,
                                           m_pViewVts //m_pVertices
                                           ,
                                           sizeof(__VertexXyzT2));

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
        s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, m_iWaterScaleWidth, m_pViewVts, sizeof(__VertexXyzT2));

        // restore
        s_lpD3DDev->SetMaterial(&BackupMtrl);
    }
}

void CPondMesh::RenderVertexPoint() // Redraw only the dots so they are visible
{
    if (m_iVC == 0 && m_iRectVC == 0) {
        return;
    }
    HRESULT hr;

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

    D3DXVECTOR4 v;
    // red dot on screen
    for (int i = 0; i <= m_iVC; ++i) {
        D3DXVec3Transform(&v, (D3DXVECTOR3 *)(&(m_pViewVts[i])), &matVP);

        float fScreenZ = (v.z / v.w);
        if (fScreenZ > 1.0 || fScreenZ < 0.0) {
            continue;
        }

        int iScreenX = int(((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f);
        int iScreenY = int((1.0f - (v.y / v.w)) * (vp.Height) / 2.0f);
        if (iScreenX >= (int)vp.X && iScreenX <= (int)vp.Width && iScreenY >= (int)vp.Y && iScreenY <= (int)vp.Height) {
            // set X (Since only 1 pixel is drawn when a dot is placed, draw an X symbol.)
            Vertices[0].Set(float(iScreenX - 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[1].Set(float(iScreenX + 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            Vertices[2].Set(float(iScreenX + 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[3].Set(float(iScreenX - 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            // render
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, Vertices, sizeof(__VertexTransformedColor));
        }
    }

    // point representing the area
    for (int i = 0; i < m_iRectVC; ++i) {
        D3DXVec3Transform(&v, (D3DXVECTOR3 *)(&(m_pRectVts[i])), &matVP);

        float fScreenZ = (v.z / v.w);
        if (fScreenZ > 1.0 || fScreenZ < 0.0) {
            continue;
        }

        int iScreenX = int(((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f);
        int iScreenY = int((1.0f - (v.y / v.w)) * (vp.Height) / 2.0f);
        if (iScreenX >= (int)vp.X && iScreenX <= (int)vp.Width && iScreenY >= (int)vp.Y && iScreenY <= (int)vp.Height) {
            // set X (Since only 1 pixel is drawn when a dot is placed, draw an X symbol.)
            Vertices[0].Set(float(iScreenX - 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[1].Set(float(iScreenX + 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            Vertices[2].Set(float(iScreenX + 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
            Vertices[3].Set(float(iScreenX - 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
            // render
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, Vertices, sizeof(__VertexTransformedColor));
        }
    }
}

void CPondMesh::SetWaterHeight(float fHeight) {
    if (m_fWaterHeight == fHeight) {
        return;
    }
    m_fWaterHeight = fHeight;

    for (int i = 0; i < 5; ++i) {
        m_vDrawBox[i].y = fHeight;
    }

    MakeDrawRect(m_vDrawBox); // Recreate the outline
    UpdateWaterHeight();      // Raise each point to the specified height
}

void CPondMesh::UpdateWaterHeight() {
    for (int i = 0; i < m_iVC; ++i) {
        m_pVertices[i].y = m_fWaterHeight;
        m_pViewVts[i].y = m_fWaterHeight;
    }
    //    ReCalcUV();
}

void CPondMesh::UpdateMovePos(__Vector3 vMovingPos) {
    for (int i = 0; i < m_iVC; ++i) {
        m_pVertices[i] += vMovingPos;
        //        m_pViewVts[i] += vMovingPos;
    }
}

void CPondMesh::MakeDrawRect(__Vector3 * p4vPos) // Create the border of the pond that will be visible on the screen
{
    if (p4vPos == NULL) {
        return;
    }

    memcpy(m_vDrawBox, p4vPos, sizeof(__Vector3) * 4);
    m_fWaterHeight = p4vPos[0].y; // Get the water level
    p4vPos = NULL;                // I'm afraid I'll get strange results from memory... ??

    //----------------------------------------------------------------------------
    DWORD color = 0xffffff00;
    m_ViewRect[0].Set(m_vDrawBox[0], color); //    upper point
    m_ViewRect[1].Set(m_vDrawBox[1], color); //    upper point
    m_ViewRect[2].Set(m_vDrawBox[2], color); //    bottom point
    m_ViewRect[3].Set(m_vDrawBox[3], color); //    bottom point
    m_ViewRect[4] = m_ViewRect[0];
    //----------------------------------------------------------------------------

    UpdateDrawPos();
}

void CPondMesh::UpdateDrawPos() {
    m_iRectVC = 0;
    // Setting the top and bottom points
    m_fWaterScaleX = SettingDrawPos(m_vDrawBox[0], m_vDrawBox[1], m_vDrawBox[3], m_vDrawBox[2], m_iWaterScaleWidth);
    // Setting left and right points
    m_fWaterScaleZ = SettingDrawPos(m_vDrawBox[0], m_vDrawBox[3], m_vDrawBox[1], m_vDrawBox[2], m_iWaterScaleHeight);
}

float CPondMesh::SettingDrawPos(__Vector3 vPos1, __Vector3 vPos2, __Vector3 vPos3, __Vector3 vPos4, int iLinePosNum) {
    if (iLinePosNum == 0) {
        return 0.0f;
    }

    __Vector3 LinePos;
    float     fLength;

    LinePos = vPos2 - vPos1;
    fLength = LinePos.Magnitude();
    fLength /= (iLinePosNum +
                1); // Find the length, calculate the points to be placed excluding the point to be placed at the end
    LinePos.Normalize();
    LinePos *= fLength; //Find a vector with a length of 1m and enter the distance between points.

    // start input
    for (int i = 1; i < iLinePosNum + 1; ++i) {
        vPos2 = vPos1 + LinePos * (float)i;
        m_pRectVts[m_iRectVC + i - 1].Set(vPos2, 0, 0, 0, 0);

        vPos4 = vPos3 + LinePos * (float)i;
        m_pRectVts[m_iRectVC + i + iLinePosNum - 1].Set(vPos4, 0, 0, 0, 0);
    }
    m_iRectVC += iLinePosNum * 2;

    return fLength;
}

void CPondMesh::MakePondPos() {
    if (m_iWaterScaleWidth * m_iWaterScaleHeight <= 0) {
        return;
    }

    if (m_pdwIndex == NULL || (m_iBackUpWidht != m_iWaterScaleWidth || m_iBackUpHeight != m_iWaterScaleHeight)) {
        delete[] m_pdwIndex;
        m_pdwIndex = new WORD[m_iWaterScaleWidth * m_iWaterScaleHeight * 6];

        m_iBackUpWidht = m_iWaterScaleWidth;
        m_iBackUpHeight = m_iWaterScaleHeight;
    }

    ASSERT(MAX_PONDMESH_VERTEX >
           m_iWaterScaleWidth * m_iWaterScaleHeight); // Check whether the number of points to be created reaches max
    int ix, iz, itemp;

    // After receiving the positions of the direction points
    float *fX, fY, *fZ;
    fX = new float[m_iWaterScaleWidth];
    for (ix = 0; ix < m_iWaterScaleWidth; ++ix) {
        fX[ix] = m_pRectVts[ix].x;
    }

    fZ = new float[m_iWaterScaleHeight];
    itemp = m_iWaterScaleWidth + m_iWaterScaleWidth;
    for (iz = 0; iz < m_iWaterScaleHeight; ++iz) {
        fZ[iz] = m_pRectVts[iz + itemp].z;
    }

    //    fY = m_pRectVts[0].y;
    fY = m_fWaterHeight;

    __Vector3 vPos;
    m_iVC = 0, m_iIC = 0; //    reset
    // First, spread the dots
    for (iz = 0; iz < m_iWaterScaleHeight; ++iz) {
        for (ix = 0; ix < m_iWaterScaleWidth; ++ix) {
            vPos.Set(fX[ix], fY, fZ[iz]);
            m_pVertices[m_iVC] = vPos;
            m_pViewVts[m_iVC].Set(vPos, 0, 0, 0, 0);
            ++m_iVC;
        }
    }

    m_iIC = (m_iWaterScaleWidth - 1) * 2 * (m_iWaterScaleHeight - 1);

    delete[] fX;
    delete[] fZ;

    //    m_iLastVertexNum = m_iWaterScaleWidth;

    MakeIndex();
    ReCalcUV();
}

BOOL CPondMesh::SetTextureName(LPCTSTR pszFName) {
    if (m_pTexture) {
        if (lstrcmpi(pszFName, m_pTexture->FileName().c_str()) == 0) {
            return TRUE;
        }
        s_MngTex.Delete(&m_pTexture);
    }
    m_pTexture = s_MngTex.Get(pszFName, TRUE);
    return m_pTexture ? TRUE : FALSE;
}

void CPondMesh::MakeIndex() {
    if (m_pdwIndex == NULL) {
        m_pdwIndex = new WORD[m_iWaterScaleWidth * m_iWaterScaleHeight * 6];
    }

    int    m = m_iWaterScaleWidth; //  next line
    int    x = 0, y = m;
    WORD * indexPtr = m_pdwIndex; // Set where to call the triangle

    --m;
    for (int j = 0; j < m_iWaterScaleHeight; j++) {
        for (int k = 0; k < m; k++) {
            indexPtr[0] = x;
            indexPtr[1] = x + 1;
            indexPtr[2] = y;
            indexPtr[3] = y;
            indexPtr[4] = x + 1;
            indexPtr[5] = y + 1;

            indexPtr += 6;
            ++x;
            ++y;
        }
        ++x;
        ++y;
    }
}

void CPondMesh::ReCalcUV() {
    if (m_iVC < 2) {
        return;
    }

    const float fTu = m_fTU, fTv = m_fTV;

    __Vector3 *     pVertices = m_pVertices;
    __VertexXyzT2 * ptmpVertices = m_pViewVts;

    //  Change to line (for x,z)
    for (int i = 0; i < m_iWaterScaleHeight; ++i) {
        ptmpVertices->tu = ptmpVertices->x / fTu;
        ptmpVertices->tv = ptmpVertices->z / fTv;
        ptmpVertices->tu2 = ptmpVertices->tu;
        ptmpVertices->tv2 = ptmpVertices->tv;

        for (int j = 0; j < m_iWaterScaleWidth; ++j) {
            (ptmpVertices + j)->tu = (ptmpVertices + j)->x / fTu;
            (ptmpVertices + j)->tv = (ptmpVertices + j)->z / fTv;
            (ptmpVertices + j)->tu2 = (ptmpVertices + j)->tu;
            (ptmpVertices + j)->tv2 = (ptmpVertices + j)->tv;
        }
        ptmpVertices += m_iWaterScaleWidth;
        pVertices += m_iWaterScaleWidth;
    }
}

void CPondMesh::ReCalcVexUV() {
    __Vector3     pBakVertices[MAX_PONDMESH_VERTEX]; //   Values used for storage and backup
    __VertexXyzT2 pBakViewVts[MAX_PONDMESH_VERTEX];  //   Value displayed on screen

    for (int i = 0; i < m_iVC; ++i) {
        pBakVertices[i].x = m_pVertices[i].x;
        pBakVertices[i].y = m_pVertices[i].y;
        pBakVertices[i].z = m_pVertices[i].z;

        pBakViewVts[i].x = m_pViewVts[i].x;
        pBakViewVts[i].y = m_pViewVts[i].y;
        pBakViewVts[i].z = m_pViewVts[i].z;
    }

    MakePondPos();

    for (int i = 0; i < m_iVC; ++i) {
        m_pVertices[i].x = pBakVertices[i].x;
        m_pVertices[i].y = pBakVertices[i].y;
        m_pVertices[i].z = pBakVertices[i].z;

        m_pViewVts[i].x = pBakViewVts[i].x;
        m_pViewVts[i].y = pBakViewVts[i].y;
        m_pViewVts[i].z = pBakViewVts[i].z;
    }
}

void CPondMesh::ClearSelectPos() {
    ZeroMemory(m_vSelectBox, sizeof(__Vector3) * 2);

    int iSize = m_vSelect.size();
    if (iSize > 0) {
        __SELECT_PO * pSel = NULL;
        it_SelVtx     it = m_vSelect.begin();
        for (int i = 0; i < iSize; i++, it++) {
            pSel = *it;
            if (pSel) {
                delete pSel;
            }
        }
        m_vSelect.clear();
    }
}

void CPondMesh::InputSelectPos(float fX, float fY, float fZ, int iVC) {
    if (m_vSelectBox[0].x == 0.0f || m_vSelectBox[0].z == 0.0f) {
        m_vSelectBox[0].x = fX, m_vSelectBox[0].y = fY, m_vSelectBox[0].z = fZ;
        m_vSelectBox[1].x = fX, m_vSelectBox[1].y = fY, m_vSelectBox[1].z = fZ;
    } else {
        // Grab the maximum area of the selected points
        if (m_vSelectBox[0].x > fX) {
            m_vSelectBox[0].x = fX;
        }
        if (m_vSelectBox[1].x < fX) {
            m_vSelectBox[1].x = fX;
        }
        if (m_vSelectBox[0].z > fZ) {
            m_vSelectBox[0].z = fZ;
        }
        if (m_vSelectBox[1].z < fZ) {
            m_vSelectBox[1].z = fZ;
        }
    }

    // Reset the height if the height is incorrect
    if (m_vSelectBox[0].y != fY) {
        SetWaterHeight(fY);
        m_vSelectBox[0].y = fY;
    }

    if (iVC > -1) // When selecting part rather than all selection
    {
        int iHeight = iVC / m_iWaterScaleWidth;
        int iWidth = iVC % m_iWaterScaleWidth;

        int           iSize = m_vSelect.size();
        it_SelVtx     it = m_vSelect.begin();
        __SELECT_PO * pSelpo = NULL;
        for (int i = 0; i < iSize; ++i, ++it) {
            pSelpo = *it;
            if (pSelpo && pSelpo->ix == iWidth && pSelpo->iz == iHeight) {
                pSelpo = NULL;
                return;
            }
        }

        // Temporarily holds the selected points
        pSelpo = new __SELECT_PO;
        pSelpo->ix = iWidth;
        pSelpo->iz = iHeight;

        m_vSelect.push_back(pSelpo);
    }
}

BOOL CPondMesh::InputDummyMovingPos(__Vector3 vDummyMovingPos, BOOL bMovePond) {
    // There is no reason to move
    if (vDummyMovingPos.x == 0.0f && vDummyMovingPos.y == 0.0f && vDummyMovingPos.z == 0.0f) {
        return FALSE;
    }
    BOOL bDrawBoxMove = FALSE;

    // Movement of selected dots area
    m_vSelectBox[0] += vDummyMovingPos;
    m_vSelectBox[1] += vDummyMovingPos;

    if (vDummyMovingPos.y != 0) {
        SetWaterHeight(m_vSelectBox[0].y); // check the height
    }

    // Pond area line processing
    if (bMovePond) {
        for (int i = 0; i < 4; ++i) {
            m_vDrawBox[i] += vDummyMovingPos;
        }
        MakeDrawRect(m_vDrawBox);       // recalculate edge points
        UpdateMovePos(vDummyMovingPos); // Recalculate calculation points for backup
    } else {
        if (m_vSelectBox[0].x < m_vDrawBox[2].x) {
            SetLeft(m_vSelectBox[0].x);
            bDrawBoxMove = TRUE;
        } else if (m_vSelectBox[1].x > m_vDrawBox[0].x) {
            SetRight(m_vSelectBox[1].x);
            bDrawBoxMove = TRUE;
        }

        if (m_vSelectBox[0].z < m_vDrawBox[0].z) {
            SetTop(m_vSelectBox[0].z);
            bDrawBoxMove = TRUE;
        } else if (m_vSelectBox[1].z > m_vDrawBox[2].z) {
            SetBottom(m_vSelectBox[1].z);
            bDrawBoxMove = TRUE;
        }

        if (vDummyMovingPos.x != 0 || vDummyMovingPos.z != 0) {
            MovingPos(); // Dots move according to the movement of the dummy
        }
    }

    return bDrawBoxMove;
}

void CPondMesh::SetLeft(float fLeft) {
    m_vDrawBox[1].x = fLeft, m_vDrawBox[2].x = fLeft;
    MakeDrawRect(m_vDrawBox);
}
void CPondMesh::SetTop(float fTop) {
    m_vDrawBox[0].z = fTop, m_vDrawBox[1].z = fTop;
    MakeDrawRect(m_vDrawBox);
}
void CPondMesh::SetRight(float fRight) {
    m_vDrawBox[0].x = fRight, m_vDrawBox[3].x = fRight;
    MakeDrawRect(m_vDrawBox);
}
void CPondMesh::SetBottom(float fBottom) {
    m_vDrawBox[2].z = fBottom, m_vDrawBox[3].z = fBottom;
    MakeDrawRect(m_vDrawBox);
}

// Basically, the selected points have already moved, so calculate the expected points to move by referring to m_pVertices, which can be considered the original.
void CPondMesh::MovingPos() {
    int           iSize = m_vSelect.size();
    it_SelVtx     it = m_vSelect.begin();
    __SELECT_PO * pSelpo = NULL;

    for (int i = 0; i < iSize; ++i, ++it) {
        pSelpo = *it;
        if (pSelpo) {
            CalcuWidth(pSelpo->ix, pSelpo->iz);
        }
    }

    ReInputBackPos();
}

void CPondMesh::CalcuWidth(int iSx, int iSy, int iEx, int iEy) {
    if (iSx < 0 || iSx > m_iWaterScaleWidth || iSy < 0 || iSy > m_iWaterScaleHeight) {
        return;
    }

    int iHeightNum = iSy * m_iWaterScaleWidth;
    //    ----------------------------------------------------------------------------------
    // Find the first point to be calculated
    __Vector3 vBakPick =
        *(m_pVertices + iHeightNum + iSx); // Coordinates before backup (reference to calculate new coordinates)
    __Vector3 vNowPick =
        *(m_pViewVts + iHeightNum + iSx); // Currently specified point (multiple points may be specified later)
    __Vector3 *     pLRVertices = m_pVertices + iHeightNum; // First point to calculate (for backup)
    __VertexXyzT2 * pLRViewVer = m_pViewVts + iHeightNum;   // First point to calculate (for current)
    //    ----------------------------------------------------------------------------------

    //    ----------------------------------------------------------------------------------
    //Has points on both ends corresponding to the location
    __Vector3 *pvLeft, *pvRight, *pvTop, *pvBottom;
    pvRight = pLRVertices; // Starting from the right
    pvLeft = pvRight + m_iWaterScaleWidth - 1;
    pvTop = m_pVertices + iSx;
    pvBottom = m_pVertices + m_iVC - m_iWaterScaleWidth + 1;
    //    ----------------------------------------------------------------------------------

    // Calculate new coordinates
    float fx1, fx2, fnx1, fnx2;
    float fny1, fny2;
    float ftemp;

    //    ----------------------------------------------------------------------------------
    int iIntervalNum = iSx - 1; // To subtract one point from each side
    if (iIntervalNum > 0) {
        ++pLRVertices, ++pLRViewVer; //    right
        fny2 = vNowPick.z - vBakPick.z;
        for (int j = 0; j < iIntervalNum; ++j, ++pLRVertices, ++pLRViewVer, ++pvTop, ++pvBottom) {
            // Find the distance ratio for each point between points and find the new location.
            fx2 = pvRight->x - vBakPick.x; // Find the ratio of left and right
            if (fx2 != 0) {
                fx1 = pvRight->x - pLRVertices->x;
                ftemp = fx1 / fx2;

                // Right end point x-backup point x: Right end point x-currently calculated point x = Right end point x-new point x: Point to be calculated x
                fnx2 = pvRight->x - vNowPick.x;
                fnx1 = fnx2 * ftemp;
                pLRViewVer->x = pvRight->x - fnx1; // Input new point of x

                // Right end point x-backup point x: Right end point x-currently calculated point x = New point z-backup point z: Change amount to be obtained z
                fny1 = fny2 * ftemp;
                pLRViewVer->z += fny1; // add change in z
            }
        }
    }
    //    ----------------------------------------------------------------------------------

    //    ----------------------------------------------------------------------------------
    iIntervalNum = m_iWaterScaleWidth - iSx - 2; // To subtract one point from each side
    if (iIntervalNum > 0) {
        ++pLRVertices, ++pLRViewVer; //    left
        fny2 = vNowPick.z - vBakPick.z;
        for (int j = 0; j < iIntervalNum; ++j, ++pLRVertices, ++pLRViewVer, ++pvTop, ++pvBottom) {
            fx2 = vBakPick.x - pvLeft->x;
            if (fx2 != 0) {
                fx1 = pLRVertices->x - pvLeft->x;
                ftemp = fx1 / fx2;

                fnx2 = vNowPick.x - pvLeft->x;
                fnx1 = fnx2 * ftemp;
                pLRViewVer->x = pvLeft->x + fnx1; // Input new point of x

                fny1 = fny2 * ftemp;
                pLRViewVer->z += fny1; // add change in z
            }
        }
    }
    //    ----------------------------------------------------------------------------------

    for (int j = 0; j < m_iWaterScaleWidth; ++j) {
        SetAllPos(j, iSy, iEx, iEy);
    }
}

void CPondMesh::SetAllPos(int iSx, int iSy, int iEx, int iEy) {
    if (iSx < 0 || iSx > m_iWaterScaleWidth || iSy < 0 || iSy > m_iWaterScaleHeight) {
        return;
    }

    // Find the first point to be calculated
    __Vector3 *     pTBVertices = m_pVertices + iSx;
    __VertexXyzT2 * pTBViewVer = m_pViewVts + iSx;

    //Has points on both ends corresponding to the location
    __Vector3 vTop, vBottom, vCenter, vNowCenter;
    vTop = *pTBVertices;
    vBottom = *(m_pVertices + m_iVC - m_iWaterScaleWidth + iSx);
    vCenter = *(m_pVertices + m_iWaterScaleWidth * iSy +
                iSx); // Currently specified point (multiple points may be specified later)
    vNowCenter = *(m_pViewVts + m_iWaterScaleWidth * iSy +
                   iSx); // Coordinates before backup (reference to calculate new coordinates)

    // Calculate new coordinates
    float fy1, fy2, fny1, fny2;
    float fnx1, fnx2;
    float ftemp;

    int iIntervalNum = iSy - 1;
    if (iIntervalNum > 0) {
        fy2 = vCenter.z - vTop.z;
        if (fy2 != 0) {
            pTBVertices += m_iWaterScaleWidth, pTBViewVer += m_iWaterScaleWidth;
            fnx2 = vNowCenter.x - vCenter.x;
            fny2 = vTop.z - vNowCenter.z;
            for (int j = 0; j < iIntervalNum;
                 ++j, pTBVertices += m_iWaterScaleWidth, pTBViewVer += m_iWaterScaleWidth) {
                fy1 = pTBVertices->z - vTop.z; // Find the ratio of left and right
                ftemp = fy1 / fy2;

                fnx1 = fnx2 * ftemp;
                pTBViewVer->x += fnx1; // add change in x

                fny1 = fny2 * ftemp;
                pTBViewVer->z = vTop.z - fny1; // Input new point of z
            }
        }
    }

    iIntervalNum = m_iWaterScaleHeight - iSy - 2;
    if (iIntervalNum > 0) {
        fy2 = vBottom.z - vCenter.z;
        if (fy2 != 0) {
            pTBVertices += m_iWaterScaleWidth, pTBViewVer += m_iWaterScaleWidth;
            fnx2 = vNowCenter.x - vCenter.x;
            fny2 = vNowCenter.z - vBottom.z;
            for (int j = 0; j < iIntervalNum;
                 ++j, pTBVertices += m_iWaterScaleWidth, pTBViewVer += m_iWaterScaleWidth) {
                fy1 = vBottom.z - pTBVertices->z;
                ftemp = fy1 / fy2;

                fnx1 = fnx2 * ftemp;
                pTBViewVer->x += fnx1; // add change in x

                fny1 = fny2 * ftemp;
                pTBViewVer->z = vBottom.z + fny1; // Input new point of z
            }
        }
    }
}

void CPondMesh::ReInputBackPos() {
    for (int i = 0; i < m_iVC; ++i) {
        m_pVertices[i].x = m_pViewVts[i].x;
        m_pVertices[i].y = m_pViewVts[i].y;
        m_pVertices[i].z = m_pViewVts[i].z;
    }
}

__Vector3 CPondMesh::GetCenter() {
    __Vector3 vCenter;
    vCenter = m_vDrawBox[2] - m_vDrawBox[0];
    float fLength = vCenter.Magnitude();
    fLength /= 2;
    vCenter.Normalize();
    vCenter = m_vDrawBox[0] + vCenter * fLength;
    return vCenter;
}

bool CPondMesh::Load1001(HANDLE hFile) {
    Release();

    DWORD dwNum;
    int   iLen;
    char  szTextueFName[_MAX_PATH];

    ReadFile(hFile, &m_iPondID, sizeof(m_iPondID), &dwNum, NULL);         //pond number
    ReadFile(hFile, &m_dwPondAlpha, sizeof(m_dwPondAlpha), &dwNum, NULL); // pond alpha
    ReadFile(hFile, &m_fWaterHeight, sizeof(m_fWaterHeight), &dwNum, NULL);
    ReadFile(hFile, &m_iWaterScaleWidth, sizeof(m_iWaterScaleWidth), &dwNum, NULL);
    ReadFile(hFile, &m_iWaterScaleHeight, sizeof(m_iWaterScaleHeight), &dwNum, NULL);
    ReadFile(hFile, &m_fTU, sizeof(m_fTU), &dwNum, NULL);
    ReadFile(hFile, &m_fTV, sizeof(m_fTV), &dwNum, NULL);
    ReadFile(hFile, &m_fWaterScaleX, sizeof(m_fWaterScaleX), &dwNum, NULL);
    ReadFile(hFile, &m_fWaterScaleZ, sizeof(m_fWaterScaleZ), &dwNum, NULL);
    ReadFile(hFile, &m_bUVState, sizeof(m_bUVState), &dwNum, NULL);

    ReadFile(hFile, m_vDrawBox, sizeof(m_vDrawBox), &dwNum, NULL); // Number of dots in one line

    ReadFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        ReadFile(hFile, m_pViewVts, m_iVC * sizeof(__VertexXyzT2), &dwNum, NULL); // vertex buffer
        ReInputBackPos();                                                         // Input new coordinates for backup
    }
    ReadFile(hFile, &m_iIC, sizeof(m_iIC), &dwNum, NULL); // IndexBuffer Count.

    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture file name length
    if (iLen > 0) {
        ReadFile(hFile, szTextueFName, iLen, &dwNum, NULL); // texture name
        szTextueFName[iLen] = NULL;
        m_pTexture = s_MngTex.Get(szTextueFName, TRUE); // load texture
    }

    MakeIndex();              // recalculate the index
    MakeDrawRect(m_vDrawBox); // Area line settings

    m_iBackUpWidht = m_iWaterScaleWidth;
    m_iBackUpHeight = m_iWaterScaleHeight;

    return 0;
}

bool CPondMesh::Load1000(HANDLE hFile) {
    Release();
    DWORD dwNum;
    int   iLen;
    char  szTextueFName[_MAX_PATH];
    float fScaleTemp;

    ReadFile(hFile, &m_iPondID, sizeof(m_iPondID), &dwNum, NULL);         //pond number
    ReadFile(hFile, &m_dwPondAlpha, sizeof(m_dwPondAlpha), &dwNum, NULL); // pond alpha

    ReadFile(hFile, &m_iWaterScaleWidth, sizeof(m_iWaterScaleWidth), &dwNum, NULL); // Number of dots in one line

    ReadFile(hFile, &fScaleTemp, sizeof(fScaleTemp), &dwNum, NULL);
    ReadFile(hFile, &fScaleTemp, sizeof(fScaleTemp), &dwNum, NULL);

    ReadFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        ReadFile(hFile, m_pVertices, m_iVC * sizeof(__Vector3), &dwNum, NULL); // vertex buffer
    }
    ReadFile(hFile, &m_iIC, sizeof(m_iIC), &dwNum, NULL); // IndexBufferCount.
    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);   // texture name length
    if (iLen > 0) {
        ReadFile(hFile, szTextueFName, iLen, &dwNum, NULL); // texture name
        szTextueFName[iLen] = NULL;
        m_pTexture = s_MngTex.Get(szTextueFName, TRUE); // load texture
    }

    //    ---------------------------------------------------------------------------------------------------------
    m_fWaterHeight = m_pVertices[0].y;
    m_iWaterScaleHeight = m_iVC / m_iWaterScaleWidth;
    m_iIC = (m_iWaterScaleWidth - 1) * 2 * (m_iWaterScaleHeight - 1);
    m_fTU = 50.0f;
    m_fTV = 50.0f;

    MakeIndex();
    ReCalcUV();
    //    ---------------------------------------------------------------------------------------------------------

    //    ---------------------------------------------------------------------------------------------------------
    for (int i = 0; i < 4; ++i) {
        m_vDrawBox[i] = m_pVertices[0];
    }
    for (int i = 0; i < m_iVC; ++i) {
        m_pViewVts[i].x = m_pVertices[i].x, m_pViewVts[i].y = m_pVertices[i].y, m_pViewVts[i].z = m_pVertices[i].z;

        if (m_vDrawBox[0].x < m_pVertices[i].x) {
            m_vDrawBox[0].x = m_pVertices[i].x, m_vDrawBox[3].x = m_pVertices[i].x;
        }
        if (m_vDrawBox[1].x > m_pVertices[i].x) {
            m_vDrawBox[1].x = m_pVertices[i].x, m_vDrawBox[2].x = m_pVertices[i].x;
        }
        if (m_vDrawBox[0].z > m_pVertices[i].z) {
            m_vDrawBox[0].z = m_pVertices[i].z, m_vDrawBox[3].z = m_pVertices[i].z;
        }
        if (m_vDrawBox[1].z < m_pVertices[i].z) {
            m_vDrawBox[1].z = m_pVertices[i].z, m_vDrawBox[2].z = m_pVertices[i].z;
        }
    }
    m_vDrawBox[0].x += 1.0f, m_vDrawBox[3].x += 1.0f, m_vDrawBox[1].x -= 1.0f, m_vDrawBox[2].x -= 1.0f;
    m_vDrawBox[0].z -= 1.0f, m_vDrawBox[3].z -= 1.0f, m_vDrawBox[1].z += 1.0f, m_vDrawBox[2].z += 1.0f;

    MakeDrawRect(m_vDrawBox); // Recreate the outline
    //    ---------------------------------------------------------------------------------------------------------

    return 0;
}

bool CPondMesh::Load(HANDLE hFile) {
    Release();
    DWORD dwNum;
    int   iLen;
    char  szTextueFName[_MAX_PATH];
    float fScaleTemp;

    ReadFile(hFile, &m_iPondID, sizeof(m_iPondID), &dwNum, NULL); //pond number

    m_dwPondAlpha = 0xddffffff;

    ReadFile(hFile, &m_iWaterScaleWidth, sizeof(int), &dwNum, NULL); // Number of dots in one line

    ReadFile(hFile, &fScaleTemp, sizeof(fScaleTemp), &dwNum, NULL);
    ReadFile(hFile, &fScaleTemp, sizeof(fScaleTemp), &dwNum, NULL);

    ReadFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        ReadFile(hFile, m_pViewVts, m_iVC * sizeof(__VertexXyzT2), &dwNum, NULL); // vertex buffer
        ReInputBackPos();                                                         // Input new coordinates for backup
    }
    ReadFile(hFile, &m_iIC, sizeof(m_iIC), &dwNum, NULL); // IndexBufferCount.
    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);   // texture name length
    if (iLen > 0) {
        ReadFile(hFile, szTextueFName, iLen, &dwNum, NULL); // texture name
        szTextueFName[iLen] = NULL;
        m_pTexture = s_MngTex.Get(szTextueFName, TRUE); // load texture
    }

    //    ---------------------------------------------------------------------------------------------------------
    m_fWaterHeight = m_pVertices[0].y;
    m_iWaterScaleHeight = m_iVC / m_iWaterScaleWidth;
    m_iIC = (m_iWaterScaleWidth - 1) * 2 * (m_iWaterScaleHeight - 1);
    m_fTU = 50.0f;
    m_fTV = 50.0f;

    MakeIndex();
    //    ---------------------------------------------------------------------------------------------------------

    //    ---------------------------------------------------------------------------------------------------------
    for (int i = 0; i < 4; ++i) {
        m_vDrawBox[i] = m_pVertices[0];
    }
    for (int i = 0; i < m_iVC; ++i) {
        if (m_vDrawBox[0].x < m_pVertices[i].x) {
            m_vDrawBox[0].x = m_pVertices[i].x, m_vDrawBox[3].x = m_pVertices[i].x;
        }
        if (m_vDrawBox[1].x > m_pVertices[i].x) {
            m_vDrawBox[1].x = m_pVertices[i].x, m_vDrawBox[2].x = m_pVertices[i].x;
        }
        if (m_vDrawBox[0].z > m_pVertices[i].z) {
            m_vDrawBox[0].z = m_pVertices[i].z, m_vDrawBox[3].z = m_pVertices[i].z;
        }
        if (m_vDrawBox[1].z < m_pVertices[i].z) {
            m_vDrawBox[1].z = m_pVertices[i].z, m_vDrawBox[2].z = m_pVertices[i].z;
        }
    }
    m_vDrawBox[0].x += 1.0f, m_vDrawBox[3].x += 1.0f, m_vDrawBox[1].x -= 1.0f, m_vDrawBox[2].x -= 1.0f;
    m_vDrawBox[0].z -= 1.0f, m_vDrawBox[3].z -= 1.0f, m_vDrawBox[1].z += 1.0f, m_vDrawBox[2].z += 1.0f;

    MakeDrawRect(m_vDrawBox); // Recreate the outline
    //    ---------------------------------------------------------------------------------------------------------

    return 0;
}

bool CPondMesh::Save(HANDLE hFile) {
    DWORD dwNum = 0;

    WriteFile(hFile, &m_iPondID, sizeof(m_iPondID), &dwNum, NULL);         //pond number
    WriteFile(hFile, &m_dwPondAlpha, sizeof(m_dwPondAlpha), &dwNum, NULL); // pond alpha
    WriteFile(hFile, &m_fWaterHeight, sizeof(m_fWaterHeight), &dwNum, NULL);
    WriteFile(hFile, &m_iWaterScaleWidth, sizeof(m_iWaterScaleWidth), &dwNum, NULL);
    WriteFile(hFile, &m_iWaterScaleHeight, sizeof(m_iWaterScaleHeight), &dwNum, NULL);
    WriteFile(hFile, &m_fTU, sizeof(m_fTU), &dwNum, NULL);
    WriteFile(hFile, &m_fTV, sizeof(m_fTV), &dwNum, NULL);
    WriteFile(hFile, &m_fWaterScaleX, sizeof(m_fWaterScaleX), &dwNum, NULL);
    WriteFile(hFile, &m_fWaterScaleZ, sizeof(m_fWaterScaleZ), &dwNum, NULL);
    WriteFile(hFile, &m_bUVState, sizeof(m_bUVState), &dwNum, NULL);

    WriteFile(hFile, m_vDrawBox, sizeof(m_vDrawBox), &dwNum, NULL); // Number of dots in one line

    WriteFile(hFile, &m_iVC, sizeof(m_iVC), &dwNum, NULL); // number of points
    if (m_iVC > 0) {
        WriteFile(hFile, m_pViewVts, m_iVC * sizeof(__VertexXyzT2), &dwNum, NULL); // vertex buffer
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

    return 0;
}
//    ==================================================================================

void CPondMesh::Rotation(__Matrix44 mRot, __Vector3 vCenter) {
    // You can back up (go back).
    //    if(m_bThisBackup==FALSE) m_bThisBackup = TRUE;

    // Rotate around the median value
    int             nCount;
    __VertexXyzT2 * pViewVts = m_pViewVts;

    __Vector3 vPos, vStPos;
    //    vStPos = pViewVts[(m_iVC+m_iLastVertexNum)/2].v;    // Find the middle position of the current pond
    vStPos = vCenter;
    for (nCount = 0; nCount < m_iVC; nCount++) {
        vPos = *pViewVts;
        vPos -= vStPos;
        vPos *= mRot;
        vPos += vStPos;
        *pViewVts = vPos;
        pViewVts++;
    }
}
/*
void CPondMesh::SetVtx()
{
    if(m_bThisBackup==FALSE) return;

    __Vector3* pVtx = m_pVertices;
    __VertexXyzT2* pViewVtx = m_pViewVts;

    float tempX = m_fWaterScaleX;
    float tempZ = m_fWaterScaleZ;

    int iNumVertex = m_iVC/m_iWaterScaleWidth;

    // Only the x-direction changes for the first line
    pVtx->Set(pViewVtx->x,pViewVtx->y,pViewVtx->z);

    for (int j=1;j<m_iWaterScaleWidth;j++)
    {    
        (pVtx+j)->x = (pVtx+j-1)->x + ((pViewVtx+j)->x-(pViewVtx+j-1)->x) / tempX;
        (pVtx+j)->y = (pViewVtx+j)->y;
        (pVtx+j)->z = (pViewVtx+j)->z;
    }
    pVtx += m_iWaterScaleWidth;
    pViewVtx += m_iWaterScaleWidth;
    
    // Convert x,z to line
    for(int i = 1 ;i<iNumVertex ; i++)
    {
        pVtx->x = (pVtx-m_iWaterScaleWidth)->x + (pViewVtx->x-(pViewVtx-m_iWaterScaleWidth)->x) / tempX;
        pVtx->y = pViewVtx->y;
        pVtx->z = (pVtx-m_iWaterScaleWidth)->z + (pViewVtx->z-(pViewVtx-m_iWaterScaleWidth)->z) / tempZ;

        for (int j=1;j<m_iWaterScaleWidth;j++)
        {    
            (pVtx+j)->x = (pVtx+j-1)->x + ((pViewVtx+j)->x-(pViewVtx+j-1)->x) / tempX;
            (pVtx+j)->y = (pViewVtx+j)->y;
            (pVtx+j)->z = (pVtx+j-m_iWaterScaleWidth)->z + ((pViewVtx+j)->z-(pViewVtx+j-m_iWaterScaleWidth)->z) / tempZ;
        }
        pVtx += m_iLastVertexNum;
        pViewVtx += m_iLastVertexNum;
    }


    // Re-adjust (to make it appear correctly on screen)
    ReCalcUV();

    m_bThisFixPosion = TRUE;
    m_bThisBackup = FALSE;
}

void CPondMesh::ReSetVtx()
{
    if(m_bThisBackup==FALSE) return;

    m_fWaterScaleX=m_fBakScaleX , m_fWaterScaleZ=m_fBakScaleZ;
    ReCalcUV();

    m_bThisFixPosion = TRUE;
    m_bThisBackup = FALSE;
}


__Vector3 CPondMesh::GetRightVct()
{
    __Vector3 reVet = m_pViewVts[1];
    reVet -= m_pViewVts[0];

    reVet.Normalize();

    reVet.x = reVet.x<0 ? -reVet.x:reVet.x;
    return reVet;
}

__Vector3 CPondMesh::GetDownVct()
{
    __Vector3 reVet = m_pViewVts[m_iLastVertexNum];
    reVet -= m_pViewVts[0];

    reVet.Normalize();
    reVet.z = reVet.z<0 ? reVet.z:-reVet.z;
    return reVet;
}
*/
/*
int CPondMesh::AddVertex(__Vector3* pvPos)
{
//    ASSERT(MAX_PONDMESH_VERTEX>m_iVC+2 && (m_iVC%2) == 0);
//    ASSERT(m_iVC<2);
    ASSERT(MAX_PONDMESH_VERTEX>m_iVC+m_iWaterScaleWidth*m_iWaterScaleHeight);    // Check whether the number of points to be created reaches max

    // Cut 4m apart from the first starting point and the second point.
    m_pVertices[m_iVC] = vPos1;
    m_pViewVts[m_iVC].Set(vPos1,0,0,0,0);

    __Vector3 LinePos = vPos2-vPos1;
    float LineLength = LinePos.Magnitude();
    int LinePosNum = (int)(LineLength/4.0f);
    if(LineLength > (float)LinePosNum*4.0f) LinePosNum++;
    ASSERT(MAX_PONDMESH_VERTEX>m_iVC+LinePosNum);

    LinePos.Normalize();
    LinePos*=4.0f;
    for(int i=1;i<LinePosNum+1;i++)
    {
        vPos2 = vPos1 + LinePos*((float)i);
        m_pVertices[m_iVC+i] = vPos2;

        vPos2 = vPos1 + LinePos*m_fWaterScaleX*((float)i);
        m_pViewVts[m_iVC+i].Set(vPos2, 0,0,0,0);
    }
    m_iLastVertexNum=LinePosNum+1;
    m_iVC += m_iLastVertexNum;

    MakeIndex();
    ReCalcUV();
    
    return m_iVC;
}

int CPondMesh::AddVertex()
{
    ASSERT(MAX_PONDMESH_VERTEX>m_iVC+m_iLastVertexNum);
    if (MAX_PONDMESH_VERTEX<=m_iVC+m_iLastVertexNum) return m_iVC;
    ASSERT(m_iVC>=m_iLastVertexNum);

    if (m_iVC<m_iLastVertexNum)
    {
        m_pVertices[m_iVC+0].Set(0,0,0);
        m_pVertices[m_iVC+1].Set(0,0,1);
        
        m_pViewVts[m_iVC+0].Set(0, 0, 0, 0, 0, 0, 0);
        m_pViewVts[m_iVC+1].Set(0, 0, 1, 0, 0, 0, 0);
        return m_iVC;
    }

    // Find the direction perpendicular to the last two existing points (taken at a distance of 4m)
    __Vector3 v1, v2, v3, vDir, vDiff, vScaleDiff;
    v1 = m_pVertices[m_iVC-2];    v2 = m_pVertices[m_iVC-1];

    v3 = v2 - v1;
    __Matrix44 mat;    mat.RotationY(D3DXToRadian(-90.0f));
    vDir = v3*mat;    vDir.Normalize();
    vDiff = vDir*4.0f;
    vScaleDiff = vDiff;
    vScaleDiff.x *= m_fWaterScaleX;
    vScaleDiff.z *= m_fWaterScaleZ;

    for(int i=0;i<m_iLastVertexNum;i++)
    {
        vDir = m_pVertices[m_iVC-m_iLastVertexNum+i];
        m_pVertices[m_iVC+i] = vDir - vDiff;

        vDir = m_pViewVts[m_iVC-m_iLastVertexNum+i];
        m_pViewVts[m_iVC+i] = vDir - vScaleDiff;
    }
    m_iVC += m_iLastVertexNum;

    // IndexBuffer Setting.    
    m_iIC += (m_iLastVertexNum-1)*2;
    ReCalcUV();

    return m_iVC;
}

int CPondMesh::DeleteVertex(int iIndex)
{
    if (iIndex<0 || iIndex>=m_iVC) return m_iVC;
    int iStart;
    iStart = (iIndex/m_iLastVertexNum);

    // Vertext Buffer delete
    for (int i=iStart*m_iLastVertexNum; i<m_iVC-m_iLastVertexNum; ++i)
    {
        m_pVertices[i] = m_pVertices[i+m_iLastVertexNum];
        m_pViewVts[i] = m_pViewVts[i+m_iLastVertexNum];
    }
    m_iVC -= m_iLastVertexNum;

    // need not indexing Index Buffer again. ^^
    m_iIC -= (m_iLastVertexNum-1)*2;

    return m_iVC;
}
*/
