// SettingDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "LoginServer.h"
#include "SettingDlg.h"
#include "DlgBrowsePath.h"
#include "LoginServerDlg.h"

#pragma warning(disable : 4786)
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

CSettingDlg::CSettingDlg(int version, CWnd * pParent /*=NULL*/)
    : CDialog(CSettingDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CSettingDlg)
    m_nVersion = version;
    m_bCompressOption = FALSE;
    m_bAllFileAdd = FALSE;
    //}}AFX_DATA_INIT

    memset(m_strDefaultPath, 0x00, MAX_PATH);

    m_pMain = (CLoginServerDlg *)pParent;
}

void CSettingDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSettingDlg)
    DDX_Control(pDX, IDC_PATH_EDIT, m_PathEdit);
    DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
    DDX_Control(pDX, IDC_PROGRESS, m_Progress);
    DDX_Text(pDX, IDC_VERSION_EDIT, m_nVersion);
    DDX_Check(pDX, IDC_CHECK, m_bCompressOption);
    DDX_Check(pDX, IDC_CHECK2, m_bAllFileAdd);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
//{{AFX_MSG_MAP(CSettingDlg)
ON_BN_CLICKED(IDC_ADDFILE, OnAddfile)
ON_BN_CLICKED(IDC_DELETEFILE, OnDeletefile)
ON_BN_CLICKED(IDC_COMPRESS, OnCompress)
ON_BN_CLICKED(IDC_PATH_BROWSE, OnPathBrowse)
ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
ON_EN_KILLFOCUS(IDC_VERSION_EDIT, OnKillfocusVersionEdit)
ON_BN_CLICKED(IDC_DBCSTEST, OnDbcstest)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    m_PathEdit.SetWindowText(m_strDefaultPath);

    m_Progress.SetRange(0, 100);
    m_Progress.SetPos(0);

    OnRefresh();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnOK() {

    CDialog::OnOK();
}

void CSettingDlg::OnAddfile() {
    CFileDialog dlg(TRUE);
    std::string addfilename, addfullpath, defaultpath;
    char        tempstr1[_MAX_PATH];
    memset(tempstr1, 0x00, _MAX_PATH);
    char szFileName[512000];
    int  strsize = 0;

    UpdateData(TRUE);

    if (m_bAllFileAdd) {
        if (AfxMessageBox("All files of Base Path will be inserted.", MB_OKCANCEL) == IDCANCEL) {
            return;
        }
        BeginWaitCursor();
        FolderRecurse(m_strDefaultPath);
        EndWaitCursor();
        return;
    }

    ::SetCurrentDirectory(m_strDefaultPath);

    szFileName[0] = 0x00;
    dlg.m_ofn.lpstrInitialDir = m_strDefaultPath;
    dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    dlg.m_ofn.lpstrFile = (LPTSTR)szFileName;
    dlg.m_ofn.nMaxFile = 512000;

    if (dlg.DoModal() == IDOK) {
        POSITION pos = dlg.GetStartPosition();
        while (pos) {
            strcpy(tempstr1, dlg.GetNextPathName(pos));
            addfullpath = _strlwr(tempstr1);
            defaultpath = _strlwr(m_strDefaultPath);
            strsize = defaultpath.size();
            addfilename = addfullpath.substr(strsize);

            if (InsertProcess(&addfilename[0])) {
                m_FileList.AddString(&addfilename[0]);
            }
        }
    }
}

