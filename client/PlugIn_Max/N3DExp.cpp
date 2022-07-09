/**********************************************************************
 *<
	FILE: CN3DExp.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/

#include "../N3Base/StdAfxBase.h"
#include "../N3Base/N3PMeshCreate.h"
#include "N3DExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HINSTANCE		g_hInst;
Interface*		g_pIntf;
ExpInterface*	g_pExpIntf;
CN3EngTool		g_Eng;

static CN3DExpClassDesc CN3DExpDesc;
ClassDesc* GetCN3DExpDesc() {return &CN3DExpDesc;}

//--- CN3DExp -------------------------------------------------------
__EXPORT_OPTION CN3DExp::m_Option;

CN3DExp::CN3DExp()
{
	this->Init();

	// allocation CN3Eng, CN3Scene | Init Engine
//	if(NULL == g_Eng.s_lpD3DDev)
//	{
//		g_Eng.Init(TRUE, NULL, 64, 64, 0, FALSE);
//	}
	m_pScene = NULL;
}

CN3DExp::~CN3DExp() 
{
}

void CN3DExp::Init()
{
	memset(&m_Option, 0, sizeof(m_Option));
	m_Option.bExportCamera		= TRUE;
	m_Option.bExportLight		= TRUE;
	m_Option.bExportGeometry	= TRUE;
	m_Option.bExportDummy		= FALSE;
	m_Option.bExportCharacter	= TRUE;

	m_Option.bAnimationKey	= TRUE;
	
	m_Option.bGenerateSmoothNormal = TRUE;
//	m_Option.bGenerateProgressiveMesh = FALSE;
	m_Option.bGenerateHalfSizeTexture = FALSE;
	m_Option.bGenerateCompressedTexture = TRUE;

	m_Option.fSamplingRate = 30.0f;

	m_nNodeCur = 0;
	m_nNodeCount = 0;

	lstrcpy(m_szPath, "");		// Path Name
	lstrcpy(m_szFileName, "");	// File Name
}

int CN3DExp::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *CN3DExp::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("n3scene");
}

const TCHAR *CN3DExp::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("NoahSystem N3Scene File");
}
	
const TCHAR *CN3DExp::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("Noah N3Scene");
}

const TCHAR *CN3DExp::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("NoahSystem");
}

const TCHAR *CN3DExp::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("NoahSystem Exporter");
}

const TCHAR *CN3DExp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *CN3DExp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int CN3DExp::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 200;
}

void CN3DExp::ShowAbout(HWND hWnd)
{			
	// Optional
}

int CN3DExp::DoExport(const TCHAR *szFileName, ExpInterface* pExpIntf, Interface *pIntf, BOOL suppressPrompts, DWORD dwOptions)
{
	// Interface Object Pointer
	g_pIntf	= pIntf; 
	g_pExpIntf = pExpIntf;
	if(NULL == g_pIntf || NULL == pExpIntf) return 0;

	// allocation CN3Eng, CN3Scene | Init Engine
	if(NULL == g_Eng.s_lpD3DDev) g_Eng.Init(TRUE, pIntf->GetMAXHWnd(), 64, 64, 0, FALSE);

	if(g_Eng.s_lpD3DDev == NULL)
	{
		// case failure
		MessageBox(::GetActiveWindow(), "Insufficient videoMemory or Not installed DirectX8", "Data Export Error", MB_OK);
		return 0;
	}
	
	this->Init();	// Init all variable

	delete m_pScene; 
	m_pScene =	new CN3Scene();
	m_pScene->ReleaseResrc();
	lstrcpy(m_szFileName, szFileName);	// Set File Name

	// Option Dialog 
	int rval = DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_EXPORT_OPTION), pIntf->GetMAXHWnd(), DlgProcExportOption, 0); // 컨트롤 패널 대화상자..
	if(rval != 1) return TRUE;

	m_bCancelExport = FALSE; // Export possible

	m_pScene->Release(); // 모두 해제하고..

	m_pScene->m_fFrmCur		= m_Option.nFrmStart; //모르겠다. 그냥 첫프레임으로 한다.
	m_pScene->m_fFrmStart	= m_Option.nFrmStart;
	m_pScene->m_fFrmEnd		= m_Option.nFrmEnd;

	INode* pNodeRoot = g_pIntf->GetRootNode(); // Root Node Pointer
	// ProgressBar 초기화..
	m_nNodeCount = 0;
	this->CountAllNodes(pNodeRoot, m_nNodeCount);

	g_pIntf->ProgressStart(GetString(IDS_PROGRESS_MSG), TRUE, DlgProcProgress, NULL); // 프로그레스 바 설정
	this->ProcessRecursive(pNodeRoot); // 차일드 노드를 순환하면서 Export.....
	g_pIntf->ProgressEnd();

	/////////////////////////////////
	// Scene Save

	// sub directory가 있다면 sub directory 만들기
	char szDir[_MAX_DIR]="";
	::CreateDirectory(m_Option.szSubDir, NULL);
	wsprintf(szDir, "%sData", m_Option.szSubDir);	::CreateDirectory(szDir, NULL);
	wsprintf(szDir, "%sChr", m_Option.szSubDir);	::CreateDirectory(szDir, NULL);
	wsprintf(szDir, "%sObject", m_Option.szSubDir);	::CreateDirectory(szDir, NULL);
	wsprintf(szDir, "%sItem", m_Option.szSubDir);	::CreateDirectory(szDir, NULL);

	// 만약 카메라가 하나도 없다면..
	if(m_pScene->CameraCount() <= 0) m_pScene->DefaultCameraAdd();
	if(m_pScene->LightCount() <= 0) m_pScene->DefaultLightAdd();
	m_pScene->SaveDataAndResourcesToFile(szFileName); // Scene 파일 저장 및 리소스 도 모두 저장..
	m_pScene->Release();

	m_pScene->ReleaseResrc();
	delete m_pScene;
	m_pScene = NULL;

	return TRUE;
}
	
bool CN3DExp::ProcessRecursive(INode* pNode)
{
	if(m_bCancelExport == TRUE) return false;
	if(NULL == pNode) return false;
	const char* szName = pNode->GetName(); // 이름..

	// 프로세스 업데이트 (!?)
//	g_pIntf->ProgressUpdate((m_nNodeCur)*100/m_nNodeCount, TRUE, pNode->GetName()); 
	m_nNodeCur++;

	// 어떤 객체인지 읽어온다. (camera? light? geom object?..)
	Object* pObj = pNode->EvalWorldState(m_Option.nFrmStart * 160).obj; 

	if(pObj)
	{
		DWORD dwSCID = pObj->SuperClassID();
		Class_ID cID = pObj->ClassID();

		if(	GEOMOBJECT_CLASS_ID == dwSCID && TRUE == m_Option.bExportGeometry) // Geometry Object
		{
			// 실제 Export Routine
			bool bRootBiped = false;
			if(	m_Option.bExportCharacter && IsBone(pNode) ) // 캐릭터 출력 옵션이 선택되어 있고.. 본이면...
			{
				Control* pCtrl = pNode->GetTMController();
				if(pCtrl) // 먼저 루트 바이패드인지 검사하고...
				{
					if(pCtrl->ClassID() == BIPBODY_CONTROL_CLASS_ID)
					{
						bRootBiped = true;
					}
//					else if(pCtrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) // 나머지는 무시한다. 
//					else if(pCtrl->ClassID() == FOOTPRINT_CLASS_ID) 
				}
				if(false==bRootBiped)
				{
					INode* pParentNode = pNode->GetParentNode();
					if (pParentNode && FALSE == IsBone(pParentNode)) bRootBiped = true;	// 본중에서 맨 상위 본이면
				}
			}

			if(bRootBiped) // 컨트롤러가 바이페드 객체이면..
			{
				return this->ProcessChr(pNode); // 뼈대 처리...
			}
			else
			{				
				return this->ProcessShape(pNode); // 그냥 오브젝트로 출력..
			}
		}
		else if(CAMERA_CLASS_ID == dwSCID && TRUE == m_Option.bExportCamera)
			return this->ProcessCamera(pNode); 
		else if(LIGHT_CLASS_ID == dwSCID && TRUE == m_Option.bExportLight)
			return this->ProcessLight(pNode);
//		else if(HELPER_CLASS_ID == dwSCID && TRUE == m_Option.bExportDummy)
//			return this->ProcessShape(pNode);
		else
		{
			char szDebug[512]; 
			wsprintf(szDebug, "Node : %s, 무시합니다.\n", pNode->GetName());
			OutputDebugString(szDebug);
		}
	}
	else
	{
		char szDebug[512]; 
		wsprintf(szDebug, "Node : %s, Type 변환이 불가능한 Node 입니다. 무시합니다.\n", pNode->GetName());
		OutputDebugString(szDebug);
	}

	int nChildCount = pNode->NumberOfChildren();
	INode* pNodeChild = NULL;
	for(int i = 0; i < nChildCount; i++)
	{
		pNodeChild = pNode->GetChildNode(i);
		if(pNodeChild==NULL) continue;

		this->ProcessRecursive(pNodeChild);
	}

	return false;
}

bool CN3DExp::ProcessCamera(INode* pNode)
{
	// Object의 pipeline 정보를 읽어온다. ObjectState 클래스에 저장한다. 
	ObjectState os = pNode->EvalWorldState(m_Option.nFrmStart * 160);
	if(CAMERA_CLASS_ID != os.obj->SuperClassID()) return false;

	CN3Camera* pCamera = new CN3Camera;
	m_pScene->CameraAdd(pCamera);
	this->ProcessTransform(pNode, pCamera, false); // 위치 에니메이션 키..
	pCamera->m_KeyRot.Release(); // At Vector Key
	pCamera->AtPosSet(0,0,0);
	pCamera->m_KeyScale.Release(); // Up Vector Key
	pCamera->UpVectorSet(0,1,0);

	INode* pTN = pNode->GetTarget(); // At Vector Key 및 처리..
	if(pTN)
	{
		CN3Transform TTrans;
		this->ProcessTransform(pTN, &TTrans, false);

		pCamera->AtPosSet(TTrans.Pos());
		pCamera->m_KeyRot.Add(TTrans.m_KeyPos, 0, TTrans.m_KeyPos.Count()); // Animation Key 처리..
	}

	
	// Camera Object 에 읽어온 ObjectState의 오브젝트를 대입(?)
	CameraObject* pCmObj = (CameraObject*)os.obj;

	CameraState CS;
	Interval valid = FOREVER;
	pCmObj->EvalCameraState(m_Option.nFrmStart * 160, valid, &CS);

	if(CS.manualClip)
	{
		pCamera->m_Data.fNP = CS.hither;
		pCamera->m_Data.fFP = CS.yon;
	}
	else
	{
		pCamera->m_Data.fNP = 0.3f;
		pCamera->m_Data.fFP = 256.0f;
	}
	pCamera->m_Data.fFOV = CS.fov;

	return TRUE;
}

bool CN3DExp::ProcessLight(INode* pNode)
{
	if(NULL == pNode) return false;

	CN3Light* pLight = new CN3Light;
	m_pScene->LightAdd(pLight);

	pLight->m_szName = "";
	this->ProcessName(pNode, pLight);
	this->ProcessTransform(pNode, pLight, false);

	ObjectState os = pNode->EvalWorldState(m_Option.nFrmStart * 160);

	// 라이트 오브젝트를 가져온다.
	GenLight* pLightObj = (GenLight*)os.obj;
	struct LightState ls;
	Interval valid = FOREVER;
//	Interval animRange = ip->GetAnimRange();
	pLightObj->EvalLightState(m_Option.nFrmStart * 160, valid, &ls);

	// This is part os the lightState, but it doesn't
	// make sense to output as an animated setting so
	// we dump it outside of ExportLightSettings()

//	Point3 ptLight = pNode->GetNodeTM(m_Option.nFrmStart * 160).GetTrans();
	Point3 ptLight = pNode->GetObjTMAfterWSM(m_Option.nFrmStart * 160).GetTrans();
	D3DCOLORVALUE __dcv;
	__dcv.r = ls.color.r;
	__dcv.g = ls.color.g;
	__dcv.b = ls.color.b;
	__dcv.a = 0;

	// 방향 구하기
	INode* pTN = pNode->GetTarget();
	__Vector3 vDir(0,-1,0);
	if(pTN)
	{	// 타겟이 있을경우 타겟의 좌표와 light의 좌표 차를 가지고 방향을 정한다.
		CN3Transform TTrans;
		this->ProcessTransform(pTN, &TTrans, false);
		vDir = TTrans.Pos() - D3DXVECTOR3(ptLight.x, ptLight.z, ptLight.y);
	}
	else if (OMNI_LGT != ls.type)
	{	// 타겟이 없을 경우 light의 rotation값을 가지고 방향을 정한다.
		Matrix3 mtxLight = pNode->GetObjTMAfterWSM(m_Option.nFrmStart * 160);	// world좌표상의 transform matrix를 구한다.
		mtxLight.NoTrans(); mtxLight.NoScale();	// 위치와 스케일 변화를 없앤다.
		Point3 ptAt = mtxLight.PointTransform(Point3(0,0,-1));	// 기본 벡터를 matrix와 곱하면 light의 방향 벡터가 나온다.
		vDir.Set(ptAt.x, ptAt.z, ptAt.y);	// z와 y를 바꾸어준다.
	}
	vDir.Normalize();

	// 라이트 종류
	memset(&pLight->m_Data, 0, sizeof(pLight->m_Data));
	switch(ls.type)
	{
		//case OMNI_LIGHT:
		case OMNI_LGT:
			if (ls.useAtten) pLight->m_Data.InitPoint( m_pScene->LightCount() - 1, D3DXVECTOR3(ptLight.x, ptLight.z, ptLight.y), __dcv, ls.attenEnd);
			else pLight->m_Data.InitPoint( m_pScene->LightCount() - 1, D3DXVECTOR3(ptLight.x, ptLight.z, ptLight.y), __dcv);
			break;
		//case FSPOT_LIGHT:
		//case TSPOT_LIGHT:
		case SPOT_LGT:
			if (ls.useAtten) 
				pLight->m_Data.InitSpot( m_pScene->LightCount() - 1, D3DXVECTOR3(ptLight.x, ptLight.z, ptLight.y), vDir, __dcv, 
									D3DXToRadian(ls.hotsize), D3DXToRadian(ls.fallsize), ls.attenEnd);
			else
				pLight->m_Data.InitSpot( m_pScene->LightCount() - 1, D3DXVECTOR3(ptLight.x, ptLight.z, ptLight.y), vDir, __dcv, 
									D3DXToRadian(ls.hotsize), D3DXToRadian(ls.fallsize));
			break;
		//case DIR_LIGHT:
		case DIRECT_LGT:
			{
				pLight->m_Data.InitDirection(m_pScene->LightCount() - 1, vDir, __dcv);
			}
			break;
		default:
			pLight->m_Data.Type = D3DLIGHT_FORCE_DWORD;
			break;
	}

	// 켜있나 꺼져있나 정해준다.
	pLight->m_Data.bOn = ls.on;
	
	return true;
}

bool CN3DExp::ProcessShape(INode* pNode)
{
	if(NULL == pNode) return false;

	CN3IMesh N3IMesh;
	if(this->ProcessIMesh(pNode, &N3IMesh) == false) return false;
	__Material mtl;
	mtl.Init();
	CN3Texture* pTex = NULL;
	this->ProcessMaterial(pNode, &mtl, &pTex, "Object\\");
	if(pTex)
	{
//		char szDrv[_MAX_DIR], szDir[_MAX_DIR], szFName[_MAX_FNAME];
//		_splitpath(pTex->FileName().c_str(), szDrv, szDir, szFName, NULL);
//		char szFN[256]; wsprintf(szFN, "Object\\%s.dxt", szFName);
		pTex->FileNameSet(pTex->FileName());
	}


	CN3Shape* pShape = NULL;
	INode* pPN = pNode->GetParentNode();
	Object* pPObj = NULL;
	Class_ID cPID = Class_ID(0, 0);
	if(pPN) pPObj = pPN->EvalWorldState(m_Option.nFrmStart * 160).obj;
	if(pPObj) cPID = pPObj->ClassID();

	if(pPN && cPID == Class_ID(DUMMY_CLASS_ID,0)) // Parent Node 가 있으면..
	{
		CN3Shape ShpTmp;
		ProcessName(pPN, &ShpTmp);

		int nSC = m_pScene->ShapeCount();
		for(int i = 0; i < nSC; i++)
		{
			CN3Shape* pShpTmp = m_pScene->ShapeGet(i);
			if(ShpTmp.m_szName == pShpTmp->m_szName) // 그룹노드의 이름이 같은 것이 있으면..
			{
				pShape = pShpTmp; // 바로 이 Shape 이다..
				break;
			}
		}
	}

	if(NULL == pShape) // Scene 에 이 노드의 그룹에 해당되는 Shape 가 없거나 혹은 단독으로 있는 메시이면..
	{
		pShape = new CN3Shape();
		Point3 ptPivot;
		if(pPN && cPID == Class_ID(DUMMY_CLASS_ID,0))
		{
			this->ProcessName(pPN, pShape);
			ptPivot = pPN->GetNodeTM(m_Option.nFrmStart * 160).GetTrans();
		}
		else
		{
			this->ProcessName(pNode, pShape);
			ptPivot = pNode->GetNodeTM(m_Option.nFrmStart * 160).GetTrans();
		}
//		pShape->PosSet(ptPivot.x * 0.0254f, ptPivot.z * 0.0254f, ptPivot.y * 0.0254f);  // Pivot 세팅.. y 와 z 를 반대로.
		pShape->PosSet(ptPivot.x, ptPivot.z, ptPivot.y);  // Pivot 세팅.. y 와 z 를 반대로.

		int nSI = m_pScene->ShapeAdd(pShape);
	}


	bool bCollision = false;
	::CharLower(N3IMesh.m_szName.begin()); // 소문자로 만들고..
	if(N3IMesh.m_szName.find("coll") != -1) // "collision" 이라는 문자열 확인 ..  그러나 오타에 대비해서 "coll" 까지만 확인
		bCollision = true;

	if(true == bCollision)
	{
		// Pivot, Offset 적용
		__Vector3 vOffset = pShape->Pos();
		N3IMesh.ApplyOffset(vOffset * -1.0f); // Offset 만큼 빼준다..
		
		CN3VMesh* pVMesh = new CN3VMesh();
		pVMesh->Import(&N3IMesh); // 메시 만들고.. Indexed 메시로부터 Import..

		std::string szVMeshFN = "Object\\" + pShape->m_szName + ".n3vmesh";  // 이름짓기..
		if (lstrlen(m_Option.szSubDir)>0) szVMeshFN = std::string(m_Option.szSubDir) + szVMeshFN;	// sub directory 있으면 추가
		pVMesh->FileNameSet(szVMeshFN); // 이름짓기..
		CN3Base::s_MngVMesh.Add(pVMesh); // 매니저에 넣고..

		pShape->CollisionMeshSet(szVMeshFN); // 충돌메시 세팅..
	}
	else // 충돌 체크 메시가 아니면 파트 추가..
	{
		// Part 추가.. Part Data 세팅..
		CN3SPart* pPD = pShape->PartAdd();
		this->ProcessName(pNode, pPD); // 파트 이름..

		// Pivot, Offset 적용
		Point3 ptPivot = pNode->GetNodeTM(m_Option.nFrmStart * 160).GetTrans();
	//	__Vector3 vOffset(ptPivot.x * 0.0254f, ptPivot.z * 0.0254f, ptPivot.y * 0.0254f);
		__Vector3 vOffset(ptPivot.x, ptPivot.z, ptPivot.y);
		N3IMesh.ApplyOffset(vOffset * -1.0f); // Offset 만큼 빼준다..
		
		CN3Mesh MeshTmp;
		MeshTmp.Import(&N3IMesh); // 임시로 메시 만들고.. Indexed 메시로부터 Import..
		if(m_Option.bGenerateSmoothNormal)
		{
			MeshTmp.ReGenerateSmoothNormal(); // Normal 값 생성..
		}
		MeshTmp.MakeIndexed(); // Indexed Mesh 로 만든다..

		// PMesh 생성..
		CN3PMeshCreate PMCreate;
		PMCreate.ConvertFromN3Mesh(&MeshTmp);
		CN3PMesh* pPMesh = PMCreate.CreateRendererMesh();
		this->ProcessName(pNode, pPMesh);
		pShape->s_MngPMesh.Add(pPMesh);

		pPD->MeshSet(pPMesh->FileName());
		if(pTex)
		{
			pPD->TexAlloc(1); // Texture 할당..
			pPD->TexSet(0, pTex->FileName());
		}
		pPD->m_Mtl = mtl;
		pPD->m_vPivot = vOffset - pShape->Pos(); // Pivot point 를 얻고..
		pPD->ReCalcMatrix(pShape->m_Matrix); // 행렬 다시 계산..
	}

	return true;
}

BOOL CALLBACK CN3DExp::DlgProcExportOption(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWndDlg, GetParent(hWndDlg));

			// Registry 에 옵션값을 넣어두었다..
			HKEY hKey = g_Eng.RegistryOpen("N3Export Option");
			if(hKey) 
			{
				g_Eng.RegistryValueGet(hKey, "Export Camera", &m_Option.bExportCamera, 4);
				g_Eng.RegistryValueGet(hKey, "Export Light", &m_Option.bExportLight, 4);
				g_Eng.RegistryValueGet(hKey, "Export Geometry", &m_Option.bExportGeometry, 4);
				g_Eng.RegistryValueGet(hKey, "Export Dummy", &m_Option.bExportDummy, 4);
				g_Eng.RegistryValueGet(hKey, "Export Character", &m_Option.bExportCharacter, 4);

				g_Eng.RegistryValueGet(hKey, "Animation Key", &m_Option.bAnimationKey, 4);
				g_Eng.RegistryValueGet(hKey, "Generate Smooth Normal", &m_Option.bGenerateSmoothNormal, 4);
//				g_Eng.RegistryValueGet(hKey, "Generate Progressive Mesh", &m_Option.bGenerateProgressiveMesh, 4);
				g_Eng.RegistryValueGet(hKey, "Generate Half Size Texture", &m_Option.bGenerateHalfSizeTexture, 4);
				g_Eng.RegistryValueGet(hKey, "Generate Compressed Texture", &m_Option.bGenerateCompressedTexture, 4);
				
				g_Eng.RegistryClose(hKey);
			}

			CheckDlgButton(hWndDlg, IDC_C_OBJ_CAMERA, m_Option.bExportCamera);
			CheckDlgButton(hWndDlg, IDC_C_OBJ_LIGHT, m_Option.bExportLight);
			CheckDlgButton(hWndDlg, IDC_C_OBJ_GEOMETRY, m_Option.bExportGeometry);
			CheckDlgButton(hWndDlg, IDC_C_OBJ_DUMMY, m_Option.bExportDummy);
			CheckDlgButton(hWndDlg, IDC_C_OBJ_CHARACTER, m_Option.bExportCharacter);

			CheckDlgButton(hWndDlg, IDC_C_ANIMATION_KEY, m_Option.bAnimationKey);

			CheckDlgButton(hWndDlg, IDC_C_GENERATE_SMOOTH_NORMAL, m_Option.bGenerateSmoothNormal);
//			CheckDlgButton(hWndDlg, IDC_C_GENERATE_PROGRESSIVE_MESH, m_Option.bGenerateProgressiveMesh);
			CheckDlgButton(hWndDlg, IDC_C_GENERATE_HALF_SIZE_TEXTURE, m_Option.bGenerateHalfSizeTexture);
			CheckDlgButton(hWndDlg, IDC_C_GENERATE_COMPRESSED_TEXTURE, m_Option.bGenerateCompressedTexture);

			Interval ii = g_pIntf->GetAnimRange();
			m_Option.nFrmStart = ii.Start() / 160; // 시작 프레임.
			m_Option.nFrmEnd = ii.End() / 160; // 끝 프레임

			SetDlgItemInt(hWndDlg, IDC_E_FRAME_START, m_Option.nFrmStart, FALSE);
			SetDlgItemInt(hWndDlg, IDC_E_FRAME_END, m_Option.nFrmEnd, FALSE);
			SetDlgItemInt(hWndDlg, IDC_E_SAMPLING_RATE, (int)(m_Option.fSamplingRate), FALSE);

			SetDlgItemText(hWndDlg, IDC_E_SUBDIR, "");
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			{
				memset(&m_Option, 0, sizeof(m_Option));

				m_Option.bExportCamera = IsDlgButtonChecked(hWndDlg, IDC_C_OBJ_CAMERA);
				m_Option.bExportLight = IsDlgButtonChecked(hWndDlg, IDC_C_OBJ_LIGHT);
				m_Option.bExportGeometry = IsDlgButtonChecked(hWndDlg, IDC_C_OBJ_GEOMETRY);
				m_Option.bExportDummy = IsDlgButtonChecked(hWndDlg, IDC_C_OBJ_DUMMY);
				m_Option.bExportCharacter = IsDlgButtonChecked(hWndDlg, IDC_C_OBJ_CHARACTER);

				m_Option.bAnimationKey = IsDlgButtonChecked(hWndDlg, IDC_C_ANIMATION_KEY);
				m_Option.nFrmStart = GetDlgItemInt(hWndDlg, IDC_E_FRAME_START, NULL, FALSE); // 시작 프레임
				m_Option.nFrmEnd = GetDlgItemInt(hWndDlg, IDC_E_FRAME_END, NULL, FALSE); // 끝 프레임
				m_Option.fSamplingRate = GetDlgItemInt(hWndDlg, IDC_E_SAMPLING_RATE, NULL, FALSE); // Sampling Rate

				m_Option.bGenerateSmoothNormal = IsDlgButtonChecked(hWndDlg, IDC_C_GENERATE_SMOOTH_NORMAL);
//				m_Option.bGenerateProgressiveMesh = IsDlgButtonChecked(hWndDlg, IDC_C_GENERATE_PROGRESSIVE_MESH);
				m_Option.bGenerateHalfSizeTexture = IsDlgButtonChecked(hWndDlg, IDC_C_GENERATE_HALF_SIZE_TEXTURE);
				m_Option.bGenerateCompressedTexture = IsDlgButtonChecked(hWndDlg, IDC_C_GENERATE_COMPRESSED_TEXTURE);

				GetDlgItemText(hWndDlg, IDC_E_SUBDIR, m_Option.szSubDir, _MAX_DIR);
				int iStrLen = lstrlen(m_Option.szSubDir);
				if (iStrLen>0)
				{
					if ('\\' != m_Option.szSubDir[iStrLen-1]) lstrcat(m_Option.szSubDir,"\\");
				}

				EndDialog(hWndDlg, 1);

				// Registry 에 옵션값을 넣어두었다..
				HKEY hKey = g_Eng.RegistryOpen("N3Export Option");
				if(NULL == hKey) RegCreateKey(HKEY_CURRENT_USER, "N3Export Option", &hKey);
				if(hKey) 
				{
					g_Eng.RegistryValueSet(hKey, "Export Camera", &m_Option.bExportCamera, 4);
					g_Eng.RegistryValueSet(hKey, "Export Light", &m_Option.bExportLight, 4);
					g_Eng.RegistryValueSet(hKey, "Export Geometry", &m_Option.bExportGeometry, 4);
					g_Eng.RegistryValueSet(hKey, "Export Dummy", &m_Option.bExportDummy, 4);
					g_Eng.RegistryValueSet(hKey, "Export Character", &m_Option.bExportCharacter, 4);

					g_Eng.RegistryValueSet(hKey, "Animation Key", &m_Option.bAnimationKey, 4);
					g_Eng.RegistryValueSet(hKey, "Generate Smooth Normal", &m_Option.bGenerateSmoothNormal, 4);
//					g_Eng.RegistryValueSet(hKey, "Generate Progressive Mesh", &m_Option.bGenerateProgressiveMesh, 4);
					g_Eng.RegistryValueSet(hKey, "Generate Half Size Texture", &m_Option.bGenerateHalfSizeTexture, 4);
					g_Eng.RegistryValueSet(hKey, "Generate Compressed Texture", &m_Option.bGenerateCompressedTexture, 4);
					
					g_Eng.RegistryClose(hKey);
				}
			}
			return TRUE;
		case IDCANCEL:
			{
				EndDialog(hWndDlg, 0);
				return TRUE;
			}
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
		return TRUE;
	default:
		break;
	}
	
	return FALSE;
}

void CN3DExp::DecodeTransformMatrix(Matrix3& matSrc, __Matrix44& matDest)
{
	// Decompose the matrix and dump the contents
	// Dump the whole Matrix
	AffineParts ap;
	decomp_affine(matSrc, &ap);

	float fAngle;
	Point3 vRot;

	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &fAngle, vRot);

	// 행렬...
	__Matrix44 m, tm;
	m.Identity();
	tm.Scale(ap.k.x, ap.k.z, ap.k.y); m *= tm; // 스케일
//	::D3DXMatrixRotationYawPitchRoll(&tm, vRot.z * fAngle, vRot.x * fAngle, vRot.y * fAngle); m *= tm; // 회전
	tm.RotationZ(fAngle * vRot.y); m *= tm; // 회전
	tm.RotationY(fAngle * vRot.z); m *= tm; // 회전
	tm.RotationX(fAngle * vRot.x); m *= tm; // 회전
//	m.PosSet(ap.t.x * 0.0254f, ap.t.z * 0.0254f, ap.t.y * 0.0254f); // 이동 // 1 Unit == 1 Inch -> Meter 로 바꾼다.. // Y, Z 는 반대로..
	m.PosSet(ap.t.x, ap.t.z, ap.t.y); // 이동 // 1 Unit == 1 Inch -> Meter 로 바꾼다.. // Y, Z 는 반대로..

	matDest = m; // 휴.. 변환 행렬 최종 완성..
}

void CN3DExp::CancelExport()
{
	m_bCancelExport = TRUE;
}

DWORD WINAPI CN3DExp::DlgProcProgress(LPVOID lpArg)
{
	return TRUE;
}

bool CN3DExp::ProcessName(INode* pNode, CN3BaseFileAccess* pBase)
{
	if(NULL == pBase) return false;
	DWORD dwType = pBase->Type();

	if(pNode && pNode->GetParentNode())
	{
		INode* pPN = pNode->GetParentNode(); // 나의 부모
		INode* pPPN = NULL;	// 나의 부모의 부모
		if(pPN) pPPN = pPN->GetParentNode();

		// 캐릭터 파트나 캐릭터 파트 스킨일 경우 내가 본 노드이면 나의 이름을 넣지 않는다.
		if ((dwType & (OBJ_CHARACTER_PART|OBJ_CHARACTER_PART_SKINS)) && IsBone(pNode))
		{
		}
		else
		{
			if(!(pBase->m_szName.empty()))
			{
				std::string szTmp = pNode->GetName(); szTmp += '_'; szTmp += pBase->m_szName;
				pBase->m_szName =  szTmp;
			}
			else pBase->m_szName = pNode->GetName();
		}

		if(NULL != pPPN) // Scene Root 가 아닐 경우에만..
		{
			this->ProcessName(pNode->GetParentNode(), pBase); // 재귀 호출
			return true;
		}
	}

	char szDir[32]	= "";
	char szExt[32]	= "";

	if(dwType & OBJ_SHAPE) { lstrcpy(szDir, "Object\\"); lstrcpy(szExt, ".N3Shape"); }
	else if(dwType & OBJ_SHAPE_PART) { lstrcpy(szDir, "Object\\"); lstrcpy(szExt, ".N3SPart"); }
	else if(dwType & OBJ_MESH) { lstrcpy(szDir, "Object\\"); lstrcpy(szExt, ".N3Mesh"); }
	else if(dwType & OBJ_MESH_PROGRESSIVE) { lstrcpy(szDir, "Object\\"); lstrcpy(szExt, ".N3PMesh"); }
	else if(dwType & OBJ_MESH_VECTOR3) { lstrcpy(szDir, "Object\\"); lstrcpy(szExt, ".N3VMesh"); }
	
	else if(dwType & OBJ_CHARACTER) { lstrcpy(szDir, "Chr\\"); lstrcpy(szExt, ".N3Chr"); }
	else if(dwType & OBJ_JOINT) { lstrcpy(szDir, "Chr\\"); lstrcpy(szExt, ".N3Joint"); }
	else if(dwType & OBJ_CHARACTER_PART) { lstrcpy(szDir, "Item\\"); lstrcpy(szExt, ".N3CPart"); }
	else if(dwType & OBJ_CHARACTER_PART_SKINS) { lstrcpy(szDir, "Item\\"); lstrcpy(szExt, ".N3CSkins"); }
	else if(dwType & OBJ_MESH_INDEXED) { lstrcpy(szDir, "Item\\"); lstrcpy(szExt, ".N3IMesh"); }
	else if(dwType & OBJ_SKIN) { lstrcpy(szDir, "Item\\"); lstrcpy(szExt, ".N3Skin"); }
	else if(dwType & OBJ_CHARACTER_PLUG) { lstrcpy(szDir, "Item\\"); lstrcpy(szExt, ".N3CPlug"); }
	
	else if(dwType & OBJ_TEXTURE) { lstrcpy(szDir, "Texture\\"); lstrcpy(szExt, ".DXT"); }
	else if(dwType & OBJ_SCENE) { lstrcpy(szDir, ""); lstrcpy(szExt, ".N3Scene"); }
	
	else
	{
		lstrcpy(szDir, "Data\\");
		if(dwType & OBJ_CAMERA) lstrcpy(szExt, ".N3Camera");
		else if(dwType & OBJ_LIGHT) lstrcpy(szExt, ".N3Light");
		else if(dwType & OBJ_TRANSFORM) lstrcpy(szExt, ".N3Transform");
		else if(dwType & OBJ_BASE) lstrcpy(szExt, ".N3Base");
		else lstrcpy(szExt, ".Unknown");
	}

	// sub directory 가 있으면 추가한다.
	std::string szFN;
	if (lstrlen(m_Option.szSubDir) > 0) szFN = std::string(m_Option.szSubDir) + szDir + pBase->m_szName + szExt;
	else szFN = szDir + pBase->m_szName + szExt;

	pBase->FileNameSet(szFN);
	
	return true;
}

bool CN3DExp::ProcessChr(INode *pNode)
{
	int i, nCC;
	INode* pNodeRootJoint = NULL; // 루트 조인트를 찾아야 한다..

	Control* pCtrl = pNode->GetTMController();
	if(pCtrl->ClassID() != BIPBODY_CONTROL_CLASS_ID)	// pNode가 바이패드가 아니면 pNode가 루트조인트다.
	{
		pNodeRootJoint = pNode;
	}//return false;
	else
	{	// pNode가 바이패드면 자식중에 루트조인트를 찾는다.

		nCC = pNode->NumberOfChildren();
		for(int i = 0; i < nCC; i++) 
		{
			INode* pNodeTmp = pNode->GetChildNode(i);
	//		Control* pCtrlTmp = pNodeTmp->GetTMController();
	//		if(NULL == pCtrlTmp) continue;
	//		const char* szJR = pNodeTmp->GetName();
	//		Class_ID cID = pCtrlTmp->ClassID();
	//		if(BIPSLAVE_CONTROL_CLASS_ID == cID) 
			if (IsBone(pNodeTmp))
			{
				pNodeRootJoint = pNodeTmp;
				break;
			}
		}
	}
	
	if(NULL == pNodeRootJoint) return false;

//	IBipedExport* pBE = (IBipedExport*)pCtrl->GetInterface(I_BIPINTERFACE); // biped export interface
//	pBE->RemoveNonUniformScale(1); // remove the non uniform scale
//	pBE->BeginFigureMode(1); // Figure Mode .. Binding pose 와 비슷
		
	CN3Joint* pJoint = new CN3Joint();
	if(false == this->ProcessJoint(pNodeRootJoint, pJoint)) // Joint 처리..
	{
		delete pJoint; pJoint = NULL;
//		pBE->EndFigureMode(1);
//		pCtrl->ReleaseInterface(I_BIPINTERFACE, pBE); // release biped export interface
		return false;
	}
	
	// 캐릭터 처리..
	CN3Chr* pChr = new CN3Chr();
	this->ProcessName(pNode, pChr);

	pChr->s_MngJoint.Add(pJoint);
	pChr->JointSet(pJoint->FileName());
	m_pScene->ChrAdd(pChr);

	pChr->m_szName = "Temp";
	pChr->FileNameSet(std::string("Chr\\Temp.n3Chr"));
	pChr->PartAlloc(64); // 충분하게 Part Data 할당.. save할때 불필요한 데이터는 제거된다.
	for(i = 0; i < 64; i++)
	{
		CN3CPart* pPDAdd = pChr->Part(i);
		CN3CPartSkins* pSkinAdd = new CN3CPartSkins();
		char szNameTmp[256];
		wsprintf(szNameTmp, "chr\\temp_%d.N3CSkins", i);
		pSkinAdd->FileNameSet(szNameTmp);
		CN3Base::s_MngSkins.Add(pSkinAdd);
		pPDAdd->SkinsSet(szNameTmp);
		CN3Base::s_MngSkins.Delete(&pSkinAdd); // 이렇게 해주어야 참조 카운트가 하나 줄어든다..
	}

	// Biped 에서 Editable Mesh 를 찾고..
	std::list<INode*> MeshList;
	this->FindNodeRecursive(pNode, Class_ID(EDITTRIOBJ_CLASS_ID, 0x00), MeshList);

	std::list<INode*>::iterator it;
	for(it = MeshList.begin(); it != MeshList.end(); it++) // Mesh 수만큼 처리..
	{
		INode* pNodeTmp = *it;

		// 충돌 체크용으로 쓰일 메시인지 살펴본다..
		bool bCollisionMesh = false;
		std::string szFNM = pNodeTmp->GetName();
		if(szFNM.size() > 0) CharLower(&(szFNM[0]));
		if(szFNM.find("coll") != -1) bCollisionMesh = true;

		if(true == bCollisionMesh) // 충돌 체크 메시면.... 
		{
		}
		else // if(false == bCollisionMesh) // 충돌 체크 메시 아니면....정상적으로 진행..
		{
			int nLOD = 0;
			int nPart = 0;
			INode* pMG = pNodeTmp->GetParentNode();	// Mesh Group node
			INode* pCG = NULL;						// Chr Group node
			if (pMG) pCG = pMG->GetParentNode();

			if (pCG == NULL || 
				(pMG && ( pMG == pNode  || IsBone(pMG)) ) )
				// 캐릭터 노드이거나 pMG가 본 노드일 경우, 이때는 LOD가 없어서 신체부위별로 그룹지어 있지 않을때이다.
			{
				std::string strM1(pNodeTmp->GetName());
				std::string strM2;
				nCC = pMG->NumberOfChildren();
				for(i = 0; i < nCC; i++)
				{
					INode* pCN = pMG->GetChildNode(i);
					__ASSERT(pCN, "null pointer : no child");
					if (!CheckObjectClassID(pCN, Class_ID(EDITTRIOBJ_CLASS_ID, 0x00))) continue;	// 메쉬가 아니면 넘어간다.
					strM2 = pCN->GetName();
					if(strM1 == strM2) break; // 이름이 같을 경우..
					++nPart; // 같지 않으면 Part 증가..
				}
			}
			else
			{	// LOD가 있어서 신체 부위별로 그룹 되어 있을경우 (pMG가 그룹이다)
				// part
				std::string strM1(pMG->GetName());
				std::string strM2;
				nCC = pCG->NumberOfChildren();
				for(i = 0; i < nCC; i++)
				{
					INode* pCN = pCG->GetChildNode(i);
					__ASSERT(pCN, "null pointer : no child");
					if (!CheckObjectClassID(pCN, Class_ID(DUMMY_CLASS_ID, 0x00))) continue;	// 그룹이 아니면 넘어간다.
					strM2 = pCN->GetName();
					if(strM1 == strM2) break; // 이름이 같을 경우..
					++nPart; // 같지 않으면 Part 증가..
				}

				// lod
				strM1 = pNodeTmp->GetName();
				nCC = pMG->NumberOfChildren();
				for(i = 0; i < nCC; i++)
				{
					INode* pCN = pMG->GetChildNode(i);
					__ASSERT(pCN, "null pointer : no child");
					if (!CheckObjectClassID(pCN, Class_ID(EDITTRIOBJ_CLASS_ID, 0x00))) continue;	// 메쉬가 아니면 넘어간다.
					strM2 = pCN->GetName();
					if(strM1 == strM2) break; // 이름이 같을 경우..
					++nLOD;	// 같지 않으면 LOD증가
				}
			}

			__ASSERT(nPart >= 0 && nPart < 64, "Part count is bigger than 64");
			__ASSERT(nLOD >= 0 && nLOD < MAX_CHR_LOD, "LOD Count is bigger than MAX_CHR_LOD");
			
			CN3CPart* pPart = pChr->Part(nPart);
			CN3CPartSkins* pSkins = pPart->Skins();
			
			CN3Skin* pSkin = pPart->Skin(nLOD);
			if(false == this->ProcessPhysique(pNodeTmp, pNodeRootJoint, pSkin)) // Skin 처리..
			{
				MessageBox(::GetActiveWindow(), pNodeTmp->GetName(), "Skin processing failed", MB_OK);
			}

			CN3Texture* pTex = NULL;
			this->ProcessMaterial(pNodeTmp, &(pPart->m_Mtl), &pTex, "Item\\");
			if(pTex)
			{
//				char szDrv[_MAX_DIR], szDir[_MAX_DIR], szFName[_MAX_FNAME];
//				_splitpath(pTex->FileName().c_str(), szDrv, szDir, szFName, NULL);
//				char szFN[256]; wsprintf(szFN, "Item\\%s.dxt", szFName);
//				pTex->FileNameSet(szFN);
				pPart->TexSet(pTex->FileName());
			}
			ProcessName(pNodeTmp, pPart);	// 이름 짓기
			ProcessName(pNodeTmp, pSkins);	// 이름 짓기
		}
	}

	pChr->Init(); // Initializing Skin, IMesh, .... Inverse Matrix....
	
//	pBE->EndFigureMode(1);
//	pCtrl->ReleaseInterface(I_BIPINTERFACE, pBE); // release biped export interface
//	pBE = NULL;
	g_pIntf->RedrawViews(0); // 결과를 본다.. Redraw if you want to see the result

//	IBipedExport* pBE = (IBipedExport*)pCtrl->GetInterface(I_BIPINTERFACE); // biped export interface
//	pBE->RemoveNonUniformScale(1); // remove the non uniform scale
//	pBE->BeginFigureMode(1); // Figure Mode .. Binding pose 와 비슷
//
//	
//	pBE->EndFigureMode(1);
//	g_pIntf->RedrawViews(0); // 결과를 본다.. Redraw if you want to see the result
//	pCtrl->ReleaseInterface(I_BIPINTERFACE, pBE); // release biped export interface

	// biped chr
/*	Control *c = pNode->GetTMController();
	if( (c->ClassID()==BIPSLAVE_CONTROL_CLASS_ID)||
		(c->ClassID()==BIPBODY_CONTROL_CLASS_ID)||
		(c->ClassID()==FOOTPRINT_CLASS_ID))
	{			
		// get the biped export interface from the controller
		IBipedExport* BipIface=(IBipedExport*)c->GetInterface(I_BIPINTERFACE);

		
		BipIface->RemoveNonUniformScale(1); // remove the non uniform scale
		BipIface->BeginFigureMode(1); // Figure Mode .. Binding pose 와 비슷

		// to do ----------------------------------------------------------------------------
		// these are subanim numbers for the center of mass controller
        Animatable* pVer = c->SubAnim(VERTICAL_SUBANIM);		// vertical
		Animatable* pHor = c->SubAnim(HORIZONTAL_SUBANIM);	// horizontal
		Animatable* pRot = c->SubAnim(ROTATION_SUBANIM);		// rotation
		
		if(NULL!=pVer)
		{
			OutputDebugString("pVer is not NULL\n");
			char temp[512]=_T("");
			sprintf(temp,"[%s][%s][%s]\n",pVer->SubAnimName(0),pVer->SubAnimName(1),pVer->SubAnimName(2));
			OutputDebugString(temp);
		}
		
		if(NULL!=pHor)
		{
			OutputDebugString("pHor is not NULL\n");
			char temp[512]=_T("");
			sprintf(temp,"[%s][%s][%s]\n",pHor->SubAnimName(0),pHor->SubAnimName(1),pHor->SubAnimName(2));
			OutputDebugString(temp);
		}

		if(NULL!=pRot)
		{
			OutputDebugString("pRot is not NULL\n");
			char temp[512]=_T("");
			sprintf(temp,"[%s][%s][%s]\n",pRot->SubAnimName(0),pRot->SubAnimName(1),pRot->SubAnimName(2));
			OutputDebugString(temp);
		}
	
		char temp[512]=_T("");
		sprintf(temp,"[%s][%s][%s]\n",c->SubAnimName(0),c->SubAnimName(1),c->SubAnimName(2));
		OutputDebugString(temp);

		//-----------------------------------------------------------------------------------
		// temp///////////////////////////////////////////////////////////////////////////////
//		Control* pPosCtrl = c->GetPositionController();
//		Control* pRotCtrl = c->GetRotationController();
//	 	Control* pSclCtrl = c->GetScaleController();
//
//		this->ExportAnimationPosition(pPosCtrl); // 위치
//		this->ExportAnimationRotation(pRotCtrl); // 회전
//		this->ExportAnimationScale(pSclCtrl);	 // 스케일
//
//		Control* pTPosCtrl = NULL;
//		INode* pTarget = pNode->GetTarget();
//		if(pTarget != NULL) pTPosCtrl = pTarget->GetTMController()->GetPositionController();
//		this->ExportAnimationPosition(pTPosCtrl); // 타겟 위치 에니메이션..
		///////////////////////////////////////////////////////////////////////////////////////

		BipIface->EndFigureMode(1);
		
		// Redraw if you want to see the result
		g_pIntf->RedrawViews(0);

		// release the interface when you are done with it
		c->ReleaseInterface(I_BIPINTERFACE,BipIface);
	}
*/
	return true;
}

