// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "N3FE.h"
#include "N3FEDoc.h"

#include "DlgRename.h"
#include "DlgBrowsePath.h"
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
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_SET_PATH, OnFileSetPath)
	ON_COMMAND(ID_TOOL_RENAME_FILES, OnToolRenameFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Engine 생성
//	m_Eng.InitEnv();
	if(m_Eng.Init(TRUE, m_hWnd, 64, 64, 0, TRUE) == false) return -1;
	m_Eng.GridCreate(200, 200); // 그리드 만들기..

	// 기본 카메라 & Light 만들기..
	m_Camera.EyePosSet(5, 5, 5);
	D3DCOLORVALUE crLgt = { 1.0f, 1.0f, 1.0f, 0 };
	m_Light.m_Data.InitDirection(0, __Vector3(0, 1, 0), crLgt);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	BOOL bCreate = m_wndSplitter.CreateStatic(this, 2, 1);
	if(bCreate != TRUE) return bCreate;

	CRect rc;
	GetClientRect(rc);
	int nW1 = 352;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CN3FEView), CSize(rc.Width(), rc.Height() / 2), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CFormViewControl), CSize(rc.Width(), rc.Height() / 2), pContext) )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
}

void CMainFrame::OnFileSetPath() 
{
	CDlgBrowsePath dlg;
	
	int nOK = dlg.DoModal();
	if(nOK == IDCANCEL) return;

	CString szPath = dlg.GetPath();
	m_Eng.PathSet(szPath);
	GetPaneControl()->SetDlgItemText(IDC_E_PATH, m_Eng.PathGet());
}

CFormViewControl* CMainFrame::GetPaneControl()
{
	return (CFormViewControl*)(m_wndSplitter.GetPane(1, 0));
}

CN3FEView* CMainFrame::GetPaneRender()
{
	return (CN3FEView*)(m_wndSplitter.GetPane(0, 0));
}

void CMainFrame::OnToolRenameFiles() 
{
	CDlgRename dlg;
	dlg.DoModal();
}
