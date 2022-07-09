// PVSManager.cpp: implementation of the CPVSManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N3Indoor.h"
#include "PVSManager.h"
#include "PortalVolume.h"

#include "MainFrm.h"
#include "OrganizeView.h"

#include "resource.h"
#include "ComDialog.h"

#include "../N3Base/N3Scene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPVSManager::CPVSManager()
{
	m_iIncreseIndex = 0;
	m_iIncreseShapeIndex = 0;
	m_iTotalCount = 0;
	m_iCurIndex = -1;
	m_bCompiled = false;
	m_bGameData = false;
	m_pCurVol = NULL;
}

CPVSManager::~CPVSManager()
{
	DeleteAllPvsObj();
}

void CPVSManager::InsertNewPortalVolumn()
{
	m_pPvsList.push_back(m_Factory.CreatePvsVol(m_iIncreseIndex++));
	m_iTotalCount++;
}

void CPVSManager::DeleteAllPvsObj()
{
	iter it = m_pPvsList.begin();

	while (it != m_pPvsList.end())	
	{
		CPortalVolume* pvs = *it;
		if (pvs) delete pvs;
		it++;
	}	
	m_pPvsList.clear();

	m_iTotalCount = 0;
	m_iIncreseIndex = 0;
	m_iIncreseShapeIndex = 0;

	ShapeInfo* pSI;
	siiter siit = m_plShapeInfoList.begin();
	while(siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;
		delete pSI;
	}
	m_plShapeInfoList.clear();
}

CPortalVolume* CPVSManager::GetPortalVolByiOrder(int iOrder)
{
	if ((m_pPvsList.size() <= 0) || (iOrder < 0) || (m_pPvsList.size()-1 < iOrder))
		return NULL;

	iter it = m_pPvsList.begin();
	for( int i = 0; i < iOrder; i++)
		it++;

	return *it;
}

bool CPVSManager::DeletePortalVolByiOrder(int iOrder)
{
	if (iOrder == -1)	return false;

	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();
	for( int i = 0; i < iOrder; i++)
		it++;

	pVol = *it;
	if (pVol)	delete pVol;
	else	return false;

	m_pPvsList.erase(it);
	m_iTotalCount--;

	return true;
}

bool CPVSManager::IsExistPortalVolumeByPointer(CPortalVolume* pVol)
{
	CPortalVolume* pVol2 = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol2 = *it++;

		if (pVol == pVol2)
			return true;
	}

	return false;
}

bool CPVSManager::IsExistTotalShapeByPointer(ShapeInfo* pSI)
{
	ShapeInfo*	pSI2 = NULL;	

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI2 = *siit++;			
		if (pSI2 == pSI)
			return true;
	}

	return false;
}

bool CPVSManager::IsExistLinkedShapeByPointer(ShapeInfo* pSI)
{
	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;
		if (pVol->IsExistLinkedShapeByPointer(pSI))	
			return true;
	}

	return false;
}

bool CPVSManager::DeletePortalVolByPointer(CPortalVolume* pVol)
{
	CPortalVolume* pVol2 = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol2 = *it++;

		if (pVol == pVol2)
			break;
	}

	if (pVol2) 
		delete pVol2;
	else 
		return false;

	m_pPvsList.erase(--it);
	m_iTotalCount--;

	return true;
}

void CPVSManager::DeletePortalVolLinkedByiID(int iID)
{
	iter itBase = m_pPvsList.begin();
	witer itLinked; 
	CPortalVolume *pVolBase;
	WVOL Linked;

	while (itBase != m_pPvsList.end())	
	{
		pVolBase = *itBase++;
		itLinked = pVolBase->m_VoltList.begin();

		while (itLinked != pVolBase->m_VoltList.end())
		{
			Linked = *itLinked;	
			if (Linked.pPvsVol->m_iID == iID)
				itLinked = pVolBase->m_VoltList.erase(itLinked);
			else
				itLinked++;
		}
	}		
}

void CPVSManager::DeleteTotalShapeLinkByiOrder(int iOrder)
{
	if (iOrder == -1)	return;

	ShapeInfo*	pSI = NULL;	
	siiter siit = m_plShapeInfoList.begin();
	for( int i = 0; i < iOrder; i++)
		siit++;

	pSI = *siit;

	// 먼저 지워질 Shape를 가지고 있는 Volume들을 찾아서..	처리한다..
	ShapePart* pSP = NULL;

	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();

	while (it != m_pPvsList.end())
	{
		pVol = *it++;
		
		spiter spit = pVol->m_lpShapePartList.begin();
		while( spit != pVol->m_lpShapePartList.end() )
		{
			pSP = *spit;
			
			if (pSP->m_iID == pSI->m_iID)
			{
				delete pSP;
				spit = pVol->m_lpShapePartList.erase(spit);
			}
			else
				spit++;
		}
	}

	delete pSI;
	m_plShapeInfoList.erase(siit);
}

