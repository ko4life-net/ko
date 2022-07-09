// PortalVolume.h: interface for the CPortalVolume class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTALVOLUME_H__1AEE8FD1_8D6A_4412_8251_825318D02B19__INCLUDED_)
#define AFX_PORTALVOLUME_H__1AEE8FD1_8D6A_4412_8251_825318D02B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "..\N3Base\N3Base.h"
#include "..\N3Base\N3BaseFileAccess.h"
#include "..\N3Base\N3Scene.h"

#include <string>
#include <list>
#include <vector>

class CPortalVolume;
class CN3Shape;

enum e_PvsState { STATE_NONE = 0, STATE_SELECTED, STATE_LINKED, STATE_VOLUME_ONLY };
enum e_PvsWallType { PWALL_ZB = 0, PWALL_ZF, PWALL_XL, PWALL_XR, PWALL_YT, PWALL_YB };
enum e_ExtBool	{ TYPE_UNKNOWN = 0, TYPE_TRUE, TYPE_FALSE };
enum e_ShapePartial	{ SHAPEPARTIAL_DO = 0, SHAPEPARTIAL_DONOT };

const DWORD dwColorNone = 0xffffffff;
const DWORD dwColorSelected = 0xffff7777;
const DWORD dwColorSelectedVolOnly = 0xff00ff00;

const DWORD dwColorXL = 0xffffff66;
const DWORD dwColorXR = 0xff66ffff;
const DWORD dwColorYT = 0xffbb1111;
const DWORD dwColorYB = 0xff6666ff;
const DWORD dwColorZF = 0xffcc99dd;
const DWORD dwColorZB = 0xff11cc77;

const float fBaseVolumnSize = 1.0f;

typedef struct tagWVol {
	e_PvsWallType ePWT;
	CPortalVolume*	pPvsVol;
	tagWVol()
	{
		pPvsVol = NULL;
	}
} WVOL;

typedef struct tagWVID {
	e_PvsWallType ePWT;
	int		iID;
	tagWVID()
	{
		iID = 0;
	}
} WVID;

typedef struct tagVisPartIndex {
	int m_iPartIndex;
	std::vector<int> m_ivVector;
} __VPI;

typedef struct tagCollisionIndex
{
	int m_iID;
	std::vector<int> m_ivVector;
	tagCollisionIndex()
	{
		m_iID = -1;
	}
} __ColIndex;

typedef struct tagShapeInfo	: public CN3Transform	
{
	int									m_iID;
	std::string						m_strShapeFile;

	int									m_iBelong;				// 소속 - 0:소속 없음 1:엘모라드 2:카루스 3:?? ....
	int									m_iEventID;				 // Event ID
	int									m_iEventType;		  // Event Type
	int									m_iNPC_ID;			   // NPC 로 쓰는 오브젝트일 경우 NPC ID
	int									m_iNPC_Status;	    // NPC 로 쓰는 오브젝트일 경우 Default Status

	CN3Shape*					m_pShape;

	//..
	tagShapeInfo()
	{
		m_iID = -1;
		m_strShapeFile = "";
		m_pShape = NULL;		

		m_iBelong = 0;
		m_iEventID = 0;	
		m_iEventType = 0;
		m_iNPC_ID = 0;	
		m_iNPC_Status = 0;	
	}
	
	const tagShapeInfo& operator = (const tagShapeInfo& si)
	{
		m_iID = si.m_iID;
		m_strShapeFile = si.m_strShapeFile;
		m_pShape = si.m_pShape;
		m_iBelong = si.m_iBelong;
		m_iEventID = si.m_iEventID;	
		m_iEventType = si.m_iEventType;
		m_iNPC_ID = si.m_iNPC_ID;	
		m_iNPC_Status = si.m_iNPC_Status;
		return *this;
	}

} ShapeInfo;

typedef struct tagIDAndPriority
{
	int						m_iID;
	int						m_iPriority;
}	IDAndPriority;

typedef struct tagVisPortalPriority
{
	CPortalVolume* m_pVol;
	int						m_iPriority;

	tagVisPortalPriority()
	{
		m_pVol = NULL;
		m_iPriority = 100;
	}
}	VisPortalPriority;

typedef struct tagFloorInfo
{
	int						m_iFloor;
	std::vector<CPortalVolume*>	m_vVolume;
} FloorInfo;

typedef std::list<FloorInfo>::iterator vfiter;
typedef std::list<VisPortalPriority>::iterator vppiter;
typedef std::list<IDAndPriority>::iterator idapiter;
typedef std::list<__VPI>::iterator	viter;
typedef std::list<CPortalVolume*>::iterator iter;
typedef std::list<WVOL>::iterator witer;
typedef std::list<WVID>::iterator wviter;
typedef std::list<ShapeInfo*>::iterator siiter;
typedef std::list<__ColIndex*>::iterator ciiter;

