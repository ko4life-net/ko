// PvsBase.h: interface for the CPvsBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PVSBASE_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_)
#define AFX_PVSBASE_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "..\N3Base\N3Base.h"

#include <string>
#include <list>

enum e_PvsState { STATE_NONE = 0, STATE_SELECTED, STATE_LINKED };
const DWORD dwColorNone = 0xff00ff00;
const DWORD dwColorLinked = 0xff6666ff;
const DWORD dwColorSelected = 0xffff6666;
const DWORD dwColorNoneAlpha = 0x66006600;
const DWORD dwColorLinkedAlpha = 0x666666ff;
const DWORD dwColorSelectedAlpha = 0x66ff6666;
const float fBaseVolumnSize = 1.0f;
const float fBaseWallSize = 0.99f;
const float fWallSizeOffset = 0.01f;
const int	_iTypeWall = 0;
const int	_iTypeVolumn = 1;
enum e_WallType { WALL_ZB = 0, WALL_ZF, WALL_XL, WALL_XR, WALL_YT, WALL_YB };
enum e_ExtBool	{ TYPE_UNKNOWN = 0, TYPE_TRUE, TYPE_FALSE };
enum e_ReturnCode {	NO_COLLISION = 0, COLLISION_AND_CONTINUE, COLLISION_AND_STOP };

class CPvsObjFactory;
class COrganizeView;


class CPvsBase : public CObject  
{
#ifdef _N3TOOL
	friend class COrganizeView;
#endif

	friend class CPvsObjFactory;

	DECLARE_DYNAMIC( CPvsBase )

	e_PvsState	m_eState;

#ifdef _N3TOOL
	std::string		m_strID;
#endif 

	int				m_iID;
	__Matrix44		m_MtxMove;	
	__Matrix44		m_MtxScale;	

	CPvsBase();
	virtual ~CPvsBase();

	virtual bool CreatePvsObject() = 0;
	virtual bool DeletePvsObject() = 0;

	virtual void TickEdit() = 0;
	virtual void TickCompile() = 0;
	virtual void TickExecute() = 0;

	virtual void RenderEdit() = 0;
	virtual void RenderCompile() = 0;
	virtual void RenderExecute() = 0;

//	virtual	void Render() = 0;	
	virtual void Translate() = 0;

	virtual void SetState(e_PvsState ePS) = 0;

	virtual void Load(FILE* stream);
	virtual void Save(FILE* stream);
};

#endif // !defined(AFX_PVSBASE_H__1391B742_4359_49B6_A268_EC098B8F20FD__INCLUDED_)