// Edit 모드..
void CPVSManager::TickEdit()
{
	CPortalVolume* pVol = NULL;
	WVOL wvol;

	int i = 0;

	iter it = m_pPvsList.begin();
	witer wit; 

	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		pVol->SetState(STATE_NONE);
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	if (pFrm->m_bViewSelectedFloor)
		return;

	if (pFrm->m_pDummy)
	{
		int iSize = pFrm->m_pDummy->m_SelObjArray.GetSize();
		for ( i = 0; i < iSize; i++ )
		{
			if (pFrm->m_pDummy->m_SelObjArray[i].eST == TYPE_VOLUME_TOTAL)
			{
				pVol = (CPortalVolume* )pFrm->m_pDummy->m_SelObjArray[i].pSelectPointer;
				pVol->SetState(STATE_SELECTED);

				wit = pVol->m_VoltList.begin();
				while( wit != pVol->m_VoltList.end() )
				{
					wvol = *wit;
					if (wvol.pPvsVol->m_eState != STATE_SELECTED)
						wvol.pPvsVol->SetState(STATE_LINKED, wvol.ePWT);
					wit++;
				}
			}
		}
	}

	if (pFrm->m_SelVolArray.GetSize() > 0)
	{
		int iSize = pFrm->m_SelVolArray.GetSize();
		for ( i = 0; i < iSize; i++ )
		{
			pVol = (CPortalVolume* )pFrm->m_SelVolArray[i];
			pVol->SetState(STATE_SELECTED);

			wit = pVol->m_VoltList.begin();
			while( wit != pVol->m_VoltList.end() )
			{
				wvol = *wit;
				if (wvol.pPvsVol->m_eState != STATE_SELECTED)
					wvol.pPvsVol->SetState(STATE_LINKED, wvol.ePWT);
				wit++;
			}
		}
	}
}

void CPVSManager::RenderEdit()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	if (pFrm->m_bViewSelectedFloor)
	{
		FloorInfo FInfo;
		vfiter vfit = pFrm->m_FloorList.begin();
		while(vfit != pFrm->m_FloorList.end())
		{
			FInfo = *vfit++;
			if (FInfo.m_iFloor == pView->GetCurFloor())
			{
				int iSize = FInfo.m_vVolume.size();
				for (int i = 0; i < iSize; i++)
					FInfo.m_vVolume[i]->RenderEdit();
			}
		}

		return;
	}

	// Shape..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Shape)
		TotalShapeRenderEdit();

	// Collision..
	if (pFrm->m_dwRenderingOption & dw_Render_Add_Collision)
		TotalCollisionRenderEdit();	

	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();

	if ( pFrm->m_LastSelectedElement.pSelectPointer )
	{
		switch (pFrm->m_LastSelectedElement.eST)
		{
			case TYPE_VOLUME_ONLY:
				((CPortalVolume* )pFrm->m_LastSelectedElement.pSelectPointer)->SetState(STATE_VOLUME_ONLY);
				break;

			case TYPE_SHAPE_ONLY:
				{
					ShapeInfo* pSI = (ShapeInfo* )pFrm->m_LastSelectedElement.pSelectPointer;
					pSI->m_pShape->PosSet(pSI->Pos());
					pSI->m_pShape->RotSet(pSI->Rot());
					pSI->m_pShape->ScaleSet(pSI->Scale());
					pSI->m_pShape->m_bDontRender = false;
					pSI->m_pShape->RenderSelected(true);
				}
				break;
		}
	}

	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		pVol->RenderEdit();
	}
}

// Compile 모드..
void CPVSManager::TickCompile()
{
	CPortalVolume* pVol = NULL;
	WVOL wvol;

	int i = 0;

	iter it = m_pPvsList.begin();
	witer wit; 

	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		pVol->SetState(STATE_NONE);
	}
}

void CPVSManager::RenderCompile()
{
	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	if (!pFrm->m_bViewSelectedTotalShape)
	{
		while(it != m_pPvsList.end())
		{
			pVol = *it++;	
			if ( pVol->m_eRenderType == TYPE_TRUE )
			{
				pVol->RenderCompile();
			}
		}
	}
	else
	{
		int iCount = pFrm->m_SelVolArray.GetSize();
		if (iCount > 0)
		{
			for ( int i = 0; i < iCount; i++ )
			{
				pVol = pFrm->m_SelVolArray.GetAt(i);
				pVol->RenderCompile();
			}
		}
	}
}

