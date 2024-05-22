#pragma once

// NpcItemSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNpcItemSet recordset
class CServerDlg;
class CNpcItemSet : public CRecordset {
  public:
    CNpcItemSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CNpcItemSet)

    void Initialize();

    CServerDlg * m_pMain;

    // Field/Param Data
    //{{AFX_FIELD(CNpcItemSet, CRecordset)
    int  m_sIndex;
    long m_iItem01;
    int  m_sPersent01;
    long m_iItem02;
    int  m_sPersent02;
    long m_iItem03;
    int  m_sPersent03;
    long m_iItem04;
    int  m_sPersent04;
    long m_iItem05;
    int  m_sPersent05;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNpcItemSet)
  public:
    virtual CString GetDefaultConnect();                   // Default connection string
    virtual CString GetDefaultSQL();                       // Default SQL for Recordset
    virtual void    DoFieldExchange(CFieldExchange * pFX); // RFX support
                                                           //}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
