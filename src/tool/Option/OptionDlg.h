// OptionDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog
struct __GameOption {
    int  iTexLOD_Chr;     // Texture Chr 0 = HIGH 1 = LOW (Its orginal in 1298)
    int  iTexLOD_Shape;   // Texture Shape 0 = HIGH 1 = LOW
    int  iTexLOD_Terrain; // Texture Terrain 0 = HIGH 1 = LOW
    int  iUseShadow;      // On Shadow 1/Off Shadow 0
    int  iViewDist;       // Visibility range 
    int  iViewWidth;      // Width of desktop 
    int  iViewHeight;     // Height of desktop
    int  iViewColorDepth; // 16/32 BIT.
    int  iEffectSndDist;  // Sound Distance
    bool bSndDuplicated;  // Duplicated Sound
    bool bSoundEnable;    // Enabled/Disabled Sound
    bool bWindowCursor;   // Enbaled/Disabled Window Cursor button

    void InitDefault() {
        iUseShadow = true;
        iTexLOD_Chr = 0;
        iTexLOD_Shape = 0;
        iTexLOD_Terrain = 0;
        iViewColorDepth = 16;
        iViewWidth = 1024;
        iViewHeight = 768;
        iViewDist = 512;
        iEffectSndDist = 48;
        bSndDuplicated = false;
        bSoundEnable = true;
        bWindowCursor = true;
    }

    __GameOption() { InitDefault(); }
};
struct __ServerOption {
    int Version;
    void InitServerDefault() { 
        Version = 1298;
    }
    __ServerOption() { InitServerDefault(); }
};
class COptionDlg : public CDialog {
  protected:
    CString      m_szInstalledPath;
    CString      m_szExeName;
    __GameOption m_Option;
    __ServerOption m_ServerOption;

    // Construction
  public:
    void SettingUpdate();
    void SettingLoad(CString szIniFile);
    void SettingServerLoad(CString szIniFile);
    void SettingSave(CString szIniFile);
    void SettingServerSave(CString szIniFile);
    void VersionUpdate(CString szIniFile, int Version);
    COptionDlg(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(COptionDlg)
    enum {
        IDD = IDD_OPTION_DIALOG
    };
    CSliderCtrl m_SldEffectSoundDist;
    CComboBox   m_CB_ColorDepth;
    CComboBox   m_CB_Resolution;
    CSliderCtrl m_SldViewDist;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionDlg)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(COptionDlg)
    virtual BOOL    OnInitDialog();
    afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void    OnOK();
    afx_msg void    OnBApplyAndExecute();
    afx_msg void    OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
    afx_msg void    OnBVersion();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
