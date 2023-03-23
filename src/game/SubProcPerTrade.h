// SubProcPerTrade.h: interface for the CSubProcPerTrade class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum e_PerTradeState {                                      // ������ ���� �ŷ� ����..
    PER_TRADE_STATE_NONE = 0,                               // �ƹ��͵� �ƴ�..
    PER_TRADE_STATE_WAIT_FOR_REQ,                           // ���濡�� ��û�ϰ� ������ ��ٸ�..
    PER_TRADE_STATE_WAIT_FOR_MY_DECISION_AGREE_OR_DISAGREE, // ���濡�Լ� ��û�ް� �� ������ ��ٸ�..
    PER_TRADE_STATE_NORMAL,                                 // ������ ���� �ŷ� �������� ����..
    PER_TRADE_STATE_ADD_AND_WAIT_FROM_SERVER,               // �������� ���ϰ� ������ ���� ������ ��ٸ�..
    PER_TRARE_STATE_EDITTING,                               // ������ ���� �ŷ� �ݾ��̳� ������ ������..
    PER_TRADE_STATE_MY_TRADE_DECISION_DONE,                 // �� �ŷ� ��ư ���� ����..
};

enum e_PerTradeResultCode {           // ������ �ŷ��� �����Ǵ� �ڵ� ����..
    PER_TRADE_RESULT_MY_DISAGREE = 0, // �ŷ��� ��û���� ���� �ŷ� ��û�� ���..
    PER_TRADE_RESULT_OTHER_DISAGREE,  // �ŷ��� ��û���� ������ �ŷ� ��û�� ���..
    PER_TRADE_RESULT_MY_CANCEL,       // �ŷ��� ��û�� ���� �ŷ� ��û�� ���..
};

enum e_PerTradeProceedCode {       // ������ �ŷ��� ��ӵǴ� ���¸� ����..
    PER_TRADE_RESULT_MY_AGREE = 0, // �ŷ��� ��û���� ���� �ŷ� ��û�� ���..
    PER_TRADE_RESULT_OTHER_AGREE,  // �ŷ��� ��û���� ������ �ŷ� ��û�� ���..
};

enum e_PerTradeItemKindBackup {
    PER_TRADE_ITEM_MONEY = 0, // ���� ���� �ŷ�â���� ADD�� ���� ���̴�..
    PER_TRADE_ITEM_OTHER,     // ���� ���� �ŷ�â���� ADD�� ���� �������̴�..
};

const DWORD dwGold = 900000000; // ��...

class CUIManager;
class CUIPerTradeDlg;
class CUITradeEditDlg;

#include "GameBase.h"

class CSubProcPerTrade : public CGameBase {
  protected:
    int m_iOtherID;
    int m_iGoldOffsetBackup;

    std::string m_szMsg; // MessageBox key

  public:
    CUIPerTradeDlg *  m_pUIPerTradeDlg;
    CUITradeEditDlg * m_pUITradeEditDlg;

    e_PerTradeState          m_ePerTradeState;
    e_PerTradeItemKindBackup m_ePerTradeItemKindBackup;

  public:
    CSubProcPerTrade();
    virtual ~CSubProcPerTrade();

    void Release();

    void InitPerTradeDlg(CUIManager * pUIManager);

    void EnterWaitMsgFromServerStatePerTradeReq();                   // ���� ������ �ŷ��� Ÿ�ο��� ��û�� ����..
    void EnterWaitMsgFromServerStatePerTradeReq(std::string szName); // ���� ������ �ŷ��� Ÿ�ο��� ��û�� ����..
    void EnterWaitMyDecisionToPerTrade(int iOtherID);                // ���� Ÿ�ο��Լ� ������ �ŷ��� ��û ���� ����..
    void LeavePerTradeState(e_PerTradeResultCode ePTRC);             // ������ �ŷ� ���°� �����Ǵ� �ڵ�..
    void ProcessProceed(e_PerTradeProceedCode ePTPC);                // ������ �ŷ��� ��ӵǴ� ���¸� ����..

    void SecureCodeBegin(); // ��ȣ �ڵ�..

    void PerTradeCoreStart();
    void PerTradeCoreInvDisable();

    void RequestItemCountEdit();
    void ItemCountEditOK();
    void ItemCountEditCancel();

    void FinalizePerTrade();        // �� �״�� ���� ��ó��..
    void PerTradeCompleteSuccess(); // ���� �ŷ� ���� ����..
    void PerTradeCompleteCancel();  // ���� �ŷ� ���..

    void PerTradeMyDecision();    // ���� �ŷ��� ���� �ߴ�..
    void PerTradeOtherDecision(); // �ٸ� ����� �ŷ��� ���� �ߴ�..
    void ResetMyTradeButtonState();
    void SecureJobStuffByMyDecision();

    void ReceiveMsgPerTradeReq(int iOtherID);
    void ReceiveMsgPerTradeAgree(BYTE bResult);
    void ReceiveMsgPerTradeAdd(BYTE bResult);
    void ReceiveMsgPerTradeOtherAdd(int iItemID, int iCount, int iDurability);
    void ReceiveMsgPerTradeOtherDecide();
    void ReceiveMsgPerTradeDoneSuccessBegin(int64_t iTotalGold);
    void ReceiveMsgPerTradeDoneItemMove(BYTE bItemPos, int iItemID, int iCount, int iDurability);
    void ReceiveMsgPerTradeDoneSuccessEnd();
    void ReceiveMsgPerTradeDoneFail();
    void ReceiveMsgPerTradeCancel();

    // Item Count OK..
    void ItemCountOK();
    void ItemCountCancel();
};
