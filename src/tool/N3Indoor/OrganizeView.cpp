// OrganizeView.cpp : implementation file
//

#include "stdafx.h"
#include "N3Indoor.h"
#include "OrganizeView.h"
#include "DlgBrowsePath.h"
#include "MainFrm.h"
#include "N3IndoorDoc.h"
#include "N3IndoorView.h"

#include "DlgShapeList.h"
#include "PortalVolume.h"

#include "FloorDlg.h"

#include "../N3Base/N3TransformCollision.h"
#include "../N3Base/N3Scene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrganizeView

IMPLEMENT_DYNCREATE(COrganizeView, CFormView)

COrganizeView::COrganizeView()
	: CFormView(COrganizeView::IDD)
{
	//{{AFX_DATA_INIT(COrganizeView)
	//}}AFX_DATA_INIT

	m_GlobalImageList.DeleteImageList();

	m_GlobalImageList.Create(32, 32, ILC_MASK | ILC_COLORDDB, 1, 1000);
	m_GlobalImageList.SetBkColor(ILD_TRANSPARENT);

	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_PVOLUMN));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_SHAPE));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_ZB));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_ZF));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_XL));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_XR));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_YT));
	m_GlobalImageList.Add(AfxGetApp()->LoadIcon(IDI_VOL_YB));

	m_iSelectionIndex = -1;
	m_iSelectionShapeIndex = -1;
	m_iSelectionLinkedShapeIndex = -1;
}

COrganizeView::~COrganizeView()
{
	m_GlobalImageList.DeleteImageList();
}

void COrganizeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrganizeView)
	DDX_Control(pDX, IDC_COMBO_FLOOR, m_ComboFloor);
	DDX_Control(pDX, IDC_LIST_SHAPES, m_ShapesListCtrl);
	DDX_Control(pDX, IDC_LIST_LINKED, m_LinkedListCtrl);
	DDX_Control(pDX, IDC_LIST_LINKTO, m_LinkToListCtrl);
	DDX_Control(pDX, IDC_LIST_SELECT, m_SelectedListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrganizeView, CFormView)
	//{{AFX_MSG_MAP(COrganizeView)
	ON_BN_CLICKED(IDC_BUTTON_RESOURCE_PATH, OnButtonResourcePath)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_COMPILE, OnCompile)
	ON_BN_CLICKED(IDC_ACTIVE, OnActive)
	ON_BN_CLICKED(IDC_STATE_PVOLUMN, OnStatePvolumn)
	ON_BN_CLICKED(IDC_ADD_SHAPE, OnAddShape)
	ON_BN_CLICKED(IDC_ADD_COLLISION, OnAddCollision)
	ON_BN_CLICKED(IDC_BTN_CREATE_VOL, OnBtnCreateVol)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE1, OnBtnCreateGate1)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE2, OnBtnCreateGate2)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE3, OnBtnCreateGate3)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE4, OnBtnCreateGate4)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE5, OnBtnCreateGate5)
	ON_BN_CLICKED(IDC_BTN_CREATE_GATE6, OnBtnCreateGate6)
	ON_BN_CLICKED(IDC_BTN_LINKED_DELETE, OnBtnLinkedDelete)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_SELECT, OnItemchangingListSelect)
	ON_BN_CLICKED(IDC_BUTTON_FILE_NAME, OnButtonFileName)
	ON_BN_CLICKED(IDC_BTN_SHAPE_DELETE, OnBtnShapeDelete)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_SHAPES, OnItemchangingListShapes)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_LINKED, OnItemchangingListLinked)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_SHAPES, OnSetfocusListShapes)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_LINKED, OnSetfocusListLinked)
	ON_BN_CLICKED(IDC_STATE_SHAPE, OnStateShape)
	ON_BN_CLICKED(IDC_INDI_PICK, OnIndiPick)
	ON_BN_CLICKED(IDC_INDI_MOVE, OnIndiMove)
	ON_BN_CLICKED(IDC_INDI_ROTATE, OnIndiRotate)
	ON_BN_CLICKED(IDC_INDI_SCALE, OnIndiScale)
	ON_BN_CLICKED(IDC_VIEW_SELECTED_TOTAL_SHAPE, OnViewSelectedTotalShape)
	ON_EN_CHANGE(IDC_TOTAL_MOVE_X, OnChangeTotalMoveX)
	ON_EN_CHANGE(IDC_TOTAL_MOVE_Y, OnChangeTotalMoveY)
	ON_EN_CHANGE(IDC_TOTAL_MOVE_Z, OnChangeTotalMoveZ)
	ON_BN_CLICKED(IDC_SET_FLOOR, OnSetFloor)
	ON_BN_CLICKED(IDC_DELETE_FLOOR, OnDeleteFloor)
	ON_BN_CLICKED(IDC_CHECK_FLOOR, OnCheckFloor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrganizeView diagnostics

#ifdef _DEBUG
void COrganizeView::AssertValid() const
{
	CFormView::AssertValid();
}

void COrganizeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void COrganizeView::InitEnvironSetting()
{
	CButton *pBut = (CButton *)GetDlgItem(IDC_EDIT);		  pBut->SetCheck(1);
	pBut = (CButton *)GetDlgItem(IDC_STATE_PVOLUMN);	pBut->SetCheck(1);

	SetDlgItemInt(IDC_TOTAL_MOVE_X, 0);
	SetDlgItemInt(IDC_TOTAL_MOVE_Y, 0);
	SetDlgItemInt(IDC_TOTAL_MOVE_Z, 0);
	pBut = (CButton *)GetDlgItem(IDC_INDI_PICK);			  pBut->SetCheck(1);	

	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);
	pBut->EnableWindow(0);	

	pBut = (CButton *)GetDlgItem(IDC_ADD_SHAPE);	  pBut->SetCheck(1);
}

/////////////////////////////////////////////////////////////////////////////
// COrganizeView message handlers

void COrganizeView::OnButtonFileName() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_strResourcePath.empty())
		AfxMessageBox("먼저 리소스 경로를 설정하세여..");

	if (pFrm->m_pSceneSource)
	{
		delete pFrm->m_pSceneSource; 
		pFrm->m_pSceneSource = NULL;
	}

	TCHAR  tch[256]; 
	GetCurrentDirectory(256, tch);
	SetCurrentDirectory(pFrm->m_strResourcePath.c_str()); 
	DWORD dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	CFileDialog dlg(TRUE, "N3Scene File", NULL, dwFlags, "N3Scene (*.n3scene)|*.n3scene||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;

	CString strOpen = dlg.GetFileName();
	pFrm->m_strFileName = strOpen;
	SetDlgItemText(IDC_EDIT_RESOURCE_NAME, strOpen);
	std::string strOp = strOpen;

	SetCurrentDirectory(tch);

	CString strExt = dlg.GetFileExt();	strExt.MakeLower();
	if (strExt != "n3scene" )
	{
		AfxMessageBox("FileType Invalidate!!");
		return;
	}

	pFrm->m_pSceneSource = new CN3Scene;
	pFrm->m_pSceneSource->m_szName = "SourceList";
	pFrm->m_pSceneSource->FileNameSet(strOp);
	pFrm->LoadSourceObjects();
}

void COrganizeView::OnButtonResourcePath() 
{
	// TODO: Add your control notification handler code here
	CDlgBrowsePath dlg;
	if(IDCANCEL == dlg.DoModal()) return;
	
	std::string szPath = dlg.GetPath();
	SetDlgItemText(IDC_EDIT_RESOURCE_PATH, szPath.c_str());	
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	pFrm->m_strResourcePath = szPath+"\\N3Indoor";

	// 경로 설정..
	CN3Base::PathSet(szPath+"\\N3Indoor");
}

