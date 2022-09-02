#pragma once

// DlgDelDTex.h : header file
//

class CDlgSetDTex;

/////////////////////////////////////////////////////////////////////////////
// CDlgDelDTex dialog

class CDlgDelDTex : public CDialog {
    // Construction
  public:
    CString m_SelDTexName;
    CDlgDelDTex(CWnd * pParent = NULL); // standard constructor

    CDlgSetDTex * m_pParent;

    void SetFileList(CComboBox * pCB);

    // Dialog Data
    //{{AFX_DATA(CDlgDelDTex)
    enum {
        IDD = IDD_DELDTEX
    };
    CListBox m_TexList;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgDelDTex)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgDelDTex)
    afx_msg void OnSelchangeListDeldtex();
    afx_msg void OnDblclkListDeldtex();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
