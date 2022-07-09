// FXMgr.h: interface for the CFXMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FXMGR_H__AFD9FE00_35BA_42D7_81F6_391BFEC6227F__INCLUDED_)
#define AFX_FXMGR_H__AFD9FE00_35BA_42D7_81F6_391BFEC6227F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

#include "N3LinkedList.h"
#include "N3Effect.h"
#include "N3EffectInstance.h"

const int MAX_EFFECT = 1024;
class CFXMgr : public CN3Base
{
protected:
	int m_nEffectCount;
	CN3Effect* m_pEffects[MAX_EFFECT];
	CN3LinkedList<CN3EffectInstance*> m_EffectInstances;

public:
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);
	
	int EffectCount() { return m_nEffectCount; }
	int EffectInstanceCount() { return m_EffectInstances.Count(); }
	CN3Effect* Effect(int index) { if(index < 0 || index >= MAX_EFFECT) return NULL; return m_pEffects[index]; };
	CN3Effect* EffectAdd(); // 추가된 Effect를 return
	CN3Effect* EffectAddFromFile(const char* szFN); // 추가된 Effect를 return

	void	Trigger(const char* szEffectName, __Vector3& vPos, __Vector3& vDir, float fVelocity, CN3Shape* pShape);
	void	Tick();
	void	Render();
	void	ReleaseInstances();
	void	Release();

	CFXMgr();
	virtual ~CFXMgr();
};


#endif // !defined(AFX_FXMGR_H__AFD9FE00_35BA_42D7_81F6_391BFEC6227F__INCLUDED_)