bool CPVSManager::StartExecuteMode()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	int iSize = pFrm->m_SelVolArray.GetSize();

	if (iSize < 1)
	{
		AfxMessageBox("선택되어있는 Volume이 없습니다....");
		return false;
	}

	m_pCurVol = pFrm->m_SelVolArray.GetAt(0);

	// Camera를 셋팅한다..
	m_vBackupEye = pFrm->m_Camera.EyePos();
	m_vBackupAt  = pFrm->m_Camera.AtPos();

	__Vector3 vec = m_pCurVol->GetCentorPoint();
	pFrm->m_Camera.EyePosSet(vec);
	pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
	pFrm->m_Camera.AtPosSet(vec + __Vector3(0.0f, 0.0f, 1.0f));
	pFrm->m_Camera.Apply();

	return true;
}

void CPVSManager::EndExecuteMode()
{
	// Camera 셋팅을 복구한다..
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	pFrm->m_Camera.EyePosSet(m_vBackupEye);
	pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
	pFrm->m_Camera.AtPosSet(m_vBackupAt);
	pFrm->m_Camera.Apply();
}

void CPVSManager::ExecuteCameraChange()
{
	if (m_pPvsList.size() < 1)
	{
		AfxMessageBox("선택 가능한 리스트가 없습니다....");
		return;
	}
	
	if (!m_pCurVol)
	{
		AfxMessageBox("선택되어있는 Volume이 없습니다....");
		return;
	}

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	m_vBackupEyeExecute = pFrm->m_Camera.EyePos();
	m_vBackupAtExecute = pFrm->m_Camera.AtPos();

	__Vector3 vec = m_pCurVol->GetCentorPoint();
	pFrm->m_Camera.EyePosSet(vec + __Vector3(0.0f, 50.0f, 0.0f));
	pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
	pFrm->m_Camera.AtPosSet(vec + __Vector3(0.0f, 0.0f, 0.1f));
	pFrm->m_Camera.Apply();
}

void CPVSManager::RestoreExecuteCameraChange()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	pFrm->m_Camera.EyePosSet(m_vBackupEyeExecute);
	pFrm->m_Camera.UpVectorSet(__Vector3(0.0f, 1.0f, 0.0f));
	pFrm->m_Camera.AtPosSet(m_vBackupAtExecute);
	pFrm->m_Camera.Apply();
}

// Execute 모드..
void CPVSManager::TickExecute()
{
	CPortalVolume* pVol = NULL;
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();

	__Vector3 vec = pFrm->m_Camera.EyePos();
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		if (pVol->IsInVolumn(vec))
		{
			m_pCurVol = pVol;
			break;
		}
	}

	it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;
		pVol->SetState(STATE_NONE);
		pVol->m_eRenderType = TYPE_UNKNOWN;
	}

	VisPortalPriority vPP;
	vppiter vppit = m_pCurVol->m_pVisiblePvsList.begin();
	while(vppit != m_pCurVol->m_pVisiblePvsList.end())
	{
		vPP = *vppit++;
		vPP.m_pVol->m_eRenderType = TYPE_TRUE;
	}	
}

void CPVSManager::RenderExecute()
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		if ( pVol->m_eRenderType == TYPE_TRUE )
		{
			pVol->RenderExecute();
		}
	}
}

void CPVSManager::TotalShapeRenderEdit()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	__Matrix44 mtxWorld;
	mtxWorld.Identity();

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);
	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	ShapeInfo* pSI;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	if (pFrm->m_bViewSelectedTotalShape)
	{
		if ((pSI = pView->GetSelectedTotalShape()) && pSI)
		{
			pSI->m_pShape->PosSet(pSI->Pos());
			pSI->m_pShape->RotSet(pSI->Rot());
			pSI->m_pShape->ScaleSet(pSI->Scale());
			pSI->m_pShape->m_bDontRender = false;
			pSI->m_pShape->Render();
		}
	}
	else
	{
		siiter siit = m_plShapeInfoList.begin();
		while(siit != m_plShapeInfoList.end())
		{
			pSI = *siit++;
			pSI->m_pShape->PosSet(pSI->Pos());
			pSI->m_pShape->RotSet(pSI->Rot());
			pSI->m_pShape->ScaleSet(pSI->Scale());
			pSI->m_pShape->m_bDontRender = false;
			pSI->m_pShape->Render();
		}
	}

	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPVSManager::TotalCollisionRenderEdit()
{
	DWORD dwAlpha, dwFog, dwLight, dwPointSize;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_POINTSIZE , &dwPointSize);
	
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		smtl.Init();
		bInit = true;
	}

	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetVertexShader(FVF_CV);	
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	ShapeInfo* pSI;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	if (pFrm->m_bViewSelectedTotalShape)
	{
		if ((pSI = pView->GetSelectedTotalShape()) && pSI)
		{
			pSI->m_pShape->Tick(-1000);

			// 로딩할때 미리 계산해 놓은 월드 행렬 적용..
			__Matrix44 mtxBackup;
			CN3Base::s_lpD3DDev->GetTransform(D3DTS_WORLD, &mtxBackup);
			CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &pSI->m_Matrix);

			if (pSI->m_pShape->CollisionMesh())
				pSI->m_pShape->CollisionMesh()->Render(0xffffffff);

			CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxBackup);
		}
	}
	else
	{
		siiter siit = m_plShapeInfoList.begin();
		while(siit != m_plShapeInfoList.end())
		{
			pSI = *siit++;
			pSI->m_pShape->Tick(-1000);

			// 로딩할때 미리 계산해 놓은 월드 행렬 적용..
			__Matrix44 mtxBackup;
			CN3Base::s_lpD3DDev->GetTransform(D3DTS_WORLD, &mtxBackup);
			CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &pSI->m_Matrix);

			if (pSI->m_pShape->CollisionMesh())
				pSI->m_pShape->CollisionMesh()->Render(0xffffffff);

			CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxBackup);
		}
	}

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_POINTSIZE, dwPointSize);
	if(dwFog) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CPVSManager::SplitShapeToVolumn(CDialog* pDlg)
{
	CString szProgress;

	int iSize = m_pPvsList.size()*m_plShapeInfoList.size();
	int iCount = 0; 

	ShapeInfo* pSI;
	siiter siit = m_plShapeInfoList.begin();
	while(siit != m_plShapeInfoList.end())
	{
		pSI = *siit++;

		pSI->m_pShape->Tick();
		pSI->m_pShape->SetMaxLOD();
		
		// 뽀갠다..
		CPortalVolume* pVol = NULL;

		iter it = m_pPvsList.begin();
		while(it != m_pPvsList.end())
		{
			szProgress.Format("%d %% In Progress.. ^^", int(iCount*100/iSize) );
			pDlg->SetDlgItemText(IDC_PROGRESS, szProgress);
			iCount++;

			pVol = *it++;	

			// Shape의폴리곤 갯수만큼 돌면서.. 먼저 Transform 시키고.. 검사.. 								
			pVol->SplitAndMakeShape(pSI);
			pVol->SplitAndMakeCollision(pSI);
		}
	}
}

