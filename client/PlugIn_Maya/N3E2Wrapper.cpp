// Copyright (C) 1998-2000 Alias | Wavefront, a division of Silicon Graphics Limited.//
// The information in this file is provided for the exclusive use of the
// licensees of Alias | Wavefront.  Such users have the right to use, modify,
// and incorporate this code into other products for purposes authorized
// by the Alias | Wavefront license agreement, without fee.
//
// ALIAS | WAVEFRONT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
// EVENT SHALL ALIAS | WAVEFRONT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

// $Revision: /main/24 $
// $Date: 2000/04/19 16:12:24 $
//
//
// Description: 
//            
// N3E2 translator for Maya    
// 
//

#ifdef WIN32
#include <process.h>
#endif

#ifdef WIN32
#define MAXPATHLEN 1024
#endif

#include <crtdbg.h>
#include "N3E2Wrapper.h"
#include "IFFReader.h"

#include "../N3Base/N3Eng.h"
#include "../N3Base/N3Scene.h"
#include "../N3Base/N3PMeshCreate.h"

extern const char * objectName( MObject object );


#include "CommCtrl.h"
#include "Resource.h"

BOOL CN3E2Wrapper::m_bCancelExport = FALSE;
__EXPORT_OPTION CN3E2Wrapper::m_Option;

CN3EngTool*		g_pEng;

// CN3E2Wrapper class methods:
CN3E2Wrapper::CN3E2Wrapper()
{
	// 진행 상황 대화상자
	HWND hWnd = M3dView::applicationShell();
	HINSTANCE hInst = MhInstPlugin;
	m_hDlgProgress = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROGRESS), hWnd, CN3E2Wrapper::DlgProcProgress);
	__ASSERT(m_hDlgProgress, "Failed to create progress dialog");
	m_hWndPB = GetDlgItem(m_hDlgProgress, IDC_PROGRESS);
	m_hWndLB = GetDlgItem(m_hDlgProgress, IDC_LIST_PROGRESS);

	m_pScene = NULL;
	g_pEng = NULL;

	this->Release();
}

CN3E2Wrapper::~CN3E2Wrapper()
{
	::DestroyWindow(m_hDlgProgress); // 진행 상자 없애기..
	this->Release();
}

void CN3E2Wrapper::Release()
{
	memset(&m_Option, 0, sizeof(m_Option));

	m_Option.bExportCamera		= TRUE;
	m_Option.bExportLight		= TRUE;
	m_Option.bExportGeometry	= TRUE;
	m_Option.bExportDummy		= FALSE;
	m_Option.bExportCharacter	= TRUE;

	m_Option.bAnimationKey	= TRUE;
	
	m_Option.bGenerateFileName = FALSE;
	m_Option.bGenerateSmoothNormal = TRUE;
//	m_Option.bGenerateProgressiveMesh = FALSE;
	m_Option.bGenerateHalfSizeTexture = FALSE;
	m_Option.bGenerateCompressedTexture = TRUE;
	m_Option.fSamplingRate = 30.0f;

	lstrcpy(m_szPath, "");		// 경로 이름
	lstrcpy(m_szFileName, "");	// 파일 이름

	delete m_pScene; m_pScene = NULL;
	delete g_pEng; g_pEng = NULL;
}

void CN3E2Wrapper::SetFileName(const char *szFileName)
{
	lstrcpy(m_szFileName, szFileName);	// 파일 이름
}

void CN3E2Wrapper::SetPath(const char *szPath)
{
	lstrcpy(m_szPath, szPath);	// 파일 이름
}

// 라이트 종류를 리턴.
CN3Light* CN3E2Wrapper::ProcessLight(MFnLight &mLight)
{
	CN3Light* pLight = new CN3Light();
	MFnTransform mT = MFnTransform(mLight.parent(0));
	this->ProcessTransform(mT, pLight); // Transform Node

	pLight->m_szName = mT.name().asChar();
	pLight->FileNameSet("Data\\" + pLight->m_szName + ".N3Light"); // 파일 이름 결정..

	// 라이트 종류
	D3DCOLORVALUE dcv = {1,1,1,1};
	pLight->m_Data.InitPoint(m_pScene->LightCount(), __Vector3(0,0,0), dcv);
	MFn::Type Type = mLight.type();
	if(Type == MFn::kDirectionalLight)
	{
		pLight->m_Data.Type = D3DLIGHT_DIRECTIONAL; // a directional light
	}
	else if(Type == MFn::kSpotLight) // a sopt light
	{
		pLight->m_Data.Type = D3DLIGHT_SPOT;
		
		MFnSpotLight Spot(mLight.object());
		pLight->m_Data.Theta = (float)Spot.coneAngle(); // 내부 원뿔각
		pLight->m_Data.Phi = (float)Spot.penumbraAngle(); // 외부 원뿔각
	}
	else // 기본적으로 point light
	{
		pLight->m_Data.Type = D3DLIGHT_POINT;
	}

	pLight->m_Data.Position = pLight->Pos();

	MFloatVector dir = mLight.lightDirection(); // 방향
	pLight->m_Data.Direction.x = dir.x;
	pLight->m_Data.Direction.y = dir.y;
	pLight->m_Data.Direction.z = -dir.z;

	MColor color = mLight.color(); // 라이트 컬러.

	pLight->m_Data.Ambient.a = 1.0f;
	pLight->m_Data.Ambient.r = color.r * 0.3f;
	pLight->m_Data.Ambient.g = color.g * 0.3f;
	pLight->m_Data.Ambient.b = color.b * 0.3f;

	pLight->m_Data.Diffuse.a = 1.0f;
	pLight->m_Data.Diffuse.r = color.r;
	pLight->m_Data.Diffuse.g = color.g;
	pLight->m_Data.Diffuse.b = color.b;

/*	MFnDependencyNode lightDG(mLight);
    MObject lightAttr = lightDG.attribute(MString("intensity"));
	MPlug plug(mLight, lightAttr); // 강도
	double intensity;
	plug.getValue(intensity);
	
	lightAttr = lightDG.attribute(MString("specular"));
	plug.setAttribute(lightAttr);
	plug.getValue(
*/

	// 나머지 기본값.
	pLight->m_Data.nNumber = m_pScene->LightCount();
	pLight->m_Data.bOn = TRUE;

	return pLight;
}

CN3Camera* CN3E2Wrapper::ProcessCamera(MFnCamera &mCamera)
{
	CN3Camera* pCamera = new CN3Camera();
	MFnTransform mT(mCamera.parent(0));
	this->ProcessTransform(mT, pCamera); // Transform 처리..
	std::string szFN = "Data\\" + pCamera->m_szName + ".N3Camera";
	pCamera->FileNameSet(szFN);

//	double dHFOV, dVFOV;
//	mCamera.getPortFieldOfView(800, 600, dHFOV, dVFOV);
//	pCamera->s_CameraData.fNP = (float)(mCamera.nearClippingPlane() * 0.01); // meter 단위 이기때문에 100 으로 나누어 준다.
//	pCamera->s_CameraData.fFP = (float)(mCamera.farClippingPlane() * 0.01);
//	pCamera->s_CameraData.fFOV = (float)dHFOV; // 90 도 렌즈로 강제 세팅..

	return pCamera;
}

MObject CN3E2Wrapper::MeshGetShader(MFnMesh& mMesh)
{
	MObjectArray ShaderArray; MIntArray IndicesArray;
	mMesh.getConnectedShaders(0, ShaderArray, IndicesArray);
//	int nInstance = PathCur.instanceNumber();
//	Mesh.getConnectedShaders(nInstance, ShaderArray, IndicesArray);

	int nSC = ShaderArray.length();
	int nIC = IndicesArray.length();
	if(nSC > 0)
	{
		MFnDependencyNode NodeShader(ShaderArray[0]);
		MObject ssAttr = NodeShader.attribute(MString("surfaceShader"));
		MPlug Plug(ShaderArray[0], ssAttr);
		MPlugArray srcPlugArray;
		Plug.connectedTo( srcPlugArray, true, false); // as destination, as source

		if(srcPlugArray.length() > 0) 
			return srcPlugArray[0].node();
	}

	MObject nullObj;
	return nullObj;
}

void CN3E2Wrapper::SceneExport()
{
	// Option Dialog 
	HINSTANCE hInst = MhInstPlugin;
	HWND hWnd = M3dView::applicationShell();
	int rval = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_EXPORT_OPTION), hWnd, DlgProcPane, NULL); // 컨트롤 패널 대화상자..
	if(rval != 1) return;
	
	g_pEng = new CN3EngTool();
//	g_pEng->InitEnv();
	g_pEng->Init(TRUE, hWnd, 64, 64, 0, FALSE);
	
	char szPath[256];
	::GetCurrentDirectory(256, szPath);
	g_pEng->PathSet(szPath); // 경로 설정...

	m_pScene = new CN3Scene();

	if(NULL == g_pEng->s_lpD3DDev)
	{
		delete m_pScene; m_pScene = NULL;
		delete g_pEng; g_pEng = NULL;

		MessageBox(::GetActiveWindow(), "VideoMemory 가 부족합니다. VideoMemory 확보 후 다시 시도하세요.", "Data Export Error", MB_OK);
		return;
	}

	MTime mOldTime = MAnimControl::currentTime(); // 현재 프레임이 몇 프레임째인지 저장..
	MTime ZeroFrm; 
	ZeroFrm.setValue(0); // ZeroFrm.setUnit(MTime::kNTSCFrame);
	MAnimControl::setCurrentTime(ZeroFrm);
	MGlobal::viewFrame(ZeroFrm);
	
	int nLI = 0;
	DWORD dwRWC = 0;
	char szInfo[1024] = "";

	m_pScene->Release(); // 모두 해제하고..

