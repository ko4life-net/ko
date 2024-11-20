// OptionDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Option.h"
#include "OptionDlg.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog {
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum {
        IDD = IDD_ABOUTBOX
    };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
    : CDialog(CAboutDlg::IDD) {
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

COptionDlg::COptionDlg(CWnd * pParent /*=NULL*/)
    : CDialog(COptionDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(COptionDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_Option.InitDefault();             // Init Default [GAME OPTIONS]
    m_ServerOption.InitServerDefault(); // Init Default [Server.ini]
}

void COptionDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptionDlg)
    DDX_Control(pDX, IDC_SLD_EFFECT_COUNT, m_SldEffectCount);
    DDX_Control(pDX, IDC_SLD_SOUND_DISTANCE, m_SldEffectSoundDist);
    DDX_Control(pDX, IDC_CB_COLORDEPTH, m_CB_ColorDepth);
    DDX_Control(pDX, IDC_CB_RESOLUTION, m_CB_Resolution);
    DDX_Control(pDX, IDC_SLD_VIEWING_DISTANCE, m_SldViewDist);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
//{{AFX_MSG_MAP(COptionDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_B_APPLY_AND_EXECUTE, OnBApplyAndExecute)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_B_VERSION, OnBVersion)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu * pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // 각종 컨트롤 초기화..
    m_SldEffectCount.SetRange(1000, 2000);
    m_SldViewDist.SetRange(256, 512);
    m_SldEffectSoundDist.SetRange(20, 48);

    int iAdd = 0;
    iAdd = m_CB_Resolution.AddString("1024 X 768");
    m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(768, 1024));
    iAdd = m_CB_Resolution.AddString("1280 X 1024");
    m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(1024, 1280));
    iAdd = m_CB_Resolution.AddString("1600 X 1200");
    m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(1200, 1600));

    iAdd = m_CB_ColorDepth.AddString("16 Bit");
    m_CB_ColorDepth.SetItemData(iAdd, 16);
    iAdd = m_CB_ColorDepth.AddString("32 Bit");
    m_CB_ColorDepth.SetItemData(iAdd, 32);

    // Loading all from ini

    fs::path fsCurDir = fs::current_path();
    SettingLoad(fsCurDir / "Option.ini");
    SettingServerLoad(fsCurDir / "Server.ini");
    // Setting all from ini
    SettingUpdate();

    return TRUE; // return TRUE  unless you set the focus to a control
}

void COptionDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COptionDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int   cxIcon = GetSystemMetrics(SM_CXICON);
        int   cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COptionDlg::OnQueryDragIcon() {
    return (HCURSOR)m_hIcon;
}

void COptionDlg::OnOK() {
    fs::path fsCurDir = fs::current_path();
    SettingSave(fsCurDir / "Option.ini");
    SettingServerSave(fsCurDir / "Server.ini");
    MessageBox("Settings saved successfully");
    CDialog::OnOK();
}

void COptionDlg::OnBApplyAndExecute() {
    fs::path fsLauncherFile = fs::current_path() / "Launcher.exe";
    OnOK();
    ShellExecuteW(NULL, L"open", fsLauncherFile.c_str(), L"", L"", SW_SHOWNORMAL); // Open Launcher
}

