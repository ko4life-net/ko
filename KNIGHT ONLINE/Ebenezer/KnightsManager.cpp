// KnightsManager.cpp: implementation of the CKnightsManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ebenezer.h"
#include "KnightsManager.h"
//#include "Knights.h"
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

CKnightsManager::CKnightsManager()
{
	m_pMain = NULL;
	
/*	CString strConnect;
	strConnect.Format (_T("ODBC;DSN=%s;UID=%s;PWD=%s"), "KN_Online", "knight", "knight");
	m_KnightsDB.SetLoginTimeout (10);
	if( !m_KnightsDB.Open(NULL,FALSE,FALSE,strConnect) )
		AfxMessageBox("KnightsDB Connection Fail...");	*/
}

CKnightsManager::~CKnightsManager()
{

}

void CKnightsManager::PacketProcess(CUser *pUser, char *pBuf)
{
	int index = 0;

	BYTE command = GetByte( pBuf, index );
	
	if( !pUser ) return;

	switch( command ) {
	case KNIGHTS_CREATE:
		CreateKnights( pUser, pBuf+index );
		break;
	case KNIGHTS_JOIN:
		JoinKnights( pUser, pBuf+index );
		break;
	case KNIGHTS_WITHDRAW:
		WithdrawKnights( pUser, pBuf+index );
		break;
	case KNIGHTS_REMOVE:
	case KNIGHTS_ADMIT:
	case KNIGHTS_REJECT:
	case KNIGHTS_CHIEF:
	case KNIGHTS_VICECHIEF:
	case KNIGHTS_OFFICER:
	case KNIGHTS_PUNISH:
		ModifyKnightsMember( pUser, pBuf+index, command );
		break;
	case KNIGHTS_DESTROY:
		DestroyKnights( pUser );
		break;
	case KNIGHTS_ALLLIST_REQ:
		AllKnightsList( pUser, pBuf+index );
		break;
	case KNIGHTS_MEMBER_REQ:
		AllKnightsMember( pUser, pBuf+index );
		break;
	case KNIGHTS_CURRENT_REQ:
		CurrentKnightsMember( pUser, pBuf+index );
		break;
	case KNIGHTS_STASH:
		break;
	case KNIGHTS_JOIN_REQ:
		JoinKnightsReq( pUser, pBuf+index );
		break;
	}
}

void CKnightsManager::CreateKnights(CUser* pUser, char *pBuf)
{
	int index = 0, send_index = 0, idlen = 0, knightindex = 0, ret_value = 3, week = 0;
	char idname[MAX_ID_SIZE+1]; memset( idname, 0x00, MAX_ID_SIZE+1 );
	CTime time = CTime::GetCurrentTime();

	char send_buff[256]; memset( send_buff, 0x00, 256 );

	if( !pUser ) return;

	idlen = GetShort( pBuf, index );
	if( idlen > MAX_ID_SIZE || idlen < 0 ) goto fail_return;
	GetString( idname, pBuf, idlen, index );

	if( !IsAvailableName( idname ) ) goto fail_return;
	if( pUser->m_pUserData->m_bKnights != 0 ) {			// 기사단에 가입되어 있습니다
		ret_value = 5;
		goto fail_return;
	}

	if( m_pMain->m_nServerGroup == 2 )	{				// 기사단은 서버 1군에서만 만들 수 있도록...
		ret_value = 8;
		goto fail_return;
	}
	// 요일 체크
	week = time.GetDayOfWeek();
/*	if(week == 1 || week == 6 || week == 7)	{}
	else {
		ret_value = 7;
		goto fail_return;
	}	*/
	if( pUser->m_pUserData->m_bLevel < 20 ) {			// 20 Level 이상이 필요
		ret_value = 2;
		goto fail_return;
	}
/*	if( pUser->m_pUserData->m_iLoyalty < 800 ) {		// 국가기여도 800 이상이 필요
		goto fail_return;
	}
	if( pUser->m_pUserData->m_bstrSkill[ORDER_SKILL] < 10 || pUser->m_pUserData->m_bstrSkill[MANNER_SKILL] < 5 ) {	// 지휘스킬 10, 예절스킬 5 이상이 필요
		goto fail_return;
	}
	if( pUser->m_pUserData->m_bCha < 120 ) {			// 매력 120 이상이 필요
		goto fail_return;
	}	*/
	if( pUser->m_pUserData->m_iGold < 500000 ) {		// 5000000노아 이상이 필요
		ret_value = 4;
		goto fail_return;
	}	

	knightindex = GetKnightsIndex( pUser->m_pUserData->m_bNation );
	//knightindex = 0;
	if( knightindex == -1 ) {							// 기사단 창설에 실패
		ret_value = 6;
		goto fail_return;
	}	

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CREATE+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetByte( send_buff, CLAN_TYPE, send_index );
	SetShort( send_buff, knightindex, send_index );
	SetByte( send_buff, pUser->m_pUserData->m_bNation, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, idname, idlen, send_index );
	SetShort( send_buff, strlen(pUser->m_pUserData->m_id), send_index );
	SetString( send_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), send_index );
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CREATE, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## CreateKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	
	pUser->Send( send_buff, send_index );
}

BOOL CKnightsManager::IsAvailableName( const char *strname)
{
	CKnights* pKnights;
	map < int, CKnights* >::iterator		Iter1, Iter2;
	
	Iter1 = m_pMain->m_KnightsArray.m_UserTypeMap .begin();
	Iter2 = m_pMain->m_KnightsArray.m_UserTypeMap.end();

	for( ; Iter1 != Iter2; Iter1++ ) {
		pKnights = (*Iter1).second;
		if( _strnicmp( pKnights->m_strName, strname, MAX_ID_SIZE ) == 0 )
			return FALSE;
	}

	return TRUE;
}

int CKnightsManager::GetKnightsIndex( int nation )
{
	//TRACE("GetKnightsIndex = nation=%d\n", nation);
	int knightindex = 0;
	// sungyong tw~
	//if( m_pMain->m_nServerNo == ELMORAD )	knightindex = 15000;
	if( nation == ELMORAD )	knightindex = 15000;
	// ~sungyong tw

	map < int, CKnights* >::iterator		Iter1, Iter2;
	
	Iter1 = m_pMain->m_KnightsArray.m_UserTypeMap.begin();
	Iter2 = m_pMain->m_KnightsArray.m_UserTypeMap.end();

	for( ; Iter1 != Iter2; Iter1++ ) {
		if( knightindex < ((*Iter1).second)->m_sIndex )	{
			if( nation == KARUS )	{							// sungyong,, 카루스와 전쟁존의 합침으로 인해서,,,
				if( ((*Iter1).second)->m_sIndex >= 15000 )	continue;
			}
			knightindex = ((*Iter1).second)->m_sIndex;
		}
	}

	knightindex++;
	if( nation == KARUS )	{
		if( knightindex >= 15000 || knightindex < 0 )	return -1;
	}
	else if( nation == ELMORAD )	{
		if( knightindex < 15000 || knightindex > 30000 )	return -1;
	}

	if( m_pMain->m_KnightsArray.GetData( knightindex ) )	// 확인 사살..
		return -1;

	return knightindex;
}

void CKnightsManager::JoinKnights(CUser *pUser, char *pBuf)
{
	int knightsindex = 0, index = 0, send_index = 0, ret_value = 0, member_id = 0, community = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	CUser* pTUser = NULL;
	CKnights* pKnights = NULL;

	if( !pUser ) return;

	if( pUser->m_pUserData->m_bZone > 2 )	{	// 전쟁존에서는 기사단 처리가 안됨
		ret_value = 12;
		goto fail_return;
	}

	if( pUser->m_pUserData->m_bFame != CHIEF && pUser->m_pUserData->m_bFame != VICECHIEF)	{
		ret_value = 6;
		goto fail_return;
	}

	knightsindex = pUser->m_pUserData->m_bKnights;
	pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );
	if( !pKnights )	{
		ret_value = 7;
		goto fail_return;
	}
	// 인원 체크
