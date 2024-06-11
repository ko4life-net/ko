// MapMng.cpp: implementation of the CMapMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
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

    // Specify Path
    m_pSceneSource = new CN3Scene;
    m_pSceneSource->m_szName = "SourceList";
    m_pSceneSource->FileNameSet("SourceList.N3Scene");

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

    // window layout

    /* orginal window size
    CRect rc(0, 0, 980, 1000);
    pMainFrm->MoveWindow(&rc);
    m_pDlgSourceList->MoveWindow(rc.right, rc.top, 300, 500);
    m_pDlgOutputList->MoveWindow(rc.right, 500, 300, 500);
    */

    RECT primaryMonitor;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryMonitor, 0)) {
        int screenWidth = primaryMonitor.right - primaryMonitor.left;
        int screenHeight = primaryMonitor.bottom - primaryMonitor.top;

        CRect rcMain(0, 0, screenWidth - 300, screenHeight);
        pMainFrm->MoveWindow(&rcMain);

        CRect rcSourceList(rcMain.right, rcMain.top, rcMain.right + 300, rcMain.top + screenHeight / 2);
        m_pDlgSourceList->MoveWindow(&rcSourceList);

        CRect rcOutputList(rcMain.right, rcMain.top + screenHeight / 2, rcMain.right + 300, rcMain.bottom);
        m_pDlgOutputList->MoveWindow(&rcOutputList);
    }

    Release();
}

// destroyer
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

// reset
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

        /*
        m_pSceneOutput->CameraGetActive()->m_Data.fNP = 1.0f; //gutek
        m_pSceneOutput->CameraGetActive()->m_Data.fFP = m_pMainFrm->GetFP(); //gutek
        */
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

//Update FarPlane...
void CMapMng::UpDateFP() {
    m_pSceneOutput->CameraGetActive()->m_Data.fFP = m_pMainFrm->GetFP();
    Invalidate();
}

// m_pSceneSource Function to add characters and objects
void CMapMng::LoadSourceObjects() {
    ASSERT(m_pSceneSource && m_pDlgSourceList);

    m_pSceneSource->Release();

    WIN32_FIND_DATA FindFileData;

    // Add all characters in source\Chr folder
    CString szChrPath;
    szChrPath.Format("%sChr\\", CN3Base::s_szPath.c_str());
    SetCurrentDirectory(szChrPath); // Change the path to the szFolder\Chr folder...
    HANDLE hFind = FindFirstFile("*.N3Chr", &FindFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        AddChr(m_pSceneSource, std::string(szChrPath + FindFileData.cFileName), FALSE);
        while (FindNextFile(hFind, &FindFileData)) {
            AddChr(m_pSceneSource, std::string(szChrPath + FindFileData.cFileName), FALSE);
        }
        FindClose(hFind);
    }

    // Add all shapes in source\Data folder
    CString szShapePath;
    szShapePath.Format("%sObject\\", CN3Base::s_szPath.c_str());
    SetCurrentDirectory(szShapePath);                  // Change the path to the szFolder\Mesh folder...
    hFind = FindFirstFile("*.N3Shape", &FindFileData); // Find file.

    if (hFind != INVALID_HANDLE_VALUE) {
        AddShape(m_pSceneSource, std::string(szShapePath + FindFileData.cFileName), FALSE);
        while (FindNextFile(hFind, &FindFileData)) {
            AddShape(m_pSceneSource, std::string(szShapePath + FindFileData.cFileName), FALSE);
        }
        FindClose(hFind);
    }

    m_pSceneSource->Tick();                       // Object initialization
    m_pDlgSourceList->UpdateTree(m_pSceneSource); // update list
}

// Function to add a character to the specified scene
CN3Transform * CMapMng::AddChr(CN3Scene * pDestScene, const std::string & szFN, BOOL bGenerateChainNumber) {
    CN3Chr * pChr = new CN3Chr;
    if (false == pChr->LoadFromFile(szFN)) // If the call fails...
    {
        delete pChr;
        return NULL;
    }

    if (bGenerateChainNumber) {
        int  nCC = pDestScene->ChrCount();
        int  nChainNumber = 0;
        char szCompare[_MAX_PATH];
        for (int i = 0; i < nCC; i++) {
            lstrcpy(szCompare, pDestScene->ChrGet(i)->m_szName.c_str());
            int nL = lstrlen(szCompare);
            if (nL < 5) {
                continue;
            }

            szCompare[nL - 5] = NULL; // Remove the trailing underscore and four-digit number.
            if (pChr->m_szName == szCompare) {
                nChainNumber = atoi(&(szCompare[nL - 4])) + 1;
            }
        }

        char szName[_MAX_PATH];
        wsprintf(szName, "%s_%.4d", pChr->m_szName.c_str(), nChainNumber);
        pChr->m_szName = szName;
    }

    pDestScene->ChrAdd(pChr);
    return pChr;
}

// Function to add Shape to the specified Scene
CN3Transform * CMapMng::AddShape(CN3Scene * pDestScene, const std::string & szFN, BOOL bGenerateChainNumber) {
    CN3Shape * pShape = new CN3Shape;
    if (false == pShape->LoadFromFile(szFN)) {
        delete pShape;
        return NULL;
    }

    /*    if(bGenerateChainNumber)
    {
        int nSC = pDestScene->ShapeCount();
        int nChainNumber = 0;
        char szCompare[_MAX_PATH];
        for(int i = 0; i < nSC; i++)
        {
            lstrcpy(szCompare, pDestScene->ShapeGet(i)->Name());
            int nL = lstrlen(szCompare);
            if(nL < 5) continue;

            szCompare[nL-5] = NULL; // Remove the trailing underscore and four-digit number.
            if(0 == lstrcmpi(pShape->Name(), szCompare)) 
            {
                nChainNumber = atoi(&(szCompare[nL-4])) + 1;
            }
        }

        char szName[_MAX_PATH];
        wsprintf(szName, "%s_%.4d", pShape->Name(), nChainNumber);
        pShape->m_szName = szName; // .. 이름을 짓는다..
        
        char szFileName2[_MAX_PATH];
        char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
        _splitpath(szFileName, szDrive, szDir, szFName, szExt);
        _makepath(szFileName2, szDrive, szDir, szName, szExt);
        pShape->FileNameSet(szFileName2); // Name the file...
    }
*/
    pDestScene->ShapeAdd(pShape);
    return pShape;
}

