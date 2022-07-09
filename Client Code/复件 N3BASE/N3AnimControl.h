// N3AnimControl.h: interface for the CN3AnimControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3ANIMCONTROL_H__37E9A095_FF76_4DD5_95A2_4CA1ABC227B0__INCLUDED_)
#define AFX_N3ANIMCONTROL_H__37E9A095_FF76_4DD5_95A2_4CA1ABC227B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3LinkedList.h"

class CN3AnimControl : public CN3Base
{
public:
	typedef struct __AnimData
	{
		friend CN3AnimControl;
protected:
		char*	szName;
public:
		float	fFrmStartUpper; // 상체 시작
		float	fFrmEndUpper; // 상체 끝
		float	fFrmPerSecUpper; // 초당 30프레임이 표준이다..

		float	fFrmStartLower; // 하체 시작
		float	fFrmEndLower; // 하체 끝
		float	fFrmPerSecLower; // 초당 30프레임이 표준이다..

		float	fFrmPlugTraceStart0;
		float	fFrmPlugTraceEnd0;
		
		float	fFrmPlugTraceStart1;
		float	fFrmPlugTraceEnd1;
		
		float	fReserved0;
		float	fReserved1;

		__AnimData()
		{
			memset(this, 0, sizeof(__AnimData));
			fFrmPerSecUpper = 30.0f;
			fFrmPerSecLower = 30.0f;
		}
		~__AnimData()
		{
			delete [] szName;
		}
		const char* Name() { return szName; }
		void NameSet(const char* szN) 
		{
			int nL = lstrlen(szN);
			if(nL <= 0) return;

			delete [] szName;
			szName = new char[nL+1];
			lstrcpy(szName, szN);
		}
		void operator = (__AnimData& other)
		{
			fFrmStartUpper = other.fFrmStartUpper;
			fFrmEndUpper = other.fFrmEndUpper;
			fFrmPerSecUpper = other.fFrmPerSecUpper;

			fFrmStartLower = other.fFrmStartLower;
			fFrmEndLower = other.fFrmEndLower;
			fFrmPerSecLower = other.fFrmPerSecLower;

			fFrmPlugTraceStart0 = other.fFrmPlugTraceStart0;
			fFrmPlugTraceEnd0 = other.fFrmPlugTraceEnd0;

			fFrmPlugTraceStart1 = other.fFrmPlugTraceStart1;
			fFrmPlugTraceEnd1 = other.fFrmPlugTraceEnd1;

			fReserved0 = other.fReserved0;
			fReserved1 = other.fReserved1;
			
			this->NameSet(other.Name());
		}

	} __AnimData;

	CN3LinkedList<__AnimData*>	m_AniList; // animation Data List

public:
	__AnimData* DataGet(int index) { if(index < 0 || index >= m_AniList.Count()) return NULL; else return m_AniList.Get(index); }

	bool Save(HANDLE hFile);
	bool Load(HANDLE hFile);
	void Release();
	
	CN3AnimControl();
	virtual ~CN3AnimControl();

};

#endif // !defined(AFX_N3ANIMCONTROL_H__37E9A095_FF76_4DD5_95A2_4CA1ABC227B0__INCLUDED_)
