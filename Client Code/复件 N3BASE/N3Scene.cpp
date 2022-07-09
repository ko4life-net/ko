// N3Scene.cpp: implementation of the CN3Scene class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Scene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Scene::CN3Scene()
{
	m_dwType |= OBJ_SCENE;

	memset(m_pCameras, 0, sizeof(m_pCameras));
	memset(m_pLights, 0, sizeof(m_pLights));
	memset(m_pShapes, 0, sizeof(m_pShapes));
	memset(m_pChrs, 0, sizeof(m_pChrs));

	this->Release();
}

CN3Scene::~CN3Scene()
{
	CN3Scene::Release();
}

void CN3Scene::Release()
{
	m_nCameraActive = 0;

	m_fFrmCur = 0.0f; // Animation Frame;
	m_fFrmStart = 0.0f; // 전체 프레임.
	m_fFrmEnd = 1000.0f; // 기본값 프레임.

	int i = 0;

	for(i = 0; i < MAX_SCENE_CAMERA; i++) { if(m_pCameras[i]) { delete m_pCameras[i]; m_pCameras[i] = NULL; } }
	for(i = 0; i < MAX_SCENE_LIGHT; i++) { if(m_pLights[i]) { delete m_pLights[i]; m_pLights[i] = NULL; } }
	for(i = 0; i < MAX_SCENE_SHAPE; i++) { if(m_pShapes[i]) { delete m_pShapes[i]; m_pShapes[i] = NULL; } }
	for(i = 0; i < MAX_SCENE_CHARACTER; i++) { if(m_pChrs[i]) { delete m_pChrs[i]; m_pChrs[i] = NULL; } }

	m_nCameraCount = 0; 
	m_nLightCount = 0;
	m_nShapeCount = 0;
	m_nChrCount = 0;

	CN3Base::ReleaseResrc(); // 모든 리소스 해제..
}

bool CN3Scene::Load(HANDLE hFile)
{
	DWORD dwRWC = 0;
	
	ReadFile(hFile, &m_nCameraActive, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fFrmCur, 4, &dwRWC, NULL); // Animation Frame;
	ReadFile(hFile, &m_fFrmStart, 4, &dwRWC, NULL); // 전체 프레임.
	ReadFile(hFile, &m_fFrmEnd, 4, &dwRWC, NULL); // 전체 프레임.

	int i = 0, nL = 0;
	char szName[512] = "";

	int nCC = 0;
	ReadFile(hFile, &nCC, 4, &dwRWC, NULL); // 카메라..
	for(i = 0; i < nCC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Camera* pCamera = new CN3Camera();
		if(false == pCamera->LoadFromFile(szName))
		{
			delete pCamera;
			continue;
		}

		this->CameraAdd(pCamera);
	}

	int nLC = 0;
	ReadFile(hFile, &nLC, 4, &dwRWC, NULL); // 카메라..
	for(i = 0; i < nLC; i++) 
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Light* pLight = new CN3Light();
		if(false == pLight->LoadFromFile(szName))
		{
			delete pLight;
			continue;
		}
		
		this->LightAdd(pLight);
	}

	int nSC = 0;
	ReadFile(hFile, &nSC, 4, &dwRWC, NULL); // Shapes..
	for(i = 0; i < nSC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Shape* pShape = new CN3Shape();
		if(false == pShape->LoadFromFile(szName))
		{
			delete pShape;
			continue;
		}

		this->ShapeAdd(pShape);
	}

	int nChrC = 0;
	ReadFile(hFile, &nChrC, 4, &dwRWC, NULL); // 캐릭터
	for(i = 0; i < nChrC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Chr* pChr = new CN3Chr();
		if(false == pChr->LoadFromFile(szName))
		{
			delete pChr;
			continue;
		}

		this->ChrAdd(pChr);
	}

	if(m_nCameraCount <= 0) this->DefaultCameraAdd();
	if(m_nLightCount <= 0) this->DefaultLightAdd();

	return true;
}