void COrganizeView::OnEdit() 
{
	CButton *pBut = (CButton *)GetDlgItem(IDC_EDIT);		  pBut->SetCheck(1);
	pBut = (CButton *)GetDlgItem(IDC_COMPILE);					pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_ACTIVE);					pBut->SetCheck(0);	

	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_VOL);							pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_DELETE);									pBut->EnableWindow(1);	
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE1);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE2);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE3);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE4);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE5);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE6);						pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_LINKED_DELETE);					pBut->EnableWindow(1);
	pBut = (CButton *)GetDlgItem(IDC_BTN_SHAPE_DELETE);						pBut->EnableWindow(1);

	//..
	pBut = (CButton *)GetDlgItem(IDC_INDI_MOVE);							  pBut->EnableWindow(1);	
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);
	if (pFrm->m_eSelectState == SELECT_STATE_PVOLUMN)
		pBut->EnableWindow(0);	
	else
		pBut->EnableWindow(1);	

	pBut = (CButton *)GetDlgItem(IDC_INDI_SCALE);							  pBut->EnableWindow(1);	

	if (pFrm->m_eState == STATE_EDIT)
		return;

	if (pFrm->m_eState == STATE_EXECUTE)
		m_PVSMgr.EndExecuteMode();

	pFrm->m_eState = STATE_EDIT;

	// TODO: Add your control notification handler code here
	if (m_PVSMgr.m_bCompiled)
		m_PVSMgr.Decompile();
}

void COrganizeView::OnCompile() 
{
	CButton *pBut = (CButton *)GetDlgItem(IDC_EDIT);		  pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_COMPILE);					pBut->SetCheck(1);
	pBut = (CButton *)GetDlgItem(IDC_ACTIVE);					pBut->SetCheck(0);	

	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_VOL);							pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_DELETE);									pBut->EnableWindow(0);	
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE1);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE2);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE3);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE4);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE5);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE6);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_LINKED_DELETE);					pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_SHAPE_DELETE);						pBut->EnableWindow(0);

	//..
	pBut = (CButton *)GetDlgItem(IDC_INDI_MOVE);							  pBut->EnableWindow(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);							  pBut->EnableWindow(0);	pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_INDI_SCALE);							  pBut->EnableWindow(0);	

	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_eState == STATE_COMPILE)
		return;

	if (pFrm->m_eState == STATE_EXECUTE)
		m_PVSMgr.EndExecuteMode();

	pFrm->m_eState = STATE_COMPILE;

	// TODO: Add your control notification handler code here
	if (!m_PVSMgr.m_bCompiled)
		m_PVSMgr.DoAllCompile();

	RefreshSelectedList();
	int iCount = m_SelectedListCtrl.GetItemCount();
	if (iCount > 0)
	{
		m_SelectedListCtrl.SetHotItem(0);
		m_iSelectionIndex = 0;
	}
	else
	{
		m_SelectedListCtrl.SetHotItem(-1);
		m_iSelectionIndex = -1;
	}

	RefreshLinkedList();

	CPortalVolume *pVol = NULL;
	pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
	if (!pVol)	return;

	pFrm->GetOrganizeView()->m_PVSMgr.CheckcompileMode(pVol);
}

void COrganizeView::OnActive() 
{
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_eState == STATE_EXECUTE)
		return;

	// TODO: Add your control notification handler code here
	if (!m_PVSMgr.m_bCompiled)
		m_PVSMgr.DoAllCompile();

	CButton *pBut;
	if (!m_PVSMgr.StartExecuteMode())
	{
		pBut = (CButton *)GetDlgItem(IDC_ACTIVE);					pBut->SetCheck(0);	
		return;
	}

	pBut = (CButton *)GetDlgItem(IDC_EDIT);							pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_COMPILE);					pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_ACTIVE);					pBut->SetCheck(1);	

	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_VOL);							pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_DELETE);									pBut->EnableWindow(0);	
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE1);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE2);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE3);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE4);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE5);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_CREATE_GATE6);						pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_LINKED_DELETE);					pBut->EnableWindow(0);
	pBut = (CButton *)GetDlgItem(IDC_BTN_SHAPE_DELETE);						pBut->EnableWindow(0);

	//..
	pBut = (CButton *)GetDlgItem(IDC_INDI_MOVE);							  pBut->EnableWindow(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);							  pBut->EnableWindow(0);	pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_INDI_SCALE);							  pBut->EnableWindow(0);

	pFrm->m_eState = STATE_EXECUTE;
}

void COrganizeView::OnStatePvolumn() 
{
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_eSelectState == SELECT_STATE_PVOLUMN)
		return;

	pFrm->m_eSelectState = SELECT_STATE_PVOLUMN;
	CButton *pBut = (CButton *)GetDlgItem(IDC_STATE_PVOLUMN);		  pBut->SetCheck(1);
	pBut = (CButton *)GetDlgItem(IDC_STATE_SHAPE);					pBut->SetCheck(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);		  pBut->EnableWindow(0);	pBut->SetCheck(0);

	// TODO: Add your control notification handler code here
}

void COrganizeView::OnStateShape() 
{
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_eSelectState == SELECT_STATE_SHAPE)
		return;

	pFrm->m_eSelectState = SELECT_STATE_SHAPE;
	CButton *pBut = (CButton *)GetDlgItem(IDC_STATE_PVOLUMN);		  pBut->SetCheck(0);
	pBut = (CButton *)GetDlgItem(IDC_STATE_SHAPE);					pBut->SetCheck(1);	

	//..
	if (pFrm->m_eState == STATE_EDIT)
	{
		pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);							  
		pBut->EnableWindow(1);	
	}

	// TODO: Add your control notification handler code here
}

void COrganizeView::OnAddShape() 
{
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();

	CButton *pBut = (CButton *)GetDlgItem(IDC_ADD_SHAPE);		  
	if (pBut->GetCheck())
		pFrm->m_dwRenderingOption |= dw_Render_Add_Shape;
	else
		pFrm->m_dwRenderingOption &= ~dw_Render_Add_Shape;

	// TODO: Add your control notification handler code here	
}

void COrganizeView::OnAddCollision() 
{
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();

	CButton *pBut = (CButton *)GetDlgItem(IDC_ADD_COLLISION);		  
	if (pBut->GetCheck())
		pFrm->m_dwRenderingOption |= dw_Render_Add_Collision;
	else
		pFrm->m_dwRenderingOption &= ~dw_Render_Add_Collision;

	// TODO: Add your control notification handler code here
}

void COrganizeView::ShapeRegisterToManager(CN3Base* pBase)
{
	if (!(pBase->Type() & OBJ_SHAPE))
		return;
	
	m_PVSMgr.RegisterShape(((CN3Shape* )pBase)->FileName(), ((CN3Shape* )pBase));
}

void COrganizeView::ShapeLinkToPVolumn(CN3Base* pBase)
{
	if (!(pBase->Type() & OBJ_SHAPE))
		return;

	CPortalVolume *pVol = NULL;
	pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
	if (!pVol)
	{
		AfxMessageBox("No items were selected!\n");
		return;
	}
	
	pVol->SetShape(((CN3Shape* )pBase)->FileName(), ((CN3Shape* )pBase), m_PVSMgr.m_iIncreseShapeIndex++);
}

