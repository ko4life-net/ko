// N3Shape.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3Shape.h"

/////////////////////////////
// CN3Shape Part ....
CN3SPart::CN3SPart() {
    m_dwType |= OBJ_SHAPE_PART;

    m_vPivot.Set(0, 0, 0);     // Local axis
    m_Matrix.Identity();        // World Matrix.. It is best to calculate it in advance when Shape Loading.
    m_bOutOfCameraRange = TRUE; // Outside Camera range...
    m_Mtl.Init();               //Material
    m_fTexFPS = 10.0f;          // Texture Animation Interval;
    m_fTexIndex = 0;            // Current Texture Index.. This is the index needed for Animation..

    //    m_vWindFactorCur.Zero();        // Current wind blowing value.. Rotate with this value..
    //    m_vWindFactorToReach.Zero();    // Windy value...
    m_fTimeToSetWind = 0;     // Time to change the wind blowing value...
    m_fWindFactorCur = 0;     // Current wind blowing value.. Rotate with this value..
    m_fWindFactorToReach = 0; // Windy value...
}

CN3SPart::~CN3SPart() {
    int iTC = m_TexRefs.size();
    for (int i = 0; i < iTC; i++) {
        s_MngTex.Delete(&m_TexRefs[i]);
    }
}

void CN3SPart::Release() {
    m_vPivot.Set(0, 0, 0);      // Local axis
    m_Matrix.Identity();        // World Matrix.. It is best to calculate it in advance when Shape Loading.
    m_bOutOfCameraRange = TRUE; // Outside Camera range...
    m_Mtl.Init();               //Material
    m_fTexFPS = 10.0f;          // Texture Animation Interval;
    m_fTexIndex = 0;            // Current Texture Index.. This is the index needed for Animation..

    //    m_vWindFactorCur.Zero();        // Current wind blowing value.. Rotate with this value..
    //    m_vWindFactorToReach.Zero();    // Windy value...
    m_fTimeToSetWind = 0;     // Time to change the wind blowing value...
    m_fWindFactorCur = 0;     // Current wind blowing value.. Rotate with this value..
    m_fWindFactorToReach = 0; // Windy value...

    int iTC = m_TexRefs.size();
    for (int i = 0; i < iTC; i++) {
        s_MngTex.Delete(&m_TexRefs[i]);
    }
    m_TexRefs.clear();
    m_PMInst.Release();
}

void CN3SPart::MeshSet(const std::string & szFN) {
    m_PMInst.Create(szFN);
}

void CN3SPart::TexAlloc(int nCount) {
    if (nCount <= 0) {
        return;
    }

    int iTC = m_TexRefs.size();
    for (int i = 0; i < iTC; i++) {
        s_MngTex.Delete(&m_TexRefs[i]);
    }
    m_TexRefs.clear();

    m_TexRefs.assign(nCount, NULL);
}

void CN3SPart::Tick(const __Matrix44 & mtxParent, const __Quaternion & qRot,
                    float fScale) // Entering the value obtained with timeGetTime controls Texture Animation.
{
    CN3PMesh * pPMesh = m_PMInst.GetMesh();
    if (NULL == pPMesh) {
        return;
    }

    m_bOutOfCameraRange = FALSE;

    // If you get too far away from the camera, it will pass by.
    __Vector3 vCenter = (this->Min() + this->Max()) * 0.5f;
    if (s_CameraData.IsOutOfFrustum(vCenter, this->Radius() * fScale)) // If it is outside the camera tetrahedron, it passes.
    {
        m_bOutOfCameraRange = TRUE;
        return;
    }

    float fDist = (vCenter - s_CameraData.vEye).Magnitude();
    float fLOD = fDist * s_CameraData.fFOV / fScale;
    //    float fLOD = fDist + fDist * (s_CameraData.fFOV - 1.0f) / 3.0f;
    //    float fLOD = fDist * s_CameraData.fFOV * (512.0f / s_CameraData.fFP);

    // Adjust the LOD level according to the camera distance.
    //    fLOD *= 256.0f / s_CameraData.fFP;
    m_PMInst.SetLOD(fLOD);

    int iTC = m_TexRefs.size();
    if (iTC > 1) // texture animation
    {
        m_fTexIndex += CN3Base::s_fSecPerFrm * m_fTexFPS;
        if (m_fTexIndex >= iTC) {
            m_fTexIndex -= (iTC * m_fTexIndex) / iTC; // If you divide by an integer, only the decimal point is left?? (It's similar anyway~)
        }
    }

    if (m_Mtl.nRenderFlags & RF_BOARD_Y) // If you have to look at the camera...
    {
        __Vector3 vPos = m_vPivot * mtxParent;
        __Vector3 vDir = s_CameraData.vEye - vPos;
        if (vDir.x > 0.0f) {
            m_Matrix.RotationY(-atanf(vDir.z / vDir.x) - (D3DX_PI * 0.5f));
        } else {
            m_Matrix.RotationY(-atanf(vDir.z / vDir.x) + (D3DX_PI * 0.5f));
        }

        // Rotates in the opposite direction to the parent rotation.
        float fAngle;
        qRot.AxisAngle(vDir, fAngle);
        if (fAngle != 0) {
            __Quaternion qRotI;
            qRotI.RotationAxis(vDir, -fAngle);
            m_Matrix *= qRotI;
        }

        m_Matrix *= mtxParent;
        m_Matrix.PosSet(vPos);
    }

    if (m_Mtl.nRenderFlags & RF_WINDY) // If you have to blow it slightly in the wind...
    {
        m_fTimeToSetWind -= CN3Base::s_fSecPerFrm;

        if (m_fTimeToSetWind <= 0) {
            //            m_vWindFactorToReach.x = 0.05f - (0.1f * (rand()%100) / 100.0f);
            //            m_vWindFactorToReach.y = 0.05f - (0.1f * (rand()%100) / 100.0f); // Let it blow slightly up and down.
            //            m_vWindFactorToReach.z = 0.05f - (0.1f * (rand()%100) / 100.0f);

            m_fWindFactorToReach = (rand() % 100) / 100.0f;
            m_fTimeToSetWind = 3.0f * ((rand() % 100) / 100.0f); // The value at which the wind will last..
        } else if (m_fWindFactorToReach != m_fWindFactorCur)
        //        else if(m_vWindFactorToReach != m_vWindFactorCur)
        {
            //            float fFactor = s_fSecPerFrm * (m_vWindFactorToReach - m_vWindFactorCur).Magnitude();
            float fFactor = s_fSecPerFrm * T_Abs(m_fWindFactorToReach - m_fWindFactorCur);

            //            if(m_vWindFactorCur.x < m_vWindFactorToReach.x) m_vWindFactorCur.x += fFactor;
            //            if(m_vWindFactorCur.x > m_vWindFactorToReach.x) m_vWindFactorCur.x -= fFactor;
            //            if(m_vWindFactorCur.y < m_vWindFactorToReach.y) m_vWindFactorCur.y += fFactor;
            //            if(m_vWindFactorCur.y > m_vWindFactorToReach.y) m_vWindFactorCur.y -= fFactor;
            //            if(m_vWindFactorCur.z < m_vWindFactorToReach.z) m_vWindFactorCur.z += fFactor;
            //            if(m_vWindFactorCur.z > m_vWindFactorToReach.z) m_vWindFactorCur.z -= fFactor;
            if (m_fWindFactorCur < m_fWindFactorToReach) {
                m_fWindFactorCur += fFactor;
            }
            if (m_fWindFactorCur > m_fWindFactorToReach) {
                m_fWindFactorCur -= fFactor;
            }

            //            if(T_Abs(m_vWindFactorToReach.x - m_vWindFactorCur.x) < fFactor) m_vWindFactorCur.x = m_vWindFactorToReach.x;
            //            if(T_Abs(m_vWindFactorToReach.y - m_vWindFactorCur.y) < fFactor) m_vWindFactorCur.y = m_vWindFactorToReach.y;
            //            if(T_Abs(m_vWindFactorToReach.z - m_vWindFactorCur.z) < fFactor) m_vWindFactorCur.z = m_vWindFactorToReach.z;
            if (T_Abs(m_fWindFactorToReach - m_fWindFactorCur) < fFactor) {
                m_fWindFactorCur = m_fWindFactorToReach;
            }

            __Vector3 vPos = m_vPivot * mtxParent;
            //            m_Matrix.Rotation(CN3Base::s_vWindFactor * m_fWindFactorCur);
            m_Matrix.Rotation(__Vector3(0.05f, 0.02f, 0.05f) * m_fWindFactorCur);
            m_Matrix *= mtxParent;
            m_Matrix.PosSet(vPos);
        }
    }
}

