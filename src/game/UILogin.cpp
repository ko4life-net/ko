// UILogIn.cpp: implementation of the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "UILogIn.h"
#include "GameProcLogIn.h"

#include "N3Base/N3UIEdit.h"
#include "N3Base/N3UIButton.h"
#include "N3Base/N3UIList.h"
#include "UIMessageBoxManager.h"

#include "Ini.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUILogIn::CUILogIn() {
    m_pEdit_id = NULL;
    m_pEdit_pw = NULL;

    m_pBtn_LogIn = NULL;
    m_pBtn_Connect = NULL;
    m_pBtn_Cancel = NULL;
    m_pBtn_Option = NULL;
    m_pBtn_Join = NULL;

    m_pGroup_ServerList = NULL;
    m_pGroup_LogIn = NULL;

    m_pText_Rights = NULL;
    m_pImg_MGameLogo = NULL;
    m_pImg_DaumLogo = NULL;

    m_pList_Server = NULL;

    m_bOpenningNow = false; // 위에서 아래로 스르륵...열려야 한다면..
    m_fMoveDelta = 0;

    m_bLogIn = false;
}

CUILogIn::~CUILogIn() {}

bool CUILogIn::ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg) {
    if (NULL == pSender) {
        return false;
    }

    //s_CameraData.vp;  //불러 오는 과정을 살펴본다
    //DWORD mm = s_CameraData.vp.Height;
    //DWORD ss = s_CameraData.vp.Width;

    if (dwMsg == UIMSG_BUTTON_CLICK) {
        if (pSender == m_pBtn_LogIn && m_pEdit_id && m_pEdit_pw) {
            CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
        } else if (pSender == m_pBtn_Connect) {
            CGameProcedure::s_pProcLogIn->ConnectToGameServer(); // 고른 게임 서버에 접속
        } else if (pSender == m_pBtn_Cancel) {
            PostQuitMessage(0);              // 종료...
        } else if (pSender == m_pBtn_Option) // 옵션..
        {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_CONFIRM_EXECUTE_OPTION, szMsg);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_EXECUTE_OPTION);
        } else if (pSender == m_pBtn_Join) {
            CIni        ini(CN3Base::PathGet() / "Server.ini", false);
            std::string szSite = ini.GetString("Join", "Registration site", "https://github.com/ko4life-net/ko");
            ShellExecute(NULL, "open", szSite.c_str(), NULL, NULL, SW_NORMAL);
            PostQuitMessage(0);
            return true;
        }
    } else if (UIMSG_LIST_DBLCLK == dwMsg) {
        CGameProcedure::s_pProcLogIn->ConnectToGameServer(); // 고른 게임 서버에 접속
    } else if (dwMsg == UIMSG_EDIT_RETURN) {
        if (!m_bLogIn && m_pEdit_id && m_pEdit_pw) {
            CN3UIBase * pMsgBox = CGameProcedure::s_pMsgBoxMgr->GetFocusMsgBox();
            if (!(pMsgBox && pMsgBox->IsVisible())) {
                CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
            }
        }
    }

    return true;
}

