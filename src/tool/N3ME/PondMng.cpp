// PondMng.cpp: implementation of the CPondMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "PondMng.h"
#include "PondMesh.h"
#include "DlgPondProperty.h"
#include "MainFrm.h"
#include "MapMng.h"
#include "LyTerrain.h"

#include "N3Base/N3EngTool.h"
#include "N3Base/N3Camera.h"
#include "N3Base/N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPondMng::CPondMng(CMainFrame * pMainFrm) {
    m_pMainFrm = pMainFrm;
    Release();
    m_pDlgProperty = new CDlgPondProperty(this);
    m_pDlgProperty->Create(IDD_POND_PROPERTY, m_pMainFrm);
}

CPondMng::~CPondMng() {
    Release();
    if (m_pDlgProperty) {
        m_pDlgProperty->DestroyWindow();
        delete m_pDlgProperty;
        m_pDlgProperty = NULL;
    }
}

void CPondMng::Release() {
    VtxBackupRelease();
    SelPondRelease();

    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM) {
            delete pRM;
        }
    }
    m_PondMeshes.clear();

    m_SelVtxArray.RemoveAll();
    m_bEditMode = FALSE;
    m_bChooseGroup = FALSE;
    m_bChooseEditPond = FALSE;
    m_bMovePond = FALSE;
    m_PCursorMode = PCM_NONE;
    m_VtxPosDummy.Release();
}

void CPondMng::MainInvalidate() {
    m_pMainFrm->Invalidate(FALSE);
}

void CPondMng::SelPondDelete(CPondMesh * pPondMesh) {
    it_PondMesh it = m_pSelPonds.begin();
    int         i, iSize = m_pSelPonds.size();
    for (i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM == pPondMesh) {
            break;
        }
    }

    if (i != iSize) {
        it = m_pSelPonds.erase(it);
    }
}

void CPondMng::SelPondRelease() {
    it_PondMesh it = m_pSelPonds.begin();
    int         iSize = m_pSelPonds.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM) {
            pRM = NULL;
        }
    }
    m_pSelPonds.clear();
}

void CPondMng::SetSelPonds(CPondMesh * pPondMesh) {
    it_PondMesh it = m_pSelPonds.begin();
    int         iSize = m_pSelPonds.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM == pPondMesh) {
            return;
        }
    }

    m_pSelPonds.push_back(pPondMesh);
}

bool CPondMng::Load(HANDLE hFile) {
    Release();

    DWORD dwNum;

    int iVersion;
    ReadFile(hFile, &iVersion, sizeof(iVersion), &dwNum, NULL); //    GetVersion

    int iPondMeshCount;
    if (iVersion == 1001) {
        ReadFile(hFile, &iPondMeshCount, sizeof(iPondMeshCount), &dwNum, NULL);
        for (int i = 0; i < iPondMeshCount; ++i) {
            CPondMesh * pPondMesh = new CPondMesh;
            pPondMesh->Load1001(hFile);
            m_PondMeshes.push_back(pPondMesh);
        }
    } else if (iVersion == 1000) {
        ReadFile(hFile, &iPondMeshCount, sizeof(iPondMeshCount), &dwNum, NULL);
        for (int i = 0; i < iPondMeshCount; ++i) {
            CPondMesh * pPondMesh = new CPondMesh;
            pPondMesh->Load1000(hFile);
            m_PondMeshes.push_back(pPondMesh);
        }
    } else {
        iPondMeshCount = iVersion;
        for (int i = 0; i < iPondMeshCount; ++i) {
            CPondMesh * pPondMesh = new CPondMesh;
            pPondMesh->Load(hFile);
            m_PondMeshes.push_back(pPondMesh);
        }
    }

    m_pDlgProperty->PondComboSetting();
    return 0;
}

bool CPondMng::Save(HANDLE hFile) {
    DWORD dwNum;

    //    version 1000 - alpha input
    int nFileVersion = 1001;
    WriteFile(hFile, &nFileVersion, sizeof(nFileVersion), &dwNum, NULL); //pond number

    int iSize = m_PondMeshes.size();
    WriteFile(hFile, &iSize, 4, &dwNum, NULL);

    it_PondMesh it = m_PondMeshes.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        pRM->Save(hFile);
    }
    return 0;
}

void CPondMng::Tick() {
    m_VtxPosDummy.Tick();
}

