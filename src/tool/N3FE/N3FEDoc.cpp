// N3FEDoc.cpp : implementation of the CN3FEDoc class
//

#include "stdafx.h"
#include "N3FE.h"

#include "N3FEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CN3FEDoc

IMPLEMENT_DYNCREATE(CN3FEDoc, CDocument)

BEGIN_MESSAGE_MAP(CN3FEDoc, CDocument)
	//{{AFX_MSG_MAP(CN3FEDoc)
	ON_COMMAND(ID_FILE_IMPORT_BUNDLE, OnFileImportBundle)
	ON_COMMAND(ID_FILE_IMPORT_PARTS, OnFileImportParts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CN3FEDoc construction/destruction

CN3FEDoc::CN3FEDoc()
{
}

CN3FEDoc::~CN3FEDoc()
{
}

BOOL CN3FEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_szBundleScripts.RemoveAll();
	m_szPartScripts.RemoveAll();
	m_EffectMgr.Release();

//	char szPath[256] = "";
//	::GetCurrentDirectory(256, szPath);
//	m_EffectMgr.PathSet(szPath); // Path 초기화..
	
	this->UpdateAllViews(NULL);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CN3FEDoc serialization

void CN3FEDoc::Serialize(CArchive& ar)
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
// CN3FEDoc diagnostics

#ifdef _DEBUG
void CN3FEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CN3FEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CN3FEDoc commands

BOOL CN3FEDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT]; // 파일 이름을 분리하고..
	_splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

	char szTmp[256];
	GetPrivateProfileString("Effect Manager", "Name", "NoName", szFName, _MAX_PATH, lpszPathName); // Ini 파일에 이름 쓰기..
	GetPrivateProfileString("Effect Manager", "BundelCount", "0", szTmp, 256, lpszPathName); // Ini 파일에 뭉치 숫자 쓰기..
	int nBC = atoi(szTmp);
	GetPrivateProfileString("Effect Manager", "PartCount", "0", szTmp, 256, lpszPathName); // Ini 파일에 파트 숫자 쓰기..
	int nPC = atoi(szTmp);

	for(int i = 0; i < nBC; i++)
	{
		CString szBundleFN;
		szBundleFN.Format("%s%s%s%.3d.N3FXBundle", szDrive, szDir, szFName, i);

		CFile file;
		if(file.Open(szBundleFN, CFile::modeRead, NULL))
		{
			DWORD dwL = file.GetLength();
			if(dwL != 0xffffffff)
			{
				char* szBuff = new char[dwL+1];
				DWORD dwRWC = 0;
				file.Read(szBuff, dwL); szBuff[dwL] = NULL;
				m_szBundleScripts.Add(szBuff);
				delete [] szBuff;
			}
			file.Close();
		}
		__EffectBundle* pEB = new __EffectBundle;
		if(false == m_EffectMgr.ParsingBundle(szBundleFN, pEB)) 
		{
			MessageBox(::GetActiveWindow(), szBundleFN2.c_str(), "Parsing Bundle Fail...", MB_OK);
		}

		m_EffectMgr.m_BundlesSrc.push_back(pEB);
	}

	for(i = 0; i < nPC; i++)
	{
		CString szPartFN;
		szPartFN.Format("%s%s%s%.3d.N3FXPart", szDrive, szDir, szFName, i);

		CFile file;
		if(file.Open(szPartFN, CFile::modeRead, NULL))
		{
			DWORD dwL = file.GetLength();
			if(dwL != 0xffffffff)
			{
				char* szBuff = new char[dwL+1];
				DWORD dwRWC = 0;
				file.Read(szBuff, dwL); szBuff[dwL] = NULL;
				m_szPartScripts.Add(szBuff);
				delete [] szBuff;
			}
			file.Close();
			
			__EffectPartBase* pEP = new __EffectPartBase;
			if(false == m_EffectMgr.ParsingParts(szPartFN, pEP))
			{
				MessageBox(::GetActiveWindow(), szPartFN2.c_str(), "Parsing Part Fail...", MB_OK);
			}

			m_EffectMgr.m_Parts.push_back(pEP);
		}
	}

	this->UpdateAllViews(NULL);

	return TRUE;
}

BOOL CN3FEDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if(!CDocument::OnSaveDocument(lpszPathName))
		return FALSE;

	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT]; // 파일 이름을 분리하고..
	_splitpath(lpszPathName, szDrive, szDir, szFName, szExt);

	///////////////////////////////////////////////////
	// Binary 저장..
	char szGameFN[512];
	_makepath(szGameFN, szDrive, szDir, szFName, ".N3FX");
	m_EffectMgr.SaveToFile(szGameFN); // Binary 저장..
	// Binary 저장..
	///////////////////////////////////////////////////

	CFile file;
	file.Open(lpszPathName, CFile::modeWrite | CFile::modeCreate);
