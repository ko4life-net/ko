#pragma once

// MagicType4Set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagicType4Set recordset

class CMagicType4Set : public CRecordset {
  public:
    CMagicType4Set(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMagicType4Set)

    // Field/Param Data
    //{{AFX_FIELD(CMagicType4Set, CRecordset)
    long    m_iNum;
    CString m_Name;
    CString m_Description;
    BYTE    m_BuffType;
    BYTE    m_Radius;
    int     m_Duration;
    BYTE    m_AttackSpeed;
    BYTE    m_Speed;
    int     m_AC;
    int     m_Attack;
    int     m_MaxHP;
    BYTE    m_HitRate;
    int     m_AvoidRate;
    int     m_Str;
    int     m_Sta;
    int     m_Dex;
    int     m_Intel;
    int     m_Cha;
    int     m_FireR;
    int     m_ColdR;
    int     m_LightningR;
    int     m_MagicR;
    int     m_DiseaseR;
    int     m_PoisonR;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMagicType4Set)
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
