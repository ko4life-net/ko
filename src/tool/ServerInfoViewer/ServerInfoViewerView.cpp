// ServerInfoViewerView.cpp : implementation of the CServerInfoViewerView class
//

#include "StdAfx.h"
#include "ServerInfoViewer.h"

#include "MainFrm.h"
#include "ServerInfoViewerDoc.h"
#include "ServerInfoViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView

IMPLEMENT_DYNCREATE(CServerInfoViewerView, CView)

BEGIN_MESSAGE_MAP(CServerInfoViewerView, CView)
//{{AFX_MSG_MAP(CServerInfoViewerView)
ON_WM_ERASEBKGND()
ON_WM_KEYDOWN()
ON_WM_LBUTTONDBLCLK()
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView construction/destruction

CServerInfoViewerView::CServerInfoViewerView() {
    // TODO: add construction code here
}

CServerInfoViewerView::~CServerInfoViewerView() {}

BOOL CServerInfoViewerView::PreCreateWindow(CREATESTRUCT & cs) {
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView drawing

void CServerInfoViewerView::OnDraw(CDC * pDC) {
    CServerInfoViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    D3DCOLOR     crBkg = 0x00000000;

    if (NULL == pFrm || NULL == pFrm->m_Eng.s_lpD3DDev) {
        return;
    }

    pFrm->m_Eng.Clear(crBkg);

    pFrm->m_Camera.Tick();
    pFrm->m_Light.Tick();

    pFrm->m_Camera.Apply();
    pFrm->m_Light.Apply();

    pFrm->m_Eng.s_lpD3DDev->BeginScene();

    __Matrix44 mtx;
    mtx.Identity();
    mtx.Scale(64, 64, 64);
    mtx.PosSet(2048.0f, 0, 2048.0f);
    pFrm->m_Eng.RenderGrid(mtx); // 4 미터에 한개씩 라인 표시..
    pFrm->m_Eng.RenderAxis();

    __CharacterSymbol * pSymbol = NULL;
    it_CSymbol          it, itEnd;

    for (it = pDoc->m_Players.begin(), itEnd = pDoc->m_Players.end(); it != itEnd; it++) {
        pSymbol = &(*it);

        pFrm->m_Eng.s_lpD3DDev->SetTransform(D3DTS_WORLD, &(pSymbol->mtxWorld));
        pFrm->m_Eng.RenderLines(pSymbol->vBoxes, 4, pSymbol->color);
    }

    for (it = pDoc->m_NPCs.begin(), itEnd = pDoc->m_NPCs.end(); it != itEnd; it++) {
        pSymbol = &(*it);

        pFrm->m_Eng.s_lpD3DDev->SetTransform(D3DTS_WORLD, &(pSymbol->mtxWorld));
        pFrm->m_Eng.RenderLines(pSymbol->vBoxes, 4, pSymbol->color);
    }

    for (it = pDoc->m_Monsters.begin(), itEnd = pDoc->m_Monsters.end(); it != itEnd; it++) {
        pSymbol = &(*it);

        pFrm->m_Eng.s_lpD3DDev->SetTransform(D3DTS_WORLD, &(pSymbol->mtxWorld));
        pFrm->m_Eng.RenderLines(pSymbol->vBoxes, 4, pSymbol->color);
    }

    pFrm->m_Eng.s_lpD3DDev->EndScene();

    pFrm->m_Eng.Present(m_hWnd);
    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView printing

BOOL CServerInfoViewerView::OnPreparePrinting(CPrintInfo * pInfo) {
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CServerInfoViewerView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
    // TODO: add extra initialization before printing
}

void CServerInfoViewerView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView diagnostics

#ifdef _DEBUG
void CServerInfoViewerView::AssertValid() const {
    CView::AssertValid();
}

void CServerInfoViewerView::Dump(CDumpContext & dc) const {
    CView::Dump(dc);
}

CServerInfoViewerDoc * CServerInfoViewerView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerInfoViewerDoc)));
    return (CServerInfoViewerDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerView message handlers

LRESULT CServerInfoViewerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        MSG msg;
        msg.hwnd = m_hWnd;
        msg.message = message;
        msg.wParam = wParam;
        msg.lParam = lParam;
        if (pFrm->m_Camera.MoveByWindowMessage(&msg)) {
            this->Invalidate(FALSE);
            return TRUE;
        }
    }

    return CView::WindowProc(message, wParam, lParam);
}

