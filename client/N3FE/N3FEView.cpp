// N3FEView.cpp : implementation of the CN3FEView class
//

#include "stdafx.h"
#include "N3FE.h"

#include "MainFrm.h"
#include "N3FEDoc.h"
#include "N3FEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3FEView

IMPLEMENT_DYNCREATE(CN3FEView, CView)

BEGIN_MESSAGE_MAP(CN3FEView, CView)
	//{{AFX_MSG_MAP(CN3FEView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3FEView construction/destruction

CN3FEView::CN3FEView()
{
	// TODO: add construction code here
	m_ptPrev.x = m_ptPrev.y = 0;
	m_vTarget.Set(0,0,50);
}

CN3FEView::~CN3FEView()
{
}

BOOL CN3FEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CN3FEView drawing

void CN3FEView::OnDraw(CDC* pDC)
{
	CN3FEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	pFrm->m_Eng.Clear(0xff606060);

	pFrm->m_Eng.s_lpD3DDev->BeginScene();
	__Matrix44 mtxScale;
	mtxScale.Scale(4,4,4);
	pFrm->m_Eng.RenderGrid(mtxScale);
	pFrm->m_Eng.RenderAxis();

	pFrm->m_Camera.Tick();
	pFrm->m_Camera.Apply();
	pFrm->m_Light.Tick();
	pFrm->m_Light.Apply();

	pDoc->m_EffectMgr.Tick();
	pDoc->m_EffectMgr.Render();

	pFrm->m_Eng.s_lpD3DDev->EndScene();
	pFrm->m_Eng.Present(m_hWnd);

	// 프레임 표시
	DWORD dwTick = GetTickCount();
	static DWORD dwTickPrev = dwTick;
	static CString szFPS;
	if(dwTick > dwTickPrev + 1000) 
	{
		szFPS.Format("%6.2f", pFrm->m_Eng.s_fFrmPerSec);
		dwTickPrev = dwTick;
	}
	pDC->TextOut(0, 0, szFPS);
}

/////////////////////////////////////////////////////////////////////////////
// CN3FEView printing

BOOL CN3FEView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CN3FEView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CN3FEView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CN3FEView diagnostics

#ifdef _DEBUG
void CN3FEView::AssertValid() const
{
	CView::AssertValid();
}

void CN3FEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CN3FEDoc* CN3FEView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CN3FEDoc)));
	return (CN3FEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3FEView message handlers

BOOL CN3FEView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE; // TODO: Add your message handler code here and/or call default
	
	return CView::OnEraseBkgnd(pDC);
}

void CN3FEView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm) pFrm->m_Eng.Reset(TRUE, cx, cy, 0);
}


LRESULT CN3FEView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
	{
		MSG	msg;	msg.hwnd = m_hWnd;	msg.message = message;	msg.wParam = wParam;	msg.lParam = lParam;
		if(pFrm->m_Camera.MoveByWindowMessage(&msg))
		{
			this->Invalidate(FALSE);
			return TRUE;
		}
	}
	
	return CView::WindowProc(message, wParam, lParam);
}

void CN3FEView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
	{
		CRect rc;
		GetClientRect(rc);
		pFrm->m_Eng.Reset(TRUE, rc.Width(), rc.Height(), 0);
	}
}

void CN3FEView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	__Vector3 vPos, vDir;
	::_Convert2D_To_3DCoordinate(point.x, point.y, CN3Base::s_CameraData.mtxView, CN3Base::s_CameraData.mtxProjection, CN3Base::s_CameraData.vp, vPos, vDir);

	float fPlane = 10000.0f;
	__Vector3 vPlane[4];
	vPlane[0].Set(-fPlane, 0, fPlane);
	vPlane[1].Set( fPlane, 0, fPlane);
	vPlane[2].Set(fPlane, 0, -fPlane);
	vPlane[3].Set(-fPlane, 0, -fPlane);

	float fT, fU, fV;
	::_IntersectTriangle(vPos, vDir, vPlane[0], vPlane[1], vPlane[2], fT, fU, fV, &m_vTarget); // 바닥과 충돌을 검출한다..
	::_IntersectTriangle(vPos, vDir, vPlane[0], vPlane[2], vPlane[3], fT, fU, fV, &m_vTarget); // 바닥과 충돌을 검출한다..
	
	CView::OnLButtonDown(nFlags, point);
}
