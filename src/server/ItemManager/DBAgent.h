// DBAgent.h: interface for the CDBAgent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "Define.h"
#include <vector>

class CItemManagerDlg;
class CDBAgent {
  public:
    void ReConnectODBC(CDatabase * m_db, char * strdb, char * strname, char * strpwd);
    BOOL DatabaseInit();
    CDBAgent();
    virtual ~CDBAgent();

    CDatabase m_GameDB;

    CItemManagerDlg * m_pMain;
};
