// PondMng.cpp: implementation of the CPondMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3ME.h"
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
    WriteFile(hFile, &nFileVersion, sizeof(nFileVersion), &dwNum, NULL); // 연못 번호

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

    // 기존에 있던 연못 그리기
    it_PondMesh it = m_PondMeshes.begin();
    int         iSize = m_PondMeshes.size();
    for (int i = 0; i < iSize; i++, it++) {
        CPondMesh * pRM = *it;
        pRM->Render();
    }

    if (m_bEditMode) {
        // 연못 새로 만드는 중이면 드래그 선 그리기
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

        // dummy 상자 그리기
        m_VtxPosDummy.Render();

        BOOL bisFix = FALSE;

        CPondMesh * pSelPond;
        it_PondMesh it = m_pSelPonds.begin();
        int         iSize = m_pSelPonds.size();
        for (int i = 0; i < iSize; i++, it++) {
            pSelPond = *it;
            if (pSelPond) {
                // 선택된 연못의 점그리기 (삘깅)
                pSelPond->RenderVertexPoint();
            }
        }

        // 선택된 점 그리기 (초록)
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

    vPos[0].Set(m_CreateLine[0].x, m_CreateLine[0].y, m_CreateLine[0].z); //    왼쪽위
    vPos[1].Set(m_CreateLine[1].x, m_CreateLine[1].y, m_CreateLine[1].z); //    오른쪽위
    vPos[2].Set(m_CreateLine[2].x, m_CreateLine[2].y, m_CreateLine[2].z); //    오른쪽 아래
    vPos[3].Set(m_CreateLine[3].x, m_CreateLine[3].y, m_CreateLine[3].z); //    왼쪽아래

    CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();
    pRM->SetTerrain(pTerrain); //    지형포인터 입력
    pRM->MakeDrawRect(vPos);   //    영역입력
    pRM->MakePondPos();

    SelPondRelease(); //    지금까정 선택한거 일단 지우기

    int iID = 0;
    m_pSelPonds.push_back(pRM); //    일단 넣기(아뒤검사위해)
    while (SetPondID(pRM, iID) == FALSE) {
        iID++; //    새로운 아뒤찾음
    }

    CDlgPondProperty dlg(this);
    dlg.m_IsModalDialog = TRUE;

    if (dlg.DoModal() == IDCANCEL) //    지금 만든연못지우기
    {
        SelPondDelete(pRM);
        pRM = NULL;
    }
    if (pRM) //    만들려는 연못
    {
        m_PondMeshes.push_back(pRM);
        SelPondRelease();
        //    외곽점들을 바탕으로 가운데 점을 향하여 일정거리마다 점들을 배치한다////
    }

    return pRM;
}

