// MapMng.cpp: implementation of the CMapMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3ME.h"
#include "MapMng.h"

#include "MainFrm.h"
#include "N3MEView.h"
#include "LyTerrainDef.h"
#include "LyTerrain.h"
//#include "DlgSceneGraph.h"
#include "DlgShapeList.h"
#include "DlgBase.h"
#include "BrushDlg.h"
#include "N3Base/N3EngTool.h"
#include "N3Base/N3Camera.h"
#include "N3Base/N3Chr.h"
#include "N3Base/N3Shape.h"
#include "N3Base/N3Scene.h"
#include "N3Base/Pick.h"
#include "N3Base/N3PMeshInstance.h"
#include "N3Base/N3ShapeMgr.h"
#include "DlgTerrainSize.h"
#include "NPCPathMgr.h"
#include "WallMgr.h"
#include "WarpMgr.h"
#include "RegenUser.h"
#include "EventMgr.h"
#include "DlgUnusedFiles.h"
#include "ProgressBar.h"
#include "SoundMgr.h"
#include "LightObjMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 생성자
CMapMng::CMapMng(CMainFrame * pMainFrm)
    : m_RiverMng(pMainFrm)
    , m_PondMng(pMainFrm) {
    ASSERT(pMainFrm->GetSafeHwnd());

    m_pMainFrm = pMainFrm;
    m_pTerrain = NULL;

    // Path 지정하기
    m_pSceneSource = new CN3Scene;
    m_pSceneSource->m_szName = "SourceList";
    m_pSceneSource->FilePathSet("SourceList.n3scene");

    m_pSceneOutput = new CN3Scene;

    //m_pDlgSourceList = new CDlgSceneGraph(m_pMainFrm, OBJ_SHAPE);
    m_pDlgSourceList = new CDlgShapeList(m_pMainFrm);
    m_pDlgSourceList->Create(IDD_SHAPE_LIST, m_pMainFrm);
    m_pDlgSourceList->SetWindowText("Object List");
    m_pDlgSourceList->m_pMapMng = this;

    //m_pDlgOutputList = new CDlgSceneGraph(m_pMainFrm, OBJ_SHAPE);
    m_pDlgOutputList = new CDlgShapeList(m_pMainFrm);
    m_pDlgOutputList->Create(IDD_SHAPE_LIST, m_pMainFrm);
    m_pDlgOutputList->SetWindowText("Output List");
    m_pDlgOutputList->m_IsSourceObj = FALSE;
    m_pDlgOutputList->m_pMapMng = this;

    m_pDlgBase = new CDlgBase(this);
    m_pDlgBase->Create(IDD_BASE, m_pMainFrm);

    m_pBrushDlg = new CBrushDlg;
    m_pBrushDlg->Create(IDD_BRUSH, m_pMainFrm);

    m_FillMode = D3DFILL_SOLID;
    m_ShadeMode = D3DSHADE_GOURAUD;

    m_bViewWireFrame = true;

    m_pDlgBase->UpdateInfo();

    m_pNPCPath = new CNPCPathMgr;
    m_pNPCPath->m_pRefMapMng = this;

    m_pWall = new CWallMgr;
    m_pWall->m_pRefMapMng = this;

    m_pEventMgr = new CEventMgr;
    m_pEventMgr->m_pRefMapMng = this;

    m_pRegenUser = new CRegenUser;
    m_pRegenUser->m_pRefMapMng = this;

    m_pWarpMgr = new CWarpMgr;
    m_pWarpMgr->m_pRefMapMng = this;

    m_pSoundMgr = new CSoundMgr;
    m_pSoundMgr->m_pRefMapMng = this;

    m_pLightObjMgr = new CLightObjMgr;
    m_pLightObjMgr->m_pRefMapMng = this;

    m_iZoneID = 0;
    m_bHideObj = false;

    m_bLoadingComplete = false;

    // 윈도 배치
    CRect rc(0, 0, 980, 1000);
    pMainFrm->MoveWindow(&rc);
    m_pDlgSourceList->MoveWindow(rc.right, rc.top, 300, 500);
    m_pDlgOutputList->MoveWindow(rc.right, 500, 300, 500);

    Release();
}

// 파괴자
CMapMng::~CMapMng() {
    Release();
    if (m_pSceneSource) {
        delete m_pSceneSource;
        m_pSceneSource = NULL;
    }
    if (m_pDlgSourceList) {
        m_pDlgSourceList->DestroyWindow();
        delete m_pDlgSourceList;
        m_pDlgSourceList = NULL;
    }
    if (m_pDlgOutputList) {
        m_pDlgOutputList->DestroyWindow();
        delete m_pDlgOutputList;
        m_pDlgOutputList = NULL;
    }
    if (m_pDlgBase) {
        m_pDlgBase->DestroyWindow();
        delete m_pDlgBase;
        m_pDlgBase = NULL;
    }
    if (m_pBrushDlg) {
        m_pBrushDlg->DestroyWindow();
        delete m_pBrushDlg;
        m_pBrushDlg = NULL;
    }
    if (m_pSceneOutput) {
        delete m_pSceneOutput;
        m_pSceneOutput = NULL;
    }
    if (m_pNPCPath) {
        delete m_pNPCPath;
        m_pNPCPath = NULL;
    }
    if (m_pWall) {
        delete m_pWall;
        m_pWall = NULL;
    }
    if (m_pEventMgr) {
        delete m_pEventMgr;
        m_pEventMgr = NULL;
    }
    if (m_pRegenUser) {
        delete m_pRegenUser;
        m_pRegenUser = NULL;
    }
    if (m_pWarpMgr) {
        delete m_pWarpMgr;
        m_pWarpMgr = NULL;
    }
    if (m_pSoundMgr) {
        delete m_pSoundMgr;
        m_pSoundMgr = NULL;
    }
    if (m_pLightObjMgr) {
        delete m_pLightObjMgr;
        m_pLightObjMgr = NULL;
    }
}

// 초기화
void CMapMng::Release() {
    if (m_pTerrain) {
        delete m_pTerrain;
        m_pTerrain = NULL;
    }
    if (m_pSceneOutput) {
        m_pSceneOutput->Release();
        m_pSceneOutput->DefaultCameraAdd();
        m_pSceneOutput->m_bDisableDefaultLight = true;
        //m_pSceneOutput->DefaultLightAdd();
        for (int i = 0; i < NUM_UNIT_LIGHT; i++) {
            D3DCOLORVALUE crLgt;
            crLgt.a = 0.0f, crLgt.r = crLgt.g = crLgt.b = 0.7f;

            CN3Light * pLight = new CN3Light;
            if (i == 0) {
                pLight->m_Data.InitDirection(0, __Vector3(0, -1, 0), crLgt);
            } else {
                pLight->m_Data.InitDirection(1, __Vector3(0, 1, 0), crLgt);
            }
            pLight->m_Data.bOn = true;
            m_pSceneOutput->LightAdd(pLight);
        }

        m_pSceneOutput->CameraGetActive()->m_Data.fNP = 1.0f;
        m_pSceneOutput->CameraGetActive()->m_Data.fFP = m_pMainFrm->GetFP();
    }

    m_pSelSourceObj = NULL;
    m_SelOutputObjArray.RemoveAll();
    m_SelOutputObjBack.RemoveAll();

    m_eSelObjBackState = eEDIT_NOT;
    m_bRenderSelObj = FALSE;
    m_bRenderAxisAndGrid = FALSE;
    m_CursorMode = CM_SELECT;
    m_pDummy = NULL;
    m_rcSelDrag.left = m_rcSelDrag.top = m_rcSelDrag.right = m_rcSelDrag.bottom = 0;

    m_RiverMng.Release();
    m_PondMng.Release();
}

//FarPlane 갱신...
void CMapMng::UpDateFP() {
    m_pSceneOutput->CameraGetActive()->m_Data.fFP = m_pMainFrm->GetFP();
    Invalidate();
}

// m_pSceneSource 캐릭터와 오브젝트를 추가하는 함수
void CMapMng::LoadSourceObjects() {
    ASSERT(m_pSceneSource && m_pDlgSourceList);

    m_pSceneSource->Release();

    fs::path fsChrDir = fs::current_path() / "Chr";
    if (fs::is_directory(fsChrDir)) {
        for (const auto & fi : fs::directory_iterator(fsChrDir)) {
            if (fi.is_regular_file() && n3std::iequals(fi.path().extension(), ".n3chr")) {
                AddChr(m_pSceneSource, "Chr" / fi.path().filename(), FALSE);
            }
        }
    }

    fs::path fsObjectDir = fs::current_path() / "Object";
    if (fs::is_directory(fsObjectDir)) {
        for (const auto & fi : fs::directory_iterator(fsObjectDir)) {
            if (fi.is_regular_file() && n3std::iequals(fi.path().extension(), ".n3shape")) {
                AddShape(m_pSceneSource, "Object" / fi.path().filename(), FALSE);
            }
        }
    }

    m_pSceneSource->Tick();                       // Object 초기화
    m_pDlgSourceList->UpdateTree(m_pSceneSource); // 목록 갱신
}

// 지정한 Scene에 캐릭터 추가하는 함수
CN3Transform * CMapMng::AddChr(CN3Scene * pDestScene, const fs::path & fsFile, BOOL bGenerateChainNumber) {
    CN3Chr * pChr = new CN3Chr;
    if (!pChr->LoadFromFile(fsFile)) { // 부르기가 실패하면..
        delete pChr;
        return NULL;
    }

    if (bGenerateChainNumber) {
        int nChainNumber = 0;
        int nCC = pDestScene->ChrCount();
        for (int i = 0; i < nCC; i++) {
            std::string szNameFull = pDestScene->ChrGet(i)->m_szName;
            size_t      iSeparatorPos = szNameFull.rfind('_');
            if (iSeparatorPos == std::string::npos) {
                continue;
            }

            std::string szName = szNameFull.substr(0, iSeparatorPos);
            if (pChr->m_szName == szName) {
                std::string szDigits = szNameFull.substr(iSeparatorPos + 1);
                std::from_chars(szDigits.data(), szDigits.data() + szDigits.size(), nChainNumber);
                ++nChainNumber;
            }
        }

        pChr->m_szName = std::format("{:s}_{:04d}", pChr->m_szName, nChainNumber);
    }

    pDestScene->ChrAdd(pChr);
    return pChr;
}

// 지정한 Scene에 Shape 추가하는 함수
CN3Transform * CMapMng::AddShape(CN3Scene * pDestScene, const fs::path & fsFile, BOOL bGenerateChainNumber) {
    CN3Shape * pShape = new CN3Shape;
    if (false == pShape->LoadFromFile(fsFile)) // 부르기가 실패하면..
    {
        delete pShape;
        return NULL;
    }

    /*
    if (bGenerateChainNumber) {
        int nChainNumber = 0;
        int nSC = pDestScene->ShapeCount();
        for (int i = 0; i < nSC; i++) {
            std::string szNameFull = pDestScene->ShapeGet(i)->m_szName;
            size_t      iSeparatorPos = szNameFull.rfind('_');
            if (iSeparatorPos == std::string::npos) {
                continue;
            }

            std::string szName = szNameFull.substr(0, iSeparatorPos);
            if (pShape->m_szName == szName) {
                std::string szDigits = szNameFull.substr(iSeparatorPos + 1);
                std::from_chars(szDigits.data(), szDigits.data() + szDigits.size(), nChainNumber);
                ++nChainNumber;
            }
        }

        pShape->m_szName = std::format("{:s}_{:04d}", pShape->m_szName, nChainNumber);

        fs::path fsFile = pShape->FilePath().parent_path() / (pShape->m_szName + pShape->FilePath().extension());
        pShape->FilePathSet(fsFile);
    }
    */

    pDestScene->ShapeAdd(pShape); // 추가 하고
    return pShape;
}

