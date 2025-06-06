// WarpInfoSet.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWarpInfoSet recordset

class CWarpInfoSet : public CRecordset {
  public:
    CWarpInfoSet(CDatabase * pDatabase = nullptr);
    DECLARE_DYNAMIC(CWarpInfoSet)

    // Field/Param Data
    //{{AFX_FIELD(CWarpInfoSet, CRecordset)
    int     m_iZoneNo;
    int     m_sWarpID;
    CString m_strWarpName;
    CString m_strAnnounce;
    long    m_dwPay;
    int     m_sZone;
    double  m_fX;
    double  m_fY;
    double  m_fZ;
    double  m_fR;
    //}}AFX_FIELD

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWarpInfoSet)
  public:
    virtual CString GetDefaultConnect();
    virtual CString GetDefaultSQL();
    virtual void    DoFieldExchange(CFieldExchange * pFX);
    //}}AFX_VIRTUAL

    BOOL OpenByZoneId(int iZoneNo);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif
};
