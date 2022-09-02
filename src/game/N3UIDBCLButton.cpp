// N3UIIcon.cpp: implementation of the CN3UIIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3UIDBCLButton.h"
#include "GameProcedure.h"
#include "UIImageTooltipDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIDBCLButton::CN3UIDBCLButton() {
    CN3UIImage::CN3UIImage();
}

CN3UIDBCLButton::~CN3UIDBCLButton() {}

DWORD CN3UIDBCLButton::MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld) {
    DWORD dwRet = UI_MOUSEPROC_NONE;

    RECT rect = GetRegion();
    if (!::PtInRect(&rect, ptCur)) // 영역 밖이면
    {
        dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
        return dwRet;
    }

    if (dwFlags & UI_MOUSE_LBDBLCLK) {
        m_pParent->ReceiveMessage(this, UIMSG_ICON_DBLCLK); // 부모에게 버튼 클릭 통지..
        dwRet |= UI_MOUSEPROC_DONESOMETHING;
        return dwRet;
    }

    dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
    return dwRet;
}