// UISkillTreeDlg.cpp: implementation of the CUISkillTreeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "Resource.h"
#include "UISkillTreeDlg.h"

#include "GameProcedure.h"
#include "PlayerMySelf.h"

#include "N3UIIcon.h"

#include "GameProcMain.h"
#include "UIHotKeyDlg.h"
#include "MagicSkillMng.h"
#include "UIManager.h"

#include "N3Base/N3UIArea.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUISkillTreeDlg::CUISkillTreeDlg()
{
	m_bOpenningNow = false; // ������ �ִ�..
	m_bClosingNow = false;	// ������ �ִ�..
	m_fMoveDelta = 0.0f; // �ε巴�� ������ ������ ����� ���ؼ� ������ġ ��꿡 �ε��Ҽ����� ����..

	m_iRBtnDownOffs = -1;
	m_iCurKindOf = 0;
	m_iCurSkillPage = 0;

	for (int i = 0; i < MAX_SKILL_FROM_SERVER; i++)
		m_iSkillInfo[i] = NULL;

	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
		for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
			for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
				m_pMySkillTree[i][j][k] = NULL;

	CN3UIWndBase::m_sSkillSelectInfo.UIWnd = UIWND_HOTKEY;
	CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
}

CUISkillTreeDlg::~CUISkillTreeDlg()
{
	Release();
	/*
		for( int i = 0; i < MAX_SKILL_KIND_OF; i++ )
			for( int j = 0; j < MAX_SKILL_PAGE_NUM; j++ )
				for( int k = 0; k < MAX_SKILL_IN_PAGE; k++ )
					if ( m_pMySkillTree[i][j][k] != NULL )
					{
						delete m_pMySkillTree[i][j][k];
						m_pMySkillTree[i][j][k] = NULL;
					}

		if ( (CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo != NULL) && (CN3UIWndBase::m_sSkillSelectInfo.UIWnd == UIWND_SKILL_TREE) )
		{
			delete CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo;
			CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
		}*/
}

void CUISkillTreeDlg::Release()
{
	CN3UIBase::Release();

	m_bOpenningNow = false; // ������ �ִ�..
	m_bClosingNow = false;	// ������ �ִ�..
	m_fMoveDelta = 0.0f; // �ε巴�� ������ ������ ����� ���ؼ� ������ġ ��꿡 �ε��Ҽ����� ����..

	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
	{
		for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
		{
			for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
			{
				if (m_pMySkillTree[i][j][k] != NULL)
				{
					delete m_pMySkillTree[i][j][k];
					m_pMySkillTree[i][j][k] = NULL;
				}
			}
		}
	}

	if ((CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo != NULL) && (CN3UIWndBase::m_sSkillSelectInfo.UIWnd == UIWND_SKILL_TREE))
	{
		delete CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo;
		CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
	}
}

bool CUISkillTreeDlg::HasIDSkill(int iID)
{
	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
		for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
			for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
				if (m_pMySkillTree[i][j][k] != NULL)
				{
					if (m_pMySkillTree[i][j][k]->pSkill->dwID == iID)
						return true;
				}

	return false;
}

void CUISkillTreeDlg::UpdateDisableCheck()
{
	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
		for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
			for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
				if (m_pMySkillTree[i][j][k] != NULL)
				{
					DWORD bitMask = UISTYLE_ICON_SKILL;
					if (!CGameProcedure::s_pProcMain->m_pMagicSkillMng->CheckValidSkillMagic(m_pMySkillTree[i][j][k]->pSkill))
						bitMask |= UISTYLE_DISABLE_SKILL;
					m_pMySkillTree[i][j][k]->pUIIcon->SetStyle(bitMask);
				}
}

void CUISkillTreeDlg::Tick()
{
	if (m_bOpenningNow) // �����ʿ��� �������� ������...������ �Ѵٸ�..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width - (int)fWidth;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)m_fMoveDelta;
		if (ptCur.x <= iXLimit) // �ٿ��ȴ�!!
		{
			ptCur.x = iXLimit;
			m_bOpenningNow = false;
		}

		this->SetPos(ptCur.x, ptCur.y);
	}
	else if (m_bClosingNow) // �����ʿ��� �������� ������...������ �Ѵٸ�..
	{
		POINT ptCur = this->GetPos();
		RECT rc = this->GetRegion();
		float fWidth = (float)(rc.right - rc.left);

		float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
		fDelta *= (fWidth - m_fMoveDelta) / fWidth;
		if (fDelta < 2.0f) fDelta = 2.0f;
		m_fMoveDelta += fDelta;

		int iXLimit = CN3Base::s_CameraData.vp.Width;
		ptCur.x = CN3Base::s_CameraData.vp.Width - (int)(fWidth - m_fMoveDelta);
		if (ptCur.x >= iXLimit) // �� ������..!!
		{
			ptCur.x = iXLimit;
			m_bClosingNow = false;

			this->SetVisibleWithNoSound(false, false, true); // �� �������� ������ �Ⱥ��̰� �Ѵ�.
		}

		this->SetPos(ptCur.x, ptCur.y);
	}

	CN3UIBase::Tick();
}

DWORD CUISkillTreeDlg::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!IsVisible()) { dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);  return dwRet; }
	// ������ ���� �ʴ´�..
	if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) { dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);  return dwRet; }

	// �巡�� �Ǵ� ������ ����..
	if (GetState() == UI_STATE_ICON_MOVING)
	{
		if (CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo)
		{
			CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon->SetRegion(GetSampleRect());
			CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon->SetMoveRect(GetSampleRect());
		}
	}

	return CN3UIWndBase::MouseProc(dwFlags, ptCur, ptOld);
}

int CUISkillTreeDlg::GetIndexInArea(POINT pt)
{
	CN3UIArea* pArea;
	RECT	rect;

	for (int i = 0; i < MAX_SKILL_IN_PAGE; i++)
	{
		pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_SKILL_TREE, i);
		rect = pArea->GetRegion();

		if ((pt.x >= rect.left) && (pt.x <= rect.right) && (pt.y >= rect.top) && (pt.y <= rect.bottom))
			return i;
	}

	return -1;
}

