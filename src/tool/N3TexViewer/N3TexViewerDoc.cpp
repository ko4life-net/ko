// N3TexViewerDoc.cpp : implementation of the CN3TexViewerDoc class
//

#include "StdAfx.h"
#include "N3TexViewer.h"
#include "N3TexViewerDoc.h"

#include "N3Base/BitMapFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3TexViewerDoc

IMPLEMENT_DYNCREATE(CN3TexViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CN3TexViewerDoc, CDocument)
//{{AFX_MSG_MAP(CN3TexViewerDoc)
ON_COMMAND(ID_FILE_SAVE_AS_BITMAP, OnFileSaveAsBitmap)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3TexViewerDoc construction/destruction

CN3TexViewerDoc::CN3TexViewerDoc() {
    m_pTex = new CN3Texture();
    m_pTexAlpha = new CN3Texture();

    m_nCurFile = 0;
}

CN3TexViewerDoc::~CN3TexViewerDoc() {
    delete m_pTex;
    delete m_pTexAlpha;
}

/////////////////////////////////////////////////////////////////////////////
// CN3TexViewerDoc serialization
void CN3TexViewerDoc::Serialize(CArchive & ar) {
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CN3TexViewerDoc diagnostics

#ifdef _DEBUG
void CN3TexViewerDoc::AssertValid() const {
    CDocument::AssertValid();
}

void CN3TexViewerDoc::Dump(CDumpContext & dc) const {
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3TexViewerDoc commands

BOOL CN3TexViewerDoc::OnNewDocument() {
    if (!CDocument::OnNewDocument()) {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    m_pTex->Release();
    m_pTexAlpha->Release();

    UpdateAllViews(NULL);

    return TRUE;
}

BOOL CN3TexViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) {
    if (!CDocument::OnOpenDocument(lpszPathName)) {
        return FALSE;
    }

    fs::path fsFile = lpszPathName;

    FindFiles(); // 파일을 찾고..

    // TODO: Add your specialized creation code here
    m_pTexAlpha->Release();
    if (!m_pTex->LoadFromFile(fsFile)) {
        return FALSE;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Alpha Texture 생성...
    m_pTexAlpha->Create(m_pTex->Width(), m_pTex->Height(), D3DFMT_A8R8G8B8, FALSE);
    LPDIRECT3DSURFACE9 lpSurf, lpSurf2;
    if (m_pTexAlpha->Get()) {
        m_pTexAlpha->Get()->GetSurfaceLevel(0, &lpSurf);
        m_pTex->Get()->GetSurfaceLevel(0, &lpSurf2);
        ::D3DXLoadSurfaceFromSurface(lpSurf, NULL, NULL, lpSurf2, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
        lpSurf2->Release();
        lpSurf2 = NULL;

        D3DLOCKED_RECT LR;
        lpSurf->LockRect(&LR, NULL, 0);
        int     width = m_pTexAlpha->Width(), height = m_pTexAlpha->Height();
        DWORD   dwAlpha = 0;
        DWORD * pPixel = NULL;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                pPixel = (DWORD *)LR.pBits + y * (LR.Pitch / 4) + x;
                dwAlpha = (*pPixel) >> 24;
                *pPixel = dwAlpha | (dwAlpha << 8) | (dwAlpha << 16) | 0xff000000;
            }
        }
        lpSurf->UnlockRect();
        lpSurf->Release();
        lpSurf = NULL;
    }
    // Alpha Texture 생성...
    ////////////////////////////////////////////////////////////////////////////////

    CString szFileName = fsFile.filename().c_str();
    SetTitle(szFileName);

    // Update status bar with the currently opened file path
    CFrameWnd * pFrm = (CFrameWnd *)AfxGetMainWnd();
    ASSERT(pFrm);
    CStatusBar * pSB = (CStatusBar *)pFrm->GetMessageBar();
    ASSERT(pSB);
    pSB->SetPaneText(0, fsFile.string().c_str());

    UpdateAllViews(NULL);

    return TRUE;
}

BOOL CN3TexViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) {
    fs::path fsFile = lpszPathName;
    if (!n3std::iequals(fsFile.extension(), ".dxt")) {
        MessageBox(::GetActiveWindow(), "You need to change the extension to DXT. Please save it as Save As.",
                   "Save failed", MB_OK);
        return FALSE;
    }

    if (!m_pTex->SaveToFile(fsFile)) {
        return FALSE;
    }

    return CDocument::OnSaveDocument(lpszPathName);
}

void CN3TexViewerDoc::SetTitle(LPCTSTR lpszTitle) {
    std::string szFmt = std::format("{:s} - {:d}, {:d}", lpszTitle, m_pTex->Width(), m_pTex->Height());

    D3DFORMAT fmtTex = m_pTex->PixelFormat();
    if (D3DFMT_DXT1 == fmtTex) {
        szFmt += " DXT1";
    } else if (D3DFMT_DXT2 == fmtTex) {
        szFmt += " DXT2";
    } else if (D3DFMT_DXT3 == fmtTex) {
        szFmt += " DXT3";
    } else if (D3DFMT_DXT4 == fmtTex) {
        szFmt += " DXT4";
    } else if (D3DFMT_DXT5 == fmtTex) {
        szFmt += " DXT5";
    } else if (D3DFMT_A1R5G5B5 == fmtTex) {
        szFmt += " A1R5G5B5";
    } else if (D3DFMT_A4R4G4B4 == fmtTex) {
        szFmt += " A4R4G4B4";
    } else if (D3DFMT_R8G8B8 == fmtTex) {
        szFmt += " R8G8B8";
    } else if (D3DFMT_A8R8G8B8 == fmtTex) {
        szFmt += " A8R8G8B8";
    } else if (D3DFMT_X8R8G8B8 == fmtTex) {
        szFmt += " X8R8G8B8";
    } else {
        szFmt += " Unknown Format";
    }

    if (m_pTex->MipMapCount() > 1) {
        szFmt += " - has MipMap";
    } else {
        szFmt += " - has no MipMap";
    }

    CDocument::SetTitle(szFmt.c_str());
}

void CN3TexViewerDoc::FindFiles() {
    if (m_fsWorkDir == CN3Base::PathGet()) {
        return;
    }

    m_fsWorkDir = CN3Base::PathGet();
    m_vDxtFiles.clear();
    m_nCurFile = 0;

    int i = 0;
    for (const auto & fsEntry : fs::recursive_directory_iterator(fs::current_path())) {
        if (!fsEntry.is_regular_file() || !n3std::iequals(fsEntry.path().extension(), ".dxt")) {
            continue;
        }

        fs::path fsDxtFile = fsEntry.path();
        m_vDxtFiles.emplace_back(fsDxtFile);

        if (fsDxtFile.c_str() == GetPathName()) {
            m_nCurFile = i;
        }
        ++i;
    }
}

void CN3TexViewerDoc::OpenFileAtIndex(int iIndex) {
    if (m_vDxtFiles.empty()) {
        return;
    }

    // Clamp the index to stay within valid range
    m_nCurFile = std::clamp(iIndex, 0, static_cast<int>(m_vDxtFiles.size()) - 1);
    OnOpenDocument(m_vDxtFiles[m_nCurFile].string().c_str());
}

void CN3TexViewerDoc::OpenNextFile() {
    OpenFileAtIndex(m_nCurFile + 1);
}

void CN3TexViewerDoc::OpenPrevFile() {
    OpenFileAtIndex(m_nCurFile - 1);
}

void CN3TexViewerDoc::OpenFirstFile() {
    OpenFileAtIndex(0);
}

void CN3TexViewerDoc::OpenLastFile() {
    OpenFileAtIndex(static_cast<int>(m_vDxtFiles.size()) - 1);
}

void CN3TexViewerDoc::OnFileSaveAsBitmap() {
    if (!m_pTex || !m_pTex->Get()) {
        return;
    }

    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST;
    CFileDialog dlg(FALSE, "bmp", NULL, dwFlags, "Bitmap file(*.bmp)|*.bmp||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    m_pTex->SaveToBitmapFile(dlg.GetPathName().GetString());
}