void CN3SPart::Render() {
    if (m_bOutOfCameraRange || m_PMInst.GetNumVertices() <= 0) {
        return;
    }

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Part++; // Rendering Information Update...
#endif

    LPDIRECT3DTEXTURE9 lpTex = NULL;
    int                iTC = m_TexRefs.size();
    if (iTC > 0) {
        int iTexIndex = (int)m_fTexIndex;
        if (iTexIndex >= 0 && iTexIndex < iTC && m_TexRefs[iTexIndex]) {
            lpTex = m_TexRefs[iTexIndex]->Get();
        }
    }

    if (m_Mtl.nRenderFlags & RF_ALPHABLENDING) // use Alpha
    {
        __AlphaPrimitive * pAP = s_AlphaMgr.Add();
        if (pAP) {
            pAP->bUseVB = FALSE;
            pAP->dwBlendDest = m_Mtl.dwDestBlend;
            pAP->dwBlendSrc = m_Mtl.dwSrcBlend;
            pAP->dwFVF = FVF_VNT1;
            pAP->dwPrimitiveSize = sizeof(__VertexT1);
            pAP->fCameraDistance = (s_CameraData.vEye - m_Matrix.Pos()).Magnitude();
            pAP->lpTex = lpTex;
            pAP->ePrimitiveType = D3DPT_TRIANGLELIST;
            pAP->nPrimitiveCount = m_PMInst.GetNumIndices() / 3;
            pAP->nRenderFlags = m_Mtl.nRenderFlags;
            pAP->nVertexCount = m_PMInst.GetNumVertices();
            pAP->MtxWorld = m_Matrix;
            pAP->pVertices = m_PMInst.GetVertices();
            pAP->pwIndices = m_PMInst.GetIndices();
        }

        return; // No rendering.
    }

    //    static DWORD dwAlpha, dwFog, dwCull;
    static DWORD dwFog, dwCull;
    //    s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
    //    if(TRUE != dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //    s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   m_Mtl.dwSrcBlend);
    //    s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  m_Mtl.dwDestBlend);

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Polygon += m_PMInst.GetNumIndices() / 3; // Rendering Information Update...
#endif

    if (m_Mtl.nRenderFlags & RF_NOTUSEFOG) // Ignore Fog...
    {
        s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
        if (TRUE == dwFog) {
            s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
        }
    }
    if (m_Mtl.nRenderFlags & RF_DOUBLESIDED) // Render Flags -
    {
        s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    }

    s_lpD3DDev->SetMaterial(&m_Mtl); // Material settings..
    s_lpD3DDev->SetTexture(0, lpTex);
    if (NULL != lpTex) {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, m_Mtl.dwColorOp);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, m_Mtl.dwColorArg1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, m_Mtl.dwColorArg2);
    } else {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }

    // When loading, apply the pre-calculated world matrix...
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

    m_PMInst.Render();

    //    if((m_Mtl.nRenderFlags & RF_ALPHABLENDING) && FALSE == dwAlpha)    s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    if ((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) {
        s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // Do not use fog..
    }
    if ((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) {
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
    }
}

