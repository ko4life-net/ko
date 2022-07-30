// EVENT_DATA.h: interface for the EVENT_DATA class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "EXEC.h"
#include "LOGIC_ELSE.h"
#pragma warning(disable : 4786)
#include <list>

typedef	 std::list<EXEC*>				ExecArray;
typedef	 std::list<LOGIC_ELSE*>			LogicElseArray;


class EVENT_DATA  
{
public:
	int					m_EventNum;
	ExecArray			m_arExec;
	LogicElseArray		m_arLogicElse;

	EVENT_DATA();
	virtual ~EVENT_DATA();
};