/*	if(pKnights->sMembers >= 24)	{
		ret_value = 8;
		goto fail_return;
	}	*/

	//community = GetByte( pBuf, index );
	member_id = GetShort( pBuf, index );
	if( member_id < 0 || member_id >= MAX_USER)	{
		ret_value = 2;
		goto fail_return;
	}
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[member_id];
	if( !pTUser )	{
		ret_value = 2;
		goto fail_return;
	}
	if( pTUser->m_bResHpType == USER_DEAD)	{
		ret_value = 3;
		goto fail_return;
	}
	if( pTUser->m_pUserData->m_bNation != pUser->m_pUserData->m_bNation ) {
		ret_value = 4;
		goto fail_return;
	}

	if( pTUser->m_pUserData->m_bKnights > 0 ) {		// 기사단에 가입되어 있습니다
		ret_value = 5;
		goto fail_return;
	}

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_JOIN_REQ, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, knightsindex, send_index );
	SetShort( send_buff, strlen( pKnights->m_strName ), send_index );
	SetString( send_buff, pKnights->m_strName, strlen( pKnights->m_strName ), send_index );
	pTUser->Send( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_JOIN, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## JoinKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::JoinKnightsReq(CUser *pUser, char *pBuf)
{
	int knightsindex = 0, index = 0, send_index = 0, ret_value = 0, member_id = 0, community = 0, flag = 0, sid = -1;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	CUser* pTUser = NULL;
	CKnights* pKnights = NULL;

	if( !pUser ) return;

	flag = GetByte( pBuf, index );
	sid = GetShort( pBuf, index );
	if( sid < 0 || sid >= MAX_USER)	{
		ret_value = 2;
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_JOIN, send_index );
		SetByte( send_buff, ret_value, send_index );
		//TRACE("## JoinKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pTUser->GetSocketID(), pTUser->m_pUserData->m_id, ret_value);
		pTUser->Send( send_buff, send_index );
		return;
	}
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sid];
	if( !pTUser )	{
		ret_value = 2;
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_JOIN, send_index );
		SetByte( send_buff, ret_value, send_index );
		//TRACE("## JoinKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
		pUser->Send( send_buff, send_index );
		return;
	}

	if( flag == 0x00 )		{	// 거절
		ret_value = 11;
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_JOIN, send_index );
		SetByte( send_buff, ret_value, send_index );
		//TRACE("## JoinKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pTUser->GetSocketID(), pTUser->m_pUserData->m_id, ret_value);
		pTUser->Send( send_buff, send_index );
		return;
	}

	knightsindex = GetShort( pBuf, index );
	pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );
	if( !pKnights )	{
		ret_value = 7;
		goto fail_return;
	}
	// 인원 체크
/*	if(pKnights->sMembers >= 24)	{
		ret_value = 8;
		goto fail_return;
	}	*/

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_JOIN+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, knightsindex, send_index );
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_JOIN, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## JoinKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	pUser->Send( send_buff, send_index );
}


void CKnightsManager::WithdrawKnights(CUser *pUser, char *pBuf)
{
	int index = 0, send_index = 0, ret_value = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	CKnights* pKnights = NULL;

	if( !pUser ) return;
	if( pUser->m_pUserData->m_bKnights < 1 ||  pUser->m_pUserData->m_bKnights > 30000) {	// 기사단에 가입되어 있지 않습니다
		ret_value = 10;
		goto fail_return;
	}

/*	pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights );
	if( !pKnights ) {
//		sprintf(errormsg, "존재하지 않는 기사단입니다.");
		//::_LoadStringFromResource(IDP_KNIGHT_NOT_AVAILABLE, buff);
		//sprintf(errormsg, buff.c_str());
		ret_value = 3;
		goto fail_return;
	}	*/

	if( pUser->m_pUserData->m_bZone > 2 )	{	// 전쟁존에서는 기사단 처리가 안됨
		ret_value = 12;
		goto fail_return;
	}

	if( pUser->m_pUserData->m_bFame == CHIEF)	{		// 단장이 탈퇴할 경우에는 클랜 파괴
		if( pUser->m_pUserData->m_bZone > 2 )	{		// 전쟁존에서는 클랜을 파괴할 수 없다
			ret_value = 12;
			goto fail_return;
		}

		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_DESTROY+0x10, send_index );
		SetShort( send_buff, pUser->GetSocketID(), send_index );
		SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
		m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
		return;
	}

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_WITHDRAW+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_WITHDRAW, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## WithDrawKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::DestroyKnights( CUser* pUser )
{
	int index = 0, send_index = 0, ret_value = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );

	if( !pUser ) return;
	if( pUser->m_pUserData->m_bFame != CHIEF ) goto fail_return;

	if( pUser->m_pUserData->m_bZone > 2 )	{
		ret_value = 12;
		goto fail_return;
	}

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_DESTROY+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );

fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_DESTROY, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## DestoryKnights Fail - nid=%d, name=%s, error_code=%d ##\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::ModifyKnightsMember(CUser *pUser, char *pBuf, BYTE command )
{
	int index = 0, send_index = 0, idlen = 0, ret_value = 0, vicechief = 0, remove_flag = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char userid[MAX_ID_SIZE+1]; memset( userid, 0x00, MAX_ID_SIZE+1 );
	CUser* pTUser = NULL;
	
	if( !pUser ) return;
	idlen = GetShort( pBuf, index );
	if( idlen > MAX_ID_SIZE || idlen <= 0 ) {	// 잘못된 아이디
		ret_value = 2;
		goto fail_return;
	}
	GetString( userid, pBuf, idlen, index );

	if( pUser->m_pUserData->m_bZone > 2 )	{	// 전쟁존에서는 기사단 처리가 안됨
		ret_value = 12;
		goto fail_return;
	}

	if( _strnicmp( userid, pUser->m_pUserData->m_id, MAX_ID_SIZE ) == 0 ) {	// 자신은 할 수 없습니다
		ret_value = 9;
		goto fail_return;
	}

	if( command == KNIGHTS_ADMIT || command == KNIGHTS_REJECT ) {	// 기사단, 멤버가입 및 멤버거절, 장교 이상이 할 수 있습니다
		if( pUser->m_pUserData->m_bFame < OFFICER ) {
			goto fail_return;
		}
	}
	else if( command == KNIGHTS_PUNISH ){							// 징계, 부기사단장 이상이 할 수 있습니다
		if( pUser->m_pUserData->m_bFame < VICECHIEF ) {
			goto fail_return;
		}
	}
	else {
		if( pUser->m_pUserData->m_bFame != CHIEF ) {				// 기사단장 만이 할 수 있습니다
			ret_value = 6;
			goto fail_return;
		}
	}

	pTUser = m_pMain->GetUserPtr( userid, 2);
	if( !pTUser )	{
		if( command == KNIGHTS_REMOVE )	{			// 게임상에 없는 유저 추방시 (100)
			remove_flag = 0;
			SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
			SetByte( send_buff, command+0x10, send_index );
			SetShort( send_buff, pUser->GetSocketID(), send_index );
			SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
			SetShort( send_buff, idlen, send_index );
			SetString( send_buff, userid, idlen, send_index );
			SetByte( send_buff, remove_flag, send_index );
			m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
			return;
		}
		else	{
			ret_value = 2;
			goto fail_return;
		}
	}
	
	if( pUser->m_pUserData->m_bNation != pTUser->m_pUserData->m_bNation ) {
		ret_value = 4;
		goto fail_return;
	}
	if ( pUser->m_pUserData->m_bKnights != pTUser->m_pUserData->m_bKnights )	{
		ret_value = 5;
		goto fail_return;
	}

	// 부단장이 3명이 됐는지를 판단 (클랜인 경우이다,,)
	if( command == KNIGHTS_VICECHIEF ){							// 부단장 임명
		if( pTUser->m_pUserData->m_bFame == VICECHIEF )	{		// 이미 부단장인 경우
			ret_value = 8;
			goto fail_return;
		}

		CKnights* pKnights = NULL;
		pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights );
		if( !pKnights )		{
			ret_value = 7;
			goto fail_return;
		}

	/*	if( !strcmp( pKnights->strViceChief_1, "") )	vicechief = 1;
		else if( !strcmp( pKnights->strViceChief_2, "") )	vicechief = 2;
		else if( !strcmp( pKnights->strViceChief_3, "") )	vicechief = 3;
		else {
			ret_value = 8;
			goto fail_return;
		}	*/
	}
	
	remove_flag = 1;	// 게임상에 있는 유저 추방시 (1)
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, userid, idlen, send_index );
	SetByte( send_buff, remove_flag, send_index );						
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	return;

fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command, send_index );
	SetByte( send_buff, ret_value, send_index );
	//TRACE("## ModifyKnights Fail - command=%d, nid=%d, name=%s, error_code=%d ##\n", command, pUser->GetSocketID(), pUser->m_pUserData->m_id, ret_value);
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::AllKnightsList(CUser *pUser, char* pBuf)
{
	int send_index = 0, buff_index = 0, count = 0, page = 0, index = 0, start = 0;
	char send_buff[4096]; memset( send_buff, 0x00, 4096 );
	char temp_buff[4096]; memset( temp_buff, 0x00, 4096 );
	map < int, CKnights*>::iterator Iter1, Iter2;
	CKnights* pKnights = NULL;

	if( !pUser ) return;

	page = GetShort( pBuf, index );
	start = page * 10;			// page : 0 ~

	Iter1 = m_pMain->m_KnightsArray.m_UserTypeMap.begin();
	Iter2 = m_pMain->m_KnightsArray.m_UserTypeMap.end();
	for( ; Iter1 != Iter2; Iter1++ ) {
		pKnights = (*Iter1).second;
		if( !pKnights ) continue;
		if( pKnights->m_byFlag != KNIGHTS_TYPE ) continue;		// 기사단 리스트만 받자
		if( pKnights->m_byNation != pUser->m_pUserData->m_bNation ) continue;
		if( count < start ) {
			count++;
			continue;
		}
		SetShort( temp_buff, pKnights->m_sIndex, buff_index );
		SetShort( temp_buff, strlen(pKnights->m_strName), buff_index );
		SetString( temp_buff, pKnights->m_strName, strlen(pKnights->m_strName), buff_index );
		SetShort( temp_buff, pKnights->m_sMembers, buff_index );
		SetShort( temp_buff, strlen(pKnights->m_strChief), buff_index );
		SetString( temp_buff, pKnights->m_strChief, strlen(pKnights->m_strChief), buff_index );
		SetDWORD( temp_buff, pKnights->m_nPoints, buff_index );
		count++;
		if( count >= start + 10 )
			break;
	}

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_ALLLIST_REQ, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, page, send_index );
	SetShort( send_buff, count-start, send_index );
	SetString( send_buff, temp_buff, buff_index, send_index );
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::AllKnightsMember(CUser *pUser, char* pBuf)
{
	int index = 0, send_index = 0, page = 0, ret_value = 0, temp_index = 0, count=0, pktsize = 0;
	char send_buff[4096]; memset( send_buff, 0x00, 4096 );
	char temp_buff[4096]; memset( temp_buff, 0x00, 4096 );
	CKnights* pKnights = NULL;

	if( !pUser ) return;
	if( pUser->m_pUserData->m_bKnights <= 0 ) {		// 기사단에 가입되어 있지 않습니다
		ret_value = 2;
		goto fail_return;
	}
/*	if( pUser->m_pUserData->m_bFame < OFFICER ) {
//		sprintf(errormsg, "장교 이상이 할 수 있습니다.");
		//::_LoadStringFromResource(IDP_MINIMUM_OFFICER, buff);
		//sprintf(errormsg, buff.c_str());
		ret_value = 3;
		goto fail_return;
	}		*/
	
	//page = GetShort( pBuf, index );

	pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights );
	if( !pKnights )	{
		ret_value = 7;
		goto fail_return;
	}

	// 단장 
/*	if( pUser->m_pUserData->m_bFame == CHIEF )	{
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_MEMBER_REQ+0x10, send_index );
		SetShort( send_buff, pUser->GetSocketID(), send_index );
		SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
		//SetShort( send_buff, page, send_index );
		m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
		return;
	}	*/

	// 직접.. 게임서버에서 유저정보를 참조해서 불러오는 방식 (단장이 아닌 모든 사람)
	if( pUser->m_pUserData->m_bFame == CHIEF )	{
		count = m_pMain->GetKnightsAllMembers( pUser->m_pUserData->m_bKnights, temp_buff, temp_index, 1 );
	}
	else	{
		count = m_pMain->GetKnightsAllMembers( pUser->m_pUserData->m_bKnights, temp_buff, temp_index, 0 );
	}

	pktsize = temp_index+4;
	if( count > 24 ) return;

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_MEMBER_REQ, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, pktsize, send_index );
	SetShort( send_buff, count, send_index );
	SetString( send_buff, temp_buff, temp_index, send_index );
	pUser->Send( send_buff, send_index );

/*	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_MEMBER_REQ+0x10, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
	//SetShort( send_buff, page, send_index );
	m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );	*/
	return;

fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_MEMBER_REQ, send_index );
	SetByte( send_buff, ret_value, send_index );
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::CurrentKnightsMember(CUser *pUser, char* pBuf)
{
	int index = 0, send_index = 0, buff_index = 0, count = 0, i=0, page = 0, start = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char temp_buff[4096]; memset( temp_buff, 0x00, 4096 );
	CUser* pTUser = NULL;
	CKnights* pKnights = NULL;
	char errormsg[128]; memset( errormsg, 0x00, 128 );
	std::string buff;

//	sprintf(errormsg, "기사단에 가입되지 않았습니다.");
	::_LoadStringFromResource(IDP_KNIGHT_NOT_REGISTERED, buff);
	sprintf(errormsg, buff.c_str());

	if( !pUser ) return;
	if( pUser->m_pUserData->m_bKnights <= 0 ) goto fail_return;
	pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights);
	if( !pKnights ) goto fail_return;
	
	page = GetShort( pBuf, index );
	start = page * 10;			// page : 0 ~

	for(i=0; i<MAX_USER; i++ ) {
		pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if( !pTUser ) continue;
		if( pTUser->m_pUserData->m_bKnights != pUser->m_pUserData->m_bKnights ) continue;
		if( count < start ) {
			count++;
			continue;
		}
		SetShort( temp_buff, strlen(pUser->m_pUserData->m_id), buff_index );
		SetString( temp_buff, pUser->m_pUserData->m_id, strlen( pUser->m_pUserData->m_id), buff_index );
		SetByte( temp_buff, pUser->m_pUserData->m_bFame, buff_index );
		SetByte( temp_buff, pUser->m_pUserData->m_bLevel, buff_index );
		SetShort( temp_buff, pUser->m_pUserData->m_sClass, buff_index );
		count++;
		if( count >= start + 10 )
			break;
	}

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CURRENT_REQ, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, strlen(pKnights->m_strChief), send_index );
	SetString( send_buff, pKnights->m_strChief, strlen( pKnights->m_strChief), send_index );
	SetShort( send_buff, page, send_index );
	SetShort( send_buff, count-start, send_index );
	SetString( send_buff, temp_buff, buff_index, send_index );
	pUser->Send( send_buff, send_index );

fail_return:
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CURRENT_REQ, send_index );
	SetByte( send_buff, 0x00, send_index );
	SetShort( send_buff, strlen(errormsg), send_index );
	SetString( send_buff, errormsg, strlen(errormsg), send_index );
	pUser->Send( send_buff, send_index );
}