// Add pObj to m_pSceneOutput.
CN3Transform * CMapMng::AddObjectToOutputScene(CN3Transform * pObj) {
    if (pObj == NULL) {
        return NULL;
    }
    ASSERT(m_pSceneOutput && pObj->m_szName.size());

    // Insert into m_pSceneOutput
    CN3Transform * pDestObj = NULL;
    if (pObj->Type() & OBJ_CHARACTER) {
        pDestObj = AddChr(m_pSceneOutput, pObj->FileName(), TRUE);
    } else if (pObj->Type() & OBJ_SHAPE) {
        pDestObj = AddShape(m_pSceneOutput, pObj->FileName(), TRUE);
    }

    if (pDestObj) {
        pDestObj->PosSet(pObj->Pos());
        m_pDlgOutputList->UpdateTree(m_pSceneOutput);
        return pDestObj;
    }
    return NULL;
}

// Delete selected object
void CMapMng::DeleteSelObjectFromOutputScene() {
    if (GetCursorMode() == CM_EDIT_RIVER) // In case of river editing, clear the selection point
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
    CFileDialog dlg(FALSE, "n3m", NULL, dwFlags, "N3ME file(*.n3m)|*.n3m||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    CString lpszPathName = dlg.GetPathName();

    // file name
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath((LPCTSTR)lpszPathName, szDrive, szDir, szFName, szExt);

    //Create n3m..^^
    char szN3M[_MAX_PATH];
    _makepath(szN3M, szDrive, szDir, szFName, "n3m");
    HANDLE hFile = CreateFile(szN3M, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(::GetActiveWindow(), lpszPathName, "Fail to open map data file for save, Pleas retry.", MB_OK);
        return;
    }
    char  comment[80] = {"The person who opens this file is an idiot...^^"};
    DWORD dwRWC;
    WriteFile(hFile, &comment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hFile);

    // terrain
    if (m_pTerrain) {
        char szTerrain[_MAX_PATH];
        _makepath(szTerrain, szDrive, szDir, szFName, "trn");
        m_pTerrain->SaveToFilePartition(szTerrain, x, z, width);
    }

    // Save sdt file (shape data text)
    char szSceneText[_MAX_PATH];
    _makepath(szSceneText, szDrive, szDir, szFName, "sdt");
    SaveObjectPostDataPartition(szSceneText, x, z, width);

    /*
    // load warp information..
    char szWarp[_MAX_PATH];
    _makepath(szWarp, szDrive, szDir, szFName, "wap");
    m_pWarpMgr->SaveToFile(szWarp);

    //Save event information..
    //char szEvent[_MAX_PATH];
    //_makepath(szEvent, szDrive, szDir, szFName, "evt");
    //m_pEventMgr->SaveToFile(szEvent);    
    //*/
}

void CMapMng::SaveToFile(LPCTSTR lpszPathName) {
    if (lstrlen(lpszPathName) == 0) {
        return;
    }

    // file name
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

    //Create n3m..^^
    char szN3M[_MAX_PATH];
    _makepath(szN3M, szDrive, szDir, szFName, "n3m");
    HANDLE hFile = CreateFile(szN3M, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(::GetActiveWindow(), lpszPathName, "Fail to open map data file for save, Pleas retry.", MB_OK);
        return;
    }

    char  comment[80] = {"The person who opens this file is an idiot...^^"};
    DWORD dwRWC;
    WriteFile(hFile, &comment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hFile);

    // terrain
    if (m_pTerrain) {
        char szTerrain[_MAX_PATH];
        _makepath(szTerrain, szDrive, szDir, szFName, "trn");
        m_pTerrain->SaveToFile(szTerrain);
    }

    // Save sdt file (shape data text)
    char szSceneText[_MAX_PATH];
    _makepath(szSceneText, szDrive, szDir, szFName, "sdt");
    SaveObjectPostData(szSceneText);

    // Save river water editing information..
    char szRiver[_MAX_PATH];
    _makepath(szRiver, szDrive, szDir, szFName, "rvr");
    m_RiverMng.SaveToFile(szRiver);

    // Save pond editing information..
    char szPond[_MAX_PATH];
    _makepath(szPond, szDrive, szDir, szFName, "pvr");
    m_PondMng.SaveToFile(szPond);

    //Save wall information..
    char szWall[_MAX_PATH];
    _makepath(szWall, szDrive, szDir, szFName, "wal");
    m_pWall->SaveToFile(szWall);

    // load warp information..
    char szWarp[_MAX_PATH];
    _makepath(szWarp, szDrive, szDir, szFName, "wap");
    m_pWarpMgr->SaveToFile(szWarp);

    //Load sound information..
    char szSound[_MAX_PATH];
    _makepath(szSound, szDrive, szDir, szFName, "tsd");
    m_pSoundMgr->SaveToFile(szSound);

    // light object information..
    char szLightObj[_MAX_PATH];
    _makepath(szLightObj, szDrive, szDir, szFName, "tld");
    m_pLightObjMgr->SaveToFile(szLightObj);

    //Save event information..
    //char szEvent[_MAX_PATH];
    //_makepath(szEvent, szDrive, szDir, szFName, "evt");
    //m_pEventMgr->SaveToFile(szEvent);
}

void CMapMng::LoadFromFile(LPCTSTR lpszPathName) {
    if (lstrlen(lpszPathName) == 0) {
        return;
    }

    // file name
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_PATH], szExt[_MAX_EXT];
    _splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

    // terrain
    delete m_pTerrain;
    m_pTerrain = new CLyTerrain;
    m_pTerrain->Init();
    char szTerrain[_MAX_PATH];
    _makepath(szTerrain, szDrive, szDir, szFName, "trn");
    m_pTerrain->LoadFromFile(szTerrain);

    // Scene
    ASSERT(m_pSceneOutput);
    char szSceneText[_MAX_PATH];
    _makepath(szSceneText, szDrive, szDir, szFName, "sdt");
    LoadObjectPostData(szSceneText);

    // river
    char szRiver[_MAX_PATH];
    _makepath(szRiver, szDrive, szDir, szFName, "rvr");
    m_RiverMng.LoadFromFile(szRiver);

    // Pond
    char szPond[_MAX_PATH];
    _makepath(szPond, szDrive, szDir, szFName, "pvr");
    m_PondMng.LoadFromFile(szPond);

    //load wall information..
    char szWall[_MAX_PATH];
    _makepath(szWall, szDrive, szDir, szFName, "wal");
    m_pWall->LoadFromFile(szWall);

    // load warp information..
    char szWarp[_MAX_PATH];
    _makepath(szWarp, szDrive, szDir, szFName, "wap");
    m_pWarpMgr->LoadFromFile(szWarp);

    //Load sound information..
    char szSound[_MAX_PATH];
    _makepath(szSound, szDrive, szDir, szFName, "tsd");
    m_pSoundMgr->LoadFromFile(szSound);

    // light object information..
    char szLightObj[_MAX_PATH];
    _makepath(szLightObj, szDrive, szDir, szFName, "tld");
    m_pLightObjMgr->LoadFromFile(szLightObj);

    //Event information..
    //char szEvent[_MAX_PATH];
    //_makepath(szEvent, szDrive, szDir, szFName, "evt");
    //m_pEventMgr->LoadFromFile(szEvent);
    //m_pEventMgr->SetActive(true);
    //m_pEventMgr->SetActive(false);

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
        return; // Loading has not finished yet...
    }

    CN3EngTool * pEng = m_pMainFrm->m_pEng;
    if (pEng == NULL) {
        return;
    }

    pEng->s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, m_FillMode);
    pEng->s_lpD3DDev->SetRenderState(D3DRS_SHADEMODE, m_ShadeMode);

    if (m_bRenderAxisAndGrid) // Draw axes and grid...
    {
        pEng->RenderAxis();
        __Matrix44 mtxWorld;
        mtxWorld.Scale(32.0f, 32.0f, 32.0f);
        if (m_SelOutputObjArray.GetSize() > 0) {
            CN3Transform * pSelObj = m_SelOutputObjArray.GetAt(0);
            if (pSelObj) // If there is a selected object...
            {
                mtxWorld.PosSet(0, pSelObj->Pos().y, 0); // Raise the height.
            }
        }
        pEng->RenderGrid(mtxWorld); // Draw grid...
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
            ((CN3Shape *)pSelObj)->RenderCollisionMesh(); // Draw collision mesh...
            ((CN3Shape *)pSelObj)->RenderClimbMesh();     // Draw a rising mesh...
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

    // grass planting test
    if (m_SowSeedMng.bActive == TRUE) {
        m_SowSeedMng.Render(s_lpD3DDev);
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

    if (vMin.x != FLT_MAX && vMax.x != -FLT_MAX) { // Adjust camera distance according to object size
        __Vector3 vDir = pCamera->Dir();
        __Vector3 vAt = vMin + ((vMax - vMin) / 2);
        pCamera->AtPosSet(vAt);
        pCamera->EyePosSet(vAt - vDir * (vMax - vMin).Magnitude());
    }
    //    else {ASSERT(0);}
    //    {    // keep distance for now since object size is unknown
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
    pCamera->EyePosSet(vAt - vDir * ((float)max(size.cx, size.cy)) / tanf(pCamera->m_Data.fFOV / 2.0f) / 2.0f);

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
        case '2':    // Same content as '1' (The reason it wasn't made into a function is because of the static variable.)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0); 
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '3':    // Same content as '1' (The reason it wasn't made into a function is because of the static variable.)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '4':    // Same content as '1' (The reason it wasn't made into a function is because of the static variable.)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        case '5':    // Same content as '1' (The reason it wasn't made into a function is because of the static variable.)
            {    static BOOL bSet = FALSE;
                if (pCamera) { static __Vector3 vEye(0, 0, 0);    static __Vector3 vAt(0, 0, 1); static __Vector3 vUp(0, 1, 0);
                    if (GetAsyncKeyState(VK_CONTROL) & 0xff00) { vEye = pCamera->EyePos();    vAt = pCamera->AtPos();    vUp = pCamera->UpVector(); pCamera->Apply(); bSet = TRUE; return FALSE; }
                    else if (bSet) { pCamera->EyePosSet(vEye); pCamera->AtPosSet(vAt);    pCamera->UpVectorSet(vUp); pCamera->Apply(); return TRUE;}
                    else return FALSE;}
            } break;
        }
    }
    else if (iButtonDownCount == 0 && !(GetAsyncKeyState(VK_MENU) & 0xff00) ) return FALSE;    // If the alt key is not pressed, there is no camera movement.

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
    default:    // If it is not a mouse message, it is not a camera movement.
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
    if (bSelectDrag == FALSE &&                    // Not in Object drag select mode
        RCM_SELECT != m_RiverMng.GetRCursorMode()) // When not in drag select mode of strong editing
    {
        CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
        if (pCamera && pCamera->MoveByWindowMessage(pMsg)) {
            return TRUE; // Filter camera movement messages
        }
    }
    if (bSelectDrag == FALSE && PCM_SELECT != m_PondMng.GetPCursorMode()) {
        CN3Camera * pCamera = m_pSceneOutput->CameraGetActive();
        if (pCamera && pCamera->MoveByWindowMessage(pMsg)) {
            return TRUE;
        }
    }

    if (m_pTerrain && m_pTerrain->MouseMsgFilter(pMsg)) {
        return TRUE; // Filter terrain edit messages
    }
    if (m_RiverMng.MouseMsgFilter(pMsg)) {
        return TRUE; // Filter strong edit messages
    }
    if (m_PondMng.MouseMsgFilter(pMsg)) {
        return TRUE; // Filter pond edit messages
    }
    if (m_pNPCPath && m_pNPCPath->MouseMsgFilter(pMsg)) {
        return TRUE; // Filter NPC path creation messages
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

    // Filter Dummy Cube moving messages
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

    // Select and place remaining objects
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
    case WM_LBUTTONDOWN: // Select object
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
    case WM_LBUTTONUP: // Select object
    {
        if (bSelectDrag) {
            POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
            if (abs(m_rcSelDrag.left - point.x) < 4 && abs(m_rcSelDrag.top - point.y) < 4) // Considered clicked
            {
                int       nPart = -1;
                CN3Base * pBaseSel = Pick(point, &nPart);

                SelectObject(pBaseSel, FALSE, (GetAsyncKeyState(VK_SHIFT) & 0xff00) ? TRUE : FALSE);
                if (m_pDlgBase && pBaseSel && pBaseSel->Type() & OBJ_SHAPE) // Part selection..
                {
                    m_pDlgBase->m_CBShapePart.SetCurSel(nPart);
                }
            } else { // dragged
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

            m_pMainFrm->UpdateTransformInfo(); // Update position, rotation value, etc.

            return TRUE;
        }
    } break;
    case WM_RBUTTONUP: // deselect object
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
    if (IsSourceObj) { // select source
        if (pObj && pObj->Type() & (OBJ_CHARACTER | OBJ_SHAPE)) {
            m_pSelSourceObj = (CN3Transform *)pObj;
        } else {
            m_pSelSourceObj = NULL;
        }
    } else { // Select already placed objects
        CN3Transform * pSelObj = NULL;
        if (pObj && pObj->Type() & (OBJ_CHARACTER | OBJ_SHAPE)) {
            pSelObj = (CN3Transform *)pObj;
            if (bAdd) // addition
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
                    m_SelOutputObjArray.RemoveAt(i); // already exists, so remove from selection list
                } else {
                    m_SelOutputObjArray.InsertAt(0, pSelObj); // addition
                }
            } else // new selection
            {
                m_SelOutputObjArray.RemoveAll();
                m_SelOutputObjArray.Add(pSelObj);
            }
        } else // Selected incorrectly or did not select character or shape.
        {
            if (bAdd == FALSE) {
                m_SelOutputObjArray.RemoveAll();
            }
        }
        OnSelChanged();
    }

    if (m_SelOutputObjArray.GetSize() == 1) // If you select one...
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
                    m_SelOutputObjArray.RemoveAt(j); // already exists, so remove from selection list
                } else {
                    m_SelOutputObjArray.InsertAt(0, pObj); // addition
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

void CMapMng::MakeGameFiles(LPCTSTR lpszPathName, float fSize) {
    if (NULL == m_pTerrain || NULL == m_pSceneOutput) {
        return;
    }
    if (lstrlen(lpszPathName) == 0) {
        return;
    }

    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

    // Save file.
    HANDLE hFile = CreateFile(lpszPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(::GetActiveWindow(), lpszPathName, "Fail to open map game data file for save, Pleas retry.", MB_OK);
        return;
    }
    char  comment[80] = {"The person who opens this file is an idiot...^^"};
    DWORD dwRWC;
    WriteFile(hFile, &m_iZoneID, sizeof(int), &dwRWC, NULL);
    WriteFile(hFile, &comment, sizeof(char) * 80, &dwRWC, NULL);
    CloseHandle(hFile);

    // Save terrain information
    HANDLE hTerrainGameFile = NULL;
    char   szTerrain[_MAX_PATH] = "";
    _makepath(szTerrain, szDrive, szDir, szFName, ".gtd");
    m_pTerrain->FileNameSet(szTerrain);
    hTerrainGameFile = CreateFile(szTerrain, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hTerrainGameFile) {
        MessageBox(::GetActiveWindow(), szTerrain, "Failed to open terrain file for save", MB_OK);
    } else {
        m_pTerrain->m_szName = szFName; // Specify a name. It is saved according to this name.
        m_pTerrain->SaveGameData(hTerrainGameFile);
        char szColorMapName[_MAX_PATH];
        _makepath(szColorMapName, szDrive, szDir, szFName, ".tct");
        m_pTerrain->MakeGameColorMap(szColorMapName);

        m_RiverMng.MakeGameFiles(hTerrainGameFile, fSize);
        m_PondMng.MakeGameFiles(hTerrainGameFile, fSize);
        //m_pSoundMgr->SaveGameData(hTerrainGameFile);
        CloseHandle(hTerrainGameFile);

        char szLightMapName[_MAX_PATH];
        _makepath(szLightMapName, szDrive, szDir, szFName, ".tlt");
        m_pTerrain->MakeGameLightMap(szLightMapName);
    }

    // Create and save Shape Manager...
    //    if(true == m_pSceneOutput->CheckOverlappedShapesAndReport()) //Check for name or location duplicates...
    //    {
    //        int idYesNo = ::MessageBox(::GetActiveWindow(), "Do you want to delete duplicate objects?", "Handling duplicate objects", MB_YESNO);
    //        if(IDYES == idYesNo)
    //        {
    //            m_pSceneOutput->DeleteOverlappedShapes();
    //            m_pDlgOutputList->UpdateTree(m_pSceneOutput); // Check output list...
    //            m_SelOutputObjArray.RemoveAll(); // Initialize selection...
    //        }
    //    }

    CN3ShapeMgr ShapeMgr;
    ShapeMgr.Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                    (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
    int nSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < nSC; i++) {
        ShapeMgr.Add(m_pSceneOutput->ShapeGet(i)); // Add Shape.
    }

    if (m_pWall) {
        m_pWall->AddWall2Coll(&ShapeMgr);
    }
    ShapeMgr.GenerateCollisionData(); // Generate collision mesh data.
    char szObjPosting[_MAX_PATH] = "";
    _makepath(szObjPosting, szDrive, szDir, szFName,
              ".opd"); // "Object Posting Data" - Shape Manager file 이름을 정하고..
    ShapeMgr.SaveToFile(szObjPosting);

    //Save event...
    char szEventName[_MAX_PATH] = "";
    _makepath(szEventName, szDrive, szDir, szFName, ".gev"); //
    if (!m_pEventMgr->MakeGameFile(szEventName, m_pTerrain->m_iHeightMapSize)) {
        MessageBox(::GetActiveWindow(), lpszPathName, "Fail to make game event file...", MB_OK);
    }

    char szLightName[_MAX_PATH] = "";
    _makepath(szLightName, szDrive, szDir, szFName, ".glo"); //
    if (!m_pLightObjMgr->MakeGameFile(szLightName)) {
        MessageBox(::GetActiveWindow(), lpszPathName, "Fail to make game light info file...", MB_OK);
    }
}

void CMapMng::MakeTerrainMovableAttr(CN3ShapeMgr * pShapeMgr) {
    m_pTerrain->MakeMoveTable(m_pEventMgr->m_ppEvent); //Motion property settings...
    pShapeMgr->MakeMoveTable(m_pEventMgr->m_ppEvent);  //Motion property settings...
}

void CMapMng::MakeServerDataFiles(LPCTSTR lpszPathName) {
    if (NULL == m_pEventMgr->m_ppEvent) {
        m_pEventMgr->SetActive(true);
        m_pEventMgr->SetActive(false);
    }

    HANDLE hFile = CreateFile(lpszPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        m_pMainFrm->MessageBox("file write error");
        return;
    }

    //save terrain..
    m_pTerrain->SaveServerData(hFile);
    //

    // Create and save Shape Manager...
    //    if(true == m_pSceneOutput->CheckOverlappedShapesAndReport()) //Check for name or location duplicates...
    //    {
    //        int idYesNo = ::MessageBox(::GetActiveWindow(), "Do you want to delete duplicate objects?", "Handling duplicate objects", MB_YESNO);
    //        if(IDYES == idYesNo)
    //        {
    //            m_pSceneOutput->DeleteOverlappedShapes();
    //            m_pDlgOutputList->UpdateTree(m_pSceneOutput); // Check output list...
    //            m_SelOutputObjArray.RemoveAll(); // Initialize selection...
    //        }
    //    }

    CN3ShapeMgr ShapeMgr;
    ShapeMgr.Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                    (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
    int nSC = m_pSceneOutput->ShapeCount();
    for (int i = 0; i < nSC; i++) {
        ShapeMgr.Add(m_pSceneOutput->ShapeGet(i)); // Add Shape.
    }
    if (m_pWall) {
        m_pWall->AddWall2Coll(&ShapeMgr);
    }
    ShapeMgr.GenerateCollisionData(); // Generate collision mesh data.
    MakeTerrainMovableAttr(&ShapeMgr);
    ShapeMgr.SaveCollisionData(hFile); // Save only collision data...

    // Save Object Event.
    //
    DWORD dwNum;
    int   iEventObjectCount = 0; // First count the number...
    for (int i = 0; i < nSC; i++) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        if (pShape->m_iEventID || pShape->m_iEventType || pShape->m_iNPC_ID ||
            pShape->m_iNPC_Status) { // If there is an event
            iEventObjectCount++;
        }
    }

    WriteFile(hFile, &iEventObjectCount, 4, &dwNum, NULL);
    for (int i = 0; i < nSC; i++) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);
        short      sEvent = 0;
        __Vector3  vPos;
        if (pShape->m_iEventID || pShape->m_iEventType || pShape->m_iNPC_ID ||
            pShape->m_iNPC_Status) // If there is an event
        {
            WriteFile(hFile, &(pShape->m_iBelong), 4, &dwNum, NULL);
            sEvent = (short)(pShape->m_iEventID);
            WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iEventType);
            WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iNPC_ID);
            WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
            sEvent = (short)(pShape->m_iNPC_Status);
            WriteFile(hFile, &sEvent, 2, &dwNum, NULL);

            vPos = pShape->Pos();
            WriteFile(hFile, &(vPos.x), 4, &dwNum, NULL);
            WriteFile(hFile, &(vPos.y), 4, &dwNum, NULL);
            WriteFile(hFile, &(vPos.z), 4, &dwNum, NULL);
        }
    }

    //Save MapTile properties...
    //
    m_pEventMgr->MakeEventArray();
    /*
    for(int z=0; z<m_pTerrain->m_iHeightMapSize; z++)
    {
        for(int x=0; x<m_pTerrain->m_iHeightMapSize; x++)
        {
            WriteFile(hFile, &(m_pEventMgr->m_ppEvent[x][z]), sizeof(short), &dwNum, NULL);
        }        
    }
    */
    for (int x = 0; x < m_pTerrain->m_iHeightMapSize; x++) {
        WriteFile(hFile, m_pEventMgr->m_ppEvent[x], sizeof(short) * m_pTerrain->m_iHeightMapSize, &dwNum, NULL);
    }

    m_pRegenUser->SaveServerData(hFile);
    m_pWarpMgr->SaveServerData(hFile);

    //Event information..
    // file name
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_PATH], szExt[_MAX_EXT];
    _splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

    //Save event information..
    char szEvent[_MAX_PATH];
    char szEvtFName[_MAX_PATH];
    sprintf(szEvtFName, "%s_Event", szFName);
    _makepath(szEvent, szDrive, szDir, szEvtFName, "txt");
    m_pEventMgr->SaveInfoTextFile(szEvent); //Text file containing event ID conditions used by the server..

    // Let's extract it as a text file.
    FILE * stream = fopen("c:\\move.txt", "w");
    for (int z = m_pTerrain->m_iHeightMapSize - 1; z >= 0; z--) {
        for (int x = 0; x < m_pTerrain->m_iHeightMapSize; x++) {
            int v = m_pEventMgr->m_ppEvent[x][z];
            fprintf(stream, "%d ", v);
        }
        fprintf(stream, "\n");
    }
    fclose(stream);
    // pulled.

    /*    
    char szCollisionFN[512];
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszPathName, szDrive, szDir, szFName, szExt);
    _makepath(szCollisionFN, szDrive,szDir, szFName, ".scd"); // Save as another name..
    CFile file;
    file.Open(szCollisionFN, CFile::modeCreate | CFile::modeWrite);
    ShapeMgr.SaveCollisionData((HANDLE)file.m_hFile); // Save only collision data...
    file.Close();
*/
    CloseHandle(hFile);
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

    // Determine the location and size of the object
    __Vector3 vDir(-1, -1, 3);
    vDir.Normalize();
    __Vector3 vMin = pObj->Min();
    __Vector3 vMax = pObj->Max();

    __Vector3 vEye, vAt, vUp(0, 1, 0);
    vAt = vMin + (vMax - vMin) / 2.0f;
    vAt.y -= (vMax - vMin).Magnitude() / 8.0f;
    vEye = vAt - vDir * ((vMax - vMin).Magnitude() * 0.7f);
    //    pEng->s_CameraData.vEye = vEye;    // Due to the LOD setting of the shape..

    // back up
    __Matrix44 mtxOldView, mtxOldProj;
    pD3DDev->GetTransform(D3DTS_VIEW, &mtxOldView);
    pD3DDev->GetTransform(D3DTS_PROJECTION, &mtxOldProj);
    DWORD dwLighting;
    pD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);

    // Setting.. (Because the transform is changed inside the Apply function, it must be placed in this location.)
    //    pEng->s_CameraData.vAt = vAt;
    //    TempCamera.s_CameraData = pEng->s_CameraData;
    CN3Camera TempCamera;
    TempCamera.EyePosSet(vEye);
    TempCamera.AtPosSet(vAt);
    TempCamera.UpVectorSet(vUp);
    TempCamera.Tick();
    TempCamera.Apply(); // Insert data into temporary camera and calculate frustum information..

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

    CN3Base::s_AlphaMgr.Render(); // Draw Alpha primitive...
    pD3DDev->EndScene();          // end
    pEng->Present(hWnd);          // present

    // restore (if you do not return it to the previous state, picking from the terrain will not work properly)
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

    m_pBrushDlg->SetTerrain(NULL); // Make the Brush window invisible.
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
    // Planting grass
    m_SowSeedMng.SetActive(FALSE);

    switch (iMode) {
    case CM_SELECT:
        m_pDummy = NULL;
        break;
    case CM_OBJECT_BRUSH:
        m_pDummy = NULL;
        break;
    case CM_POS:
        m_pDummy = &m_PosDummy;
        m_pMainFrm->UpdateTransformInfo(); // Update position, rotation value, etc.
        break;
    case CM_ROTATE:
        m_pDummy = &m_RotDummy;
        break;
    case CM_SCALE:
        m_pDummy = &m_ScaleDummy;
        m_pMainFrm->UpdateTransformInfo(); // Update position, rotation value, etc.
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

                CN3ShapeMgr ShapeMgr;
                ShapeMgr.Create((m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE,
                                (m_pTerrain->m_iHeightMapSize - 1) * TERRAIN_CELL_SIZE);
                int nSC = m_pSceneOutput->ShapeCount();
                for (int i = 0; i < nSC; i++) {
                    ShapeMgr.Add(m_pSceneOutput->ShapeGet(i)); // Add Shape.
                }
                if (m_pWall) {
                    m_pWall->AddWall2Coll(&ShapeMgr);
                }
                ShapeMgr.GenerateCollisionData(); // Generate collision mesh data.

                MakeTerrainMovableAttr(&ShapeMgr);
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
        // Planting grass
    case CM_EDIT_SEED: {
        m_SowSeedMng.SetActive(TRUE);
    } break;
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

        m_pDummy->m_pTerrainRef = m_pTerrain; // Insert a terrain pointer into the dummy.
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

void CMapMng::DropSelObjToTerrain() { // Attach the selected object to the terrain. (Adjust Y value only)
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

void CMapMng::ImportTerrain(const char * szMeshFN) {
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

    m_pTerrain->Import(szMeshFN, dlg.m_fSize);

    m_pEventMgr->SetActive(true);
    m_pEventMgr->SetActive(false);

    SetCursor(hCurrentCur);

    m_bLoadingComplete = true;
}

void CMapMng::ImportTerrainHeight(const char * szMeshFN) {
    HCURSOR hLoadCur, hCurrentCur;
    hCurrentCur = GetCursor();
    hLoadCur = AfxGetApp()->LoadCursor(IDC_LOAD);
    SetCursor(hLoadCur);

    if (!m_pTerrain) {
        return;
    } else {
        m_pTerrain->ImportHeight(szMeshFN);
    }

    SetCursor(hCurrentCur);

    m_bLoadingComplete = true;
}

void CMapMng::RenderGrid(float fGridSize,
                         float fMaxDistance) // Draw a grid with fGridSize size up to fMaxDistance distance.
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

void CMapMng::SaveObjectPostData(LPCTSTR lpszFileName) {
    /*
    //////////////////////////////////
    // OldData
    if (m_pSceneOutput == NULL) return;

    FILE* stream = fopen(lpszFileName, "w");
    if (stream == NULL)
    {
        m_pMainFrm->MessageBox("The file cannot be created.");
        return;
    }

    // Separate the folder name...
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszFileName, szDrive, szDir, szFName, szExt);

    int iSC = m_pSceneOutput->ShapeCount();
    fprintf(stream, "Shape Count : %d\n", iSC);

    for(int i=0; i<iSC; ++i)
    {
        CN3Shape* pShape = m_pSceneOutput->ShapeGet(i);
        ASSERT(pShape);
        if (pShape == NULL) continue;

        char szSFN[MAX_PATH];
        _makepath(szSFN, szDrive, szDir, pShape->Name(), ".n3shape");
        pShape->SaveToFile(szSFN); // Save shape information as binary file..

        fprintf(stream, "%s\n", pShape->Name()); // Write the shape file name in the text.
    }
    fclose(stream);
    // OldData
    //////////////////////////////////
*/

    if (m_pSceneOutput == NULL) {
        return;
    }

    FILE * stream = fopen(lpszFileName, "w");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("The file cannot be created.");
        return;
    }

    // Separate the folder name...
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszFileName, szDrive, szDir, szFName, szExt); // Contains only the file name and extension.

    int iSC = m_pSceneOutput->ShapeCount();
    fprintf(stream, "Shape Post Count : %d\n", iSC);

    char szSFN[MAX_PATH] = "";
    for (int i = 0; i < iSC; ++i) {
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(i);

        _splitpath(pShape->FileName().c_str(), NULL, NULL, szFName,
                   szExt);                            // Contains only the file name and extension.
        _makepath(szSFN, NULL, NULL, szFName, szExt); // Recreate the file name.

        __Vector3    vPos = pShape->Pos();
        __Vector3    vScale = pShape->Scale();
        __Quaternion qtRot = pShape->Rot();
        int          iSPC = pShape->PartCount();
        fprintf(stream,
                "FileName[ %s ] PartCount[ %d ] Position[ %f %f %f] Rotation[ %f %f %f %f ] Scale[ %f %f %f ] Belong [ "
                "%d ] Attribute [ %d %d %d %d ]\n",
                szSFN, iSPC, vPos.x, vPos.y, vPos.z, qtRot.x, qtRot.y, qtRot.z, qtRot.w, vScale.x, vScale.y, vScale.z,
                pShape->m_iBelong, pShape->m_iEventID, pShape->m_iEventType, pShape->m_iNPC_ID, pShape->m_iNPC_Status);
        for (int j = 0; j < iSPC; j++) {
            CN3SPart * pPart = pShape->Part(j);
            fprintf(stream, "\tPart - DiffuseARGB[ %f %f %f %f ] AmbientARGB[ %f %f %f %f ]\n", pPart->m_Mtl.Diffuse.a,
                    pPart->m_Mtl.Diffuse.r, pPart->m_Mtl.Diffuse.g, pPart->m_Mtl.Diffuse.b, pPart->m_Mtl.Ambient.a,
                    pPart->m_Mtl.Ambient.r, pPart->m_Mtl.Ambient.g, pPart->m_Mtl.Ambient.b);
        }
    }

    fclose(stream);
}

void CMapMng::SaveObjectPostDataPartition(LPCTSTR lpszFileName, float psx, float psz, float width) {
    if (m_pSceneOutput == NULL) {
        return;
    }

    FILE * stream = fopen(lpszFileName, "w");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("The file cannot be created.");
        return;
    }

    // Separate the folder name...
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszFileName, szDrive, szDir, szFName, szExt); // Contains only the file name and extension.

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

        ShapeList.push_back(i);
    }

    int RealCount = ShapeList.size();
    fprintf(stream, "Shape Post Count : %d\n", RealCount);

    char szSFN[MAX_PATH] = "";

    std::list<int>::iterator it, ite;
    ite = ShapeList.end();

    ProgressBar.Create("Pick out Objects.", 50, RealCount);
    for (it = ShapeList.begin(); it != ite; it++) {
        ProgressBar.StepIt();
        int        idx = (*it);
        CN3Shape * pShape = m_pSceneOutput->ShapeGet(idx);

        _splitpath(pShape->FileName().c_str(), NULL, NULL, szFName,
                   szExt);                            // Contains only the file name and extension.
        _makepath(szSFN, NULL, NULL, szFName, szExt); // Recreate the file name.

        __Vector3 vPos = pShape->Pos();
        vPos.x -= sx;
        vPos.z -= sz;

        __Vector3    vScale = pShape->Scale();
        __Quaternion qtRot = pShape->Rot();
        int          iSPC = pShape->PartCount();
        fprintf(stream,
                "FileName[ %s ] PartCount[ %d ] Position[ %f %f %f] Rotation[ %f %f %f %f ] Scale[ %f %f %f ] Belong [ "
                "%d ] Attribute [ %d %d %d %d ]\n",
                szSFN, iSPC, vPos.x, vPos.y, vPos.z, qtRot.x, qtRot.y, qtRot.z, qtRot.w, vScale.x, vScale.y, vScale.z,
                pShape->m_iBelong, pShape->m_iEventID, pShape->m_iEventType, pShape->m_iNPC_ID, pShape->m_iNPC_Status);
        for (int j = 0; j < iSPC; j++) {
            CN3SPart * pPart = pShape->Part(j);
            fprintf(stream, "\tPart - DiffuseARGB[ %f %f %f %f ] AmbientARGB[ %f %f %f %f ]\n", pPart->m_Mtl.Diffuse.a,
                    pPart->m_Mtl.Diffuse.r, pPart->m_Mtl.Diffuse.g, pPart->m_Mtl.Diffuse.b, pPart->m_Mtl.Ambient.a,
                    pPart->m_Mtl.Ambient.r, pPart->m_Mtl.Ambient.g, pPart->m_Mtl.Ambient.b);
        }
    }

    fclose(stream);
}

