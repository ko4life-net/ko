// DlgShapeList.cpp : implementation file
//

#include "StdAfx.h"
#include "N3Indoor.h"
#include "DlgShapeList.h"

#include "MainFrm.h"
#include "OrganizeView.h"
#include "ShapeTypeDlg.h"

#include "N3Base/N3Shape.h"
#include "N3Base/N3Scene.h"

#include <map>
#include <string>

typedef std::multimap<std::string, CN3Shape *> ShapeMap;
typedef ShapeMap::value_type                   SMValue;
typedef ShapeMap::iterator                     SMIter;

typedef std::multimap<std::string, ShapeInfo *> ShapeInfoMap;
typedef ShapeInfoMap::value_type                SIMValue;
typedef ShapeInfoMap::iterator                  SIMIter;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShapeList dialog

CDlgShapeList::CDlgShapeList(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgShapeList::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgShapeList)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_IsSourceObj = TRUE;
}

void CDlgShapeList::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgShapeList)
    DDX_Control(pDX, IDC_LIST_OBJ, m_ListShape);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgShapeList, CDialog)
//{{AFX_MSG_MAP(CDlgShapeList)
ON_BN_CLICKED(IDC_BTN_SORT, OnBtnSort)
ON_LBN_SELCHANGE(IDC_LIST_OBJ, OnSelchangeListObj)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_OK, OnOk)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShapeList message handlers

void CDlgShapeList::UpdateTree(CN3Scene * pScene) {
    m_ListShape.ResetContent();

    if (pScene && m_IsSourceObj) // Source용 버전..
    {
        m_pSceneRef = pScene;

        int nSC = m_pSceneRef->ShapeCount();
        for (int i = 0; i < nSC; i++) {
            CN3Shape * pShape = m_pSceneRef->ShapeGet(i);
            if (pShape) {
                char szFName[MAX_PATH];
                _splitpath(pShape->FileName().c_str(), NULL, NULL, szFName, NULL);
                m_ListShape.InsertString(i, szFName);
                m_ListShape.SetItemDataPtr(i, pShape);
            }
        }
    } else if (!pScene && !m_IsSourceObj) // Output용 버전..
    {
        CMainFrame *    pFrm = (CMainFrame *)AfxGetMainWnd();
        COrganizeView * pView = pFrm->GetOrganizeView();

        int         i = 0;
        ShapeInfo * pSI = NULL;
        siiter      siit = pView->m_PVSMgr.m_plShapeInfoList.begin();
        while (siit != pView->m_PVSMgr.m_plShapeInfoList.end()) {
            pSI = *siit++;
            char szFName[MAX_PATH];
            _splitpath(pSI->m_strShapeFile.c_str(), NULL, NULL, szFName, NULL);
            m_ListShape.InsertString(i, szFName);
            m_ListShape.SetItemDataPtr(i, pSI);
            i++;
        }

        CPortalVolume * pVol = NULL;
        iter            it = pView->m_PVSMgr.m_pPvsList.begin();
        while (it != pView->m_PVSMgr.m_pPvsList.end()) {
            pVol = *it++;

            siiter siit = pVol->m_plShapeInfoList.begin();
            while (siit != pVol->m_plShapeInfoList.end()) {
                pSI = *siit++;
                char szFName[MAX_PATH];
                _splitpath(pSI->m_strShapeFile.c_str(), NULL, NULL, szFName, NULL);
                m_ListShape.InsertString(i, szFName);
                m_ListShape.SetItemDataPtr(i, pSI);
                i++;
            }
        }
    }
}