void CPondMng::RemovePondMesh(int iPondID) //    연못을 만들거나 선택하여 지우고자 버튼 눌렀을시
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
        if (bCtrlKeyState == 2) //    연못을 회전하는 중
        {
            SetRotatePonds(ptDownBuff.x - point.x);
            ptDownBuff = point;
            return TRUE;
        }

        if (PCM_CREATE == m_PCursorMode) { // 새로운 연못 추가할때 드래그 하는 선 설정
            __Vector3 vRayDir, vRayOrig; // 화면 중앙(시점)과 마우스 포인터를 이은 직선의 방향과 원점
            __Vector3 vPN, vPV;          // 평면의 법선과 포함된 점
            __Vector3 vPos;              // 위의 평면과 직선의 만나는 점(구할 점)

            vPN.Set(0, 1, 0);
            vPV = vMouseStrPos;
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig); // 이함수 잠시 빌려씀.
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT; //    시작점과 마우스점을 구했음

            ReSetDrawRect(vMouseStrPos, vPos); //    받은 두점을 맵상의 사각형태로 변환
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
        if (bCtrlKeyState > 0) //    연못을 회전하는 중
        {
            if (bCtrlKeyState < 2) {
                bCtrlKeyState++;
            }
            SetVtxCenter();
            return TRUE;
        }

        if (PCM_CREATE == m_PCursorMode) { // 새로운 연못 추가 취소
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
        if (bCtrlKeyState > 0) //    연못을 회전하는 중
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

            // 드레그가 아니고 그냥 클릭일경우 드래그 영역을 3x3정도로 잡아준다.
            if (m_rcSelDrag.right - m_rcSelDrag.left < 3 && m_rcSelDrag.bottom - m_rcSelDrag.top < 3) {
                m_rcSelDrag.left = point.x - 1;
                m_rcSelDrag.right = point.x + 1;
                m_rcSelDrag.top = point.y - 1;
                m_rcSelDrag.bottom = point.y + 1;
            }
            //    shift키는 여러개의 분산된 점(들)을 선택시
            if (SelectVtxByDragRect(&m_rcSelDrag, (pMsg->wParam & MK_SHIFT) ? TRUE : FALSE)) {
                vMouseStrPos = m_VtxPosDummy.m_vPos; //    더미의 위치 입력
            } else {
                m_PCursorMode = PCM_NONE; //    선택된 점이 없으므로
            }

            return TRUE;
        }
    } break;
    case WM_RBUTTONDOWN: {
        if (bCtrlKeyState) { //    연못의 선택된 점에서 회전을 함
            return TRUE;
        }
        if (m_bMovePond == TRUE) {
            m_bMovePond = FALSE;
        }

        if (PCM_SELECT == m_PCursorMode) { // Select 취소
            m_PCursorMode = PCM_NONE;
            ReleaseCapture();
            return TRUE;
        } else if (PCM_NONE == m_PCursorMode) { // 새로운 연못 추가
            POINT        point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
            CLyTerrain * pTerrain = m_pMainFrm->GetMapMng()->GetTerrain();

            __Vector3 vPos;
            if (pTerrain && pTerrain->Pick(point.x, point.y, &vPos)) {
                m_PCursorMode = PCM_CREATE;

                vMouseStrPos = vPos; //    처음지점 입력

                DWORD color = 0xffffff00; //    그려질 선색
                for (int i = 0; i < 5; ++i) {
                    m_CreateLine[i].Set(vPos, color); //    초기화
                }

                return TRUE;
            }
        }
    } break;
    case WM_RBUTTONUP: {
        if (bCtrlKeyState) { //    연못의 선택된 점에서 회전을 함
            return TRUE;
        }

        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (PCM_CREATE == m_PCursorMode) { // 새로운 연못 추가
            m_PCursorMode = PCM_NONE;
            ReleaseCapture();

            __Vector3 vRayDir, vRayOrig; // 화면 중앙(시점)과 마우스 포인터를 이은 직선의 방향과 원점
            __Vector3 vPN, vPV;          // 평면의 법선과 포함된 점
            __Vector3 vPos;              // 위의 평면과 직선의 만나는 점(구할 점)

            vPN.Set(0, 1, 0);
            vPV = vMouseStrPos;
            m_VtxPosDummy.GetPickRay(point, vRayDir, vRayOrig); // 이함수 잠시 빌려씀.
            __Vector3 vPR = vPV - vRayOrig;
            float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
            vPos = vRayOrig + vRayDir * fT;

            ReSetDrawRect(vMouseStrPos, vPos);

            CPondMesh * pRM = CreateNewPondMesh(); //    새로운 연못
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
            if (!(GetAsyncKeyState(VK_CONTROL) & 0xff00)) //    체크하여 떼어진 상태면 원상복귀
            {
                if (bCtrlKeyState != 0) {
                    bCtrlKeyState = 0;
                }
                return FALSE;
            }

            if (GetAsyncKeyState('A') & 0xff00) { //    현재 선택된 연못(들)의 모든점을 대상으로 한다
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

void CPondMng::ReSetDrawRect(__Vector3 vStrPos, __Vector3 vEndPos) //    받은 두 점으로 맵상의 사각형태의 점 만듬
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
            SetSelPond(NULL); // 선택한연못 해제..
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
    for (int i = 0; i < iSize; ++i, ++it) // 이미 선택된 연못이 있다면..
    {
        pSelPond = *it;
        if (pSelPond == NULL) {
            continue;
        }

        int iVC = pSelPond->VertexCount(); // 그연못의 점 숫자를 구하기
        for (int k = 0; k < iVC; ++k) {
            __VertexXyzT2 * pVtx = pSelPond->GetVertex(k); // 점 하나 구하기
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
                        m_SelVtxArray.RemoveAt(j); // 이미 있으므로 선택목록에서 제거
                    } else {
                        m_SelVtxArray.InsertAt(0, pVtx); // 추가
                    }

                    pSelPond->InputSelectPos(pVtx->x, pVtx->y, pVtx->z, k); //    좌표입력하여 가상의 영역잡음
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
                    m_SelVtxArray.RemoveAt(j); // 이미 있으므로 선택목록에서 제거
                } else {
                    m_SelVtxArray.InsertAt(0, pVtx); // 추가
                }

                pSelPond->InputSelectPos(pVtx->x, pVtx->y, pVtx->z); //    좌표입력하여 가상의 영역잡음
            }
        }
    }

    if (iSize == 0) // 선택된 연못이 아무것도 없다면 (모든연못 검색해서 연못 선택후 그 연못 점들만 선택..)
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

            int iVC = pRM->VertexCount(); // 이연못의 점 갯수
            pSelPond = NULL;
            bChkSamePond = TRUE;
            for (int j = 0; j < iVC; ++j) {
                __VertexXyzT2 * pVtx = pRM->GetVertex(j); // 점 하나 구하기
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
                    m_SelVtxArray.Add(pVtx);                           // 추가
                    pRM->InputSelectPos(pVtx->x, pVtx->y, pVtx->z, j); //    좌표입력하여 가상의 영역잡음
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
    DWORD dwNum = 0;

    it_PondMesh it = m_PondMeshes.begin();
    WriteFile(hFile, &iPondCount, sizeof(int), &dwNum, NULL);
    for (int i = 0; i < iPondCount; i++, it++) {
        CPondMesh * pRM = *it;
        ASSERT(pRM);

        int            iVC = pRM->VertexCount();
        __VertexXyzT2 *pVtx0 = pRM->GetVertex(0), *pSrcVtx = NULL;
        ASSERT(pVtx0);
        WriteFile(hFile, &iVC, sizeof(iVC), &dwNum, NULL); // 점 갯수

        if (iVC <= 0) {
            continue;
        }

        int iWidthVtxNum = pRM->GetWaterScaleWidht();
        WriteFile(hFile, &iWidthVtxNum, sizeof(int), &dwNum, NULL); // 점 갯수

        int iLen = 0;

        CN3Texture * pPondTex = pRM->TexGet();
        if (pPondTex) {
            std::string szFile = pPondTex->FilePath().filename().string();
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

void CPondMng::SetVtxCenter() //    연못(들)의 중간점을 찾아 세팅,예전 스케일도 백업
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
            pvCenter[i] = pRM->GetCenter(); //    연못(들)의 중간점을 받음, 현재의 스케일도 백업
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

    if (m_pDlgProperty && (vMovePos.y != 0.0f || m_bMovePond == TRUE)) //    높이나 움직여서 정보를 갱신
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
    SelectVtxByDragRect(NULL, FALSE, TRUE); //    일단 현재 선택된 모든 연못의 점들 전부 선택
    m_bMovePond = TRUE;
    MainInvalidate();
}