// pObj를 m_pSceneOutput에 추가한다.
CN3Transform * CMapMng::AddObjectToOutputScene(CN3Transform * pObj) {
    if (pObj == NULL) {
        return NULL;
    }
    ASSERT(m_pSceneOutput && pObj->m_szName.size());

    // m_pSceneOutput에 넣기
    CN3Transform * pDestObj = NULL;
    if (pObj->Type() & OBJ_CHARACTER) {
        pDestObj = AddChr(m_pSceneOutput, pObj->FilePath(), TRUE);
    } else if (pObj->Type() & OBJ_SHAPE) {
        pDestObj = AddShape(m_pSceneOutput, pObj->FilePath(), TRUE);
    }

    if (pDestObj) {
        pDestObj->PosSet(pObj->Pos());
        m_pDlgOutputList->UpdateTree(m_pSceneOutput);
        return pDestObj;
    }
    return NULL;
}

// 선택한 객체를 지움
void CMapMng::DeleteSelObjectFromOutputScene() {
    if (GetCursorMode() == CM_EDIT_RIVER) // 강물 편집일 경우 선택 점 지우기
    {
        m_RiverMng.DeleteSelectedVertex();
        return;
    }

    if (GetCursorMode() == CM_EDIT_POND) {
        //    {
        //        m_PondMng.DeleteSelectedVertex();
        return;
    }
    //    }

    int iSize = m_SelOutputObjArray.GetSize();
    if (iSize == 0) {
        return;
    }
    for (int i = 0; i < iSize; ++i) {
        CN3Transform * pSelObj = m_SelOutputObjArray.GetAt(i);
        if (pSelObj == NULL) {
            continue;
        }
        DWORD dwType = pSelObj->Type();
        if (dwType & OBJ_CHARACTER) {
            m_pSceneOutput->ChrDelete((CN3Chr *)pSelObj);
        } else if (dwType & OBJ_SHAPE) {
            m_pSceneOutput->ShapeDelete((CN3Shape *)pSelObj);
        }
    }
    m_SelOutputObjArray.RemoveAll();
    if (m_pDummy) {
        m_pDummy->SetSelObj(NULL);
    }
    m_pDlgOutputList->UpdateTree(m_pSceneOutput);
    Invalidate();
}

void CMapMng::SavePartition(float x, float z, float width) {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, "n3m", NULL, dwFlags, "N3ME 파일(*.n3m)|*.n3m||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    fs::path fsFileTmp = fs::path(dlg.GetPathName().GetString()).replace_extension(".n3m");

    HANDLE hFile = CreateFileW(fsFileTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Fail to open map data file for save, Pleas retry.",
                    MB_OK);
        return;
    }
    char  szComment[80] = {"이파일 여는 사람 바보..^^"};
    DWORD dwRWC;
    WriteFile(hFile, &szComment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hFile);

    // 지형
    if (m_pTerrain) {
        m_pTerrain->SaveToFilePartition(fsFileTmp.replace_extension(".trn"), x, z, width);
    }

    // sdt파일 저장(shape data text)
    SaveObjectPostDataPartition(fsFileTmp.replace_extension(".sdt"), x, z, width);

    /*
    // warp 정보 load..
    m_pWarpMgr->SaveToFile(fsFileTmp.replace_extension(".wap"));

    //이벤트 정보 저장..
    m_pEventMgr->SaveToFile(fsFileTmp.replace_extension(".evt"));
    */
}

void CMapMng::SaveToFile(const fs::path & fsFile) {
    if (fsFile.empty()) {
        return;
    }

    fs::path fsFileTmp = fs::path(fsFile).replace_extension(".n3m");

    HANDLE hFile = CreateFileW(fsFileTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Fail to open map data file for save, Please retry.",
                    MB_OK);
        return;
    }

    char  szComment[80] = {"이파일 여는 사람 바보..^^"};
    DWORD dwRWC;
    WriteFile(hFile, &szComment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hFile);

    // 지형
    if (m_pTerrain) {
        m_pTerrain->SaveToFile(fsFileTmp.replace_extension(".trn"));
    }

    // sdt파일 저장(shape data text)
    SaveObjectPostData(fsFileTmp.replace_extension(".sdt"));

    // 강물 편집 정보 저장..
    m_RiverMng.SaveToFile(fsFileTmp.replace_extension(".rvr"));

    // 연못 편집 정보 저장..
    m_PondMng.SaveToFile(fsFileTmp.replace_extension(".pvr"));

    //벽 정보 저장..
    m_pWall->SaveToFile(fsFileTmp.replace_extension(".wal"));

    // warp 정보 load..
    m_pWarpMgr->SaveToFile(fsFileTmp.replace_extension(".wap"));

    // sound 정보 Load..
    m_pSoundMgr->SaveToFile(fsFileTmp.replace_extension(".tsd"));

    // light object 정보..
    m_pLightObjMgr->SaveToFile(fsFileTmp.replace_extension(".tld"));

    /*
    // 이벤트 정보 저장..
    m_pEventMgr->SaveToFile(fsFileTmp.replace_extension("evt"));
    */
}

void CMapMng::LoadFromFile(const fs::path & fsFile) {
    if (fsFile.empty()) {
        return;
    }

    fs::path fsFileTmp(fsFile);

    // 지형
    delete m_pTerrain;
    m_pTerrain = new CLyTerrain;
    m_pTerrain->Init();
    m_pTerrain->LoadFromFile(fsFileTmp.replace_extension(".trn"));

    // Scene
    ASSERT(m_pSceneOutput);
    LoadObjectPostData(fsFileTmp.replace_extension(".sdt"));

    // river
    m_RiverMng.LoadFromFile(fsFileTmp.replace_extension(".rvr"));

    // Pond
    m_PondMng.LoadFromFile(fsFileTmp.replace_extension(".pvr"));

    //벽 정보 load..
    m_pWall->LoadFromFile(fsFileTmp.replace_extension(".wal"));

    // warp 정보 load..
    m_pWarpMgr->LoadFromFile(fsFileTmp.replace_extension(".wap"));

    // sound 정보 Load..
    m_pSoundMgr->LoadFromFile(fsFileTmp.replace_extension(".tsd"));

    // light object 정보..
    m_pLightObjMgr->LoadFromFile(fsFileTmp.replace_extension(".tld"));

    /*
    // 이벤트 정보..
    m_pEventMgr->LoadFromFile(fsFileTmp.replace_extension(".evt"));
    m_pEventMgr->SetActive(true);
    m_pEventMgr->SetActive(false);
    */

    m_pDlgOutputList->UpdateTree(m_pSceneOutput);
}

void CMapMng::Tick() {
    m_pSceneOutput->Tick();
    if (m_pDummy) {
        m_pDummy->Tick();
    }
    m_RiverMng.Tick();
    m_PondMng.Tick();
    if (m_pTerrain) {
        m_pTerrain->Tick();
    }
}

void CMapMng::Render() {
    if (false == m_bLoadingComplete) {
        return; // 로딩이 아직 안 끝났다...
    }

    CN3EngTool * pEng = m_pMainFrm->m_pEng;
    if (pEng == NULL) {
        return;
    }

    pEng->s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, m_FillMode);
    pEng->s_lpD3DDev->SetRenderState(D3DRS_SHADEMODE, m_ShadeMode);

    if (m_bRenderAxisAndGrid) // 축과 그리드 그리기...
    {
        pEng->RenderAxis();
        __Matrix44 mtxWorld;
        mtxWorld.Scale(32.0f, 32.0f, 32.0f);
        if (m_SelOutputObjArray.GetSize() > 0) {
            CN3Transform * pSelObj = m_SelOutputObjArray.GetAt(0);
            if (pSelObj) // 선택된 객체가 있으면..
            {
                mtxWorld.PosSet(0, pSelObj->Pos().y, 0); // 높이를 올린다.
            }
        }
        pEng->RenderGrid(mtxWorld); // 그리드 그리기...
    }

    if (m_pTerrain) {
        m_pTerrain->Render();
    }
    if (m_pSceneOutput && !m_bHideObj) {
        m_pSceneOutput->Render();
    }

    int iSize = m_SelOutputObjArray.GetSize();
    for (int i = 0; i < iSize; ++i) {
        CN3Transform * pSelObj = m_SelOutputObjArray.GetAt(i);
        if (pSelObj == NULL) {
            continue;
        }

        if (pSelObj->Type() & OBJ_SHAPE) {
            ((CN3Shape *)pSelObj)->RenderSelected(m_bViewWireFrame);
            ((CN3Shape *)pSelObj)->RenderCollisionMesh(); // 충돌 메시 그리기...
            ((CN3Shape *)pSelObj)->RenderClimbMesh();     // 올라가는 메시 그리기..
        }
    }

    if (m_CursorMode == CM_OBJECT_BRUSH) {
        if (m_pSelSourceObj && m_bRenderSelObj) {
            m_bRenderSelObj = FALSE;
            DWORD dwType = m_pSelSourceObj->Type();
            if (dwType & OBJ_CHARACTER) {
                ((CN3Chr *)m_pSelSourceObj)->Render();
            } else if (dwType & OBJ_SHAPE) {
                ((CN3Shape *)m_pSelSourceObj)->Render();
            }
        }
    }
    if (m_pDummy) {
        m_pDummy->Render();
    }

    if (m_pNPCPath && m_pNPCPath->m_bActive) {
        m_pNPCPath->Render();
    }
    if (m_pWall && m_pWall->m_bActive) {
        m_pWall->Render();
    }
    if (m_pEventMgr && m_pEventMgr->m_bActive) {
        m_pEventMgr->Render();
    }
    if (m_pRegenUser && m_pRegenUser->m_bActive) {
        m_pRegenUser->Render();
    }
    if (m_pSoundMgr && m_pSoundMgr->m_bActive) {
        m_pSoundMgr->Render();
    }
    if (m_pLightObjMgr && m_pLightObjMgr->m_bActive) {
        m_pLightObjMgr->Render();
    }

    m_RiverMng.Render();
    m_PondMng.Render();
    //    RenderGrid(256.0f, 4096);
    RenderDragRect(&m_rcSelDrag);
}

void CMapMng::RenderDragRect(RECT * rc) {
    ASSERT(rc);
    if (rc->top == rc->bottom && rc->left == rc->right) {
        return;
    }
    __VertexTransformedColor Vertices[5];
    DWORD                    color = 0xff00ffff;
    Vertices[0].Set((float)rc->left, (float)rc->top, 0.0f, 0.1f, color);
    Vertices[1].Set((float)rc->right, (float)rc->top, 0.0f, 0.1f, color);
    Vertices[2].Set((float)rc->right, (float)rc->bottom, 0.0f, 0.1f, color);
    Vertices[3].Set((float)rc->left, (float)rc->bottom, 0.0f, 0.1f, color);
    Vertices[4] = Vertices[0];

    CN3Eng *          pEng = m_pMainFrm->m_pEng;
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    HRESULT hr;
    hr = pD3DDev->SetTexture(0, NULL);
    hr = pD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = pD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    hr = pD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
    hr = pD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, Vertices, sizeof(__VertexTransformedColor));
}

void CMapMng::FocusSelObj() {
    ASSERT(m_pSceneOutput);
    CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
    int         iSize = m_SelOutputObjArray.GetSize();
    if (iSize <= 0 || pCamera == NULL) {
        return;
    }
    __Vector3 vMin, vMax;
    vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
    vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    CN3Transform *          pN3;
    CN3TransformCollision * pSelObj;
    for (int i = 0; i < iSize; ++i) {
        pN3 = m_SelOutputObjArray.GetAt(i);
        pSelObj = (CN3TransformCollision *)pN3;

        __Vector3 vTmpMin = pSelObj->Min();
        __Vector3 vTmpMax = pSelObj->Max();

        if (vTmpMin.x < vMin.x) {
            vMin.x = vTmpMin.x;
        }
        if (vTmpMin.y < vMin.y) {
            vMin.y = vTmpMin.y;
        }
        if (vTmpMin.z < vMin.z) {
            vMin.z = vTmpMin.z;
        }
        if (vTmpMax.x > vMax.x) {
            vMax.x = vTmpMax.x;
        }
        if (vTmpMax.y > vMax.y) {
            vMax.y = vTmpMax.y;
        }
        if (vTmpMax.z > vMax.z) {
            vMax.z = vTmpMax.z;
        }
    }

    if (vMin.x != FLT_MAX && vMax.x != -FLT_MAX) { // 물체 크기에 맞춰 카메라 거리 조절
        __Vector3 vDir = pCamera->Dir();
        __Vector3 vAt = vMin + ((vMax - vMin) / 2);
        pCamera->AtPosSet(vAt);
        pCamera->EyePosSet(vAt - vDir * (vMax - vMin).Magnitude());
    }
    //    else {ASSERT(0);}
    //    {    // 물체 크기를 알 수 없으므로 지금 거리 유지
    //        CN3TransformCollision* pSelObj = m_SelOutputObjArray.GetAt(0);
    //        pCamera->m_vRot = pSelObj->m_vPos;
    //        __Vector3 vDiff = pSelObj->m_vPos - pCamera->m_vRot;
    //        pCamera->m_vPos += vDiff;
    //    }
    Invalidate();
}

