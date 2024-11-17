// DlgShapeList.cpp : implementation file
//

#include "StdAfx.h"
#include "N3ME.h"
#include "DlgShapeList.h"

#include "MainFrm.h"
#include "MapMng.h"

#include "N3Base/N3Shape.h"
#include "N3Base/N3Scene.h"

#include <map>
#include <string>

typedef std::multimap<std::string, CN3Shape *> ShapeMap;
typedef ShapeMap::value_type                   SMValue;
typedef ShapeMap::iterator                     SMIter;

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
ON_LBN_SELCHANGE(IDC_LIST_OBJ, OnSelchangeListObj)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BTN_SORT, OnBtnSort)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShapeList message handlers

void CDlgShapeList::UpdateTree(CN3Scene * pScene) {
    m_ListShape.ResetContent();

    m_pSceneRef = pScene;

    int nSC = m_pSceneRef->ShapeCount();
    for (int i = 0; i < nSC; i++) {
        CN3Shape * pShape = m_pSceneRef->ShapeGet(i);
        if (pShape) {
            m_ListShape.InsertString(i, pShape->FilePath().stem().string().c_str());
            m_ListShape.SetItemDataPtr(i, pShape);
        }
    }
}

void CDlgShapeList::SelectObject(void * pItemData) {
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

    m_ListShape.ResetContent();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShapeList::OnSelchangeListObj() {
    int   SelCount = m_ListShape.GetSelCount();
    int * SelIdx = new int[SelCount];
    m_ListShape.GetSelItems(SelCount, SelIdx);
    for (int i = 0; i < SelCount; i++) {
        CN3Shape * pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(SelIdx[i]);
        if (pShape) {
            if (i == 0) {
                m_pMapMng->SelectObject((CN3Base *)pShape, m_IsSourceObj);
            } else {
                m_pMapMng->SelectObject((CN3Base *)pShape, m_IsSourceObj, !m_IsSourceObj);
            }
        }
    }
    delete[] SelIdx;

    int idx = m_ListShape.GetCurSel();
    if (idx < 0) {
        return;
    }

    CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
    if (pWnd && m_ListShape.GetSafeHwnd()) {
        CN3Shape * pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(idx);
        if (pShape) {
            //m_pMapMng->SelectObject((CN3Base*)pShape, m_IsSourceObj, !m_IsSourceObj);
            m_pMapMng->RenderObjectToWindow((CN3TransformCollision *)pShape, pWnd->m_hWnd);
        } else {
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
    HACCEL hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    if (hAccel) {
        TranslateAccelerator(AfxGetMainWnd()->m_hWnd, hAccel, pMsg);
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgShapeList::OnBtnSort() {
    ShapeMap Map;
    Map.clear();

    int cnt = m_ListShape.GetCount();
    for (int i = 0; i < cnt; i++) {
        CN3Shape * pShape = (CN3Shape *)m_ListShape.GetItemDataPtr(i);

        CString szShapeFileName;
        m_ListShape.GetText(i, szShapeFileName);
        Map.insert(SMValue(szShapeFileName.GetString(), pShape));
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