int CDlgShapeList::GetTotalShapeInfoCount() // Output용..
{
    if (m_IsSourceObj) {
        ASSERT(0);
        return 0;
    }

    CMainFrame *    pFrm = (CMainFrame *)AfxGetMainWnd();
    COrganizeView * pView = pFrm->GetOrganizeView();

    int         i = 0;
    ShapeInfo * pSI = NULL;
    siiter      siit = pView->m_PVSMgr.m_plShapeInfoList.begin();
    while (siit != pView->m_PVSMgr.m_plShapeInfoList.end()) {
        pSI = *siit++;
        i++;
    }

    CPortalVolume * pVol = NULL;
    iter            it = pView->m_PVSMgr.m_pPvsList.begin();
    while (it != pView->m_PVSMgr.m_pPvsList.end()) {
        pVol = *it++;

        siiter siit = pVol->m_plShapeInfoList.begin();
        while (siit != pVol->m_plShapeInfoList.end()) {
            pSI = *siit++;
            i++;
        }
    }

    return i;
}

CN3Shape * CDlgShapeList::GetShapeByiOrder(int iOrder) // Output용..
{
    if (m_IsSourceObj) {
        ASSERT(0);
        return 0;
    }

    CMainFrame *    pFrm = (CMainFrame *)AfxGetMainWnd();
    COrganizeView * pView = pFrm->GetOrganizeView();

    int         i = 0;
    ShapeInfo * pSI = NULL;
    siiter      siit = pView->m_PVSMgr.m_plShapeInfoList.begin();
    while (siit != pView->m_PVSMgr.m_plShapeInfoList.end()) {
        pSI = *siit++;
        if (i == iOrder) {
            return pSI->m_pShape;
        }
        i++;
    }

    CPortalVolume * pVol = NULL;
    iter            it = pView->m_PVSMgr.m_pPvsList.begin();
    while (it != pView->m_PVSMgr.m_pPvsList.end()) {
        pVol = *it++;

        siiter siit = pVol->m_plShapeInfoList.begin();
        while (siit != pVol->m_plShapeInfoList.end()) {
            pSI = *siit++;
            if (i == iOrder) {
                return pSI->m_pShape;
            }
            i++;
        }
    }

    return NULL;
}

void CDlgShapeList::SelectObject(void * pItemData) // Output용 버전..
{
    if (m_IsSourceObj) {
        return;
    }

    int nSC = m_ListShape.GetCount();
    for (int i = 0; i < nSC; i++) {
        if (m_ListShape.GetItemDataPtr(i) == pItemData) {
            m_ListShape.SetCurSel(i);
            return;
        }
    }
}

BOOL CDlgShapeList::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_ListShape.ResetContent();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShapeList::OnSelchangeListObj() {
    // TODO: Add your control notification handler code here
    CMainFrame * pFrm = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();

    // Source와 Output 공용..
    int idx = m_ListShape.GetCurSel();
    if (idx < 0) {
        return;
    }

    CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
    if (pWnd && m_ListShape.GetSafeHwnd()) {
        bool bFound = false;
        if (m_IsSourceObj) {
            CN3Shape * pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(idx);
            if (pShape) {
                pFrm->RenderObjectToWindow((CN3TransformCollision *)pShape, pWnd->m_hWnd);
                bFound = true;
            }
        } else {
            // Output..
            ShapeInfo * pSI = (ShapeInfo *)m_ListShape.GetItemDataPtr(idx);
            if (pSI) {
                pFrm->RenderObjectToWindow((CN3TransformCollision *)pSI->m_pShape, pWnd->m_hWnd);
                bFound = true;

                if (pFrm->m_pDummy) {
                    SelectElement se;
                    se.eST = TYPE_SHAPE_ONLY;
                    se.pSelectPointer = pSI;
                    if (se.pSelectPointer) {
                        pFrm->m_pDummy->SetSelObj(se);
                    }
                }

                pFrm->m_LastSelectedElement.eST = TYPE_SHAPE_ONLY;
                pFrm->m_LastSelectedElement.pSelectPointer = pSI;

                pFrm->UpdateShapeInfoDisplay();
            }
        }

        if (!bFound) {
            CRect rc;
            pWnd->GetClientRect(&rc);
            CDC * pDC = pWnd->GetDC();
            pDC->FillSolidRect(&rc, RGB(192, 192, 192));
            pWnd->ReleaseDC(pDC);
        }
    }
    pWnd->Invalidate();
}