void COptionDlg::SettingSave(const fs::path & fsIniFile) {
    CString szBuff;

    if (IsDlgButtonChecked(IDC_R_TEX_CHR_HIGH)) {
        m_Option.iTexLOD_Chr = 0;
    } else if (IsDlgButtonChecked(IDC_R_TEX_CHR_LOW)) {
        m_Option.iTexLOD_Chr = 1;
    } else {
        m_Option.iTexLOD_Chr = 1;
    }

    if (IsDlgButtonChecked(IDC_R_TEX_SHAPE_HIGH)) {
        m_Option.iTexLOD_Shape = 0;
    } else if (IsDlgButtonChecked(IDC_R_TEX_SHAPE_LOW)) {
        m_Option.iTexLOD_Shape = 1;
    } else {
        m_Option.iTexLOD_Shape = 0;
    }

    if (IsDlgButtonChecked(IDC_R_TEX_TERRAIN_HIGH)) {
        m_Option.iTexLOD_Terrain = 0;
    } else if (IsDlgButtonChecked(IDC_R_TEX_TERRAIN_LOW)) {
        m_Option.iTexLOD_Terrain = 1;
    } else {
        m_Option.iTexLOD_Terrain = 1;
    }

    if (IsDlgButtonChecked(IDC_C_SHADOW)) {
        m_Option.iUseShadow = 1;
    } else {
        m_Option.iUseShadow = 0;
    }

    m_Option.bWindowMode = IsDlgButtonChecked(IDC_C_WINDOW_MODE) ? true : false;
    m_Option.bWeaponEffect = IsDlgButtonChecked(IDC_C_WEAPON_EFFECT) ? true : false;

    int iSel = m_CB_Resolution.GetCurSel();

    m_Option.iViewWidth = 1024;
    m_Option.iViewHeight = 768;
    if (0 == iSel) {
        m_Option.iViewWidth = 1024;
        m_Option.iViewHeight = 768;
    } else if (1 == iSel) {
        m_Option.iViewWidth = 1280;
        m_Option.iViewHeight = 1024;
    } else if (2 == iSel) {
        m_Option.iViewWidth = 1600;
        m_Option.iViewHeight = 1200;
    }

    iSel = m_CB_ColorDepth.GetCurSel();
    if (CB_ERR != iSel) {
        m_Option.iViewColorDepth = m_CB_ColorDepth.GetItemData(iSel);
        if (m_Option.iViewColorDepth != 16 && m_Option.iViewColorDepth != 32) {
            m_Option.iViewColorDepth = 16;
        }
    } else {
        m_Option.iViewColorDepth = 16;
    }

    m_Option.iEffectCount = m_SldEffectCount.GetPos();
    if (m_Option.iEffectCount < 1000) {
        m_Option.iEffectCount = 1000;
    } else if (m_Option.iEffectCount > 2000) {
        m_Option.iEffectCount = 2000;
    }

    m_Option.iViewDist = m_SldViewDist.GetPos();
    if (m_Option.iViewDist < 256) {
        m_Option.iViewDist = 256;
    } else if (m_Option.iViewDist > 512) {
        m_Option.iViewDist = 512;
    }

    m_Option.iEffectSndDist = m_SldEffectSoundDist.GetPos();
    if (m_Option.iEffectSndDist < 20) {
        m_Option.iEffectSndDist = 20;
    } else if (m_Option.iEffectSndDist > 48) {
        m_Option.iEffectSndDist = 48;
    }

    m_Option.bSndEffect = IsDlgButtonChecked(IDC_C_SOUND_EFFECT) ? true : false;
    m_Option.bSndDuplicated = IsDlgButtonChecked(IDC_C_SOUND_DUPLICATE) ? true : false;
    m_Option.bWindowCursor = IsDlgButtonChecked(IDC_C_CURSOR_WINDOW) ? true : false;

    CIni ini(fsIniFile, false);
    ini.SetInt("Texture", "LOD_Chr", m_Option.iTexLOD_Chr);
    ini.SetInt("Texture", "LOD_Shape", m_Option.iTexLOD_Shape);
    ini.SetInt("Texture", "LOD_Terrain", m_Option.iTexLOD_Terrain);
    ini.SetInt("Shadow", "Use", m_Option.iUseShadow);
    ini.SetInt("ViewPort", "Width", m_Option.iViewWidth);
    ini.SetInt("ViewPort", "Height", m_Option.iViewHeight);
    ini.SetInt("ViewPort", "ColorDepth", m_Option.iViewColorDepth);
    ini.SetInt("ViewPort", "Distance", m_Option.iViewDist);
    ini.SetInt("Effect", "Count", m_Option.iEffectCount);
    ini.SetInt("Sound", "Distance", m_Option.iEffectSndDist);
    ini.SetBool("Sound", "Bgm", m_Option.bSndBackground);
    ini.SetBool("Sound", "Effect", m_Option.bSndEffect);
    ini.SetBool("Sound", "Duplicate", m_Option.bSndDuplicated);
    ini.SetBool("Cursor", "WindowCursor", m_Option.bWindowCursor);
    ini.SetBool("Screen", "WindowMode", m_Option.bWindowMode);
    ini.SetBool("WeaponEffect", "EffectVisible", m_Option.bWeaponEffect);
}

void COptionDlg::SettingLoad(const fs::path & fsIniFile) {
    CIni ini(fsIniFile, false);

    m_Option.iTexLOD_Chr = ini.GetInt("Texture", "LOD_Chr", 0);
    m_Option.iTexLOD_Shape = ini.GetInt("Texture", "LOD_Shape", 0);
    m_Option.iTexLOD_Terrain = ini.GetInt("Texture", "LOD_Terrain", 0);
    m_Option.iUseShadow = ini.GetInt("Shadow", "Use", 1);
    m_Option.iViewWidth = ini.GetInt("ViewPort", "Width", 1024);
    m_Option.iViewHeight = ini.GetInt("ViewPort", "Height", 768);
    m_Option.iViewColorDepth = ini.GetInt("ViewPort", "ColorDepth", 16);
    m_Option.iViewDist = ini.GetInt("ViewPort", "Distance", 512);
    m_Option.iEffectSndDist = ini.GetInt("Sound", "Distance", 48);
    m_Option.iEffectCount = ini.GetInt("Effect", "Count", 2000);

    m_Option.bSndBackground = ini.GetBool("Sound", "Bgm", true);
    m_Option.bSndEffect = ini.GetBool("Sound", "Effect", true);
    m_Option.bSndDuplicated = ini.GetBool("Sound", "Duplicate", false);
    m_Option.bWindowCursor = ini.GetBool("Cursor", "WindowCursor", true);
    m_Option.bWindowMode = ini.GetBool("Screen", "WindowMode", false);
    m_Option.bWeaponEffect = ini.GetBool("WeaponEffect", "EffectVisible", true);
}