bool CUILogIn::Load(HANDLE hFile) {
    if (CN3UIBase::Load(hFile) == false) {
        return false;
    }

    m_pGroup_LogIn = GetChildByID("Group_LogIn");
    __ASSERT(m_pGroup_LogIn, "NULL UI Component!!");
    if (m_pGroup_LogIn) {
        m_pBtn_LogIn = (CN3UIButton *)m_pGroup_LogIn->GetChildByID("Btn_Login");
        __ASSERT(m_pBtn_LogIn, "NULL UI Component!!");
        m_pBtn_Cancel = (CN3UIButton *)m_pGroup_LogIn->GetChildByID("Btn_Cancel");
        __ASSERT(m_pBtn_Cancel, "NULL UI Component!!");
        m_pBtn_Option = (CN3UIButton *)m_pGroup_LogIn->GetChildByID("Btn_Option");
        __ASSERT(m_pBtn_Option, "NULL UI Component!!");
        m_pBtn_Join = (CN3UIButton *)m_pGroup_LogIn->GetChildByID("Btn_Join");
        __ASSERT(m_pBtn_Option, "NULL UI Component!!");

        m_pEdit_id = (CN3UIEdit *)m_pGroup_LogIn->GetChildByID("Edit_ID");
        __ASSERT(m_pEdit_id, "NULL UI Component!!");
        m_pEdit_pw = (CN3UIEdit *)m_pGroup_LogIn->GetChildByID("Edit_PW");
        __ASSERT(m_pEdit_pw, "NULL UI Component!!");
    }

    // TODO: This needs to be implemented. Hidding group notices for now.
    const int   GROUP_NOTICE_COUNT = 3;
    std::string szGroupNotice = "Group_Notice_";
    for (size_t i = 0; i < GROUP_NOTICE_COUNT; ++i) {
        CN3UIBase * pGroupNotice = GetChildByID(szGroupNotice + std::to_string(i + 1));
        if (pGroupNotice) {
            pGroupNotice->SetVisible(false);
        }
    }

    m_pText_Rights = GetChildByID("Text_Rights");
    __ASSERT(m_pText_Rights, "NULL UI Component!!");
    m_pImg_MGameLogo = GetChildByID("Img_MGame");
    __ASSERT(m_pImg_MGameLogo, "NULL UI Component!!");
    m_pImg_DaumLogo = GetChildByID("Img_Daum");
    __ASSERT(m_pImg_DaumLogo, "NULL UI Component!!");

    // TODO: implement
    auto pPremiumNotice = GetChildByID("premium");
    if (pPremiumNotice) {
        pPremiumNotice->SetVisible(false);
    }

    if (m_pText_Rights) {
        m_pText_Rights->SetVisible(false);
    }
    if (m_pImg_MGameLogo) {
        m_pImg_MGameLogo->SetVisible(false);
    }
    if (m_pImg_DaumLogo) {
        m_pImg_DaumLogo->SetVisible(false);
    }

    m_pGroup_ServerList = GetChildByID("Group_ServerList");
    __ASSERT(m_pGroup_ServerList, "NULL UI Component!!");
    if (m_pGroup_ServerList) {
        m_pList_Server = (CN3UIList *)(m_pGroup_ServerList->GetChildByID("List_Server"));
        __ASSERT(m_pList_Server, "NULL UI Component!!");
        m_pBtn_Connect = (CN3UIButton *)m_pGroup_ServerList->GetChildByID("Btn_Connect");
        __ASSERT(m_pBtn_Connect, "NULL UI Component!!");

        // TODO: There is something else happening here in the v1264 client.
        // if ( m_pList_Server ) sub_409CC0(m_pList_Server, 3);
        // Quick look; I believe 3 is the child element count to calculate positioning of UI elements.
        m_pGroup_ServerList->SetVisible(false);
    }

    // Note that the 1264 loads more UI elements that are not really used / exists in the game assets,
    // such as Text_RemainDays, Text_RemainPoints, Btn_Exit

    return true;
}

void CUILogIn::AccountIDGet(std::string & szID) {
    if (m_pEdit_id) {
        szID = m_pEdit_id->GetString();
    } else {
        szID = "";
    }
}

void CUILogIn::AccountPWGet(std::string & szPW) {
    if (m_pEdit_pw) {
        szPW = m_pEdit_pw->GetString();
    } else {
        szPW = "";
    }
}