void CMapMng::FocusAt(__Vector3 v) {
    ASSERT(m_pSceneOutput);
    CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();

    __Vector3 vDir = pCamera->Dir();
    __Vector3 vAt = v;

    pCamera->AtPosSet(vAt);
    pCamera->EyePosSet(vAt - vDir * 10);

    Invalidate();
}

void CMapMng::FocusAll() {
    ASSERT(m_pSceneOutput);
    CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
    if (m_pTerrain == NULL || pCamera == NULL) {
        return;
    }
    SIZE      size = m_pTerrain->GetPatchNum(1.0f);
    __Vector3 vDir = pCamera->Dir();
    __Vector3 vAt;
    vAt.Set(size.cx / 2.0f, 0, size.cy / 2.0f);
    pCamera->AtPosSet(vAt);
    pCamera->EyePosSet(vAt - vDir * ((float)std::max(size.cx, size.cy)) / tanf(pCamera->m_Data.fFOV / 2.0f) / 2.0f);

    Invalidate();
}

/*
BOOL CMapMng::CameraMove(LPMSG pMsg)
{
    static int iButtonDownCount = 0;
    if (pMsg->message == WM_MOUSEWHEEL)
    {
        //short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        short zDelta = (short)((pMsg->wParam>>16)&0x0000ffff);
        CN3Camera* pCamera = m_pSceneOutput->CameraGetActive();

        if(pCamera)
        {
            float fD = (pCamera->AtPos() - pCamera->EyePos()).Magnitude();
            pCamera->MoveStraight(fD * zDelta * 0.001f);
            pCamera->Apply();
            return TRUE;
        }
    }
    else if (pMsg->message == WM_KEYDOWN)
    {
        CN3Camera* pCamera = m_pSceneOutput->CameraGetActive();
        switch(pMsg->wParam)
        {
        case VK_UP:
            if (pCamera) {pCamera->Move(pCamera->Dir()*10.0f); pCamera->Apply(); return TRUE;}
            break;
        case VK_DOWN:
            if (pCamera) {pCamera->Move(pCamera->Dir()*(-10.0f)); pCamera->Apply(); return TRUE;}
            break;
        case VK_RIGHT:
            if (pCamera) {pCamera->Rotate(0, -0.1f); pCamera->Apply(); return TRUE;}
            break;
        case VK_LEFT:
            if (pCamera) {pCamera->Rotate(0, 0.1f); pCamera->Apply(); return TRUE;}
            break;
        case '1':
            {
                static BOOL bSet = FALSE;
                if (pCamera)
                {
                    static __Vector3 vEye(0, 0, 0);
                    static __Vector3 vAt(0, 0, 1);
                    static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00)
                    {
                        vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector();
                        pCamera->Apply();
                        bSet = TRUE;
                        return FALSE;
                    }
                    else if (bSet)
                    {
                        pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp);
                        pCamera->Apply();
                        return TRUE;
                    }
                    else return FALSE;
                }

            }
            break;
        case '2':    // '1'과 같은 내용임 (함수로 만들지 않은 이유는 static변수 때문에..)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '3':    // '1'과 같은 내용임 (함수로 만들지 않은 이유는 static변수 때문에..)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '4':    // '1'과 같은 내용임 (함수로 만들지 않은 이유는 static변수 때문에..)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '5':    // '1'과 같은 내용임 (함수로 만들지 않은 이유는 static변수 때문에..)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        }
    }
    else if (iButtonDownCount == 0 && !(GetAsyncKeyState(VK_MENU) & 0xff00) ) return FALSE;    // alt 키가 안눌렸을경우는 카메라 움직임이 아니다.

    static CPoint ptPrev;
    switch(pMsg->message)
    {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        if (iButtonDownCount == 0)
        {
            SetCapture(pMsg->hwnd);
            ptPrev.x = short(LOWORD(pMsg->lParam)); ptPrev.y = short(HIWORD(pMsg->lParam));
        }
        ++iButtonDownCount;
        return TRUE;
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        --iButtonDownCount;
        if (iButtonDownCount <= 0) ReleaseCapture();
        return TRUE;
        break;
    case WM_MOUSEMOVE:
        break;
    default:    // 마우스 메세지가 아닐경우 카메라 움직임이 아니다.
        return FALSE;
    }

    DWORD nFlags = pMsg->wParam;
    CPoint point(short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam)));
    CPoint ptDelta = point - ptPrev;
    ptPrev = point;

    CN3Camera* pCamera = m_pSceneOutput->CameraGetActive();
    if (pCamera == NULL) return FALSE;

    if(    (nFlags & MK_LBUTTON) &&
        (nFlags & MK_MBUTTON) ) // Alt + LB + MB
    {
        float fZoom = -(float)(ptDelta.x)/1000.0f;
        pCamera->Zoom(fZoom);
        pCamera->Apply();
        SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_ZOOM)));
        return TRUE;
    }
    else if((nFlags & MK_LBUTTON) ) // Alt + LB
    {
        float fRX = (float)(ptDelta.y)/200.0f;
        float fRY = -(float)(ptDelta.x)/200.0f;
        pCamera->LookAround(fRX, fRY);
        pCamera->Apply();
        SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_ROTATE)));
        return TRUE;
    }
    else if((nFlags & MK_RBUTTON) ) // Alt + RB
    {
        float fRX = (float)(ptDelta.y)/200.0f;
        float fRY = (float)(ptDelta.x)/200.0f;
        pCamera->Rotate(fRX, -fRY);
        pCamera->Apply();
        return TRUE;
    }
    else if((nFlags & MK_MBUTTON) ) // Alt + MB
    {
        __Vector3 vDelta = pCamera->EyePos() - pCamera->AtPos();
        float fDelta = vDelta.Magnitude() * 0.002f / pCamera->m_Data.fFOV;
        __Vector3 vD1;
        vD1.Set(-(float)(ptDelta.x), (float)(ptDelta.y), 0 );

        vD1 *= fDelta;

        pCamera->MovePlane(vD1.x, -vD1.y);
        pCamera->Apply();
        SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_MOVE)));
        return TRUE;
    }
    return FALSE;
}
*/

BOOL CMapMng::MouseMsgFilter(LPMSG pMsg) {
    switch (pMsg->message) {
    case WM_LBUTTONDOWN: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (m_CursorMode == CM_FLAT_TERRAIN && m_pBrushDlg->m_rdoFlatMode == 0) {
            __Vector3 vPos;
            if (m_pTerrain && m_pTerrain->Pick(point.x, point.y, &vPos)) {
                m_pBrushDlg->SetFlatHeight(vPos.y);
            }
        }
    } break;
    case WM_KEYDOWN: {
        switch (pMsg->wParam) {
        case 0x20: //space bar...
        {
            CPoint point = ((CN3MEView *)m_pMainFrm->GetActiveView())->m_CurrMousePos;
            if (!m_pTerrain) {
                break;
            }

            __Vector3 vec;

            if (!m_pTerrain->Pick(point.x, point.y, &vec, NULL)) {
                break;
            }

            CStatusBar * pBar = GetStatusBar();
            CString      str;
            str.Format("X:%4.2f Y:%4.2f Z:%4.2f", vec.x, vec.y, vec.z);
            if (pBar) {
                pBar->SetPaneText(0, str);
            }
            Invalidate();
        }
        }
    }
    }

    static BOOL bSelectDrag = FALSE;
    if (bSelectDrag == FALSE &&                    // Object drag select 모드가 아니고
        RCM_SELECT != m_RiverMng.GetRCursorMode()) // 강편집의 drag select모드가 아닐때
    {
        CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
        if (pCamera && pCamera->MoveByWindowMessage(pMsg)) {
            return TRUE; // 카메라 이동 메세지 거르기
        }
    }
    if (bSelectDrag == FALSE && PCM_SELECT != m_PondMng.GetPCursorMode()) {
        CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
        if (pCamera && pCamera->MoveByWindowMessage(pMsg)) {
            return TRUE;
        }
    }

    if (m_pTerrain && m_pTerrain->MouseMsgFilter(pMsg)) {
        return TRUE; // 지형 편집 메세지 거르기
    }
    if (m_RiverMng.MouseMsgFilter(pMsg)) {
        return TRUE; // 강 편집 메세지 거르기
    }
    if (m_PondMng.MouseMsgFilter(pMsg)) {
        return TRUE; // 연못 편집 메시지 거르기
    }
    if (m_pNPCPath && m_pNPCPath->MouseMsgFilter(pMsg)) {
        return TRUE; // NPC 길만들기 메세지 거르기
    }
    if (m_pWall && m_pWall->MouseMsgFilter(pMsg)) {
        return TRUE;
    }
    if (m_pEventMgr && m_pEventMgr->MouseMsgFilter(pMsg)) {
        return TRUE;
    }
    if (m_pRegenUser && m_pRegenUser->MouseMsgFilter(pMsg)) {
        return TRUE;
    }
    if (m_pSoundMgr && m_pSoundMgr->MouseMsgFilter(pMsg)) {
        return TRUE;
    }
    if (m_pLightObjMgr && m_pLightObjMgr->MouseMsgFilter(pMsg)) {
        return TRUE;
    }

    // Dummy Cube움직이는 메세지 거르기
    if (m_pDummy && m_pDummy->MouseMsgFilter(pMsg)) {
        return TRUE;
    }

    {
        HWND hWnd = GetFocus();
        if (hWnd == m_pMainFrm->GetActiveView()->GetSafeHwnd()) {
            if (GetAsyncKeyState(VK_CONTROL) & 0xff00) {
                if (GetAsyncKeyState(0x43) & 0xff00) //    'c'
                {
                    SetEditState(eEDIT_COPY);
                } else if (GetAsyncKeyState(0x56) & 0xff00) //    'v'
                {
                    SetEditState(eEDIT_PASTE);
                }
            }
        }
    }

    // 나머지 객체 선택 및 배치
    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (bSelectDrag) {
            m_rcSelDrag.right = point.x;
            m_rcSelDrag.bottom = point.y;
            return TRUE;
        } else if (m_CursorMode == CM_OBJECT_BRUSH && m_pSelSourceObj) {
            __Vector3 vPos;
            if (m_pTerrain && m_pTerrain->Pick(point.x, point.y, &vPos)) {
                m_pSelSourceObj->PosSet(vPos);
                m_pSelSourceObj->Tick();
                m_bRenderSelObj = TRUE;
                CStatusBar * pBar = GetStatusBar();
                CString      str;
                str.Format("X:%f Y:%f Z:%f", vPos.x, vPos.y, vPos.z);
                if (pBar) {
                    pBar->SetPaneText(0, str);
                }
                return TRUE;
            }
        } else if (m_CursorMode == CM_ROTATE && m_pTerrain) {
            return TRUE;
            /*
                __Vector3 vPos;
                CN3Transform* pDestObj = NULL;

                int iSize = m_SelOutputObjArray.GetSize();
                for(int j=0;j<iSize;++j)
                {
                    pDestObj = m_SelOutputObjArray.GetAt(j);
                    if(pDestObj==NULL) continue;

                    vPos = pDestObj->Pos();
                    vPos.y = m_pTerrain->GetHeight(vPos.x,vPos.z);
                    pDestObj->PosSet(vPos);
                }
                */
        }
    } break;
    case WM_LBUTTONDOWN: // 객체 선택
    {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (m_CursorMode == CM_OBJECT_BRUSH && m_pSelSourceObj) {
            __Vector3 vPos;
            if (m_pTerrain && m_pTerrain->Pick(point.x, point.y, &vPos)) {
                m_pSelSourceObj->PosSet(vPos);
                m_bRenderSelObj = FALSE;

                AddObjectToOutputScene(m_pSelSourceObj);
                return TRUE;
            }

        } else if (m_CursorMode == CM_SELECT || m_CursorMode == CM_POS || m_CursorMode == CM_ROTATE ||
                   m_CursorMode == CM_SCALE) {
            SetCapture(pMsg->hwnd);
            bSelectDrag = TRUE;
            m_rcSelDrag.right = m_rcSelDrag.left = point.x;
            m_rcSelDrag.bottom = m_rcSelDrag.top = point.y;
            return TRUE;
        }
    } break;
    case WM_LBUTTONUP: // 객체 선택
    {
        if (bSelectDrag) {
            POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
            if (abs(m_rcSelDrag.left - point.x) < 4 && abs(m_rcSelDrag.top - point.y) < 4) // 클릭한걸로 간주
            {
                int       nPart = -1;
                CN3Base * pBaseSel = Pick(point, &nPart);

                SelectObject(pBaseSel, FALSE, (GetAsyncKeyState(VK_SHIFT) & 0xff00) ? TRUE : FALSE);
                if (m_pDlgBase && pBaseSel && pBaseSel->Type() & OBJ_SHAPE) // 파트 선택..
                {
                    m_pDlgBase->m_CBShapePart.SetCurSel(nPart);
                }
            } else { // 드레그 한 것
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
                SelectObjectByDragRect(&m_rcSelDrag, (GetAsyncKeyState(VK_SHIFT) & 0xff00) ? TRUE : FALSE);
            }
            ReleaseCapture();
            bSelectDrag = FALSE;
            m_rcSelDrag.left = m_rcSelDrag.top = m_rcSelDrag.right = m_rcSelDrag.bottom = 0;

            m_pMainFrm->UpdateTransformInfo(); // 위치, 회전값등을 업데이트한다.

            return TRUE;
        }
    } break;
    case WM_RBUTTONUP: // 객체 선택 해제
    {
        if (bSelectDrag) {
            ReleaseCapture();
            bSelectDrag = FALSE;
            m_rcSelDrag.left = m_rcSelDrag.top = m_rcSelDrag.right = m_rcSelDrag.bottom = 0;
            return TRUE;
        }
    } break;
    }
    return FALSE;
}

