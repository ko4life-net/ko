// LightObjMgr.cpp: implementation of the CLightObjMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "LightObjMgr.h"
#include "DlgLight.h"
#include "LyTerrainDef.h"
#include "LyTerrain.h"
#include "MapMng.h"
#include "MainFrm.h"
#include "N3Base/N3Scene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightObjMgr::CLightObjMgr() {
    m_pRefMapMng = NULL; // Terrain reference pointer..
    m_bActive = false;

    m_iVersion = 1;

    m_pDlg = new CDlgLight;
    m_pDlg->Create(IDD_LIGHT);
    m_pDlg->ShowWindow(FALSE);
    m_pDlg->m_pRefLightObjMgr = this;

    m_ListObj.clear();

    m_BaseCube[0].Set(0, 1, 0); // front LT
    m_BaseCube[1].Set(1, 1, 0); // front RT
    m_BaseCube[2].Set(0, 0, 0); // front LB
    m_BaseCube[3].Set(1, 0, 0); // front RB
    m_BaseCube[4].Set(0, 1, 1); // behind LT
    m_BaseCube[5].Set(1, 1, 1); // behind RT
    m_BaseCube[6].Set(0, 0, 1); // behind LB
    m_BaseCube[7].Set(1, 0, 1); // behind RB

    m_pCurrLO = NULL;
    m_VtxPosDummy.Release();
}

CLightObjMgr::~CLightObjMgr() {
    ClearList();

    if (m_pDlg) {
        m_pDlg->DestroyWindow();
        delete m_pDlg;
        m_pDlg = NULL;
    }

    if (m_pCurrLO) {
        delete m_pCurrLO;
        m_pCurrLO = NULL;
    }
}

void CLightObjMgr::ClearList() {
    std::list<LIGHTOBJ *>::iterator it, ite;

    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pLO = (*it);
        if (pLO) {
            delete pLO;
        }
    }
    m_ListObj.clear();
}

bool CLightObjMgr::Load(HANDLE hFile) {
    m_pDlg->Reset();
    ClearList();

    CN3Scene * pOutPutScene = m_pRefMapMng->GetSceneOutput();
    int        NumLgt = pOutPutScene->LightCount();
    for (int i = 0; i < NumLgt; i++) {
        int idx = pOutPutScene->LightGet(i)->m_Data.nNumber;
        if (idx == IDX_CURR_LIGHT || idx == IDX_SEL_LIGHT || idx == IDX_STANDBY_LIGHT) {
            pOutPutScene->LightDelete(idx);
        }
    }

    DWORD dwRWC;
    ReadFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    if (m_iVersion <= 1) {
        int cnt = 0;
        ReadFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);

        for (int i = 0; i < cnt; i++) {
            LIGHTOBJ * pLO = new LIGHTOBJ;
            CN3Light * pLight = new CN3Light;

            pLO->pRefLight = pLight;
            pOutPutScene->LightAdd(pLight);

            ReadFile(hFile, &(pLO->szName[0]), 80, &dwRWC, NULL);
            pLight->Load(hFile);
            pLight->m_Data.bOn = false;
            pLight->m_Data.nNumber = IDX_STANDBY_LIGHT;

            m_ListObj.push_back(pLO);
            m_pDlg->InsertLOList(pLO);
        }
    }

    return true;
}

bool CLightObjMgr::Save(HANDLE hFile) {
    DWORD dwRWC;

    WriteFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    int cnt = m_ListObj.size();
    WriteFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);

    std::list<LIGHTOBJ *>::iterator it, ite;

    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pLO = (*it);
        WriteFile(hFile, &(pLO->szName[0]), 80, &dwRWC, NULL);
        pLO->pRefLight->Save(hFile);
    }
    return true;
}

