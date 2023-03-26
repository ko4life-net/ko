// UIClassChange.cpp: implementation of the CUIClassChange class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UIClassChange.h"

#include "PacketDef.h"

#include "GameProcedure.h"
#include "PlayerMySelf.h"

#include "GameProcMain.h"
#include "UISkillTreeDlg.h"

#include "APISocket.h"
#include "UIVarious.h"

#include "UIHotkeyDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIClassChange::CUIClassChange() {
    m_pBtn_Ok = NULL;
    m_pBtn_Cancel = NULL;
    m_pBtn_Class0 = NULL;
    m_pBtn_Class1 = NULL;

    m_pText_Warning = NULL;
    m_pText_Info0 = NULL;
    m_pText_Info1 = NULL;
    m_pText_Title = NULL;
    m_pText_Message = NULL;

    m_eClassChangeState = UISTATE_NORMAL;
}

CUIClassChange::~CUIClassChange() {}

void CUIClassChange::Release() {
    CN3UIBase::Release();
}

bool CUIClassChange::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_Ok = (CN3UIButton *)GetChildByID("Btn_Ok");
    __ASSERT(m_pBtn_Ok, "NULL UI Component!!");
    m_pBtn_Cancel = (CN3UIButton *)GetChildByID("Btn_Cancel");
    __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
    m_pBtn_Class0        = (CN3UIButton*)GetChildByID("Btn_Class0");        __ASSERT(m_pBtn_Class0,    "NULL UI Component!!");
    m_pBtn_Class1        = (CN3UIButton*)GetChildByID("Btn_Class1");        __ASSERT(m_pBtn_Class1,    "NULL UI Component!!");

    m_pText_Warning = (CN3UIString *)GetChildByID("Text_Waring");
    __ASSERT(m_pText_Warning, "NULL UI Component!!");
    m_pText_Info0        = (CN3UIString*)GetChildByID("Text_info0");        __ASSERT(m_pText_Info0, "NULL UI Component!!");
    m_pText_Info1        = (CN3UIString*)GetChildByID("Text_info1");        __ASSERT(m_pText_Info1, "NULL UI Component!!");
    m_pText_Message = (CN3UIString *)GetChildByID("Text_Message");
    __ASSERT(m_pText_Message, "NULL UI Component!!");

    return true;
}

