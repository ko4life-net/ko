// MainFrm.cpp : implementation of the CMainFrame class
//

#include "StdAfx.h"
#include "N3TexViewer.h"

#include "MainFrm.h"
#include "N3TexViewerDoc.h"
#include "DlgFormat.h"
#include "ProgressBar.h"
#include "N3Base/BitMapFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_COMMAND(ID_ADJUST_WINDOW_SIZE, OnAdjustWindowSize)
ON_COMMAND(ID_FILE_CONVERT, OnFileConvert)
ON_COMMAND(ID_TOOL_CONVERT_FILES_AUTOMATICALY, OnToolConvertFilesAutomaticaly)
ON_COMMAND(ID_TOOL_CONVERT_FILES_MANUALLY, OnToolConvertFilesManually)
ON_COMMAND(ID_TOOL_CUT_BMP, OnToolCutBmp)
ON_COMMAND(ID_FILE_OPEN_NEXT, OnFileOpenNext)
ON_COMMAND(ID_FILE_OPEN_PREV, OnFileOpenPrev)
ON_COMMAND(ID_FILE_OPEN_FIRST, OnFileOpenFirst)
ON_COMMAND(ID_FILE_OPEN_LAST, OnFileOpenLast)
ON_COMMAND(ID_TOOL_SAVE_REPEAT, OnToolSaveRepeat)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR, // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() {
    // TODO: add member initialization code here
}

CMainFrame::~CMainFrame() {}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
                               WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
                                   CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1; // fail to create
    }

    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1; // fail to create
    }

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    CN3Base::PathSet(fs::current_path());

    if (m_Eng.Init(TRUE, m_hWnd, 64, 64, 0, TRUE) == false) {
        return -1;
    }

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT & cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const {
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext & dc) const {
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnAdjustWindowSize() {
    this->AdjustWindowSize();
}

void CMainFrame::AdjustWindowSize() {
    // 그림크기에 윈도우 사이즈 맞추기..
    DWORD dwStyle = this->GetStyle();
    CRect rcFrm, rcView;
    this->GetWindowRect(rcFrm);
    GetActiveView()->GetClientRect(rcView);
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)GetActiveDocument();

    int nW = rcFrm.Width() - rcView.Width() + pDoc->m_pTex->Width();
    int nH = rcFrm.Height() - rcView.Height() + pDoc->m_pTex->Height();

    if (pDoc->m_pTex->Width() <= 0) {
        nW += 256;
    }
    if (pDoc->m_pTex->Height() <= 0) {
        nH += 256;
    }

    if (nW < 256) {
        nW = 256;
    }

    SetWindowPos(NULL, 0, 0, nW, nH, SWP_NOMOVE | SWP_NOZORDER);
}

void CMainFrame::OnFileConvert() {
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)GetActiveDocument();
    if (NULL == pDoc->m_pTex || NULL == pDoc->m_pTex->Get()) {
        return;
    }

    CDlgFormat dlg;
    dlg.m_Fmt = pDoc->m_pTex->PixelFormat();
    dlg.m_nWidth = pDoc->m_pTex->Width();
    dlg.m_nHeight = pDoc->m_pTex->Height();
    dlg.m_bMipMap = pDoc->m_pTex->MipMapCount() > 1 ? TRUE : FALSE;
    int nOK = dlg.DoModal();

    if (IDOK != nOK) {
        return;
    }

    pDoc->m_pTex->Convert(dlg.m_Fmt, dlg.m_nWidth, dlg.m_nHeight, dlg.m_bMipMap);
    pDoc->SetTitle(""); // 타이틀 바꾸기..
}

