#pragma once

// SettingDlg.h : header file
//

#pragma warning(disable : 4786)		// Visual C++ Only
#include <set>
using namespace std;

#include "ZipArchive.h"

typedef std::set <int>	HistoryList;
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
class CLoginServerDlg;
class CSettingDlg : public CDialog
{
// Construction
public:
	bool IsDBCSString( const char* string );
	void FolderRecurse( const char* foldername, bool b_test=false );
	bool InsertProcess( const char* filename );
	bool Repacking( int version );
	void RepackingHistory();
	CSettingDlg(int version ,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTING };
	CEdit	m_PathEdit;
	CListBox	m_FileList;
	CProgressCtrl	m_Progress;
	UINT	m_nVersion;
	BOOL	m_bCompressOption;
	BOOL	m_bAllFileAdd;
	//}}AFX_DATA

	char m_strDefaultPath[_MAX_PATH];

	CLoginServerDlg*	m_pMain;
	CZipArchive		m_ZipArchive;
	HistoryList		m_RepackingVersionList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	afx_msg void OnAddfile();
	afx_msg void OnDeletefile();
	afx_msg void OnCompress();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPathBrowse();
	afx_msg void OnRefresh();
	afx_msg void OnKillfocusVersionEdit();
	afx_msg void OnDbcstest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