#ifdef _N3TOOL
void CN3SPart::RenderSelected(bool bWireFrame) {
    if (m_bOutOfCameraRange || m_PMInst.GetNumVertices() <= 0) {
        return;
    }

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Part++; // Rendering Information Update...
    CN3Base::s_RenderInfo.nShape_Polygon += m_PMInst.GetNumIndices() / 3;
#endif _DEBUG

    LPDIRECT3DTEXTURE9 lpTex = NULL;
    int                iTC = m_TexRefs.size();
    if (iTC > 0) {
        int iTexIndex = (int)m_fTexIndex;
        if (iTexIndex >= 0 && iTexIndex < iTC && m_TexRefs[iTexIndex]) {
            lpTex = m_TexRefs[iTexIndex]->Get();
        }
    }

    s_lpD3DDev->SetTexture(0, lpTex);
    s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    if (lpTex != NULL) {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    } else {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }

    s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);
    if (bWireFrame) {
        __Vector3 vLines[4];

        int          nFC = m_PMInst.GetNumIndices() / 3;
        WORD *       pwIndices = m_PMInst.GetIndices();
        __VertexT1 * pVs = m_PMInst.GetVertices();

        for (int i = 0; i < nFC; i++) {
            vLines[0] = pVs[pwIndices[i * 3 + 0]];
            vLines[1] = pVs[pwIndices[i * 3 + 1]];
            vLines[2] = pVs[pwIndices[i * 3 + 2]];
            vLines[3] = vLines[0];
            CN3Base::RenderLines(vLines, 3, 0xff00ff00);
        }
    } else {
        m_PMInst.Render();
    }
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
void CN3SPart::RenderAxis() {
    // Draw axis...
    float fRadius = 1.0f;
    if (m_PMInst.GetMesh()) {
        fRadius = m_PMInst.GetMesh()->Radius();
    }
    static CN3Transform TTmp;
    TTmp.m_Matrix = m_Matrix;
    TTmp.Render(&m_Matrix, fRadius * 1.5f);
}
#endif // end of _N3TOOL

bool CN3SPart::Load(HANDLE hFile) {
    DWORD dwRWC;
    int   nL = 0;
    char  szFN[256];

    ReadFile(hFile, &m_vPivot, sizeof(__Vector3), &dwRWC, NULL);

    ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
    ReadFile(hFile, szFN, nL, &dwRWC, NULL);
    szFN[nL] = NULL; // Mesh file name..
    this->MeshSet(szFN);

    ReadFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // texture

    int iTC = 0;
    ReadFile(hFile, &iTC, 4, &dwRWC, NULL);
    ReadFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
    m_TexRefs.clear();
    this->TexAlloc(iTC);          // Texture Pointer Pointer allocation..
    for (int j = 0; j < iTC; j++) // Read the file name as much as Texture Count and load the texture.
    {
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL > 0) {
            ReadFile(hFile, szFN, nL, &dwRWC, NULL);
            szFN[nL] = NULL; //Texture file name..
            m_TexRefs[j] = s_MngTex.Get(szFN, true, s_Options.iTexLOD_Shape);
        }
    }

    return true;
}

#ifdef _N3TOOL
bool CN3SPart::Save(HANDLE hFile) {
    DWORD dwRWC;

    WriteFile(hFile, &m_vPivot, sizeof(__Vector3), &dwRWC, NULL);

    CN3PMesh * pPMesh = m_PMInst.GetMesh();
    __ASSERT(pPMesh, "Progressive mesh pointer is NULL!");
    int nL = 0;
    if (pPMesh) {
        nL = pPMesh->FileName().size();
    } else {
        MessageBox(GetActiveWindow(),
                   "Progressive mesh pointer is NULL! : The object may not be displayed properly (the resource file may not be displayed properly). "
                   "It is highly likely that it has not been loaded.)",
                   "warning", MB_OK);
    }

    WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
    if (nL > 0) {
        //        if(-1 == pPMesh->FileName().find("object\\")) // I put it in to change the path temporarily. I will delete it later if I don't need it.
        //        {
        //            char szFNTmp[256];
        //            wsprintf(szFNTmp, "Object\\%s.N3PMesh", pPMesh->Name());
        //            pPMesh->FileNameSet(szFNTmp);
        //
        //            SetFilePointer(hFile, -4, 0, FILE_CURRENT);
        //            nL = pPMesh->FileName().size();
        //            WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
        //        }

        WriteFile(hFile, pPMesh->FileName().c_str(), nL, &dwRWC, NULL); // Mesh file name..
    }

    WriteFile(hFile, &m_Mtl, sizeof(__Material), &dwRWC, NULL); // texture

    int iTC = m_TexRefs.size();
    WriteFile(hFile, &iTC, 4, &dwRWC, NULL);
    WriteFile(hFile, &m_fTexFPS, 4, &dwRWC, NULL);
    for (int j = 0; j < iTC; j++)// Write Texture File name...
    {
        if (m_TexRefs[j]) {
            nL = m_TexRefs[j]->FileName().size();
        } else {
            nL = 0;
        }

        WriteFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL > 0) {
            //            if(-1 == m_TexRefs[j]->FileName().find("object\\")) // I put it in to change the path temporarily. I will delete it later if I don't need it.
            //            {
            //                // Separate the folder name...
            //                char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
            //                _splitpath(m_TexRefs[j]->FileName(), szDrive, szDir, szFName, szExt);
            //
            //                char szFNTmp[256];
            //                wsprintf(szFNTmp, "Object\\%s.DXT", szFName);
            //                m_TexRefs[j]->FileNameSet(szFNTmp);
            //
            //                SetFilePointer(hFile, -4, 0, FILE_CURRENT);
            //                nL = lstrlen(m_TexRefs[j]->FileName());
            //                WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
            //            }

            WriteFile(hFile, m_TexRefs[j]->FileName().c_str(), nL, &dwRWC, NULL); //Texture file name..
        }
    }

    return true;
}
#endif // end of _N3TOOL

