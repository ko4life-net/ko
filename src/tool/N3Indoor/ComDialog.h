#pragma once

// ComDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComDialog dialog

class CComDialog : public CDialog {
    // Construction
  public:
    CComDialog(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CComDialog)
    enum {
        IDD = IDD_COMPUTING
    };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CComDialog)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CComDialog)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
