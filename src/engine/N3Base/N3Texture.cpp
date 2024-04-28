// N3Texture.cpp: implementation of the CN3Texture class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "N3Texture.h"

#ifdef _N3TOOL
#include "BitmapFile.h"
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

    if (s_dwTextureCaps & TEX_CAPS_POW2) // As long as it is a multiplier of 2...
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

    if ((s_dwTextureCaps & TEX_CAPS_SQUAREONLY) && nWidth != nHeight) // As long as it has a square texture...
    {
        if (nWidth > nHeight) {
            nWidth = nHeight;
        } else {
            nHeight = nWidth;
        }
    }

    // If the video card does not support textures higher than 256...
    if (nWidth > 256 && CN3Base::s_DevCaps.MaxTextureWidth <= 256) {
        nWidth = CN3Base::s_DevCaps.MaxTextureWidth;
    }
    if (nHeight > 256 && CN3Base::s_DevCaps.MaxTextureHeight <= 256) {
        nHeight = CN3Base::s_DevCaps.MaxTextureHeight;
    }

    // 헤더 세팅..
    memset(&m_Header, 0, sizeof(m_Header));

    // MipMap step decision..
    // Create MipMap only up to 4
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
        CLogWriter::Write("N3Texture: createtexture err D3DERR_INVALIDCALL(%s)", m_szFileName.c_str());
        return false;
    }
    if (rval == D3DERR_OUTOFVIDEOMEMORY) {
        CLogWriter::Write("N3Texture: createtexture err D3DERR_OUTOFVIDEOMEMORY(%s)", m_szFileName.c_str());
        return false;
    }
    if (rval == E_OUTOFMEMORY) {
        CLogWriter::Write("N3Texture: createtexture err E_OUTOFMEMORY(%s)", m_szFileName.c_str());
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

bool CN3Texture::LoadFromFile(const std::string & szFileName) {
    if (m_lpTexture != NULL) {
        this->Release();
    }

    this->FileNameSet(szFileName); // Copy the file name...
    std::string szFullPath;
    if (-1 != m_szFileName.find(':') || -1 != m_szFileName.find("\\\\") ||
        -1 != m_szFileName.find("//")) // If the string contains ':', '\\', or '//', it is a full path.
    {
        szFullPath = m_szFileName;
    } else {
        if (NULL != s_szPath[0]) {
            szFullPath = s_szPath;
        }
        szFullPath += m_szFileName;
    }

    int nFNL = szFullPath.size();
    if (lstrcmpi(&szFullPath[nFNL - 3], "DXT") == 0) {
        HANDLE hFile =
            ::CreateFile(szFullPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
#ifdef _N3GAME
            CLogWriter::Write("invalid file handle(%d) - Can't open texture file(%s)", (int)hFile, szFullPath.c_str());
#endif
            return false;
        }
        this->Load(hFile);
        CloseHandle(hFile);
    } else {
        D3DXIMAGE_INFO ImgInfo;
        HRESULT        rval =
            D3DXCreateTextureFromFileEx(s_lpD3DDev, szFullPath.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                                        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
                                        D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 0, &ImgInfo, NULL, &m_lpTexture);
        if (rval == D3D_OK) {
            D3DSURFACE_DESC sd;
            m_lpTexture->GetLevelDesc(0, &sd);

            m_Header.nWidth = sd.Width;
            m_Header.nHeight = sd.Height;
            m_Header.Format = sd.Format;
        } else {
#ifdef _N3GAME
            CLogWriter::Write("N3Texture - Failed to load texture(%s)", szFullPath.c_str());
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

    if (NULL == m_lpTexture) {
        this->Release();
        return false;
    }
    return true;
}

bool CN3Texture::Load(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);

    DWORD dwRWC = 0;

    __DXT_HEADER HeaderOrg;                                       // Save the header...
    ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // Read the header..
    if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] ||
        3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
    {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
    }

    // You need to read the DXT Format, check whether it is supported or not, and if not, decide on an alternative format.
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
    } // If DXT is not supported, reduce the width and height.
    if (m_iLOD > 0 && m_iLOD <= 2 && HeaderOrg.nWidth >= 16 && HeaderOrg.nHeight >= 16) // Create as small as LOD...
    {
        for (int i = 0; i < m_iLOD; i++) {
            iWCreate /= 2;
            iHCreate /= 2;
        }
    } else {
        m_iLOD = 0; // Unless LOD is applied...
    }

    int iLODPrev = m_iLOD;
    this->Create(iWCreate, iHCreate, fmtNew, HeaderOrg.bMipMap); // Create a surface...
    m_iLOD = iLODPrev;

    if (m_lpTexture == NULL) {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture error - Can't create texture (%s)", m_szFileName.c_str());
#endif
        return false;
    }

    D3DSURFACE_DESC sd;
    D3DLOCKED_RECT  LR;
    int             iMMC = m_lpTexture->GetLevelCount(); // Number of MipMaps created

    // If it is a compressed format...
    if (D3DFMT_DXT1 == HeaderOrg.Format || D3DFMT_DXT2 == HeaderOrg.Format || D3DFMT_DXT3 == HeaderOrg.Format ||
        D3DFMT_DXT4 == HeaderOrg.Format || D3DFMT_DXT5 == HeaderOrg.Format) {
        if (TRUE == bDXTSupport) // If compressed texture is supported...
        {
            if (iMMC > 1) {
                if (m_iLOD > 0) // Skip by LOD...
                {
                    int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                    for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                        if (D3DFMT_DXT1 == HeaderOrg.Format) {
                            iSkipSize += iWTmp * iHTmp / 2; // DXT1 format is compressed to 1/4 of the 16-bit format.
                        } else {
                            iSkipSize +=
                                iWTmp * iHTmp; // DXT2 ~ DXT5 formats are compressed to 1/2 of the 16-bit format.
                        }
                    }
                    ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip it.
                }

                for (int i = 0; i < iMMC; i++) {
                    m_lpTexture->GetLevelDesc(i, &sd);
                    m_lpTexture->LockRect(i, &LR, NULL, NULL);

                    int iTexBytesLength = 0;
                    iTexBytesLength = sd.Format == D3DFMT_DXT1 ? (sd.Width * sd.Height / 2) : (sd.Width * sd.Height);
                    ReadFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // Write data in a row...
                    m_lpTexture->UnlockRect(i);
                }

                // Skip extra data for video cards that do not compress textures...
                int iWTmp = HeaderOrg.nWidth / 2, iHTmp = HeaderOrg.nHeight / 2;
                for (; iWTmp >= 4 && iHTmp >= 4;
                     iWTmp /= 2,
                     iHTmp /= 2) { // It is formatted as A1R5G5B5 or A4R4G4B4, which contains two bytes per pixel.
                    ::SetFilePointer(hFile, iWTmp * iHTmp * 2, 0, FILE_CURRENT); // Skip it.
                }
            } else // pair of if(iMMC > 1)
            {
                m_lpTexture->GetLevelDesc(0, &sd);
                m_lpTexture->LockRect(0, &LR, NULL, NULL);

                int iTexBytesLength = 0;
                iTexBytesLength = sd.Format == D3DFMT_DXT1 ? (sd.Width * sd.Height / 2) : (sd.Width * sd.Height);
                ReadFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // Write data in a row...
                m_lpTexture->UnlockRect(0);

                // Skip extra data for video cards that do not compress textures...
                ::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight / 4, 0, FILE_CURRENT); // Skip.
                if (HeaderOrg.nWidth >= 1024) {
                    SetFilePointer(hFile, 256 * 256 * 2, 0,
                                   FILE_CURRENT); // If size is larger than 512, skip data for dock..
                }
            }
        } else // If DXT is not supported...
        {
            if (iMMC > 1) // Skip by LOD...
            {
                // Skip compressed data..
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) {
                    if (D3DFMT_DXT1 == HeaderOrg.Format) {
                        iSkipSize += iWTmp * iHTmp / 2; // DXT1 format is compressed to 1/4 of the 16-bit format.
                    } else {
                        iSkipSize += iWTmp * iHTmp; // DXT2 ~ DXT5 format is compressed to 1/2 of the 16-bit format.
                    }
                }
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip.

                // Skip as much as LOD...
                iWTmp = HeaderOrg.nWidth / 2;
                iHTmp = HeaderOrg.nHeight / 2;
                iSkipSize = 0;
                if (m_iLOD > 0) {
                    for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                        iSkipSize += iWTmp * iHTmp * 2; // I think the pixel size is the pitch divided by the width...
                    }
                }

                // Skip if the video card supports texture size is small..
                for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight;
                     iWTmp /= 2, iHTmp /= 2) {
                    iSkipSize += iWTmp * iHTmp * 2;
                }
                if (iSkipSize) {
                    ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip it.
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
                // Skip compressed data..
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                if (D3DFMT_DXT1 == HeaderOrg.Format) {
                    iSkipSize =
                        iWTmp * iHTmp / 2; // The DXT1 format is compressed to 1/4 the size of the 16-bit format.
                } else {
                    iSkipSize = iWTmp * iHTmp; // DXT2 ~ DXT5 formats are compressed to 1/2 of the 16-bit format.
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
            if (m_iLOD > 0) // Skip by LOD...
            {
                int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
                for (int i = 0; i < m_iLOD; i++, iWTmp /= 2, iHTmp /= 2) {
                    iSkipSize +=
                        iWTmp * iHTmp * iPixelSize; // I think the pixel size is the pitch divided by the width...
                }
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip.
            }

            // Skip if the video card supported texture size is small..
            int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
            for (; iWTmp > s_DevCaps.MaxTextureWidth || iHTmp > s_DevCaps.MaxTextureHeight; iWTmp /= 2, iHTmp /= 2) {
                iSkipSize += iWTmp * iHTmp * iPixelSize;
            }
            if (iSkipSize) {
                ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip
            }

            // Reading data...
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
            // Skip if the video card supports texture size is small..
            if (HeaderOrg.nWidth >= 512 && m_Header.nWidth <= 256) {
                ::SetFilePointer(hFile, HeaderOrg.nWidth * HeaderOrg.nHeight * iPixelSize, 0,
                                 FILE_CURRENT); // Skip
            }

            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->LockRect(0, &LR, NULL, NULL);
            for (int y = 0; y < (int)sd.Height; y++) {
                ReadFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, iPixelSize * sd.Width, &dwRWC, NULL);
            }
            m_lpTexture->UnlockRect(0);

            if (m_Header.nWidth >= 512 && m_Header.nHeight >= 512) {
                SetFilePointer(hFile, 256 * 256 * 2, 0,
                               FILE_CURRENT); // If size is larger than 512, skip data for dock..
            }
        }
    }
    //    this->GenerateMipMap(); // Create a Mip Map..
    return true;
}