void COptionDlg::SettingServerLoad(const fs::path & fsIniFile) {
    CIni ini(fsIniFile, false);
    m_ServerOption.Version = ini.GetInt("Version", "Files", m_ServerOption.Version);
}

void COptionDlg::SettingServerSave(const fs::path & fsIniFile) {
    CIni ini(fsIniFile, false);
    ini.SetInt("Version", "Files", m_ServerOption.Version);
}

void COptionDlg::SettingUpdate() {
    if (m_Option.iTexLOD_Chr) {
        CheckRadioButton(IDC_R_TEX_CHR_HIGH, IDC_R_TEX_CHR_LOW, IDC_R_TEX_CHR_LOW);
    } else {
        CheckRadioButton(IDC_R_TEX_CHR_HIGH, IDC_R_TEX_CHR_LOW, IDC_R_TEX_CHR_HIGH);
    }

    if (m_Option.iTexLOD_Shape) {
        CheckRadioButton(IDC_R_TEX_SHAPE_HIGH, IDC_R_TEX_SHAPE_LOW, IDC_R_TEX_SHAPE_LOW);
    } else {
        CheckRadioButton(IDC_R_TEX_SHAPE_HIGH, IDC_R_TEX_SHAPE_LOW, IDC_R_TEX_SHAPE_HIGH);
    }

    if (m_Option.iTexLOD_Terrain) {
        CheckRadioButton(IDC_R_TEX_TERRAIN_HIGH, IDC_R_TEX_TERRAIN_LOW, IDC_R_TEX_TERRAIN_LOW);
    } else {
        CheckRadioButton(IDC_R_TEX_TERRAIN_HIGH, IDC_R_TEX_TERRAIN_LOW, IDC_R_TEX_TERRAIN_HIGH);
    }

    CheckDlgButton(IDC_C_SHADOW, m_Option.iUseShadow);

    SetDlgItemInt(IDC_E_VERSION, m_ServerOption.Version);

    int iSel = 0;
    if (1024 == m_Option.iViewWidth) {
        iSel = 0;
    } else if (1280 == m_Option.iViewWidth) {
        iSel = 1;
    } else if (1600 == m_Option.iViewWidth) {
        iSel = 2;
    }
    m_CB_Resolution.SetCurSel(iSel);

    if (16 == m_Option.iViewColorDepth) {
        iSel = 0;
    }
    if (32 == m_Option.iViewColorDepth) {
        iSel = 1;
    }
    m_CB_ColorDepth.SetCurSel(iSel);

    m_SldEffectCount.SetPos(m_Option.iEffectCount);
    m_SldEffectSoundDist.SetPos(m_Option.iEffectSndDist);
    m_SldViewDist.SetPos(m_Option.iViewDist);

    CheckDlgButton(IDC_C_BACKGROUND_MUSIC, m_Option.bSndBackground);
    CheckDlgButton(IDC_C_SOUND_EFFECT, m_Option.bSndEffect);
    CheckDlgButton(IDC_C_SOUND_DUPLICATE, m_Option.bSndDuplicated);
    CheckDlgButton(IDC_C_CURSOR_WINDOW, m_Option.bWindowCursor);
    CheckDlgButton(IDC_C_WINDOW_MODE, m_Option.bWindowMode);
    CheckDlgButton(IDC_C_WEAPON_EFFECT, m_Option.bWeaponEffect);
}

void COptionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar) {
    if ((void *)pScrollBar == (void *)(&m_SldViewDist)) {
        m_Option.iViewDist = m_SldViewDist.GetPos();
    } else if ((void *)pScrollBar == (void *)(&m_SldEffectSoundDist)) {
        m_Option.iEffectSndDist = m_SldEffectSoundDist.GetPos();
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COptionDlg::OnBVersion() {
    CString szMsg;
    szMsg.LoadString(IDS_CONFIRM_WRITE_REGISRY);
    if (IDNO == MessageBox(szMsg, "", MB_YESNO)) {
        return;
    }
    int iVersion = GetDlgItemInt(IDC_E_VERSION);
    VersionUpdate((fs::current_path() / "Server.ini").string(), iVersion);
}

void COptionDlg::VersionUpdate(const fs::path & fsIniFile, int Version) {
    CIni ini(fsIniFile, false);
    m_ServerOption.Version = Version;
    ini.SetInt("Version", "Files", Version);
}
