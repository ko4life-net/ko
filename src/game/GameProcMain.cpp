// GameProcMain.cpp: implementation of the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////
#include <io.h>

#include "StdAfx.h"
#include "Resource.h"

#include "GameEng.h"
#include "GameProcMain.h"
#include "LocalInput.h"

#include "APISocket.h"
#include "Compress.h"
#include "PacketDef.h"

#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"
#include "ServerMesh.h"
#include "N3FXMgr.h"

#include "UIManager.h"
#include "UILoading.h"
#include "UIChat.h"
#include "UIInventory.h"
#include "UICmd.h"
#include "UICmdList.h"
#include "UICmdEdit.h"
#include "UIVarious.h"
#include "UIStateBar.h"
#include "UITargetBar.h"
#include "UITransactionDlg.h"
#include "UIHelp.h"
#include "UIMessageWnd.h"
#include "UINotice.h"
#include "UIDroppedItemDlg.h"
#include "UIPerTradeDlg.h"
#include "UIPartyOrForce.h"
#include "UISkillTreeDlg.h"
#include "UIHotKeyDlg.h"
#include "UIClassChange.h"
#include "UINpcEvent.h"
#include "UIItemExchange.h"
#include "UIRepairTooltipDlg.h"
#include "UINpcTalk.h"
#include "UIKnightsOperation.h"
#include "UIPartyBBS.h"
#include "UIWareHouseDlg.h"
#include "UINPCChangeEvent.h"
#include "UIWarp.h"
#include "UIInn.h"
#include "UICreateClanName.h"
#include "UIPartyBBS.h"
#include "UITradeSellBBS.h"
#include "UITradeBBSSelector.h"
#include "UITradeBBSEditDlg.h"
#include "UIQuestMenu.h"
#include "UIQuestTalk.h"
#include "UILevelGuide.h"
#include "UIDead.h"
#include "UIRookieTip.h"
#include "UIExitMenu.h"

#include "SubProcPerTrade.h"
#include "CountableItemEditDlg.h"
#include "MagicSkillMng.h"
#include "WarMessage.h"
#include "GameCursor.h"

#include "N3WorldManager.h"

#include "LightMgr.h"

#include "N3Base/N3SkyMng.h"
#include "N3Base/N3ShapeExtra.h"
#include "N3Base/N3Camera.h"
#include "N3Base/N3SndObj.h"
#include "N3Base/N3SndObjStream.h"
#include "N3Base/N3SndMgr.h"
#include "N3Base/N3TableBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcMain::CGameProcMain() // r�⺻ ������.. �� ������ ��Ȱ�� ��� ����..
{
    m_fLBClickTime = 0.0f;
    m_bLoadComplete = FALSE;
    m_fRequestGameSave = 300.0f;

    //sound obj...
    m_pSnd_Town = NULL;
    m_pSnd_Battle = NULL;

    m_iJoinReqClan = 0;
    m_iJoinReqClanRequierID = 0;

    m_fRotateValue = -1.0f;

    m_bDoCancelExitRequest = false;
    m_eExitType = EXIT_TYPE_NONE;
    m_iExitCurCountDown = SECONDS_TO_EXIT_GAME_AFTER_ATTACK;
    m_fExitCurCountDownToReach = -1.0f;

    //UI
    m_pUIMsgDlg = new CUIMessageWnd;
    m_pUIChatDlg = new CUIChat();
    m_pUIStateBarAndMiniMap = new CUIStateBar();
    m_pUIVar = new CUIVarious();
    m_pUICmd = new CUICmd();
    m_pUICmdList = new CUICmdList();
    m_pUICmdEditDlg = new CUICmdEdit();
    m_pUITargetBar = new CUITargetBar();
    m_pUIHelp = new CUIHelp();
    m_pUINotice = new CUINotice();
    m_pUIClassChange = new CUIClassChange();
    m_pUINpcEvent = new CUINPCEvent();
    m_pUIItemREDlg = new CUIItemExchange();
    m_pUIRepairTooltip = new CUIRepairTooltipDlg();
    m_pUIDroppedItemDlg = new CUIDroppedItemDlg();
    m_pUITransactionDlg = new CUITransactionDlg();
    m_pUIInventory = new CUIInventory();
    m_pUIPartyOrForce = new CUIPartyOrForce();
    m_pUISkillTreeDlg = new CUISkillTreeDlg();
    m_pUIHotKeyDlg = new CUIHotKeyDlg();
    m_pUINpcTalk = new CUINpcTalk();
    m_pUIKnightsOp = new CUIKnightsOperation(); // ���� ����Ʈ ����, ����, ��...
    m_pUIPartyBBS = new CUIPartyBBS();          // ��Ƽ ���� �ý��� �Խ���??..
    m_pUIWareHouseDlg = new CUIWareHouseDlg();
    m_pUINpcChange = new CUINPCChangeEvent();
    m_pUIWarp = new CUIWarp();
    m_pUIInn = new CUIInn();
    m_pUICreateClanName = new CUICreateClanName();
    m_pUITradeBBS = new CUITradeSellBBS();
    m_pUITradeBBSSelector = new CUITradeBBSSelector();
    m_pUITradeBBSEdit = new CUITradeBBSEditDlg();
    m_pUIQuestMenu = new CUIQuestMenu();
    m_pUIQuestTalk = new CUIQuestTalk();
    m_pUILevelGuide = new CUILevelGuide();
    m_pUIDead = new CUIDead();
    m_pUIRookieTip = new CUIRookieTip();
    m_pUIExitMenu = new CUIExitMenu();

    m_pSubProcPerTrade = new CSubProcPerTrade();
    m_pMagicSkillMng = new CMagicSkillMng(this);
    m_pTargetSymbol = new CN3Shape(); // �÷��̾ Ÿ������ ���� ĳ������ ��ġ���� �׸��� �ȴ�..
    m_pWarMessage = new CWarMessage;

    m_pLightMgr = new CLightMgr;
}

CGameProcMain::~CGameProcMain() {
    this->Release();

    //UI
    delete m_pUIMsgDlg;
    delete m_pUIChatDlg;
    delete m_pUIStateBarAndMiniMap;
    delete m_pUIVar;
    delete m_pUICmd;
    delete m_pUICmdList;
    delete m_pUICmdEditDlg;
    delete m_pUITargetBar;
    delete m_pUIHelp;
    delete m_pUINotice;
    delete m_pUIClassChange;
    delete m_pUINpcEvent;
    delete m_pUIItemREDlg;
    delete m_pUIRepairTooltip;
    delete m_pUIDroppedItemDlg;
    delete m_pUITransactionDlg;
    delete m_pUIInventory;
    delete m_pUIPartyOrForce;
    delete m_pUISkillTreeDlg;
    delete m_pUIHotKeyDlg;
    delete m_pUINpcTalk;
    delete m_pUIKnightsOp;
    delete m_pUIPartyBBS;
    delete m_pUIWareHouseDlg;
    delete m_pUINpcChange;
    delete m_pUIWarp;
    delete m_pUIInn;
    delete m_pUICreateClanName;
    delete m_pUITradeBBS;
    delete m_pUITradeBBSSelector;
    delete m_pUITradeBBSEdit;
    delete m_pUIQuestMenu;
    delete m_pUIQuestTalk;
    delete m_pUILevelGuide;
    delete m_pUIDead;
    delete m_pUIRookieTip;
    delete m_pUIExitMenu;

    delete m_pSubProcPerTrade;
    delete m_pMagicSkillMng;
    delete m_pWarMessage;
    delete m_pTargetSymbol; // �÷��̾ Ÿ������ ���� ĳ������ ��ġ���� �׸��� �ȴ�..

    delete m_pLightMgr;
}

void CGameProcMain::Release() {
    this->ReleaseSound();
    this->ReleaseUIs();

    m_pSubProcPerTrade->Release();
    m_pMagicSkillMng->Release();
    m_pWarMessage->Release();
    m_pTargetSymbol->Release();
    m_pLightMgr->Release();

    CGameProcedure::Release();
}

void CGameProcMain::ReleaseUIs() {
    m_pUIChatDlg->Release();
    m_pUIMsgDlg->Release();
    m_pUICmd->Release();
    m_pUICmdList->Release();
    m_pUICmdEditDlg->Release();
    m_pUIVar->Release();
    m_pUIStateBarAndMiniMap->Release();
    m_pUITargetBar->Release();
    m_pUIHelp->Release();
    m_pUINotice->Release();
    m_pUIClassChange->Release();
    m_pUINpcEvent->Release();
    m_pUIItemREDlg->Release();
    m_pUIRepairTooltip->Release();
    m_pUIDroppedItemDlg->Release();
    m_pUIPartyOrForce->Release();
    m_pUISkillTreeDlg->Release();
    m_pUIInventory->Release();
    m_pUIHotKeyDlg->Release();
    m_pUINpcTalk->Release();
    //    m_pUITradeList->Release();
    m_pUIKnightsOp->Release(); // ���� ����Ʈ ����, ����, ��...
    m_pUIPartyBBS->Release();
    m_pUIWareHouseDlg->Release();
    m_pUINpcChange->Release();
    m_pUIWarp->Release();
    m_pUIInn->Release();
    m_pUICreateClanName->Release();
    m_pUIRookieTip->Release();
    m_pUIExitMenu->Release();

    CN3UIBase::DestroyTooltip();
}

void CGameProcMain::Init() {
    CGameProcedure::Init();
    m_pLightMgr->Release();
    s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

    s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));

    if (m_pWarMessage) {
        m_pWarMessage->InitFont();
    }
    this->InitUI();                                                       // ������ ���� �ٸ� UI �ε�...
    this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // �� �ε�..

    //sound obj...
    if (m_pSnd_Battle == NULL) {
        int iIDSndBattle =
            ((NATION_KARUS == s_pPlayer->m_InfoBase.eNation) ? ID_SOUND_BGM_KA_BATTLE : ID_SOUND_BGM_EL_BATTLE);
        m_pSnd_Battle = s_pEng->s_SndMgr.CreateStreamObj(iIDSndBattle); // �������� ID
        if (m_pSnd_Battle) {
            m_pSnd_Battle->Looping(true);
            m_pSnd_Battle->Stop();
        }
    }
    if (m_pSnd_Town == NULL) {
        m_pSnd_Town = s_pEng->s_SndMgr.CreateStreamObj(ID_SOUND_BGM_TOWN); // �������� ID
        if (m_pSnd_Town) {
            m_pSnd_Town->Looping(true);
            m_pSnd_Town->Play(NULL, 3.0f);
        }
    }

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data...", 0);
    }

    // ��� ���..
    char szPathOld[_MAX_PATH], szPathFind[_MAX_PATH];
    ::GetCurrentDirectory(_MAX_PATH, szPathOld);

    _finddata_t fi;
    long        hFind = -1;

    // ���ҽ� �� �б�..
    // ���ϸ��̼� �� �б�..
    lstrcpy(szPathFind, szPathOld);
    lstrcat(szPathFind, "\\Chr");
    ::SetCurrentDirectory(szPathFind);
    hFind = _findfirst("*.N3Anim", &fi);
    if (hFind) {
        std::string szFN = "Chr\\";
        szFN += fi.name;
        CN3AnimControl * pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
        while (_findnext(hFind, &fi) != -1) {
            szFN = "Chr\\";
            szFN += fi.name;
            pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
        }
    }
    _findclose(hFind);

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data... 10 %", 10);
    }

    // ���ҽ� �� �б�..
    // �ؽ�ó �� �б�..
    lstrcpy(szPathFind, szPathOld);
    lstrcat(szPathFind, "\\Item");
    ::SetCurrentDirectory(szPathFind);
    hFind = _findfirst("*.dxt", &fi);
    if (hFind) {
        std::string szFN = "Item\\";
        szFN += fi.name;
        CN3Texture * pObjTmp = s_MngTex.Get(szFN.c_str());
        while (_findnext(hFind, &fi) != -1) {
            szFN = "Item\\";
            szFN += fi.name;
            pObjTmp = s_MngTex.Get(szFN.c_str());
        }
    }
    _findclose(hFind);

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data... 25 %", 25);
    }

    // ���ҽ� �� �б�..
    // ����Ʈ �� �б�..
    lstrcpy(szPathFind, szPathOld);
    lstrcat(szPathFind, "\\Chr");
    ::SetCurrentDirectory(szPathFind);
    hFind = _findfirst("*.N3Joint", &fi);
    if (hFind) {
        std::string szFN = "Chr\\";
        szFN += fi.name;
        CN3Joint * pObjTmp = s_MngJoint.Get(szFN.c_str());
        while (_findnext(hFind, &fi) != -1) {
            szFN = "Chr\\";
            szFN += fi.name;
            pObjTmp = s_MngJoint.Get(szFN.c_str());
        }
    }
    _findclose(hFind);

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data... 50 %", 50);
    }

    // ���ҽ� �� �б�..
    // ��Ų �б�..
    lstrcpy(szPathFind, szPathOld);
    lstrcat(szPathFind, "\\Item");
    ::SetCurrentDirectory(szPathFind);
    hFind = _findfirst("*.N3CSkins", &fi);
    if (hFind) {
        std::string szFN = "Item\\";
        szFN += fi.name;
        CN3CPartSkins * pObjTmp = s_MngSkins.Get(szFN.c_str());
        while (_findnext(hFind, &fi) != -1) {
            szFN = "Item\\";
            szFN += fi.name;
            pObjTmp = s_MngSkins.Get(szFN.c_str());
        }
    }
    _findclose(hFind);

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data... 75 %", 75);
    }

    // ���ҽ� �� �б�..
    // PMesh �б�..
    lstrcpy(szPathFind, szPathOld);
    lstrcat(szPathFind, "\\Item");
    ::SetCurrentDirectory(szPathFind);
    hFind = _findfirst("*.N3PMesh", &fi);
    if (hFind) {
        std::string szFN = "Item\\";
        szFN += fi.name;
        CN3PMesh * pObjTmp = s_MngPMesh.Get(szFN.c_str());
        while (_findnext(hFind, &fi) != -1) {
            szFN = "Item\\";
            szFN += fi.name;
            pObjTmp = s_MngPMesh.Get(szFN.c_str());
        }
    }
    _findclose(hFind);

    if (s_pUILoading) {
        s_pUILoading->Render("Loading Character Data... 100 %", 100);
    }

    this->MsgSend_GameStart();
    // ��� ������..
    ::SetCurrentDirectory(szPathOld);
}

void CGameProcMain::InitPlayerPosition(
    const __Vector3 & vPos) // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
{
    __Vector3 vPosFinal = vPos;
    float     fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);   // ������ ���̰� ���..
    float     fYObject = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // ������Ʈ���� ���� ����� ���̰� ���..
    if (!s_pWorldMgr->IsIndoor()) {
        if (T_Abs(vPos.y - fYObject) < T_Abs(vPos.y - fYTerrain)) {
            vPosFinal.y = fYObject; // ���� ����� ���� ���´�..
        } else {
            vPosFinal.y = fYTerrain;
        }
    } else {
        if (fYObject > fYTerrain) {
            vPosFinal.y = fYObject;
        } else {
            vPosFinal.y = fYTerrain;
        }
    }

    s_pPlayer->PositionSet(vPosFinal, true); // ĳ���� ��ġ ����..
    s_pPlayer->m_vPosFromServer = vPos;
    m_vPlayerPosSended = vPos; // �ֱٿ� ���� ��ġ ����..
    m_fMsgSendTimeMove = 0;    // �ð��� ����Ѵ�..

    this->CommandSitDown(false, false, true); // ������ �����.. �ɾ��ִ� ���¿��� �����ϸ�.. ���װ� �ִ�..
    this->TargetSelect(-1, false);            // Ÿ�� ����..
    this->UpdateCameraAndLight();             // ī�޶�� ����Ʈ �ٽ� ���..

    s_pPlayer->Action(PSA_BASIC, true, NULL, true); // ������ �⺻ �ڼ�..
}

void CGameProcMain::Tick() {
    CGameProcedure::Tick(); // Ű, ���콺 �Է� ���..

    if (FALSE == m_bLoadComplete) {
        return; // �ε��� �ȵǾ�����.. ���ư���.
    }
    if (!s_pSocket->IsConnected()) {
        return;
    }

#ifdef _DEBUG
    if (s_pLocalInput->IsKeyPressed(DIK_F8)) {
        BYTE byBuff[32];
        int  iOffset = 0;
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_TEMP_TEST);
        s_pSocket->Send(byBuff, iOffset);

        int  iNPC = s_pOPMgr->m_NPCs.size();
        char szBuff[1024];
        sprintf(szBuff, "NPC Region Test : %d", iNPC);
        CLogWriter::Write(szBuff);
        it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
        for (; it != itEnd; it++) {
            CPlayerNPC * pNPC = it->second;

            sprintf(szBuff, "    ID(%d) Name(%s) Pos(%.1f, %.1f)", pNPC->IDNumber(), pNPC->IDString().c_str(),
                    pNPC->m_vPosFromServer.x, pNPC->m_vPosFromServer.z);
            CLogWriter::Write(szBuff);
        }
    }
#endif

    DWORD dwMouseFlags = s_pLocalInput->MouseGetFlag(); // ���콺 ��ư �÷��� - LocalInput.h ����
    this->ProcessLocalInput(dwMouseFlags);              // Ű���峪 ���콺 �Է��� UI ������ ó��...

    MsgSend_Continous(); // ���� �ð����� �����Ӱ� ȸ����, ���ݵ��� üũ�ؼ� ��Ŷ ����� ����..

    s_pPlayer->Tick(); // �÷��̾� ƽ(����)
    s_pWorldMgr->Tick();
    s_pOPMgr->Tick(s_pPlayer->Position()); // �ٸ� ���� ������ ƽ(����)
    //    s_pFX->Tick(); //���ο��� ī�޶� ���� �� ��� ��ġ�� ������ ���� Render()�Լ� ������ �ű�...

    __Vector3 ListenerPos = s_pPlayer->Position();
    __Vector3 ListenerDir = s_pPlayer->Direction();
    __Vector3 ListenerUp(0, 1, 0);

    // Sound Tick...
    CN3SndObj::SetListenerPos(&ListenerPos);
    CN3SndObj::SetListenerOrientation(&ListenerDir, &ListenerUp);

    this->UpdateUI_MiniMap();     // �̴ϸ� ������Ʈ..
    this->UpdateUI_TargetBar();   // Ÿ�ٹ� ó��..
    this->UpdateBGM();            // ��������� ��Ȳ�� ���� ó��..
    this->UpdateCameraAndLight(); // ī�޶�� ����Ʈ ó��..

//    ProcessPlayerInclination();                            // ��� ó��..(������ �־ ��簡 ���ϸ� �̲�����..).
#ifdef _N3_64GRID_
    m_SMesh.Tick(s_pPlayer, &m_Terrain); // ���� �޽� ƽ.(����)
#endif

    m_pUIStateBarAndMiniMap->UpdatePosition(s_pPlayer->Position(), s_pPlayer->Yaw()); // ��ġ ������Ʈ.

    if (m_pMagicSkillMng) {
        m_pMagicSkillMng->Tick();
    }
    if (m_pWarMessage) {
        m_pWarMessage->Tick();
    }
    if (m_pLightMgr) {
        m_pLightMgr->Tick();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // �ƹ� ��Ŷ�� �Ⱥ������� 2�ʿ� �ѹ� N3_TIME_NOTIFY ������..
    float        fTime = CN3Base::TimeGet();
    static float fTimePrev = fTime;

    static float fTimeInterval1 = 0;
    if (0 == s_pSocket->m_iSendByteCount) {
        fTimeInterval1 += fTime - fTimePrev;
        if (fTimeInterval1 >= 2.0f) {
            BYTE byCmd = N3_TIME_NOTIFY;
            s_pSocket->Send(&byCmd, 1);
            s_pSocket->m_iSendByteCount = 0;
            fTimeInterval1 = 0;
        }
    } else {
        s_pSocket->m_iSendByteCount = 0;
        fTimeInterval1 = 0;
    }
    // �ƹ� ��Ŷ�� �Ⱥ������� 2�ʿ� �ѹ� N3_TIME_NOTIFY ������..
    ////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    // Ÿ�̸� ����� ��ƾ..
    static float fInterval2 = 0, fInterval3 = 0, fInterval4 = 0, fInterval5 = 0;
    fInterval2 += fTime - fTimePrev;
    fInterval3 += fTime - fTimePrev;
    fInterval4 += fTime - fTimePrev;
    fInterval5 += fTime - fTimePrev;
    m_fRequestGameSave += fTime - fTimePrev;
    if (fInterval2 > 1200.0f) // ���� ��û..
    {
        BYTE byBuff[4];                                               // ����..
        int  iOffset = 0;                                             // �ɼ�..
        s_pSocket->MP_AddByte(byBuff, iOffset, N3_REQUEST_GAME_SAVE); // ���� ��û Ŀ���..
        s_pSocket->Send(byBuff, iOffset);                             // ����..

        fInterval2 = 0.0f;
    }
    if (fInterval3 > 10.0f) // ���ǵ��� üũ..
    {
        MsgSend_SpeedCheck(); // ���ǵ��� üũ �ϱ�

        fInterval3 = 0.0f;
    }
    if (s_pPlayer->m_InfoBase.iLevel < 12 && fInterval4 > 20.0f) // �ð��� ������ �� �ϳ��� ǥ��..
    {
        std::string szMsg;
        ::_LoadStringFromResource(IDS_HELP_TIP_ALL, szMsg);
        this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // ���� ǥ��..
        ::_LoadStringFromResource(IDS_HELP_TIP1 + rand() % 30, szMsg);
        this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // ���� ǥ��..
        fInterval4 = 0;
    }
    if (fInterval5 > 5.0f) // �ð��� ������ �� �ϳ��� ǥ��..
    {
        //        m_pUIChatDlg->ChangeChattingMode(N3_CHAT_CONTINUE); // ä�ø�� ������ �ٲٱ�...
        m_pUIChatDlg->ShowContinueMsg();
        fInterval5 = 0;
    }
    // Ÿ�̸� ����� ��ƾ..
    ////////////////////////////////////////////////////////////////////////////////////

    // Exit Menu
    if (m_bDoCancelExitRequest && m_fExitCurCountDownToReach != -1.0f) {
        if (m_fExitCurCountDownToReach < (float)SECONDS_TO_EXIT_GAME_AFTER_ATTACK) {
            m_fExitCurCountDownToReach += fTime - fTimePrev;
        } else {
            m_fExitCurCountDownToReach = -1.0f;
            m_bDoCancelExitRequest = false;
        }
    }

    if (m_eExitType > EXIT_TYPE_NONE) {
        int iCountDown = (int)((float)SECONDS_TO_EXIT_GAME_AFTER_ATTACK - m_fExitCurCountDownToReach);
        if (m_iExitCurCountDown > iCountDown) {
            m_iExitCurCountDown = iCountDown;
            if (m_pUIChatDlg) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_EXIT_GAME_IN, szMsg);
                char szBuff[100]{};
                sprintf(szBuff, szMsg.c_str(), m_iExitCurCountDown);
                m_pUIChatDlg->AddChatMsg(N3_CHAT_NORMAL, szBuff, 0xFFFF0000);
            }
            if (m_iExitCurCountDown <= 0) {
                e_GameExitType eExitType = m_eExitType;
                m_eExitType = EXIT_TYPE_NONE;
                m_iExitCurCountDown = SECONDS_TO_EXIT_GAME_AFTER_ATTACK;
                m_fExitCurCountDownToReach = -1.0f;
                m_bDoCancelExitRequest = false;
                //m_bRecruitingParty = false; // TODO:

                if (eExitType == EXIT_TYPE_SELECTCHAR) {
                    m_pUIExitMenu->SelectCharacter();
                } else if (eExitType == EXIT_TYPE_EXIT) {
                    ::PostQuitMessage(0);
                } else if (eExitType == EXIT_TYPE_OPTION) {
                    ::ShellExecute(NULL, "open", "Option.exe", NULL, NULL, SW_SHOWNORMAL);
                    ::PostQuitMessage(0);
                }
            }
        }
    }

    // TODO: Implement
    //if (m_bRecruitingParty) {
    //    if ((fTime - m_fRecruitingPartyCurTime) > 180.0f && m_pUIPartyBBS) {
    //        std::string szMsg;
    //        m_pUIPartyBBS->GetRecruitingPartyString(szMsg);
    //        if (!szMsg.empty()) {
    //            m_fRecruitingPartyCurTime = fTime;
    //            CGameProcedure::s_pProcMain->MsgSend_Chat(N3_CHAT_SEEKING_PARTY, szMsg);
    //        }
    //    }
    //}

    // Previous time since last tick
    fTimePrev = fTime;

    // Rotates the player's camera 180 degrees from the current position.
    if (m_fRotateValue >= 0.0f && !CGameProcedure::s_pUIMgr->m_bDoneSomething) {
        m_fRotateValue += s_fSecPerFrm * __PI2;
        float fRadian = s_fSecPerFrm * __PI2;
        if (m_fRotateValue > __PI) {
            fRadian -= m_fRotateValue - __PI;
            m_fRotateValue = -1.0f;
        }
        if (fRadian != 0.0f) {
            if (CGameBase::s_pPlayer->IsAlive()) {
                float fRotX = fRadian / s_fSecPerFrm;
                if (CGameProcedure::s_pEng->ViewPoint() == VP_THIRD_PERSON) {
                    CGameProcedure::s_pEng->CameraYawAdd(fRotX);
                } else if (!CGameBase::s_pPlayer->m_bStun) {
                    CGameBase::s_pPlayer->RotAdd(fRotX);
                }
            }
        }
    }
}

void CGameProcMain::Render() {
    if (FALSE == m_bLoadComplete) {
        return; // �ε��� ������??
    }

    D3DCOLOR crSky = ACT_WORLD->GetSkyColorWithSky();
    s_pEng->Clear(crSky); // �Ȱ� ������ �־ Ŭ����.. -> �ϴû���� Ŭ���� �ؾ� �ϴ��� ����� ���´�..
    s_pEng->BeginScene(); // �� ���� ����...

    ACT_WORLD->RenderSky();                                   // �ϴ� ������..
    float fSunAngle = ACT_WORLD->GetSunAngleByRadinWithSky(); // ���� ������ ��������..

    DWORD dwFilter = D3DTEXF_LINEAR;
    CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER,
                                         dwFilter); // �ؽ��ĸ� �ٿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����
    CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER,
                                         dwFilter); // �ؽ��ĸ� �ÿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����
    CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER,
                                         dwFilter); // �ؽ��ĸ� �ٿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����
    CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MINFILTER,
                                         dwFilter); // �ؽ��ĸ� �ٿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����
    CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MAGFILTER,
                                         dwFilter); // �ؽ��ĸ� �ÿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����
    CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER,
                                         dwFilter); // �ؽ��ĸ� �ٿ��� ����� ��� �ȼ��� ������ó�� ���̴� �� ����

    ACT_WORLD->RenderTerrain();   // ���� ����..
    ACT_WORLD->RenderShape();     // ��ü ����..
    s_pOPMgr->Render(fSunAngle);  // �ٸ� �÷��̾� ����..
    s_pPlayer->Render(fSunAngle); // �÷��̾� ����..

#ifdef _DEBUG
    ACT_WORLD->RenderCollisionWithShape(s_pPlayer->Position()); // �浹 �޽� ����..
#endif

#ifdef _N3_64GRID_
    m_SMesh.Render(); // ���� �޽� ����..
#endif

    this->RenderTarget(); // Ÿ������ ���� ĳ���� Ȥ�� ������Ʈ ������..

    ACT_WORLD->RenderGrass(); //    Ǯ ���� (asm)
    s_pFX->Tick();
    s_pFX->Render();
    ACT_WORLD->RenderBirdMgr();

    CN3Base::s_AlphaMgr.Render(); // ���� ���ĵ� ������� ������..

    ACT_WORLD->RenderSkyWeather(); // �ϴ� ������..

    CGameProcedure::Render(); // UI �� �׹��� �⺻���� �͵� ������..
    if (m_pWarMessage) {
        m_pWarMessage->RenderMessage();
    }
    if (s_pGameCursor) {
        s_pGameCursor->Render();
    }

    s_pEng->EndScene();
    s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcMain::RenderTarget() {
    if (NULL == m_pTargetSymbol) {
        return;
    }

    // �÷��̾ Ÿ������ ���� ĳ������ ��ġ���� �׸��� �ȴ�..
    CPlayerBase * pTarget =
        s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false); //��ü�� �����Ǳ� �������� ĳ���� ��Ŀ���� �ش�.
    if (NULL == pTarget && NULL == s_pPlayer->m_pObjectTarget) {
        return;
    }

    float     fScale = 1;
    float     fYScale = 1;
    __Vector3 vPos(0, 0, 0);

    if (pTarget) {
        fYScale = pTarget->Height() * 1.3f;
        fScale = pTarget->Radius() * 2.0f;
        vPos = pTarget->Position();
    } else {
        __Vector3 vTmp = s_pPlayer->m_pObjectTarget->Max() - s_pPlayer->m_pObjectTarget->Min();
        fYScale = vTmp.y;
        vTmp.y = 0;
        fScale = vTmp.Magnitude();
        vPos = s_pPlayer->m_pObjectTarget->Pos();
    }

    m_pTargetSymbol->ScaleSet(fScale, fYScale, fScale);
    m_pTargetSymbol->PosSet(vPos);
    m_pTargetSymbol->Tick();
    if (m_pTargetSymbol->Part(1)) // �ٴ��� �ɹ��� ���ٴ� ���� �����..
    {
        CN3PMesh * pPMesh = m_pTargetSymbol->Part(1)->Mesh();
        if (pPMesh && pPMesh->GetMaxNumVertices() == 4) {
            __VertexT1 * pVs = m_pTargetSymbol->Part(1)->Mesh()->GetVertices();
            for (int i = 0; i < 4; i++) {
                pVs[i].y =
                    ACT_WORLD->GetHeightWithTerrain(vPos.x + (pVs[i].x * fYScale), vPos.z + (pVs[i].z * fYScale));
                pVs[i].y -= vPos.y;
                pVs[i].y /= fYScale;
                pVs[i].y += 0.1f / fYScale;
            }
        }
    }
    m_pTargetSymbol->Render();

#ifdef _DEBUG
    if (pTarget) {
        pTarget->RenderCollisionMesh();
    }
    if (s_pPlayer->m_pObjectTarget) {
        s_pPlayer->m_pObjectTarget->RenderCollisionMesh();
    }
#endif
}

bool CGameProcMain::ProcessPacket(DataPack * pDataPack, int & iOffset) {
    int iOffsetPrev = iOffset;
    if (false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) {
        iOffset = iOffsetPrev;
    } else {
        return true;
    }

    int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // Ŀ��� �Ľ�..

    switch (iCmd) // Ŀ��忡 �ٶ� �б�..
    {
#ifdef _DEBUG
    case N3_TEMP_TEST: {
        int  iNPC = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        char szBuff[32];
        sprintf(szBuff, "NPC Region Test : %d -> ", iNPC);
        std::string szLog = szBuff;
        for (int i = 0; i < iNPC; i++) {
            int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            sprintf(szBuff, "%d, ", iID);
            szLog += szBuff;
        }
        CLogWriter::Write(szLog.c_str());
    }
        return true;
#endif

    case N3_MYINFO: // ���� ���� �޽���..
        this->MsgRecv_MyInfo_All(pDataPack, iOffset);
        return true;
    case N3_HP_CHANGE:
        this->MsgRecv_MyInfo_HP(pDataPack, iOffset);
        return true;
    case N3_MSP_CHANGE:
        this->MsgRecv_MyInfo_MSP(pDataPack, iOffset);
        return true;
    case N3_EXP_CHANGE:
        this->MsgRecv_MyInfo_EXP(pDataPack, iOffset);
        return true;
    case N3_REALM_POINT_CHANGE: // ���� �⿩��..
        this->MsgRecv_MyInfo_RealmPoint(pDataPack, iOffset);
        return true;
    case N3_LEVEL_CHANGE:
        this->MsgRecv_MyInfo_LevelChange(pDataPack, iOffset);
        return true;
    case N3_POINT_CHANGE:
        this->MsgRecv_MyInfo_PointChange(pDataPack, iOffset);
        return true;
    case N3_CHAT: // ä�� �޽���..
        this->MsgRecv_Chat(pDataPack, iOffset);
        return true;
    case N3_WARP: {
        float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
        float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;

        float fY = ACT_WORLD->GetHeightWithTerrain(fX, fZ, true);
        float fYObject = ACT_WORLD->GetHeightWithShape(fX, fZ);
        if (fYObject > fY) {
            fY = fYObject;
        }

        this->InitPlayerPosition(
            __Vector3(fX, fY, fZ)); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
    }
        return true;
    case N3_MOVE:
        this->MsgRecv_UserMove(pDataPack, iOffset);
        return true;
    case N3_ROTATE: // ȸ�� Ŀ���..
        this->MsgRecv_Rotation(pDataPack, iOffset);
        return true;
    case N3_REGENE: {
        //                if(m_pUIDead) m_pUIDead->MsgRecv_Revival(pDataPack, iOffset);
        this->MsgRecv_Regen(pDataPack, iOffset);
        std::string szMsg;
        ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
        MessageBoxClose(szMsg);
        m_pUITargetBar->SetVisible(false);
    }
        return true;
    case N3_DEAD:
        this->MsgRecv_Dead(pDataPack, iOffset);
        return true;
    case N3_TIME:
        this->MsgRecv_Time(pDataPack, iOffset);
        return true;
    case N3_WEATHER:
        this->MsgRecv_Weather(pDataPack, iOffset);
        return true;
    case N3_USER_INOUT: // �ٸ� ���� ��/�ƿ�..
        this->MsgRecv_UserInOut(pDataPack, iOffset);
        return true;
    case N3_UPDATE_REGION_UPC: // ÷�� �α׿��ϸ� �� �ֺ� ������ ĳ���͵� ������Ʈ...
        this->MsgRecv_UserInAndRequest(pDataPack, iOffset);
        return true;
    case N3_REQUEST_USER_IN:                               // ������ ��û�� UserIn �� ���� �ڼ��� ���� �ޱ�..
        this->MsgRecv_UserInRequested(pDataPack, iOffset); //
        return true;
    case N3_UPDATE_REGION_NPC: // ÷�� �α׿��ϸ� �� �ֺ� ������ ĳ���͵� ������Ʈ...
        this->MsgRecv_NPCInAndRequest(pDataPack, iOffset);
        return true;
    case N3_REQUEST_NPC_IN:                               // ������ ��û�� UserIn �� ���� �ڼ��� ���� �ޱ�..
        this->MsgRecv_NPCInRequested(pDataPack, iOffset); //
        return true;
    case N3_NPC_INOUT: // NPC ��/�ƿ�..
        this->MsgRecv_NPCInOut(pDataPack, iOffset);
        return true;
    case N3_ATTACK:
        this->MsgRecv_Attack(pDataPack, iOffset);
        return true;
    case N3_NPC_MOVE: // NPC ������ ��Ŷ..
        this->MsgRecv_NPCMove(pDataPack, iOffset);
        return true;
    case N3_TARGET_HP:
        this->MsgRecv_TargetHP(pDataPack, iOffset);
        return true;
    case N3_ITEM_MOVE:
        this->MsgRecv_ItemMove(pDataPack, iOffset); // Item Move�� ���� ����..
        return true;
    case N3_ITEM_BUNDLE_DROP:
        this->MsgRecv_ItemBundleDrop(pDataPack, iOffset);
        return true;
    case N3_ITEM_BUNDLE_OPEN_REQUEST:
        this->MsgRecv_ItemBundleOpen(pDataPack, iOffset);
        return true;
    case N3_ITEM_TRADE_START:
        this->MsgRecv_ItemTradeStart(pDataPack, iOffset);
        return true;
    case N3_ITEM_TRADE:
        this->MsgRecv_ItemTradeResult(pDataPack, iOffset);
        return true;
    case N3_ITEM_DROPPED_GET:
        this->MsgRecv_ItemDroppedGetResult(pDataPack, iOffset); // ���� ������ ������ �Ա� ���..
        return true;
    case N3_ITEM_TRADE_REPAIR:
        this->MsgRecv_NpcEvent(pDataPack, iOffset);
        return true;
    case N3_ITEM_REPAIR_REQUEST:
        this->MsgRecv_ItemRepair(pDataPack, iOffset);
        return true;
    case N3_ITEM_COUNT_CHANGE:
        this->MsgRecv_ItemCountChange(pDataPack, iOffset);
        return true;
    case N3_ITEM_DESTROY:
        this->MsgRecv_ItemDestroy(pDataPack, iOffset);
        return true;
    case N3_ITEM_WEIGHT_CHANGE:
        this->MsgRecv_ItemWeightChange(pDataPack, iOffset);
        return true;
    case N3_USER_LOOK_CHANGE:
        this->MsgRecv_UserLookChange(pDataPack, iOffset);
        return true;
    case N3_ZONE_CHANGE:
        this->MsgRecv_ZoneChange(pDataPack, iOffset);
        this->MsgSend_ZoneChangeComplete(); // Zone Loading �Ϸ� ��Ŷ ����..
        return true;
    case N3_STATE_CHANGE:
        this->MsgRecv_UserState(pDataPack, iOffset);
        return true;
    case N3_NOTICE:
        this->MsgRecv_Notice(pDataPack, iOffset);
        return true;
    case N3_PARTY_OR_FORCE:
        this->MsgRecv_PartyOrForce(pDataPack, iOffset);
        return true;
    case N3_PER_TRADE:
        this->MsgRecv_PerTrade(pDataPack, iOffset);
        return true;
    case N3_SKILL_CHANGE:
        this->MsgRecv_SkillChange(pDataPack, iOffset);
        return true;
    case N3_MAGIC:
        this->MsgRecv_MagicProcess(pDataPack, iOffset);
        return true;
    case N3_CLASS_CHANGE:
        this->MsgRecv_NpcChangeOpen(pDataPack, iOffset);
        return true;
    case N3_OBJECT_EVENT:
        this->MsgRecv_ObjectEvent(pDataPack, iOffset);
        return true;
    case N3_CHAT_SELECT_TARGET: {
        std::string szID, szMsg;
        int         iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ID ���ڿ� ����..
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);       // ID ���ڿ�..

        e_ChatMode eCM = N3_CHAT_UNKNOWN;
        if (szID.empty()) {
            ::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_FAIL, szMsg);
            eCM = N3_CHAT_NORMAL;
        } else {
            ::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_SUCCESS, szMsg);
            eCM = N3_CHAT_PRIVATE;
        }

        this->MsgOutput(szID + szMsg, 0xffffff00);
        m_pUIChatDlg->ChangeChattingMode(eCM); // �ڵ����� �ӼӸ� ���� �ٲپ� �ش�..
    }
        return true;
    case N3_CONCURRENT_USER_COUNT: // ���� �����ڼ� ...
    {
        int iUserCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ID ���ڿ� ����..

        std::string szFmt;
        ::_LoadStringFromResource(IDS_FMT_CONCURRENT_USER_COUNT, szFmt);
        char szBuff[128] = "";
        sprintf(szBuff, szFmt.c_str(), iUserCount);
        this->MsgOutput(szBuff, D3DCOLOR_ARGB(255, 255, 255, 0));
    }
        return true;
    case N3_DURABILITY_CHANGE:
        this->MsgRecv_DurabilityChange(pDataPack, iOffset);
        return true;
    case N3_KNIGHTS:
        this->MsgRecv_Knights(pDataPack, iOffset);
        return true;
    case N3_KNIGHTS_LIST_BASIC:
        this->MsgRecv_KnightsListBasic(pDataPack, iOffset);
        return true;
    case N3_COMPRESSED_PACKET: // ����� ������ �̴�... �ѹ� �� �Ľ��ؾ� �Ѵ�!!!
        this->MsgRecv_CompressedPacket(pDataPack, iOffset);
        return true;
    case N3_CONTINOUS_PACKET: // ����� ������ �̴�... �ѹ� �� �Ľ��ؾ� �Ѵ�!!!
        this->MsgRecv_ContinousPacket(pDataPack, iOffset);
        return true;
    case N3_WAREHOUSE:                               // ������..
        this->MsgRecv_WareHouse(pDataPack, iOffset); // ������ ���� ��Ŷ..
        return true;
    case N3_FRIEND_INFO:
        if (m_pUIVar->m_pPageFriends) {
            m_pUIVar->m_pPageFriends->MsgRecv_MemberInfo(pDataPack, iOffset);
        }
        return true;
    case N3_NOAH_CHANGE:
        this->MsgRecv_NoahChange(pDataPack, iOffset);
        return true;
    case N3_WARP_LIST:
        this->MsgRecv_WarpList(pDataPack, iOffset);
        return true;
        //        case N3_SERVER_CHECK:
        //            this->MsgRecv_ServerCheckAndRequestConcurrentUserCount(pDataPack, iOffset);
        //            return true;
        //        case N3_SERVER_CONCURRENT_CONNECT:
        //            this->MsgRecv_ConcurrentUserCountAndSendServerCheck(pDataPack, iOffset);
        //            return true;
    case N3_CORPSE_CHAR: //regen�� �Ͽ� ���� �������� ��ü���� �˸���.
        this->MsgRecv_Corpse(pDataPack, iOffset);
        return true;
    case N3_PARTY_BBS:
        if (m_pUIPartyBBS) {
            m_pUIPartyBBS->MsgRecv_RefreshData(pDataPack, iOffset);
        }
        return true;
    case N3_TRADE_BBS:
        if (m_pUITradeBBS) {
            m_pUITradeBBS->MsgRecv_TradeBBS(pDataPack, iOffset);
        }
        return true;
    case N3_QUEST_SELECT:
        if (m_pUIQuestMenu) {
            m_pUIQuestMenu->Open(pDataPack, iOffset);
        }
        return true;
    case N3_QUEST_TALK:
        if (m_pUIQuestTalk) {
            m_pUIQuestTalk->Open(pDataPack, iOffset);
        }
        return true;
        //        case N3_CLAN:
        //            this->MsgRecv_Clan(pDataPack, iOffset);
        return true;
    }

    return false;
}