bool CN3DExp::ProcessTransform(INode* pNode, CN3Transform* pTransform, bool bLocalCoordinate) // 위치
{
	if(NULL == pTransform || NULL == pNode) return false;

	pTransform->m_szName = "";
	this->ProcessName(pNode, pTransform);

	TimeValue mTime;
	Matrix3 mMtx, mMtxIP; // Matrix, Inverse Parent Matrix
	AffineParts mAP;

	mTime = m_Option.nFrmStart * 160;
	mMtx = pNode->GetNodeTM(mTime); // 0 Frame 째의 키값을 가져온다..
	if(bLocalCoordinate) mMtx *= Inverse(pNode->GetParentTM(mTime)); // 로컬 좌표로 바꾸기..
	decomp_affine(mMtx, &mAP);

//	pTransform->PosSet(mAP.t.x * 0.0254f, mAP.t.z * 0.0254f, mAP.t.y * 0.0254f); // 1 Unit == 1 Inch -> Meter 로 바꾼다.. // 위치 - y, z 를 바꾸어 준다..
	pTransform->PosSet(mAP.t.x, mAP.t.z, mAP.t.y); // 1 Unit == 1 Inch -> Meter 로 바꾼다.. // 위치 - y, z 를 바꾸어 준다..
	__Quaternion qt; // 회전.
	qt.x = mAP.q.x; qt.y = mAP.q.y; qt.z = mAP.q.z; qt.w = mAP.q.w;
	float fD = 0.0f;
	__Vector3 vAxis(0,1,0);
	qt.AxisAngle(vAxis, fD);
	float fTmp = vAxis.y; vAxis.y = vAxis.z; vAxis.z = fTmp; // y축과 z축을 바꾼다음..
	qt.RotationAxis(vAxis, fD); // 쿼터니언 계산 및 세팅.
	pTransform->RotSet(qt);
	pTransform->ScaleSet(mAP.k.x * mAP.f, mAP.k.y * mAP.f, mAP.k.z * mAP.f); // Scale

	if(FALSE == m_Option.bAnimationKey) return true; // Animation Key 처리 옵션..

	int nKC = 0;
	if(pTransform->Type() & OBJ_JOINT) // Joint 일때는 Sampling...
	{
		nKC = (m_Option.nFrmEnd - m_Option.nFrmStart) * (m_Option.fSamplingRate / 30.0f);
	}
	else
	{
		Control* pmCtrl = pNode->GetTMController();
		if(NULL == pmCtrl) return true;

		IKeyControl* pmKey = GetKeyControlInterface(pmCtrl);
		if(NULL == pmKey) return true;

		nKC = pmKey->GetNumKeys();
		if(nKC <= 0) return true; // 키값이 없으면 샘플링 하지 않는다..

		IKey mKTmp;
		pmKey->GetKey(nKC - 1, &mKTmp); // 마지막키를 가져오고..
		nKC = (mKTmp.time / 160.0f) * (m_Option.fSamplingRate / 30.0f); // 이수치로 나눠 줘야 한프레임이 된다.. 고로 마지막 프레임이 된다.
		if(nKC <= 0) return true;
	}
	
	pTransform->m_KeyPos.Alloc(nKC, m_Option.fSamplingRate, KEY_VECTOR3); // 키 할당..
	if(pTransform->Type() & OBJ_JOINT) pTransform->m_KeyRot.Alloc(nKC, m_Option.fSamplingRate, KEY_QUATERNION); // 회전 키 할당.. 이건 쿼터니언 할당이다...
	else pTransform->m_KeyRot.Alloc(nKC, m_Option.fSamplingRate, KEY_VECTOR3); // 회전 키 할당.. 이건 쿼터니언 할당이다...
	pTransform->m_KeyScale.Alloc(nKC, m_Option.fSamplingRate, KEY_VECTOR3); // 키 할당..

	__Vector3* pVKey = NULL;
	
	for(int i = 0; i < nKC; i++)
	{
		mTime = m_Option.nFrmStart * 160 +
				(int)(i*30.0f/m_Option.fSamplingRate)*160;
		mMtx = pNode->GetNodeTM(mTime); // Key 에 해당하는 Transform Sampling
		if(bLocalCoordinate) mMtx *= Inverse(pNode->GetParentTM(mTime)); // 로컬 좌표로 바꾸기.. // Parent Matrix 역행렬
		decomp_affine(mMtx, &mAP);

		__Vector3* pVP = (__Vector3*)(pTransform->m_KeyPos.DataGet(i));
		__Vector3* pVS = (__Vector3*)(pTransform->m_KeyScale.DataGet(i));
		__Quaternion* pQR = (__Quaternion*)(pTransform->m_KeyRot.DataGet(i));
		pVP->Set(mAP.t.x, mAP.t.z, mAP.t.y); // 위치 // 1 Unit == 1 Inch -> Meter 로 바꾼다..
		if(pQR) // 회전.
		{
			__Quaternion qt;
			qt.x = mAP.q.x; qt.y = mAP.q.y; qt.z = mAP.q.z; qt.w = mAP.q.w;
			fD = 0.0f;
			__Vector3 vAxis(0,1,0);
			qt.AxisAngle(vAxis, fD);
			float fTmp = vAxis.y; vAxis.y = vAxis.z; vAxis.z = fTmp; // y축과 z축을 바꾼다음..
//			while (fD>D3DX_PI) fD -= (D3DX_PI*2.0f);
//			while (fD<= (-D3DX_PI)) fD += (D3DX_PI*2.0f);
			pQR->RotationAxis(vAxis, fD); // 쿼터니언 계산 및 세팅.
		}
		pVS->Set(mAP.k.x * mAP.f, mAP.k.y * mAP.f, mAP.k.z * mAP.f); // Scale
	}

	// 키값 정리..
	CN3AnimKey* pKey[3] = { &(pTransform->m_KeyPos), &(pTransform->m_KeyRot), &(pTransform->m_KeyScale) };
	for(i = 0; i < 3; i++)
	{
		nKC = pKey[i]->Count();
		
		bool bSameKeys = true;
		float fCriticlaMass = 0.00001f;
		if(pKey[i]->Type() == KEY_VECTOR3)
		{
			__Vector3* pV0 = (__Vector3*)pKey[i]->DataGet(0);
			__Vector3* pV = NULL;
			for(int j = 0; j < nKC; j++)
			{
				pV = (__Vector3*)(pKey[i]->DataGet(j));
				if(	fabs(pV0->x - pV->x) > fCriticlaMass ||
					fabs(pV0->y - pV->y) > fCriticlaMass || 
					fabs(pV0->z - pV->z) > fCriticlaMass )
				{
					bSameKeys = false;
					break;
				}
			}
		}
		else if(pKey[i]->Type() == KEY_QUATERNION)
		{
			__Quaternion* pV0 = (__Quaternion*)pKey[i]->DataGet(0);
			__Quaternion* pV = NULL;
			for(int j = 0; j < nKC; j++)
			{
				pV = (__Quaternion*)(pKey[i]->DataGet(j));
				if(	fabs(pV0->x - pV->x) > fCriticlaMass ||
					fabs(pV0->y - pV->y) > fCriticlaMass || 
					fabs(pV0->z - pV->z) > fCriticlaMass || 
					fabs(pV0->w - pV->w) > fCriticlaMass )
				{
					bSameKeys = false;
					break;
				}
			}
		}
		if(bSameKeys)
			pKey[i]->Release(); // 키값이 모두 똑같다면.. 지운다.
	}

	return true;
}

