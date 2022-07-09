#if !defined(_N3E2WRAPPER_H__INCUDED_)
#define _N3E2WRAPPER_H__INCUDED_

// stl 쓸려고...
#pragma warning(disable : 4786)

/*==============================================*/
/* Header file for the N3E2 wrapper class		*/
/*==============================================*/
#define MAYA_SRC

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream.h>

#include <maya/M3dView.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnTransform.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnReflectShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnMesh.h>
#include <maya/MMatrix.h>
#include <maya/MFnLight.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnCamera.h>
#include <maya/MFnSet.h>
#include <maya/MDagPathArray.h>
#ifndef MAYA1X
#include <maya/MAnimControl.h>
#endif
#include <maya/MTime.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h> 
#include <maya/MItDag.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MFloatArray.h>

#ifndef MAYA1X
#include <maya/MFnIkJoint.h>
#endif

#include <maya/MComputation.h>

#include <maya/ilib.h>

const float N3_PI = 3.141592f;

#ifdef WIN32
#ifndef MAXPATHLEN
#define MAXPATHLEN					512
#endif
#endif


#include "MAYA\MString.h"	// Added by ClassView
#include "MAYA\MObject.h"	// Added by ClassView

#include "../N3Base/My_3DStruct.h"
#include "../N3Base/N3EngTool.h"
#include "../N3Base/N3Scene.h"

const int MAX_TEXTURE = 1024;

class CN3E2Wrapper
{
private:
	// different extensions will get added to this:
	char	m_szPath[1024];		// 경로 이름
	char	m_szFileName[1024];	// 파일 이름
	
	CN3Scene*		m_pScene;
	static __EXPORT_OPTION m_Option;
	HWND	m_hDlgProgress, m_hWndPB, m_hWndLB;
	
protected:
	static BOOL m_bCancelExport; // DialogBox...


	MObject MeshGetShader(MFnMesh &pMMesh);

	CN3Texture* ProcessTexture(MFnMesh &mMesh); // 텍스처 인덱스값이 리턴된다..
	CN3Joint*	ProcessJoint(MFnIkJoint& mJoint);
	bool		ProcessIMesh(MFnMesh& mMesh, CN3IMesh* pIMesh);
	bool		ProcessVMesh(MFnMesh &mMesh, CN3VMesh* pVMesh);
	bool		ProcessSkin(MFnSkinCluster& mSkin, class CN3Skin* pSkin);

	CN3Camera*	ProcessCamera(MFnCamera &mCamera);
	CN3Light*	ProcessLight(MFnLight &mLight);

	bool 		ProcessShape(MFnMesh& mMesh);
	bool		ProcessChr(MFnSkinCluster &mSkin);

//	CN3TMesh*	ProcessTMesh(MFnMesh &mMesh);
	int ProcessMaterial(MObject mShaderObj, __Material* pMtl);
	int ProcessTransform(MFnTransform &mTransform, class CN3Transform* pTransform);

public:
	bool IsSelected(MSelectionList& mSelList, MObject mObj);
	void GetWorldTransform(MFnTransform& mTransform, MMatrix& mMtx);
	void ProcessName(MObject mObj, std::string& szName);
	void ProcessAnimKey(	MFnAnimCurve* pmACs,
							class CN3AnimKey* pKey,
							bool bReverseZ,
							float fScale,
							bool bQuaternion,
							MTransformationMatrix::RotationOrder mRotOrder = MTransformationMatrix::kYXZ);
	void ProcessJointTransform(CN3Joint *pJoint, __Vector3* pvTrans, __Quaternion* pqRot, __Vector3* pvScale, bool bProcessChild);
	bool HaveJoint(MFnMesh &mObj);
	bool FindAnimCurve(MObject &mObj, MObjectArray &mAnimCurveArray);
	void ProcessMatrix(MFnTransform &mTransform, __Matrix44& mtx, __Vector3& vPos, __Quaternion& qtRot, __Vector3& vScale);
	void SceneExport();

	void Release();
	void SetPath(const char* szPath);
	void SetFileName(const char* szFileName);

	static BOOL CALLBACK DlgProcProgress(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgProcPane(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	CN3E2Wrapper();
	virtual	~CN3E2Wrapper();
};

#endif // #if !defined(_N3E2WRAPPER_H__INCUDED_)
