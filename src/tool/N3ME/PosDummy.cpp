// PosDummy.cpp: implementation of the CPosDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "PosDummy.h"
#include "LyTerrain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosDummy::CPosDummy() {}

CPosDummy::~CPosDummy() {}

void CPosDummy::SetSelObj(CN3Transform * pObj) {
    m_SelObjArray.RemoveAll();
    if (pObj) {
        m_SelObjArray.Add(pObj);
        m_vPos = pObj->Pos();
        //        m_qRot.x = m_qRot.y = m_qRot.z = m_qRot.w = 0;
    }
}

BOOL CPosDummy::MouseMsgFilter(LPMSG pMsg) {
    if (m_SelObjArray.GetSize() == 0) {
        return FALSE;
    }

    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        DWORD nFlags = pMsg->wParam;
        if (m_pSelectedCube && (nFlags & MK_LBUTTON)) {
            __Vector3 vRayDir,
                vRayOrig; // Direction and origin of the straight line connecting the screen center (viewpoint) and the mouse pointer
            __Vector3 vPN, vPV; // Plane normal and contained points
            __Vector3 vPos;     // Point where the above plane meets the straight line (point to find)
            __Vector3 vCameraDir = s_CameraData.vAt - s_CameraData.vEye;
            vCameraDir.Normalize();
            GetPickRay(point, vRayDir, vRayOrig);
            vPV = m_vPrevPos;
            __Matrix44 mat = m_Matrix;
            mat.PosSet(0, 0, 0);

            switch (m_pSelectedCube->iType) {
            case DUMMY_CENTER: {
                vPN = vCameraDir;
                __Vector3 vPR = vPV - vRayOrig;
                float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
                vPos = vRayOrig + vRayDir * fT;

                // Find the height of the terrain from a point on the plane.
                if (m_pTerrainRef) {
                    m_pTerrainRef->Pick(point.x, point.y, &vPos);
                }

                __Vector3 vDiffPos = vPos - m_vPos;
                TransDiff(&vDiffPos, NULL, NULL);
                m_vPos = vPos;
            } break;
            case DUMMY_X: {
                vPN.Set(0, vCameraDir.y, vCameraDir.z);
                vPN.Normalize();
                __Vector3 vPR = vPV - vRayOrig;
                float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
                vPos = vRayOrig + vRayDir * fT;
                vPos += ((m_pSelectedCube->vCenterPos * (-1.0f)) * mat);

                __Vector3 vDiffPos;
                vDiffPos.Set(vPos.x - m_vPos.x, 0, 0);
                TransDiff(&vDiffPos, NULL, NULL);
                m_vPos.x = vPos.x;
            } break;
            case DUMMY_Y: {
                vPN.Set(vCameraDir.x, 0, vCameraDir.z);
                vPN.Normalize();
                __Vector3 vPR = vPV - vRayOrig;
                float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
                vPos = vRayOrig + vRayDir * fT;
                vPos += ((m_pSelectedCube->vCenterPos * (-1.0f)) * mat);

                __Vector3 vDiffPos;
                vDiffPos.Set(0, vPos.y - m_vPos.y, 0);
                TransDiff(&vDiffPos, NULL, NULL);
                m_vPos.y = vPos.y;
            } break;
            case DUMMY_Z: {
                vPN.Set(vCameraDir.x, vCameraDir.y, 0);
                vPN.Normalize();
                __Vector3 vPR = vPV - vRayOrig;
                float     fT = D3DXVec3Dot(&vPN, &vPR) / D3DXVec3Dot(&vPN, &vRayDir);
                vPos = vRayOrig + vRayDir * fT;
                vPos += ((m_pSelectedCube->vCenterPos * (-1.0f)) * mat);

                __Vector3 vDiffPos;
                vDiffPos.Set(0, 0, vPos.z - m_vPos.z);
                TransDiff(&vDiffPos, NULL, NULL);
                m_vPos.z = vPos.z;
            } break;
            }
            return TRUE;
        }
    } break;
    }

    return CTransDummy::MouseMsgFilter(pMsg);
}
