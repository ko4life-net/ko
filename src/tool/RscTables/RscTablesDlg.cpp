// RscTablesDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "RscTables.h"
#include "RscTablesDlg.h"
#include "DlgDataCount.h"

#include <string>

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
// CRscTablesDlg dialog

CRscTablesDlg::CRscTablesDlg(CWnd * pParent /*=NULL*/)
    : CDialog(CRscTablesDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CRscTablesDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRscTablesDlg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRscTablesDlg)
    DDX_Control(pDX, IDC_LIST_TABLE_EXT, m_ListTableExt);
    DDX_Control(pDX, IDC_LIST_DATATYPE, m_ListBoxDataType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRscTablesDlg, CDialog)
//{{AFX_MSG_MAP(CRscTablesDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_COMMAND(ID_FILE_NEW, OnFileNew)
ON_COMMAND(ID_FILE_SAVE_ENUM, OnFileSaveEnum)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ENUM, OnUpdateFileSaveEnum)
ON_COMMAND(ID_FILE_OPEN_ENUM, OnFileOpenEnum)
ON_COMMAND(ID_FILE_EXIT, OnFileExit)
ON_COMMAND(ID_CONVERT_TEXT2BIN, OnConvertText2bin)
ON_WM_CLOSE()
ON_WM_SIZE()
ON_COMMAND(ID_EDIT_INSERT, OnEditInsert)
ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
ON_BN_CLICKED(IDC_B_OPEN_TABLE_BASIC, OnBOpenTableBasic)
ON_BN_CLICKED(IDC_B_OPEN_TABLE_EXT_ENUM, OnBOpenTableExtEnum)
ON_LBN_DBLCLK(IDC_LIST_TABLE_EXT, OnDblclkListTableExt)
ON_BN_CLICKED(IDC_B_GENERATE_SELECTED, OnBGenerateSelected)
ON_BN_CLICKED(IDC_B_GENERATE_ALL, OnBGenerateAll)
ON_COMMAND(ID_ITEM_FILE_LIST_LOAD, OnItemFileListLoad)
ON_COMMAND(ID_ITEM_FILE_LIST_SAVE, OnItemFileListSave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRscTablesDlg message handlers

BOOL CRscTablesDlg::OnInitDialog() {
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

    //
    CString szItemName;
    for (int i = 0; i < MAX_ITEM_EXTENSION; i++) {
        szItemName.LoadString(IDS_FMT_TABLE_EXT0 + i);
        m_ListTableExt.AddString(szItemName);
    }

    return TRUE; // return TRUE  unless you set the focus to a control
}

void CRscTablesDlg::OnSysCommand(UINT nID, LPARAM lParam) {
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

void CRscTablesDlg::OnPaint() {
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
HCURSOR CRscTablesDlg::OnQueryDragIcon() {
    return (HCURSOR)m_hIcon;
}

// 데이타 형식을 새로 만든다.
void CRscTablesDlg::OnFileNew() {
    // TODO: Add your command handler code here
    CDlgDataCount dlg;
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }
    int iCount = dlg.m_iCount;
    if (iCount < 0) {
        return;
    }
    m_Generator.Release();

    m_Generator.DataTypeAssign(iCount, DT_STRING); // 기본값은 문자열
    m_Generator.DataTypeSet(0, DT_DWORD);          // 첫번째 열은 정수..

    m_ListBoxDataType.SetDividerWidth(100);
    this->UpdateAllInfo(); // 리스트 박스 갱신하고.. 화면에 표시...
}

BOOL CRscTablesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT * pResult) {
    // TODO: Add your specialized code here and/or call the base class
    if ((void *)wParam == &m_ListBoxDataType) {
        CPropertyItem * pItem = (CPropertyItem *)lParam;
        int             iIndex = -1;
        sscanf(pItem->m_propName, "Data%d", &iIndex);
        if (iIndex < 0 || iIndex >= m_Generator.DataTypeCount()) {
            return TRUE;
        }
        if (0 == iIndex && lstrcmpi(pItem->m_curValue, "DWORD") != 0) // 0 번이 DWORD 가 아니면 경고한다..
        {
            MessageBox("Data0 는 인덱스로 쓰이기 때문에 반드시 DWORD 형이어야 합니다.", "경고");
        }

        DATA_TYPE DataType = DT_NONE;
        if (lstrcmpi(pItem->m_curValue, "char") == 0) {
            DataType = DT_CHAR;
        } else if (lstrcmpi(pItem->m_curValue, "BYTE") == 0) {
            DataType = DT_BYTE;
        } else if (lstrcmpi(pItem->m_curValue, "short") == 0) {
            DataType = DT_SHORT;
        } else if (lstrcmpi(pItem->m_curValue, "WORD") == 0) {
            DataType = DT_WORD;
        } else if (lstrcmpi(pItem->m_curValue, "int") == 0) {
            DataType = DT_INT;
        } else if (lstrcmpi(pItem->m_curValue, "DWORD") == 0) {
            DataType = DT_DWORD;
        } else if (lstrcmpi(pItem->m_curValue, "string") == 0) {
            DataType = DT_STRING;
        } else if (lstrcmpi(pItem->m_curValue, "float") == 0) {
            DataType = DT_FLOAT;
        } else if (lstrcmpi(pItem->m_curValue, "double") == 0) {
            DataType = DT_DOUBLE;
        }

        ASSERT(DataType != DT_NONE);
        m_Generator.DataTypeSet(iIndex, DataType);

        return TRUE;
    }

    return CDialog::OnNotify(wParam, lParam, pResult);
}

// data type 설정해 놓을 것을 저장
void CRscTablesDlg::OnFileSaveEnum() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, "enm", NULL, dwFlags, "Data Enum File(*.enm)|*.enm||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_Generator.DataTypeSave(dlg.GetPathName().GetString());
}

