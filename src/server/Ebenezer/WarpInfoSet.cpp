// WarpInfoSet.cpp : implementation file
//

#include "StdAfx.h"
#include "WarpInfoSet.h"
#include "EbenezerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarpInfoSet

IMPLEMENT_DYNAMIC(CWarpInfoSet, CRecordset)

CWarpInfoSet::CWarpInfoSet(CDatabase * pdb /*=nullptr*/)
    : CRecordset(pdb) {
    //{{AFX_FIELD_INIT(CWarpInfoSet)
    m_iZoneNo = 0;
    m_sWarpID = 0;
    m_strWarpName = _T("");
    m_strAnnounce = _T("");
    m_dwPay = 0;
    m_sZone = 0;
    m_fX = 0.0;
    m_fY = 0.0;
    m_fZ = 0.0;
    m_fR = 0.0;
    m_nFields = 10; // Number of fields mapped in DoFieldExchange
    //}}AFX_FIELD_INIT
    m_nDefaultType = snapshot;
}

CString CWarpInfoSet::GetDefaultConnect() {
    return CEbenezerDlg::GetInstance()->GetGameDBConnectionString();
}

CString CWarpInfoSet::GetDefaultSQL() {
    return _T("[dbo].[K_WARP_INFO]");
}

void CWarpInfoSet::DoFieldExchange(CFieldExchange * pFX) {
    //{{AFX_FIELD_MAP(CWarpInfoSet)
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Int(pFX, _T("ZoneNo"), m_iZoneNo);
    RFX_Int(pFX, _T("strWarpID"), m_sWarpID);
    RFX_Text(pFX, _T("strWarpName"), m_strWarpName);
    RFX_Text(pFX, _T("strAnnounce"), m_strAnnounce);
    RFX_Long(pFX, _T("dwPay"), m_dwPay);
    RFX_Int(pFX, _T("sZone"), m_sZone);
    RFX_Double(pFX, _T("fX"), m_fX);
    RFX_Double(pFX, _T("fY"), m_fY);
    RFX_Double(pFX, _T("fZ"), m_fZ);
    RFX_Double(pFX, _T("fR"), m_fR);
    //}}AFX_FIELD_MAP
}

#ifdef _DEBUG
void CWarpInfoSet::AssertValid() const {
    CRecordset::AssertValid();
}

void CWarpInfoSet::Dump(CDumpContext & dc) const {
    CRecordset::Dump(dc);
}
#endif

BOOL CWarpInfoSet::OpenByZoneId(int iZoneNo) {
    CString strSQL;
    strSQL.Format(
        _T("SELECT k.ZoneNo, k.strWarpID, k.strWarpName, k.strAnnounce, k.dwPay, k.sZone, k.fX, k.fY, k.fZ, k.fR ")
        _T("FROM [dbo].[K_WARP_INFO] k ")
        _T("INNER JOIN ZONE_INFO zi ON zi.ZoneNo = k.sZone ")
        _T("WHERE k.ZoneNo = %d"),
        iZoneNo);

    if (!Open(m_nDefaultType, strSQL, CRecordset::readOnly)) {
        AfxMessageBox(_T("CWarpInfoSet::OpenByZoneId failed to open recordset"));
        return FALSE;
    }
    return TRUE;
}