void CSettingDlg::OnDeletefile() {
    int              selcount = 0;
    CString          delfilename, compname, errmsg;
    std::string      filename;
    std::vector<int> sellist;
    _VERSION_INFO *  pInfo = NULL;

    selcount = m_FileList.GetSelCount();
    if (selcount == 0) {
        AfxMessageBox("File Not Selected.");
        return;
    }

    BeginWaitCursor();

    sellist.reserve(selcount);

    m_FileList.GetSelItems(selcount, &sellist[0]);

    for (int i = 0; i < selcount; i++) {
        m_FileList.GetText(sellist[i], delfilename);
        filename = delfilename;

        pInfo = m_pMain->m_VersionList.GetData(filename);
        if (pInfo) {
            if (m_pMain->m_DBProcess.DeleteVersion(filename.c_str()) == FALSE) {
                errmsg.Format("%s DB Delete Fail", filename.c_str());
                AfxMessageBox(errmsg);
                return;
            }
            if (pInfo->sHistoryVersion > 0) { // Restore
                pInfo->sVersion = pInfo->sHistoryVersion;
                pInfo->sHistoryVersion = 0;
                compname.Format("patch%.4d.zip", pInfo->sVersion);
                if (m_pMain->m_DBProcess.InsertVersion(pInfo->sVersion, filename.c_str(),
                                                       (const char *)(LPCTSTR)compname, 0) == FALSE) {
                    m_pMain->m_VersionList.DeleteData(filename);
                    errmsg.Format("%s DB Insert Fail", filename.c_str());
                    AfxMessageBox(errmsg);
                    return;
                }
            } else {
                if (m_pMain->m_VersionList.DeleteData(filename) == FALSE) {
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
    CString         fullpathname, filename, errmsg, compname, compfullpath;
    DWORD           dwsize;
    CFile           file;
    std::string     addfilename;
    _VERSION_INFO * pInfo = NULL;

    UpdateData(TRUE);

    int count = m_FileList.GetCount();
    if (count == 0) {
        return;
    }

    BeginWaitCursor();
    compname.Format("patch%.4d.zip", m_nVersion);
    compfullpath.Format("%s\\%s", m_strDefaultPath, compname);

    set<int>::iterator Iter;

    while (!m_RepackingVersionList.empty()) {
        Iter = m_RepackingVersionList.begin();
        m_RepackingVersionList.erase(Iter);
    }

    m_ZipArchive.Open(compfullpath, CZipArchive::create);

    SetDlgItemText(IDC_STATUS, "Compressing..");

    for (int i = 0; i < count; i++) {
        m_FileList.GetText(i, filename);

        fullpathname = m_strDefaultPath;
        fullpathname += filename;
        if (!file.Open(fullpathname, CFile::modeRead)) {
            errmsg.Format("%s File Open Fail", filename);
            AfxMessageBox(errmsg);
            continue;
        }
        dwsize = file.GetLength();
        file.Close();

        if (!m_ZipArchive.AddNewFile(fullpathname, m_strDefaultPath, -1, dwsize)) {
            errmsg.Format("%s File Compress Fail", filename);
            AfxMessageBox(errmsg);
            continue;
        }
        m_Progress.SetPos(i * 100 / count);

        addfilename = (const char *)(LPCTSTR)filename;
        pInfo = m_pMain->m_VersionList.GetData(addfilename);
        if (pInfo) {
            m_RepackingVersionList.insert(pInfo->sHistoryVersion);
        }
    }
    SetDlgItemText(IDC_STATUS, "Compressed");

    m_ZipArchive.Close();

    if (!m_bCompressOption) { // Current Version 만 압축
        if (!m_RepackingVersionList.empty()) {
            RepackingHistory();
        }
    }

    m_Progress.SetPos(100);

    EndWaitCursor();
}

void CSettingDlg::OnPathBrowse() {
    CDlgBrowsePath pathdlg;
    if (pathdlg.DoModal() == IDOK) {
        strcpy(m_strDefaultPath, (char *)(LPCTSTR)pathdlg.m_szPath);
        m_PathEdit.SetWindowText(m_strDefaultPath);
    }
}

void CSettingDlg::OnRefresh() {
    UpdateData(TRUE);

    m_FileList.ResetContent();

    _VERSION_INFO *                             pInfo = NULL;
    std::map<string, _VERSION_INFO *>::iterator Iter1, Iter2;
    Iter1 = m_pMain->m_VersionList.m_UserTypeMap.begin();
    Iter2 = m_pMain->m_VersionList.m_UserTypeMap.end();
    for (; Iter1 != Iter2; Iter1++) {
        pInfo = (*Iter1).second;
        if (pInfo->sVersion == m_nVersion) {
            m_FileList.AddString(pInfo->strFileName.c_str());
        }
    }
}

BOOL CSettingDlg::DestroyWindow() {
    set<int>::iterator Iter;

    while (!m_RepackingVersionList.empty()) {
        Iter = m_RepackingVersionList.begin();
        m_RepackingVersionList.erase(Iter);
    }

    return CDialog::DestroyWindow();
}

void CSettingDlg::RepackingHistory() {
    _VERSION_INFO *    pInfo = NULL;
    CString            errmsg;
    set<int>::iterator historyIter1, historyIter2;

    SetDlgItemText(IDC_STATUS, "Repacking...");

    historyIter1 = m_RepackingVersionList.begin();
    historyIter2 = m_RepackingVersionList.end();

    for (; historyIter1 != historyIter2; historyIter1++) {
        if (!Repacking(*historyIter1)) {
            errmsg.Format("%d Repacking Fail", *historyIter1);
            AfxMessageBox(errmsg);
        }
    }

    SetDlgItemText(IDC_STATUS, "Repacked");
}

bool CSettingDlg::Repacking(int version) {
    _VERSION_INFO * pInfo = NULL;
    std::string     addfilename;
    CString         filename, errmsg, compname, compfullpath;
    DWORD           dwsize;
    CFile           file;

    compname.Format("patch%.4d.zip", version);
    compfullpath.Format("%s\\%s", m_strDefaultPath, compname);

    m_ZipArchive.Open(compfullpath, CZipArchive::create);

    std::map<string, _VERSION_INFO *>::iterator Iter1, Iter2;
    Iter1 = m_pMain->m_VersionList.m_UserTypeMap.begin();
    Iter2 = m_pMain->m_VersionList.m_UserTypeMap.end();
    for (; Iter1 != Iter2; Iter1++) {
        pInfo = (*Iter1).second;
        if (pInfo->sVersion == version) {
            filename = m_strDefaultPath;
            filename += pInfo->strFileName.c_str();
            if (file.Open(filename, CFile::modeRead) == FALSE) {
                errmsg.Format("%s File Open Fail", filename);
                AfxMessageBox(errmsg);
                continue;
            }
            dwsize = file.GetLength();
            file.Close();

            if (!m_ZipArchive.AddNewFile(filename, m_strDefaultPath, -1, dwsize)) {
                errmsg.Format("%s File Compress Fail", addfilename.c_str());
                AfxMessageBox(errmsg);
                return false;
            }
        }
    }

    m_ZipArchive.Close();

    return true;
}

bool CSettingDlg::InsertProcess(const char * filename) {
    _VERSION_INFO *pInfo1 = NULL, *pInfo2 = NULL;
    CString        compname, errmsg;
    std::string    addfilename;
    int            historyversion = 0;

    if (IsDBCSString(filename)) {
        errmsg.Format("%s include DBCS character", filename);
        AfxMessageBox(errmsg);
        return false;
    }

    addfilename = filename;
    compname.Format("patch%.4d.zip", m_nVersion);

    pInfo1 = m_pMain->m_VersionList.GetData(addfilename);
    if (pInfo1) {
        historyversion = pInfo1->sVersion;
        if (m_pMain->m_VersionList.DeleteData(addfilename) == FALSE) {
            return false;
        }
        if (m_pMain->m_DBProcess.DeleteVersion(addfilename.c_str()) == FALSE) {
            errmsg.Format("%s DB Delete Fail", addfilename.c_str());
            AfxMessageBox(errmsg);
            return false;
        }
    }

    pInfo2 = new _VERSION_INFO;
    pInfo2->sVersion = m_nVersion;
    pInfo2->strFileName = addfilename;
    pInfo2->strCompName = compname;
    pInfo2->sHistoryVersion = historyversion;
    if (m_pMain->m_VersionList.PutData(addfilename, pInfo2) == FALSE) {
        delete pInfo2;
        return false;
    }

    if (m_pMain->m_DBProcess.InsertVersion(m_nVersion, addfilename.c_str(), (const char *)(LPCTSTR)compname,
                                           historyversion) == FALSE) {
        m_pMain->m_VersionList.DeleteData(addfilename);
        errmsg.Format("%s DB Insert Fail", addfilename.c_str());
        AfxMessageBox(errmsg);
        return false;
    }

    return true;
}

void CSettingDlg::OnKillfocusVersionEdit() {
    OnRefresh();
}

void CSettingDlg::FolderRecurse(const char * foldername, bool b_test) {
    CFileFind   ff;
    std::string addfilename, addfullpath, defaultpath;
    char        tempstr1[_MAX_PATH];
    memset(tempstr1, 0x00, _MAX_PATH);
    int strsize = 0;

    ::SetCurrentDirectory(foldername);

    BOOL bFind = ff.FindFile();
    while (bFind) {
        bFind = ff.FindNextFile();
        strcpy(tempstr1, ff.GetFilePath());

        if (ff.IsDots()) {
            continue;
        }
        if (ff.IsDirectory()) {
            FolderRecurse(tempstr1, b_test);
            continue;
        }

        if (b_test) { // 단순 검사만 수행...
            if (IsDBCSString(tempstr1)) {
                CString errmsg;
                errmsg.Format("%s include DBCS character", tempstr1);
                AfxMessageBox(errmsg);
            }
            continue;
        }

        addfullpath = _strlwr(tempstr1);
        defaultpath = _strlwr(m_strDefaultPath);
        strsize = defaultpath.size();
        addfilename = addfullpath.substr(strsize);

        if (InsertProcess(&addfilename[0])) {
            m_FileList.AddString(&addfilename[0]);
        }
    }
}

bool CSettingDlg::IsDBCSString(const char * string) {
    int total_count = strlen(string);

    for (int i = 0; i < total_count; i++) {
        if (IsDBCSLeadByte(string[i++])) {
            return true;
        }
    }

    return false;
}

void CSettingDlg::OnDbcstest() {
    FolderRecurse(m_strDefaultPath, true);
    AfxMessageBox("Test Done..");
}
