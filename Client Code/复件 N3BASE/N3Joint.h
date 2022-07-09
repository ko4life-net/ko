// N3Joint.h: interface for the CN3Joint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3IJoint_h__INCLUDED_)
#define AFX_N3IJoint_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"
#include "N3LinkedList.h"

const int MAX_JOINT_TRANSFORM = 64;

class CN3Joint : public CN3Transform
{
	friend class CN3Joint; // Position, Rotation, Scale 값들을 자유롭게 다룰수 있어야 한다..
public:
	BOOL		m_bHaveOriginRot; // 원래 회전값이 있는지..
	__Matrix44	m_MtxOriginRot; // 원래 회전값..

protected:
	CN3Joint* m_pParent;
	CN3LinkedList<CN3Joint*> m_ChildrenList;

public:
	void ChildDelete(CN3Joint* pChild);
	void CopyExceptAnimationKey(CN3Joint* pJDest, CN3Joint* pJSrc);
	void AddKey(CN3Joint* pJDest, CN3Joint* pJSrc, int nIndexS, int nIndexE);
	void KeyDelete(CN3Joint* pJoint, int nKS, int nKE);

	void MatricesBlendGet(__Matrix44 *pMtxs, int &nJointIndex, float fFrm0, float fFrm1, float fWeight0);
	void MatricesGet(__Matrix44 *pMtxs, int& nJointIndex);
	
	void Tick(float fFrm);
	void ReCalcMatrix();
	void ParentSet(CN3Joint* pParent);
	void ChildAdd(CN3Joint* pChild);
	CN3Joint* Parent() { return m_pParent; }
	CN3Joint* Child(int index) { return m_ChildrenList.Get(index); }
	int ChildCount() { return m_ChildrenList.Count(); }

	void NodeCount(int &nCount);
	BOOL FindIndex(const char *szName, int &nIndex);
	BOOL FindPointerByName(const char* szName, CN3Joint *&pJoint); // 이름을 넣으면 해당 노드의 포인터를 돌려준다..
	BOOL FindPointerByID(int nID, CN3Joint *&pJoint);

	void RotSet(__Quaternion& qtRot) { m_qRot = qtRot; this->ReCalcMatrix(); }
	void RotSet(float x, float y, float z, float w) { m_qRot.x = x; m_qRot.y = y; m_qRot.z = z; m_qRot.w = w; this->ReCalcMatrix(); }
	void Render(const __Matrix44* pMtxParent = NULL, float fUnitSize = 0.1f);

	void Release();
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	CN3Joint();
	virtual ~CN3Joint();

};

#endif // !defined(AFX_N3IJoint_h__INCLUDED_)