// Ű����� ���콺 �������� ó���Ѵ�..
void CGameProcMain::ProcessLocalInput(DWORD dwMouseFlags) {
    // Loading�� �� ��..
    if (FALSE == m_bLoadComplete) {
        return;
    }

    if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) { /* ���ΰ� ��ŷ� ���̸�.. */
        return;
    }

    //////////////////////////////////////////
    //
    // ���콺 ó��.
    //
    POINT ptPrev = s_pLocalInput->MouseGetPosOld();
    POINT ptCur = s_pLocalInput->MouseGetPos();

    OnMouseMove(ptCur, ptPrev);

    if (dwMouseFlags & MOUSE_RBCLICK) {
        OnMouseRBtnPress(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_RBDOWN) {
        OnMouseRbtnDown(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_RBCLICK) {
        OnMouseRBtnPressd(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_RBDBLCLK) {
        OnMouseRDBtnPress(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_LBCLICK) {
        OnMouseLBtnPress(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_LBDOWN) {
        OnMouseLbtnDown(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_LBCLICKED) {
        OnMouseLBtnPressd(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_LBDBLCLK) {
        OnMouseLDBtnPress(ptCur, ptPrev);
    }
    if (dwMouseFlags & MOUSE_MBCLICK) {
        m_fRotateValue = 0.0f;
    }

    // ���콺�� ���� ī�޶� ȸ��...
    float fRotY = 0, fRotX = 0;
    if (0 == ptCur.x) {
        fRotY = -2.0f;
    } else if ((CN3Base::s_CameraData.vp.Width - 1) == ptCur.x) {
        fRotY = 2.0f;
    }
    if (0 == ptCur.y) {
        fRotX = -1.0f;
    } else if ((CN3Base::s_CameraData.vp.Height - 1) == ptCur.y) {
        fRotX = 1.0f;
    }
    if (fRotY) {
        if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
            s_pEng->CameraYawAdd(fRotY);
        } else {
            s_pPlayer->RotAdd(fRotY);
        }
    }
    if (fRotX && VP_THIRD_PERSON != s_pEng->ViewPoint()) {
        s_pEng->CameraPitchAdd(fRotX);
    }

    //
    // ���콺 ó��.
    //
    //////////////////////////////////////////

    //////////////////////////////////////////
    // ��Ű
    int iHotKey = -1;
    if (s_pLocalInput->IsKeyPress(KM_HOTKEY1)) {
        iHotKey = 0;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY2)) {
        iHotKey = 1;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY3)) {
        iHotKey = 2;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY4)) {
        iHotKey = 3;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY5)) {
        iHotKey = 4;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY6)) {
        iHotKey = 5;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY7)) {
        iHotKey = 6;
    } else if (s_pLocalInput->IsKeyPress(KM_HOTKEY8)) {
        iHotKey = 7;
    }

    if ((iHotKey >= 0 && iHotKey < 8) && CN3UIBase::GetFocusedEdit() == NULL &&
        m_pSubProcPerTrade->m_ePerTradeState == PER_TRADE_STATE_NONE) {
        m_pUIHotKeyDlg->EffectTriggerByHotKey(iHotKey);
    }
    // ��Ű
    //////////////////////////////////////////

    if (s_pLocalInput->IsKeyPress(KM_CAMERA_CHANGE)) // ���� ��ȯ..
    {
        this->CommandCameraChange(); // ī�޶� ���� �ٲٱ�..
    }

    // ����Ī�϶� Ȩ, ����Ű�� ī�޷� �ø��� ������..
    if (s_pEng->ViewPoint() == VP_THIRD_PERSON) {
        float fPitch = 0;
        if (s_pLocalInput->IsKeyDown(DIK_HOME)) {
            fPitch = D3DXToRadian(45.0f); // home Ű�� ������..
        } else if (s_pLocalInput->IsKeyDown(DIK_END)) {
            fPitch = D3DXToRadian(-45.0f); // End Ű�� ������..
        }
        if (fPitch) {
            s_pEng->CameraPitchAdd(fPitch);
        }
    }

    if (!IsUIKeyOperated() && NULL == CN3UIBase::GetFocusedEdit()) // ä�ø�尡 �ƴҶ�
    {
#ifdef _DEBUG
        if (s_pLocalInput->IsKeyDown(DIK_Q)) {
            s_pPlayer->m_bTempMoveTurbo = true; // ��û ���� �����̰� �Ѵ�..  // �ӽ� �Լ�.. ���߿� ������..
        } else {
            s_pPlayer->m_bTempMoveTurbo = false; // ��û ���� �����̰� �Ѵ�..  // �ӽ� �Լ�.. ���߿� ������..
        }
#endif
        if (s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) //���� ��ڴ� �� ����� ����Ҽ� �ִ�.
        {
            if (s_pLocalInput->IsKeyDown(DIK_Q)) {
                s_pPlayer->m_bTempMoveTurbo = true; // ��û ���� �����̰� �Ѵ�..  // �ӽ� �Լ�.. ���߿� ������..
            } else {
                s_pPlayer->m_bTempMoveTurbo = false; // ��û ���� �����̰� �Ѵ�..  // �ӽ� �Լ�.. ���߿� ������..
            }
        }

        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_ATTACK)) {
            this->CommandToggleAttackContinous(); // �ڵ� ����..}
        }
        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_RUN)) {
            this->CommandToggleWalkRun(); // �ȱ� / �ٱ� ���
        }
        if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_ENEMY)) {
            this->CommandTargetSelect_NearstEnemy(); // ���� ����� �� Ÿ�� ���..
        }
        if (s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_PARTY)) {
            this->CommandTargetSelect_NearstOurForce(); // ���� ����� ��Ƽ Ÿ�����..
        }

        float fRotKeyDelta = D3DXToRadian(60); // �ʴ� 60 �� ����..
        if (s_pLocalInput->IsKeyDown(KM_ROTATE_LEFT) || s_pLocalInput->IsKeyDown(DIK_LEFT)) {
            if (s_pPlayer->IsAlive()) {
                s_pPlayer->RotAdd(-fRotKeyDelta); // �ʴ� 180 �� �������� ����.
            }
            if (m_pUIDroppedItemDlg->IsVisible()) {
                m_pUIDroppedItemDlg->LeaveDroppedState();
            }
        }
        if (s_pLocalInput->IsKeyDown(KM_ROTATE_RIGHT) || s_pLocalInput->IsKeyDown(DIK_RIGHT)) {
            if (s_pPlayer->IsAlive()) {
                s_pPlayer->RotAdd(fRotKeyDelta); // �ʴ� 180 �� ���������� ����.
            }
            if (m_pUIDroppedItemDlg->IsVisible()) {
                m_pUIDroppedItemDlg->LeaveDroppedState();
            }
        }

        if (s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyDown(DIK_UP)) {
            bool bStart = false;
            if (s_pLocalInput->IsKeyPress(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPress(DIK_UP)) {
                if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
                    if (!s_pPlayer->m_bTargetOrPosMove) {
                        bStart = true;
                    }
                    s_pPlayer->m_bTargetOrPosMove = false;
                    if (s_pPlayer->m_bAttackContinous) {
                        CommandToggleAttackContinous();
                    }
                } else {
                    bStart = true; // ������ �����̸�
                }
            }
            this->CommandMove(MD_FOWARD, bStart); // ������ �̵�..
        } else if (s_pLocalInput->IsKeyDown(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyDown(DIK_DOWN)) {
            bool bStart = false;
            if (s_pLocalInput->IsKeyPress(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPress(DIK_DOWN)) {
                if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
                    if (!s_pPlayer->m_bTargetOrPosMove) {
                        bStart = true;
                    }
                    s_pPlayer->m_bTargetOrPosMove = false;
                    if (s_pPlayer->m_bAttackContinous) {
                        CommandToggleAttackContinous();
                    }
                } else {
                    bStart = true; // ������ �����̸�
                }
            }
            this->CommandMove(MD_BACKWARD, bStart); // �ڷ� �̵�..
        } else if (s_pLocalInput->IsKeyPress(KM_TOGGLE_MOVE_CONTINOUS)) {
            this->CommandToggleMoveContinous();
        }

        if (s_pLocalInput->IsKeyPressed(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPressed(DIK_UP) ||
            s_pLocalInput->IsKeyPressed(KM_MOVE_BACKWARD) ||
            s_pLocalInput->IsKeyPressed(DIK_DOWN)) // ����/���� Ű�� ���� ����.
        {
            this->CommandMove(MD_STOP, true);
        }

        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_INVENTORY)) {
            this->CommandToggleUIInventory();
        }
        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_STATE)) {
            this->CommandToggleUIState();
        }
        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_SKILL)) {
            this->CommandToggleUISkillTree();
        }
        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_SITDOWN)) {
            this->CommandSitDown(true, !s_pPlayer->m_bSitDown);
        }

        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_HELP)) {
            if (m_pUIHelp->IsVisible()) {
                m_pUIHelp->SetVisible(false);
            } else {
                m_pUIHelp->SetVisible(true);
            }
        }

        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_COMMAND_LIST)) {
            this->CommandToggleCmdList();
        }

        if (s_pLocalInput->IsKeyPress(KM_TOGGLE_MINIMAP)) {
            this->CommandToggleUIMiniMap();
        }

        if (s_pLocalInput->IsKeyPress(DIK_PRIOR)) {
            if (m_pUIHotKeyDlg) {
                m_pUIHotKeyDlg->PageUp();
            }
        }

        if (s_pLocalInput->IsKeyPress(DIK_NEXT)) {
            if (m_pUIHotKeyDlg) {
                m_pUIHotKeyDlg->PageDown();
            }
        }

    } // end of if ( !m_UIChatDlg.IsChatMode() )

    // ..... ������ Ű���� ó��..
#if _DEBUG
    if (s_pLocalInput->IsKeyPress(DIK_F12)) { // ����� �׽�Ʈ..
        s_pEng->Lightning();                  // ���� ġ��..
    }
#endif
}

void CGameProcMain::ProcessPlayerInclination() // ��翡 �� ������..
{
    /*    float fFrm = GetIndepTimeFrame();

    __Vector3 vNorm, vNormXZ; 
    s_pTerrain->GetNormal( s_pEng->m_matPlayer.Pos().x, s_pEng->m_matPlayer.Pos().z, vNorm );

    vNorm.Normalize();
    vNormXZ = vNorm;
    vNormXZ.y = 0.0f;

    if ( vNormXZ.Magnitude() > 0.8f )
    {
        vNormXZ.x *= 0.01f*fFrm;
        vNormXZ.z *= 0.01f*fFrm;
        s_pPlayer->m_vPlayerIncline = vNormXZ;
    }*/
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Continous() // Ư�� ����(?)�Ͽ��� �������� ���������� �޽����� ����..
{
    float fTime = s_pEng->TimeGet();

    if (fTime >= m_fMsgSendTimeMove + PACKET_INTERVAL_MOVE) // 1�ʰ� ��������..
    {
        __Vector3 vPos = s_pPlayer->Position();
        if (m_vPlayerPosSended != vPos) {
            this->MsgSend_Move(true, true); // �����̶� ���������� ������ ��Ŷ ����..
        }
    }

    if (false == s_pPlayer->m_bMoveContinous &&
        fTime >= m_fMsgSendTimeRot + PACKET_INTERVAL_ROTATE) // �÷��̾ ������ �ְ�.. 2�ʰ� ��������..
    {
        float fYaw = s_pPlayer->Yaw();
        if (fYaw != m_fPlayerYawSended) {
            MsgSend_Rotation(); // �����̶� ȸ�������� ȸ�� ��Ŷ ����
        }
    }
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Attack(int iTargetID, float fInterval,
                                   float fDistance) // ���� ��Ŷ ������ - ���̺��� ���� �ֱ⸦ ���� �༭ ��ŷ�� ���´�.
{
    if (s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead()) {
        return; // ���� ���̴�..
    }

    BYTE byBuff[32];  // ����..
    int  iOffset = 0; // �ɼ�..

    BYTE bySuccess = true;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ATTACK); // ���� Ŀ���..
    CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);      // ??? ������??
    CAPISocket::MP_AddByte(byBuff, iOffset, bySuccess); // ���� ����.. - �ϴ� �������� ������.

    fInterval += 0.1f;

    CAPISocket::MP_AddShort(byBuff, iOffset, iTargetID);              // ���� ���̵�..
    CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fInterval * 100)); // ������ �ð�
    CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fDistance * 10));  // ������ �Ÿ�

    s_pSocket->Send(byBuff, iOffset); // ����..
}

void CGameProcMain::MsgSend_Move(bool bMove, bool bContinous) {
    __Vector3 vPos(0, 0, 0);
    float     fSpeed = s_pPlayer->MoveSpeed();

    BYTE byMoveFlag = 0;

    if (true == bMove) // �����̱� �����Ҷ��� �����϶���
    {
        if (s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead()) {
            return; // ���� ���̴�..
        }

        vPos = s_pPlayer->NextPos(PACKET_INTERVAL_MOVE); // ���� 1������ ��ġ�� ����ؼ�.
        byMoveFlag |= 0x01;
    } else // �����ÿ���
    {
        s_pPlayer->m_bTargetOrPosMove = false;
        s_pPlayer->m_iMoveTarget = -1;
        vPos = s_pPlayer->Position();
        fSpeed = 0;
    }

    if (true == bContinous) // ���������� �����̴� �Ŷ��..
    {
        byMoveFlag |= 0x02;
    }

    BYTE byBuff[64];  // ���� ����..
    int  iOffset = 0; // �ɼ�..

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_MOVE);     // Ŀ���..
    CAPISocket::MP_AddWord(byBuff, iOffset, vPos.x * 10); // ���� ��ġ
    CAPISocket::MP_AddWord(byBuff, iOffset, vPos.z * 10);
    CAPISocket::MP_AddShort(byBuff, iOffset, vPos.y * 10);
    CAPISocket::MP_AddWord(byBuff, iOffset, fSpeed * 10); // �ӵ�
    CAPISocket::MP_AddByte(byBuff, iOffset, byMoveFlag);  // ������ �÷���..
    s_pSocket->Send(byBuff, iOffset);                     // ��Ŷ�� ����..

    m_vPlayerPosSended = s_pPlayer->Position(); // �ֱٿ� ���� ��ġ ����..

    if (true == bMove) {
        m_fMsgSendTimeMove = CN3Base::TimeGet(); // �ð��� ����Ѵ�..
    }
}

void CGameProcMain::MsgSend_Rotation() {
    if (s_pPlayer->IsDead()) {
        return; // ���� ���� ��� ����!!
    }

    BYTE byBuff[8];
    int  iOffset = 0;

    float fYaw = s_pPlayer->Yaw(); // ����..

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ROTATE);
    CAPISocket::MP_AddShort(byBuff, iOffset, fYaw * 100);

    s_pSocket->Send(byBuff, iOffset);

    m_fPlayerYawSended = fYaw;              // ȸ������ ���
    m_fMsgSendTimeRot = CN3Base::TimeGet(); // �ð��� ����Ѵ�..
}

void CGameProcMain::MsgSend_Chat(e_ChatMode eMode, const std::string & szChat) {
    if (szChat.empty() || szChat.size() >= 128) {
        return;
    }
    if (eMode == N3_CHAT_CLAN && s_pPlayer->m_InfoExt.iKnightsID <= 0) {
        return;
    }

    BYTE byBuff[512];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_CHAT);
    CAPISocket::MP_AddByte(byBuff, iOffset, eMode);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)szChat.size());
    CAPISocket::MP_AddString(byBuff, iOffset, szChat);

    __ASSERT(iOffset < 512, "Send Buffer OverFlow");
    s_pSocket->Send(byBuff, iOffset); // ������..
}

void CGameProcMain::MsgSend_ChatSelectTarget(const std::string & szTargetID) {
    if (szTargetID.empty() || szTargetID.size() > 20) {
        return;
    }

    int  iOffset = 0;
    BYTE byBuff[32];

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_CHAT_SELECT_TARGET);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)szTargetID.size());
    CAPISocket::MP_AddString(byBuff, iOffset, szTargetID);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Regen() {
    if (s_pPlayer->m_iSendRegeneration >= 2) {
        return; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    }

    BYTE byBuff[4];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_REGENE);
    CAPISocket::MP_AddByte(byBuff, iOffset, 1); //1: ������ ��Ƴ���..

    CLogWriter::Write("Send Regeneration");

    s_pSocket->Send(byBuff, iOffset); // ������..

    s_pPlayer->m_iSendRegeneration = 2; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    TRACE("���� - �ٽ� ��Ƴ���\n");
}

bool CGameProcMain::MsgSend_RequestItemBundleOpen(CPlayerNPC * pCorpse) {
    if (NULL == pCorpse || pCorpse->m_iDroppedItemID <= 0) {
        return false;
    }

    float fDistTmp = (pCorpse->Position() - s_pPlayer->Position()).Magnitude();
    if (fDistTmp >= (pCorpse->Radius() * 2.0f + 6.0f)) {
        return false;
    }

    int iItemBundleID = pCorpse->m_iDroppedItemID;
    m_pUIDroppedItemDlg->m_iItemBundleID = pCorpse->m_iDroppedItemID;

    s_pOPMgr->CorpseRemove(pCorpse, false); // ���� �����ϰ� ���ش�..

    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_BUNDLE_OPEN_REQUEST);
    CAPISocket::MP_AddDword(byBuff, iOffset, iItemBundleID);

    s_pSocket->Send(byBuff, iOffset); // ������..

    return true;
}

void CGameProcMain::MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo) {
    BYTE byBuff[4];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
    //    CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_PERMIT);
    CAPISocket::MP_AddByte(byBuff, iOffset, bYesNo);

    s_pSocket->Send(byBuff, iOffset); // ������..
}

bool CGameProcMain::MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string & szID) {
    if (szID.empty() || szID.size() > 20) {
        return false;
    }

    bool          bIAmLeader, bIAmMember;
    int           iMemberIndex = -1;
    CPlayerBase * pTarget = NULL;
    this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);
    if (true == bIAmMember && false == bIAmLeader) {
        return false; // ���� ��Ƽ�� ��� �ְ� ������ �ƴϸ� ����..
    }

    BYTE byBuff[32];
    int  iOffset = 0;

    e_SubPacket_Party eCmdParty = N3_SP_PARTY_OR_FORCE_CREATE;
    if (m_pUIPartyOrForce->MemberCount() >= 2) {
        eCmdParty = N3_SP_PARTY_OR_FORCE_INSERT;
    }

    m_pUIPartyOrForce->m_iPartyOrForce = iPartyOrForce;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
    //    CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
    CAPISocket::MP_AddByte(byBuff, iOffset, eCmdParty);
    CAPISocket::MP_AddShort(byBuff, iOffset, szID.size());
    CAPISocket::MP_AddString(byBuff, iOffset, szID);

    s_pSocket->Send(byBuff, iOffset); // ������..

    if (m_pUIPartyOrForce->MemberCount() <= 0) // ó�� �����ϴ� ���...
    {
        m_pUIPartyOrForce->MemberAdd(s_pPlayer->IDNumber(), s_pPlayer->IDString(), s_pPlayer->m_InfoBase.iLevel,
                                     s_pPlayer->m_InfoBase.eClass, s_pPlayer->m_InfoBase.iHP,
                                     s_pPlayer->m_InfoBase.iHPMax); // ���� �̸� �־� ���´�..
    }

    TRACE("Party or Force ���� ��û - Target ID(%s)\n", szID.c_str());

    return true;
}

void CGameProcMain::MsgSend_PartyOrForceLeave(int iPartyOrForce) {
    if (m_pUIPartyOrForce->MemberCount() <= 0) {
        return; // ��Ƽ���� ����..
    }

    CPlayerBase * pTarget = NULL;
    bool          bIAmLeader, bIAmMember;
    int           iMemberIndex = -1;
    this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);

    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
    //    CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
    if (bIAmLeader) // ���� �����ϰ��..
    {
        if (iMemberIndex > 0 && pTarget) // ��Ƽ���̴�.. �ѾƳ���..
        {
            CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
            CAPISocket::MP_AddShort(byBuff, iOffset, pTarget->IDNumber());
        } else // Ÿ���� ��Ƽ���� �ƴϸ� ��Ƽ �ǰ���..
        {
            CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_DESTROY);
        }
    } else if (bIAmMember) // ������ �ƴϸ� Ż�� �޽����� ������..
    {
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
        CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->IDNumber());
    }
    s_pSocket->Send(byBuff, iOffset); // ������..
}

void CGameProcMain::MsgSend_ObjectEvent(int iEventID, int iNPCID) {
    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_OBJECT_EVENT);
    CAPISocket::MP_AddShort(byBuff, iOffset, iEventID); // Index
    CAPISocket::MP_AddShort(byBuff, iOffset, iNPCID);   // Parameter

    s_pSocket->Send(byBuff, iOffset); // ������..
}

void CGameProcMain::MsgSend_Weather(int iWeather, int iPercent) {
    if (iWeather < 1 || iWeather > 3) {
        return;
    }
    if (iPercent < 0 || iPercent > 100) {
        return;
    }

    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_WEATHER);       // -> byte - ����.... 0x01 - ����.. 0x02 -  �� 0x03
    CAPISocket::MP_AddByte(byBuff, iOffset, iWeather);         // -> byte - ����.... 0x01 - ����.. 0x02 -  �� 0x03
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iPercent); // short -> ������ �Ȱ�, ��, �� �� �� �ۼ�Ʈ��

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Time(int iHour, int iMin) {
    BYTE byBuff[12];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_TIME);
    CAPISocket::MP_AddShort(byBuff, iOffset, 0); // year
    CAPISocket::MP_AddShort(byBuff, iOffset, 0); // month
    CAPISocket::MP_AddShort(byBuff, iOffset, 0); // day
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iHour);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iMin);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Administrator(e_SubPacket_Administrator eSP, const std::string & szID) {
    if (szID.empty() || szID.size() >= 20) {
        return;
    }

    BYTE byBuff[64];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ADMINISTRATOR); // ������ ������Ŷ..
    CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
    CAPISocket::MP_AddShort(byBuff, iOffset, szID.size());
    CAPISocket::MP_AddString(byBuff, iOffset, szID);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoinReq(bool bJoin) {
    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // ������ ������Ŷ..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN_REQ);
    CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)bJoin);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_iJoinReqClanRequierID);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_iJoinReqClan);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoin(int iTargetID) {
    BYTE byBuff[4];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // ������ ������Ŷ..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iTargetID);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsLeave(std::string & szName) {
    BYTE byBuff[64];
    int  iOffset = 0;

    int iLen = szName.size();

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // ������ ������Ŷ..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_MEMBER_REMOVE);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iLen);
    CAPISocket::MP_AddString(byBuff, iOffset, szName); // ���̵� ���ڿ� ��Ŷ�� �ֱ�..
    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsWithdraw() {
    BYTE byBuff[2];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_WITHDRAW);
    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsAppointViceChief(std::string & szName) {
    BYTE byBuff[64];
    int  iOffset = 0;

    int iLen = szName.size();

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // ������ ������Ŷ..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_APPOINT_VICECHIEF);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iLen);
    CAPISocket::MP_AddString(byBuff, iOffset, szName); // ���̵� ���ڿ� ��Ŷ�� �ֱ�..
    s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::MsgRecv_MyInfo_All(DataPack * pDataPack, int & iOffset) {
    int iZone = s_pPlayer->m_InfoExt.iZoneCur;
    s_pPlayer->Release(); // �ϴ� ��â �� ���� �ϰ�....
    s_pPlayer->m_InfoExt.iZoneCur = iZone;

    int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    int         iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    std::string szID;
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);
    s_pPlayer->IDSet(iID, szID, D3DCOLOR_XRGB(100, 210, 255)); // ���� �Ķ����� �ϴû� �߰�..

    float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;

    s_pPlayer->m_InfoBase.eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoBase.eRace = (e_Race)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoBase.eClass = (e_Class)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iFace = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �� ���..
    s_pPlayer->m_InfoExt.iHair = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �Ӹ�ī��

    __TABLE_PLAYER_LOOKS * pLooks =
        s_pTbl_UPC_Looks->Find(s_pPlayer->m_InfoBase.eRace); // User Player Character Skin ����ü ������..
    if (NULL == pLooks) {
        CLogWriter::Write("CGameProcMain::MsgRecv_MyInfo_All : failed find character resource data (Race : %d)",
                          s_pPlayer->m_InfoBase.eRace);
    }
    __ASSERT(pLooks, "failed find character resource data");
    s_pPlayer->InitChr(pLooks); // ���� ����..

    s_pPlayer->m_InfoExt.iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iTitle = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoBase.iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
    s_pPlayer->m_InfoExt.iBonusPointRemain =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���� ���ʽ� ����Ʈ..

    s_pPlayer->m_InfoExt.iExpNext = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iExp = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRealmPoint = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iCity = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string   szKnightsName;
    int           iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� ID
    e_KnightsDuty eKnightsDuty =
        (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);    // ���ܿ����� ����..
    int iKnightNameLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� �̸� ����.
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iKnightNameLen);
    int iKnightsGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �Ҽ� ���� ���
    int iKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);  // �Ҽ� ���� ����

    // ���� ���� ����..
    s_pPlayer->m_InfoExt.eKnightsDuty = eKnightsDuty; // ���ܿ����� ����..
    s_pPlayer->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);
    m_pUIVar->UpdateKnightsInfo();

    s_pPlayer->m_InfoBase.iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoBase.iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iMSP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iWeight = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    s_pPlayer->m_InfoExt.iStrength = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iStrength_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iStamina = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iStamina_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iDexterity = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iDexterity_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iIntelligence = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iIntelligence_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iMagicAttak = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iMagicAttak_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    s_pPlayer->m_InfoExt.iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    //    s_pPlayer->m_InfoExt.iAttack_Delta        = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iGuard = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    //    s_pPlayer->m_InfoExt.iGuard_Delta        = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistFire = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistCold = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistLight = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistMagic = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistCurse = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iRegistPoison = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    s_pPlayer->m_InfoExt.iGold = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoBase.iAuthority = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); //����..

    // ��ų UI ����..
    for (int i = 0; i < 9; i++) {
        m_pUISkillTreeDlg->m_iSkillInfo[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    }
    m_pUISkillTreeDlg->InitIconUpdate();
    m_pUIHotKeyDlg->ReleaseItem();
    m_pUIHotKeyDlg->InitIconUpdate(); // ��Ű�� ��ȿ���� �˻��ϰ� ��ȿ�ϸ� ������Ʈ������ �о�´�..

    // �����ϰ� �ִ� ��..
    int iItemIDInSlots[ITEM_SLOT_COUNT];
    memset(iItemIDInSlots, -1, sizeof(iItemIDInSlots));
    int iItemDurabilityInSlots[ITEM_SLOT_COUNT];
    memset(iItemDurabilityInSlots, -1, sizeof(iItemDurabilityInSlots));
    int iItemCountInSlots[ITEM_SLOT_COUNT];
    memset(iItemCountInSlots, -1, sizeof(iItemCountInSlots));

    for (int i = 0; i < ITEM_SLOT_COUNT; i++) // ���� ������ŭ..
    {
        iItemIDInSlots[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iItemDurabilityInSlots[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        iItemCountInSlots[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    }

    m_fMsgSendTimeMove = 0; // Network ReQuest Ÿ�̸� �ʱ�ȭ..
    m_fMsgSendTimeRot = 0;
    m_fPlayerYawSended = 0;                     // �ֱٿ� �޽����� ���� ������ �÷��̾� y �� ȸ����.
    m_vPlayerPosSended = s_pPlayer->Position(); // �ֱٿ� �޽����� ���� ������ �÷��̾� ��ġ.

    // ����â ��ġ�� ��� ����
    if (m_pUIVar->m_pPageState) {
        m_pUIVar->m_pPageState->UpdateID(szID); // �̸� ����.
    }
    m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt));

    //__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
    //if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);

    // ���� �� ����
    m_pUIStateBarAndMiniMap->UpdateExp(s_pPlayer->m_InfoExt.iExp, s_pPlayer->m_InfoExt.iExpNext, true);
    m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, true);
    m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, true);

    m_pUIPartyOrForce->MemberInfoReInit(); // ��Ƽ â.. ����..

    __TABLE_ITEM_BASIC * pItem = NULL;    // ������ ���̺� ����ü ������..
    __TABLE_ITEM_EXT *   pItemExt = NULL; // ������ ���̺� ����ü ������..

    int iItemIDInInventorys[MAX_ITEM_INVENTORY];
    memset(iItemIDInInventorys, -1, sizeof(iItemIDInInventorys));
    int iItemCountInInventorys[MAX_ITEM_INVENTORY];
    memset(iItemCountInInventorys, -1, sizeof(iItemCountInInventorys));
    int iItemDurabilityInInventorys[MAX_ITEM_INVENTORY];
    memset(iItemDurabilityInInventorys, -1, sizeof(iItemDurabilityInInventorys));

    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) // ���� ������ŭ..
    {
        iItemIDInInventorys[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iItemDurabilityInInventorys[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        iItemCountInInventorys[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    }

    m_pUIInventory->ReleaseItem();

    std::string szResrcFN, szIconFN;
    for (int i = 0; i < ITEM_SLOT_COUNT; i++) // ���� ������ŭ..
    {
        if (0 == iItemIDInSlots[i]) {
            continue;
        }

        pItem = s_pTbl_Items_Basic->Find(iItemIDInSlots[i] / 1000 * 1000); // �� ������ ���..
        if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
            pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemIDInSlots[i] % 1000); // �� ������ ���..
        } else {
            pItemExt = NULL;
        }

        if (NULL == pItem || NULL == pItemExt) {
            N3_WARN("NULL slot Item!!!");
            CLogWriter::Write("MyInfo - slot - Unknown Item %d, IDNumber", iItemIDInSlots[i]);
            continue; // �������� ������..
        }

        e_PartPosition ePart;
        e_PlugPosition ePlug;
        e_ItemType     eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, &szResrcFN, &szIconFN, ePart,
                                                                       ePlug); // �����ۿ� ���� ���� �̸��� ����
        if (ITEM_TYPE_UNKNOWN == eType) {
            CLogWriter::Write("MyInfo - slot - Unknown Item");
        }
        __ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item Type");
        e_ItemSlot eSlot = (e_ItemSlot)i;

        switch (eSlot) {
        case ITEM_SLOT_EAR_RIGHT:
        case ITEM_SLOT_EAR_LEFT:
        case ITEM_SLOT_NECK:
        case ITEM_SLOT_RING_RIGHT:
        case ITEM_SLOT_RING_LEFT: {
            if (ITEM_TYPE_ICONONLY != eType) {
                CLogWriter::Write("MyInfo - slot - Invalid Item");
            }
            __ASSERT(ITEM_TYPE_ICONONLY == eType, "Invalid Item");
        } break;
        case ITEM_SLOT_UPPER:  // Parts - ��ü
        case ITEM_SLOT_LOWER:  // Parts - ��ü
        case ITEM_SLOT_GLOVES: // �尩
        case ITEM_SLOT_SHOES:  // �Ź�
        case ITEM_SLOT_HEAD:   // �Ӹ�ī�� ?? -> ���� ??
        {
            if (ITEM_TYPE_PART != eType) {
                CLogWriter::Write("MyInfo - slot - Invalid Item");
            }
            __ASSERT(ITEM_TYPE_PART == eType, "Invalid Item");
            s_pPlayer->PartSet(ePart, szResrcFN, pItem, pItemExt); // ��Ʈ�� ����..
        } break;
        case ITEM_SLOT_HAND_RIGHT: // ������
        case ITEM_SLOT_HAND_LEFT:  // �޼�
        {
            if (ITEM_TYPE_PLUG != eType) {
                CLogWriter::Write("MyInfo - slot - Invalid Item");
            }
            __ASSERT(ITEM_TYPE_PLUG == eType, "Invalid Item");

            e_PlugPosition ePlugPos;
            if (ITEM_SLOT_HAND_RIGHT == eSlot) {
                ePlugPos = PLUG_POS_RIGHTHAND;
            } else {
                ePlugPos = PLUG_POS_LEFTHAND;
            }
            s_pPlayer->PlugSet(ePlugPos, szResrcFN, pItem, pItemExt); // ��Ʈ�� ����..
        } break;
        case ITEM_SLOT_SHOULDER: // ����
        {
        } break;
        case ITEM_SLOT_BELT: {
        } break;
        }

        s_pPlayer->DurabilitySet(eSlot, iItemDurabilityInSlots[eSlot]); // ���� ���� ó��..

        __IconItemSkill * spItem = new __IconItemSkill;
        spItem->pItemBasic = pItem;
        spItem->pItemExt = pItemExt;
        spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
        spItem->iCount = iItemCountInSlots[i];
        spItem->iDurability = iItemDurabilityInSlots[i];

        // �κ��丮 ���Կ� �ִ´�.
        m_pUIInventory->m_pMySlot[i] = spItem;
        TRACE("Init Inv Msg Slot %d \n", iItemIDInSlots[i]);
    }

    // �κ��丮..
    int iItemCount = 0;
    for (int i = 0; i < MAX_ITEM_INVENTORY; i++) // �κ��丮 ������ŭ..
    {
        if (!iItemIDInInventorys[i]) {
            continue;
        }

        pItem = s_pTbl_Items_Basic->Find(iItemIDInInventorys[i] / 1000 * 1000); // �� ������ ���..
        pItemExt = NULL;
        if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
            pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemIDInInventorys[i] % 1000); // �� ������ ���..
        }
        if (NULL == pItem || NULL == pItemExt) {
            N3_WARN("NULL Inv Item");
            CLogWriter::Write("MyInfo - Inv - Unknown Item %d, IDNumber", iItemIDInInventorys[i]);
            continue; // �������� ������..
        }

        e_PartPosition ePart;
        e_PlugPosition ePlug;
        e_ItemType     eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart,
                                                                       ePlug); // �����ۿ� ���� ���� �̸��� ����
        if (ITEM_TYPE_UNKNOWN == eType) {
            CLogWriter::Write("MyInfo - slot - Unknown Item");
        }
        __ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item");

        __IconItemSkill * spItem = new __IconItemSkill;
        spItem->pItemBasic = pItem;
        spItem->pItemExt = pItemExt;
        spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
        spItem->iCount = iItemCountInInventorys[i];
        spItem->iDurability = iItemDurabilityInInventorys[i];

        m_pUIInventory->m_pMyInvWnd[i] = spItem;
        TRACE("Init Inv Msg Inve %d, iOrder %d \n", iItemIDInInventorys[i], i);
    }

    // Inventory & Slot Icon Operation.. ^^
    m_pUIInventory->InitIconUpdate();

    if (m_pUISkillTreeDlg) {
        m_pUISkillTreeDlg->UpdateDisableCheck();
    }
    if (m_pUIHotKeyDlg) {
        m_pUIHotKeyDlg->UpdateDisableCheck();
    }

    if (s_pPlayer->Part(PART_POS_UPPER)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->PartSet(PART_POS_UPPER, pLooks->szPartFNs[PART_POS_UPPER], NULL, NULL); // ��ü..
    }
    if (s_pPlayer->Part(PART_POS_LOWER)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER], NULL, NULL); // ��ü..
    }
    if (s_pPlayer->Part(PART_POS_HANDS)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->PartSet(PART_POS_HANDS, pLooks->szPartFNs[PART_POS_HANDS], NULL, NULL); // ��..
    }
    if (s_pPlayer->Part(PART_POS_FEET)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->PartSet(PART_POS_FEET, pLooks->szPartFNs[PART_POS_FEET], NULL, NULL); // �ٸ�..
    }
    if (s_pPlayer->Part(PART_POS_FACE)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->InitFace();
    }
    if (s_pPlayer->Part(PART_POS_HAIR_HELMET)->FileName().empty()) { // �ƹ��͵� ���þȵǾ� ������ ���� �̸��� ����..
        s_pPlayer->InitHair();
    }

    s_pPlayer->InventoryChrAnimationInitialize();
    s_pPlayer->SetSoundAndInitFont();

    //    s_pPlayer->SettingCollisionCheckPoint();

    ////////////////////////////////////////////////////////////
    // �⺻�� �б�..
    int iRun = 1;
    //    if(false == CGameProcedure::RegGetSetting("UserRun", &iRun, 4)) iRun = 1; // �Ȱ� �ٴ� ���¸� ������Ʈ������ �а�.. �⺻���� �ٴ� �����̴�..
    //    if(1 == iRun)
    this->CommandToggleWalkRun(); // �ٰ� �����..

    e_ViewPoint eVP = VP_THIRD_PERSON;
    if (false == CGameProcedure::RegGetSetting("CameraMode", &eVP, 4)) {
        eVP = VP_THIRD_PERSON; // ī�޶� ���� ���
    }
    s_pEng->ViewPointChange(eVP);
    // �⺻�� �б�..
    ////////////////////////////////////////////////////////////

    this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.

    // berserk temp
    //s_pPlayer->PlugSet(PLUG_POS_BACK, "item/babacloak.n3cplug_cloak", NULL);    // ��Ʈ�� ����..
    // end berserk temp

    // berserk
    //s_pPlayer->AttachCloak();

    //..
    s_pOPMgr->Release(); // �ٸ� ���� ���� Ŭ���� �ʱ�ȭ..

    m_bLoadComplete = TRUE; // �ε� ��..

    return true;
}