void CPVSManager::RegisterShape(std::string szStr, CN3Shape* pShape)
{
	ShapeInfo*	pSI = new ShapeInfo;
	pSI->m_iID = m_iIncreseShapeIndex++;	
	pSI->m_strShapeFile = szStr;
	pSI->m_pShape = pShape;

	m_plShapeInfoList.push_back(pSI);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();
	pView->RefreshTotalShape();
}

void CPVSManager::DoAllCompile()
{
	if (m_pPvsList.size() < 1)
	{
		AfxMessageBox("선택 가능한 리스트가 없습니다....");
		return;
	}

	Decompile();

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	CComDialog* pDlg = new CComDialog;
	pDlg->Create(IDD_COMPUTING, pView);
	pDlg->ShowWindow(true);

	CString szProgress;
	szProgress.Format("PVS In Progres.. ^^");
	pDlg->SetDlgItemText(IDC_PROGRESS, szProgress);

	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		DecompilePerVolume(pVol);
		ComputeVisibilty(pVol);
	}

	//..
	SplitShapeToVolumn((CComDialog* )pDlg);

	pDlg->DestroyWindow();
	delete pDlg;

	m_bCompiled = true;
}

void CPVSManager::DecompilePerVolume(CPortalVolume *pVol)
{
	pVol->m_pVisiblePvsList.clear();
	CPortalVolume* pVolFrom = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVolFrom = *it++;	
		pVolFrom->SetState(STATE_NONE);
		pVolFrom->m_eRenderType = TYPE_UNKNOWN;
		pVolFrom->m_iPriority = -1;
	}
}

void CPVSManager::Decompile()
{
	CPortalVolume* pVol = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		pVol->SetState(STATE_NONE);
		pVol->m_eRenderType = TYPE_UNKNOWN;
		pVol->m_iPriority = -1;
		pVol->ClearCompileData();
	}

	m_bCompiled = false;
}

void CPVSManager::CheckcompileMode(CPortalVolume *pVol)
{
	CPortalVolume* pVolFrom = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVolFrom = *it++;	
		pVolFrom->SetState(STATE_NONE);
		pVolFrom->m_eRenderType = TYPE_UNKNOWN;
	}

	VisPortalPriority vPP;
	vppiter vppit = pVol->m_pVisiblePvsList.begin();

	while(vppit != pVol->m_pVisiblePvsList.end())
	{
		vPP = *vppit++;	
		vPP.m_pVol->m_eRenderType = TYPE_TRUE;
	}
}