void CPondMng::Render() {
    HRESULT hr;
    // backup state
    DWORD dwZEnable, dwLighting, dwFog;
    hr = s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    hr = s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    hr = s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);

    // set state
    if (dwZEnable != D3DZB_TRUE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    }
    if (dwLighting != FALSE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    }
    if (dwFog != FALSE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    // Drawing an existing pond
    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        pRM->Render();
    }

    if (m_bEditMode) {
        // If you are creating a new pond, draw a drag line
        if (m_PCursorMode == PCM_CREATE) {
            __Matrix44 matWorld;
            matWorld.Identity();
            s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

            // set texture
            hr = s_lpD3DDev->SetTexture(0, NULL);
            hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

            // Render
            s_lpD3DDev->SetFVF(FVF_XYZCOLOR);
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, m_CreateLine, sizeof(__VertexXyzColor));
        }

        // draw dummy box
        m_VtxPosDummy.Render();

        BOOL bisFix = FALSE;

        CPondMesh * pSelPond;
        it_PondMesh it = m_pSelPonds.begin();
        int         iSize = m_pSelPonds.size();
        for (int i = 0; i < iSize; i++, it++) {
            pSelPond = *it;
            if (pSelPond) {
                // Drawing points on the selected pond (pinching)
                pSelPond->RenderVertexPoint();
            }
        }

        // Draw selected point (green)
        iSize = m_SelVtxArray.GetSize();
        if (iSize > 0) {
            // transform
            __Matrix44 matView, matProj, matVP;
            s_lpD3DDev->GetTransform(D3DTS_VIEW, &matView);
            s_lpD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
            D3DXMatrixMultiply(&matVP, &matView, &matProj);
            D3DVIEWPORT9 vp = s_CameraData.vp;

            __VertexTransformedColor Vertices[4];
            D3DCOLOR                 clr;
            if (bisFix == TRUE) {
                clr = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0x00);
            } else {
                clr = D3DCOLOR_ARGB(0xff, 0x00, 0xff, 0x00);
            }
            s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);

            for (int i = 0; i < iSize; ++i) {
                __VertexXyzT2 * pVtx = m_SelVtxArray.GetAt(i);
                if (pVtx == NULL) {
                    continue;
                }
                D3DXVECTOR4 v;
                D3DXVec3Transform(&v, (D3DXVECTOR3 *)(pVtx), &matVP);

                float fScreenZ = (v.z / v.w);
                if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                    continue;
                }

                int iScreenX = int(((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f);
                int iScreenY = int((1.0f - (v.y / v.w)) * (vp.Height) / 2.0f);
                if (iScreenX >= (int)vp.X && iScreenX <= (int)vp.Width && iScreenY >= (int)vp.Y &&
                    iScreenY <= (int)vp.Height) {
                    // set X (Since only 1 pixel is drawn when a dot is placed, draw an X mark.)
                    Vertices[0].Set(float(iScreenX - 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
                    Vertices[1].Set(float(iScreenX + 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
                    Vertices[2].Set(float(iScreenX + 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
                    Vertices[3].Set(float(iScreenX - 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
                    // render
                    s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, Vertices, sizeof(__VertexTransformedColor));
                }
            }
        }

        // Draw drag area
        if (PCM_SELECTING == m_PCursorMode) {
            m_pMainFrm->GetMapMng()->RenderDragRect(&m_rcSelDrag);
        }
    }

    // restore
    if (dwZEnable != D3DZB_TRUE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    }
    if (dwLighting != FALSE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
    }
    if (dwFog != FALSE) {
        hr = s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
    }
}

CPondMesh * CPondMng::CreateNewPondMesh() {
    CPondMesh * pRM = new CPondMesh;
    __Vector3   vPos[4];

    vPos[0].Set(m_CreateLine[0].x, m_CreateLine[0].y, m_CreateLine[0].z); //    top left
    vPos[1].Set(m_CreateLine[1].x, m_CreateLine[1].y, m_CreateLine[1].z); //    top right
    vPos[2].Set(m_CreateLine[2].x, m_CreateLine[2].y, m_CreateLine[2].z); //    right below
    vPos[3].Set(m_CreateLine[3].x, m_CreateLine[3].y, m_CreateLine[3].z); //    bottom left

    CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();
    pRM->SetTerrain(pTerrain); //    Terrain pointer input
    pRM->MakeDrawRect(vPos);   //    Area input
    pRM->MakePondPos();

    SelPondRelease(); //    Delete whatever you have selected so far

    int iID = 0;
    m_pSelPonds.push_back(pRM); //    Put it in first (to check the back)
    while (SetPondID(pRM, iID) == FALSE) {
        iID++; //    Found a new one
    }

    CDlgPondProperty dlg(this);
    dlg.m_IsModalDialog = TRUE;

    if (dlg.DoModal() == IDCANCEL) //    Delete the pond you just created
    {
        SelPondDelete(pRM);
        pRM = NULL;
    }
    if (pRM) //    the pond you want to make
    {
        m_PondMeshes.push_back(pRM);
        SelPondRelease();
        //    Based on the outer points, dots are placed at a certain distance toward the center point. ////
    }

    return pRM;
}

void CPondMng::RemovePondMesh(int iPondID) //    When you press the button to create or select a pond and delete it,
{
    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM && pRM->GetPondID() == iPondID) {
            delete pRM;
            it = m_PondMeshes.erase(it);
            m_SelVtxArray.RemoveAll();
            m_VtxPosDummy.SetSelVtx(NULL);
            SelPondRelease();
            m_pMainFrm->Invalidate();
            return;
        }
    }
}

void CPondMng::GoPond(int iPondID) {
    CPondMesh * pPond = GetPondMesh(iPondID);
    if (pPond == NULL) {
        return;
    }

    __Vector3   vPondPos = pPond->GetCenter();
    CN3Camera * pCamera = m_pMainFrm->GetMapMng()->CameraGet();
    if (pCamera) {
        __Vector3 vCamVector = pCamera->m_vPos - pCamera->m_vAt;
        pCamera->m_vAt = vPondPos;
        pCamera->m_vPos = vPondPos + vCamVector;

        SelPondRelease();
        SetSelPonds(pPond);
        m_pDlgProperty->UpdateInfo();
    }
    m_pMainFrm->Invalidate();
}

BOOL CPondMng::MouseMsgFilter(LPMSG pMsg) {
    if (FALSE == m_bEditMode) {
        return FALSE;
    }
    if (GetAsyncKeyState(VK_MENU) & 0xff00) {
        return FALSE;
    }
    static __Vector3 vMouseStrPos;

    if (m_VtxPosDummy.MouseMsgFilter(pMsg)) {
        __Vector3 vMov = m_VtxPosDummy.m_vPos - vMouseStrPos;

        if (m_bShift) {
            vMov.x /= 10.0f, vMov.y /= 10.0f, vMov.z /= 10.0f;
            m_VtxPosDummy.PosSet(vMouseStrPos + vMov);
        }

        InputDummyMovePos(vMov);
        vMouseStrPos = m_VtxPosDummy.m_vPos;
        return TRUE;
    }

    static POINT ptDownBuff;
    static int   bCtrlKeyState = 0; //, bShiftState=0;

    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (bCtrlKeyState == 2) //   Rotating pond
        {
            SetRotatePonds(ptDownBuff.x - point.x);
            ptDownBuff = point;
            return TRUE;
        }

        if (PCM_CREATE == m_PCursorMode) { // Setting the dragging line when adding a new pond
            __Vector3 vRayDir,
                vRayOrig; // Direction and origin of the straight line connecting the screen center (viewpoint) and the mouse pointer
            __Vector3 vPN, vPV; // Plane normal and contained points
            __Vector3 vPos;     // The point where the above plane meets the straight line (point to be found)

            vPN.Set(0, 1, 0);
            vPV = vMouseStrPos;
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig); // Let me borrow this function for a moment.
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT; //    Find the starting point and mouse point.

            ReSetDrawRect(vMouseStrPos, vPos); //    Convert the two points received into a rectangle on the map.
            return TRUE;
        } else if (PCM_SELECTING == m_PCursorMode) {
            m_rcSelDrag.right = point.x;
            m_rcSelDrag.bottom = point.y;
            return TRUE;
        } else if (m_bMovePond == TRUE) {
            vMouseStrPos = m_VtxPosDummy.m_vPos;
        }
    } break;
    case WM_LBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (m_bMovePond == TRUE) {
            m_bMovePond = FALSE;
        }

        ptDownBuff = point;
        if (bCtrlKeyState > 0) //    Rotating pond
        {
            if (bCtrlKeyState < 2) {
                bCtrlKeyState++;
            }
            SetVtxCenter();
            return TRUE;
        }

        if (PCM_CREATE == m_PCursorMode) { // Cancel adding new pond
            m_PCursorMode = PCM_NONE;
            ReleaseCapture();
            return TRUE;
        } else if (PCM_NONE == m_PCursorMode || PCM_SELECT == m_PCursorMode) {
            m_PCursorMode = PCM_SELECTING;
            SetCapture(pMsg->hwnd);
            m_rcSelDrag.right = m_rcSelDrag.left = point.x;
            m_rcSelDrag.bottom = m_rcSelDrag.top = point.y;
            return TRUE;
        }
    } break;
    case WM_LBUTTONUP: {
        if (bCtrlKeyState > 0) //    Rotating pond
        {
            bCtrlKeyState--;
            return TRUE;
        }

        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (PCM_SELECTING == m_PCursorMode) {
            m_PCursorMode = PCM_SELECT;
            ReleaseCapture();

            if (m_rcSelDrag.left > point.x) {
                m_rcSelDrag.right = m_rcSelDrag.left;
                m_rcSelDrag.left = point.x;
            } else {
                m_rcSelDrag.right = point.x;
            }
            if (m_rcSelDrag.top > point.y) {
                m_rcSelDrag.bottom = m_rcSelDrag.top;
                m_rcSelDrag.top = point.y;
            } else {
                m_rcSelDrag.bottom = point.y;
            }

            // If it is just a click rather than a drag, the drag area is set to about 3x3.
            if (m_rcSelDrag.right - m_rcSelDrag.left < 3 && m_rcSelDrag.bottom - m_rcSelDrag.top < 3) {
                m_rcSelDrag.left = point.x - 1;
                m_rcSelDrag.right = point.x + 1;
                m_rcSelDrag.top = point.y - 1;
                m_rcSelDrag.bottom = point.y + 1;
            }
            //    The shift key selects multiple distributed points(s).
            if (SelectVtxByDragRect(&m_rcSelDrag, (pMsg->wParam & MK_SHIFT) ? TRUE : FALSE)) {
                vMouseStrPos = m_VtxPosDummy.m_vPos; //  Enter the location of the dummy
            } else {
                m_PCursorMode = PCM_NONE; //  Since there are no points selected
            }

            return TRUE;
        }
    } break;
    case WM_RBUTTONDOWN: {
        if (bCtrlKeyState) { //  Rotates at a selected point in the pond
            return TRUE;
        }
        if (m_bMovePond == TRUE) {
            m_bMovePond = FALSE;
        }

        if (PCM_SELECT == m_PCursorMode) { // Select Cancel
            m_PCursorMode = PCM_NONE;
            ReleaseCapture();
            return TRUE;
        } else if (PCM_NONE == m_PCursorMode) { // Added new pond
            POINT        point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
            CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();

            __Vector3 vPos;
            if (pTerrain && pTerrain->Pick(point.x, point.y, &vPos)) {
                m_PCursorMode = PCM_CREATE;

                vMouseStrPos = vPos; //    Enter the first point

                DWORD color = 0xffffff00; //  Line color to be drawn
                for (int i = 0; i < 5; ++i) {
                    m_CreateLine[i].Set(vPos, color); //  reset
                }

                return TRUE;
            }
        }
    } break;
    case WM_RBUTTONUP: {
        if (bCtrlKeyState) { //  Rotates at a selected point in the pond
            return TRUE;
        }

        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (PCM_CREATE == m_PCursorMode) { // Added new pond
            m_PCursorMode = PCM_NONE;
            ReleaseCapture();

            __Vector3 vRayDir,
                vRayOrig; // Direction and origin of the straight line connecting the screen center (viewpoint) and the mouse pointer
            __Vector3 vPN, vPV; // Plane normal and contained points
            __Vector3 vPos;     // Point where the above plane meets the straight line (point to find)

            vPN.Set(0, 1, 0);
            vPV = vMouseStrPos;
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig);
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT;

            ReSetDrawRect(vMouseStrPos, vPos);

            CPondMesh * pRM = CreateNewPondMesh(); // new pond
            SetSelPond(pRM);
            return TRUE;
        }
    } break;

    case WM_KEYDOWN:
        switch (pMsg->wParam) {
        case VK_CONTROL:
            if (bCtrlKeyState != 0) {
                break;
            }
            bCtrlKeyState++;
            SetVtxBackup();
            break;
        }
        break;
    case WM_KEYUP:
        switch (pMsg->wParam) {
        case VK_CONTROL:
            if (bCtrlKeyState > 0) {
                bCtrlKeyState--;
            }
            SetVtxBackup();
            break;
        }
        break;

    default: {
        if (bCtrlKeyState > 0) {
            if (!(GetAsyncKeyState(VK_CONTROL) &
                  0xff00)) //  If it is checked and removed, it will return to its original state.
            {
                if (bCtrlKeyState != 0) {
                    bCtrlKeyState = 0;
                }
                return FALSE;
            }

            if (GetAsyncKeyState('A') & 0xff00) { //  Targets all points in the currently selected pond(s)
                MovePond();
            }
            return TRUE;
        }

        if (!(GetAsyncKeyState(VK_SHIFT) & 0xff00)) {
            m_bShift = TRUE;
        } else if (m_bShift == TRUE) {
            m_bShift = FALSE;
        }
    } break;
    }
    return FALSE;
}

