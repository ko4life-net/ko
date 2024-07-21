// UIEndingDisplay.cpp: implementation of the CUIEndingDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIEndingDisplay.h"
#include "UIManager.h"
#include "GameProcedure.h"
#include "GameEng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIEndingDisplay::CUIEndingDisplay() {}

CUIEndingDisplay::~CUIEndingDisplay() {
    Release();
}

void CUIEndingDisplay::Release() {
    CN3UIBase::Release();
}

void CUIEndingDisplay::Render() {
    // 화면 가운데로 맞추기..
    RECT rc = this->GetRegion();
    int  iX = (s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
    int  iY = (s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
    this->SetPos(iX, iY);

    CGameProcedure::s_pEng->Clear(0);     // 배경은 검은색
    CGameProcedure::s_pEng->BeginScene(); // 씬 렌더 ㅅ작...

    CUIManager::RenderStateSet();
    CN3UIBase::Render();
    CUIManager::RenderStateRestore();

    CGameProcedure::s_pEng->EndScene(); // 씬 렌더 시작...
    CGameProcedure::s_pEng->Present(CN3Base::s_hWndBase);
}