CN3Base * CMapMng::Pick(POINT point, int * pnPart) // Object Picking...
{
    static __Sort sort[MAX_SCENE_SHAPE];
    int           nSortCount = 0;
    memset(sort, 0, sizeof(sort));

    CPick pick;
    pick.SetPickXY(point.x, point.y);
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();

    int        nSC = m_pSceneOutput->ShapeCount();
    CN3Shape * pShape = NULL;
    //    CN3PMesh* pPMesh;
    __Vector3 vDist;
    for (int i = 0; i < nSC; i++) {
        pShape = m_pSceneOutput->ShapeGet(i);
        if (pShape == NULL) {
            continue;
        }

        //        int nPC = pShape->PartCount();
        //        for(int j = 0; j < nPC; j++)
        //        {
        //            CN3SPart* pPart = pShape->Part(j);
        //            if (pPart == NULL) continue;
        //            pPMesh = pPart->Mesh();
        //            if(pPMesh == NULL) continue;
        //
        vDist = pShape->s_CameraData.vEye - pShape->Pos();

        sort[nSortCount].pObj = pShape;
        sort[nSortCount].fCamDist = vDist.Magnitude();
        sort[nSortCount].vMin = pShape->Min();
        sort[nSortCount].vMax = pShape->Max();

        nSortCount++;
        //        }
    }
    /*
    CN3Chr* pChr;
    CN3Mesh* pMesh;
    int nCC = m_pSceneOutput->ChrCount();
    for(int i = 0; i < nCC; i++)
    {
        pChr = m_pSceneOutput->ChrGet(i);
        _ASSERT(pChr);

        int nPC = pChr->PartCount();
        for(int j = 0; j < nPC; j++)
        {
            if(pChr->MeshGet(0, j) == NULL) continue;

            pMesh = pChr->MeshBoxGet(j);
            if(pMesh == NULL) continue;

            vDist = pChr->s_CameraData.vEye - pChr->m_vPos;

            sort[nSortCount].pObj = pChr;
            sort[nSortCount].fCamDist = vDist.Magnitude();
            sort[nSortCount].vMin = pMesh->Min() * pChr->m_Matrix;
            sort[nSortCount].vMax = pMesh->Max() * pChr->m_Matrix;

            nSortCount++;
        }
    }
*/
    qsort(sort, nSortCount, sizeof(__Sort), SortByCameraDistance);

    int       nPart = -1;
    __Vector3 vI;
    for (int i = 0; i < nSortCount; i++) {
        //        bIntersect = pick.PickByBox(sort[i].vMin, sort[i].vMax, vI);
        nPart = sort[i].pObj->CheckCollisionPrecisely(true, point.x, point.y);
        if (nPart >= 0) {
            if (pnPart) {
                *pnPart = nPart;
            }
            return sort[i].pObj;
        }
    }
    return NULL;
}

void CMapMng::SelectObject(CN3Base * pObj, BOOL IsSourceObj, BOOL bAdd) {
    if (IsSourceObj) { // source 선택
        if (pObj && pObj->Type() & (OBJ_CHARACTER | OBJ_SHAPE)) {
            m_pSelSourceObj = (CN3Transform *)pObj;
        } else {
            m_pSelSourceObj = NULL;
        }
    } else { // 이미 배치된 객체 선택
        CN3Transform * pSelObj = NULL;
        if (pObj && pObj->Type() & (OBJ_CHARACTER | OBJ_SHAPE)) {
            pSelObj = (CN3Transform *)pObj;
            if (bAdd) // 추가
            {
                BOOL bAleadySelected = FALSE;
                int  i, iSize = m_SelOutputObjArray.GetSize();
                for (i = 0; i < iSize; ++i) {
                    if (m_SelOutputObjArray.GetAt(i) == pSelObj) {
                        bAleadySelected = TRUE;
                        break;
                    }
                }
                if (bAleadySelected) {
                    m_SelOutputObjArray.RemoveAt(i); // 이미 있으므로 선택목록에서 제거
                } else {
                    m_SelOutputObjArray.InsertAt(0, pSelObj); // 추가
                }
            } else // 새로 선택
            {
                m_SelOutputObjArray.RemoveAll();
                m_SelOutputObjArray.Add(pSelObj);
            }
        } else // 잘못 선택했거나 캐릭터나 shape를 선택하지 않았다.
        {
            if (bAdd == FALSE) {
                m_SelOutputObjArray.RemoveAll();
            }
        }
        OnSelChanged();
    }

    if (m_SelOutputObjArray.GetSize() == 1) // 한개를 선택했다면..
    {
        m_pMainFrm->UpdateTransformInfo();
    }
}

void CMapMng::OnSelChanged() {
    CN3Transform * pSelObj = NULL;
    int            iCount = m_SelOutputObjArray.GetSize();
    if (iCount > 0) {
        pSelObj = m_SelOutputObjArray.GetAt(0);
    }
    if (pSelObj) {
        if (m_pDlgBase && m_pDlgBase->IsWindowVisible()) {
            m_pDlgBase->UpdateInfo();
            m_pDlgBase->UpdateWindow();
        }
        if (iCount == 1 && m_pDlgOutputList && m_pDlgOutputList->GetSafeHwnd() != NULL) {
            //m_pDlgOutputList->SelectObject(TVI_ROOT, pSelObj);
            m_pDlgOutputList->SelectObject(pSelObj);
        }
    }
    if (m_pDummy) {
        m_pDummy->SetSelObj(pSelObj);
        for (int i = 1; i < iCount; ++i) {
            m_pDummy->AddSelObj(m_SelOutputObjArray.GetAt(i));
        }
    }
    Invalidate();
}

void CMapMng::SelectObjectByDragRect(RECT * pRect, BOOL bAdd) {
    if (pRect == NULL || m_pSceneOutput == NULL) {
        return;
    }
    if (bAdd == FALSE) {
        m_SelOutputObjArray.RemoveAll();
    }

    CN3Eng *          pEng = m_pMainFrm->m_pEng;
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    __Matrix44 matView, matProj, matVP;
    pD3DDev->GetTransform(D3DTS_VIEW, &matView);
    pD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixMultiply(&matVP, &matView, &matProj);

    D3DVIEWPORT9 vp = pEng->s_CameraData.vp;

    int iSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < iSC;) {
        CN3TransformCollision * pObj = m_pSceneOutput->ShapeGet(i);
        D3DXVECTOR4             v;
        __Vector3               vPos = pObj->Pos();
        D3DXVec3Transform(&v, &vPos, &matVP);
        float fScreenZ = (v.z / v.w);
        if (fScreenZ < 1.0 && fScreenZ > 0.0) {
            float fScreenX = ((v.x / v.w) + 1.0f) * (vp.Width) / 2.0f;
            float fScreenY = (1.0f - (v.y / v.w)) * (vp.Height) / 2.0f;
            if (fScreenX >= pRect->left && fScreenX <= pRect->right && fScreenY >= pRect->top &&
                fScreenY <= pRect->bottom) {
                BOOL bAleadySelected = FALSE;
                int  j, iSize = m_SelOutputObjArray.GetSize();
                for (j = 0; j < iSize; ++j) {
                    if (m_SelOutputObjArray.GetAt(j) == pObj) {
                        bAleadySelected = TRUE;
                        break;
                    }
                }
                if (bAleadySelected) {
                    m_SelOutputObjArray.RemoveAt(j); // 이미 있으므로 선택목록에서 제거
                } else {
                    m_SelOutputObjArray.InsertAt(0, pObj); // 추가
                }
            }
        }
        ++i;
    }
    OnSelChanged();
}

int CMapMng::SortByCameraDistance(const void * pArg1, const void * pArg2) {
    __Sort * pObj1 = (__Sort *)pArg1;
    __Sort * pObj2 = (__Sort *)pArg2;

    if (pObj1->fCamDist < pObj2->fCamDist) {
        return -1;
    } else if (pObj1->fCamDist > pObj2->fCamDist) {
        return 1;
    } else {
        return 0;
    }
}