// data type 설정해 놓을 것을 열기
void CRscTablesDlg::OnFileOpenEnum() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, "enm", NULL, dwFlags, "Data Enum File(*.enm)|*.enm||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_Generator.DataTypeLoad(dlg.GetPathName().GetString());

    this->UpdateAllInfo(); // 리스트 박스 갱신하고.. 화면에 표시...
}

void CRscTablesDlg::OnUpdateFileSaveEnum(CCmdUI * pCmdUI) {
    pCmdUI->Enable(m_Generator.DataTypeCount() > 0);
}

void CRscTablesDlg::OnFileExit() {
    PostQuitMessage(0);
}

// Text정보를 bin 데이터로 변환하기
void CRscTablesDlg::OnConvertText2bin() {
    int iDataCount = m_Generator.DataTypeCount();
    if (iDataCount <= 0) {
        MessageBox("먼저 데이터 갯수와 형식을 지정해주어야 합니다.");
        return;
    }

    char        szBuff[10240] = "";
    CString     FileName;
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
    CFileDialog dlg(TRUE, "txt", NULL, dwFlags, "txt(탭으로 분리)(*.txt)|*.txt||", NULL);
    dlg.m_ofn.nMaxFile = 10240;
    dlg.m_ofn.lpstrFile = szBuff;
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    POSITION pos = dlg.GetStartPosition();
    while (pos != NULL) {
        m_Generator.Convert2Bin(dlg.GetNextPathName(pos).GetString());
    }
}

void CRscTablesDlg::OnOK() {}

void CRscTablesDlg::OnClose() {
    CDialog::OnClose();
}

void CRscTablesDlg::UpdateInfo() {
    // 화면에 표시
    int iDataCount = m_Generator.DataTypeCount();
    for (int i = 0; i < iDataCount; ++i) {
        int iSel = -1;

        switch (m_Generator.DataType(i)) {
        case DT_CHAR:
            iSel = 0;
            break;
        case DT_BYTE:
            iSel = 1;
            break;
        case DT_SHORT:
            iSel = 2;
            break;
        case DT_WORD:
            iSel = 3;
            break;
        case DT_INT:
            iSel = 4;
            break;
        case DT_DWORD:
            iSel = 5;
            break;
        case DT_STRING:
            iSel = 6;
            break;
        case DT_FLOAT:
            iSel = 7;
            break;
        case DT_DOUBLE:
            iSel = 8;
            break;

        case DT_NONE:
        default:
            ASSERT(0);
            break;
        }

        CPropertyItem * pItem = m_ListBoxDataType.GetPropItem(i);
        if (pItem) {
            pItem->m_crColor = iSel;
        }
    }
    m_ListBoxDataType.UpdateWindow();
}

void CRscTablesDlg::UpdateAllInfo() {
    m_ListBoxDataType.ResetContent();

    int iDataCount = m_Generator.DataTypeCount();
    for (int i = 0; i < iDataCount; ++i) {
        CString str;
        str.Format("Data%d", i);
        m_ListBoxDataType.AddPropItem(str, "", PIT_COMBO, "char|BYTE|short|WORD|int|DWORD|string|float|double|");
    }
    this->UpdateInfo();
}

