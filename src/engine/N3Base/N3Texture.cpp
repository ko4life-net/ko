// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3Texture.h"

#ifdef _N3TOOL
#include "BitMapFile.h"
#endif // #ifdef _N3TOOL

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Texture::CN3Texture() {
    m_dwType |= OBJ_TEXTURE;

    memset(&m_Header, 0, sizeof(m_Header));
    m_lpTexture = NULL;
    m_iLOD = 0;
}

CN3Texture::~CN3Texture() {
    if (m_lpTexture) {
        int nRefCount = m_lpTexture->Release();
        if (nRefCount == 0) {
            m_lpTexture = NULL;
        }
    }
}

void CN3Texture::Release() {
    if (32 == m_Header.nWidth && 32 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_32X32--;
    } else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_64X64--;
    } else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_128X128--;
    } else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_256X256--;
    } else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_512X512--;
    } else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_Huge--;
    } else {
        s_ResrcInfo.nTexture_Loaded_OtherSize--;
    }

    memset(&m_Header, 0, sizeof(m_Header));
    if (m_lpTexture && m_lpTexture->Release() == 0) {
        m_lpTexture = NULL;
    }
    m_iLOD = 0;

    CN3BaseFileAccess::Release();
}

bool CN3Texture::Create(int nWidth, int nHeight, D3DFORMAT Format, BOOL bGenerateMipMap) {
    if (nWidth != nHeight) {
        if (nWidth <= 1 || nHeight <= 1 || D3DFMT_UNKNOWN == Format) {
            return false;
        }
    }
    if (m_lpTexture != NULL) {
        this->Release();
    }

    if (s_dwTextureCaps & TEX_CAPS_POW2) // 2 의 승수만 된다면..
    {
        int nW = 1, nH = 1;
        for (; nW <= nWidth; nW *= 2) {
            ;
        }
        nW /= 2;
        for (; nH <= nHeight; nH *= 2) {
            ;
        }
        nH /= 2;

        nWidth = nW;
        nHeight = nH;
    }

    if ((s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // 정사각형 텍스처만 되면..
    {
        if (nWidth > nHeight) {
            nWidth = nHeight;
        } else {
            nHeight = nWidth;
        }
    }

    // 비디오 카드가 256 이상의 텍스처를 지원 하지 못하면..
    if (nWidth > 256 && CN3Base::s_DevCaps.MaxTextureWidth <= 256) {
        nWidth = CN3Base::s_DevCaps.MaxTextureWidth;
    }
    if (nHeight > 256 && CN3Base::s_DevCaps.MaxTextureHeight <= 256) {
        nHeight = CN3Base::s_DevCaps.MaxTextureHeight;
    }

    // 헤더 세팅..
    memset(&m_Header, 0, sizeof(m_Header));

    // MipMap 단계 결정..
    // 4 X 4 픽셀까지만 MipMap 을 만든다..
    int nMMC = 1;
    if (bGenerateMipMap) {
        nMMC = 0;
        for (int nW = nWidth, nH = nHeight; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) {
            nMMC++;
        }
    }

    HRESULT rval = s_lpD3DDev->CreateTexture(nWidth, nHeight, nMMC, 0, Format, D3DPOOL_MANAGED, &m_lpTexture, NULL);

#ifdef _N3GAME
    if (rval == D3DERR_INVALIDCALL) {
        CLogWriter::Write("N3Texture: createtexture err D3DERR_INVALIDCALL(%s)", FilePath().string().c_str());
        return false;
    } else if (rval == D3DERR_OUTOFVIDEOMEMORY) {
        CLogWriter::Write("N3Texture: createtexture err D3DERR_OUTOFVIDEOMEMORY(%s)", FilePath().string().c_str());
        return false;
    } else if (rval == E_OUTOFMEMORY) {
        CLogWriter::Write("N3Texture: createtexture err E_OUTOFMEMORY(%s)", FilePath().string().c_str());
        return false;
    }
#endif
    if (NULL == m_lpTexture) {
        __ASSERT(m_lpTexture, "Texture pointer is NULL!");
        return false;
    }

    m_Header.nWidth = nWidth;
    m_Header.nHeight = nHeight;
    m_Header.Format = Format;
    m_Header.bMipMap = bGenerateMipMap;

    if (32 == m_Header.nWidth && 32 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_32X32++;
    } else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_64X64++;
    } else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_128X128++;
    } else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_256X256++;
    } else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_512X512++;
    } else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) {
        s_ResrcInfo.nTexture_Loaded_Huge++;
    } else {
        s_ResrcInfo.nTexture_Loaded_OtherSize++;
    }

    return true;
}

