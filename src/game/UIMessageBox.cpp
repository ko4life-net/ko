// UIMessageBox.cpp: implementation of the CUIMessageBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UIMessageBox.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"
#include "GameProcCharacterSelect.h"
#include "UIKnightsOperation.h"
#include "UICreateClanName.h"
#include "UIPartyBBS.h"
#include "GameEng.h"
#include "GameProcedure.h"
#include "GameProcLogin.h"
#include "LocalInput.h"
#include "UIMessageBoxManager.h"
#include "SubProcPerTrade.h"
#include "ShellApi.h"

#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIEdit.h"
#include "N3Base/N3Shape.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIMessageBox::CUIMessageBox() {
    m_pBtn_OK = NULL;
    m_pBtn_Yes = NULL;
    m_pBtn_No = NULL;
    m_pBtn_Cancel = NULL;
    m_pText_Message = NULL;
    m_pText_Title = NULL;
    m_pEdit_Common = NULL;

    m_eBehavior = BEHAVIOR_NOTHING;
    m_iStyle = MB_OK;
}

CUIMessageBox::~CUIMessageBox() {}

bool CUIMessageBox::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pBtn_OK = (CN3UIButton *)GetChildByID("Btn_OK");
    __ASSERT(m_pBtn_OK, "NULL UI Component!!");
    m_pBtn_Yes = (CN3UIButton *)GetChildByID("Btn_Yes");
    __ASSERT(m_pBtn_Yes, "NULL UI Component!!");
    m_pBtn_No = (CN3UIButton *)GetChildByID("Btn_No");
    __ASSERT(m_pBtn_No, "NULL UI Component!!");
    m_pBtn_Cancel = (CN3UIButton *)GetChildByID("Btn_Cancel");
    __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
    m_pText_Message = (CN3UIString *)GetChildByID("Text_Message");
    __ASSERT(m_pText_Message, "NULL UI Component!!");
    m_pText_Title = (CN3UIString *)GetChildByID("Text_Title");
    __ASSERT(m_pText_Title, "NULL UI Component!!");
    m_pEdit_Common = (CN3UIEdit *)GetChildByID("Edit_Common");
    __ASSERT(m_pEdit_Common, "NULL UI Component!!");

    return true;
}

void CUIMessageBox::SetText(const std::string & szMsg) {
    m_pText_Message->SetString(szMsg);
}

void CUIMessageBox::SetTitle(const std::string & szTitle) {
    m_pText_Title->SetString(szTitle);
}

