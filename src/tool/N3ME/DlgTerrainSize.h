#pragma once

// DlgTerrainSize.h : header file
//

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainSize dialog

class CDlgTerrainSize : public CDialog {
    // Construction
  public:
    CDlgTerrainSize(CWnd * pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTerrainSize)
    enum {
        IDD = IDD_TERRAINSIZE
    };
    float m_fSize;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTerrainSize)
  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgTerrainSize)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
