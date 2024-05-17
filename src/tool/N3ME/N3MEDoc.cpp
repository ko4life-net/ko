// N3MEDoc.cpp : implementation of the CN3MEDoc class
//

#include "StdAfx.h"
#include "N3ME.h"
#include "N3MEDoc.h"
#include "MapMng.h"
#include "MainFrm.h"

#include "DTexGroupMng.h"
#include "DTexMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3MEDoc

IMPLEMENT_DYNCREATE(CN3MEDoc, CDocument)

BEGIN_MESSAGE_MAP(CN3MEDoc, CDocument)
//{{AFX_MSG_MAP(CN3MEDoc)
ON_COMMAND(ID_FILE_IMPORT_POST_DATA_FROM_SCENE, OnFileImportPostDataFromScene)
ON_COMMAND(ID_FILE_IMPORT_SHAPES, OnFileImportShapes)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3MEDoc construction/destruction

CN3MEDoc::CN3MEDoc() {}

CN3MEDoc::~CN3MEDoc() {}

BOOL CN3MEDoc::OnNewDocument() {
    if (!CDocument::OnNewDocument()) {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    // When run for the first time, the AfxGetMainWnd() function does not pass CMainFrame*
    // Since ActiveWindow() is passed, use the method below
    CWinThread * pThread = AfxGetThread();
    if (pThread) {
        CMainFrame * pFrm = (CMainFrame *)pThread->m_pMainWnd;
        if (pFrm) {
            CMapMng * pMapMng = pFrm->GetMapMng();
            if (pMapMng) {
                pMapMng->Release();
                pMapMng->UpdateAll();
            }
        }
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CN3MEDoc serialization

void CN3MEDoc::Serialize(CArchive & ar) {
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CN3MEDoc diagnostics

#ifdef _DEBUG
void CN3MEDoc::AssertValid() const {
    CDocument::AssertValid();
}

void CN3MEDoc::Dump(CDumpContext & dc) const {
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3MEDoc commands

BOOL CN3MEDoc::OnOpenDocument(LPCTSTR lpszPathName) {
    if (!CDocument::OnOpenDocument(lpszPathName)) {
        return FALSE;
    }

    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pFrm->GetMapMng()->m_bLoadingComplete = false;
        pFrm->GetMapMng()->LoadFromFile(lpszPathName);
        pFrm->GetMapMng()->m_bLoadingComplete = true;

        //        pFrm->GetDTexGroupMng()->Init();
        //        pFrm->GetDTexGroupMng()->LoadFromFile();
        //        pFrm->GetDTexMng()->Init(pFrm);
        //        pFrm->GetDTexMng()->LoadFromFile();
    }

    return TRUE;
}

BOOL CN3MEDoc::OnSaveDocument(LPCTSTR lpszPathName) {
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pFrm->GetMapMng()->SaveToFile(lpszPathName);
    }

    return TRUE; //CDocument::OnSaveDocument(lpszPathName);
}

void CN3MEDoc::OnFileImportPostDataFromScene() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, "n3scene", NULL, dwFlags, "N3 Scene file(*.N3Scene)|*.N3Scene||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm && pFrm->GetMapMng()) {
        CMapMng * pMapMng = pFrm->GetMapMng();
        pMapMng->ImportPostDataFromScene(dlg.GetPathName());
    }
}

void CN3MEDoc::OnFileImportShapes() {
    CMapMng *    pMapMng = NULL;
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pMapMng = pFrm->GetMapMng();
    }
    if (NULL == pMapMng) {
        return;
    }

    char        szBuff[256000] = "";
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
    CFileDialog dlg(TRUE, "n3shape", NULL, dwFlags, "Shape files(*.N3Shape)|*.N3Shape||", NULL);
    dlg.m_ofn.nMaxFile = 25600;
    dlg.m_ofn.lpstrFile = szBuff;
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    POSITION pos = dlg.GetStartPosition();
    CString  szPath;
    while (pos != NULL) {
        szPath = dlg.GetNextPathName(pos);
        pMapMng->ImportShape(szPath);
    }

    pMapMng->UpdateAll();
}