typedef struct tagShapePart
{
	int									m_iID;
	std::list<__VPI>			 m_viIndex;									// Visible Index List..				

	tagShapePart()
	{
		m_iID = -1;
	}	
	void Clear()
	{
		__VPI vpi;
		viter vit =	m_viIndex.begin();	
		while (vit != m_viIndex.end())
		{
			vpi = *vit++;
			vpi.m_ivVector.clear();
		}
		m_viIndex.clear();
	}
	~tagShapePart()
	{
		Clear();
	}
} ShapePart;

typedef std::list<ShapePart*>::iterator spiter;

//////////////////////////////////////////

class CPortalVolume	: public CN3Transform
{
	friend class CPortalFactory;	
	friend class CPVSManager;
	friend class COrganizeView;
	friend class CTransDummy;
	friend class CMainFrame;
	friend class CDlgShapeList;

	e_PvsState	m_eState;
	e_ExtBool	 m_eRenderType;		

	std::string		m_strID;
	int					m_iID;

	__VertexColor				m_pvVertex[8];
	unsigned short			   m_pIndex[36];

	////////////////////////////////////////
	std::list<WVOL>				m_VoltList;		
	std::list<WVID>				m_piIDList;									// 로드에 필요한 중간 데이터..		
	std::list<IDAndPriority>	m_piVisibleIDList;						// 로드에 필요한 중간 데이터..		
	std::list<ShapeInfo* >		m_plShapeInfoList;
	std::list<ShapePart* >		m_lpShapePartList;	
	std::list<__ColIndex* >		m_lpShapeColPartList;
	std::list<VisPortalPriority>	m_pVisiblePvsList;		
	////////////////////////////////////////

public:
	int									m_iPriority;							//.. 컴파일 모드에서 Portal의 우선순위..	-1로 먼저 클리어 한다음.. 0 순위는 자기 자신..

private:
	 bool QueryHeight(float fx, float fz, float& fy);
	bool HasCollisionData();
	void ClearCompileData();
	ShapeInfo* GetNthLinkedShapeInfo(int iOrder);
	CPortalVolume* GetNthLinkedVolume(int iOrder);
	int	   GetLinkedVolumeCount();
	int	   GetLinkedShapeCount();
	void SetShape(std::string szStr, CN3Shape* pShape, int iOrder);

	bool IsExistLinkedShapeByPointer(ShapeInfo* pSI);

	__Vector3 GetCentorPoint();
	float GetTopVolumeHeight();
	void DeleteLinkByiOrder(int iOrder);
	void DeleteLinkedShapeByiOrder(int iOrder);

	void RenderShapeEdit();
	void RenderCollisionEdit();
	void RenderShapeCompile();
	void RenderCollisionCompile();
	void RenderShapeExecute();
	void RenderCollisionExecute();

	//..
	void SplitAndMakeShape(ShapeInfo* pSI);
	void SplitAndMakeCollision(ShapeInfo* pSI);

	//.. 
	bool IsInVolumn(__Vector3 vec);
	bool IsInVolumn2d(int ix, int iz);
	bool IsInVolumnEx(__Vector3 vec1, __Vector3 vec2, __Vector3 Vec3);
	bool IsInVolumnExEx(__Vector3 vec1, __Vector3 vec2);
	bool IsInVolumnExExEx(__Vector3 vOrig, __Vector3 vDir, e_PvsWallType ePWT, __Vector3 &vPick);

	bool	IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir,
							  const __Vector3& v0, const __Vector3& v1, const __Vector3& v2,	
							  float& fT, float& fU, float& fV, __Vector3* pVCol);

	bool	Load(HANDLE hFile, bool bGameData = false);
	bool	Save(HANDLE hFile, bool bGameData = false);

	void	LoadGameData(HANDLE hFile);
	void	SaveGameData(HANDLE hFile);

	//..
	bool	PickWithVolume(__Vector3 &vPos, __Vector3& vDir, __Vector3* pVec);	

	//..
	void	UpdatePosAll(float fx, float fy, float fz);
	bool	GetHeight(int iX, int iZ, float &fHeight);

public:
	CPortalVolume();
	virtual ~CPortalVolume();

	void RenderEdit();
	void RenderCompile();
	void RenderExecute();

	void SetState(e_PvsState ePS, e_PvsWallType ePWT = PWALL_ZB);

	__Vector3		Min();
	__Vector3		Max();
};

#endif // !defined(AFX_PORTALVOLUME_H__1AEE8FD1_8D6A_4412_8251_825318D02B19__INCLUDED_)
