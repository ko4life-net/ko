#pragma once

// MakeGradeItemTableSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeGradeItemTableSet recordset
class CServerDlg;
class CMakeGradeItemTableSet : public CRecordset {
  public:
    CMakeGradeItemTableSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMakeGradeItemTableSet)

    void Initialize();

    CServerDlg * m_pMain;

    // Field/Param Data
    //{{AFX_FIELD(CMakeGradeItemTableSet, CRecordset)
    BYTE m_byItemIndex;
    int  m_byGrade_1;
    int  m_byGrade_2;
    int  m_byGrade_3;
    int  m_byGrade_4;
    int  m_byGrade_5;
    int  m_byGrade_6;
    int  m_byGrade_7;
    int  m_byGrade_8;
    int  m_byGrade_9;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMakeGradeItemTableSet)
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
