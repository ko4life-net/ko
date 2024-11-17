// MainFrm.cpp : implementation of the CMainFrame class
//

#include "StdAfx.h"
#include "SkyViewer.h"

#include "SkyViewerView.h"
#include "FormViewProperty.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_COMMAND(ID_IMPORT_OBJECT, OnImportObject)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR, // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() {}

CMainFrame::~CMainFrame() {}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
                               WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
                                   CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1; // fail to create
    }

    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1; // fail to create
    }

    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    CN3Base::PathSet(fs::current_path());

    if (!m_Eng.Init(TRUE, m_hWnd, 64, 64, 0, TRUE)) {
        return -1;
    }
    m_Eng.GridCreate(1000, 1000); // 그리드 만들기..

    m_Camera.m_bFogUse = true;

    D3DCOLORVALUE crLgt = {1.0f, 1.0f, 1.0f, 1.0f};
    m_Lights[0].m_Data.InitDirection(0, __Vector3(0, 0, 1), crLgt);
    m_Lights[1].m_Data.InitDirection(1, __Vector3(0, 0, -1), crLgt);
    m_Lights[2].m_Data.InitPoint(2, __Vector3(0, 0, 0), crLgt, 32.0f);

    m_ObjectBundle.LoadFromFile(fs::path("ChrSelect") / "el_chairs.n3shape"); // 배경으로 쓸 오브젝트 부르기..

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT & cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const {
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext & dc) const {
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext * pContext) {
    CRect rc;
    GetClientRect(rc);
    int nW1 = rc.Width() / 4;

    if (m_wndSplitter.CreateStatic(this, 1, 2) == FALSE) {
        return FALSE;
    }
    if (m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFormViewProperty), CSize(nW1, rc.Height()), pContext) == FALSE) {
        return FALSE;
    }
    if (m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSkyViewerView), CSize(rc.Width() - nW1, rc.Height()), pContext) ==
        FALSE) {
        return FALSE;
    }

    m_wndSplitter.SetColumnInfo(0, nW1, nW1 / 2);

    return TRUE;
}

CSkyViewerView * CMainFrame::GetViewRender() {
    return (CSkyViewerView *)(m_wndSplitter.GetPane(0, 1));
}

CFormViewProperty * CMainFrame::GetViewProperty() {
    return (CFormViewProperty *)(m_wndSplitter.GetPane(0, 0));
}

void CMainFrame::OnImportObject() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, ".N3Shape", NULL, dwFlags, "Shape Data(*.N3Shape)|*.N3Shape||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    fs::path fsFile = dlg.GetPathName().GetString();
    fs::path fsObjPrev = m_ObjectBundle.FilePath();
    m_ObjectBundle.Release();
    if (m_ObjectBundle.LoadFromFile(fsFile)) { // 배경으로 쓸 오브젝트 부르기..
        CSkyViewerView * pView = (CSkyViewerView *)(m_wndSplitter.GetPane(0, 1));
        pView->InvalidateRect(NULL, FALSE);
    } else {
        m_ObjectBundle.LoadFromFile(fsObjPrev);
    }
}