bool CN3Scene::Save(HANDLE hFile)
{
	::CreateDirectory("Texture", NULL);
	::CreateDirectory("Data", NULL);
	::CreateDirectory("Mesh", NULL);
	::CreateDirectory("Chr", NULL);

	DWORD dwRWC = 0;
	
	WriteFile(hFile, &m_nCameraActive, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fFrmCur, 4, &dwRWC, NULL); // Animation Frame;
	WriteFile(hFile, &m_fFrmStart, 4, &dwRWC, NULL); // 전체 프레임.
	WriteFile(hFile, &m_fFrmEnd, 4, &dwRWC, NULL); // 전체 프레임.

	int i = 0, nL = 0;
	
	WriteFile(hFile, &m_nCameraCount, 4, &dwRWC, NULL); // 카메라..
	for(i = 0; i < m_nCameraCount; i++)
	{
		nL = lstrlen(m_pCameras[i]->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		WriteFile(hFile, m_pCameras[i]->FileName(), nL, &dwRWC, NULL);
		m_pCameras[i]->SaveToFile();
	}

	WriteFile(hFile, &m_nLightCount, 4, &dwRWC, NULL); // 카메라..
	for(i = 0; i < m_nLightCount; i++) 
	{
		nL = lstrlen(m_pLights[i]->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		WriteFile(hFile, m_pLights[i]->FileName(), nL, &dwRWC, NULL);
		m_pLights[i]->SaveToFile();
	}

	WriteFile(hFile, &m_nShapeCount, 4, &dwRWC, NULL); // Shapes..
	for(i = 0; i < m_nShapeCount; i++)
	{
		nL = lstrlen(m_pShapes[i]->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, m_pShapes[i]->FileName(), nL, &dwRWC, NULL);
		m_pShapes[i]->SaveToFile();
	}

	WriteFile(hFile, &m_nChrCount, 4, &dwRWC, NULL); // 캐릭터
	for(i = 0; i < m_nChrCount; i++)
	{
		nL = lstrlen(m_pChrs[i]->FileName());
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, m_pChrs[i]->FileName(), nL, &dwRWC, NULL);
		m_pChrs[i]->SaveToFile();
	}

	int nC = 0;
	
	s_MngTex.SaveToFiles();
	s_MngMesh.SaveToFiles();
	s_MngPMesh.SaveToFiles();
	s_MngJoint.SaveToFiles();
	s_MngCPart.SaveToFiles();
	s_MngVMesh.SaveToFiles();

	return true;
}

void CN3Scene::Render()
{
	int i = 0;
	for(i = 0; i < m_nCameraCount; i++)
	{
		__ASSERT(m_pCameras[i], "Camera pointer is NULL");
		if(m_nCameraActive != i) m_pCameras[i]->Render();
	}

	for(i = 0; i < m_nLightCount; i++)
	{
		__ASSERT(m_pLights[i], "Light pointer is NULL");
		m_pLights[i]->Render(NULL, 0.5f);
	}
	
	for(i = 0; i < m_nShapeCount; i++)
	{
		__ASSERT(m_pShapes[i], "Shape pointer is NULL!");
		m_pShapes[i]->Render();
	}

	for(i = 0; i < m_nChrCount; i++)
	{
		__ASSERT(m_pChrs[i], "Character pointer is NULL!");
		m_pChrs[i]->Render(m_fFrmCur);
	}
}

void CN3Scene::Tick(float fFrm)
{
	if(fFrm <  m_fFrmStart || fFrm > m_fFrmEnd)
	{
		m_fFrmCur += 30.0f / CN3Base::s_fFrmPerSec; // 일정하게 움직이도록 시간에 따라 움직이는 양을 조절..
		if(m_fFrmCur > m_fFrmEnd) m_fFrmCur = m_fFrmStart;
	}
	else
	{
		m_fFrmCur = fFrm;
	}

	this->TickCameras(fFrm);
	this->TickLights(fFrm);
	this->TickShapes(fFrm);
	this->TickChrs(fFrm);
}

void CN3Scene::TickCameras(float fFrm)
{
	for(int i = 0; i < m_nCameraCount; i++)
	{
		m_pCameras[i]->Tick(m_fFrmCur);
		if(m_nCameraActive == i) m_pCameras[i]->Apply(); // 카메라 데이터 값을 적용한다..
	}
}

void CN3Scene::TickLights(float fFrm)
{
	for(int i = 0; i < 8; i++) s_lpD3DDev->LightEnable(i, FALSE);
	for(i = 0; i < m_nLightCount; i++)
	{
		__ASSERT(m_pLights[i], "Light pointer is NULL!");
		m_pLights[i]->Tick(m_fFrmCur);

		if(m_pLights[i]->m_Data.nNumber >= 0 && m_pLights[i]->m_Data.nNumber < 8)
		{
			s_lpD3DDev->LightEnable(m_pLights[i]->m_Data.nNumber, m_pLights[i]->m_Data.bOn);
			if(m_pLights[i]->m_Data.bOn)
			{
				// Ambient Light 값을 0번라이트의 값에 맞춘다..
				if(m_pLights[i]->m_Data.nNumber == 0)
				{
					DWORD dwAmbient;
					dwAmbient = 0xff000000 |
								(((DWORD)(m_pLights[i]->m_Data.Diffuse.r * 255 * 0.6f)) << 16) |
								(((DWORD)(m_pLights[i]->m_Data.Diffuse.g * 255 * 0.6f)) << 8) |
								(((DWORD)(m_pLights[i]->m_Data.Diffuse.b * 255 * 0.6f)) << 0) ;
//					s_lpD3DDev->SetRenderState( D3DRS_AMBIENT, dwAmbient ); // Ambient Light 조정..

					// 라이트가 항상 카메라를 따라오게 만든다..
					m_pLights[i]->m_Data.Position = s_CameraData.vEye;
					__Vector3 vDir = s_CameraData.vAt - s_CameraData.vEye;
					vDir.Normalize();
					m_pLights[i]->m_Data.Direction = vDir;
				}

				s_lpD3DDev->SetLight(m_pLights[i]->m_Data.nNumber, &m_pLights[i]->m_Data);
			}
		}
	}
}

void CN3Scene::TickShapes(float fFrm)
{
	for(int i = 0; i < m_nShapeCount; i++)
	{
		__ASSERT(m_pShapes[i], "Shape pointer is NULL");
		m_pShapes[i]->Tick(m_fFrmCur);
	}
}

void CN3Scene::TickChrs(float fFrm)
{
	for(int i = 0; i < m_nChrCount; i++)
	{
		__ASSERT(m_pChrs[i], "Character pointer is NULL");
//		for(int j = 0; j < 1000; j++) 
		m_pChrs[i]->Tick(m_fFrmCur);
	}
}

int CN3Scene::CameraAdd(CN3Camera *pCamera)
{
	if(m_nCameraCount >= MAX_SCENE_CAMERA) return -1;
	if(pCamera == NULL) return -1;

	if(m_pCameras[m_nCameraCount]) delete m_pCameras[m_nCameraCount];
	m_pCameras[m_nCameraCount] = pCamera;

	m_nCameraCount++;
	return m_nCameraCount;
}

void CN3Scene::CameraDelete(int index)
{
	if(index < 0 || index >= m_nCameraCount) return;

	delete m_pCameras[index];
	m_pCameras[index] = NULL;
	
	m_nCameraCount--;
	for(int i = index; i < m_nCameraCount; i++) m_pCameras[i] = m_pCameras[i+1];
	m_pCameras[m_nCameraCount] = NULL;
}

void CN3Scene::CameraDelete(CN3Camera *pCamera)
{
	for(int i = 0; i < m_nCameraCount; i++) 
	{
		if(m_pCameras[i] == pCamera) 
		{
			this->CameraDelete(i);
			break;
		}
	}
}

void CN3Scene::CameraSetActive(int index)
{
	if(index < 0 || index >= m_nCameraCount) return;
	
	m_nCameraActive = index;
}

int CN3Scene::LightAdd(CN3Light *pLight)
{
	if(pLight == NULL) return -1;

	if(m_pLights[m_nLightCount]) delete m_pLights[m_nLightCount];
	m_pLights[m_nLightCount] = pLight;

	m_nLightCount++;
	return m_nLightCount;
}

void CN3Scene::LightDelete(int index)
{
	if(index < 0 || index >= m_nLightCount) return;

	delete m_pLights[index];
	m_pLights[index] = NULL;
	
	m_nLightCount--;
	for(int i = index; i < m_nLightCount; i++) m_pLights[i] = m_pLights[i+1];
	m_pLights[m_nLightCount] = NULL;
}

void CN3Scene::LightDelete(CN3Light *pLight)
{
	for(int i = 0; i < m_nLightCount; i++) 
	{
		if(m_pLights[i] == pLight) 
		{
			this->LightDelete(i);
			break;
		}
	}
}

int CN3Scene::ShapeAdd(CN3Shape *pShape)
{
	if(pShape == NULL) return -1;

	for(int i = 0; i < m_nShapeCount; i++)
	{
		if(pShape == m_pShapes[i]) return -1;
	}

	if(m_pShapes[m_nShapeCount]) delete m_pShapes[m_nShapeCount];
	m_pShapes[m_nShapeCount] = pShape;

	m_nShapeCount++;
	return m_nShapeCount;
}

void CN3Scene::ShapeDelete(int index)
{
	if(index < 0 || index >= m_nShapeCount) return;

	delete m_pShapes[index];
	m_pShapes[index] = NULL;
	
	m_nShapeCount--;
	for(int i = index; i < m_nShapeCount; i++) m_pShapes[i] = m_pShapes[i+1];
	m_pShapes[m_nShapeCount] = NULL;
}

void CN3Scene::ShapeDelete(CN3Shape *pShape)
{
	for(int i = 0; i < m_nShapeCount; i++) 
	{
		if(m_pShapes[i] == pShape) 
		{
			this->ShapeDelete(i);
			break;
		}
	}
}

int CN3Scene::ChrAdd(CN3Chr *pChr)
{
	if(pChr == NULL) return -1;

	for(int i = 0; i < m_nChrCount; i++)
	{
		if(pChr == m_pChrs[i]) return -1;
	}

	if(m_pChrs[m_nChrCount]) delete m_pChrs[m_nChrCount];
	m_pChrs[m_nChrCount] = pChr;

	m_nChrCount++;
	return m_nChrCount;
}

void CN3Scene::ChrDelete(int index)
{
	if(index < 0 || index >= m_nChrCount) return;

	delete m_pChrs[index];
	m_pChrs[index] = NULL;
	
	m_nChrCount--;
	for(int i = index; i < m_nChrCount; i++) m_pChrs[i] = m_pChrs[i+1];
	m_pChrs[m_nChrCount] = NULL;
}

void CN3Scene::ChrDelete(CN3Chr *pChr)
{
	for(int i = 0; i < m_nChrCount; i++) 
	{
		if(m_pChrs[i] == pChr) 
		{
			this->ChrDelete(i);
			break;
		}
	}
}


void CN3Scene::CheckOverlappedAndReport()
{
	// 이름 중복 검사..
	CN3Base* pBases[8192];
	memset(pBases, 0, 8192*4);

	int i, j, nBC = 0;
	for(i = 0; i < m_nCameraCount; i++) pBases[nBC++] = m_pCameras[i];
	for(i = 0; i < m_nLightCount; i++) pBases[nBC++] = m_pLights[i];
	for(i = 0; i < m_nShapeCount; i++) pBases[nBC++] = m_pShapes[i];
	for(i = 0; i < m_nChrCount; i++) pBases[nBC++] = m_pChrs[i];

	for(i = 0; i < nBC; i++)
	{
		for(j = i+1; j < nBC; j++)
		{
			if(lstrcmpi(pBases[i]->Name(), pBases[j]->Name()) == 0) 
			{
				char szErr[512];
				wsprintf(szErr, "%s : 이름이 같은 오브젝트가 있습니다.", pBases[i]->Name());
				MessageBox(s_hWndBase, szErr, "Scene 중복 이름 점검", MB_OK);
			}
		}
	}
}

bool CN3Scene::LoadDataAndResourcesFromFile(const char *szFileName)
{
	if(lstrlen(szFileName) <= 0) return false;

	char szPath[512] = "", szDrv[_MAX_DRIVE] = "", szDir[_MAX_DIR] = "";
	::_splitpath(szFileName, szDrv, szDir, NULL, NULL);
	::_makepath(szPath, szDrv, szDir, NULL, NULL);

	this->Release();
	this->PathSet(szPath);
	return CN3Base::LoadFromFile(szFileName);
}

bool CN3Scene::SaveDataAndResourcesToFile(const char *szFileName)
{
	if(lstrlen(szFileName) <= 0) return false;

	char szPath[512] = "", szDrv[_MAX_DRIVE] = "", szDir[_MAX_DIR] = "";
	::_splitpath(szFileName, szDrv, szDir, NULL, NULL);
	::_makepath(szPath, szDrv, szDir, NULL, NULL);

	this->PathSet(szPath);
	return CN3Base::SaveToFile(szFileName);
}

void CN3Scene::DefaultCameraAdd()
{
	CN3Camera* pCamera = new CN3Camera();
	pCamera->NameSet("DefaultCamera");
	pCamera->FileNameSet("Data\\DefaultCamera.N3Camera");
	this->CameraAdd(pCamera);
}
void CN3Scene::DefaultLightAdd()
{
	// Light 초기화..
	CN3Light* pLight = new CN3Light();
	pLight->NameSet("DefaultLight");
	pLight->FileNameSet("Data\\DefaultLight.N3Light");
	int nLight = this->LightAdd(pLight) - 1;

	D3DCOLORVALUE ltColor = { 0.8f, 0.8f, 0.8f, 1.0f};
	pLight->m_Data.InitDirection(__Vector3(0,0,0), __Vector3(-1.0f,-1.0f,0.5f), ltColor);
	pLight->PosSet(1000.0f, 1000.0f, -1000.0f);
	pLight->m_Data.bOn = TRUE;
	pLight->m_Data.nNumber = nLight;
}