void COrganizeView::OnCurserSelect(e_EditMode eED)
{
	CPortalVolume *pVol = NULL;
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	CButton *pBut = NULL;
	pBut = (CButton *)GetDlgItem(IDC_INDI_PICK);								  pBut->SetCheck(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_MOVE);							  pBut->SetCheck(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);							  pBut->SetCheck(0);	
	pBut = (CButton *)GetDlgItem(IDC_INDI_SCALE);							  pBut->SetCheck(0);	

	// TODO: Add your control notification handler code here
	pFrm->m_eEditState = eED;

	switch (eED)
	{
		case EDIT_SELECT:
			pBut = (CButton *)GetDlgItem(IDC_INDI_PICK);							  
			pBut->SetCheck(1);	
			SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_SELECT)));
			if (pFrm->m_pDummy)
			{
				pFrm->m_pDummy->ClearObjs();
				pFrm->m_pDummy = NULL;
			}
			pFrm->m_SelVolArray.RemoveAll();
			pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
			if (!pVol)	return;

			pFrm->m_SelVolArray.Add(pVol);
			break;

		case EDIT_MOVE:
			pBut = (CButton *)GetDlgItem(IDC_INDI_MOVE);							  
			pBut->SetCheck(1);	
			SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_TRANSFORM)));
			if (pFrm->m_pDummy)
				pFrm->m_pDummy->ClearObjs();
			pFrm->m_pDummy = &pFrm->m_PosDummy;
			pFrm->m_SelVolArray.RemoveAll();
			break;

		case EDIT_ROTATE:
			pBut = (CButton *)GetDlgItem(IDC_INDI_ROTATE);							  
			pBut->SetCheck(1);	
			SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_TRANSFORM)));
			if (pFrm->m_pDummy)
				pFrm->m_pDummy->ClearObjs();
			pFrm->m_pDummy = &pFrm->m_RotDummy;
			pFrm->m_SelVolArray.RemoveAll();
			break;

		case EDIT_SCALE:
			pBut = (CButton *)GetDlgItem(IDC_INDI_SCALE);							  
			pBut->SetCheck(1);	
			SetCursor(LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_TRANSFORM)));
			if (pFrm->m_pDummy)
				pFrm->m_pDummy->ClearObjs();
			pFrm->m_pDummy = &pFrm->m_ScaleDummy;
			pFrm->m_SelVolArray.RemoveAll();
			break;
	}
}

void COrganizeView::OnBtnCreateVol() 
{
	// TODO: Add your control notification handler code here
	m_PVSMgr.InsertNewPortalVolumn();
	RefreshSelectedList();
	RefreshLinkToList();
	int iCount = m_SelectedListCtrl.GetItemCount();
	m_SelectedListCtrl.SetHotItem(iCount-1);
	m_iSelectionIndex = iCount-1;
	RefreshSelectedList();
	RefreshLinkToList();	
	RefreshLinkedList();

	int iOrder = m_SelectedListCtrl.GetHotItem();
	if (iOrder == -1)
		return;

	CPortalVolume *pVol = NULL;
	pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
	if (!pVol) return;

	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	__Vector3 vec = pVol->GetCentorPoint();
	float fMag = (pFrm->m_Camera.EyePos() - pFrm->m_Camera.AtPos()).Magnitude();
	__Vector3 vDir = pFrm->m_Camera.Dir();

	pFrm->m_Camera.EyePosSet(vec-vDir*fMag);
	pFrm->m_Camera.AtPosSet(vec);
	pFrm->m_Camera.Apply();
}

void COrganizeView::RefreshSelectedList()
{
	m_SelectedListCtrl.DeleteAllItems();
	m_SelectedListCtrl.SetImageList(&m_GlobalImageList, LVSIL_NORMAL);

	iter it = m_PVSMgr.m_pPvsList.begin();
	while(it != m_PVSMgr.m_pPvsList.end())
	{
		CPortalVolume* pVol = *it++;
		m_SelectedListCtrl.InsertItem(pVol->m_iID, pVol->m_strID.c_str(), 0 );		
	}
}

void COrganizeView::RefreshLinkToList()
{
	m_LinkToListCtrl.DeleteAllItems();
	m_LinkToListCtrl.SetImageList(&m_GlobalImageList, LVSIL_NORMAL);

	iter it = m_PVSMgr.m_pPvsList.begin();
	while(it != m_PVSMgr.m_pPvsList.end())
	{
		CPortalVolume* pVol = *it++;
		m_LinkToListCtrl.InsertItem(pVol->m_iID, pVol->m_strID.c_str(), 0);		
	}
}

void COrganizeView::RefreshLinkedList()
{
	m_LinkedListCtrl.DeleteAllItems();
	m_LinkedListCtrl.SetImageList(&m_GlobalImageList, LVSIL_NORMAL);

	CPortalVolume *pVol;
	WVOL Linked;
	int iOrder = m_SelectedListCtrl.GetHotItem();
	if (iOrder == -1)
		return;

	pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
	if (!pVol) return;

	SetDlgItemText(IDC_SELECT_VOL_TEXT, pVol->m_strID.c_str());

	std::string str; 
	witer it = pVol->m_VoltList.begin();
	while(it != pVol->m_VoltList.end())
	{
		Linked = *it++;
		switch (Linked.ePWT)
		{
		case PWALL_ZB:
			str = "ZB";
			break;
		case PWALL_ZF:
			str = "ZF";
			break;
		case PWALL_XL:
			str = "XL";
			break;
		case PWALL_XR:
			str = "XR";
			break;
		case PWALL_YT:
			str = "YT";
			break;
		case PWALL_YB:
			str = "YB";
			break;
		}
		str += Linked.pPvsVol->m_strID;
				
		m_LinkedListCtrl.InsertItem(Linked.pPvsVol->m_iID, str.c_str(), (int)(Linked.ePWT)+2);				
	}

	int iCount = m_LinkedListCtrl.GetItemCount();		

	ShapeInfo* pSI;
	std::vector<char> buffer(32, NULL);
	siiter siit = pVol->m_plShapeInfoList.begin();
	while(siit != pVol->m_plShapeInfoList.end())
	{
		pSI = *siit++;
		sprintf(buffer.begin(), "Shape_%d", pSI->m_iID);
		str = buffer.begin();
		m_LinkedListCtrl.InsertItem(iCount++, str.c_str(), 1);						
	}
}

void COrganizeView::OnBtnDelete() 
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	// TODO: Add your control notification handler code here
	CPortalVolume *pVol = NULL;
	int iOrder = m_SelectedListCtrl.GetHotItem();
	if ( iOrder != -1 )
	{
		pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
		if (!pVol)	return;

		RefreshFloorInfoAll(pVol->m_iID);
		m_PVSMgr.DeletePortalVolLinkedByiID(pVol->m_iID);
		m_PVSMgr.DeletePortalVolByiOrder(iOrder);
		pFrm->TotalValidateCheckAfterDelete();

		RefreshSelectedList();
		RefreshLinkToList();
		m_SelectedListCtrl.SetHotItem(-1);
		m_iSelectionIndex = -1;
		RefreshLinkedList();
	}
}

void COrganizeView::OnItemchangingListSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame* pFrm = NULL;
	CPortalVolume *pVol = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_iSelectionIndex != pNMListView->iItem)
	{
		m_iSelectionIndex = pNMListView->iItem;
		m_SelectedListCtrl.SetHotItem(m_iSelectionIndex);
		RefreshLinkedList();

		pVol = NULL;
		pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
		if (!pVol)	return;

		// Compile Mode면..
		if (pFrm->m_eState == STATE_COMPILE)
			pFrm->GetOrganizeView()->m_PVSMgr.CheckcompileMode(pVol);

		int iVolCount = pVol->GetLinkedVolumeCount();		
		int iShapeCount = pVol->GetLinkedShapeCount();
		int iTotalCount = iVolCount + iShapeCount;

		if (m_iSelectionLinkedShapeIndex >= iTotalCount)
		{
			m_iSelectionLinkedShapeIndex = iTotalCount-1;
			m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
		}
	}

	OnStatePvolumn();

	if (pFrm->m_pDummy)
	{
		SelectElement se;
		se.eST = TYPE_VOLUME_ONLY;
		se.pSelectPointer = m_PVSMgr.GetPortalVolByiOrder(m_iSelectionIndex);
		pFrm->m_pDummy->SetSelObj(se);
	}

	pFrm->m_LastSelectedElement.eST = TYPE_VOLUME_ONLY;
	pFrm->m_LastSelectedElement.pSelectPointer = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());

	pFrm->m_SelVolArray.RemoveAll();
	pFrm->TotalValidateCheckAfterDelete();

	*pResult = 0;
}

