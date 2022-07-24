// Knights.cpp: implementation of the CKnights class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ebenezer.h"
#include "Knights.h"
#include "User.h"
#include "GameDefine.h"
#include "PacketDefine.h"
#include "EbenezerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKnights::CKnights()
{
	//m_pMain = NULL;
	
/*	CString strConnect;
	strConnect.Format (_T("ODBC;DSN=%s;UID=%s;PWD=%s"), "KN_Online", "knight", "knight");
	m_KnightsDB.SetLoginTimeout (10);
	if( !m_KnightsDB.Open(NULL,FALSE,FALSE,strConnect) )
		AfxMessageBox("KnightsDB Connection Fail...");	*/
}

CKnights::~CKnights()
{

}

void CKnights::InitializeValue()
{
	m_sIndex = 0;
	m_byFlag = 0;			// 1 : Clan, 2 : 기사단
	m_byNation = 0;		// nation
	m_byGrade = 0;			// clan 등급 (1 ~ 5등급)
	m_byRanking = 0;		// clan 등급 (1 ~ 5등)
	m_sMembers = 0;
	memset( m_strName, 0x00, MAX_ID_SIZE+1 );
	memset( m_strChief, 0x00, MAX_ID_SIZE+1 );
	memset( m_strViceChief_1, 0x00, MAX_ID_SIZE+1 );	// 부단장 1
	memset( m_strViceChief_2, 0x00, MAX_ID_SIZE+1 );	// 부단장 2 (기사단에서는 장교)
	memset( m_strViceChief_3, 0x00, MAX_ID_SIZE+1 ); // 부단장 3	(기사단에서는 사용하지 않음)
	memset( m_Image, 0x00, MAX_KNIGHTS_MARK );
	m_nMoney = 0;
	m_sDomination = 0;
	m_nPoints = 0;
	
	for( int i=0; i<MAX_CLAN; i++ )	{
		m_arKnightsUser[i].byUsed = 0;
		memset( m_arKnightsUser[i].strUserName, 0x00, MAX_ID_SIZE+1);
	}

	for(int i=0; i<MAX_KNIGHTS_BANK; i++ )	{
		m_StashItem[i].nNum = 0;
		m_StashItem[i].sCount = 0;
		m_StashItem[i].sDuration = 0;
	}
}
