// RotDummy.cpp: implementation of the CRotDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "RotDummy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRotDummy::CRotDummy() {
    m_ceDType = DUMMY_ROT;
    m_cePrevType = DUMMY_SWAP;
}

CRotDummy::~CRotDummy() {}

BOOL CRotDummy::MouseMsgFilter(LPMSG pMsg) {
    if (m_SelObjArray.GetSize() == 0) {
        return FALSE;
    }

    static POINT ptPrevCursor;
    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        DWORD nFlags = pMsg->wParam;

        int         iDiff = point.x - ptPrevCursor.x;
        const float fDelta = 0.005f;
        ptPrevCursor = point;
        if (m_pSelectedCube && (nFlags & MK_LBUTTON)) {
            switch (m_pSelectedCube->iType) {
            case DUMMY_CENTER: {
            } break;
            case DUMMY_X: {
                __Quaternion q;
                q.RotationAxis(1, 0, 0, fDelta * iDiff);
                m_qRot *= q;

                this->TransDiff(NULL, &q, NULL);
            } break;
            case DUMMY_Y: {
                __Quaternion q;
                q.RotationAxis(0, 1, 0, fDelta * iDiff);
                m_qRot *= q;

                this->TransDiff(NULL, &q, NULL);
            } break;
            case DUMMY_Z: {
                __Quaternion q;
                q.RotationAxis(0, 0, 1, fDelta * iDiff);
                m_qRot *= q;

                this->TransDiff(NULL, &q, NULL);
            } break;
            }
            return TRUE;
        }
    }
    case WM_LBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        ptPrevCursor.x = point.x;
        ptPrevCursor.y = point.y;
    } break;
    }

    return CTransDummy::MouseMsgFilter(pMsg);
}

void CRotDummy::SetSelObj(SelectElement Obj, bool bOne) {
    if (bOne) {
        m_SelObjArray.RemoveAll();
    } else {
        // 이미 있으면 추가하지 않는다..
        int iSize = m_SelObjArray.GetSize();
        for (int i = 0; i < iSize; i++) {
            if (m_SelObjArray[i].pSelectPointer == Obj.pSelectPointer) {
                // 이미 있으므로 선택목록에서 제거
                m_SelObjArray.RemoveAt(i);
                return;
            }
        }
    }

    m_SelObjArray.Add(Obj);
    m_vPos = Obj.pSelectPointer->Pos();

    m_qRot = Obj.pSelectPointer->Rot();
    m_qRot.Identity(); // Quaternion 회전이기 땜시... 초기화를 해준다..
}
