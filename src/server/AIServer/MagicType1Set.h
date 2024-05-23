#pragma once

// MagicType1Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagicType1Set recordset

class CMagicType1Set : public CRecordset {
  public:
    CMagicType1Set(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMagicType1Set)

    // Field/Param Data
    //{{AFX_FIELD(CMagicType1Set, CRecordset)
    long    m_iNum;
    CString m_Name;
    CString m_Description;
    BYTE    m_Type;
    int     m_HitRate;
    int     m_Hit;
    BYTE    m_Delay;
    BYTE    m_ComboType;
    BYTE    m_ComboCount;
    int     m_ComboDamage;
    int     m_Range;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMagicType1Set)
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
