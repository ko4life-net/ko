// GameProcMain.h: interface for the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameProcedure.h"
#include <set>

typedef typename std::set<int>::iterator it_ID;
typedef std::pair<it_ID, bool>           pair_ID;

class CGameProcMain : public CGameProcedure {
    friend class CGameProcedure;

  protected:
    std::set<int> m_SetNPCID;
    std::set<int> m_SetUPCID;

  public:
#ifdef _N3_64GRID_
    CServerMesh * m_pSMesh; // �������� �ʿ��� �޽� Ŭ����..
#endif
    class CUIInventory *        m_pUIInventory;          // �κ��丮
    class CUIVarious *          m_pUIVar;                // ĳ���� ����â, ���� ���� ���� �������� �� �ٿ뵵 UI
    class CUIChat *             m_pUIChatDlg;            // ä�� ����� ��ȭ����..
    class CUIMessageWnd *       m_pUIMsgDlg;             // ���� �޽��� ��� ����.
    class CUIStateBar *         m_pUIStateBarAndMiniMap; // mp,hp,exp, minimap....
    class CUICmd *              m_pUICmd;                // ���� �ϴ��� ���ɹ�ư â..
    class CUICmdList *          m_pUICmdList;
    class CUICmdEdit *          m_pUICmdEditDlg;
    class CUITargetBar *        m_pUITargetBar; // Ÿ�� ����â..
    class CUITransactionDlg *   m_pUITransactionDlg;
    class CUIDroppedItemDlg *   m_pUIDroppedItemDlg;
    class CSubProcPerTrade *    m_pSubProcPerTrade;
    class CUIHelp *             m_pUIHelp;
    class CUIPartyOrForce *     m_pUIPartyOrForce;
    class CUISkillTreeDlg *     m_pUISkillTreeDlg;
    class CUIHotKeyDlg *        m_pUIHotKeyDlg;
    class CUINotice *           m_pUINotice;
    class CUIClassChange *      m_pUIClassChange;
    class CUINPCEvent *         m_pUINpcEvent;
    class CUIItemExchange *     m_pUIItemREDlg;
    class CUIRepairTooltipDlg * m_pUIRepairTooltip;
    class CUIWareHouseDlg *     m_pUIWareHouseDlg;
    class CUIInn *              m_pUIInn;
    class CUICreateClanName *   m_pUICreateClanName;

    class CUIKnightsOperation * m_pUIKnightsOp;        // ���� ����Ʈ ����, ����, ��...
    class CUIPartyBBS *         m_pUIPartyBBS;         // ��Ƽ ���� �Խ���.
    class CUITradeSellBBS *     m_pUITradeBBS;         // ��ŷ� �Խ���
    class CUIQuestMenu *        m_pUIQuestMenu;        // Quest Menu
    class CUIQuestTalk *        m_pUIQuestTalk;        // Quest Talk
    class CUILevelGuide *       m_pUILevelGuide;       // Level guide
    class CUIDead *             m_pUIDead;             // Dead UI
    class CUITradeBBSSelector * m_pUITradeBBSSelector; // ��ŷ� �Խ��� ���� ����
    class CUITradeBBSEditDlg *  m_pUITradeBBSEdit;     // ��ŷ� �Խù� ����
    class CUIRookieTip *        m_pUIRookieTip;        // RookieTip when closing notice ui
    class CUIExitMenu *         m_pUIExitMenu;         // Exit Menu

    class CN3Shape * m_pTargetSymbol; // �÷��̾ Ÿ������ ���� ĳ������ ��ġ���� �׸��� �ȴ�..

    class CN3SndObjStream *m_pSnd_Town, *m_pSnd_Battle; //��������, �������� ������..
    class CMagicSkillMng * m_pMagicSkillMng;

    class CUINpcTalk *        m_pUINpcTalk;
    class CUINPCChangeEvent * m_pUINpcChange;
    class CUIWarp *           m_pUIWarp;
    class CWarMessage *       m_pWarMessage; // ������� �޽���
    class CLightMgr *         m_pLightMgr;

