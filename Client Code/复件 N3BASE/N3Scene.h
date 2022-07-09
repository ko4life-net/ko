// N3Scene.h: interface for the CN3Scene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Scene_h__INCLUDED_)
#define AFX_N3Scene_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3Shape.h"
#include "N3Chr.h"
#include "N3Camera.h"
#include "N3Light.h"
#include "N3Mesh.h"
#include "N3PMesh.h"
#include "N3IMesh.h"
#include "N3VMesh.h"

const int MAX_SCENE_CAMERA = 32;
const int MAX_SCENE_LIGHT = 32;
const int MAX_SCENE_SHAPE = 4096;
const int MAX_SCENE_CHARACTER = 4096;

class CN3Scene : public CN3Base
{
public:
	float		m_fFrmCur, m_fFrmStart, m_fFrmEnd; // 현재, 시작, 끝 프레임..

protected:
	int	m_nCameraActive; // 현재 선택된 카메라..
	int m_nCameraCount;
	int m_nLightCount;
	int m_nShapeCount;
	int m_nChrCount;

	CN3Camera*		m_pCameras[MAX_SCENE_CAMERA];
	CN3Light*		m_pLights[MAX_SCENE_LIGHT];
	CN3Shape*		m_pShapes[MAX_SCENE_SHAPE];
	CN3Chr*			m_pChrs[MAX_SCENE_CHARACTER];
	
public:
	void DefaultCameraAdd();
	void DefaultLightAdd();
	
	bool LoadDataAndResourcesFromFile(const char* szFileName);
	bool SaveDataAndResourcesToFile(const char* szFileName);

	void CheckOverlappedAndReport();

	void Tick(float fFrm = FRAME_SELFPLAY);
	void TickCameras(float fFrm = FRAME_SELFPLAY);
	void TickLights(float fFrm = FRAME_SELFPLAY);
	void TickShapes(float fFrm = FRAME_SELFPLAY);
	void TickChrs(float fFrm = FRAME_SELFPLAY);
	void Render();
	
	int	 CameraAdd(CN3Camera *pCamera);
	void CameraDelete(CN3Camera* pCamera);
	void CameraDelete(int index);
	int	 CameraCount() { return m_nCameraCount; }
	CN3Camera* CameraGet(int index) { if(index < 0 || index >= m_nCameraCount) return NULL; return m_pCameras[index]; }
	
	void CameraSetActive(int index);
	int	 CameraGetActiveNumber() { return m_nCameraActive; };
	CN3Camera* CameraGetActive() { if(m_nCameraActive < 0 || m_nCameraActive >= m_nCameraCount) return NULL; return m_pCameras[m_nCameraActive]; }

	int	 LightAdd(CN3Light* pLight);
	void LightDelete(CN3Light* pLight);
	void LightDelete(int index);
	int	 LightCount() { return m_nLightCount; }
	CN3Light* LightGet(int index) { if(index < 0 || index >= m_nLightCount) return NULL; return m_pLights[index]; }

	int	 ShapeAdd(CN3Shape* pShape);
	void ShapeDelete(CN3Shape* pShape);
	void ShapeDelete(int index);
	int	 ShapeCount() { return m_nShapeCount; }
	CN3Shape* ShapeGet(int index) { if(index < 0 || index >= m_nShapeCount) return NULL; return m_pShapes[index]; }

	int	 ChrAdd(CN3Chr* pChr);
	void ChrDelete(int index);
	void ChrDelete(CN3Chr* pChr);
	int	 ChrCount() { return m_nChrCount; }
	CN3Chr* ChrGet(int index) { if(index < 0 || index >= m_nChrCount) return NULL; return m_pChrs[index]; }

	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	void Release();

	CN3Scene();
	virtual ~CN3Scene();

};

#endif // !defined(AFX_N3Scene_h__INCLUDED_)
