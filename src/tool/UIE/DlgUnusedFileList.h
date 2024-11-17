#pragma once

// DlgUnusedFileList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUnusedFileList dialog

class CDlgUnusedFileList : public CDialog {
  public:
    std::vector<fs::path> m_vFiles;
    // Construction
  public:
    CDlgUnusedFileList(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgUnusedFileList)
    enum {
        IDD = IDD_UNUSED_FILE
    };
    CListBox m_ListFiles;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgUnusedFileList)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgUnusedFileList)
    afx_msg void OnBDeleteSelect();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