bool CGameProcMain::MsgRecv_Chat(DataPack * pDataPack, int & iOffset) {
    std::string szChat;                                                                     // ����..
    e_ChatMode  eCM = (e_ChatMode)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);   // ä�� Ÿ��
    e_Nation    eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ������� ����
    int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);              // �������
    int         iChatLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);         // ä�� ���ڿ� ����..
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szChat, iChatLen);

    if (eCM == N3_CHAT_CONTINUE_DELETE) { //���� ���� ����...
        m_pUIChatDlg->DeleteContinueMsg();
        return true;
    } else if (eCM == N3_CHAT_TITLE_DELETE) { //Ÿ��Ʋ ���� ����...
        m_pUIChatDlg->SetNoticeTitle("", 0xffffffff);
        return true;
    }

    D3DCOLOR crChat = 0xffffffff;
    switch (eCM) {
    case N3_CHAT_NORMAL:
        crChat = D3DCOLOR_ARGB(255, 255, 255, 255);
        break;
    case N3_CHAT_PRIVATE:
        crChat = D3DCOLOR_ARGB(255, 192, 192, 0);
        break;
    case N3_CHAT_PARTY:
        crChat = D3DCOLOR_ARGB(255, 0, 192, 192);
        break;
    case N3_CHAT_FORCE:
        crChat = D3DCOLOR_ARGB(255, 0, 192, 192);
        break;
    //case N3_CHAT_SHOUT:    crChat = D3DCOLOR_ARGB(255,255,0,0);        break;
    case N3_CHAT_SHOUT:
        crChat = D3DCOLOR_ARGB(255, 0xf8, 0x66, 0x05);
        break;
    case N3_CHAT_CLAN:
        crChat = D3DCOLOR_ARGB(255, 0, 255, 0);
        break;
    case N3_CHAT_PUBLIC:
        crChat = D3DCOLOR_ARGB(255, 255, 255, 0);
        break;
    case N3_CHAT_WAR:
        crChat = D3DCOLOR_ARGB(255, 255, 255, 0);
        break;
    case N3_CHAT_TITLE:
        crChat = D3DCOLOR_ARGB(255, 255, 255, 0);
        break;
    case N3_CHAT_CONTINUE:
        crChat = D3DCOLOR_ARGB(255, 255, 255, 0);
        break;
    }

    if (eCM == N3_CHAT_TITLE) {
        m_pUIChatDlg->SetNoticeTitle(szChat, crChat);
        return true;
    } else if (N3_CHAT_WAR == eCM) {
        if (m_pWarMessage) {
            m_pWarMessage->SetMessage(szChat, D3DFONT_BOLD, crChat);
        }
        return true;
    } else if (N3_CHAT_CONTINUE == eCM) {
        m_pUIChatDlg->AddContinueMsg(eCM, szChat, crChat);
        return true;
    }

    // �뿪 ����...           ��,.�� a
    if (N3_CHAT_NORMAL == eCM || N3_CHAT_PRIVATE == eCM || N3_CHAT_SHOUT == eCM) {
        if (eNation != s_pPlayer->m_InfoBase.eNation) {
            CPlayerBase * pTalker = s_pOPMgr->UPCGetByID(iID, false);
            bool          bIamManager = (0 == s_pPlayer->m_InfoBase.iAuthority) ? true : false;
            bool          bTalkerIsManager = (pTalker && 0 == pTalker->m_InfoBase.iAuthority) ? true : false;

            if (!(bIamManager || bTalkerIsManager)) // ���� ��ڰ� �ƴϰ� ���浵 ��ڰ� �ƴϸ�
            {
                int i = szChat.find(':');
                if (i >= 0) {
                    for (; i < iChatLen; i++) {
                        szChat[i] = '!' + rand() % 10; // �̻��� ���� �ٲ۴�..
                    }
                }
            }
        }
    }

    // ǳ���� �ֱ�..
    CPlayerBase * pBPC = NULL;
    if (iID == s_pPlayer->IDNumber()) {
        pBPC = s_pPlayer;
    } else {
        pBPC = s_pOPMgr->CharacterGetByID(iID, false);
    }
    if (pBPC && s_pPlayer->Distance(pBPC->Position()) < 4.0f &&
        (N3_CHAT_NORMAL == eCM || N3_CHAT_SHOUT == eCM)) { // ���� ä�� Ȥ�� ��ġ���϶���..
        pBPC->BalloonStringSet(szChat, crChat);
    }

    // ä��â�� �ֱ�..
    m_pUIChatDlg->AddChatMsg(eCM, szChat, crChat);

    return true;
}

bool CGameProcMain::MsgRecv_UserMove(DataPack * pDataPack, int & iOffset) {
    int   iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) /
               10.0f; // ����ϰų� �̵����϶����� ���� ��ġ. �����Ҷ��� ���� ��ġ�� �޴´�.
    float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;
    float fSpeed = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) /
                   10.0f; // ����ϰų� �̵����϶����� �����̴� �ӵ�. �����Ҷ��� 0 �� �´�.
    BYTE byMoveFlag =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �����̴� �÷���.. 0 ���� 1 ���, 2 ��� ������

    // �Լ��� �;� �� �κ�.. ^^
    // ���̵�, �÷��̾� ���� 1, �÷��̾� ���� 2, ���� xzy ��ġ, ���� xzy ����, 1�ʵ� x, z, y dnlcl..

    if (0xff == byMoveFlag) // ������ġ ������Ʈ ��Ŷ�̴�..
    {
        if (iID == s_pPlayer->IDNumber()) // �� �Ž�����..
        {
            this->InitPlayerPosition(
                __Vector3(fX, fY, fZ)); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
        } else {
            CPlayerNPC * pBPC = s_pOPMgr->CharacterGetByID(iID, true); // �ٸ� �÷��̾ ��..
            if (NULL == pBPC) {
                this->MsgSend_UserInRequest(iID); // User ������ ���� ��� ��û�Ѵ�..
                return false;
            } else if (pBPC->IsDead()) {
                return false;
            }

            pBPC->MoveTo(fX, fY, fZ, 0, 0); // ���� ��ġ..
            pBPC->PositionSet(__Vector3(fX, fY, fZ), true);
        }
        return false;
    }

    if (iID == s_pPlayer->IDNumber()) // �� �Ž�����..
    {
        s_pPlayer->m_vPosFromServer.Set(fX, fY, fZ);
        return false;
    }

    CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(iID, true); // �ٸ� �÷��̾ ��..
    if (NULL == pUPC) {
        return false;
    }

    //    if(0.0f != fSpeed) TRACE("Move - %4.1f ( %d : Mode %d ) %.2f��\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());
    //    else TRACE("Stop - %4.1f ( %d : Mode %d )  %.2f��\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());

    //    ������ ������ ���� ������ ������Ʈ..
    pUPC->MoveTo(fX, fY, fZ, fSpeed, byMoveFlag); // ���� ��ġ..

    return true;
}

bool CGameProcMain::MsgRecv_Rotation(DataPack * pDataPack, int & iOffset) {
    int   iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    float fYaw = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 100.0f;

    if (s_pPlayer->IDNumber() == iID) {
        return false;
    }

    CPlayerNPC * pBPC = NULL;
    pBPC = s_pOPMgr->CharacterGetByID(iID, true); // �ٸ� ���� ��..
    if (NULL == pBPC) {
        return false;
    }

    pBPC->RotateTo(fYaw, false); // ������ ���� ���� x, y, z������

    return true;
}

/*
bool CGameProcMain::MsgRecv_Dead(DataPack* pDataPack, int& iOffset)
{
    int iID    = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//    TRACE("Dead message Receive.. !!!!!!!! %d \n", iID);

    if ( iID == s_pPlayer->IDNumber() )
    {
        s_pPlayer->Action(PSA_PRE_DYING, true, NULL, true); // �÷��̾� ���̱�.
    }
    else
    {
        CPlayerOther* pReadyToDead = s_pOPMgr->PlayerGetByID(iID, true); // ��� �ִ� �ѵ��߿��� �˻��غ���..
        if(pReadyToDead) // ������ �ױ��������� �����.
        {
            pReadyToDead->Acttion(PSA_PRE_DYING, true, NULL, true);
        }
    }

    return true;
}
*/

bool CGameProcMain::MsgRecv_Regen(DataPack * pDataPack, int & iOffset) {
    __Vector3 vPosPlayer;
    vPosPlayer.x = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    vPosPlayer.z = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    vPosPlayer.y = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;

    this->InitPlayerPosition(vPosPlayer); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
    s_pPlayer->RegenerateCollisionMesh(); // �浹 �޽ø� �ٽ� �����..

    s_pPlayer->m_iSendRegeneration = 0; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    s_pPlayer->m_fTimeAfterDeath = 0;   // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    TRACE("���� - �ٽ� ��Ƴ���(%.1f, %.1f)\n", vPosPlayer.x, vPosPlayer.z);

    //
    //���� & ȿ�� �ʱ�ȭ..
    if (m_pUIStateBarAndMiniMap) {
        m_pUIStateBarAndMiniMap->ClearMagic();
    }
    if (m_pMagicSkillMng) {
        m_pMagicSkillMng->ClearDurationalMagic();
    }
    if (CGameProcedure::s_pFX) {
        s_pFX->StopMine();
    }

    CLogWriter::Write("Receive Regeneration");

    if (s_pPlayer->Nation() == NATION_KARUS) {
        CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
    } else if (s_pPlayer->Nation() == NATION_ELMORAD) {
        CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);
    }

    return true;
}

bool CGameProcMain::MsgRecv_Time(DataPack * pDataPack, int & iOffset) {
    short year = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    short month = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    short day = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    short hour = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    short min = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    ACT_WORLD->SetGameTimeWithSky(year, month, day, hour, min);

    return true;
}

bool CGameProcMain::MsgRecv_Weather(DataPack * pDataPack, int & iOffset) {
    int iWeather =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // -> byte - ����.... 0x01 - ����.. 0x02 -  �� 0x03
    int iPercent =
        CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // short -> ������ �Ȱ�, ��, �� �� �� �ۼ�Ʈ��

    ////////////////////////////////////////////////////
    // ���ǵ� �� üũ�� �ð�....
    /*    float fTimeFromServer = CAPISocket::Parse_GetFloat(pDataPack->m_pData, iOffset); // float -> ���������� �ִ� �ð�...
    float fTimeLocal = CN3Base::TimeGet();
    
    static float fTimeFromServerPrev = fTimeFromServer;
    static float fTimeLocalPrev = fTimeLocal;
    static int iTSH = 0; // Trying SpeedHacking...
    if(fTimeFromServer != 0.0f)
    {
        float fTDS = fTimeFromServer - fTimeFromServerPrev;
        if(fTDS > 355.0f && fTDS < 365.0f) // 6�п� �ѹ� �ְԲ� �Ǿ� �ִ�..
        {
            float fTDL = fTimeLocal - fTimeLocalPrev;
            if(fTDL < (fTDS * 0.8f) || fTDL > (fTDS * 1.2f)) // ���� Ÿ���� ���� Ÿ�Ӻ��� 20�ۼ�Ʈ �̻� ũ��..
            {
                iTSH++; // Try Speed Hacking ... ++
            }
            else 
            {
                iTSH = 0;
            }

            if(iTSH >= 3) CGameProcedure::s_bUseSpeedHack = true; // ���ǵ� ��ŷ....
        }

        fTimeFromServerPrev = fTimeFromServer;
        fTimeLocalPrev = fTimeLocal;
    }
    // ���ǵ� �� üũ�� �ð�....
    ////////////////////////////////////////////////////
*/

    if (iPercent < 20) {
        iPercent = 20;
    }
    if (iPercent > 100) {
        iPercent = 100;
    }
    float fPercent = iPercent / 100.0f;

    //enum {    GEW_CLEAR=0,        // ����
    //        GEW_DRIZZLE_RAIN=1,        // ������
    //        GEW_RAINY=2,            // ���緮�� ��
    //        GEW_HEAVY_RAIN=3,        // ����
    //        GEW_SNOW1=11,            // ������ ��
    //        GEW_SNOW2=12,            // ������ ��
    //        GEW_HEAVY_SNOW=13        // ����
    TRACE("���� - ����(%d - %d)\n", iWeather, iPercent);

    ACT_WORLD->SetWeatherWithSky((CN3SkyMng::eSKY_WEATHER)iWeather, iPercent); // �ϴ� ��ȭ�ϱ�
    float fDelta = 1.0f;
    if (0x1 == iWeather) {
        fDelta = 0.5f + (1.0f - fPercent) * 0.5f; // ���� ����. �ۼ�Ʈ�� �Ȱ�...
    } else if (0x02 == iWeather) {
        fDelta = 0.25f + (1.0f - fPercent) * 0.75f; // ��� .
    } else if (0x03 == iWeather) {
        fDelta = 0.25f + (1.0f - fPercent) * 0.75f; // ����..
    }

    s_pEng->FarPlaneDeltaSet(fDelta, false);

    return true;
}

bool CGameProcMain::MsgRecv_UserInOut(DataPack * pDataPack, int & iOffset) {
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    if (0x01 == iType) { // ���� ���ö�.(���� ���ӻ��� ���� ���� �����ϸ鼭 ����..)
        this->MsgRecv_UserIn(pDataPack, iOffset, false);
    } else if (0x02 == iType) { // User ������.
        this->MsgRecv_UserOut(pDataPack, iOffset);
    } else if (0x03 == iType) { // ���� ���ö� (�׾��� ��Ƴ��ų�, game start�ϴ� ������.)
        this->MsgRecv_UserIn(pDataPack, iOffset, true);
    } else if (0x04 == iType) { // ���� ���ö� (warp)
        this->MsgRecv_UserIn(pDataPack, iOffset, false);
    }

    return true;
}

bool CGameProcMain::MsgRecv_UserIn(DataPack * pDataPack, int & iOffset, bool bWithFX) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    std::string szName;
    int         iNameLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szName, iNameLen);

    e_Nation eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �Ҽ� ����. 0 �̸� ����. 1

    // ���� ����
    int           iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ���� ID
    e_KnightsDuty eKnightsDuty =
        (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);            // �Ҽ� ����. 0 �̸� ����. 1
    int         iKnightNameLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� �̸� ����.
    std::string szKnightsName;
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iKnightNameLen);
    int iKnightsGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���
    int iKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);  // ����

    int     iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ����...
    e_Race  eRace = (e_Race)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    e_Class eClass = (e_Class)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    float   fXPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float   fZPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float   fYPos = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;

    float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos); // ������ ���̰� ���..
    float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos),
                                                            1.0f); // ������Ʈ���� ���� ����� ���̰� ���..
    if (fYObject > fYTerrain) {
        fYPos = fYObject;
    } else {
        fYPos = fYTerrain;
    }

    int iFace = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �Ӹ�ī��..
    int iHair = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �� ���

    int iStatus =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 1 - ���ֱ�.. 2 - �ɾ��ֱ�.. 3 ... �׾��ִ�..
    int iStatusSize =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0 - ���� ũ��, 1 - Ŀ�� �ִ�. 2 - �۾�����..

    int  iRecruitParty = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 1 - ����. 2 - ��Ƽ ����..
    BYTE byAuthority = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);   // ����...

    DWORD dwItemIDs
        [MAX_ITEM_SLOT_OPC]; // ���� ������ - �ٸ� �÷��̾�(NPC ����) 0 ~ 4 ��ü,��ü,���,��,�� 5 ���� 6 ������ 7 �޼�
    int iItemDurabilities[MAX_ITEM_SLOT_OPC]; // ���� �������� ������..
    for (int i = 0; i < MAX_ITEM_SLOT_OPC; i++) {
        dwItemIDs[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // �����ϰ� �ִ� �����۵��� ID
        iItemDurabilities[i] =
            CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �����ϰ� �ִ� �����۵��� ���� ������
    }

    if (iID == s_pPlayer->IDNumber()) {
        return false; // �� ��Ŷ�̸� .. // �����Ѵ�..
    }

    CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(iID, false);
    if (pUPC) // �̹� ���̵� ���� ĳ���� ������..
    {
        CLogWriter::Write("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos,
                          fZPos);
        TRACE("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);

        pUPC->Action(PSA_BASIC, true, NULL, true); // ������ �츮��..
        pUPC->m_fTimeAfterDeath = 0;
        pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);
        return false;
    }

    D3DCOLOR crID = (eNation != s_pPlayer->m_InfoBase.eNation)
                        ? D3DCOLOR_XRGB(255, 96, 96)
                        : D3DCOLOR_XRGB(128, 128, 255); // ������ ���� �ٸ��� ����

    pUPC = new CPlayerOther();
    pUPC->IDSet(iID, szName, crID);
    pUPC->m_InfoBase.eNation = eNation;
    pUPC->m_InfoBase.eClass = eClass;
    pUPC->m_InfoBase.iLevel = iLevel;
    pUPC->m_InfoBase.iAuthority = byAuthority;
    pUPC->Init(eRace, iFace, iHair, dwItemIDs, iItemDurabilities);
    pUPC->RotateTo(D3DXToRadian(rand() % 360), true);
    pUPC->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);

    //__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKightsID);
    //if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);

    pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true); // �ٸ� �÷��̾� ���� ��ġ ����..
    pUPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);                 // ���� ��ġ..
    pUPC->RotateTo(D3DXToRadian(rand() % 360), true);
    s_pOPMgr->UPCAdd(pUPC); // ĳ���� �߰�...

    //if(bWithFX)
    if (bWithFX && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER) {
        if (eNation == NATION_KARUS) {
            CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_KARUS, iID, -1);
        } else if (eNation == NATION_ELMORAD) {
            CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_ELMORAD, iID, -1);
        }
    }

    // �ɾ� �ֱ�.., ���ֱ� ����..
    if (1 == iStatus) // ���ֱ�..
    {
        pUPC->Action(PSA_BASIC, true, NULL, true);
        pUPC->ActionMove(PSM_STOP);
    } else if (2 == iStatus) // �ɾ� �ֱ�..
    {
        pUPC->Action(PSA_SITDOWN, true, NULL, true);
        pUPC->ActionMove(PSM_STOP);
    } else if (3 == iStatus) // �׾� �ִ�..
    {
        s_pOPMgr->MoveToCorpsesForcely(pUPC, false); // ��ü�� �ű��..
    }

    if (0x01 == iStatusSize) {
        pUPC->ScaleSetGradually(1.0f); // ���� ũ��..
        pUPC->FlickerFactorSet(1.0f);  // ��Ȱ�Ǽ� ������.. ���ڰŸ���..
    } else if (0x02 == iStatusSize) {
        pUPC->ScaleSetGradually(2.0f); // Ŀ����..
    } else if (0x03 == iStatusSize) {
        pUPC->ScaleSetGradually(0.5f); // �۾�����.
    } else if (0x04 == iStatusSize) {
        pUPC->FlickerFactorSet(0.7f); // ��Ȱ�Ǽ� ������.. ���ڰŸ���..
    }

    if (1 == iRecruitParty) {
        pUPC->InfoStringSet("", 0);
    } else if (2 == iRecruitParty) // ��Ƽ ����..
    {
        int iLevel = pUPC->m_InfoBase.iLevel;
        int iLMin = iLevel - 8;
        if (iLMin < 0) {
            iLMin = 0;
        }
        int iLMax = iLevel + 8;
        if (iLMax > 60) {
            iLMax = 60;
        }

        char        szBuff[128];
        std::string szMsg;
        ::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
        sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
        pUPC->InfoStringSet(szBuff, 0xff00ff00);
    }

    return true;
}

bool CGameProcMain::MsgRecv_UserOut(DataPack * pDataPack, int & iOffset) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    //���� ������ ĳ���͸� ��ü�� �����.
    CPlayerNPC * pUPC = s_pOPMgr->UPCGetByID(iID, false);
    if (pUPC == NULL) {
        return false;
    }
    if (pUPC->IsDead()) { //ĳ���Ͱ� ���� ���¿��� UserOut�� �ϰ� �Ǹ� ��ü�� �ٲ��ش�.
        s_pOPMgr->CorpseAdd(iID);
        return true;
    }

    return s_pOPMgr->UPCDelete(iID); // ĳ���� ����...
}

// ���� ������ ��� ���̵� ī��Ʈ��ŭ �޴´�... �۱�.. ������Ʈ�� �ʿ��� �͸� �������� ��û..
bool CGameProcMain::MsgRecv_UserInAndRequest(DataPack * pDataPack, int & iOffset) {
    int iUPCCountReceived = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    //    TRACE("UPC region update : %d\n", iUPCCountReceived);

    if (0 == iUPCCountReceived) {
        return false;
    }
    if (iUPCCountReceived < 0 || iUPCCountReceived >= 1000) {
        char szErr[256];
        sprintf(szErr, "���� ��û ���� - �÷��̾� ���� %d", iUPCCountReceived);
        CGameProcedure::ReportDebugStringAndSendToServer(szErr);
        __ASSERT(0, szErr);
        return false;
    }

    int iID = 0;
    m_SetUPCID.clear(); // �� �����...
    it_ID   itID, itIDEnd = m_SetUPCID.end();
    pair_ID pairID;

    for (int i = 0; i < iUPCCountReceived; i++) {
        iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        //        TRACE("               ID : %d\n", iID);
        if (iID == s_pPlayer->IDNumber()) {
            continue; // �� ���̵��̴�.. ���� ������ �ȵȴ�.
        }

        pairID = m_SetUPCID.insert(iID);
    }

    if (m_SetUPCID.empty()) // ���� ������ �Ѱ��� ���ٸ� ���� ������..
    {
        s_pOPMgr->ReleaseUPCs();
        return false;
    }

    // ���� ���� ���̵�� ����Ʈ�� �ִ� UPC ID �� �˻��ؼ�..
    CPlayerOther * pUPC = NULL;
    it_UPC         itUPC = s_pOPMgr->m_UPCs.begin(), itUPCEnd = s_pOPMgr->m_UPCs.end();
    for (; itUPC != itUPCEnd;) {
        pUPC = itUPC->second;
        iID = pUPC->IDNumber();

        itID = m_SetUPCID.find(iID);
        if (itID != itIDEnd) // ���� ���� ����Ʈ�� ������..
        {
            m_SetUPCID.erase(itID); // ��û�� ����Ʈ���� ����..
            itUPC++;
        } else // ���� ���°��� ������ �����..
        {
            //            TRACE("           delete : %d\n", iID);

            if (pUPC && pUPC->IsDead()) {
                itUPC++;
            } else {
                delete pUPC;
                pUPC = NULL;
                itUPC = s_pOPMgr->m_UPCs.erase(itUPC);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // �ٷ� ��û ��Ŷ�� ����� ������..
    int iNewUPCCount = m_SetUPCID.size();
    if (iNewUPCCount > 0) {
        int               iOffset = 0;                                     // ������ ������..
        std::vector<BYTE> byBuff(iNewUPCCount * 2 + 10, 0);                // ��Ŷ ����..
        CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, N3_REQUEST_USER_IN); // Ŀ���.
        CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewUPCCount);      // ���̵� ����..

        itID = m_SetUPCID.begin();
        itIDEnd = m_SetUPCID.end();
        for (int i = 0; itID != itIDEnd; itID++, i++) {
            iID = *itID;
            CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID); // �ڼ��� ������ �ʿ��� ���̵��..
        }
        s_pSocket->Send(&(byBuff[0]), iOffset); // ������
    }
    // �ٷ� ��û ��Ŷ�� ����� ������..
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

bool CGameProcMain::MsgRecv_UserInRequested(DataPack * pDataPack, int & iOffset) {
    int iPlayerCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    if (0 == iPlayerCount) {
        return false;
    }
    if (iPlayerCount < 0 || iPlayerCount >= 1000) {
        char szErr[256];
        sprintf(szErr, "�������� �ޱ� ���� - �÷��̾� ���� %d", iPlayerCount);
        CGameProcedure::ReportDebugStringAndSendToServer(szErr);
        __ASSERT(0, szErr);
        return false;
    }

#ifdef _DEBUG
    float fTime = CN3Base::TimeGet();
#endif

    //    int iOffset2 = iOffset;
    for (int i = 0; i < iPlayerCount; i++) {
        this->MsgRecv_UserIn(pDataPack, iOffset); // �÷��̾� ���� ��ŭ ���� ��...
    }

#ifdef _DEBUG
    TRACE("  User In Requested (Count:%d) : Avg(%.4f Sec)\n", iPlayerCount,
          (CN3Base::TimeGet() - fTime) / iPlayerCount);
#endif

    return true;
}

bool CGameProcMain::MsgRecv_NPCInOut(DataPack * pDataPack, int & iOffset) {
    BYTE byType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    if (byType == 0x01) // NPC ���ö�
    {
        return this->MsgRecv_NPCIn(pDataPack, iOffset); // NPC In ó��
    } else                                              // NPC ������.
    {
        return this->MsgRecv_NPCOut(pDataPack, iOffset);
    }

    return true;
}

bool CGameProcMain::MsgRecv_NPCIn(DataPack * pDataPack, int & iOffset) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);         // Server���� �����ϴ� ���� ID
    int iIDResrc = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);    // ���ҽ� ID
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);        // NPC Type - 0x05 : ����
    int iItemTrdeID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // ������ �ŷ��� �׷� ID ������ ��û�� ID
    int iScale = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);      // ������ 100 �� 1.0
    int iItemID0 = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);    // ���ҽ� ID
    int iItemID1 = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);    // ���ҽ� ID
    int iNameLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    std::string szName; // NPC ���̵�..
    if (iNameLen > 0) {
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szName, iNameLen);
    } else {
        szName = "";
    }

    //#ifdef _DEBUG
    //    CLogWriter::Write("NPC In - ID(%d) Name(%s) Time(%.1f)", iID, szName.c_str(), CN3Base::TimeGet()); // ĳ�� ����..
    //#endif

    e_Nation eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // �Ҽ� ����. 0 �̸� ����. 1
    int      iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    float fXPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fZPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fYPos = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;

    float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos); // ������ ���̰� ���..
    float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos),
                                                            1.0f); // ������Ʈ���� ���� ����� ���̰� ���..
    if (fYObject > fYTerrain) {
        fYPos = fYObject;
    } else {
        fYPos = fYTerrain;
    }

    DWORD dwStatus = CAPISocket::Parse_GetDword(
        pDataPack->m_pData, iOffset); // ����... ���������� or �����ؼ� ����. 0 �� ����, 1 ����. 2, 4, 8, 16 ....
    DWORD dwType = CAPISocket::Parse_GetByte(pDataPack->m_pData,
                                             iOffset); // Ÿ��... 0 �̸� ĳ���� Ÿ�� NPC, 1 �̸� ������Ʈ Ÿ�� NPC

    CPlayerNPC * pNPC = s_pOPMgr->NPCGetByID(iID, false);
    if (pNPC) // �̹� ���̵� ���� ĳ���� ������..
    {
        CLogWriter::Write("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos,
                          fZPos);
        TRACE("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);
        pNPC->Action(PSA_BASIC, true, NULL, true); // ������ �츮��..
        pNPC->m_fTimeAfterDeath = 0;
        pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);
        return false;
    }

    D3DCOLOR crID =
        (eNation != s_pPlayer->m_InfoBase.eNation) ? D3DCOLOR_XRGB(255, 128, 128) : D3DCOLOR_XRGB(192, 192, 255);

    pNPC = new CPlayerNPC();
    pNPC->IDSet(iID, szName, crID); // �ʱ�ȭ.. �� ID ����.
    pNPC->m_InfoBase.eNation = eNation;
    pNPC->m_InfoBase.eRace = RACE_NPC; // NPC ��� �� �˸���.
    pNPC->m_InfoBase.iLevel = iLevel;
    pNPC->m_InfoBase.iAuthority = AUTHORITY_NPC; // ���� NPC�� ������ ����..

    s_pOPMgr->NPCAdd(pNPC); // ĳ���� �߰�...

    // ���� ��Ŷ�� ���� ĳ���͸� ġ��..(?) �����ش�.. ����������, ���� ������...
    CN3Shape *             pShape = NULL;
    __TABLE_PLAYER_LOOKS * pLooks = NULL;
    if (0 == dwType) {
        pLooks = s_pTbl_NPC_Looks->Find(iIDResrc); // �⺻ ��Ų..
        if (NULL == pLooks)                        // ĳ���� �⺻ ��� ���̺��� ������...
        {
            pLooks = s_pTbl_NPC_Looks->GetIndexedData(0);
            char szBuff[256];
            sprintf(szBuff, "Normal NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
            this->MsgOutput(szBuff, 0xffff00ff);
            CLogWriter::Write(szBuff);
        }
    } else {
        pShape = ACT_WORLD->ShapeGetByIDWithShape(iIDResrc); // Object NPC ���� ã�� ����...
        if (NULL == pShape) {
            char szBuff[256];
            sprintf(szBuff, "Object NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
            this->MsgOutput(szBuff, 0xffff00ff);
            CLogWriter::Write(szBuff);
            pLooks = s_pTbl_NPC_Looks->GetIndexedData(0);
        }
    }

    if (pLooks) {
        pNPC->InitChr(pLooks); // ���� ����..
        pNPC->ScaleSet(iScale / 100.0f);

        std::string    szPartFN;
        e_PartPosition eParts[6] = {PART_POS_UPPER, PART_POS_LOWER, PART_POS_FACE,
                                    PART_POS_HANDS, PART_POS_FEET,  PART_POS_HAIR_HELMET};
        for (int i = 0; i < 6; i++) {
            pNPC->PartSet(eParts[i], pLooks->szPartFNs[eParts[i]], NULL, NULL);
        }

        if (iItemID0) {
            __TABLE_ITEM_BASIC * pItem0 = s_pTbl_Items_Basic->Find(iItemID0 / 1000 * 1000);
            __TABLE_ITEM_EXT *   pItemExt0 = NULL;
            if (pItem0 && pItem0->byExtIndex >= 0 && pItem0->byExtIndex < MAX_ITEM_EXTENSION) {
                pItemExt0 = s_pTbl_Items_Exts[pItem0->byExtIndex]->Find(iItemID0 % 1000); // �� ������ ���..
            }
            if (pItem0 && pItemExt0) {
                e_PartPosition ePart;
                e_PlugPosition ePlug;
                std::string    szItemFN;
                CGameProcedure::MakeResrcFileNameForUPC(pItem0, &szItemFN, NULL, ePart, ePlug);
                pNPC->PlugSet(PLUG_POS_RIGHTHAND, szItemFN, pItem0, pItemExt0);
            } else {
                __ASSERT(0, "Invalid Item ID And Extension");
            }
        }

        if (iItemID1) {
            __TABLE_ITEM_BASIC * pItem1 = s_pTbl_Items_Basic->Find(iItemID1 / 1000 * 1000);
            __TABLE_ITEM_EXT *   pItemExt1 = NULL;
            if (pItem1 && pItem1->byExtIndex >= 0 && pItem1->byExtIndex < MAX_ITEM_EXTENSION) {
                pItemExt1 = s_pTbl_Items_Exts[pItem1->byExtIndex]->Find(iItemID1 % 1000); // �� ������ ���..
            }
            if (pItem1 && pItemExt1) {
                e_PartPosition ePart;
                e_PlugPosition ePlug;
                std::string    szItemFN;
                CGameProcedure::MakeResrcFileNameForUPC(pItem1, &szItemFN, NULL, ePart, ePlug);
                pNPC->PlugSet(PLUG_POS_LEFTHAND, szItemFN, pItem1, pItemExt1);
            } else {
                N3_WARN("Invalid Item ID And Extension");
            }
        }
    } else if (pShape && (pShape->Type() & OBJ_SHAPE_EXTRA)) {
        CN3ShapeExtra * pSE = (CN3ShapeExtra *)pShape;
        pNPC->m_pShapeExtraRef = pSE; // ���� ������Ʈ ����..
        pSE->m_bVisible = true;       // ������ ���������� �ʰ� �ϴ� �÷��� ����..

        bool      bShouldBeRotate = true;
        __Vector3 vAxis(0, 1, 0);
        float     fRadian = 0, fRadian2 = 0;
        bool      bVisible = false;

        if (OBJECT_TYPE_DOOR_LEFTRIGHT == pSE->m_iEventType) // �¿쿭�� ����
        {
            vAxis.Set(0, 1, 0);
            fRadian = D3DXToRadian(80);
            fRadian2 = 0;
        } else if (OBJECT_TYPE_DOOR_TOPDOWN == pSE->m_iEventType) {
            vAxis.Set(0, 0, 1);
            fRadian = D3DXToRadian(90);
            fRadian2 = 0;
        } // ���Ͽ��� ����
        else if (OBJECT_TYPE_LEVER_TOPDOWN == pSE->m_iEventType) {
            vAxis.Set(1, 0, 0);
            fRadian = D3DXToRadian(45);
            fRadian2 = D3DXToRadian(-45);
        } // ���� ����
        else if (OBJECT_TYPE_FLAG == pSE->m_iEventType) {
            bShouldBeRotate = false;
        }

        if (0x01 == dwStatus) // ���̰� ����ų� �¿�� ���� �ִ� ���..
        {
            if (bShouldBeRotate) // ����
            {
                pSE->RotateTo(0, vAxis, fRadian, 1, true);  // �ٷ� ����.
                pSE->RotateTo(1, vAxis, -fRadian, 1, true); // �ٷ� ����.
            } else                                          // ���̱�
            {
                pSE->m_bVisible = true;
            }
        } else if (0x00 == dwStatus) // �Ⱥ��̰� ����ų� �����ִ�..
        {
            if (bShouldBeRotate) // �ݱ�
            {
                pSE->RotateTo(0, vAxis, fRadian2, 1, true);  // �ٷ� ����.
                pSE->RotateTo(1, vAxis, -fRadian2, 1, true); // �ٷ� ����.
            } else                                           // �Ⱥ��̱�.
            {
                pSE->m_bVisible = false;
            }
        }
    } else {
        char szBuff[256];
        sprintf(szBuff, "No Resource NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
        CLogWriter::Write(szBuff);
    }

    pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true); // ���� ��ġ ����..
    pNPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);                 // ���� ��ġ..
    pNPC->RotateTo(D3DXToRadian(rand() % 360), false);
    pNPC->Action(PSA_BASIC, true, NULL, true);
    pNPC->ActionMove(PSM_STOP);

    return true;
}