void CLightObjMgr::SetActive(bool active) {
    if (m_bActive == active) {
        return;
    }
    m_bActive = active;

    CN3Scene * pOutPutScene = m_pRefMapMng->GetSceneOutput();

    if (active) {
        m_pDlg->ShowWindow(TRUE);
        LPLIGHTOBJ pLO = new LIGHTOBJ;
        CN3Light * pLgt = new CN3Light;

        pLO->pRefLight = pLgt;
        pOutPutScene->LightAdd(pLgt);

        //set light..
        D3DCOLORVALUE crLgt;
        crLgt.a = 0.0f;
        crLgt.r = crLgt.g = crLgt.b = 1.0f;
        pLO->pRefLight->m_Data.InitPoint(IDX_CURR_LIGHT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), crLgt, 0.0f, 0.0f);
        m_vCurrLOPos.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

        if (m_pCurrLO) {
            pOutPutScene->LightDelete(m_pCurrLO->pRefLight);
            delete m_pCurrLO;
            m_pCurrLO = NULL;
        }
        m_pCurrLO = pLO;
    } else {
        m_pDlg->ShowWindow(FALSE);
        if (m_pCurrLO) {
            pOutPutScene->LightDelete(m_pCurrLO->pRefLight);
            delete m_pCurrLO;
            m_pCurrLO = NULL;
        }
    }
}

BOOL CLightObjMgr::MouseMsgFilter(LPMSG pMsg) {
    if (!m_pRefMapMng) {
        return FALSE;
    }
    CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();
    if (!m_bActive || !pRefTerrain) {
        return FALSE;
    }

    switch (pMsg->message) {
    case WM_RBUTTONUP: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};

        __Vector3 vec;
        if (!pRefTerrain->Pick(point.x, point.y, &vec, NULL)) {
            break;
        }
        vec.y += 1.0f;

        m_vCurrLOPos.Set(vec, 0.0f, 0.0f);
        m_pCurrLO->pRefLight->PosSet(vec);
        m_VtxPosDummy.PosSet(vec);
        m_VtxPosDummy.SetSelVtx(&m_vCurrLOPos);
    }
        return TRUE;
    }

    if (m_VtxPosDummy.MouseMsgFilter(pMsg)) {
        m_pCurrLO->pRefLight->PosSet(m_vCurrLOPos.x, m_vCurrLOPos.y, m_vCurrLOPos.z);
        return TRUE;
    }
    return FALSE;
}

void CLightObjMgr::Render() {
    m_VtxPosDummy.Tick();
    m_VtxPosDummy.Render();

    // set transform
    HRESULT hr;

    D3DXMATRIX mtx;
    D3DXMatrixIdentity(&mtx);

    hr = s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtx); // Apply world matrix...

    // set texture
    hr = s_lpD3DDev->SetTexture(0, NULL);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    // backup state
    DWORD dwZEnable, dwLighting, dwCullMode;
    hr = s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    hr = s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    hr = s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);

    // set state
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    hr = s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    hr = s_lpD3DDev->SetFVF(FVF_XYZCOLOR);

    //Drawing an already created light object...
    std::list<LIGHTOBJ *>::iterator it;
    LIGHTOBJ *                      pLO;
    for (it = m_ListObj.begin(); it != m_ListObj.end(); it++) {
        pLO = (*it);
        if (!pLO || !(pLO->pRefLight)) {
            continue;
        }

        MakeCube(pLO->pRefLight->Pos(), 0xff0000ff);
        hr = s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &(m_CubeVB[0]), sizeof(__VertexXyzColor));
    }

    //Draw the light object currently being created.
    if (m_pCurrLO && m_pCurrLO->pRefLight) {
        MakeCube(m_pCurrLO->pRefLight->Pos(), 0xffff0000);
        hr = s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &(m_CubeVB[0]), sizeof(__VertexXyzColor));
    }

    //Draw the selected path in the dialog window...
    pLO = m_pDlg->m_pSelLO;
    if (pLO) {
        MakeCube(pLO->pRefLight->Pos(), 0xff00ff00);
        hr = s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &(m_CubeVB[0]), sizeof(__VertexXyzColor));
    }

    // restore
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
    hr = s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);
}

