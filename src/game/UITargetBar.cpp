// UITargetBar.cpp: implementation of the UITargetBar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UITargetBar.h"

#include "N3Base/N3UIProgress.h"
#include "N3Base/N3UIString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUITargetBar::CUITargetBar() {
    m_pProgressHP = NULL;
    m_pProgressHP_burn = NULL;
    m_pProgressHP_curse = NULL;
    m_pProgressHP_slow = NULL;
    m_pStringID = NULL;
    m_fTimeSendPacketLast = 0;
}

CUITargetBar::~CUITargetBar() {}

void CUITargetBar::Release() {
    CN3UIBase::Release();

    m_pProgressHP = NULL;
    m_pProgressHP_burn = NULL;
    m_pProgressHP_curse = NULL;
    m_pProgressHP_slow = NULL;
    m_pStringID = NULL;
    m_fTimeSendPacketLast = 0;
}

void CUITargetBar::UpdateHP(int iHP, int iHPMax, bool bUpdateImmediately) {
    __ASSERT(iHPMax > 0, "Invalid Max HP");
    if (iHP < 0 || iHPMax <= 0) {
        return;
    }
    if (NULL == m_pProgressHP) {
        return;
    }

    int iPercentage = iHP * 100 / iHPMax;

    if (bUpdateImmediately) {
        m_pProgressHP->SetCurValue(iPercentage);
        m_pProgressHP_burn->SetCurValue(iPercentage);
        m_pProgressHP_curse->SetCurValue(iPercentage);
        m_pProgressHP_slow->SetCurValue(iPercentage);
    } else {
        m_pProgressHP->SetCurValue(iPercentage, 0.5f, 50.0f); // 1�ʵڿ� �ʴ� 50 �� �ӵ��� ���ϰ� �Ѵ�.
        m_pProgressHP_burn->SetCurValue(iPercentage, 0.5f, 50.0f);
        m_pProgressHP_curse->SetCurValue(iPercentage, 0.5f, 50.0f);
        m_pProgressHP_slow->SetCurValue(iPercentage, 0.5f, 50.0f);
    }
    return;
}

BOOL CUITargetBar::SetIDString(const std::string & szID, D3DCOLOR crID) {
    m_pStringID->SetString(szID);
    m_pStringID->SetColor(crID);
    return TRUE;
}

bool CUITargetBar::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pProgressHP = (CN3UIProgress *)GetChildByID("pro_target");
    __ASSERT(m_pProgressHP, "NULL UI Component!!");
    m_pStringID = (CN3UIString *)GetChildByID("text_target");
    __ASSERT(m_pStringID, "NULL UI Component!!");
    m_pProgressHP_burn = (CN3UIProgress *)GetChildByID("Progress_HP_lasting");
    __ASSERT(m_pProgressHP_burn, "NULL UI Component!!");
    m_pProgressHP_curse = (CN3UIProgress *)GetChildByID("Progress_HP_drop");
    __ASSERT(m_pProgressHP_curse, "NULL UI Component!!");
    m_pProgressHP_slow = (CN3UIProgress *)GetChildByID("Progress_HP_slow");
    __ASSERT(m_pProgressHP_slow, "NULL UI Component!!");

    if (m_pProgressHP_burn && m_pProgressHP_curse &&m_pProgressHP_slow) {
        m_pProgressHP_burn->SetRange(0, 100);
        m_pProgressHP_curse->SetRange(0, 100);
        m_pProgressHP_slow->SetRange(0, 100);
        m_pProgressHP_burn->SetVisible(false);
        m_pProgressHP_curse->SetVisible(false);
        m_pProgressHP_slow->SetVisible(false);
    }
    if (m_pProgressHP) {
        m_pProgressHP->SetRange(0, 100);
    }
    if (m_pStringID) // ��Ʈ�� �ٲ۴�.
    {
        std::string szFontID;
        ::_LoadStringFromResource(IDS_FONT_ID, szFontID);

        DWORD dwH = m_pStringID->GetFontHeight();
        m_pStringID->SetFont(szFontID, dwH, FALSE, FALSE);
    }

    return true;
}