//	m_pScene->m_fFrmCur = (float)MAnimControl::currentTime().value();
	m_pScene->m_fFrmStart = (float)m_Option.nFrmStart; // (float)MAnimControl::minTime().value();
	m_pScene->m_fFrmEnd = (float)m_Option.nFrmEnd; // (float)MAnimControl::maxTime().value();

	MObjectArray mObjects;

	int i = 0;

	MItDependencyNodes IterNodes1(MFn::kCamera); // scene 의 노드들을 체크..
	MItDependencyNodes IterNodes2(MFn::kLight); // scene 의 노드들을 체크..
	MItDependencyNodes IterNodes3(MFn::kMesh); // scene 의 노드들을 체크..
	MItDependencyNodes IterNodes4(MFn::kSkinClusterFilter); // scene 의 노드들을 체크..

	for(i = 0; !IterNodes1.isDone(); IterNodes1.next(), i++) mObjects.append(IterNodes1.item());
	for(i = 0; !IterNodes2.isDone(); IterNodes2.next(), i++) mObjects.append(IterNodes2.item());
	for(i = 0; !IterNodes3.isDone(); IterNodes3.next(), i++) mObjects.append(IterNodes3.item());
	for(i = 0; !IterNodes4.isDone(); IterNodes4.next(), i++) mObjects.append(IterNodes4.item());

	MSelectionList mSelList;
	if(m_Option.bExportSelectedOnly)
	{
		MGlobal::getActiveSelectionList(mSelList);
	}

	// Object Count... 
	int nObjectCount = mObjects.length();

	// 진행 상황 대화상자 컨트롤 핸들
	::SendMessage(m_hWndPB, PBM_SETRANGE, 0, MAKELPARAM(0, nObjectCount));
	::SendMessage(m_hWndLB, LB_RESETCONTENT, 0, 0);
	::ShowWindow(m_hDlgProgress, SW_SHOW); // 진행 대화상자를 보이고..
	::ShowWindow(m_hDlgProgress, SW_RESTORE);

	m_bCancelExport = FALSE;

	for (i = 0; i < nObjectCount && FALSE == m_bCancelExport; i++)
	{
		// Dialog Message 처리...
		MSG msg;
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

		if(TRUE == m_bCancelExport) break; // Export Cancel 버튼을 누르면 취소..

		MObject mObj = mObjects[i];
		MFn::Type mType = mObj.apiType();

		if(m_Option.bExportSelectedOnly && IsSelected(mSelList, mObj) == false) continue; // 선택된 것만 Export 해야 하면..

		bool bExport = true;
		if(MFn::kCamera == mType && TRUE == m_Option.bExportCamera) // 카메라..
		{
			MFnCamera mCamera(mObj);
			const char* szCamera = mCamera.name().asChar();
			if(	strstr(szCamera, "front") == NULL && 
				strstr(szCamera, "side") == NULL && 
				strstr(szCamera, "top") == NULL)
			{
				CN3Camera* pCamera = this->ProcessCamera(mCamera);
				m_pScene->CameraAdd(pCamera);
			}
		}
		else if(	(MFn::kPointLight == mType ||
					MFn::kDirectionalLight == mType ||
					MFn::kAmbientLight == mType ||
					MFn::kSpotLight == mType) && 
					TRUE == m_Option.bExportLight )// 라이트
		{
			CN3Light* pLight = this->ProcessLight(MFnLight(mObj));
			pLight->m_Data.nNumber = m_pScene->LightCount(); // 라이트 번호 붙이기..
			m_pScene->LightAdd(pLight); // Scene 에 라이트 추가..
		}
		else if(mType == MFn::kMesh && TRUE == m_Option.bExportGeometry)
		{
			MFnMesh mMesh(mObj);

			const char* szMeshName = mMesh.name().asChar();

			bool bHaveJoint = false; // 만약 뼈대를 처리해야 하면..
			if(TRUE == m_Option.bExportCharacter) bHaveJoint = this->HaveJoint(mMesh);
			if(true == bHaveJoint) // 관절에 연결된 메시면 지나간다..
			{
//				wsprintf(szInfo, "Skinning 이 된 메시(%s)를 무시합니다.", mMesh.name().asChar());
//				nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
//				::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog
				continue; 
			}
			else
			{
				bool bProcessShape = false;
				if(mMesh.parentCount() > 0 && mMesh.parent(0).apiType() == MFn::kTransform)
				{
					MFnTransform mTM(mMesh.parent(0));
					if(mTM.childCount() == 1)  bProcessShape = true; // 첫번째 자식일 경우만 처리..
					else if(mTM.childCount() > 1 && mTM.child(0) == mMesh.object()) bProcessShape = true; // 첫번째 자식일 경우만 처리..
				}
				if(bProcessShape) ProcessShape(mMesh);
			}
		}
		else if(mType == MFn::kSkinClusterFilter && m_Option.bExportCharacter) // 스키닝 데이터 및 캐릭터....
		{
			MFnSkinCluster mSkin(mObj);
			this->ProcessChr(mSkin);
		}
		else
		{
			wsprintf(szInfo, "노드(%s) 무시합니다.", mObj.apiTypeStr());
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

			bExport = false;
		}

		if(bExport == true)
		{
			if(mObj.hasFn(MFn::kDependencyNode) == true)
			{
				MFnDependencyNode node(mObj);
				wsprintf(szInfo, "%d/%d - Type : %s Name : %s", i, nObjectCount, mObj.apiTypeStr(), node.name().asChar());
			}
			else
			{
				wsprintf(szInfo, "%d/%d - Type : %s NoName", i, nObjectCount, mObj.apiTypeStr());
			}
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog
			SendMessage(m_hWndPB, PBM_SETPOS, i+1, 0); // Progress dialog
		}
	}

	// Scene 저장.
	bool bSaveOK = true;
	if(m_bCancelExport == TRUE)
	{
		bSaveOK = false;
		int nYesNo = MessageBox(hWnd, "File 저장", "Export를 취소하셨습니다. 저장하시겠습니까?", MB_YESNO);
		if(nYesNo == IDYES) bSaveOK = true;
	}
	
	if(bSaveOK)
	{
		m_pScene->Tick(0);
		if(m_pScene->CameraCount() <= 0) m_pScene->DefaultCameraAdd(); // 기본 카메라
		if(m_pScene->LightCount() <= 0) m_pScene->DefaultLightAdd(); // 기본 라이트 추가..

		m_pScene->SaveDataAndResourcesToFile(m_szFileName); // Scene , Resource 저장..
		m_pScene->Release();
	}

	MAnimControl::setCurrentTime(mOldTime); // 원래 대로..
	MGlobal::viewFrame(mOldTime);

	delete m_pScene; m_pScene = NULL;
	delete g_pEng; g_pEng = NULL;
}

bool CN3E2Wrapper::ProcessIMesh(MFnMesh &mMesh, CN3IMesh* pIMesh)
{
	if(NULL == pIMesh) return false;
	pIMesh->Release();

	// 이름 짓기..
	MFnTransform mTM(mMesh.parent(0));
	pIMesh->m_szName = mTM.name().asChar();

	// Polygon 을 모두 삼각형 메시로 만들고 갯수를 세어준다..
	int nPC = mMesh.numPolygons();
	if(nPC <= 0) return false;

	int nFC = 0, nVI = 0, nUVI = 0;

	MIntArray mVIArray, mVITmp, mUVIArray;
	MFloatArray mFAU, mFAV;
	MPointArray mVArray;
	mMesh.getUVs(mFAU, mFAV);
	int nUVC = mFAU.length(); // UV 좌표 Count
	mMesh.getPoints(mVArray);
	int nVC = mVArray.length(); // Vertex Count
	
	std::vector<MVector> Normals(nVC, MVector());
	std::vector<int> iNormalCounts(nVC, 0);

	for(int i = 0; i < nPC; i++)
	{
		MVector vNTmp;
		mMesh.getPolygonNormal(i, vNTmp); // normal
		mMesh.getPolygonVertices(i, mVITmp); // polygon 에 있는 점 Index

		mMesh.getPolygonUVid(i, 0, nUVI);
		nVI = mVITmp[0];
		Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
		mVIArray.append(nVI); // Vertex Index
		mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

		mMesh.getPolygonUVid(i, 1, nUVI);
		nVI = mVITmp[1];
		Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
		mVIArray.append(nVI); // Vertex Index
		mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

		mMesh.getPolygonUVid(i, 2, nUVI);
		nVI = mVITmp[2];
		Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
		mVIArray.append(nVI); // Vertex Index
		mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

		nFC++;

		int nVITmp = mVITmp.length();
		for(int j = 3; j < nVITmp; j++) // 한면에 점이 네개 이상 있는 거면..
		{
			mMesh.getPolygonUVid(i, 0, nUVI);
			nVI = mVITmp[0];
			Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
			mVIArray.append(nVI); // Vertex Index
			mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

			mMesh.getPolygonUVid(i, nVITmp - j + 1, nUVI);
			nVI = mVITmp[nVITmp - j + 1];
			Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
			mVIArray.append(nVI); // Vertex Index
			mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

			mMesh.getPolygonUVid(i, nVITmp - j + 2, nUVI);
			nVI = mVITmp[nVITmp - j + 2];
			Normals[nVI] += vNTmp; iNormalCounts[nVI]++; // 면의 Normal 값 추가.
			mVIArray.append(nVI); // Vertex Index
			mUVIArray.append(nUVI); // VertexIndex, UV Index 기록..

			nFC++;
		}
	}

	pIMesh->Create(nFC, nVC, nUVC); // 메시 생성..

	MFnTransform mTransform(mMesh.parent(0));
	MMatrix mMtxWorld; mMtxWorld.setToIdentity();
	this->GetWorldTransform(mTransform, mMtxWorld);
	MMatrix mMtxWorldRot = mMtxWorld; // 회전행렬만 떼고..
	mMtxWorldRot.matrix[3][0] = mMtxWorldRot.matrix[3][1] = mMtxWorldRot.matrix[3][2] = 0;
	mMtxWorldRot.matrix[3][3] = 1.0;
	
	__VertexXyzNormal* pVDest = pIMesh->Vertices();
	for(i = 0; i < nVC; i++) 
	{
		MPoint mVTmp = mVArray[i]; // World Matrix 곱하고..
		mVTmp *= mMtxWorld;
		pVDest[i].x = (float)(mVTmp.x * 0.01); // Vertex Data 세팅 - meter 단위 이기 때문에 100으로 나누어 준다.
		pVDest[i].y = (float)(mVTmp.y * 0.01);
		pVDest[i].z = -(float)(mVTmp.z * 0.01); // Z 축은 반대로 한다.

		// Normal 값 처리...
//		if(iNormalCounts[i] > 0)
//		{
//			mNTmp.x = mNTmp.x / iNormalCounts[i];
//			mNTmp.y = mNTmp.y / iNormalCounts[i];
//			mNTmp.z = mNTmp.z / iNormalCounts[i];
//		}
		
		MVector mNTmp = Normals[i] * mMtxWorldRot;
		mNTmp.normalize();
		pVDest[i].n.x = (float)mNTmp.x;
		pVDest[i].n.y = (float)mNTmp.y;
		pVDest[i].n.z = -(float)mNTmp.z; // Z 축은 반대로 한다.

	}

	for(i = 0; i < nUVC; i++) pIMesh->UVSet(i, mFAU[i], 1.0f - mFAV[i]); // UV 데이터 세팅..
//	for(i = 0; i < nUVC; i++) pIMesh->UVSet(i, mFAU[i], mFAV[i]); // UV 데이터 세팅..
	for(i = 0; i < nFC; i++) // 
	{
		pIMesh->VertexIndexSet(i*3+0, mVIArray[i*3+0]); // Vertex Index 세팅
		pIMesh->VertexIndexSet(i*3+1, mVIArray[i*3+2]);
		pIMesh->VertexIndexSet(i*3+2, mVIArray[i*3+1]);

		if(nUVC > 0) // UV Index 가 있어야 한다..
		{
			pIMesh->UVIndexSet(i*3+0, mUVIArray[i*3+0]); // UV Index 세팅..
			pIMesh->UVIndexSet(i*3+1, mUVIArray[i*3+2]);
			pIMesh->UVIndexSet(i*3+2, mUVIArray[i*3+1]);
		}
	}

	if(m_Option.bGenerateSmoothNormal)
	{
		pIMesh->ReGenerateSmoothNormal();
	}

	return true;
}

bool CN3E2Wrapper::ProcessVMesh(MFnMesh &mMesh, CN3VMesh* pVMesh)
{
	if(NULL == pVMesh) return false;
	pVMesh->Release();

	// Polygon 을 모두 삼각형 메시로 만들고 갯수를 세어준다..
	int nPC = mMesh.numPolygons();
	if(nPC <= 0) return false;

	pVMesh->CreateVertices(nPC*3);
	__Vector3* pvDests = pVMesh->Vertices();

	MIntArray mVIs;
	MPointArray mVs; mMesh.getPoints(mVs); // 위치들..
	MPoint mPosTmp;

	MFnTransform mTransform(mMesh.parent(0)); // 월드 행렬
	MMatrix mMtxWorld; mMtxWorld.setToIdentity();
	this->GetWorldTransform(mTransform, mMtxWorld);
	
	for(int i = 0; i < nPC; i++)
	{
		mMesh.getPolygonVertices(i, mVIs); // polygon 에 있는 점 Index
		if(mVIs.length() != 3)
		{
			char szErr[256];
			wsprintf(szErr, "%s 는 삼각 폴리곤이 아닙니다.", mMesh.name().asChar());
			break;
		}

		for(int j = 0; j < 3; j++)
		{
			mPosTmp = mVs[mVIs[j]];
			mPosTmp *= mMtxWorld;

			pvDests[i*3+j].Set(	(float)(mPosTmp.x*0.01),
								(float)(mPosTmp.y*0.01),
								-(float)(mPosTmp.z*0.01) );
		}
	}

	// 이름 처리..
	pVMesh->m_szName = "";
	this->ProcessName(mTransform.object(), pVMesh->m_szName);
	std::string szFN = pVMesh->m_szName + ".N3VMesh";
	pVMesh->FileNameSet(szFN);

	return true;
}

