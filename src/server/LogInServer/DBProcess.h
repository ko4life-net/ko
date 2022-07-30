// DBProcess.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CVersionManagerDlg;
class CDBProcess  
{
public:
	BOOL IsCurrentUser( const char* accountid, char* strServerIP, int &serverno );
	void ReConnectODBC(CDatabase *m_db, const char *strdb, const char *strname, const char *strpwd);
	BOOL DeleteVersion( const char* filename );
	BOOL InsertVersion( int version, const char* filename, const char* compname, int historyversion );
	BOOL InitDatabase( char* strconnection );
	int MgameLogin( const char* id, const char* pwd );
	int AccountLogin( const char* id, const char* pwd );
	BOOL LoadVersionList();
	BOOL LoadUserCountList();

	CDBProcess();
	virtual ~CDBProcess();

	CDatabase	m_VersionDB;
	CVersionManagerDlg* m_pMain;
};