#ifdef _N3TOOL
bool CN3Texture::CreateFromSurface(LPDIRECT3DSURFACE9 lpSurf, D3DFORMAT Format, BOOL bGenerateMipMap) {
    if (lpSurf == NULL) {
        return false;
    }

    D3DSURFACE_DESC sd;
    lpSurf->GetDesc(&sd);

    if (this->Create(sd.Width, sd.Height, Format, bGenerateMipMap) == false) {
        return false;
    }
    if (bGenerateMipMap) {
        this->GenerateMipMap(lpSurf);
    }

    return true;
}
#endif // end of _N3TOOL

bool CN3Texture::LoadFromFile(const fs::path & fsFile) {
    if (m_lpTexture != NULL) {
        this->Release();
    }

    bool bSuccess = false;

    this->FilePathSet(fsFile);
    if (n3std::iequals(fsFile.extension(), ".dxt")) {
        HANDLE hFile =
            ::CreateFileW(FilePathAbs().c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
#ifdef _N3GAME
            CLogWriter::Write("invalid file handle(%d) - Can't open texture file(%s)", (int)hFile,
                              FilePathAbs().string().c_str());
#endif
            return false;
        }
        bSuccess = this->Load(hFile);
        CloseHandle(hFile);
    } else {
        D3DXIMAGE_INFO ImgInfo;
        HRESULT        rval =
            D3DXCreateTextureFromFileExW(s_lpD3DDev, FilePathAbs().c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                                         D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
                                         D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 0, &ImgInfo, NULL, &m_lpTexture);
        if (rval == D3D_OK) {
            D3DSURFACE_DESC sd;
            m_lpTexture->GetLevelDesc(0, &sd);

            m_Header.nWidth = sd.Width;
            m_Header.nHeight = sd.Height;
            m_Header.Format = sd.Format;
            bSuccess = true;
        } else {
#ifdef _N3GAME
            CLogWriter::Write("N3Texture - Failed to load texture(%s)", FilePathAbs().string().c_str());
#endif
        }

        if (32 == m_Header.nWidth && 32 == m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_32X32++;
        } else if (64 == m_Header.nWidth && 64 == m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_64X64++;
        } else if (128 == m_Header.nWidth && 128 == m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_128X128++;
        } else if (256 == m_Header.nWidth && 256 == m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_256X256++;
        } else if (512 == m_Header.nWidth && 512 == m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_512X512++;
        } else if (512 < m_Header.nWidth && 512 < m_Header.nHeight) {
            s_ResrcInfo.nTexture_Loaded_Huge++;
        } else {
            s_ResrcInfo.nTexture_Loaded_OtherSize++;
        }
    }

    if (!m_lpTexture) {
        this->Release();
        return false;
    }

    return bSuccess;
}