void CPVSManager::ComputeVisibilty(CPortalVolume * const pVol)
{
	SetPriority(pVol);

	// 렌더링 속도를 빠르게 하기 위해서.. 우선순위대로 정렬한 후.. 순서대로 렌더링한다..
	std::priority_queue< CPortalVolume*, std::vector<CPortalVolume*>, Myless<CPortalVolume*> > pQueue; 

	CPortalVolume* pVolTo = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVolTo = *it++;	
		if (pVolTo->m_eRenderType == TYPE_TRUE)
			pQueue.push(pVolTo);
	}

	VisPortalPriority vPP;
	while (!pQueue.empty())
	{
		vPP.m_pVol = pQueue.top();
		vPP.m_iPriority = vPP.m_pVol->m_iPriority;
		ASSERT(vPP.m_iPriority != -1);
		pVol->m_pVisiblePvsList.push_back(vPP);			
		pQueue.pop();
	}

	// 디버그용 함수.. ^^
	DebugFunc();
}

e_PvsWallType CPVSManager::GetReverseWallType(e_PvsWallType ePWT)
{
	switch (ePWT)
	{
		case PWALL_ZB:
			return PWALL_ZF;
		case PWALL_ZF:
			return PWALL_ZB;
		case PWALL_XL:
			return PWALL_XR;
		case PWALL_XR:
			return PWALL_XL;
		case PWALL_YT:
			return PWALL_YB;
		case PWALL_YB:
			return PWALL_YT;
	}

	return PWALL_ZB;
}

void CPVSManager::SetPriority(CPortalVolume * const pVol)
{
	pVol->m_iPriority = 0;	
	pVol->m_eRenderType = TYPE_TRUE;

	WVOL wvol;
	witer wit = pVol->m_VoltList.begin();
	while(wit != pVol->m_VoltList.end())
	{
		wvol = *wit++;
		SetPriorityEx(wvol.pPvsVol, GetReverseWallType(wvol.ePWT), 1);
	}
}

void CPVSManager::SetPriorityEx(CPortalVolume * const pVol, e_PvsWallType ePWT, int iRecursive)
{
	pVol->m_iPriority = iRecursive;	
	pVol->m_eRenderType = TYPE_TRUE;

	WVOL wvol;
	witer wit = pVol->m_VoltList.begin();
	while(wit != pVol->m_VoltList.end())
	{
		wvol = *wit++;
		if (wvol.ePWT != ePWT)
		{
			wvol.pPvsVol->m_iPriority = iRecursive+1;	
			wvol.pPvsVol->m_eRenderType = TYPE_TRUE;
			SetPriorityRecursive(wvol.pPvsVol, wvol.ePWT, iRecursive+2);
		}
	}
}

void CPVSManager::SetPriorityRecursive(CPortalVolume * const pVol, e_PvsWallType ePWT,  int iRecursive)
{
	if (!HasWallTypeVolume(pVol, ePWT))
	{
		SetPriorityNoneRecursive(pVol, iRecursive);
		return;
	}
	else
	{
		WVOL wvol;
		witer wit = pVol->m_VoltList.begin();
		while(wit != pVol->m_VoltList.end())
		{
			wvol = *wit++;
			if (wvol.ePWT == ePWT)
			{
				if ((wvol.pPvsVol->m_iPriority == -1) || (wvol.pPvsVol->m_iPriority>1))
					wvol.pPvsVol->m_iPriority = iRecursive;	
				wvol.pPvsVol->m_eRenderType = TYPE_TRUE;
				SetPriorityRecursive(wvol.pPvsVol, ePWT, iRecursive+1);
			}
			else if (wvol.ePWT != GetReverseWallType(ePWT))
				SetPriorityNoneRecursiveEx(wvol.pPvsVol, iRecursive+1);				
		}
	}
}

bool CPVSManager::HasWallTypeVolume(CPortalVolume * const pVol, e_PvsWallType ePWT)
{
	WVOL wvol;
	witer wit = pVol->m_VoltList.begin();
	while(wit != pVol->m_VoltList.end())
	{
		wvol = *wit++;
		if (wvol.ePWT == ePWT)
			return true;	
	}

	return false;
}

void CPVSManager::SetPriorityNoneRecursive(CPortalVolume * const pVol, int iRecursive)
{
	if ((pVol->m_iPriority == -1) || (pVol->m_iPriority>1))
		pVol->m_iPriority = iRecursive;	
	pVol->m_eRenderType = TYPE_TRUE;

	WVOL wvol;
	witer wit = pVol->m_VoltList.begin();
	while(wit != pVol->m_VoltList.end())
	{
		wvol = *wit++;
		SetPriorityNoneRecursiveEx(wvol.pPvsVol, iRecursive+1);
	}
}

void CPVSManager::SetPriorityNoneRecursiveEx(CPortalVolume * const pVol, int iRecursive)
{
	if ((pVol->m_iPriority == -1) || (pVol->m_iPriority>1))
		pVol->m_iPriority = iRecursive;	
	pVol->m_eRenderType = TYPE_TRUE;

	WVOL wvol;
	witer wit = pVol->m_VoltList.begin();
	while(wit != pVol->m_VoltList.end())
	{
		wvol = *wit++;
		if ((wvol.pPvsVol->m_iPriority == -1) || (wvol.pPvsVol->m_iPriority>1))
			wvol.pPvsVol->m_iPriority = iRecursive+1;	
		wvol.pPvsVol->m_eRenderType = TYPE_TRUE;
	}
}

