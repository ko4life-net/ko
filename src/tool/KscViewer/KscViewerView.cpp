// KscViewerView.cpp : implementation of the CKscViewerView class
//

#include "StdAfx.h"
#include "KscViewer.h"

#include "KscViewerDoc.h"
#include "KscViewerView.h"
#include "N3Base/JpegFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKscViewerView

IMPLEMENT_DYNCREATE(CKscViewerView, CView)

BEGIN_MESSAGE_MAP(CKscViewerView, CView)
//{{AFX_MSG_MAP(CKscViewerView)
ON_WM_CREATE()
ON_WM_DROPFILES()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKscViewerView construction/destruction

CKscViewerView::CKscViewerView() {
    // TODO: add construction code here
}

CKscViewerView::~CKscViewerView() {}

BOOL CKscViewerView::PreCreateWindow(CREATESTRUCT & cs) {
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKscViewerView drawing

void CKscViewerView::OnDraw(CDC * pDC) {
    CKscViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    DrawImage();
}

/////////////////////////////////////////////////////////////////////////////
// CKscViewerView diagnostics

#ifdef _DEBUG
void CKscViewerView::AssertValid() const {
    CView::AssertValid();
}

void CKscViewerView::Dump(CDumpContext & dc) const {
    CView::Dump(dc);
}

CKscViewerDoc * CKscViewerView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKscViewerDoc)));
    return (CKscViewerDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKscViewerView message handlers

int CKscViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CView::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    DragAcceptFiles();
    return 0;
}

void CKscViewerView::OnDropFiles(HDROP hDropInfo) {
    CKscViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    fs::path::value_type szFile[260]{};
    ::DragQueryFileW(hDropInfo, 0xFFFF, NULL, 0);
    ::DragQueryFileW(hDropInfo, 0, szFile, std::size(szFile) - 1);
    ::DragFinish(hDropInfo);

    fs::path fsFile = szFile;
    if (pDoc) {
        pDoc->OnOpenDocument(fsFile.string().c_str());
    }

    CView::OnDropFiles(hDropInfo);
}

BOOL CKscViewerView::DrawImage() {
    CKscViewerDoc * pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (pDoc == NULL) {
        return FALSE;
    }
    CN3JpegFile * pFile = pDoc->GetJpegFile();

    if (pFile == NULL) {
        return FALSE;
    }

    CDC * theDC = GetDC();
    pFile->DrawImage(theDC->m_hDC);

    ReleaseDC(theDC);
    return TRUE;
}