void CKnightsManager::ReceiveKnightsProcess( CUser* pUser, char *pBuf, BYTE command)
{
	int index = 0, send_index = 0, pktsize = 0, count = 0;
	BYTE result;
	char send_buff[2048]; memset( send_buff, 0x00, 2048 );
	CUser* pTUser = NULL;
	char errormsg[128]; memset( errormsg, 0x00, 128 );
	std::string buff;

//	sprintf(errormsg, "기사단 DB처리에 실패하였습니다.");
	::_LoadStringFromResource(IDP_KNIGHT_DB_FAIL, buff);
	sprintf(errormsg, buff.c_str());

	result = GetByte( pBuf, index );

	//TRACE("ReceiveKnightsProcess - command=%d, result=%d, nid=%d, name=%s, index=%d, fame=%d\n", command, result, pUser->GetSocketID(), pUser->m_pUserData->m_id, pUser->m_pUserData->m_bKnights, pUser->m_pUserData->m_bFame);

	if( result > 0 ) {
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, command-0x10, send_index );
		//SetByte( send_buff, 0x00, send_index );
		SetByte( send_buff, result, send_index );
		SetShort( send_buff, strlen(errormsg), send_index );
		SetString( send_buff, errormsg, strlen(errormsg), send_index );
		pUser->Send( send_buff, send_index );
		return;
	}

	switch(command) {
	case KNIGHTS_CREATE+0x10:
		RecvCreateKnights( pUser, pBuf+index );
		break;
	case KNIGHTS_JOIN+0x10:
	case KNIGHTS_WITHDRAW+0x10:
		RecvJoinKnights( pUser, pBuf+index, command );
		break;
	case KNIGHTS_REMOVE+0x10:
	case KNIGHTS_ADMIT+0x10:
	case KNIGHTS_REJECT+0x10:
	case KNIGHTS_CHIEF+0x10:
	case KNIGHTS_VICECHIEF+0x10:
	case KNIGHTS_OFFICER+0x10:
	case KNIGHTS_PUNISH+0x10:
		RecvModifyFame( pUser, pBuf+index, command );
		break;
	case KNIGHTS_DESTROY+0x10:
		RecvDestroyKnights( pUser, pBuf+index );
		break;
	case KNIGHTS_MEMBER_REQ+0x10:
		{
			CKnights* pKnights = NULL;
			pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights);
			if( !pKnights ) break;
			pktsize = GetShort( pBuf, index );
			count = GetShort( pBuf, index );

			if( count > 24 ) break;

			SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
			SetByte( send_buff, KNIGHTS_MEMBER_REQ, send_index );
			SetByte( send_buff, 0x01, send_index );
			SetShort( send_buff, pktsize, send_index );
			SetShort( send_buff, count, send_index );
			//SetShort( send_buff, strlen(pKnights->strChief), send_index );
			//SetString( send_buff, pKnights->strChief, strlen( pKnights->strChief), send_index );
			//SetString( send_buff, pBuf+index, pktsize, send_index );
			SetString( send_buff, pBuf + index, pktsize, send_index );
			pUser->Send( send_buff, send_index );
		}
		break;
	case KNIGHTS_STASH+0x10:
		break;
	case KNIGHTS_LIST_REQ+0x10:
		RecvKnightsList( pBuf+index );
		break;
	}
}

void CKnightsManager::RecvCreateKnights(CUser *pUser, char *pBuf)
{
	int index = 0, send_index = 0, namelen = 0, idlen = 0, knightsindex = 0, nation = 0, community = 0, money = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char knightsname[MAX_ID_SIZE+1]; memset( knightsname, 0x00, MAX_ID_SIZE+1 );
	char chiefname[MAX_ID_SIZE+1]; memset( chiefname, 0x00, MAX_ID_SIZE+1 );
	CKnights* pKnights = NULL;

	if( !pUser ) return;

	community = GetByte( pBuf, index );
	knightsindex = GetShort( pBuf, index );
	nation = GetByte( pBuf, index );
	namelen = GetShort( pBuf, index );
	GetString( knightsname, pBuf, namelen, index );
	idlen = GetShort( pBuf, index );
	GetString( chiefname, pBuf, idlen, index );

	pKnights = new CKnights;
	pKnights->InitializeValue();

	pKnights->m_sIndex = knightsindex;
	pKnights->m_byFlag = community;
	pKnights->m_byNation = nation;
	strcpy( pKnights->m_strName, knightsname );
	strcpy( pKnights->m_strChief, chiefname );
	memset( pKnights->m_strViceChief_1, 0x00, MAX_ID_SIZE+1);
	memset( pKnights->m_strViceChief_2, 0x00, MAX_ID_SIZE+1);
	memset( pKnights->m_strViceChief_3, 0x00, MAX_ID_SIZE+1);
	pKnights->m_sMembers = 1;
	pKnights->m_nMoney = 0;
	pKnights->m_nPoints = 0;
	pKnights->m_byGrade = 5;
	pKnights->m_byRanking = 0;

	pUser->m_pUserData->m_bKnights = knightsindex;
	pUser->m_pUserData->m_bFame = CHIEF;
	money = pUser->m_pUserData->m_iGold - 500000;
	pUser->m_pUserData->m_iGold = money;

	for(int i=0; i<MAX_CLAN; i++)	{
		pKnights->m_arKnightsUser[i].byUsed = 0;
		strcpy(pKnights->m_arKnightsUser[i].strUserName, "");
	}	

	m_pMain->m_KnightsArray.PutData( pKnights->m_sIndex, pKnights );

	// 클랜정보에 추가
	AddKnightsUser( knightsindex, chiefname );

	//TRACE("RecvCreateKnights - nid=%d, name=%s, index=%d, fame=%d, money=%d\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, knightsindex, pUser->m_pUserData->m_bFame, money);

	//if( pKnights->bFlag == KNIGHTS_TYPE )	{
/*	memset( send_buff, 0x00, 128 ); send_index = 0;
		SetByte( send_buff, WIZ_KNIGHTS_LIST, send_index );
		SetByte( send_buff, 0x02, send_index );					// Insert Knights From List 
		SetShort( send_buff, knightsindex, send_index );
		SetShort( send_buff, namelen, send_index );
		SetString( send_buff, knightsname, namelen, send_index );
		m_pMain->Send_All( send_buff, send_index, pUser );	*/
	//}

	memset( send_buff, 0x00, 128 ); send_index = 0;
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CREATE, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, knightsindex, send_index );
	SetShort( send_buff, namelen, send_index );
	SetString( send_buff, knightsname, namelen, send_index );
	SetByte( send_buff, 5, send_index );  // knights grade
	SetByte( send_buff, 0, send_index );
	SetDWORD( send_buff, money, send_index );
	m_pMain->Send_Region( send_buff, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ, NULL, false );
	//pUser->Send( send_buff, send_index );

	memset( send_buff, 0x00, 128 ); send_index = 0;
	SetByte( send_buff, UDP_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_CREATE, send_index );
	SetByte( send_buff, community, send_index );
	SetShort( send_buff, knightsindex, send_index );
	SetByte( send_buff, nation, send_index );
	SetShort( send_buff, namelen, send_index );
	SetString( send_buff, knightsname, namelen, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, chiefname, idlen, send_index );
	if( m_pMain->m_nServerGroup == 0 )
		m_pMain->Send_UDP_All( send_buff, send_index );
	else
		m_pMain->Send_UDP_All( send_buff, send_index, 1 );
}

