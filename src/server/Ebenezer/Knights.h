// Knights.h: interface for the CKnights class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "define.h"
#include "gamedefine.h"

class CUser;
class CEbenezerDlg;
class CKnights  
{
public:
	short	m_sIndex;
	BYTE    m_byFlag;			// 1 : Clan, 2 : 기사단
	BYTE	m_byNation;			// nation
	BYTE	m_byGrade;			// clan 등급 (1 ~ 5등급)
	BYTE	m_byRanking;		// clan 등급 (1 ~ 5등)
	char	m_strName[MAX_ID_SIZE+1];
	short	m_sMembers;
	char	m_strChief[MAX_ID_SIZE+1];
	char	m_strViceChief_1[MAX_ID_SIZE+1];		// 부단장 1
	char	m_strViceChief_2[MAX_ID_SIZE+1];		// 부단장 2 (기사단에서는 장교)
	char	m_strViceChief_3[MAX_ID_SIZE+1];		// 부단장 3	(기사단에서는 사용하지 않음)
	__int64	m_nMoney;
	short	m_sDomination;
	int		m_nPoints;
	BYTE	m_Image[MAX_KNIGHTS_MARK];
	_ITEM_DATA	m_StashItem[MAX_KNIGHTS_BANK];
	_KNIGHTS_USER m_arKnightsUser[MAX_CLAN];		// 클랜원의 정보

public:
	CKnights();
	virtual ~CKnights();

	void InitializeValue();

private:
	
};

