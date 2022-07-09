// RscTablesDlg.h : header file
//

#if !defined(AFX_RSCTABLESDLG_H__27563D04_544E_4500_AD52_DBFCA32FBF7E__INCLUDED_)
#define AFX_RSCTABLESDLG_H__27563D04_544E_4500_AD52_DBFCA32FBF7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Common Control\PropertyList.h"
#include "RscTablesDef.h"
#include "TableGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CRscTablesDlg dialog
class CRscTablesDlg : public CDialog
{
// Construction
public:
	CRscTablesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRscTablesDlg)
	enum { IDD = IDD_RSCTABLES_DIALOG };
	CListBox	m_ListTableExt;
	CPropertyList	m_ListBoxDataType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRscTablesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CTableGenerator m_Generator;

	// Generated message map functions
	//{{AFX_MSG(CRscTablesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveEnum();
	afx_msg void OnUpdateFileSaveEnum(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenEnum();
	afx_msg void OnFileExit();
	afx_msg void OnConvertText2bin();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnBOpenTableBasic();
	afx_msg void OnBOpenTableExtEnum();
	afx_msg void OnDblclkListTableExt();
	afx_msg void OnBGenerateSelected();
	afx_msg void OnBGenerateAll();
	afx_msg void OnItemFileListLoad();
	afx_msg void OnItemFileListSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool BrowseDataEnumAndTxt(int iIndex, BOOL bOpen, std::string* pszFN_Enm, std::string* pszFN_Txt);
	void UpdateGenerationInfo();

	void UpdateAllInfo();
	void UpdateInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSCTABLESDLG_H__27563D04_544E_4500_AD52_DBFCA32FBF7E__INCLUDED_)