bool CN3Texture::Load(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);

    DWORD dwRWC = 0;

    __DXT_HEADER HeaderOrg;                                       // 헤더를 저장해 놓고..
    ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // 헤더를 읽는다..
    if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] ||
        3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
    {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", FilePath().string().c_str());
#endif
    }

    // DXT Format 을 읽어야 하는데 지원이 되는지 안되는지 보고 지원안되면 대체 포맷을 정한다.
    bool      bDXTSupport = FALSE;
    D3DFORMAT fmtNew = HeaderOrg.Format;
    if (D3DFMT_DXT1 == HeaderOrg.Format) {
        if (s_dwTextureCaps & TEX_CAPS_DXT1) {
            bDXTSupport = true;
        } else {
            fmtNew = D3DFMT_A1R5G5B5;
        }
    } else if (D3DFMT_DXT2 == HeaderOrg.Format) {
        if (s_dwTextureCaps & TEX_CAPS_DXT2) {
            bDXTSupport = true;
        } else {
            fmtNew = D3DFMT_A4R4G4B4;
        }
    } else if (D3DFMT_DXT3 == HeaderOrg.Format) {
        if (s_dwTextureCaps & TEX_CAPS_DXT3) {
            bDXTSupport = true;
        } else {
            fmtNew = D3DFMT_A4R4G4B4;
        }
    } else if (D3DFMT_DXT4 == HeaderOrg.Format) {
        if (s_dwTextureCaps & TEX_CAPS_DXT4) {
            bDXTSupport = true;
        } else {
            fmtNew = D3DFMT_A4R4G4B4;
        }
    } else if (D3DFMT_DXT5 == HeaderOrg.Format) {
        if (s_dwTextureCaps & TEX_CAPS_DXT5) {
            bDXTSupport = true;
        } else {
            fmtNew = D3DFMT_A4R4G4B4;
        }
    }

    int iWCreate = HeaderOrg.nWidth, iHCreate = HeaderOrg.nHeight;
    if (fmtNew != HeaderOrg.Format) {
        iWCreate /= 2;
        iHCreate /= 2;
    } // DXT 지원이 안되면 너비 높이를 줄인다.
    if (m_iLOD > 0 && m_iLOD <= 2 && HeaderOrg.nWidth >= 16 && HeaderOrg.nHeight >= 16) // LOD 만큼 작게 생성..
    {
        for (int i = 0; i < m_iLOD; i++) {
            iWCreate /= 2;
            iHCreate /= 2;
        }
    } else {
        m_iLOD = 0; // LOD 적용이 아니면..
    }

    int iLODPrev = m_iLOD;
    this->Create(iWCreate, iHCreate, fmtNew, HeaderOrg.bMipMap); // 서피스 만들고..
    m_iLOD = iLODPrev;

    if (m_lpTexture == NULL) {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture error - Can't create texture (%s)", FilePath().string().c_str());
#endif
        return false;
    }

    D3DSURFACE_DESC sd;
    D3DLOCKED_RECT  LR;
    int             iMMC = m_lpTexture->GetLevelCount(); // 생성한 MipMap 수

    // 압축 포맷이면..
    if (D3DFMT_DXT1 == HeaderOrg.Format || D3DFMT_DXT2 == HeaderOrg.Format || D3DFMT_DXT3 == HeaderOrg.Format ||
        D3DFMT_DXT4 == HeaderOrg.Format || D3DFMT_DXT5 == HeaderOrg.Format) {
        if (TRUE == bDXTSupport) // 압축 텍스처 지원이면..
        {
            if (iMMC > 1) {
                if (m_iLOD > 0) // LOD 만큼 건너뛰기...
                {
                    int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                    for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                        if (D3DFMT_DXT1 == HeaderOrg.Format) {
                            iSkipSize += iWTmp * iHTmp / 2; // DXT1 형식은 16비트 포맷에 비해 1/4 로 압축..
                        } else {
                            iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 형식은 16비트 포맷에 비해 1/2 로 압축..
                        }
                    }
                    ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
                }

                for (int i = 0; i < iMMC; i++) {
                    m_lpTexture->GetLevelDesc(i, &sd);
                    m_lpTexture->LockRect(i, &LR, NULL, NULL);

                    int iTexBytesLength = 0;
                    iTexBytesLength = sd.Format == D3DFMT_DXT1 ? (sd.Width * sd.Height / 2) : (sd.Width * sd.Height);
                    ReadFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
                    m_lpTexture->UnlockRect(i);
                }

                // 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기..
                int iWTmp = HeaderOrg.nWidth / 2, iHTmp = HeaderOrg.nHeight / 2;
                for (; iWTmp >= 4 && iHTmp >= 4;
                     iWTmp /= 2,
                     iHTmp /= 2) { // 한픽셀에 두바이트가 들어가는 A1R5G5B5 혹은 A4R4G4B4 포맷으로 되어 있다..
                    ::SetFilePointer(hFile, iWTmp * iHTmp * 2, 0, FILE_CURRENT); // 건너뛰고.
                }
            } else // pair of if(iMMC > 1)
            {
                m_lpTexture->GetLevelDesc(0, &sd);
                m_lpTexture->LockRect(0, &LR, NULL, NULL);

                int iTexBytesLength = 0;
                iTexBytesLength = sd.Format == D3DFMT_DXT1 ? (sd.Width * sd.Height / 2) : (sd.Width * sd.Height);
                ReadFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
                m_lpTexture->UnlockRect(0);

                // 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기..
                ::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight / 4, 0, FILE_CURRENT); // 건너뛰고.
                if (HeaderOrg.nWidth >= 1024) {
                    SetFilePointer(hFile, 256 * 256 * 2, 0,
                                   FILE_CURRENT); // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
                }
            }
        } else // DXT 지원이 안되면..
        {
            if (iMMC > 1) // LOD 만큼 건너뛰기...
            {
                // 압축 데이터 건너뛰기..
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) {
                    if (D3DFMT_DXT1 == HeaderOrg.Format) {
                        iSkipSize += iWTmp * iHTmp / 2; // DXT1 형식은 16비트 포맷에 비해 1/4 로 압축..
                    } else {
                        iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 형식은 16비트 포맷에 비해 1/2 로 압축..
                    }
                }
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.

                // LOD 만큼 건너뛰기..
                iWTmp = HeaderOrg.nWidth / 2;
                iHTmp = HeaderOrg.nHeight / 2;
                iSkipSize = 0;
                if (m_iLOD > 0) {
                    for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                        iSkipSize += iWTmp * iHTmp * 2; // 피치에 너비를 나눈게 픽셀의 크기라 생각한다...
                    }
                }

                // 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
                for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight;
                     iWTmp /= 2, iHTmp /= 2) {
                    iSkipSize += iWTmp * iHTmp * 2;
                }
                if (iSkipSize) {
                    ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
                }

                for (int i = 0; i < iMMC; i++) {
                    m_lpTexture->GetLevelDesc(i, &sd);
                    m_lpTexture->LockRect(i, &LR, NULL, NULL);
                    int nH = sd.Height;
                    for (int y = 0; y < nH; y++) {
                        ReadFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, 2 * sd.Width, &dwRWC, NULL);
                    }
                    m_lpTexture->UnlockRect(i);
                }
            } else // pair of if(iMMC > 1)
            {
                // 압축 데이터 건너뛰기..
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                if (D3DFMT_DXT1 == HeaderOrg.Format) {
                    iSkipSize = iWTmp * iHTmp / 2; // DXT1 형식은 16비트 포맷에 비해 1/4 로 압축..
                } else {
                    iSkipSize = iWTmp * iHTmp; // DXT2 ~ DXT5 형식은 16비트 포맷에 비해 1/2 로 압축..
                }
            }
        }
    } else {
        int iPixelSize = 0;
        if (fmtNew == D3DFMT_A1R5G5B5 || fmtNew == D3DFMT_A4R4G4B4) {
            iPixelSize = 2;
        } else if (fmtNew == D3DFMT_R8G8B8) {
            iPixelSize = 3;
        } else if (fmtNew == D3DFMT_A8R8G8B8 || fmtNew == D3DFMT_X8R8G8B8) {
            iPixelSize = 4;
        } else {
            __ASSERT(0, "Not supported texture format");
        }

        if (iMMC > 1) {
            if (m_iLOD > 0) // LOD 만큼 건너뛰기...
            {
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                    iSkipSize += iWTmp * iHTmp * iPixelSize; // 피치에 너비를 나눈게 픽셀의 크기라 생각한다...
                }
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
            }

            // 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
            int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
            for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2) {
                iSkipSize += iWTmp * iHTmp * iPixelSize;
            }
            if (iSkipSize) {
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
            }

            // 데이터 읽기..
            for (int i = 0; i < iMMC; i++) {
                m_lpTexture->GetLevelDesc(i, &sd);
                m_lpTexture->LockRect(i, &LR, NULL, NULL);
                for (int y = 0; y < (int)sd.Height; y++) {
                    ReadFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
                }
                m_lpTexture->UnlockRect(i);
            }
        } else // pair of if(iMMC > 1)
        {
            // 비디오 카드 지원 텍스처 크기가 작을경우 건너뛰기..
            if (HeaderOrg.nWidth >= 512 && m_Header.nWidth <= 256) {
                ::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight * iPixelSize, 0,
                                 FILE_CURRENT); // 건너뛰고.
            }

            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->LockRect(0, &LR, NULL, NULL);
            for (int y = 0; y < (int)sd.Height; y++) {
                ReadFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
            }
            m_lpTexture->UnlockRect(0);

            if (m_Header.nWidth >= 512 && m_Header.nHeight >= 512) {
                SetFilePointer(hFile, 256 * 256 * 2, 0,
                               FILE_CURRENT); // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
            }
        }
    }
    //    this->GenerateMipMap(); // Mip Map 을 만든다..
    return true;
}