bool CUISkillTreeDlg::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (NULL == pSender) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender->m_szID == "btn_close")
			Close();
		//..
		if (pSender->m_szID == "btn_left")
			PageLeft();
		if (pSender->m_szID == "btn_right")
			PageRight();
		//..
		if (pSender->m_szID == "btn_0")
			PointPushUpButton(1);
		if (pSender->m_szID == "btn_1")
			PointPushUpButton(2);
		if (pSender->m_szID == "btn_2")
			PointPushUpButton(3);
		if (pSender->m_szID == "btn_3")
			PointPushUpButton(4);
		if (pSender->m_szID == "btn_4")
			PointPushUpButton(5);
		if (pSender->m_szID == "btn_5")
			PointPushUpButton(6);
		if (pSender->m_szID == "btn_6")
			PointPushUpButton(7);
		if (pSender->m_szID == "btn_7")
			PointPushUpButton(8);
		//..
		if (pSender->m_szID == "btn_public")
			SetPageInIconRegion(0, 0);
		if ((pSender->m_szID == "btn_ranger0") || (pSender->m_szID == "btn_blade0") || (pSender->m_szID == "btn_mage0") ||
			(pSender->m_szID == "btn_cleric0") || (pSender->m_szID == "btn_hunter0") || (pSender->m_szID == "btn_berserker0") ||
			(pSender->m_szID == "btn_sorcerer0") || (pSender->m_szID == "btn_shaman0"))
			SetPageInIconRegion(1, 0);
		if ((pSender->m_szID == "btn_ranger1") || (pSender->m_szID == "btn_blade1") || (pSender->m_szID == "btn_mage1") ||
			(pSender->m_szID == "btn_cleric1") || (pSender->m_szID == "btn_hunter1") || (pSender->m_szID == "btn_berserker1") ||
			(pSender->m_szID == "btn_sorcerer1") || (pSender->m_szID == "btn_shaman1"))
			SetPageInIconRegion(2, 0);
		if ((pSender->m_szID == "btn_ranger2") || (pSender->m_szID == "btn_blade2") || (pSender->m_szID == "btn_mage2") ||
			(pSender->m_szID == "btn_cleric2") || (pSender->m_szID == "btn_hunter2") || (pSender->m_szID == "btn_berserker2") ||
			(pSender->m_szID == "btn_sorcerer2") || (pSender->m_szID == "btn_shaman2"))
			SetPageInIconRegion(3, 0);
		//if( (pSender->m_szID == "btn_ranger3") || (pSender->m_szID == "btn_blade3") || (pSender->m_szID == "btn_mage3") || 
		//		(pSender->m_szID == "btn_cleric3") || (pSender->m_szID == "btn_hunter3") || (pSender->m_szID == "btn_berserker3") || 
		//		(pSender->m_szID == "btn_sorcerer3") || (pSender->m_szID == "btn_shaman3") )
		//	SetPageInIconRegion(4, 0);
	}

	// Temp Define
#define FAIL_CODE {		\
				SetState(UI_STATE_COMMON_NONE);	\
				return false;	\
			}

	__IconItemSkill* spSkill, * spSkillCopy;

	DWORD dwLBitMask = 0x000f0000;
	DWORD dwRBitMask = 0x0f000000;
	DWORD dwBitMask = dwLBitMask | dwRBitMask;

	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	DWORD bitMask;

	switch (dwMsg & dwBitMask)
	{
	case UIMSG_ICON_RDOWN_FIRST:
		m_iRBtnDownOffs = GetIndexInArea(ptCur);
		break;

	case UIMSG_ICON_RUP:
	{
		int iRBtn = GetIndexInArea(ptCur);
		if (iRBtn != -1 && m_iRBtnDownOffs != -1 && m_iRBtnDownOffs == iRBtn)
		{
			// �� �ڸ��� �ִ��� �˻�..
			CUIHotKeyDlg* pDlg = CGameProcedure::s_pProcMain->m_pUIHotKeyDlg;
			int iIndex;
			if (pDlg->GetEmptySlotIndex(iIndex))
			{
				// Get Item..
				spSkill = GetHighlightIconItem((CN3UIIcon*)pSender);

				spSkillCopy = new __IconItemSkill();
				spSkillCopy->pSkill = spSkill->pSkill;
				spSkillCopy->szIconFN = spSkill->szIconFN;

				// ������ �ε��ϱ�.. ^^
				spSkillCopy->pUIIcon = new CN3UIIcon;
				spSkillCopy->pUIIcon->Init(this);
				spSkillCopy->pUIIcon->SetTex(spSkill->szIconFN);
				spSkillCopy->pUIIcon->SetUVRect(0, 0, 1, 1);
				spSkillCopy->pUIIcon->SetUIType(UI_TYPE_ICON);

				bitMask = UISTYLE_ICON_SKILL;
				if (!CGameProcedure::s_pProcMain->m_pMagicSkillMng->CheckValidSkillMagic(spSkillCopy->pSkill))
					bitMask |= UISTYLE_DISABLE_SKILL;
				spSkillCopy->pUIIcon->SetStyle(bitMask);

				// Save Select Info..
				CN3UIWndBase::m_sSkillSelectInfo.UIWnd = UIWND_SKILL_TREE;
				CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = spSkillCopy;

				pDlg->SetReceiveSelectedSkill(iIndex);

				CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
				pDlg->CloseIconRegistry();
			}
		}
	}
	break;

	case UIMSG_ICON_DOWN_FIRST:
		// Get Item..
		spSkill = GetHighlightIconItem((CN3UIIcon*)pSender);

		// ���纻�� �����.. 
		spSkillCopy = new __IconItemSkill();
		spSkillCopy->pSkill = spSkill->pSkill;
		spSkillCopy->szIconFN = spSkill->szIconFN;

		// ������ �ε��ϱ�.. ^^
		spSkillCopy->pUIIcon = new CN3UIIcon;
		spSkillCopy->pUIIcon->Init(this);
		spSkillCopy->pUIIcon->SetTex(spSkill->szIconFN);
		spSkillCopy->pUIIcon->SetUVRect(0, 0, 1, 1);
		spSkillCopy->pUIIcon->SetUIType(UI_TYPE_ICON);

		bitMask = UISTYLE_ICON_SKILL;
		if (!CGameProcedure::s_pProcMain->m_pMagicSkillMng->CheckValidSkillMagic(spSkillCopy->pSkill))
			bitMask |= UISTYLE_DISABLE_SKILL;
		spSkillCopy->pUIIcon->SetStyle(bitMask);

		spSkillCopy->pUIIcon->SetRegion(GetSampleRect());
		spSkillCopy->pUIIcon->SetMoveRect(GetSampleRect());

		// Save Select Info..
		CN3UIWndBase::m_sSkillSelectInfo.UIWnd = UIWND_SKILL_TREE;
		CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = spSkillCopy;
		break;

	case UIMSG_ICON_DOWN:
		if (GetState() == UI_STATE_ICON_MOVING)
		{
			if (CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo)
			{
				CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon->SetRegion(GetSampleRect());
				CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon->SetMoveRect(GetSampleRect());
			}
		}
		break;

	case UIMSG_ICON_UP:
		// Hot Key �����츦 ���� �ٴϸ鼭 �˻�..
	{
		CUIHotKeyDlg* pDlg = CGameProcedure::s_pProcMain->m_pUIHotKeyDlg;
		if (!IsIn(ptCur.x, ptCur.y) && pDlg->IsIn(ptCur.x, ptCur.y))
		{
			if (!pDlg->IsSelectedSkillInRealIconArea())
			{
				// ���ҽ� Free..
				spSkill = CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo;
				if (spSkill)
				{
					// �Ŵ������� ����..
					RemoveChild(spSkill->pUIIcon);

					// ���ҽ� ����..
					spSkill->pUIIcon->Release();
					delete spSkill->pUIIcon;
					spSkill->pUIIcon = NULL;
					delete spSkill;
					spSkill = NULL;
				}
			}
		}
		else
		{
			// ���ҽ� Free..
			spSkill = CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo;
			if (spSkill)
			{
				// �Ŵ������� ����..
				RemoveChild(spSkill->pUIIcon);

				// ���ҽ� ����..
				spSkill->pUIIcon->Release();
				delete spSkill->pUIIcon;
				spSkill->pUIIcon = NULL;
				delete spSkill;
				spSkill = NULL;
			}
		}

		CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
		SetState(UI_STATE_COMMON_NONE);
		pDlg->CloseIconRegistry();
	}
	break;

	case UIMSG_ICON_DBLCLK:
		SetState(UI_STATE_COMMON_NONE);
		break;
	}

	return false;
}