void COrganizeView::SelectVolumeByPick(CPortalVolume* pVol, bool bOne)
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();
	SelectElement se;
	CPortalVolume* pVol2 = NULL;

	pFrm->m_LastSelectedElement.eST = TYPE_VOLUME_TOTAL;
	pFrm->m_LastSelectedElement.pSelectPointer = pVol;

	OnStatePvolumn();
	
	if (pFrm->m_eEditState != EDIT_SELECT)
	{
		se.eST = TYPE_VOLUME_TOTAL;
		se.pSelectPointer = pVol;

		__ASSERT(pFrm->m_pDummy, "Oops!! -.-;");
		pFrm->m_pDummy->SetSelObj(se, bOne);

		//  만약 Dummy Size가 0보다 크고.. pVol이 없으면..
		if ( (pFrm->m_pDummy->m_SelObjArray.GetSize() > 0) && !pFrm->m_pDummy->IsExistTotalVolByPointer(pVol) )
		{
			pVol2 = pFrm->m_pDummy->GetFirstElementTotalVol();
			if (pVol2) 
				pVol = pVol2;
		}

		int iIndex = m_PVSMgr.GetIndexByVolumePointer(pVol);

		m_iSelectionIndex = iIndex;
		m_SelectedListCtrl.SetHotItem(m_iSelectionIndex);
		RefreshLinkedList();

		// Compile Mode면..
		if (pFrm->m_eState == STATE_COMPILE)
			pFrm->GetOrganizeView()->m_PVSMgr.CheckcompileMode(pVol);

		int iVolCount = pVol->GetLinkedVolumeCount();		
		int iShapeCount = pVol->GetLinkedShapeCount();
		int iTotalCount = iVolCount + iShapeCount;

		if (m_iSelectionLinkedShapeIndex >= iTotalCount)
		{
			m_iSelectionLinkedShapeIndex = iTotalCount-1;
			m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
		}
	}
	else
	{
		// Compile Mode면..
		if (pFrm->m_eState == STATE_COMPILE)
			pFrm->GetOrganizeView()->m_PVSMgr.CheckcompileMode(pVol);

		bool bFound = false;

		if (bOne)
			pFrm->m_SelVolArray.RemoveAll();
		else
		{
			// 이미 있으면 추가하지 않는다..
LOOP_1:
			int iSize = pFrm->m_SelVolArray.GetSize();
			for ( int i = 0; i < iSize; i++ )
			{
				if (pFrm->m_SelVolArray[i] == pVol)
				{
					// 이미 있으므로 선택목록에서 제거
					pFrm->m_SelVolArray.RemoveAt(i);
					bFound = true;
					goto LOOP_1;
				}
			}
		}
		if (!bFound)
			pFrm->m_SelVolArray.Add(pVol);
	
		bFound = false;

		int iSize = pFrm->m_SelVolArray.GetSize();
		if (iSize > 0)
		{
			for (int i = 0; i < iSize; i++)
			{
				if (pVol == pFrm->m_SelVolArray.GetAt(i))
					bFound = true;
			}
		}

		if (pFrm->m_SelVolArray.GetSize() < 1)
		{
			m_iSelectionIndex = -1;
			m_SelectedListCtrl.SetHotItem(m_iSelectionIndex);
			RefreshLinkedList();
			m_iSelectionLinkedShapeIndex = -1;
			m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
			return;
		}

		if (!bFound)
			pVol = pFrm->m_SelVolArray.GetAt(0);

		int iIndex = m_PVSMgr.GetIndexByVolumePointer(pVol);

		m_iSelectionIndex = iIndex;
		m_SelectedListCtrl.SetHotItem(m_iSelectionIndex);
		RefreshLinkedList();

		int iVolCount = pVol->GetLinkedVolumeCount();		
		int iShapeCount = pVol->GetLinkedShapeCount();
		int iTotalCount = iVolCount + iShapeCount;

		if (m_iSelectionLinkedShapeIndex >= iTotalCount)
		{
			m_iSelectionLinkedShapeIndex = iTotalCount-1;
			m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
		}
	}
}

void COrganizeView::SelectObjectByDragRect(RECT* pRect, BOOL bAdd)	// 드래그 해서 객체 선택하기
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	CN3Eng* pEng = &pFrm->m_Eng;
	LPDIRECT3DDEVICE8 pD3DDev = pEng->s_lpD3DDev;

	__Matrix44 matView, matProj, matVP;
	pD3DDev->GetTransform(D3DTS_VIEW, &matView);
	pD3DDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixMultiply(&matVP, &matView, &matProj);

	// 나중에 ^^..



}

void COrganizeView::RefreshTotalShape()
{
	m_ShapesListCtrl.DeleteAllItems();
	m_ShapesListCtrl.SetImageList(&m_GlobalImageList, LVSIL_NORMAL);

	int iCount = 0;
	ShapeInfo* pSI;
	std::string str; 
	std::vector<char> buffer(32, NULL);
	siiter siit = m_PVSMgr.m_plShapeInfoList.begin();
	while(siit != m_PVSMgr.m_plShapeInfoList.end())
	{
		pSI = *siit++;
		sprintf(buffer.begin(), "Part_%d", pSI->m_iID);
		str = buffer.begin();
		m_ShapesListCtrl.InsertItem(iCount++, str.c_str(), 1);						
	}
}

void COrganizeView::OnItemchangingListLinked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	CPortalVolume *pVol = NULL;
	int iOrder = m_SelectedListCtrl.GetHotItem();
	if (iOrder == -1)
	{
		*pResult = 0;
		return;
	}

	pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
	if (!pVol) 
	{
		*pResult = 0;
		return;
	}

	int iCount = pVol->GetLinkedVolumeCount();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_iSelectionLinkedShapeIndex != pNMListView->iItem)
	{
		m_iSelectionLinkedShapeIndex = pNMListView->iItem;
		m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
	}

	int iIndex = m_iSelectionLinkedShapeIndex - iCount;
	if (iIndex >= 0)
	{
		OnStateShape();

		CMainFrame* pFrm = NULL;
		pFrm = (CMainFrame*)AfxGetMainWnd();
		if (pFrm->m_pDummy)
		{
			SelectElement se;
			se.eST = TYPE_SHAPE_ONLY;
			se.pSelectPointer = pVol->GetNthLinkedShapeInfo(iIndex);
			if (se.pSelectPointer)
				pFrm->m_pDummy->SetSelObj(se);
		}

		pFrm->m_LastSelectedElement.eST = TYPE_SHAPE_ONLY;
		pFrm->m_LastSelectedElement.pSelectPointer = pVol->GetNthLinkedShapeInfo(iIndex);

		pFrm->UpdateShapeInfoDisplay();
	}
	else
	{
		pFrm->m_LastSelectedElement.eST = TYPE_VOLUME_ONLY;
		pFrm->m_LastSelectedElement.pSelectPointer = pVol->GetNthLinkedVolume(m_iSelectionLinkedShapeIndex);
	}

	pFrm->m_SelVolArray.RemoveAll();

	*pResult = 0;
}

void COrganizeView::OnItemchangingListShapes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_iSelectionShapeIndex != pNMListView->iItem)
	{
		m_iSelectionShapeIndex = pNMListView->iItem;
		m_ShapesListCtrl.SetHotItem(m_iSelectionShapeIndex);
	}

	OnStateShape();

	if (pFrm->m_pDummy)
	{
		SelectElement se;
		se.eST = TYPE_SHAPE_ONLY;
		se.pSelectPointer = GetNthShapeInfo(m_iSelectionShapeIndex);
		pFrm->m_pDummy->SetSelObj(se);
	}

	pFrm->m_LastSelectedElement.eST = TYPE_SHAPE_ONLY;
	pFrm->m_LastSelectedElement.pSelectPointer = GetNthShapeInfo(m_iSelectionShapeIndex);

	pFrm->m_SelVolArray.RemoveAll();
	pFrm->UpdateShapeInfoDisplay();

	*pResult = 0;
}

ShapeInfo* COrganizeView::GetNthShapeInfo(int iOrder)
{
	ShapeInfo* pSI;
	int iCount = 0;
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	siiter siit = m_PVSMgr.m_plShapeInfoList.begin();
	while(siit != m_PVSMgr.m_plShapeInfoList.end())
	{
		pSI = *siit++;
		if (iCount == iOrder)
			return pSI;
		iCount++;
	}

	return NULL;
}

