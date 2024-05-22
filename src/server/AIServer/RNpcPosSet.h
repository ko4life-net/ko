#pragma once

// RNpcPosSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRNpcPosSet recordset
class CServerDlg;
class CRNpcPosSet : public CRecordset {
  public:
    CRNpcPosSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CRNpcPosSet)

    void Initialize();

    CServerDlg * m_pMain;

    // Field/Param Data
    //{{AFX_FIELD(CRNpcPosSet, CRecordset)
    int     m_ZoneID;
    long    m_NpcID;
    BYTE    m_ActType;
    BYTE    m_RegenType;
    BYTE    m_DungeonFamily;
    BYTE    m_SpecialType;
    BYTE    m_TrapNumber;
    long    m_LeftX;
    long    m_TopZ;
    long    m_RightX;
    long    m_BottomZ;
    long    m_LimitMinX;
    long    m_LimitMinZ;
    long    m_LimitMaxX;
    long    m_LimitMaxZ;
    BYTE    m_NumNPC;
    int     m_RegTime;
    BYTE    m_DotCnt;
    CString m_path;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRNpcPosSet)
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
