#pragma once

// ItemLevelTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemLevelTableSet recordset

class CItemLevelTableSet : public CRecordset {
  public:
    CItemLevelTableSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CItemLevelTableSet)

    // Field/Param Data
    //{{AFX_FIELD(CItemLevelTableSet, CRecordset)
    int  m_Effect;
    int  m_Swd;
    int  m_Spe;
    int  m_Blt;
    int  m_Bow;
    int  m_Thr;
    int  m_Sta;
    int  m_Amr;
    int  m_Glv;
    int  m_Boot;
    int  m_Shld;
    int  m_Hlm;
    int  m_Gaiter;
    int  m_Wrist;
    int  m_Str;
    int  m_Dex;
    int  m_Con;
    int  m_Wis;
    int  m_Agi;
    long m_Hpmax;
    long m_Mpmax;
    long m_Price;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CItemLevelTableSet)
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
