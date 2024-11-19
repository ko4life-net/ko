#include "StdAfx.h"
#include "LoginServer.h"
#include "SettingDlg.h"
#include "LoginServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

CSettingDlg::CSettingDlg(short sVersion, CWnd * pParent /*=NULL*/)
    : CDialog(CSettingDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CSettingDlg)
    m_nVersion = sVersion;
    m_bRepackPrevVersions = FALSE;
    m_bAllFileAdd = FALSE;
    //}}AFX_DATA_INIT

    m_pMain = (CLoginServerDlg *)pParent;
}

void CSettingDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSettingDlg)
    DDX_Control(pDX, IDC_PATH_EDIT, m_PathEdit);
    DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
    DDX_Control(pDX, IDC_PROGRESS, m_Progress);
    DDX_Text(pDX, IDC_VERSION_EDIT, m_nVersion);
    DDX_Check(pDX, IDC_CHECK_REPACK_VERSIONS, m_bRepackPrevVersions);
    DDX_Check(pDX, IDC_CHECK_ADD_ALL_FILES, m_bAllFileAdd);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
//{{AFX_MSG_MAP(CSettingDlg)
ON_BN_CLICKED(IDC_ADDFILE, OnAddFile)
ON_BN_CLICKED(IDC_DELETEFILE, OnDeleteFile)
ON_BN_CLICKED(IDC_COMPRESS, OnCompress)
ON_BN_CLICKED(IDC_PATH_BROWSE, OnPathBrowse)
ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
ON_EN_KILLFOCUS(IDC_VERSION_EDIT, OnKillFocusVersionEdit)
ON_BN_CLICKED(IDC_DBCSTEST, OnDbcstest)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    m_PathEdit.SetWindowText(m_fsDefaultDir.string().c_str());

    m_Progress.SetRange(0, 100);
    m_Progress.SetPos(0);

    // Initialize the tooltip
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(GetDlgItem(IDC_DBCSTEST),
                      _T("Checks all added files for invalid double-byte (DBCS) characters."));
    m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_REPACK_VERSIONS), _T("Repacks previous patches to remove outdated files."));
    m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_ADD_ALL_FILES),
                      _T("Adds all files automatically relative to the base path."));
    m_ToolTip.Activate(TRUE);

    OnRefresh();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnOK() {
    CDialog::OnOK();
}

void CSettingDlg::OnAddFile() {
    UpdateData(TRUE);

    if (m_bAllFileAdd) {
        if (AfxMessageBox("All files of Base Path will be inserted.", MB_OKCANCEL) == IDCANCEL) {
            return;
        }
        BeginWaitCursor();
        FolderRecurse(m_fsDefaultDir);
        EndWaitCursor();
        return;
    }

    fs::current_path(m_fsDefaultDir);
    std::string szInitialDir = fs::path(m_fsDefaultDir).make_preferred().string();

    CFileDialog dlg(TRUE);
    dlg.m_ofn.lpstrInitialDir = szInitialDir.c_str();
    dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;

    std::vector<char> vFilesBuff(512000);
    dlg.m_ofn.lpstrFile = vFilesBuff.data();
    dlg.m_ofn.nMaxFile = static_cast<DWORD>(vFilesBuff.size());
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    POSITION pos = dlg.GetStartPosition();
    while (pos) {
        fs::path fsFile = fs::path(dlg.GetNextPathName(pos).GetString());
        fsFile.make_relative(m_fsDefaultDir, true).normalize();
        if (InsertProcess(fsFile)) {
            m_FileList.AddString(fsFile.string().c_str());
        }
    }
}