void COrganizeView::OnSetfocusListShapes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iIndex = m_ShapesListCtrl.GetHotItem();

	ShapeInfo* pSI = NULL;
	int iCount = 0;
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	siiter siit = m_PVSMgr.m_plShapeInfoList.begin();
	while(siit != m_PVSMgr.m_plShapeInfoList.end())
	{
		pSI = *siit++;
		if (iCount == iIndex)
			break;
		iCount++;
	}

	pFrm->m_LastSelectedElement.eST = TYPE_SHAPE_ONLY;
	pFrm->m_LastSelectedElement.pSelectPointer = pSI;
	
	*pResult = 0;
}

void COrganizeView::OnSetfocusListLinked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPortalVolume *pVol = NULL;
	int iOrder = m_SelectedListCtrl.GetHotItem();
	if (iOrder == -1)
	{
		*pResult = 0;
		return;
	}

	pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
	if (!pVol) 
	{
		*pResult = 0;
		return;
	}

	*pResult = 0;
}

void COrganizeView::OnBtnCreateGate1() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_ZB);
}

void COrganizeView::OnBtnCreateGate2() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_ZF);
}

void COrganizeView::OnBtnCreateGate3() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_XL);
}

void COrganizeView::OnBtnCreateGate4() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_XR);
}

void COrganizeView::OnBtnCreateGate5() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_YB);
}

void COrganizeView::OnBtnCreateGate6() 
{
	// TODO: Add your control notification handler code here
	OnCreatePortalGateWrapper(PWALL_YT);
}

void COrganizeView::OnCreatePortalGateWrapper(e_PvsWallType ePWT)
{
	CPortalVolume *pVolFrom = NULL, *pVolTo = NULL;	
	pVolFrom = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
	if (!pVolFrom)
	{
		AfxMessageBox("No items were selected!\n");		
		return;
	}

	POSITION pos = m_LinkToListCtrl.GetFirstSelectedItemPosition();
	int nItem = m_LinkToListCtrl.GetNextSelectedItem(pos);
	pVolTo = m_PVSMgr.GetPortalVolByiOrder(nItem);
	if (!pVolTo)
	{
		AfxMessageBox("No items were selected!\n");		
		return;
	}		

	WVOL wvolFrom, wvolTo;
	wvolFrom.ePWT = ePWT;
	wvolFrom.pPvsVol = pVolTo;

	switch (ePWT)
	{
		case PWALL_ZB:
			wvolTo.ePWT = PWALL_ZF;
			break;
		case PWALL_ZF:
			wvolTo.ePWT = PWALL_ZB;
			break;
		case PWALL_XL:
			wvolTo.ePWT = PWALL_XR;
			break;
		case PWALL_XR:
			wvolTo.ePWT = PWALL_XL;
			break;
		case PWALL_YT:
			wvolTo.ePWT = PWALL_YB;
			break;
		case PWALL_YB:
			wvolTo.ePWT = PWALL_YT;
			break;
	}
	wvolTo.pPvsVol = pVolFrom;

	pVolFrom->m_VoltList.push_back(wvolFrom);
	pVolTo->m_VoltList.push_back(wvolTo);

	RefreshLinkedList();

	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	__Vector3 vec = pVolFrom->GetCentorPoint();
	float fMag = (pFrm->m_Camera.EyePos() - pFrm->m_Camera.AtPos()).Magnitude();
	__Vector3 vDir = pFrm->m_Camera.Dir();

	pFrm->m_Camera.EyePosSet(vec-vDir*fMag);
	pFrm->m_Camera.AtPosSet(vec);
	pFrm->m_Camera.Apply();
}

void COrganizeView::OnBtnLinkedDelete() 
{
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();

	POSITION pos = m_LinkedListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("No items were selected!\n");
		return;
	} 
	else
	{
		CPortalVolume *pVol = NULL;
		pVol = 	m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
		if (!pVol)
		{
			AfxMessageBox("No items were selected!\n");
			return;
		}

		int nItem;
	    while (pos)
		{
			nItem = m_LinkedListCtrl.GetNextSelectedItem(pos);
			// you could do your own processing on nItem here
			int iCount = pVol->GetLinkedVolumeCount();

			if (nItem - iCount < 0)
				pVol->DeleteLinkByiOrder(nItem);
			else
				pVol->DeleteLinkedShapeByiOrder(nItem - iCount);
		}
	}

	pFrm->TotalValidateCheckAfterDelete();
	RefreshLinkedList();		
}

bool COrganizeView::IsSelectedPvs()
{
	CPortalVolume *pVol = NULL;
	pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
	if (!pVol)
		return false;

	return true;
}

bool COrganizeView::QueryHeight(float fx, float fz, float& fy)
{
	if (!m_PVSMgr.m_pCurVol)
	{
		AfxMessageBox("No Current Volume???\n");
		return false;
	}

	if (!m_PVSMgr.m_pCurVol->HasCollisionData())
	{
		AfxMessageBox("No Collision Data.. May it not be Compiled???\n");
		return false;
	}

	return m_PVSMgr.m_pCurVol->QueryHeight(fx, fz, fy);	
}

void COrganizeView::ExecuteCameraChange()
{
	m_PVSMgr.ExecuteCameraChange();
}

void COrganizeView::RestoreExecuteCameraChange()
{
	m_PVSMgr.RestoreExecuteCameraChange();
}

void COrganizeView::TickRender()
{
	m_PVSMgr.m_iCurIndex = m_SelectedListCtrl.GetHotItem();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
	{
		switch (pFrm->m_eState)
		{
			case STATE_EDIT:
				m_PVSMgr.TickEdit();
				m_PVSMgr.RenderEdit();
				break;
			case STATE_COMPILE:
				m_PVSMgr.TickCompile();
				m_PVSMgr.RenderCompile();
				break;
			case STATE_EXECUTE:
				m_PVSMgr.TickExecute();
				m_PVSMgr.RenderExecute();
				break;
		}
	}
}

bool COrganizeView::OnFileNew() 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (!pFrm) return false;

	// 기존 작업데이터가 있는자 살펴본다..
	if ((m_PVSMgr.m_iTotalCount != 0) || (m_PVSMgr.m_plShapeInfoList.size() > 0))
	{
		// 지울것인지 물어본다..
		if (AfxMessageBox("작업중인 데이터가 삭제됩니다. 계속하시겠습니까?", MB_OKCANCEL|MB_APPLMODAL) == IDCANCEL)
			return false;

		// 지운다 -.-;
		m_PVSMgr.DeleteAllPvsObj();
		RefreshSelectedList();
		RefreshLinkToList();
		m_SelectedListCtrl.SetHotItem(-1);
		m_iSelectionIndex = -1;
		m_iSelectionShapeIndex = -1;
		m_iSelectionLinkedShapeIndex = -1;
		RefreshLinkedList();
		RefreshTotalShape();
		m_PVSMgr.m_bCompiled = false;

		FloorInfo FInfo;
		vfiter vfit = pFrm->m_FloorList.begin();
		while (vfit != pFrm->m_FloorList.end())
		{
			FInfo = *vfit++;
			FInfo.m_vVolume.clear();
		}
		pFrm->m_FloorList.clear();
		m_ComboFloor.ResetContent();
	}

	// 카메라등등 초기화..	
	if(pFrm->m_pDummy)
		pFrm->m_pDummy->ClearObjs();
	pFrm->m_SelVolArray.RemoveAll();
	pFrm->m_LastSelectedElement.pSelectPointer = NULL;

	pFrm->OutputDlgRefresh();

	OnIndiPick();	

	return true;
}

void COrganizeView::OnFileWorkshopOpen() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	if (!OnFileNew())
		return;

	DWORD dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	CFileDialog dlg(TRUE, "N3Interior Workshop File", NULL, dwFlags, "Workshop File(*.wshop)|*.wshop||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;

	CString strOpen = dlg.GetPathName(), strExt = dlg.GetFileExt();
	if (strExt.CompareNoCase("wshop") != 0) return;
	std::string str = strOpen;

	OpenWSFileInternal(str);
	AfxGetApp()->AddToRecentFileList(str.c_str());
	CN3IndoorApp* pApp = (CN3IndoorApp*)AfxGetApp();
	pApp->UpdateMRU();
}