void CKnightsManager::RecvJoinKnights(CUser *pUser, char* pBuf, BYTE command)
{
	int send_index = 0, knightsindex = 0, index = 0, idlen = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char finalstr[128]; memset( finalstr, 0x00, 128 );
	CKnights*	pKnights = NULL;

	if( !pUser ) return;

	knightsindex = GetShort( pBuf, index );
	pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );

	if( command == KNIGHTS_JOIN+0x10 ) {
		pUser->m_pUserData->m_bKnights = knightsindex;
		pUser->m_pUserData->m_bFame = TRAINEE;
		sprintf( finalstr, "#### %s님이 가입하셨습니다. ####", pUser->m_pUserData->m_id );
		// 클랜정보에 추가
		AddKnightsUser( knightsindex, pUser->m_pUserData->m_id );

		//TRACE("RecvJoinKnights - 가입, nid=%d, name=%s, index=%d, fame=%d\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, pUser->m_pUserData->m_bKnights, pUser->m_pUserData->m_bFame);
	}
	else {		// 탈퇴..
		pUser->m_pUserData->m_bKnights = 0;
		pUser->m_pUserData->m_bFame = 0;

		// 클랜정보에 추가
		RemoveKnightsUser( knightsindex, pUser->m_pUserData->m_id );

	/*	if(pKnights)	{
			if( !strcmp( pKnights->strViceChief_1, pUser->m_pUserData->m_id) )
				memset( pKnights->strViceChief_1, 0x00, MAX_ID_SIZE+1 );
			else if( !strcmp( pKnights->strViceChief_2, pUser->m_pUserData->m_id) )
				memset( pKnights->strViceChief_2, 0x00, MAX_ID_SIZE+1 );
			else if( !strcmp( pKnights->strViceChief_3, pUser->m_pUserData->m_id) )
				memset( pKnights->strViceChief_3, 0x00, MAX_ID_SIZE+1 );
		}	*/
		sprintf( finalstr, "#### %s님이 탈퇴하셨습니다. ####", pUser->m_pUserData->m_id );

		//TRACE("RecvJoinKnights - 탈퇴, nid=%d, name=%s, index=%d, fame=%d\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, pUser->m_pUserData->m_bKnights, pUser->m_pUserData->m_bFame);
	}

	//TRACE("RecvJoinKnights - command=%d, nid=%d, name=%s, index=%d, fame=%d\n", command, pUser->GetSocketID(), pUser->m_pUserData->m_id, pUser->m_pUserData->m_bKnights, pUser->m_pUserData->m_bFame);

	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command-0x10, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, pUser->GetSocketID(), send_index );
	SetShort( send_buff, pUser->m_pUserData->m_bKnights, send_index );
	SetByte( send_buff, pUser->m_pUserData->m_bFame, send_index );
	if( pKnights )	{
		SetShort( send_buff, strlen( pKnights->m_strName ), send_index );
		SetString( send_buff, pKnights->m_strName, strlen( pKnights->m_strName ), send_index );
		SetByte( send_buff, pKnights->m_byGrade, send_index );  // knights grade
		SetByte( send_buff, pKnights->m_byRanking, send_index );  // knights grade
	}
	m_pMain->Send_Region( send_buff, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ, NULL, false );
	//pUser->Send( send_buff, send_index );

	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_CHAT, send_index );
	SetByte( send_buff, KNIGHTS_CHAT, send_index );
	SetByte( send_buff, 1, send_index );
	SetShort( send_buff, -1, send_index );
	SetShort( send_buff, strlen(finalstr), send_index );
	SetString( send_buff, finalstr, strlen(finalstr), send_index );
	m_pMain->Send_KnightsMember( knightsindex, send_buff, send_index );

	idlen = strlen( pUser->m_pUserData->m_id );
	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, UDP_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command-0x10, send_index );
	SetShort( send_buff, knightsindex, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, pUser->m_pUserData->m_id, idlen, send_index );
	if( m_pMain->m_nServerGroup == 0 )
		m_pMain->Send_UDP_All( send_buff, send_index );
	else
		m_pMain->Send_UDP_All( send_buff, send_index, 1 );
}

void CKnightsManager::RecvModifyFame(CUser *pUser, char *pBuf, BYTE command)
{
	int index = 0, send_index = 0, knightsindex = 0, idlen = 0, vicechief = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char finalstr[128]; memset( finalstr, 0x00, 128 );
	char userid[MAX_ID_SIZE+1]; memset( userid, 0x00, MAX_ID_SIZE+1 );
	CUser* pTUser = NULL;
	CKnights*	pKnights = NULL;

	if( !pUser ) return;

	knightsindex = GetShort( pBuf, index );
	idlen = GetShort( pBuf, index );
	GetString( userid, pBuf, idlen, index );
	vicechief = GetByte( pBuf, index );

	pTUser = m_pMain->GetUserPtr( userid, 0x02 );
	pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );

	switch( command ) {
	case KNIGHTS_REMOVE+0x10:
		if( pTUser ) {
			pTUser->m_pUserData->m_bKnights = 0;
			pTUser->m_pUserData->m_bFame = 0;
			sprintf( finalstr, "#### %s님이 추방되셨습니다. ####", pTUser->m_pUserData->m_id );

			RemoveKnightsUser( knightsindex, pTUser->m_pUserData->m_id );
		}
		else	{
			RemoveKnightsUser( knightsindex, userid );
		}
		break;
	case KNIGHTS_ADMIT+0x10:
		if( pTUser )
			pTUser->m_pUserData->m_bFame = KNIGHT;
		break;
	case KNIGHTS_REJECT+0x10:
		if( pTUser ) {
			pTUser->m_pUserData->m_bKnights = 0;
			pTUser->m_pUserData->m_bFame = 0;

			RemoveKnightsUser( knightsindex, pTUser->m_pUserData->m_id );
		}
		break;
	case KNIGHTS_CHIEF+0x10:
		if( pTUser )	{
			pTUser->m_pUserData->m_bFame = CHIEF;
			ModifyKnightsUser( knightsindex, pTUser->m_pUserData->m_id );
			sprintf( finalstr, "#### %s님이 단장으로 임명되셨습니다. ####", pTUser->m_pUserData->m_id );
		}
		break;
	case KNIGHTS_VICECHIEF+0x10:
		if( pTUser )	{
			pTUser->m_pUserData->m_bFame = VICECHIEF;
			ModifyKnightsUser( knightsindex, pTUser->m_pUserData->m_id );
			sprintf( finalstr, "#### %s님이 부단장으로 임명되셨습니다. ####", pTUser->m_pUserData->m_id );
		}
		break;
	case KNIGHTS_OFFICER+0x10:
		if( pTUser )
			pTUser->m_pUserData->m_bFame = OFFICER;
		break;
	case KNIGHTS_PUNISH+0x10:
		if( pTUser )
			pTUser->m_pUserData->m_bFame = PUNISH;
		break;
	}

/*	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command-0x10, send_index );
	SetByte( send_buff, 0x01, send_index );
	pUser->Send( send_buff, send_index );
*/
	//TRACE("RecvModifyFame - command=%d, nid=%d, name=%s, index=%d, fame=%d\n", command, pTUser->GetSocketID(), pTUser->m_pUserData->m_id, knightsindex, pTUser->m_pUserData->m_bFame);
	
	if( pTUser ) {
		//TRACE("RecvModifyFame - command=%d, nid=%d, name=%s, index=%d, fame=%d\n", command, pTUser->GetSocketID(), pTUser->m_pUserData->m_id, knightsindex, pTUser->m_pUserData->m_bFame);
		memset( send_buff, 0x00, 128 ); send_index = 0;
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_MODIFY_FAME, send_index );
		SetByte( send_buff, 0x01, send_index );
		if( command == KNIGHTS_REMOVE+0x10 )	{
			SetShort( send_buff, pTUser->GetSocketID(), send_index );
			SetShort( send_buff, pTUser->m_pUserData->m_bKnights, send_index );
			SetByte( send_buff, pTUser->m_pUserData->m_bFame, send_index );
			m_pMain->Send_Region( send_buff, send_index, pTUser->m_pUserData->m_bZone, pTUser->m_RegionX, pTUser->m_RegionZ, NULL, false );
		}
		else	{
			SetShort( send_buff, pTUser->GetSocketID(), send_index );
			SetShort( send_buff, pTUser->m_pUserData->m_bKnights, send_index );
			SetByte( send_buff, pTUser->m_pUserData->m_bFame, send_index );
			pTUser->Send( send_buff, send_index );
		}

		if( command == KNIGHTS_REMOVE+0x10 )	{
			memset( send_buff, 0x00, 128 );		send_index = 0;
			SetByte( send_buff, WIZ_CHAT, send_index );
			SetByte( send_buff, KNIGHTS_CHAT, send_index );
			SetByte( send_buff, 1, send_index );
			SetShort( send_buff, -1, send_index );
			SetShort( send_buff, strlen(finalstr), send_index );
			SetString( send_buff, finalstr, strlen(finalstr), send_index );
			pTUser->Send( send_buff, send_index );
		}
	}

	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_CHAT, send_index );
	SetByte( send_buff, KNIGHTS_CHAT, send_index );
	SetByte( send_buff, 1, send_index );
	SetShort( send_buff, -1, send_index );
	SetShort( send_buff, strlen(finalstr), send_index );
	SetString( send_buff, finalstr, strlen(finalstr), send_index );
	m_pMain->Send_KnightsMember( knightsindex, send_buff, send_index );

	idlen = strlen( userid );
	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, UDP_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, command-0x10, send_index );
	SetShort( send_buff, knightsindex, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, userid, idlen, send_index );
	if( m_pMain->m_nServerGroup == 0 )
		m_pMain->Send_UDP_All( send_buff, send_index );
	else
		m_pMain->Send_UDP_All( send_buff, send_index, 1 );
}

