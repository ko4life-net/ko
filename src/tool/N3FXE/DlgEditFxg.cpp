// DlgEditFxg.cpp : implementation file
//

#include "StdAfx.h"
#include "N3FXE.h"
#include "MainFrm.h"
#include "DlgEditFxg.h"
#include "N3Base/N3FXGroup.h"
#include "DlgNewFileName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditFxg dialog

CDlgEditFxg::CDlgEditFxg(CWnd * pParent /*=NULL*/)
    : CDialog(CDlgEditFxg::IDD, pParent) {
    //{{AFX_DATA_INIT(CDlgEditFxg)
    m_bLoop_0 = FALSE;
    m_bLoop_1 = FALSE;
    m_bLoop_2 = FALSE;
    m_bLoop_3 = FALSE;
    m_strFileName = _T("");
    m_strFXBName_0 = _T("");
    m_strFXBName_1 = _T("");
    m_strFXBName_2 = _T("");
    m_strFXBName_3 = _T("");
    //}}AFX_DATA_INIT

    m_pJoint[0] = &m_CBJoint_0;
    m_pJoint[1] = &m_CBJoint_1;
    m_pJoint[2] = &m_CBJoint_2;
    m_pJoint[3] = &m_CBJoint_3;

    m_pLoop[0] = &m_bLoop_0;
    m_pLoop[1] = &m_bLoop_1;
    m_pLoop[2] = &m_bLoop_2;
    m_pLoop[3] = &m_bLoop_3;

    m_pFXBName[0] = &m_strFXBName_0;
    m_pFXBName[1] = &m_strFXBName_1;
    m_pFXBName[2] = &m_strFXBName_2;
    m_pFXBName[3] = &m_strFXBName_3;
}

void CDlgEditFxg::DoDataExchange(CDataExchange * pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEditFxg)
    DDX_Control(pDX, IDC_CB_JOINT_3, m_CBJoint_3);
    DDX_Control(pDX, IDC_CB_JOINT_2, m_CBJoint_2);
    DDX_Control(pDX, IDC_CB_JOINT_1, m_CBJoint_1);
    DDX_Control(pDX, IDC_CB_JOINT_0, m_CBJoint_0);
    DDX_Check(pDX, IDC_CHK_LOOP_0, m_bLoop_0);
    DDX_Check(pDX, IDC_CHK_LOOP_1, m_bLoop_1);
    DDX_Check(pDX, IDC_CHK_LOOP_2, m_bLoop_2);
    DDX_Check(pDX, IDC_CHK_LOOP_3, m_bLoop_3);
    DDX_Text(pDX, IDC_EDT_FILENAME, m_strFileName);
    DDX_Text(pDX, IDC_EDT_FXB_NAME_0, m_strFXBName_0);
    DDX_Text(pDX, IDC_EDT_FXB_NAME_1, m_strFXBName_1);
    DDX_Text(pDX, IDC_EDT_FXB_NAME_2, m_strFXBName_2);
    DDX_Text(pDX, IDC_EDT_FXB_NAME_3, m_strFXBName_3);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgEditFxg, CDialog)
//{{AFX_MSG_MAP(CDlgEditFxg)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BTN_SAVE_AS, OnBtnSaveAs)
ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
ON_BN_CLICKED(IDC_BTN_LOAD_FXB_0, OnBtnLoadFxb0)
ON_BN_CLICKED(IDC_BTN_LOAD_FXB_1, OnBtnLoadFxb1)
ON_BN_CLICKED(IDC_BTN_LOAD_FXB_2, OnBtnLoadFxb2)
ON_BN_CLICKED(IDC_BTN_LOAD_FXB_3, OnBtnLoadFxb3)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditFxg message handlers

BOOL CDlgEditFxg::OnInitDialog() {
    CDialog::OnInitDialog();

    for (int i = 0; i < MAX_FXB; i++) {
        (*m_pLoop[i]) = FALSE;
        m_pFXBName[i]->Empty();

        m_pJoint[i]->ResetContent();
        m_pJoint[i]->Clear();
        m_pJoint[i]->InsertString(0, "배꼽");
        m_pJoint[i]->SetCurSel(0);

        //    m_CBPartName들 채우기..
        m_pJoint[i]->InsertString(1, "오른손");
        m_pJoint[i]->InsertString(2, "왼손");
    }
    m_strFileName.Empty();

    UpdateData(FALSE);

    return TRUE;
}

