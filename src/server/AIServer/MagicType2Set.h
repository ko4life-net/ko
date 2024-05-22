#pragma once

// MagicType2Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagicType2Set recordset
class CServerDlg;
class CMagicType2Set : public CRecordset {
  public:
    CMagicType2Set(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMagicType2Set)

    void Initialize();

    CServerDlg * m_pMain;

    // Field/Param Data
    //{{AFX_FIELD(CMagicType2Set, CRecordset)
    long    m_iNum;
    CString m_Name;
    CString m_Description;
    BYTE    m_HitType;
    int     m_HitRate;
    int     m_AddDamage;
    int     m_AddRange;
    BYTE    m_NeedArrow;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMagicType2Set)
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