bool CN3Texture::SkipFileHandle(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);

    __DXT_HEADER HeaderOrg; // 헤더를 저장해 놓고..
    DWORD        dwRWC = 0;
    ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // 헤더를 읽는다..
    if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] ||
        3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
    {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", FilePath().string().c_str());
#endif
    }

    // 압축 포맷이면..
    if (D3DFMT_DXT1 == HeaderOrg.Format || D3DFMT_DXT2 == HeaderOrg.Format || D3DFMT_DXT3 == HeaderOrg.Format ||
        D3DFMT_DXT4 == HeaderOrg.Format || D3DFMT_DXT5 == HeaderOrg.Format) {
        int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
        if (HeaderOrg.bMipMap) {
            // 압축 데이터 건너뛰기..
            for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) {
                if (D3DFMT_DXT1 == HeaderOrg.Format) {
                    iSkipSize += iWTmp * iHTmp / 2;
                } else {
                    iSkipSize += iWTmp * iHTmp;
                }
            }
            // 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기..
            iWTmp = HeaderOrg.nWidth / 2;
            iHTmp = HeaderOrg.nHeight / 2;
            for (; iWTmp >= 4 && iHTmp >= 4;
                 iWTmp /= 2, iHTmp /= 2) { // 한픽셀에 두바이트가 들어가는 A1R5G5B5 혹은 A4R4G4B4 포맷으로 되어 있다..
                iSkipSize += iWTmp * iHTmp * 2; // 건너뛰고.
            }
        } else // pair of if(HeaderOrg.bMipMap)
        {
            // 압축 데이터 건너뛰기..
            if (D3DFMT_DXT1 == HeaderOrg.Format) {
                iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight / 2;
            } else {
                iSkipSize += iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight;
            }

            // 텍스처 압축안되는 비디오 카드를 위한 여분의 데이터 건너뛰기..
            iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight * 2;
            if (HeaderOrg.nWidth >= 1024) {
                iSkipSize += 256 * 256 * 2; // 사이즈가 1024 보다 클경우 부두용 데이터 건너뛰기..
            }
        }

        ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
    } else {
        int iPixelSize = 0;
        if (HeaderOrg.Format == D3DFMT_A1R5G5B5 || HeaderOrg.Format == D3DFMT_A4R4G4B4) {
            iPixelSize = 2;
        } else if (HeaderOrg.Format == D3DFMT_R8G8B8) {
            iPixelSize = 3;
        } else if (HeaderOrg.Format == D3DFMT_A8R8G8B8 || HeaderOrg.Format == D3DFMT_X8R8G8B8) {
            iPixelSize = 4;
        } else {
            __ASSERT(0, "Not supported texture format");
        }

        int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
        if (HeaderOrg.bMipMap) {
            for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) {
                iSkipSize += iWTmp * iHTmp * iPixelSize;
            }
        } else {
            iSkipSize += iWTmp * iHTmp * iPixelSize;
            if (HeaderOrg.nWidth >= 512) {
                iSkipSize += 256 * 256 * 2; // 사이즈가 512 보다 클경우 부두용 데이터 건너뛰기..
            }
        }

        ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // 건너뛰고.
    }
    return true;
}