bool CGameProcMain::MsgRecv_NPCOut(DataPack * pDataPack, int & iOffset) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Server���� �����ϴ� ���� ID
    return s_pOPMgr->NPCDelete(iID);                                   // ĳ���� ����...
}

// ���� ������ ��� ���̵� ī��Ʈ��ŭ �޴´�... �۱�.. ������Ʈ�� �ʿ��� �͸� �������� ��û..
bool CGameProcMain::MsgRecv_NPCInAndRequest(DataPack * pDataPack, int & iOffset) {
    int iNPCCountReceived = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    TRACE("NPC region update : %d\n", iNPCCountReceived);

    if (0 == iNPCCountReceived) {
        return false;
    }
    if (iNPCCountReceived < 0 || iNPCCountReceived >= 1000) {
        char szErr[256];
        sprintf(szErr, "�������� ��û ���� - NPC ���� %d", iNPCCountReceived);
        CGameProcedure::ReportDebugStringAndSendToServer(szErr);
        __ASSERT(0, szErr);
        return false;
    }

    int iID = 0;
    m_SetNPCID.clear(); // �� �����...
    it_ID   itID, itIDEnd = m_SetNPCID.end();
    pair_ID pairID;

    for (int i = 0; i < iNPCCountReceived; i++) {
        iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pairID = m_SetNPCID.insert(iID);
        TRACE("               ID : %d\n", iID);
    }

    if (m_SetNPCID.empty()) // ���� ������ �Ѱ��� ���ٸ� ���� ������..
    {
        s_pOPMgr->ReleaseNPCs();
        return false;
    }

    // ���� ���� ���̵�� ����Ʈ�� �ִ� NPC ID �� �˻��ؼ�..
    CPlayerNPC * pNPC = NULL;
    it_NPC       itNPC = s_pOPMgr->m_NPCs.begin(), itNPCEnd = s_pOPMgr->m_NPCs.end();
    for (; itNPC != itNPCEnd;) {
        pNPC = itNPC->second;
        iID = pNPC->IDNumber();

        itID = m_SetNPCID.find(iID);
        if (itID != itIDEnd) // ���� ���� ����Ʈ�� ������..
        {
            m_SetNPCID.erase(itID); // ��û�� ����Ʈ���� ����..
            itNPC++;
        } else // ���� ���°��� ������ �����..
        {
            TRACE("           delete : %d\n", iID);

            if (pNPC && pNPC->IsDead()) {
                itNPC++;
            } else {
                delete pNPC;
                pNPC = NULL;
                itNPC = s_pOPMgr->m_NPCs.erase(itNPC);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // �ٷ� ��û ��Ŷ�� ����� ������..
    int iNewNPCCount = m_SetNPCID.size();
    if (iNewNPCCount > 0) {
        int               iOffset = 0;                                    // ������ ������..
        std::vector<BYTE> byBuff(iNewNPCCount * 2 + 10, 0);               // ��Ŷ ����..
        CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, N3_REQUEST_NPC_IN); // Ŀ���.
        CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewNPCCount);     // ���̵� ����..

        itID = m_SetNPCID.begin();
        itIDEnd = m_SetNPCID.end();
        for (int i = 0; itID != itIDEnd; itID++, i++) {
            iID = *itID;
            CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID); // �ڼ��� ������ �ʿ��� ���̵��..
        }
        s_pSocket->Send(&(byBuff[0]), iOffset); // ������
    }
    // �ٷ� ��û ��Ŷ�� ����� ������..
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

bool CGameProcMain::MsgRecv_NPCInRequested(DataPack * pDataPack, int & iOffset) {
    int iNPCCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    if (0 == iNPCCount) {
        return false;
    }
    if (iNPCCount < 0 || iNPCCount >= 1000) {
        char szErr[256];
        sprintf(szErr, "�������� �ޱ���� - NPC ���� %d", iNPCCount);
        CGameProcedure::ReportDebugStringAndSendToServer(szErr);
        __ASSERT(0, szErr);
        return false;
    }

#ifdef _DEBUG
    float fTime = CN3Base::TimeGet();
#endif

    for (int i = 0; i < iNPCCount; i++) {
        this->MsgRecv_NPCIn(pDataPack, iOffset); // �÷��̾� ���� ��ŭ ���� ��...
    }

#ifdef _DEBUG
    TRACE("  NPC In Requested (Count:%d) : Avg(%.4f Sec)\n", iNPCCount, (CN3Base::TimeGet() - fTime) / iNPCCount);
#endif

    return true;
}

bool CGameProcMain::MsgRecv_NPCMove(DataPack * pDataPack, int & iOffset) {
    int   iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    float fXPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fZPos = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fYPos = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;
    float fSpeed = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;

    // �Լ��� �;� �� �κ�.. ^^
    CPlayerNPC * pNPC = NULL;
    pNPC = s_pOPMgr->NPCGetByID(iID, true); // NPC�� ID�μ� ���..
    if (NULL == pNPC) {
        this->MsgSend_NPCInRequest(iID);
        return false; // ����ִ� NPC�� ������..
    }

    float fY = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos); // ���� ���̰�..
    int   iMoveMode = (fSpeed > 0) ? 2 : 0;
    pNPC->MoveTo(fXPos, fY, fZPos, fSpeed, iMoveMode); // NPC ��ġ ����..

    __ASSERT(!(iMoveMode && fSpeed == 0), "Invalid NPC Move Packet");

    return true;
}

bool CGameProcMain::MsgRecv_Attack(DataPack * pDataPack, int & iOffset) {
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData,
                                          iOffset); // 0x01 - �������� ����, 0x02 ���� ���� 0x03, ���� ���� ����
    int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);      // 0x00 ����, 0x01 ����
    int iIDAttacker = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ������ ��
    int iIDTarget = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);   // Ÿ���� �Ǽ� ���ݴ��ѳ�.

    if (iIDAttacker == iIDTarget) {
        return false; // �����ڿ� �����ڰ� ���� ���????
    }

    bool bIAmTarget = (iIDTarget == s_pPlayer->IDNumber()) ? true : false;
    bool bIAmAttacker = (iIDAttacker == s_pPlayer->IDNumber()) ? true : false;

    CPlayerBase * pAttacker = NULL;
    if (bIAmAttacker) {
        pAttacker = s_pPlayer; // �����ϴ� ���� �� �ڽ��̸�
    } else {
        pAttacker = s_pOPMgr->CharacterGetByID(iIDAttacker, true);
    }
    if (NULL == pAttacker) // ��� �����ϴ� ���� ����??
    {
        if (iIDAttacker > 10000) {                   // NPC �� 1000 �̻��̴�.
            this->MsgSend_NPCInRequest(iIDAttacker); // NPC ������ ���� ��� ��û�Ѵ�..
        } else if (iIDAttacker < 3000) {
            this->MsgSend_UserInRequest(iIDAttacker); // NPC ������ ���� ��� ��û�Ѵ�..
        }
        return false; // �����ϴ� ���� ������ ���ư���.
    }

    CPlayerBase * pTarget = NULL;
    if (bIAmTarget) // Ÿ���� �����..
    {
        this->CommandSitDown(false, false); // ������ �����.
        if (m_pMagicSkillMng->IsCasting()) {
            __TABLE_UPC_SKILL * pSkill = s_pTbl_Skill->Find(s_pPlayer->m_dwMagicID);
            if (pSkill) {
                int SuccessValue = rand() % 100;
                if (SuccessValue >= pSkill->iPercentSuccess) {       // ��ų ���̺��� �ִ� Ȯ����� �����Ѵ�..
                    s_pPlayer->Action(PSA_BASIC, false, NULL, true); // ĳ���� ���, �⺻�������� ���� ����..
                }
            }
        }
        pTarget = s_pPlayer;
    } else // Ÿ���� �ٸ����̸�..
    {
        pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, true); //  �ϴ� ����ִ� �ѵ��߿��� �����ͺ���..
        if (NULL == pTarget) {
            pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, false); // ���ٸ� �׾�� �ѵ� ������� Ÿ������ ���
        }
    }

    if (NULL == pTarget) {
        return false; // Ÿ���� ����!!!!
    }

    //    pTarget->m_iIDTargetMe = iIDAttacker; // � ���� �����ϴ� �ž�??

    if (pAttacker != s_pPlayer && pAttacker && pAttacker->IsAlive()) // �����ϴ� ���� ���� �ƴϰ� �ٸ� ����ִ� ���϶�..
    {
        ((CPlayerNPC *)pAttacker)->RotateTo(pTarget); // Ÿ���� ���ؼ� ���� ���
        pAttacker->m_iIDTarget = iIDTarget;           // Ÿ�� ID ����..
        if (0x01 == iType) {
            pAttacker->Action(PSA_ATTACK, false, pTarget); // �������� ���� ����..
        } else if (0x02 == iType) {
            pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // ���� ����..
        }
        //        else if(0x03 == iType) pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // ���� ���� ����..
    }

    if (bIAmTarget || bIAmAttacker) {
        m_bDoCancelExitRequest = true;
        m_fExitCurCountDownToReach = 0.0f;
        if (m_eExitType > EXIT_TYPE_NONE) {
            m_eExitType = EXIT_TYPE_NONE;
            m_iExitCurCountDown = SECONDS_TO_EXIT_GAME_AFTER_ATTACK;
            if (m_pUIChatDlg) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_EXIT_GAME_CANCELED, szMsg);
                m_pUIChatDlg->AddChatMsg(N3_CHAT_NORMAL, szMsg, 0xFFFF0000);
            }
        }
    }

    pTarget->m_bGuardSuccess = false; // �� �����ߴ����� ���� �÷���..
    if (0x0 == iResult)               // ���� ����
    {
        if (pTarget->IsAlive() &&
            0 == pTarget->m_iSkillStep) // ���� ���� �ƴϰ� ��ų�� ���� ���� �ƴϸ� ���� ������ �Ѵ�..
        {
            pTarget->m_bGuardSuccess = true; // �� �����ߴ����� ���� �÷���..
            pTarget->Action(PSA_GUARD, false);
        }

        if (pAttacker == s_pPlayer) // �����ϴ� ����� �÷��̾� �ڽ��̸�..
        {
            char        szBuf[128] = "";
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_ATTACK_FAILED, szFmt);
            sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str());
            MsgOutput(szBuf, 0xffffffff);
        }
    } else if (0x2 == iResult) // Attack And Dead - �̹� �������� �״´�!!!
    {
        if (pTarget == s_pPlayer) {
            //            if(m_pUIDead) m_pUIDead->SetVisible(true);
            std::string szMsg;
            ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
            CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // �ٽ� ���� �޽��� ����..
        }

        pTarget->m_fTimeAfterDeath = 0.1f; // �ױ�����..
        if (pTarget->m_pShapeExtraRef) {
            s_pOPMgr->CharacterDelete(pTarget->IDNumber()); // ������Ʈ �϶��� ���ֹ�����..!!
        }
    }

    if (pTarget == s_pPlayer) {
        TRACE("player is under of attack (%d)\n", iIDAttacker);
    }

    return true;
}

bool CGameProcMain::MsgRecv_Dead(DataPack * pDataPack, int & iOffset) {
    int iIDTarget = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Ÿ���� �Ǽ� ���ݴ��ѳ�.

    CPlayerBase * pTarget = NULL;
    if (iIDTarget == s_pPlayer->IDNumber()) {
        pTarget = s_pPlayer;

        //        if(m_pUIDead) m_pUIDead->SetVisible(true);
        std::string szMsg;
        ::_LoadStringFromResource(IDS_REGENERATION, szMsg);
        CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // �ٽ� ���� �޽��� ����..
        CLogWriter::Write("Dead!!!");
    } else {
        pTarget = this->CharacterGetByID(iIDTarget, false);
    }

    if (pTarget) {
        if (pTarget->m_pShapeExtraRef) {
            s_pOPMgr->CharacterDelete(pTarget->IDNumber());
        } else {
            pTarget->m_fTimeAfterDeath = 0.1f; // �ױ�����..
            pTarget->ActionDying(PSD_KEEP_POSITION, __Vector3(0, 0, 1));
        }

        return true;
    }

    return false;
}

bool CGameProcMain::MsgRecv_ItemMove(DataPack * pDataPack, int & iOffset) {
    __InfoPlayerMySelf * pInfoExt = &(s_pPlayer->m_InfoExt);
    __InfoPlayerBase *   pInfoBase = &(s_pPlayer->m_InfoBase);

    BYTE bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
    if (bResult) {
        pInfoExt->iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoExt->iGuard = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoExt->iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        pInfoBase->iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoExt->iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        // �����ۿ� ���� �����Ȱ��̴�..
        pInfoExt->iStrength_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iStamina_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iDexterity_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iIntelligence_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iMagicAttak_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        pInfoExt->iRegistFire = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iRegistCold = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iRegistLight = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iRegistMagic = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iRegistCurse = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        pInfoExt->iRegistPoison = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        if (pInfoBase->iHP > pInfoBase->iHPMax) {
            pInfoBase->iHP = pInfoBase->iHPMax; // �����˻�..
        }
        if (pInfoExt->iMSP > pInfoExt->iMSPMax) {
            pInfoExt->iMSP = pInfoExt->iMSPMax; // �����˻�..
        }

        m_pUIVar->m_pPageState->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax);
        m_pUIVar->m_pPageState->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax);

        m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
        m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);

        m_pUIVar->m_pPageState->UpdateAttackPoint(pInfoExt->iAttack, pInfoExt->iAttack_Delta);
        m_pUIVar->m_pPageState->UpdateGuardPoint(pInfoExt->iGuard, pInfoExt->iGuard_Delta);
        m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);

        m_pUIVar->m_pPageState->UpdateStrength(pInfoExt->iStrength, pInfoExt->iStrength_Delta);
        m_pUIVar->m_pPageState->UpdateStamina(pInfoExt->iStamina, pInfoExt->iStamina_Delta);
        m_pUIVar->m_pPageState->UpdateDexterity(pInfoExt->iDexterity, pInfoExt->iDexterity_Delta);
        m_pUIVar->m_pPageState->UpdateIntelligence(pInfoExt->iIntelligence, pInfoExt->iIntelligence_Delta);
        m_pUIVar->m_pPageState->UpdateMagicAttak(pInfoExt->iMagicAttak, pInfoExt->iMagicAttak_Delta);

        m_pUIVar->m_pPageState->UpdateRegistFire(pInfoExt->iRegistFire, pInfoExt->iRegistFire_Delta);
        m_pUIVar->m_pPageState->UpdateRegistCold(pInfoExt->iRegistCold, pInfoExt->iRegistCold_Delta);
        m_pUIVar->m_pPageState->UpdateRegistLight(pInfoExt->iRegistLight, pInfoExt->iRegistLight_Delta);
        m_pUIVar->m_pPageState->UpdateRegistMagic(pInfoExt->iRegistMagic, pInfoExt->iRegistMagic_Delta);
        m_pUIVar->m_pPageState->UpdateRegistCurse(pInfoExt->iRegistCurse, pInfoExt->iRegistCurse_Delta);
        m_pUIVar->m_pPageState->UpdateRegistPoison(pInfoExt->iRegistPoison, pInfoExt->iRegistPoison_Delta);
    }

    m_pUIInventory->ReceiveResultFromServer(bResult);
    TRACE("Init Inv Move Res %d \n", (int)bResult);

    return true;
}

bool CGameProcMain::MsgRecv_ItemWeightChange(DataPack * pDataPack, int & iOffset) // ������ ���� ��ȭ..
{
    __InfoPlayerMySelf * pInfoExt = &(s_pPlayer->m_InfoExt);
    pInfoExt->iWeight = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);

    return true;
}

bool CGameProcMain::MsgRecv_UserLookChange(DataPack * pDataPack, int & iOffset) {
    int        iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    e_ItemSlot eSlot = (e_ItemSlot)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    DWORD      dwItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    int        iDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    if (iID == s_pPlayer->IDNumber()) {
        return false;
    }

    CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(iID, false);
    if (NULL == pUPC) {
        return false;
    }

    __TABLE_ITEM_BASIC * pItem = s_pTbl_Items_Basic->Find(dwItemID / 1000 * 1000);

    __TABLE_ITEM_EXT * pItemExt = NULL;
    if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
        pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(dwItemID % 1000); // �� ������ ���..
    }
    if (dwItemID && (NULL == pItem || NULL == pItemExt)) {
        N3_WARN("NULL Item!!!");
        return false;
    }

    e_PartPosition ePartPos = PART_POS_UNKNOWN, ePartPos2 = PART_POS_UNKNOWN;
    e_PlugPosition ePlugPos = PLUG_POS_UNKNOWN, ePlugPos2 = PLUG_POS_UNKNOWN;

    if (ITEM_SLOT_HEAD == eSlot) {
        ePartPos = PART_POS_HAIR_HELMET;
    } else if (ITEM_SLOT_UPPER == eSlot) {
        ePartPos = PART_POS_UPPER;
    } else if (ITEM_SLOT_LOWER == eSlot) {
        ePartPos = PART_POS_LOWER;
    } else if (ITEM_SLOT_GLOVES == eSlot) {
        ePartPos = PART_POS_HANDS;
    } else if (ITEM_SLOT_SHOES == eSlot) {
        ePartPos = PART_POS_FEET;
    } else if (ITEM_SLOT_HAND_RIGHT == eSlot) {
        ePlugPos = PLUG_POS_RIGHTHAND;
    } else if (ITEM_SLOT_HAND_LEFT == eSlot) {
        ePlugPos = PLUG_POS_LEFTHAND;
    }

    if (ePartPos != PART_POS_UNKNOWN) {
        if (dwItemID) // �������� �ִ� ���
        {
            std::string szItemFN;
            CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2);
            pUPC->PartSet(ePartPos, szItemFN, pItem, pItemExt); // ������ ���̱�..
            pUPC->DurabilitySet(eSlot, iDurability);
        } else {
            __TABLE_PLAYER_LOOKS * pLooks =
                s_pTbl_UPC_Looks->Find(pUPC->m_InfoBase.eRace); // User Player Character Skin ����ü ������..
            if (NULL == pLooks) {
                CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - failed find table : Race (%d)",
                                  pUPC->m_InfoBase.eRace);
                __ASSERT(pLooks, "failed find table");
            } else {
                if (PART_POS_HAIR_HELMET == ePartPos) {
                    pUPC->InitHair(); // �Ӹ��ε� ���� ����..
                } else {
                    pUPC->PartSet(ePartPos, pLooks->szPartFNs[ePartPos], NULL, NULL);
                }
            }
        }

        return true;
    } else if (ePlugPos != PLUG_POS_UNKNOWN) {
        if (dwItemID) {
            std::string szItemFN;
            CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2);
            pUPC->PlugSet(ePlugPos, szItemFN, pItem, pItemExt);
            pUPC->DurabilitySet(eSlot, iDurability);
        } else {
            pUPC->PlugSet(ePlugPos, "", NULL, NULL);
            pUPC->DurabilitySet(eSlot, 0);
        }

        // Sound Range �����̸� ���带 �ٲپ��ش�..
        __Vector3 vPosPlayer = s_pPlayer->Position();
        float     fDist = pUPC->Distance(vPosPlayer);
        if (fDist < SOUND_RANGE_TO_SET) {
            pUPC->SetSoundPlug(pItem); // SOUND_RANGE �ȿ� ������.
        }

        return true;
    } else {
        CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - Unknown Slot(%d) Item(%d)", eSlot, dwItemID);
        return false;
    }

    return false;
}

bool CGameProcMain::MsgRecv_ItemBundleDrop(DataPack * pDataPack, int & iOffset) // Item �� �ʵ忡 ��Ÿ���µ��� ���� ����
{
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    int iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

    CPlayerBase * pCorpse = s_pOPMgr->NPCGetByID(iID, false);
    if (NULL == pCorpse) {
        pCorpse = s_pOPMgr->CorpseGetByID(iID);
    }

    if (pCorpse) {
        pCorpse->m_iDroppedItemID = iItemID; // ����Ʈ�� ������ ���̵� ��ġ
    }

    return true;
}

bool CGameProcMain::MsgRecv_ItemBundleOpen(DataPack * pDataPack, int & iOffset) // ������ ���ڸ� ���ų� ��ü�� ������..
{
    DWORD dwItemID = 0;
    int   iItemCount = 0;

    POINT ptCur = s_pLocalInput->MouseGetPos();
    m_pUIDroppedItemDlg->EnterDroppedState(ptCur.x, ptCur.y);
    TRACE("������ ����Ʈ ����.. \n");

    for (int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++) {
        dwItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        // �̺κп� ���� ������â�� ���� �غ��Ѵ�..
        if (dwItemID) {
            m_pUIDroppedItemDlg->AddToItemTable(dwItemID, iItemCount, i);
        }
    }

    m_pUIDroppedItemDlg->InitIconUpdate();

    return true;
}

void CGameProcMain::MsgRecv_ItemRepair(DataPack * pDataPack, int & iOffset) // Item Repair Result..
{
    int     iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    int64_t iGold = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
    m_pUIInventory->ReceiveResultFromServer(iResult, iGold);
}

void CGameProcMain::MsgRecv_ItemCountChange(DataPack * pDataPack, int & iOffset) // Item Count Change..
{
    int iTotalCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Trade id

    for (int i = 0; i < iTotalCount; i++) {
        int iDistrict = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // Trade id
        int iIndex = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);    // Trade id
        int iID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);      // Trade id
        int iCount = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);   // Trade id
        m_pUIInventory->ItemCountChange(iDistrict, iIndex, iCount, iID);
    }

    if (m_pUISkillTreeDlg) {
        m_pUISkillTreeDlg->UpdateDisableCheck();
    }
    if (m_pUIHotKeyDlg) {
        m_pUIHotKeyDlg->UpdateDisableCheck();
    }
}

void CGameProcMain::MsgRecv_ItemDestroy(DataPack * pDataPack, int & iOffset) // Item Count Change..
{
    int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // Trade id
    m_pUIInventory->ReceiveResultItemRemoveFromServer(iResult);
};

void CGameProcMain::MsgRecv_MyInfo_HP(DataPack * pDataPack, int & iOffset) {
    int iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    int iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    int  iHPChange = iHP - s_pPlayer->m_InfoBase.iHP;
    char szBuf[256] = "";
    if (iHPChange < 0) {
        std::string szFmt;
        ::_LoadStringFromResource(IDS_MSG_FMT_HP_LOST, szFmt);
        sprintf(szBuf, szFmt.c_str(), -iHPChange);
        MsgOutput(szBuf, 0xffff3b3b);
    } else if (iHPChange > 0) {
        std::string szFmt;
        ::_LoadStringFromResource(IDS_MSG_FMT_HP_RECOVER, szFmt);
        sprintf(szBuf, szFmt.c_str(), iHPChange);
        MsgOutput(szBuf, 0xff6565ff);
    }

    s_pPlayer->m_InfoBase.iHPMax = iHPMax;
    s_pPlayer->m_InfoBase.iHP = iHP;

    m_pUIVar->m_pPageState->UpdateHP(iHP, iHPMax);
    m_pUIStateBarAndMiniMap->UpdateHP(iHP, iHPMax, false);
}

void CGameProcMain::MsgRecv_MyInfo_MSP(DataPack * pDataPack, int & iOffset) {
    int iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    int iMSP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    int iMSPChange = iMSP - s_pPlayer->m_InfoExt.iMSP;

    e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
    bool              bUseMP = (CLASS_REPRESENT_WARRIOR != eCR && CLASS_REPRESENT_ROGUE != eCR) ? true : false;

    char szBuf[256] = "";
    if (iMSPChange < 0) {
        std::string szFmt;
        if (bUseMP) {
            ::_LoadStringFromResource(IDS_MSG_FMT_MP_USE, szFmt);
        } else {
            ::_LoadStringFromResource(IDS_MSG_FMT_SP_USE, szFmt);
        }
        sprintf(szBuf, szFmt.c_str(), -iMSPChange);
        MsgOutput(szBuf, 0xffff3b3b);
    } else if (iMSPChange > 0) {
        std::string szFmt;
        if (bUseMP) {
            ::_LoadStringFromResource(IDS_MSG_FMT_MP_RECOVER, szFmt);
        } else {
            ::_LoadStringFromResource(IDS_MSG_FMT_SP_RECOVER, szFmt);
        }
        sprintf(szBuf, szFmt.c_str(), iMSPChange);
        MsgOutput(szBuf, 0xff6565ff);
    }

    s_pPlayer->m_InfoExt.iMSPMax = iMSPMax;
    s_pPlayer->m_InfoExt.iMSP = iMSP;

    m_pUIVar->m_pPageState->UpdateMSP(iMSP, iMSPMax);
    m_pUIStateBarAndMiniMap->UpdateMSP(iMSP, iMSPMax, false);

    if (m_pUISkillTreeDlg) {
        m_pUISkillTreeDlg->UpdateDisableCheck();
    }
    if (m_pUIHotKeyDlg) {
        m_pUIHotKeyDlg->UpdateDisableCheck();
    }
}

void CGameProcMain::MsgRecv_MyInfo_EXP(DataPack * pDataPack, int & iOffset) {
    int iExp = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    int iExpChange = iExp - s_pPlayer->m_InfoExt.iExp;
    s_pPlayer->m_InfoExt.iExp = iExp;
    m_pUIVar->m_pPageState->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext);
    m_pUIStateBarAndMiniMap->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext, false);

    if (s_pPlayer->m_InfoExt.iLevelPrev == s_pPlayer->m_InfoBase.iLevel && iExpChange != 0) {
        char szBuf[256] = "";
        if (iExpChange > 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_EXP_GET, szFmt);
            sprintf(szBuf, szFmt.c_str(), iExpChange);
        } else if (iExpChange < 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_EXP_LOST, szFmt);
            sprintf(szBuf, szFmt.c_str(), -iExpChange);
        }
        MsgOutput(szBuf, 0xffffff00);
    }

    s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
}

bool CGameProcMain::MsgRecv_MyInfo_LevelChange(DataPack * pDataPack, int & iOffset) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    int iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    if (iID == s_pPlayer->IDNumber()) {
        __InfoPlayerBase *   pInfoBase = &(s_pPlayer->m_InfoBase);
        __InfoPlayerMySelf * pInfoExt = &(s_pPlayer->m_InfoExt);

        int iLevelPrev = pInfoBase->iLevel;
        pInfoBase->iLevel = iLevel;
        pInfoExt->iBonusPointRemain = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���� ���ʽ� ����Ʈ..

        BYTE bExtraSkillPoint = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ��Ż ����Ʈ
        TRACE("Skill change Extra value %d\n", bExtraSkillPoint);

        int iExpNext = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        int iExp = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

        // ���� ���� ����ġ�� ������ش�..
        int iExpChange = 0;
        if (iLevel > iLevelPrev) {
            iExpChange = (pInfoExt->iExpNext - pInfoExt->iExp) + iExp; // ������ �Ѱ��..
        } else {
            iExpChange = -(pInfoExt->iExp + iExpNext - iExp); // �����ٿ� �Ѱ��..
        }
        char szBuf[256] = "";
        if (iExpChange > 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_EXP_GET, szFmt);
            sprintf(szBuf, szFmt.c_str(), iExpChange);
        } else if (iExpChange < 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_EXP_LOST, szFmt);
            sprintf(szBuf, szFmt.c_str(), -iExpChange);
        }
        MsgOutput(szBuf, 0xffffff00);

        pInfoExt->iExpNext = iExpNext;
        pInfoExt->iExp = iExp;

        pInfoBase->iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoBase->iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        pInfoExt->iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoExt->iMSP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        pInfoExt->iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        pInfoExt->iWeight = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt)); // ��� ���� ������Ʈ..

        m_pUIStateBarAndMiniMap->UpdateExp(pInfoExt->iExp, pInfoExt->iExpNext, true);
        m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
        m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);

        m_pUISkillTreeDlg->m_iSkillInfo[0] = bExtraSkillPoint;
        m_pUISkillTreeDlg->InitIconUpdate(); // ������ ��ȭ�Ǿ����Ƿ� .. ��ų�� �߰��� �� �ִ�..

        if (iLevel > iLevelPrev) // �̰��� Level Up ȿ���� �־��ش�..
        {
            if (s_pPlayer->Nation() == NATION_KARUS) {
                CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
            } else if (s_pPlayer->Nation() == NATION_ELMORAD) {
                CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
            }
        }
    } else // �ٸ� ���̴�..
    {
        CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(iID, false);
        if (pUPC) {
            if (iLevel > pUPC->m_InfoBase.iLevel) {
                if (pUPC->Nation() == NATION_KARUS) {
                    CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
                } else if (pUPC->Nation() == NATION_ELMORAD) {
                    CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
                }
            }
            pUPC->m_InfoBase.iLevel = iLevel;
        }
    }

    return TRUE;
}

void CGameProcMain::MsgRecv_MyInfo_RealmPoint(DataPack * pDataPack, int & iOffset) {
    s_pPlayer->m_InfoExt.iRealmPoint = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    if (m_pUIVar->m_pPageState) {
        m_pUIVar->m_pPageState->UpdateRealmPoint(s_pPlayer->m_InfoExt.iRealmPoint); // ���� �⿩���� 10�� ����� ǥ��
    }
}

void CGameProcMain::MsgRecv_MyInfo_PointChange(DataPack * pDataPack, int & iOffset) {
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
    int iVal = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..

    s_pPlayer->m_InfoBase.iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    s_pPlayer->m_InfoExt.iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
    m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);

    m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
    m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

    m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
    m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);

    // ����Ʈ ü���� -     ������ b1(1�� 2ü�� 3��ø 4���� 5����) s(-1 +1)
    //                     ������ b1(1�� 2ü�� 3��ø 4���� 5����) s(�����ġ)

    if (0x01 == iType) // Strength
    {
        s_pPlayer->m_InfoExt.iStrength = iVal;
        m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);
    } else if (0x02 == iType) // Stamina
    {
        s_pPlayer->m_InfoExt.iStamina = iVal;
        m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);
    } else if (0x03 == iType) // Dex
    {
        s_pPlayer->m_InfoExt.iDexterity = iVal;
        m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);
    } else if (0x04 == iType) // Intelligence
    {
        s_pPlayer->m_InfoExt.iIntelligence = iVal;
        m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence,
                                                   s_pPlayer->m_InfoExt.iIntelligence_Delta);
    } else if (0x05 == iType) // Magic Attack
    {
        s_pPlayer->m_InfoExt.iMagicAttak = iVal;
        m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak,
                                                 s_pPlayer->m_InfoExt.iMagicAttak_Delta);
    }

    if (iType >= 1 && iType <= 5) {
        s_pPlayer->m_InfoExt.iBonusPointRemain--;
        m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(
            s_pPlayer->m_InfoExt.iBonusPointRemain); // ���ʽ� ����Ʈ ������ �����Ѱ�??
    }
}

