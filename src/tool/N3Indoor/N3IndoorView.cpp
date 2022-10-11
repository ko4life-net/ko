// N3IndoorView.cpp : implementation of the CN3IndoorView class
//

#include "StdAfx.h"
#include "N3Indoor.h"

#include "MainFrm.h"
#include "N3IndoorDoc.h"
#include "N3IndoorView.h"
#include "OrganizeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorView

IMPLEMENT_DYNCREATE(CN3IndoorView, CView)

BEGIN_MESSAGE_MAP(CN3IndoorView, CView)
//{{AFX_MSG_MAP(CN3IndoorView)
ON_WM_SIZE()
ON_WM_DESTROY()
ON_WM_CHAR()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorView construction/destruction

CN3IndoorView::CN3IndoorView() {
    // TODO: add construction code here
    m_bSwapped = false;
    m_bValid = true;
    m_bExecuteCameraTop = false;
    m_rcSelDrag.left = m_rcSelDrag.top = m_rcSelDrag.right = m_rcSelDrag.bottom = 0;
}

CN3IndoorView::~CN3IndoorView() {}

BOOL CN3IndoorView::PreCreateWindow(CREATESTRUCT & cs) {
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorView drawing

void CN3IndoorView::OnDraw(CDC * pDC) {
    CN3IndoorDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: add draw code for native data here

    TickRender();
}

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorView diagnostics

#ifdef _DEBUG
void CN3IndoorView::AssertValid() const {
    CView::AssertValid();
}

void CN3IndoorView::Dump(CDumpContext & dc) const {
    CView::Dump(dc);
}

CN3IndoorDoc * CN3IndoorView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CN3IndoorDoc)));
    return (CN3IndoorDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorView message handlers

void CN3IndoorView::OnSize(UINT nType, int cx, int cy) {
    CView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    ((CMainFrame *)AfxGetMainWnd())->m_Eng.Reset(TRUE, cx, cy, 0);
}

void CN3IndoorView::TickRender() {
    CMainFrame * pFrm = ((CMainFrame *)AfxGetMainWnd());
    if (!pFrm) {
        return;
    }

    pFrm->m_Eng.Clear(0xff222222);
    pFrm->m_Eng.s_lpD3DDev->BeginScene();
    __Matrix44 mtxScale;
    __Matrix44 mtxWorld;
    mtxWorld.Identity();
    pFrm->m_Eng.s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);

    mtxScale.Scale(1, 1, 1);
    if (pFrm->m_eState != STATE_EXECUTE) {
        pFrm->m_Eng.RenderGrid(mtxScale);
        pFrm->m_Eng.RenderAxis(true);
    }

    pFrm->m_Camera.Tick();
    pFrm->m_Camera.Apply();
    pFrm->m_Light.Tick();
    pFrm->m_Light.Apply();

    pFrm->m_Eng.s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    // Render..
    COrganizeView * pView = pFrm->GetOrganizeView();
    if (pView) {
        pView->TickRender();
    }

    if (pFrm->m_pDummy) {
        pFrm->m_pDummy->Tick();
        pFrm->m_pDummy->Render();
    }

    RenderDragRect(&m_rcSelDrag);

    pFrm->m_Eng.s_lpD3DDev->EndScene();
    pFrm->m_Eng.Present(m_hWnd);

    // 프레임 표시
    DWORD          dwTick = GetTickCount();
    static DWORD   dwTickPrev = dwTick;
    static CString szFPS;
    if (dwTick > dwTickPrev + 100) {
        szFPS.Format("FPS : %6.2f", ((CMainFrame *)AfxGetMainWnd())->m_Eng.s_fFrmPerSec);
        dwTickPrev = dwTick;
    }

    if (pFrm) {
        pFrm->SetFPSString((LPCTSTR)szFPS);
    }
}

LRESULT CN3IndoorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    CMainFrame *    pFrm = NULL;
    COrganizeView * pView = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();
    if (!pFrm) {
        return CView::WindowProc(message, wParam, lParam);
    }

    MSG msg;
    msg.hwnd = m_hWnd;
    msg.message = message;
    msg.wParam = wParam;
    msg.lParam = lParam;

    if (CustomCameraMove(&msg) || pFrm->m_Camera.MoveByWindowMessage(&msg)) {
        this->Invalidate(FALSE);
    }

    return CView::WindowProc(message, wParam, lParam);
}

void CN3IndoorView::OnDestroy() {
    CView::OnDestroy();

    // TODO: Add your message handler code here
    m_bValid = false;
}