void CMapMng::LoadObjectPostData(LPCTSTR lpszFileName) {
    if (m_pSceneOutput == NULL) {
        return;
    }

    // Release the objects in the scene...
    m_pSceneOutput->ShapeRelease();
    m_pSceneOutput->ChrRelease();

    FILE * stream = fopen(lpszFileName, "r");
    if (stream == NULL) {
        m_pMainFrm->MessageBox("The specified text file could not be found.");
        return;
    }

    // Separate the folder name...
    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _splitpath(lpszFileName, szDrive, szDir, szFName, szExt);

    char szFirstLine[256];
    fgets(szFirstLine, 256, stream);           // Read the first line...
    if (strstr(szFirstLine, "Shape Count : ")) // If there is a string, it is old data.
    {
        int iSC = 0, result = 0;
        sscanf(szFirstLine, "Shape Count : %d\n", &iSC);
        for (int i = 0; i < iSC; ++i) {
            char szDestName[_MAX_PATH];
            result = fscanf(stream, "%s\n", szDestName); // Read the file name...
            if (result == EOF) {
                break;
            }

            char szSFN[MAX_PATH];
            _makepath(szSFN, szDrive, szDir, szDestName, ".n3shape");

            CN3Shape * pShape = new CN3Shape;
            if (false == pShape->LoadFromFile(szSFN)) {
                delete pShape;
                pShape = NULL;
                continue; // Read shape information as binary file..
            }

            szDestName[lstrlen(szDestName) - 5] = NULL; // Subtract the _0000 string..
            _makepath(szSFN, NULL, NULL, szDestName, ".n3shape");
            pShape->m_szName = szDestName;
            pShape->FileNameSet(std::string(szSFN)); // Set file name again...

            m_pSceneOutput->ShapeAdd(pShape);
        }
    } else // This is newly created data..
    {
        int iSC = 0;
        sscanf(szFirstLine, "Shape Post Count : %d\n", &iSC);

        char szSFN[MAX_PATH] = "", szSFN2[MAX_PATH] = "";
        char szLine[1024] = "";
        for (int i = 0; i < iSC; ++i) {
            CN3Shape * pShape = new CN3Shape();
            m_pSceneOutput->ShapeAdd(pShape); // addition..

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
            // Write the shape file name in the text.
            wsprintf(szSFN2, "Object\\%s", szSFN);
            pShape->LoadFromFile(szSFN2); // Read from file...
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
            pShape->ReCalcMatrix(); // Recalculate the matrix...

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

void CMapMng::ImportPostDataFromScene(const char * szFileName) {
    HANDLE hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        return;
    }

    DWORD dwRWC = 0;

    int   nCameraActive;
    float fFrmCur, fFrmStart, fFrmEnd;
    ReadFile(hFile, &nCameraActive, 4, &dwRWC, NULL);
    ReadFile(hFile, &fFrmCur, 4, &dwRWC, NULL);   // Animation Frame;
    ReadFile(hFile, &fFrmStart, 4, &dwRWC, NULL); // Entire frame.
    ReadFile(hFile, &fFrmEnd, 4, &dwRWC, NULL);   // Entire frame.

    int  nL = 0;
    char szName[512] = "";

    int nCC = 0;
    ReadFile(hFile, &nCC, 4, &dwRWC, NULL); // camera..
    for (int i = 0; i < nCC; i++) {
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL <= 0) {
            continue;
        }

        ReadFile(hFile, szName, nL, &dwRWC, NULL);
        szName[nL] = NULL;
    }

    int nLC = 0;
    ReadFile(hFile, &nLC, 4, &dwRWC, NULL); // camera..
    for (int i = 0; i < nLC; i++) {
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL <= 0) {
            continue;
        }

        ReadFile(hFile, szName, nL, &dwRWC, NULL);
        szName[nL] = NULL;
    }

    int nSC = 0;
    ReadFile(hFile, &nSC, 4, &dwRWC, NULL); // Shapes..
    for (int i = 0; i < nSC; i++) {
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL <= 0) {
            continue;
        }

        ReadFile(hFile, szName, nL, &dwRWC, NULL);
        szName[nL] = NULL;

        // Import...
        this->AddShape(m_pSceneOutput, std::string(szName), TRUE);
    }

    int nChrC = 0;
    ReadFile(hFile, &nChrC, 4, &dwRWC, NULL); // character
    for (int i = 0; i < nChrC; i++) {
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL <= 0) {
            continue;
        }

        ReadFile(hFile, szName, nL, &dwRWC, NULL);
        szName[nL] = NULL;
    }

    m_pDlgOutputList->UpdateTree(m_pSceneOutput); // update tree...

    CloseHandle(hFile);
}

