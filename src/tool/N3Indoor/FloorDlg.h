#pragma once

// FloorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloorDlg dialog

class CFloorDlg : public CDialog {
    int m_iCurFloor;

    // Construction
  public:
    CFloorDlg(CWnd * pParent = NULL); // standard constructor
    int GetFloor();

    // Dialog Data
    //{{AFX_DATA(CFloorDlg)
    enum {
        IDD = IDD_FLOORSET
    };
    CEdit m_NumFloor;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFloorDlg)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CFloorDlg)
    afx_msg void OnChangeFloorNumber();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