BOOL CN3IndoorView::ExecuteModeMsgFilter(MSG * pMsg) {
    CMainFrame *    pFrm = NULL;
    COrganizeView * pView = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pView = pFrm->GetOrganizeView();
    } else {
        return FALSE;
    }
    if (!pView) {
        return FALSE;
    }

    switch (pMsg->message) {
    case WM_KEYDOWN: {
        float fRY = 0.01f;
        switch (pMsg->wParam) {
        case 0x41: // 왼쪽..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec = vAt - vPos;
                vec.Normalize();
                __Matrix44 mtx;
                mtx.RotationY(-fRY * 10.0f);
                vec *= mtx;
                pFrm->m_Camera.AtPosSet(vPos + vec);
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;

        case 0x44: // 오른쪽..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec = vAt - vPos;
                vec.Normalize();
                __Matrix44 mtx;
                mtx.RotationY(fRY * 10.0f);
                vec *= mtx;
                pFrm->m_Camera.AtPosSet(vPos + vec);
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;

        case 0x57: // 전진..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec = vAt - vPos;
                vec.Normalize();
                vec *= 0.6f;

                COrganizeView * pView = pFrm->GetOrganizeView();
                __Vector3       vDest = vPos + vec;
                if (pView->QueryHeight(vDest.x, vDest.z, vDest.y)) {
                    pFrm->m_Camera.EyePosSet(vDest);
                    pFrm->m_Camera.AtPosSet(vDest + vec);
                }
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;

        case 0x53: // 후진..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec = vAt - vPos;
                vec.Normalize();
                vec *= 0.6f;

                COrganizeView * pView = pFrm->GetOrganizeView();
                __Vector3       vDest = vPos - vec;
                if (pView->QueryHeight(vDest.x, vDest.z, vDest.y)) {
                    pFrm->m_Camera.EyePosSet(vDest);
                    pFrm->m_Camera.AtPosSet(vDest + vec);
                }
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;

        case 0x58: // 밑으로..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec;
                vec.Set(0.0f, -0.1f, 0.0f);
                pFrm->m_Camera.EyePosSet(vPos + vec);
                pFrm->m_Camera.AtPosSet(vAt + vec);
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;

        case 0x43: // 위로..
            if (m_bExecuteCameraTop) {
                pView->RestoreExecuteCameraChange();
                m_bExecuteCameraTop = !m_bExecuteCameraTop;
            }
            {
                __Vector3 vPos = pFrm->m_Camera.EyePos();
                __Vector3 vAt = pFrm->m_Camera.AtPos();
                __Vector3 vec;
                vec.Set(0.0f, 0.1f, 0.0f);
                pFrm->m_Camera.EyePosSet(vPos + vec);
                pFrm->m_Camera.AtPosSet(vAt + vec);
                pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
            }
            break;
        }
    } break;
    }

    return FALSE;
}

void CN3IndoorView::RenderDragRect(RECT * rc) {
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

    CMainFrame *      pFrm = (CMainFrame *)AfxGetMainWnd();
    CN3Eng *          pEng = &(pFrm->m_Eng);
    LPDIRECT3DDEVICE9 pD3DDev = pEng->s_lpD3DDev;

    HRESULT hr;
    hr = pD3DDev->SetTexture(0, NULL);
    hr = pD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = pD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    hr = pD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
    hr = pD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, Vertices, sizeof(__VertexTransformedColor));
}

