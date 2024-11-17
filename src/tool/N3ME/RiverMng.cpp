// RiverMng.cpp: implementation of the CRiverMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3ME.h"
#include "RiverMng.h"
#include "RiverMesh.h"
#include "DlgRiverProperty.h"
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

CRiverMng::CRiverMng(CMainFrame * pMainFrm) {
    m_pMainFrm = pMainFrm;
    m_pDlgProperty = new CDlgRiverProperty(this);
    Release();
    m_pDlgProperty->Create(IDD_RIVER_PROPERTY, m_pMainFrm);
}

CRiverMng::~CRiverMng() {
    Release();
    if (m_pDlgProperty) {
        m_pDlgProperty->DestroyWindow();
        delete m_pDlgProperty;
        m_pDlgProperty = NULL;
    }
}

void CRiverMng::Release() {
    it_RiverMesh it = m_RiverMeshes.begin();
    int          iSize = m_RiverMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CRiverMesh * pRM = *it;
        if (pRM) {
            delete pRM;
        }
    }
    m_RiverMeshes.clear();

    m_SelVtxArray.RemoveAll();
    m_pSelRiver = NULL;
    m_bEditMode = FALSE;
    m_RCursorMode = RCM_NONE;
    m_VtxPosDummy.Release();
}

bool CRiverMng::Load(HANDLE hFile) {
    Release();

    DWORD dwNum;
    int   iRiverMeshCount;
    ReadFile(hFile, &iRiverMeshCount, sizeof(iRiverMeshCount), &dwNum, NULL);
    for (int i = 0; i < iRiverMeshCount; ++i) {
        CRiverMesh * pRvrMesh = new CRiverMesh;
        pRvrMesh->Load(hFile);
        m_RiverMeshes.push_back(pRvrMesh);
    }
    return 0;
}

bool CRiverMng::Save(HANDLE hFile) {
    DWORD dwNum;
    int   iSize = m_RiverMeshes.size();
    WriteFile(hFile, &iSize, 4, &dwNum, NULL);

    it_RiverMesh it = m_RiverMeshes.begin();
    for (int i = 0; i < iSize; i++, it++) {
        CRiverMesh * pRM = *it;
        pRM->Save(hFile);
    }
    return 0;
}

void CRiverMng::Tick() {
    m_VtxPosDummy.Tick();
}

