// N3Texture7.h: interface for the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Texture_h__INCLUDED_)
#define AFX_N3Texture_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CN3Texture : public CN3Base 
{
public:
	typedef struct __DXT_HEADER
	{
		char szID[4]; // "NTF2" - Noah Texture File Ver. 2.0
		int nWidth;
		int nHeight;
		D3DFORMAT Format; // 0 - 압축 안함 1 ~ 5 : D3DFMT_DXT1 ~ D3DFMT_DXT5
		BOOL bMipMap; // Mip Map ??
	} __DxtHeader;

protected:
	__DXT_HEADER m_Header;
	LPDIRECT3DTEXTURE8 m_lpTexture;

public:
	bool GenerateMipMap(LPDIRECT3DSURFACE8 lpSurf = NULL); // NULL 이면 0 레벨의 서피스로부터 생성..
	void UpdateRenderInfo();
	bool LoadFromFile(const char* szFileName);
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	DWORD		Width() { return m_Header.nWidth; }
	DWORD		Height() { return m_Header.nHeight; }
	D3DFORMAT	PixelFormat() { return m_Header.Format; }
	
	LPDIRECT3DTEXTURE8 Get() { return m_lpTexture; }
	operator LPDIRECT3DTEXTURE8 () { return m_lpTexture; }

	bool Convert(D3DFORMAT Format, int nWidth = 0, int nHeight = 0);

	bool CreateFromSurface(LPDIRECT3DSURFACE8 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap);
	bool Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap); // 장치에 맞게 생성
	
	void Release();

	CN3Texture();
	virtual ~CN3Texture();
};

#endif // !defined(AFX_N3Texture_h__INCLUDED_)