void CUISkillTreeDlg::PageLeft()
{
	if (m_iCurSkillPage == 0)
		return;

	SetPageInIconRegion(m_iCurKindOf, m_iCurSkillPage - 1);
}

void CUISkillTreeDlg::PageRight()
{
	if (m_iCurSkillPage == 2)
		return;

	SetPageInIconRegion(m_iCurKindOf, m_iCurSkillPage + 1);
}

void CUISkillTreeDlg::PointPushUpButton(int iValue)
{
	int iCurKindOfBackup, iCurSkillPageBackup;
	iCurKindOfBackup = m_iCurKindOf;
	iCurSkillPageBackup = m_iCurSkillPage;

	int iSkillExtra;			// ��ųâ�� ��..
	int iSkillPoint;
	std::string str;
	CN3UIString* pStrName = NULL, * pStrName2 = NULL;
	pStrName = (CN3UIString*)GetChildByID("string_skillpoint"); __ASSERT(pStrName, "NULL UI Component!!");
	str = pStrName->GetString();
	iSkillExtra = atoi(str.c_str());

	if (iSkillExtra == 0)
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_SKILL_POINT_EXTRA_NOT_EXIST, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
		return;
	}

	if ((iValue == 1) || (iValue == 2) || (iValue == 3) || (iValue == 4))	//.. 
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
		return;
	}

	// �������� ���� ���¸�.. ������ų�� �ø��� ����..
	switch (iValue)
	{
	case 5:	case 6:	case 7: case 8:
	{
		switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
		{
		case NATION_KARUS:			// ī�罺..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_KA_WARRIOR:
			case CLASS_KA_ROGUE:
			case CLASS_KA_WIZARD:
			case CLASS_KA_PRIEST:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_BEFORE_CLASS_CHANGE, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;

		case NATION_ELMORAD:		// �����..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_EL_WARRIOR:
			case CLASS_EL_ROGUE:
			case CLASS_EL_WIZARD:
			case CLASS_EL_PRIEST:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_BEFORE_CLASS_CHANGE, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;
		}
	}
	break;
	}

	if (iValue == 8)
	{
		switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
		{
		case NATION_KARUS:			// ī�罺..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_KA_SORCERER:
			case CLASS_KA_HUNTER:
			case CLASS_KA_SHAMAN:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;

		case NATION_ELMORAD:		// �����..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_EL_MAGE:
			case CLASS_EL_RANGER:
			case CLASS_EL_CLERIC:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;
		}
	}

	if (iValue == 7)
	{
		switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
		{
		case NATION_KARUS:			// ī�罺..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_KA_BERSERKER:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;

		case NATION_ELMORAD:		// �����..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_EL_BLADE:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;
		}
	}

	if (iValue == 6)
	{
		switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
		{
		case NATION_KARUS:			// ī�罺..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_KA_HUNTER:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;

		case NATION_ELMORAD:		// �����..
			switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
			{
			case CLASS_EL_RANGER:
			{
				std::string szMsg;
				::_LoadStringFromResource(IDS_SKILL_POINT_NOT_YET, szMsg);
				CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
				return;
			}
			break;
			}
			break;
		}
	}

	switch (iValue)
	{
	case 1:
		pStrName2 = (CN3UIString*)GetChildByID("string_0"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 2:
		pStrName2 = (CN3UIString*)GetChildByID("string_1"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 3:
		pStrName2 = (CN3UIString*)GetChildByID("string_2"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 4:
		pStrName2 = (CN3UIString*)GetChildByID("string_3"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 5:
		pStrName2 = (CN3UIString*)GetChildByID("string_4"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 6:
		pStrName2 = (CN3UIString*)GetChildByID("string_5"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 7:
		pStrName2 = (CN3UIString*)GetChildByID("string_6"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	case 8:
		pStrName2 = (CN3UIString*)GetChildByID("string_7"); __ASSERT(pStrName2, "NULL UI Component!!");
		break;

	}

	str = pStrName2->GetString();
	iSkillPoint = atoi(str.c_str());

	// �ڱ� �ڽ� �������� ���ϼ� ����..
	if (iSkillPoint >= CGameBase::s_pPlayer->m_InfoBase.iLevel)
	{
		std::string szMsg;
		::_LoadStringFromResource(IDS_SKILL_UP_INVALID, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
		return;
	}

	// ������� ������.. ���� ������Ʈ..	
	BYTE byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SKILL_CHANGE);
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)iValue);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);

	// ��ų ����Ʈ ������Ʈ..
	iSkillExtra--;
	pStrName->SetStringAsInt(iSkillExtra);
	m_iSkillInfo[0] = iSkillExtra;

	TRACE("Send to Server Skill point up index %d\n", iValue);

	iSkillPoint++;
	pStrName2->SetStringAsInt(iSkillPoint);
	switch (iValue)
	{
	case 1:
		m_iSkillInfo[1] = iSkillPoint;
		break;
	case 2:
		m_iSkillInfo[2] = iSkillPoint;
		break;
	case 3:
		m_iSkillInfo[3] = iSkillPoint;
		break;
	case 4:
		m_iSkillInfo[4] = iSkillPoint;
		break;
	case 5:
		m_iSkillInfo[5] = iSkillPoint;
		break;
	case 6:
		m_iSkillInfo[6] = iSkillPoint;
		break;
	case 7:
		m_iSkillInfo[7] = iSkillPoint;
		break;
	case 8:
		m_iSkillInfo[8] = iSkillPoint;
		break;
	}

	switch (iValue)		// ��ų ������ ������Ʈ..
	{
	case 5:
	case 6:
	case 7:
	case 8:
		InitIconUpdate();
		break;
	}

	SetPageInIconRegion(iCurKindOfBackup, iCurSkillPageBackup);
}

void CUISkillTreeDlg::Render()
{
	if (!m_bVisible) return;	// ������ ������ �ڽĵ��� render���� �ʴ´�.
	__IconItemSkill* spSkill = NULL;

	TooltipRenderDisable();

	for (UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if ((GetState() == UI_STATE_ICON_MOVING) && (pChild->UIType() == UI_TYPE_ICON) && (CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo) &&
			((CN3UIIcon*)pChild == CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon))
			continue;
		pChild->Render();
		if ((pChild->UIType() == UI_TYPE_ICON) && (pChild->GetStyle() & UISTYLE_ICON_HIGHLIGHT) &&
			(GetState() == UI_STATE_COMMON_NONE))
		{
			spSkill = GetHighlightIconItem((CN3UIIcon*)pChild);
			TooltipRenderEnable(spSkill);
		}
	}


	CheckButtonTooltipRenderEnable();

	if ((GetState() == UI_STATE_ICON_MOVING) && (CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo))
		CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo->pUIIcon->Render();
}

void CUISkillTreeDlg::CheckButtonTooltipRenderEnable()
{
#define IN_RECT(a, b)	 ((a.left < b.x) && (a.right > b.x) && (a.top < b.y) && (a.bottom > b.y))

	RECT rect[MAX_SKILL_KIND_OF];
	memset(rect, 0, sizeof(RECT) * MAX_SKILL_KIND_OF);

	rect[SKILL_DEF_BASIC] = ((CN3UIButton*)GetChildByID("btn_public"))->GetClickRect();

	switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
	{
	case NATION_ELMORAD:
		rect[SKILL_DEF_SPECIAL0] = ((CN3UIButton*)GetChildByID("btn_blade0"))->GetClickRect();
		rect[SKILL_DEF_SPECIAL1] = ((CN3UIButton*)GetChildByID("btn_blade1"))->GetClickRect();
		rect[SKILL_DEF_SPECIAL2] = ((CN3UIButton*)GetChildByID("btn_blade2"))->GetClickRect();
		//rect[SKILL_DEF_SPECIAL3]	= ((CN3UIButton* )GetChildByID("btn_blade3"))->GetClickRect();
		break;
	case NATION_KARUS:
		rect[SKILL_DEF_SPECIAL0] = ((CN3UIButton*)GetChildByID("btn_berserker0"))->GetClickRect();
		rect[SKILL_DEF_SPECIAL1] = ((CN3UIButton*)GetChildByID("btn_berserker1"))->GetClickRect();
		rect[SKILL_DEF_SPECIAL2] = ((CN3UIButton*)GetChildByID("btn_berserker2"))->GetClickRect();
		//rect[SKILL_DEF_SPECIAL3]	= ((CN3UIButton* )GetChildByID("btn_berserker3"))->GetClickRect();
		break;
	}

	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();

	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
	{
		if (IN_RECT(rect[i], ptCur))
			ButtonTooltipRender(i);
	}
}
void CUISkillTreeDlg::MasterIconsButtonsVisibleSateSet()
{
	switch (CGameBase::s_pPlayer->m_InfoBase.eNation) {
	case NATION_ELMORAD:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass) {
		case CLASS_EL_WARRIOR:
		case CLASS_EL_ROGUE:
		case CLASS_EL_WIZARD:
		case CLASS_EL_PRIEST:

			GetChildByID("img_Paladin")->SetVisible(false);
			GetChildByID("img_Arc Mage")->SetVisible(false);
			GetChildByID("img_kasar hood")->SetVisible(false);
			GetChildByID("img_Blade Master")->SetVisible(false);
			GetChildByID("btn_master")->SetVisible(false);
			GetChildByID("img_master")->SetVisible(false);

			break;
		}
		break;
	case NATION_KARUS:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass) {
		case CLASS_KA_WARRIOR:
		case CLASS_KA_ROGUE:
		case CLASS_KA_WIZARD:
		case CLASS_KA_PRIEST:

			GetChildByID("img_Shadow Knight")->SetVisible(false);
			GetChildByID("img_Elemental Lord")->SetVisible(false);
			GetChildByID("img_Shadow Bane")->SetVisible(false);
			GetChildByID("img_Berserker Hero")->SetVisible(false);
			GetChildByID("btn_master")->SetVisible(false);
			GetChildByID("img_master")->SetVisible(false);

			break;
		}
		break;
	}
}

void CUISkillTreeDlg::ButtonTooltipRender(int iIndex)
{
	std::string szStr;
	char pszDesc[256];		memset(pszDesc, 0, sizeof(char) * 256);
	if (!m_pStr_info->IsVisible())	m_pStr_info->SetVisible(true);

	switch (iIndex)
	{
	case SKILL_DEF_BASIC:
		::_LoadStringFromResource(IDS_SKILL_INFO_BASE, szStr);
		sprintf(pszDesc, szStr.c_str());
		break;

	case SKILL_DEF_SPECIAL0:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_EL_BLADE:
		case CLASS_KA_BERSERKER:
			::_LoadStringFromResource(IDS_SKILL_INFO_BLADE0, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_RANGER:
		case CLASS_KA_HUNTER:
			::_LoadStringFromResource(IDS_SKILL_INFO_RANGER0, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_CLERIC:
		case CLASS_KA_SHAMAN:
			::_LoadStringFromResource(IDS_SKILL_INFO_CLERIC0, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_MAGE:
		case CLASS_KA_SORCERER:
			::_LoadStringFromResource(IDS_SKILL_INFO_MAGE0, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		}
		break;

	case SKILL_DEF_SPECIAL1:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_EL_BLADE:
		case CLASS_KA_BERSERKER:
			::_LoadStringFromResource(IDS_SKILL_INFO_BLADE1, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_RANGER:
		case CLASS_KA_HUNTER:
			::_LoadStringFromResource(IDS_SKILL_INFO_RANGER1, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_CLERIC:
		case CLASS_KA_SHAMAN:
			::_LoadStringFromResource(IDS_SKILL_INFO_CLERIC1, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_MAGE:
		case CLASS_KA_SORCERER:
			::_LoadStringFromResource(IDS_SKILL_INFO_MAGE1, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		}
		break;

	case SKILL_DEF_SPECIAL2:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_EL_BLADE:
		case CLASS_KA_BERSERKER:
			::_LoadStringFromResource(IDS_SKILL_INFO_BLADE2, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_RANGER:
		case CLASS_KA_HUNTER:
			::_LoadStringFromResource(IDS_SKILL_INFO_RANGER2, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_CLERIC:
		case CLASS_KA_SHAMAN:
			::_LoadStringFromResource(IDS_SKILL_INFO_CLERIC2, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_MAGE:
		case CLASS_KA_SORCERER:
			::_LoadStringFromResource(IDS_SKILL_INFO_MAGE2, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		}
		break;

	case SKILL_DEF_SPECIAL3:
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_EL_BLADE:
		case CLASS_KA_BERSERKER:
			::_LoadStringFromResource(IDS_SKILL_INFO_BLADE3, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_RANGER:
		case CLASS_KA_HUNTER:
			::_LoadStringFromResource(IDS_SKILL_INFO_RANGER3, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_CLERIC:
		case CLASS_KA_SHAMAN:
			::_LoadStringFromResource(IDS_SKILL_INFO_CLERIC3, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		case CLASS_EL_MAGE:
		case CLASS_KA_SORCERER:
			::_LoadStringFromResource(IDS_SKILL_INFO_MAGE3, szStr);
			sprintf(pszDesc, szStr.c_str());
			break;
		}
		break;
	}

	m_pStr_info->SetString(pszDesc);
	m_pStr_info->Render();
}

void CUISkillTreeDlg::TooltipRenderEnable(__IconItemSkill* spSkill)
{
	if (NULL == spSkill || NULL == spSkill->pSkill) return;

	std::string szStr;
	bool bFound = false;

	char pszDesc[256];
	if (!m_pStr_info->IsVisible())	m_pStr_info->SetVisible(true);
	sprintf(pszDesc, "%s", spSkill->pSkill->szDesc.c_str());	m_pStr_info->SetString(pszDesc);

	if ((spSkill->pSkill->dw1stTableType != 1) && (spSkill->pSkill->dw1stTableType != 2))
	{
		if (!m_pStr_skill_mp->IsVisible())	m_pStr_skill_mp->SetVisible(true);
		if (spSkill->pSkill->iExhaustMSP == 0)
		{
			::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NO_MANA, szStr);
			sprintf(pszDesc, "%s", szStr.c_str());
		}
		else
		{
			::_LoadStringFromResource(IDS_SKILL_TOOLTIP_USE_MANA, szStr);
			sprintf(pszDesc, szStr.c_str(), spSkill->pSkill->iExhaustMSP);
		}
		m_pStr_skill_mp->SetString(pszDesc);
	}

	if (!m_pStr_skill_item0->IsVisible())	m_pStr_skill_item0->SetVisible(true);
	switch (spSkill->pSkill->iNeedSkill)
	{
	case 1055:
	case 2055:
		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_DUAL, szStr);
		sprintf(pszDesc, szStr.c_str());
		bFound = true;
		break;

	case 1056:
	case 2056:
		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_DOUBLE, szStr);
		sprintf(pszDesc, szStr.c_str());
		bFound = true;
		break;
	}

	if (!bFound)
	{
		switch (spSkill->pSkill->dwNeedItem)
		{
		case 0:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID1, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 1:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID2, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 2:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID3, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 3:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID4, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 4:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID5, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 5:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID6, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 6:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID7, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 7:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID8, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 8:		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID9, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 10:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID10, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 11:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID11, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 12:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID12, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 13:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID13, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 21:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID14, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 22:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID15, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 23:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID16, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		case 24:	::_LoadStringFromResource(IDS_SKILL_TOOLTIP_NEED_ITEM_ID17, szStr);	sprintf(pszDesc, szStr.c_str());	break;
		default:	sprintf(pszDesc, "");		break;
		}
	}
	m_pStr_skill_item0->SetString(pszDesc);

	if (!m_pStr_skill_item1->IsVisible())	m_pStr_skill_item1->SetVisible(true);
	if (spSkill->pSkill->dwExhaustItem == 0)
	{
		::_LoadStringFromResource(IDS_SKILL_TOOLTIP_USE_ITEM_NO, szStr);
		sprintf(pszDesc, szStr.c_str());
	}
	else
	{
		__TABLE_ITEM_BASIC* pItem = NULL;
		pItem = CGameBase::s_pTbl_Items_Basic->Find(spSkill->pSkill->dwExhaustItem);
		if (pItem)
		{
			::_LoadStringFromResource(IDS_SKILL_TOOLTIP_USE_ITEM_EXIST, szStr);
			sprintf(pszDesc, szStr.c_str(), pItem->szName.c_str());
		}
		else
		{
			__ASSERT(0, "NULL Item!!!");
		}
	}
	m_pStr_skill_item1->SetString(pszDesc);

	m_pStr_info->Render();
	m_pStr_skill_mp->Render();
	m_pStr_skill_item0->Render();
	m_pStr_skill_item1->Render();
}

void CUISkillTreeDlg::TooltipRenderDisable()
{
	m_pStr_info->SetVisible(false);
	m_pStr_skill_mp->SetVisible(false);
	m_pStr_skill_item0->SetVisible(false);
	m_pStr_skill_item1->SetVisible(false);
}

void CUISkillTreeDlg::InitIconWnd(e_UIWND eWnd)
{
	CN3UIWndBase::InitIconWnd(eWnd);

	m_pStr_info = (CN3UIString*)GetChildByID("string_info");			__ASSERT(m_pStr_info, "NULL UI Component!!");
	m_pStr_skill_mp = (CN3UIString*)GetChildByID("string_skill_mp");		__ASSERT(m_pStr_skill_mp, "NULL UI Component!!");
	m_pStr_skill_item0 = (CN3UIString*)GetChildByID("string_skill_item0");	__ASSERT(m_pStr_skill_item0, "NULL UI Component!!");
	m_pStr_skill_item1 = (CN3UIString*)GetChildByID("string_skill_item1");	__ASSERT(m_pStr_skill_item1, "NULL UI Component!!");
}

void CUISkillTreeDlg::InitIconUpdate()
{
	__TABLE_UPC_SKILL* pUSkill = NULL;

	// ���� ������ ��� Ŭ����..
	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
		for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
			for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
				if (m_pMySkillTree[i][j][k] != NULL)
				{
					__IconItemSkill* spSkill = m_pMySkillTree[i][j][k];

					// �Ŵ������� ����..
					RemoveChild(spSkill->pUIIcon);

					// ���ҽ� ����..
					spSkill->pUIIcon->Release();
					delete spSkill->pUIIcon;
					spSkill->pUIIcon = NULL;
					delete spSkill;
					spSkill = NULL;
					m_pMySkillTree[i][j][k] = NULL;
				}


	// ���̵� = ���� �ڵ�*1000 + 001����.. (���� �ڵ�+1)*100 + 001����..
	int iSkillIDFirst, iSkillIndexFirst, iSkillIndexLast, iModulo;
	iSkillIDFirst = CGameBase::s_pPlayer->m_InfoBase.eClass * 1000 + 1;
	iSkillIndexFirst = CGameBase::s_pTbl_Skill->IDToIndex(iSkillIDFirst);
	iSkillIndexLast = CGameBase::s_pTbl_Skill->GetSize();

	if (iSkillIndexFirst == -1)
	{
		PageButtonInitialize();
		return;		// ù��° ��ų�� ������.. �ȵȴ�..
	}

	if (CGameBase::s_pPlayer->m_InfoBase.eClass != CLASS_EL_DRUID)
	{
		for (int i = iSkillIndexFirst; i < CGameBase::s_pTbl_Skill->GetSize(); i++)
		{
			pUSkill = CGameBase::s_pTbl_Skill->GetIndexedData(i);
			int iDivide = pUSkill->dwID / 1000;
			if (iDivide != (iSkillIDFirst / 1000))
			{
				iSkillIndexLast = i;
				break;
			}
		}
	}

	for (int i = iSkillIndexFirst; i < iSkillIndexLast; i++)
	{
		__TABLE_UPC_SKILL* pUSkill = CGameBase::s_pTbl_Skill->GetIndexedData(i);
		if (pUSkill == NULL) continue;
		if (pUSkill->dwID >= UIITEM_TYPE_SONGPYUN_ID_MIN) continue;

		// ������ ���� �Ǵ��� Ȯ���Ѵ�..
		iModulo = pUSkill->iNeedSkill % 10;
		switch (iModulo)
		{
		case 0:																				// Base Skill.. ���� ���������� �Ǵ��Ѵ�..
			if (pUSkill->iNeedLevel <= CGameBase::s_pPlayer->m_InfoBase.iLevel)		// �� �������� ���ų� ������..
				AddSkillToPage(pUSkill);
			break;

		case 5:																				// ���� Skill.. �������� �ٸ���..
			if (pUSkill->iNeedLevel <= m_iSkillInfo[5])
				AddSkillToPage(pUSkill, 1);
			break;

		case 6:																				// ���� Skill.. �������� �ٸ���..
			if (pUSkill->iNeedLevel <= m_iSkillInfo[6])
				AddSkillToPage(pUSkill, 2);
			break;

		case 7:																				// ���� Skill.. �������� �ٸ���..
			if (pUSkill->iNeedLevel <= m_iSkillInfo[7])
				AddSkillToPage(pUSkill, 3);
			break;

		case 8:																				// ���� Skill.. �������� �ٸ���..
			if (pUSkill->iNeedLevel <= m_iSkillInfo[8])
				AddSkillToPage(pUSkill, 4);
			break;
		}
	}

	PageButtonInitialize();
}

void CUISkillTreeDlg::PageButtonInitialize()
{
	SetPageInIconRegion(0, 0);
	SetPageInCharRegion();

	// �������� ���� ������ ����.. m_iSkillInfo[MAX_SKILL_FROM_SERVER];										// ������ �޴� ���� ����..	
	CN3UIString* pStrName = (CN3UIString*)GetChildByID("string_skillpoint"); __ASSERT(pStrName, "NULL UI Component!!");
	pStrName->SetStringAsInt(m_iSkillInfo[0]);
	pStrName = (CN3UIString*)GetChildByID("string_0"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[1]);
	pStrName = (CN3UIString*)GetChildByID("string_1"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[2]);
	pStrName = (CN3UIString*)GetChildByID("string_2"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[3]);
	pStrName = (CN3UIString*)GetChildByID("string_3"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[4]);
	pStrName = (CN3UIString*)GetChildByID("string_4"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[5]);
	pStrName = (CN3UIString*)GetChildByID("string_5"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[6]);
	pStrName = (CN3UIString*)GetChildByID("string_6"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[7]);
	pStrName = (CN3UIString*)GetChildByID("string_7"); __ASSERT(pStrName, "NULL UI Component!!");
	if (pStrName) pStrName->SetStringAsInt(m_iSkillInfo[8]);

	ButtonVisibleStateSet();
}

void CUISkillTreeDlg::ButtonVisibleStateSet()
{
	// temp macro..
#define ASSET_0 {	\
	__ASSERT(pButton, "NULL UI Component!!");	if (!pButton) return;	pButton->SetVisible(false);	pButton->SetState(UI_STATE_BUTTON_NORMAL); \
}
#define ASSET_1 {	\
	__ASSERT(pButton, "NULL UI Component!!"); if (!pButton) return;	pButton->SetVisible(true);	if ( m_iCurKindOf == 1 )	pButton->SetState(UI_STATE_BUTTON_DOWN);	\
}
#define ASSET_2 {	\
	__ASSERT(pButton, "NULL UI Component!!"); if (!pButton) return;	pButton->SetVisible(true);	if ( m_iCurKindOf == 2 )	pButton->SetState(UI_STATE_BUTTON_DOWN);	\
}
#define ASSET_3 {	\
	__ASSERT(pButton, "NULL UI Component!!"); if (!pButton) return;	pButton->SetVisible(true);	if ( m_iCurKindOf == 3 )	pButton->SetState(UI_STATE_BUTTON_DOWN);	\
}
#define ASSET_4 {	\
	__ASSERT(pButton, "NULL UI Component!!"); if (!pButton) return;	pButton->SetVisible(true);	if ( m_iCurKindOf == 4 )	pButton->SetState(UI_STATE_BUTTON_DOWN);	\
}

	CN3UIButton* pButton;
	pButton = (CN3UIButton*)GetChildByID("btn_public");
	__ASSERT(pButton, "NULL UI Component!!");
	pButton->SetState(UI_STATE_BUTTON_NORMAL);

	// All Button Set Invisible..
	// Elmorad..
	switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
	{
	case NATION_ELMORAD:
		pButton = (CN3UIButton*)GetChildByID("btn_ranger0");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_ranger1");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_ranger2");	ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_ranger3");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_blade0");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_blade1");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_blade2");	ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_blade3");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_mage0");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_mage1");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_mage2");		ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_mage3");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_cleric0");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_cleric1");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_cleric2");	ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_cleric3");	ASSET_0
			break;

		// Karus..
	case NATION_KARUS:
		pButton = (CN3UIButton*)GetChildByID("btn_hunter0");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_hunter1");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_hunter2");		ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_hunter3");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_berserker0");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_berserker1");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_berserker2");	ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_berserker3");	ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_sorcerer0");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_sorcerer1");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_sorcerer2");		ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_sorcerer3");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_shaman0");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_shaman1");		ASSET_0
			pButton = (CN3UIButton*)GetChildByID("btn_shaman2");		ASSET_0
			//pButton = (CN3UIButton* )GetChildByID("btn_shaman3");		ASSET_0	
			break;
	}

	if (m_iCurKindOf == 0)
	{
		pButton = (CN3UIButton*)GetChildByID("btn_public");
		pButton->SetState(UI_STATE_BUTTON_DOWN);
	}

	switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
	{
	case CLASS_KA_BERSERKER:
		pButton = (CN3UIButton*)GetChildByID("btn_berserker0");	ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_berserker1");	ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_berserker2");	ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_berserker3");	ASSET_4
			break;

	case CLASS_KA_HUNTER:
		pButton = (CN3UIButton*)GetChildByID("btn_hunter0");		ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_hunter1");		ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_hunter2");		ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_hunter3");		ASSET_4
			break;

	case CLASS_KA_SHAMAN:
		pButton = (CN3UIButton*)GetChildByID("btn_shaman0");		ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_shaman1");		ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_shaman2");		ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_shaman3");		ASSET_4
			break;

	case CLASS_KA_SORCERER:
		pButton = (CN3UIButton*)GetChildByID("btn_sorcerer0");		ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_sorcerer1");		ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_sorcerer2");		ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_sorcerer3");		ASSET_4
			break;

	case CLASS_EL_BLADE:
		pButton = (CN3UIButton*)GetChildByID("btn_blade0");	ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_blade1");	ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_blade2");	ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_blade3");	ASSET_4
			break;

	case CLASS_EL_RANGER:
		pButton = (CN3UIButton*)GetChildByID("btn_ranger0");	ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_ranger1");	ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_ranger2");	ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_ranger3");	ASSET_4
			break;

	case CLASS_EL_CLERIC:
		pButton = (CN3UIButton*)GetChildByID("btn_cleric0");	ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_cleric1");	ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_cleric2");	ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_cleric3");	ASSET_4
			break;

	case CLASS_EL_MAGE:
		pButton = (CN3UIButton*)GetChildByID("btn_mage0");		ASSET_1
			pButton = (CN3UIButton*)GetChildByID("btn_mage1");		ASSET_2
			pButton = (CN3UIButton*)GetChildByID("btn_mage2");		ASSET_3
			//pButton = (CN3UIButton* )GetChildByID("btn_mage3");		ASSET_4
			break;
	}
}

