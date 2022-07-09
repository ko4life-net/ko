// N3Base.h: interface for the CN3Base class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Base_h__INCLUDED_)
#define AFX_N3Base_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "My_3DStruct.h"
#include "N3Mng.h"
#include <stdio.h>
#include <mmsystem.h>

class CN3Texture;
class CN3Mesh;
class CN3VMesh;
class CN3PMesh;
class CN3Joint;
class CN3CPart;
class CN3AnimControl;

const DWORD TEX_CAPS_DXT1 =			0x00000001;
const DWORD TEX_CAPS_DXT2 =			0x00000002;
const DWORD TEX_CAPS_DXT3 =			0x00000004;
const DWORD TEX_CAPS_DXT4 =			0x00000008;
const DWORD TEX_CAPS_DXT5 =			0x00000010;
const DWORD TEX_CAPS_SQUAREONLY =	0x00000020;
const DWORD TEX_CAPS_MIPMAP =		0x00000040;
const DWORD TEX_CAPS_POW2 =			0x00000080;

const int MAX_CAMERA_RADIUS = 64;

class CN3Base  
{
public:
	static CN3Mng<CN3Texture>	s_MngTex; // Texture Manager
	static CN3Mng<CN3Mesh>		s_MngMesh; // Normal Mesh Manager
	static CN3Mng<CN3VMesh>		s_MngVMesh; // 단순히 폴리곤만 갖고 있는 메시 - 주로 충돌 체크에 쓴다..
	static CN3Mng<CN3PMesh>		s_MngPMesh; // Progressive Mesh Manager
	static CN3Mng<CN3Joint>		s_MngJoint; // Joint Manager
	static CN3Mng<CN3CPart>		s_MngCPart; // Character 에 쓰는 Part Manager
	static CN3Mng<CN3AnimControl>	CN3Base::s_MngAniCtrl; // Animation Manager

	typedef struct __CameraData
	{
		__Vector3 vEye; // Camera Position Vector
		__Vector3 vAt; // Camera At Vector
		__Vector3 vUp; // Camera Up Vector
		float fEyeDotValues[MAX_CAMERA_RADIUS][5];	// Camera Eye Vector - Camera Direction Vector 를 Dot Porduct 한값.. 
													// 첫번째 배열[MAX_CAMERA_RADIUS]- 3 meter 단위로 MAX_CAMERA_RADIUS개, 즉 카메라를 뒤로 MAX_CAMERA_RADIUS * 10 meter 까지 뺀 거리를 커버할수 있다는 의미이다.
													// 두번째 배열[5] - 카메라 위치. 1~4:Left, Right, Top, Bottom
		
		__Vector3 vFrustumNormals[5]; // 카메라 절두체 평면의 법선 벡터.. 0:Camera 위치 1~4:Left, Right, Top, Bottom , 각 평면의 위치는 vPos 이다..
		__Vector3 vFarPoints[4]; // LeftTop, RightTop, BottomRight, BottomLeft

		float fFOV; // 카메라 렌즈 각 : Field Of View
		float fAspect; // 종횡비
		float fNP; // NearPlane
		float fFP; // FarPlane
		D3DVIEWPORT8 vp; // ViewPort;
		__Matrix44 mtxView;
		__Matrix44 mtxViewInverse;
		__Matrix44 mtxProjection;

		BOOL IsOutOfFrustum(__Vector3& vPosition, float fRadius) // 미리 계산된 카메라 평면의 도트 프로덕트 값을 기준으로 카메라 사면체 밖에 있으면  참을 돌려준다.
		{
			int nEyeDotIndex = (int)(fRadius / 3.0f); // 3 Meter 단위로 미리 계산해 놓는다..
			if(nEyeDotIndex < 0) nEyeDotIndex = 0;
			else if(nEyeDotIndex >= MAX_CAMERA_RADIUS) nEyeDotIndex = MAX_CAMERA_RADIUS - 1;
			if(	vPosition.Dot(vFrustumNormals[0]) < fEyeDotValues[nEyeDotIndex][0] || // Front...
				vPosition.Dot(vFrustumNormals[1]) < fEyeDotValues[nEyeDotIndex][1] || // Left
				vPosition.Dot(vFrustumNormals[2]) < fEyeDotValues[nEyeDotIndex][2] || // Right
				vPosition.Dot(vFrustumNormals[3]) < fEyeDotValues[nEyeDotIndex][3] || // Top
				vPosition.Dot(vFrustumNormals[4]) < fEyeDotValues[nEyeDotIndex][4] ) // Bottom
			{
				return TRUE;
			}
			
			return FALSE;
		}
	} __CameraData;