bool CUIMessageBox::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    //s_CameraData.vp;  //�ҷ� ���� ������ ���캻��
    //DWORD mm = s_CameraData.vp.Height;
    //DWORD ss = s_CameraData.vp.Width;

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        CPlayerMySelf *            pPlayer = CGameBase::s_pPlayer;
        CGameProcMain *            pProcMain = CGameProcedure::s_pProcMain;
        CGameProcLogIn *           pProcLogIn = CGameProcedure::s_pProcLogIn;
        CGameProcCharacterSelect * pProcChrSelect = CGameProcedure::s_pProcCharacterSelect;
        CN3Shape *                 pShape = CGameBase::s_pPlayer->m_pObjectTarget;

        if (pSender == m_pBtn_OK || pSender == m_pBtn_Yes) {
            //this_ui
            if (m_pParentUI) {
                m_pParentUI->CallBackProc(m_iChildID, 1);
            }

            switch (m_eBehavior) {
            case BEHAVIOR_EXIT: {
                if (CGameProcedure::s_pProcActive == pProcMain) // ���� ���� ���ν����̸�..
                {
                    pProcMain->ReleaseSound();
                    ::PostQuitMessage(0);
                } else {
                    ::PostQuitMessage(0);
                }
            } break; // ������..
            case BEHAVIOR_RESTART_GAME: {
                if (CGameProcedure::s_pProcActive == pProcMain) // ���� ���� ���ν����̸�..
                {
                    std::string szIP = CGameProcedure::s_pSocket->GetCurrentIP();
                    DWORD       dwPort = CGameProcedure::s_pSocket->GetCurrentPort();

                    CGameProcedure::s_bNeedReportConnectionClosed = false; // ���������� �������� �����ؾ� �ϴ���..
                    CGameProcedure::s_pSocket->Disconnect();
                    Sleep(2000); // 2�� ������.. ������ ó���� �ð��� �ش�.
                    CGameProcedure::s_pSocket->Connect(CGameProcedure::s_hWndBase, szIP.c_str(), dwPort);
                    CGameProcedure::s_bNeedReportConnectionClosed = true; // ���������� �������� �����ؾ� �ϴ���..
                    CGameProcedure::s_bNeedReportVersionCheck = true;
                    CGameProcedure::ProcActiveSet(
                        (CGameProcedure *)CGameProcedure::s_pProcCharacterSelect); // �ٽ� ĳ���� ����..
                }
            } break;
            case BEHAVIOR_REGENERATION:
                pProcMain->MsgSend_Regen();
                break; // ��Ȱ �޽��� ������..
            case BEHAVIOR_PARTY_PERMIT:
                pProcMain->MsgSend_PartyOrForcePermit(0, true);
                break;
            case BEHAVIOR_PARTY_DISBAND:
                pProcMain->MsgSend_PartyOrForceLeave(0);
                break;
            case BEHAVIOR_FORCE_PERMIT:
                pProcMain->MsgSend_PartyOrForcePermit(1, true);
                break;
            case BEHAVIOR_FORCE_DISBAND:
                pProcMain->MsgSend_PartyOrForceLeave(1);
                break;
            case BEHAVIOR_REQUEST_BINDPOINT:
                if (pShape) {
                    pProcMain->MsgSend_ObjectEvent(pShape->m_iEventID, 0);
                }
                break;
            case BEHAVIOR_KNIGHTS_CREATE:
                pProcMain->m_pUICreateClanName->MsgSend_MakeClan();
                break;
            case BEHAVIOR_KNIGHTS_DESTROY:
                pProcMain->m_pUIKnightsOp->MsgSend_KnightsDestroy();
                break; // ���� �ǰ���..
            case BEHAVIOR_KNIGHTS_WITHDRAW:
                pProcMain->m_pUIKnightsOp->MsgSend_KnightsWithdraw();
                break; // ���� Ż���ϱ�..
            case BEHAVIOR_PERSONAL_TRADE_PERMIT:
                pProcMain->m_pSubProcPerTrade->ProcessProceed(PER_TRADE_RESULT_MY_AGREE);
                break; // ���� ���..
            case BEHAVIOR_TRADE_DECISION_DONE:
                pProcMain->m_pSubProcPerTrade->PerTradeMyDecision();
                break;
            case BEHAVIOR_MGAME_LOGIN:
                pProcLogIn->MsgSend_AccountLogIn(LIC_MGAME);
                break;
            case BEHAVIOR_DELETE_CHR: // ĳ���� �����..
            {
                std::string szKey;
                if (m_pEdit_Common) {
                    szKey = m_pEdit_Common->GetString();
                }
                pProcChrSelect->MsgSend_DeleteChr(szKey);
            } break;
            case BEHAVIOR_CLAN_JOIN:
                pProcMain->MsgSend_KnightsJoinReq(true);
                break;
            case BEHAVIOR_PARTY_BBS_REGISTER:
                if (pProcMain->m_pUIPartyBBS) {
                    pProcMain->m_pUIPartyBBS->MsgSend_Register();
                }
                break; // ��Ƽ �Խ��ǿ� ���
            case BEHAVIOR_PARTY_BBS_REGISTER_CANCEL:
                if (pProcMain->m_pUIPartyBBS) {
                    pProcMain->m_pUIPartyBBS->MsgSend_RegisterCancel();
                }
                break; // ��Ƽ �Խ��ǿ� ��� ����
            case BEHAVIOR_EXECUTE_OPTION: {
                ::ShellExecute(NULL, "open", "Option.exe", NULL, NULL, SW_SHOWNORMAL); // Ȩ�������� �̵�..
                PostQuitMessage(0);                                                    // ����...
            } break;
            default:
                break;
            }
        } else if (pSender == m_pBtn_No || pSender == m_pBtn_Cancel) {
            //this_ui
            if (m_pParentUI) {
                m_pParentUI->CallBackProc(m_iChildID, 2);
            }

            this->SetVisible(false);
            this->SetText("");
            this->SetTitle("");

            switch (m_eBehavior) {
            case BEHAVIOR_PARTY_PERMIT:
                pProcMain->MsgSend_PartyOrForcePermit(0, false);
                break;
            case BEHAVIOR_FORCE_PERMIT:
                pProcMain->MsgSend_PartyOrForcePermit(1, false);
                break;
            case BEHAVIOR_PERSONAL_TRADE_PERMIT:
                pProcMain->m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
                break; // ���� ����..
            case BEHAVIOR_PERSONAL_TRADE_FMT_WAIT:
                pProcMain->m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_CANCEL);
                break; // ���� ���..
            case BEHAVIOR_TRADE_DECISION_DONE:
                pProcMain->m_pSubProcPerTrade->ResetMyTradeButtonState();
                break;
            case BEHAVIOR_CLAN_JOIN:
                pProcMain->MsgSend_KnightsJoinReq(false);
                break;
            }
        }

        this->SetVisible(false);
        this->SetText("");
        this->SetTitle("");
        this->SetVisibleEditControl(false);
        m_eBehavior = BEHAVIOR_NOTHING;
        pSender->SetState(UI_STATE_BUTTON_NORMAL);
    }

    return true;
}

