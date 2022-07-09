// N3Chr.h: interface for the CN3Chr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Chr_h__INCLUDED_)
#define AFX_N3Chr_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Texture.h"
#include "N3TransformCollision.h"
#include "N3Joint.h"
#include "N3Skin.h"
#include "N3IMesh.h"
#include "N3PMeshInstance.h"
#include "N3LinkedList.h"
#include "N3AnimControl.h"

const int MAX_CHR_JOINT = 256;
const int MAX_CHR_LOD = 4; // 4 단계 Level Of Detail

enum e_PartType	{ PART_HAIR = 0, PART_FACE, PART_UPPER, PART_LOWER, PART_HAND, PART_FOOT, PART_UNKNOWN = 0xffffffff };
enum e_PlugType { PLUG_RIGHTHAND = 0, PLUG_LEFTHAND, PLUG_TWOHAND, PLUG_QUIVER, PLUG_UNKNOWN = 0xffffffff };

class CN3CPart : public CN3Base
{
	friend class CN3Chr;
public:
	char		m_szID[64]; // 파일 이름이 아닌 고유한 Part 이름..
	__Material	m_Mtl;
	e_PartType	m_Type;
protected:
	CN3Texture* m_pTexRef;
	CN3IMesh	m_IMeshes[MAX_CHR_LOD];
	CN3Skin		m_Skins[MAX_CHR_LOD];
public:
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	void Render(int nLOD);
	CN3Texture* Tex() { return m_pTexRef; }
	void TexSet(const char* szFN);
	CN3IMesh* Mesh(int nLOD) { if(nLOD >= 0 && nLOD < MAX_CHR_LOD) return &m_IMeshes[nLOD]; else return NULL; }
	CN3Skin* Skin(int nLOD) { if(nLOD >= 0 && nLOD < MAX_CHR_LOD) return &m_Skins[nLOD]; else return NULL; }

	void Release();
	
	CN3CPart();
	virtual ~CN3CPart();
};

class CN3CPlug : public CN3Base
{
	friend class CN3Chr;
public:
	e_PlugType	m_Type;
	int m_nJointIndex; // 붙는 Mesh 의 Joint Index.. 어느 조인트에 붙느냐.. 그것이 문제로다...
	__Material m_Mtl;
	__Matrix44 m_Matrix;

	int 		m_nTraceStep; // 궤적의 길이..
	D3DCOLOR	m_crTrace; // 궤적 색깔.. 검은색이면 없다..
	float		m_fTrace0; // 궤적 위치..
	float		m_fTrace1; // 궤적 위치..

protected:
	CN3PMeshInstance	m_PMeshInst; // Progressive Mesh Instance
	CN3Texture*			m_pTexRef; // Texture Reference Pointer
	__Vector3	m_vPosition; // 붙는 Mesh 의 Offset Vector.
//		__Vector3 m_vRotation; // 붙는 Mesh 의 Rotation일.
	__Matrix44	m_MtxRot; // Rotation Matrix;
	__Vector3	m_vScale; // 붙는 Mesh 의 스케일.

public:
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	void Render(__Matrix44& mtxParent, __Matrix44& mtxJoint);
	void RecalcMatrix();

	__Vector3& Position() { return m_vPosition; }
	__Matrix44& Rotation() { return m_MtxRot; }
	__Vector3& Scale() { return m_vScale; }

	void PositionSet(__Vector3& vPos) { m_vPosition = vPos; this->RecalcMatrix(); }
	void RotationSet(__Matrix44& mtx) { m_MtxRot = mtx; this->RecalcMatrix(); }
	void ScaleSet(__Vector3& vScale2) { m_vScale = vScale2; this->RecalcMatrix(); }
	
	CN3Texture* Tex() { return m_pTexRef; }
	void TexSet(const char* szFN);
	
	CN3PMesh* PMesh() { return m_PMeshInst.GetMesh(); }
	CN3PMeshInstance* PMeshInst() { return &m_PMeshInst; }
	void PMeshSet(const char* szFN);
	
	void Release();

	CN3CPlug();
	virtual ~CN3CPlug();
};

const int MAX_PLUG_TRACE = 2; // 최대 두개의 무기 궤적을 남긴다..
const int MAX_PLUG_TRACE_VERTEX = 24; // 무기 궤적 점의 수.. 점 8 개로는 잔상이 3단계로 남는다..

class CN3Chr : public CN3TransformCollision
{
public:
	static float s_fDistLODs[MAX_CHR_LOD];
	static CN3Mesh* s_pBoxMesh;

public:
	int		m_nLOD; // Level Of Detail - 강제로 세팅할수 있도록 한다..
	float	m_fFrmPrev; // 직전의 Frame

protected:
	CN3LinkedList<CN3CPart*>	m_Parts; // 각 캐릭터의 부분별 Data Pointer List
	CN3LinkedList<CN3CPlug*>	m_Plugs; // 이 캐릭터에 붙이는 무기등의 Data Pointer List
	CN3Joint*	m_pJointRef; // 한개의 뼈대만을 쓴다..
	CN3Joint*	m_pJointBlend; // 한개의 뼈대만을 쓴다..
	__Matrix44*	m_pMtxs; // 조인트에 대한 행렬
	__Matrix44*	m_pMtxInverses; // 조인트에 대한 역행렬
	
//	CN3Skin*	m_pSkinCollision;
	