#ifdef _N3TOOL
bool CN3Texture::SaveToFile() {
    if (!n3std::iequals(FilePath().extension(), ".dxt")) {
        return false;
    }

    return CN3BaseFileAccess::SaveToFile();
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::SaveToFile(const fs::path & fsFile) {
    this->FilePathSet(fsFile);
    return this->SaveToFile();
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::Save(HANDLE hFile) {
    if (NULL == m_lpTexture) {
        return false;
    }

    CN3BaseFileAccess::Save(hFile);

    DWORD dwRWC = 0;

    D3DSURFACE_DESC sd;
    m_lpTexture->GetLevelDesc(0, &sd);

    int nMMC = m_lpTexture->GetLevelCount();
    (nMMC > 1) ? m_Header.bMipMap = TRUE : m_Header.bMipMap = FALSE;
    if (TRUE == m_Header.bMipMap) // MipMap 갯수가 맞는지 확인..
    {
        int nMMC2 = 0;
        for (int nW = sd.Width, nH = sd.Height; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) {
            nMMC2++;
        }
        if (nMMC < nMMC2) {
#ifdef _N3GAME
            CLogWriter::Write("N3Texture save warning - Invalid MipMap Count (%s)", FilePath().string().c_str());
#endif
            m_Header.bMipMap = FALSE;
            nMMC = 1;
        } else {
            nMMC = nMMC2;
        }
    }

    m_Header.szID[0] = 'N';
    m_Header.szID[1] = 'T';
    m_Header.szID[2] = 'F';
    m_Header.szID[3] = 3; // Noah Texture File Ver '3'
    m_Header.nWidth = sd.Width;
    m_Header.nHeight = sd.Height;
    m_Header.bMipMap = (nMMC > 1) ? TRUE : FALSE;

    WriteFile(hFile, &m_Header, sizeof(m_Header), &dwRWC, NULL); // 헤더를 쓰고

    if (m_lpTexture == NULL) {
        return false;
    }

    if (D3DFMT_DXT1 == sd.Format || D3DFMT_DXT2 == sd.Format || D3DFMT_DXT3 == sd.Format || D3DFMT_DXT4 == sd.Format ||
        D3DFMT_DXT5 == sd.Format) {
        D3DLOCKED_RECT LR;

        for (int i = 0; i < nMMC; i++) {
            m_lpTexture->GetLevelDesc(i, &sd);

            m_lpTexture->LockRect(i, &LR, NULL, NULL);

            int iTexBytesLength = 0;
            iTexBytesLength = sd.Format == D3DFMT_DXT1 ? (sd.Width * sd.Height / 2) : (sd.Width * sd.Height);
            WriteFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // 일렬로 된 데이터를 쓰고..
            m_lpTexture->UnlockRect(i);
        }

        // 추가로 압축되지 않은 형식을 써준다.. 절반 크기이다.
        // 압축되지 않은 형식을 해상도를 한단계 낮추어서 저장.
        LPDIRECT3DSURFACE9 lpSurfSrc = NULL, lpSurfDest = NULL;
        D3DFORMAT          fmtExtra = D3DFMT_UNKNOWN;
        if (D3DFMT_DXT1 == sd.Format) {
            fmtExtra = D3DFMT_A1R5G5B5;
        } else {
            fmtExtra = D3DFMT_A4R4G4B4;
        }

        int nMMC2 = nMMC - 1;
        if (nMMC == 1) {
            nMMC2 = nMMC;
        }
        for (int i = 0; i < nMMC2; i++) {
            m_lpTexture->GetLevelDesc(i, &sd);
            m_lpTexture->GetSurfaceLevel(i, &lpSurfSrc);
            int nW = sd.Width / 2, nH = sd.Height / 2;
            s_lpD3DDev->CreateOffscreenPlainSurface(nW, nH, fmtExtra, D3DPOOL_DEFAULT, &lpSurfDest, NULL);
            D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE,
                                       0); // 서피스 복사.
            int nPixelSize = 2;
            lpSurfDest->LockRect(&LR, NULL, NULL);
            for (int y = 0; y < nH; y++) {
                WriteFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
            }
            lpSurfDest->UnlockRect();
            lpSurfDest->Release();
            lpSurfDest = NULL;
            lpSurfSrc->Release();
            lpSurfSrc = NULL;
        }

        if (nMMC == 1 && m_Header.nWidth >= 1024) // 부두를 위해 256 * 256 짜리 하나 더 저장해준다..
        {
            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
            int nW = 256, nH = 256;
            s_lpD3DDev->CreateOffscreenPlainSurface(nW, nH, fmtExtra, D3DPOOL_DEFAULT, &lpSurfDest, NULL);
            D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE,
                                       0); // 서피스 복사.
            int nPixelSize = 2;
            lpSurfDest->LockRect(&LR, NULL, NULL);
            for (int y = 0; y < nH; y++) {
                WriteFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
            }
            lpSurfDest->UnlockRect();
            lpSurfDest->Release();
            lpSurfDest = NULL;
            lpSurfSrc->Release();
            lpSurfSrc = NULL;
        }
    } else // 일반적인 포맷이면.
    {
        int nPixelSize = 0;
        if (D3DFMT_A1R5G5B5 == sd.Format || D3DFMT_A4R4G4B4 == sd.Format) {
            nPixelSize = 2;
        } else if (D3DFMT_R8G8B8 == sd.Format) {
            nPixelSize = 3;
        } else if (D3DFMT_A8R8G8B8 == sd.Format || D3DFMT_X8R8G8B8 == sd.Format) {
            nPixelSize = 4;
        } else {
            __ASSERT(0, "this Texture Format Not Supported");
        }

        D3DLOCKED_RECT LR;
        for (int i = 0; i < nMMC; i++) {
            m_lpTexture->GetLevelDesc(i, &sd);
            m_lpTexture->LockRect(i, &LR, NULL, 0); // 각 레벨 Lock
            int nH = sd.Height;
            for (int y = 0; y < nH; y++) { // 그냥 픽셀 저장..
                WriteFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, sd.Width * nPixelSize, &dwRWC, NULL);
            }
            m_lpTexture->UnlockRect(i);
        }

        if (nMMC == 1 && m_Header.nWidth >= 512) // 부두를 위해 256 * 256 짜리 하나 더 저장해준다..
        {
            LPDIRECT3DSURFACE9 lpSurfSrc = NULL, lpSurfDest = NULL;

            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
            int nW = 256, nH = 256;
            s_lpD3DDev->CreateOffscreenPlainSurface(nW, nH, sd.Format, D3DPOOL_DEFAULT, &lpSurfDest, NULL);
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL,
                                                      D3DX_FILTER_TRIANGLE, 0); // 서피스 복사.
            lpSurfDest->LockRect(&LR, NULL, NULL);
            for (int y = 0; y < nH; y++) {
                WriteFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, nW * 2, &dwRWC, NULL);
            }
            lpSurfDest->UnlockRect();
            lpSurfDest->Release();
            lpSurfDest = NULL;
            lpSurfSrc->Release();
            lpSurfSrc = NULL;
        }
    }

    return true;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::Convert(D3DFORMAT Format, int nWidth, int nHeight, BOOL bGenerateMipMap) {
    if (m_lpTexture == NULL) {
        return false;
    }

    D3DSURFACE_DESC dsd;
    m_lpTexture->GetLevelDesc(0, &dsd);
    if (0 >= nWidth || 0 >= nHeight) {
        nWidth = dsd.Width;
        nHeight = dsd.Height;
    }

    LPDIRECT3DTEXTURE9 lpTexOld = m_lpTexture;

    m_lpTexture = NULL;
    if (this->Create(nWidth, nHeight, Format, bGenerateMipMap) == false) {
        return false;
    }
    if (bGenerateMipMap) {
        LPDIRECT3DSURFACE9 lpTSOld;
        lpTexOld->GetSurfaceLevel(0, &lpTSOld);
        this->GenerateMipMap(lpTSOld); // MipMap 생성
        lpTSOld->Release();
    } else {
        LPDIRECT3DSURFACE9 lpTSNew;
        LPDIRECT3DSURFACE9 lpTSOld;
        m_lpTexture->GetSurfaceLevel(0, &lpTSNew);
        lpTexOld->GetSurfaceLevel(0, &lpTSOld);
        D3DXLoadSurfaceFromSurface(lpTSNew, NULL, NULL, lpTSOld, NULL, NULL, D3DX_FILTER_NONE,
                                   0); // 첫번재 레벨 서피스 복사.
        lpTSOld->Release();
        lpTSNew->Release();
    }

    lpTexOld->Release();
    lpTexOld = NULL;

    return true;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::GenerateMipMap(LPDIRECT3DSURFACE9 lpSurfSrc) {
    if (m_lpTexture == NULL) {
        return false;
    }

    // MipMap 이 몇개 필요한지 계산..
    int nMMC = m_lpTexture->GetLevelCount();
    int nMMC2 = 0;
    for (int nW = m_Header.nWidth, nH = m_Header.nHeight; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) {
        nMMC2++;
    }

    bool bNeedReleaseSurf = false;
    if (NULL == lpSurfSrc) {
        bNeedReleaseSurf = true;
        if (D3D_OK != m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc)) {
            return false;
        }
    }

    HRESULT rval = D3D_OK;
    if (nMMC < nMMC2) // 적으면 새로 생성..
    {
        LPDIRECT3DTEXTURE9 lpTexOld = m_lpTexture;
        m_lpTexture = NULL;
        rval = this->CreateFromSurface(lpSurfSrc, m_Header.Format, TRUE);
        if (bNeedReleaseSurf) {
            lpSurfSrc->Release();
            lpSurfSrc = NULL;
        }
        lpTexOld->Release();
        lpTexOld = NULL;

        if (D3D_OK == rval) {
            m_Header.bMipMap = TRUE;
            return true;
        } else {
            m_Header.bMipMap = FALSE;
            return FALSE;
        }
    } else // MipMap 이 있으면 그냥 표면만 복사
    {
        if (false == bNeedReleaseSurf) // 다른 서피스에서 복사해야 되는 거면 0 레벨도 복사..
        {
            LPDIRECT3DSURFACE9 lpSurfDest;
            m_lpTexture->GetSurfaceLevel(0, &lpSurfDest);
            DWORD   dwFilter = D3DX_FILTER_TRIANGLE; // 기본 필터는 없다..
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, dwFilter,
                                                      0); // 작은 맵 체인에 서피스 이미지 축소 복사
            lpSurfDest->Release();
            lpSurfDest = NULL;
        }

        for (int i = 1; i < nMMC2; i++) {
            LPDIRECT3DSURFACE9 lpSurfDest, lpSurfUp;
            m_lpTexture->GetSurfaceLevel(i - 1, &lpSurfUp);
            m_lpTexture->GetSurfaceLevel(i, &lpSurfDest);
            DWORD   dwFilter = D3DX_FILTER_TRIANGLE; // 기본 필터는 없다..
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfUp, NULL, NULL, dwFilter,
                                                      0); // 작은 맵 체인에 서피스 이미지 축소 복사
            lpSurfDest->Release();
            lpSurfUp->Release();
        }

        if (bNeedReleaseSurf) {
            lpSurfSrc->Release();
            lpSurfSrc = NULL;
        }
        if (D3D_OK == rval) {
            m_Header.bMipMap = TRUE;
            return true;
        } else {
            m_Header.bMipMap = FALSE;
            return FALSE;
        }
    }
}
#endif // end of _N3TOOL