void CPondMng::ReSetDrawRect(__Vector3 vStrPos,
                             __Vector3 vEndPos) //  Create a rectangular point on the map with the two points received.
{
    if (vStrPos.x > vEndPos.x) {
        m_CreateLine[0].x = vStrPos.x;
        m_CreateLine[1].x = vEndPos.x;
        m_CreateLine[3].x = vStrPos.x;
        m_CreateLine[2].x = vEndPos.x;
    } else {
        m_CreateLine[0].x = vEndPos.x;
        m_CreateLine[1].x = vStrPos.x;
        m_CreateLine[3].x = vEndPos.x;
        m_CreateLine[2].x = vStrPos.x;
    }

    if (vStrPos.z < vEndPos.z) {
        m_CreateLine[0].z = vStrPos.z;
        m_CreateLine[1].z = vStrPos.z;
        m_CreateLine[3].z = vEndPos.z;
        m_CreateLine[2].z = vEndPos.z;
    } else {
        m_CreateLine[0].z = vEndPos.z;
        m_CreateLine[1].z = vEndPos.z;
        m_CreateLine[3].z = vStrPos.z;
        m_CreateLine[2].z = vStrPos.z;
    }

    m_CreateLine[4] = m_CreateLine[0];
}

CPondMesh * CPondMng::GetPondMesh(int iPondID) {
    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM->GetPondID() == iPondID) {
            return pRM;
        }
    }
    return NULL;
}

