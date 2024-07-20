// PartyUser.h: interface for the CPartyUser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CPartyUser {
  public:
    char m_strUserID[MAX_ID_SIZE + 1]; // 캐릭터의 이름
    int  m_iUserId;                    // User의 번호
  public:
    CPartyUser();
    virtual ~CPartyUser();
};