void CMapMng::ImportShape(const char * szFullPath) {
    // Import...
    this->AddShape(m_pSceneOutput, szFullPath, FALSE);
}

void CMapMng::DeleteUnusedFiles() {
    typedef std::map<std::string, CN3BaseFileAccess *> mapBase;
    typedef mapBase::value_type                        valBase;
    typedef mapBase::iterator                          it_Base;

    std::vector<std::string> invalidFNs;
    std::vector<std::string> unusedFNs;
    std::string              szFN;

    // First, put everything into the map.
    mapBase mBases;
    int     iSC = m_pSceneOutput->ShapeCount();

    CN3Shape *   pShape = NULL;
    CN3VMesh *   pVMesh = NULL;
    CN3SPart *   pPart = NULL;
    CN3PMesh *   pPMesh = NULL;
    CN3Texture * pTex = NULL;
    for (int i = 0; i < iSC; i++) {
        pShape = m_pSceneOutput->ShapeGet(i);
        if (NULL == pShape) {
            continue;
        }

        szFN = CN3Base::PathGet() + pShape->FileName();
        CharLower(&(szFN[0]));
        mBases.insert(valBase(szFN, pShape));

        pVMesh = pShape->CollisionMesh();
        if (pVMesh) {
            szFN = CN3Base::PathGet() + pVMesh->FileName();
            CharLower(&(szFN[0]));
            mBases.insert(valBase(szFN, pVMesh));
        } else {
            invalidFNs.push_back("NULL VMesh : " + pShape->FileName());
        }

        int iSPC = pShape->PartCount();
        for (int j = 0; j < iSPC; j++) {
            pPart = pShape->Part(j);
            if (NULL == pPart) {
                CString szErr;
                szErr.Format("NULL Part : %s - %dth Part", pShape->FileName().c_str(), j);
                invalidFNs.push_back(szErr.operator LPCTSTR());
                continue;
            }

            pPMesh = pPart->Mesh();
            if (pPMesh) {
                szFN = CN3Base::PathGet() + pPMesh->FileName();
                CharLower(&(szFN[0]));
                mBases.insert(valBase(szFN, pPMesh));
            } else {
                CString szErr;
                szErr.Format("NULL PMesh : %s - %dth Part", pShape->FileName().c_str(), j);
                invalidFNs.push_back(szErr.operator LPCTSTR());
            }

            int iSTC = pPart->TexCount();
            for (int k = 0; k < iSTC; k++) {
                pTex = pPart->Tex(k);
                if (pTex) {
                    szFN = CN3Base::PathGet() + pTex->FileName();
                    CharLower(&(szFN[0]));
                    mBases.insert(valBase(szFN, pTex));
                } else {
                    CString szErr;
                    szErr.Format("NULL Texture : %s - %dth Part, %dth Texture", pShape->FileName().c_str(), j, k);
                    invalidFNs.push_back(szErr.operator LPCTSTR());
                    continue;
                }
            }
        }
    }

    // Find the file...
    std::string szPath = CN3Base::PathGet() + "object\\";
    ::SetCurrentDirectory(szPath.c_str());
    CFileFind ff;

    BOOL    bFind;
    CString szFNTmp;
    CString szFNTmp2;

    for (ff.FindFile(); bFind = ff.FindNextFile();) {
        szFNTmp = ff.GetFilePath();
        szFNTmp2 = ff.GetFileName();

        if (szFNTmp2 == "." || szFNTmp2 == "..") {
            continue;
        }

        szFNTmp.MakeLower();

        szFN = szFNTmp;
        it_Base it = mBases.find(szFN);
        if (it != mBases.end()) {
            continue; // If I found it, I wrote it.
        }

        unusedFNs.push_back(szFN);
    }

    if (!bFind) {
        szFNTmp = ff.GetFilePath();
        szFNTmp2 = ff.GetFileName();

        szFNTmp.MakeLower();

        szFN = szFNTmp;
        it_Base it = mBases.find(szFN);
        if (it == mBases.end()) {
            unusedFNs.push_back(szFN);
        }
    }

    // Open the file deletion dialog box..
    CDlgUnusedFiles dlg;
    int             iUFC = unusedFNs.size();
    for (int i = 0; i < iUFC; i++) {
        dlg.m_FileNames.Add(unusedFNs[i].c_str());
    }

    int iIFC = invalidFNs.size();
    for (int i = 0; i < iIFC; i++) {
        dlg.m_InvalidFileNames.Add(invalidFNs[i].c_str());
    }

    dlg.DoModal();

    // Update all..
    m_pSelSourceObj = NULL; // If you do it like this, it won't work.
    m_SelOutputObjArray.RemoveAll();
    this->LoadSourceObjects(); // Read the Source Object again...
    this->UpdateAll();         // Update everything...
}