void CLightObjMgr::MakeCube(__Vector3 cv, D3DCOLOR color) {
    __Vector3 tmp;
    cv.x -= 0.5f;
    cv.y -= 0.5f;
    cv.z -= 0.5f;

    //front lt...
    tmp = cv + m_BaseCube[0];
    m_CubeVB[0].Set(tmp.x, tmp.y, tmp.z, color);
    tmp = cv + m_BaseCube[1];
    m_CubeVB[1].Set(tmp.x, tmp.y, tmp.z, color);
    tmp = cv + m_BaseCube[2];
    m_CubeVB[2].Set(tmp.x, tmp.y, tmp.z, color);

    //front rb...
    m_CubeVB[3] = m_CubeVB[2];
    m_CubeVB[4] = m_CubeVB[1];
    tmp = cv + m_BaseCube[3];
    m_CubeVB[5].Set(tmp.x, tmp.y, tmp.z, color);

    //right lt..
    m_CubeVB[6] = m_CubeVB[1];
    tmp = cv + m_BaseCube[5];
    m_CubeVB[7].Set(tmp.x, tmp.y, tmp.z, color);
    m_CubeVB[8] = m_CubeVB[5];

    //right rb..
    m_CubeVB[9] = m_CubeVB[8];
    m_CubeVB[10] = m_CubeVB[7];
    tmp = cv + m_BaseCube[7];
    m_CubeVB[11].Set(tmp.x, tmp.y, tmp.z, color);

    //back lt..
    m_CubeVB[12] = m_CubeVB[7];
    tmp = cv + m_BaseCube[4];
    m_CubeVB[13].Set(tmp.x, tmp.y, tmp.z, color);
    m_CubeVB[14] = m_CubeVB[11];

    //back rb..
    m_CubeVB[15] = m_CubeVB[14];
    m_CubeVB[16] = m_CubeVB[13];
    tmp = cv + m_BaseCube[6];
    m_CubeVB[17].Set(tmp.x, tmp.y, tmp.z, color);

    //left lt..
    m_CubeVB[18] = m_CubeVB[13];
    m_CubeVB[19] = m_CubeVB[0];
    m_CubeVB[20] = m_CubeVB[17];

    //left rb..
    m_CubeVB[21] = m_CubeVB[20];
    m_CubeVB[22] = m_CubeVB[19];
    m_CubeVB[23] = m_CubeVB[2];

    //top lt..
    m_CubeVB[24] = m_CubeVB[13];
    m_CubeVB[25] = m_CubeVB[12];
    m_CubeVB[26] = m_CubeVB[0];

    //top rb..
    m_CubeVB[27] = m_CubeVB[26];
    m_CubeVB[28] = m_CubeVB[25];
    m_CubeVB[29] = m_CubeVB[1];

    //bottom lt..
    m_CubeVB[30] = m_CubeVB[2];
    m_CubeVB[31] = m_CubeVB[5];
    m_CubeVB[32] = m_CubeVB[17];

    //botom rb..
    m_CubeVB[33] = m_CubeVB[32];
    m_CubeVB[34] = m_CubeVB[31];
    m_CubeVB[35] = m_CubeVB[14];
}

void CLightObjMgr::DownLoad() {
    CN3Scene * pOutPutScene = m_pRefMapMng->GetSceneOutput();

    if (m_pCurrLO) {
        pOutPutScene->LightDelete(m_pCurrLO->pRefLight);
        delete m_pCurrLO;
    }

    m_pCurrLO = m_pDlg->m_pSelLO;
    m_pCurrLO->pRefLight->m_Data.bOn = true;
    m_pCurrLO->pRefLight->m_Data.nNumber = IDX_CURR_LIGHT;

    std::list<LIGHTOBJ *>::iterator it, ite;
    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pSrcLO = (*it);
        if (pSrcLO == m_pCurrLO) {
            m_ListObj.erase(it);
            break;
        }
    }

    m_vCurrLOPos.Set(m_pCurrLO->pRefLight->Pos(), 0.0f, 0.0f);
    m_VtxPosDummy.PosSet(m_vCurrLOPos.x, m_vCurrLOPos.y, m_vCurrLOPos.z);
    m_VtxPosDummy.SetSelVtx(&m_vCurrLOPos);
}