bool CN3E2Wrapper::ProcessSkin(MFnSkinCluster &mSkin, CN3Skin* pSkin)
{
	if(NULL == pSkin) return false;
	pSkin->Release();

	int nLI = 0;
	DWORD dwRWC = 0;
	char szInfo[1024] = "";

	MObjectArray mMeshArray;
	mSkin.getOutputGeometry(mMeshArray);
	__ASSERT(mMeshArray.length() >= 1, "Input geometry count is not 1");
	MFnMesh mMeshOutput(mMeshArray[0]); // Original Mesh

	MStatus stat;
	MDagPath mSkinPath;
	int nGI = mSkin.indexForOutputConnection(0, &stat); // Geometry Index
	mSkin.getPathAtIndex(nGI, mSkinPath); // Geometry Path
	MItGeometry mGIt(mSkinPath); // iterate through the components of this geometry

	int nGVC = mGIt.count(); // Geometry Data
	MPointArray mVArray;
	mMeshOutput.getPoints(mVArray);
	int nVC = mVArray.length();
	if(nGVC != nVC) 
	{
		char szWarning[256];
		MFnDagNode nodeTmp(mMeshOutput.parent(0));
		wsprintf(szWarning, "Mesh - %s, Skin - %s", nodeTmp.name().asChar(), mSkin.name().asChar());
		MessageBox(::GetActiveWindow(), szWarning, "Warning - Skin vertex count is different to mesh vertex count", MB_OK);
	}











	if(false == this->ProcessIMesh(mMeshOutput, pSkin)) return false; // Indexed Mesh 처리.

	// Joint 가 그룹 되어 있다면 Parent Transform 처리..
/*	if(mJointRoot.parentCount() > 0 && mJointRoot.parent(0).hasFn(MFn::kTransform))
	{
		MFnTransform mTJP(mJointRoot.parent(0));

		// Normal 값 다시 정리..
		MMatrix mMtx; mMtx.setToIdentity();
		this->GetWorldTransform(mTJP, mMtx);
		MTransformationMatrix mTMtx(mMtx);

		double dRots[3];
		MTransformationMatrix::RotationOrder rotOrder = MTransformationMatrix::kXYZ;
		mTMtx.getRotation(dRots, rotOrder);
		__Matrix44 mtxRot;
		mtxRot.Rotation((float)dRots[0], (float)dRots[1], -(float)dRots[2]);

		int nVC = pIMesh->VertexCount();
		__VertexXyzNormal* pVs = pIMesh->Vertices();
		__Vector3 vNTmp;
		
		for(int i = 0; i < nVC; i++)
		{
			vNTmp = pVs[i].n;
			vNTmp *= mtxRot;
			pVs[i].n = vNTmp;
		}
	}
*/


	__VertexSkinned* pVBone = pSkin->SkinVertices();
	MFnTransform mTM(mMeshOutput.parent(0)); // Mesh, Joint 에 적용된 Transform
	MMatrix mMtxM; mMtxM.setToIdentity();
	this->GetWorldTransform(mTM, mMtxM);

	int iFind = pSkin->m_szName.find('_');
	if(iFind > 0) pSkin->m_szName = pSkin->m_szName.substr(iFind + 1); // 언더바가 있으면 잘라준다..
	pSkin->FileNameSet("Item\\" + pSkin->m_szName + ".N3Skin"); // 파일 이름 결정..

	for(int i = 0; !mGIt.isDone(); mGIt.next(), i++) // 루프를 돌면서 컴포넌트(Geometry 의 한점...)을 처리한다.
	{
		MObject mComp = mGIt.component();
		MFn::Type mType = mComp.apiType();

		// Weight 값을 구한다.
		MFloatArray mWTs;
		unsigned int nWTs;
		mSkin.getWeights(mSkinPath, mComp, mWTs, nWTs);
		if (nWTs == 0)
		{
			lstrcpy(szInfo, "Skin 처리 오류 : Weight 값이 없습니다.");
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

			continue;
		}

		MPoint mPt;
		mPt = mGIt.position();
//		mPt = mVArray[j]; // 원래 이게 정상이지만.. 바인딩 포즈로 돌아가야 한다..

		mPt *= mMtxM; // 행렬을 곱해주고..
		mPt.x *= 0.01; mPt.y *= 0.01; mPt.z *= -0.01; // Z 축은 음수로..
		
		pVBone[i].vOrigin.Set((float)mPt.x, (float)mPt.y, (float)mPt.z);

		
		// Weight 값이 0이상인 것만 고른다..
		int nAffect = 0;
		static int s_nJoints[128];
		static float s_fWeights[128];
		for (int j = 0; j < (int)nWTs ; j++ )
		{
			if(mWTs[j] > 0) 
			{
				s_nJoints[nAffect] = j;
				s_fWeights[nAffect] = mWTs[j];
				nAffect++;
			}
		}

		if(nAffect > 0)
		{
			pVBone[i].nAffect = nAffect;
			pVBone[i].pnJoints = new int[nAffect];
			if(nAffect > 1) pVBone[i].pfWeights = new float[nAffect];

			for (j = 0; j < nAffect; j++ )
			{
				pVBone[i].pnJoints[j] = s_nJoints[j];
				if(nAffect > 1) pVBone[i].pfWeights[j] = s_fWeights[j];
			}
		}
	}

	return true;
}

bool CN3E2Wrapper::ProcessShape(MFnMesh& mMesh)
{
	if(mMesh.numPolygons() > 10000) // 면의 갯수가 만개 넘어가면... CN3IMesh 로 처리 불가능...
	{
		CN3VMesh VMesh;
		this->ProcessVMesh(mMesh, &VMesh);
		VMesh.SaveToFile();
		return false;
	}

	int nLI = 0;
	DWORD dwRWC = 0;
	char szInfo[1024] = "";

	MFnDagNode mNodeTmp(mMesh.object());
	MFnTransform mTG;
	bool bValid = false;
	while(1)
	{
		if(mNodeTmp.parent(0).apiType() == MFn::kWorld)
		{
			mTG.setObject(mNodeTmp.object());
			bValid = true;
			break;
		}

		mNodeTmp.setObject(mNodeTmp.parent(0));
	}
	if(false == bValid) return false;

	MFnTransform mTM(mMesh.parent(0));

	std::string szNodeFullName;
	this->ProcessName(mTG.object(), szNodeFullName);
	int nSC = m_pScene->ShapeCount();
	CN3Shape* pShape = NULL;
	for(int i = 0; i < nSC; i++)
	{
		if(szNodeFullName == m_pScene->ShapeGet(i)->m_szName)
		{
			pShape = m_pScene->ShapeGet(i);
			break;
		}
	}

	if(NULL == pShape)
	{
		pShape = new CN3Shape();
		this->ProcessTransform(mTG, pShape);
		pShape->m_szName = ""; this->ProcessName(mTG.object(), pShape->m_szName); // 이름을 다시정하기.
		pShape->FileNameSet("Object\\" + pShape->m_szName + ".N3Shape"); // 파일 이름 정하기..
		
		m_pScene->ShapeAdd(pShape);

		MMatrix mMtxJWorld; mMtxJWorld.setToIdentity();
		this->GetWorldTransform(mTG, mMtxJWorld);

		// 피벗점 계산
		MPoint mvPivot;
		mvPivot = mTG.rotatePivot(MSpace::kTransform);
		pShape->PosSet((float)(mvPivot.x*0.01), (float)(mvPivot.y*0.01), -(float)(mvPivot.z*0.01));
	}

	CN3IMesh IMesh;
	this->ProcessIMesh(mMesh, &IMesh);
	if(IMesh.FaceCount() <= 0) return false;

	// 이름에 "collision" 이라는 문자열이 들어가면.. 충돌 메시다..
	std::string szTmp = IMesh.m_szName;
	if(szTmp.size()) CharLower(&(szTmp[0]));
	if(szTmp.find("coll") != -1 || szTmp.find("climb") != -1)
	{
		// 메시의 점위치를 피벗점에 대해 다시 계산.. Shape 의 로컬 좌표로 맞추어 준다..
		IMesh.ApplyOffset(pShape->Pos() * -1.0f);
		int nFC = IMesh.FaceCount();

		CN3VMesh* pVMesh = new CN3VMesh();
		szNodeFullName = ""; this->ProcessName(mTM.object(), szNodeFullName);
		pVMesh->m_szName = mTM.name().asChar(); // 이름 다시 정하고..
		pVMesh->FileNameSet("Object\\" + szNodeFullName + ".N3VMesh"); // 파일 이름 결정..

		pVMesh->CreateVertices(nFC*3);

		__VertexT1* pVSrc = IMesh.BuildVertexList();
		__Vector3* pVDest = pVMesh->Vertices();
		for(i = 0; i < nFC * 3; i++)
		{
			pVDest[i].x = pVSrc[i].x; // 위치만 세팅해 준다...
			pVDest[i].y = pVSrc[i].y; // 위치만 세팅해 준다...
			pVDest[i].z = pVSrc[i].z; // 위치만 세팅해 준다...
		}
		pShape->s_MngVMesh.Add(pVMesh);
		if(szTmp.find("coll") != -1) pShape->CollisionMeshSet(pVMesh->FileName());
		else if(szTmp.find("climb") != -1) pShape->ClimbMeshSet(pVMesh->FileName());
	}
	else // 충돌 메시가 아니면..
	{
		CN3SPart* pPD = pShape->PartAdd(); // Part 추가 해주고..
		szNodeFullName = ""; this->ProcessName(mTM.object(), szNodeFullName);
		pPD->m_szName = mTM.name().asChar(); // Part 이름 짓기..
		pPD->FileNameSet("Object\\" + szNodeFullName + "N3CPart");

		// 피벗점 계산
		MPoint mvPivot = mTM.rotatePivot(MSpace::kTransform);
		pPD->m_vPivot.Set((float)(mvPivot.x*0.01), (float)(mvPivot.y*0.01), -(float)(mvPivot.z*0.01));
		pPD->m_vPivot -= pShape->Pos();

		// 메시의 점위치를 Shape 파트의 피벗점에 대해 다시 계산.. Shape의 part 로컬 좌표로 맞추어 준다..
		__Vector3 vOffset((float)(mvPivot.x * 0.01), (float)(mvPivot.y * 0.01), -(float)(mvPivot.z * 0.01));
		IMesh.ApplyOffset(vOffset * -1.0f);

		// CN3Mesh 로 Convert
		CN3Mesh N3Mesh;
		N3Mesh.Import(&IMesh);
		N3Mesh.MakeIndexed();
		if(m_Option.bGenerateSmoothNormal)
		{
			N3Mesh.ReGenerateSmoothNormal();
		}

		// Progressive Mesh 생성..
		CN3PMeshCreate PMeshCreate;
		if (PMeshCreate.ConvertFromN3Mesh(&N3Mesh)==false)
		{
			MessageBox(::GetActiveWindow(), "변환할 수 없거나 내용이 없는 N3Mesh입니다.", "PMesh Convert error", MB_OK);
			//return;
		}

		// 자동 PMesh 생성 옵션
		PMeshCreate.m_PMCOption.bUseSumOfLoss = true;	// Loss의 합을 사용
		PMeshCreate.m_PMCOption.bTriangleWeight = true;	// 사라지는 삼각형 가중치 사용
		PMeshCreate.m_PMCOption.bArea = true;	// false : 세변은 길이의 합으로 한다.
		PMeshCreate.m_PMCOption.fWeight = 1.0f;	// 사라지는 삼각형 가중치 (중요도)

		CN3PMesh *pPMesh = PMeshCreate.CreateRendererMesh(); // Progressive Mesh 생성
		pPMesh->m_szName = mTM.name().asChar(); // 걍 이름..
		std::string szFN; this->ProcessName(mTM.object(), szFN);

		int iLen = szFN.size();
		if(m_Option.bGenerateFileName && iLen >= 11) // Item Code ::: 0_2345_78_0
			szFN = szFN.substr(iLen - 11);
		pPMesh->FileNameSet("Object\\" + szFN + ".N3PMesh"); // 파일 이름 결정..

		pShape->s_MngPMesh.Add(pPMesh); // Progressive Mesh Manager 에 추가..
		pPD->MeshSet(pPMesh->FileName()); // Mesh 세팅..
		
		////////////////////////////////////////////////
		// 재질 및 텍스처 처리..
		pPD->m_Mtl.dwColorArg1 = D3DTA_DIFFUSE;
		pPD->m_Mtl.dwColorArg2 = D3DTA_TEXTURE;

		CN3Texture* pTex = this->ProcessTexture(mMesh);
		if(pTex) // 텍스처가 쓰인 재질이면 재질은 기본적인 흰색.. 컬러 오퍼레이션은 Modulate
		{
			std::string szTFN = pTex->FileName(); // 파일 이름을 검사해서..
			if(szTFN.size() > 0) CharLower(&(szTFN[0]));
			if(-1 == szTFN.find("object\\")) // "Item\" 이라는 문자열이 없으면
			{
				szTFN = "Object\\" + pTex->FileName(); // Item 이라는 경로를 붙인다..
				pTex->FileNameSet(szTFN);
			}

			
			pPD->m_Mtl.dwColorOp = D3DTOP_MODULATE;
			pPD->TexAlloc(1);
			pPD->TexSet(0, pTex); // Texture Setting ... 
			
//			D3DFORMAT fmtTex = pTex->PixelFormat();
//			if(	fmtTex == D3DFMT_DXT2 ||
//				fmtTex == D3DFMT_DXT3 || 
//				fmtTex == D3DFMT_DXT4 || 
//				fmtTex == D3DFMT_DXT5 ) pPD->m_Mtl.nRenderFlags = RF_ALPHABLENDING;
		}
		else
		{
//			this->ProcessMaterial(this->MeshGetShader(mMesh), &(pPD->m_Mtl));
			pPD->m_Mtl.dwColorOp = D3DTOP_SELECTARG1;
		}
		// 재질 및 텍스처 처리..
		////////////////////////////////////////////////
	}

	return true;
}