void CRiverMng::Render() {
    HRESULT hr;
    // backup state
    DWORD dwZEnable, dwLighting;
    hr = s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    hr = s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);

    // 기존에 있던 강 그리기
    it_RiverMesh it = m_RiverMeshes.begin();
    int          iSize = m_RiverMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CRiverMesh * pRM = *it;
        pRM->Render();
    }

    if (m_bEditMode) {
        // 강 새로 만드는 중이면 드래그 선 그리기
        if (m_RCursorMode == RCM_CREATE) {
            // set state
            hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
            hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

            __Matrix44 matWorld;
            matWorld.Identity();
            s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

            // set texture
            hr = s_lpD3DDev->SetTexture(0, NULL);
            hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

            // Render
            s_lpD3DDev->SetFVF(FVF_XYZCOLOR);
            s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, m_CreateLine, sizeof(__VertexXyzColor));
        }

        // dummy 상자 그리기
        m_VtxPosDummy.Render();

        if (m_pSelRiver) {
            // 선택된 강의 점그리기 (빨강)
            m_pSelRiver->RenderVertexPoint();

            int iVC = m_pSelRiver->VertexCount();
            if (iVC > 4) {
                // 선택된 강의 Extrude될 변 표시하기
                // backup state
                __Material BackupMtrl;
                s_lpD3DDev->GetMaterial(&BackupMtrl);

                // set material
                D3DCOLORVALUE color;
                color.a = 1.0f;
                color.r = 1.0f;
                color.g = 0.0f;
                color.b = 0.0f;
                __Material mtrl;
                mtrl.Init(color);
                s_lpD3DDev->SetMaterial(&mtrl);

                // set state
                hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
                hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

                __Matrix44 matWorld;
                matWorld.Identity();
                s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

                // set texture
                hr = s_lpD3DDev->SetTexture(0, NULL);
                hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
                hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

                // render
                __VertexXyzT2 * pVtx = m_pSelRiver->GetVertex(iVC - 4);
                s_lpD3DDev->SetFVF(FVF_XYZT2);
                s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 3, pVtx, sizeof(__VertexXyzT2));
                //s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, pVtx, sizeof(__VertexXyzT2));

                // restore material
                s_lpD3DDev->SetMaterial(&BackupMtrl);
            }
        }

        // 선택된 점 그리기 (초록)
        int iSize = m_SelVtxArray.GetSize();
        if (iSize > 0) {
            // transform
            __Matrix44 matView, matProj, matVP;
            s_lpD3DDev->GetTransform(D3DTS_VIEW, &matView);
            s_lpD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
            D3DXMatrixMultiply(&matVP, &matView, &matProj);
            D3DVIEWPORT9 vp = s_CameraData.vp;

            __VertexTransformedColor Vertices[4];
            D3DCOLOR                 clr = D3DCOLOR_ARGB(0xff, 0x00, 0xff, 0x00);
            s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);

            for (int i = 0; i < iSize; ++i) {
                __VertexXyzT2 * pVtx = m_SelVtxArray.GetAt(i);
                if (pVtx == NULL) {
                    continue;
                }
                D3DXVECTOR4 v;
                D3DXVec3Transform(&v, pVtx, &matVP);

                float fScreenZ = (v.z / v.w);
                if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                    continue;
                }

                int iScreenX = int(((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f);
                int iScreenY = int((1.0f - (v.y / v.w)) * (vp.Height) / 2.0f);
                if (iScreenX >= (int)vp.X && iScreenX <= (int)vp.Width && iScreenY >= (int)vp.Y &&
                    iScreenY <= (int)vp.Height) {
                    // set X (점을 찍으면 1픽셀밖에 안찍으므로 X표시를 그린다.
                    Vertices[0].Set(float(iScreenX - 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
                    Vertices[1].Set(float(iScreenX + 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
                    Vertices[2].Set(float(iScreenX + 2), float(iScreenY - 2), 0.5f, 0.5f, clr);
                    Vertices[3].Set(float(iScreenX - 2), float(iScreenY + 2), 0.5f, 0.5f, clr);
                    // render
                    s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, Vertices, sizeof(__VertexTransformedColor));
                }
            }
        }

        // 드래그 영역 그리기
        if (RCM_SELECT == m_RCursorMode) {
            m_pMainFrm->GetMapMng()->RenderDragRect(&m_rcSelDrag);
        }
    }

    // restore
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
}

CRiverMesh * CRiverMng::CreateNewRiverMesh(__Vector3 & vPos1, __Vector3 & vPos2, __Vector3 & vPos3, __Vector3 & vPos4) {
    CRiverMesh * pRM = new CRiverMesh;
    pRM->AddVertex(vPos1, vPos2, vPos3, vPos4);
    m_pSelRiver = pRM;

    int iID = 0;
    while (SetRiverID(pRM, iID) == FALSE) {
        iID++;
    }

    CDlgRiverProperty dlg(this);
    dlg.m_IsModalDialog = TRUE;

    if (dlg.DoModal() == IDCANCEL) {
        delete pRM;
        pRM = NULL;
        m_pSelRiver = NULL;
    }
    if (pRM) {
        m_RiverMeshes.push_back(pRM);
    }
    return pRM;
}

void CRiverMng::RemoveRiverMesh(int iRiverID) {
    it_RiverMesh it = m_RiverMeshes.begin();
    int          iSize = m_RiverMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CRiverMesh * pRM = *it;
        if (pRM && pRM->GetRiverID() == iRiverID) {
            delete pRM;
            it = m_RiverMeshes.erase(it);
            m_SelVtxArray.RemoveAll();
            m_VtxPosDummy.SetSelVtx(NULL);
            m_pSelRiver = NULL;
            m_pMainFrm->Invalidate();
            return;
        }
    }
}