void COrganizeView::OnFileOpenGamedata() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	if (!OnFileNew())
		return;

	DWORD dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	CFileDialog dlg(TRUE, "N3Interior Game Data File", NULL, dwFlags, "N3Indoor File(*.n3indoor)|*.n3indoor||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;

	CString strOpen = dlg.GetPathName(), strExt = dlg.GetFileExt();
	if (strExt.CompareNoCase("n3indoor") != 0) return;
	std::string str = strOpen;

	OpenGDFileInternal(str);
	AfxGetApp()->AddToRecentFileList(str.c_str());
	CN3IndoorApp* pApp = (CN3IndoorApp*)AfxGetApp();
	pApp->UpdateMRU();
}

void COrganizeView::OpenWorkShopFile(std::string strFile)
{
	if (!OnFileNew())
		return;

	OpenWSFileInternal(strFile);
	AfxGetApp()->AddToRecentFileList(strFile.c_str());
	CN3IndoorApp* pApp = (CN3IndoorApp*)AfxGetApp();
	pApp->UpdateMRU();
}

void COrganizeView::OpenGameDataFile(std::string strFile)
{
	if (!OnFileNew())
		return;

	OpenGDFileInternal(strFile);
	AfxGetApp()->AddToRecentFileList(strFile.c_str());
	CN3IndoorApp* pApp = (CN3IndoorApp*)AfxGetApp();
	pApp->UpdateMRU();
}

void COrganizeView::OpenWSFileInternal(std::string strFile)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	m_PVSMgr.m_bGameData = false;
	m_PVSMgr.LoadFromFile(strFile);

	RefreshSelectedList();
	RefreshLinkToList();
	int iCount = m_SelectedListCtrl.GetItemCount();
	if (iCount > 0)
	{
		m_SelectedListCtrl.SetHotItem(0);
		m_iSelectionIndex = 0;
		CPortalVolume* pVol = m_PVSMgr.GetPortalVolByiOrder(0);
		if (pVol)
			pFrm->m_SelVolArray.Add(pVol);
	}
	else
	{
		m_SelectedListCtrl.SetHotItem(-1);
		m_iSelectionIndex = -1;
	}

	RefreshLinkedList();
	RefreshTotalShape();
	m_iSelectionShapeIndex = -1;
	m_iSelectionLinkedShapeIndex = -1;

	UpdateData(FALSE);	

	std::string strT;
	char szFName[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(strFile.c_str(), NULL, NULL, szFName, szExt);
	strT = szFName;	strT += szExt;

	pFrm->SetWindowText(strT.c_str());
	if (pFrm->m_pDlgOutputList && pFrm->m_pDlgOutputList->IsWindowVisible()	)
		pFrm->m_pDlgOutputList->UpdateTree();
}

void COrganizeView::OpenGDFileInternal(std::string strFile)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	m_PVSMgr.m_bGameData = true;
	m_PVSMgr.LoadFromFile(strFile);
	m_PVSMgr.m_bCompiled = true;

	RefreshSelectedList();
	RefreshLinkToList();
	int iCount = m_SelectedListCtrl.GetItemCount();
	if (iCount > 0)
	{
		m_SelectedListCtrl.SetHotItem(0);
		m_iSelectionIndex = 0;
		CPortalVolume* pVol = m_PVSMgr.GetPortalVolByiOrder(0);
		if (pVol)
			pFrm->m_SelVolArray.Add(pVol);
	}
	else
	{
		m_SelectedListCtrl.SetHotItem(-1);
		m_iSelectionIndex = -1;
	}
	RefreshLinkedList();
	RefreshTotalShape();
	m_iSelectionShapeIndex = -1;
	m_iSelectionLinkedShapeIndex = -1;

	UpdateData(FALSE);		

	CPortalVolume *pVol = NULL;
	pVol = m_PVSMgr.GetPortalVolByiOrder(m_SelectedListCtrl.GetHotItem());
	if (!pVol)	return;

	if (pFrm->m_eState == STATE_COMPILE)
		pFrm->GetOrganizeView()->m_PVSMgr.CheckcompileMode(pVol);

	std::string strT;
	char szFName[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(strFile.c_str(), NULL, NULL, szFName, szExt);
	strT = szFName;	strT += szExt;

	pFrm->SetWindowText(strT.c_str());
	if (pFrm->m_pDlgOutputList && pFrm->m_pDlgOutputList->IsWindowVisible()	)
		pFrm->m_pDlgOutputList->UpdateTree();
}

void COrganizeView::OnFileSaveWorkshop() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_strFileName.size() <= 0)
	{
		AfxMessageBox("SourceList가  없습니다..Data는 저장되지 않을것 입니다..");
		return;
	}

	if (!GetDlgItemInt(IDC_TOTAL_MOVE_X) || !GetDlgItemInt(IDC_TOTAL_MOVE_X) || !GetDlgItemInt(IDC_TOTAL_MOVE_Z))
	{
		AfxMessageBox("Map Offset이 0 입니다.Data는 저장되지 않을것 입니다..");
		return;
	}

	DWORD dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	CFileDialog dlg(FALSE, "N3Interior Workshop File", NULL, dwFlags, "Workshop File(*.wshop)|*.wshop||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;

	CString strSave = dlg.GetPathName();
	std::string str = strSave;

	m_PVSMgr.m_bGameData = false;
	m_PVSMgr.SaveToFile(str);
}

void COrganizeView::OnFileSaveGamedata() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_strFileName.size() <= 0)
	{
		AfxMessageBox("SourceList가  없습니다..Data는 저장되지 않을것 입니다..");
		return;
	}

	if (!GetDlgItemInt(IDC_TOTAL_MOVE_X) || !GetDlgItemInt(IDC_TOTAL_MOVE_X) || !GetDlgItemInt(IDC_TOTAL_MOVE_Z))
	{
		AfxMessageBox("Map Offset이 0 입니다.Data는 저장되지 않을것 입니다..");
		return;
	}

	// TODO: Add your command handler code here
	DWORD dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	CFileDialog dlg(FALSE, "N3Interior Game Data File", NULL, dwFlags, "N3Indoor File(*.n3indoor)|*.n3indoor||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;
	
	CString strSave = dlg.GetPathName();
	std::string str = strSave;

	if (!m_PVSMgr.m_bCompiled)
		m_PVSMgr.DoAllCompile();

	m_PVSMgr.m_bGameData = true;
	m_PVSMgr.SaveToFile(str);	
}

////////////////////////////////////////////////////////////////////////
void COrganizeView::OnFileServerData()
{
	// 기존 작업데이터가 있는자 살펴본다..
	if (!(m_PVSMgr.m_iTotalCount && m_PVSMgr.m_plShapeInfoList.size()))
	{
		AfxMessageBox("Data가  없습니다..Data는 저장되지 않을것 입니다..");
		return;
	}

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(FALSE, "smd", NULL, dwFlags, "Server Map Data file(*.smd)|*.smd||", NULL);
	if(dlg.DoModal() == IDCANCEL) return;

	CString strOpen = dlg.GetPathName(), strExt = dlg.GetFileExt(), strFiles, strCount;
	if (strExt.CompareNoCase("smd") != 0) return;

	if (!m_PVSMgr.m_bCompiled)
		m_PVSMgr.DoAllCompile();

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];
	_splitpath(strOpen,drive,dir,fname,ext);
	strFiles = drive; strFiles += dir;strFiles += fname;

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	int iCount = pFrm->m_FloorList.size();

	// Terrain Size Calc..
	int iMax = GetTerrainSize();

	for (int i = 0; i < iCount; i++)
	{
		CString strFName;
		strCount.Format("_%d.", i);
		strFName = strFiles + strCount + strExt;

		HANDLE hFile = CreateFile(strFName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox("File Create Error!!\n");
			return;
		}		

		// Terrain Data..
		WriteTerrainData(hFile, iMax, i);

		WriteCollisionData(hFile, iMax);

		// Object Event Data..
		WriteObjectEventData(hFile, i);

		CloseHandle(hFile);
	}
}