CN3Joint* CN3E2Wrapper::ProcessJoint(MFnIkJoint &mJoint)
{
	CN3Joint* pJoint = new CN3Joint();
	//////////////////////////////////////////////////////////////////////////////////////////
	// Joint 의 Transform을 처리..
	this->ProcessTransform(mJoint, pJoint);
	
	// Rotation Order
	MTransformationMatrix::RotationOrder rotOrder = mJoint.rotationOrder(); // MTransformationMatrix::kXYZ;
//	if(rotOrder == MTransformationMatrix::kXYZ) pJoint->m_RotSeq = CN3Joint::ROT_SEQ_XYZ;
//	else if(rotOrder == MTransformationMatrix::kYXZ) pJoint->m_RotSeq = CN3Joint::ROT_SEQ_YXZ;
//	else
//	{
//		MessageBox(::GetActiveWindow(), "NotSupported rotation order. Must kXYZ or kYXZ", "Joint export Warning", MB_OK);
//	}

	char szName[512];
	if(mJoint.parent(0).apiType() == MFn::kTransform)
	{
		MFnTransform mTJ(mJoint.parent(0));
		wsprintf(szName, "Chr\\%s.N3Joint", mTJ.name().asChar()); // 파일 이름 정하기..
	}
	else
	{
		wsprintf(szName, "Chr\\%s.N3Joint", mJoint.name().asChar()); // 파일 이름 정하기..
	}
	
	pJoint->m_szName = mJoint.name().asChar(); // 이름짓기.. FullName 으로 짓지는 않는다..

	// 회전 축 값을 구한다..
	if(pJoint->m_KeyOrient.Count() <= 0) // Joint Orient Key 값이 없으면.. Rotation Key 값을 Orient 만큼 비튼다..
	{
		double dAxis[3];
		MStatus stat = MStatus::kSuccess;
		stat = mJoint.getOrientation(dAxis, rotOrder); // local axis...

		// Origin Rotation 값이 있으면..
		if(dAxis[0] || dAxis[1] || dAxis[2]) // Origin Rotation 이 있으면..
		{
			__Matrix44 mtxRot;
			mtxRot.Rotation((float)-dAxis[0], (float)-dAxis[1], (float)dAxis[2]);
			__Quaternion qRot = mtxRot;
			this->ProcessJointTransform(pJoint, NULL, &qRot, NULL, false);
		}
	}
	
	// 차일드 조인트 처리..
	int nCC = mJoint.childCount();
	for(int i = 0; i < nCC; i++)
	{
		MObject mObj = mJoint.child(i);
		if(mObj.hasFn(MFn::kJoint)) // Joint 일 경우..
		{
			MFnIkJoint mJointChild(mObj);
			CN3Joint* pJointChild = this->ProcessJoint(mJointChild);
			pJoint->ChildAdd(pJointChild);
		}
		else if(mObj.hasFn(MFn::kTransform)) // 보통 Transform 일 경우 차일드가 메시 인지 본다..
		{
			int ttt = 0;
		}
	}

	return pJoint;
}

void CN3E2Wrapper::ProcessJointTransform(CN3Joint *pJoint, __Vector3* pvTrans, __Quaternion* pqRot, __Vector3* pvScale, bool bProcessChild)
{
	if(NULL == pJoint) return;

	bool bRoot = true;
	if(pJoint->Parent()) bRoot = false;

	if(pvScale)
	{
		pJoint->PosSet(pJoint->Pos() * (*pvScale)); // trans 키를 스케일에 따라 맞추고..
		int nK = pJoint->m_KeyPos.Count(); // 에니메이션 키도 변경해준다..
		for(int i = 0; i < nK; i++)
		{
			__Vector3* pvKey = (__Vector3*)(pJoint->m_KeyPos.DataGet(i));
			(*pvKey) *= (*pvScale);
		}
	}

	if(pvTrans) // 이동값 적용
	{
		pJoint->PosSet(pJoint->Pos() + (*pvTrans));
		int nK = pJoint->m_KeyPos.Count(); // 에니메이션 키도 변경해준다..
		for(int i = 0; i < nK; i++)
		{
			__Vector3* pvKey = (__Vector3*)(pJoint->m_KeyPos.DataGet(i));
			(*pvKey) += (*pvTrans);
		}
	}
	
	if(pqRot) // 회전값을 변경해준다..
	{
		__Quaternion qRot2 = pJoint->Rot() * (*pqRot);
		pJoint->RotSet(qRot2);

		int nK = pJoint->m_KeyRot.Count(); // 회전값 키도 변경해준다..
		for(int i = 0; i < nK; i++)
		{
			__Quaternion* pqRot2 = (__Quaternion*)pJoint->m_KeyRot.DataGet(i);
			(*pqRot2) *= (*pqRot);
		}
	}
	
	if(bProcessChild && pvScale) // 하위 노드 처리 ?? 
	{
		int nCC = pJoint->ChildCount();
		for(int i = 0; i < nCC; i++) 
			this->ProcessJointTransform((CN3Joint*)pJoint->Child(i), NULL, NULL, pvScale, true); // 하위 노드들은 스케일만 처리해 준다..
	}
}

void CN3E2Wrapper::ProcessMatrix(MFnTransform &mTransform, __Matrix44& mtx, __Vector3& vPos, __Quaternion& qtRot, __Vector3& vScale)
{
	// 행렬 변환..
	static double dRs[4], dSs[3];
//	MTransformationMatrix::RotationOrder RotOrder = MTransformationMatrix::kXYZ;
//	mTransform.getRotation(dRs, RotOrder, MSpace::kTransform); // 회전
	MVector mVecPos;
	mVecPos = mTransform.translation(MSpace::kTransform);
	mVecPos *= 0.01; // 100 분의 1로.. 미터 좌표로 맞춘다..
	mVecPos.z = -mVecPos.z;

	mTransform.getScale(dSs); // 확대

	vPos.Set((float)mVecPos.x, (float)mVecPos.y, (float)mVecPos.z);
//	vRot.Set((float)-dRs[0], (float)-dRs[1], (float)dRs[2]);
	vScale.Set((float)dSs[0], (float)dSs[1], (float)dSs[2]);

	mTransform.getRotationQuaternion(dRs[0], dRs[1], dRs[2], dRs[3]); // 회전 쿼터니언
	qtRot.x = (float)dRs[0];
	qtRot.y = (float)dRs[1];
	qtRot.z = (float)dRs[2];
	qtRot.w = (float)dRs[3];

	__Vector3 vAxis; float fAngle;
	qtRot.AxisAngle(vAxis, fAngle);
	vAxis.x *= -1; // x 축 회전이 반대..
	vAxis.y *= -1; // y 축 회전이 반대..
	vAxis.Normalize();
	qtRot.RotationAxis(vAxis, fAngle);

	// 최종 행렬 계산..
	mtx.Identity();
	static __Matrix44 mtxTmp;
//	mtxTmp.Rotation((float)-dRs[0], (float)-dRs[1], (float)dRs[2]);	mtx *= mtxTmp;
	mtxTmp = qtRot; mtx *= mtxTmp;
	mtxTmp.Scale((float)dSs[0], (float)dSs[1], (float)dSs[2]);		mtx *= mtxTmp;
	mtx.PosSet((float)mVecPos.x, (float)mVecPos.y, (float)mVecPos.z); // 위치..
}

bool CN3E2Wrapper::FindAnimCurve(MObject &mObj, MObjectArray &mAnimCurveArray)
{
	MItDependencyGraph::Direction Direction = MItDependencyGraph::kUpstream;
//	MItDependencyGraph::Traversal TraversalType = MItDependencyGraph::kBreadthFirst;
//	MItDependencyGraph::Level Level = MItDependencyGraph::kNodeLevel;
	MItDependencyGraph::Traversal TraversalType = MItDependencyGraph::kDepthFirst;
	MItDependencyGraph::Level Level = MItDependencyGraph::kPlugLevel;
	MFn::Type Filter = MFn::kAnimCurve;
	
	MStatus stat = MS::kSuccess;
	MItDependencyGraph dgIter(mObj, Filter, Direction, TraversalType, Level, &stat);

	bool bFind = false;
	for (int i = 0; !dgIter.isDone(); dgIter.next(), i++)
	{
		bFind = true;
		mAnimCurveArray.append(dgIter.thisNode(&stat));
	}

	return bFind;
}

bool CN3E2Wrapper::HaveJoint(MFnMesh &mMesh)
{
	MFnTransform mTransform(mMesh.parent(0));
	int nCC = mTransform.childCount();
	int nMC = 0;
	for(int i = 0; i < nCC; i++)
	{
		if(mTransform.child(i).apiType() == MFn::kMesh) nMC++;
	}

	if(nMC >= 2) // return true; // 두개보다 크면.. 
	{
		MItDependencyGraph::Direction Direction = MItDependencyGraph::kUpstream;
		MItDependencyGraph::Traversal TraversalType = MItDependencyGraph::kBreadthFirst;
		MItDependencyGraph::Level Level = MItDependencyGraph::kNodeLevel;
		MFn::Type Filter = MFn::kJoint;

		MItDependencyGraph dgIter(mMesh.object(), Filter, Direction, TraversalType, Level);

		for (int i = 0; !dgIter.isDone(); dgIter.next(), i++)
		{
			MObject mObjTmp = dgIter.thisNode();
			if(mObjTmp.apiType() == MFn::kJoint)
				return true; // 조인트가 있으면..
		}
	}
	return false;
}