void CPVSManager::DebugFunc()
{
	TRACE("Trace Start!!!\n");

	CPortalVolume* pVol = NULL;

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		if (pVol->m_eRenderType == TYPE_TRUE)
		{
			TRACE2("Rendering Volume iPriority %d, ID %d\n", pVol->m_iPriority, pVol->m_iID);
		}
	}
}

bool CPVSManager::Save(HANDLE hFile)
{
	DWORD dwNum;
	std::string strSrc;

	// Version..
	WriteFile(hFile, &ciVersion, sizeof(int), &dwNum, NULL);	

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	WriteCryptographString(hFile, pFrm->m_strFileName);

	COrganizeView* pView = pFrm->GetOrganizeView();
	int iT = pView->GetDlgItemInt(IDC_TOTAL_MOVE_X);
	WriteFile(hFile, &iT, sizeof(int), &dwNum, NULL);	
	iT = pView->GetDlgItemInt(IDC_TOTAL_MOVE_Y);
	WriteFile(hFile, &iT, sizeof(int), &dwNum, NULL);	
	iT = pView->GetDlgItemInt(IDC_TOTAL_MOVE_Z);
	WriteFile(hFile, &iT, sizeof(int), &dwNum, NULL);	

	// 전체 Shape.. 
	int iCount  = m_plShapeInfoList.size();
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);	

	ShapeInfo*	pSI = NULL;	
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;
		WriteFile(hFile, &pSI->m_iID, sizeof(int), &dwNum, NULL);
		WriteCryptographString(hFile, pSI->m_strShapeFile);

		// Shape의 데이터 저장..
		WriteFile(hFile, &pSI->m_iBelong, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iEventID, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iEventType, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iNPC_ID, sizeof(int), &dwNum, NULL);	
		WriteFile(hFile, &pSI->m_iNPC_Status, sizeof(int), &dwNum, NULL);	
		pSI->Save(hFile);
	}	

	// Total Count..
	WriteFile(hFile, &m_iTotalCount, sizeof(int), &dwNum, NULL);	
	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		// 자신의 데이터 저장..
		pVol = *it++;
		pVol->Save(hFile, m_bGameData);
	}

	// Floor..
	FloorInfo FInfo;
	iCount = pFrm->m_FloorList.size();
	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);	

	vfiter vfit = pFrm->m_FloorList.begin();
	while(vfit != pFrm->m_FloorList.end())
	{
		FInfo = *vfit++;
		WriteFile(hFile, &FInfo.m_iFloor, sizeof(int), &dwNum, NULL);					
		iCount = FInfo.m_vVolume.size();			
		WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);	
		for (int i = 0; i < iCount; i++)
			WriteFile(hFile, &FInfo.m_vVolume[i]->m_iID, sizeof(int), &dwNum, NULL);								
	}

	return true;
}

bool CPVSManager::LoadOldVersion(HANDLE hFile, int iVersionFromData)
{
	//..

	return true;
}