	typedef struct __RenderInfo
	{
		int nShape;
		int nShapePart;
		int nChr;
		int nChrPart;

		int nPolygon; // 단순 폴리곤
		int nPolygonCharacter; // 캐릭터 폴리곤
		
		int nTexture_32X32; // 32 X 32 Texture
		int nTexture_64X64; // 64 X 64 Texture
		int nTexture_128X128; // 128 X 128 Texture
		int nTexture_256X256; // 256 X 256 Texture
		int nTexture_512X512; // 512 X 512 Texture
		int nTexture_Huge; // 512 X 512 이상 size
		int nTexture_OtherSize; // Other size
	} __RenderInfo;

	typedef struct __ResrcInfo
	{
		int nTexture_Loaded_32X32; // 32 X 32 Texture
		int nTexture_Loaded_64X64; // 64 X 64 Texture
		int nTexture_Loaded_128X128; // 128 X 128 Texture
		int nTexture_Loaded_256X256; // 256 X 256 Texture
		int nTexture_Loaded_512X512; // 512 X 512 Texture
		int nTexture_Loaded_Huge; // 512 X 512 이상 size
		int nTexture_Loaded_OtherSize; // Other size
	} __ResrcInfo;


	static LPDIRECT3DDEVICE8		s_lpD3DDev; // Device 참조 포인터.. 멋대로 해제하면 안된다..
	static D3DPRESENT_PARAMETERS	s_DevParam; // Device 생성 Present Parameter
	static D3DCAPS8	s_DevCaps; // Device 호환성...
	static DWORD	s_dwTextureCaps; // Texture 지원.. DXT1 ~ DXT5, Square Only
	static HWND		s_hWndDevice; // Device Window Handle
	static HWND		s_hWndBase; // Init 할때 쓴 Window Handle
	static HWND		s_hWndPresent; // 최근에 Present 한 Window Handle

	static __CameraData	s_CameraData; // 카메라 데이터 정적 변수..
	static __RenderInfo	s_RenderInfo; // Rendering Information..
	static __ResrcInfo	s_ResrcInfo; // Rendering Information..
	static float		s_fFrmPerSec; // Frame Per Second

protected:
	static char				s_szPath[512]; // 프로그램이 실행된 경로.. 
	static LARGE_INTEGER	s_TimePrev; // 시간..

protected:
	DWORD	m_dwType; // "MESH", "CAMERA", "SCENE", "???" .... 등등등...
	char*	m_szName;
	char*	m_szFileName; // Base Path 를 제외한 로컬 경로 + 파일 이름

public:
	float TimeGet();
	static const char* PathGet() { return s_szPath; }
	static void PathSet(const char* szPath);
	
	void ReleaseResrc();
	void SaveResrc();
	
	void TimerProcess();

	const char* Name() { return m_szName; }
	void NameSet(const char* szName);

	const char* FileNameShort(); // 베이스 경로를 제외한 짧은 이름을 돌려준다..(로컬 경로이다..)
	const char* FileName() { return m_szFileName; } // Full Path
	void FileNameSet(const char* szFileName);

	DWORD Type() { return m_dwType; } // 객체 종류..

	bool LoadFromFile(); // 파일에서 읽어오기.
	bool LoadFromFile(const char* szFileName); // 파일에서 읽어오기.
	bool SaveToFile(); // 현재 파일 이름대로 저장.
	bool SaveToFile(const char* szFileName); // 새이름으로 저장.

	virtual bool Load(HANDLE hFile); // 핸들에서 읽어오기..
	virtual bool Save(HANDLE hFile); // 핸들을 통해 저장..

	void RenderLines(const __Vector3 *pvLines, int nCount, D3DCOLOR color);

	virtual void Release();
	CN3Base();
	virtual ~CN3Base();
};

#endif // !defined(AFX_N3Base_h__INCLUDED_)