void CGameProcMain::InitUI() {
    const int iW = CN3Base::s_CameraData.vp.Width;
    const int iH = CN3Base::s_CameraData.vp.Height;

    RECT rc;
    int  iX = 0, iY = 0;

    e_Nation eNation = s_pPlayer->m_InfoBase.eNation; // ����....

    __TABLE_UI_RESRC * pTbl = s_pTbl_UI->Find(eNation);
    if (NULL == pTbl) {
        return;
    }

    m_pUICmd->Init(s_pUIMgr);
    m_pUICmd->LoadFromFile(pTbl->szCmd);
    rc = m_pUICmd->GetRegion();
    m_pUICmd->SetPos((iW - (rc.right - rc.left)) / 2, iH - (rc.bottom - rc.top));
    m_pUICmd->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);

    m_pUICmdList->Init(s_pUIMgr);
    m_pUICmdList->LoadFromFile(pTbl->szKaCmdList);
    rc = m_pUICmdList->GetRegion();
    m_pUICmdList->SetPos(iW - (rc.right - rc.left), 10);
    m_pUICmdList->SetVisible(false);

    m_pUICmdEditDlg->Init(s_pUIMgr);
    m_pUICmdEditDlg->LoadFromFile(pTbl->szCmdEdit);
    m_pUICmdEditDlg->SetVisibleWithNoSound(false);
    rc = m_pUICmdEditDlg->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUICmdEditDlg->SetPos(iX, iY);
    m_pUICmdEditDlg->SetStyle(UISTYLE_USER_MOVE_HIDE);

    m_pUIChatDlg->Init(s_pUIMgr); //Manager �ڽ����� ����Ʈ�� �߰�
    m_pUIChatDlg->LoadFromFile(pTbl->szChat);
    rc = m_pUIChatDlg->GetRegion();
    RECT rcCmd = m_pUICmd->GetRegion();
    rcCmd.top += 5; // .. �ϵ� �ڵ�..
    iX = 0;
    iY = iH - ((rc.bottom - rc.top) + (rcCmd.bottom - rcCmd.top));
    CGameProcedure::UIPostData_Read(UI_POST_WND_CHAT, m_pUIChatDlg, iX, iY);
    m_pUIChatDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
    m_pUIChatDlg->SetVisibleWithNoSound(true);

    m_pUIMsgDlg->Init(s_pUIMgr);
    m_pUIMsgDlg->LoadFromFile(pTbl->szMsgOutput);
    m_pUIMsgDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);

    // ä��â�� �޽��� â ��ġ ���߱�..
    m_pUIChatDlg->MoveOffset(0, -1);

    m_pUIStateBarAndMiniMap->Init(s_pUIMgr);
    m_pUIStateBarAndMiniMap->LoadFromFile(pTbl->szStateBar);
    m_pUIStateBarAndMiniMap->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
    m_pUIStateBarAndMiniMap->SetPos(0, 0);

    // �ٿ뵵 UI - ����, ���ܰ���, ����Ʈ, ģ�� ������...
    m_pUIVar->Init(s_pUIMgr);
    m_pUIVar->LoadFromFile(pTbl->szVarious);
    m_pUIVar->SetVisibleWithNoSound(false);
    m_pUIVar->m_pPageState->LoadFromFile(pTbl->szState);
    m_pUIVar->m_pPageState->SetVisibleWithNoSound(true);
    m_pUIVar->m_pPageKnights->LoadFromFile(pTbl->szKnights);
    m_pUIVar->m_pPageKnights->SetVisibleWithNoSound(false);
    m_pUIVar->m_pPageFriends->LoadFromFile(pTbl->szFriends);
    m_pUIVar->m_pPageFriends->SetVisibleWithNoSound(false);
    m_pUIVar->SetStyle(m_pUIVar->GetStyle() | UISTYLE_POS_LEFT);
    //    m_pUIVar->m_pPageQuest->LoadFromFile(pTbl->szQuest);
    //    m_pUIVar->m_pPageQuest->SetVisibleWithNoSound(false);

    m_pUITargetBar->Init(s_pUIMgr);
    m_pUITargetBar->LoadFromFile(pTbl->szTargetBar);
    m_pUITargetBar->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
    rc = m_pUITargetBar->GetRegion();
    m_pUITargetBar->SetPos((iW - (rc.right - rc.left)) / 2, 0);
    m_pUITargetBar->SetVisibleWithNoSound(false);

    m_pUIHelp->Init(s_pUIMgr);
    m_pUIHelp->LoadFromFile(pTbl->szHelp);
    CGameProcedure::UIPostData_Read(UI_POST_WND_HELP, m_pUIHelp, 0, 0);
    rc = m_pUIHelp->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIHelp->SetPos(iX, iY);
    //    m_pUIHelp->SetStyle(UISTYLE_SHOW_ME_ALONE);

    // ��������..
    m_pUINotice->Init(s_pUIMgr);
    m_pUINotice->LoadFromFile(pTbl->szNotice);
    m_pUINotice->SetVisibleWithNoSound(false);
    m_pUINotice->SetPos(iX, 10);

    m_pUIClassChange->Init(s_pUIMgr);
    m_pUIClassChange->LoadFromFile(pTbl->szClassChange);
    m_pUIClassChange->SetVisibleWithNoSound(false);
    rc = m_pUIClassChange->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIClassChange->SetPos(iX, iY);
    m_pUIClassChange->SetStyle(UISTYLE_USER_MOVE_HIDE);

    m_pUINpcChange->Init(s_pUIMgr);
    m_pUINpcChange->LoadFromFile(pTbl->szChangeClassInit);
    m_pUINpcChange->SetVisibleWithNoSound(false);
    rc = m_pUINpcChange->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUINpcChange->SetPos(iX, iY);
    m_pUINpcChange->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);

    m_pUINpcEvent->Init(s_pUIMgr);
    m_pUINpcEvent->LoadFromFile(pTbl->szNpcEvent);
    m_pUINpcEvent->SetVisibleWithNoSound(false);
    m_pUINpcEvent->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
    rc = m_pUINpcEvent->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUINpcEvent->SetPos(iX, iY);

    m_pUINpcTalk->Init(s_pUIMgr);
    m_pUINpcTalk->LoadFromFile(pTbl->szNpcTalk);
    m_pUINpcTalk->SetVisibleWithNoSound(false);
    rc = m_pUINpcTalk->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUINpcTalk->SetPos(iX, iY);

    m_pUIWarp->Init(s_pUIMgr);
    m_pUIWarp->LoadFromFile(pTbl->szZoneChangeOrWarp);
    m_pUIWarp->SetVisibleWithNoSound(false);
    rc = m_pUIWarp->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIWarp->SetPos(iX, iY);
    m_pUIWarp->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_SHOW_ME_ALONE);

    //    m_pUITradeList->Init(s_pUIMgr);
    //    m_pUITradeList->LoadFromFile(pTbl->szNpcExchangeList);
    //    m_pUITradeList->SetVisibleWithNoSound(false);
    //    rc = m_pUITradeList->GetRegion();
    //    iX = (iW - (rc.right - rc.left))/2;
    //    iY = (iH - (rc.bottom - rc.top))/2;
    //    m_pUITradeList->SetPos(iX, iY);

    m_pUIItemREDlg->Init(s_pUIMgr);
    m_pUIItemREDlg->LoadFromFile(pTbl->szExchangeRepair);
    m_pUIItemREDlg->SetVisibleWithNoSound(false);
    rc = m_pUIItemREDlg->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIItemREDlg->SetPos(iX, iY);
    m_pUIItemREDlg->InitIconWnd(UIWND_EXCHANGE_REPAIR);
    m_pUIItemREDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIItemREDlg->SetState(UI_STATE_COMMON_NONE);

    m_pUIRepairTooltip->Init(s_pUIMgr);
    m_pUIRepairTooltip->LoadFromFile(pTbl->szRepairTooltip);
    m_pUIRepairTooltip->SetVisibleWithNoSound(false);
    m_pUIRepairTooltip->InitPos();
    m_pUIRepairTooltip->SetStyle(UISTYLE_ALWAYSTOP | UISTYLE_FOCUS_UNABLE);

    m_pUIPartyOrForce->Init(s_pUIMgr);
    m_pUIPartyOrForce->LoadFromFile(pTbl->szPartyOrForce);
    m_pUIPartyOrForce->SetStyle(UISTYLE_HIDE_UNABLE);
    rc = m_pUIPartyOrForce->GetRegion();
    iX = iW - (rc.right - rc.left);
    m_pUIPartyOrForce->SetPos(iX, 0);
    m_pUIPartyOrForce->SetVisible(false); // ������ �Ⱥ��̱�~
    //    CGameProcedure::UIPostData_Read(UI_POST_WND_PARTY, m_pUIPartyOrForce, iX, 0);

    // Dropped Item Dlg..
    m_pUIDroppedItemDlg->Init(s_pUIMgr);
    m_pUIDroppedItemDlg->LoadFromFile(pTbl->szDroppedItem);
    m_pUIDroppedItemDlg->SetVisibleWithNoSound(false);
    m_pUIDroppedItemDlg->InitIconWnd(UIWND_DROPITEM);
    m_pUIDroppedItemDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIDroppedItemDlg->SetState(UI_STATE_COMMON_NONE);
    m_pUIDroppedItemDlg->SetStyle(m_pUIDroppedItemDlg->GetStyle() | UISTYLE_USER_MOVE_HIDE);

    m_pUITransactionDlg->Init(s_pUIMgr);
    m_pUITransactionDlg->LoadFromFile(pTbl->szTransaction);
    rc = m_pUITransactionDlg->GetRegion();
    m_pUITransactionDlg->SetPos(iW - (rc.right - rc.left), 10);
    m_pUITransactionDlg->SetVisibleWithNoSound(false);
    m_pUITransactionDlg->InitIconWnd(UIWND_TRANSACTION);
    m_pUITransactionDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUITransactionDlg->SetState(UI_STATE_COMMON_NONE);
    m_pUITransactionDlg->SetStyle(m_pUITransactionDlg->GetStyle() | UISTYLE_POS_RIGHT | UISTYLE_USER_MOVE_HIDE);

    m_pSubProcPerTrade->InitPerTradeDlg(s_pUIMgr);

    m_pUIInventory->Init(s_pUIMgr);
    m_pUIInventory->LoadFromFile(pTbl->szInventory);
    m_pUIInventory->SetVisibleWithNoSound(false);
    m_pUIInventory->SetPos(465, 10);
    m_pUIInventory->InitIconWnd(UIWND_INVENTORY);
    m_pUIInventory->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIInventory->SetState(UI_STATE_COMMON_NONE);
    m_pUIInventory->SetStyle(m_pUIInventory->GetStyle() | UISTYLE_POS_RIGHT);

    // Countable Item Edit Dlg..
    CN3UIWndBase::m_pCountableItemEdit = new CCountableItemEditDlg;
    CN3UIWndBase::m_pCountableItemEdit->Init(s_pUIMgr);
    CN3UIWndBase::m_pCountableItemEdit->LoadFromFile(pTbl->szPersonalTradeEdit);
    CN3UIWndBase::m_pCountableItemEdit->SetStyle(UISTYLE_ALWAYSTOP);
    // ��ġ ��� ..
    rc = CN3UIWndBase::m_pCountableItemEdit->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    CN3UIWndBase::m_pCountableItemEdit->SetPos(iX, iY);
    CN3UIWndBase::m_pCountableItemEdit->SetVisibleWithNoSound(false);
    CN3UIWndBase::m_pCountableItemEdit->SetUIType(UI_TYPE_BASE);
    CN3UIWndBase::m_pCountableItemEdit->SetState(UI_STATE_COMMON_NONE);

    m_pUISkillTreeDlg->Init(s_pUIMgr);
    m_pUISkillTreeDlg->LoadFromFile(pTbl->szSkillTree);
    m_pUISkillTreeDlg->SetVisibleWithNoSound(false);
    rc = m_pUISkillTreeDlg->GetRegion();
    m_pUISkillTreeDlg->SetPos(iW - (rc.right - rc.left), 10);
    m_pUISkillTreeDlg->InitIconWnd(UIWND_SKILL_TREE);
    m_pUISkillTreeDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUISkillTreeDlg->SetState(UI_STATE_COMMON_NONE);
    m_pUISkillTreeDlg->SetStyle(m_pUISkillTreeDlg->GetStyle() | UISTYLE_POS_RIGHT);

    // default ui pos ..    �ػ󵵰� ����Ǹ�.. ��� ��ġ�� ���ؾ� �Ѵ�.. by ecli666
    rc = m_pUIStateBarAndMiniMap->GetRegion();
    m_pUIHotKeyDlg->Init(s_pUIMgr);
    m_pUIHotKeyDlg->LoadFromFile(pTbl->szHotKey);
    m_pUIHotKeyDlg->SetStyle(UISTYLE_HIDE_UNABLE);
    CGameProcedure::UIPostData_Read(UI_POST_WND_HOTKEY, m_pUIHotKeyDlg, rc.left, rc.bottom);
    m_pUIHotKeyDlg->SetVisibleWithNoSound(true); // ������ ���δ�!!!
    m_pUIHotKeyDlg->InitIconWnd(UIWND_HOTKEY);
    m_pUIHotKeyDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIHotKeyDlg->SetState(UI_STATE_COMMON_NONE);

    m_pUIKnightsOp->Init(s_pUIMgr); // ���� ����Ʈ ����, ����, ��...
    m_pUIKnightsOp->LoadFromFile(pTbl->szKnightsOperation);
    m_pUIKnightsOp->SetVisibleWithNoSound(false);

    // ��Ƽ ���� �Խ���..
    m_pUIPartyBBS->Init(s_pUIMgr);
    m_pUIPartyBBS->LoadFromFile(pTbl->szPartyBBS);
    m_pUIPartyBBS->SetVisibleWithNoSound(false);
    rc = m_pUIPartyBBS->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIPartyBBS->SetPos(iX, iY);

    m_pUIWareHouseDlg->Init(s_pUIMgr);
    m_pUIWareHouseDlg->LoadFromFile(pTbl->szWareHouse);
    rc = m_pUIWareHouseDlg->GetRegion();
    m_pUIWareHouseDlg->SetPos(iW - (rc.right - rc.left), 10);
    m_pUIWareHouseDlg->SetVisibleWithNoSound(false);
    m_pUIWareHouseDlg->InitIconWnd(UIWND_WARE_HOUSE);
    m_pUIWareHouseDlg->SetUIType(UI_TYPE_ICON_MANAGER);
    m_pUIWareHouseDlg->SetState(UI_STATE_COMMON_NONE);
    m_pUIWareHouseDlg->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_POS_RIGHT);

    m_pTargetSymbol->LoadFromFile(
        pTbl->szTargetSymbolShape); // �÷��̾ Ÿ������ ���� ĳ������ ��ġ���� �׸��� �ȴ�..

    m_pUIInn->Init(s_pUIMgr);
    m_pUIInn->LoadFromFile(pTbl->szInn);
    m_pUIInn->SetVisibleWithNoSound(false);
    m_pUIInn->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
    rc = m_pUIInn->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIInn->SetPos(iX, iY);

    m_pUICreateClanName->Init(s_pUIMgr);
    m_pUICreateClanName->LoadFromFile(pTbl->szInputClanName);
    m_pUICreateClanName->SetVisibleWithNoSound(false);
    rc = m_pUICreateClanName->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUICreateClanName->SetPos(iX, iY);
    m_pUICreateClanName->SetStyle(UISTYLE_USER_MOVE_HIDE);

    // Quest Menu
    m_pUIQuestMenu->Init(s_pUIMgr);
    m_pUIQuestMenu->LoadFromFile(pTbl->szQuestMenu);
    m_pUIQuestMenu->SetVisibleWithNoSound(false);
    m_pUIQuestMenu->SetStyle(UISTYLE_USER_MOVE_HIDE);

    // Quest Talk
    m_pUIQuestTalk->Init(s_pUIMgr);
    m_pUIQuestTalk->LoadFromFile(pTbl->szQuestTalk);
    m_pUIQuestTalk->SetVisibleWithNoSound(false);
    m_pUIQuestTalk->SetStyle(UISTYLE_USER_MOVE_HIDE);
    rc = m_pUIQuestTalk->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIQuestTalk->SetPos(iX, iY);

    // Level Guide
    m_pUILevelGuide->Init(s_pUIMgr);
    m_pUILevelGuide->LoadFromFile(pTbl->szLevelGuide);
    m_pUILevelGuide->SetVisibleWithNoSound(false);
    m_pUILevelGuide->SetStyle(m_pUILevelGuide->GetStyle() | UISTYLE_POS_RIGHT);
    rc = m_pUILevelGuide->GetRegion();
    m_pUILevelGuide->SetPos(iW - (rc.right - rc.left), 10);
    m_pUILevelGuide->SetState(UI_STATE_COMMON_NONE);

    // dead ui
    m_pUIDead->Init(s_pUIMgr);
    m_pUIDead->LoadFromFile(pTbl->szDead);
    m_pUIDead->SetVisibleWithNoSound(false);
    rc = m_pUIDead->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIDead->SetPos(iX, iY);

    // ��ŷ� �Խ���
    m_pUITradeBBS->Init(s_pUIMgr);
    m_pUITradeBBS->LoadFromFile(pTbl->szTradeBBS);
    m_pUITradeBBS->SetVisibleWithNoSound(false);
    m_pUITradeBBS->SetStyle(UISTYLE_USER_MOVE_HIDE);
    rc = m_pUITradeBBS->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUITradeBBS->SetPos(iX, iY);

    // ��ŷ� ���� ����
    m_pUITradeBBSSelector->Init(s_pUIMgr);
    m_pUITradeBBSSelector->LoadFromFile(pTbl->szTradeBBSSelector);
    m_pUITradeBBSSelector->SetVisibleWithNoSound(false);
    rc = m_pUITradeBBSSelector->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUITradeBBSSelector->SetPos(iX, iY);
    m_pUITradeBBSSelector->SetStyle(UISTYLE_USER_MOVE_HIDE);

    // ��ŷ� �׸� ����
    m_pUITradeBBSEdit->LoadFromFile(pTbl->szTradeExplanation);
    m_pUITradeBBSEdit->SetVisibleWithNoSound(false);
    rc = m_pUITradeBBSEdit->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUITradeBBSEdit->SetPos(iX, iY);

    // Rookie tip
    m_pUIRookieTip->Init(s_pUIMgr);
    m_pUIRookieTip->LoadFromFile(pTbl->szRookieTip);
    m_pUIRookieTip->SetVisibleWithNoSound(false);
    rc = m_pUIRookieTip->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIRookieTip->SetPos(iX, iY);
    m_pUIRookieTip->SetState(UI_STATE_COMMON_NONE);

    //Exit Menu
    m_pUIExitMenu->Init(s_pUIMgr);
    m_pUIExitMenu->LoadFromFile(pTbl->szExitMenu);
    m_pUIExitMenu->SetVisibleWithNoSound(false);
    rc = m_pUIExitMenu->GetRegion();
    iX = (iW - (rc.right - rc.left)) / 2;
    iY = (iH - (rc.bottom - rc.top)) / 2;
    m_pUIExitMenu->SetPos(iX, iY);
    m_pUIExitMenu->SetStyle(UISTYLE_MODAL);
}

void CGameProcMain::MsgSend_RequestTargetHP(short siIDTarget, BYTE byUpdateImmediately) {
    BYTE byBuff[4];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_TARGET_HP);
    CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);
    CAPISocket::MP_AddByte(byBuff, iOffset, byUpdateImmediately); // 0x00 - ���� �þ�Բ�.. 0x01 - ��� ������Ʈ..

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgRecv_TargetHP(DataPack * pDataPack, int & iOffset) {
    int  iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // SID
    BYTE byUpdateImmediately =
        CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x00 - ���� �þ�Բ�.. 0x01 - ��� ������Ʈ..
    int iTargetHPMax = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);    // HP
    int iTargetHPCur = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);    // HP
    int iTargetHPChange = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // HP

    if (iTargetHPMax <= 0) {
        CPlayerNPC * pTarget = this->s_pOPMgr->CharacterGetByID(iID, true);
        std::string  szBuff("Invalid HP Change - 0 ID(");
        if (pTarget) {
            szBuff += pTarget->IDString();
        }
        szBuff += ')';
        CLogWriter::Write(szBuff.c_str());
    }
    __ASSERT(iTargetHPMax > 0, "�ִ� ü�� ��ġ�� 0�̻��̾�� �մϴ�.");

    if (iID == s_pPlayer->m_iIDTarget) // ���� �����ϴ� ���̸�..
    {
        bool bUI = false;
        if (byUpdateImmediately) {
            bUI = true;
        }
        m_pUITargetBar->UpdateHP(iTargetHPCur, iTargetHPMax, bUI);
        //    TRACE("Target HP Update %d %d\n", iID, iTargetHPPercent);
    }

    CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(iID, true);
    if (pTarget) {
        pTarget->m_InfoBase.iHP = iTargetHPCur;
        pTarget->m_InfoBase.iHPMax = iTargetHPMax;

        char szBuf[256] = "";
        if (iTargetHPChange < 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_LOST, szFmt);
            sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), -iTargetHPChange);
            MsgOutput(szBuf, 0xffffffff);
        } else if (iTargetHPChange > 0) {
            std::string szFmt;
            ::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_RECOVER, szFmt);
            sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), iTargetHPChange);
            MsgOutput(szBuf, 0xff6565ff);
        }
    }
}

// ��ŷ�..................
bool CGameProcMain::MsgSend_NPCEvent(short siIDTarget) {
    BYTE byBuff[4];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_NPC_EVENT);
    CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);

    s_pSocket->Send(byBuff, iOffset);

    return true;
}

void CGameProcMain::MsgSend_NPCInRequest(int iID) // NPC ������ ���� ��� ��û�Ѵ�..
{
    TRACE("found NPC ghost (ID:%d)\n", iID);

    int  iOffset = 0; // ������ ������..
    BYTE byBuff[32];
    ;                                                           // ��Ŷ ����..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_REQUEST_NPC_IN); // Ŀ���.
    CAPISocket::MP_AddShort(byBuff, iOffset, 1);                // ���̵� ����..
    CAPISocket::MP_AddShort(byBuff, iOffset, iID);              // �ڼ��� ������ �ʿ��� ���̵��..

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_UserInRequest(int iID) // User ������ ���� ��� ��û�Ѵ�..
{
    TRACE("found User ghost (ID:%d)\n", iID);

    int  iOffset = 0; // ������ ������..
    BYTE byBuff[32];
    ;                                                            // ��Ŷ ����..
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_REQUEST_USER_IN); // Ŀ���.
    CAPISocket::MP_AddShort(byBuff, iOffset, 1);                 // ���̵� ����..
    CAPISocket::MP_AddShort(byBuff, iOffset, iID);               // �ڼ��� ������ �ʿ��� ���̵��..

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Warp() // ���� - ���̵��� �ɼ��� �ִ�..
{
    __WarpInfo WI;
    int        iSel = m_pUIWarp->InfoGetCur(WI);
    if (iSel < 0 || WI.szName.empty()) {
        return;
    }

    m_pUIWarp->m_CurrWI = WI;
    if (CGameBase::s_pPlayer->m_InfoExt.iGold < m_pUIWarp->m_CurrWI.iGold) {
        std::string szMsg;
        ::_LoadStringFromResource(IDS_WARP_REQUIRED_COINS, szMsg); // 7612
        char szMsgBuff[400]{};
        sprintf(szMsgBuff, szMsg.c_str(), m_pUIWarp->m_CurrWI.szName.c_str(), m_pUIWarp->m_CurrWI.iGold);
        MsgOutput(szMsgBuff, 0xffff3b3b);
        return;
    }

    BYTE byBuff[8];
    int  iOffset = 0;

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_WARP_LIST);
    CAPISocket::MP_AddByte(byBuff, iOffset, WI.iID); // ���� ���̵� ������...
    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_ZoneChangeComplete() // �� ü���� �Ϸ�.. (�� �ε� ��..)
{
    BYTE byBuff[4];
    int  iOffset = 0;
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_ZONE_CHANGE);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::DoCommercialTransaction(int iTradeID) {
    if (m_pUITransactionDlg->IsVisible()) {
        return;
    }

    m_pUITransactionDlg->m_iTradeID = iTradeID;

    if (!m_pUITransactionDlg->IsVisible()) {
        m_pUITransactionDlg->SetVisible(true);
    }

    if (m_pUIInventory->IsVisible()) { // �κ��丮�� �ȿ��� ������..
        this->CommandToggleUIInventory();
    }

    if (m_pUISkillTreeDlg->IsVisible()) {
        m_pUISkillTreeDlg->Close();
    }

    m_pUITransactionDlg->EnterTransactionState();
}

bool CGameProcMain::MsgRecv_ItemTradeStart(DataPack * pDataPack, int & iOffset) // ������ ��ŷ�..
{
    int          iTradeID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // Trade id
    CPlayerNPC * pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
    if (!pNPC) {
        return true;
    }
    m_pUINpcEvent->Open(NPC_EVENT_ITEM_TRADE, iTradeID, pNPC->GetNPCOriginID());

    return true;
}

bool CGameProcMain::MsgRecv_ItemTradeResult(DataPack * pDataPack, int & iOffset) // ������ ��ŷ� ���..
{
    uint8_t bfType = 0x00;
    int     iMoney = 0;
    uint8_t bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // Trade id

    switch (bResult) {
    case 0x00:
        bfType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // Trade id
        m_pUITransactionDlg->ReceiveResultTradeFromServer(bResult, bfType, iMoney);
        break;

    case 0x01:
        iMoney = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // Trade id
        m_pUITransactionDlg->ReceiveResultTradeFromServer(bResult, bfType, iMoney);
        break;

    case 0x03:
        m_pUITransactionDlg->ReceiveResultTradeMoveSuccess();
        break;

    case 0x04:
        m_pUITransactionDlg->ReceiveResultTradeMoveFail();
        break;
    }

    return true;
}

void CGameProcMain::InitZone(int iZone, const __Vector3 & vPosPlayer) {
    if (m_pSnd_Battle) {
        m_pSnd_Battle->Stop(0.0f); // ���� ���߱�..
    }
    if (m_pSnd_Town) {
        m_pSnd_Town->Stop(0.0f);
    }

    MsgSend_SpeedCheck(true); // ���ǵ��� üũ ���ؽð� ���ϱ�
    static int iZonePrev = -1;
    if (iZonePrev != iZone) // �ٸ� ���� ��쿡�� �ε�..
    {
        m_pLightMgr->Release();
        s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

        if (m_pUIChatDlg) {
            m_pUIChatDlg->ChangeChattingMode(N3_CHAT_NORMAL); //�� �̵��� �ϸ� �Ϲ� ä�û��·� �ٲ��ش�.
        }
        if (m_pUIPartyOrForce) {
            m_pUIPartyOrForce->MemberDestroy(); //�� �̵��� �Ҷ� ��Ƽ�� ����...
        }
        this->UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.

        s_pPlayer->m_bMoveContinous = true; // �����..
        this->CommandToggleMoveContinous();

        CLogWriter::Write("CGameProcMain::InitZone -> Zone Change (%d -> %d) Position(%.1f, %.1f, %.1f)", iZonePrev,
                          iZone, vPosPlayer.x, vPosPlayer.y, vPosPlayer.z);

        m_bLoadComplete = false;                                           // �ε� ����..
        CLogWriter::Write("%d->ClearDurationalMagic()", m_pMagicSkillMng); // TmpLog1122
        m_pMagicSkillMng->ClearDurationalMagic();
        CLogWriter::Write("%d->ClearAll()", s_pFX); // TmpLog1122
        s_pFX->ClearAll();

        if (s_pUILoading) {
            CLogWriter::Write("s_pUILoading->Render()"); // TmpLog1122
            s_pUILoading->Render("", 0);
        }

        s_pPlayer->m_InfoExt.iZoneCur = iZone;
        iZonePrev = iZone; // �ֱٿ� ���� �� ��ȣ�� ����صд�.

        CLogWriter::Write("%d->Find(s_pPlayer->m_InfoExt.iZoneCur)", s_pTbl_Zones); // TmpLog1122
        __TABLE_ZONE * pZoneData = s_pTbl_Zones->Find(s_pPlayer->m_InfoExt.iZoneCur);
        if (NULL == pZoneData) {
            CLogWriter::Write("can't find zone data. (zone : %d)", s_pPlayer->m_InfoExt.iZoneCur);
            __ASSERT(0, "Zone Data Not Found!");
            return;
        }

        CLogWriter::Write("%d->Release()", s_pOPMgr); // TmpLog1122

        s_pOPMgr->Release();                               // �ٸ� �ѵ� �� ������..
        CLogWriter::Write("%d->InitWorld()", s_pWorldMgr); // TmpLog1122
        s_pWorldMgr->InitWorld(iZone, vPosPlayer);

        // �̴ϸ� �ε�..
        CLogWriter::Write("%d->GetWidthByMeterWithTerrain()", ACT_WORLD); // TmpLog1122
        float fWidth = ACT_WORLD->GetWidthByMeterWithTerrain();
        CLogWriter::Write("%d->LoadMap()", m_pUIStateBarAndMiniMap); // TmpLog1122
        CLogWriter::Write("%d->szMiniMapFNszMiniMapFN", pZoneData);  // TmpLog1122
        m_pUIStateBarAndMiniMap->LoadMap(pZoneData->szMiniMapFN, fWidth, fWidth);

        CLogWriter::Write("GetRepresentClass()"); // TmpLog1122
        // �� ���� ���ϱ�..
        float             fZoom = 6.0f;
        e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
        if (CLASS_REPRESENT_ROGUE == eCR) {
            fZoom = 3.0f; // �α� �迭�� ���� ���� �θ� �ڼ��� ���δ�..
        }
        CLogWriter::Write("%d->ZoomSet()", m_pUIStateBarAndMiniMap); // TmpLog1122
        m_pUIStateBarAndMiniMap->ZoomSet(fZoom);

        CLogWriter::Write("%d->szTerrainFN.c_str()", pZoneData); // TmpLog1122
        //char szBuf[256];
        char szFName[_MAX_PATH];
        _splitpath(pZoneData->szTerrainFN.c_str(), NULL, NULL, szFName, NULL);
        char szFName2[_MAX_PATH];
        char szFullPathName[_MAX_PATH];
        sprintf(szFName2, "%s_Bird", szFName);
        _makepath(szFullPathName, NULL, "misc\\bird", szFName2, "lst");

        m_pLightMgr->LoadZoneLight(pZoneData->szLightObjFN.c_str());

        m_bLoadComplete = true; // �ε� ����..
    }

    // ī�޶� ����..
    CN3Camera * pCamera = s_pEng->CameraGetActive(); // Ȱ��ȭ�� ī�޶� ���..
    if (pCamera) {
        __Vector3 vPosPlayer = s_pPlayer->Position();
        pCamera->m_Data.fFOV = D3DXToRadian(70); // Field of View ..
        pCamera->m_Data.fFP = 512.0f;            // Far Plane..
        pCamera->m_Data.fNP = 0.5f;              // Near Plane..
        CLogWriter::Write("pCamera->LookAt()");  // TmpLog1122
        pCamera->LookAt(vPosPlayer + __Vector3(0, 0, -1), vPosPlayer, __Vector3(0, 1, 0));
        CLogWriter::Write("pCamera->Tick()"); // TmpLog1122
        pCamera->Tick();
        CLogWriter::Write("pCamera->Apply()"); // TmpLog1122
        pCamera->Apply();
    }
    // �⺻���� ĳ������ġ�� ī�޶� ��ġ ���..
    ////////////////////////////////////////////////////////////////////////////////

    CLogWriter::Write("InitPlayerPosition() Position(%.1f, %.1f, %.1f)", vPosPlayer.x, vPosPlayer.y,
                      vPosPlayer.z);               // TmpLog1122
    this->InitPlayerPosition(vPosPlayer);          // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
    CLogWriter::Write("%d->Release()2", s_pOPMgr); // TmpLog1122
    s_pOPMgr->Release();                           // �ٸ� �÷��̾� ����...
}

void CGameProcMain::MsgSend_GameStart() {
    BYTE byBuff[32];  // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_GAMESTART);                 // ���� ��ŸƮ ��Ŷ Ŀ���..
    CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->IDString().size()); // ���̵� ���� ��Ŷ�� �ֱ�..
    CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());      // ���̵� ���ڿ� ��Ŷ�� �ֱ�..

    s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::CommandToggleWalkRun() {
    bool bRun = s_pPlayer->ToggleRunMode();
    bool bMove = s_pPlayer->m_bMoveContinous;

    // Ŀ�ǵ� ��ư ������Ʈ..
    if (m_pUICmd->m_pBtn_Act_Walk) {
        m_pUICmd->m_pBtn_Act_Walk->SetVisible(bRun);
        if (bMove) {
            m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
        } else {
            m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }
    if (m_pUICmd->m_pBtn_Act_Run) {
        m_pUICmd->m_pBtn_Act_Run->SetVisible(!bRun);
        if (bMove) {
            m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
        } else {
            m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }

    return bRun;
}

bool CGameProcMain::CommandToggleAttackContinous() {
    CPlayerBase * pTarget = s_pPlayer->TargetPointerCheck(false);
    this->CommandEnableAttackContinous(!s_pPlayer->m_bAttackContinous, pTarget);

    return s_pPlayer->m_bAttackContinous;
}

bool CGameProcMain::CommandToggleMoveContinous() {
    if (s_pPlayer->IsDead()) {
        return false;
    }

    s_pPlayer->ToggleMoveMode(); // �ڵ� ���� ���..
    if (s_pPlayer->m_bMoveContinous) {
        this->CommandMove(MD_FOWARD, true);
        if (m_pUICmd->m_pBtn_Act_Run) {
            m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
        }
        if (m_pUICmd->m_pBtn_Act_Walk) {
            m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
        }
    } else {
        this->CommandMove(MD_STOP, true); // ������ ��Ŷ ������..
        if (m_pUICmd->m_pBtn_Act_Run) {
            m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pUICmd->m_pBtn_Act_Walk) {
            m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }

    return s_pPlayer->m_bMoveContinous;
}

void CGameProcMain::CommandMove(e_MoveDirection eMD, bool bStartOrEnd) {
    if (bStartOrEnd) {
        this->CloseUIs(); // ���� ��ŷ�, �������... UI �ݱ�..
    }

    if (s_pPlayer->IsDead()) {
        return; // ���� ���� ��� ����!!
    }

    if (MD_FOWARD == eMD || MD_BACKWARD == eMD) {
        s_pUIMgr->UserMoveHideUIs();
        this->CommandSitDown(false, false, true); // ������ �����..
        if (s_pPlayer->m_bStun) {
            return; // ������ ���� �������� ����..
        }
        if (MD_FOWARD == eMD) {
            if (s_pPlayer->IsRunning()) {
                s_pPlayer->ActionMove(PSM_RUN); // �پ��..
            } else {
                s_pPlayer->ActionMove(PSM_WALK); // �ɾ��..
            }
        } else {
            s_pPlayer->ActionMove(PSM_WALK_BACKWARD); // ����..
        }

        if (bStartOrEnd) // �����̱� �����Ҷ����� �浹üũ�� ���� �غ���..
        {
            float fSpeed =
                s_pPlayer
                    ->MoveSpeedCalculationAndCheckCollision(); // �ӵ��� ���ϰ� �� �ӵ��� �浹 üũ�� �Ѵ�. ���ϰ��� 0 �̸� �浹�̴�..
            if (0 == fSpeed)                                   // �������̴� ��Ȳ�̸�..
            {
                s_pPlayer->ActionMove(PSM_STOP); // ����..
            } else {
                this->MsgSend_Move(true, false); // �����̴� �����̸�.. ������ ���� ��Ŷ ������.
            }

            if (m_pUICmd->m_pBtn_Act_Run) {
                m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
            }
            if (m_pUICmd->m_pBtn_Act_Walk) {
                m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
            }
        }
    } else if (MD_STOP == eMD) {
        s_pPlayer->m_bMoveContinous = false; // ��� ���� ����..
        s_pPlayer->ActionMove(PSM_STOP);

        if (m_pUICmd->m_pBtn_Act_Run) {
            m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
        }
        if (m_pUICmd->m_pBtn_Act_Walk) {
            m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
        }

        this->MsgSend_Move(false, false); // ������ ���� ��Ŷ ������.
    }

    if (s_pPlayer->m_bAttackContinous) // �������̸�..
    {
        CPlayerBase * pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
        if (s_pPlayer->IsAttackableTarget(pTarget)) {      // ���� �����ϸ�..
            s_pPlayer->Action(PSA_ATTACK, false, pTarget); // ����
        }
    }
}

void CGameProcMain::CommandEnableAttackContinous(bool bEnable, CPlayerBase * pTarget) {
    if (bEnable == s_pPlayer->m_bAttackContinous) {
        return;
    }
    if (bEnable) {
        this->CloseUIs(); // ���� ��ŷ�, �������... UI �ݱ�..
        s_pUIMgr->UserMoveHideUIs();

        if (s_pPlayer->m_bStun) {
            return; // ������ ���� ���� ����..
        }
        if (NULL == pTarget) {
            return;
        }
        s_pPlayer->RotateTo(pTarget); // ������ ������.
        if (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) {
            return; // ������ ������ �Ѿ��..
        }
    }
    s_pPlayer->m_bAttackContinous = bEnable; // ���¸� ����ϰ�..

    if (bEnable) {
        SetGameCursor(s_hCursorAttack);
    } else {
        e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
        SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
    }

    if (m_pUICmd->m_pBtn_Act_Attack) {
        if (bEnable) {
            m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_DOWN);
        } else {
            m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_NORMAL);
        }
    }

    std::string szMsg;
    if (bEnable) // �ڵ� ����!
    {
        ::_LoadStringFromResource(IDS_MSG_ATTACK_START, szMsg);
        char szBuff[260]{};
        sprintf(szBuff, szMsg.c_str(), pTarget->IDString().c_str());
        szMsg = szBuff;
        this->PlayBGM_Battle();

        if (s_pPlayer->IsAttackableTarget(pTarget)) {
            s_pPlayer->Action(PSA_BASIC, true, pTarget);
        }
    } else // �ڵ� ���� �ƴ�.
    {
        ::_LoadStringFromResource(IDS_MSG_ATTACK_STOP, szMsg);
        s_pPlayer->Action(PSA_BASIC, true, pTarget);
    }

    this->MsgOutput(szMsg, 0xff00ffff);

    if (bEnable &&
        false == s_pPlayer->IsAttackableTarget(pTarget)) // ����, �Ÿ� �� ���� üũ�ؼ� ���� �Ұ����ϸ� ���ư���..
    {
        std::string szMsg;
        ::_LoadStringFromResource(IDS_MSG_ATTACK_DISABLE, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        return;
    }
}

bool CGameProcMain::CommandToggleUIState() {
    bool bNeedOpen = !(m_pUIVar->IsVisible());
    if (bNeedOpen) {
        s_pUIMgr->SetFocusedUI(m_pUIVar);
        m_pUIVar->Open();
    } else {
        m_pUIVar->Close();
    }

    // Ŀ�ǵ� ��ư ������Ʈ..
    //    if(m_pUICmd->m_pBtn_Character)
    //    {
    //        if(bNeedOpen) m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_DOWN); // ��ư ���� ����..
    //        else m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_NORMAL); // ��ư ����
    //    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIInventory() {
    bool bNeedOpen = false;

    // ���� �ŷ����̸�..
    if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) {
        return bNeedOpen;
    }

    if (m_pUINpcEvent->IsVisible()) {
        return bNeedOpen;
    }

    if (m_pUIInn->IsVisible()) {
        return bNeedOpen;
    }
    if (m_pUICreateClanName->IsVisible()) {
        return bNeedOpen;
    }

    if (m_pUIWareHouseDlg->IsVisible()) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close(true);
        }
        return bNeedOpen;
    }

    if (m_pUITransactionDlg->IsVisible()) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close(true);
        }
        return bNeedOpen;
    }

    if (m_pUINotice->IsVisible()) {
        m_pUINotice->Close();
    }

    if (m_pUILevelGuide->IsVisible()) {
        m_pUILevelGuide->SetVisible(false);
    }

    if (m_pUIInventory->IsVisible()) {
        m_pUIInventory->Close(true);
        return bNeedOpen;
    } else {
        bNeedOpen = true;
        if (m_pUISkillTreeDlg->IsVisible()) {
            m_pUISkillTreeDlg->Close();
        }

        s_pUIMgr->SetFocusedUI(m_pUIInventory);
        m_pUIInventory->Open();
    }

    return bNeedOpen;
}