#ifdef _USE_VERTEXBUFFER
void CN3SPart::PartialRender(int iCount, LPDIRECT3DINDEXBUFFER9 pIB) {
    if (m_bOutOfCameraRange || m_PMInst.GetNumVertices() <= 0) {
        return;
    }

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Part++; // Rendering Information Update...
#endif

    LPDIRECT3DTEXTURE9 lpTex = NULL;
    int                iTC = m_TexRefs.size();
    if (iTC > 0) {
        int iTexIndex = (int)m_fTexIndex;
        if (iTexIndex >= 0 && iTexIndex < iTC && m_TexRefs[iTexIndex]) {
            lpTex = m_TexRefs[iTexIndex]->Get();
        }
    }

    if (m_Mtl.nRenderFlags & RF_ALPHABLENDING) // use Alpha
    {
        __AlphaPrimitive * pAP = s_AlphaMgr.Add();
        if (pAP) {
            pAP->bUseVB = FALSE;
            pAP->dwBlendDest = m_Mtl.dwDestBlend;
            pAP->dwBlendSrc = m_Mtl.dwSrcBlend;
            pAP->dwFVF = FVF_VNT1;
            pAP->dwPrimitiveSize = sizeof(__VertexT1);
            pAP->fCameraDistance = (s_CameraData.vEye - m_Matrix.Pos()).Magnitude();
            pAP->lpTex = lpTex;
            pAP->ePrimitiveType = D3DPT_TRIANGLELIST;
            pAP->nPrimitiveCount = m_PMInst.GetNumIndices() / 3;
            pAP->nRenderFlags = m_Mtl.nRenderFlags;
            pAP->nVertexCount = m_PMInst.GetNumVertices();
            pAP->MtxWorld = m_Matrix;
            pAP->pVertices = m_PMInst.GetVertices();
            pAP->pwIndices = m_PMInst.GetIndices();
        }

        return; // No rendering.
    }

    static DWORD dwFog, dwCull;

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Polygon += m_PMInst.GetNumIndices() / 3; // Rendering Information Update...
#endif

    if (m_Mtl.nRenderFlags & RF_NOTUSEFOG) // Ignore Fog...
    {
        s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
        if (TRUE == dwFog) {
            s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
        }
    }
    if (m_Mtl.nRenderFlags & RF_DOUBLESIDED) // Render Flags -
    {
        s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    }

    s_lpD3DDev->SetMaterial(&m_Mtl); // Material settings..
    s_lpD3DDev->SetTexture(0, lpTex);
    if (NULL != lpTex) {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, m_Mtl.dwColorOp);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, m_Mtl.dwColorArg1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, m_Mtl.dwColorArg2);
    } else {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }

   // Apply the pre-calculated world matrix when loading...
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

    m_PMInst.PartialRender(iCount, pIB);

    if ((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) {
        s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // Do not use fog..
    }
    if ((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) {
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
    }
}
#else
void CN3SPart::PartialRender(int iCount, WORD * pIndices) {
    if (m_bOutOfCameraRange || m_PMInst.GetNumVertices() <= 0) {
        return;
    }

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Part++; // Rendering Information Update...
#endif

    LPDIRECT3DTEXTURE9 lpTex = NULL;
    int                iTC = m_TexRefs.size();
    if (iTC > 0) {
        int iTexIndex = (int)m_fTexIndex;
        if (iTexIndex >= 0 && iTexIndex < iTC && m_TexRefs[iTexIndex]) {
            lpTex = m_TexRefs[iTexIndex]->Get();
        }
    }

    if (m_Mtl.nRenderFlags & RF_ALPHABLENDING) // use Alpha
    {
        __AlphaPrimitive * pAP = s_AlphaMgr.Add();
        if (pAP) {
            pAP->bUseVB = FALSE;
            pAP->dwBlendDest = m_Mtl.dwDestBlend;
            pAP->dwBlendSrc = m_Mtl.dwSrcBlend;
            pAP->dwFVF = FVF_VNT1;
            pAP->dwPrimitiveSize = sizeof(__VertexT1);
            pAP->fCameraDistance = (s_CameraData.vEye - m_Matrix.Pos()).Magnitude();
            pAP->lpTex = lpTex;
            pAP->ePrimitiveType = D3DPT_TRIANGLELIST;
            pAP->nPrimitiveCount = m_PMInst.GetNumIndices() / 3;
            pAP->nRenderFlags = m_Mtl.nRenderFlags;
            pAP->nVertexCount = m_PMInst.GetNumVertices();
            pAP->MtxWorld = m_Matrix;
            pAP->pVertices = m_PMInst.GetVertices();
            pAP->pwIndices = m_PMInst.GetIndices();
        }

        return; // No rendering.
    }

    static DWORD dwFog, dwCull;

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape_Polygon += m_PMInst.GetNumIndices() / 3; // Rendering Information Update...
#endif

    if (m_Mtl.nRenderFlags & RF_NOTUSEFOG) // Ignore Fog...
    {
        s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
        if (TRUE == dwFog) {
            s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
        }
    }
    if (m_Mtl.nRenderFlags & RF_DOUBLESIDED) // Render Flags -
    {
        s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCull);
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    }

    s_lpD3DDev->SetMaterial(&m_Mtl); // Material settings..
    s_lpD3DDev->SetTexture(0, lpTex);
    if (NULL != lpTex) {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, m_Mtl.dwColorOp);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, m_Mtl.dwColorArg1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, m_Mtl.dwColorArg2);
    } else {
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    }

    // Apply the pre-calculated world matrix when loading...
    s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

    m_PMInst.PartialRender(iCount, pIndices);

    if ((m_Mtl.nRenderFlags & RF_NOTUSEFOG) && TRUE == dwFog) {
        s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, TRUE); // Do not use fog..
    }
    if ((m_Mtl.nRenderFlags & RF_DOUBLESIDED) && D3DCULL_NONE != dwCull) {
        s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCull);
    }
}
#endif

// CN3Shape Part ....
///////////////////////////////////

///////////////////////////////////
// CN3Shape
CN3Shape::CN3Shape() {
    m_dwType |= OBJ_SHAPE;

    m_bDontRender = false;
    m_bVisible = true;

    m_iBelong = 0;
    m_iEventID = 0;    //Event ID
    m_iEventType = 0;  //Event Type
    m_iNPC_ID = 0;     // NPC ID if the object is used as an NPC
    m_iNPC_Status = 0; // NPC Status in case of object used as NPC
}

CN3Shape::~CN3Shape() {
    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        delete m_Parts[i];
    }
    m_Parts.clear();
}

void CN3Shape::Release() {
    m_bDontRender = false;
    m_bVisible = true;

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        delete m_Parts[i];
    }
    m_Parts.clear();

    m_iBelong = 0;
    m_iEventID = 0;    //Event ID
    m_iEventType = 0;  // Event Type - Bind point, gate, lever, etc...
    m_iNPC_ID = 0;     // NPC ID if the object is used as an NPC
    m_iNPC_Status = 0; // NPC Status if the object is used as an NPC

    CN3TransformCollision::Release();
}

