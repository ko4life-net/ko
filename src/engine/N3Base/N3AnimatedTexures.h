// N3AnimatedTexures.h: interface for the CN3AnimatedTexures class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "N3BaseFileAccess.h"
#include <vector>
#include "N3Texture.h"	// Added by ClassView

class CN3AnimatedTexures : public CN3BaseFileAccess
{
protected:
	std::vector<class CN3Texture*> m_TexRefs;
	float m_fTexIndex;
	float m_fTexFPS;

public:
	void Tick();
	CN3Texture* Tex(int iIndex) { if(iIndex < 0 || iIndex >= m_TexRefs.size()) return NULL; return m_TexRefs[iIndex]; }
	CN3Texture* TexCur() { return this->Tex((int)m_fTexIndex); }
	void Release();
	bool Load(HANDLE hFile);
#ifdef _N3TOOL
	bool Save(HANDLE hFile);
#endif // #ifdef _N3TOOL

	CN3AnimatedTexures();
	virtual ~CN3AnimatedTexures();

};