bool CDlgEditFxg::LoadScript(const fs::path & fsFile) {
    m_strFileName = fsFile.c_str();
    CN3FXGroup * pGroup = new CN3FXGroup;
    if (!pGroup->DecodeScriptFile(fsFile)) {
        delete pGroup;
        return false;
    }

    //////////////////////////////////////////////////
    //각 컨트롤 셋팅...
    int cnt = pGroup->GetCount();
    for (int i = 0; i < cnt; i++) {
        __FXBInfo * pInfo = pGroup->GetFXBInfo(i);

        m_pJoint[i]->SetCurSel(pInfo->joint);
        *(m_pLoop[i]) = pInfo->IsLooping;
        m_pFXBName[i]->Format(pInfo->szFxbFile);
    }
    UpdateData(FALSE);
    //
    //////////////////////////////////////////////////

    delete pGroup;
    return true;
}

void CDlgEditFxg::SaveGameData() {
    fs::path fsFile = m_strFileName.GetString();

    //OnBtnSave();
    if (fsFile.empty()) {
        return;
    }

    CN3FXGroup * pGroup = new CN3FXGroup;
    if (!pGroup->DecodeScriptFile(fsFile)) {
        delete pGroup;
        return;
    }

    fs::path fsFxgFile = fs::path(fsFile).replace_extension(".fxg");
    pGroup->SaveToFile(fsFxgFile);

    delete pGroup;
    return;
}

void CDlgEditFxg::OnOK() {
    return; //CDialog::OnOK();
}

void CDlgEditFxg::OnClose() {
    CDialog::OnClose();
    m_pRefFrm->DestroyGroup(this);
}

void CDlgEditFxg::OnBtnSaveAs() {
    UpdateData(TRUE);

    CDlgNewFileName dlg;
    dlg.m_strExt = ".n3fxgroup";
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    fs::path fsFile = CN3Base::PathGet() / "fx" / dlg.m_strNewFileName.GetString();
    fsFile += dlg.m_strExt.GetString();
    m_strFileName = fsFile.c_str();

    UpdateData(FALSE);
    OnBtnSave();
}

void CDlgEditFxg::OnBtnSave() {
    UpdateData(TRUE);

    if (m_strFileName == _T("")) {
        OnBtnSaveAs();
        return;
    }

    FILE * file = fopen((LPCTSTR)m_strFileName, "w");
    if (!file) {
        MessageBox("n3fxgroup파일 생성 실패..-.-;;", "ERR02", MB_OK);
        return;
    }

    fprintf(file, "<N3FXGROUP>\n");

    for (int i = 0; i < MAX_FXB; i++) {
        if (m_pFXBName[i]->IsEmpty()) {
            continue;
        }

        int joint = m_pJoint[i]->GetCurSel();

        if ((*(m_pLoop[i]))) {
            fprintf(file, "<fxb> %s %d TRUE\n", (LPCTSTR)(*(m_pFXBName[i])), joint);
        } else {
            fprintf(file, "<fxb> %s %d FALSE\n", (LPCTSTR)(*(m_pFXBName[i])), joint);
        }
    }
    fclose(file);

    SaveGameData();
}

void CDlgEditFxg::OnBtnLoadFxb0() {
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "fxb", NULL, dwFlags, "fxb File(*.fxb)|*.fxb||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_strFXBName_0 = (fs::path("fx") / dlg.GetFileName().GetString()).c_str();
    UpdateData(FALSE);
}

void CDlgEditFxg::OnBtnLoadFxb1() {
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "fxb", NULL, dwFlags, "fxb File(*.fxb)|*.fxb||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_strFXBName_1 = (fs::path("fx") / dlg.GetFileName().GetString()).c_str();
    UpdateData(FALSE);
}

void CDlgEditFxg::OnBtnLoadFxb2() {
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "fxb", NULL, dwFlags, "fxb File(*.fxb)|*.fxb||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_strFXBName_2 = (fs::path("fx") / dlg.GetFileName().GetString()).c_str();
    UpdateData(FALSE);
}

void CDlgEditFxg::OnBtnLoadFxb3() {
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "fxb", NULL, dwFlags, "fxb File(*.fxb)|*.fxb||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_strFXBName_3 = (fs::path("fx") / dlg.GetFileName().GetString()).c_str();
    UpdateData(FALSE);
}
