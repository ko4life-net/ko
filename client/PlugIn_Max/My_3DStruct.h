#include "D3DX8.h"

struct __EXPORT_HEADER
{
	char szID[8];		// ID "N3Scene1"
	char szRemark[64];	// 설명..
	
	int nNodeCount; // 전체 노드 카운트
	int nStaticFrame; // 정지장면 한프레임..
	BOOL bExportCurrentFrame; // 전체 에니메이션을 
	int nFrmStart;	// 에니메이션 시작 프레임 
	int nFrmEnd;		// 에니메이션 끝 프레임

	D3DCOLORVALUE dcvBackground;	// 배경 색
	D3DCOLORVALUE dcvAmbientLight;	// 기본 조명 색

	int nCameraCount;	// scene 내의 카메라 갯수
	int nMaterialCount;	// scene 내의 재질 갯수
	int nTextureCount;	// scene 내의 텍스처 갯수
	int nLightCount;	// scene 내의 조명 갯수

	BOOL bObjGeometry;	// 지오메트리 데이터를 갖고 있다.
	BOOL bObjShape;		// 2D 폴리곤 데이터를 갖고 있다.
	BOOL bObjCamera;	// 카메라 데이터를 갖고 있다.
	BOOL bObjLight;		// 라이트 데이터를 갖고 있다.
	BOOL bObjDummy;		// 도우미 오브젝트(??? - 실제 겜에서는 필요없고 개발시에만 필요한 오브젝트를 말한다)데이터를 갖고 있다. 

	BOOL bMesh;						// 폴리곤 데이터를 갖고 있다.
	BOOL bMaterial;					// 재질 데이터를 갖고 있다.
	
	BOOL bAnimationKey;				// 에니메이션 키를 갖고 있다.
	BOOL bMeshAnimation;			// 폴리곤 에니메이션 키를 갖고 있다.
	BOOL bCameraAndLightAnimation;	// 카메라, 조명 에니메이션을 갖고 있다.
	BOOL bInverseKinematicsJoint;	// IK 관절들을 갖고 있다..

	BOOL bGenerateTexture; // 텍스처 파일을 자동으로 최적화 시켜서 생성 Direct3D 의 포맷에 맞게 2의 제곱수 단위로 맞추어서 "OBM" 비트맵 파일로 저장.
	BOOL bGenerateIndexedVertex; // 색인된 점으로 인코딩후 저장.. 메모리에 좀더 효율적이다..
	BOOL bGenerateSmoothNormal; // 부드럽게 보이도록 법선 벡터들을 재 계산한다.
};

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