bool CN3DExp::ProcessJoint(INode *pNode, CN3Joint *pJoint)
{
	if(NULL == pJoint || NULL == pNode) return false;
	if (false == IsBone(pNode)) return false;	// 본이 아니면 무시

	bool bLocalCoordinate = true;
	if(pJoint->Parent() == NULL) bLocalCoordinate = false;
	this->ProcessTransform(pNode, pJoint, bLocalCoordinate); // Transformation 처리.. 루트일때는 월드 값..
	
	// 이름이 너무 기니까.. 강제로 한다.
	pJoint->m_szName = pNode->GetName(); 
	char szJFN[256];
	wsprintf(szJFN, "%sChr\\%s.N3Joint", m_Option.szSubDir, pNode->GetName());
	pJoint->FileNameSet(szJFN);

	///////////////////////////////////////
	// 자식 객체 처리..		
	int nCC = pNode->NumberOfChildren();
	for(int i = 0; i < nCC; i++)
	{
		INode* pNodeChild = pNode->GetChildNode(i);
		if(false == IsBone(pNodeChild)) continue;

		CN3Joint* pChild = new CN3Joint();
		pJoint->ChildAdd(pChild);

		if(true != this->ProcessJoint(pNodeChild, pChild))
		{
			pJoint->ChildDelete(pChild); // 실패하면 차일드에서 삭제..
			delete pChild; pChild = NULL;
		}
	}
	// 자식 객체 처리..
	///////////////////////////////////////

	return true;
}