void COrganizeView::WriteCollisionData(HANDLE hFile, int iMax)
{
	DWORD dwNum;
	const int TERRAIN_CELL_SIZE = 4;
	float fSize = (iMax-1) * TERRAIN_CELL_SIZE;

	WriteFile(hFile, &fSize, 4, &dwNum, NULL); // 맵 실제 미터 단위 너비
	WriteFile(hFile, &fSize, 4, &dwNum, NULL); // 맵 실제 미터 단위 길이

	// 충돌 체크 폴리곤 데이터 쓰기..
	int iColCount = 0;
	WriteFile(hFile, &iColCount, 4, &dwNum, NULL);

	const int CELL_MAIN_DEVIDE = 4; // 메인셀은 4 X 4 의 서브셀로 나뉜다..
	const int CELL_SUB_SIZE = 4; // 4 Meter 가 서브셀의 사이즈이다..
	const int CELL_MAIN_SIZE = CELL_MAIN_DEVIDE * CELL_SUB_SIZE; // 메인셀 크기는 서브셀갯수 X 서브셀 크기이다.

	// Cell Data 쓰기.
	int z = 0;
	for(float fZ = 0.0f; fZ < fSize; fZ += CELL_MAIN_SIZE, z++)
	{
		int x = 0;
		for(float fX = 0.0f; fX < fSize; fX += CELL_MAIN_SIZE, x++)
		{
			BOOL bExist = FALSE;
			WriteFile(hFile, &bExist, 4, &dwNum, NULL); // 데이터가 있는 셀인지 쓰고..
		}
	}
}

void COrganizeView::WriteObjectEventData(HANDLE hFile, int iFloor)
{
	DWORD dwNum;
	ShapeInfo* pSI = NULL;
	CPortalVolume* pVol = NULL;
	int iCount = 0, ith = 0;
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	FloorInfo FInfo;
	bool bFound = false;

	vfiter vfit = pFrm->m_FloorList.begin();
	while (vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;
		if (ith == iFloor)
		{
			bFound = true;
			break;
		}
		ith++;
	}

	if (!bFound)
		return;
	else
	{
		int iSize = FInfo.m_vVolume.size();
		for (int i = 0; i < iSize; i++)
		{
			pVol = FInfo.m_vVolume[i];
			
			siiter siit = pVol->m_plShapeInfoList.begin();
			while(siit != pVol->m_plShapeInfoList.end())
			{
				pSI = *siit++;
				if (pSI->m_iEventID || pSI->m_iEventType || pSI->m_iNPC_ID || pSI->m_iNPC_Status ) // 이벤트가 있으면
					iCount++;
			}
		}
	}

	WriteFile(hFile, &iCount, 4, &dwNum, NULL);
	short sEvent;
	__Vector3 vPos;

	int iSize = FInfo.m_vVolume.size();
	for (int i = 0; i < iSize; i++)
	{
		pVol = FInfo.m_vVolume[i];
		
		siiter siit = pVol->m_plShapeInfoList.begin();
		while(siit != pVol->m_plShapeInfoList.end())
		{
			pSI = *siit++;
			if (pSI->m_iEventID || pSI->m_iEventType || pSI->m_iNPC_ID || pSI->m_iNPC_Status ) // 이벤트가 있으면
			{
				WriteFile(hFile, &(pSI->m_iBelong), 4, &dwNum, NULL);
				sEvent = (short)(pSI->m_iEventID); WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
				sEvent = (short)(pSI->m_iEventType); WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
				sEvent = (short)(pSI->m_iNPC_ID); WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
				sEvent = (short)(pSI->m_iNPC_Status); WriteFile(hFile, &sEvent, 2, &dwNum, NULL);
				
				vPos = pSI->Pos();
				WriteFile(hFile, &(vPos.x), 4, &dwNum, NULL);
				WriteFile(hFile, &(vPos.y), 4, &dwNum, NULL);
				WriteFile(hFile, &(vPos.z), 4, &dwNum, NULL);
			}
		}
	}
}

int	COrganizeView::GetTerrainSize()
{
	const int TERRAIN_CELL_SIZE = 4;

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	__Vector3 vMin, vMax;
	pFrm->FindMinMaxTotalShape(vMin, vMax);
	int ixmax = abs(vMax.x - 0.0f) +1;
	int izmax = abs(vMax.z - 0.0f) +1;
	int iMax = (ixmax > izmax) ? ixmax : izmax;	
	int iMod = iMax % TERRAIN_CELL_SIZE;
	if (iMod != 0)
		iMax = (iMax / TERRAIN_CELL_SIZE) + 1;
	iMax += 1;
	return iMax;
}

void COrganizeView::WriteTerrainData(HANDLE hFile, int iMax, int iFloor)
{
	const int TERRAIN_CELL_SIZE = 4;

	int x, z;
	float UnitDist;
	DWORD dwNum;

	WriteFile(hFile, &iMax, sizeof(int), &dwNum, NULL);
	iTempOffs += 4;
	TRACE1("File Offset %d\n", iTempOffs);
	UnitDist = (float)TERRAIN_CELL_SIZE;
	WriteFile(hFile, &UnitDist, sizeof(float), &dwNum, NULL);
	iTempOffs += 4;
	TRACE1("File Offset %d\n", iTempOffs);

	//  그냥 하나씩 저장해도 차이 없지만 네트워크로 저장할때는 파일 엑세스 숫자를 줄이고 한꺼번에 저장해야  빠르다.. ...
	DWORD dwAlloc = iMax * iMax * sizeof(float);
	HGLOBAL hAlloc = ::GlobalAlloc(GMEM_FIXED, dwAlloc);
	__ASSERT(hAlloc, "Global allocation failed.");
	float* pfHeights = (float*)::GlobalLock(hAlloc);

	for(x=0;x<iMax;x++)
		for(z=0;z<iMax;z++)
			pfHeights[x*iMax+z] = GetFloorHeight(iFloor, x*TERRAIN_CELL_SIZE, z*TERRAIN_CELL_SIZE);

	WriteFile(hFile, pfHeights, dwAlloc, &dwNum, NULL); // 파일에 쓴다..
	iTempOffs += dwAlloc;
	TRACE1("File Offset %d\n", iTempOffs);
	pfHeights = NULL;
	::GlobalUnlock(hAlloc);
	::GlobalFree(hAlloc);
}

float COrganizeView::GetFloorHeight(int iFloor, int iX, int iZ)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	int ith = 0;
	FloorInfo FInfo;
	bool bFound = false;

	vfiter vfit = pFrm->m_FloorList.begin();
	while (vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;
		if (ith == iFloor)
		{
			bFound = true;
			break;
		}
		ith++;
	}

	if (!bFound)
		return 0.0f;
	else
	{
		int iCount = FInfo.m_vVolume.size();
		float fHeight;
		for (int i = 0; i < iCount; i++)
		{
			CPortalVolume* pVol = FInfo.m_vVolume[i];
			if (pVol->GetHeight(iX, iZ, fHeight))
				return fHeight;
		}
	}

	return 0.0f;
}

void COrganizeView::OnBtnShapeDelete() 
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	// TODO: Add your control notification handler code here
	POSITION pos = m_ShapesListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("No items were selected!\n");
		return;
	} 
	else
	{
		int iOrder = m_ShapesListCtrl.GetHotItem();
		if (iOrder == -1)
		{
			AfxMessageBox("No items were selected!\n");		
			return;
		}

		m_PVSMgr.DeleteTotalShapeLinkByiOrder(iOrder);

		pFrm->TotalValidateCheckAfterDelete();
		RefreshTotalShape();
		RefreshLinkedList();
	}
}