// 텍스처 인덱스 계산..
CN3Texture* CN3E2Wrapper::ProcessTexture(MFnMesh &mMesh)
{
	int nLI = 0;
	DWORD dwRWC = 0;
	char szInfo[1024] = "";

	MStringArray szResult;
	MFnLambertShader mShader(this->MeshGetShader(mMesh));
	MString szCmd = MString("listConnections ") + mShader.name() + MString(".color;");
	MGlobal::executeCommand( szCmd, szResult );

	MString szTexture = szResult[0];
	MString szSurface = mMesh.name();

	if(szTexture.length() <= 0) return NULL;

	// 사이즈를 가져온다..
	MString szCommand = MString( "getAttr " ) + szTexture + MString( ".outSize" );
	MIntArray nWH;
	if(MGlobal::executeCommand(szCommand, nWH) != MS::kSuccess)
	{
		wsprintf(szInfo, "텍스처 파일 처리 오류 : Surface - %s, Texture - %s", szSurface.asChar(), szTexture.asChar());
		nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
		::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

		return NULL;
	}

//	int nW = nWH[1], nH = nWH[0]; // 사이즈를 알아내고..
//	if(nW < 4 || nH < 4)
//	{
//		wsprintf(szInfo, "텍스처 파일 처리 오류 : 너비, 높이가 너무 작습니다. Surface - %s, Texture - %s", szSurface.asChar(), szTexture.asChar());
//		nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
//		::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog
//
//		return NULL;
//	}

	// 텍스처를 변환한다..
	MString szFile;
	MStringArray szResult2;
	szCommand = MString("getAttr ") + szTexture + MString(".fileTextureName");
	MGlobal::executeCommand(szCommand, szResult2);
	szFile = szResult2[0];

	////////////////////////////////////////////
	// Texture List 에 등록되어 있는지 본다..
	char szFNSrc[1024]; // 파일 이름을 정하고..
	lstrcpy(szFNSrc, szFile.asChar());
	int nFN = lstrlen(szFNSrc);
	for(int i = 0; i < nFN; i++) 
		if(szFNSrc[i] == '/') szFNSrc[i] = '\\';

	// 문자열에  ":" 이 없으면 앞의 두글자가 '\\' 혹은 '//' 이 아니면 풀네임을 만들어야 한다.
	if(!strstr(szFNSrc, ":") && !(szFNSrc[0] == '\\' && szFNSrc[1] == '\\'))
	{
		MString szWorkSpace;
		MGlobal::executeCommand(MString("workspace -fullName"), szWorkSpace);
		lstrcpy(szFNSrc, szWorkSpace.asChar());
		lstrcat(szFNSrc, szFile.asChar());
	}
	// WorkSpace 이름을 가져오고..

	char szFNDest[1024]; // 저장할 이름
	lstrcpy(szFNDest, szFNSrc);
	nFN = lstrlen(szFNDest);
	for(i = nFN - 1; i >= 0; i--) // 저장할 이름을 만든다..
	{
		if(szFNDest[i] == '.') { szFNDest[i+1] = 'D'; szFNDest[i+2] = 'X'; szFNDest[i+3] = 'T'; szFNDest[i+4] = NULL; }
		if(szFNDest[i] == '\\' || szFNDest[i] == '/') 
		{
			lstrcpy(szFNDest, &szFNDest[i+1]);
			break;
		}
	}
	
	CN3Texture* pTex = NULL;

	static char szFNs[1024][256]; // 파일 이름이 중복되는지 체크...
	if(m_pScene->s_MngTex.Count() <= 0) memset(szFNs, 0, sizeof(szFNs));
	for(i = 0; i < 1024; i++)
	{
		if(NULL == szFNs[i][0]) break;
		if(lstrcmpi(szFNDest, szFNs[i]) == 0)
		{
			pTex = m_pScene->s_MngTex.Get(szFNDest);
			return pTex;
		}
	}

	lstrcpy(szFNs[i], szFNDest);
	pTex = new CN3Texture();
	if(pTex->LoadFromFile(szFNSrc)) // 파일을 읽고...
	{
		pTex->m_szName = szFNDest; // 이름 정하기.. - 파일 이름으로 정한다.
		pTex->FileNameSet(szFNDest); // 파일 이름 정하기.
		CN3Base::s_MngTex.Add(pTex); // Manager 에 등록
	}
	else
	{
		wsprintf(szInfo, "텍스처 파일 처리 오류 : 파일을 읽을수 없습니다. - %s", szFNSrc);
		nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
		::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

		delete pTex;
		return NULL;
	}

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

	return pTex;
/*
	// 이부분은 Maya Image 를 직접 읽는 부분이다..
	IFFimageReader Reader;
	if(Reader.open(szFile) == MS::kSuccess)
	{
		int nW2, nH2;
		Reader.getSize(nW2, nH2);
		if(nW != nW2 || nH != nH2)
		{
			wsprintf(szInfo, "텍스처 파일 처리 오류 : 파일을 읽을수 없습니다. - %s", szFile.asChar());
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

			delete pTex;
			return NULL;
		}

	//	if(Reader.isRGB()) nBPP += 3;
	//	else
	//	{
	//		Reader.close();
	//		DeleteFile(szFile.asChar()); // 파일 지우기..
	//		wsprintf(szInfo, "텍스처 파일 처리 오류 : GrayScale Texture 는 차후에 지원 됩니다. Surface - %s, Texture - %s", szSurface.asChar(), szTexture.asChar());
	//		::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
	//		::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog
	//		return false;
	//	}
		
		// Surface 생성 및 채우기.
		LPDIRECT3DSURFACE8 lpSurf;
		g_pEng->s_lpD3DDev->CreateImageSurface(nW, nH, D3DFMT_A8R8G8B8, &lpSurf);

		if(NULL == lpSurf)
		{
			Reader.close();
			wsprintf(szInfo, "텍스처 파일 처리 오류 : Direct3D Texture 생성 실패(%d, %d)", nW, nH);
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

			return NULL;
		}

		Reader.readImage(); // 이미지 읽기..
		int nBPP = 0;
		nBPP = Reader.getBytesPerChannel() * 4;
		BOOL bAlpha = Reader.hasAlpha();
		const unsigned char * pBuffer = Reader.getPixelMap();

		D3DLOCKED_RECT LR;
		lpSurf->LockRect(&LR, NULL, 0);
		unsigned char *pDest = NULL, *pSrc = NULL;
		for(int y = 0; y < nH; y++)
		{
			pSrc = (unsigned char*)pBuffer + (nH - y - 1) * nW * nBPP; // 거꾸로 저장되어 있다..
			pDest = (unsigned char*)LR.pBits + y * LR.Pitch;
			for(int x = 0; x < nW; x++, pDest += 4, pSrc += nBPP)
			{
				*(pDest+3) = *(pSrc+3); // Alpha
				*(pDest+2) = *(pSrc+2); // Red
				*(pDest+1) = *(pSrc+1); // Green
				*(pDest+0) = *(pSrc+0); // Blue
			}
		}
		lpSurf->UnlockRect();
		Reader.close();

		if(bAlpha) pTex->CreateFromSurface(lpSurf, D3DFMT_DXT3, TRUE);
//		if(bAlpha) pTex->CreateFromSurface(lpSurf, D3DFMT_DXT4, TRUE);
		else pTex->CreateFromSurface(lpSurf, D3DFMT_DXT1, TRUE);
		
		lpSurf->Release(); lpSurf = NULL;

		if(pTex->Get() == NULL)
		{
			wsprintf(szInfo, "### !!! Texture Export 실패(%.3d) : w,h,w2,h2(%.4d, %.4d, %.4d, %.4d) FileName : \"%s\" TextureName \"%s\" MeshName - \"%s\"", 
				m_pScene->s_MngTex.Count(), pTex->Width(), pTex->Height(), nW, nH, szFile.asChar(), pTex->m_szName.c_str(), mMesh.name().asChar());
			nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
			::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog

			delete pTex;
			return NULL;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Output Window 에 텍스처 출력..
	wsprintf(szInfo, "### Texture Export (%.3d) : w,h,w2,h2(%.4d, %.4d, %.4d, %.4d) FileName : \"%s\" TextureName \"%s\" MeshName - \"%s\"", 
		m_pScene->s_MngTex.Count(), pTex->Width(), pTex->Height(), nW, nH, szFile.asChar(), pTex->m_szName.c_str(), mMesh.name().asChar());
	nLI = ::SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM)szInfo); // Progress dialog
	::SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)nLI,0); // Progress dialog
*/
}

int CN3E2Wrapper::ProcessMaterial(MObject mShaderObj, __Material *pMtl)
{
	if(NULL == pMtl) return -1;

	pMtl->Init();

	MColor DC, AC, SC, TC, EC(0.0, 0.0, 0.0, 0.0);
	AC = SC = TC = EC;
	DC.r = 1.0; DC.g = 1.0; DC.b = 1.0; DC.a = 1.0;

	if(mShaderObj.apiType() == MFn::kLambert)
	{
		MFnLambertShader mShader(mShaderObj);
		DC = mShader.color();
		AC = mShader.ambientColor();
//		SC = mShader.specularColor();
		TC = mShader.transparency();
		EC = mShader.incandescence();
	}
	else if(mShaderObj.apiType() == MFn::kReflect)
	{
		MFnReflectShader mShader(mShaderObj);
		DC = mShader.color();
		AC = mShader.ambientColor();
		SC = mShader.specularColor();
		TC = mShader.transparency();
		EC = mShader.incandescence();
	}
	else if(mShaderObj.apiType() == MFn::kBlinn)
	{
		MFnBlinnShader mShader(mShaderObj);
		DC = mShader.color();
		AC = mShader.ambientColor();
		SC = mShader.specularColor();
		TC = mShader.transparency();
		EC = mShader.incandescence();
	}
	else if(mShaderObj.apiType() == MFn::kPhong)
	{
		MFnPhongShader mShader(mShaderObj);
		DC = mShader.color();
		AC = mShader.ambientColor();
		SC = mShader.specularColor();
		TC = mShader.transparency();
		EC = mShader.incandescence();
	}
	else
	{
	}

	pMtl->Diffuse.r = DC.r;
	pMtl->Diffuse.g = DC.g;
	pMtl->Diffuse.b = DC.b;
	pMtl->Diffuse.a = ((1.0f - TC.r) + (1.0f - TC.g) + (1.0f - TC.b))/3.0f; // 투명값..

	pMtl->Ambient.r = DC.r * 0.7f;
	pMtl->Ambient.g = DC.g * 0.7f;
	pMtl->Ambient.b = DC.b * 0.7f;
	pMtl->Ambient.a = pMtl->Diffuse.a;

//	pMtl->Specular.r = SC.r;
//	pMtl->Specular.g = SC.g;
//	pMtl->Specular.b = SC.b;
//	pMtl->Specular.a = 1.0f;

//	pMtl->Emissive.r = EC.r;
//	pMtl->Emissive.g = EC.g;
//	pMtl->Emissive.b = EC.b;
//	pMtl->Emissive.a = 1.0f;

//	pMtl->Power = 0.0f;

	return 0;
}

