// DlgSceneGraph.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "DlgSceneGraph.h"
#include "MainFrm.h"
#include "OrganizeView.h"
#include "ShapeTypeDlg.h"

#include "../N3Base/N3TransformCollision.h"
#include "../N3Base/N3Scene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSceneGraph dialog


CDlgSceneGraph::CDlgSceneGraph(CWnd* pParent /*=NULL*/, DWORD dwFlag)
	: CDialog(CDlgSceneGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSceneGraph)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwFlag = dwFlag;
	m_pSceneRef  = NULL;
}

void CDlgSceneGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSceneGraph)
	DDX_Control(pDX, IDC_TREE_OBJ, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSceneGraph, CDialog)
	//{{AFX_MSG_MAP(CDlgSceneGraph)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_OBJ, OnDblclkTreeObj)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_OBJ, OnRclickTreeObj)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJ, OnSelchangedTreeObj)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgSceneGraph::UpdateTree(CN3Scene* pScene)
{
	m_Tree.DeleteAllItems();

	m_pSceneRef = pScene;

	this->UpdateTreeItem(TVI_ROOT, m_pSceneRef);
	this->ExpandTree(TVI_ROOT);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSceneGraph message handlers

BOOL CDlgSceneGraph::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	HACCEL hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	if (hAccel) TranslateAccelerator(AfxGetMainWnd()->m_hWnd, hAccel, pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgSceneGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSceneGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
	if (pWnd && m_Tree.GetSafeHwnd())
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();

		if (hItem)
		{
			LPARAM lParam = m_Tree.GetItemData(hItem);
			CN3Base* pObj = (CN3Base*)lParam;
			if (pObj)
			{
				((CMainFrame*)AfxGetMainWnd())->RenderObjectToWindow((CN3TransformCollision*)pObj, pWnd->m_hWnd);
			}
			else
			{
				CRect rc;
				pWnd->GetClientRect(&rc);
				pWnd->ClientToScreen(&rc);
				ScreenToClient(&rc);
				dc.FillSolidRect(&rc, RGB(192, 192,192));
			}
		}
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgSceneGraph::RenderObj(CN3TransformCollision* pObj)
{
	CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
	if (pWnd)
	{
		((CMainFrame*)AfxGetMainWnd())->RenderObjectToWindow((CN3TransformCollision*)pObj, pWnd->m_hWnd);
	}
}

void CDlgSceneGraph::OnSelchangedTreeObj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// TODO: Add your control notification handler code here
	RenderObj((CN3TransformCollision* )pNMTreeView->itemNew.lParam);
	
	*pResult = 0;
}

void CDlgSceneGraph::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	const int iPreviewCX = 200;	const int iPreviewCY = 150;
	if(m_Tree.GetSafeHwnd() != NULL)
	{
		int iPreviewTop = cy-iPreviewCY;
		int iPreviewLeft = (cx-iPreviewCX)/2; if (iPreviewLeft<0) iPreviewLeft = 0;
		m_Tree.SetWindowPos(NULL, 0, 0, cx, iPreviewTop, SWP_NOZORDER);
		GetDlgItem(IDC_PREVIEW)->SetWindowPos(NULL, iPreviewLeft, iPreviewTop, iPreviewCX, iPreviewCY, SWP_NOZORDER);
	}		
}

void CDlgSceneGraph::OnDblclkTreeObj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSceneGraph::OnRclickTreeObj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgSceneGraph::UpdateTreeItem(HTREEITEM hParent, CN3Base* pBase)
{
	if(pBase == NULL) return;
	
	DWORD dwType = pBase->Type();
	int nItem = 0;
	if(dwType & OBJ_SCENE) nItem = 0;
	else if(dwType & OBJ_CAMERA) nItem = 1;
	else if(dwType & OBJ_LIGHT) nItem = 2;
	else if(dwType & OBJ_SHAPE) nItem = 3;
	else if(dwType & OBJ_CHARACTER) nItem = 4;
	else if(dwType & OBJ_MESH) nItem = 5;
	else if(dwType & OBJ_MESH_PROGRESSIVE) nItem = 6;
	else if(dwType & OBJ_MESH_INDEXED) nItem = 7;
	else if(dwType & OBJ_JOINT) nItem = 8;
	else if(dwType & OBJ_SKIN) nItem = 9;
	else if(dwType & OBJ_TRANSFORM) nItem = 10;

	HTREEITEM hItem = m_Tree.InsertItem(TVIF_IMAGE |TVIF_SELECTEDIMAGE| TVIF_TEXT, pBase->m_szName.c_str(), nItem, nItem, 0, 0, NULL, hParent, NULL);
	m_Tree.SetItemData(hItem, (DWORD)pBase);

	if(dwType & OBJ_SCENE)
	{
		CString str;
		str.Format("Scene : %s", m_pSceneRef->m_szName.c_str());
		m_Tree.SetItemText(hItem, str);

		int i = 0;
		HTREEITEM hItem2 = NULL;

		if (m_dwFlag & OBJ_SHAPE)
		{
			hItem2 = m_Tree.InsertItem("Shape", hItem);
			int nSC = m_pSceneRef->ShapeCount();
			for(i = 0; i < nSC; i++) this->UpdateTreeItem(hItem2, m_pSceneRef->ShapeGet(i));
			m_Tree.SortChildren(hItem2);
		}
	}
	m_Tree.Expand(hItem, TVE_EXPAND);
}

void CDlgSceneGraph::ExpandTree(HTREEITEM hItem)
{
	if(hItem == NULL) return;

	m_Tree.Expand(hItem, TVE_EXPAND);

	HTREEITEM hChild = m_Tree.GetChildItem(hItem);
	while(hChild != NULL)
	{
		this->ExpandTree(hChild);
		hChild = m_Tree.GetNextItem(hChild, TVGN_NEXT);
	}
}


void CDlgSceneGraph::OnOk() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (!pFrm->m_pSceneSource) return;

	CShapeTypeDlg	dlg; 
	dlg.DoModal();

	if (!dlg.m_bValidate) return;

	HTREEITEM hItem = m_Tree.GetSelectedItem();

	if (hItem)
	{
		LPARAM lParam = m_Tree.GetItemData(hItem);
		CN3Base* pObj = (CN3Base*)lParam;
		if (pObj)
		{
			if (dlg.m_bPartial)
				pFrm->GetOrganizeView()->ShapeRegisterToManager(pObj);
			else
			{
				if ( !pFrm->GetOrganizeView()->IsSelectedPvs() )
				{
					AfxMessageBox("No items or Not Volumn were selected!\n");
					return;
				}

				pFrm->GetOrganizeView()->ShapeLinkToPVolumn(pObj);
			}
		}
	}	
}