void CMainFrame::OnToolConvertFilesAutomaticaly() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
    CFileDialog dlg(TRUE, "dxt", NULL, dwFlags, "Generic Image Files(*.bmp, *.tga, *.jpg)|*.bmp;*.tga;*.jpg||", NULL);
    std::string szCurDir = fs::current_path().string();
    dlg.m_ofn.lpstrInitialDir = szCurDir.c_str();

    std::vector<char> vFilesBuff(512000);
    dlg.m_ofn.lpstrFile = vFilesBuff.data();
    dlg.m_ofn.nMaxFile = static_cast<DWORD>(vFilesBuff.size());
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    CN3Texture Tex;
    POSITION   pos = dlg.GetStartPosition();
    CString    FileName;
    while (pos != NULL) {
        fs::path fsFile = dlg.GetNextPathName(pos).GetString();
        Tex.LoadFromFile(fsFile);

        if (Tex.Get()) {
            D3DFORMAT Fmt = Tex.PixelFormat();
            if (Fmt == D3DFMT_R8G8B8 || Fmt == D3DFMT_X8R8G8B8 || Fmt == D3DFMT_R5G6B5 || Fmt == D3DFMT_X1R5G5B5) {
                Tex.Convert(D3DFMT_DXT1);
            } else if (Fmt == D3DFMT_A8R8G8B8 || Fmt == D3DFMT_A1R5G5B5) {
                Tex.Convert(D3DFMT_DXT3);
            }

            Tex.m_szName = fsFile.stem().string();
            Tex.SaveToFile(fsFile.replace_extension("dxt"));
            Tex.Release();
        }
    }
}

void CMainFrame::OnToolConvertFilesManually() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
    CFileDialog dlg(TRUE, "dxt", NULL, dwFlags,
                    "Generic Image Files(*.dxt, *.bmp, *.tga, *.jpg)|*.dxt;*.bmp;*.tga;*.jpg|"
                    "BitmapFile File(*.bmp)|*.bmp|"
                    "Targa File(*.tga)|*.tga|"
                    "DXT File(*.dxt)|*.dxt||",
                    NULL);

    std::string szCurDir = fs::current_path().string();
    dlg.m_ofn.lpstrInitialDir = szCurDir.c_str();

    std::vector<char> vFilesBuff(512000);
    dlg.m_ofn.lpstrFile = vFilesBuff.data();
    dlg.m_ofn.nMaxFile = static_cast<DWORD>(vFilesBuff.size());
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    CN3Texture Tex;
    Tex.LoadFromFile(dlg.GetPathName().GetString());

    CDlgFormat dlgFormat;
    dlgFormat.m_nWidth = Tex.Width();
    dlgFormat.m_nHeight = Tex.Height();
    dlgFormat.m_bMipMap = Tex.MipMapCount() > 1 ? TRUE : FALSE;
    if (dlgFormat.DoModal() == IDCANCEL) {
        return;
    }

    POSITION pos = dlg.GetStartPosition();
    while (pos != NULL) {
        fs::path fsFile = dlg.GetNextPathName(pos).GetString();
        Tex.LoadFromFile(fsFile);
        if (Tex.Get()) {
            fsFile.replace_extension(".dxt");
            Tex.Convert(dlgFormat.m_Fmt, dlgFormat.m_nWidth, dlgFormat.m_nHeight, dlgFormat.m_bMipMap);
            Tex.m_szName = fsFile.stem().string();
            Tex.SaveToFile(fsFile);
        }
    }
}

void CMainFrame::OnToolCutBmp() {
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST;
    CFileDialog dlg(TRUE, "bmp", NULL, dwFlags, "Bitmap file(*.bmp)|*.bmp||", NULL);
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    CDlgFormat dlgFmt; // 크기 및 저장 형식 지정.
    if (IDCANCEL == dlgFmt.DoModal()) {
        return;
    }

    int       nW = dlgFmt.m_nWidth;   // 너비
    int       nH = dlgFmt.m_nHeight;  // 높이
    D3DFORMAT fmtSave = dlgFmt.m_Fmt; // 포맷 - BitMap 일때는 무시..

    int  nYesNo = MessageBox("DXT File 로 저장하시겠습니까?", "저장 형식", MB_YESNO);
    bool bSaveToDXT = false;
    if (IDYES == nYesNo) {
        bSaveToDXT = true;
    }

    BMPCutter(dlg.GetPathName().GetString(), nW, nH, bSaveToDXT, fmtSave);
}