void CMapMng::MakeGameFiles(const fs::path & fsFile, float fSize) {
    if (NULL == m_pTerrain || NULL == m_pSceneOutput) {
        return;
    }
    if (fsFile.empty()) {
        return;
    }

    fs::path fsFileTmp(fsFile);

    // 파일 저장.
    HANDLE hGmdFile =
        CreateFileW(fsFileTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hGmdFile) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Fail to open map game data file for save, Pleas retry.",
                    MB_OK);
        return;
    }
    char  comment[80] = {"이파일 여는 사람 바보..^^"};
    DWORD dwRWC;
    WriteFile(hGmdFile, &m_iZoneID, sizeof(int), &dwRWC, NULL);
    WriteFile(hGmdFile, &comment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hGmdFile);

    // 지형정보 저장
    m_pTerrain->FilePathSet(fsFileTmp.replace_extension(".gtd"));
    HANDLE hGtdFile =
        CreateFileW(fsFileTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hGtdFile) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Failed to open terrain file for save", MB_OK);
    } else {
        m_pTerrain->m_szName = fsFileTmp.stem().string(); // 이름을 지정한다.. 이 이름대로 저장된다.
        m_pTerrain->SaveGameData(hGtdFile);

        m_pTerrain->MakeGameColorMap(fsFileTmp.replace_extension(".tct"));

        m_RiverMng.MakeGameFiles(hGtdFile, fSize);
        m_PondMng.MakeGameFiles(hGtdFile, fSize);
        //m_pSoundMgr->SaveGameData(hGtdFile);
        CloseHandle(hGtdFile);

        m_pTerrain->MakeGameLightMap(fsFileTmp.replace_extension(".tlt"));
    }

    // Shape Manager 만들고 저장...
    //if (true == m_pSceneOutput->CheckOverlappedShapesAndReport()) { // 이름이나 위치 중복 확인..
    //    int idYesNo = ::MessageBox(::GetActiveWindow(), "중복된 오브젝트들을 삭제 하시겠습니까?",
    //                               "중복된 오브젝트 처리", MB_YESNO);
    //    if (IDYES == idYesNo) {
    //        m_pSceneOutput->DeleteOverlappedShapes();
    //        m_pDlgOutputList->UpdateTree(m_pSceneOutput); // 아웃풋 리스트 체크..
    //        m_SelOutputObjArray.RemoveAll();              // 셀렉션 초기화..
    //    }
    //}

    std::unique_ptr<CN3ShapeMgr> pShapeMgr = std::make_unique<CN3ShapeMgr>();
    pShapeMgr->Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                      (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
    int nSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < nSC; i++) {
        pShapeMgr->Add(m_pSceneOutput->ShapeGet(i)); // Shape 추가.
    }

    if (m_pWall) {
        m_pWall->AddWall2Coll(pShapeMgr.get());
    }
    // "Object Posting Data" - Shape Manager file
    pShapeMgr->GenerateCollisionData(); // 충돌 메시 데이터를 생성한다...
    pShapeMgr->SaveToFile(fsFileTmp.replace_extension(".opd"));

    //이벤트 저장..
    if (!m_pEventMgr->MakeGameFile(fsFileTmp.replace_extension(".gev"), m_pTerrain->m_iHeightMapSize)) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Failed to make game event file...", MB_OK);
    }

    if (!m_pLightObjMgr->MakeGameFile(fsFileTmp.replace_extension(".glo"))) {
        MessageBoxW(::GetActiveWindow(), fsFileTmp.c_str(), L"Failed to make game light info file...", MB_OK);
    }
}

void CMapMng::MakeTerrainMovableAttr(CN3ShapeMgr * pShapeMgr) {
    m_pTerrain->MakeMoveTable(m_pEventMgr->m_ppEvent); //움직임 속성 셋팅...
    pShapeMgr->MakeMoveTable(m_pEventMgr->m_ppEvent);  //움직임 속성 셋팅...
}

void CMapMng::MakeServerDataFiles(const fs::path & fsFile) {
    if (NULL == m_pEventMgr->m_ppEvent) {
        m_pEventMgr->SetActive(true);
        m_pEventMgr->SetActive(false);
    }

    HANDLE hSmdFile = CreateFileW(fsFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hSmdFile) {
        m_pMainFrm->MessageBox("file write error");
        return;
    }

    //terrain 저장..
    m_pTerrain->SaveServerData(hSmdFile);
    //

    // Shape Manager 만들고 저장...
    //if (true == m_pSceneOutput->CheckOverlappedShapesAndReport()) { // 이름이나 위치 중복 확인..
    //    int idYesNo = ::MessageBox(::GetActiveWindow(), "중복된 오브젝트들을 삭제 하시겠습니까?",
    //                               "중복된 오브젝트 처리", MB_YESNO);
    //    if (IDYES == idYesNo) {
    //        m_pSceneOutput->DeleteOverlappedShapes();
    //        m_pDlgOutputList->UpdateTree(m_pSceneOutput); // 아웃풋 리스트 체크..
    //        m_SelOutputObjArray.RemoveAll();              // 셀렉션 초기화..
    //    }
    //}

    std::unique_ptr<CN3ShapeMgr> pShapeMgr = std::make_unique<CN3ShapeMgr>();
    pShapeMgr->Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                      (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
    int nSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < nSC; i++) {
        pShapeMgr->Add(m_pSceneOutput->ShapeGet(i)); // Shape 추가.
    }
    if (m_pWall) {
        m_pWall->AddWall2Coll(pShapeMgr.get());
    }
    pShapeMgr->GenerateCollisionData(); // 충돌 메시 데이터를 생성한다..
    MakeTerrainMovableAttr(pShapeMgr.get());
    pShapeMgr->SaveCollisionData(hSmdFile); // 충돌 데이터만 저장...

    // Object Event 저장.
    //
    DWORD dwNum;
    int   iEventObjectCount = 0; // 먼저 갯수를 세고..
    for (int i = 0; i < nSC; i++) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        if (pShape->m_iEventID || pShape->m_iEventType || pShape->m_iNPC_ID ||
            pShape->m_iNPC_Status) { // 이벤트가 있으면
            iEventObjectCount++;
        }
    }

    WriteFile(hSmdFile, &iEventObjectCount, 4, &dwNum, NULL);
    for (int i = 0; i < nSC; i++) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        short      sEvent = 0;
        __Vector3  vPos;
        if (pShape->m_iEventID || pShape->m_iEventType || pShape->m_iNPC_ID || pShape->m_iNPC_Status) // 이벤트가 있으면
        {
            WriteFile(hSmdFile, &(pShape->m_iBelong), 4, &dwNum, NULL);
            sEvent = (short)(pShape->m_iEventID);
            WriteFile(hSmdFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iEventType);
            WriteFile(hSmdFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iNPC_ID);
            WriteFile(hSmdFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iNPC_Status);
            WriteFile(hSmdFile, &sEvent, 2, &dwNum, NULL);

            vPos = pShape->Pos();
            WriteFile(hSmdFile, &(vPos.x), 4, &dwNum, NULL);
            WriteFile(hSmdFile, &(vPos.y), 4, &dwNum, NULL);
            WriteFile(hSmdFile, &(vPos.z), 4, &dwNum, NULL);
        }
    }

    //MapTile속성 저장...
    //
    m_pEventMgr->MakeEventArray();
    /*
    for (int z = 0; z < m_pTerrain->m_iHeightMapSize; z++) {
        for (int x = 0; x < m_pTerrain->m_iHeightMapSize; x++) {
            WriteFile(hSmdFile, &(m_pEventMgr->m_ppEvent[x][z]), sizeof(short), &dwNum, NULL);
        }
    }
    */
    for (int x = 0; x < m_pTerrain->m_iHeightMapSize; x++) {
        WriteFile(hSmdFile, m_pEventMgr->m_ppEvent[x], sizeof(short) * m_pTerrain->m_iHeightMapSize, &dwNum, NULL);
    }

    m_pRegenUser->SaveServerData(hSmdFile);
    m_pWarpMgr->SaveServerData(hSmdFile);

    //이벤트 정보 저장..
    fs::path fsEventFile = fsFile.parent_path() / (fsFile.stem() + "_Event.txt");
    m_pEventMgr->SaveInfoTextFile(fsEventFile); //서버에서 쓰는 이벤트 아이디 조건들이 들어 있는 텍스트 파일..

    // 텍스트파일로 함 뽑아보자..
    fs::path fsTmpMoveFile = fs::temp_directory_path() / "N3ME_move.txt";
    FILE *   stream = _wfopen(fsTmpMoveFile.c_str(), L"w");
    for (int z = m_pTerrain->m_iHeightMapSize - 1; z >= 0; z--) {
        for (int x = 0; x < m_pTerrain->m_iHeightMapSize; x++) {
            int v = m_pEventMgr->m_ppEvent[x][z];
            fprintf(stream, "%d ", v);
        }
        fprintf(stream, "\n");
    }
    fclose(stream);
    //뽑았다.

    /*
    fs::path fsCollisionFile = fs::path(fsFile).replace_extension(".scd");
    CFile    file;
    file.Open(fsCollisionFile.string().c_str(), CFile::modeCreate | CFile::modeWrite);
    pShapeMgr->SaveCollisionData((HANDLE)file.m_hFile); // 충돌 데이터만 저장...
    file.Close();
    */
    CloseHandle(hSmdFile);
}

BOOL CMapMng::GetObjectMinMax(CN3Transform * pObj, __Vector3 & vMin, __Vector3 & vMax) {
    if (pObj == NULL) {
        return FALSE;
    }
    DWORD dwType = pObj->Type();
    /*
    if ( dwType & OBJ_CHARACTER)
    {
        CN3Chr* pChr = (CN3Chr*)pObj;
        CN3Mesh* pMesh;
        vMin.Set(FLT_MAX,FLT_MAX,FLT_MAX); vMax.Set(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        int nPC = pChr->PartCount();
        for(int i = 0; i < nPC; i++)
        {
            if(pChr->MeshGet(0, i) == NULL) continue;

            pMesh = pChr->MeshBoxGet(i);
            if(pMesh == NULL) continue;

            __Vector3 vMeshMin, vMeshMax;
            vMeshMin = pMesh->Min() * pChr->m_Matrix;
            vMeshMax = pMesh->Max() * pChr->m_Matrix;

            if (vMin.x>vMeshMin.x) vMin.x = vMeshMin.x;
            if (vMin.y>vMeshMin.y) vMin.y = vMeshMin.y;
            if (vMin.z>vMeshMin.z) vMin.z = vMeshMin.z;
            if (vMax.x<vMeshMax.x) vMax.x = vMeshMax.x;
            if (vMax.y<vMeshMax.y) vMax.y = vMeshMax.y;
            if (vMax.z<vMeshMax.z) vMax.z = vMeshMax.z;
        }
    }
    else*/
    if (dwType & OBJ_SHAPE) {
        CN3Shape * pShape = (CN3Shape *)pObj;
        CN3PMesh * pPMesh;
        vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
        vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        int nPC = pShape->PartCount();
        for (int i = 0; i < nPC; i++) {
            CN3SPart * pPart = pShape->Part(i);
            if (pPart == NULL) {
                continue;
            }
            pPMesh = pPart->Mesh();
            if (pPMesh == NULL) {
                continue;
            }

            __Vector3 vMeshMin, vMeshMax;
            vMeshMin = pPMesh->Min() * pShape->m_Matrix;
            vMeshMax = pPMesh->Max() * pShape->m_Matrix;

            if (vMin.x > vMeshMin.x) {
                vMin.x = vMeshMin.x;
            }
            if (vMin.y > vMeshMin.y) {
                vMin.y = vMeshMin.y;
            }
            if (vMin.z > vMeshMin.z) {
                vMin.z = vMeshMin.z;
            }
            if (vMax.x < vMeshMax.x) {
                vMax.x = vMeshMax.x;
            }
            if (vMax.y < vMeshMax.y) {
                vMax.y = vMeshMax.y;
            }
            if (vMax.z < vMeshMax.z) {
                vMax.z = vMeshMax.z;
            }
        }
    } else {
        return FALSE;
    }

    return TRUE;
}