void CUIClassChange::Open(int iCode) {
    m_eClassChangeState = UISTATE_NORMAL;

    SetVisible(true);

    __InfoPlayerBase *   pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
    __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

    std::string szSuccess;
    ::_LoadStringFromResource(IDS_CLASS_CHANGE_SUCCESS, szSuccess);
    std::string szNotYet;
    ::_LoadStringFromResource(IDS_CLASS_CHANGE_NOT_YET, szNotYet);
    std::string szAlready;
    ::_LoadStringFromResource(IDS_CLASS_CHANGE_ALREADY, szAlready);
    std::string szItemInSlot;
    ::_LoadStringFromResource(IDS_MSG_HASITEMINSLOT, szItemInSlot);

    m_pBtn_Ok->SetVisible(false);
    m_pBtn_Cancel->SetVisible(false);
    m_pBtn_Class0->SetVisible(false);
    m_pBtn_Class1->SetVisible(false);

    m_pText_Warning->SetVisible(false);
    m_pText_Info0->SetVisible(false);
    m_pText_Info1->SetVisible(false);
    m_pText_Message->SetVisible(true);

    std::string szClassTmp;

    switch (iCode) {
    case N3_SP_CLASS_CHANGE_SUCCESS:
        m_pText_Message->SetString(szSuccess);
        m_pBtn_Class0->SetVisible(true);
        m_pBtn_Class1->SetVisible(true);
        m_pBtn_Cancel->SetVisible(true);

        m_pText_Info0->SetVisible(true);
        m_pText_Info1->SetVisible(true);

        m_eClass = pInfoBase->eClass;
        switch (pInfoBase->eClass) {
        case CLASS_KA_WARRIOR:
            CGameProcedure::GetTextByClass(CLASS_KA_BERSERKER, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_KA_GUARDIAN, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_KA_ROGUE:
            CGameProcedure::GetTextByClass(CLASS_KA_HUNTER, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_KA_PENETRATOR, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_KA_WIZARD:
            CGameProcedure::GetTextByClass(CLASS_KA_SORCERER, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_KA_NECROMANCER, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_KA_PRIEST:
            CGameProcedure::GetTextByClass(CLASS_KA_SHAMAN, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_KA_DARKPRIEST, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_EL_WARRIOR:
            CGameProcedure::GetTextByClass(CLASS_EL_BLADE, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_EL_PROTECTOR, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_EL_ROGUE:
            CGameProcedure::GetTextByClass(CLASS_EL_RANGER, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_EL_ASSASIN, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_EL_WIZARD:
            CGameProcedure::GetTextByClass(CLASS_EL_MAGE, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_EL_ENCHANTER, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        case CLASS_EL_PRIEST:
            CGameProcedure::GetTextByClass(CLASS_EL_CLERIC, szClassTmp);
            m_pText_Info0->SetString(szClassTmp);
            CGameProcedure::GetTextByClass(CLASS_EL_DRUID, szClassTmp);
            m_pText_Info1->SetString(szClassTmp);
            break;
        }
        break;

    case N3_SP_CLASS_CHANGE_NOT_YET:
        m_pText_Message->SetString(szNotYet);
        m_pBtn_Ok->SetVisible(true);
        break;

    case N3_SP_CLASS_CHANGE_ALREADY:
        m_pText_Message->SetString(szAlready);
        m_pBtn_Ok->SetVisible(true);
        break;

    case IDS_MSG_HASITEMINSLOT:
        m_pText_Message->SetString(szItemInSlot);
        m_pBtn_Ok->SetVisible(true);
        break;
    }
}

bool CUIClassChange::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    __InfoPlayerBase *   pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
    __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_Ok) {
            if (m_eClassChangeState == UISTATE_NORMAL) {
                Close();
            } else {
                ChangeToNormalState();
            }
        }

        else if (pSender == m_pBtn_Cancel) {
            Close();
        }

        else if (pSender == m_pBtn_Class0) {
            switch (pInfoBase->eClass) {
            case CLASS_KA_WARRIOR:
                pInfoBase->eClass = CLASS_KA_BERSERKER;
                break;
            case CLASS_KA_ROGUE:
                pInfoBase->eClass = CLASS_KA_HUNTER;
                break;
            case CLASS_KA_WIZARD:
                pInfoBase->eClass = CLASS_KA_SORCERER;
                break;
            case CLASS_KA_PRIEST:
                pInfoBase->eClass = CLASS_KA_SHAMAN;
                break;
            case CLASS_EL_WARRIOR:
                pInfoBase->eClass = CLASS_EL_BLADE;
                break;
            case CLASS_EL_ROGUE:
                pInfoBase->eClass = CLASS_EL_RANGER;
                break;
            case CLASS_EL_WIZARD:
                pInfoBase->eClass = CLASS_EL_MAGE;
                break;
            case CLASS_EL_PRIEST:
                pInfoBase->eClass = CLASS_EL_CLERIC;
                break;
            }

            CGameProcedure::s_pProcMain->m_pUIVar->UpdateAllStates(pInfoBase, pInfoExt); // ����â ��ġ�� ��� ����

            BYTE byBuff[4];
            int  iOffset = 0;
            CAPISocket::MP_AddByte(byBuff, iOffset, N3_CLASS_CHANGE);
            CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_CLASS_CHANGE_REQ);
            CAPISocket::MP_AddShort(byBuff, iOffset, (short)pInfoBase->eClass);
            CGameProcedure::s_pSocket->Send(byBuff, iOffset);

            CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->InitIconUpdate();

            // �����ϴ� ����..  ��Ű ������ ��� ���ش�..
            CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->ClassChangeHotkeyFlush();
            Close();
        }

        else if (pSender == m_pBtn_Class1) {
            ChangeToWarningState();
        }
    }

    return true;
}

void CUIClassChange::Close() {
    m_eClassChangeState = UISTATE_NORMAL;

    SetVisible(false);
}

void CUIClassChange::RestorePrevClass() {
    __InfoPlayerBase *   pInfoBase = &(CGameBase::s_pPlayer->m_InfoBase);
    __InfoPlayerMySelf * pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

    pInfoBase->eClass = m_eClass;
    CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->InitIconUpdate();

    CGameProcedure::s_pProcMain->m_pUIVar->UpdateAllStates(pInfoBase, pInfoExt); // ����â ��ġ�� ��� ����
}

void CUIClassChange::ChangeToWarningState() {
    m_eClassChangeState = UISTATE_WARNING;

    m_pBtn_Ok->SetVisible(true);
    m_pBtn_Cancel->SetVisible(false);
    m_pBtn_Class0->SetVisible(false);
    m_pBtn_Class1->SetVisible(false);

    m_pText_Warning->SetVisible(true);
    m_pText_Info0->SetVisible(false);
    m_pText_Info1->SetVisible(false);
    m_pText_Message->SetVisible(false);
}

void CUIClassChange::ChangeToNormalState() {
    m_eClassChangeState = UISTATE_NORMAL;

    m_pBtn_Ok->SetVisible(false);
    m_pBtn_Cancel->SetVisible(true);
    m_pBtn_Class0->SetVisible(true);
    m_pBtn_Class1->SetVisible(true);

    m_pText_Warning->SetVisible(false);
    m_pText_Info0->SetVisible(true);
    m_pText_Info1->SetVisible(true);
    m_pText_Message->SetVisible(true);
}

//this_ui_add_start
void CUIClassChange::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus) {
    CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

    m_eClassChangeState = UISTATE_NORMAL;
}
//this_ui_add_end
