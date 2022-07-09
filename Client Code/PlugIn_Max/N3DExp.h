/**********************************************************************
	FILE: CN3DExp.h

	DESCRIPTION:	Template Utility

	CREATED BY:

	HISTORY:

 	Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/

#ifndef __CN3DEXP__H
#define __CN3DEXP__H

#include "stdafx.h"

#include "../N3Base/N3EngTool.h"
#include "../N3Base/N3Scene.h"

#include "Bipexp.h" // biped
#include "Phyexp.h" // physique

#include "Max.h"
#include "resource.h"

#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "decomp.h"

#include <direct.h>
#include <commdlg.h>

#include "stdmat.h"
#include "bitmap.h"

#include "D3D8.h"
#include "D3D8Types.h"

//#include "ISkin.h"

//--Name : Plugin Unique ID----------------------------------------
#define CN3DEXP_CLASS_ID	Class_ID(0x4fc463a7, 0x3b1a661e)
//-----------------------------------------------------------------

extern TCHAR *GetString(int id);
extern HINSTANCE g_hInst;

struct __UVWH
{
	float	fV;
	float	fU;
	float	fW;
	float	fH;

	float	fUOffset;
	float	fVOffset;
	float	fUTile;
	float	fVTile;
};

#define BIP_BONE_CLASS_ID	0x00009125

//------------------------------------------------------------------------------------------------------
// Name : CN3DExp
// Desc : class CN3DExp : public SceneExport(Max SDK)
//------------------------------------------------------------------------------------------------------
class CN3DExp : public SceneExport
{
public:

	char			m_szPath[1024];		// Path Name
	char			m_szFileName[1024];	// File Name
	CN3Scene*		m_pScene;			// N3Scene

protected:
	static __EXPORT_OPTION m_Option;	// Export Option - Window Procedure 땜시 Static 으로 쓴다..
	BOOL			m_bCancelExport;	// TRUE?->Cancel
	int				m_nNodeCur;			// Current Processing Node Count
	int				m_nNodeCount;		// Total Node Count
	int				m_nLightCount;
	static HWND		hParams;

public:
	void CountAllNodes(INode* pNode, int& nCount);
	void Init();
	
	bool ProcessIMesh(INode* pNode, CN3IMesh* pIMesh);
	bool ProcessMaterial(INode* pNode, __Material* pMtl, CN3Texture** pTex, LPCTSTR pszDir);
	bool ProcessRecursive(INode* pNode);					// Parent Node -> Child Node
	bool ProcessName(INode* pNode, CN3BaseFileAccess* pBase);		// Process Make Name
	bool ProcessCamera(INode* pNode);					// Export Camera Data
	bool ProcessLight(INode* pNode);						// Export Light Data
	bool ProcessShape(INode *pNode);						// Export Shape (Mesh,PMesh,Texture)
	bool ProcessChr(INode* pNode);
	bool ProcessJoint(INode* pNode, CN3Joint* pJoint);
	
	bool IsBone(INode *pNode);
	bool FindNodeRecursive(INode* pNode, Class_ID cID, std::list<INode*>& list);
	bool CheckObjectClassID(INode* pNode, const Class_ID& cID) const;	// 주어진 노드의 오브젝트가 주어진 클래스 아이디인지 검사
	Modifier* FindPhysiqueModifier(INode* pNode);
//	bool FindNodeIndex(INode* pNodeCompare, INode* pNodeSrc, Class_ID& cID, int& nNodeIndex);
	bool FindBoneIndex(INode* pNodeCompare, INode* pNodeSrc, int& nNodeIndex);
	bool ProcessPhysique(INode* pNode, INode* pNodeRootJoint, CN3Skin* pSkin);

	bool ProcessTransform(INode* pNode, CN3Transform* pTransform, bool bLocalCoordinate); // Animation Key

	/* plan add function
	void ExportMesh
	void ExportPMesh
	void ExportTexture
	*/
	static BOOL CALLBACK DlgProcExportOption(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI DlgProcProgress(LPVOID lpArg);
	
	void CancelExport();
	void DecodeTransformMatrix(Matrix3& matSrc, __Matrix44& matDest);

	int				ExtCount();							// Number of extensions supported
	const TCHAR *	Ext(int n);							// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();							// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();						// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();						// ASCII Author name
	const TCHAR *	CopyrightMessage();					// ASCII Copyright message
	const TCHAR *	OtherMessage1();					// Other message #1
	const TCHAR *	OtherMessage2();					// Other message #2
	unsigned int	Version();							// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);				// Show DLL's "About..." box
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);	// Export file

	CN3DExp();											//Constructor
	~CN3DExp();											//Destructor	
};


//------------------------------------------------------------------------------------------------------
// Name : CN3DExpClassDesc
// Desc : class CN3DExpClassDesc : public ClassDesc
//------------------------------------------------------------------------------------------------------
class CN3DExpClassDesc : public ClassDesc
{
public:
	int 			IsPublic() { return 1; }
	void*			Create(BOOL loading = FALSE) { return new CN3DExp; }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return CN3DEXP_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }
	const TCHAR*	InternalName() { return _T("N3DExp"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return g_hInst; }			// returns owning module handle
};

#endif // __CN3DEXP__H