void CKnightsManager::RecvDestroyKnights(CUser *pUser, char *pBuf)
{
	int send_index = 0, knightsindex = 0, index = 0, flag = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	char finalstr[128]; memset( finalstr, 0x00, 128 );
	CKnights*	pKnights = NULL;
	CUser* pTUser = NULL;

	if( !pUser ) return;

	knightsindex = GetShort( pBuf, index );

	pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );
	if( !pKnights )		{
		//TRACE("### RecvDestoryKnights  Fail == index = %d ###\n", knightsindex);
		return;
	}

	flag = pKnights->m_byFlag;

	// 클랜이나 기사단이 파괴된 메시지를 보내고 유저 데이타를 초기화
	if( flag == CLAN_TYPE)
		sprintf( finalstr, "#### %s 클랜이 해체되었습니다 ####", pKnights->m_strName );
	else if( flag == KNIGHTS_TYPE )
		sprintf( finalstr, "#### %s 기사단이 해체되었습니다 ####", pKnights->m_strName );

	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_CHAT, send_index );
	SetByte( send_buff, KNIGHTS_CHAT, send_index );
	SetByte( send_buff, 1, send_index );
	SetShort( send_buff, -1, send_index );
	SetShort( send_buff, strlen(finalstr), send_index );
	SetString( send_buff, finalstr, strlen(finalstr), send_index );
	m_pMain->Send_KnightsMember( knightsindex, send_buff, send_index );

	for( int i=0; i<MAX_USER; i++ ) {
		pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if( !pTUser ) continue;
		if( pTUser->m_pUserData->m_bKnights == knightsindex ) {
			pTUser->m_pUserData->m_bKnights = 0;
			pTUser->m_pUserData->m_bFame = 0;

			RemoveKnightsUser( knightsindex, pTUser->m_pUserData->m_id );

			memset( send_buff, 0x00, 128 ); send_index = 0;
			SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
			SetByte( send_buff, KNIGHTS_MODIFY_FAME, send_index );
			SetByte( send_buff, 0x01, send_index );
			SetShort( send_buff, pTUser->GetSocketID(), send_index );
			SetShort( send_buff, pTUser->m_pUserData->m_bKnights, send_index );
			SetByte( send_buff, pTUser->m_pUserData->m_bFame, send_index );
			m_pMain->Send_Region( send_buff, send_index, pTUser->m_pUserData->m_bZone, pTUser->m_RegionX, pTUser->m_RegionZ, NULL, false );
			//pTUser->Send( send_buff, send_index );
		}
	}
	
	m_pMain->m_KnightsArray.DeleteData( knightsindex );
	//TRACE("RecvDestoryKnights - nid=%d, name=%s, index=%d, fame=%d\n", pUser->GetSocketID(), pUser->m_pUserData->m_id, knightsindex, pUser->m_pUserData->m_bFame);

	memset( send_buff, 0x00, 128 ); send_index = 0;
	SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_DESTROY, send_index );
	SetByte( send_buff, 0x01, send_index );
	pUser->Send( send_buff, send_index );

	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, UDP_KNIGHTS_PROCESS, send_index );
	SetByte( send_buff, KNIGHTS_DESTROY, send_index );
	SetShort( send_buff, knightsindex, send_index );
	if( m_pMain->m_nServerGroup == 0 )
		m_pMain->Send_UDP_All( send_buff, send_index );
	else
		m_pMain->Send_UDP_All( send_buff, send_index, 1 );

	//if( flag == KNIGHTS_TYPE )	{
/*	memset( send_buff, 0x00, 128 ); send_index = 0;
		SetByte( send_buff, WIZ_KNIGHTS_LIST, send_index );
		SetByte( send_buff, 0x03, send_index );					// Knights Remove From List 
		SetShort( send_buff, knightsindex, send_index );
		m_pMain->Send_All( send_buff, send_index, pUser );	*/
	//}
}

BOOL CKnightsManager::LoadAllKnights()
{
	SQLHSTMT		hstmt = NULL;
	//SQLRETURN		retcode;
	BOOL			bData = TRUE,	retval = FALSE;
	CString			tempid, tempchief, tempvice_1, tempvice_2, tempvice_3;
	TCHAR			szSQL[1024];
	memset( szSQL, 0x00, 1024 );
/*
	CKnights* pKnights = NULL;

	SQLCHAR IDName[MAX_ID_SIZE+1], Chief[MAX_ID_SIZE+1], ViceChief_1[MAX_ID_SIZE+1], ViceChief_2[MAX_ID_SIZE+1], ViceChief_3[MAX_ID_SIZE+1], Nation;
	memset( IDName, 0x00, MAX_ID_SIZE+1 ); memset( Chief, 0x00, MAX_ID_SIZE+1 ); memset( ViceChief_1, 0x00, MAX_ID_SIZE+1 ); 
	memset( ViceChief_2, 0x00, MAX_ID_SIZE+1 );	memset( ViceChief_3, 0x00, MAX_ID_SIZE+1 );
	SQLSMALLINT	IDNum, Members, Domination;
	SQLINTEGER Money, Points;
	SQLINTEGER Indexind = SQL_NTS;

	//wsprintf( szSQL, TEXT( "SELECT IDNum, Nation, IDName, Members, Chief, ViceChief, Officer, Gold, Domination, Points, Mark FROM KNIGHTS" ) );
	wsprintf( szSQL, TEXT( "SELECT IDNum, Nation, IDName, Members, Chief, ViceChief_1, ViceChief_2, ViceChief_3, Gold, Domination, Points, Mark FROM KNIGHTS" ) );

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, m_KnightsDB.m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, 1024);
		if (retcode == SQL_SUCCESS|| retcode == SQL_SUCCESS_WITH_INFO) {
			while (bData) {
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					SQLGetData(hstmt,1  ,SQL_C_SSHORT,&IDNum, 0,&Indexind);
					SQLGetData(hstmt,2  ,SQL_C_TINYINT,&Nation, 0,&Indexind);
					SQLGetData(hstmt,3  ,SQL_C_CHAR  ,IDName, MAX_ID_SIZE,&Indexind);
					SQLGetData(hstmt,4  ,SQL_C_SSHORT,&Members, 0,&Indexind);
					SQLGetData(hstmt,5  ,SQL_C_CHAR  ,Chief, MAX_ID_SIZE,&Indexind);
					SQLGetData(hstmt,6  ,SQL_C_CHAR  ,ViceChief_1, MAX_ID_SIZE,&Indexind);
					SQLGetData(hstmt,7  ,SQL_C_CHAR  ,ViceChief_2, MAX_ID_SIZE,&Indexind);
					SQLGetData(hstmt,8  ,SQL_C_CHAR  ,ViceChief_3, MAX_ID_SIZE,&Indexind);
					SQLGetData(hstmt,9  ,SQL_C_LONG, &Money, 0,&Indexind);
					SQLGetData(hstmt,10  ,SQL_C_SSHORT,&Domination, 0,&Indexind);
					SQLGetData(hstmt,11  ,SQL_C_LONG, &Points, 0,&Indexind);

					tempid = IDName;
					tempid.TrimRight();
					tempchief = Chief;
					tempchief.TrimRight();
					tempvice_1 = ViceChief_1;
					tempvice_1.TrimRight();
					tempvice_2 = ViceChief_2;
					tempvice_2.TrimRight();
					tempvice_3 = ViceChief_3;
					tempvice_3.TrimRight();

					if( m_pMain->m_nServerNo == KARUS )	{
						if( IDNum < 15000 )	{
							pKnights = new CKnights;
							pKnights->sIndex = IDNum;
							pKnights->bNation = Nation;
							strcpy( pKnights->strName, (char*)(LPCTSTR)tempid );
							pKnights->sMembers = Members;
							strcpy( pKnights->strChief, (char*)(LPCTSTR)tempchief );
							strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)tempvice_1 );
							strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)tempvice_2 );
							strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)tempvice_3 );
							pKnights->nMoney = Money;
							pKnights->sDomination = Domination;
							pKnights->nPoints = Points;

							if( !m_pMain->m_KnightsArray.PutData(pKnights->sIndex, pKnights) ) {
								TRACE("Knights PutData Fail - %d\n", pKnights->sIndex);
								delete pKnights;
								pKnights = NULL;
							}
						}
					}
					else if( m_pMain->m_nServerNo == ELMORAD )	{
						if( IDNum >= 15000 && IDNum < 30000 )	{
							pKnights = new CKnights;
							pKnights->sIndex = IDNum;
							pKnights->bNation = Nation;
							strcpy( pKnights->strName, (char*)(LPCTSTR)tempid );
							pKnights->sMembers = Members;
							strcpy( pKnights->strChief, (char*)(LPCTSTR)tempchief );
							strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)tempvice_1 );
							strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)tempvice_2 );
							strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)tempvice_3 );
							pKnights->nMoney = Money;
							pKnights->sDomination = Domination;
							pKnights->nPoints = Points;

							if( !m_pMain->m_KnightsArray.PutData(pKnights->sIndex, pKnights) ) {
								TRACE("Knights PutData Fail - %d\n", pKnights->sIndex);
								delete pKnights;
								pKnights = NULL;
							}

							//TRACE("knightindex = %d\n", IDNum);

						}
					}
					else	{
						pKnights = new CKnights;
						pKnights->sIndex = IDNum;
						pKnights->bNation = Nation;
						strcpy( pKnights->strName, (char*)(LPCTSTR)tempid );
						pKnights->sMembers = Members;
						strcpy( pKnights->strChief, (char*)(LPCTSTR)tempchief );
						strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)tempvice_1 );
						strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)tempvice_2 );
						strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)tempvice_3 );
						pKnights->nMoney = Money;
						pKnights->sDomination = Domination;
						pKnights->nPoints = Points;

						if( !m_pMain->m_KnightsArray.PutData(pKnights->sIndex, pKnights) ) {
							TRACE("Knights PutData Fail - %d\n", pKnights->sIndex);
							delete pKnights;
							pKnights = NULL;
						}
					}

					bData = TRUE;
				}
				else
					bData = FALSE;

				memset( IDName, 0x00, MAX_ID_SIZE+1 ); memset( Chief, 0x00, MAX_ID_SIZE+1 ); memset( ViceChief_1, 0x00, MAX_ID_SIZE+1 ); 
				memset( ViceChief_2, 0x00, MAX_ID_SIZE+1 );	memset( ViceChief_3, 0x00, MAX_ID_SIZE+1 );
			}
			retval = TRUE;
		}
		else {
			DisplayErrorMsg( hstmt );
			retval = FALSE;
		}
	
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT,hstmt);
	}
	else
		return FALSE;	*/
	
	return retval;
}