BOOL CMainFrame::BMPCutter(const fs::path & fsFile, int iWidth, int iHeight, bool bSaveToDXT, D3DFORMAT fmtDXT) {
    CBitMapFile BMF;
    if (false == BMF.LoadFromFile(fsFile)) {
        return FALSE;
    }

    fs::path fsDir = fsFile.parent_path() / fsFile.stem();
    fs::create_directory(fsDir);

    int xx = BMF.Width() / iWidth;
    int yy = BMF.Height() / iHeight;

    // progress bar
    CProgressBar ProgressBar;
    ProgressBar.Create("cutting bitmap..", 50, yy * xx);
    ProgressBar.SetStep(1);

    std::string szStem = fsFile.stem().string();
    for (int y = 0; y < yy; y++) {
        for (int x = 0; x < xx; x++) {
            fs::path fsDxtFile = fsDir / std::format("{:s}_{:02}{:02}.dxt", szStem, x, y);

            RECT rcDest = {x * iWidth, y * iHeight, (x + 1) * iWidth, (y + 1) * iHeight};
            if (bSaveToDXT) {
                fs::path fsBmpTmpFile = fs::temp_directory_path() / "N3TexViewer_TempConvert.bmp";
                if (!BMF.SaveRectToFile(fsBmpTmpFile, rcDest)) {
                    continue;
                }

                CN3Texture Tex;
                if (!Tex.LoadFromFile(fsBmpTmpFile)) {
                    continue;
                }

                Tex.Convert(fmtDXT);
                Tex.SaveToFile(fsDxtFile);
            } else {
                BMF.SaveRectToFile(fsDxtFile, rcDest);
            }

            ProgressBar.StepIt();
        }
    }

    return TRUE;
    /*
    if (fsFile.empty() || iWidth <= 0 || iHeight <= 0) {
        MessageBox("가로 세로가 0이하인 bitmap으로 나눌 수 없습니다.", "error");
        return FALSE;
    }

    CFile          file;
    CFileException fe;

    // 읽기 모드로 파일 열기
    if (!file.Open(fsFile.string().c_str(), CFile::modeRead | CFile::shareDenyWrite, &fe)) {
        MessageBox("원본 bitmap을 열 수 없습니다.", "error");
        return FALSE;
    }

    // 파일 길이
    DWORD dwBitsSize;
    dwBitsSize = file.GetLength();

    // 파일 헤더 읽기
    BITMAPFILEHEADER bmfHeader;
    if (file.Read(&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader)) {
        MessageBox("원본 bitmap이 이상합니다.", "error");
        return FALSE;
    }

    // bmp 파일임을 나타내는 "BM"마커 확인
    if (bmfHeader.bfType != 0x4D42) {
        MessageBox("원본 파일이 bitmap파일이 아닙니다.", "error");
        return FALSE;
    }

    // BITMAPINFOHEADER 얻기
    BITMAPINFOHEADER bmInfoHeader;
    if (file.Read(&bmInfoHeader, sizeof(bmInfoHeader)) != sizeof(bmInfoHeader)) {
        return FALSE;
    }

    // 픽셀당 비트 수 확인
    WORD wBitCount = bmInfoHeader.biBitCount;
    if (24 != wBitCount) // 24비트 bmp가 아니면 return해 버린다.
    {
        MessageBox("원본 bitmap이 24bit파일이 아닙니다.", "error");
        return FALSE;
    }

    // 가로, 세로로 나누어야 할 수 계산
    int iCX, iCY;
    iCX = (bmInfoHeader.biWidth + iWidth - 1) / iWidth;
    iCY = (bmInfoHeader.biHeight + iHeight - 1) / iHeight;
    if (iCX <= 0 || iCY <= 0) {
        MessageBox("나눌 수 없습니다.", "error");
        return FALSE;
    }

    // 실제 이미지 비트 주소
    //    LPVOID pSrcImageBit;
    //    pSrcImageBit = (LPVOID)((BYTE*)pSrcDIB + (bmfHeader.bfOffBits - sizeof(bmfHeader)));

    // 실제 이미지의 메모리상에 잡힌 가로 길이 (24bit)
    int iRealWidthSrc = ((int)((bmInfoHeader.biWidth * 3 + 3) / 4)) * 4;

    // 새로 만들 이미지 메모리 할당
    int    iRealWidthDest = ((int)((iWidth * 3 + 3) / 4)) * 4;
    int    iDestDIBSize = sizeof(BITMAPINFOHEADER) + iRealWidthDest * iHeight;
    LPVOID pDestDIB;
    if ((pDestDIB = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, iDestDIBSize)) == NULL) {
        MessageBox("메모리를 할당하지 못했습니다.", "error");
        return FALSE;
    }

    // 새로 만들 이미지 file header 정보 채우기
    BITMAPFILEHEADER bmfHeaderDest;
    memset(&bmfHeaderDest, 0, sizeof(bmfHeaderDest));
    bmfHeaderDest.bfType = 0x4D42; // "BM"
    bmfHeaderDest.bfSize = sizeof(bmfHeaderDest) + iDestDIBSize;
    bmfHeaderDest.bfOffBits = sizeof(bmfHeaderDest) + sizeof(BITMAPINFOHEADER);

    // 새로 만들 이미지 bitmap info header 정보 채우기
    BITMAPINFOHEADER bmInfoHeaderDest;
    memset(&bmInfoHeaderDest, 0, sizeof(bmInfoHeaderDest));
    bmInfoHeaderDest.biSize = sizeof(bmInfoHeaderDest);
    bmInfoHeaderDest.biWidth = iWidth;
    bmInfoHeaderDest.biHeight = iHeight;
    bmInfoHeaderDest.biPlanes = 1;
    bmInfoHeaderDest.biBitCount = 24;
    bmInfoHeaderDest.biSizeImage = iRealWidthDest * iHeight;
    memcpy(pDestDIB, &bmInfoHeaderDest, sizeof(bmInfoHeaderDest));

    fs::path fsDir = fsFile.parent_path() / fsFile.stem();
    fs::create_directory(fsDir);

    // 쪼갠 정보를 tcd파일에 넣어서 저장
    DWORD    dwNum;
    fs::path fsTcdFile = fsDir / (fsFile.stem() + ".tcd");
    HANDLE   hFile = CreateFileW(fsTcdFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != hFile) {
        WriteFile(hFile, &iCX, sizeof(iCX), &dwNum, NULL);
        WriteFile(hFile, &iCY, sizeof(iCX), &dwNum, NULL);
        CloseHandle(hFile);
    }

    // progress bar
    CProgressBar ProgressBar;
    ProgressBar.Create("cutting bitmap..", 50, iCY * iCX);
    ProgressBar.SetStep(1);

    // 새로 쪼개서 저장하기
    BYTE * pTmpBitDest;
    pTmpBitDest = ((BYTE *)pDestDIB) + sizeof(BITMAPINFOHEADER);

    std::string szStem = fsFile.stem().string();
    for (int j = 0; j < iCY; ++j) {
        for (int i = 0; i < iCX; ++i) {
            memset(pTmpBitDest, 0, iDestDIBSize - sizeof(BITMAPINFOHEADER));
            for (int y = 0; y < iHeight; ++y) {
                if ((iHeight * j + y) >= bmInfoHeader.biHeight) {
                    break; // 맨 아래가 짤릴 경우가 있다
                }

                // 원본파일의 읽어올 부분의 file position을 맞게 세팅한다.
                file.Seek(bmfHeader.bfOffBits + iRealWidthSrc * (bmInfoHeader.biHeight - 1 - (iHeight * j + y)) +
                              3 * (iWidth * i),
                          CFile::begin);

                if (i == (iCX - 1)) { // 맨 오른쪽 끝은 짤릴 가능성이 있다.
                    file.Read(pTmpBitDest + iRealWidthDest * (iHeight - 1 - y),
                              bmInfoHeader.biWidth * 3 - iRealWidthDest * (iCX - 1));
                } else {
                    file.Read(pTmpBitDest + iRealWidthDest * (iHeight - 1 - y), iRealWidthDest);
                }
            }

            // 저장하기
            fs::path fsTexFileTmp;
            if (bSaveToDXT) {
                fsTexFileTmp = fs::temp_directory_path() / "N3TexViewer_TempConvert.bmp";
            } else {
                fsTexFileTmp = fsDir / std::format("{:s}_{:02}{:02}.dxt", szStem, i, iCY - 1 - j);
            }

            hFile =
                CreateFileW(fsTexFileTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE != hFile) {
                WriteFile(hFile, &bmfHeaderDest, sizeof(bmfHeaderDest), &dwNum, NULL);
                WriteFile(hFile, pDestDIB, iDestDIBSize, &dwNum, NULL);
                CloseHandle(hFile);
            }
            ProgressBar.StepIt();
            this->UpdateWindow();

            if (bSaveToDXT) { // DXT 를 저장하려면..
                CN3Texture TexTmp;
                TexTmp.LoadFromFile(fsTcdFile);     // 로딩
                if (true == TexTmp.Convert(fmtDXT)) // 변환
                {
                    fs::path fsDxtFile = (fsDir / fsFile.stem()) + std::format("_{:02}{:02}.dxt", i, iCY - 1 - j);
                    TexTmp.SaveToFile(fsDxtFile);
                }

                fs::remove(fsTcdFile.c_str()); // 임시 비트맵 파일 지우기..
            }
        }
    }

    // 메모리 풀어줌
    ::GlobalFree(pDestDIB);
    file.Close();

    return TRUE;
    */
}