void CMapMng::RenderObjectToWindow(CN3TransformCollision * pObj, HWND hWnd) {
    if (pObj == NULL || hWnd == NULL) {
        return;
    }
    DWORD dwType = pObj->Type();
    if (!(dwType & OBJ_CHARACTER || dwType & OBJ_SHAPE)) {
        return;
    }
    ASSERT(m_pMainFrm);

    CN3Eng *          pEng = m_pMainFrm->m_pEng;
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    // Save CameraData
    __CameraData CameraDataBackUp;
    CopyMemory(&CameraDataBackUp, &CN3Base::s_CameraData, sizeof(CameraDataBackUp));

    // clear & Set Viewport - tigger
    CRect rc;
    ::GetClientRect(hWnd, &rc);
    pEng->Clear(0xff606060, rc);
    pEng->SetViewPort(rc);

    // begin
    pD3DDev->BeginScene();

    // Object의 위치 및 크기 파악
    __Vector3 vDir(-1, -1, 3);
    vDir.Normalize();
    __Vector3 vMin = pObj->Min();
    __Vector3 vMax = pObj->Max();

    __Vector3 vEye, vAt, vUp(0, 1, 0);
    vAt = vMin + (vMax - vMin) / 2.0f;
    vAt.y -= (vMax - vMin).Magnitude() / 8.0f;
    vEye = vAt - vDir * ((vMax - vMin).Magnitude() * 0.7f);
    //    pEng->s_CameraData.vEye = vEye;    // Shape의 LOD설정 때문에..

    // back up
    __Matrix44 mtxOldView, mtxOldProj;
    pD3DDev->GetTransform(D3DTS_VIEW, &mtxOldView);
    pD3DDev->GetTransform(D3DTS_PROJECTION, &mtxOldProj);
    DWORD dwLighting;
    pD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);

    // camera frustum 세팅..(Apply함수 내부에서 transform을 바꾸기때문에 이 위치에 넣어야 한다.)
    //    pEng->s_CameraData.vAt = vAt;
    //    TempCamera.s_CameraData = pEng->s_CameraData;
    CN3Camera TempCamera;
    TempCamera.EyePosSet(vEye);
    TempCamera.AtPosSet(vAt);
    TempCamera.UpVectorSet(vUp);
    TempCamera.Tick();
    TempCamera.Apply(); // 임시카메라에 데이터를 넣고 frustum 정보를 계산..

    // View Matrix 및 Projection Matrix Setting
    //    __Matrix44 viewmtx;
    //    ::D3DXMatrixLookAtLH(&viewmtx, &vEye, &vAt, &vUp);
    //    pD3DDev->SetTransform(D3DTS_VIEW, &viewmtx);
    //    __Matrix44 prjmtx;
    //    ::D3DXMatrixPerspectiveFovLH(&prjmtx, D3DXToRadian(54.0f), pEng->s_CameraData.fAspect, 0.01f, 1000.0f);
    //    pD3DDev->SetTransform(D3DTS_PROJECTION, &prjmtx);

    // Set Render State
    pD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    // 그리기
    if (dwType & OBJ_CHARACTER) {
        CN3Chr * pChr = (CN3Chr *)pObj;
        pChr->Tick(-1000);
        pChr->Render();
    } else if (dwType & OBJ_SHAPE) {
        CN3Shape * pShape = (CN3Shape *)pObj;
        pShape->Tick(-1000);
        pShape->Render();
    }

    CN3Base::s_AlphaMgr.Render(); // Alpha primitive 그리기...
    pD3DDev->EndScene();          // end
    pEng->Present(hWnd);          // present

    // restore (이전 상태로 되돌려주지 않으면 지형에서 picking이 제대로 되지 않는다)
    pD3DDev->SetTransform(D3DTS_VIEW, &mtxOldView);
    pD3DDev->SetTransform(D3DTS_PROJECTION, &mtxOldProj);
    CopyMemory(&CN3Base::s_CameraData, &CameraDataBackUp, sizeof(CameraDataBackUp));
    pD3DDev->SetViewport(&CN3Base::s_CameraData.vp);
    pD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
}

void CMapMng::SetCursorMode(int iMode) {
    if (m_CursorMode == iMode) {
        return;
    }
    if (NULL == m_pBrushDlg) {
        MessageBox(::GetActiveWindow(), "Please, Activate brush dialog", "NULL brush dialog", MB_OK);
        return;
    }

    if (m_pDummy) {
        m_pDummy->SetSelObj(NULL);
    }

    m_pBrushDlg->SetTerrain(NULL); // Brush 창을 보이지 않게한다.
    if (m_pTerrain) {
        m_pTerrain->SetEditMode(TEM_NOT);
    }
    if (m_pNPCPath) {
        m_pNPCPath->SetActive(false);
    }
    if (m_pWall) {
        m_pWall->SetActive(false);
    }
    if (m_pEventMgr) {
        m_pEventMgr->SetActive(false);
    }
    if (m_pRegenUser) {
        m_pRegenUser->SetActive(false);
    }
    if (m_pSoundMgr) {
        m_pSoundMgr->SetActive(false);
    }
    if (m_pLightObjMgr) {
        m_pLightObjMgr->SetActive(false);
    }

    m_RiverMng.SetEditMode(FALSE);
    m_PondMng.SetEditMode(FALSE);

    switch (iMode) {
    case CM_SELECT:
        m_pDummy = NULL;
        break;
    case CM_OBJECT_BRUSH:
        m_pDummy = NULL;
        break;
    case CM_POS:
        m_pDummy = &m_PosDummy;
        m_pMainFrm->UpdateTransformInfo(); // 위치, 회전값등을 업데이트한다.
        break;
    case CM_ROTATE:
        m_pDummy = &m_RotDummy;
        break;
    case CM_SCALE:
        m_pDummy = &m_ScaleDummy;
        m_pMainFrm->UpdateTransformInfo(); // 위치, 회전값등을 업데이트한다.
        break;
    case CM_EDIT_TERRAIN:
        m_pBrushDlg->SetTerrain(m_pTerrain);
        if (m_pTerrain) {
            m_pTerrain->SetEditMode(TEM_HEIGHTEN);
        }
        break;
    case CM_SMOOTH_TERRAIN:
        m_pBrushDlg->SetTerrain(m_pTerrain);
        if (m_pTerrain) {
            m_pTerrain->SetEditMode(TEM_SMOOTH);
        }
        break;
    case CM_FLAT_TERRAIN:
        m_pBrushDlg->SetTerrain(m_pTerrain);
        if (m_pTerrain) {
            m_pTerrain->SetEditMode(TEM_FLAT);
        }
        break;
    case CM_SET_DTEX:
        if (m_pTerrain) {
            m_pTerrain->SetEditMode(TEM_DTEX);
        }
        break;
    case CM_EDIT_RIVER:
        m_RiverMng.SetEditMode(TRUE);
        break;
    case CM_EDIT_POND:
        m_PondMng.SetEditMode(TRUE);
        break;
    case CM_EDIT_LIGHTMAP:
        if (m_pTerrain) {
            m_pTerrain->SetEditMode(TEM_LIGHTMAP);
        }
        break;
    case CM_MAKE_NPCPATH:
        if (m_pTerrain && m_pNPCPath) {
            if (m_pEventMgr && m_pNPCPath->m_pppRefEvent == NULL) {
                m_pEventMgr->SetActive(true);
                m_pEventMgr->SetActive(false);

                std::unique_ptr<CN3ShapeMgr> pShapeMgr = std::make_unique<CN3ShapeMgr>();
                pShapeMgr->Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                                  (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
                int nSC = m_pSceneOutput->ShapeCount();
                for (int i = 0; i < nSC; i++) {
                    pShapeMgr->Add(m_pSceneOutput->ShapeGet(i)); // Shape 추가.
                }
                if (m_pWall) {
                    m_pWall->AddWall2Coll(pShapeMgr.get());
                }
                pShapeMgr->GenerateCollisionData(); // 충돌 메시 데이터를 생성한다..

                MakeTerrainMovableAttr(pShapeMgr.get());
                m_pNPCPath->m_pppRefEvent = m_pEventMgr->m_ppEvent;
            }
            m_pNPCPath->SetActive(true);
        }
        break;
    case CM_MAKE_WALL:
        if (m_pTerrain && m_pWall) {
            m_pWall->SetActive(true);
        }
        break;
    case CM_EDIT_EVENT:
        if (m_pTerrain && m_pEventMgr) {
            m_pEventMgr->SetActive(true);
        }
        break;
    case CM_EDIT_REGENUSER:
        if (m_pTerrain && m_pRegenUser) {
            m_pRegenUser->SetActive(true);
        }
        break;
    case CM_EDIT_SOUND:
        if (m_pTerrain && m_pSoundMgr) {
            m_pSoundMgr->SetActive(true);
        }
        break;
    case CM_EDIT_LIGHT: {
        m_pLightObjMgr->SetActive(TRUE);
    } break;

    default:
        ASSERT(0);
        break;
    }

    m_CursorMode = iMode;
    if (m_pDummy) {
        int iSize = m_SelOutputObjArray.GetSize();
        if (iSize > 0) {
            m_pDummy->SetSelObj(m_SelOutputObjArray.GetAt(0));
            for (int i = 1; i < iSize; ++i) {
                m_pDummy->AddSelObj(m_SelOutputObjArray.GetAt(i));
            }
        } else {
            m_pDummy->SetSelObj(NULL);
        }

        m_pDummy->m_pTerrainRef = m_pTerrain; // 더미에 지형 포인터를 넣어준다.
    }
    m_pMainFrm->GetActiveView()->Invalidate(FALSE);
}

void CMapMng::Invalidate() {
    if (m_pMainFrm) {
        CView * pView = m_pMainFrm->GetActiveView();
        if (pView) {
            pView->Invalidate(FALSE);
        }
    }
}

void CMapMng::DropSelObjToTerrain() { // 선택한 객체를 지형에 붙인다.(Y값만 조정)
    if (m_pTerrain == NULL) {
        return;
    }
    int iSize = m_SelOutputObjArray.GetSize();
    for (int i = 0; i < iSize; ++i) {
        CN3Transform * pSelObj = m_SelOutputObjArray.GetAt(i);
        ASSERT(pSelObj);
        __Vector3 vPos = pSelObj->Pos();
        vPos.y = m_pTerrain->GetHeight(vPos.x, vPos.z);
        pSelObj->PosSet(vPos);
        if (i == 0 && m_pDummy) {
            m_pDummy->PosSet(vPos);
        }
    }
    Invalidate();
}

void CMapMng::ImportTerrain(const fs::path & fsVmeshFile) {
    CDlgTerrainSize dlg;
    dlg.m_fSize = 1024.0f * 4.0f;
    if (dlg.DoModal() != IDOK) {
        return;
    }

    HCURSOR hLoadCur, hCurrentCur;
    hCurrentCur = GetCursor();
    hLoadCur = AfxGetApp()->LoadCursor(IDC_LOAD);
    SetCursor(hLoadCur);

    if (m_pTerrain) {
        m_pTerrain->Release();
    } else {
        m_pTerrain = new CLyTerrain;
    }

    m_pTerrain->Import(fsVmeshFile, dlg.m_fSize);

    m_pEventMgr->SetActive(true);
    m_pEventMgr->SetActive(false);

    SetCursor(hCurrentCur);

    m_bLoadingComplete = true;
}

void CMapMng::ImportTerrainHeight(const fs::path & fsVmeshFile) {
    HCURSOR hLoadCur, hCurrentCur;
    hCurrentCur = GetCursor();
    hLoadCur = AfxGetApp()->LoadCursor(IDC_LOAD);
    SetCursor(hLoadCur);

    if (!m_pTerrain) {
        return;
    } else {
        m_pTerrain->ImportHeight(fsVmeshFile);
    }

    SetCursor(hCurrentCur);

    m_bLoadingComplete = true;
}

void CMapMng::RenderGrid(float fGridSize, float fMaxDistance) // fGridSize크기로 fMaxDistance거리까지 격자를 그려준다
{
    if (m_pMainFrm == NULL) {
        return;
    }
    CN3Eng * pEng = m_pMainFrm->m_pEng;
    if (pEng == NULL) {
        return;
    }
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    static float            fPrevGridSize = 0;
    static float            fPrevMaxDistance = 0;
    static __VertexXyzColor pVertices[100 * 4];
    static int              iVC = 0;

    if (fGridSize != fPrevGridSize || fMaxDistance != fPrevMaxDistance) {
        fPrevGridSize = fGridSize;
        fPrevMaxDistance = fMaxDistance;
        int            iCount = (int)(fMaxDistance / fGridSize);
        const D3DCOLOR color = 0xff00ff00;
        iVC = 0;
        for (int i = 0; i < iCount; ++i) {
            pVertices[iVC++].Set(i * fGridSize, 0, 0, color);
            pVertices[iVC++].Set(i * fGridSize, 0, fMaxDistance, color);
        }
        for (int i = 0; i < iCount; ++i) {
            pVertices[iVC++].Set(0, 0, i * fGridSize, color);
            pVertices[iVC++].Set(fMaxDistance, 0, i * fGridSize, color);
        }
    }

    // Backup state
    DWORD dwLighting, dwZEnable;
    pD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    pD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);

    // Set Render State
    pD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    pD3DDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    __Matrix44 matWorld;
    matWorld.Identity();
    pD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

    pD3DDev->SetTexture(0, NULL);

    // render
    pD3DDev->SetFVF(FVF_XYZCOLOR);
    pD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, iVC / 2, pVertices, sizeof(__VertexXyzColor));

    // restore
    pD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
    pD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
}