BOOL CKnightsManager::LoadKnightsIndex(int index)
{
	SQLHSTMT		hstmt = NULL;
	//SQLRETURN		retcode;
	BOOL			bData = TRUE,	retval = FALSE;
	CString			tempid, tempchief, tempvice_1, tempvice_2, tempvice_3;
	TCHAR			szSQL[1024];
	memset( szSQL, 0x00, 1024 );

/*	CKnights* pKnights = NULL;

	SQLCHAR IDName[MAX_ID_SIZE+1], Chief[MAX_ID_SIZE+1], ViceChief_1[MAX_ID_SIZE+1], ViceChief_2[MAX_ID_SIZE+1], ViceChief_3[MAX_ID_SIZE+1], Nation;
	memset( IDName, 0x00, MAX_ID_SIZE+1 ); memset( Chief, 0x00, MAX_ID_SIZE+1 ); memset( ViceChief_1, 0x00, MAX_ID_SIZE+1 ); 
	memset( ViceChief_2, 0x00, MAX_ID_SIZE+1 );	memset( ViceChief_3, 0x00, MAX_ID_SIZE+1 );
	SQLSMALLINT	IDNum, Members, Domination;
	SQLINTEGER Money, Points;
	SQLINTEGER Indexind = SQL_NTS;

	//wsprintf( szSQL, TEXT( "SELECT IDNum, Nation, IDName, Members, Chief, ViceChief, Officer, Gold, Domination, Points, Mark FROM KNIGHTS" ) );
	wsprintf( szSQL, TEXT( "SELECT IDNum, Nation, IDName, Members, Chief, ViceChief_1, ViceChief_2, ViceChief_3, Gold, Domination, Points, Mark FROM KNIGHTS WHERE IDNum=%d" ), index );

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, m_KnightsDB.m_hdbc, &hstmt );
	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, 1024);
		if (retcode == SQL_SUCCESS|| retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hstmt,1  ,SQL_C_SSHORT,&IDNum, 0,&Indexind);
				SQLGetData(hstmt,2  ,SQL_C_TINYINT,&Nation, 0,&Indexind);
				SQLGetData(hstmt,3  ,SQL_C_CHAR  ,IDName, MAX_ID_SIZE,&Indexind);
				SQLGetData(hstmt,4  ,SQL_C_SSHORT,&Members, 0,&Indexind);
				SQLGetData(hstmt,5  ,SQL_C_CHAR  ,Chief, MAX_ID_SIZE,&Indexind);
				SQLGetData(hstmt,6  ,SQL_C_CHAR  ,ViceChief_1, MAX_ID_SIZE,&Indexind);
				SQLGetData(hstmt,7  ,SQL_C_CHAR  ,ViceChief_2, MAX_ID_SIZE,&Indexind);
				SQLGetData(hstmt,8  ,SQL_C_CHAR  ,ViceChief_3, MAX_ID_SIZE,&Indexind);
				SQLGetData(hstmt,9  ,SQL_C_LONG, &Money, 0,&Indexind);
				SQLGetData(hstmt,10  ,SQL_C_SSHORT,&Domination, 0,&Indexind);
				SQLGetData(hstmt,11  ,SQL_C_LONG, &Points, 0,&Indexind);

				tempid = IDName;
				tempid.TrimRight();
				tempchief = Chief;
				tempchief.TrimRight();
				tempvice_1 = ViceChief_1;
				tempvice_1.TrimRight();
				tempvice_2 = ViceChief_2;
				tempvice_2.TrimRight();
				tempvice_3 = ViceChief_3;
				tempvice_3.TrimRight();

				if( m_pMain->m_nServerNo == BATTLE ) {
					pKnights = m_pMain->m_KnightsArray.GetData( index );
					if( pKnights )	{
						pKnights->sIndex = IDNum;
						pKnights->bNation = Nation;
						strcpy( pKnights->strName, (char*)(LPCTSTR)tempid );
						pKnights->sMembers = Members;
						strcpy( pKnights->strChief, (char*)(LPCTSTR)tempchief );
						strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)tempvice_1 );
						strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)tempvice_2 );
						strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)tempvice_3 );
						pKnights->nMoney = Money;
						pKnights->sDomination = Domination;
						pKnights->nPoints = Points;
					}
					else	{
						pKnights = new CKnights;
						pKnights->sIndex = IDNum;
						pKnights->bNation = Nation;
						strcpy( pKnights->strName, (char*)(LPCTSTR)tempid );
						pKnights->sMembers = Members;
						strcpy( pKnights->strChief, (char*)(LPCTSTR)tempchief );
						strcpy( pKnights->strViceChief_1, (char*)(LPCTSTR)tempvice_1 );
						strcpy( pKnights->strViceChief_2, (char*)(LPCTSTR)tempvice_2 );
						strcpy( pKnights->strViceChief_3, (char*)(LPCTSTR)tempvice_3 );
						pKnights->nMoney = Money;
						pKnights->sDomination = Domination;
						pKnights->nPoints = Points;

						if( !m_pMain->m_KnightsArray.PutData(pKnights->sIndex, pKnights) ) {
							TRACE("Knights PutData Fail - %d\n", pKnights->sIndex);
							delete pKnights;
							pKnights = NULL;
						}
					}
				}
			}

		}
		else {
			DisplayErrorMsg( hstmt );
			retval = FALSE;
		}
	
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT,hstmt);
	}
	else
		return FALSE;
*/	
	return retval;
}

