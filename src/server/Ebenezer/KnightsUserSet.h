#pragma once

// KnightsUserSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKnightsUserSet recordset

class CKnightsUserSet : public CRecordset {
  public:
    CKnightsUserSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CKnightsUserSet)

    // Field/Param Data
    //{{AFX_FIELD(CKnightsUserSet, CRecordset)
    int     m_sIDNum;
    CString m_strUserID;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CKnightsUserSet)
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