BOOL CRiverMng::MouseMsgFilter(LPMSG pMsg) {
    if (FALSE == m_bEditMode) {
        return FALSE;
    }
    if (m_VtxPosDummy.MouseMsgFilter(pMsg)) {
        return TRUE;
    }

    static POINT ptDownBuff;

    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (RCM_CREATE == m_RCursorMode) { // 새로운 강 추가할때 드래그 하는 선 설정
            __Vector3 vRayDir, vRayOrig; // 화면 중앙(시점)과 마우스 포인터를 이은 직선의 방향과 원점
            __Vector3 vPN, vPV;          // 평면의 법선과 포함된 점
            __Vector3 vPos;              // 위의 평면과 직선의 만나는 점(구할 점)

            vPN.Set(0, 1, 0);
            vPV = m_CreateLine[0];
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig); // 이함수 잠시 빌려씀.
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT;
            m_CreateLine[1] = vPos;
            return TRUE;
        } else if (RCM_SELECT == m_RCursorMode) {
            m_rcSelDrag.right = point.x;
            m_rcSelDrag.bottom = point.y;
            return TRUE;
        }
    } break;
    case WM_LBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (RCM_CREATE == m_RCursorMode) { // 새로운 강 추가 취소
            m_RCursorMode = RCM_NONE;
            ReleaseCapture();
            return TRUE;
        } else if (RCM_NONE == m_RCursorMode) {
            m_RCursorMode = RCM_SELECT;
            SetCapture(pMsg->hwnd);

            m_rcSelDrag.right = m_rcSelDrag.left = point.x;
            m_rcSelDrag.bottom = m_rcSelDrag.top = point.y;
            return TRUE;
        }
    } break;
    case WM_LBUTTONUP: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (RCM_SELECT == m_RCursorMode) {
            m_RCursorMode = RCM_NONE;
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

            // 드레그가 아니고 그냥 클릭일경우 드래그 영역을 3x3정도로 잡아준다.
            if (m_rcSelDrag.right - m_rcSelDrag.left < 3 && m_rcSelDrag.bottom - m_rcSelDrag.top < 3) {
                m_rcSelDrag.left = point.x - 1;
                m_rcSelDrag.right = point.x + 1;
                m_rcSelDrag.top = point.y - 1;
                m_rcSelDrag.bottom = point.y + 1;
            }
            SelectVtxByDragRect(&m_rcSelDrag, (pMsg->wParam & MK_SHIFT) ? TRUE : FALSE);

            return TRUE;
        }
    } break;
    case WM_RBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (RCM_SELECT == m_RCursorMode) { // Select 취소
            m_RCursorMode = RCM_NONE;
            ReleaseCapture();
            return TRUE;
        } else if (RCM_NONE == m_RCursorMode) { // 새로운 강 추가
            CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();

            __Vector3 vPos;
            if (pTerrain && pTerrain->Pick(point.x, point.y, &vPos)) {
                m_RCursorMode = RCM_CREATE;

                DWORD color = 0xffff0000;
                m_CreateLine[0].Set(vPos, color);
                m_CreateLine[1].Set(vPos, color);

                SetCapture(pMsg->hwnd);
                return TRUE;
            }
        }
    } break;
    case WM_RBUTTONUP: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (RCM_CREATE == m_RCursorMode) { // 새로운 강 추가
            m_RCursorMode = RCM_NONE;
            ReleaseCapture();

            __Vector3 vRayDir, vRayOrig; // 화면 중앙(시점)과 마우스 포인터를 이은 직선의 방향과 원점
            __Vector3 vPN, vPV;          // 평면의 법선과 포함된 점
            __Vector3 vPos;              // 위의 평면과 직선의 만나는 점(구할 점)

            vPN.Set(0, 1, 0);
            vPV = m_CreateLine[0];
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig); // 이함수 잠시 빌려씀.
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT;
            m_CreateLine[1] = vPos;

            __Vector3 vPos1, vPos2, vPos3, vPos4;
            vPos1 = m_CreateLine[0];
            vPos4 = m_CreateLine[1];
            vPos2 = vPos1 * (1.0f - 0.33f) + vPos4 * 0.33f;
            vPos3 = vPos1 * (1.0f - 0.66f) + vPos4 * 0.66f;

            CRiverMesh * pRM = CreateNewRiverMesh(vPos1, vPos2, vPos3, vPos4);
            SetSelRiver(pRM);
            return TRUE;
        }
    } break;
    }
    return FALSE;
}

CRiverMesh * CRiverMng::GetRiverMesh(int iRiverID) {
    it_RiverMesh it = m_RiverMeshes.begin();
    int          iSize = m_RiverMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CRiverMesh * pRM = *it;
        if (pRM->GetRiverID() == iRiverID) {
            return pRM;
        }
    }
    return NULL;
}

void CRiverMng::SetSelRiver(CRiverMesh * pRiverMesh) {
    m_VtxPosDummy.SetSelVtx(NULL);
    m_pSelRiver = pRiverMesh;
    if (m_pDlgProperty) {
        m_pDlgProperty->UpdateInfo();
    }
}

