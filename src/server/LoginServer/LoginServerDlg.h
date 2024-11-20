// LoginServerDlg.h : header file
//

#pragma once

#include "Define.h"
#include "Resource.h"
#include "IOCPort.h"
#include "DBProcess.h"
#include "STLMap.h"
#include <vector>
#include <string>
/////////////////////////////////////////////////////////////////////////////
// CLoginServerDlg dialog

typedef CSTLMap<fs::path, _VERSION_INFO> VersionInfoList; // TODO: does the key need to be fs::path?
typedef std::vector<_SERVER_INFO *>      ServerInfoList;

class CLoginServerDlg : public CDialog {
    // Construction
  public:
    static CLoginServerDlg * GetInstance() { return s_pInstance; }

    CString ConnectionStringLogin() const;

    BOOL GetInfoFromIni();

    CLoginServerDlg(CWnd * pParent = NULL); // standard constructor

    static CIOCPort m_Iocport;

    // m_szFtpUrl / m_szFtpPath / patch0000.zip
    std::string m_szFtpUrl;  // The url for the FTP server to download patches
    std::string m_szFtpPath; // The directory path for the FTP

    fs::path m_fsDefaultDir;

    short m_nLastVersion;

    std::string m_szOdbcLogDsn, m_szOdbcLogUid, m_szOdbcLogPwd;

    std::string m_szVersionTable;

    VersionInfoList m_VersionList;
    ServerInfoList  m_ServerList;
    int             m_nServerCount;

    CDBProcess m_DBProcess;

    // Dialog Data
    //{{AFX_DATA(CLoginServerDlg)
    enum {
        IDD = IDD_LOGINSERVER_DIALOG
    };
    CListBox m_OutputList;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLoginServerDlg)
  public:
    virtual BOOL PreTranslateMessage(MSG * pMsg);
    virtual BOOL DestroyWindow();

  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    HICON m_hIcon;

    static CLoginServerDlg * s_pInstance;

    // Generated message map functions
    //{{AFX_MSG(CLoginServerDlg)
    virtual BOOL    OnInitDialog();
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void    OnVersionSetting();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