void CSettingDlg::OnDeleteFile() {
    int iSelCount = m_FileList.GetSelCount();
    if (iSelCount == 0) {
        AfxMessageBox("File Not Selected.");
        return;
    }

    BeginWaitCursor();

    std::vector<int> vSelFiles(iSelCount);
    m_FileList.GetSelItems(iSelCount, vSelFiles.data());

    for (int i = 0; i < iSelCount; i++) {
        CString szTmpStr;
        m_FileList.GetText(vSelFiles[i], szTmpStr);
        fs::path fsSelFile = szTmpStr.GetString();

        _VERSION_INFO * pInfo = m_pMain->m_VersionList.GetData(fsSelFile);
        if (pInfo) {
            if (!m_pMain->m_DBProcess.DeleteVersion(fsSelFile)) {
                AfxMessageBox(std::format("{:s} DB Delete Fail", fsSelFile.string()).c_str());
                return;
            }
            if (pInfo->sHistoryVersion > 0) { // Restore
                pInfo->sVersion = pInfo->sHistoryVersion;
                pInfo->sHistoryVersion = 0;
                fs::path fsPatchFileName = std::format("patch{:04d}.zip", pInfo->sVersion);
                if (!m_pMain->m_DBProcess.InsertVersion(pInfo->sVersion, fsSelFile, fsPatchFileName, 0)) {
                    m_pMain->m_VersionList.DeleteData(fsSelFile);
                    AfxMessageBox(std::format("{:s} DB Insert Fail", fsSelFile).c_str());
                    return;
                }
            } else {
                if (!m_pMain->m_VersionList.DeleteData(fsSelFile)) {
                    return;
                }
            }
            Sleep(10);
        }
    }

    EndWaitCursor();

    OnRefresh();
}

void CSettingDlg::OnCompress() {
    UpdateData(TRUE);

    int iFilesCount = m_FileList.GetCount();
    if (iFilesCount == 0) {
        return;
    }

    BeginWaitCursor();
    m_RepackingVersionList.clear();

    fs::path fsPatchFileName = m_fsDefaultDir / std::format("patch{:04d}.zip", m_nVersion);
    m_ZipArchive.Open(fsPatchFileName.string().c_str(), CZipArchive::create);

    SetDlgItemText(IDC_STATUS, "Compressing..");

    for (int i = 0; i < iFilesCount; i++) {
        CString szTmpStr;
        m_FileList.GetText(i, szTmpStr);
        fs::path fsCurFile = szTmpStr.GetString();

        fs::path fsCurFileAbs = m_fsDefaultDir / fsCurFile;

        std::string szCurFileAbs = fsCurFileAbs.string();
        std::string szDefaultDir = m_fsDefaultDir.string();

        CFile file;
        if (!file.Open(szCurFileAbs.c_str(), CFile::modeRead)) {
            AfxMessageBox(std::format("{:s} File Open Fail", fsCurFile).c_str());
            continue;
        }
        DWORD dwSize = file.GetLength();
        file.Close();

        if (!m_ZipArchive.AddNewFile(szCurFileAbs.c_str(), szDefaultDir.c_str(), -1, dwSize)) {
            AfxMessageBox(std::format("{:s} File Compress Fail", fsCurFile).c_str());
            continue;
        }
        m_Progress.SetPos(i * 100 / iFilesCount);

        _VERSION_INFO * pInfo = m_pMain->m_VersionList.GetData(fsCurFile);
        if (pInfo && pInfo->sHistoryVersion > 0) {
            m_RepackingVersionList.insert(pInfo->sHistoryVersion);
        }
    }
    SetDlgItemText(IDC_STATUS, "Compressed");

    m_ZipArchive.Close();

    if (m_bRepackPrevVersions) {
        RepackingHistory();
    }

    m_Progress.SetPos(100);

    EndWaitCursor();
}

void CSettingDlg::OnPathBrowse() {
    CFolderPickerDialog dlg;
    dlg.m_ofn.lpstrTitle = "Select a base path to where your patch content";
    if (dlg.DoModal() == IDOK) {
        m_fsDefaultDir = fs::path(dlg.GetPathName().GetString()).normalize();
        m_PathEdit.SetWindowText(m_fsDefaultDir.string().c_str());
    }
}

void CSettingDlg::OnRefresh() {
    UpdateData(TRUE);

    m_FileList.ResetContent();
    for (const auto & [_, pInfo] : m_pMain->m_VersionList.m_UserTypeMap) {
        if (pInfo->sVersion == m_nVersion) {
            m_FileList.AddString(pInfo->fsFile.string().c_str());
        }
    }
}

BOOL CSettingDlg::DestroyWindow() {
    m_RepackingVersionList.clear();
    return CDialog::DestroyWindow();
}

void CSettingDlg::RepackingHistory() {
    if (m_RepackingVersionList.empty()) {
        return;
    }

    SetDlgItemText(IDC_STATUS, "Repacking...");
    for (const auto & sVersion : m_RepackingVersionList) {
        if (!Repacking(sVersion)) {
            AfxMessageBox(std::format("{:d} Repacking Fail", sVersion).c_str());
        }
    }
    SetDlgItemText(IDC_STATUS, "Repacked");
}

