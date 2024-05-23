#pragma once

// MagicType5Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagicType5Set recordset

class CMagicType5Set : public CRecordset {
  public:
    CMagicType5Set(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMagicType5Set)

    // Field/Param Data
    //{{AFX_FIELD(CMagicType5Set, CRecordset)
    long    m_iNum;
    CString m_Name;
    CString m_Description;
    BYTE    m_Type;
    BYTE    m_ExpRecover;
    int     m_NeedStone;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMagicType5Set)
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
