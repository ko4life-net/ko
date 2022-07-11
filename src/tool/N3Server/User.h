// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_)
#define AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCPSocket2.h"
#include "Define.h"

class CEbenezerDlg;

class CUser : public CIOCPSocket2  
{
public:
	void UserInOut( BYTE Type );
	void Initialize();
	void MoveProcess( char* pBuf );
	void LoginProcess( char* pBuf );
	void Parsing( int len, char* pData );
	void CloseProcess();
	CUser();
	virtual ~CUser();

	float m_curx;
	float m_curz;
	float m_cury;

	short m_RegionX;
	short m_RegionZ;

	char m_UserId[MAX_ID_SIZE];

	CEbenezerDlg* m_pMain;
};

#endif // !defined(AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_)
