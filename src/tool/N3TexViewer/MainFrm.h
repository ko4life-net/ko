// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3EngTool.h"

class CMainFrame : public CFrameWnd
{
public:
	CN3EngTool m_Eng;

public: // create from serialization only
	void AdjustWindowSize();
	CMainFrame();
	virtual ~CMainFrame();

	BOOL BMPCutter(LPCTSTR lpszFileName, int iWidth, int iHeight, bool bSaveToDXT = false, D3DFORMAT fmtDXT = D3DFMT_DXT1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	DECLARE_DYNCREATE(CMainFrame)

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAdjustWindowSize();
	afx_msg void OnFileConvert();
	afx_msg void OnToolConvertFilesAutomaticaly();
	afx_msg void OnToolConvertFilesManually();
	afx_msg void OnToolCutBmp();
	afx_msg void OnFileOpenNext();
	afx_msg void OnFileOpenPrev();
	afx_msg void OnFileOpenFirst();
	afx_msg void OnFileOpenLast();
	afx_msg void OnToolSaveRepeat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