bool CN3DExp::IsBone(INode *pNode)
{
	if(pNode == NULL)return false;
//	const char* szName = pNode->GetName();
	ObjectState os = pNode->EvalWorldState(m_Option.nFrmStart * 160);
	if (!os.obj) return false;

	Class_ID cID = os.obj->ClassID();
	if(Class_ID(BONE_CLASS_ID,0) == cID ||
		BONE_OBJ_CLASSID == cID)	// New procedural bone object for Magma (subclass of GeomObject)
		return true;

	if(cID == Class_ID(DUMMY_CLASS_ID, 0))	// 더미이지만 TMController가 바이패트 컨트롤일경우가 있으므로
		return false;
	
	Control *cont = pNode->GetTMController();
	//other Biped parts
	if(	cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
		//Biped root "Bip01"
		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID
	) return true;
	return false;
}

bool CN3DExp::FindNodeRecursive(INode *pNode, Class_ID cID, std::list<INode*> &list)
{
	if(NULL == pNode) return false;

	Object* pObj = pNode->EvalWorldState(m_Option.nFrmStart * 160).obj;
	const char* szName = pNode->GetName();
	
	if(pObj)
	{
		TSTR szClass;
		pObj->GetClassName(szClass);
		Class_ID cID2 = pObj->ClassID();
		DWORD dwSCID = pObj->SuperClassID();

		if(cID2 == cID)
			list.push_back(pNode);
	}

	///////////////////////////////////////
	// 자식 객체 처리..		
	int nCC = pNode->NumberOfChildren();
	for(int i = 0; i < nCC; i++)
	{
		INode* pNodeChild = pNode->GetChildNode(i);

		this->FindNodeRecursive(pNodeChild, cID, list);
	}

	return true;
}

