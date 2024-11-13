// KscViewerDoc.cpp : implementation of the CKscViewerDoc class
//

#include "StdAfx.h"
#include "KscViewer.h"

#include "KscViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKscViewerDoc

IMPLEMENT_DYNCREATE(CKscViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CKscViewerDoc, CDocument)
//{{AFX_MSG_MAP(CKscViewerDoc)
ON_COMMAND(ID_FILE_SAVE, OnFileSave)
ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKscViewerDoc construction/destruction

CKscViewerDoc::CKscViewerDoc() {
    m_pJpegFile = NULL;
    m_pJpegFile = new CN3JpegFile;
}

CKscViewerDoc::~CKscViewerDoc() {
    if (m_pJpegFile) {
        delete m_pJpegFile;
    }
    m_pJpegFile = NULL;
}

BOOL CKscViewerDoc::OnNewDocument() {
    if (!CDocument::OnNewDocument()) {
        return FALSE;
    }

    if (m_pJpegFile) {
        m_pJpegFile->Release();
    }

    m_fsKscFile.clear();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CKscViewerDoc serialization

void CKscViewerDoc::Serialize(CArchive & ar) {
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CKscViewerDoc diagnostics

#ifdef _DEBUG
void CKscViewerDoc::AssertValid() const {
    CDocument::AssertValid();
}

void CKscViewerDoc::Dump(CDumpContext & dc) const {
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKscViewerDoc commands

BOOL CKscViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) {
    fs::path fsFile = lpszPathName;
    //if (!CDocument::OnOpenDocument(lpszPathName)) {
    //    return FALSE;
    //}
    if (m_pJpegFile == NULL) {
        return FALSE;
    }

    fs::path fsExt = fsFile.extension();
    if (n3std::iequals(fsExt, ".ksc")) {
        if (m_pJpegFile->DecryptJPEG(fsFile)) {
            m_fsKscFile = fsFile;
        }
    } else if (n3std::iequals(fsExt, ".jpg")) {
        if (m_pJpegFile->LoadJpegFile(fsFile)) {
            m_fsKscFile = fsFile;
        }
    }

    UpdateAllViews(NULL);
    return TRUE;
}

CN3JpegFile * CKscViewerDoc::GetJpegFile() {
    return m_pJpegFile;
}

BOOL CKscViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) {
    if (!m_pJpegFile) {
        return FALSE;
    }
    if (m_fsKscFile.empty()) {
        return FALSE;
    }
    if (!n3std::iequals(m_fsKscFile.extension(), ".ksc")) {
        return FALSE;
    }

    return m_pJpegFile->SaveFromDecryptToJpeg(m_fsKscFile, lpszPathName);
}

void CKscViewerDoc::OnFileSave() {
    AfxGetApp()->DoWaitCursor(1);

    fs::path fsJpgFile = fs::path(m_fsKscFile).replace_extension(".jpg");
    OnSaveDocument(fsJpgFile.string().c_str());
    AfxGetApp()->DoWaitCursor(-1);
}

void CKscViewerDoc::OnFileSaveAs() {
    if (m_fsKscFile.empty()) {
        return;
    }
    if (!n3std::iequals(m_fsKscFile.extension(), ".ksc")) {
        return;
    }

    fs::path fsFile = m_fsKscFile.stem() + ".jpg";
    CString  szFile = fsFile.c_str();
    CString  szFilter = "Jpeg File (*.jpg)|*.jpg||";

    // OPENFILENAME - so i can get to its Help page easily
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog fileDlg(FALSE, szFile, szFile, dwFlags, szFilter);

    std::string szCurDir = fs::current_path().string();
    fileDlg.m_ofn.lpstrInitialDir = szCurDir.c_str();
    fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;

    if (fileDlg.DoModal() == IDOK) {
        AfxGetApp()->DoWaitCursor(1);

        OnSaveDocument(fileDlg.GetPathName());
        AfxGetApp()->DoWaitCursor(-1);
    }
}
