// EVENT.h: interface for the EVENT class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "EVENT_DATA.h"
#pragma warning(disable : 4786)
#include "STLMap.h"

typedef CSTLMap<EVENT_DATA> EventDataArray;

class EVENT {
  public:
    void DeleteAll();
    void Parsing(char * pBuf);
    void Init();
    BOOL LoadEvent(int zone);
    int  m_Zone;

    EventDataArray m_arEvent;

    EVENT();
    virtual ~EVENT();
};