int CN3E2Wrapper::ProcessTransform(MFnTransform &mTransform, CN3Transform *pTransform)
{
	int nLI = 0;
	char szInfo[1024] = "";

	if(NULL == pTransform) return -1;
	pTransform->m_szName = mTransform.name().asChar();  // 이름 짓기....

	__Vector3 vPos, vScale;
	__Quaternion qtRot;
	this->ProcessMatrix(mTransform, pTransform->m_Matrix, vPos, qtRot, vScale); // 행렬 처리..

	if(pTransform->Type() & OBJ_CAMERA) // 카메라 일경우
	{
		CN3Camera* pCamera = (CN3Camera*)pTransform;
		
		pCamera->EyePosSet(vPos);
		__Matrix44 mtxR = qtRot;
		__Vector3 vAt(0,0,50); // 떨어진 거리만큼...
		vAt *= mtxR; 
		pCamera->AtPosSet(vAt);
		pCamera->UpVectorSet(0,1,0);
	}
	else
	{
		pTransform->PosSet(vPos);
		pTransform->RotSet(qtRot); // 회전은 쿼터니언으로 들어간다..
		pTransform->ScaleSet(vScale);
	}

	if(FALSE == m_Option.bAnimationKey) return -1; // Animation Key 처리 옵션이 켜질 경우만 한다..

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 에니메이션..
	MObjectArray mAKs;
	this->FindAnimCurve(mTransform.object(), mAKs);
//	MStatus stat2 = mTransform.setRotationOrder(MTransformationMatrix::kXYZ, true);

	MFnAnimCurve mACs[3][3]; // Position, Rotation, Scale...
	MFnAnimCurve mACJointOrients[3]; // Joint Orient

	int nTranslateCount = 0;

	int nAK = mAKs.length();
	int nKType = -1, nT = 0, nR = 0, nS = 0;
	char szTmp[512];
	for(int i = 0; i < nAK; i++)
	{
		MFnAnimCurve mAC(mAKs[i]);

		lstrcpy(szTmp, mAC.name().asChar()); // 일단 이름으로 비교해보고.....
		CharLower(szTmp);

		if(NULL != strstr(szTmp, "translatex")) // 카메라의 경우에는 translate 값이 두개가 들어온다..
		{
			 nTranslateCount++; 
		}

		nKType = -1;
		if(NULL != strstr(szTmp, "translatex")) { if(nTranslateCount == 2) nKType = 3; else nKType = 0; }
		else if(NULL != strstr(szTmp, "translatey")) { if(nTranslateCount == 2) nKType = 4; else nKType = 1; }
		else if(NULL != strstr(szTmp, "translatez")) { if(nTranslateCount == 2) nKType = 5; else nKType = 2; }
		else if(NULL != strstr(szTmp, "rotatex")) nKType = 3;
		else if(NULL != strstr(szTmp, "rotatey")) nKType = 4;
		else if(NULL != strstr(szTmp, "rotatez")) nKType = 5;
		else if(NULL != strstr(szTmp, "scalex")) nKType = 6;
		else if(NULL != strstr(szTmp, "scaley")) nKType = 7;
		else if(NULL != strstr(szTmp, "scalez")) nKType = 8;
		else if(NULL != strstr(szTmp, "jointorientx")) nKType = 9;
		else if(NULL != strstr(szTmp, "jointorienty")) nKType = 10;
		else if(NULL != strstr(szTmp, "jointorientz")) nKType = 11;
		else if(NULL != strstr(szTmp, "visibility")) nKType = -1;
		else // 이름으로 판단할수 없다면.. ID 로 판단한다...??
		{
//			MTypeId id = mAC.typeId();
//			DWORD dwID = id.id();
//			if(dwID == 0x5043544c) { nKType = 0 + nT; nT++; } // Translate
//			else if(dwID == 0x50435441) { nKType = 3 + nR; nR++; } // Rotate
//			else if(dwID == 0x50435455) { nKType = 6 + nS; nS++; } // Scale
//			else nKType = -1;
		
			lstrcpy(szTmp, mAC.typeName().asChar());
			if(NULL != strstr(szTmp, "animCurveTL")) { nKType = 0 + nT; nT++; } // Translate
			else if(NULL != strstr(szTmp, "animCurveTA")) { nKType = 3 + nR; nR++; } // Rotate
			else if(NULL != strstr(szTmp, "animCurveTU")) { nKType = 6 + nS; nS++; } // Scale
			else nKType = -1;
		}

		CN3AnimKey* pKey = NULL;
		if(nKType == 0) { mACs[0][0].setObject(mAC.object()); }
		else if(nKType == 1) { mACs[0][1].setObject(mAC.object()); }
		else if(nKType == 2) { mACs[0][2].setObject(mAC.object()); }
		else if(nKType == 3) { mACs[1][0].setObject(mAC.object()); }
		else if(nKType == 4) { mACs[1][1].setObject(mAC.object()); }
		else if(nKType == 5) { mACs[1][2].setObject(mAC.object()); }
		else if(nKType == 6) { mACs[2][0].setObject(mAC.object()); }
		else if(nKType == 7) { mACs[2][1].setObject(mAC.object()); }
		else if(nKType == 8) { mACs[2][2].setObject(mAC.object()); }

		// Joint Orient Key 값..
		else if(nKType == 9) { mACJointOrients[0].setObject(mAC.object()); }
		else if(nKType == 10) { mACJointOrients[1].setObject(mAC.object()); }
		else if(nKType == 11) { mACJointOrients[2].setObject(mAC.object()); }

		else continue;
	}

	for(i = 0; i < 3; i++)
	{
		int nKs[3];
		nKs[0] = mACs[i][0].numKeys();
		nKs[1] = mACs[i][1].numKeys();
		nKs[2] = mACs[i][2].numKeys();
		if(nKs[0] > 0 || nKs[1] > 0 || nKs[2] > 0)
		{
			if(nKs[0] <= 0 || nKs[1] <= 0 || nKs[2] <= 0)
			{
				wsprintf(szInfo, "Transform %s : All Animation Key must have at least over 1 Key value.", mTransform.name().asChar());
				MessageBox(::GetActiveWindow(), szInfo, "Invalid Animation Key", MB_OK);
			}
			else
			{
//				if(i == 0) this->ProcessAnimKey(mACs[i], &pTransform->m_KeyPos, true, 0.01f, false); // Translation Animation Key 를 처리한다..
				if(i == 0) this->ProcessAnimKey(mACs[i], &pTransform->m_KeyPos, true, 0.01f, false); // Translation Animation Key 를 처리한다..
				else if(i == 1)
				{
					if(nTranslateCount == 2) // View Translate ... 요건 카메라일대 타겟처럼 처리한다.
					{
						this->ProcessAnimKey(mACs[i], &pTransform->m_KeyRot, true, 0.01f, false); // ViewTarget Position Key 를 처리한다..
					}
					else
					{
						MTransformationMatrix::RotationOrder rotOrder = mTransform.rotationOrder(); // 회전 순서에 따라 처리가 달라져야 한다..
						this->ProcessAnimKey(mACs[i], &pTransform->m_KeyRot, true, 1.0f, true, rotOrder); // Rotation Animation Key 를 쿼터니언으로 처리한다.. 

//						if(pTransform->Type() & OBJ_JOINT) // 조인트인 경우...
//						{
						// -180 도에서 180 도 사이에 있게 만든다..
//						int nKC = pTransform->m_KeyRot.Count();
//						for(int i = 0; i < nKC; i++)
//						{
//							__Vector3* pV = pTransform->m_KeyRot.DataGet(i);
//							if(pV->x > D3DX_PI) { while(pV->x > D3DX_PI) pV->x -= 3.1415192654f * 2.0f; }
//							if(pV->y > D3DX_PI) { while(pV->y > D3DX_PI) pV->y -= 3.1415192654f * 2.0f; }
//							if(pV->z > D3DX_PI) { while(pV->z > D3DX_PI) pV->z -= 3.1415192654f * 2.0f; }
//
//							if(pV->x < -D3DX_PI) { while(pV->x < -D3DX_PI) pV->x += 3.1415192654f * 2.0f; }
//							if(pV->y < -D3DX_PI) { while(pV->y < -D3DX_PI) pV->y += 3.1415192654f * 2.0f; }
//							if(pV->z < -D3DX_PI) { while(pV->z < -D3DX_PI) pV->z += 3.1415192654f * 2.0f; }
//						}
//						}
					} // end of else
				}
				else if(i == 2)
				{
					this->ProcessAnimKey(mACs[i], &pTransform->m_KeyScale, false, 1.0f, false, MTransformationMatrix::kXYZ); // Scale Animation Key 를 처리한다..
				} // end of if(i == 0)
			} // end of if(nKs[0] <= 0 || nKs[1] <= 0 || nKs[2] <= 0)
		} // end of if(nKs[0] > 0 || nKs[1] > 0 || nKs[2] > 0)
	} // end of for(i = 0; i < 3; i++)

	if(pTransform->Type() & OBJ_JOINT) // Joint 이면... Orient 키 값도 본다..
	{
		CN3Joint* pJoint = (CN3Joint*)pTransform;
		this->ProcessAnimKey(mACJointOrients, &(pJoint->m_KeyOrient), true, 1.0f, true, MTransformationMatrix::kXYZ);
	}
	// 에니메이션..
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

void CN3E2Wrapper::ProcessAnimKey(	MFnAnimCurve* pmACs,
									CN3AnimKey *pKey,
									bool bReverseZ,
									float fScale,
									bool bQuaternion,
									MTransformationMatrix::RotationOrder mRotOrder)
{
	// 가장 긴것을 찾는다..
	double dfFrmMax = 0.0f, dfFrmTmp = 0.0f;
	int nKC = 0;
	for(int i = 0; i < 3; i++)
	{
		nKC = pmACs[i].numKeys();
		dfFrmTmp = pmACs[i].time(nKC - 1).value();
		if(dfFrmTmp > dfFrmMax) dfFrmMax = dfFrmTmp;
	}

	if(dfFrmMax <= 1) return;
	dfFrmMax = m_pScene->m_fFrmEnd - m_pScene->m_fFrmStart; // 전체 Scene 을 대상으로 한다.

	int nFrmMax = (int)(dfFrmMax * m_Option.fSamplingRate / 30.0f); // 30 Frame Per Sec 로 Sampling
	
	if(bQuaternion) pKey->Alloc(nFrmMax, m_Option.fSamplingRate, KEY_QUATERNION); // 로테이션이면 쿼터니언으로 할당.
	else pKey->Alloc(nFrmMax, m_Option.fSamplingRate, KEY_VECTOR3); // 벡터로 할당..

	MTime mTime; mTime.setUnit(MTime::kNTSCFrame);
	if(bQuaternion)
	{
		double dRs[4];
		__Quaternion* pQt;
//		MMatrix mMtx;
//		MTransformationMatrix mTMtx;
		__Matrix44 mtxRotFinal, mtxRots[3];
		int nRotSeqs[3] = {1,0,2}; // 기본 YXZ 회전..

		// 회전순서
		if(MTransformationMatrix::kXYZ == mRotOrder) { nRotSeqs[0] = 0; nRotSeqs[1] = 1; nRotSeqs[2] = 2; }
		else if(MTransformationMatrix::kYZX == mRotOrder) { nRotSeqs[0] = 1; nRotSeqs[1] = 2; nRotSeqs[2] = 0; }
		else if(MTransformationMatrix::kZXY == mRotOrder) { nRotSeqs[0] = 2; nRotSeqs[1] = 0; nRotSeqs[2] = 1; }
		else if(MTransformationMatrix::kXZY == mRotOrder) { nRotSeqs[0] = 0; nRotSeqs[1] = 2; nRotSeqs[2] = 1; }
		else if(MTransformationMatrix::kYXZ == mRotOrder) { nRotSeqs[0] = 1; nRotSeqs[1] = 0; nRotSeqs[2] = 2; }
		else if(MTransformationMatrix::kZYX == mRotOrder) { nRotSeqs[0] = 2; nRotSeqs[1] = 1; nRotSeqs[2] = 0; }
		else MessageBox(::GetActiveWindow(), "Not supported rotation order", "Warning - animation key", MB_OK);

		for(i = 0; i < nFrmMax; i++)
		{
			pQt = (__Quaternion*)(pKey->DataGet(i));

			double dfFrm = i * 30.0f / m_Option.fSamplingRate; // 60 Frame Per Sec 로 Sampling
			mTime.setValue(dfFrm);

			memset(dRs, 0, sizeof(double) * 4);

			pmACs[0].evaluate(mTime, dRs[0]);
			pmACs[1].evaluate(mTime, dRs[1]);
			pmACs[2].evaluate(mTime, dRs[2]);

			// -180 ~ 180 도 사이로 맞춘다..
			if(dRs[0] < -D3DX_PI)
				dRs[0] -= __PI2 * (int)(dRs[0]/__PI2 - 1);
			if(dRs[0] > D3DX_PI)
				dRs[0] -= __PI2 * (int)(dRs[0]/__PI2 + 1);
			
			if(dRs[1] < -D3DX_PI)
				dRs[1] -= __PI2 * (int)(dRs[1]/__PI2 - 1);
			if(dRs[1] > D3DX_PI)
				dRs[1] -= __PI2 * (int)(dRs[1]/__PI2 + 1);
			
			if(dRs[2] < -D3DX_PI)
				dRs[2] -= __PI2 * (int)(dRs[2]/__PI2 - 1);
			if(dRs[2] > D3DX_PI)
				dRs[2] -= __PI2 * (int)(dRs[2]/__PI2 + 1);

			mtxRots[0].RotationX(-dRs[0]);
			mtxRots[1].RotationY(-dRs[1]);
			mtxRots[2].RotationZ(dRs[2]);

//			mMtx.setToIdentity();
//			mTMtx = mMtx;
//			mTMtx.setRotation(dRs, MTransformationMatrix::kYXZ, MSpace::kTransform);
//			mTMtx.getRotationQuaternion(dRs[0], dRs[1], dRs[2], dRs[3], MSpace::kTransform);
//			pQt->x = (float)dRs[0];
//			pQt->y = (float)dRs[1];
//			pQt->z = (float)dRs[2];
//			pQt->w = -(float)dRs[3];

			mtxRotFinal.Identity();
			mtxRotFinal *= mtxRots[nRotSeqs[0]];
			mtxRotFinal *= mtxRots[nRotSeqs[1]];
			mtxRotFinal *= mtxRots[nRotSeqs[2]];

			*pQt = mtxRotFinal;
		}
	}
	else
	{
		double dfValue;
		__Vector3* pV = NULL;
		for(i = 0; i < nFrmMax; i++)
		{
			pV = (__Vector3*)(pKey->DataGet(i));

			double dfFrm = i * 30.0f / m_Option.fSamplingRate;
			mTime.setValue(dfFrm);

			pmACs[0].evaluate(mTime, dfValue); pV->x = (float)dfValue;
			pmACs[1].evaluate(mTime, dfValue); pV->y = (float)dfValue;
			pmACs[2].evaluate(mTime, dfValue); pV->z = (float)dfValue;

			if(1.0f != fScale) *pV *= fScale; // 단위가 meter 라서 그렇다..
			if(TRUE == bReverseZ) pV->z = -(pV->z);// Z 축을 음수로 한다..
		}
	}
}

bool CN3E2Wrapper::ProcessChr(MFnSkinCluster &mSkin)
{
	/////////////////////////////////////////////////////////////////////////////////////
	// 영향을 주는 관절을 찾아서 처리한다..
	MStatus stat;
	MDagPathArray PathArray;
	mSkin.influenceObjects(PathArray, &stat);
	int nJC = PathArray.length(); // 관절 갯수를 기억해 둔다.
	MFnIkJoint mJointRoot;
	for(int i = 0; i < nJC; i++)
	{
		mJointRoot.setObject(PathArray[i].node());
		if(mJointRoot.parent(0).apiType() != MFn::kJoint) // 루트 관절일 경우에 처리..
			break;
	}
	if(i == nJC) return false; // Root Joint 가 없을 경우..

	// 중복되는지 찾아본다..
	std::string szJointFN;
	if(mJointRoot.parentCount() > 0 && mJointRoot.parent(0).hasFn(MFn::kTransform))
	{
		MFnTransform mP1(mJointRoot.parent(0));
		szJointFN = ""; this->ProcessName(mP1.parent(0), szJointFN); // 뼈의 이름을 알아보고..
		szJointFN = "Chr\\" + szJointFN + ".N3Joint";
	}
	else
		szJointFN = mJointRoot.name().asChar(); // 뼈의 이름을 알아보고..

	static std::string szJointFNs[512]; // 중복되는지 체크...
	if(m_pScene->s_MngJoint.Count() <= 0) 
	{
		for(i = 0; i < 512; i++) szJointFNs[i] = "";
	}

	bool bOverlapped = false;
	for(i = 0; i < 256; i++)
	{
		if(szJointFNs[i].size() <= 0) break;
		if(szJointFN == szJointFNs[i])
		{
			bOverlapped = true;
			break;
		}
	}
	
	CN3Chr* pChr = NULL;
	CN3Joint* pJoint = NULL;

	if(false == bOverlapped) // 중복되지 않는다면..새로 만든다..
	{
		szJointFNs[i] = szJointFN; // 조인트 이름을 기록하고..

		pChr = new CN3Chr();
		m_pScene->ChrAdd(pChr); // Scene 에 캐릭터 추가..
		pChr->PartAlloc(64); // 충분하게 Part Data 할당..

		if(mJointRoot.parentCount() > 0)
		{
			if(mJointRoot.parent(0).hasFn(MFn::kTransform))
			{
				MFnTransform mP1(mJointRoot.parent(0));
				if(mP1.parentCount() > 0) this->ProcessName(mP1.parent(0), pChr->m_szName); // 이름 짓기....
				else this->ProcessName(mP1.parent(0), pChr->m_szName); // 이름 짓기....
				pChr->FileNameSet("Chr\\" + pChr->m_szName + ".N3Chr"); // 파일 이름 결정..
			}
			else
			{
				this->ProcessName(mJointRoot.parent(0), pChr->m_szName); // 이름 짓기....
				pChr->FileNameSet("Chr\\" + pChr->m_szName + ".N3Chr"); // 파일 이름 결정..);
			}
		}
		else 
		{
			pChr->m_szName = mJointRoot.name().asChar();
			std::string szFN = "Chr\\"; szFN += mJointRoot.name().asChar(); szFN += ".N3Chr"; // 파일 이름 결정..
			pChr->FileNameSet(szFN);
		}

		pJoint = this->ProcessJoint(mJointRoot); // Joint 처리
		if(NULL == pJoint) 
		{
			delete pChr;
			return NULL;
		}
		pJoint->FileNameSet(szJointFN); // 파일 이름 설정..
		
		pChr->s_MngJoint.Add(pJoint);
		pChr->JointSet(pJoint->FileName()); // Joint Setting;
		
		///////////////////////////////////////////////////////////////////////
		// Joint 가 그룹 되어 있다면 Parent Transform 처리..
		if(mJointRoot.parentCount() > 0 && mJointRoot.parent(0).hasFn(MFn::kTransform))
		{
			MFnTransform mTJ(mJointRoot.parent(0)); // Joint Transform Node
			
			MTransformationMatrix mTMJ = mTJ.transformation();
//			MMatrix mMtxJT; mMtxJT.setToIdentity();
//			this->GetWorldTransform(mTJ, mMtxJT);
//			MTransformationMatrix mTMJ(mMtxJT);

			MVector mVPos;
			double dfScale[3], dfRot[3];
			MTransformationMatrix::RotationOrder RotOrder = MTransformationMatrix::kXYZ;

			mVPos = mTMJ.translation(MSpace::kTransform) * 0.01;
			
			MMatrix mMtxJWorld; mMtxJWorld.setToIdentity();
			this->GetWorldTransform(mTJ, mMtxJWorld);
			mVPos.x = mMtxJWorld.matrix[3][0] * 0.01;
			mVPos.y = mMtxJWorld.matrix[3][1] * 0.01;
			mVPos.z = mMtxJWorld.matrix[3][2] * 0.01;
			
			MTransformationMatrix mTMtxJWorld(mMtxJWorld);
			mTMtxJWorld.getScale(dfScale, MSpace::kTransform);
			mTMtxJWorld.getRotation(dfRot, RotOrder, MSpace::kTransform);

			__Vector3 vTrans((float)mVPos.x, (float)mVPos.y, -(float)mVPos.z);
			__Vector3 vScale((float)dfScale[0], (float)dfScale[1], (float)dfScale[2]);
			__Vector3 vRot((float)-dfRot[0], (float)-dfRot[1], (float)dfRot[2]);
			
			__Matrix44 mtxRotG; mtxRotG.Rotation(vRot);
			__Quaternion qRot = mtxRotG;
			vTrans *= mtxRotG;
			this->ProcessJointTransform(pJoint, &vTrans, &qRot, &vScale, true);

			pJoint->PosSet(pJoint->Pos() * mtxRotG); // 위치 값도 방향과 스케일에 맞게 
			int nK = pJoint->m_KeyPos.Count(); // 에니메이션 키도 변경해준다..
			for(i = 0; i < nK; i++)
			{
				__Vector3* pvKey = (__Vector3*)(pJoint->m_KeyPos.DataGet(i));
				(*pvKey) *= mtxRotG;
			}
		}
	}
	else
	{
		pJoint = m_pScene->s_MngJoint.Get(szJointFN);

		int nChrCount = m_pScene->ChrCount(); // 같은 조인트 포인터를 갖는 캐릭터 포인터를 찾는다..
		for(int i = 0; i < nChrCount; i++)
		{
			CN3Chr* pChrTmp = m_pScene->ChrGet(i);
			if(pChrTmp->Joint() == pJoint)
			{
				pChr = pChrTmp;
				break;
			}
		}
	}

	if(NULL == pChr) return false;

	/////////////////////////////////////////////////// ............ // Mesh
	// 원래 Mesh
	MObjectArray mMeshArray;
	mSkin.getInputGeometry(mMeshArray);
	__ASSERT(1 == mMeshArray.length(), "binding mesh count is not 1");
	MFnMesh mMeshOrg(mMeshArray[0]); // Original Mesh

	mSkin.getOutputGeometry(mMeshArray); 
	__ASSERT(1 == mMeshArray.length(), "Output mesh count is not 1");
	MFnMesh mMeshOutput(mMeshArray[0]); // Output Mesh

	bool bCollisionMesh = false;

	MFnTransform mMT(mMeshOrg.parent(0)); // Mesh Transform
	MFnTransform mGT;// Mesh GroupTransform
	if(mMT.parentCount() > 0 && mMT.parent(0).hasFn(MFn::kTransform))
		mGT.setObject(mMT.parent(0));
	else
		mGT.setObject(mMT.object());

	// 충돌 체크용으로 쓰일 메시인지 살펴본다..
	std::string szFNM = mMT.name().asChar();
	if(szFNM.size() > 0) CharLower(&(szFNM[0]));
	if(szFNM.find("coll") != -1) bCollisionMesh = true;

	if(true == bCollisionMesh) // 충돌 체크 메시면.... 
	{
/*		CN3IMesh* pN3Mesh = this->ProcessIMesh(mMeshOrg); // Indexed Mesh 처리.
		int nVC = pN3Mesh->VertexCount();
		int nFC = pN3Mesh->FaceCount();
		__VertexXyzNormal* pVSrc = pN3Mesh->Vertices();
		WORD* pwISrc = pN3Mesh->VertexInices();

		
		CN3VMesh* pVMesh = new CN3VMesh();
		pVMesh->CreateVertices(nVC);
		pVMesh->CreateIndex(nFC * 3);
		__Vector3* pVDest = pVMesh->Vertices();
		WORD* pwIDest = pVMesh->Indices();

		memcpy(pwIDest, pwISrc, nFC * 3 * 2);
		for(int i = 0; i < nVC; i++) pVDest[i] = pVSrc[i].v;

		delete pN3Mesh;

		pVMesh->m_szName = "";
		this->ProcessName(mMeshOrg.object(), pVMesh);
		std::string szFN = "Chr\\" + pVMesh->m_szName + ".N3VMesh"; // 파일 이름 결정..
		pVMesh->FileNameSet(szFN);

		pChr->s_MngVMesh.Add(pVMesh);
		pChr->CollisionMeshSet(pVMesh->m_szName);

		this->ProcessSkin(mSkin, pChr->CollisionSkin()); // Skin 처리.
*/
	}
	else if(false == bCollisionMesh) // 충돌 체크 메시 아니면....정상적으로 진행..
	{
		int nLOD = 0;
		int nPart = 0;

		if(mGT.parentCount() > 0 && mGT.parent(0).hasFn(MFn::kTransform)) // 그룹 트랜스폼의 상위 노드가 있을 경우..
		{
			MFnTransform mCT(mGT.parent(0)); // Character Transform
			int nC = mCT.childCount();
			for(i = 0; i < nC; i++)
			{
				if(!mCT.child(i).hasFn(MFn::kTransform)) continue;

				MFnTransform mTTmp(mCT.child(i));
				if(mTTmp.object() == mGT.object()) break; // 캐릭터 트랜스폼 밑의 차일드의 이름과 그룹의 이름이 같을 경우..
				nPart++; // 같지 않으면 Part 증가..
			}

			int nGTC = mGT.childCount();
			for(i = 0; i < nGTC; i++)
			{
				if(!mGT.child(i).hasFn(MFn::kTransform)) continue; 

				MFnTransform mTTmp(mGT.child(i));
				if(mTTmp.name() == mMT.name()) break; // Mesh Transform 과 Mesh 그룹의 차일드 메시가 이름이 같으면..
				nLOD++;
			}
		}
		else // pair of if(mGT.parentCount() > 0 && mGT.parent(0).apiType() == MFn::kTransform) // 그룹 트랜스폼의 상위 노드가 있을 경우..
		{
			int nC = mGT.childCount();
			for(i = 0; i < nC; i++)
			{
				if(mGT.child(i).apiType() != MFn::kTransform) continue;

				MFnTransform mTTmp(mGT.child(i));
				if(mTTmp.name() == mMT.name()) break; // 캐릭터 트랜스폼 밑의 차일드의 이름과 그룹의 이름이 같을 경우..
				nPart++; // 같지 않으면 Part 증가..
			}
		}
		
		__ASSERT(nPart >= 0 && nPart < 64, "Part count is bigger than 64");
		__ASSERT(nLOD >= 0 && nLOD < MAX_CHR_LOD, "LOD Count is bigger than MAX_CHR_LOD");
		
		CN3CPart* pPart = pChr->Part(nPart);
		CN3Skin* pSkin = pPart->Skin(nLOD);

		if(pPart->m_szName.empty()) // 파트의 이름이 없으면..
		{
			pPart->m_szName = mGT.name().asChar();
			std::string szFN; this->ProcessName(mGT.object(), szFN);

			int iLen = szFN.size();
			int iLen2 = pPart->m_szName.size();
			if(m_Option.bGenerateFileName && iLen >= 11 && iLen2 >= 11) // Item Code ::: 0_2345_78_0
				szFN = szFN.substr(iLen - 11);
			pPart->FileNameSet("item\\" + szFN + ".N3CPart");
		}
		
		if(NULL == pSkin) // Skin 이 없으면 넣는다..
		{
			CN3CPartSkins* pSkinsAdd = new CN3CPartSkins();
			pSkinsAdd->m_szName = mGT.name().asChar(); // 이름과 파일 이름을 정하고..
			std::string szFN; this->ProcessName(mGT.object(), szFN);

			int iLen = szFN.size();
			int iLen2 = pSkinsAdd->m_szName.size();
			if(m_Option.bGenerateFileName && iLen >= 11 && iLen2 >= 11) // Item Code ::: 0_2345_78_0
				szFN = szFN.substr(iLen - 11);
			pSkinsAdd->FileNameSet("item\\" + szFN + ".N3CSkins");

			CN3Base::s_MngSkins.Add(pSkinsAdd);
			pPart->SkinsSet(pSkinsAdd->FileName());
			pSkin = pPart->Skin(nLOD); // 다시 포인터 구하기.
		} 

		if(false == this->ProcessSkin(mSkin, pSkin)) // Skin 처리..
		{
			MessageBox(::GetActiveWindow(), mSkin.name().asChar(), "Skin processing failed", MB_OK);
			return false;
		}

		// 텍스처 처리..
		pPart->m_Mtl.Init();
		pPart->m_Mtl.dwColorArg1 = D3DTA_DIFFUSE;
		pPart->m_Mtl.dwColorArg2 = D3DTA_TEXTURE;

		CN3Texture* pTex = ProcessTexture(mMeshOutput);
		if(pTex)
		{
			pTex->m_szName = mGT.name().asChar(); // 이름과 파일 이름을 정하고..
			std::string szFN; this->ProcessName(mGT.object(), szFN);
			int iLen = szFN.size();
			int iLen2 = pTex->m_szName.size();
			if(m_Option.bGenerateFileName && iLen >= 11 && iLen2 >= 11) // Item Code ::: 0_2345_78_0
				szFN = szFN.substr(iLen - 11);
			pTex->FileNameSet("Item\\" + szFN + ".DXT"); // Part 이름과 Texture 이름을 같게 한다.
			
			pPart->m_Mtl.dwColorOp = D3DTOP_MODULATE;
			pPart->TexSet(pTex);

//			D3DFORMAT fmtTex = pTex->PixelFormat();
//			if(	fmtTex == D3DFMT_DXT2 ||
//				fmtTex == D3DFMT_DXT3 || 
//				fmtTex == D3DFMT_DXT4 || 
//				fmtTex == D3DFMT_DXT5 ) pPart->m_Mtl.nRenderFlags = RF_ALPHABLENDING;
		}
		else // 텍스처가 없는 경우에만 재질
		{
//			this->ProcessMaterial(this->MeshGetShader(mMeshOutput), &(pPart->m_Mtl)); 
			pPart->m_Mtl.dwColorOp = D3DTOP_SELECTARG1;
		}
	} // if(false == bCollisionMesh) // 충돌 체크 메시 아니면....정상적으로 진행..

	return true;
}

void CN3E2Wrapper::ProcessName(MObject mObj, std::string& szName)
{
	if(mObj.hasFn(MFn::kWorld) == FALSE && mObj.hasFn(MFn::kDependencyNode)) // 최상위 노드가 아니면..
	{
		MFnDependencyNode mNode(mObj);

		std::string szNameBack = szName;
		szName = mNode.name().asChar();
		if(szNameBack.size() > 0)
		{
			szName += '_';
			szName += szNameBack;
		}

		if(mObj.hasFn(MFn::kDagNode))
		{
			MFnDagNode mDAG(mObj);
			if(mDAG.parentCount() > 0) this->ProcessName(mDAG.parent(0), szName);
		}
	}
}
/*
const DWORD OBJ_UNKNOWN				= 0;
const DWORD OBJ_BASE				= 0x1;
const DWORD OBJ_BASE_HIERARCHY 		= 0x2;
const DWORD OBJ_TRANSFORM 			= 0x4;
const DWORD OBJ_SCENE				= 0x8;
const DWORD OBJ_TEXTURE				= 0x10;

const DWORD OBJ_CAMERA				= 0x20;
const DWORD OBJ_LIGHT				= 0x40;
const DWORD OBJ_SHAPE				= 0x80;
const DWORD OBJ_CHARACTER			= 0x100;

const DWORD OBJ_MESH				= 0x200;
const DWORD OBJ_MESH_PROGRESSIVE	= 0x400;
const DWORD OBJ_MESH_INDEXED		= 0x800;
const DWORD OBJ_MESH_VECTOR3		= 0x1000;
const DWORD OBJ_JOINT				= 0x2000;
const DWORD OBJ_SKIN				= 0x4000;

const DWORD OBJ_DUMMY				= 0x8000;
const DWORD OBJ_EFFECT				= 0x10000;

const DWORD OBJ_ANIM_CONTROL		= 0x20000;
const DWORD OBJ_SUBSCENE			= 0x40000;
*/


void CN3E2Wrapper::GetWorldTransform(MFnTransform &mTransform, MMatrix &mMtx)
{
	int nParent = mTransform.parentCount();
	mMtx *= mTransform.transformationMatrix();
	if(1 == nParent && mTransform.parent(0).hasFn(MFn::kTransform))
	{
		MFnTransform mPT(mTransform.parent(0));
		this->GetWorldTransform(mPT, mMtx);
	}
}

bool CN3E2Wrapper::IsSelected(MSelectionList& mSelList, MObject mObj)
{
	if(mObj.hasFn(MFn::kDependencyNode))
	{
		MFnDependencyNode mDag(mObj);
		const char* szName = mDag.name().asChar();
		int ttttt = 0;
	}

	bool bSel = false;
	int nSelCount = mSelList.length();
	MObject mObjTmp;
	for(int i = 0; i < nSelCount; i++)
	{
		mSelList.getDependNode(i, mObjTmp);
		if(mObjTmp == mObj)
		{
			return true;
		}
	}

	if(mObj.hasFn(MFn::kDagNode))
	{
		MFnDagNode mDNode(mObj);
		if(mDNode.parentCount() > 0)
		{
			return IsSelected(mSelList, mDNode.parent(0));
		}
	}
	else if(mObj.hasFn(MFn::kSkinClusterFilter))
	{
		MFnSkinCluster mSkin(mObj);
		MObjectArray mMeshArray;
		mSkin.getInputGeometry(mMeshArray);
		__ASSERT(1 == mMeshArray.length(), "binding mesh count is not 1");
		return IsSelected(mSelList, mMeshArray[0]); // Original Mesh
	}

	return false;
}


INT_PTR CALLBACK CN3E2Wrapper::DlgProcProgress( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) 
    {
        case WM_INITDIALOG:
        {
			break;
        }
        case WM_COMMAND:
        {
			switch( LOWORD(wParam) )
			{
			case IDC_B_CANCEL:
				m_bCancelExport = TRUE;
				break;
			}
			break;
		}
		case WM_CLOSE:
		{
			ShowWindow(hDlg, SW_HIDE);
			break;
		}
    }
    return FALSE; // Didn't handle message
}

