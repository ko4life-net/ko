// N3Scene.h: interface for the CN3Scene class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Camera.h"
#include "N3Light.h"
#include "N3Shape.h"
#include "N3Chr.h"
#include "N3Mesh.h"

const int MAX_SCENE_CAMERA = 32;
const int MAX_SCENE_LIGHT = 32;
const int MAX_SCENE_SHAPE = 10240;
const int MAX_SCENE_CHARACTER = 4096;

#include <vector>

typedef typename std::vector<class CN3Shape *>::iterator it_Shape;
typedef typename std::vector<class CN3Chr *>::iterator   it_Chr;

class CN3Scene : public CN3BaseFileAccess {
  public:
    float    m_fFrmCur, m_fFrmStart, m_fFrmEnd; // 현재, 시작, 끝 프레임..
    bool     m_bDisableDefaultLight;            // 참이면 기본라이트를 끈다..
    D3DCOLOR m_AmbientLightColor;

  protected:
    int m_nCameraActive; // 현재 선택된 카메라..
    int m_nCameraCount;
    int m_nLightCount;

    class CN3Camera *             m_pCameras[MAX_SCENE_CAMERA];
    class CN3Light *              m_pLights[MAX_SCENE_LIGHT];
    std::vector<class CN3Shape *> m_Shapes;
    std::vector<class CN3Chr *>   m_Chrs;

  public:
    void DefaultCameraAdd();
    void DefaultLightAdd();

    bool LoadDataAndResourcesFromFile(const fs::path & fsFile);
    bool SaveDataAndResourcesToFile(const fs::path & fsFile);

    //    bool CheckOverlappedShapesAndReport();
    //    void DeleteOverlappedShapes();

    void Tick(float fFrm = FRAME_SELFPLAY);
    void TickCameras(float fFrm = FRAME_SELFPLAY);
    void TickLights(float fFrm = FRAME_SELFPLAY);
    void TickShapes(float fFrm = FRAME_SELFPLAY);
    void TickChrs(float fFrm = FRAME_SELFPLAY);
    void Render();

    int         CameraAdd(CN3Camera * pCamera);
    void        CameraDelete(CN3Camera * pCamera);
    void        CameraDelete(int iIndex);
    int         CameraCount() { return m_nCameraCount; }
    CN3Camera * CameraGet(int iIndex) {
        if (iIndex < 0 || iIndex >= m_nCameraCount) {
            return NULL;
        }
        return m_pCameras[iIndex];
    }

    void        CameraSetActive(int iIndex);
    int         CameraGetActiveNumber() { return m_nCameraActive; };
    CN3Camera * CameraGetActive() {
        if (m_nCameraActive < 0 || m_nCameraActive >= m_nCameraCount) {
            return NULL;
        }
        return m_pCameras[m_nCameraActive];
    }

    int        LightAdd(CN3Light * pLight);
    void       LightDelete(CN3Light * pLight);
    void       LightDelete(int iIndex);
    int        LightCount() { return m_nLightCount; }
    CN3Light * LightGet(int iIndex) {
        if (iIndex < 0 || iIndex >= m_nLightCount) {
            return NULL;
        }
        return m_pLights[iIndex];
    }

    int        ShapeAdd(CN3Shape * pShape);
    void       ShapeDelete(CN3Shape * pShape);
    void       ShapeDelete(int iIndex);
    int        ShapeCount() { return m_Shapes.size(); }
    CN3Shape * ShapeGet(int iIndex) {
        if (m_Shapes.empty() || iIndex < 0 || iIndex >= m_Shapes.size()) {
            return NULL;
        }
        return m_Shapes[iIndex];
    }
    CN3Shape * ShapeGetByFile(const fs::path & fsFile) {
        if (m_Shapes.empty()) {
            return NULL;
        }
        int iSize = m_Shapes.size();
        for (int i = 0; i < iSize; i++) {
            if (fsFile == m_Shapes[i]->FilePath()) {
                return m_Shapes[i];
            }
        }
        return NULL;
    }

    void ShapeRelease();

    int      ChrAdd(CN3Chr * pChr);
    void     ChrDelete(int iIndex);
    void     ChrDelete(CN3Chr * pChr);
    int      ChrCount() { return m_Chrs.size(); }
    CN3Chr * ChrGet(int iIndex) {
        if (iIndex < 0 || iIndex >= m_Chrs.size()) {
            return NULL;
        }
        return m_Chrs[iIndex];
    }

    void ChrRelease();

    bool Load(HANDLE hFile);
    bool Save(HANDLE hFile);

    void Release();

    CN3Scene();
    virtual ~CN3Scene();
};