bool CN3DExp::CheckObjectClassID(INode* pNode, const Class_ID& cID) const	// 주어진 노드가 주어진 클래스 아이디인지 검사
{
	if (NULL == pNode) return false;
	Object* pObj = pNode->EvalWorldState(m_Option.nFrmStart * 160).obj;

	if(pObj)
	{
//		TSTR szClass;
//		pObj->GetClassName(szClass);
		Class_ID cID2(pObj->ClassID());
//		DWORD dwSCID = pObj->SuperClassID();

		if(cID2 == cID) return true;
	}
	return false;
}




// Bone 및 Skin 처리 루틴.. 일부분이지만 나중에 쓸수도 있다..
////////////////////////////////////
/*
				ISkin* pmSkin = NULL;
				ModContext *pMC = NULL;
				Object* pObj = pNode->GetObjectRef();
				if (NULL == pObj) return NULL;
				
				TSTR szClass;
				pObj->GetClassName(szClass);
				Class_ID cID = pObj->ClassID();
				SClass_ID scID = pObj->SuperClassID();

				while (scID == GEN_DERIVOB_CLASS_ID && pMC == NULL && pmSkin == NULL)
				{
					IDerivedObject* pDerObj = (IDerivedObject *)(pObj);

					pDerObj->GetClassName(szClass);

					int nMC = nMC = pDerObj->NumModifiers();
					for(int i = 0; i < nMC; i++)
					{
						Modifier* pMod = pDerObj->GetModifier(i); // Get the modifier. 
						
						pMod->GetClassName(szClass);
						cID = pMod->ClassID();
						scID = pMod->SuperClassID();


						if (pMod->ClassID() == SKIN_CLASSID) // is this the correct Physique Modifier based on index?
						{
							pmSkin = (ISkin*)pMod;
							pMC = pDerObj->GetModContext(i);
							break;
						}
					}
					pObj = pDerObj->GetObjRef();
					scID = pObj->SuperClassID();
				}

				if(pmSkin)
				{
					int nBC = pmSkin->GetNumBones();
					for(int i = 0 ; i < nBC; i++)
					{
						const char* szBone = pmSkin->GetBoneName(i);
						INode* pBN = pmSkin->GetBone(i);
					}
				}

//				if(pMC && pMC->localData)
//				{
//					BoneModData *bmd = (BoneModData *) mc->localData;
//				}
				
*/

