#pragma once

// DlgCtrlHeightScale.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCtrlHeightScale dialog

class CDlgCtrlHeightScale : public CDialog {
    // Construction
  public:
    CDlgCtrlHeightScale(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgCtrlHeightScale)
    enum {
        IDD = IDD_CONTROL_HEIGHT_SCALE
    };
    CSliderCtrl m_sldr_Scale;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgCtrlHeightScale)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgCtrlHeightScale)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