CPondMesh * CPondMng::GetSelPond() {
    int iSize = m_pSelPonds.size();
    if (iSize == 0) {
        return NULL;
    }

    return *(m_pSelPonds.begin());
}

it_PondMesh CPondMng::GetDrawPond() {
    int iSize = m_PondMeshes.size();
    if (iSize == 0) {
        return {};
    }

    it_PondMesh it = m_PondMeshes.begin();
    return it;
}

void CPondMng::SetSelPond(CPondMesh * pPondMesh, BOOL bChooseGroup) {
    int iSize = m_pSelPonds.size();
    if (iSize == 0 && pPondMesh != NULL) {
        m_VtxPosDummy.SetSelVtx(NULL);
        m_pSelPonds.push_back(pPondMesh);
    } else if (pPondMesh == NULL && bChooseGroup == FALSE && m_bChooseEditPond == FALSE) {
        SelPondRelease();
    }

    if (bChooseGroup == TRUE && pPondMesh != NULL && m_bChooseEditPond == FALSE) {
        SetSelPonds(pPondMesh);
    }
    if (m_pDlgProperty) {
        m_pDlgProperty->UpdateInfo();
    }
}

BOOL CPondMng::SetPondID(CPondMesh * pPondMesh, int iPondID) {
    if (pPondMesh == NULL) {
        return FALSE;
    }
    if (pPondMesh->GetPondID() == iPondID) {
        return TRUE;
    }
    if (GetPondMesh(iPondID) == NULL) {
        pPondMesh->SetPondID(iPondID);
        return TRUE;
    }
    return FALSE;
}