void CMainFrame::OnFileOpenNext() {
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)(this->GetActiveDocument());
    if (pDoc) {
        pDoc->OpenNextFile();
    }
    //AdjustWindowSize();
}

void CMainFrame::OnFileOpenPrev() {
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)(this->GetActiveDocument());
    if (pDoc) {
        pDoc->OpenPrevFile();
    }
    //AdjustWindowSize();
}

void CMainFrame::OnFileOpenFirst() {
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)(this->GetActiveDocument());
    if (pDoc) {
        pDoc->OpenFirstFile();
    }
    //AdjustWindowSize();
}

void CMainFrame::OnFileOpenLast() {
    CN3TexViewerDoc * pDoc = (CN3TexViewerDoc *)(this->GetActiveDocument());
    if (pDoc) {
        pDoc->OpenLastFile();
    }
    //AdjustWindowSize();
}

void CMainFrame::OnToolSaveRepeat() {
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
    CFileDialog dlg(TRUE, "dxt", NULL, dwFlags, "Noah Texture Files(*.dxt)|*.dxt||", NULL);
    std::string szCurDir = fs::current_path().string();
    dlg.m_ofn.lpstrInitialDir = szCurDir.c_str();

    std::vector<char> vFilesBuff(512000);
    dlg.m_ofn.lpstrFile = vFilesBuff.data();
    dlg.m_ofn.nMaxFile = static_cast<DWORD>(vFilesBuff.size());
    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    POSITION pos = dlg.GetStartPosition();

    std::vector<fs::path> vFiles;
    while (pos) {
        vFiles.emplace_back(dlg.GetNextPathName(pos).GetString());
    }

    // progress bar
    CProgressBar ProgressBar;
    ProgressBar.Create("ReSave DXT Files..", 50, static_cast<int>(vFiles.size()));
    ProgressBar.SetStep(1);

    for (const auto & fsFile : vFiles) {
        CN3Texture Tex;
        Tex.LoadFromFile(fsFile);
        Tex.SaveToFile(fsFile);

        ProgressBar.StepIt();
    }
}