void CUISkillTreeDlg::AddSkillToPage(__TABLE_UPC_SKILL* pUSkill, int iOffset)
{
	int i, j;
	bool bFound = false;

	// m_pMySkillTree[iOffset]�� ���� ���̵� �ִ��� ���캻��..
	for (i = 0; i < MAX_SKILL_PAGE_NUM; i++)
		for (j = 0; j < MAX_SKILL_IN_PAGE; j++)
		{
			if (m_pMySkillTree[iOffset][i][j] != NULL)
			{
				if (m_pMySkillTree[iOffset][i][j]->pSkill->dwID == pUSkill->dwID)
					goto stop;
			}
		}

	// m_pMySkillTree[iOffset]�� �� �� �ִ��� ���캻��..
	for (i = 0; i < MAX_SKILL_PAGE_NUM; i++)
		for (j = 0; j < MAX_SKILL_IN_PAGE; j++)
		{
			if (m_pMySkillTree[iOffset][i][j] == NULL)
			{
				bFound = true;
				goto stop;
			}
		}

stop:
	if (!bFound) return;

	__IconItemSkill* spSkill = new __IconItemSkill();
	spSkill->pSkill = pUSkill;

	// ������ �̸� �����.. ^^
	char buffer[MAX_PATH]{};
	sprintf(buffer, "UI\\skillicon_%.2d_%d.dxt", pUSkill->dwID % 100, pUSkill->dwID / 100);
	spSkill->szIconFN = buffer;

	// ������ �ε��ϱ�.. ^^
	spSkill->pUIIcon = new CN3UIIcon;
	spSkill->pUIIcon->Init(this);
	spSkill->pUIIcon->SetTex(spSkill->szIconFN);
	spSkill->pUIIcon->SetUVRect(0, 0, 1, 1);
	spSkill->pUIIcon->SetUIType(UI_TYPE_ICON);
	spSkill->pUIIcon->SetStyle(UISTYLE_ICON_SKILL);

	CN3UIArea* pArea = NULL;
	pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_SKILL_TREE, j);
	if (pArea)
	{
		spSkill->pUIIcon->SetRegion(pArea->GetRegion());
		spSkill->pUIIcon->SetMoveRect(pArea->GetRegion());
	}

	// ������ ���� ����..
	m_pMySkillTree[iOffset][i][j] = spSkill;
}