//	file.Write("[Effect Manager]\r\n", 18);
//	file.Write("BundleCount=\r\n", 14);
//	file.Write("PartCount=\r\n", 12);
	file.Close();

	CString szTmp;
	WritePrivateProfileString("Effect Manager", "Name", szFName, lpszPathName); // Ini 파일에 이름 쓰기..
	szTmp.Format("%d", m_EffectMgr.BundleCount());
	WritePrivateProfileString("Effect Manager", "BundelCount", szTmp, lpszPathName); // Ini 파일에 뭉치 숫자 쓰기..
	szTmp.Format("%d", m_EffectMgr.PartCount());
	WritePrivateProfileString("Effect Manager", "PartCount", szTmp, lpszPathName); // Ini 파일에 파트 숫자 쓰기..

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Text 저장..
	CString szBundleFN, szBS;
	for(int i = 0; i < m_szBundleScripts.GetSize(); i++)
	{
		szBundleFN.Format("%s%s%s%.3d.N3FXBundle", szDrive, szDir, szFName, i);
		if(file.Open(szBundleFN, CFile::modeWrite | CFile::modeCreate, NULL))
		{
			CString szBS = m_szBundleScripts[i];
			DWORD dwL = szBS.GetLength();
			if(dwL != 0xffffffff)
			{
				DWORD dwRWC = 0;
				file.Write(szBS, dwL);
			}
			file.Close();
		}
	}

	CString szPartsFN, szPS;
	for(i = 0; i < m_szPartScripts.GetSize(); i++)
	{
		szPartsFN.Format("%s%s%s%.3d.N3FXPart", szDrive, szDir, szFName, i);
		if(file.Open(szPartsFN, CFile::modeWrite | CFile::modeCreate, NULL))
		{
			szPS = m_szPartScripts[i];
			DWORD dwL = szPS.GetLength();
			if(dwL != 0xffffffff)
			{
				DWORD dwRWC = 0;
				file.Write(szPS, dwL);
			}
		}
		file.Close();
	}
	// Text 저장..
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CN3FEDoc::UpdateEffectData(BOOL bSaveAndUpdateEffectData)
{
	CString szPath = this->GetPathName();
	this->OnSaveDocument(szPath);
	this->OnOpenDocument(szPath);
}

void CN3FEDoc::OnFileImportBundle() 
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(TRUE, "N3FXBundle Files", NULL, dwFlags, "Effect Bundle Files(*.N3FXBundle)|*.N3FXBundle||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;

	if(dlg.DoModal() == IDCANCEL) return;
	std::string szPath = dlg.GetPathName();

	__EffectBundle* pEB = new __EffectBundle;
	if(false ==	m_EffectMgr.ParsingBundle((LPCTSTR)szPath, pEB))
	{
		delete pEB;
		pEB = NULL;
		return;
	}
	else
	{
		m_EffectMgr.m_BundlesSrc.push_back(pEB);
	}
	
	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwL = ::GetFileSize(hFile, NULL);
	if(dwL != 0xffffffff)
	{
		char* szBuff = new char[dwL+1];
		DWORD dwRWC = 0;
		ReadFile(hFile, szBuff, dwL, &dwRWC, NULL); szBuff[dwL] = NULL;
		m_szBundleScripts.Add(szBuff);
		delete [] szBuff;

		this->UpdateAllViews(NULL);
	}
	CloseHandle(hFile);
}

void CN3FEDoc::OnFileImportParts() 
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlg(TRUE, "N3FXParts Files", NULL, dwFlags, "Effect Parts Files(*.N3FXParts)|*.N3FXParts||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;

	if(dlg.DoModal() == IDCANCEL) return;
	std::string szPath = dlg.GetPathName();
	
	__EffectPartBase* pEPB = new __EffectPartBase;
	if(false == m_EffectMgr.ParsingParts((LPCTSTR)szPath, pEPB)) 
	{
		delete pEPB;
		pEPB = NULL;
		return;
	}
	else
	{
		m_EffectMgr.m_Parts.push_back(pEPB);
	}

	DWORD dwRWC = 0;
	HANDLE hFile = ::CreateFile(szPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwL = ::GetFileSize(hFile, NULL);
	if(dwL != 0xffffffff)
	{
		char* szBuff = new char[dwL+1];
		DWORD dwRWC = 0;
		ReadFile(hFile, szBuff, dwL, &dwRWC, NULL); szBuff[dwL] = NULL;
		m_szPartScripts.Add(szBuff);
		delete [] szBuff;

		this->UpdateAllViews(NULL);
	}
	CloseHandle(hFile);
}
