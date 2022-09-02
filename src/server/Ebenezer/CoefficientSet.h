#pragma once

// CoefficientSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoefficientSet recordset

class CCoefficientSet : public CRecordset {
  public:
    CCoefficientSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CCoefficientSet)

    // Field/Param Data
    //{{AFX_FIELD(CCoefficientSet, CRecordset)
    int    m_sClass;
    double m_ShortSword;
    double m_Sword;
    double m_Axe;
    double m_Club;
    double m_Spear;
    double m_Pole;
    double m_Staff;
    double m_Bow;
    double m_Hp;
    double m_Mp;
    double m_Sp;
    double m_Ac;
    double m_Hitrate;
    double m_Evasionrate;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoefficientSet)
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
