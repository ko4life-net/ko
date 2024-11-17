// N3Texture7.h: interface for the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"
#include <string>

class CN3Texture : public CN3BaseFileAccess {
  public:
    typedef struct __DXT_HEADER {
        char      szID[4]; // "NTF"숫자 - Noah Texture File Ver. ?.0
        int       nWidth;
        int       nHeight;
        D3DFORMAT Format;  // 0 - 압축 안함 1 ~ 5 : D3DFMT_DXT1 ~ D3DFMT_DXT5
        BOOL      bMipMap; // Mip Map ??
    } __DxtHeader;

  protected:
    __DXT_HEADER       m_Header;
    LPDIRECT3DTEXTURE9 m_lpTexture;

  public:
    void UpdateRenderInfo();
    bool LoadFromFile(const fs::path & fsFile);
    bool Load(HANDLE hFile);
    bool SkipFileHandle(HANDLE hFile);

#ifdef _N3TOOL
    bool GenerateMipMap(LPDIRECT3DSURFACE9 lpSurf = NULL); // NULL 이면 0 레벨의 서피스로부터 생성..
    bool Convert(D3DFORMAT Format, int nWidth = 0, int nHeight = 0, BOOL bGenerateMipMap = TRUE);
    //#ifdef _N3TOOL
    bool SaveToFile();
    bool SaveToFile(const fs::path & fsFile);
    bool Save(HANDLE hFile);
    bool SaveToBitmapFile(const fs::path & fsFile); // Save as 24-bit bitmap file
    bool CreateFromSurface(LPDIRECT3DSURFACE9 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap);
#endif // end of _N3TOOL

    DWORD     Width() { return m_Header.nWidth; }
    DWORD     Height() { return m_Header.nHeight; }
    D3DFORMAT PixelFormat() { return m_Header.Format; }
    int       MipMapCount() {
        if (NULL == m_lpTexture) {
            return 0;
        } else {
            return m_lpTexture->GetLevelCount();
        }
    }

    bool Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap); // 장치에 맞게 생성
    LPDIRECT3DTEXTURE9 Get() { return m_lpTexture; }
                       operator LPDIRECT3DTEXTURE9() { return m_lpTexture; }

    void Release();
    CN3Texture();
    virtual ~CN3Texture();
};