void CMapMng::SaveObjectPostData(const fs::path & fsFile) {
    /*
    //////////////////////////////////
    // OldData
    if (m_pSceneOutput == NULL) {
        return;
    }

    FILE * stream = _wfopen(fsFile.c_str(), L"w");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("파일을 만들수 없습니다.");
        return;
    }

    int iSC = m_pSceneOutput->ShapeCount();
    fprintf(stream, "Shape Count : %d\n", iSC);

    for (int i = 0; i < iSC; ++i) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        ASSERT(pShape);
        if (pShape == NULL) {
            continue;
        }

        fs::path fsShapeFile = fsFile.parent_path() / (pShape->m_szName + ".n3shape");
        pShape->SaveToFile(fsShapeFile); // Shape 정보 binary file로 저장..

        fprintf(stream, "%s\n", pShape->m_szName); // 텍스트에 Shape 파일 이름을 쓴다..
    }
    fclose(stream);
    // OldData
    //////////////////////////////////
    */

    if (m_pSceneOutput == NULL) {
        return;
    }

    FILE * stream = _wfopen(fsFile.c_str(), L"w");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("파일을 만들수 없습니다.");
        return;
    }

    int iSC = m_pSceneOutput->ShapeCount();
    fprintf(stream, "Shape Post Count : %d\n", iSC);

    std::string szSFN;
    for (int i = 0; i < iSC; ++i) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);

        szSFN = pShape->FilePath().filename().string();

        __Vector3    vPos = pShape->Pos();
        __Vector3    vScale = pShape->Scale();
        __Quaternion qtRot = pShape->Rot();
        int          iSPC = pShape->PartCount();
        fprintf(stream,
                "FileName[ %s ] PartCount[ %d ] Position[ %f %f %f] Rotation[ %f %f %f %f ] Scale[ %f %f %f ] Belong [ "
                "%d ] Attribute [ %d %d %d %d ]\n",
                szSFN.c_str(), iSPC, vPos.x, vPos.y, vPos.z, qtRot.x, qtRot.y, qtRot.z, qtRot.w, vScale.x, vScale.y,
                vScale.z, pShape->m_iBelong, pShape->m_iEventID, pShape->m_iEventType, pShape->m_iNPC_ID,
                pShape->m_iNPC_Status);
        for (int j = 0; j < iSPC; j++) {
            CN3SPart * pPart = pShape->Part(j);
            fprintf(stream, "\tPart - DiffuseARGB[ %f %f %f %f ] AmbientARGB[ %f %f %f %f ]\n", pPart->m_Mtl.Diffuse.a,
                    pPart->m_Mtl.Diffuse.r, pPart->m_Mtl.Diffuse.g, pPart->m_Mtl.Diffuse.b, pPart->m_Mtl.Ambient.a,
                    pPart->m_Mtl.Ambient.r, pPart->m_Mtl.Ambient.g, pPart->m_Mtl.Ambient.b);
        }
    }

    fclose(stream);
}

void CMapMng::SaveObjectPostDataPartition(const fs::path & fsFile, float psx, float psz, float width) {
    if (m_pSceneOutput == NULL) {
        return;
    }

    FILE * stream = _wfopen(fsFile.c_str(), L"w");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("파일을 만들수 없습니다.");
        return;
    }

    float sx = (int)(psx / TERRAIN_CELL_SIZE) * TERRAIN_CELL_SIZE;
    float sz = (int)(psz / TERRAIN_CELL_SIZE) * TERRAIN_CELL_SIZE;
    float ex = sx + width;
    float ez = sz + width;

    int iSC = m_pSceneOutput->ShapeCount();

    std::list<int> ShapeList;
    ShapeList.clear();

    CProgressBar ProgressBar;
    ProgressBar.Create("Pick out Objects.", 50, iSC);

    for (int i = 0; i < iSC; ++i) {
        ProgressBar.StepIt();
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        if (!pShape) {
            continue;
        }

        if (pShape->Pos().x > ex || pShape->Pos().x < sx || pShape->Pos().z > ez || pShape->Pos().z < sz) {
            continue;
        }

        ShapeList.emplace_back(i);
    }

    int RealCount = ShapeList.size();
    fprintf(stream, "Shape Post Count : %d\n", RealCount);

    ProgressBar.Create("Pick out Objects.", 50, RealCount);
    std::string szSFN;
    for (auto it = ShapeList.begin(); it != ShapeList.end(); it++) {
        ProgressBar.StepIt();
        int        idx = (*it);
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(idx);

        szSFN = pShape->FilePath().filename().string();

        __Vector3 vPos = pShape->Pos();
        vPos.x -= sx;
        vPos.z -= sz;

        __Vector3    vScale = pShape->Scale();
        __Quaternion qtRot = pShape->Rot();
        int          iSPC = pShape->PartCount();
        fprintf(stream,
                "FileName[ %s ] PartCount[ %d ] Position[ %f %f %f] Rotation[ %f %f %f %f ] Scale[ %f %f %f ] Belong [ "
                "%d ] Attribute [ %d %d %d %d ]\n",
                szSFN.c_str(), iSPC, vPos.x, vPos.y, vPos.z, qtRot.x, qtRot.y, qtRot.z, qtRot.w, vScale.x, vScale.y,
                vScale.z, pShape->m_iBelong, pShape->m_iEventID, pShape->m_iEventType, pShape->m_iNPC_ID,
                pShape->m_iNPC_Status);
        for (int j = 0; j < iSPC; j++) {
            CN3SPart * pPart = pShape->Part(j);
            fprintf(stream, "\tPart - DiffuseARGB[ %f %f %f %f ] AmbientARGB[ %f %f %f %f ]\n", pPart->m_Mtl.Diffuse.a,
                    pPart->m_Mtl.Diffuse.r, pPart->m_Mtl.Diffuse.g, pPart->m_Mtl.Diffuse.b, pPart->m_Mtl.Ambient.a,
                    pPart->m_Mtl.Ambient.r, pPart->m_Mtl.Ambient.g, pPart->m_Mtl.Ambient.b);
        }
    }

    fclose(stream);
}

void CMapMng::LoadObjectPostData(const fs::path & fsFile) {
    if (m_pSceneOutput == NULL) {
        return;
    }

    // Scene 에 있는 오브젝트들 Release...
    m_pSceneOutput->ShapeRelease();
    m_pSceneOutput->ChrRelease();

    FILE * stream = _wfopen(fsFile.c_str(), L"r");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("지정한 텍스트 파일을 찾을 수 없습니다.");
        return;
    }

    char szFirstLine[256];
    fgets(szFirstLine, 256, stream);           // 첫째 줄을 읽고..
    if (strstr(szFirstLine, "Shape Count : ")) // 문자열이 있으면 예전 데이터이다..
    {
        int iSC = 0, result = 0;
        sscanf(szFirstLine, "Shape Count : %d\n", &iSC);
        for (int i = 0; i < iSC; ++i) {
            char szDestName[_MAX_PATH]{};
            result = fscanf(stream, "%s\n", szDestName); // 파일 이름을 읽고..
            if (result == EOF) {
                break;
            }

            CN3Shape * pShape = new CN3Shape;
            fs::path   fsShapeFile = (fsFile.parent_path() / szDestName).replace_extension(".n3shape");
            if (!pShape->LoadFromFile(fsShapeFile)) {
                delete pShape;
                pShape = NULL;
                continue; // Shape 정보 binary file로 읽기..
            }

            szDestName[lstrlen(szDestName) - 5] = '\0'; // Remove the _0000 suffix.
            pShape->m_szName = szDestName;
            pShape->FilePathSet(pShape->m_szName + ".n3shape"); // 다시 파일 이름 설정..

            m_pSceneOutput->ShapeAdd(pShape);
        }
    } else { // 새로 만든 데이터이다..
        int iSC = 0;
        sscanf(szFirstLine, "Shape Post Count : %d\n", &iSC);

        char szSFN[MAX_PATH]{};
        char szLine[1024]{};
        for (int i = 0; i < iSC; ++i) {
            CN3Shape * pShape = new CN3Shape();
            m_pSceneOutput->ShapeAdd(pShape); // 추가..

            int          iSPC = 0, iBelong = 0, iEventID = 0, iEventType = 0, iNPC_ID = 0, iNPC_Status = 0;
            __Vector3    vPos(0, 0, 0);
            __Vector3    vScale(1, 1, 1);
            __Quaternion qtRot;
            qtRot.Identity();

            fgets(szLine, 1024, stream);
            sscanf(szLine,
                   "FileName[ %s ] PartCount[ %d ] Position[ %f %f %f] Rotation[ %f %f %f %f ] Scale[ %f %f %f ] "
                   "Belong [ %d ] Attribute [ %d %d %d %d ]\n",
                   szSFN, &iSPC, &(vPos.x), &(vPos.y), &(vPos.z), &(qtRot.x), &(qtRot.y), &(qtRot.z), &(qtRot.w),
                   &(vScale.x), &(vScale.y), &(vScale.z), &(iBelong), &(iEventID), &(iEventType), &(iNPC_ID),
                   &(iNPC_Status));
            pShape->LoadFromFile("Object" / fs::path(szSFN)); // 파일에서 읽고..
            for (int j = 0; j < iSPC; j++) {
                fgets(szLine, 1024, stream);

                CN3SPart * pPart = pShape->Part(j);
                if (pPart) {
                    sscanf(szLine, "\tPart - DiffuseARGB[ %f %f %f %f ] AmbientARGB[ %f %f %f %f ]\n",
                           &(pPart->m_Mtl.Diffuse.a), &(pPart->m_Mtl.Diffuse.r), &(pPart->m_Mtl.Diffuse.g),
                           &(pPart->m_Mtl.Diffuse.b), &(pPart->m_Mtl.Ambient.a), &(pPart->m_Mtl.Ambient.r),
                           &(pPart->m_Mtl.Ambient.g), &(pPart->m_Mtl.Ambient.b));
                }
            }

            pShape->PosSet(vPos);
            pShape->RotSet(qtRot);
            pShape->ScaleSet(vScale);
            pShape->ReCalcMatrix(); // 행렬 다시 계산..

            pShape->m_iBelong = iBelong;
            pShape->m_iEventID = iEventID;
            pShape->m_iEventType = iEventType;
            pShape->m_iNPC_ID = iNPC_ID;
            pShape->m_iNPC_Status = iNPC_Status;
        }
    }

    fclose(stream);
}

CStatusBar * CMapMng::GetStatusBar() {
    CFrameWnd * pFrame = (CFrameWnd *)AfxGetMainWnd();
    if (pFrame == NULL || pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) == FALSE) {
        return NULL;
    }

    CStatusBar * pBar = (CStatusBar *)pFrame->GetMessageBar();
    if (pBar == NULL || pBar->IsKindOf(RUNTIME_CLASS(CStatusBar)) == FALSE) {
        return NULL;
    }

    return pBar;
}

void CMapMng::UpdateAll() {
    if (m_pDlgOutputList) {
        m_pDlgOutputList->UpdateTree(m_pSceneOutput);
    }
    if (m_pDlgSourceList) {
        m_pDlgSourceList->UpdateTree(m_pSceneSource);
    }
}

