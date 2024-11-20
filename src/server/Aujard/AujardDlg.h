// AujardDlg.h : header file
//

#pragma once

#include "SharedMem.h"
#include "DBAgent.h"
#include "Define.h"
#include "STLMap.h"
#include "Resource.h"

typedef CSTLMap<_ITEM_TABLE> ItemtableArray;
/////////////////////////////////////////////////////////////////////////////
// CAujardDlg dialog

class CAujardDlg : public CDialog {
    // Construction
  public:
    static CAujardDlg * GetInstance() { return s_pInstance; }

    void         CouponEvent(char * pData);
    void         BattleEventResult(char * pData);
    void         WriteLogFile(char * pData);
    void         SaveUserData();
    void         WritePacketLog();
    void         UserKickOut(char * pBuf);
    void         SetLogInInfo(char * pBuf);
    void         KnightsList(char * pBuf);
    void         ConCurrentUserCount();
    void         AllKnightsMember(char * pBuf);
    void         DestroyKnights(char * pBuf);
    void         ModifyKnightsMember(char * pBuf, BYTE command);
    void         WithdrawKnights(char * pBuf);
    void         JoinKnights(char * pBuf);
    void         CreateKnights(char * pBuf);
    void         KnightsPacket(char * pBuf);
    _USER_DATA * GetUserPtr(const char * struserid, int & uid);
    void         UserDataSave(char * pBuf);
    void         AllSaveRoutine();
    void         AllCharInfoReq(char * pBuf);
    void         AccountLogIn(char * pBuf);
    void         DeleteChar(char * pBuf);
    void         CreateNewChar(char * pBuf);
    void         SelectNation(char * pBuf);
    BOOL         LoadItemTable();
    void         UserLogOut(char * pBuf);
    CAujardDlg(CWnd * pParent = NULL); // standard constructor

    BOOL InitializeMMF();
    void SelectCharacter(char * pBuf);

    CString CreateConnectionString(std::string_view szDsn, std::string_view szUid, std::string_view szPwd) const;
    CString ConnectionStringGame() const;
    CString ConnectionStringAccount() const;
    CString ConnectionStringLogin() const;

    CSharedMemQueue m_LoggerSendQueue;
    CSharedMemQueue m_LoggerRecvQueue;

    HANDLE m_hReadQueueThread;
    HANDLE m_hMMFile;
    char * m_lpMMFile;

    CDBAgent m_DBAgent;

    ItemtableArray m_ItemtableArray;

    int m_nServerNo, m_nZoneNo;

    std::string m_szOdbcGameDsn, m_szOdbcGameUid, m_szOdbcGamePwd;
    std::string m_szOdbcAccountDsn, m_szOdbcAccountUid, m_szOdbcAccountPwd;
    std::string m_szOdbcLogDsn, m_szOdbcLogUid, m_szOdbcLogPwd;

    CFile m_LogFile;

    int m_iPacketCount;     // packet의 수를 체크
    int m_iSendPacketCount; // packet의 수를 체크
    int m_iRecvPacketCount; // packet의 수를 체크
    int m_iLogFileDay;

    // Dialog Data
    //{{AFX_DATA(CAujardDlg)
    enum {
        IDD = IDD_AUJARD_DIALOG
    };
    CListBox m_OutputList;
    CString  m_DBProcessNum;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAujardDlg)
  public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG * pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    HICON m_hIcon;

    static CAujardDlg * s_pInstance;

    // Generated message map functions
    //{{AFX_MSG(CAujardDlg)
    virtual BOOL    OnInitDialog();
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void    OnOK();
    afx_msg void    OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