bool CPVSManager::Load(HANDLE hFile)
{
	DWORD dwNum;
	int iT;

	ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);
	if (iT != ciVersion)
		return LoadOldVersion(hFile, iT);

	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	std::string strSrc = ReadDecryptString(hFile);
	if (pFrm->m_strFileName != strSrc)
	{
		if (pFrm->m_strFileName.size())
		{
			if (pFrm->m_pSceneSource)
			{
				delete pFrm->m_pSceneSource; 
				pFrm->m_pSceneSource = NULL;
			}
		}

		pFrm->m_strFileName = strSrc;
		pFrm->m_pSceneSource = new CN3Scene;
		pFrm->m_pSceneSource->m_szName = "SourceList";
		pFrm->m_pSceneSource->FileNameSet(strSrc);
		pFrm->LoadSourceObjects();
		pView->SetDlgItemText(IDC_EDIT_RESOURCE_NAME, strSrc.c_str());
	}

	if (!pFrm->m_pSceneSource)
	{
		AfxMessageBox("SourceList를 Load하지 못했습니다..Data는 로드되지 않을것 입니다..");
		return false;
	}

	if(m_bGameData)
		Decompile();

	ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);
	pView->SetDlgItemInt(IDC_TOTAL_MOVE_X, iT);
	ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);	
	pView->SetDlgItemInt(IDC_TOTAL_MOVE_Y, iT);
	ReadFile(hFile, &iT, sizeof(int), &dwNum, NULL);	
	pView->SetDlgItemInt(IDC_TOTAL_MOVE_Z, iT);

	m_iIncreseIndex = 0;
	m_iIncreseShapeIndex = 0;
	m_iTotalCount = 0;
	m_iCurIndex = -1;

	// 전체 Shape 갯수 로드..
	int iCount, iCount2;
	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	for (int i = 0; i < iCount; i++ )
	{
		ShapeInfo*	pSI = new ShapeInfo;
		ReadFile(hFile, &pSI->m_iID, sizeof(int), &dwNum, NULL);
		
		// 문자열 길이..
		strSrc = ReadDecryptString(hFile);
		pSI->m_strShapeFile = strSrc;

		// SourceList에서.. Shape의 Pointer를 연결한다..
		pSI->m_pShape = pFrm->m_pSceneSource->ShapeGetByFileName(strSrc);
		ASSERT(pSI->m_pShape);

		ReadFile(hFile, &pSI->m_iBelong, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iEventID, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iEventType, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iNPC_ID, sizeof(int), &dwNum, NULL);	
		ReadFile(hFile, &pSI->m_iNPC_Status, sizeof(int), &dwNum, NULL);	
		pSI->Load(hFile);
		m_plShapeInfoList.push_back(pSI);
	}

	// Total Count.. 
	ReadFile(hFile, &m_iTotalCount, sizeof(int), &dwNum, NULL);

	CPortalVolume* pVol = NULL, *pVolTo = NULL;
	int iID;

	for( i = 0; i < m_iTotalCount; i++ )
	{
		ReadFile(hFile, &iID, sizeof(int), &dwNum, NULL);
		if (m_iIncreseIndex < (iID+1))
			m_iIncreseIndex = iID+1;
		pVol = m_Factory.CreatePvsVol(iID);
		pVol->Load(hFile, m_bGameData);
		m_pPvsList.push_back(pVol);
	}

	// 링크된 아이디들을 읽어서 연결시킴..
	WVID wvid;
	WVOL wvol;
	IDAndPriority IDAP;
	VisPortalPriority vPP;

	iter it = m_pPvsList.begin();
	idapiter idapit;
	while(it != m_pPvsList.end())
	{
		pVol = *it++;
		
		wviter wvit = pVol->m_piIDList.begin();
		while(wvit != pVol->m_piIDList.end())
		{
			wvid = *wvit++;
			wvol.ePWT = wvid.ePWT;
			wvol.pPvsVol = GetPortalVolPointerByID(wvid.iID);
			pVol->m_VoltList.push_back(wvol);
		}

		pVol->m_piIDList.clear();

		if(m_bGameData)
		{
			idapit = pVol->m_piVisibleIDList.begin();
			while (idapit != pVol->m_piVisibleIDList.end())
			{
				 IDAP = *idapit++;
				 pVolTo = GetPortalVolPointerByID(IDAP.m_iID);
				 pVolTo->m_eRenderType = TYPE_TRUE;
				 vPP.m_pVol = pVolTo;
				 vPP.m_iPriority = IDAP.m_iPriority;
				 pVol->m_pVisiblePvsList.push_back(vPP);
			}

			pVol->m_piVisibleIDList.clear();
		}
	}

	if (m_bGameData)
	{
		it = m_pPvsList.begin();
		while(it != m_pPvsList.end())
		{
			pVol = *it++;	
			pVol->SetState(STATE_NONE);
			pVol->m_eRenderType = TYPE_UNKNOWN;
		}
	}

	// Floor..
	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);

	for (i = 0; i < iCount; i++)
	{
		FloorInfo FInfo;
		ReadFile(hFile, &FInfo.m_iFloor, sizeof(int), &dwNum, NULL);		
		ReadFile(hFile, &iCount2, sizeof(int), &dwNum, NULL);
		for (int j = 0; j < iCount2; j++)
		{
			ReadFile(hFile, &iID, sizeof(int), &dwNum, NULL);											
			pVol = GetPortalVolPointerByID(iID);
			FInfo.m_vVolume.push_back(pVol);
		}
		pFrm->m_FloorList.push_back(FInfo);
	}

	SetMaxShapeIndex();
	pView->RefreshFloorCombo();

	return true;
}

void CPVSManager::SetMaxShapeIndex()
{
	CPortalVolume* pVol = NULL;
	ShapeInfo*	pSI = NULL;	

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;			
		if (m_iIncreseShapeIndex < (pSI->m_iID+1))
			m_iIncreseShapeIndex = pSI->m_iID+1;
	}

	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;
		
		siit = pVol->m_plShapeInfoList.begin();
		while( siit != pVol->m_plShapeInfoList.end() )
		{
			pSI = *siit++;			
			if (m_iIncreseShapeIndex < (pSI->m_iID+1))
				m_iIncreseShapeIndex = pSI->m_iID+1;
		}
	}
}

CPortalVolume* CPVSManager::GetPortalVolPointerByID(int iID)
{
	CPortalVolume* pVol = NULL;

	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		// 자신의 데이터 저장..
		pVol = *it++;
		if (pVol->m_iID == iID)
			return pVol;
	}

	ASSERT(0);
	return NULL;
}