void CUISkillTreeDlg::Open()
{

	// ������ ������!!
	SetVisible(true);
	this->SetPos(CN3Base::s_CameraData.vp.Width, 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = true;
	m_bClosingNow = false;

	m_iRBtnDownOffs = -1;

	MasterIconsButtonsVisibleSateSet();
}

void CUISkillTreeDlg::Close()
{
	// ���ҽ� Free..
	__IconItemSkill* spSkill = NULL;
	spSkill = CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo;
	if (spSkill)
	{
		// �Ŵ������� ����..
		RemoveChild(spSkill->pUIIcon);

		// ���ҽ� ����..
		spSkill->pUIIcon->Release();
		delete spSkill->pUIIcon;
		spSkill->pUIIcon = NULL;
		delete spSkill;
		spSkill = NULL;
	}
	CN3UIWndBase::m_sSkillSelectInfo.pSkillDoneInfo = NULL;
	SetState(UI_STATE_COMMON_NONE);
	CN3UIWndBase::AllHighLightIconFree();

	// ������ ������..!!
//	SetVisible(false); // �� ������ ���� ���ش�..
	RECT rc = this->GetRegion();
	this->SetPos(CN3Base::s_CameraData.vp.Width - (rc.right - rc.left), 10);
	m_fMoveDelta = 0;
	m_bOpenningNow = false;
	m_bClosingNow = true;

	if (m_pSnd_CloseUI) m_pSnd_CloseUI->Play(); // �ݴ� �Ҹ�..

	m_iRBtnDownOffs = -1;
}

__IconItemSkill* CUISkillTreeDlg::GetHighlightIconItem(CN3UIIcon* pUIIcon)
{
	for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
	{
		if ((m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k] != NULL) &&
			(m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k]->pUIIcon == pUIIcon))
			return m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k];
	}

	return NULL;
}