void CLightObjMgr::UpLoad(const char * pName, float fRange, float fAtten, D3DCOLORVALUE crLgt) {
    m_pCurrLO->pRefLight->m_Data.InitPoint(IDX_STANDBY_LIGHT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), crLgt, fRange, fAtten);
    m_pCurrLO->pRefLight->m_Data.bOn = false;
    sprintf(m_pCurrLO->szName, pName);

    m_ListObj.push_back(m_pCurrLO);

    LPLIGHTOBJ pLO = new LIGHTOBJ;
    CN3Light * pLgt = new CN3Light;

    pLO->pRefLight = pLgt;
    CN3Scene * pOutPutScene = m_pRefMapMng->GetSceneOutput();
    pOutPutScene->LightAdd(pLgt);

    //set light..
    crLgt.a = 0.0f;
    crLgt.r = crLgt.g = crLgt.b = 1.0f;
    pLO->pRefLight->m_Data.InitPoint(IDX_CURR_LIGHT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), crLgt, 0.0f, 0.0f);
    m_vCurrLOPos.Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

    m_pCurrLO = pLO;

    m_vCurrLOPos.Set(m_pCurrLO->pRefLight->Pos(), 0.0f, 0.0f);
    m_VtxPosDummy.PosSet(m_vCurrLOPos.x, m_vCurrLOPos.y, m_vCurrLOPos.z);
    m_VtxPosDummy.SetSelVtx(&m_vCurrLOPos);
}

void CLightObjMgr::DeleteLO(LPLIGHTOBJ pLO) {
    if (!pLO) {
        return;
    }

    CN3Scene * pOutPutScene = m_pRefMapMng->GetSceneOutput();
    pOutPutScene->LightDelete(pLO->pRefLight);

    std::list<LIGHTOBJ *>::iterator it, ite;
    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pSrcLO = (*it);
        if (pSrcLO == pLO) {
            delete pLO;
            m_ListObj.erase(it);
            break;
        }
    }
}

void CLightObjMgr::RefreshCurrLights(float fRange, float fAtten, D3DCOLORVALUE crLgt) {
    __Vector3 vPos = m_pCurrLO->pRefLight->Pos();
    m_pCurrLO->pRefLight->m_Data.InitPoint(IDX_CURR_LIGHT, vPos, crLgt, fRange, fAtten);
}

void CLightObjMgr::ChangeSelLights() {
    std::list<LIGHTOBJ *>::iterator it, ite;
    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pLO = (*it);
        if (pLO && pLO->pRefLight) {
            pLO->pRefLight->m_Data.nNumber = IDX_STANDBY_LIGHT;
            pLO->pRefLight->m_Data.bOn = false;
        }
    }

    if (m_pDlg->m_pSelLO && m_pDlg->m_pSelLO->pRefLight) {
        m_pDlg->m_pSelLO->pRefLight->m_Data.nNumber = IDX_SEL_LIGHT;
        m_pDlg->m_pSelLO->pRefLight->m_Data.bOn = true;
    }
}

bool CLightObjMgr::MakeGameFile(char * szFN) {
    int cnt = m_ListObj.size();
    if (cnt <= 0) {
        return true;
    }

    HANDLE hFile = CreateFile(szFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD dwRWC;
    WriteFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);
    WriteFile(hFile, &cnt, sizeof(int), &dwRWC, NULL);

    std::list<LIGHTOBJ *>::iterator it, ite;

    ite = m_ListObj.end();
    for (it = m_ListObj.begin(); it != ite; it++) {
        LIGHTOBJ * pLO = (*it);
        pLO->pRefLight->Save(hFile);
    }
    CloseHandle(hFile);
    return true;
}