#define CRY_KEY 0x0816

void CPVSManager::WriteCryptographString(HANDLE hFile, std::string strSrc)
{
	DWORD dwNum;

	int iCount = strSrc.size();
	std::vector<char> buffer;

	for( int i = 0; i < iCount; i++)
		buffer.push_back( ((int)strSrc[i] ^ CRY_KEY) );

	WriteFile(hFile, &iCount, sizeof(int), &dwNum, NULL);
	WriteFile(hFile, buffer.begin(), iCount, &dwNum, NULL);
}

std::string CPVSManager::ReadDecryptString(HANDLE hFile)
{
	DWORD dwNum;
	int iCount;

	ReadFile(hFile, &iCount, sizeof(int), &dwNum, NULL);
	std::vector<char> buffer(iCount);

	ReadFile(hFile, &(buffer[0]), iCount, &dwNum, NULL);				// string
	for( int i = 0; i < iCount; i++)
		buffer[i] ^= CRY_KEY;
	buffer.push_back((char)0x00);

	std::string strDest;
	strDest = buffer.begin();
	
	return strDest;
}

CN3Shape* CPVSManager::GetShapeByManager(std::string szStr)
{
	ShapeInfo*	pSI = NULL;	

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;			
		if (pSI->m_pShape->FileName() == szStr)
			return pSI->m_pShape;
	}

	ASSERT(0);
	return NULL;
}

int CPVSManager::GetIndexByVolumePointer(CPortalVolume* const pVol)
{
	int iIndex = 0;

	CPortalVolume* pVolu = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVolu = *it++;	
		if (pVolu == pVol)
			return iIndex;
		iIndex++;
	}

	return iIndex;
}

ShapeInfo* CPVSManager::GetShapeInfoByManager(int iID)
{
	ShapeInfo*	pSI = NULL;	

	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;			
		if (pSI->m_iID == iID)
			return pSI;
	}

	ASSERT(0);
	return NULL;
}

bool CPVSManager::IsValidPortalVolume(CPortalVolume* pVol)
{
	CPortalVolume* pVol2 = NULL;
	iter it = m_pPvsList.begin();

	while(it != m_pPvsList.end())
	{
		pVol2 = *it++;	
		if (pVol == pVol2)
			return true;
	}
	
	return false;
}

CPortalVolume* CPVSManager::PickCollision(float fx, float fy)
{
	CMainFrame* pFrm = (CMainFrame* )AfxGetMainWnd();
	COrganizeView* pView = pFrm->GetOrganizeView();

	// Compute the vector of the pick ray in screen space
	__Vector3 vTmp;
	vTmp.x =  ( ( ( 2.0f * fx ) / (CN3Base::s_CameraData.vp.Width) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._11;
	vTmp.y = -( ( ( 2.0f * fy ) / (CN3Base::s_CameraData.vp.Height) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._22;
	vTmp.z =  1.0f;

	// Transform the screen space pick ray into 3D space
	__Matrix44* pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
	__Vector3 vDir;
	vDir.x  = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
	vDir.y  = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
	vDir.z  = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
	__Vector3 vPos = pMtxVI->Pos(), vPick;

	CPortalVolume* pVol = NULL, *pVolNe = NULL;
	iter it;

	float fDistMax = FLT_MAX, fDT;

	it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;	
		if(pVol->PickWithVolume(vPos, vDir, &vPick))		// 양면 체크 할까하다가.. 
		{
			fDT = (vPick - CN3Base::s_CameraData.vEye).Magnitude();
			if (fDT <= fDistMax)
			{
				fDistMax = fDT;
				pVolNe = pVol;
			}
		}
	}

	return pVolNe;

	// 먼저 Volume 갯수만큼 돌면서.. Volume에 링큳된 Shape들을 먼저 검사한다..	나중에 구현..
}

void CPVSManager::UpdatePosAll(float fx, float fy, float fz)
{
	m_vBackupTotalMove;

	CPortalVolume* pVol = NULL;
	iter it = m_pPvsList.begin();
	while(it != m_pPvsList.end())
	{
		pVol = *it++;
		pVol->UpdatePosAll(pVol->Pos().x + fx - m_vBackupTotalMove.x, pVol->Pos().y + fy - m_vBackupTotalMove.y, pVol->Pos().z + fz - m_vBackupTotalMove.z);
	}

	ShapeInfo*	pSI = NULL;	
	siiter siit = m_plShapeInfoList.begin();
	while( siit != m_plShapeInfoList.end() )
	{
		pSI = *siit++;	
		pSI->PosSet(pSI->Pos().x + fx - m_vBackupTotalMove.x, pSI->Pos().y + fy - m_vBackupTotalMove.y, pSI->Pos().z + fz - m_vBackupTotalMove.z);
	}

	m_vBackupTotalMove.Set(fx, fy, fz);
}