    //..
    BOOL m_bLoadComplete; // �ε��� �Ϸ�Ǿ���??

    float     m_fMsgSendTimeMove; // �ֱ� �̵� �޽��� ���� �ð�..
    float     m_fMsgSendTimeRot;  // �ֱ� ȸ�� �޽��� ���� �ð�..
    float     m_fPlayerYawSended; // �ֱٿ� �޽����� ���� ������ �÷��̾� y �� ȸ����.
    float     m_fRequestGameSave; // �ֱٿ� ���� ������ ������ ��û�� �ð�
    __Vector3 m_vPlayerPosSended; // �ֱٿ� �޽����� ���� ������ �÷��̾� ��ġ.

    __Vector3 m_vMouseLBClickedPos;
    __Vector3 m_vMouseSkillPos;

    float m_fLBClickTime;

    int m_iJoinReqClan;
    int m_iJoinReqClanRequierID;

    int KM_COUNT;

    float m_fRotateValue;

    int            m_iExitCurCountDown;
    bool           m_bDoCancelExitRequest;
    e_GameExitType m_eExitType;
    float          m_fExitCurCountDownToReach;

  protected:
    bool ProcessPacket(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_CharacterSelect(DataPack * pDataPack, int & iOffset); // virtual
    int  MsgRecv_VersionCheck(DataPack * pDataPack, int & iOffset);    // virtual

    bool MsgRecv_MyInfo_All(DataPack * pDataPack, int & iOffset);
    void MsgRecv_MyInfo_HP(DataPack * pDataPack, int & iOffset);
    void MsgRecv_MyInfo_MSP(DataPack * pDataPack, int & iOffset);
    void MsgRecv_MyInfo_EXP(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_MyInfo_LevelChange(DataPack * pDataPack, int & iOffset);
    void MsgRecv_MyInfo_RealmPoint(DataPack * pDataPack, int & iOffset);
    void MsgRecv_MyInfo_PointChange(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_Chat(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_UserMove(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_Rotation(DataPack * pDataPack, int & iOffset);
    //    bool    MsgRecv_Dead(DataPack* pDataPack, int& iOffset);
    bool MsgRecv_Regen(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Corpse(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_Time(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_Weather(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_UserInAndRequest(
        DataPack * pDataPack,
        int &
            iOffset); // ���� ������ ��� User ���̵� ī��Ʈ��ŭ �޴´�... �۱�.. ������Ʈ�� �ʿ��� �͸� �������� ��û..
    bool MsgRecv_UserInRequested(DataPack * pDataPack,
                                 int &      iOffset); // �������� ��û�� ������ ���̵���� �޾Ƽ� User ó��..
    bool MsgRecv_UserInOut(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_UserIn(DataPack * pDataPack, int & iOffset, bool bWithFX = false);
    bool MsgRecv_UserOut(DataPack * pDataPack, int & iOffset);
    void MsgRecv_UserState(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_NPCInAndRequest(
        DataPack * pDataPack,
        int &
            iOffset); // ���� ������ ��� NPC ���̵� ī��Ʈ��ŭ �޴´�... �۱�.. ������Ʈ�� �ʿ��� �͸� �������� ��û..
    bool MsgRecv_NPCInRequested(DataPack * pDataPack,
                                int &      iOffset); // �������� ��û�� NPC ���̵���� �޾Ƽ� User ó��..
    bool MsgRecv_NPCInOut(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_NPCIn(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_NPCOut(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_NPCMove(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_Attack(DataPack * pDataPack, int & iOffset);
    bool MsgRecv_Dead(DataPack * pDataPack, int & iOffset);

    bool MsgRecv_ItemMove(DataPack * pDataPack, int & iOffset);             // Item Move�� ���� ����..
    bool MsgRecv_ItemBundleDrop(DataPack * pDataPack, int & iOffset);       // Item �� �ʵ忡 ��Ÿ���µ��� ���� ����
    bool MsgRecv_ItemBundleOpen(DataPack * pDataPack, int & iOffset);       // ������ ���ڸ� ���ų� ��ü�� ������..
    bool MsgRecv_ItemTradeStart(DataPack * pDataPack, int & iOffset);       // ������ ��ŷ�..
    bool MsgRecv_ItemTradeResult(DataPack * pDataPack, int & iOffset);      // ������ ��ŷ� ���..
    bool MsgRecv_ItemDroppedGetResult(DataPack * pDataPack, int & iOffset); // ���� ������ ������ �Ա� ���..
    bool MsgRecv_ItemWeightChange(DataPack * pDataPack, int & iOffset);     // ������ ���� ��ȭ..

    bool MsgRecv_UserLookChange(DataPack * pDataPack, int & iOffset); // �Ѹ���� �ٲ��.. ������ �����

    void MsgRecv_TargetHP(DataPack * pDataPack, int & iOffset);

    void MsgRecv_ZoneChange(DataPack * pDataPack, int & iOffset); // �� �ܼ��� �� ü����.

    void MsgRecv_Notice(DataPack * pDataPack, int & iOffset);
    void MsgRecv_PartyOrForce(DataPack * pDataPack, int & iOffset);
    void MsgRecv_PerTrade(DataPack * pDataPack, int & iOffset);

    void MsgRecv_SkillChange(DataPack * pDataPack, int & iOffset);  // ��ų ��ȭ..
    void MsgRecv_MagicProcess(DataPack * pDataPack, int & iOffset); // ��ų ��ȭ..
    void MsgRecv_ClassChange(DataPack * pDataPack, int & iOffset);  // ���� ��ȭ..
    void MsgRecv_ObjectEvent(DataPack * pDataPack, int & iOffset);  // ������Ʈ �̺�Ʈ ��û�� ���� ����

    void MsgRecv_DurabilityChange(DataPack * pDataPack, int & iOffset); // ������ ����..
    void MsgRecv_NpcEvent(DataPack * pDataPack, int & iOffset);         // Npc Event(Exchange, Repair both)..

    void MsgRecv_Knights(DataPack * pDataPack, int & iOffset);          // ���� ���� ���� ��Ŷ..
    void MsgRecv_KnightsListBasic(DataPack * pDataPack, int & iOffset); // ���� �⺻ ������..

    void MsgRecv_CompressedPacket(DataPack * pDataPack,
                                  int & iOffset); // ����� ��Ŷ�̴�... ���� Ǯ�� ������ ���鼭 �ѹ��� �Ľ��ؾ� �Ѵ�!!!
    void MsgRecv_ContinousPacket(DataPack * pDataPack,
                                 int &      iOffset); // �پ ���� ��Ŷ�̴�.. ������ ���鼭 �ѹ��� �Ľ��ؾ� �Ѵ�!!!

    void MsgRecv_ItemRepair(DataPack * pDataPack, int & iOffset);      // Item Repair Result..
    void MsgRecv_ItemCountChange(DataPack * pDataPack, int & iOffset); // Item Count Change..
    void MsgRecv_ItemDestroy(DataPack * pDataPack, int & iOffset);     // Item Count Change..

    void MsgRecv_WareHouse(DataPack * pDataPack, int & iOffset);     // ������ ���� ��Ŷ..
    void MsgRecv_WareHouseOpen(DataPack * pDataPack, int & iOffset); // ������ ����..

    void MsgRecv_NpcChangeOpen(DataPack * pDataPack, int & iOffset);                // Class Change�� �ʱ�ȭ..
    void MsgRecv_AllPointInit(DataPack * pDataPack, int & iOffset);                 // All Point �ʱ�ȭ..
    void MsgRecv_SkillPointInit(DataPack * pDataPack, int & iOffset);               // Skill Point �ʱ�ȭ..
    void MsgRecv_PointChangePriceQueryRequest(DataPack * pDataPack, int & iOffset); // ���ݿ� ���� ���� ��Ŷ..

    void MsgRecv_NoahChange(DataPack * pDataPack, int & iOffset); // ��� ����..
    void MsgRecv_WarpList(DataPack * pDataPack, int & iOffset);   // ���� ����Ʈ..
    //    void    MsgRecv_ServerCheckAndRequestConcurrentUserCount(DataPack* pDataPack, int& iOffset);            // ���� IP �� ��Ʈ�� �޾� �����ڸ� üũ�� ����..
    //    void    MsgRecv_ConcurrentUserCountAndSendServerCheck(DataPack* pDataPack, int& iOffset);

    //knights...
    void MsgRecv_Knights_Create(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_Withdraw(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_Join(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_Leave(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_AppointViceChief(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_MemberInfoAll(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_GradeChangeAll(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knights_Duty_Change(DataPack * pDataPack, int & iOffset);
    void MsgRecv_Knigts_Join_Req(DataPack * pDataPack, int & iOffset);

  public:
    void ProcessUIKeyInput(bool bEnable = true);
    bool OnMouseMove(POINT ptCur, POINT ptPrev);
    bool OnMouseLbtnDown(POINT ptCur, POINT ptPrev);
    bool OnMouseLBtnPressd(POINT ptCur, POINT ptPrev);
    bool OnMouseLBtnPress(POINT ptCur, POINT ptPrev);
    bool OnMouseLDBtnPress(POINT ptCur, POINT ptPrev);
    bool OnMouseRbtnDown(POINT ptCur, POINT ptPrev);
    bool OnMouseRBtnPressd(POINT ptCur, POINT ptPrev);
    bool OnMouseRBtnPress(POINT ptCur, POINT ptPrev);
    bool OnMouseRDBtnPress(POINT ptCur, POINT ptPrev);
    void ControlViewVDegree(short sValue);
    void PlayBGM_Town();
    void PlayBGM_Battle();

    void DoCommercialTransaction(int iTradeID); // ��ŷ�..

    const __InfoPartyOrForce * PartyOrForceConditionGet(bool & bIAmLeader, bool & bIAmMember, int & iMemberIndex,
                                                        class CPlayerBase *& pTarget);
    void                       TargetSelect(int iID, bool bMustAlive);
    void                       TargetSelect(class CPlayerNPC * pTarget);

    bool CommandToggleUIInventory();
    bool CommandToggleUIState();
    bool CommandToggleAttackContinous();
    bool CommandToggleMoveContinous();
    bool CommandToggleWalkRun();
    bool CommandToggleUISkillTree();
    bool CommandToggleCmdList();
    bool OpenCmdEdit(std::string msg);
    bool CommandToggleUIMiniMap();
    bool CommandToggleUINotice();
    bool CommandToggleUILevelGuide();

    void CommandMove(e_MoveDirection eMD, bool bStartOrEnd); // �����̴� ����(������, ����), �����̱� �����ϴ°�?
    void CommandEnableAttackContinous(bool bEnable, CPlayerBase * pTarget);
    void CommandCameraChange(); // ī�޶� ���� �ٲٱ�..
    void CommandExitMenu();
    void CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately = false);

    void CommandTargetSelect_NearstEnemy();    // ���� ����� �� Ÿ�� ���..
    void CommandTargetSelect_NearstOurForce(); // ���� ����� ��Ƽ Ÿ�����..

    void CloseUIs(); // ���� ��ŷ�, �������... UI �ݱ�..

    void MsgOutput(const std::string & szMsg, D3DCOLOR crMsg);

    void InitZone(int iZone, const __Vector3 & vPosPlayer);
    void InitUI();
    void InitPlayerPosition(const __Vector3 & vPos); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.

    void MsgSend_Continous(); // Ư�� ����(?)�Ͽ��� �������� ���������� �޽����� ����..
    void MsgSend_Attack(int iTargetID, float fInterval,
                        float fDistance); // ���� ��Ŷ ������ - ���̺��� ���� �ֱ⸦ ���� �༭ ��ŷ�� ���´�.
    void MsgSend_Move(bool bMove,
                      bool bContinous); // �������� ������ ��Ŷ�� ������.. // �����̴°� ? �ֱ������� �����̴� �ǰ�?
    void MsgSend_Rotation();            // �������� ȸ�� ��Ŷ�� ������..
    void MsgSend_Chat(enum e_ChatMode eMode, const std::string & szChat); // �������� ä�� �޽����� ������..
    void MsgSend_ChatSelectTarget(const std::string & szTargetID);        // �ϴ��� ä�� ��� ���ϱ�.
    void MsgSend_Regen();
    bool MsgSend_RequestItemBundleOpen(CPlayerNPC * pCorpse); // ������ ���ڸ� ���ų� ��ü�� ������..
    void MsgSend_RequestTargetHP(short siIDTarget,
                                 BYTE  byUpdateImmediately); // 0x00 - ���� �þ�Բ�.. 0x01 - ��� ������Ʈ..
    void MsgSend_GameStart();
    bool MsgSend_NPCEvent(short siIDTarget);
    void MsgSend_NPCInRequest(int iID);  // NPC ������ ���� ��� ��û�Ѵ�..
    void MsgSend_UserInRequest(int iID); // User ������ ���� ��� ��û�Ѵ�..
    void MsgSend_Warp();                 // ����?? - ��ü������ �ɼ��� �ִ�..
    void MsgSend_ZoneChangeComplete();   // �� ü���� �Ϸ�.. (�� �ε� ��..)
    void MsgSend_StateChange(enum e_SubPacket_State eSP, int iState);
    void MsgSend_PerTradeReq(int iDestID, bool bNear = true);
    void MsgSend_SpeedCheck(bool bInit = false);

    void MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo);              // iPartyOrForce 1 : Party, 2:Force
    void MsgSend_PartyOrForceLeave(int iPartyOrForce);                            // iPartyOrForce 1 : Party, 2:Force
    bool MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string & szID); // iPartyOrForce 1 : Party, 2:Force

    void MsgSend_ObjectEvent(int iEventID, int iNPCID); // ������Ʈ�� �����Ǿ� �ִ� �̺�Ʈ ��û..
    void MsgSend_Weather(int iWeather, int iPercent);
    void MsgSend_Time(int iHour, int iMin);
    void MsgSend_Administrator(enum e_SubPacket_Administrator eSP, const std::string & szID);

    void MsgSend_KnightsJoin(int iTargetID);
    void MsgSend_KnightsLeave(std::string & szName);
    void MsgSend_KnightsWithdraw();
    void MsgSend_KnightsAppointViceChief(std::string & szName);
    void MsgSend_KnightsJoinReq(bool bJoin);
    void MsgSend_PerTradeBBSReq(std::string szName, int iDestID);
    void MsgSend_CharacterSelect(); // virtual

    void ProcessPlayerInclination();            // ��� ó��..(������ �־ ��簡 ���ϸ� �̲�����..).
    void ProcessLocalInput(DWORD dwMouseFlags); // Ű���� �������� ó���Ѵ�..
    void ParseChattingCommand(const std::string & szCmd);

    void UpdateUI_PartyOrForceButtons(); // Ŀ�ǵ� �ٿ� �ִ� ��Ƽ ��ư�� ��Ȳ�� ���� ������Ʈ ���ش�.
    void UpdateUI_MiniMap();
    void UpdateUI_TargetBar();
    void UpdateBGM();
    void UpdateCameraAndLight();

    void RenderTarget();

    void Init();    // UI �� UI ���ҽ����� �д´�.
    void Release(); // Release..
    void ReleaseUIs();
    void ReleaseSound();

    void Tick();   // ����� ���..
    void Render(); // ������..

    CGameProcMain();          // ������.
    virtual ~CGameProcMain(); // �Ҹ���.
};