void CPondMng::SetEditMode(BOOL bEditMode) {
    m_bEditMode = bEditMode;
    if (m_bEditMode) {
        if (m_pDlgProperty) {
            m_pDlgProperty->ShowWindow(TRUE);
        }
    } else {
        SetSelPond(NULL);
        if (m_pDlgProperty) {
            m_pDlgProperty->ShowWindow(FALSE);
        }
        m_PCursorMode = PCM_NONE;
    }
}

void CPondMng::ClearSelectRcAllPond() {
    CPondMesh * pRM = NULL;
    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        pRM = *it;
        if (pRM) {
            pRM->ClearSelectPos();
        }
    }
}

BOOL CPondMng::SelectVtxByDragRect(RECT * pRect, BOOL bAdd, BOOL bSelectPond) {
    ClearSelectRcAllPond();
    if (bSelectPond == TRUE) {
        m_SelVtxArray.RemoveAll();
    } else {
        if (pRect == NULL) {
            return FALSE;
        }
        if (bAdd == FALSE) {
            m_SelVtxArray.RemoveAll();
            SetSelPond(NULL); // Clear selected pond..
        }
    }

    CN3EngTool *      pEng = m_pMainFrm->m_pEng;
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    __Matrix44 matView, matProj, matVP;
    pD3DDev->GetTransform(D3DTS_VIEW, &matView);
    pD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixMultiply(&matVP, &matView, &matProj);

    D3DVIEWPORT9 vp = pEng->s_CameraData.vp;

    CPondMesh * pSelPond = NULL;
    int         iSize = m_pSelPonds.size();
    it_PondMesh it = m_pSelPonds.begin();
    for (int i = 0; i < iSize; ++i, ++it) // If there is already a pond selected...
    {
        pSelPond = *it;
        if (pSelPond == NULL) {
            continue;
        }

        int iVC = pSelPond->VertexCount(); // Find the number of points in the pond
        for (int k = 0; k < iVC; ++k) {
            __VertexXyzT2 * pVtx = pSelPond->GetVertex(k); // find one point
            if (pVtx == NULL) {
                continue;
            }

            if (bSelectPond == FALSE) {
                D3DXVECTOR4 v;
                D3DXVec3Transform(&v, (D3DXVECTOR3 *)(pVtx), &matVP);
                float fScreenZ = (v.z / v.w);
                if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                    continue;
                }

                float fScreenX = ((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f;
                float fScreenY = (1.0f - (v.y / v.w)) * (vp.Height) / 2.0f;
                if (fScreenX >= pRect->left && fScreenX <= pRect->right && fScreenY >= pRect->top &&
                    fScreenY <= pRect->bottom) {
                    BOOL bAleadySelected = FALSE;
                    int  j, ivtxSize = m_SelVtxArray.GetSize();
                    for (j = 0; j < ivtxSize; ++j) {
                        if (m_SelVtxArray.GetAt(j) == pVtx) {
                            bAleadySelected = TRUE;
                            break;
                        }
                    }
                    if (bAleadySelected) {
                        m_SelVtxArray.RemoveAt(j); // already exists, so remove from selection list
                    } else {
                        m_SelVtxArray.InsertAt(0, pVtx); // addition
                    }

                    pSelPond->InputSelectPos(pVtx->x, pVtx->y, pVtx->z,
                                             k); // Virtual area noise by inputting coordinates
                }
            } else {
                BOOL bAleadySelected = FALSE;
                int  j, ivtxSize = m_SelVtxArray.GetSize();
                for (j = 0; j < ivtxSize; ++j) {
                    if (m_SelVtxArray.GetAt(j) == pVtx) {
                        bAleadySelected = TRUE;
                        break;
                    }
                }
                if (bAleadySelected) {
                    m_SelVtxArray.RemoveAt(j); // already exists, so remove from selection list
                } else {
                    m_SelVtxArray.InsertAt(0, pVtx); // addition
                }

                pSelPond->InputSelectPos(pVtx->x, pVtx->y, pVtx->z); //  Virtual area noise by inputting coordinates
            }
        }
    }

    if (iSize ==
        0) // If there are no selected ponds (search all ponds, select a pond, and then select only those pond points...)
    {
        ASSERT(m_SelVtxArray.GetSize() == 0);

        it = m_PondMeshes.begin();
        iSize = m_PondMeshes.size();
        BOOL bChkSamePond;
        for (int i = 0; i < iSize; ++i, ++it) {
            CPondMesh * pRM = *it;
            if (pRM == NULL) {
                continue;
            }

            int iVC = pRM->VertexCount(); // Number of points in this pond
            pSelPond = NULL;
            bChkSamePond = TRUE;
            for (int j = 0; j < iVC; ++j) {
                __VertexXyzT2 * pVtx = pRM->GetVertex(j); // find one point
                if (pVtx == NULL) {
                    continue;
                }

                D3DXVECTOR4 v;
                D3DXVec3Transform(&v, (D3DXVECTOR3 *)(pVtx), &matVP);
                float fScreenZ = (v.z / v.w);
                if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                    continue;
                }

                float fScreenX = ((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f;
                float fScreenY = (1.0f - (v.y / v.w)) * (vp.Height) / 2.0f;
                if (fScreenX >= pRect->left && fScreenX <= pRect->right && fScreenY >= pRect->top &&
                    fScreenY <= pRect->bottom) {
                    m_SelVtxArray.Add(pVtx);                           // addition
                    pRM->InputSelectPos(pVtx->x, pVtx->y, pVtx->z, j); // Virtual area noise by inputting coordinates
                    if (bChkSamePond == TRUE) {
                        pSelPond = pRM;
                        SetSelPond(pSelPond, m_bChooseGroup);
                        bChkSamePond = FALSE;
                    }
                }
            }
            if (pSelPond && m_bChooseGroup == FALSE) {
                break;
            }
        }
    }

    iSize = m_SelVtxArray.GetSize();
    if (iSize == 0) {
        SetSelPond(NULL);
        m_VtxPosDummy.SetSelVtx(NULL);
    } else {
        m_VtxPosDummy.SetSelVtx(m_SelVtxArray.GetAt(0));
        for (int i = 1; i < iSize; ++i) {
            m_VtxPosDummy.AddSelVtx(m_SelVtxArray.GetAt(i));
        }
        SetVtxBackup();
        return TRUE;
    }

    return FALSE;
}

void CPondMng::ReCalcUV() {
    int iSize = m_pSelPonds.size();
    if (iSize == 0) {
        return;
    }
    it_PondMesh it = m_pSelPonds.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pSelPond = *it;
        pSelPond->ReCalcUV();
    }
    m_pMainFrm->Invalidate(FALSE);
}

void CPondMng::MakeGameFiles(HANDLE hFile, float fSize) {
    int   iPondCount = m_PondMeshes.size();
    DWORD dwNum;

    it_PondMesh it = m_PondMeshes.begin();
    WriteFile(hFile, &iPondCount, sizeof(int), &dwNum, NULL);
    for (int i = 0; i < iPondCount; i++, it++) {
        CPondMesh * pRM = *it;
        ASSERT(pRM);

        int            iVC = pRM->VertexCount();
        __VertexXyzT2 *pVtx0 = pRM->GetVertex(0), *pSrcVtx = NULL;
        ASSERT(pVtx0);
        WriteFile(hFile, &iVC, sizeof(iVC), &dwNum, NULL); // number of points

        if (iVC <= 0) {
            continue;
        }

        int iWidthVtxNum = pRM->GetWaterScaleWidht();
        WriteFile(hFile, &iWidthVtxNum, sizeof(int), &dwNum, NULL); // number of points

        CN3Texture * pPondTex = pRM->TexGet();
        int          iLen = 0;

        if (pPondTex) {
            char szFileName[MAX_PATH], szFindName[50];
            sprintf(szFileName, "%s", pPondTex->FileName().c_str());
            iLen = pPondTex->FileName().size();
            for (int i = iLen; i > 0; --i) {
                if (szFileName[i] == '\\') {
                    sprintf(szFindName, "%s", &szFileName[i + 1]);
                    iLen -= i;
                    i = 0;
                }
            }
            WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture file name length
            if (iLen > 0) {
                WriteFile(hFile, szFindName, iLen, &dwNum, NULL); // texture file name
            }
        } else {
            WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // texture file name length
        }

        // XyxT2 -> XyzColorT2 Converting.
        DWORD        dwAplha = pRM->GetAlphaFactor();
        __VertexPond __vTemp;
        for (int k = 0; k < iVC; ++k) {
            pSrcVtx = pVtx0 + k;
            __vTemp.Set(*pSrcVtx, 0.0f, 1.0f, 0.0f, dwAplha);
            WriteFile(hFile, &__vTemp, sizeof(__VertexPond), &dwNum, NULL); // vertex buffer
        }

        int iIC = pRM->IndexCount();
        WriteFile(hFile, &iIC, sizeof(iIC), &dwNum, NULL); // IndexBuffer Count.
    }
}

void CPondMng::ReCalcSelectedVertex() {
    int iSize = m_pSelPonds.size();
    if (iSize == 0) {
        return;
    }

    it_PondMesh it = m_pSelPonds.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh *     pSelPond = *it;
        __VertexXyzT2 * pVtx0 = pSelPond->GetVertex(0);
        __VertexXyzT2 * pVtxSel = m_SelVtxArray.GetAt(0);
        int             nIndex = pVtxSel - pVtx0;
        int             iLastVtxNum = pSelPond->LastVertexCount();

        nIndex = (nIndex / iLastVtxNum) * iLastVtxNum;
        pVtxSel = pSelPond->GetVertex(nIndex);
        ASSERT(pSelPond->VertexCount() - iLastVtxNum >= nIndex);
        ASSERT(pVtxSel);

        __Vector3 vPos1, vPos2, vDif;
        vPos1 = *(pVtxSel);
        vPos2 = *(pVtxSel + iLastVtxNum - 1);

        vDif = vPos2 - vPos1;
        float Length = vDif.Magnitude();
        vDif.Normalize();
        vDif *= Length / (float)iLastVtxNum;

        for (int i = 1; i < iLastVtxNum; i++) {
            vPos2 = vPos1 + vDif * (float)i;
            (pVtxSel + i)->Set(vPos2, 0, 0, 0, 0);
        }
        pSelPond->ReCalcUV();
    }

    m_pMainFrm->Invalidate(FALSE);
}