void CKnightsManager::RecvKnightsList( char* pBuf )
{
	CKnights* pKnights = NULL;

	int nation = 0, members = 0, index = 0, iLength = 0, knightsindex = 0, points = 0, ranking = 0;
	char knightsname[MAX_ID_SIZE+1]; memset( knightsname, 0x00, MAX_ID_SIZE+1 );

	knightsindex = GetShort( pBuf, index );
	nation = GetByte( pBuf, index );
	iLength = GetShort( pBuf, index );
	GetString( knightsname, pBuf, iLength, index );
	members = GetShort( pBuf, index );
	points = GetDWORD( pBuf, index ); // knights grade
	ranking = GetByte( pBuf, index );

	if( m_pMain->m_nServerNo == BATTLE )  {
		pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );
		if( pKnights )	{
			pKnights->m_sIndex = knightsindex;
			pKnights->m_byNation = nation;
			strcpy( pKnights->m_strName, knightsname );
			pKnights->m_sMembers = members;
			pKnights->m_nPoints = points;
			pKnights->m_byGrade = m_pMain->GetKnightsGrade( points );
			pKnights->m_byRanking = ranking;
		}
		else	{
			pKnights = new CKnights;
			pKnights->m_sIndex = knightsindex;
			pKnights->m_byNation = nation;
			strcpy( pKnights->m_strName, knightsname );
			pKnights->m_sMembers = members;
			strcpy( pKnights->m_strChief, "" );
			strcpy( pKnights->m_strViceChief_1, "" );
			strcpy( pKnights->m_strViceChief_2, "" );
			strcpy( pKnights->m_strViceChief_3, "" );
			pKnights->m_nMoney = 0;
			pKnights->m_sDomination = 0;
			pKnights->m_nPoints = points;
			pKnights->m_byGrade = m_pMain->GetKnightsGrade( points );
			pKnights->m_byRanking = ranking;

			if( !m_pMain->m_KnightsArray.PutData(pKnights->m_sIndex, pKnights) ) {
				TRACE("Recv Knights PutData Fail - %d\n", pKnights->m_sIndex);
				delete pKnights;
				pKnights = NULL;
			}
		}
	}
}

BOOL CKnightsManager::AddKnightsUser( int index, char* UserName )
{
	CKnights* pKnights = NULL;
	BOOL bCheckFlag = FALSE;

	pKnights = m_pMain->m_KnightsArray.GetData( index );
	if( !pKnights )	{
		TRACE("#### AddKnightsUser knightsindex fail : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}
	
	for(int i=0; i<MAX_CLAN; i++)	{
		if( pKnights->m_arKnightsUser[i].byUsed == 0 )	{
			pKnights->m_arKnightsUser[i].byUsed = 1;
			strcpy(pKnights->m_arKnightsUser[i].strUserName, UserName);
			bCheckFlag = TRUE;
			//TRACE("+++ AddKnightsUser knightsindex : username=%s, knightsindex=%d, i=%d \n", UserName, index, i);
			break;
		}
	}

	if( bCheckFlag == FALSE )	{
		//TRACE("#### AddKnightsUser user full : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}	

	return TRUE;
}

BOOL CKnightsManager::ModifyKnightsUser( int index, char* UserName )
{
	CKnights* pKnights = NULL;
	BOOL bCheckFlag = FALSE;

	pKnights = m_pMain->m_KnightsArray.GetData( index );
	if( !pKnights )	{
		TRACE("#### ModifyKnightsUser knightsindex fail : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}
	
	for(int i=0; i<MAX_CLAN; i++)	{
		if( pKnights->m_arKnightsUser[i].byUsed == 0 )	continue;
		if( !strcmp( pKnights->m_arKnightsUser[i].strUserName , UserName ) )	{
			pKnights->m_arKnightsUser[i].byUsed = 1;
			strcpy(pKnights->m_arKnightsUser[i].strUserName, UserName);
			bCheckFlag = TRUE;
			break;
		}
	}

	if( bCheckFlag == FALSE )	{
		//TRACE("#### ModifyKnightsUser user full : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}	

	return TRUE;
}

BOOL CKnightsManager::RemoveKnightsUser( int index, char* UserName )
{
	CKnights* pKnights = NULL;
	BOOL bCheckFlag = FALSE;

	pKnights = m_pMain->m_KnightsArray.GetData( index );
	if( !pKnights )	{
		TRACE("#### RemoveKnightsUser knightsindex fail : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}
	
	for(int i=0; i<MAX_CLAN; i++)	{
		if( pKnights->m_arKnightsUser[i].byUsed == 0 )	continue;
		if( !strcmp( pKnights->m_arKnightsUser[i].strUserName , UserName ) )	{
			pKnights->m_arKnightsUser[i].byUsed = 0;
			strcpy(pKnights->m_arKnightsUser[i].strUserName, "");
			bCheckFlag = TRUE;
			//TRACE("---> RemoveKnightsUser knightsindex : username=%s, knightsindex=%d, i=%d \n", UserName, index, i);
			break;
		}
	}

	if( bCheckFlag == FALSE )	{
		//TRACE("#### RemoveKnightsUser user full : username=%s, knightsindex=%d ####\n", UserName, index);
		return FALSE;
	}	

	return TRUE;
}

void CKnightsManager::SetKnightsUser( int index, char* UserName )
{
	CKnights* pKnights = NULL;
	BOOL bCheckFlag = FALSE;
	BOOL bAddFlag = FALSE;

	pKnights = m_pMain->m_KnightsArray.GetData( index );
	if( !pKnights )	{
		TRACE("#### SetKnightsUser knightsindex fail : username=%s, knightsindex=%d ####\n", UserName, index);
		return;
	}

	//TRACE("--- SetKnightsUser knightsindex : username=%s, knightsindex=%d \n", UserName, index);
	
	for(int i=0; i<MAX_CLAN; i++)	{
		if( pKnights->m_arKnightsUser[i].byUsed == 0 )	continue;
		if( !strcmp( pKnights->m_arKnightsUser[i].strUserName , UserName ) )	{
			//TRACE("### SetKnightsUser knightsindex - name is same : username=%s, knightsindex=%d \n", UserName, index);
			bCheckFlag = TRUE;
			break;
		}
	}

	if( bCheckFlag == FALSE )	{
		bAddFlag = AddKnightsUser( index, UserName );
		if( !bAddFlag )	{
			//TRACE("#### SetKnightsUser user full : username=%s, knightsindex=%d ####\n", UserName, index);
			return;
		}
	}	
}

void CKnightsManager::RecvKnightsAllList(char *pBuf)
{
	int index = 0, knightsindex = 0, points = 0, count = 0, grade=0, ranking = 0;
	int send_index = 0, temp_index = 0, send_count = 0;
	CKnights* pKnights = NULL;
	char send_buff[512];	memset(send_buff, 0x00, 512);
	char temp_buff[512];	memset(temp_buff, 0x00, 512);

	count = GetByte( pBuf, index );

	for(int i=0; i<count; i++)	{
		knightsindex = -1;		points = -1;
		knightsindex = GetShort( pBuf, index );
		points = GetDWORD( pBuf, index );
		ranking = GetByte( pBuf, index );

		pKnights = m_pMain->m_KnightsArray.GetData( knightsindex );
		if( !pKnights )	{
			TRACE("#### RecvKnightsAllList knightsindex fail : knightsindex=%d ####\n", knightsindex);
			continue;
		}
		if( pKnights->m_nPoints != points )	{
			pKnights->m_nPoints = points;
			pKnights->m_byGrade = m_pMain->GetKnightsGrade( points );

			SetShort( temp_buff, pKnights->m_sIndex, temp_index );
			SetByte( temp_buff, pKnights->m_byGrade, temp_index );
			SetByte( temp_buff, pKnights->m_byRanking, temp_index );
			send_count++;
		}
		else if( pKnights->m_byRanking != ranking )	{
			pKnights->m_byRanking = ranking;

			SetShort( temp_buff, pKnights->m_sIndex, temp_index );
			SetByte( temp_buff, pKnights->m_byGrade, temp_index );
			SetByte( temp_buff, pKnights->m_byRanking, temp_index );
			send_count++;
		}	
	}

	if( send_count > 0 )	{
		SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_ALLLIST_REQ, send_index );
		SetShort( send_buff, send_count, send_index );
		SetString( send_buff, temp_buff, temp_index, send_index );
		m_pMain->Send_All( send_buff, send_index, NULL);
	}
}