bool CN3DExp::ProcessIMesh(INode *pNode, CN3IMesh* pIMesh)
{
	if(NULL == pNode) return false;
	if(NULL == pIMesh) return false;
	if(pNode->ClassID() == Class_ID(DUMMY_CLASS_ID,0)) return false; // 더미이면.. 넘어간다..
	const char* szNamTmp = pNode->GetName();

	TriObject* pTriObj = NULL;

	BOOL bNeedDelete = FALSE;
	Object *pObj = pNode->EvalWorldState(m_Option.nFrmStart * 160).obj;
	if (pObj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{ 
		pTriObj = (TriObject *)pObj->ConvertToType(m_Option.nFrmStart * 160, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (pObj != pTriObj) bNeedDelete = TRUE;
	}

	if (pTriObj == NULL || pNode->IsGroupHead()) // 지오메트리가 아니거나 그룹 헤드면..
	{
		char szDebug[512];
		wsprintf(szDebug, "%s -> May be Camera Target or Light Symbol\n", pNode->GetName());
		OutputDebugString(szDebug);
		return false;
	}

	Mesh* pmMesh = &pTriObj->GetMesh(); // TriObject에서 Mesh를 얻어온다.
	pmMesh->buildNormals(); // Normal 을 만든다..

	int nFC = pmMesh->getNumFaces(); // Mesh에서 Face를 읽어온다.
	int nVC = pmMesh->getNumVerts(); // // Mesh에서 점 갯수 읽어온다.
	int nUVC = pmMesh->getNumTVerts(); // 텍스처 매핑 점...

	if(nFC <= 0) // 점과 버텍스가 하나도 없으면 돌아간다..
	{
		OutputDebugString("Mesh Export - 점과 페이스가 하나도 없습니다..\n");
		if(bNeedDelete) delete pTriObj;
		return false;
	}

//	Matrix3 mMtx = pNode->GetNodeTM(m_Option.nFrmStart * 160); // 월드 행렬가져온다.
	Matrix3 mMtx = pNode->GetObjTMAfterWSM(m_Option.nFrmStart * 160); // 월드 행렬가져온다.
//	Matrix3 mMtx = pNode->GetObjectTM(m_Option.nFrmStart * 160); // 월드 행렬가져온다.

	__VertexXyzNormal*	pvDest1 = NULL;
	__Vector3*			pvDest2 = NULL;
	CN3VMesh* pVMesh = NULL;
	if(nFC >= 8192) // 좀 큰거면 .. 지형용으로 뽑을 VMesh 라고 생각하고... 
	{
		pVMesh = new CN3VMesh();
		pVMesh->CreateVertices(nFC * 3);
		pvDest2 = pVMesh->Vertices();
	}
	else
	{
		Point3 ptTmp;
		pIMesh->Create(nFC, nVC, nUVC);
		pvDest1 = pIMesh->Vertices(); // 쓸 데이터를 가져오고 
		this->ProcessName(pNode, pIMesh);

		for(int i = 0; i < nVC; i++)
		{
			ptTmp = mMtx * pmMesh->getVert(i); // Vertex 수만큼 루프를 돌린다.
			pvDest1[i].x = ptTmp.x; // * 0.0254f; // 1 Unit == 1 Inch -> Meter 로 바꾼다..
			pvDest1[i].y = ptTmp.z; // * 0.0254f;
			pvDest1[i].z = ptTmp.y; // * 0.0254f; // y 와 z 를 바꾼다..
		}
		for(i = 0; i < nUVC; i++)
		{
			pIMesh->UVSet(i, pmMesh->tVerts[i].x, 1.0f - pmMesh->tVerts[i].y); // 일단 텍스처 매핑을 기록해 둔다..
		}
	}

	//////////////////////////////////////
	// 스케일을 검사한다.
	int vx1 = 0, vx2 = 2, vx3 = 1;  // Triangle 순서 회전 정하기.
	AffineParts ap; 
	decomp_affine(mMtx, &ap);
	if(ap.k.x * ap.f <= 0 || ap.k.y * ap.f <= 0 || ap.k.z * ap.f <= 0)
	{
//		MessageBox(::GetActiveWindow(), pNode->GetName(), 
//			"경고 : 스케일 값이 음수 입니다 -> Face가 뒤집혀 보일 수 있습니다.", MB_OK);
		vx1 = 0; vx2 = 1; vx3 = 2;
	}

	Point3 ptTmp, ptNTmp;
	int nVI[3], nUVI[3];
	for (int i = 0; i < nFC; i++) // Face Count 만큼 돌면서..
	{
		Face* pFace = &(pmMesh->faces[i]);
		nVI[0] = pFace->v[vx1];
		nVI[1] = pFace->v[vx2];
		nVI[2] = pFace->v[vx3];
		
		if(pvDest2) // 지형용으로 뽑을 거면...
		{
			for(int j = 0; j < 3; j++)
			{
				ptTmp = mMtx * pmMesh->getVert(nVI[j]); // Vertex 수만큼 루프를 돌린다.
				pvDest2[i*3+j].x = ptTmp.x; // * 0.0254f; // 1 Unit == 1 Inch -> Meter 로 바꾼다..
				pvDest2[i*3+j].y = ptTmp.z; // * 0.0254f; // 1 Unit == 1 Inch -> Meter 로 바꾼다..
				pvDest2[i*3+j].z = ptTmp.y; // * 0.0254f; // 1 Unit == 1 Inch -> Meter 로 바꾼다..
			}
		}
		else
		{
			pIMesh->VertexIndexSet(i*3+0, nVI[0]); // Vertex Indices
			pIMesh->VertexIndexSet(i*3+1, nVI[1]);
			pIMesh->VertexIndexSet(i*3+2, nVI[2]);

			ptNTmp = mMtx * pmMesh->getFaceNormal(i); // Face Normal
			ptNTmp = ptNTmp.Normalize(); // Normalize

			pvDest1[nVI[0]].n.x = ptNTmp.x;
			pvDest1[nVI[0]].n.y = ptNTmp.z; // y 
			pvDest1[nVI[0]].n.z = ptNTmp.y;

			pvDest1[nVI[1]].n.x = ptNTmp.x;
			pvDest1[nVI[1]].n.y = ptNTmp.z; // y 
			pvDest1[nVI[1]].n.z = ptNTmp.y;

			pvDest1[nVI[2]].n.x = ptNTmp.x;
			pvDest1[nVI[2]].n.y = ptNTmp.z; // y 
			pvDest1[nVI[2]].n.z = ptNTmp.y;

			if(nUVC > 0)
			{
				nUVI[0] = pmMesh->tvFace[i].t[vx1]; // UV Indices
				nUVI[1] = pmMesh->tvFace[i].t[vx2];
				nUVI[2] = pmMesh->tvFace[i].t[vx3];

				pIMesh->UVIndexSet(i*3+0, nUVI[0]);
				pIMesh->UVIndexSet(i*3+1, nUVI[1]);
				pIMesh->UVIndexSet(i*3+2, nUVI[2]);
			}
		}
	}

	if(m_Option.bGenerateSmoothNormal == TRUE && pvDest1)
	{
		pIMesh->ReGenerateSmoothNormal();	// 법선 벡터 부드럽게 재계산..
	}

	if (bNeedDelete)
	{
		delete pTriObj;
		pTriObj = NULL;
	}

	if(pVMesh) // 지형용으로 뽑은 메시면..
	{
		pVMesh->m_szName = pNode->GetName();
		pVMesh->SaveToFile(pVMesh->m_szName + ".n3vmesh"); // 저장..
		
		delete pVMesh; pVMesh = NULL;
		return false; // 이렇게 리턴해야 IMesh 로 뽑지 않는다..
	}

	return true;
}

bool CN3DExp::ProcessMaterial(INode* pNode, __Material* pMtl, CN3Texture** ppTex, LPCTSTR pszDir)
{
	if(NULL == pNode || NULL == pMtl || NULL == ppTex) return false;

	const char* szName = pNode->GetName();

	pMtl->Init();
	*ppTex = NULL;
	
	Mtl* pmMtl = pNode->GetMtl(); // 노드에서 Material을 얻어온다.
	if(NULL == pmMtl) return false; // Material 이 있으면 정보를 뽑아온다.

	// Methods to access sub-materials of meta-materials 
//	Mtl* pmMtlTmp = NULL;
	int nSM = pmMtl->NumSubMtls();
//	if(nSM > 0)
//	{
//		for(int i = 0; i < nSM; i++)
//		{
//			if(pmMtl->GetSubMtl(i) != NULL) 
//			{
//				pmMtlTmp = pmMtl->GetSubMtl(i);
//				break;
//			}
//		}
//	}
//	else
//	{
//		pmMtlTmp = pmMtl;
//	}
	
	if(pmMtl && pmMtl->GetSubTexmap(1) != NULL) // 텍스처가 씌어 있는 재질이면.
	{
		Texmap* pmTex = pmMtl->GetSubTexmap(1); // Diffuse Texture Map
		BitmapTex* pBMT = NULL;
		
		if(pmTex && pmTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{ 
			pBMT = (BitmapTex*)pmTex; // 캐스트 연산자로 바꿔줄 수 있다.
		}

		if(pBMT && lstrlen(pBMT->GetMapName()) > 0)
		{
			std::string szBMPFN, szBMPFN2;
			char szDrv[32], szDir[128], szName[128], szExt[128];
			szBMPFN = pBMT->GetMapName();
			_splitpath(szBMPFN.c_str(), szDrv, szDir, szName, szExt);

			// sub directory 지정되어 있으면 붙이기
			if (lstrlen(m_Option.szSubDir)>0) szBMPFN2 = std::string(m_Option.szSubDir);
			else szBMPFN2 = "";

			// 텍스쳐 폴더 경로 붙이기
			if (pszDir) szBMPFN2 += pszDir;
			else szBMPFN2 += "Texture\\";
			szBMPFN2 += szName; szBMPFN2 += ".DXT"; // 이름과 확장자를 바꾸어 준다..
			
			CharLower(szBMPFN.begin());
			CharLower(szBMPFN2.begin());

			int nTCPrev = m_pScene->s_MngTex.Count(); // 텍스처 중복 체크..
			bool bOverlapped = false;
			for(int i = 0; i < nTCPrev; i++)
			{
				if(m_pScene->s_MngTex.Get(i)->FileName() == szBMPFN2)
				{
					bOverlapped = true;
				}
			}

			if(bOverlapped)
			{
				(*ppTex) = m_pScene->s_MngTex.Get(szBMPFN2); // 비트맵을 읽고..
			}
			else
			{
				(*ppTex) = new CN3Texture();
				
				if(false == (*ppTex)->LoadFromFile(szBMPFN)) // 비트맵 읽기가 실패하면..
				{
					delete (*ppTex); (*ppTex) = NULL;
					return false;
				}
				else // 비트맵을 읽었으면..
				{
					(*ppTex)->m_szName = pBMT->GetName();
					(*ppTex)->FileNameSet(szBMPFN2);
					m_pScene->s_MngTex.Add(*ppTex); // Texture Manager 에 넣어준다.

					CN3Texture* pTex = *ppTex;
					if(m_Option.bGenerateHalfSizeTexture) // Texture Size 를 절반으로 줄여 출력..
					{
						D3DFORMAT fmt = pTex->PixelFormat();
						int nW2 = pTex->Width() / 2;
						int nH2 = pTex->Height() / 2;
						pTex->Convert(fmt, nW2, nH2);
					}

					if(m_Option.bGenerateCompressedTexture) // Texture 압축 사용
					{
						D3DFORMAT fmt = pTex->PixelFormat(), NewFormat = D3DFMT_UNKNOWN;
						if(D3DFMT_X8R8G8B8 == fmt) NewFormat = D3DFMT_DXT1;
						else if(D3DFMT_A8R8G8B8 == fmt) NewFormat = D3DFMT_DXT3;

						if(D3DFMT_UNKNOWN != NewFormat)
						{
							pTex->Convert(NewFormat);
						}
					}
				}
			}

			if(*ppTex && pMtl)
			{
				D3DFORMAT fmt = (*ppTex)->PixelFormat();
				if(D3DFMT_A8R8G8B8 == fmt || D3DFMT_DXT3 == fmt) // 알파채널 텍스처이면..
				{
					pMtl->nRenderFlags |= RF_ALPHABLENDING;
					pMtl->dwSrcBlend = D3DBLEND_SRCALPHA;
					pMtl->dwSrcBlend = D3DBLEND_INVSRCALPHA;
				}
			}
		}
		pMtl->Diffuse.a = 1.0f;
		pMtl->Diffuse.r = pMtl->Diffuse.g = pMtl->Diffuse.b = 150.0f/255.0f;

		pMtl->Ambient.a = 1.0f;
		pMtl->Ambient.r = pMtl->Ambient.g = pMtl->Ambient.b = 150.0f/255.0f;
	}
	else
	{	// 이렇게 텍스쳐가 없을경우 하는 이유는 맥스에서는 diffuse대신에 텍스쳐를 넣는 개념이기 때문에 
		// 텍스쳐가 있는경우에는 그냥 1을 넣고 아닌경우에만 메터리얼에 정해진 값을 넣는다.
		pMtl->Diffuse.a = 1.0f;
		pMtl->Diffuse.r = pmMtl->GetDiffuse().r;
		pMtl->Diffuse.g = pmMtl->GetDiffuse().g;
		pMtl->Diffuse.b = pmMtl->GetDiffuse().b;

		pMtl->Ambient.a = 1.0f;
		pMtl->Ambient.r = pmMtl->GetAmbient().r;
		pMtl->Ambient.g = pmMtl->GetAmbient().g;
		pMtl->Ambient.b = pmMtl->GetAmbient().b;
	}



	pMtl->Specular.a = 1.0f;
	pMtl->Specular.r = pmMtl->GetSpecular().r;
	pMtl->Specular.g = pmMtl->GetSpecular().g;
	pMtl->Specular.b = pmMtl->GetSpecular().b;

	if (pmMtl->GetSelfIllumColorOn())
	{
		pMtl->Emissive.a = 1.0f;
		pMtl->Emissive.r = pmMtl->GetSelfIllumColor().r;
		pMtl->Emissive.g = pmMtl->GetSelfIllumColor().g;
		pMtl->Emissive.b = pmMtl->GetSelfIllumColor().b;
	}
	else
	{
		pMtl->Emissive.a = 1.0f;
		pMtl->Emissive.r = pMtl->Emissive.g = pMtl->Emissive.b = pmMtl->GetSelfIllum();
	}

	pMtl->Power = pmMtl->GetShinStr();
	if(!pNode->GetBackCull()) pMtl->nRenderFlags |= RF_DOUBLESIDED;	

	return true;
}

void CN3DExp::CountAllNodes(INode *pNode, int &nCount)
{
	nCount++;

	int nChildCount = pNode->NumberOfChildren();
	INode* pNodeChild = NULL;
	for(int i = 0; i < nChildCount; i++)
	{
		pNodeChild = pNode->GetChildNode(i);
		if(pNodeChild==NULL) continue;

		this->CountAllNodes(pNodeChild, nCount);
	}
}

Modifier* CN3DExp::FindPhysiqueModifier(INode* pNode)
{
	// Get object from node. Abort if no object.
	for(Object* pObj = pNode->GetObjectRef(); pObj && pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID; ) // Is derived object ?
	{
		IDerivedObject *pDerObj = (IDerivedObject*)(pObj); // Yes -> Cast.
		// Iterate over all entries of the modifier stack.
		int nModStackCount = pDerObj->NumModifiers();
		for(int i = 0; i < nModStackCount; i++)
		{
			Modifier* pMod = pDerObj->GetModifier(i); // Get current modifier.
			if (pMod->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)) // Is this Physique ?
			{
				return pMod; // Yes -> Exit.
			}
		}
		pObj = pDerObj->GetObjRef();
	}
	
	return NULL; // Not found.
}

bool CN3DExp::ProcessPhysique(INode* pNode, INode* pNodeRootJoint, CN3Skin* pN3Skin)
{
    if(NULL == pNode) return false;
	if(NULL == pNodeRootJoint) return false;
	if(NULL == pN3Skin) return false;

	Modifier *pPhyMod = FindPhysiqueModifier(pNode);
    if (NULL == pPhyMod) return false;

    IPhysiqueExport *pPE = (IPhysiqueExport*)pPhyMod->GetInterface(I_PHYINTERFACE);
    if (NULL == pPE) return false;

	// create a ModContext Export Interface for the specific node of the Physique Modifier
	IPhyContextExport *pPCE = (IPhyContextExport*)pPE->GetContextInterface(pNode);
	if(NULL == pPCE)
	{
		pPhyMod->ReleaseInterface(I_PHYINTERFACE, pPE);
		return false;
	}

	Object* pObj = pNode->EvalWorldState(m_Option.nFrmStart * 160).obj; 
	int nPtCount = pObj->NumPoints(); // compute the transformed Point3 at time t
	if(nPtCount <= 0)
	{
		pPE->ReleaseContextInterface(pPCE);
		pPhyMod->ReleaseInterface(I_PHYINTERFACE, pPE);
		return false;
	}
	
	if(false == this->ProcessIMesh(pNode, pN3Skin)) return false;
	if(nPtCount != 	pN3Skin->VertexCount())
	{
		MessageBox(::GetActiveWindow(), pNode->GetName(), "Can't process physique. Because it's vertex count is different from mesh's vertex count", MB_OK);
		return false;
	}

	__VertexSkinned* pVDest = pN3Skin->SkinVertices();
	memset(pVDest, 0, sizeof(__VertexSkinned) * nPtCount);

//	pPCE->ConvertToRigid(TRUE); // we convert all vertices to Rigid in this example

	Matrix3 mMtxOrg = pNode->GetObjTMAfterWSM(m_Option.nFrmStart * 160); // 원래 메시의 월드 행렬
	for (int i = 0; i < nPtCount;  i++)
	{
		IPhyVertexExport *pVE = pPCE->GetVertexInterface(i);
		if(NULL == pVE) continue;

		Point3 ptOrg = pObj->GetPoint(i);
		ptOrg = ptOrg * mMtxOrg;
//		pVDest[i].vOrigin.Set(ptOrg.x * 0.0254f, ptOrg.z * 0.0254f, ptOrg.y * 0.0254f);
		pVDest[i].vOrigin.Set(ptOrg.x, ptOrg.z, ptOrg.y);
//		pObj->SetPoint(i, ptBlendP); // 확인하기 위해서 넣어본다..

		int nVType = pVE->GetVertexType();
		if(nVType & BLENDED_TYPE) // Blend Type
		{
			IPhyBlendedRigidVertex *pVtxBlend = (IPhyBlendedRigidVertex*)pVE;
			
			int nWC = pVtxBlend->GetNumberNodes();
			if(nWC > 0)
			{
				pVDest[i].nAffect = nWC;
				pVDest[i].pfWeights = new float[nWC];
				pVDest[i].pnJoints = new int[nWC];

				for (int j = 0; j < nWC; j++)
				{
					int nJI = 0;
					INode *pBone = pVtxBlend->GetNode(j);
					this->FindBoneIndex(pNodeRootJoint, pBone, nJI); // 포인터를 가지고 인덱스를 찾는다..
					float fWeight = pVtxBlend->GetWeight(j);

					const char* szBone = pBone->GetName();

					pVDest[i].pnJoints[j] = nJI;
					pVDest[i].pfWeights[j] = fWeight;
				}
			}
		}
		else 
		{
			pVDest[i].nAffect = 1;
			pVDest[i].pnJoints = new int[1];

			IPhyRigidVertex *pVtxNoBlend = (IPhyRigidVertex*)pVE;
			INode *pBone = pVtxNoBlend->GetNode();
			int nJI = 0;
			this->FindBoneIndex(pNodeRootJoint, pBone, nJI); // 포인터를 가지고 인덱스를 찾는다..

			const char* szBone = pBone->GetName();

			pVDest[i].pnJoints[0] = nJI;
		}

		pPCE->ReleaseVertexInterface(pVE);
		pVE = NULL;	
	}
	
	pPE->ReleaseContextInterface(pPCE);
	pPhyMod->ReleaseInterface(I_PHYINTERFACE, pPE);

	pN3Skin->RecalcWeight();	// weight 값이 1.0이 되도록 한번더 다시 계산해준다.

	return TRUE;
}
/*
bool CN3DExp::FindNodeIndex(INode* pNodeCompare, INode* pNodeSrc, Class_ID& cID, int& nNodeIndex)
{
	if(NULL == pNodeCompare || NULL == pNodeSrc) return false;
	Control* pCtrl = pNodeCompare->GetTMController();
	if(NULL == pCtrl) return false;
	if(pCtrl->ClassID() != cID) return false; // 바이페드가 아니면 무시한다..

	DWORD dwID = pNodeSrc->SuperClassID();
	DWORD dwID2 = pNodeCompare->SuperClassID();

	const char* sz1 = pNodeSrc->GetName();
	const char* sz2 = pNodeCompare->GetName();

	if(pNodeSrc == pNodeCompare)
		return true;
	else
		nNodeIndex++;

	int nCC = pNodeCompare->NumberOfChildren();
	for(int i = 0; i < nCC; i++)
	{
		INode* pNodeChild = pNodeCompare->GetChildNode(i);
		if(this->FindNodeIndex(pNodeChild, pNodeSrc, cID, nNodeIndex) == true) return true;
	}

	return false;
}
*/
bool CN3DExp::FindBoneIndex(INode* pNodeCompare, INode* pNodeSrc, int& nNodeIndex)
{
	if(NULL == pNodeCompare || NULL == pNodeSrc) return false;
	if(false == IsBone(pNodeCompare)) return false; // 본이 아니면 무시한다..

	DWORD dwID = pNodeSrc->SuperClassID();
	DWORD dwID2 = pNodeCompare->SuperClassID();

	const char* sz1 = pNodeSrc->GetName();
	const char* sz2 = pNodeCompare->GetName();

	if(pNodeSrc == pNodeCompare)
		return true;
	else
		nNodeIndex++;

	int nCC = pNodeCompare->NumberOfChildren();
	for(int i = 0; i < nCC; i++)
	{
		INode* pNodeChild = pNodeCompare->GetChildNode(i);
		if(this->FindBoneIndex(pNodeChild, pNodeSrc, nNodeIndex) == true) return true;
	}

	return false;
}