BOOL CN3IndoorView::CustomCameraMove(MSG * pMsg) {
    if (!m_bValid) {
        return FALSE;
    }
    CMainFrame *    pFrm = NULL;
    COrganizeView * pView = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pView = pFrm->GetOrganizeView();
    } else {
        return FALSE;
    }
    if (!pView) {
        return FALSE;
    }

    static POINT ptPrev;

    POINT point = {(short)LOWORD(pMsg->lParam), (short)HIWORD(pMsg->lParam)};
    POINT ptDelta = {point.x - ptPrev.x, point.y - ptPrev.y};

    if (pFrm->m_pDummy && pFrm->m_pDummy->MouseMsgFilter(pMsg)) {
        return TRUE;
    }

    // 나머지 객체 선택 및 배치
    static BOOL bSelectDrag = FALSE;
    switch (pMsg->message) {
    case WM_KEYDOWN: {
        if (pMsg->wParam == 0x2d && pFrm->m_pDummy) {
            switch (pFrm->m_pDummy->m_cePrevType) {
            case DUMMY_POS: {
                pFrm->m_PosDummy.PosSet(pFrm->m_pDummy->Pos());
                pFrm->m_PosDummy.RotSet(pFrm->m_pDummy->Rot());
                pFrm->m_pDummy = &pFrm->m_PosDummy;
            } break;

            case DUMMY_ROT: {
                pFrm->m_RotDummy.PosSet(pFrm->m_pDummy->Pos());
                pFrm->m_RotDummy.RotSet(pFrm->m_pDummy->Rot());
                pFrm->m_pDummy = &pFrm->m_RotDummy;
            } break;

            case DUMMY_SCALE: {
                pFrm->m_ScaleDummy.PosSet(pFrm->m_pDummy->Pos());
                pFrm->m_ScaleDummy.RotSet(pFrm->m_pDummy->Rot());
                pFrm->m_pDummy = &pFrm->m_ScaleDummy;
            } break;

            case DUMMY_SWAP: {
                e_DummyType eDT = pFrm->m_pDummy->GetDummyState();
                pFrm->m_SwappedDummy.PosSet(pFrm->m_pDummy->Pos());
                pFrm->m_SwappedDummy.RotSet(pFrm->m_pDummy->Rot());
                pFrm->m_pDummy = &pFrm->m_SwappedDummy;
                pFrm->m_pDummy->m_cePrevType = eDT;
            } break;
            }
        }
    } break;

    case WM_LBUTTONDOWN: // 객체 선택
    {
        if (!::_IsKeyDown(VK_MENU)) {
            ::SetCapture(pMsg->hwnd);
            bSelectDrag = TRUE;
            m_rcSelDrag.right = m_rcSelDrag.left = point.x;
            m_rcSelDrag.bottom = m_rcSelDrag.top = point.y;
            return TRUE;
        }
    } break;

    case WM_MOUSEMOVE: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (bSelectDrag) {
            m_rcSelDrag.right = point.x;
            m_rcSelDrag.bottom = point.y;
            return TRUE;
        }
    } break;

    case WM_LBUTTONUP: // 객체 선택
    {
        if (bSelectDrag) {
            if (abs(m_rcSelDrag.left - point.x) < 6 && abs(m_rcSelDrag.top - point.y) < 6) // 클릭한걸로 간주
            {
                CPortalVolume * pVol = NULL;
                pVol = pView->m_PVSMgr.PickCollision(point.x, point.y);
                if (pVol) {
                    pView->SelectVolumeByPick(pVol, (GetAsyncKeyState(VK_SHIFT) & 0xff00) ? false : true);
                }
            } else {
                // 드레그 한 것
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
                pView->SelectObjectByDragRect(&m_rcSelDrag, (GetAsyncKeyState(VK_SHIFT) & 0xff00) ? TRUE : FALSE);
            }
            ReleaseCapture();
            bSelectDrag = FALSE;
            m_rcSelDrag.left = m_rcSelDrag.top = m_rcSelDrag.right = m_rcSelDrag.bottom = 0;

            // 위치, 회전값등을 업데이트한다.

            return TRUE;
        }
    } break;
    }

    if (pFrm->m_eState != STATE_EXECUTE) {
        return FALSE;
    }

    return ExecuteModeMsgFilter(pMsg);
}

void CN3IndoorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
    CMainFrame *    pFrm = NULL;
    COrganizeView * pView = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        pView = pFrm->GetOrganizeView();
    }

    // TODO: Add your message handler code here and/or call default
    if (nChar == 0x08 && pFrm->m_eState == STATE_EXECUTE) // BackSpace.. ^^
    {
        if (!m_bExecuteCameraTop) {
            pView->ExecuteCameraChange();
        } else {
            pView->RestoreExecuteCameraChange();
        }

        m_bExecuteCameraTop = !m_bExecuteCameraTop;
    }

    CView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CN3IndoorView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message) {
    // TODO: Add your message handler code here and/or call default
    CMainFrame * pFrm = NULL;
    pFrm = (CMainFrame *)AfxGetMainWnd();
    if (pFrm) {
        switch (pFrm->m_eEditState) {
        case EDIT_SELECT:
            SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_SELECT)));
            return TRUE;
        case EDIT_MOVE:
        case EDIT_ROTATE:
        case EDIT_SCALE:
            SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_TRANSFORM)));
            return TRUE;
        }
    }

    return CView::OnSetCursor(pWnd, nHitTest, message);
}