BOOL CALLBACK CN3E2Wrapper::DlgProcPane(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			// Registry 에 옵션값을 넣어두었다..
			HKEY hKey = g_pEng->RegistryOpen("N3Export Option");
			if(hKey) 
			{
				g_pEng->RegistryValueGet(hKey, "Export Camera", &m_Option.bExportCamera, 4);
				g_pEng->RegistryValueGet(hKey, "Export Light", &m_Option.bExportLight, 4);
				g_pEng->RegistryValueGet(hKey, "Export Geometry", &m_Option.bExportGeometry, 4);
				g_pEng->RegistryValueGet(hKey, "Export Dummy", &m_Option.bExportDummy, 4);
				g_pEng->RegistryValueGet(hKey, "Export Character", &m_Option.bExportCharacter, 4);

				g_pEng->RegistryValueGet(hKey, "Animation Key", &m_Option.bAnimationKey, 4);
				g_pEng->RegistryValueGet(hKey, "Generate File Name", &m_Option.bGenerateFileName, 4);
				g_pEng->RegistryValueGet(hKey, "Generate Smooth Normal", &m_Option.bGenerateSmoothNormal, 4);
//				g_pEng->RegistryValueGet(hKey, "Generate Progressive Mesh", &m_Option.bGenerateProgressiveMesh, 4);
				g_pEng->RegistryValueGet(hKey, "Generate Half Size Texture", &m_Option.bGenerateHalfSizeTexture, 4);
				g_pEng->RegistryValueGet(hKey, "Generate Compressed Texture", &m_Option.bGenerateCompressedTexture, 4);
				
				g_pEng->RegistryClose(hKey);
			}

			CheckDlgButton(hDlg, IDC_C_OBJ_CAMERA, m_Option.bExportCamera);
			CheckDlgButton(hDlg, IDC_C_OBJ_LIGHT, m_Option.bExportLight);
			CheckDlgButton(hDlg, IDC_C_OBJ_GEOMETRY, m_Option.bExportGeometry);
			CheckDlgButton(hDlg, IDC_C_OBJ_DUMMY, m_Option.bExportDummy);
			CheckDlgButton(hDlg, IDC_C_OBJ_CHARACTER, m_Option.bExportCharacter);

			CheckDlgButton(hDlg, IDC_C_ANIMATION_KEY, m_Option.bAnimationKey);

			CheckDlgButton(hDlg, IDC_C_GENERATE_FILE_NAME, m_Option.bGenerateFileName);
			CheckDlgButton(hDlg, IDC_C_GENERATE_SMOOTH_NORMAL, m_Option.bGenerateSmoothNormal);
//			CheckDlgButton(hDlg, IDC_C_GENERATE_PROGRESSIVE_MESH, m_Option.bGenerateProgressiveMesh);
			CheckDlgButton(hDlg, IDC_C_GENERATE_HALF_SIZE_TEXTURE, m_Option.bGenerateHalfSizeTexture);
			CheckDlgButton(hDlg, IDC_C_GENERATE_COMPRESSED_TEXTURE, m_Option.bGenerateCompressedTexture);

			m_Option.nFrmStart = (int)(MAnimControl::minTime().value()); // 시작 프레임.
			m_Option.nFrmEnd = (int)(MAnimControl::maxTime().value() +0.5); // 끝 프레임

			SetDlgItemInt(hDlg, IDC_E_FRAME_START, m_Option.nFrmStart, FALSE);
			SetDlgItemInt(hDlg, IDC_E_FRAME_END, m_Option.nFrmEnd, FALSE);
			SetDlgItemInt(hDlg, IDC_E_SAMPLING_RATE, (int)(m_Option.fSamplingRate), FALSE);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			{
				memset(&m_Option, 0, sizeof(m_Option));

				m_Option.bExportCamera = IsDlgButtonChecked(hDlg, IDC_C_OBJ_CAMERA);
				m_Option.bExportLight = IsDlgButtonChecked(hDlg, IDC_C_OBJ_LIGHT);
				m_Option.bExportGeometry = IsDlgButtonChecked(hDlg, IDC_C_OBJ_GEOMETRY);
				m_Option.bExportDummy = IsDlgButtonChecked(hDlg, IDC_C_OBJ_DUMMY);
				m_Option.bExportCharacter = IsDlgButtonChecked(hDlg, IDC_C_OBJ_CHARACTER);

				m_Option.bAnimationKey = IsDlgButtonChecked(hDlg, IDC_C_ANIMATION_KEY);
				m_Option.nFrmStart = GetDlgItemInt(hDlg, IDC_E_FRAME_START, NULL, FALSE); // 시작 프레임
				m_Option.nFrmEnd = GetDlgItemInt(hDlg, IDC_E_FRAME_END, NULL, FALSE); // 끝 프레임
				m_Option.fSamplingRate = (float)GetDlgItemInt(hDlg, IDC_E_SAMPLING_RATE, NULL, FALSE); // Sampling Rate

				m_Option.bGenerateFileName = IsDlgButtonChecked(hDlg, IDC_C_GENERATE_FILE_NAME);
				m_Option.bGenerateSmoothNormal = IsDlgButtonChecked(hDlg, IDC_C_GENERATE_SMOOTH_NORMAL);
//				m_Option.bGenerateProgressiveMesh = IsDlgButtonChecked(hDlg, IDC_C_GENERATE_PROGRESSIVE_MESH);
				m_Option.bGenerateHalfSizeTexture = IsDlgButtonChecked(hDlg, IDC_C_GENERATE_HALF_SIZE_TEXTURE);
				m_Option.bGenerateCompressedTexture = IsDlgButtonChecked(hDlg, IDC_C_GENERATE_COMPRESSED_TEXTURE);

				EndDialog(hDlg, 1);

				// Registry 에 옵션값을 넣어두었다..
				HKEY hKey = g_pEng->RegistryOpen("N3Export Option");
				if(NULL == hKey) RegCreateKey(HKEY_CURRENT_USER, "N3Export Option", &hKey);
				if(hKey) 
				{
					g_pEng->RegistryValueSet(hKey, "Export Camera", &m_Option.bExportCamera, 4);
					g_pEng->RegistryValueSet(hKey, "Export Light", &m_Option.bExportLight, 4);
					g_pEng->RegistryValueSet(hKey, "Export Geometry", &m_Option.bExportGeometry, 4);
					g_pEng->RegistryValueSet(hKey, "Export Dummy", &m_Option.bExportDummy, 4);
					g_pEng->RegistryValueSet(hKey, "Export Character", &m_Option.bExportCharacter, 4);

					g_pEng->RegistryValueSet(hKey, "Animation Key", &m_Option.bAnimationKey, 4);
					g_pEng->RegistryValueSet(hKey, "Generate File Name", &m_Option.bGenerateFileName, 4);
					g_pEng->RegistryValueSet(hKey, "Generate Smooth Normal", &m_Option.bGenerateSmoothNormal, 4);
//					g_pEng->RegistryValueSet(hKey, "Generate Progressive Mesh", &m_Option.bGenerateProgressiveMesh, 4);
					g_pEng->RegistryValueSet(hKey, "Generate Half Size Texture", &m_Option.bGenerateHalfSizeTexture, 4);
					g_pEng->RegistryValueSet(hKey, "Generate Compressed Texture", &m_Option.bGenerateCompressedTexture, 4);
					
					g_pEng->RegistryClose(hKey);
				}
			}
			return TRUE;
		case IDCANCEL:
			{
				EndDialog(hDlg, 0);
				return TRUE;
			}
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	
	return FALSE;
}
