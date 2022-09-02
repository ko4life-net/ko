#pragma once

// MagicType8Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagicType8Set recordset

class CMagicType8Set : public CRecordset {
  public:
    CMagicType8Set(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMagicType8Set)

    // Field/Param Data
    //{{AFX_FIELD(CMagicType8Set, CRecordset)
    long    m_iNum;
    CString m_Name;
    CString m_Description;
    BYTE    m_Target;
    int     m_Radius;
    BYTE    m_WarpType;
    int     m_ExpRecover;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMagicType8Set)
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