bool CN3Texture::SkipFileHandle(HANDLE hFile) {
    CN3BaseFileAccess::Load(hFile);

    __DXT_HEADER HeaderOrg; // Save the header...
    DWORD        dwRWC = 0;
    ReadFile(hFile, &HeaderOrg, sizeof(HeaderOrg), &dwRWC, NULL); // Read the header...
    if ('N' != HeaderOrg.szID[0] || 'T' != HeaderOrg.szID[1] || 'F' != HeaderOrg.szID[2] ||
        3 != HeaderOrg.szID[3]) // "NTF"3 - Noah Texture File Ver. 3.0
    {
#ifdef _N3GAME
        CLogWriter::Write("N3Texture Warning - Old format DXT file (%s)", m_szFileName.c_str());
#endif
    }

    // If it is a compressed format...
    if (D3DFMT_DXT1 == HeaderOrg.Format || D3DFMT_DXT2 == HeaderOrg.Format || D3DFMT_DXT3 == HeaderOrg.Format ||
        D3DFMT_DXT4 == HeaderOrg.Format || D3DFMT_DXT5 == HeaderOrg.Format) {
        int iWTmp = HeaderOrg.nWidth, iHTmp = HeaderOrg.nHeight, iSkipSize = 0;
        if (HeaderOrg.bMipMap) {
            // Skip compressed data..
            for (; iWTmp >= 4 && iHTmp >= 4; iWTmp /= 2, iHTmp /= 2) {
                if (D3DFMT_DXT1 == HeaderOrg.Format) {
                    iSkipSize += iWTmp * iHTmp / 2;
                } else {
                    iSkipSize += iWTmp * iHTmp;
                }
            }
            // Skipping extra data for video cards that don't compress textures.
            iWTmp = HeaderOrg.nWidth / 2;
            iHTmp = HeaderOrg.nHeight / 2;
            for (; iWTmp >= 4 && iHTmp >= 4;
                 iWTmp /= 2,
                 iHTmp /= 2) { // It is formatted as A1R5G5B5 or A4R4G4B4, which contains two bytes per pixel.
                iSkipSize += iWTmp * iHTmp * 2; // Skip
            }
        } else // pair of if(HeaderOrg.bMipMap)
        {
            // Skip compressed data..
            if (D3DFMT_DXT1 == HeaderOrg.Format) {
                iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight / 2;
            } else {
                iSkipSize += iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight;
            }

            // Skipping extra data for video cards that don't compress textures.
            iSkipSize += HeaderOrg.nWidth * HeaderOrg.nHeight * 2;
            if (HeaderOrg.nWidth >= 1024) {
                iSkipSize += 256 * 256 * 2; // If the size is larger than 1024, skip the dock data.
            }
        }

        ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip
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
                iSkipSize += 256 * 256 * 2; // If the size is larger than 512, skip the dock data.
            }
        }

        ::SetFilePointer(hFile, iSkipSize, 0, FILE_CURRENT); // Skip
    }
    return true;
}

