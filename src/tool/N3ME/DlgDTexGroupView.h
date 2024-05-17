#pragma once

// DlgDTexGroupView.h : header file
//
class CDTexGroupMng;
class CDTexGroup;
class CN3Texture;
/////////////////////////////////////////////////////////////////////////////
// CDlgDTexGroupView dialog

class CDlgDTexGroupView : public CDialog {
    // Construction
  public:
    float        m_fTexSurfaceSize;
    CN3Texture * m_pPrevTex;
    RECT         m_PrevTexRect;

    int m_iRadioState; // 0 Automatic photo 2 Forced photo 3 Forced full tile photo

  public:
    CDTexGroupMng * m_pDTexGroupMng;

    void SetGroupMng(CDTexGroupMng * pDTexGroupMng) { m_pDTexGroupMng = pDTexGroupMng; }
    void AddGroup(CDTexGroup * pGroup);
    void ResetAll();

    CDlgDTexGroupView(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgDTexGroupView)
    enum {
        IDD = IDD_DTEX_GROUP
    };
    CSliderCtrl m_SliderBrushSize;
    CTreeCtrl   m_Tree;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgDTexGroupView)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgDTexGroupView)
    afx_msg void OnSelchangedTreeDtexGroup(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnTileauto();
    afx_msg void OnTilemanuel();
    afx_msg void OnTilemanuelNomix();
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