int CUISkillTreeDlg::GetSkilliOrder(__IconItemSkill* spSkill)
{
	for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
	{
		if (m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k] == spSkill)
			return k;
	}

	return -1;
}

RECT CUISkillTreeDlg::GetSampleRect()
{
	RECT rect;
	CN3UIArea* pArea;
	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_SKILL_TREE, 0);
	rect = pArea->GetRegion();
	float fWidth = rect.right - rect.left;
	float fHeight = rect.bottom - rect.top;
	fWidth *= 0.5f; fHeight *= 0.5f;
	rect.left = ptCur.x - (int)fWidth;  rect.right = ptCur.x + (int)fWidth;
	rect.top = ptCur.y - (int)fHeight; rect.bottom = ptCur.y + (int)fHeight;
	return rect;
}

void CUISkillTreeDlg::SetPageInIconRegion(int iKindOf, int iPageNum)		// ������ �������� ���� ������ ����..
{
	if ((iKindOf >= MAX_SKILL_KIND_OF) || (iPageNum >= MAX_SKILL_PAGE_NUM))
		return;

	m_iCurKindOf = iKindOf;
	m_iCurSkillPage = iPageNum;

	for (int i = 0; i < MAX_SKILL_KIND_OF; i++)
	{
		if (i != iKindOf)
		{
			for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
				for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
				{
					if (m_pMySkillTree[i][j][k] != NULL)
						m_pMySkillTree[i][j][k]->pUIIcon->SetVisible(false);
				}
		}
		else
		{
			for (int j = 0; j < MAX_SKILL_PAGE_NUM; j++)
			{
				if (j != iPageNum)
				{
					for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
					{
						if (m_pMySkillTree[i][j][k] != NULL)
							m_pMySkillTree[i][j][k]->pUIIcon->SetVisible(false);
					}
				}
				else
				{
					for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
					{
						if (m_pMySkillTree[i][j][k] != NULL)
							m_pMySkillTree[i][j][k]->pUIIcon->SetVisible(true);
					}
				}
			}
		}
	}

	// ������ ���� ���ڿ� ������Ʈ.. ���� ��ų ������ ���� ��ų �������� �������� ���̸� String���̰�.. �ƴϸ� �Ⱥ��̰�..
	CN3UIString* pStrName;
	std::string str;
	char	cstr[4];

	for (int k = 0; k < MAX_SKILL_IN_PAGE; k++)
	{
		if (m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k] != NULL)
		{
			str = "string_list_";
			sprintf(cstr, "%d", k);	str += cstr;
			pStrName = (CN3UIString*)GetChildByID(str);	 __ASSERT(pStrName, "NULL UI Component!!");
			pStrName->SetString(m_pMySkillTree[m_iCurKindOf][m_iCurSkillPage][k]->pSkill->szName);
			pStrName->SetVisible(true);
		}
		else
		{
			str = "string_list_";
			sprintf(cstr, "%d", k);	str += cstr;
			pStrName = (CN3UIString*)GetChildByID(str);	 __ASSERT(pStrName, "NULL UI Component!!");
			pStrName->SetVisible(false);
		}
	}

	ButtonVisibleStateSet();

	CN3UIString* pStr = (CN3UIString*)GetChildByID("string_page");	__ASSERT(pStr, "NULL UI Component!!");
	sprintf(cstr, "%d", iPageNum + 1);
	if (pStr) pStr->SetString(cstr);
}