void CN3Texture::UpdateRenderInfo() {}

#ifdef _N3TOOL
bool CN3Texture::SaveToBitmapFile(const fs::path & fsFile) {
    if (fsFile.empty()) {
        return false;
    }
    if (NULL == m_lpTexture) {
        return false;
    }

    LPDIRECT3DSURFACE9 lpSurfSrc = NULL;
    m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);

    if (NULL == lpSurfSrc) {
        return false;
    }

    LPDIRECT3DSURFACE9 lpSurfDest = NULL;
    s_lpD3DDev->CreateOffscreenPlainSurface(m_Header.nWidth, m_Header.nHeight, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                            &lpSurfDest, NULL);

    if (NULL == lpSurfDest) {
        return false;
    }
    if (D3D_OK != D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE,
                                             0)) // 서피스 복사.
    {
        lpSurfDest->Release();
        lpSurfDest = NULL;
        lpSurfSrc->Release();
        lpSurfSrc = NULL;
    }

    CBitMapFile bmpf;
    bmpf.Create(m_Header.nWidth, m_Header.nHeight);

    D3DLOCKED_RECT LR;
    lpSurfDest->LockRect(&LR, NULL, 0);
    for (int y = 0; y < m_Header.nHeight; y++) {
        BYTE * pPixelsSrc = ((BYTE *)LR.pBits) + y * LR.Pitch;
        BYTE * pPixelsDest = (BYTE *)(bmpf.Pixels(0, y));
        for (int x = 0; x < m_Header.nWidth; x++) {
            pPixelsDest[0] = pPixelsSrc[0];
            pPixelsDest[1] = pPixelsSrc[1];
            pPixelsDest[2] = pPixelsSrc[2];

            pPixelsSrc += 4;
            pPixelsDest += 3;
        }
    }
    lpSurfDest->UnlockRect();

    lpSurfDest->Release();
    lpSurfDest = NULL;
    lpSurfSrc->Release();
    lpSurfSrc = NULL;

    return bmpf.SaveToFile(fsFile);
}
#endif // end of _N3TOOL