void CMapMng::DeleteOverlappedObjects() // Find objects with overlapping positions.
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
                OverlappedObjects.push_back(pObj1);
                break;
            }
        }

        iPosProgress++;
        ProgressBar.SetPos(iPosProgress);
    }

    iSC = OverlappedObjects.size();
    for (int i = 0; i < iSC; i++) {
        m_pSceneOutput->ShapeDelete(OverlappedObjects[i]); // Delete overlaps...
    }

    // update...
    m_pSelSourceObj = NULL; // If you do it like this, it won't work.
    m_SelOutputObjArray.RemoveAll();
    if (m_pDlgOutputList) {
        m_pDlgOutputList->UpdateTree(m_pSceneOutput); // Update everything...
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
        if (pObj->FileName() == m_pSelSourceObj->FileName()) {
            SameObjects.push_back(pObj);
        }
    }

    iSC = SameObjects.size();
    for (int i = 0; i < iSC; i++) {
        m_pSceneOutput->ShapeDelete(SameObjects[i]); // Delete overlaps...
    }

    // update...
    m_pSceneSource->ShapeDelete((CN3Shape *)m_pSelSourceObj);
    m_pSelSourceObj = NULL; // If you do it like this, it won't work.
    m_SelOutputObjArray.RemoveAll();
    this->UpdateAll(); // Update everything...
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
        // Delete the existing backup
        m_SelOutputObjBack.RemoveAll();
        vOldPos.Zero();

        // Back up the selected items
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

            // Enter a new location to take the picture
            ASSERT(m_pTerrain);
            CPoint point = ((CN3MEView *)m_pMainFrm->GetActiveView())->m_CurrMousePos;
            m_pTerrain->Pick(point.x, point.y, &vNewPos);

            if (vOldPos.Magnitude() == 0.0f || vNewPos.Magnitude() == 0.0f) {
                return;
            }

            vNewPos -= vOldPos; // Looking for a new space to move to

            if (m_pDummy) // Input new coordinates of the dummy
            {
                m_pDummy->PosSet(m_pDummy->Pos() + vNewPos);
            }

            m_SelOutputObjArray.RemoveAll(); // Delete the existing selected information
            for (int j = 0; j < iSize; ++j) {
                pDestObj = m_SelOutputObjBack.GetAt(j); // Find backed up data
                if (pDestObj == NULL) {
                    continue;
                }

                pNewObj = AddObjectToOutputScene(pDestObj); // Create new data only with address information
                if (pNewObj == NULL) {
                    continue;
                }

                m_SelOutputObjArray.InsertAt(0, pNewObj); // Input new information into existing information

                pNewObj->ScaleSet(pDestObj->Scale()); // Enter size
                pNewObj->RotSet(pDestObj->Rot());     // input rotation angle

                vObjPos = pDestObj->Pos() + vNewPos;
                vObjPos.y = m_pTerrain->GetHeight(vObjPos.x, vObjPos.z);
                pNewObj->PosSet(vObjPos); // Input new location point
            }
            OnSelChanged(); // Update with new data such as data input
        }
    } break;
    }
}