BOOL CRiverMng::SetRiverID(CRiverMesh * pRiverMesh, int iRiverID) {
    if (pRiverMesh == NULL) {
        return FALSE;
    }
    if (pRiverMesh->GetRiverID() == iRiverID) {
        return TRUE;
    }
    if (GetRiverMesh(iRiverID) == NULL) {
        pRiverMesh->SetRiverID(iRiverID);
        return TRUE;
    }
    return FALSE;
}

void CRiverMng::SetEditMode(BOOL bEditMode) {
    m_bEditMode = bEditMode;
    if (m_bEditMode) {
        if (m_pDlgProperty) {
            m_pDlgProperty->ShowWindow(TRUE);
        }
    } else {
        SetSelRiver(NULL);
        if (m_pDlgProperty) {
            m_pDlgProperty->ShowWindow(FALSE);
        }
        m_RCursorMode = RCM_NONE;
    }
}

void CRiverMng::SelectVtxByDragRect(RECT * pRect, BOOL bAdd) {
    if (pRect == NULL) {
        return;
    }
    if (bAdd == FALSE) {
        m_SelVtxArray.RemoveAll();
        SetSelRiver(NULL); // 선택한강 해제..
    }

    CN3EngTool *      pEng = m_pMainFrm->m_pEng;
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    __Matrix44 matView, matProj, matVP;
    pD3DDev->GetTransform(D3DTS_VIEW, &matView);
    pD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixMultiply(&matVP, &matView, &matProj);

    D3DVIEWPORT9 vp = pEng->s_CameraData.vp;

    if (m_pSelRiver) // 이미 선택된 강이 있다면..
    {
        int iVC = m_pSelRiver->VertexCount(); // 그강의 점 숫자를 구하기
        for (int i = 0; i < iVC; ++i) {
            __VertexXyzT2 * pVtx = m_pSelRiver->GetVertex(i); // 점 하나 구하기
            if (pVtx == NULL) {
                continue;
            }

            D3DXVECTOR4 v;
            D3DXVec3Transform(&v, pVtx, &matVP);
            float fScreenZ = (v.z / v.w);
            if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                continue;
            }

            float fScreenX = ((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f;
            float fScreenY = (1.0f - (v.y / v.w)) * (vp.Height) / 2.0f;
            if (fScreenX >= pRect->left && fScreenX <= pRect->right && fScreenY >= pRect->top &&
                fScreenY <= pRect->bottom) {
                BOOL bAleadySelected = FALSE;
                int  j, iSize = m_SelVtxArray.GetSize();
                for (j = 0; j < iSize; ++j) {
                    if (m_SelVtxArray.GetAt(j) == pVtx) {
                        bAleadySelected = TRUE;
                        break;
                    }
                }
                if (bAleadySelected) {
                    m_SelVtxArray.RemoveAt(j); // 이미 있으므로 선택목록에서 제거
                } else {
                    m_SelVtxArray.InsertAt(0, pVtx); // 추가
                }
            }
        }
    } else // 선택된 강이 아무것도 없다면 (모든강 검색해서 맨처음 골라지는 강 선택후 그 강 점들만 선택..)
    {
        ASSERT(m_SelVtxArray.GetSize() == 0);

        CRiverMesh * pSelRiver = NULL; // 선택된 강
        it_RiverMesh it = m_RiverMeshes.begin();
        int          iSize = m_RiverMeshes.size();
        for (int i = 0; i < iSize; i++, it++) {
            CRiverMesh * pRM = *it;
            if (pRM == NULL) {
                continue;
            }

            int iVC = pRM->VertexCount(); // 이강의 점 갯수
            for (int j = 0; j < iVC; ++j) {
                __VertexXyzT2 * pVtx = pRM->GetVertex(j); // 점 하나 구하기
                if (pVtx == NULL) {
                    continue;
                }

                D3DXVECTOR4 v;
                D3DXVec3Transform(&v, pVtx, &matVP);
                float fScreenZ = (v.z / v.w);
                if (fScreenZ > 1.0 || fScreenZ < 0.0) {
                    continue;
                }

                float fScreenX = ((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f;
                float fScreenY = (1.0f - (v.y / v.w)) * (vp.Height) / 2.0f;
                if (fScreenX >= pRect->left && fScreenX <= pRect->right && fScreenY >= pRect->top &&
                    fScreenY <= pRect->bottom) {
                    m_SelVtxArray.Add(pVtx); // 추가
                    pSelRiver = pRM;
                    SetSelRiver(pSelRiver);
                }
            }
            if (pSelRiver) {
                break;
            }
        }
    }
    int iSize = m_SelVtxArray.GetSize();
    if (iSize == 0) {
        SetSelRiver(NULL);
        m_VtxPosDummy.SetSelVtx(NULL);
    } else {
        m_VtxPosDummy.SetSelVtx(m_SelVtxArray.GetAt(0));
        for (int i = 1; i < iSize; ++i) {
            m_VtxPosDummy.AddSelVtx(m_SelVtxArray.GetAt(i));
        }
    }
}

void CRiverMng::ExtrudeRiverEdge() {
    if (m_pSelRiver == NULL) {
        return;
    }
    int iVC = m_pSelRiver->AddVertex();

    m_SelVtxArray.RemoveAll(); // 기존에 선택한 점 해제

    if (iVC >= 4) {
        __VertexXyzT2 * pVtx = m_pSelRiver->GetVertex(iVC - 4);
        m_SelVtxArray.Add(pVtx);
        m_SelVtxArray.Add(pVtx + 1);
        m_SelVtxArray.Add(pVtx + 2);
        m_SelVtxArray.Add(pVtx + 3);

        m_VtxPosDummy.SetSelVtx(pVtx);
        //m_VtxPosDummy.AddSelVtx(pVtx);
        m_VtxPosDummy.AddSelVtx(pVtx + 1);
        m_VtxPosDummy.AddSelVtx(pVtx + 2);
        m_VtxPosDummy.AddSelVtx(pVtx + 3);
    }

    m_pMainFrm->Invalidate(FALSE);
}

void CRiverMng::DeleteSelectedVertex() // 선택된 점들 지우기
{
    if (m_pSelRiver == NULL) {
        return;
    }

    int iVC = m_pSelRiver->VertexCount();
    if (iVC <= 4) {
        AfxMessageBox("Can't delete vertex. plz press 'Delete River' if you want.");
        return;
    }

    int iSize = m_SelVtxArray.GetSize();
    if (iSize > 0) {
        // Delete line first selected vertex placed.
        __VertexXyzT2 * pVtx0 = m_pSelRiver->GetVertex(0);
        __VertexXyzT2 * pVtxSel = m_SelVtxArray.GetAt(0);
        int             iIndex = pVtxSel - pVtx0;
        iVC = m_pSelRiver->DeleteVertex(iIndex);
    }

    m_SelVtxArray.RemoveAll(); // 기존에 선택한 점 해제
    m_VtxPosDummy.SetSelVtx(NULL);
    m_pMainFrm->Invalidate(FALSE);
}

void CRiverMng::ReCalcUV() {
    if (m_pSelRiver == NULL) {
        return;
    }
    m_pSelRiver->ReCalcUV();
    m_pMainFrm->Invalidate(FALSE);
}

void CRiverMng::MakeGameFiles(HANDLE hFile, float fSize) {
    int   iRiverCount = m_RiverMeshes.size();
    DWORD dwNum;

    it_RiverMesh it = m_RiverMeshes.begin();
    WriteFile(hFile, &iRiverCount, sizeof(int), &dwNum, NULL);
    for (int i = 0; i < iRiverCount; i++, it++) {
        CRiverMesh * pRM = *it;
        ASSERT(pRM);

        int            iVC = pRM->VertexCount();
        __VertexXyzT2 *pVtx0 = pRM->GetVertex(0), *pSrcVtx = NULL;
        ASSERT(pVtx0);
        WriteFile(hFile, &iVC, sizeof(iVC), &dwNum, NULL); // 점 갯수

        // XyxT2 -> XyzColorT2 Converting.
        __VertexRiver * pTemp = new __VertexRiver[iVC];
        for (int k = 0; k < iVC; k++) {
            pSrcVtx = pVtx0 + k;
            if (k % 4 == 0 || k % 4 == 3) {
                pTemp[k].Set(pSrcVtx->x, pSrcVtx->y, pSrcVtx->z, 0.0f, 1.0f, 0.0f, 0x30ffffff, pSrcVtx->tu, pSrcVtx->tv,
                             pSrcVtx->tu2, pSrcVtx->tv2);
            } else {
                pTemp[k].Set(pSrcVtx->x, pSrcVtx->y, pSrcVtx->z, 0.0f, 1.0f, 0.0f, 0xffffffff, pSrcVtx->tu, pSrcVtx->tv,
                             pSrcVtx->tu2, pSrcVtx->tv2);
            }
        }

        if (iVC > 0) {
            WriteFile(hFile, pTemp, iVC * sizeof(__VertexRiver), &dwNum, NULL); // vertex buffer
        }
        int iIC = pRM->IndexCount();
        WriteFile(hFile, &iIC, sizeof(iIC), &dwNum, NULL); // IndexBuffer Count.
        delete[] pTemp;
        pTemp = NULL;

        int iLen = 0;

        CN3Texture * pRiverTex = pRM->TexGet();
        if (pRiverTex) {
            std::string szFile = pRiverTex->FilePath().filename().string();
            if (szFile.empty()) {
                WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
            } else {
                // The `+ 1` ensures that the null-termination byte is written to file, addressing a previous
                // issue where the algorithm mistakenly calculated the length of the string doing a reverse loop
                // to extract the filename from path.
                // Notably, this issue persists even in their official maps from the 2xxx versions, potentially
                // causing confusion or problems for those interpreting these formats.
                //
                // The root cause is that when reading strings from files, we consistently read 4 bytes
                // for the string length, followed by the string itself based on that length. If the
                // null-termination byte is read directly into, for example, a dynamically allocated container,
                // the null-byte will be included, leading to failed string comparisons, even though the string
                // values appear identical.
                //
                // Therefore to stay compatible with the official formats, we sadly include that null-byte.
                iLen = szFile.length() + 1;
                WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
                WriteFile(hFile, szFile.c_str(), iLen, &dwNum, NULL);
            }
        } else {
            WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL);
        }
    }

    /*
    // 모든 강 정보 저장 (*.grm) game river main
    int iRiverCount = m_RiverMeshes.size();

    CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();
    SIZE         size = pTerrain->GetPatchNum(fSize);

    CN3River      river;
    __RiverInfo * RiverInfos = river.CreateRiverInfo(iRiverCount);
    river.SetMaxPatchSize(size.cx, size.cy);

    it_RiverMesh it = m_RiverMeshes.begin();
    for (int i = 0; i < iRiverCount; i++, it++) {
        CRiverMesh * pRM = *it;
        ASSERT(pRM);

        RiverInfos[i].iRiverID = pRM->GetRiverID();
        RiverInfos[i].dwAlphaFactor = pRM->GetAlphaFactor();
        RiverInfos[i].fSpeed1 = pRM->GetSpeed1();
        RiverInfos[i].fSpeed2 = pRM->GetSpeed2();
        CN3Texture * pTex = pRM->TexGet();
        ASSERT(pTex);
        if (pTex) {
            RiverInfos[i].SetTexName(pTex->Name());
        }

        // animation texture
        RiverInfos[i].fAnimTexFPS = pRM->GetAnimTexFPS();
        int iAnimTexCount = pRM->GetAnimTexCount();
        RiverInfos[i].SetAnimTexCount(iAnimTexCount);
        for (int j = 0; j < iAnimTexCount; ++j) {
            __ASSERT(pRM->AnimTexGet(j), "");
            RiverInfos[i].SetAnimTexName(j, pRM->AnimTexGet(j)->Name());
        }
    }
    river.Save(hFile);

    if (iRiverCount <= 0) {
        return;
    }

    // 각각의 패치 정보 저장 (*.grp) game river patch
    int iPatchCount = size.cx * size.cy;
    // 각 패치별로 정보 분류하기
    __TempPatch * TempPatches = new __TempPatch[iPatchCount];

    it = m_RiverMeshes.begin();
    for (int i = 0; i < iRiverCount; i++, it++) {
        CRiverMesh * pRM = *it;
        int          iVC = pRM->VertexCount();
        for (int j = 0; j < iVC; ++j) {
            __VertexXyzT2 * pVtx = pRM->GetVertex(j);
            int             iX = int(pVtx->x / fSize);
            int             iZ = int(pVtx->z / fSize);
            int             iPatchPos = iZ * size.cx + iX;
            __TempRiver *   pTempRiver = TempPatches[iPatchPos].GetRiver(pRM->GetRiverID());
            if (pTempRiver == NULL) {
                pTempRiver = new __TempRiver;
                pTempRiver->iRiverID = pRM->GetRiverID();
                TempPatches[iPatchPos].RiverArray.Add(pTempRiver);
            }
            __TempVertex * pTempVtx = new __TempVertex;
            pTempVtx->index = j;
            pTempVtx->pVtx = pVtx;
            pTempRiver->VtxArray.Add(pTempVtx);
        }
    }

    // CN3RiverPatch구조에 알맞게 넣기.
    CN3RiverPatch * RiverPatches = new CN3RiverPatch[iPatchCount];
    for (int i = 0; i < iPatchCount; ++i) {
        int       iRC = TempPatches[i].RiverArray.GetSize();
        __River * Rivers = RiverPatches[i].CreateRiver(iRC);
        for (int j = 0; j < iRC; ++j) {
            __TempRiver * pTempRiver = TempPatches[i].RiverArray.GetAt(j);
            ASSERT(pTempRiver);
            Rivers[j].iRiverID = pTempRiver->iRiverID;
            int iVC = pTempRiver->VtxArray.GetSize();
            Rivers[j].iVertexCount = iVC;

            if (iVC <= 0) {
                continue;
            }
            __VertexRiver * pVertices;
            pVertices = Rivers[j].pVertices = new __VertexRiver[iVC];
            for (int k = 0; k < iVC; ++k) {
                __TempVertex * pTempVtx = pTempRiver->VtxArray.GetAt(k);
                pVertices[k].index = pTempVtx->index;
                pVertices[k].Set(pTempVtx->pVtx->v, pTempVtx->pVtx->tu, pTempVtx->pVtx->tv, pTempVtx->pVtx->tu2,
                                 pTempVtx->pVtx->tv2);
            }
        }
    }

    // 메모리 할당한거 지우기
    delete[] TempPatches;

    // RiverPatches 저장하기
    for (int i = 0; i < size.cx; ++i) {
        for (int j = 0; j < size.cy; ++j) {
            RiverPatches[j * size.cy + i].Save(hFile);
        }
    }
    delete[] RiverPatches;
    */
}

/*
void CRiverMng::MakeGameFiles(const fs::path & fsFileStem, float fSize) {
    // 모든 강 정보 저장 (*.grm) game river main
    int iRiverCount = m_RiverMeshes.size();

    CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();
    SIZE         size = pTerrain->GetPatchNum(fSize);

    CN3River      river;
    __RiverInfo * RiverInfos = river.CreateRiverInfo(iRiverCount);
    river.SetMaxPatchSize(size.cx, size.cy);

    for (int i = 0; i < iRiverCount; ++i) {
        CRiverMesh * pRM = m_RiverMeshes.Get(i);
        ASSERT(pRM);
        RiverInfos[i].iRiverID = pRM->GetRiverID();
        RiverInfos[i].dwAlphaFactor = pRM->GetAlphaFactor();
        RiverInfos[i].fSpeed1 = pRM->GetSpeed1();
        RiverInfos[i].fSpeed2 = pRM->GetSpeed2();
        CN3Texture * pTex = pRM->TexGet();
        ASSERT(pTex);
        if (pTex) {
            RiverInfos[i].SetTexName(pTex->Name());
        }

        // animation texture
        RiverInfos[i].fAnimTexFPS = pRM->GetAnimTexFPS();
        int iAnimTexCount = pRM->GetAnimTexCount();
        RiverInfos[i].SetAnimTexCount(iAnimTexCount);
        for (int j = 0; j < iAnimTexCount; ++j) {
            __ASSERT(pRM->AnimTexGet(j), "");
            RiverInfos[i].SetAnimTexName(j, pRM->AnimTexGet(j)->Name());
        }
    }

    river.SaveToFile(("River" / fsFileStem).replace_extension(".grm"));

    if (iRiverCount <= 0) {
        return;
    }

    // 각각의 패치 정보 저장 (*.grp) game river patch
    int iPatchCount = size.cx * size.cy;
    // 각 패치별로 정보 분류하기
    __TempPatch * TempPatches = new __TempPatch[iPatchCount];

    for (int i = 0; i < iRiverCount; ++i) {
        CRiverMesh * pRM = m_RiverMeshes.Get(i);
        int          iVC = pRM->VertexCount();
        for (int j = 0; j < iVC; ++j) {
            __VertexXyzT2 * pVtx = pRM->GetVertex(j);
            int             iX = int(pVtx->x / fSize);
            int             iZ = int(pVtx->z / fSize);
            int             iPatchPos = iZ * size.cx + iX;
            __TempRiver *   pTempRiver = TempPatches[iPatchPos].GetRiver(pRM->GetRiverID());
            if (pTempRiver == NULL) {
                pTempRiver = new __TempRiver;
                pTempRiver->iRiverID = pRM->GetRiverID();
                TempPatches[iPatchPos].RiverArray.Add(pTempRiver);
            }
            __TempVertex * pTempVtx = new __TempVertex;
            pTempVtx->index = j;
            pTempVtx->pVtx = pVtx;
            pTempRiver->VtxArray.Add(pTempVtx);
        }
    }

    // CN3RiverPatch구조에 알맞게 넣기.
    CN3RiverPatch * RiverPatches = new CN3RiverPatch[iPatchCount];
    for (int i = 0; i < iPatchCount; ++i) {
        int       iRC = TempPatches[i].RiverArray.GetSize();
        __River * Rivers = RiverPatches[i].CreateRiver(iRC);
        for (int j = 0; j < iRC; ++j) {
            __TempRiver * pTempRiver = TempPatches[i].RiverArray.GetAt(j);
            ASSERT(pTempRiver);
            Rivers[j].iRiverID = pTempRiver->iRiverID;
            int iVC = pTempRiver->VtxArray.GetSize();
            Rivers[j].iVertexCount = iVC;

            if (iVC <= 0) {
                continue;
            }
            __VertexRiver * pVertices;
            pVertices = Rivers[j].pVertices = new __VertexRiver[iVC];
            for (int k = 0; k < iVC; ++k) {
                __TempVertex * pTempVtx = pTempRiver->VtxArray.GetAt(k);
                pVertices[k].index = pTempVtx->index;
                pVertices[k].Set(pTempVtx->pVtx->v, pTempVtx->pVtx->tu, pTempVtx->pVtx->tv, pTempVtx->pVtx->tu2,
                                 pTempVtx->pVtx->tv2);
            }
        }
    }

    // 메모리 할당한거 지우기
    delete[] TempPatches;

    // RiverPatches 저장하기
    for (int i = 0; i < size.cx; ++i) {
        for (int j = 0; j < size.cy; ++j) {
            fs::path fsGrpFile = "River" / fsFileStem;
            fsGrpFile += std::format("_{:02}{:02}.grp", i, j);
            RiverPatches[j * size.cy + i].SaveToFile(fsGrpFile);
        }
    }
    delete[] RiverPatches;
}
*/

void CRiverMng::ReCalcSelectedVertex() {
    if (!m_pSelRiver) {
        return;
    }

    __VertexXyzT2 * pVtx0 = m_pSelRiver->GetVertex(0);
    __VertexXyzT2 * pVtxSel = m_SelVtxArray.GetAt(0);
    int             nIndex = pVtxSel - pVtx0;
    nIndex = (nIndex / 4) * 4;

    pVtxSel = m_pSelRiver->GetVertex(nIndex);
    ASSERT(m_pSelRiver->VertexCount() - 4 >= nIndex);
    ASSERT(pVtxSel);

    __Vector3 vPos1, vPos2, vPos3, vPos4;
    vPos1 = *pVtxSel;
    vPos4 = *(pVtxSel + 3);
    vPos2 = vPos1 * (1.0f - 0.33f) + vPos4 * 0.33f;
    vPos3 = vPos1 * (1.0f - 0.66f) + vPos4 * 0.66f;

    *(pVtxSel + 1) = vPos2;
    *(pVtxSel + 2) = vPos3;
    m_pMainFrm->Invalidate(FALSE);
}

it_RiverMesh CRiverMng::GetDrawRiver() {
    int iSize = m_RiverMeshes.size();
    if (iSize == 0) {
        return {};
    }

    it_RiverMesh it = m_RiverMeshes.begin();
    return it;
}

void CRiverMng::GoRiver(int iRiverID) {
    CRiverMesh * pRiver = GetRiverMesh(iRiverID);
    if (pRiver == NULL) {
        return;
    }

    __Vector3   vRiverPos = pRiver->GetCenter();
    CN3Camera * pCamera = m_pMainFrm->GetMapMng()->CameraGet();
    if (pCamera) {
        __Vector3 vCamVector = pCamera->m_vPos - pCamera->m_vAt;
        pCamera->m_vAt = vRiverPos;
        pCamera->m_vPos = vRiverPos + vCamVector;

        SetSelRiver(pRiver);
    }
    m_pMainFrm->Invalidate();
}