void CUISkillTreeDlg::AllClearImageByName(const std::string& szFN, bool bTrueOrNot)
{
	//	CN3UIImage* pImage;
	CN3UIBase* pBase;
	CN3UIButton* pButton;

	std::string str;
	char	cstr[4];

	for (int i = 0; i < 4; i++)
	{
		str = "img_";	str += szFN;	sprintf(cstr, "_%d", i);	str += cstr;
		pBase = GetChildBaseByName(str);
		if (pBase) pBase->SetVisible(bTrueOrNot);
	}

	str = "img_";	str += szFN;
	pBase = GetChildBaseByName(str);
	if (pBase) pBase->SetVisible(bTrueOrNot);

	for (int i = 0; i < 4; i++)
	{
		str = "btn_";	str += szFN;	sprintf(cstr, "%d", i);	str += cstr;
		pButton = GetChildButtonByName(str);
		if (pButton) pButton->SetVisible(bTrueOrNot);
	}
}

void CUISkillTreeDlg::SetPageInCharRegion()						// ���� �������� ���� ������ ����..
{
	AllClearImageByName("public", false);

	switch (CGameBase::s_pPlayer->m_InfoBase.eNation)
	{
	case NATION_KARUS:			// ī�罺..
		AllClearImageByName("hunter", false);
		AllClearImageByName("berserker", false);
		AllClearImageByName("sorcerer", false);
		AllClearImageByName("shaman", false);

		// ����.. 
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_KA_WARRIOR:
		case CLASS_KA_ROGUE:
		case CLASS_KA_WIZARD:
		case CLASS_KA_PRIEST:
			AllClearImageByName("public", true);
			break;

		case CLASS_KA_BERSERKER:
			AllClearImageByName("berserker", true);
			break;

		case CLASS_KA_HUNTER:
			AllClearImageByName("hunter", true);
			break;

		case CLASS_KA_SORCERER:
			AllClearImageByName("sorcerer", true);
			break;

		case CLASS_KA_SHAMAN:
			AllClearImageByName("shaman", true);
			break;
		}
		break;

	case NATION_ELMORAD:		// �����..
		AllClearImageByName("ranger", false);
		AllClearImageByName("blade", false);
		AllClearImageByName("mage", false);
		AllClearImageByName("cleric", false);

		// ����.. 
		switch (CGameBase::s_pPlayer->m_InfoBase.eClass)
		{
		case CLASS_EL_WARRIOR:
		case CLASS_EL_ROGUE:
		case CLASS_EL_WIZARD:
		case CLASS_EL_PRIEST:
			AllClearImageByName("public", true);
			break;

		case CLASS_EL_RANGER:
			AllClearImageByName("ranger", true);
			break;

		case CLASS_EL_BLADE:
			AllClearImageByName("blade", true);
			break;

		case CLASS_EL_MAGE:
			AllClearImageByName("mage", true);
			break;

		case CLASS_EL_CLERIC:
			AllClearImageByName("cleric", true);
			break;
		}
		break;
	}
}