void CRscTablesDlg::OnSize(UINT nType, int cx, int cy) {
    CDialog::OnSize(nType, cx, cy);

    if (m_ListBoxDataType.GetSafeHwnd() != NULL) {
        CRect rcOrg, rcClient;
        m_ListBoxDataType.GetWindowRect(rcOrg); // 그래프 컨트롤의 원래 화면 좌표를
        this->GetClientRect(rcClient);          //
        this->ClientToScreen(rcClient);         // 화면 좌표로 바꾸고..

        int cx2 = rcOrg.Width();
        int cy2 = rcClient.bottom - rcOrg.top - 5;

        m_ListBoxDataType.SetWindowPos(NULL, 0, 0, cx2, cy2, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void CRscTablesDlg::OnEditInsert() {
    // TODO: Add your command handler code here
    int iCurSel = m_ListBoxDataType.GetCurSel();
    if (m_Generator.DataTypeInsert(iCurSel, DT_STRING)) { // 기본적으로 string추가
        UpdateAllInfo();                                  // 리스트 박스 갱신하고.. 화면에 표시...
    }
}

void CRscTablesDlg::OnEditDelete() {
    // TODO: Add your command handler code here
    const int iCurSel = m_ListBoxDataType.GetCurSel();
    if (m_Generator.DataTypeDelete(iCurSel)) {
        UpdateAllInfo(); // 리스트 박스 갱신하고.. 화면에 표시...
    }
}

bool CRscTablesDlg::BrowseDataEnumAndTxt(int iIndex, BOOL bOpen, std::string * pszFN_Enm, std::string * pszFN_Txt) {
    if (pszFN_Enm) {
        *pszFN_Enm = "";
    }
    if (pszFN_Txt) {
        *pszFN_Txt = "";
    }

    if (pszFN_Enm) {
        CString     FileName;
        DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
        CFileDialog dlg(bOpen, "enm", NULL, dwFlags, "Data Enum File(*.enm)|*.enm||", NULL);
        if (dlg.DoModal() == IDCANCEL) {
            return false;
        }
        FileName = dlg.GetFileName();

        *pszFN_Enm = FileName;
    }

    if (pszFN_Txt) {
        CString     FileName2;
        DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
        CFileDialog dlg2(bOpen, "txt", NULL, dwFlags, "Tab 으로 분리된 Txt File(*.txt)|*.txt||", NULL);
        if (dlg2.DoModal() == IDCANCEL) {
            return false;
        }
        FileName2 = dlg2.GetFileName();
        *pszFN_Txt = FileName2;
    }

    if (bOpen) {
        if (iIndex == -2) {
            if (pszFN_Enm) {
                char szFName[256], szExt[256];
                ::_splitpath(pszFN_Enm->c_str(), NULL, NULL, szFName, szExt);
                std::string szFN = szFName;
                szFN += szExt;
                SetDlgItemText(IDC_E_TABLE_BASIC_ENM, szFN.c_str());
            }
            if (pszFN_Txt) {
                char szFName[256], szExt[256];
                ::_splitpath(pszFN_Txt->c_str(), NULL, NULL, szFName, szExt);
                std::string szFN = szFName;
                szFN += szExt;
                SetDlgItemText(IDC_E_TABLE_BASIC_TXT, szFN.c_str());
            }
        } else if (iIndex == -1) {
            if (pszFN_Enm) {
                char szFName[256], szExt[256];
                ::_splitpath(pszFN_Enm->c_str(), NULL, NULL, szFName, szExt);
                std::string szFN = szFName;
                szFN += szExt;
                SetDlgItemText(IDC_E_TABLE_EXT_ENM, szFN.c_str());
            }
        } else if (iIndex >= 0 && iIndex < MAX_ITEM_EXTENSION) {
            if (pszFN_Enm) {
                char szFName[256], szExt[256];
                ::_splitpath(pszFN_Enm->c_str(), NULL, NULL, szFName, szExt);
                std::string szFN = szFName;
                szFN += szExt;
                SetDlgItemText(IDC_E_TABLE_EXT_ENM, szFN.c_str());
            }
            if (pszFN_Txt) {
                char szFName[256], szExt[256];
                ::_splitpath(pszFN_Txt->c_str(), NULL, NULL, szFName, szExt);

                CString szFN;
                szFN.LoadString(IDS_FMT_TABLE_EXT0 + iIndex);
                szFN += szFName;
                szFN += szExt;

                m_ListTableExt.DeleteString(iIndex);
                m_ListTableExt.InsertString(iIndex, szFN);
            }
        }
    }

    return true;
}

void CRscTablesDlg::OnBOpenTableBasic() {
    std::string szFN_Enm, szFN_Txt;
    if (false == this->BrowseDataEnumAndTxt(-2, TRUE, &szFN_Enm, &szFN_Txt)) {
        return;
    }
    m_Generator.OpenSource(szFN_Enm, szFN_Txt);
}

void CRscTablesDlg::OnBOpenTableExtEnum() {
    std::string szFN_Enm;
    if (false == this->BrowseDataEnumAndTxt(-1, TRUE, &szFN_Enm, NULL)) {
        return;
    }
    m_Generator.OpenReference_Enum(szFN_Enm);
}

void CRscTablesDlg::OnDblclkListTableExt() {
    int iSel = m_ListTableExt.GetCurSel();
    if (iSel < 0 || iSel >= MAX_ITEM_EXTENSION) {
        return;
    }

    std::string szFN_Txt;
    if (false == this->BrowseDataEnumAndTxt(iSel, TRUE, NULL, &szFN_Txt)) {
        return;
    }
    m_Generator.OpenReference_Txt(iSel, szFN_Txt);
}

void CRscTablesDlg::OnBGenerateSelected() {
    int iSel = m_ListTableExt.GetCurSel();
    if (iSel < 0 || iSel >= MAX_ITEM_EXTENSION) {
        return;
    }

    std::string szFN_Enm, szFN_Txt;
    if (false == this->BrowseDataEnumAndTxt(-3, FALSE, &szFN_Enm, &szFN_Txt)) {
        return;
    }
    m_Generator.Generate(iSel, szFN_Enm, szFN_Txt);
}

void CRscTablesDlg::OnBGenerateAll() {
    std::string szFN_Enm, szFN_Txt;
    if (false == this->BrowseDataEnumAndTxt(-3, FALSE, &szFN_Enm, &szFN_Txt)) {
        return;
    }
    m_Generator.Generate(-1, szFN_Enm, szFN_Txt);
}

void CRscTablesDlg::UpdateGenerationInfo() {
    char    szFName[256] = "", szExt[256] = "";
    CString szFN;

    ::_splitpath(m_Generator.m_szEnmBasic.c_str(), NULL, NULL, szFName, szExt);
    szFN = szFName;
    szFN += szExt;
    SetDlgItemText(IDC_E_TABLE_BASIC_ENM, szFN);

    ::_splitpath(m_Generator.m_szTxtBasic.c_str(), NULL, NULL, szFName, szExt);
    szFN = szFName;
    szFN += szExt;
    SetDlgItemText(IDC_E_TABLE_BASIC_TXT, szFN);

    ::_splitpath(m_Generator.m_szEnmExt.c_str(), NULL, NULL, szFName, szExt);
    szFN = szFName;
    szFN += szExt;
    SetDlgItemText(IDC_E_TABLE_EXT_ENM, szFN);

    m_ListTableExt.ResetContent();
    for (int i = 0; i < MAX_ITEM_EXTENSION; i++) {
        ::_splitpath(m_Generator.m_szTxtExts[i].c_str(), NULL, NULL, szFName, szExt);

        szFN.LoadString(IDS_FMT_TABLE_EXT0 + i);
        szFN += szFName;
        szFN += szExt;

        m_ListTableExt.AddString(szFN);
    }
}

void CRscTablesDlg::OnItemFileListLoad() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(TRUE, "igl", NULL, dwFlags, "Item generation list(*.igl)|*.igl||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    char szBuffs[32][256];
    memset(szBuffs, 0, sizeof(szBuffs));

    FILE * pFile = fopen(dlg.GetPathName(), "r");
    if (NULL == pFile) {
        return;
    }
    for (int i = 0; i < MAX_ITEM_EXTENSION + 3; i++) {
        fgets(szBuffs[i], 256, pFile);
        int iLen = lstrlen(szBuffs[i]);
        if (iLen > 0) {
            szBuffs[i][iLen - 1] = NULL; // '\r'
        } else {
            szBuffs[i][0] = NULL;
        }
    }
    fclose(pFile);
    pFile = NULL;

    m_Generator.OpenSource(szBuffs[0], szBuffs[1]);
    m_Generator.OpenReference_Enum(szBuffs[2]);
    for (int i = 0; i < MAX_ITEM_EXTENSION + 3; i++) {
        m_Generator.OpenReference_Txt(i, szBuffs[i + 3]);
    }

    this->UpdateGenerationInfo();
}

void CRscTablesDlg::OnItemFileListSave() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, "igl", NULL, dwFlags, "Item generation list(*.igl)|*.igl||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    char szBuffs[32][256];
    memset(szBuffs, 0, sizeof(szBuffs));

    lstrcpy(szBuffs[0], m_Generator.m_szEnmBasic.c_str());
    lstrcpy(szBuffs[1], m_Generator.m_szTxtBasic.c_str());
    lstrcpy(szBuffs[2], m_Generator.m_szEnmExt.c_str());
    for (int i = 0; i < MAX_ITEM_EXTENSION + 3; i++) {
        lstrcpy(szBuffs[i + 3], m_Generator.m_szTxtExts[i].c_str());
    }

    FILE * pFile = fopen(dlg.GetPathName(), "w");
    if (NULL == pFile) {
        return;
    }
    for (int i = 0; i < MAX_ITEM_EXTENSION + 3; i++) {
        fputs(szBuffs[i], pFile);
        fputs("\n", pFile);
    }
    fclose(pFile);
    pFile = NULL;
}