void CDlgShapeList::OnSize(UINT nType, int cx, int cy) {
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    const int iPreviewCX = 200;
    const int iPreviewCY = 150;
    const int iButtonCY = 20;
    if (m_ListShape.GetSafeHwnd() != NULL) {
        int iPreviewTop = cy - iPreviewCY;
        int iPreviewLeft = (cx - iPreviewCX) / 2;
        if (iPreviewLeft < 0) {
            iPreviewLeft = 0;
        }
        m_ListShape.SetWindowPos(NULL, 0, iButtonCY, cx, iPreviewTop - iButtonCY, SWP_NOZORDER);
        GetDlgItem(IDC_PREVIEW)->SetWindowPos(NULL, iPreviewLeft, iPreviewTop, iPreviewCX, iPreviewCY, SWP_NOZORDER);
    }
}

BOOL CDlgShapeList::PreTranslateMessage(MSG * pMsg) {
    // TODO: Add your specialized code here and/or call the base class
    HACCEL hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    if (hAccel) {
        TranslateAccelerator(AfxGetMainWnd()->m_hWnd, hAccel, pMsg);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgShapeList::OnBtnSort() {
    // TODO: Add your control notification handler code here
    // Source 버전..
    if (m_IsSourceObj) {
        ShapeMap Map;
        Map.clear();

        int cnt = m_ListShape.GetCount();
        for (int i = 0; i < cnt; i++) {
            char        buff[MAX_PATH];
            std::string str;
            CN3Shape *  pShape;

            pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(i);
            m_ListShape.GetText(i, buff);
            str = buff;

            Map.insert(SMValue(str, pShape));
        }

        m_ListShape.ResetContent();

        SMIter it = Map.begin();
        for (int i = 0; i < cnt; i++) {
            std::string str = (*it).first;
            CN3Shape *  pShape = (*it).second;

            m_ListShape.InsertString(i, str.c_str());
            m_ListShape.SetItemDataPtr(i, pShape);
            it++;
        }
    }
    // Output 버전..
    else {
        ShapeInfoMap Map;
        Map.clear();

        int cnt = m_ListShape.GetCount();
        for (int i = 0; i < cnt; i++) {
            char        buff[MAX_PATH];
            std::string str;
            ShapeInfo * pSI;

            pSI = (ShapeInfo *)m_ListShape.GetItemDataPtr(i);
            m_ListShape.GetText(i, buff);
            str = buff;

            Map.insert(SIMValue(str, pSI));
        }

        m_ListShape.ResetContent();

        SIMIter it = Map.begin();
        for (int i = 0; i < cnt; i++) {
            std::string str = (*it).first;
            ShapeInfo * pSI = (*it).second;

            m_ListShape.InsertString(i, str.c_str());
            m_ListShape.SetItemDataPtr(i, pSI);
            it++;
        }
    }
}

void CDlgShapeList::OnOk() {
    // TODO: Add your control notification handler code here
    // 당연히 Source 버전..
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    CN3Shape *   pShape = NULL;
    int          SelCount = m_ListShape.GetSelCount();
    int *        SelIdx = new int[SelCount];
    m_ListShape.GetSelItems(SelCount, SelIdx);
    for (int i = 0; i < SelCount; i++) {
        pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(SelIdx[i]);
        if (pShape) {
            break;
        }
    }
    delete[] SelIdx;

    if (!pShape) {
        return;
    }

    CShapeTypeDlg dlg;
    dlg.DoModal();

    if (!dlg.m_bValidate) {
        return;
    }
    if (dlg.m_bPartial) {
        pFrm->GetOrganizeView()->ShapeRegisterToManager(pShape);
    } else {
        if (!pFrm->GetOrganizeView()->IsSelectedPvs()) {
            AfxMessageBox("No items or Not Volumn were selected!\n");
            return;
        }

        pFrm->GetOrganizeView()->ShapeLinkToPVolumn(pShape);
    }

    pFrm->OutputDlgRefresh();
}