	__VertexColor	m_vPlugTraces[MAX_PLUG_TRACE][MAX_PLUG_TRACE_VERTEX]; // 무기 궤적
	bool			m_bRenderPlugTraces[MAX_PLUG_TRACE];
	DWORD			m_dwTickPlugPrevs[MAX_PLUG_TRACE];
	
	CN3AnimControl* m_pAniCtrlRef; // Animation Control Reference Pointer
	CN3AnimControl::__AnimData* m_pAniCur;

	int		m_nAniCur; // 현재 에니메이션
	int		m_nAniPrev; // 전의 에니메이션..
	int		m_nAniLoopCountCur; // 에니메이션 루핑 카운트..

	float	m_fFrmCurUpper; // 현재 프레임 0 - 상체
	float	m_fFrmCurLower; // 현재 프레임 1 - 상체

	float m_fFrmBlendUpper;
	float m_fFrmBlendLower;
	float m_fBlendTimeCur; // 현재 블렌딩 시간..
	float m_fBlendTime; // 블렌딩할 시간. (초단위)

	__Vector3	m_vCollisionBoxes[8]; // 충돌 체크를 위한 폴리곤
	WORD		m_wCollisionIndices[36]; // 충돌 체크를 위한 폴리곤 인덱스

public:
	bool		CheckCollisionByBox(int xScreen, int yScreen);
	bool		CheckCollisionByBox(__Vector3& v0, __Vector3& v1, __Vector3 &vCollision, __Vector3 &vNormal);
	void		RemakePlugTracePolygons();
	__Matrix44 * MatrixGet(int nJointIndex) { return &m_pMtxs[nJointIndex]; }

//	void		CollisionSkinSet(const char* szFN);
//	CN3Skin*	CollisionSkin() { return m_pSkinCollision; }

	void		PartDelete(int nPart);
	int 		PartAdd();
	int			PartCount() { return m_Parts.Count(); }
	CN3CPart*	Part(int nPart) { if(nPart < 0 || nPart >= m_Parts.Count()) return NULL; return m_Parts.Get(nPart); }
	void		PartSet(int nPart, const char* szFN);

	void		PlugDelete(int nPlug);
	int			PlugAdd();
	int			PlugCount() { return m_Plugs.Count(); }
	CN3CPlug*	Plug(int nPlug) { if(nPlug < 0 || nPlug >= m_Plugs.Count()) return NULL; return m_Plugs.Get(nPlug); }
	void		PlugSet(int nPart, const char* szFN);

	void	Tick(float fFrm = FRAME_SELFPLAY);

	int		LODGet() { return m_nLOD; }

	CN3Joint*	Joint() { return m_pJointRef; }
	void		JointSet(const char* szFN);

	// Animation 관련 함수
	void	AniDefaultSet();
	CN3AnimControl* AniCtrl() { return m_pAniCtrlRef; }
	void	AniCtrlSet(const char* szFN);
	int		AniCurGet() { return m_nAniCur; }
	int		AniCurSet(int nAni, int nLoopCount = 0, float fBlendTime = 0.5f); // Animation 번호, 루프 카운트(0이면 무한루프, 1이상이면 카운트 만큼 실행하고 전의 걸로 돌아간다.), 블렌딩하는 시간(초단위)
//	int		AniCurSet(const char* szName, int nLoopCount);
	bool	IsLooping() { if(m_nAniLoopCountCur == 0) return true; else return false; }
	float	FrmCurUpper() { return m_fFrmCurUpper; }
	float	FrmCurLower() { return m_fFrmCurLower; }
	float	AniBlendDelta() { if(m_fBlendTime == 0) return 0.0f; return m_fBlendTimeCur / m_fBlendTime; }
	float	AniDeltaUpper()
	{
		if(NULL == m_pAniCur) return 0.0f;
		return (m_fFrmCurUpper - m_pAniCur->fFrmStartUpper)/(m_pAniCur->fFrmEndUpper - m_pAniCur->fFrmStartUpper); 
	}
	float	AniDeltaLower()
	{ 
		if(NULL == m_pAniCur) return 0.0f;
		return (m_fFrmCurLower - m_pAniCur->fFrmStartLower)/(m_pAniCur->fFrmEndLower - m_pAniCur->fFrmStartLower);
	}

	void Init();
	void BuildMesh();
	void Render(float fFrm);
	void Render2(float fFrm);

	bool Save(HANDLE hFile);
	bool Load(HANDLE hFile);

	void Release();
	CN3Chr();
	virtual ~CN3Chr();

};

#endif // !defined(AFX_N3Chr_h__INCLUDED_)