void CN3Shape::Tick(float fFrm) {
    if (false == m_bVisible) {
        return;
    }

   // If the distance is close, loosen the curling more.
    // Find the largest scale value...
    float fScale = m_vScale.x;
    if (fScale < m_vScale.y) {
        fScale = m_vScale.y;
    }
    if (fScale < m_vScale.z) {
        fScale = m_vScale.z;
    }

    // If you are far away from the camera, it will pass by.
    float fDist = (m_vPos - s_CameraData.vEye).Magnitude();
    if (fDist > s_CameraData.fFP + m_fRadius * fScale * 2.0f) {
        m_bDontRender = true;
        return;
    }

    //    float fDelta = 2.4f;
    //    if(fDist < 64.0f)
    //        fDelta += (64.0f - fDist)/24.0f;
    __Vector3 vCenter = (this->Min() + this->Max()) * 0.5f;
    if (s_CameraData.IsOutOfFrustum(vCenter, this->Radius())) {
        m_bDontRender = true;
        return;
    }

    m_bDontRender = false;

    CN3TransformCollision::Tick(fFrm);

    CN3SPart * pPD = NULL;
    int        iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        if (NULL == m_Parts[i]) {
            continue;
        }

        m_Parts[i]->Tick(m_Matrix, m_qRot, fScale);
    }
}

// Camera position, camera plane (observation frustum plane) -> It is an array of 12 vectors.
// [0][1]:Camera position and vector, [2][3]:Camera range position and direction vector, [4][5] ~ [10][11]:Up, down, left and right plane vectors
void CN3Shape::Render() {
    if (false == m_bVisible) {
        return;
    }
    if (true == m_bDontRender) {
        return;
    }

#ifdef _DEBUG
    CN3Base::s_RenderInfo.nShape++;
#endif

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        if (NULL == m_Parts[i]) {
            continue;
        }

        m_Parts[i]->Render();
    }
}

#ifdef _N3TOOL
void CN3Shape::RenderSelected(bool bWireFrame) {
    // Draw axis...
    CN3Transform::Render(NULL, m_fRadius * 3.0f);

    CN3SPart * pPD = NULL;
    int        iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->RenderSelected(bWireFrame);
    }
}
#endif // end of _N3TOOL

bool CN3Shape::Load(HANDLE hFile) {
    CN3TransformCollision::Load(hFile); // Read basic information...

    DWORD dwRWC = 0;

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        delete m_Parts[i];
    }
    m_Parts.clear();

    ReadFile(hFile, &iPC, 4, &dwRWC, NULL); // Part Count
    if (iPC > 0) {
        m_Parts.assign(iPC, NULL);
        for (int i = 0; i < iPC; i++) {
            m_Parts[i] = new CN3SPart();
            m_Parts[i]->Load(hFile);
            m_Parts[i]->ReCalcMatrix(m_Matrix); //Part Matrix Calculation
        }
    }

    ReadFile(hFile, &m_iBelong, 4, &dwRWC, NULL);     // belong
    ReadFile(hFile, &m_iEventID, 4, &dwRWC, NULL);    //Event ID
    ReadFile(hFile, &m_iEventType, 4, &dwRWC, NULL);  // Event Type - Bind point, gate, lever, etc...
    ReadFile(hFile, &m_iNPC_ID, 4, &dwRWC, NULL);     // NPC ID if the object is used as an NPC
    ReadFile(hFile, &m_iNPC_Status, 4, &dwRWC, NULL); // NPC Status if the object is used as an NPC

    this->FindMinMax();

    return true;
}

bool CN3Shape::LoadTransformOnly(HANDLE hFile) {
    return CN3Transform::Load(hFile); // Read basic information...
}

#ifdef _N3TOOL
bool CN3Shape::Save(HANDLE hFile) {
    CN3TransformCollision::Save(hFile); // Read basic information...

    DWORD dwRWC = 0;

    int nL = 0;

    CN3SPart * pPD = NULL;
    int        iPC = m_Parts.size();
    WriteFile(hFile, &iPC, 4, &dwRWC, NULL); // Mesh FileName
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->Save(hFile);
    }

    WriteFile(hFile, &m_iBelong, 4, &dwRWC, NULL);     // belong
    WriteFile(hFile, &m_iEventID, 4, &dwRWC, NULL);    //Event ID
    WriteFile(hFile, &m_iEventType, 4, &dwRWC, NULL);  // Event Type - Bind point, gate, lever, etc...
    WriteFile(hFile, &m_iNPC_ID, 4, &dwRWC, NULL);     // NPC ID if the object is used as an NPC
    WriteFile(hFile, &m_iNPC_Status, 4, &dwRWC, NULL); // NPC Status if the object is used as an NPC

    return true;
}
#endif // end of _N3TOOL

void CN3Shape::PartDelete(int iIndex) {
    if (iIndex < 0 || iIndex >= m_Parts.size()) {
        return;
    }
    it_SPart it = m_Parts.begin();
    for (int i = 0; i < iIndex; i++, it++) {
        ;
    }
    delete *it;
    m_Parts.erase(it);
}

#ifdef _N3TOOL
void CN3Shape::RenderSelected(int iPart, bool bWireFrame) {
    if (iPart < 0 || iPart >= m_Parts.size()) {
        return;
    }
    m_Parts[iPart]->RenderSelected(bWireFrame);
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Shape::IsPMeshProcessed() {
    int iPC = m_Parts.size();
    if (iPC <= 0) {
        return false;
    }

    CN3SPart * pPD = NULL;
    for (int i = 0; i < iPC; i++) {
        pPD = m_Parts[i];
        if (NULL == pPD || NULL == pPD->MeshInstance()) {
            continue;
        }

        if (false == pPD->MeshInstance()->IsLOD()) {
            return false;
        }
    }

    return true;
}
#endif // end of _N3TOOL

void CN3Shape::ReCalcMatrix() {
    CN3Transform::ReCalcMatrix(); // Calculate the Transform Matrix...

    // Recalculate the matrix of each part.
    ReCalcPartMatrix();
}

void CN3Shape::ReCalcPartMatrix() {
    // Recalculate the matrix of each part.
    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->ReCalcMatrix(m_Matrix);
    }
}