BOOL CServerInfoViewerView::OnEraseBkgnd(CDC * pDC) {
    return TRUE; // TODO: Add your message handler code here and/or call default

    return CView::OnEraseBkgnd(pDC);
}

void CServerInfoViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    // TODO: Add your message handler code here and/or call default
    if (nChar == 'F') // 선택된 오브젝트를 포커스..
    {
        this->FocusGround();
    }

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CServerInfoViewerView::FocusGround() {
    CServerInfoViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    __CharacterSymbol *pSymbol = NULL, *pSymbolFocus = NULL;
    it_CSymbol         it, itEnd;

    CPoint point;
    GetCursorPos(&point);
    ScreenToClient(&point);

    __Vector3 vPos, vDir;
    ::_Convert2D_To_3DCoordinate(point.x, point.y, CN3Base::s_CameraData.mtxView, CN3Base::s_CameraData.mtxProjection,
                                 CN3Base::s_CameraData.vp, vPos, vDir);

    __Vector3 v0(0, 0, 4096), v1(4096, 0, 4096), v2(4096, 0, 0), v3(0, 0, 0); // 큰판에 찍히면..
    __Vector3 vCol;
    float     t, u, v;
    if (::_IntersectTriangle(vPos, vDir, v0, v1, v2, t, u, v, &vCol) ||
        ::_IntersectTriangle(vPos, vDir, v0, v2, v3, t, u, v, &vCol)) {
        CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
        __Vector3    vAt = vCol;
        __Vector3    vEye = vAt + __Vector3(0, 500, -500);
        pFrm->m_Camera.EyePosSet(vEye);
        pFrm->m_Camera.AtPosSet(vAt);
        pFrm->m_Camera.UpVectorSet(0, 1, 0);

        pFrm->GetActiveView()->InvalidateRect(NULL, FALSE);
    }

    /*    __Vector3 v0, v1, v2, v3;
    for(it = pDoc->m_Players.begin(), itEnd = pDoc->m_Players.end(); it != itEnd; it++)
    {
        pSymbol = &(*it);

        v0 = pSymbol->vBoxes[0] * pSymbol->mtxWorld;
        v1 = pSymbol->vBoxes[1] * pSymbol->mtxWorld;
        v2 = pSymbol->vBoxes[2] * pSymbol->mtxWorld;
        v3 = pSymbol->vBoxes[3] * pSymbol->mtxWorld;
        if(    ::IntersectTriangle(vPos, vDir, v0, v1, v2) ||
            ::IntersectTriangle(vPos, vDir, v0, v2, v3) )
        {
            pSymbolFocus = pSymbol;
            break;
        }
    }

    if(pSymbolFocus)
    {
        CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
        __Vector3 vAt = pSymbolFocus->mtxWorld.Pos();
        __Vector3 vEye = vAt + __Vector3(0, 50, - 50);
        pFrm->m_Camera.EyePosSet(vEye);
        pFrm->m_Camera.AtPosSet(vAt);
        pFrm->m_Camera.UpVectorSet(0, 1, 0);

        pFrm->GetActiveView()->InvalidateRect(NULL, FALSE);
    }
*/
}

void CServerInfoViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) {
    // TODO: Add your message handler code here and/or call default
    CServerInfoViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    __CharacterSymbol *pSymbol = NULL, *pSymbolFocus = NULL;
    it_CSymbol         it, itEnd;

    __Vector3 vPos, vDir;
    ::_Convert2D_To_3DCoordinate(point.x, point.y, CN3Base::s_CameraData.mtxView, CN3Base::s_CameraData.mtxProjection,
                                 CN3Base::s_CameraData.vp, vPos, vDir);

    __Vector3 v0(0, 0, 4096), v1(4096, 0, 4096), v2(4096, 0, 0), v3(0, 0, 0); // 큰판에 찍히면..
    __Vector3 vCol;
    float     t, u, v;
    if (::_IntersectTriangle(vPos, vDir, v0, v1, v2, t, u, v, &vCol) ||
        ::_IntersectTriangle(vPos, vDir, v0, v2, v3, t, u, v, &vCol)) {
        CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
        __Vector3    vAt = vCol;
        __Vector3    vEye = vAt + __Vector3(0, 500, -500);
        pFrm->m_Camera.EyePosSet(vEye);
        pFrm->m_Camera.AtPosSet(vAt);
        pFrm->m_Camera.UpVectorSet(0, 1, 0);

        pFrm->GetActiveView()->InvalidateRect(NULL, FALSE);
    }

    CView::OnLButtonDblClk(nFlags, point);
}