void CPondMng::SetVtxCenter() //  Find the midpoint of the pond(s) and set it, and also back up the old scale.
{
    int iSize = m_pSelPonds.size();
    m_vPondsCenter.Zero();

    if (iSize == 0) {
        return;
    }

    __Vector3   vtotCenter;
    __Vector3 * pvCenter;
    int         nCenterCnt = 0;

    pvCenter = new __Vector3[iSize];

    it_PondMesh it = m_pSelPonds.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM) {
            pvCenter[i] = pRM->GetCenter(); // Receive midpoint of pond(s), also backing up current scale
            nCenterCnt++;
        }
    }

    if (iSize == 1) {
        m_vPondsCenter = pvCenter[0];
    } else {
        float Stx, Enx, Stz, Enz;
        Stx = Enx = pvCenter[0].x, Stz = Enz = pvCenter[0].z;
        for (int i = 0; i < nCenterCnt; i++) {
            if (Stx > pvCenter[i].x) {
                Stx = pvCenter[i].x;
            }
            if (Enx < pvCenter[i].x) {
                Enx = pvCenter[i].x;
            }
            if (Stz > pvCenter[i].z) {
                Stz = pvCenter[i].z;
            }
            if (Enz < pvCenter[i].z) {
                Enz = pvCenter[i].z;
            }
        }

        m_vPondsCenter.Set(Stx + (Enx - Stx) / 2, 0.0f, Stz + (Enz - Stz) / 2);
    }

    delete[] pvCenter;
    pvCenter = NULL;
}