#ifdef _N3TOOL
bool CN3Texture::SaveToFile() {
    char szExt[_MAX_EXT];
    _splitpath(m_szFileName.c_str(), NULL, NULL, NULL, szExt);
    if (lstrcmpi(szExt, ".dxt") != 0) {
        return false;
    }

    return CN3BaseFileAccess::SaveToFile();
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
bool CN3Texture::SaveToFile(const std::string & szFileName) {
    this->FileNameSet(szFileName);
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
    if (TRUE == m_Header.bMipMap) // Check if the MipMap number is correct.
    {
        int nMMC2 = 0;
        for (int nW = sd.Width, nH = sd.Height; nW >= 4 && nH >= 4; nW /= 2, nH /= 2) {
            nMMC2++;
        }
        if (nMMC < nMMC2) {
#ifdef _N3GAME
            CLogWriter::Write("N3Texture save warning - Invalid MipMap Count (%s)", m_szFileName.c_str());
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

    WriteFile(hFile, &m_Header, sizeof(m_Header), &dwRWC, NULL); // write header

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
            WriteFile(hFile, (BYTE *)LR.pBits, iTexBytesLength, &dwRWC, NULL); // Write data in a row...
            m_lpTexture->UnlockRect(i);
        }

        // Additional uncompressed format is used. It is half the size.
        // Save the uncompressed format at one level lower in resolution.
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
                                       0); // Copy surface.
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

        if (nMMC == 1 && m_Header.nWidth >= 1024) // Save one more size 256 * 256 for the dock.
        {
            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
            int nW = 256, nH = 256;
            s_lpD3DDev->CreateOffscreenPlainSurface(nW, nH, fmtExtra, D3DPOOL_DEFAULT, &lpSurfDest, NULL);
            D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE,
                                       0); // Copy surface.
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
    } else // If it is a general format.
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
            m_lpTexture->LockRect(i, &LR, NULL, 0); // Lock each level
            int nH = sd.Height;
            for (int y = 0; y < nH; y++) { // Just save the pixel...
                WriteFile(hFile, (BYTE *)LR.pBits + y * LR.Pitch, sd.Width * nPixelSize, &dwRWC, NULL);
            }
            m_lpTexture->UnlockRect(i);
        }

        if (nMMC == 1 && m_Header.nWidth >= 512) // Save another 256 * 256 size for the dock.
        {
            LPDIRECT3DSURFACE9 lpSurfSrc = NULL, lpSurfDest = NULL;

            m_lpTexture->GetLevelDesc(0, &sd);
            m_lpTexture->GetSurfaceLevel(0, &lpSurfSrc);
            int nW = 256, nH = 256;
            s_lpD3DDev->CreateOffscreenPlainSurface(nW, nH, sd.Format, D3DPOOL_DEFAULT, &lpSurfDest, NULL);
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL,
                                                      D3DX_FILTER_TRIANGLE, 0); // Copy surface.
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
        this->GenerateMipMap(lpTSOld); //Create MipMap
        lpTSOld->Release();
    } else {
        LPDIRECT3DSURFACE9 lpTSNew;
        LPDIRECT3DSURFACE9 lpTSOld;
        m_lpTexture->GetSurfaceLevel(0, &lpTSNew);
        lpTexOld->GetSurfaceLevel(0, &lpTSOld);
        D3DXLoadSurfaceFromSurface(lpTSNew, NULL, NULL, lpTSOld, NULL, NULL, D3DX_FILTER_NONE,
                                   0); // Copy first level surfaces.
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

    // Calculate how many MipMap are needed.
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
    if (nMMC < nMMC2) //If you write down, create a new one..
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
    } else // If you have a MipMap, just copy the surface
    {
        if (false == bNeedReleaseSurf) // If you need to copy from another surface, also copy level 0.
        {
            LPDIRECT3DSURFACE9 lpSurfDest;
            m_lpTexture->GetSurfaceLevel(0, &lpSurfDest);
            DWORD   dwFilter = D3DX_FILTER_TRIANGLE; // There is no default filter.
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, dwFilter,
                                                      0); // Copy surface image reduction to small map chain
            lpSurfDest->Release();
            lpSurfDest = NULL;
        }

        for (int i = 1; i < nMMC2; i++) {
            LPDIRECT3DSURFACE9 lpSurfDest, lpSurfUp;
            m_lpTexture->GetSurfaceLevel(i - 1, &lpSurfUp);
            m_lpTexture->GetSurfaceLevel(i, &lpSurfDest);
            DWORD   dwFilter = D3DX_FILTER_TRIANGLE; // There is no default filter.
            HRESULT rval = D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfUp, NULL, NULL, dwFilter,
                                                      0); // Copy surface image reduction to small map chain
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
bool CN3Texture::SaveToBitmapFile(const std::string & szFN) {
    if (szFN.empty()) {
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
                                             0)) // Copy surface.
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

    return bmpf.SaveToFile(szFN.c_str());
}
#endif // end of _N3TOOL