void CN3Shape::FindMinMax() {
    m_vMin.Zero();
    m_vMax.Zero();
    m_fRadius = 0;

    int iPC = m_Parts.size();
    if (iPC <= 0) {
        return;
    }

    m_vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
    m_vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    __Vector3 vMinTmp(0, 0, 0);
    __Vector3 vMaxTmp(0, 0, 0);

    // Find the largest point...
    static __Matrix44 mtxWI;
    D3DXMatrixInverse(&mtxWI, NULL, &m_Matrix); // World Matrix Inverse
    for (int i = 0; i < iPC; i++) {
        vMinTmp = m_Parts[i]->Min() * mtxWI; // Converts the minimum value in the world to local coordinates.
        vMaxTmp = m_Parts[i]->Max() * mtxWI; // Converts the maximum value in the world to local coordinates.

        if (vMinTmp.x < m_vMin.x) {
            m_vMin.x = vMinTmp.x;
        }
        if (vMinTmp.y < m_vMin.y) {
            m_vMin.y = vMinTmp.y;
        }
        if (vMinTmp.z < m_vMin.z) {
            m_vMin.z = vMinTmp.z;
        }
        if (vMaxTmp.x > m_vMax.x) {
            m_vMax.x = vMaxTmp.x;
        }
        if (vMaxTmp.y > m_vMax.y) {
            m_vMax.y = vMaxTmp.y;
        }
        if (vMaxTmp.z > m_vMax.z) {
            m_vMax.z = vMaxTmp.z;
        }
    }

    // Calculate the radius using the maximum and minimum values.
    m_fRadius = (m_vMax - m_vMin).Magnitude() * 0.5f;
}

int CN3Shape::CheckCollisionPrecisely(bool bIgnoreBoxCheck, int ixScreen, int iyScreen, __Vector3 * pVCol,
                                      __Vector3 * pVNormal) {
    // Compute the vector of the pick ray in screen space
    __Vector3 vTmp;

    vTmp.x = (((2.0f * ixScreen) / (CN3Base::s_CameraData.vp.Width)) - 1) / CN3Base::s_CameraData.mtxProjection._11;
    vTmp.y = -(((2.0f * iyScreen) / (CN3Base::s_CameraData.vp.Height)) - 1) / CN3Base::s_CameraData.mtxProjection._22;
    vTmp.z = 1.0f;

    // Transform the screen space pick ray into 3D space
    __Matrix44 * pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
    __Vector3    vPos, vDir;
    vDir.x = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
    vDir.y = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
    vDir.z = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
    vPos = pMtxVI->Pos();

    return this->CheckCollisionPrecisely(bIgnoreBoxCheck, vPos, vDir, pVCol, pVNormal);
}

int CN3Shape::CheckCollisionPrecisely(bool bIgnoreBoxCheck, const __Vector3 & vPos, const __Vector3 & vDir,
                                      __Vector3 * pVCol, __Vector3 * pVNormal) {
    if (false == bIgnoreBoxCheck && false == ::_CheckCollisionByBox(vPos, vDir, m_vMin * m_Matrix, m_vMax * m_Matrix)) {
        return -1; //Check the box first..
    }

    __Vector3 vPos2 = vPos, vDir2 = vDir;
    int       iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        CN3PMeshInstance * pPMI = m_Parts[i]->MeshInstance();
        if (NULL == pPMI) {
            continue;
        }

        __VertexT1 * pVs = pPMI->GetVertices();
        WORD *       pwIs = pPMI->GetIndices();
        int          nIndexCount = pPMI->GetNumIndices();

        int nFC = nIndexCount / 3; // Face Count
        if (nFC > 64 && false == ::_CheckCollisionByBox(vPos, vDir, m_Parts[i]->Min(), m_Parts[i]->Max())) {
            continue; //If the number of faces is more than 24, check the box first.
        }

        static __Matrix44 mtxWI;
        D3DXMatrixInverse(&mtxWI, NULL, &(m_Parts[i]->m_Matrix)); // World Matrix Inverse

        vPos2 = vPos * mtxWI;
        mtxWI.PosSet(0, 0, 0);
        vDir2 = vDir * mtxWI; // Rotate by inverse matrix..

        int       nCI0, nCI1, nCI2;
        __Vector3 v0, v1, v2;
        for (int j = 0; j < nFC; j++) // Collision check for each face...
        {
            nCI0 = pwIs[j * 3 + 0];
            nCI1 = pwIs[j * 3 + 1];
            nCI2 = pwIs[j * 3 + 2];

            if (false == ::_IntersectTriangle(vPos2, vDir2, pVs[nCI0], pVs[nCI1], pVs[nCI2])) {
                continue;
            }

            float fT, fU, fV;
            ::_IntersectTriangle(vPos2, vDir2, pVs[nCI0], pVs[nCI1], pVs[nCI2], fT, fU, fV, pVCol);
            if (pVCol) {
                (*pVCol) *= m_Parts[i]->m_Matrix;
            }
            if (pVNormal) {
                (*pVNormal).Cross(pVs[nCI1] - pVs[nCI0], pVs[nCI2] - pVs[nCI1]);
                (*pVNormal).Normalize();

                D3DXMatrixInverse(&mtxWI, NULL, &(m_Parts[i]->m_Matrix)); // World Matrix Inverse
                mtxWI.PosSet(0, 0, 0);                                    // Rotate by inverse matrix..
                (*pVNormal) *= mtxWI;                                     // Rotate by inverse matrix..
            }
            return i;
        }
    }

    return -1;
}

bool CN3Shape::MakeCollisionMeshByParts() //Make the collision mesh a box...
{
    int iPC = m_Parts.size();
    int iVC = 0, iIC = 0;
    for (int i = 0; i < iPC; i++) {
        CN3PMesh * pPMesh = m_Parts[i]->Mesh();
        if (NULL == pPMesh) {
            continue;
        }

        iVC += 8;
        iIC += 36;
    }

    if (iVC <= 0 || iIC <= 0) {
        return false;
    }

    CN3VMesh * pVMesh = new CN3VMesh();
    pVMesh->CreateVertices(iVC);
    pVMesh->CreateIndex(iIC);

    __Vector3 * pVDest = pVMesh->Vertices();
    WORD *      pwIDest = pVMesh->Indices();

    __Matrix44 mtxI = m_Matrix;
    D3DXMatrixInverse(&mtxI, NULL, &m_Matrix);
    CN3VMesh VMTmp;

    iVC = 0;
    iIC = 0;
    for (int i = 0; i < iPC; i++) {
        CN3PMesh * pPMesh = m_Parts[i]->Mesh();
        if (NULL == pPMesh) {
            continue;
        }

        VMTmp.CreateCube(pPMesh->Min(), pPMesh->Max());

        __Vector3 * pVSrc = VMTmp.Vertices();
        WORD *      pwISrc = VMTmp.Indices();

        m_Parts[i]->Tick(m_Matrix, m_qRot, 1.0f);
        __Matrix44 mtxPart = m_Parts[i]->m_Matrix;
        mtxPart *= mtxI;
        for (int j = 0; j < 8; j++) {
            pVDest[iVC + j] = pVSrc[j] * mtxPart;
        }
        for (int j = 0; j < 36; j++) {
            pwIDest[iIC + j] = pwISrc[j] + iVC;
        }

        iVC += 8;
        iIC += 36;
    }

    if (iVC <= 0 || iIC <= 0) {
        delete pVMesh;
        return false;
    }

    int  iCount = CN3Base::s_MngVMesh.Count();
    char szBuff[256];
    sprintf(szBuff, "%s_collision_%d.n3vmesh", m_szFileName.c_str(), iCount); // Create a temporary name...

    pVMesh->FileNameSet(szBuff);
    CN3Base::s_MngVMesh.Delete(&m_pMeshCollision); // Delete the previous one...
    CN3Base::s_MngVMesh.Add(pVMesh);
    m_pMeshCollision = s_MngVMesh.Get(pVMesh->FileName());

    this->FindMinMax();

    return true;
}

