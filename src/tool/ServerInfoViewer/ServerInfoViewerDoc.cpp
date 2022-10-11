// ServerInfoViewerDoc.cpp : implementation of the CServerInfoViewerDoc class
//

#include "StdAfx.h"
#include "ServerInfoViewer.h"

#include "ServerInfoViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerDoc

IMPLEMENT_DYNCREATE(CServerInfoViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerInfoViewerDoc, CDocument)
//{{AFX_MSG_MAP(CServerInfoViewerDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerDoc construction/destruction

CServerInfoViewerDoc::CServerInfoViewerDoc() {

    /////////////////////
    // Test
    __CharacterSymbol symbol;
    __Vector3         vPos;
    for (int i = 0; i < 1000; i++) {
        symbol.mtxWorld.Identity();
        symbol.mtxWorld.PosSet((rand() % 40000) / 10.f, 0, (rand() % 40000) / 10.f);
        symbol.color = D3DCOLOR_ARGB(255, rand() % 256, rand() % 256, rand() % 256);
        m_Players.push_back(symbol);
    }
    // Test
    /////////////////////
}

CServerInfoViewerDoc::~CServerInfoViewerDoc() {}

BOOL CServerInfoViewerDoc::OnNewDocument() {
    if (!CDocument::OnNewDocument()) {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerDoc serialization

void CServerInfoViewerDoc::Serialize(CArchive & ar) {
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerDoc diagnostics

#ifdef _DEBUG
void CServerInfoViewerDoc::AssertValid() const {
    CDocument::AssertValid();
}

void CServerInfoViewerDoc::Dump(CDumpContext & dc) const {
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerInfoViewerDoc commands