void COrganizeView::SelectNextShape()
{
	CPortalVolume *pVol = NULL;

	int iOrder = m_SelectedListCtrl.GetHotItem();
	if (iOrder == -1)
		return;

	pVol = m_PVSMgr.GetPortalVolByiOrder(iOrder);
	if (!pVol) return;

	int iVolCount = pVol->GetLinkedVolumeCount();		
	int iShapeCount = pVol->GetLinkedShapeCount();
	int iTotalCount = iVolCount + iShapeCount;
	if (iShapeCount < 1) return;		

	if (m_iSelectionLinkedShapeIndex >= iTotalCount)
	{
		m_iSelectionLinkedShapeIndex = iTotalCount-1;
		m_LinkedListCtrl.SetHotItem(m_iSelectionLinkedShapeIndex);
	}

	int iIndex = m_iSelectionLinkedShapeIndex - iVolCount;
	if (iShapeCount == 1)
	{
		iIndex = 0;
		m_iSelectionLinkedShapeIndex = iVolCount;
		m_LinkedListCtrl.SetHotItem(iVolCount);
	}
	else
	{
		if (iIndex < 0)
		{
			iIndex = 0;
			m_iSelectionLinkedShapeIndex = iVolCount;
			m_LinkedListCtrl.SetHotItem(iVolCount);
		}
		else
		{
			iIndex++;
			if (iIndex == iShapeCount)
			{
				iIndex = 0;
				m_iSelectionLinkedShapeIndex = iVolCount;
				m_LinkedListCtrl.SetHotItem(iVolCount);
			}
			else
			{
				m_iSelectionLinkedShapeIndex = iVolCount+iIndex;
				m_LinkedListCtrl.SetHotItem(iVolCount+iIndex);
			}
		}
	}

	OnStateShape();

	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();
	if (pFrm->m_pDummy)
	{
		SelectElement se;
		se.eST = TYPE_SHAPE_ONLY;
		se.pSelectPointer = pVol->GetNthLinkedShapeInfo(iIndex);
		pFrm->m_pDummy->SetSelObj(se);
	}
	else if (pFrm->m_eEditState == EDIT_SELECT)
		pFrm->m_SelVolArray.RemoveAll();

	pFrm->m_LastSelectedElement.eST = TYPE_SHAPE_ONLY;
	pFrm->m_LastSelectedElement.pSelectPointer = pVol->GetNthLinkedShapeInfo(iIndex);

	pFrm->UpdateShapeInfoDisplay();
}

void COrganizeView::OnViewSelectedTotalShape() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_bViewSelectedTotalShape = !pFrm->m_bViewSelectedTotalShape;	
}

ShapeInfo* COrganizeView::GetSelectedTotalShape()
{
	return GetNthShapeInfo(m_iSelectionShapeIndex);
}

void COrganizeView::OnIndiPick() 
{
	// TODO: Add your control notification handler code here
	OnCurserSelect(EDIT_SELECT);
}

void COrganizeView::OnIndiMove() 
{
	// TODO: Add your control notification handler code here
	OnCurserSelect(EDIT_MOVE);
}

void COrganizeView::OnIndiRotate() 
{
	// TODO: Add your control notification handler code here
	OnCurserSelect(EDIT_ROTATE);
}

void COrganizeView::OnIndiScale() 
{
	// TODO: Add your control notification handler code here
	OnCurserSelect(EDIT_SCALE);
}

void COrganizeView::OnChangeTotalMoveX() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	BOOL bTrans; 
	int ix = GetDlgItemInt(IDC_TOTAL_MOVE_X, &bTrans, TRUE);	
	int iy = GetDlgItemInt(IDC_TOTAL_MOVE_Y, &bTrans, TRUE);	
	int iz = GetDlgItemInt(IDC_TOTAL_MOVE_Z, &bTrans, TRUE);	
	UpdatePosAll(ix, iy, iz);
}

void COrganizeView::OnChangeTotalMoveY() 
{
	OnChangeTotalMoveX();
}

void COrganizeView::OnChangeTotalMoveZ() 
{
	OnChangeTotalMoveX();
}

void COrganizeView::UpdatePosAll(int iX, int iY, int iZ)
{
	m_PVSMgr.UpdatePosAll(iX, iY, iZ);
}

void COrganizeView::OnSetFloor() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	int iSize = pFrm->m_SelVolArray.GetSize();

	if (iSize < 1)
	{
		AfxMessageBox("선택되어있는 Volume이 없습니다....");
		return;
	}

	CFloorDlg FDlg;
	int iReturn = FDlg.DoModal();	
	if (iReturn == 2)
		return;

	int iCur = FDlg.GetFloor();

	// 데이터 추가..
	FloorInfo FInfo;
	FInfo.m_iFloor = iCur;
	for (int i = 0; i < iSize; i++)
		FInfo.m_vVolume.push_back(pFrm->m_SelVolArray.GetAt(i));

	pFrm->m_FloorList.push_back(FInfo);

	// ComboBox 추가..
	CString str; str.Format("%d 층", iCur);
	m_ComboFloor.AddString(str);
	iSize = m_ComboFloor.GetCount();
	m_ComboFloor.SetCurSel(iSize-1);
	m_ComboFloor.SetItemData(iSize-1, iCur);
}

void COrganizeView::OnDeleteFloor() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();

	int iCurSel = m_ComboFloor.GetCurSel();
	int iCur = m_ComboFloor.GetItemData(iCurSel);
	
	FloorInfo FInfo;
	vfiter vfit = pFrm->m_FloorList.begin();
	while(vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit;
		if (FInfo.m_iFloor == iCur)
		{
			FInfo.m_vVolume.clear();
			pFrm->m_FloorList.erase(vfit);
			break;
		}
		vfit++;
	}	

	m_ComboFloor.DeleteString(iCurSel);
	int iSize = m_ComboFloor.GetCount();
	m_ComboFloor.SetCurSel(iSize-1);
}

void COrganizeView::OnCheckFloor() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = NULL;
	pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_bViewSelectedFloor = !pFrm->m_bViewSelectedFloor;			
}

int COrganizeView::GetCurFloor()
{
	return m_ComboFloor.GetItemData(m_ComboFloor.GetCurSel());
}

void COrganizeView::RefreshFloorCombo()
{
	m_ComboFloor.ResetContent();

	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	FloorInfo FInfo;
	int iSize;
	
	vfiter vfit = pFrm->m_FloorList.begin();
	while(vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;

		CString str; str.Format("%d 층", FInfo.m_iFloor);
		m_ComboFloor.AddString(str);
		iSize = m_ComboFloor.GetCount();
		m_ComboFloor.SetCurSel(iSize-1);
		m_ComboFloor.SetItemData(iSize-1, FInfo.m_iFloor);
	}	
}

void COrganizeView::RefreshFloorInfoAll(int iID)
{
	int iCount ;
	std::vector<CPortalVolume* > vecVol;
	m_ComboFloor.ResetContent();
	std::vector<FloorInfo>	m_FloorVector;

	CMainFrame* pFrm =  (CMainFrame* )AfxGetMainWnd();
	FloorInfo FInfo;
	int i; 
	
	vfiter vfit = pFrm->m_FloorList.begin();
	while(vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;
		iCount = FInfo.m_vVolume.size();
		vecVol.clear();

		// Verify..
		for (i = 0; i < iCount; i++)
		{
			if (iID != FInfo.m_vVolume[i]->m_iID)		
				vecVol.push_back(FInfo.m_vVolume[i]);
		}
		FInfo.m_vVolume.clear();
		iCount = vecVol.size();

		// Verify..
		for (i = 0; i < iCount; i++)
			FInfo.m_vVolume.push_back(vecVol[i]);

		if (iCount != 0)	// 한개라도 남아있으면.. 
		{
			CString str; str.Format("%d 층", FInfo.m_iFloor);
			m_ComboFloor.AddString(str);
			int iSize = m_ComboFloor.GetCount();
			m_ComboFloor.SetCurSel(iSize-1);
			m_ComboFloor.SetItemData(iSize-1, FInfo.m_iFloor);
		}

		m_FloorVector.push_back(FInfo);
	}

	// 전부 지운다..
	vfit = pFrm->m_FloorList.begin();
	while (vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;
		FInfo.m_vVolume.clear();
	}
	pFrm->m_FloorList.clear();

	iCount = m_FloorVector.size();
	for (i = 0; i < iCount; i++)
	{
		pFrm->m_FloorList.push_back(m_FloorVector[i]);
	}
}


