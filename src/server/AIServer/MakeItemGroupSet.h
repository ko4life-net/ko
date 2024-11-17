#pragma once

// MakeItemGroupSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeItemGroupSet recordset
class CMakeItemGroupSet : public CRecordset {
  public:
    CMakeItemGroupSet(CDatabase * pDatabase = NULL);
    DECLARE_DYNAMIC(CMakeItemGroupSet)

    int  m_iItemGroupNum;
    long m_iItem[30];
    int  m_sPersent[30];

    virtual CString GetDefaultConnect();
    virtual CString GetDefaultSQL();
    virtual void    DoFieldExchange(CFieldExchange * pFX);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif
};