bool CN3Shape::MakeCollisionMeshByPartsDetail() // As it is... create a collision mesh...
{
    int iPC = m_Parts.size();
    int iMaxNumVertices = 0, iMaxNumIndices = 0;
    int iVC = 0, iIC = 0;
    for (int i = 0; i < iPC; i++) {
        CN3PMesh *         pPMesh = m_Parts[i]->Mesh();
        CN3PMeshInstance * pPMI = m_Parts[i]->MeshInstance();
        if (NULL == pPMesh || NULL == pPMI) {
            continue;
        }

        iMaxNumVertices = pPMesh->GetMaxNumVertices();
        iMaxNumIndices = pPMesh->GetMaxNumIndices();

        iVC += iMaxNumVertices;
        iIC += iMaxNumIndices;
    }

    if (iVC <= 0 || iIC <= 0) {
        return false;
    }

    CN3VMesh * pVMesh = new CN3VMesh();
    pVMesh->CreateVertices(iVC);
    pVMesh->CreateIndex(iIC);

    __Vector3 * pVDest = pVMesh->Vertices();
    WORD *      pwIDest = pVMesh->Indices();

    __VertexT1 * pVSrc = NULL;
    WORD *       pwISrc = NULL;

    iVC = 0;
    iIC = 0;
    __Matrix44 mtxI = m_Matrix;
    D3DXMatrixInverse(&mtxI, NULL, &m_Matrix);

    for (int i = 0; i < iPC; i++) {
        CN3PMesh *         pPMesh = m_Parts[i]->Mesh();
        CN3PMeshInstance * pPMI = m_Parts[i]->MeshInstance();
        if (NULL == pPMesh || NULL == pPMI) {
            continue;
        }

        iMaxNumVertices = pPMesh->GetMaxNumVertices();
        iMaxNumIndices = pPMesh->GetMaxNumIndices();
        pPMI->SetLODByNumVertices(iMaxNumVertices);
        pVSrc = pPMesh->GetVertices();
        pwISrc = pPMI->GetIndices();
        if (NULL == pVSrc || NULL == pwISrc) {
            continue;
        }

        m_Parts[i]->Tick(m_Matrix, m_qRot, 1.0f);
        __Matrix44 mtxPart = m_Parts[i]->m_Matrix;
        mtxPart *= mtxI;
        for (int j = 0; j < iMaxNumVertices; j++) {
            pVDest[iVC + j] = pVSrc[j] * mtxPart;
        }
        for (int j = 0; j < iMaxNumIndices; j++) {
            pwIDest[iIC + j] = pwISrc[j] + iVC;
        }

        iVC += iMaxNumVertices;
        iIC += iMaxNumIndices;
    }

    if (iVC <= 0 || iIC <= 0) {
        delete pVMesh;
        return false;
    }

    int  iCount = CN3Base::s_MngVMesh.Count();
    char szBuff[256];
    sprintf(szBuff, "%s_collision_%d.n3vmesh", m_szFileName.c_str(), iCount); // Create a temporary name...

    pVMesh->FileNameSet(szBuff);
    CN3Base::s_MngVMesh.Delete(&m_pMeshCollision); // Delete the previous one...
    CN3Base::s_MngVMesh.Add(pVMesh);
    m_pMeshCollision = s_MngVMesh.Get(pVMesh->FileName());

    this->FindMinMax();

    return true;
}

#ifdef _N3TOOL
void CN3Shape::MakeDefaultMaterial() {
    // Set the material of each part to basic white.
    __Material mtlBasic;
    mtlBasic.Init();

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        memcpy(&(m_Parts[i]->m_Mtl), &mtlBasic, sizeof(_D3DMATERIAL9));
    }
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
void CN3Shape::RemoveRenderFlags(int nFlags) {
    // Recalculate the matrix of each part.
    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->m_Mtl.nRenderFlags &= (~nFlags);
        ; //Default white...
    }
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Shape::SaveToSameFolder(const std::string & szFullPath) {
    if (szFullPath.empty()) {
        return false;
    }

    std::string szPath = szFullPath;
    for (int i = szFullPath.size() - 1; i >= 0; i--) {
        if ('\\' == szPath[i] || '/' == szPath[i]) {
            szPath = szPath.substr(0, i + 1);
            break;
        }
    }

    char        szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    std::string szNameTmp, szOldFN;

    int                      iPC = m_Parts.size();
    std::vector<std::string> OldPartFNs;
    std::vector<std::string> OldTexFNs;
    for (int i = 0; i < iPC; i++) {
        szOldFN = m_Parts[i]->Mesh()->FileName();
        OldPartFNs.push_back(szOldFN); // Save file name..

        _splitpath(szOldFN.c_str(), szDrive, szDir, szFName, szExt);
        szNameTmp = szPath + szFName + szExt;
        m_Parts[i]->Mesh()->SaveToFile(szNameTmp);

        int iTC = m_Parts[i]->TexCount();
        for (int j = 0; j < iTC; j++) {
            CN3Texture * pTex = m_Parts[i]->Tex(j);

            szOldFN = pTex->FileName();
            OldTexFNs.push_back(szOldFN); // Save file name..

            _splitpath(szOldFN.c_str(), szDrive, szDir, szFName, szExt);
            szNameTmp = szPath + szFName + szExt;
            pTex->SaveToFile(szNameTmp);
        }
    }

    szOldFN = m_szFileName;
    _splitpath(m_szFileName.c_str(), szDrive, szDir, szFName, szExt);
    szNameTmp = szPath + szFName + szExt;
    this->SaveToFile(szNameTmp);
    m_szFileName = szOldFN;

    // Return the file name to the original..
    iPC = m_Parts.size();
    int iSeq = 0;
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->Mesh()->FileNameSet(OldPartFNs[i]);

        int iTC = m_Parts[i]->TexCount();
        for (int j = 0; j < iTC; j++) {
            m_Parts[i]->Tex(j)->FileNameSet(OldTexFNs[iSeq]);
            iSeq++;
        }
    }

    //   By: Ecli666 (On 2002-10-16 11:44:19 AM)
    //
    szOldFN = CollisionMesh()->FileName();
    _splitpath(CollisionMesh()->FileName().c_str(), szDrive, szDir, szFName, szExt);
    szNameTmp = szPath + szFName + szExt;
    CollisionMesh()->SaveToFile(szNameTmp);
    CollisionMesh()->FileNameSet(szOldFN);

    // ~(By Ecli666 On 2002-10-16 11:44:19 AM)

    return true;
}

