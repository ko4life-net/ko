// N3Indoor.h : main header file for the N3INDOOR application
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h" // main symbols

#pragma warning(disable : 4786)
#include <string>

#define GAPP ((CN3IndoorApp *)AfxGetApp)

/////////////////////////////////////////////////////////////////////////////
// CN3IndoorApp:
// See N3Indoor.cpp for the implementation of this class
//

class CN3IndoorApp : public CWinApp {
  public:
    bool m_bActive;

    void Write(const char * lpszFormat, ...);

    CN3IndoorApp();

    fs::path GetMRU(size_t iIndex);
    void     UpdateMRU();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CN3IndoorApp)
  public:
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG lCount);
    virtual int  Run();
    //}}AFX_VIRTUAL

    // Implementation
    //{{AFX_MSG(CN3IndoorApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