bool CGameProcMain::OpenCmdEdit(std::string msg) {
    bool bNeedOpen = !(m_pUICmdEditDlg->IsVisible());

    if (bNeedOpen) {
        s_pUIMgr->SetFocusedUI(m_pUICmdEditDlg);
        m_pUICmdEditDlg->Open(msg);
    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleCmdList() {
    bool bNeedOpen = !(m_pUICmdList->IsVisible());
    if (bNeedOpen) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->LeaveTransactionState();
        }
        if (m_pUIWareHouseDlg->IsVisible()) {
            m_pUIWareHouseDlg->LeaveWareHouseState();
        }
        if (m_pUISkillTreeDlg->IsVisible()) {
            m_pUISkillTreeDlg->Close();
        }

        s_pUIMgr->SetFocusedUI(m_pUICmdList);
        m_pUICmdList->Open();
    } else {
        m_pUICmdList->Close();
    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleUISkillTree() {
    bool bNeedOpen = !(m_pUISkillTreeDlg->IsVisible());

    // ���� �ŷ����̸�..
    if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) {
        return bNeedOpen;
    }

    if (bNeedOpen) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->LeaveTransactionState();
        }
        if (m_pUIWareHouseDlg->IsVisible()) {
            m_pUIWareHouseDlg->LeaveWareHouseState();
        }
        if (m_pUINotice->IsVisible()) {
            m_pUINotice->Close();
        }
        if (m_pUILevelGuide->IsVisible()) {
            m_pUILevelGuide->SetVisible(false);
        }

        s_pUIMgr->SetFocusedUI(m_pUISkillTreeDlg);
        m_pUISkillTreeDlg->Open();
    } else {
        m_pUISkillTreeDlg->Close();
    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleUINotice() {
    bool bNeedOpen = !m_pUINotice->IsVisible();

    if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) {
        return bNeedOpen;
    }

    if (bNeedOpen) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->LeaveTransactionState();
        }
        if (m_pUIWareHouseDlg->IsVisible()) {
            m_pUIWareHouseDlg->LeaveWareHouseState();
        }
        if (m_pUISkillTreeDlg->IsVisible()) {
            m_pUISkillTreeDlg->Close();
        }
        if (m_pUILevelGuide->IsVisible()) {
            m_pUILevelGuide->SetVisible(false);
        }

        s_pUIMgr->SetFocusedUI(m_pUINotice);
        m_pUINotice->Open();
    } else {
        m_pUINotice->Close();
    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleUILevelGuide() {
    bool bNeedOpen = !(m_pUILevelGuide->IsVisible());

    if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) {
        return bNeedOpen;
    }

    if (bNeedOpen) {
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->LeaveTransactionState();
        }
        if (m_pUIWareHouseDlg->IsVisible()) {
            m_pUIWareHouseDlg->LeaveWareHouseState();
        }
        if (m_pUISkillTreeDlg->IsVisible()) {
            m_pUISkillTreeDlg->Close();
        }
        if (m_pUINotice->IsVisible()) {
            m_pUINotice->SetVisible(false);
        }

        s_pUIMgr->SetFocusedUI(m_pUILevelGuide);
        m_pUILevelGuide->SetVisible(true);
    } else {
        m_pUILevelGuide->SetVisible(false);
    }

    return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIMiniMap() {
    return m_pUIStateBarAndMiniMap->ToggleMiniMap();
}

void CGameProcMain::CommandCameraChange() // ī�޶� ���� �ٲٱ�..
{
    if (VP_THIRD_PERSON == s_pEng->ViewPoint() && s_pPlayer->m_bTargetOrPosMove) {
        s_pPlayer->m_bMoveContinous = false;
        s_pPlayer->m_bTargetOrPosMove = false;
        CommandToggleMoveContinous();
    }

    s_pEng->ViewPointChange(VP_UNKNOWN); // ������� ������ �ٲ۴�..
}

void CGameProcMain::CommandExitMenu() {
    if (m_pUIExitMenu) {
        return m_pUIExitMenu->SetVisible(true);
    }
}

void CGameProcMain::MsgOutput(const std::string & szMsg, D3DCOLOR crMsg) {
    m_pUIMsgDlg->AddMsg(szMsg, crMsg);
}

bool CGameProcMain::MsgRecv_ItemDroppedGetResult(DataPack * pDataPack, int & iOffset) // ���� ������ ������ �Ա� ���..
{
    BYTE        bResult = 0;
    BYTE        bPos = 0;
    int         iItemID = 0;
    int64_t     iGold = 0;
    int         iStrLen = 0;
    short       sItemCount = 0;
    std::string szString = "";

    bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    if ((bResult == 0x01) || (bResult == 0x02) || (bResult == 0x05)) {
        bPos = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        if ((bResult == 0x01) || (bResult == 0x05)) {
            sItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        }
        iGold = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
    }

    if (bResult == 0x03) {
        iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iStrLen = (int)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szString, iStrLen);
    }

    TRACE("���� - Item Get %d %lld\n", bResult, iGold);

    if (m_pUIDroppedItemDlg) {
        m_pUIDroppedItemDlg->GetItemByIDToInventory(bResult, iItemID, iGold, bPos, sItemCount, iStrLen, szString);
    }
    return true;
}

void CGameProcMain::MsgRecv_ZoneChange(DataPack * pDataPack, int & iOffset) {
    int   iZone = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
    float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;
    int   iVictoryNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    CGameProcedure::LoadingUIChange(iVictoryNation);

    __Vector3 vPosPlayer;
    vPosPlayer.x = fX;
    vPosPlayer.y = fY;
    vPosPlayer.z = fZ;
    this->InitPlayerPosition(vPosPlayer); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
    s_pPlayer->RegenerateCollisionMesh(); // �浹 �޽ø� �ٽ� �����..
    s_pPlayer->m_iSendRegeneration = 0;   // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
    s_pPlayer->m_fTimeAfterDeath = 0;     // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���

    if (s_pPlayer->IsDead()) {
        TRACE("ZoneChange - �ٽ� ��Ƴ���(%.1f, %.1f)\n", fX, fZ);

        //���� & ȿ�� �ʱ�ȭ..
        if (m_pUIStateBarAndMiniMap) {
            m_pUIStateBarAndMiniMap->ClearMagic();
        }
        if (m_pMagicSkillMng) {
            m_pMagicSkillMng->ClearDurationalMagic();
        }
        if (CGameProcedure::s_pFX) {
            s_pFX->StopMine();
        }

        if (s_pPlayer->Nation() == NATION_KARUS) {
            CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(),
                                                 -1);
        } else if (s_pPlayer->Nation() == NATION_ELMORAD) {
            CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(),
                                                 -1);
        }
    }

    this->InitZone(iZone, __Vector3(fX, fY, fZ)); // Zone Update
}

void CGameProcMain::MsgRecv_UserState(DataPack * pDataPack, int & iOffset) {
    int               iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    e_SubPacket_State eSP = (e_SubPacket_State)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01
    int               iState = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    CPlayerBase * pBPC = NULL;
    if (s_pPlayer->IDNumber() == iID) {
        pBPC = s_pPlayer;
    } else {
        pBPC = s_pOPMgr->UPCGetByID(iID, false);
    }

    if (NULL == pBPC) {
        return;
    }

    if (N3_SP_STATE_CHANGE_SITDOWN == eSP) // �ɱ�,����,����...
    {
        if (pBPC != s_pPlayer) // �÷��̾ �ƴҶ�..
        {
            if (0x01 == iState) {
                pBPC->Action(PSA_BASIC, true);
            } else if (0x02 == iState) {
                pBPC->Action(PSA_SITDOWN, true);
            } else if (0x03 == iState) {
                pBPC->Action(PSA_DYING, false, NULL, true);
            }
        }
    } else if (N3_SP_STATE_CHANGE_RECRUIT_PARTY == eSP) // ��Ƽ ����..
    {
        if (0x01 == iState) {
            pBPC->InfoStringSet("", 0);
        } else if (0x02 == iState) {
            int iLevel = pBPC->m_InfoBase.iLevel;
            int iLMin = iLevel - 8;
            if (iLMin < 0) {
                iLMin = 0;
            }
            int iLMax = iLevel + 8;
            if (iLMax > 60) {
                iLMax = 60;
            }

            char        szBuff[128];
            std::string szMsg;
            ::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
            sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
            pBPC->InfoStringSet(szBuff, 0xff00ff00);
        }
    } else if (N3_SP_STATE_CHANGE_SIZE == eSP) // ũ�� ����
    {
        if (0x01 == iState) {
            pBPC->ScaleSetGradually(1.0f); // ���� ũ��..
            pBPC->FlickerFactorSet(1.0f);  // ��Ȱ�Ǽ� ������.. ���ڰŸ���..
        } else if (0x02 == iState) {
            pBPC->ScaleSetGradually(2.0f); // Ŀ����..
        } else if (0x03 == iState) {
            pBPC->ScaleSetGradually(0.5f); // �۾�����.
        } else if (0x04 == iState) {
            pBPC->FlickerFactorSet(0.7f); // ��Ȱ�Ǽ� ������.. ���ڰŸ���..
        }
    } else if (N3_SP_STATE_CHANGE_ACTION == eSP) // ũ�� ����
    {
        switch (iState) {
        case 1u:
            pBPC->AnimationAdd(ANI_GREETING0, true); 
            break;
        case 2u:
            pBPC->AnimationAdd(ANI_GREETING1, true);
            break;
        case 3u:
            pBPC->AnimationAdd(ANI_GREETING2, true);
            break;
        case 11u:
            pBPC->AnimationAdd(ANI_WAR_CRY1, true); 
            break;
        case 12u:
            pBPC->AnimationAdd(ANI_WAR_CRY0, true);
            break;
        case 13u:
            pBPC->AnimationAdd(ANI_WAR_CRY2, true);
            break;
        }
    }
}

void CGameProcMain::MsgRecv_Notice(DataPack * pDataPack, int & iOffset) {
    if (m_pUINotice) {
        m_pUINotice->RemoveNotice();
    }

    bool bNextLineIsEventText = false;

    int iNoticeCount = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    for (int i = 0; i < iNoticeCount; i++) {
        int iStrLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        if (iStrLen <= 0) {
            continue;
        }

        std::string szNotice;
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szNotice, iStrLen);

        if (m_pUINotice) {
            if (bNextLineIsEventText) {
                m_pUINotice->m_TextsEvent.push_back(szNotice);
            } else {
                char * pChr = strrchr(szNotice.data(), '#');
                if (pChr) {
                    *pChr = ' ';
                    bNextLineIsEventText = true;
                }
                m_pUINotice->m_Texts.push_back(szNotice);
            }
        }
    }

    if (m_pUINotice && iNoticeCount > 0) {
        m_pUINotice->GenerateText();

        RECT rc = m_pUINotice->GetRegion();
        int  iX = CN3Base::s_CameraData.vp.Width - (rc.right - rc.left);
        m_pUINotice->SetPos(iX, 10);
        m_pUINotice->SetVisible(true);
    }
}

void CGameProcMain::MsgRecv_PartyOrForce(DataPack * pDataPack, int & iOffset) {
    //    int iPartyOrForce = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    int iSubCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch (iSubCmd) {
    case N3_SP_PARTY_OR_FORCE_PERMIT: // 0x02    // Send - b1(YesNo) | Recv - s1(ID) ��û�� ����� ID
    {
        int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int         iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        std::string szID;
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iStrLen);

        if (iID >= 0) {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_PARTY_PERMIT, szMsg);
            CGameProcedure::MessageBoxPost(szID + szMsg, "", MB_YESNO, BEHAVIOR_PARTY_PERMIT);
        }
    } break;

    case N3_SP_PARTY_OR_FORCE_INSERT: // 0x02    // Send - s1(ID) | Recv - s3(ID, HPMax, HP) b2(Level, Class) - ���ڿ��� ID �� �˾Ƴ���..
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        if (iID >= 0) {
            int         iIDLength = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            std::string szID;
            CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iIDLength);
            int     iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            int     iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            int     iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
            e_Class eClass = (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset));

            m_pUIPartyOrForce->MemberAdd(iID, szID, iLevel, eClass, iHP, iHPMax); // �ٸ��� ��Ƽ���߰�..
            if (iID != s_pPlayer->IDNumber())                                     // �ڱ� �ڽ��� �ƴ� ��� �޽��� ���.
            {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_PARTY_INSERT, szMsg);
                this->MsgOutput(szID + szMsg, D3DCOLOR_ARGB(255, 255, 255, 255));
            }
        } else // ��Ƽ�� ���ü� ����..
        {
            std::string szMsg;

            if (-1 == iID) {
                ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_REJECTED,
                                          szMsg); // ������ ��Ƽ�� �����⸦ ���� �Ͽ���..
            } else if (-2 == iID) {
                ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_LEVEL_DIFFERENCE, szMsg); // ���� ���̰� �ʹ� ����...
            } else if (-3 == iID) {
                ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_INVALID_NATION, szMsg); // ��Ƽ�� ���� �� ���� �����̴�.
            } else {
                ::_LoadStringFromResource(IDS_PARTY_INSERT_ERR, szMsg); // ������ ��Ƽ�� �����⸦ ���� �Ͽ���..
            }

            this->MsgOutput(szMsg, D3DCOLOR_ARGB(255, 255, 255, 255));
            if (m_pUIPartyOrForce->MemberCount() == 1) {
                m_pUIPartyOrForce->MemberDestroy(); // ����� �Ѹ��̸� ���� ��Ƽ�� ���� ����.
            }
        }

        this->UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.
    } break;

    case N3_SP_PARTY_OR_FORCE_REMOVE: // 0x03    // Send - s1(ID) | Recv - s1(ID) -
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        if (iID == s_pPlayer->IDNumber()) {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg);       // ��Ƽ�� ������..
            this->MsgOutput(szMsg, D3DCOLOR_ARGB(255, 255, 255, 255)); // ��Ƽ ���� �޽���
            m_pUIPartyOrForce->MemberDestroy();                        // �ڱ� �ڽ��̸�.. ��Ƽ�� �ǰ���..
        } else {
            int                        iMemberIndex = -1;
            const __InfoPartyOrForce * pInfo = m_pUIPartyOrForce->MemberInfoGetByID(iID, iMemberIndex);
            if (pInfo) {
                std::string szMsg;
                ::_LoadStringFromResource(IDS_PARTY_LEAVE, szMsg); // ��Ƽ�� ������..
                this->MsgOutput(pInfo->szID + szMsg,
                                D3DCOLOR_ARGB(255, 255, 255, 255)); // ���� ��Ƽ���� �����ٴ� �޽���..
                m_pUIPartyOrForce->MemberRemove(iID);               // ���̸�..
            }
        }

        this->UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.
    } break;

    case N3_SP_PARTY_OR_FORCE_DESTROY: // 0x04    // Send
    {
        m_pUIPartyOrForce->MemberDestroy(); // ��Ƽ �ǰ���..
        std::string szMsg;
        ::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg);
        this->MsgOutput(szMsg, D3DCOLOR_ARGB(255, 255, 255, 255));

        this->UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.
    } break;

    case N3_SP_PARTY_OR_FORCE_HP_CHANGE: // 0x05    // Recv - s3(ID, HPMax, HP) - �ڱ� �ڽ��̸� ��Ƽ�� ���� �Ѵ�..
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        m_pUIPartyOrForce->MemberHPChange(iID, iHP, iHPMax);
    } break;

    case N3_SP_PARTY_OR_FORCE_LEVEL_CHANGE: // 0x06    // Recv - s1(ID), b1(Level)
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        m_pUIPartyOrForce->MemberLevelChange(iID, iLevel);
    } break;

    case N3_SP_PARTY_OR_FORCE_CLASS_CHANGE: // 0x07    // Recv - s1(ID), b1(Class)�幰���� �����Ҷ�...
    {
        int     iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_Class eClass = (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset));

        m_pUIPartyOrForce->MemberClassChange(iID, eClass);
    } break;

    case N3_SP_PARTY_OR_FORCE_STATUS_CHANGE: // 0x08    // Recv - s1(ID), b1(Status)...��, ����, ���Ӽ�����, �ູ
    {
        int           iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_PartyStatus ePS = (e_PartyStatus)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        int           iSuffer = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        bool          bSuffer = (iSuffer) ? true : false;

        m_pUIPartyOrForce->MemberStatusChange(iID, ePS, bSuffer);
    } break;
    }
}

void CGameProcMain::CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately) {
    if (bSitDown == s_pPlayer->m_bSitDown) {
        return; // ���°� ������ ���ư���..
    }

    // �ɱ� ���� ����..
    if (bLimitInterval) {
        float        fTime = CN3Base::TimeGet();
        static float fTimePrev = fTime - 4.0f;
        if (fTime - fTimePrev < 3.0f) {
            return;
        }
        fTimePrev = fTime;
    }

    e_StateAction eSA = s_pPlayer->State();
    e_StateMove   eSM = s_pPlayer->StateMove();

    if (bSitDown) {
        if (eSM != PSM_STOP) { // �ٴ� ���̸� �����.
            return;
        }

        if (this->m_pMagicSkillMng->IsCasting()) { // ��ų ĳ���� ���̸� ���� ���Ѵ�..
            return;
        }

        if (s_pPlayer->m_bAttackContinous) {                 // ��� �����ϴ� ���̸�..
            this->CommandEnableAttackContinous(false, NULL); // ��� ���� ���..
        }
    }

    bool bVBs[2] = {false, false};
    int  iState = 0;
    if (bSitDown) {
        eSA = PSA_SITDOWN;
        bVBs[0] = false;
        bVBs[1] = true;
        iState = 0x02;
    } // �ɴ� ���
    else {
        eSA = PSA_BASIC;
        bVBs[0] = true;
        bVBs[1] = false;
        iState = 0x01;
    } // �Ͼ�� ���

    s_pPlayer->m_bSitDown = bSitDown;
    s_pPlayer->Action(eSA, true, NULL, bImmediately);

    if (m_pUICmd->m_pBtn_Act_SitDown) {
        m_pUICmd->m_pBtn_Act_SitDown->SetVisible(bVBs[0]);
        m_pUICmd->m_pBtn_Act_SitDown->SetState(UI_STATE_BUTTON_DOWN); // ��ư ����
    }
    if (m_pUICmd->m_pBtn_Act_StandUp) {
        m_pUICmd->m_pBtn_Act_StandUp->SetVisible(bVBs[1]);
        m_pUICmd->m_pBtn_Act_StandUp->SetState(UI_STATE_BUTTON_DOWN); // ��ư ����
    }

    this->MsgSend_StateChange(N3_SP_STATE_CHANGE_SITDOWN, iState); // �ɾҴ�.. ��Ŷ..
}

void CGameProcMain::CommandTargetSelect_NearstEnemy() // ���� ����� �� Ÿ�� ���..
{
    CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByNearstEnemy(s_pPlayer->m_InfoBase.eNation, s_pPlayer->Position());
    this->TargetSelect(pTarget);
    s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CommandTargetSelect_NearstOurForce() // ���� ����� ��Ƽ Ÿ�����..
{
    CPlayerOther * pTarget = m_pUIPartyOrForce->MemberGetByNearst(s_pPlayer->Position());
    this->TargetSelect(pTarget);
    s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CloseUIs() {}

void CGameProcMain::MsgSend_StateChange(e_SubPacket_State eSP, int iState) {
    BYTE byBuff[4];   // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_STATE_CHANGE); // ���� ��ȭ..
    CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
    CAPISocket::MP_AddByte(byBuff, iOffset, iState);

    s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_PerTradeReq(int iDestID, bool bNear) {
    BYTE byBuff[8]{}; // ��Ŷ ����..
    int  iOffset = 0; // ��Ŷ ������..

    CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);
    CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_REQ);
    CAPISocket::MP_AddShort(byBuff, iOffset, (short)iDestID); // ���� ���̵�..
    if (bNear) {
        CAPISocket::MP_AddByte(byBuff, iOffset, 1);
    } else {
        CAPISocket::MP_AddByte(byBuff, iOffset, 2);
    }
    //1:�Ϲ� �ŷ�
    //2:��ŷ� �Խ��� �ŷ�

    s_pSocket->Send(byBuff, iOffset); // ����..

    TRACE("���̵�: %d, ������ �ŷ� ��û ��Ŷ ����.. \n", iDestID);
}

void CGameProcMain::MsgRecv_PerTrade(DataPack * pDataPack, int & iOffset) {
    BYTE    bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    short   sOtherID, sItemCount, sCount, sDurability;
    BYTE    bResult, bItemPos;
    int     iItemID;
    int64_t iCount, iTotalGold;

    switch (bSubCom) {
    case N3_SP_PER_TRADE_REQ:
        TRACE("������ �ŷ� ��û ��Ŷ ����.. \n");
        sOtherID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        if (m_pUITransactionDlg->IsVisible()) /* ���ΰ� �ŷ����̸�.. */
        {
            // �����Ѵ�..
            m_pUITransactionDlg->LeaveTransactionState();
            m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
            break;
        }

        if (m_pUIWareHouseDlg->IsVisible()) /* �����Կ� �������̸�.. */
        {
            // �����Ѵ�..
            m_pUIWareHouseDlg->LeaveWareHouseState();
            m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
            break;
        }

        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->Close();
        }

        if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) {
            std::string stdMsg;
            ::_LoadStringFromResource(IDS_PER_TRADEING_OTHER, stdMsg);
            CGameProcedure::s_pProcMain->MsgOutput(stdMsg, 0xff9b9bff);
            break;
        }

        if (m_pUIQuestMenu->IsVisible()) {
            m_pUIQuestMenu->SetVisible(false);
        }

        if (m_pUIQuestTalk->IsVisible()) {
            m_pUIQuestTalk->SetVisible(false);
        }

        if (m_pUINpcEvent->IsVisible()) {
            m_pUINpcEvent->Close();
        }

        if (m_pUITradeBBSSelector->IsVisible()) {
            m_pUITradeBBSSelector->SetVisible(false);
        }

        if (m_pUITradeBBS->IsVisible()) {
            m_pUITradeBBS->SetVisible(false);
        }

        m_pSubProcPerTrade->ReceiveMsgPerTradeReq(sOtherID);
        break;

    case N3_SP_PER_TRADE_AGREE:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        m_pSubProcPerTrade->ReceiveMsgPerTradeAgree(bResult);
        break;

    case N3_SP_PER_TRADE_ADD:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        m_pSubProcPerTrade->ReceiveMsgPerTradeAdd(bResult);
        break;

    case N3_SP_PER_TRADE_OTHER_ADD:
        iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iCount = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
        sDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pSubProcPerTrade->ReceiveMsgPerTradeOtherAdd(iItemID, iCount, (int)sDurability);
        break;

    case N3_SP_PER_TRADE_OTHER_DECIDE:
        m_pSubProcPerTrade->ReceiveMsgPerTradeOtherDecide();
        break;

    case N3_SP_PER_TRADE_DONE:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        if (bResult == 0x01) // �����̸�..
        {
            iTotalGold = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
            m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessBegin(iTotalGold);
            sItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            for (int i = 0; i < sItemCount; i++) {
                bItemPos = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
                iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
                sCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
                sDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
                m_pSubProcPerTrade->ReceiveMsgPerTradeDoneItemMove(bItemPos, iItemID, sCount, sDurability);
            }
            m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessEnd();
        } else { // �����̸�..
            m_pSubProcPerTrade->ReceiveMsgPerTradeDoneFail();
        }
        break;

    case N3_SP_PER_TRADE_CANCEL:
        m_pSubProcPerTrade->ReceiveMsgPerTradeCancel();
        break;
    }
}

void CGameProcMain::TargetSelect(int iID, bool bMustAlive) {
    CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(iID, bMustAlive);
    this->TargetSelect(pTarget);
}

void CGameProcMain::TargetSelect(CPlayerNPC * pTarget) {
    if (PSA_SPELLMAGIC == s_pPlayer->State()) { // ���� ĳ������ �ϴ� �߰����� Ÿ���� �� �ٲ۴�..
        return;
    }

    int iIDTargetPrev = s_pPlayer->m_iIDTarget; // ���� Ÿ�� ���..
    s_pPlayer->m_iIDTarget = -1;
    s_pPlayer->m_pObjectTarget = NULL;

    if (pTarget && pTarget->m_InfoBase.iAuthority != AUTHORITY_MANAGER) { //��ڴ� ��Ŀ���� ���� �ʰ� �ϱ� ���ؼ�...
        s_pPlayer->m_iIDTarget = pTarget->IDNumber();
        if (iIDTargetPrev != s_pPlayer->m_iIDTarget) // ���� Ÿ�ٰ� �ٸ���
        {
            this->CommandEnableAttackContinous(false, NULL); // �ڵ� ���� ���..

            D3DCOLOR crID = 0xffffffff;
            if (pTarget->PlayerType() == PLAYER_OTHER) // User..
            {
                if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) {
                    crID = 0xffff4040; // �ٸ� �����̸�
                } else {
                    crID = 0xff6b9fff;
                }
            } else // NPC
            {
                if (pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) {
                    crID = 0xffff6060; // �ٸ� �����̸�
                } else {
                    crID = 0xff1064ff;
                }
            }

            if (pTarget->IsAlive()) //����ִ� ���¸� target bar�� �����ش�.
            {
                m_pUITargetBar->SetVisible(true);
                m_pUITargetBar->SetIDString(pTarget->IDString(), crID);      // �̸��� �ְ�..
                this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, 0x01); // �������� HP������ ��û�Ѵ�..
            } else {
                m_pUITargetBar->SetVisible(false); // Ÿ�� �� ���ֱ�..
            }
        }
    } else {
        s_pPlayer->m_iIDTarget = -1;       // Ÿ�� ���̵� ���..
        m_pUITargetBar->SetVisible(false); // Ÿ�� �� ���ֱ�..

        this->CommandEnableAttackContinous(false, NULL); // �ڵ� ���� ���..
    }

    if (PSA_SITDOWN != s_pPlayer->State() && PSM_STOP == s_pPlayer->StateMove() && PSA_BASIC == s_pPlayer->State()) {
        s_pPlayer->Action(PSA_BASIC, true); // �⺻ �ڼ� �ٲٱ�..
    }

    this->UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.
}

void CGameProcMain::MsgRecv_SkillChange(DataPack * pDataPack, int & iOffset) // ��ų ��ȭ..
{
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    int iValue = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    m_pUISkillTreeDlg->m_iSkillInfo[iType] = iValue;
    m_pUISkillTreeDlg->m_iSkillInfo[0]++;
    m_pUISkillTreeDlg->InitIconUpdate(); // ��ų ����Ʈ�� ��ȭ�Ǿ����Ƿ� .. ��ų�� ��ȭ�� �� �ִ�..
}

void CGameProcMain::MsgRecv_MagicProcess(DataPack * pDataPack, int & iOffset) {
    e_SubPacket_Magic eSP = (e_SubPacket_Magic)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch (eSP) {
    case N3_SP_MAGIC_CASTING:
        m_pMagicSkillMng->MsgRecv_Casting(pDataPack, iOffset);
        break;
    case N3_SP_MAGIC_FLYING:
        m_pMagicSkillMng->MsgRecv_Flying(pDataPack, iOffset);
        break;
    case N3_SP_MAGIC_EFFECTING:
        m_pMagicSkillMng->MsgRecv_Effecting(pDataPack, iOffset);
        break;
    case N3_SP_MAGIC_FAIL:
        m_pMagicSkillMng->MsgRecv_Fail(pDataPack, iOffset);
        break;
    case N3_SP_MAGIC_TYPE4BUFFTYPE:
        m_pMagicSkillMng->MsgRecv_BuffType(pDataPack, iOffset);
        break;
    }
}

void CGameProcMain::MsgRecv_ClassChange(DataPack * pDataPack, int & iOffset) // ���� ��ȭ..
{
    e_SubPacket_ClassChange eSP = (e_SubPacket_ClassChange)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch (eSP) {
    case N3_SP_CLASS_CHANGE_SUCCESS: // Success..
        m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_SUCCESS);
        break;

    case N3_SP_CLASS_CHANGE_NOT_YET: // Not Yet..
        m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_NOT_YET);
        break;

    case N3_SP_CLASS_CHANGE_ALREADY: // Already..
        m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_ALREADY);
        break;

    case N3_SP_CLASS_CHANGE_FAILURE: // Failure..
        m_pUIClassChange->RestorePrevClass();
        break;
    }
}

void CGameProcMain::MsgRecv_ObjectEvent(DataPack * pDataPack, int & iOffset) {
    int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);   // Event Type
    int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���..

    if (OBJECT_TYPE_BINDPOINT == iType) // ���ε� ����Ʈ
    {
        std::string szMsg;
        if (0x01 == iResult) {
            ::_LoadStringFromResource(IDS_BIND_POINT_SUCCESS, szMsg);
        } else {
            ::_LoadStringFromResource(IDS_BIND_POINT_FAILED, szMsg);
        }
        this->MsgOutput(szMsg, 0xff00ff00);
    } else if (OBJECT_TYPE_DOOR_LEFTRIGHT == iType || OBJECT_TYPE_DOOR_TOPDOWN == iType ||
               OBJECT_TYPE_LEVER_TOPDOWN == iType || OBJECT_TYPE_FLAG == iType) // ������Ʈ �ٷ��..
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);      // ���� ���� ���� ID
        int iActivate = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���� ����..

        CPlayerNPC * pNPC = s_pOPMgr->NPCGetByID(iID, true);
        __ASSERT(pNPC, "Invalid NPC ID");
        if (pNPC) {
            __ASSERT(pNPC->m_pShapeExtraRef && (pNPC->m_pShapeExtraRef->Type() & OBJ_SHAPE_EXTRA), "Invalid object!!");
            std::string szMsg = "Object Event - invalid object!!";
            if (pNPC->m_pShapeExtraRef) {
                __Vector3 vAxis(0, 1, 0);
                float     fRadian = D3DXToRadian(90);
                bool      bShouldBeRotate = true;        // ������ �ϴ���??
                if (OBJECT_TYPE_DOOR_LEFTRIGHT == iType) // �¿쿭�� ����
                {
                    if (0x01 == iActivate) {
                        fRadian = D3DXToRadian(80);
                        ::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
                    } else {
                        fRadian = D3DXToRadian(0);
                        ::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
                    }
                    vAxis.Set(0, 1, 0);
                } else if (OBJECT_TYPE_DOOR_TOPDOWN == iType) // ���Ͽ��� ����
                {
                    if (0x01 == iActivate) {
                        fRadian = D3DXToRadian(90);
                        ::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
                    } else {
                        D3DXToRadian(0);
                        ::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
                    }
                    vAxis.Set(0, 0, 1);
                } else if (OBJECT_TYPE_LEVER_TOPDOWN == iType) // ���� ����
                {
                    if (0x01 == iActivate) {
                        fRadian = D3DXToRadian(-45);
                        ::_LoadStringFromResource(IDS_LEVER_ACTIVATE, szMsg);
                    } else {
                        fRadian = D3DXToRadian(45);
                        ::_LoadStringFromResource(IDS_LEVER_DEACTIVATE, szMsg);
                    }
                    vAxis.Set(1, 0, 0);
                } else if (OBJECT_TYPE_FLAG == iType) {
                    bShouldBeRotate = false; // ������ �ϴ���??
                }

                if (0x01 == iActivate) {
                } else // if(0x00 == iActivate);
                {
                    ::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
                }

                CN3ShapeExtra * pSE = (CN3ShapeExtra *)pNPC->m_pShapeExtraRef;
                if (bShouldBeRotate) {
                    pSE->RotateTo(0, vAxis, fRadian, 1);
                    pSE->RotateTo(1, vAxis, -fRadian, 1);
                } else {
                    if (iActivate) {
                        pSE->m_bVisible = true;
                    } else {
                        pSE->m_bVisible = false;
                    }
                }
            }
            this->MsgOutput(szMsg, 0xff00ff00);
        }
    } else {
        __ASSERT(0, "Unknown Object Event");
    }
}