void CUILogIn::ConnectButtonSetEnable(bool bEnable) {
    eUI_STATE eState1 = (bEnable ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
    eUI_STATE eState2 = (bEnable ? UI_STATE_LIST_ENABLE : UI_STATE_LIST_DISABLE);

    if (m_pBtn_Connect) {
        m_pBtn_Connect->SetState(eState1);
    }
    if (m_pList_Server) {
        m_pList_Server->SetState(eState2);
    }
}

void CUILogIn::FocusToID() {
    if (m_pEdit_id) {
        m_pEdit_id->SetFocus();
    }
}

void CUILogIn::FocusCircular() {
    if (NULL == m_pEdit_id || NULL == m_pEdit_pw) {
        return;
    }

    if (m_pEdit_id->HaveFocus()) {
        m_pEdit_pw->SetFocus();
    } else {
        m_pEdit_id->SetFocus();
    }
}

void CUILogIn::InitEditControls() {
    if (m_pEdit_id) {
        m_pEdit_id->SetString("");
        m_pEdit_id->SetFocus();
    }

    if (m_pEdit_pw) {
        m_pEdit_pw->SetString("");
    }
}

bool CUILogIn::ServerInfoAdd(const __GameServerInfo & GSI) {
    m_ListServerInfos.push_back(GSI);
    return true;
}

bool CUILogIn::ServerInfoGet(int iIndex, __GameServerInfo & GSI) {
    if (NULL == m_pList_Server) {
        return false;
    }
    if (iIndex < 0 || iIndex >= m_ListServerInfos.size()) {
        return false;
    }

    GSI = m_ListServerInfos[iIndex];
    return true;
}

bool CUILogIn::ServerInfoGetCur(__GameServerInfo & GSI) {
    GSI.Init();
    if (NULL == m_pList_Server) {
        return false;
    }

    int iIndex = m_pList_Server->GetCurSel();
    return this->ServerInfoGet(iIndex, GSI);
}

void CUILogIn::ServerInfoUpdate() {
    if (NULL == m_pList_Server) {
        return;
    }

    m_pList_Server->ResetContent();
    if (!m_ListServerInfos.empty()) {
        sort(m_ListServerInfos.begin(), m_ListServerInfos.end(),
             [](const __GameServerInfo & lhs, const __GameServerInfo & rhs) {
                 return lhs.iConcurrentUserCount <= rhs.iConcurrentUserCount;
             });

        int iSize = m_ListServerInfos.size();
        for (int i = 0; i < iSize; i++) {
            m_pList_Server->AddString(m_ListServerInfos[i].szName);
        }
    }
}

void CUILogIn::Tick() {
    CN3UIBase::Tick();

    if (m_pGroup_ServerList) {
        if (m_bOpenningNow) // 위에서 아래로 스르륵...열려야 한다면..
        {
            POINT ptCur = m_pGroup_ServerList->GetPos();
            RECT  rc = m_pGroup_ServerList->GetRegion();
            float fHeight = (float)(rc.bottom - rc.top);

            float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
            fDelta *= (fHeight - m_fMoveDelta) / fHeight;
            if (fDelta < 2.0f) {
                fDelta = 2.0f;
            }
            m_fMoveDelta += fDelta;

            int iYLimit = 0;
            ptCur.y = (int)(m_fMoveDelta - fHeight);
            if (ptCur.y >= iYLimit) // 다열렸다!!
            {
                ptCur.y = iYLimit;
                m_bOpenningNow = false;
            }

            m_pGroup_ServerList->SetPos(ptCur.x, ptCur.y);
        }
    }
}

void CUILogIn::OpenServerList() {
    if (m_bOpenningNow || NULL == m_pGroup_ServerList) {
        return;
    }

    // 스르륵 열린다!!
    m_pGroup_ServerList->SetVisible(true);
    RECT rc = m_pGroup_ServerList->GetRegion();
    m_pGroup_ServerList->SetPos(0, -(rc.bottom - rc.top));

    m_fMoveDelta = 0;
    m_bOpenningNow = true;
}

void CUILogIn::SetVisibleLogInUIs(bool bEnable) {
    if (m_pGroup_LogIn) {
        m_pGroup_LogIn->SetVisible(bEnable); // 로그인을 숨긴다..
    }

    // 로그인한 계정의 구분에 따라 UI 만지기...
    if (m_pText_Rights) {
        m_pText_Rights->SetVisible(false);
    }
    if (m_pImg_MGameLogo) {
        m_pImg_MGameLogo->SetVisible(false);
    }
    if (m_pImg_DaumLogo) {
        m_pImg_DaumLogo->SetVisible(false);
    }

    if (false == bEnable) {
        if (LIC_MGAME == CGameProcedure::s_eLogInClassification) {
            if (m_pText_Rights && m_pImg_MGameLogo) {
                // 아래쪽 중단으로 맞춘다..
                RECT rcView = {0, 0, s_CameraData.vp.Width, s_CameraData.vp.Height};
                int  iX = (rcView.right - (m_pText_Rights->GetWidth() + m_pImg_MGameLogo->GetWidth())) / 2;
                int  iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
                m_pText_Rights->SetPos(iX, iY);
                RECT rc = m_pText_Rights->GetRegion();
                m_pImg_MGameLogo->SetPos(rc.right, rcView.bottom - m_pImg_MGameLogo->GetHeight() - 20);

                m_pText_Rights->SetVisible(true);
                m_pImg_MGameLogo->SetVisible(true);
            }
        } else if (LIC_DAUM == CGameProcedure::s_eLogInClassification) {
            if (m_pText_Rights && m_pImg_DaumLogo) {
                // 아래쪽 중단으로 맞춘다..
                RECT rcView = {0, 0, s_CameraData.vp.Width, s_CameraData.vp.Height};
                int  iX = (rcView.right - (m_pText_Rights->GetWidth() + m_pImg_DaumLogo->GetWidth())) / 2;
                int  iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
                m_pText_Rights->SetPos(iX, iY);
                RECT rc = m_pText_Rights->GetRegion();
                m_pImg_DaumLogo->SetPos(rc.right, rcView.bottom - m_pImg_DaumLogo->GetHeight() - 20);

                m_pText_Rights->SetVisible(true);
                m_pImg_DaumLogo->SetVisible(true);
            }
        } else {
            if (m_pText_Rights) {
                RECT rcView = {0, 0, s_CameraData.vp.Width, s_CameraData.vp.Height};
                int  iX = (rcView.right - m_pText_Rights->GetWidth()) / 2;
                int  iY = rcView.bottom - m_pText_Rights->GetHeight() - 20;
                m_pText_Rights->SetPos(iX, iY);

                m_pText_Rights->SetVisible(true);
            }
        }
    }
}

bool CUILogIn::OnKeyPress(int iKey) {
    if (!m_bLogIn) {
        switch (iKey) {
        case DIK_TAB:
            FocusCircular();
            return true;
            //        case DIK_NUMPADENTER:
            //        case DIK_RETURN:
            //            CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
            //            return true;
        }
    } else if (!m_bOpenningNow && m_pGroup_ServerList && m_pGroup_ServerList->IsVisible()) {
        switch (iKey) {
        case DIK_UP: {
            if (NULL == m_pList_Server) {
                return false;
            }

            int iIndex = m_pList_Server->GetCurSel();

            if (iIndex > 0) {
                m_pList_Server->SetCurSel(iIndex - 1);
            }
            int iCnt = m_pList_Server->GetCount();
        }
            return true;
        case DIK_DOWN: {
            if (NULL == m_pList_Server) {
                return false;
            }

            int iIndex = m_pList_Server->GetCurSel();
            int iCnt = m_pList_Server->GetCount();

            if (iCnt - iIndex > 1) {
                m_pList_Server->SetCurSel(iIndex + 1);
            }
        }
            return true;
        case DIK_NUMPADENTER:
        case DIK_RETURN:
            ReceiveMessage(m_pBtn_Connect, UIMSG_BUTTON_CLICK);
            return true;
        }
    }

    return CN3UIBase::OnKeyPress(iKey);
}