void CPondMng::SetRotatePonds(float fMove) {
    int iSize = m_pSelPonds.size();
    if (iSize == 0) {
        return;
    }
    if (m_vPondsCenter.x == 0.0f && m_vPondsCenter.z == 0.0f) {
        return;
    }

    if (m_bShift) {
        fMove /= 5.0f;
    }

    __Matrix44 matRotate;
    matRotate.RotationY(D3DXToRadian(fMove / 10.0f));

    m_VtxPosDummy.PosRotate(matRotate, m_vPondsCenter);

    it_PondMesh it = m_pSelPonds.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        if (pRM) {
            pRM->Rotation(matRotate, m_vPondsCenter);
        }
    }
}

void CPondMng::SetVtxBackup() {
    int iSelSize = m_SelVtxArray.GetSize();
    if (iSelSize == 0) {
        return;
    }

    VtxBackupRelease();

    for (int i = 0; i < iSelSize; i++) {
        __VertexXyzT2 * pVtx = m_SelVtxArray.GetAt(i);

        __Vector3 * pvVtx = new __Vector3;
        pvVtx->x = pVtx->x;
        pvVtx->y = pVtx->y;
        pvVtx->z = pVtx->z;
        m_SelVtxBakArray.push_back(pvVtx);
    }
}

void CPondMng::ReSetVtxBackup() {
    int iSize = m_SelVtxBakArray.size();
    int iVtxSize = m_SelVtxArray.GetSize();
    if (iSize == 0 || iVtxSize == 0) {
        return;
    }

    it_SelVtxBak SetVtx = m_SelVtxBakArray.begin();
    for (int i = 0; i < iSize; i++, SetVtx++) {
        __Vector3 *     pBacVtx = *SetVtx;
        __VertexXyzT2 * pVtx = m_SelVtxArray.GetAt(i);
        if (pBacVtx) {
            pVtx->x = pBacVtx->x;
            pVtx->y = pBacVtx->y;
            pVtx->z = pBacVtx->z;
        }
    }
}