void CGameProcMain::ParseChattingCommand(const std::string & szCmd) {
    static char szCmds[4][1024] = {"", "", "", ""};
    static BYTE byBuff[1024] = "";
    sscanf(szCmd.c_str(), "/%s %s %s %s", szCmds[0], szCmds[1], szCmds[2], szCmds[3]);

    //upper command
    std::transform(szCmds[0], szCmds[0] + std::strlen(szCmds[0]), szCmds[0],
                [](unsigned char c) { return std::toupper(c); });

    if (0 == lstrcmp(szCmds[0], "GOTO")) {
        float fX = (float)atof(szCmds[1]);
        float fZ = (float)atof(szCmds[2]);

        int iOffset = 0;
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_WARP);
        CAPISocket::MP_AddWord(byBuff, iOffset, (fX * 10));
        CAPISocket::MP_AddWord(byBuff, iOffset, (fZ * 10));

        s_pSocket->Send(byBuff, iOffset);
    } else if (0 == lstrcmp(szCmds[0], "SYSTEMGAMETIMEON")) {
        if (m_pUIStateBarAndMiniMap) {
            m_pUIStateBarAndMiniMap->SetSystemTimeVisibility(true);
        }
    } else if (0 == lstrcmp(szCmds[0], "SYSTEMGAMETIMEOFF")) {
        if (m_pUIStateBarAndMiniMap) {
            m_pUIStateBarAndMiniMap->SetSystemTimeVisibility(false);
        }
    }

    int eCmd = -1;

    if (m_pUICmdList->g_commandMappings.count(szCmds[0]) > 0) {
        eCmd = m_pUICmdList->g_commandMappings[szCmds[0]];
    }

    switch (eCmd) {
    ////////////////////////////////////////////////////////////////////////////////////////
    //PRIVATE
    ////////////////////////////////////////////////////////////////////////////////////////
    case iPrivateCommand ::CMD_WHISPER: {
        this->MsgSend_ChatSelectTarget(szCmds[1]);
    } break;

    case iPrivateCommand::CMD_TOWN: {
        if (s_pPlayer->m_bStun) {
            return;
        }
        if (s_pPlayer->m_InfoBase.iHP * 2 >= s_pPlayer->m_InfoBase.iHPMax) 
        {
            int iOffset = 0;
            CAPISocket::MP_AddWord(byBuff, iOffset, N3_HOME); 
            s_pSocket->Send(byBuff, iOffset);
        } else 
        {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_ERR_GOTO_TOWN_OUT_OF_HP, szMsg);
            this->MsgOutput(szMsg, 0xffff00ff);
        }
    } break;

    case iPrivateCommand::CMD_EXIT: {
        PostQuitMessage(0);
    } break;

    case iPrivateCommand::CMD_GREETING:
    case iPrivateCommand::CMD_GREETING2:
    case iPrivateCommand::CMD_GREETING3: {
        if (s_pPlayer->State() == PSA_BASIC && s_pPlayer->StateMove() == PSM_STOP) {
            this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 1 + (eCmd - CMD_GREETING));
        }
    } break;

    case iPrivateCommand::CMD_PROVOKE:
    case iPrivateCommand::CMD_PROVOKE2:
    case iPrivateCommand::CMD_PROVOKE3: {
        if (s_pPlayer->State() == PSA_BASIC && s_pPlayer->StateMove() == PSM_STOP) {
            this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 11 + (eCmd - CMD_PROVOKE));
        }
    } break;

    case iPrivateCommand::CMD_GAME_SAVE: {
        if (m_fRequestGameSave > 300.0f) {
            BYTE byBuff[4];                                              
            int  iOffset = 0;                                             
            s_pSocket->MP_AddByte(byBuff, iOffset, N3_REQUEST_GAME_SAVE); // Save request command..
            s_pSocket->Send(byBuff, iOffset);                             
            m_fRequestGameSave = 0.0f;

            std::string szMsg;
            ::_LoadStringFromResource(IDS_REQUEST_GAME_SAVE, szMsg);
            this->MsgOutput(szMsg, 0xffffff00);
        } else {
            char        szBuf[256];
            std::string szMsg;
            ::_LoadStringFromResource(IDS_DELAY_GAME_SAVE, szMsg);
            sprintf(szBuf, szMsg.c_str(), 5);
            this->MsgOutput(szBuf, 0xffffff00);
        }
    } break;

    case iPrivateCommand::CMD_RECOMMEND: {
        //TODO
    } break;
    case iPrivateCommand::CMD_INDIVIDUAL_BATTLE: {
        //TODO
    } break;
    ////////////////////////////////////////////////////////////////////////////////////////
    // TRADE
    ////////////////////////////////////////////////////////////////////////////////////////
    case iTradeCommand::CMD_TRADE: {
        CPlayerOther * pOPC = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
        if (pOPC && (pOPC->Position() - s_pPlayer->Position()).Magnitude() < (pOPC->Height() + 5.0f) &&
            !m_pUITransactionDlg
                 ->IsVisible()) // If another player is targeted and nearby.. // Individual item trade.. // If not in trade session..
        {
            std::string szMsg;
            ::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
            MsgOutput(pOPC->IDString() + szMsg, 0xffffff00);

            MsgSend_PerTradeReq(pOPC->IDNumber());

            if (m_pUINpcEvent->IsVisible()) {
                m_pUINpcEvent->Close();
            }

            if (m_pUIQuestTalk->IsVisible()) {
                m_pUIQuestTalk->SetVisible(false);
            }

            if (m_pUIQuestMenu->IsVisible()) {
                m_pUIQuestMenu->SetVisible(false);
            }

            m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq();
        }

    } break;
    case iTradeCommand::CMD_FORBIDTRADE:
        //TODO
        break;

    case iTradeCommand::CMD_PERMITTRADE:
        //TODO
        break;

    case iTradeCommand::CMD_MERCHANT:
        //TODO
        break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //PARTY
    ////////////////////////////////////////////////////////////////////////////////////////
    case iPartyCommand::CMD_PARTY: {
        CPlayerBase * pTarget = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
        if (pTarget) {
            std::string szMsg;
            if (this->MsgSend_PartyOrForceCreate(0, pTarget->IDString())) {
                ::_LoadStringFromResource(IDS_PARTY_INVITE, szMsg); // Party request..
            } else {
                ::_LoadStringFromResource(IDS_PARTY_INVITE_FAILED, szMsg); // Party invitation failed
            }
            this->MsgOutput(pTarget->IDString() + szMsg, 0xffffff00);
        }
    } break;
    case iPartyCommand::CMD_LEAVEPARTY: {
        this->MsgSend_PartyOrForceLeave(0); // Party request..
    } break;
    case iPartyCommand::CMD_SEEKING_PARTY: {
        if (m_pUIPartyBBS) {
            if (s_pPlayer->m_bRecruitParty) {
                m_pUIPartyBBS->MsgSend_RegisterCancel();
            } else {
                m_pUIPartyBBS->MsgSend_Register();
            }
        }

        //            if(m_pUIPartyBBS && !m_pUIPartyBBS->IsVisible())
        //            m_pUIPartyBBS->MsgSend_RefreshData(0);
    } break;
    case iPartyCommand::CMD_FORBIDPARTY:
        //TODO
        break;
    case iPartyCommand::CMD_PERMITPARTY:
        //TODO
        break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //CLAN
    ////////////////////////////////////////////////////////////////////////////////////////
    case iClanCommand::CMD_JOINCLAN: {
        if (s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF ||
            s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_VICECHIEF) {
            this->MsgSend_KnightsJoin(s_pPlayer->m_iIDTarget);
        }
    } break;
    case iClanCommand::CMD_WITHDRAWCLAN: {
        this->MsgSend_KnightsWithdraw();
    } break;
    case iClanCommand::CMD_FIRECLAN: {
        if (s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF) {
            std::string szName = szCmds[1];
            MsgSend_KnightsLeave(szName);
        }
    } break;
    case iClanCommand::CMD_COMMAND: {
        //TODO
    } break;
    case iClanCommand::CMD_CLAN_WAR: {
        //TODO
    } break;
    case iClanCommand::CMD_SURRENDER: {
        //TODO
    } break;
    case iClanCommand::CMD_APPOINTVICECHIEF: {
        if (s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF) {
            std::string szName = szCmds[1];
            MsgSend_KnightsAppointViceChief(szName);
        }
    } break;
    case iClanCommand::CMD_CLAN_CHAT: {
        //TODO
    } break;
    case iClanCommand::CMD_CLAN_BATTLE: {
        //TODO
    } break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //KNIGHTS
    ////////////////////////////////////////////////////////////////////////////////////////
    case iKnightsCommand::CMD_CONFEDERACY: {
        //TODO 1298
    } break;
    case iKnightsCommand::CMD_BAN_KNIGHTS: {
        //TODO 1298
    } break;
    case iKnightsCommand::CMD_QUIT_KNIGHTS: {
        //TODO 1298
    } break;
    case iKnightsCommand::CMD_BASE: {
        //TODO 1298
    } break;
    case iKnightsCommand::CMD_DECLARATION: {
        //TODO 1298
    } break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //GUARDIAN MONSTER
    ////////////////////////////////////////////////////////////////////////////////////////
    case iGuardianCommand::CMD_GUARD_HIDE: {
        //TODO 1298
    } break; 
    case iGuardianCommand::CMD_GUARD: {
        //TODO 1298
    } break;
    case iGuardianCommand::CMD_GUARD_DEFEND: {
        //TODO 1298
    } break;
    case iGuardianCommand::CMD_GUARD_LOOK_OUT: {
        //TODO 1298
    } break;
    case iGuardianCommand::CMD_GUARD_FORMATION: {
        //TODO 1298
    } break;
    case iGuardianCommand::CMD_GUARD_REST: {
        //TODO 1298
    } break;
    case iGuardianCommand::CMD_GUARD_DESTROY: {
        //TODO 1298
    } break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //KING
    ////////////////////////////////////////////////////////////////////////////////////////
    case iKingCommand::CMD_KING_ROYALORDER: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_PRIZE: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_EXPRATE: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_DROPRATE: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_RAIN: {
        //TODO 1298
    } break; 
    case iKingCommand::CMD_KING_SNOW: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_CLEAR: {
        //TODO 1298
    } break;
    case iKingCommand::CMD_KING_REWARD: {
        //TODO 1298
    } break;
    ////////////////////////////////////////////////////////////////////////////////////////
    //GM
    ////////////////////////////////////////////////////////////////////////////////////////
    case iGmCommand::CMD_VISIBLE: {
    } break;
    case iGmCommand::CMD_INVISIBLE: {
    } break;
    case iGmCommand::CMD_CLEAN: {
        int iPercent = atoi(szCmds[1]);
        this->MsgSend_Weather(1, iPercent);
    } break;

    case iGmCommand::CMD_RAINING: {
        int iPercent = atoi(szCmds[1]);
        this->MsgSend_Weather(2, iPercent);
    } break;
    case iGmCommand::CMD_SNOWING: {
        int iPercent = atoi(szCmds[1]);
        this->MsgSend_Weather(3, iPercent);
    } break;
    case iGmCommand::CMD_TIME: {
        int iHour = atoi(szCmds[1]);
        int iMin = atoi(szCmds[2]);
        this->MsgSend_Time(iHour, iMin);
    } break;
    case iGmCommand::CMD_CU_COUNT: {
        int iOffset = 0;
        CAPISocket::MP_AddByte(byBuff, iOffset, N3_CONCURRENT_USER_COUNT);
        s_pSocket->Send(byBuff, iOffset);
    } break;
    case iGmCommand::CMD_NOTICE: {
        if (szCmd.size() >= 7) {
            std::string szChat = szCmd.substr(6); // The remaining string excluding "/Notice"
            this->MsgSend_Chat(N3_CHAT_PUBLIC, szChat);
        }
    }
    case iGmCommand::CMD_ARREST: {
        this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_ARREST, szCmds[1]); 
    } break;
    case iGmCommand::CMD_FORBIDCONNECT: {
        this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_FORBID_CONNECT, szCmds[1]); 
    } break;
    case iGmCommand::CMD_FORBIDCHAT: {
        this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_FORBID, szCmds[1]);
    } break;
    case iGmCommand::CMD_PERMITCHAT: {
        this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_PERMIT, szCmds[1]); 
    } break;
    case iGmCommand::CMD_NOTICEALL: {
        //TODO
    } break;
    case iGmCommand::CMD_CUTOFF: {
        //TODO
    } break;
    case iGmCommand::CMD_VIEW: {
        //TODO
    } break;
    case iGmCommand::CMD_UNVIEW: {
        //TODO
    } break;
    case iGmCommand::CMD_FORBIDUSER: {
        //TODO
    } break;
    case iGmCommand::CMD_SUMMONUSER: {
        //TODO
    } break;
    case iGmCommand::CMD_ATTACKDISABLE: {
        //TODO
    } break;
    case iGmCommand::CMD_ATTACKENABLE: {
        //TODO
    } break;
    case iGmCommand::CMD_PLC: {
        //TODO
    } break;
    default:
        break;
    }
}

void CGameProcMain::UpdateUI_PartyOrForceButtons() {
    // Change party button state..
    CPlayerBase * pTarget = NULL;
    bool          bIAmLeader = false, bIAmMemberOfParty = false;
    int           iMemberIndex = -1;
    this->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
    m_pUIPartyOrForce->MemberSelect(iMemberIndex);

    m_pUICmd->UpdatePartyButtons(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
}

const __InfoPartyOrForce * CGameProcMain::PartyOrForceConditionGet(bool & bIAmLeader, bool & bIAmMember,
                                                                   int & iMemberIndex, class CPlayerBase *& pTarget) {
    // Change party button state..
    bIAmLeader = false;
    iMemberIndex = -1;
    bIAmMember = false;
    pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

    if (m_pUIPartyOrForce->MemberCount() >= 2) {
        bIAmMember = true;
        if (m_pUIPartyOrForce->MemberInfoGetByIndex(0)->iID == s_pPlayer->IDNumber()) {
            bIAmLeader = true;
        }
        return m_pUIPartyOrForce->MemberInfoGetByID(s_pPlayer->m_iIDTarget, iMemberIndex);
    }

    return NULL;
}

void CGameProcMain::UpdateUI_MiniMap() {
    if (NULL == m_pUIStateBarAndMiniMap || !m_pUIStateBarAndMiniMap->IsVisible()) {
        return;
    }

    m_pUIStateBarAndMiniMap->PositionInfoClear();

    D3DCOLOR crType = 0xffffffff;
    e_Nation eNation = s_pPlayer->m_InfoBase.eNation;

    it_NPC       it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
    CPlayerNPC * pNPC = NULL;
    for (; it != itEnd; it++) // NPC
    {
        pNPC = it->second;

        if (eNation != pNPC->m_InfoBase.eNation) {
            crType = 0xff800000; // �ٸ� ���� NPC Ȥ�� ���� ��Ȳ��
        } else {
            crType = 0xff00a0ff; // ���� ���� NPC �ϴû�
        }

        m_pUIStateBarAndMiniMap->PositionInfoAdd(pNPC->IDNumber(), pNPC->Position(), crType, false);
    }

    it_UPC         it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
    CPlayerOther * pUPC = NULL;
    __TABLE_ZONE * pZoneInfo = s_pTbl_Zones->Find(s_pPlayer->m_InfoExt.iZoneCur);
    for (; it2 != itEnd2; it2++) // User
    {
        pUPC = it2->second;

        bool bDrawTop = false;
        if (eNation != pUPC->m_InfoBase.eNation) // �����ϰ��
        {
            if (pUPC->State() == PSA_SITDOWN) {
                pUPC->m_InfoBase.bRenderID = false; // ���̵� ǥ������ ����.
                continue;                           // �ɾ�������.. ��������..
            } else {
                pUPC->m_InfoBase.bRenderID = true; // ���̵� ǥ����.
            }

            if (pZoneInfo && FALSE == pZoneInfo->bIndicateEnemyPlayer) {
                continue; // ������ ǥ�� ���� �ʾƾ� �Ѵٸ�.. ��������..
            }

            crType = 0xffff4040; // �ٸ� ����  - ���� ������
        } else                   // ���� ������..
        {
            int                        iMemberIndex = -1;
            const __InfoPartyOrForce * pPI = m_pUIPartyOrForce->MemberInfoGetByID(pUPC->IDNumber(), iMemberIndex);
            if (pPI) {
                bDrawTop = true;     // �� ���� �׸���.
                crType = 0xffffff00; // ��Ƽ ����� // �����
            } else {
                crType = 0xff0040ff; // ��Ƽ ����� �ƴϸ� ���� �Ķ���
            }
        }

        if (pUPC &&
            pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER) { // ��ڰ� �ƴѰ�츸 �̴ϸʿ� ����Ʈ�� ����ش�.
            m_pUIStateBarAndMiniMap->PositionInfoAdd(pUPC->IDNumber(), pUPC->Position(), crType, bDrawTop);
        }
    }
}

void CGameProcMain::UpdateUI_TargetBar() {
    if (NULL == m_pUITargetBar || !m_pUITargetBar->IsVisible()) {
        return;
    }

    //���� ĳ���Ͱ� ���õǾ������� target bar�� �׷����� �ʴ´�.
    CPlayerNPC * t_pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
    if (t_pTarget && t_pTarget->State() == PSA_DEATH) {
        m_pUITargetBar->SetVisible(false);
        return;
    }

    CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);
    if (pTarget) // Ÿ���� ��ȿ�ϸ�..
    {
        float fTimeTmp = CN3Base::TimeGet();
        if (fTimeTmp >
            m_pUITargetBar->m_fTimeSendPacketLast + PACKET_INTERVAL_REQUEST_TARGET_HP) // 1�ʰ� ������ Ÿ�� ���� ��û
        {
            this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, false);
            m_pUITargetBar->m_fTimeSendPacketLast = fTimeTmp;
        }
    } else // Ÿ���� ������..
    {
        s_pPlayer->m_iIDTarget = -1;
        m_pUITargetBar->SetVisible(false);
    }
}

void CGameProcMain::UpdateBGM() {
    if (NULL == m_pSnd_Battle || !m_pSnd_Battle->IsPlaying()) {
        return;
    }

    //    if(s_pPlayer->pTarget && s_pPlayer->pTarget->IsAlive())
    __Vector3 vPosPlayer = s_pPlayer->Position();
    e_Nation  eNationPlayer = s_pPlayer->m_InfoBase.eNation;

    bool          bStopBattleBgm = true;
    CPlayerBase * pBPC;
    it_NPC        it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
    for (; it != itEnd && bStopBattleBgm; it++) {
        pBPC = it->second;
        if (eNationPlayer == pBPC->m_InfoBase.eNation) {
            continue;
        }

        if ((vPosPlayer - pBPC->Position()).Magnitude() < 12.0f) {
            bStopBattleBgm = false;
        }
    }

    CPlayerOther * pUPC;
    it_UPC         it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
    for (; it2 != itEnd2 && bStopBattleBgm; it2++) {
        pUPC = it2->second;
        if (eNationPlayer == pUPC->m_InfoBase.eNation) {
            continue;
        }

        if ((vPosPlayer - pUPC->Position()).Magnitude() < 12.0f) {
            bStopBattleBgm = false;
        }
    }

    if (bStopBattleBgm) {
        this->PlayBGM_Town();
    }
}

void CGameProcMain::UpdateCameraAndLight() {
    __Vector3 vPosPlayer = s_pPlayer->Position();
    if (s_pPlayer->State() == PSA_SITDOWN) {
        float fRootY = s_pPlayer->RootPosition().y;
        float fH = s_pPlayer->Height();
        vPosPlayer.y += fRootY - (fH / 2.0f);
    }

    D3DCOLOR crDiffuses[MAX_GAME_LIGHT];
    D3DCOLOR crAmbients[MAX_GAME_LIGHT];
    for (int i = 0; i < MAX_GAME_LIGHT; i++) {
        crDiffuses[i] = ACT_WORLD->GetLightDiffuseColorWithSky(i);
        crAmbients[i] = ACT_WORLD->GetLightAmbientColorWithSky(i);
    }

    s_pEng->Tick(crDiffuses, crAmbients, ACT_WORLD->GetFogColorWithSky(), vPosPlayer, s_pPlayer->Rotation(),
                 s_pPlayer->Height(), ACT_WORLD->GetSunAngleByRadinWithSky()); // ĳ���� ��ġ�� ���� ������ �־��ش�..
    s_pEng->ApplyCameraAndLight(); // ī�޶�� ����Ʈ�� ���õ� ���� D3D Device �� �����Ѵ�.
}

void CGameProcMain::MsgRecv_DurabilityChange(DataPack * pDataPack, int & iOffset) // ������ ����..
{
    e_ItemSlot eSlot = (e_ItemSlot)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    int        iCurValue = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

    s_pPlayer->DurabilitySet(eSlot, iCurValue);
    m_pUIInventory->DurabilityChange(eSlot, iCurValue);
}

void CGameProcMain::MsgRecv_NpcEvent(DataPack * pDataPack, int & iOffset) // Npc Event(Exchange, Repair both)..
{
    int          iTradeID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // Trade id
    CPlayerNPC * pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
    if (!pNPC) {
        return;
    }

    m_pUINpcEvent->Open(NPC_EVENT_TRADE_REPAIR, iTradeID, pNPC->GetNPCOriginID());
}

void CGameProcMain::MsgRecv_Knights(DataPack * pDataPack, int & iOffset) {
    e_SubPacket_Knights eSP =
        (e_SubPacket_Knights)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset)); // Sub Packet

    switch (eSP) {
    case N3_SP_KNIGHTS_CREATE: // ����..
        this->MsgRecv_Knights_Create(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_WITHDRAW: //Ż��
        this->MsgRecv_Knights_Withdraw(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_JOIN: //����
        this->MsgRecv_Knights_Join(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_MEMBER_REMOVE: //��� ���� -
        this->MsgRecv_Knights_Leave(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_APPOINT_VICECHIEF: //�δ��� �Ӹ� - �����㰡�� ����
        this->MsgRecv_Knights_AppointViceChief(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_MEMBER_INFO_ALL:
        this->MsgRecv_Knights_MemberInfoAll(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_GRADE_CHANGE_ALL:
        this->MsgRecv_Knights_GradeChangeAll(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_DESTROY: // �ǰ��� Send - | Recv - b1(1:���� 0:����)
    {
        BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        std::string szMsg;
        switch ((e_SubPacket_KNights_Common)bSubCom) {
        case N3_SP_KNIGHTS_COMMON_SUCCESS: //Ŭ���ı� ����
            ::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
            m_pUIKnightsOp->KnightsInfoDelete(s_pPlayer->m_InfoExt.iKnightsID);
            this->MsgOutput(szMsg, 0xffffff00);
            break;
        case N3_SP_KNIGHTS_COMMON_DBFAIL:         //DB�˻� ����..
        case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER: //���� ����..
            break;
        }
        s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;
        s_pPlayer->KnightsInfoSet(0, "", 0, 0);
        m_pUIVar->UpdateKnightsInfo();
    } break;
    case N3_SP_KNIGHTS_DUTY_CHANGE: // ���� ����..
        MsgRecv_Knights_Duty_Change(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_JOIN_REQ:
        MsgRecv_Knigts_Join_Req(pDataPack, iOffset);
        break;

        /*    case N3_SP_KNIGHTS_APPOINT_CHIEF: //���� �Ӹ� - �����㰡�� ����
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_CHIEF_SUCCESS, szMsg); // ����
        }
        break;
    case N3_SP_KNIGHTS_DESTROY: // �ǰ��� Send - | Recv - b1(1:���� 0:����)
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_DESTROY_SUCCESS, szMsg); // ����

            s_pPlayer->m_InfoExt.iKnightsID = 0;
            s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;

            // ���ܿ��� ����..
            m_pUIVar->m_pPageKnights->UpdateKnightsName("");
            m_pUIVar->m_pPageKnights->UpdateKnightsChiefName("");
            m_pUIVar->m_pPageKnights->UpdateKnightsDuty(KNIGHTS_DUTY_UNKNOWN);

            m_pUIKnightsOp->MsgSend_KnightsList(0); // �ٽ� ���� ����Ʈ�� ��û�Ѵ�..
        }
        break;

    case N3_SP_KNIGHTS_MEMBER_JOIN_ADMIT: //��� ���� �㰡 Send - s1(Knights ID) | Recv - b1(1:���� 0:����)
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_ADMIT_SUCCESS, szMsg); // ����
        }
        break;
    case N3_SP_KNIGHTS_MEMBER_JOIN_REJECT: //��� ���� ���� - �����㰡�� ����
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_REJECT_SUCCESS, szMsg); // ����
        }
        break;
    case N3_SP_KNIGHTS_MEMBER_PUNISH: //��� ¡�� - �����㰡�� ����
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_PUNISH_SUCCESS, szMsg); // ����
        }
        break;
    case N3_SP_KNIGHTS_APPOINT_OFFICER: // �屳�Ӹ� - �����㰡�� ����
        {
            ::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_OFFICER_SUCCESS, szMsg); // ����
        }
        break;
    case N3_SP_KNIGHTS_LIST: //��� ����Ʈ ��û Send - | s1(Knights Count) Loop { s1(Knights ID) s1(Name Length) str1 (Name) }
        {
            if(false == m_pUIKnightsOp->IsVisible())
            {
                m_pUIKnightsOp->Open(s_pPlayer->m_InfoExt.eKnightsDuty); // UI ���� ����Ʈ�� �ʱ�ȭ..
            }
            else 
            {
                m_pUIKnightsOp->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty); // UI ���� ����Ʈ�� �ʱ�ȭ..
            }
            m_pUIKnightsOp->MsgRecv_KnightsList(pDataPack, iOffset);
        }
        break;
    case N3_SP_KNIGHTS_MEMBER_INFO_ALL: // ��ü ��� Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
        {
            m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
        }
        break;
    case N3_SP_KNIGHTS_MEMBER_INFO_ONLINE: //���� ���� ����Ʈ Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
        {
            m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
        }
        break;
    case N3_SP_KNIGHTS_STASH: //���� â��
        {
        }
        break;
    case N3_SP_KNIGHTS_DUTY_CHANGE: // ���� ����..
        {
            int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

            if(iID == s_pPlayer->IDNumber()) // �� ���� ����..
            {
                m_pUIVar->m_pPageKnights->UpdateKnightsDuty(eDuty);
            }
            else
            {
                CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
                if(pUPC) // �ٸ��� ���� ����..
                {
                }
            }
        }
        break;
    default:
        __ASSERT(0, "Invalid Knights SubPacket");
        break;
*/
    }
}

void CGameProcMain::MsgRecv_KnightsListBasic(DataPack * pDataPack, int & iOffset) // ���� �⺻ ���� �ޱ�..
{
    e_SubPacket_KnightsList eSP =
        (e_SubPacket_KnightsList)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset)); // Sub Packet
    switch (eSP) {
    case N3_SP_KNIGHTS_LIST_BASIC_ALL: // Receive - s1(knights Count) { s21(id, �̸�����), str1(�̸�) }
    {
        int iCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ���� ����
        for (int i = 0; i < iCount; i++) {
            std::string szID;
            int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);  // ���� ID
            int         iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ID ���ڿ� ����..
            CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);       // ID ���ڿ�..

            m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ���� ���� ��� �� �޴´�..
        }
    } break;
    case N3_SP_KNIGHTS_LIST_BASIC_INSERT: // Receive - s2(id, �̸�����), str1(�̸�)
    {
        std::string szID;
        int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);  // ���� ID
        int         iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ID ���ڿ� ����..
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);       // ID ���ڿ�..

        m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ���� ���� �߰�..
    } break;
    case N3_SP_KNIGHTS_LIST_BASIC_REMOVE: // Receive - s1(id)
    {
        int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ���� ID
        m_pUIKnightsOp->KnightsInfoDelete(iID);                            // ���� ���� �����..
    } break;
    }
}

void CGameProcMain::MsgRecv_CompressedPacket(DataPack * pDataPack,
                                             int &      iOffset) // ����� ������ �̴�... �ѹ� �� �Ľ��ؾ� �Ѵ�!!!
{
    short sCompLen, sOrgLen;
    DWORD dwCrcValue;
    sCompLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);   // ����� ����Ÿ���̾��...
    sOrgLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);    // ��������Ÿ���̾��...
    dwCrcValue = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // CRC�� ���...

    /// ���� ������ ��� �� ����
    CCompressMng Compressor;
    Compressor.PreUncompressWork((char *)(pDataPack->m_pData + iOffset), sCompLen, sOrgLen); // ���� Ǯ��...
    iOffset += sCompLen;

    if (Compressor.Extract() == false || Compressor.m_nErrorOccurred != 0 || dwCrcValue != Compressor.m_dwCrc) {
        return;
    }

    // ���� Ǯ�� ����Ÿ �б�
    BYTE * pDecodeBuf = (BYTE *)(Compressor.m_pOutputBuffer);

    // �ӽ÷� ������ �� �����..
    DataPack DataPackTemp;
    DataPackTemp.m_Size = sOrgLen;
    DataPackTemp.m_pData = pDecodeBuf;
    int iOffset2 = 0;
    this->ProcessPacket(&DataPackTemp, iOffset2); // �ٷ� �Ľ�...
    DataPackTemp.m_Size = 0;
    DataPackTemp.m_pData = NULL;
}

void CGameProcMain::MsgRecv_ContinousPacket(DataPack * pDataPack,
                                            int &      iOffset) // ����� ������ �̴�... �ѹ� �� �Ľ��ؾ� �Ѵ�!!!
{
    int iWholeSize = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ��������Ÿ���̾��...
    int iOffset2 = iOffset, iOffsetPrev = 0, iSizeThisPacket = 0;
    int i = 0;
    while (iOffset2 < iWholeSize) {
        iOffsetPrev = iOffset2;
        iSizeThisPacket = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset2); // ��������Ÿ���̾��...

        if (iSizeThisPacket <= 0 || iSizeThisPacket >= iWholeSize) {
            char szErr[256];
            sprintf(szErr, "���� ��Ŷ �ޱ� ���� - ����(%d) ũ��(%d/%d)", i, iSizeThisPacket, iWholeSize);
            CGameProcedure::ReportDebugStringAndSendToServer(szErr);
            __ASSERT(0, szErr);
            break; // �����!!
        }
        i++;

        this->ProcessPacket(pDataPack, iOffset2);
        iOffset2 = iOffsetPrev + iSizeThisPacket + 2;
    }

    iOffset += iWholeSize;
}

void CGameProcMain::MsgRecv_WareHouse(DataPack * pDataPack, int & iOffset) // ������ ���� ��Ŷ..
{
    BYTE bResult, bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch ((e_SubPacket_WareHouse)bSubCom) {
    case N3_SP_WARE_INN:
        m_pUIInn->SetVisible(true);
        break;

    case N3_SP_WARE_OPEN:
        MsgRecv_WareHouseOpen(pDataPack, iOffset);
        break;

    case N3_SP_WARE_GET_IN:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
        if (m_pUIWareHouseDlg) {
            m_pUIWareHouseDlg->ReceiveResultToWareMsg(bResult);
        }
        break;

    case N3_SP_WARE_GET_OUT:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
        if (m_pUIWareHouseDlg) {
            m_pUIWareHouseDlg->ReceiveResultFromWareMsg(bResult);
        }
        break;

    case N3_SP_WARE_WARE_MOVE:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
        if (m_pUIWareHouseDlg) {
            m_pUIWareHouseDlg->ReceiveResultWareToWareMsg(bResult);
        }
        break;

    case N3_SP_WARE_INV_MOVE:
        bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 : true, 0x00 : false..
        if (m_pUIWareHouseDlg) {
            m_pUIWareHouseDlg->ReceiveResultInvToInvMsg(bResult);
        }
        break;
    }
}

void CGameProcMain::MsgRecv_WareHouseOpen(DataPack * pDataPack, int & iOffset) // ������ ����..
{
    if (m_pUIWareHouseDlg->IsVisible()) {
        return;
    }

    int     iItemID, iItemDurability, iItemCount;
    int64_t iWareGold = CAPISocket::Parse_GetInt64(pDataPack->m_pData, iOffset);
    m_pUIWareHouseDlg->EnterWareHouseStateStart(iWareGold);

    for (int i = 0; i < MAX_ITEM_WARE_PAGE * MAX_ITEM_TRADE; i++) // ���� ������ŭ..
    {
        iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
        iItemDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        iItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIWareHouseDlg->AddItemInWare(iItemID, iItemDurability, iItemCount, i);
    }

    if (!m_pUIWareHouseDlg->IsVisible()) {
        m_pUIWareHouseDlg->SetVisible(true);
    }

    if (m_pUIInventory->IsVisible()) { // �κ��丮�� �ȿ��� ������..
        this->CommandToggleUIInventory();
    }

    if (m_pUISkillTreeDlg->IsVisible()) {
        m_pUISkillTreeDlg->Close();
    }

    m_pUIWareHouseDlg->EnterWareHouseStateEnd();
}

void CGameProcMain::PlayBGM_Town() {
    if (m_pSnd_Battle) {
        m_pSnd_Battle->Stop(3.0f);
    }
    if (NULL == m_pSnd_Town || m_pSnd_Town->IsPlaying()) {
        return;
    }
    m_pSnd_Town->SetMaxVolume(60);
    m_pSnd_Town->Play(NULL, 3.0f); // ���� ���� ����.. ������ ������ ���Ͱ� ������ �Ѵ�..
}

void CGameProcMain::PlayBGM_Battle() {
    if (m_pSnd_Town) {
        m_pSnd_Town->Stop(3.0f);
    }
    if (NULL == m_pSnd_Battle || m_pSnd_Battle->IsPlaying()) {
        return;
    }
    m_pSnd_Battle->SetMaxVolume(80);
    m_pSnd_Battle->Play(NULL, 3.0f); // ���� ���� ����.. ������ ������ ���Ͱ� ������ �Ѵ�..
}

void CGameProcMain::ReleaseSound() {
    if (ACT_WORLD && ACT_WORLD->GetSkyRef()) {
        ACT_WORLD->GetSkyRef()->ReleaseSound();
    }
    CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Town);
    CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Battle);
}

void CGameProcMain::MsgRecv_NpcChangeOpen(DataPack * pDataPack, int & iOffset) // Class Change�� �ʱ�ȭ..
{
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch (bSubCom) {
    case N3_SP_CLASS_CHANGE_PURE:
        m_pUINpcChange->Open();
        break;

    case N3_SP_CLASS_CHANGE_REQ:
        this->MsgRecv_ClassChange(pDataPack, iOffset);
        break;

    case N3_SP_CLASS_ALL_POINT:
        this->MsgRecv_AllPointInit(pDataPack, iOffset);
        break;

    case N3_SP_CLASS_SKILL_POINT:
        this->MsgRecv_SkillPointInit(pDataPack, iOffset);
        break;

    case N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY:
        this->MsgRecv_PointChangePriceQueryRequest(pDataPack, iOffset);
        break;
    }
}

void CGameProcMain::MsgRecv_AllPointInit(DataPack * pDataPack, int & iOffset) // All Point �ʱ�ȭ..
{
    BYTE  bType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    DWORD dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

    char        szBuf[256] = "";
    std::string szMsg;

    switch (bType) {
    case 0x00: // ���� ����..
        ::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
        sprintf(szBuf, szMsg.c_str(), dwGold);
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;

    case 0x01: // ����..
        s_pPlayer->m_InfoExt.iStrength = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);

        s_pPlayer->m_InfoExt.iStamina = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);

        s_pPlayer->m_InfoExt.iDexterity = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);

        s_pPlayer->m_InfoExt.iIntelligence = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence,
                                                   s_pPlayer->m_InfoExt.iIntelligence_Delta);

        s_pPlayer->m_InfoExt.iMagicAttak = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak,
                                                 s_pPlayer->m_InfoExt.iMagicAttak_Delta);

        s_pPlayer->m_InfoBase.iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        s_pPlayer->m_InfoExt.iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        s_pPlayer->m_InfoExt.iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        s_pPlayer->m_InfoExt.iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
        m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);

        m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
        m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

        m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
        m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);

        s_pPlayer->m_InfoExt.iBonusPointRemain =
            CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ���� ���ʽ� ����Ʈ..
        m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(
            s_pPlayer->m_InfoExt.iBonusPointRemain); // ���ʽ� ����Ʈ ������ �����Ѱ�??

        // �� ����.. �κ��丮�� �ٲٰ� ��ŷ�.. ���� �ŷ��ʹ� ��Ÿ��..
        s_pPlayer->m_InfoExt.iGold = dwGold;
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->GoldUpdate();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->GoldUpdate();
        }
        if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible()) {
            m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
        }

        break;

    case 0x02: // Already..
        ::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
        sprintf(szBuf, szMsg.c_str());
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;
    }
}

void CGameProcMain::MsgRecv_SkillPointInit(DataPack * pDataPack, int & iOffset) // Skill Point �ʱ�ȭ..
{
    BYTE        bType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    DWORD       dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    char        szBuf[256] = "";
    std::string szMsg;

    switch (bType) {
    case 0x00: // ���� ����..
        ::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
        sprintf(szBuf, szMsg.c_str(), dwGold);
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;

    case 0x01: // ����..
        m_pUISkillTreeDlg->m_iSkillInfo[0] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        for (int i = 1; i < 9; i++) {
            m_pUISkillTreeDlg->m_iSkillInfo[i] = 0;
        }
        m_pUISkillTreeDlg->InitIconUpdate();

        // �� ����.. �κ��丮�� �ٲٰ� ��ŷ�.. ���� �ŷ��ʹ� ��Ÿ��..
        s_pPlayer->m_InfoExt.iGold = dwGold;
        if (m_pUIInventory->IsVisible()) {
            m_pUIInventory->GoldUpdate();
        }
        if (m_pUITransactionDlg->IsVisible()) {
            m_pUITransactionDlg->GoldUpdate();
        }
        if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible()) {
            m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
        }

        if (m_pUIHotKeyDlg) {
            m_pUIHotKeyDlg->AllFactorClear();
        }
        break;

    case 0x02: // Already..
        ::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
        sprintf(szBuf, szMsg.c_str());
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;
    }
}

