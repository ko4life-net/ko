// DlgRename.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog


CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRename)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRename)
	DDX_Control(pDX, IDC_CB_DIGIT, m_CBDigit);
	DDX_Control(pDX, IDC_LIST, m_FileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
	//{{AFX_MSG_MAP(CDlgRename)
	ON_BN_CLICKED(IDC_B_ADD, OnBAdd)
	ON_BN_CLICKED(IDC_B_CONVERT, OnBConvert)
	ON_BN_CLICKED(IDC_B_REMOVE, OnBRemove)
	ON_BN_CLICKED(IDC_B_SORT, OnBSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRename message handlers

void CDlgRename::OnBAdd() 
{
	char szBuff[10240] = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CFileDialog dlg(TRUE, "Any Files", NULL, dwFlags, "All Files(*.*)|*.*|Bitmap files(*.bmp)|*.bmp|DXT files(*.dxt)|*.dxt||", NULL);
	char szCurPath[256]; GetCurrentDirectory(256, szCurPath);
	dlg.m_ofn.lpstrInitialDir = szCurPath;
	dlg.m_ofn.nMaxFile = 10240;
	dlg.m_ofn.lpstrFile = szBuff;

	if(dlg.DoModal() == IDCANCEL) return;

	POSITION pos = dlg.GetStartPosition();
	CStringArray szFNs;
	while(pos != NULL)
	{
		szFNs.Add(dlg.GetNextPathName(pos));
	}

	int nFN = szFNs.GetSize();
	for(int i = 0; i < nFN; i++)
	{
		int nSel = 0;
		if(i == 0) nSel = m_FileList.AddString(szFNs[nFN-1]);
		else if(i == nFN - 1) nSel = m_FileList.AddString(szFNs[0]);
		else nSel = m_FileList.AddString(szFNs[i]);

		m_FileList.SetSel(nSel);
	}
}

void CDlgRename::OnBRemove() 
{
	int nIndices[1024];
	memset(nIndices, -1, sizeof(nIndices));

	m_FileList.GetSelItems(1024, nIndices);

	int index = 0;
	for(int i = 1023; i >= 0; i--)
	{
		index = nIndices[i];
		if(index >= 0)
		{
			m_FileList.DeleteString(index);
		}
	}
}

void CDlgRename::OnBConvert() 
{
	CString szName;
	GetDlgItemText(IDC_E_NAME, szName);

	int nDigit = m_CBDigit.GetCurSel();
	
	int nIndices[1024];
	memset(nIndices, -1, sizeof(nIndices));

	m_FileList.GetSelItems(1024, nIndices);

	int index = 0;
	int nSeq = 0;
	for(int i = 0; i < 1024; i++)
	{
		index = nIndices[i];
		if(index < 0) continue;
		
		CString szPath;
		m_FileList.GetText(index, szPath);
		int nF = szPath.ReverseFind('.');
		if(nF < 0) continue;

		int nF2 = szPath.ReverseFind('\\');
		if(nF2 < 0)
		{
			nF2 = szPath.ReverseFind('/');
		}
		if(nF2 < 0) continue;
		CString szDir = szPath.Left(nF2+1);
		CString szExt = szPath.Mid(nF + 1, szPath.GetLength() - nF); // 확장자..
		CString szNew = szDir + szName; // 이름

		CString szAdd;
		if(nDigit == 0) szAdd.Format("%.1d.%s", nSeq, szExt);
		if(nDigit == 1) szAdd.Format("%.2d.%s", nSeq, szExt);
		if(nDigit == 2) szAdd.Format("%.3d.%s", nSeq, szExt);
		szNew += szAdd;

		rename(szPath, szNew);
		nSeq++;
	}

	for(i = 1023; i >= 0; i--)
	{
		index = nIndices[i];
		if(index < 0) continue;
		m_FileList.DeleteString(index);
	}
}


BOOL CDlgRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CBDigit.SetCurSel(2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRename::OnBSort() 
{
}