void CUIMessageBox::SetBoxStyle(int iStyle) {
    if (m_pBtn_OK) {
        m_pBtn_OK->SetVisible(false);
    }
    if (m_pBtn_Yes) {
        m_pBtn_Yes->SetVisible(false);
    }
    if (m_pBtn_No) {
        m_pBtn_No->SetVisible(false);
    }
    if (m_pBtn_Cancel) {
        m_pBtn_Cancel->SetVisible(false);
    }

    m_iStyle = iStyle;

    if (MB_YESNO == iStyle) {
        if (m_pBtn_Yes) {
            m_pBtn_Yes->SetVisible(true);
        }
        if (m_pBtn_No) {
            m_pBtn_No->SetVisible(true);
        }
    } else if (MB_CANCEL == iStyle) {
        if (m_pBtn_Cancel) {
            m_pBtn_Cancel->SetVisible(true);
        }
    } else if (MB_OK == iStyle) {
        if (m_pBtn_OK) {
            m_pBtn_OK->SetVisible(true);
        }
    } else {
        __ASSERT(0, "Not Supported MessageBox Type!!");
        if (m_pBtn_OK) {
            m_pBtn_OK->SetVisible(true);
        }
    }
}

void CUIMessageBox::SetVisibleEditControl(bool bVisible) // Edit Control Visible
{
    if (NULL == m_pEdit_Common) {
        return;
    }

    m_pEdit_Common->SetString("");
    m_pEdit_Common->SetVisible(bVisible);
    if (bVisible) {
        m_pEdit_Common->SetFocus();
    } else {
        m_pEdit_Common->KillFocus();
    }
}

void CUIMessageBox::SetVisible(bool bVisible) {
    CN3UIBase::SetVisible(bVisible);

    if (!bVisible) {
        if (CGameProcedure::s_pMsgBoxMgr) {
            CGameProcedure::s_pMsgBoxMgr->ReFocusMsgBox();
        }
    }
}

bool CUIMessageBox::OnKeyPress(int iKey) {
    if (!IsVisible()) {
        return CN3UIBase::OnKeyPress(iKey);
    }

    if (MB_YESNO == m_iStyle) {
        switch (iKey) {
        case DIK_ESCAPE:
            ReceiveMessage(m_pBtn_No, UIMSG_BUTTON_CLICK);
            return true;
        case DIK_RETURN:
            ReceiveMessage(m_pBtn_Yes, UIMSG_BUTTON_CLICK);
            return true;
        }
    } else if (MB_CANCEL == m_iStyle) {
        switch (iKey) {
        case DIK_ESCAPE:
        case DIK_RETURN:
            ReceiveMessage(m_pBtn_Cancel, UIMSG_BUTTON_CLICK);
            return true;
        }
    } else if (MB_OK == m_iStyle) {
        switch (iKey) {
        case DIK_ESCAPE:
        case DIK_RETURN:
            ReceiveMessage(m_pBtn_OK, UIMSG_BUTTON_CLICK);
            return true;
        }
    }

    return CN3UIBase::OnKeyPress(iKey);
}