void CGameProcMain::MsgRecv_PointChangePriceQueryRequest(DataPack * pDataPack, int & iOffset) // ���ݿ� ���� ���� ��Ŷ..
{
    DWORD dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    m_pUINpcChange->ReceivePriceFromServer(dwGold);
}

void CGameProcMain::MsgRecv_NoahChange(DataPack * pDataPack, int & iOffset) // ��� ����..
{
    BYTE  bType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    DWORD dwGoldOffset = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
    DWORD dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

    char        szBuf[256] = "";
    std::string szMsg;

    switch (bType) {
    case N3_SP_NOAH_GET:
        ::_LoadStringFromResource(IDS_NOAH_CHANGE_GET, szMsg);
        sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xff6565ff);
        break;

    case N3_SP_NOAH_LOST:
        ::_LoadStringFromResource(IDS_NOAH_CHANGE_LOST, szMsg);
        sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;

    case N3_SP_NOAH_SPEND:
        ::_LoadStringFromResource(IDS_NOAH_CHANGE_LOST, szMsg);
        sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
        CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
        break;
    }

    // �� ����.. �κ��丮�� �ٲٰ� ��ŷ�.. ���� �ŷ��ʹ� ��Ÿ��..
    s_pPlayer->m_InfoExt.iGold = dwGold;
    if (m_pUIInventory->IsVisible()) {
        m_pUIInventory->GoldUpdate();
    }
    if (m_pUITransactionDlg->IsVisible()) {
        m_pUITransactionDlg->GoldUpdate();
    }
    if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible()) {
        m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
    }
}

void CGameProcMain::MsgRecv_WarpList(DataPack * pDataPack, int & iOffset) // ���� ����Ʈ - �� ü������ �� ���� �ִ�..
{
    m_pUIWarp->Reset();

    int iStrLen = 0;

    BYTE bySubOp = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    if (bySubOp == 1) {
        int iListCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        for (int i = 0; i < iListCount; i++) {
            __WarpInfo WI;

            WI.iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);                  // ���� ID
            iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);                 // �̸� ����
            CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, WI.szName, iStrLen);      // �̸�
            iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);                 // ���ǹ� ����
            CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, WI.szAgreement, iStrLen); // ���ǹ�
            WI.iZone = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);                // ����ȣ
            WI.iMaxUser = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);             // �ִ� ���� ī��Ʈ.
            WI.iGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);                // ��
            WI.vPos.x = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;     // ��ǥ
            WI.vPos.z = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;     //
            WI.vPos.y = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;     //

            m_pUIWarp->InfoAdd(WI);
        }

        m_pUIWarp->UpdateList();
        m_pUIWarp->SetVisible(true);
    } else if (bySubOp == 2) {
        BYTE bySubSubOp = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        switch (bySubSubOp) {
        case 1:
            std::string szMsg;
            ::_LoadStringFromResource(IDS_WARP_ARRIVED, szMsg); // 6606
            __WarpInfo WI = m_pUIWarp->m_CurrWI;
            if (WI.szName.empty()) {
                return;
            }

            char szMsgBuff[400]{};
            sprintf(szMsgBuff, szMsg.c_str(), WI.szName.c_str());
            MsgOutput(szMsgBuff, 0xffffff00);
            break;
        }
    }
}

/*
void CGameProcMain::MsgRecv_ServerCheckAndRequestConcurrentUserCount(DataPack* pDataPack, int& iOffset)    // ���� IP �� ��Ʈ�� �޾� �����ڸ� üũ�� ����..
{
    std::string szIP;
    int iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // IP..
    CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szIP, iStrLen);
    DWORD dwPort = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Port

    __WarpInfo WI;
    if(m_pUIWarp->InfoGetCur(WI) < 0) return;

    bool bNeedConnectSubSocket = (szIP != s_pSocket->GetCurrentIP() || dwPort != s_pSocket->GetCurrentPort()); // �����ؾ� �� IP �� ��Ʈ�� �Ȱ�����

    if(bNeedConnectSubSocket) // ���� �������� �����ؾ� �ϸ�..
    {
        int iErr = s_pSocketSub->Connect(s_hWndSubSocket, szIP.c_str(), dwPort); // ���� �������� �����ؼ�..
        if(iErr)
        {
            this->ReportServerConnectionFailed(WI.szName, iErr, false);
            return;
        }
    }

    // ������ üũ..
    int iOffsetSend = 0;
    BYTE byBuff[8];
    
    CAPISocket::MP_AddByte(byBuff, iOffsetSend, N3_SERVER_CONCURRENT_CONNECT);
    CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iZone);
    CAPISocket::MP_AddByte(byBuff, iOffsetSend, s_pPlayer->m_InfoBase.eNation); // ������ ������..

    if(bNeedConnectSubSocket) s_pSocketSub->Send(byBuff, iOffsetSend); // ���� �������� ������.
    else s_pSocket->Send(byBuff, iOffsetSend); // �� �������� ������..
}


void CGameProcMain::MsgRecv_ConcurrentUserCountAndSendServerCheck(DataPack* pDataPack, int& iOffset)            // �����ڸ� �ް� ������ �����ϰڴٴ� ��Ŷ�� ������.
{
    int iConcurrentUser = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // IP..
    if(s_pSocketSub->IsConnected()) s_pSocketSub->Disconnect();

    __WarpInfo WI;
    if(m_pUIWarp->InfoGetCur(WI) < 0) return;

    if(iConcurrentUser < WI.iMaxUser) // ���� ���Ѻ��� ������..
    {
        int iOffsetSend = 0;
        BYTE byBuff[8];
        
        CAPISocket::MP_AddByte(byBuff, iOffsetSend, N3_SERVER_CHECK);
        CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iID);

        s_pSocket->Send(byBuff, iOffsetSend);
    }
    else
    {
        std::string szMsg; ::_LoadStringFromResource(IDS_MSG_CONCURRENT_USER_OVERFLOW, szMsg); // ���� ���� ���� �ʰ�..
        this->MsgOutput(szMsg, 0xffff0000);
    }
}
*/

void CGameProcMain::MsgRecv_Knights_Create(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch ((e_SubPacket_KNights_Create)bSubCom) {
    case N3_SP_KNIGHTS_CREATE_SUCCESS: {
        int         sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        std::string szID;
        int         iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);  // ���� ID
        int         iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // ID ���ڿ� ����..
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);       // ID ���ڿ�..
        int   iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);      // ���
        int   iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);       // ����
        DWORD dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

        if (s_pPlayer->IDNumber() == sid) {
            m_pUIInn->Message(IDS_CLAN_MAKE_SUCCESS);

            // �� ����.. �κ��丮�� �ٲٰ� ��ŷ�.. ���� �ŷ��ʹ� ��Ÿ��..
            s_pPlayer->m_InfoExt.iGold = dwGold;
            if (m_pUIInventory->IsVisible()) {
                m_pUIInventory->GoldUpdate();
            }
            if (m_pUITransactionDlg->IsVisible()) {
                m_pUITransactionDlg->GoldUpdate();
            }
            if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible()) {
                m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
            }

            //����(Ŭ��)UI������Ʈ...�ض�...
            s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_CHIEF;
            s_pPlayer->KnightsInfoSet(iID, szID, iGrade, iRank);
            m_pUIVar->UpdateKnightsInfo();

            if (m_pUIVar->m_pPageKnights->IsVisible()) {
                m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
                m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
            }

            //m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // ���� ���� �߰�..
        } else {
            CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(sid, true);
            if (pUPC) {
                pUPC->KnightsInfoSet(iID, szID, iGrade, iRank);
            }
        }
    } break;
    case N3_SP_KNIGHTS_CREATE_FAIL_LOWLEVEL:
        m_pUIInn->Message(IDS_CLAN_DENY_LOWLEVEL);
        break;
    case N3_SP_KNIGHTS_CREATE_FAIL_DUPLICATEDNAME:
        m_pUICreateClanName->Open(IDS_CLAN_REINPUT_NAME);
        break;

    case N3_SP_KNIGHTS_CREATE_FAIL_LOWMONEY:
        m_pUIInn->Message(IDS_CLAN_DENY_LOWGOLD);
        break;
    case N3_SP_KNIGHTS_CREATE_FAIL_ALREADYJOINED:
        m_pUIInn->Message(IDS_CLAN_DENY_ALREADYJOINED);
        break;
    case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDDAY:
        m_pUIInn->Message(IDS_CLAN_DENY_INVALIDDAY);
        break;
    case N3_SP_KNIGHTS_CREATE_FAIL_DBFAIL:
    case N3_SP_KNIGHTS_CREATE_FAIL_UNKNOWN:
        m_pUIInn->Message(IDS_CLAN_DENY_UNKNOWN);
        break;
    case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDSERVER:
        m_pUIInn->Message(IDS_CLAN_DENY_INVALID_SERVER);
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_Withdraw(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string szMsg;
    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_DBFAIL:
        break;
    case N3_SP_KNIGHTS_COMMON_SUCCESS: {
        int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        if (s_pPlayer->IDNumber() == sid) {
            s_pPlayer->m_InfoExt.iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            s_pPlayer->m_InfoExt.eKnightsDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
            m_pUIVar->UpdateKnightsInfo();

            s_pPlayer->KnightsInfoSet(s_pPlayer->m_InfoExt.iKnightsID, "", 0, 0);
            ::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
            this->MsgOutput(szMsg, 0xffffff00);

            if (m_pUIVar->m_pPageKnights->IsVisible()) {
                m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
                m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
            }
        } else {
            int           iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
            e_KnightsDuty eKnightsDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

            CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(sid, true);
            if (pUPC) {
                //__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKnightsID);
                //if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);
                //else pUPC->KnightsNameSet("", 0xffff0000);
                pUPC->KnightsInfoSet(iKnightsID, "", 0, 0);
            }
        }
    } break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:
    case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:
    case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER:
    case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER:
    case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT:
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:
    case N3_SP_KNIGHTS_COMMON_FAIL_FULL:
    case N3_SP_KNIGHTS_COMMON_FAIL_ME:
    case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
        ::_LoadStringFromResource(IDS_CLAN_WITHDRAW_FAIL, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_Join(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string szMsg;
    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB�˻� ����..
        break;
    case N3_SP_KNIGHTS_COMMON_SUCCESS: //Ŭ������ ����
    {
        int           sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int           iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        int           iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� �̸� ����.
        std::string   szKnightsName;
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);
        int iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���
        int iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);  // ����

        if (s_pPlayer->IDNumber() == sid) {
            s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
            s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
            m_pUIVar->UpdateKnightsInfo();

            ::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
            this->MsgOutput(szMsg, 0xffffff00);

            if (m_pUIVar->m_pPageKnights->IsVisible()) {
                m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
                m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
            }
        } else {
            CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(sid, true);
            if (pUPC) {
                pUPC->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
            }
        }
    } break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER: //���� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER: //��������� �׾� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //��������� ������ �ٸ�..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //��������� �̹� �ٸ� Ŭ���̳� ���ܿ� ���ԵǾ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //������ ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN: //�������� �ʴ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_FULL: //�ο��� Ǯ..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ME:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_REJECT:
        ::_LoadStringFromResource(IDS_CLAN_JOIN_REJECT, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_Leave(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string szMsg;
    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB�˻� ����..
        break;
    case N3_SP_KNIGHTS_COMMON_SUCCESS: //Ŭ��Ż�� ����
    {
        int           sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int           iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        int           iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� �̸� ����.
        std::string   szKnightsName;
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);
        int iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���
        int iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);  // ����

        if (s_pPlayer->IDNumber() == sid) {
            s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
            s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
            m_pUIVar->UpdateKnightsInfo();

            ::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
            this->MsgOutput(szMsg, 0xffffff00);

            if (m_pUIVar->m_pPageKnights->IsVisible()) {
                m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
                m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
            }
        } else {
            CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(sid, true);
            if (pUPC) {
                pUPC->KnightsInfoSet(iID, "", 0, 0);
            }
        }
    } break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER: //���� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER: //��������� �׾� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //��������� ������ �ٸ�..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //��������� �̹� �ٸ� Ŭ���̳� ���ܿ� ���ԵǾ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //������ ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN: //�������� �ʴ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_FULL: //�ο��� Ǯ..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ME:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_AppointViceChief(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string szMsg;
    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB�˻� ����..
        break;
    case N3_SP_KNIGHTS_COMMON_SUCCESS: //Ŭ������ ����
    {
        int           iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        s_pPlayer->m_InfoExt.iKnightsID = iID;
        s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
        m_pUIVar->UpdateKnightsInfo();

        ::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);

        if (m_pUIVar->m_pPageKnights->IsVisible()) {
            m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
            m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
        }
    } break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER: //���� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER: //��������� �׾� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //��������� ������ �ٸ�..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //��������� �̹� �ٸ� Ŭ���̳� ���ܿ� ���ԵǾ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //������ ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN: //�������� �ʴ� ����..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_FULL: //�ο��� Ǯ..
        ::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_ME:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
        ::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
        this->MsgOutput(szMsg, 0xffffff00);
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_MemberInfoAll(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    std::string szMsg;
    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_SUCCESS:
        m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
        break;
    case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB�˻� ����..
    default:
        break;
    }
}

void CGameProcMain::MsgRecv_Knights_GradeChangeAll(DataPack * pDataPack, int & iOffset) {
    int iCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
    if (iCount <= 0) {
        return;
    }

    std::vector<int> iIDs(iCount, 0);
    std::vector<int> iGrades(iCount, 0);
    std::vector<int> iRanks(iCount, 0);

    for (int i = 0; i < iCount; i++) {
        iIDs[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        iGrades[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
        iRanks[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
    }

    it_UPC it = s_pOPMgr->m_UPCs.begin(), itEnd = s_pOPMgr->m_UPCs.end();
    for (; it != itEnd; it++) {
        CPlayerOther * pUPC = it->second;
        if (NULL == pUPC) {
            continue;
        }

        int iIDTmp = pUPC->m_InfoExt.iKnightsID;
        if (iIDTmp <= 0) {
            continue;
        }

        for (int i = 0; i < iCount; i++) {
            if (iIDs[i] == iIDTmp) {
                pUPC->KnightsInfoSet(iIDTmp, pUPC->m_InfoExt.szKnights, iGrades[i], iRanks[i]);
                break;
            }
        }
    }
}

void CGameProcMain::MsgRecv_Knights_Duty_Change(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_SUCCESS: {
        int           sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        int           iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

        if (s_pPlayer->IDNumber() == sid) {
            s_pPlayer->m_InfoExt.iKnightsID = iID;
            s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
            m_pUIVar->UpdateKnightsInfo();
            if (s_pPlayer->m_InfoExt.iKnightsID == 0) {
                s_pPlayer->KnightsInfoSet(0, "", 0, 0);
            }
            //std::string szName;
            //__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
            //if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);
            //else m_pUIVar->m_pPageKnights->UpdateKnightsName("");
        } else {
            CPlayerOther * pUPC = s_pOPMgr->UPCGetByID(sid, true);
            if (pUPC) {
                if (iID == 0) {
                    pUPC->KnightsInfoSet(0, "", 0, 0);
                }
            }
        }
    } break;
    }
}

void CGameProcMain::MsgRecv_Knigts_Join_Req(DataPack * pDataPack, int & iOffset) {
    BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

    switch ((e_SubPacket_KNights_Common)bSubCom) {
    case N3_SP_KNIGHTS_COMMON_SUCCESS: {
        m_iJoinReqClanRequierID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
        m_iJoinReqClan = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

        int         iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // �Ҽ� ���� �̸� ����.
        std::string szKnightsName;
        CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);

        //            std::string szName;
        //            __KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(m_iJoinReqClan);
        //            if(!pKIB)
        //            {
        //                MsgSend_KnightsJoinReq(false);
        //                break;
        //            }

        std::string szMsg;
        ::_LoadStringFromResource(IDS_CLAN_JOIN_REQ, szMsg);

        char szTmp[256];
        sprintf(szTmp, szMsg.c_str(), szKnightsName.c_str());

        CGameProcedure::s_pProcMain->MessageBoxPost(szTmp, "", MB_YESNO, BEHAVIOR_CLAN_JOIN);
    } break;
    }
}

int CGameProcMain::MsgRecv_VersionCheck(DataPack * pDataPack, int & iOffset) // virtual
{
    int iVersion = CGameProcedure::MsgRecv_VersionCheck(pDataPack, iOffset);
    this->MsgSend_CharacterSelect(); // virtual

    return iVersion;
}

bool CGameProcMain::MsgRecv_CharacterSelect(DataPack * pDataPack, int & iOffset) // virtual
{
    bool bSuccess = CGameProcedure::MsgRecv_CharacterSelect(pDataPack, iOffset);

    //���������� �׾ ���� ü���� �ϴ� ���� �ٽ� ���� �������ش�.
    if (s_pPlayer->IsDead()) {
        this->InitPlayerPosition(
            s_pPlayer->Position());           // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
        s_pPlayer->RegenerateCollisionMesh(); // �浹 �޽ø� �ٽ� �����..
        s_pPlayer->m_iSendRegeneration = 0;   // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
        s_pPlayer->m_fTimeAfterDeath = 0;     // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���

        //
        //���� & ȿ�� �ʱ�ȭ..
        if (m_pUIStateBarAndMiniMap) {
            m_pUIStateBarAndMiniMap->ClearMagic();
        }
        if (m_pMagicSkillMng) {
            m_pMagicSkillMng->ClearDurationalMagic();
        }
        if (CGameProcedure::s_pFX) {
            s_pFX->StopMine();
        }

        CLogWriter::Write("Receive Regeneration");

        if (s_pPlayer->Nation() == NATION_KARUS) {
            CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(),
                                                 -1);
        } else if (s_pPlayer->Nation() == NATION_ELMORAD) {
            CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(),
                                                 -1);
        }
    }

    this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // ���� �ٲ۴�..
    s_pProcMain->MsgSend_GameStart();                                     // ���ε��� ������ ���� ����..

    return bSuccess;
}

void CGameProcMain::MsgRecv_Corpse(DataPack * pDataPack, int & iOffset) {
    int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); //regen�ϰ��� �ϴ� ������ ���̵�

    if (s_pPlayer->IDNumber() != iID) {
        s_pOPMgr->CorpseAdd(iID);

        if (s_pPlayer->m_iIDTarget == iID) {
            s_pPlayer->m_iIDTarget = -1; //���� ���� regen�� ĳ���Ϳ� ��Ŀ���� ������ �ִٸ� �ʱ�ȭ ���ش�.
        }
    }
}

void CGameProcMain::MsgSend_PerTradeBBSReq(std::string szName, int iDestID) {
    if (!m_pUITransactionDlg->IsVisible()) //// ���ΰ� ������ �ŷ�.. // ��ŷ� ���� �ƴϸ�..
    {
        std::string szMsg;
        ::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
        MsgOutput(szName + szMsg, 0xffffff00);

        MsgSend_PerTradeReq(iDestID, false);

        if (m_pUINpcEvent->IsVisible()) {
            m_pUINpcEvent->Close();
        }
        m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq(szName);
    }
}

void CGameProcMain::MsgSend_CharacterSelect() // virtual
{
    CGameProcedure::MsgSend_CharacterSelect();

    m_pMagicSkillMng->ClearDurationalMagic();
    s_pFX->ClearAll();
}

void CGameProcMain::ControlViewVDegree(short sValue) {
    if (s_pEng->ViewPoint() == VP_THIRD_PERSON) {
        float fPitch = 0;
        if (sValue > 0) {
            fPitch = D3DXToRadian(45.0f) * 3;
        } else {
            fPitch = D3DXToRadian(-45.0f) * 3;
        }
        if (fPitch) {
            s_pEng->CameraPitchAdd(fPitch);
        }
    }
}
/*
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev)
{
    if(s_pUIMgr->m_bDoneSomething) return false;

    if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
    {
        if(m_pMagicSkillMng->m_dwRegionMagicState==1)
        {
            __Vector3 vPick;
            CPlayerNPC*     pTarget = NULL;
        
            int iID = -1;
            pTarget = s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &vPick); // ��濡 �򸰳ѵ� ��ŷ..
            this->TargetSelect(iID, false); // Ÿ���� ��´�..
            if(NULL == pTarget) // Ÿ���� ������..
            {
                s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &vPick); // ���� ��ġ�� �����Ѵ�..
                if(NULL == s_pPlayer->m_pObjectTarget) // Ÿ�ٵ� ������..
                {
                    ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vPick); // ������ ����..
                }
            }
            s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, vPick);
        }
    }

    return true;
}
*/
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    if (ptCur.x != ptPrev.x || ptCur.y != ptPrev.y) {
        if (m_pMagicSkillMng->m_dwRegionMagicState == 1) {
            __Vector3 vNormal, vMyPos, vGap, vDir;

            vMyPos = s_pPlayer->Position();
            vMyPos.y += s_pPlayer->Height() / 2;

            ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseSkillPos); // ������ ����..

            vDir = m_vMouseSkillPos - vMyPos;
            vGap = vDir;
            vDir.Normalize();

            bool bColShape =
                ACT_WORLD->CheckCollisionWithShape(vMyPos, vDir, vGap.Magnitude(), &m_vMouseSkillPos, &vNormal);

            if (!bColShape) // Ÿ�ٵ� ������..
            {
                int iID = -1;
                s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &m_vMouseSkillPos); // ��濡 �򸰳ѵ� ��ŷ..
                this->TargetSelect(iID, false);                           // Ÿ���� ��´�..
            } else {
                m_vMouseSkillPos.y = ACT_WORLD->GetHeightWithTerrain(m_vMouseSkillPos.x, m_vMouseSkillPos.z);
            }
            s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID,
                                m_vMouseSkillPos);
        }
    }

    return true;
}

// ���� ���� Ŭ��
bool CGameProcMain::OnMouseLDBtnPress(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    CPlayerNPC * pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

    if (pTarget && pTarget->m_InfoBase.iAuthority == AUTHORITY_MANAGER) {
        s_pPlayer->m_iIDTarget = -1;
        pTarget = NULL;
    }

    if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
        if (s_pPlayer->IsAttackableTarget(pTarget, false)) {
            this->CommandMove(MD_STOP, true);
            this->CommandEnableAttackContinous(true, pTarget); // �ڵ� ����
        } else if (pTarget && VP_THIRD_PERSON == s_pEng->ViewPoint()) {
            this->CommandMove(MD_FOWARD, true);
            s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
        }
    } else {
        s_pPlayer->m_bAttackContinous = false;
        CommandToggleAttackContinous();
    }
    return true;
}

// ���� Ŭ��
bool CGameProcMain::OnMouseLBtnPress(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    POINT ptPlayer = ::_Convert3D_To_2DCoordinate(s_pPlayer->Position(), CN3Base::s_CameraData.mtxView,
                                                  CN3Base::s_CameraData.mtxProjection, CN3Base::s_CameraData.vp.Width,
                                                  CN3Base::s_CameraData.vp.Height);

    __Vector3  vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
    __Matrix44 mtxTmp;
    mtxTmp.RotationY(s_pEng->CameraYaw());
    vDir *= mtxTmp;
    vDir.Normalize();
    float fYaw = ::_Yaw2D(vDir.x, vDir.z);

    CPlayerNPC * pTarget = NULL;
    BOOL         bFindCorpse = false;

    int iID = -1;
    pTarget = s_pOPMgr->PickPrecisely(ptCur.x, ptCur.y, iID, &m_vMouseLBClickedPos); // ��濡 �򸰳ѵ� ��ŷ..
    this->TargetSelect(iID, false);                                                  // Ÿ���� ��´�..
    if (NULL == pTarget)                                                             // Ÿ���� ������..
    {
        if (s_pPlayer->m_bAttackContinous) {                 // ��� �����ϴ� ���̸�..
            this->CommandEnableAttackContinous(false, NULL); // ��� ���� ���..
        }

        s_pPlayer->m_pObjectTarget =
            ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &m_vMouseLBClickedPos); // ���� ��ġ�� �����Ѵ�..
        if (NULL == s_pPlayer->m_pObjectTarget)                                      // Ÿ�ٵ� ������..
        {
            // ��ü ������ ������ ���� ����..
            CPlayerNPC * pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ��ŷ..
            if (false == this->MsgSend_RequestItemBundleOpen(pCorpse))          // ��ü ������ ��û..
            {
                ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseLBClickedPos); // ������ ����..
            } else {
                bFindCorpse = true;
            }
        }
    }

    if (m_pMagicSkillMng->m_dwRegionMagicState == 1) {
        //        s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseLBClickedPos);
        s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID,
                            m_vMouseSkillPos);
        m_pMagicSkillMng->m_dwRegionMagicState = 2;
    }

    if (!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint()) {
        m_fLBClickTime = CN3Base::TimeGet();
        this->CommandSitDown(false, false); // �ϴ� ������ �����..

        if (pTarget) {
            if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL)) {
                if (s_pPlayer->IsAttackableTarget(pTarget, false)) {
                    if (s_pPlayer->m_bTargetOrPosMove) {
                        this->CommandMove(MD_STOP, true);
                    }

                    this->CommandEnableAttackContinous(true, pTarget); // �ڵ� ����
                } else {
                    this->CommandMove(MD_FOWARD, true);
                    s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
                }
            } else {
                if (s_pPlayer->m_bTargetOrPosMove && s_pPlayer->m_iMoveTarget != pTarget->IDNumber()) {
                    this->CommandMove(MD_STOP, true);
                }
            }
        } else if (!bFindCorpse) {
            s_pPlayer->RotateTo(fYaw, true);
            __Vector3 vMovePoint;
            ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ������ ����..

            float fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

            if (!s_pPlayer->m_bTargetOrPosMove && fDist > 1.5f) {
                this->CommandMove(MD_FOWARD, true);
            }

            if (fDist > 1.5f) {
                s_pPlayer->SetMoveTargetPos(vMovePoint);
            } else if (s_pPlayer->m_bTargetOrPosMove) {
                this->CommandMove(MD_STOP, true);
            }
        }
    } else if (!s_pPlayer->IsDead()) {
        if (pTarget && (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL))) {
            if (s_pPlayer->IsAttackableTarget(pTarget, false)) {
                if (s_pPlayer->m_bTargetOrPosMove) {
                    this->CommandMove(MD_STOP, true);
                }

                this->CommandEnableAttackContinous(true, pTarget); // �ڵ� ����
            } else {
                s_pPlayer->m_bAttackContinous = false;
                CommandToggleAttackContinous();
            }
        }
    }

    return true;
}

// ���� Ŭ����
bool CGameProcMain::OnMouseLBtnPressd(POINT ptCur, POINT ptPrev) {
    if (!s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) {
        //        if(!s_pPlayer->m_bTargetOrPosMove)
        //            this->CommandMove(MD_STOP, true);
    }
    return true;
}

// ���� ������ ������
bool CGameProcMain::OnMouseLbtnDown(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    POINT ptPlayer = ::_Convert3D_To_2DCoordinate(s_pPlayer->Position(), CN3Base::s_CameraData.mtxView,
                                                  CN3Base::s_CameraData.mtxProjection, CN3Base::s_CameraData.vp.Width,
                                                  CN3Base::s_CameraData.vp.Height);

    __Vector3  vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
    __Matrix44 mtxTmp;
    mtxTmp.RotationY(s_pEng->CameraYaw());
    vDir *= mtxTmp;
    vDir.Normalize();
    float fYaw = ::_Yaw2D(vDir.x, vDir.z);

    if (!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint() &&
        !s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) // ����Ī �����̸�.. UI �� �ǵ��� �ʾ�����..
    {
        __Vector3 vMovePoint;
        float     fDist;
        float     fNowTime = CN3Base::TimeGet();

        if (fNowTime - m_fLBClickTime > 0.1f && s_pPlayer->m_bTargetOrPosMove) {
            if (s_pPlayer->m_bAttackContinous) {
                CommandToggleAttackContinous();
            }

            this->CommandSitDown(false, false); // �ϴ� ������ �����..
            s_pPlayer->RotateTo(fYaw, true);
            this->CommandMove(MD_FOWARD, false);

            ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ������ ����..
            fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

            s_pPlayer->SetMoveTargetPos(vMovePoint);
        } else if (fNowTime - m_fLBClickTime > 0.1f && !s_pPlayer->m_bTargetOrPosMove) {
            if (s_pPlayer->m_bAttackContinous) {
                CommandToggleAttackContinous();
            }

            s_pPlayer->RotateTo(fYaw, true);
            ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // ������ ����..

            fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

            if (fDist > 1.5f) {
                this->CommandMove(MD_FOWARD, true);
                s_pPlayer->SetMoveTargetPos(vMovePoint);
            }
        }
    }
    return true;
}

// ������ Ŭ��
bool CGameProcMain::OnMouseRBtnPress(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    if (s_pLocalInput->IsKeyDown(DIK_LCONTROL) || s_pLocalInput->IsKeyDown(DIK_RCONTROL)) {
        m_pUIHotKeyDlg->EffectTriggerByMouse();
        return true;
    }

    int          iID = -1;
    CPlayerNPC * pNPC = s_pOPMgr->PickNPC(ptCur.x, ptCur.y, iID); // ��ŷ..

    if (NULL == pNPC) {
        CPlayerNPC * pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // ��ŷ..
        if (false == this->MsgSend_RequestItemBundleOpen(pCorpse))          // ��ü ������ ������ ���� ����..
        {
            CN3Shape * pShape = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true);
            if (pShape && pShape == s_pPlayer->m_pObjectTarget && pShape->m_iEventID) // Event �� ������..
            {
                float fD = (s_pPlayer->Position() - pShape->Pos()).Magnitude();
                float fDLimit = (s_pPlayer->Radius() + pShape->Radius()) * 2.0f;
                if (fD > fDLimit) // �Ÿ��� �ָ�
                {
                    std::string szMsg;
                    if (OBJECT_TYPE_BINDPOINT == pShape->m_iEventType) {
                        ::_LoadStringFromResource(IDS_BIND_POINT_REQUEST_FAIL, szMsg);
                    } else {
                        ::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
                    }
                    this->MsgOutput(szMsg, 0xffff8080);
                } else {
                    if (OBJECT_TYPE_BINDPOINT == pShape->m_iEventType) {
                        std::string szMsg;
                        ::_LoadStringFromResource(IDS_REQUEST_BINDPOINT, szMsg);
                        this->MessageBoxPost(szMsg, "", MB_YESNO,
                                             BEHAVIOR_REQUEST_BINDPOINT); // ������ ����Ʈ ���� �޽��� �ڽ�
                    } else if (OBJECT_TYPE_WARP_POINT == pShape->m_iEventType) {
                        this->MsgSend_ObjectEvent(pShape->m_iEventID, pShape->m_iNPC_ID); // ������Ʈ �̺�Ʈ ������..
                    }
                }
            }
        }
    } else if (pNPC->IDNumber() == s_pPlayer->m_iIDTarget) // Ÿ������ ���� ĳ���Ϳ� ��ŷ�� ĳ���Ͱ� ���� ..)
    {
        if (pNPC->m_pShapeExtraRef) // ������Ʈ ������ NPC �̸�.. ��Ʈ�� �� NPC�� ID �� ������..
        {
            if (pNPC->m_pShapeExtraRef->m_iNPC_ID > 0) {
                float fD = (s_pPlayer->Position() - pNPC->m_pShapeExtraRef->Pos()).Magnitude();
                float fDLimit = (s_pPlayer->Radius() + pNPC->m_pShapeExtraRef->Radius()) * 2.0f;
                if (fD > fDLimit) // �Ÿ��� �ָ�
                {
                    std::string szMsg;
                    ::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
                    this->MsgOutput(szMsg, 0xffff8080);
                } else {
                    this->MsgSend_ObjectEvent(pNPC->m_pShapeExtraRef->m_iEventID, pNPC->IDNumber());
                }
            }
        } else // ���� NPC �̸�..
        {
            if (pNPC->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) // ���� ���� �϶���..
            {
                float fD = (s_pPlayer->Position() - pNPC->Position()).Magnitude();
                float fDLimit = (s_pPlayer->Radius() + pNPC->Radius()) * 3.0f;
                if (fD > fDLimit) // �Ÿ��� �ָ�
                {
                    std::string szMsg;
                    ::_LoadStringFromResource(IDS_ERR_REQUEST_NPC_EVENT_SO_FAR, szMsg);
                    this->MsgOutput(szMsg, 0xffff8080);
                } else {
                    s_pPlayer->ActionMove(PSM_STOP); // �ϴ� ���߰�..
                    pNPC->RotateTo(s_pPlayer);       // ������ �÷��̾� ������ ������.
                    this->MsgSend_NPCEvent(iID);     // �̺�Ʈ ��û..
                    if (m_pUITransactionDlg) {
                        m_pUITransactionDlg->m_iNpcID = pNPC->IDNumber();
                    }
                }
            }
        }
    }
    return true;
}

// ������ Ŭ����
bool CGameProcMain::OnMouseRBtnPressd(POINT ptCur, POINT ptPrev) {
    return true;
}

// ������ ������ ������
bool CGameProcMain::OnMouseRbtnDown(POINT ptCur, POINT ptPrev) {
    float fMouseSensivity = 0.02f;

    float fRotY = D3DX_PI * ((ptCur.x - ptPrev.x) * fMouseSensivity); // ȸ���� ���� ����ϰ�..
    float fRotX = D3DX_PI * ((ptCur.y - ptPrev.y) * fMouseSensivity);
    if (fRotY && s_pPlayer->IsAlive()) {
        if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
            s_pEng->CameraYawAdd(fRotY);
        } else if (false == s_pPlayer->m_bStun) {
            s_pPlayer->RotAdd(fRotY); // ������ ���� ��������..
        }
    }
    if (fRotX) {
        if (VP_THIRD_PERSON == s_pEng->ViewPoint()) {
            s_pEng->CameraZoom(-fRotX); // ī�޶� Ȯ��
        } else {
            s_pEng->CameraPitchAdd(fRotX); // ī�޶� ����
        }
    }

    if (fRotY || fRotX) {
        SetGameCursor(NULL);
        POINT ptToScreen{ptPrev.x, ptPrev.y};
        ::ClientToScreen(s_hWndBase, &ptToScreen);
        ::SetCursorPos(ptToScreen.x, ptToScreen.y);
        s_pLocalInput->MouseSetPos(ptPrev.x, ptPrev.y);
    }
    return true;
}

// ������ ���� Ŭ��
bool CGameProcMain::OnMouseRDBtnPress(POINT ptCur, POINT ptPrev) {
    if (s_pUIMgr->m_bDoneSomething) {
        return false;
    }

    //��ų ������ ���Ǿ��ٸ�....
    m_pUIHotKeyDlg->EffectTriggerByMouse();

    return true;
}

void CGameProcMain::ProcessUIKeyInput(bool bEnable) {
    if (m_pUIChatDlg && !m_pUIChatDlg->IsChatMode()) {
        CGameProcedure::ProcessUIKeyInput();
        if (s_pLocalInput->IsKeyPress(DIK_RETURN) && !s_bKeyPress) {
            m_pUIChatDlg->SetFocus();
        }
    } else if (m_pUIChatDlg && m_pUIChatDlg->IsChatMode()) {
        s_bKeyPress = false;
        if (m_pUIChatDlg->GetEnableKillFocus()) {
            m_pUIChatDlg->SetEnableKillFocus(false);
            m_pUIChatDlg->KillFocus();
        }
    }
}

void CGameProcMain::MsgSend_SpeedCheck(bool bInit) {
    BYTE  byBuff[10];                 // ����..
    int   iOffset = 0;                // �ɼ�..
    float fTime = CN3Base::TimeGet(); // Ŭ���̾�Ʈ �ð�

    s_pSocket->MP_AddByte(byBuff, iOffset, N3_CHECK_SPEEDHACK); // ���ǵ��� üũ ��Ŷ..
    s_pSocket->MP_AddByte(byBuff, iOffset, bInit);  // ������ ���� �ð����� �� Ÿ�� true �̸� ���ؽð� false�� üũŸ��
    s_pSocket->MP_AddFloat(byBuff, iOffset, fTime); // Ŭ���̾�Ʈ �ð�
    s_pSocket->Send(byBuff, iOffset);               // ����..
}