bool CN3Shape::SaveToSameFolderAndMore(const std::string & szFullPath, const std::string & szRelativePath) {
    if (szFullPath.empty()) {
        return false;
    }

    std::string szPath = szFullPath;
    for (int i = szFullPath.size() - 1; i >= 0; i--) {
        if ('\\' == szPath[i] || '/' == szPath[i]) {
            szPath = szPath.substr(0, i + 1);
            break;
        }
    }

    char        szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    std::string szNameTmp, szOldFN;

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        szOldFN = m_Parts[i]->Mesh()->FileName();

        _splitpath(szOldFN.c_str(), szDrive, szDir, szFName, szExt);
        szNameTmp = szPath + szFName + szExt;
        m_Parts[i]->Mesh()->SaveToFile(szNameTmp);
        m_Parts[i]->Mesh()->FileNameSet(szRelativePath + szFName + szExt);

        int iTC = m_Parts[i]->TexCount();
        for (int j = 0; j < iTC; j++) {
            CN3Texture * pTex = m_Parts[i]->Tex(j);

            szOldFN = pTex->FileName();

            _splitpath(szOldFN.c_str(), szDrive, szDir, szFName, szExt);
            szNameTmp = szPath + szFName + szExt;
            pTex->SaveToFile(szNameTmp);
            pTex->FileNameSet(szRelativePath + szFName + szExt);
        }
    }

    _splitpath(m_szFileName.c_str(), szDrive, szDir, szFName, szExt);
    szNameTmp = szPath + szFName + szExt;
    this->SaveToFile(szNameTmp);
    m_szFileName = szRelativePath + szFName + szExt;

    //    By: Ecli666 (On 2002-10-16 11:44:19 AM)
    //
    if (CollisionMesh()) {
        _splitpath(CollisionMesh()->FileName().c_str(), szDrive, szDir, szFName, szExt);
        szNameTmp = szPath + szFName + szExt;
        CollisionMesh()->SaveToFile(szNameTmp);
        szOldFN = szFName;
        szOldFN += szExt;
        CollisionMesh()->FileNameSet(szRelativePath + szFName + szExt);
    }
    //    ~(By Ecli666 On 2002-10-16 11:44:19 AM)

    return true;
}

#endif // end of _N3TOOL

//    By: Ecli666 (On 2002-08-06 4:33:32 PM)
//
void CN3Shape::SetMaxLOD() {
    m_bDontRender = false;

    int iPC = m_Parts.size();
    for (int i = 0; i < iPC; i++) {
        m_Parts[i]->m_bOutOfCameraRange = FALSE;
        m_Parts[i]->m_PMInst.SetLOD(0);
    }
}

__Matrix44 CN3Shape::GetPartMatrix(int iPartIndex) {
    return m_Parts[iPartIndex]->m_Matrix;
}

#ifdef _USE_VERTEXBUFFER
void CN3Shape::PartialRender(int iPartIndex, int iCount, LPDIRECT3DINDEXBUFFER9 pIB) {
    if (iPartIndex >= m_Parts.size()) {
        return;
    }

    m_Parts[iPartIndex]->PartialRender(iCount, pIB);
}
#else
void CN3Shape::PartialRender(int iPartIndex, int iCount, WORD * pIndices) {
    if (iPartIndex >= m_Parts.size()) {
        return;
    }

    m_Parts[iPartIndex]->PartialRender(iCount, pIndices);
}
#endif

int CN3Shape::GetIndexbufferCount(int iPartIndex) {
    if (iPartIndex >= m_Parts.size()) {
        return 0;
    }

    return m_Parts[iPartIndex]->MeshInstance()->GetNumIndices();
}

int CN3Shape::GetIndexByiOrder(int iPartIndex, int iOrder) {
    return m_Parts[iPartIndex]->MeshInstance()->GetIndexByiOrder(iOrder);
}

__Vector3 CN3Shape::GetVertexByIndex(int iPartIndex, int iIndex) {
    return m_Parts[iPartIndex]->MeshInstance()->GetVertexByIndex(iIndex);
}

int CN3Shape::GetColIndexbufferCount() {
    if (CollisionMesh()) {
        if (CollisionMesh()->IndexCount()) {
            return CollisionMesh()->IndexCount();
        } else {
            return CollisionMesh()->VertexCount();
        }
    } else {
        return -1;
    }
}

int CN3Shape::GetColIndexByiOrder(int iOrder) {
    return CollisionMesh()->GetColIndexByiOrder(iOrder);
}

__Vector3 CN3Shape::GetColVertexByIndex(int iIndex) {
    return CollisionMesh()->GetColVertexByIndex(iIndex);
}

void CN3Shape::PartialColRender(int iCount, int * piIndices) {
    CollisionMesh()->PartialColRender(iCount, piIndices);
}

void CN3Shape::PartialGetCollision(int iIndex, __Vector3 & vec) {
    CollisionMesh()->PartialGetCollision(iIndex, vec);
}

//    ~(By Ecli666 On 2002-08-06 4:33:32 PM)
