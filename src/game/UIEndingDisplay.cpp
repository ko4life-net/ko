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
    // È­¸é °¡¿îµ¥·Î ¸ÂÃß±â..
    RECT rc = this->GetRegion();
    int  iX = (s_CameraData.vp.Width - (rc.right - rc.left)) / 2;
    int  iY = (s_CameraData.vp.Height - (rc.bottom - rc.top)) / 2;
    this->SetPos(iX, iY);

    CGameProcedure::s_pEng->Clear(0);     // ¹è°æÀº °ËÀº»ö
    CGameProcedure::s_pEng->BeginScene(); // ¾À ·»´õ ¤µÀÛ...

    CUIManager::RenderStateSet();
    CN3UIBase::Render();
    CUIManager::RenderStateRestore();

    CGameProcedure::s_pEng->EndScene(); // ¾À ·»´õ ½ÃÀÛ...
    CGameProcedure::s_pEng->Present(CN3Base::s_hWndBase);
}