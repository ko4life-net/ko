// N3FXBundleGame.h: interface for the CN3FXBundleGame class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3Base/N3FXBundle.h"

class CN3FXBundleGame : public CN3FXBundle
{
public:
	int					m_iID;			//겜에서 쓰는 아디..
	int					m_iIdx;
	bool				m_bRegion;

	//포물선 운동을 위한...싸인곡선 이용함다..
	float				m_fDistance;
	float				m_fHeight;
	
public:
	void	SetPreBundlePos(int iSourceID, int iJoint);
	void	Trigger(int iSourceID=0, int iTargetID = 0, int iTargetJoint = 0, int iSndID = -1);
	void	Trigger(int iSourceID, __Vector3 TargetPos, int iSndID = -1);
	bool	Tick();
	
	void	Duplicate(CN3FXBundleGame* pDestBundle);
	bool	Load(HANDLE hFile);

	CN3FXBundleGame();
	virtual ~CN3FXBundleGame();

};

