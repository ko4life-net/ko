#pragma once
// StartPositionSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartPositionSet recordset

class CStartPositionSet : public CRecordset {
  public:
    CStartPositionSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CStartPositionSet)

    // Field/Param Data
    //{{AFX_FIELD(CStartPositionSet, CRecordset)
    long m_ZoneID;
    long m_sKarusX;
    long m_sKarusZ;
    long m_sElmoradX;
    long m_sElmoradZ;
    BYTE m_bRangeX;
    BYTE m_bRangeZ;
    long m_sKarusGateX;
    long m_sKarusGateZ;
    long m_sElmoGateX;
    long m_sElmoGateZ;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStartPositionSet)
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