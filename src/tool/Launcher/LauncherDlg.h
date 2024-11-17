// LauncherDlg.h : header file
//

#pragma once

#include <string>
#include <vector>

#include <afxinet.h>

#include "MacProgressCtrl.h"
#include "Resource.h"
#include "Define.h"
#include "ZipArchive.h"

//#include "DlgTransparent.h"

const int MAX_DOWNLOAD_FILE = 64;
/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg dialog

class CLauncherDlg : public CDialog {
  public:
    struct __ServerInfo {
        std::string szIP;
        std::string szName;
        int         iUserCount;
    };

    std::vector<__ServerInfo> m_InfoServers;
    CZipArchive               m_zip;

    HINTERNET          m_hInetSession;
    HINTERNET          m_hFtpConnection;
    class CAPISocket * m_pSocket;

    int      m_nCurVersion;
    char     m_strServiceName[256];
    fs::path m_fsExeFileName;
    fs::path m_fsInstalledDir;

    int         m_nServerVersion;
    std::string m_szFtpUrl;
    std::string m_szFtpPath;

    int         m_nGetFileNum;
    std::string m_szGetFileNames[MAX_DOWNLOAD_FILE];
    int         m_nVersionNum[MAX_DOWNLOAD_FILE];

    HKEY m_hRegistryKey; // 레지스트리 키 핸들..

    // Construction
  public:
    void PacketSend_DownloadInfo();
    void PacketReceive_DownloadInfo(const BYTE * pBuf, int & iIndex);
    void PacketReceive_ServerList(const BYTE * pBuf, int & iIndex);
    void PacketReceive_Version(const BYTE * pBuf, int & iIndex);
    void PacketProcess(BYTE * pBuf, int size);

    void LoadStringFromResource(DWORD dwID, std::string & szString);
    void FTP_Close();
    BOOL IsFtpExistFile(const std::string & szFtpUrl, const std::string & szFileName);
    BOOL GetDownloadFile(const std::string & szFtpUrl, const std::string & szFileName);
    BOOL FTP_Open();
    void DownloadProcess();
    void StartGame();

    CLauncherDlg(CWnd * pParent = NULL); // standard constructor

    void PacketSend_VersionReq();

    bool ArchiveClose();
    bool ArchiveOpen(CString OpenFileName);
    bool ArchiveExtract(CString ExtractFolder);

    // Dialog Data
    //{{AFX_DATA(CLauncherDlg)
    enum {
        IDD = IDD_LAUNCHER_DIALOG
    };
    CStatic          m_Status;
    CMacProgressCtrl m_progress;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLauncherDlg)
  public:
    virtual BOOL DestroyWindow();

  protected:
    virtual void    DoDataExchange(CDataExchange * pDX); // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CLauncherDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