bool CSettingDlg::Repacking(short sVersion) {
    fs::path fsCompFile = m_fsDefaultDir / std::format("patch{:04d}.zip", sVersion);

    std::string szCompFile = fsCompFile.string();
    m_ZipArchive.Open(szCompFile.c_str(), CZipArchive::create);

    for (const auto & [_, pInfo] : m_pMain->m_VersionList.m_UserTypeMap) {
        if (pInfo->sVersion == sVersion) {
            fs::path fsCurFile = m_fsDefaultDir / pInfo->fsFile;

            CFile       file;
            std::string szCurFile = fsCurFile.string();
            if (file.Open(szCurFile.c_str(), CFile::modeRead) == FALSE) {
                AfxMessageBox(std::format("{:s} File Open Fail", szCurFile).c_str());
                continue;
            }
            DWORD dwSize = file.GetLength();
            file.Close();

            std::string szDefaultDir = m_fsDefaultDir.string();
            if (!m_ZipArchive.AddNewFile(szCurFile.c_str(), szDefaultDir.c_str(), -1, dwSize)) {
                AfxMessageBox(std::format("{:s} File Compress Fail", szCurFile).c_str());
                return false;
            }
        }
    }

    m_ZipArchive.Close();

    return true;
}

bool CSettingDlg::InsertProcess(const fs::path & fsFile) {
    if (IsDBCSString(fsFile.string())) {
        AfxMessageBox(std::format("'{:s}' include DBCS character", fsFile).c_str());
        return false;
    }

    short           sHistoryVersion = 0;
    _VERSION_INFO * pInfo1 = m_pMain->m_VersionList.GetData(fsFile);
    if (pInfo1) {
        sHistoryVersion = pInfo1->sVersion;
        if (!m_pMain->m_VersionList.DeleteData(fsFile)) {
            return false;
        }
        if (!m_pMain->m_DBProcess.DeleteVersion(fsFile)) {
            AfxMessageBox(std::format("'{:s}' DB Delete Fail", fsFile).c_str());
            return false;
        }
    }

    _VERSION_INFO * pInfo2 = new _VERSION_INFO;
    pInfo2->sVersion = static_cast<short>(m_nVersion);
    pInfo2->fsFile = fsFile;
    pInfo2->fsPatchFileName = std::format("patch{:04d}.zip", pInfo2->sVersion);
    pInfo2->sHistoryVersion = sHistoryVersion;
    if (!m_pMain->m_VersionList.PutData(fsFile, pInfo2)) {
        delete pInfo2;
        return false;
    }

    if (!m_pMain->m_DBProcess.InsertVersion(m_nVersion, fsFile, pInfo2->fsPatchFileName, sHistoryVersion)) {
        m_pMain->m_VersionList.DeleteData(fsFile);
        AfxMessageBox(std::format("'{:s}' DB Insert Fail", fsFile).c_str());
        return false;
    }

    return true;
}

void CSettingDlg::OnKillFocusVersionEdit() {
    OnRefresh();
}

void CSettingDlg::FolderRecurse(const fs::path & fsDir, bool bVerifyDbcs /*= false*/) {
    for (const auto & fsEntry : fs::recursive_directory_iterator(fsDir)) {
        fs::path fsFile = fsEntry.path();

        // Skip directory names, since we anyway recursively iterating through them
        if (fsEntry.is_directory()) {
            continue;
        }

        fsFile.make_relative(m_fsDefaultDir, true).normalize();

        if (bVerifyDbcs) { // Perform simple DBCS verification
            if (IsDBCSString(fsFile.string().c_str())) {
                AfxMessageBox(std::format("'{:s}' includes DBCS character", fsFile).c_str());
            }
            continue;
        }

        if (InsertProcess(fsFile)) {
            m_FileList.AddString(fsFile.string().c_str());
        }
    }
}

// IsDBCSLeadByte is deprecated and should be avoided. Its behavior may vary with different locales.
bool CSettingDlg::IsDBCSString(const std::string & szStr) {
    return std::ranges::any_of(szStr, [](unsigned char ch) { return IsDBCSLeadByte(ch); });
}

void CSettingDlg::OnDbcstest() {
    FolderRecurse(m_fsDefaultDir, true);
    AfxMessageBox("Test Done..");
}

BOOL CSettingDlg::PreTranslateMessage(MSG * pMsg) {
    if (m_ToolTip.m_hWnd != NULL) {
        m_ToolTip.RelayEvent(pMsg);
    }
    return CDialog::PreTranslateMessage(pMsg);
}
