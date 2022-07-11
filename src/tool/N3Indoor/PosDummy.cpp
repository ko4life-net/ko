// PosDummy.cpp: implementation of the CPosDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PosDummy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosDummy::CPosDummy()
{
	m_ceDType = DUMMY_POS;
	m_cePrevType = DUMMY_SWAP;
}

CPosDummy::~CPosDummy()
{

}

void CPosDummy::SetSelObj(SelectElement Obj, bool bOne)
{
	if (bOne)
		m_SelObjArray.RemoveAll();
	else
	{
		// 이미 있으면 추가하지 않는다..
		int iSize = m_SelObjArray.GetSize();
		for ( int i = 0; i < iSize; i++ )
		{
			if (m_SelObjArray[i].pSelectPointer == Obj.pSelectPointer)
			{
				// 이미 있으므로 선택목록에서 제거
				m_SelObjArray.RemoveAt(i);
				if (m_SelObjArray.GetSize() > 0)
				{
					SelectElement se = m_SelObjArray.GetAt(0);	
					m_vPos = se.pSelectPointer->Pos();
					m_qRot.x = m_qRot.y = m_qRot.z = m_qRot.w = 0;
				}
				return;
			}
		}
	}

	m_SelObjArray.Add(Obj);
	m_vPos = Obj.pSelectPointer->Pos();
	m_qRot.x = m_qRot.y = m_qRot.z = m_qRot.w = 0;
}

BOOL CPosDummy::MouseMsgFilter(LPMSG pMsg)
{
	if (m_SelObjArray.GetSize() == 0) return FALSE;

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
			DWORD nFlags = pMsg->wParam;
			if (m_pSelectedCube && (nFlags & MK_LBUTTON))
			{
				__Vector3 vRayDir, vRayOrig;	// 화면 중앙(시점)과 마우스 포인터를 이은 직선의 방향과 원점
				__Vector3 vPN, vPV;	// 평면의 법선과 포함된 점
				__Vector3 vPos;	// 위의 평면과 직선의 만나는 점(구할 점)
				__Vector3 vCameraDir = s_CameraData.vAt - s_CameraData.vEye;	vCameraDir.Normalize();
				GetPickRay(point, vRayDir, vRayOrig);
				vPV = m_vPrevPos;
				__Matrix44 mat = m_Matrix;	mat.PosSet(0,0,0);

				switch(m_pSelectedCube->iType)
				{
				case DUMMY_CENTER:
					{
						vPN = vCameraDir;
						float fT = D3DXVec3Dot(&vPN,&(vPV-vRayOrig)) / D3DXVec3Dot(&vPN, &vRayDir);
						vPos = vRayOrig + vRayDir*fT;

						// 평면상의 점에서 지형의 높이를 구한다.
						__Vector3 v1, v2, v3, v4;	float fSize = 1000.0f;
						v1.Set(-fSize, 0.0f,  fSize);
						v2.Set( fSize, 0.0f,  fSize);
						v3.Set( fSize, 0.0f, -fSize);
						v4.Set(-fSize, 0.0f, -fSize);
		
						__Vector3 vpo;	float t, u, v;
						if (::_IntersectTriangle( vRayOrig, vRayDir, v1, v2, v4, t, u, v, &vpo) || ::_IntersectTriangle( vRayOrig, vRayDir, v3, v4, v2, t, u, v, &vpo))
							vPos = vpo;

						__Vector3 vDiffPos = vPos - m_vPos;
						TransDiff(&vDiffPos, NULL, NULL);
						m_vPos = vPos;
					}
					break;
				case DUMMY_X:
					{
						vPN.Set(0, vCameraDir.y, vCameraDir.z);		vPN.Normalize();
						float fT = D3DXVec3Dot(&vPN,&(vPV-vRayOrig)) / D3DXVec3Dot(&vPN, &vRayDir);
						vPos = vRayOrig + vRayDir*fT;
						vPos += ((m_pSelectedCube->vCenterPos*(-1.0f))*mat);

						__Vector3 vDiffPos;	vDiffPos.Set(vPos.x - m_vPos.x, 0, 0);
						TransDiff(&vDiffPos, NULL, NULL);
						m_vPos.x = vPos.x;
					}
					break;
				case DUMMY_Y:
					{
						vPN.Set(vCameraDir.x, 0, vCameraDir.z);		vPN.Normalize();
						float fT = D3DXVec3Dot(&vPN,&(vPV-vRayOrig)) / D3DXVec3Dot(&vPN, &vRayDir);
						vPos = vRayOrig + vRayDir*fT;
						vPos += ((m_pSelectedCube->vCenterPos*(-1.0f))*mat);

						__Vector3 vDiffPos;	vDiffPos.Set(0, vPos.y - m_vPos.y, 0);
						TransDiff(&vDiffPos, NULL, NULL);
						m_vPos.y = vPos.y;
					}
					break;
				case DUMMY_Z:
					{
						vPN.Set(vCameraDir.x, vCameraDir.y, 0);		vPN.Normalize();
						float fT = D3DXVec3Dot(&vPN,&(vPV-vRayOrig)) / D3DXVec3Dot(&vPN, &vRayDir);
						vPos = vRayOrig + vRayDir*fT;
						vPos += ((m_pSelectedCube->vCenterPos*(-1.0f))*mat);

						__Vector3 vDiffPos;	vDiffPos.Set(0, 0, vPos.z - m_vPos.z);
						TransDiff(&vDiffPos, NULL, NULL);
						m_vPos.z = vPos.z;
					}
					break;
				}
				return TRUE;
			}
		}
		break;
	}

	return CTransDummy::MouseMsgFilter(pMsg);
}