CN3UIImage* CUISkillTreeDlg::GetChildImageByName(const std::string& szFN)
{
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (CN3UIBase*)(*itor);
		if ((pChild->UIType() == UI_TYPE_IMAGE) && (szFN.compare(pChild->m_szID) == 0))
			return (CN3UIImage*)pChild;
	}

	return NULL;
}

CN3UIBase* CUISkillTreeDlg::GetChildBaseByName(const std::string& szFN)
{
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (CN3UIBase*)(*itor);
		if (szFN.compare(pChild->m_szID) == 0)
			return pChild;
	}

	return NULL;
}

CN3UIButton* CUISkillTreeDlg::GetChildButtonByName(const std::string& szFN)
{
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (CN3UIBase*)(*itor);
		if ((pChild->UIType() == UI_TYPE_BUTTON) && (szFN.compare(pChild->m_szID) == 0))
			return (CN3UIButton*)pChild;
	}

	return NULL;
}

//this_ui_add_start
bool CUISkillTreeDlg::OnKeyPress(int iKey)
{
	switch (iKey)
	{
	case DIK_PRIOR:
		PageLeft();
		return true;
	case DIK_NEXT:
		PageRight();
		return true;
	case DIK_ESCAPE:
		if (!m_bClosingNow) this->Close();
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUISkillTreeDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

	if (bReFocus)
	{
		if (bVisible)
			CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
		else
			CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
	}
}

void CUISkillTreeDlg::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if (bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}
//this_ui_add_end