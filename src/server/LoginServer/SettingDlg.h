#pragma once

#include "ZipArchive.h"

#include <set>

typedef std::set<short> HistoryList;

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
class CLoginServerDlg;
class CSettingDlg : public CDialog {
    // Construction
  public:
    bool IsDBCSString(const std::string & szStr);
    void FolderRecurse(const fs::path & fsDir, bool bVerifyDbcs = false);
    bool InsertProcess(const fs::path & fsFile);
    bool Repacking(short sVersion);
    void RepackingHistory();
    CSettingDlg(short sVersion, CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CSettingDlg)
    enum {
        IDD = IDD_SETTING
    };
    CEdit         m_PathEdit;
    CListBox      m_FileList;
    CProgressCtrl m_Progress;
    UINT          m_nVersion;
    BOOL          m_bCompressOption;
    BOOL          m_bAllFileAdd;
    //}}AFX_DATA

    fs::path m_fsDefaultDir;

    CLoginServerDlg * m_pMain;
    CZipArchive       m_ZipArchive;
    HistoryList       m_RepackingVersionList;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSettingDlg)
  public:
    virtual BOOL DestroyWindow();

  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CSettingDlg)
    afx_msg void OnAddFile();
    afx_msg void OnDeleteFile();
    afx_msg void OnCompress();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnPathBrowse();
    afx_msg void OnRefresh();
    afx_msg void OnKillFocusVersionEdit();
    afx_msg void OnDbcstest();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