void CMapMng::ImportPostDataFromScene(const fs::path & fsFile) {
    HANDLE hFile = CreateFileW(fsFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        return;
    }

    DWORD dwRWC = 0;

    int   nCameraActive;
    float fFrmCur, fFrmStart, fFrmEnd;
    ReadFile(hFile, &nCameraActive, 4, &dwRWC, NULL);
    ReadFile(hFile, &fFrmCur, 4, &dwRWC, NULL);   // Animation Frame;
    ReadFile(hFile, &fFrmStart, 4, &dwRWC, NULL); // 전체 프레임.
    ReadFile(hFile, &fFrmEnd, 4, &dwRWC, NULL);   // 전체 프레임.

    int         iLen = 0;
    std::string szFile;

    int nCC = 0;
    ReadFile(hFile, &nCC, 4, &dwRWC, NULL); // 카메라..
    for (int i = 0; i < nCC; i++) {
        iLen = 0;
        ReadFile(hFile, &iLen, 4, &dwRWC, NULL);
        if (iLen > 0) {
            szFile.assign(iLen, '\0');
            ReadFile(hFile, szFile.data(), iLen, &dwRWC, NULL);
        }
    }

    int nLC = 0;
    ReadFile(hFile, &nLC, 4, &dwRWC, NULL); // 카메라..
    for (int i = 0; i < nLC; i++) {
        iLen = 0;
        ReadFile(hFile, &iLen, 4, &dwRWC, NULL);
        if (iLen > 0) {
            szFile.assign(iLen, '\0');
            ReadFile(hFile, szFile.data(), iLen, &dwRWC, NULL);
        }
    }

    int nSC = 0;
    ReadFile(hFile, &nSC, 4, &dwRWC, NULL); // Shapes..
    for (int i = 0; i < nSC; i++) {
        iLen = 0;
        ReadFile(hFile, &iLen, 4, &dwRWC, NULL);
        if (iLen > 0) {
            szFile.assign(iLen, '\0');
            ReadFile(hFile, szFile.data(), iLen, &dwRWC, NULL);
            AddShape(m_pSceneOutput, szFile, TRUE);
        }
    }

    int nChrC = 0;
    ReadFile(hFile, &nChrC, 4, &dwRWC, NULL); // 캐릭터
    for (int i = 0; i < nChrC; i++) {
        iLen = 0;
        ReadFile(hFile, &iLen, 4, &dwRWC, NULL);
        if (iLen > 0) {
            szFile.assign(iLen, '\0');
            ReadFile(hFile, szFile.data(), iLen, &dwRWC, NULL);
        }
    }

    m_pDlgOutputList->UpdateTree(m_pSceneOutput); // 트리 업데이트...

    CloseHandle(hFile);
}

void CMapMng::ImportShape(const fs::path & fsFile) {
    // Import...
    this->AddShape(m_pSceneOutput, fsFile, FALSE);
}

void CMapMng::DeleteUnusedFiles() {
    std::map<fs::path, CN3BaseFileAccess *> mBases;
    std::vector<fs::path>                   vUnusedFiles;
    std::vector<std::string>                vErroredFiles;

    int iSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < iSC; i++) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        if (NULL == pShape) {
            continue;
        }

        mBases.emplace(pShape->FilePathAbs(), pShape);

        CN3VMesh * pVMesh = pShape->CollisionMesh();
        if (pVMesh) {
            mBases.emplace(pVMesh->FilePathAbs(), pVMesh);
        } else {
            vErroredFiles.emplace_back(std::format("NULL VMesh : {:s}", pShape->FilePath()));
        }

        int iSPC = pShape->PartCount();
        for (int j = 0; j < iSPC; j++) {
            CN3SPart * pPart = pShape->Part(j);
            if (NULL == pPart) {
                vErroredFiles.emplace_back(std::format("NULL Part : {:s} - {:d}th Part", pShape->FilePath(), j));
                continue;
            }

            CN3PMesh * pPMesh = pPart->Mesh();
            if (pPMesh) {
                mBases.emplace(pPMesh->FilePathAbs(), pPMesh);
            } else {
                vErroredFiles.emplace_back(std::format("NULL Part : {:s} - {:d}th Part", pShape->FilePath(), j));
            }

            int iSTC = pPart->TexCount();
            for (int k = 0; k < iSTC; k++) {
                CN3Texture * pTex = pPart->Tex(k);
                if (pTex) {
                    mBases.emplace(pTex->FilePathAbs(), pTex);
                } else {
                    vErroredFiles.emplace_back(
                        std::format("NULL Texture : {:s} - {:d}th Part, {:d}th Texture", pShape->FilePath(), j, k));
                    continue;
                }
            }
        }
    }

    fs::path fsObjectDir = CN3Base::PathGet() / "Object";
    if (fs::is_directory(fsObjectDir)) {
        for (const auto & fi : fs::directory_iterator(fsObjectDir)) {
            if (!fi.is_regular_file()) {
                continue;
            }

            fs::path fsFindFile = fi.path();
            fsFindFile.make_lower();
            if (mBases.contains(fsFindFile)) {
                vUnusedFiles.emplace_back(fsFindFile);
            }
        }
    }

    CDlgUnusedFiles dlg;
    dlg.m_vFiles = vUnusedFiles;
    dlg.m_vErroredFiles = vErroredFiles;
    dlg.DoModal();

    m_pSelSourceObj = NULL; // 이렇게 해주어야 뻑이 안난다.
    m_SelOutputObjArray.RemoveAll();
    this->LoadSourceObjects();
    this->UpdateAll();
}

void CMapMng::DeleteOverlappedObjects() // 위치가 겹친 젝트를 찾는다.
{
    std::vector<CN3Shape *> OverlappedObjects;
    int                     iSC = m_pSceneOutput->ShapeCount();

    CProgressBar ProgressBar;
    ProgressBar.Create("Delete overlapped objects..", 50, iSC);
    ProgressBar.SetPos(1);
    int iPosProgress = 0;

    CN3Shape *pObj1, *pObj2;
    for (int i = 0; i < iSC; i++) {
        pObj1 = m_pSceneOutput->ShapeGet(i);
        for (int j = i + 1; j < iSC; j++) {
            pObj2 = m_pSceneOutput->ShapeGet(j);
            if (pObj1->Pos() == pObj2->Pos() || (pObj1->Pos() - pObj2->Pos()).Magnitude() < 0.3f) {
                OverlappedObjects.emplace_back(pObj1);
                break;
            }
        }

        iPosProgress++;
        ProgressBar.SetPos(iPosProgress);
    }

    iSC = OverlappedObjects.size();
    for (int i = 0; i < iSC; i++) {
        m_pSceneOutput->ShapeDelete(OverlappedObjects[i]); // 겹친거 지우기..
    }

    // 업데이트...
    m_pSelSourceObj = NULL; // 이렇게 해주어야 뻑이 안난다.
    m_SelOutputObjArray.RemoveAll();
    if (m_pDlgOutputList) {
        m_pDlgOutputList->UpdateTree(m_pSceneOutput); // 몽땅 업데이트...
    }
}

void CMapMng::DeleteSelectedSourceObjects() {
    if (NULL == m_pSelSourceObj || !(m_pSelSourceObj->Type() & OBJ_SHAPE)) {
        return;
    }

    std::vector<CN3Shape *> SameObjects;
    int                     iSC = m_pSceneOutput->ShapeCount();

    CN3Shape * pObj;
    for (int i = 0; i < iSC; i++) {
        pObj = m_pSceneOutput->ShapeGet(i);
        if (pObj->FilePath() == m_pSelSourceObj->FilePath()) {
            SameObjects.emplace_back(pObj);
        }
    }

    iSC = SameObjects.size();
    for (int i = 0; i < iSC; i++) {
        m_pSceneOutput->ShapeDelete(SameObjects[i]); // 겹친거 지우기..
    }

    // 업데이트...
    m_pSceneSource->ShapeDelete((CN3Shape *)m_pSelSourceObj);
    m_pSelSourceObj = NULL; // 이렇게 해주어야 뻑이 안난다.
    m_SelOutputObjArray.RemoveAll();
    this->UpdateAll(); // 몽땅 업데이트...
}

CN3Camera * CMapMng::CameraGet() {
    if (m_pSceneOutput) {
        return m_pSceneOutput->CameraGetActive();
    }
    return NULL;
}

void CMapMng::SetLight(float fLgt) {
    if (m_pSceneOutput) {
        for (int i = 0; i < NUM_UNIT_LIGHT; i++) {
            CN3Light * pLight = m_pSceneOutput->LightGet(i);
            if (pLight) {
                D3DCOLORVALUE crLgt;
                crLgt.a = 0.0f, crLgt.r = crLgt.g = crLgt.b = fLgt;

                if (i == 0) {
                    pLight->m_Data.InitDirection(0, __Vector3(0, -1, 0), crLgt);
                } else {
                    pLight->m_Data.InitDirection(1, __Vector3(0, 1, 0), crLgt);
                }
            }
        }
        Invalidate();
    }
}

void CMapMng::ActWarpMgr() {
    m_pWarpMgr->SetActive(true);
}

bool CMapMng::IsWarpMgrActive() {
    return m_pWarpMgr->IsActive();
}

void CMapMng::SetEditState(ENUM_EDIT_STATE eEditStat) {
    static __Vector3 vOldPos;
    static DWORD     dwChkTime = 0;
    DWORD            dwNowTick = GetTickCount();
    if (dwNowTick - dwChkTime < 1000) {
        return;
    }
    dwChkTime = dwNowTick;

    switch (eEditStat) {
    default:
    case eEDIT_NOT: {
        //            m_SelOutputObjBack.RemoveAll();
        m_eSelObjBackState = eEDIT_NOT;
    } break;

    case eEDIT_COPY: {
        //    기존백업한거 지우고
        m_SelOutputObjBack.RemoveAll();
        vOldPos.Zero();

        //    선택한 것들을 백업하고
        CN3Transform * pDestObj = NULL;
        int            iSize = m_SelOutputObjArray.GetSize();
        for (int j = 0; j < iSize; ++j) {
            pDestObj = m_SelOutputObjArray.GetAt(j);
            if (pDestObj == NULL) {
                continue;
            }

            m_SelOutputObjBack.InsertAt(0, pDestObj);

            if (vOldPos.Magnitude() == 0.0f) {
                vOldPos = pDestObj->Pos();
            }
        }
        m_eSelObjBackState = eEDIT_COPY;
    } break;

    case eEDIT_PASTE: {
        if (m_eSelObjBackState == eEDIT_COPY) {
            CN3Transform *pDestObj = NULL, *pNewObj = NULL;
            __Vector3     vNewPos, vObjPos;
            int           iSize = m_SelOutputObjBack.GetSize();
            if (iSize == 0) {
                return;
            }

            //    찍을 새로운위치를 입력
            ASSERT(m_pTerrain);
            CPoint point = ((CN3MEView *)m_pMainFrm->GetActiveView())->m_CurrMousePos;
            m_pTerrain->Pick(point.x, point.y, &vNewPos);

            if (vOldPos.Magnitude() == 0.0f || vNewPos.Magnitude() == 0.0f) {
                return;
            }

            vNewPos -= vOldPos; //    새로 이사갈 백터를 구함

            if (m_pDummy) //    더미의 새로운 좌표 입력
            {
                m_pDummy->PosSet(m_pDummy->Pos() + vNewPos);
            }

            m_SelOutputObjArray.RemoveAll(); //    기존 선택된 정보를 지우고
            for (int j = 0; j < iSize; ++j) {
                pDestObj = m_SelOutputObjBack.GetAt(j); //    백업된 데이터를 찾는다
                if (pDestObj == NULL) {
                    continue;
                }

                pNewObj = AddObjectToOutputScene(pDestObj); //    주소의 정보로만 새로운 데이터를 만든다
                if (pNewObj == NULL) {
                    continue;
                }

                m_SelOutputObjArray.InsertAt(0, pNewObj); //    기존 정보에 새로 입력한다

                pNewObj->ScaleSet(pDestObj->Scale()); //    크기 입력
                pNewObj->RotSet(pDestObj->Rot());     //    회전각 입력

                vObjPos = pDestObj->Pos() + vNewPos;
                vObjPos.y = m_pTerrain->GetHeight(vObjPos.x, vObjPos.z);
                pNewObj->PosSet(vObjPos); //    새로운 위치점 입력
            }
            OnSelChanged(); //    데이터입력등 새로운데이터로 갱신
        }
    } break;
    }
}