void CPondMng::VtxBackupRelease() {
    int iSize = m_SelVtxBakArray.size();
    if (iSize == 0) {
        return;
    }

    it_SelVtxBak SetVtx = m_SelVtxBakArray.begin();
    for (int i = 0; i < iSize; i++, SetVtx++) {
        __Vector3 * pVtx = *SetVtx;
        if (pVtx) {
            delete pVtx;
        }
    }
    m_SelVtxBakArray.clear();
}

void CPondMng::InputDummyMovePos(__Vector3 vMovePos) {
    if (vMovePos.x == 0.0f && vMovePos.y == 0.0f && vMovePos.z == 0.0f) {
        return;
    }

    int         iSize = m_pSelPonds.size();
    it_PondMesh it = m_pSelPonds.begin();
    CPondMesh * pRM;
    BOOL        bDrawBoxMove;

    for (int i = 0; i < iSize; ++i, ++it) {
        pRM = *it;
        if (pRM) {
            bDrawBoxMove = pRM->InputDummyMovingPos(vMovePos, m_bMovePond);
            if (bDrawBoxMove == TRUE) {
                m_pDlgProperty->UpdateWaterLength(pRM);
            }
        }
    }

    if (m_pDlgProperty && (vMovePos.y != 0.0f || m_bMovePond == TRUE)) // Update information by height or movement
    {
        m_pDlgProperty->UpdateInfo();
        MainInvalidate();
    }
}

void CPondMng::StationPond() {
    CPondMesh * pRM = NULL;
    it_PondMesh it = m_pSelPonds.begin();
    int         iSize = m_pSelPonds.size();
    for (int i = 0; i < iSize; i++, it++) {
        pRM = *it;
        if (pRM) {
            pRM->MakePondPos();
        }
    }

    MainInvalidate();
}

void CPondMng::MovePond() {
    SelectVtxByDragRect(NULL, FALSE, TRUE); // First, select all points in all currently selected ponds.
    m_bMovePond = TRUE;
    MainInvalidate();
}