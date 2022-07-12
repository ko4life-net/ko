// N3IndoorDoc.cpp : implementation of the CN3IndoorDoc class
//

#include "stdafx.h"
#include "N3Indoor.h"

#include "N3IndoorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorDoc

IMPLEMENT_DYNCREATE(CN3IndoorDoc, CDocument)

BEGIN_MESSAGE_MAP(CN3IndoorDoc, CDocument)
	//{{AFX_MSG_MAP(CN3IndoorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorDoc construction/destruction

CN3IndoorDoc::CN3IndoorDoc()
{
	// TODO: add one-time construction code here

}

CN3IndoorDoc::~CN3IndoorDoc()
{
}

BOOL CN3IndoorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CN3IndoorDoc serialization

void CN3IndoorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorDoc diagnostics

#ifdef _DEBUG
void CN3IndoorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CN3IndoorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorDoc commands
