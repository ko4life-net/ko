// PVSManager.h: interface for the CPVSManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PVSMANAGER_H__D2D86F4E_2762_4D61_B136_34CFB5E3657D__INCLUDED_)
#define AFX_PVSMANAGER_H__D2D86F4E_2762_4D61_B136_34CFB5E3657D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "..\N3Base\N3Base.h"
#include "..\N3Base\N3BaseFileAccess.h"

#include "PortalFactory.h"
#include "PortalVolume.h"

#include <list>
#include <vector>
#include <queue>

class COrganizeView;

// 우선순위대로 정렬..
template<class T> struct Myless : public std::less<T> {
bool operator()(const T& x, const T& y) const
{
	return (x->m_iPriority > y->m_iPriority);
}
};

class CPVSManager    : public CN3BaseFileAccess
{
	friend class COrganizeView;
	friend class CMainFrame;
	friend class CDlgShapeList;

	/////////////////////////////////////////
	std::list<CPortalVolume*>	m_pPvsList;
	
	//.. Main Shape List.. ^^
	std::list<ShapeInfo* >	  m_plShapeInfoList;

	//.. Current Portal Volume.. ^^
	CPortalVolume*				m_pCurVol;
	__Vector3						m_vBackupEye;
	__Vector3						m_vBackupAt;
	__Vector3						m_vBackupEyeExecute;
	__Vector3						m_vBackupAtExecute;

	// Total Position Move Backup..
	__Vector3						m_vBackupTotalMove;

private:
	bool	m_bCompiled;
	bool	m_bGameData;	

	CPortalFactory			m_Factory;

	int		m_iTotalCount;
	int		m_iIncreseIndex;
	int		m_iIncreseShapeIndex;

	int		m_iCurIndex;
//////////////////////////////////////////////////

	void	InsertNewPortalVolumn();
	void	DeleteAllPvsObj();

	CPortalVolume* GetPortalVolByiOrder(int iOrder);
	CPortalVolume* GetPortalVolPointerByID(int iID);
	bool	DeletePortalVolByiOrder(int iOrder);
	bool	DeletePortalVolByPointer(CPortalVolume* pVol);
	void	DeletePortalVolLinkedByiID(int iID);
	void	DeleteTotalShapeLinkByiOrder(int iOrder);

	bool	IsExistPortalVolumeByPointer(CPortalVolume* pVol);
	bool	IsExistTotalShapeByPointer(ShapeInfo* pSI);
	bool	IsExistLinkedShapeByPointer(ShapeInfo* pSI);

	// Load Save..
	bool	Load(HANDLE hFile);
	bool	LoadOldVersion(HANDLE hFile, int iVersionFromData);
	bool	Save(HANDLE hFile);
	void	SetMaxShapeIndex();

	// Edit 모드..
	void	TickEdit();
	void	RenderEdit();

	// Compile 모드..
	void	TickCompile();
	void	RenderCompile();

	// Execute 모드..
	void	TickExecute();
	void	RenderExecute();

	bool	StartExecuteMode();
	void	EndExecuteMode();

	void	ExecuteCameraChange();
	void	RestoreExecuteCameraChange();

	//.. 
	void	TotalShapeRenderEdit();
	void	TotalCollisionRenderEdit();

	//  Compiling..
	void	DoAllCompile();
	void	CheckcompileMode(CPortalVolume *pVol);
	void	Decompile();
	void	DecompilePerVolume(CPortalVolume *pVol);

	// Utility.. 
	e_PvsWallType GetReverseWallType(e_PvsWallType ePWT);

	// Visibility를 결정한다..
	void	ComputeVisibilty(CPortalVolume * const pVol);
	void	SetPriority(CPortalVolume * const pVol);
	void	SetPriorityEx(CPortalVolume * const pVol, e_PvsWallType ePWT, int iRecursive);
	void	SetPriorityRecursive(CPortalVolume * const pVol, e_PvsWallType ePWT, int iRecursive);
	bool	HasWallTypeVolume(CPortalVolume * const pVol, e_PvsWallType ePWT);
	void	SetPriorityNoneRecursive(CPortalVolume * const pVol, int iRecursive);
	void	SetPriorityNoneRecursiveEx(CPortalVolume * const pVol, int iRecursive);
	void	DebugFunc();

	// Shape를 공간에 맞게 쪼갠다..
	void	SplitShapeToVolumn(CDialog* pDlg);

	// Shape를 리스트에 등록한다..
	void	RegisterShape(std::string szStr, CN3Shape* pShape);
	bool	IsValidPortalVolume(CPortalVolume* pVol);

	void	UpdatePosAll(float fx, float fy, float fz);

////////////////////////////////////////////////////////////////
public:
	CPortalVolume* PickCollision(float fx, float fy);
	int		  GetIndexByVolumePointer(CPortalVolume* const pVol);

public:
	//..
	ShapeInfo* GetShapeInfoByManager(int iID);
	CN3Shape* GetShapeByManager(std::string szStr);

	// String Cryptograph.. ^^
	static void WriteCryptographString(HANDLE hFile, std::string strSrc);
	static std::string ReadDecryptString(HANDLE hFile);

	CPVSManager();
	virtual ~CPVSManager();
};

#endif // !defined(AFX_PVSMANAGER_H__D2D86F4E_2762_4D61_B136_34CFB5E3657D__INCLUDED_)
