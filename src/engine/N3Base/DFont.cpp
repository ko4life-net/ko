// DFont.cpp: implementation of the CDFont class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <d3dx9.h>
#include "DFont.h"

const int   MAX_NUM_VERTICES = 50 * 6;
const float Z_DEFAULT = 0.9f;
const float RHW_DEFAULT = 1.0f;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HDC   CDFont::s_hDC = NULL;
int   CDFont::s_iInstanceCount = 0;
HFONT CDFont::s_hFontOld = NULL;

CDFont::CDFont(const std::string & szFontName, DWORD dwHeight, DWORD dwFlags) {
    if (0 == s_iInstanceCount) {
        s_hDC = CreateCompatibleDC(NULL);
        // Create a temporary font and get s_hFontOld.
        HFONT hFont = CreateFont(0, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                 CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "굴림");
        if (hFont) {
            s_hFontOld = (HFONT)(SelectObject(s_hDC, hFont));
            SelectObject(s_hDC, s_hFontOld);
            DeleteObject(hFont);
        }
    }
    s_iInstanceCount++;

    __ASSERT(!szFontName.empty(), "??");

    m_szFontName = szFontName;
    m_dwFontHeight = dwHeight;
    m_dwFontFlags = dwFlags;

    m_pd3dDevice = NULL;
    m_pTexture = NULL;
    m_pVB = NULL;

    m_iPrimitiveCount = 0;
    m_PrevLeftTop.x = m_PrevLeftTop.y = 0;

    m_hFont = NULL;
    m_dwFontColor = 0xffffffff;
    m_Size.cx = 0;
    m_Size.cy = 0;
    m_Is2D = (dwFlags & D3DFONT_3D) ? FALSE : TRUE;
}

CDFont::~CDFont() {
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    s_iInstanceCount--;
    if (s_iInstanceCount <= 0) {
        if (s_hFontOld) {
            SelectObject(s_hDC, s_hFontOld);
        }
        DeleteDC(s_hDC);
        s_hDC = NULL;
    }
}

HRESULT CDFont::SetFont(const std::string & szFontName, DWORD dwHeight, DWORD dwFlags) {
    __ASSERT(!szFontName.empty(), "");
    if (NULL == s_hDC) {
        __ASSERT(0, "NULL DC Handle");
        return E_FAIL;
    }

    m_szFontName = szFontName;
    m_dwFontHeight = dwHeight;
    m_dwFontFlags = dwFlags;

    if (m_hFont) {
        if (s_hFontOld) {
            SelectObject(s_hDC, s_hFontOld);
        }
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT   nHeight = -MulDiv(m_dwFontHeight, (INT)(GetDeviceCaps(s_hDC, LOGPIXELSY) * m_fTextScale), 72);
    DWORD dwBold = (m_dwFontFlags & D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic = (m_dwFontFlags & D3DFONT_ITALIC) ? TRUE : FALSE;
    m_hFont = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, m_szFontName.c_str());
    if (NULL == m_hFont) {
        __ASSERT(0, "NULL Font Handle");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CDFont::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice) {
    // Keep a local copy of the device
    m_pd3dDevice = pd3dDevice;
    m_fTextScale = 1.0f; // Draw fonts into texture without scaling

    return S_OK;
}

HRESULT CDFont::RestoreDeviceObjects() {
    HRESULT hr;

    m_iPrimitiveCount = 0;

    // __ASSERT(NULL == s_hDC && NULL == m_hFont, "??");
    // m_hDC = CreateCompatibleDC(NULL);
    __ASSERT(NULL == m_hFont, "??");

    if (NULL == s_hDC) {
        __ASSERT(0, "Can't Create DC");
        return E_FAIL;
    }
    SetMapMode(s_hDC, MM_TEXT);

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT   nHeight = -MulDiv(m_dwFontHeight, (INT)(GetDeviceCaps(s_hDC, LOGPIXELSY) * m_fTextScale), 72);
    DWORD dwBold = (m_dwFontFlags & D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic = (m_dwFontFlags & D3DFONT_ITALIC) ? TRUE : FALSE;
    m_hFont = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, m_szFontName.c_str());
    if (NULL == m_hFont) {
        return E_FAIL;
    }

    // Create vertex buffer for the letters
    __ASSERT(m_pVB == NULL, "??");
    int   iVBSize = 0;
    DWORD dwFVF = 0;
    if (m_Is2D) {
        iVBSize = MAX_NUM_VERTICES * sizeof(__VertexTransformed);
        dwFVF = FVF_TRANSFORMED;
    } else {
        iVBSize = MAX_NUM_VERTICES * sizeof(__VertexXyzColorT1);
        dwFVF = FVF_XYZCOLORT1;
    }

    // if (FAILED(hr = m_pd3dDevice->CreateVertexBuffer(iVBSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB))) {
    if (FAILED(hr = m_pd3dDevice->CreateVertexBuffer(iVBSize, 0, dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL))) {
        return hr;
    }

    return S_OK;
}

HRESULT CDFont::InvalidateDeviceObjects() {
    if (m_pVB) {
        m_pVB->Release();
        m_pVB = NULL;
    }

    if (m_hFont) {
        if (s_hDC && s_hFontOld) {
            SelectObject(s_hDC, s_hFontOld);
        }
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }
    return S_OK;
}

HRESULT CDFont::DeleteDeviceObjects() {
    if (m_pTexture) {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
    m_pd3dDevice = NULL;

    return S_OK;
}

HRESULT CDFont::SetText(const std::string & szText, DWORD dwFlags) {
    if (NULL == s_hDC || NULL == m_hFont) {
        return E_FAIL;
    }

    if (szText.empty()) {
        m_iPrimitiveCount = 0;
        if (m_pTexture) {
            m_pTexture->Release();
            m_pTexture = NULL;
        }
        return S_OK;
    }

    int iStrLen = szText.size();

    HRESULT hr;
    // \n Calculate character length by subtracting and making it one line
    int  iCount = 0;
    int  iTempCount = 0;
    SIZE size;

    std::string szTemp(iStrLen, '?');
    while (iCount < iStrLen) {
        if ('\n' == szText[iCount]) { // \n
            ++iCount;
        } else if (0x80 & szText[iCount]) { // 2BYTE characters
            if ((iCount + 2) > iStrLen) {   // It's a strange string...
                // __ASSERT(0, "It's a strange string.!!!");
                break;
            } else {
                memcpy(&(szTemp[iTempCount]), &(szText[iCount]), 2);
                iTempCount += 2;
                iCount += 2;
            }
        } else { // 1BYTE 문자
            memcpy(&(szTemp[iTempCount]), &(szText[iCount]), 1);
            ++iTempCount;
            ++iCount;
        }
        __ASSERT(iCount <= iStrLen, "??"); // If you receive a strange text
    }

    // szTemp[iTempCount] = 0x00;

    // Determine texture size
    SelectObject(s_hDC, m_hFont);
    GetTextExtentPoint32(s_hDC, szTemp.c_str(), szTemp.size(), &size);
    szTemp = "";

    if (size.cx <= 0 || size.cy <= 0) {
        __ASSERT(0, "Invalid Text Size - ?????");
        return E_FAIL;
    }
    int iExtent = size.cx * size.cy;

    SIZE size2; // Size of half letters in Hangul...
    GetTextExtentPoint32(s_hDC, "진", lstrlen("진"), &size2);
    size2.cx = ((size2.cx / 2) + (size2.cx % 2));

    int iTexSizes[7] = {32, 64, 128, 256, 512, 1024, 2048};
    for (int i = 0; i < 7; ++i) {
        if (iExtent <= (iTexSizes[i] - size2.cx - size2.cy - 1) * iTexSizes[i]) {
            m_dwTexWidth = m_dwTexHeight = iTexSizes[i];
            break;
        }
    }

    // Establish the font and texture size
    m_fTextScale = 1.0f; // Draw fonts into texture without scaling

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS9 d3dCaps;
    m_pd3dDevice->GetDeviceCaps(&d3dCaps);

    if (m_dwTexWidth > d3dCaps.MaxTextureWidth) {
        m_fTextScale = (float)d3dCaps.MaxTextureWidth / (float)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    // If the size of the existing texture is different from the size of the new texture, create it again.
    if (m_pTexture) {
        D3DSURFACE_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        m_pTexture->GetLevelDesc(0, &sd);
        if (sd.Width != m_dwTexWidth) {
            m_pTexture->Release();
            m_pTexture = NULL;
        }
    }

    // Create a new texture for the font
    if (NULL == m_pTexture) {
        int iMipMapCount = 1;
        if (dwFlags & D3DFONT_FILTERED) {
            iMipMapCount = 0; // Filtering text creates a mipmap.
        }

        hr = m_pd3dDevice->CreateTexture(m_dwTexWidth, m_dwTexHeight, iMipMapCount, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED,
                                         &m_pTexture, NULL);
        if (FAILED(hr)) {
            return hr;
        }
    }

    // Prepare to create a bitmap
    DWORD *    pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = (int)m_dwTexWidth;
    bmi.bmiHeader.biHeight = -(int)m_dwTexHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount = 32;

    // Create a DC and a bitmap for the font
    HBITMAP hbmBitmap = CreateDIBSection(s_hDC, &bmi, DIB_RGB_COLORS, (VOID **)&pBitmapBits, NULL, 0);

    if (NULL == hbmBitmap) {
        __ASSERT(0, "CreateDIBSection failed");
        if (m_pTexture) {
            m_pTexture->Release();
            m_pTexture = NULL;
        }
        return E_FAIL;
    }

    HGDIOBJ hObjPrev = ::SelectObject(s_hDC, hbmBitmap);

    // Set text properties
    SetTextColor(s_hDC, RGB(255, 255, 255));
    SetBkColor(s_hDC, RGB(0, 0, 0));
    SetTextAlign(s_hDC, TA_TOP);

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.

    // Printing text and creating a board to print text on
    if (m_Is2D) {
        Make2DVertex(size.cy, szText);
    } else {
        Make3DVertex(size.cy, szText, dwFlags);
    }

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect(0, &d3dlr, 0, 0);
    WORD * pDst16 = (WORD *)d3dlr.pBits;
    BYTE   bAlpha; // 4-bit measure of pixel intensity

    DWORD x, y;
    for (y = 0; y < m_dwTexHeight; y++) {
        for (x = 0; x < m_dwTexWidth; x++) {
            bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth * y + x] & 0xff) >> 4);
            if (bAlpha > 0) {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            } else {
                *pDst16++ = 0x0000;
            }
        }
    }

    // Done updating texture, so clean up used objects
    m_pTexture->UnlockRect(0);

    ::SelectObject(s_hDC, hObjPrev); // You must choose the previous one.
    DeleteObject(hbmBitmap);         // It is erased properly.

    ////////////////////////////////////////////////////////////
    // The filtering texture is created as a MipMap.
    if (dwFlags & D3DFONT_FILTERED) {
        int iMMC = m_pTexture->GetLevelCount();
        for (int i = 1; i < iMMC; i++) {
            LPDIRECT3DSURFACE9 lpSurfSrc = NULL;
            LPDIRECT3DSURFACE9 lpSurfDest = NULL;
            m_pTexture->GetSurfaceLevel(i - 1, &lpSurfSrc);
            m_pTexture->GetSurfaceLevel(i, &lpSurfDest);

            if (lpSurfSrc && lpSurfDest) {
                ::D3DXLoadSurfaceFromSurface(lpSurfDest, NULL, NULL, lpSurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE,
                                             0); // Copy Surface
            }

            if (lpSurfSrc) {
                lpSurfSrc->Release();
            }
            if (lpSurfDest) {
                lpSurfDest->Release();
            }
        }
    }
    // The filtering texture is created as a MipMap.
    ////////////////////////////////////////////////////////////

    return S_OK;
}

void CDFont::Make2DVertex(const int iFontHeight, const std::string & szText) {
    if (NULL == m_pVB || NULL == s_hDC || NULL == m_hFont) {
        __ASSERT(0, "NULL Vertex Buffer or DC or Font Handle ");
        return;
    }
    if (szText.empty()) {
        return;
    }
    int iStrLen = szText.size();

    // lock vertex buffer
    __VertexTransformed * pVertices = NULL;
    DWORD                 dwNumTriangles = 0;
    m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

    DWORD sx = 0; // start x y
    DWORD x = 0;
    DWORD y = 0;
    float vtx_sx = 0;
    float vtx_sy = 0; // vertex start x y
    int   iCount = 0;
    int   iTempCount = 0;

    char  szTempChar[3] = "";
    DWORD dwColor = 0xffffffff; // font color
    m_dwFontColor = 0xffffffff;
    SIZE size;

    float fMaxX = 0.0f, fMaxY = 0.0f; // To investigate the maximum and minimum values of the range in which letters can be printed.

    while (iCount < iStrLen) {
        if ('\n' == szText[iCount]) { // \n
            ++iCount;

            // Create vertices
            if (sx != x) {
                FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
                FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
                FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
                FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

                FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
                FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

                __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); // There is not enough vertex buffer.
                if (dwNumTriangles + 2 >= MAX_NUM_VERTICES) {
                    break;
                }

                FLOAT fLeft = vtx_sx + 0 - 0.5f;
                FLOAT fRight = vtx_sx + w - 0.5f;
                FLOAT fTop = vtx_sy + 0 - 0.5f;
                FLOAT fBottom = vtx_sy + h - 0.5f;
                pVertices->Set(fLeft, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fRight, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
                ++pVertices;

                dwNumTriangles += 2;

                if (fMaxX < fRight) {
                    fMaxX = fRight;
                }
                if (fMaxY < fBottom) {
                    fMaxY = fBottom;
                }
            }
            // Skip to the next line on the screen
            sx = x;
            vtx_sx = 0;
            vtx_sy = vtx_sy + ((float)(iFontHeight)) / m_fTextScale;
            continue;
        } else if (0x80 & szText[iCount]) { 
            memcpy(szTempChar, &(szText[iCount]), 2);
            iCount += 2;
            szTempChar[2] = 0x00;
        } else { 
            memcpy(szTempChar, &(szText[iCount]), 1);
            iCount += 1;
            szTempChar[1] = 0x00;
        }

        SelectObject(s_hDC, m_hFont);
        GetTextExtentPoint32(s_hDC, szTempChar, lstrlen(szTempChar), &size);
        if ((x + size.cx) > m_dwTexWidth) { // Create a vertex and move it to the next line..
            // Create vertices
            if (sx != x) {
                FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
                FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
                FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
                FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

                FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
                FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

                __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); //There is not enough vertex buffer.
                if (dwNumTriangles + 2 >= MAX_NUM_VERTICES) {
                    break;
                }

                FLOAT fLeft = vtx_sx + 0 - 0.5f;
                FLOAT fRight = vtx_sx + w - 0.5f;
                FLOAT fTop = vtx_sy + 0 - 0.5f;
                FLOAT fBottom = vtx_sy + h - 0.5f;
                pVertices->Set(fLeft, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fRight, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
                ++pVertices;
                dwNumTriangles += 2;

                if (fMaxX < fRight) {
                    fMaxX = fRight;
                }
                if (fMaxY < fBottom) {
                    fMaxY = fBottom;
                }

                // Skip to next line of texture
                x = sx = 0;
                y += iFontHeight;
                vtx_sx = vtx_sx + w;
            } else {
                x = sx = 0;
                y += iFontHeight;
            }
        }

        // dc
        SelectObject(s_hDC, m_hFont);
        ExtTextOut(s_hDC, x, y, ETO_OPAQUE, NULL, szTempChar, lstrlen(szTempChar), NULL);
        x += size.cx;
    }

    // Create the last remaining vertex
    if (sx != x) {
        FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
        FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
        FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
        FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

        FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
        FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

        __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); // There is not enough vertex buffer.

        FLOAT fLeft = vtx_sx + 0 - 0.5f;
        FLOAT fRight = vtx_sx + w - 0.5f;
        FLOAT fTop = vtx_sy + 0 - 0.5f;
        FLOAT fBottom = vtx_sy + h - 0.5f;
        pVertices->Set(fLeft, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty2);
        ++pVertices;
        pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
        ++pVertices;
        pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
        ++pVertices;
        pVertices->Set(fRight, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty1);
        ++pVertices;
        pVertices->Set(fRight, fBottom, Z_DEFAULT, RHW_DEFAULT, dwColor, tx2, ty2);
        ++pVertices;
        pVertices->Set(fLeft, fTop, Z_DEFAULT, RHW_DEFAULT, dwColor, tx1, ty1);
        ++pVertices;
        dwNumTriangles += 2;

        if (fMaxX < fRight) {
            fMaxX = fRight;
        }
        if (fMaxY < fBottom) {
            fMaxY = fBottom;
        }
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    m_iPrimitiveCount = dwNumTriangles;
    m_PrevLeftTop.x = m_PrevLeftTop.y = 0;
    m_Size.cx = (long)fMaxX;
    m_Size.cy = (long)fMaxY;
}

void CDFont::Make3DVertex(const int iFontHeight, const std::string & szText, DWORD dwFlags) {
    if (NULL == m_pVB || NULL == s_hDC || NULL == m_hFont) {
        __ASSERT(0, "NULL Vertex Buffer or DC or Font Handle ");
        return;
    }

    int iStrLen = szText.size();

    // Insert into temporary vertex buffer
    __VertexXyzColorT1   TempVertices[MAX_NUM_VERTICES];
    __VertexXyzColorT1 * pVertices = TempVertices;
    DWORD                dwNumTriangles = 0;

    DWORD sx = 0; // start x y
    DWORD x = 0;
    DWORD y = 0;
    float vtx_sx = 0;
    float vtx_sy = 0; // vertex start x y
    int   iCount = 0;
    int   iTempCount = 0;

    char szTempChar[3] = "";
    SIZE size;

    float fMaxX = 0.0f, fMaxY = 0.0f; // To investigate the maximum and minimum values of the range in which letters can be printed.

    while (iCount < iStrLen) {
        if ('\n' == szText[iCount]) // \n
        {
            ++iCount;

            // Create vertices
            if (sx != x) {
                FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
                FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
                FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
                FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

                FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
                FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

                __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); // There is not enough vertex buffer.
                if (dwNumTriangles + 2 >= MAX_NUM_VERTICES) {
                    break;
                }

                FLOAT fLeft = vtx_sx + 0;
                FLOAT fRight = vtx_sx + w;
                FLOAT fTop = vtx_sy + 0;
                FLOAT fBottom = vtx_sy - h;
                pVertices->Set(fLeft, fBottom, Z_DEFAULT, m_dwFontColor, tx1, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fRight, fTop, Z_DEFAULT, m_dwFontColor, tx2, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
                ++pVertices;

                dwNumTriangles += 2;
                if (fMaxX < fRight) {
                    fMaxX = fRight;
                }
                if (fMaxY < (-fBottom)) {
                    fMaxY = (-fBottom);
                }
            }
            // Skip to the next line on the screen
            sx = x;
            vtx_sx = 0;
            vtx_sy = vtx_sy - ((float)(iFontHeight)) / m_fTextScale;
            continue;
        } else if (0x80 & szText[iCount]) 
        {
            memcpy(szTempChar, &(szText[iCount]), 2);
            iCount += 2;
            szTempChar[2] = 0x00;
        } else 
        {
            memcpy(szTempChar, &(szText[iCount]), 1);
            iCount += 1;
            szTempChar[1] = 0x00;
        }

        SelectObject(s_hDC, m_hFont);
        GetTextExtentPoint32(s_hDC, szTempChar, lstrlen(szTempChar), &size);
        if ((x + size.cx) > m_dwTexWidth) { // Create a vertex and move it to the next line..
            // Create vertices
            if (sx != x) {
                FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
                FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
                FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
                FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

                FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
                FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

                __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); // There is not enough vertex buffer.
                if (dwNumTriangles + 2 >= MAX_NUM_VERTICES) {
                    break;
                }

                FLOAT fLeft = vtx_sx + 0;
                FLOAT fRight = vtx_sx + w;
                FLOAT fTop = vtx_sy + 0;
                FLOAT fBottom = vtx_sy - h;
                pVertices->Set(fLeft, fBottom, Z_DEFAULT, m_dwFontColor, tx1, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fRight, fTop, Z_DEFAULT, m_dwFontColor, tx2, ty1);
                ++pVertices;
                pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
                ++pVertices;
                pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
                ++pVertices;
                dwNumTriangles += 2;
                if (fMaxX < fRight) {
                    fMaxX = fRight;
                }
                if (fMaxY < (-fBottom)) {
                    fMaxY = (-fBottom);
                }

                // Skip to next line of texture
                x = sx = 0;
                y += iFontHeight;
                vtx_sx = vtx_sx + w;
            } else {
                x = sx = 0;
                y += iFontHeight;
            }
        }

        // dc
        SelectObject(s_hDC, m_hFont);
        ExtTextOut(s_hDC, x, y, ETO_OPAQUE, NULL, szTempChar, lstrlen(szTempChar), NULL);
        x += size.cx;
    }

    // Create the last remaining vertex
    if (sx != x) {
        FLOAT tx1 = ((FLOAT)(sx)) / m_dwTexWidth;
        FLOAT ty1 = ((FLOAT)(y)) / m_dwTexHeight;
        FLOAT tx2 = ((FLOAT)(x)) / m_dwTexWidth;
        FLOAT ty2 = ((FLOAT)(y + iFontHeight)) / m_dwTexHeight;

        FLOAT w = (tx2 - tx1) * m_dwTexWidth / m_fTextScale;
        FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

        __ASSERT(dwNumTriangles + 2 < MAX_NUM_VERTICES, "??"); // There is not enough vertex buffer.

        FLOAT fLeft = vtx_sx + 0;
        FLOAT fRight = vtx_sx + w;
        FLOAT fTop = vtx_sy + 0;
        FLOAT fBottom = vtx_sy - h;
        pVertices->Set(fLeft, fBottom, Z_DEFAULT, m_dwFontColor, tx1, ty2);
        ++pVertices;
        pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
        ++pVertices;
        pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
        ++pVertices;
        pVertices->Set(fRight, fTop, Z_DEFAULT, m_dwFontColor, tx2, ty1);
        ++pVertices;
        pVertices->Set(fRight, fBottom, Z_DEFAULT, m_dwFontColor, tx2, ty2);
        ++pVertices;
        pVertices->Set(fLeft, fTop, Z_DEFAULT, m_dwFontColor, tx1, ty1);
        ++pVertices;
        dwNumTriangles += 2;
        if (fMaxX < fRight) {
            fMaxX = fRight;
        }
        if (fMaxY < (-fBottom)) {
            fMaxY = (-fBottom);
        }
    }

    if (dwFlags & D3DFONT_CENTERED) // If it is centered, calculate the vertex coordinates to the center and correct it.
    {
        // Find the longest line...
        int iRectangleCount = dwNumTriangles / 2;

        int iContinueCount = 1;

        float fCX = 0;
        float fCY = 0;
        iCount = 0;
        while (iCount < iRectangleCount) {
            iContinueCount = 1;
            fCX = TempVertices[iCount * 6 + 3].x;
            fCY = TempVertices[iCount * 6].y;

            while (iCount + iContinueCount < iRectangleCount) {
                if (TempVertices[(iCount + iContinueCount) * 6].y == fCY) { // It is the same line as the next square.
                    fCX = TempVertices[(iCount + iContinueCount) * 6 + 3].x;
                    ++iContinueCount;
                } else { // It is a different line from the next square.
                    break;
                }
            }
            __ASSERT(fCX > 0.0f, "??");
            float fDiffX = -fCX / 2.0f;
            for (int i = iCount; i < iCount + iContinueCount; ++i) {
                for (int j = 0; j < 6; ++j) {
                    TempVertices[i * 6 + j].x += fDiffX;
                }
            }
            iCount += iContinueCount;
        }
    }

    // Move to vertex buffer.
    // lock vertex buffer
    m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

    iCount = dwNumTriangles * 3;
    for (int i = 0; i < iCount; ++i) {
        TempVertices[i].x /= ((float)m_dwFontHeight); // reduce to a certain size
        TempVertices[i].y /= ((float)m_dwFontHeight); // reduce to a certain size

        *pVertices++ = TempVertices[i];
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    m_iPrimitiveCount = dwNumTriangles;
    m_PrevLeftTop.x = m_PrevLeftTop.y = 0;
    m_Size.cx = (long)(fMaxX / ((float)m_dwFontHeight));
    m_Size.cy = (long)(fMaxY / ((float)m_dwFontHeight));
}

HRESULT CDFont::DrawText(FLOAT sx, FLOAT sy, DWORD dwColor, DWORD dwFlags, FLOAT fZ) {
    if (NULL == m_pVB || NULL == s_hDC || NULL == m_hFont) {
        __ASSERT(0, "NULL Vertex Buffer or DC or Font Handle ");
        return E_FAIL;
    }

    if (m_iPrimitiveCount <= 0) {
        return S_OK;
    }
    if (m_pd3dDevice == NULL || !m_Is2D) {
        return E_FAIL;
    }

    // Position color adjustment
    D3DXVECTOR2 vDiff = D3DXVECTOR2(sx, sy) - m_PrevLeftTop;
    if (fabs(vDiff.x) > 0.5f || fabs(vDiff.y) > 0.5f || dwColor != m_dwFontColor) {
        // lock vertex buffer
        __VertexTransformed * pVertices;
        // m_pVB->Lock( 0, 0, (VOID**)&pVertices, D3DLOCK_NOSYSLOCK );
        m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

        int iVC = m_iPrimitiveCount * 3;
        if (fabs(vDiff.x) > 0.5f) {
            m_PrevLeftTop.x = sx;
            for (int i = 0; i < iVC; ++i) {
                pVertices[i].x += vDiff.x;
            }
        }

        if (fabs(vDiff.y) > 0.5f) {
            m_PrevLeftTop.y = sy;
            for (int i = 0; i < iVC; ++i) {
                pVertices[i].y += vDiff.y;
            }
        }

        if (dwColor != m_dwFontColor) {
            m_dwFontColor = dwColor;
            m_PrevLeftTop.y = sy;
            for (int i = 0; i < iVC; ++i) {
                pVertices[i].color = m_dwFontColor;
            }
        }

        // if (fZ != 1.0f) { // If the Z value does not reach 1.0f, change it.
        //     for (int i = 0; i < iVC; ++i) {
        //         pVertices[i].z = fZ;
        //     }
        // }

        // Unlock
        m_pVB->Unlock();
    }

    // back up render state
    DWORD dwAlphaBlend, dwSrcBlend, dwDestBlend, dwZEnable, dwFog;
    m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
    m_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
    m_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
    m_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    m_pd3dDevice->GetRenderState(D3DRS_FOGENABLE, &dwFog);
    DWORD dwColorOp, dwColorArg1, dwColorArg2, dwAlphaOp, dwAlphaArg1, dwAlphaArg2, dwMinFilter, dwMagFilter;
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwColorArg2);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOp);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwAlphaArg2);
    m_pd3dDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &dwMinFilter);
    m_pd3dDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &dwMagFilter);

    // Set up renderstate
    if (TRUE != dwAlphaBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    }
    if (D3DBLEND_SRCALPHA != dwSrcBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    }
    if (D3DBLEND_INVSRCALPHA != dwDestBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    // if (1.0f == fZ)
    // {
    if (D3DZB_FALSE != dwZEnable) {
        m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    }
    // }
    // else if ( D3DZB_TRUE != dwZEnable) m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );    // If fZ is not 1.0, turn on the z buffer and draw.
    if (FALSE != dwFog) {
        m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    if (D3DTOP_MODULATE != dwColorOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    if (D3DTA_TEXTURE != dwColorArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (D3DTA_DIFFUSE != dwColorArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }
    if (D3DTOP_MODULATE != dwAlphaOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    }
    if (D3DTA_TEXTURE != dwAlphaArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }
    if (D3DTA_DIFFUSE != dwAlphaArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    if (dwFlags & D3DFONT_FILTERED) {
        // Set filter states
        if (D3DTEXF_LINEAR != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        if (D3DTEXF_LINEAR != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        }
    } else {
        if (D3DTEXF_POINT != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
        if (D3DTEXF_POINT != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        }
    }

    // render
    m_pd3dDevice->SetFVF(FVF_TRANSFORMED);
    m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(__VertexTransformed));
    m_pd3dDevice->SetTexture(0, m_pTexture);
    m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iPrimitiveCount);

    // Restore the modified renderstates
    if (TRUE != dwAlphaBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
    }
    if (D3DBLEND_SRCALPHA != dwSrcBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
    }
    if (D3DBLEND_INVSRCALPHA != dwDestBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
    }
    // if (1.0f == fZ)
    // {
    if (D3DZB_FALSE != dwZEnable) {
        m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    }
    // }
    // else if ( D3DZB_TRUE != dwZEnable) m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, dwZEnable );
    if (FALSE != dwFog) {
        m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, dwFog);
    }
    if (D3DTOP_MODULATE != dwColorOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
    }
    if (D3DTA_TEXTURE != dwColorArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1);
    }
    if (D3DTA_DIFFUSE != dwColorArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwColorArg2);
    }
    if (D3DTOP_MODULATE != dwAlphaOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOp);
    }
    if (D3DTA_TEXTURE != dwAlphaArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);
    }
    if (D3DTA_DIFFUSE != dwAlphaArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwAlphaArg2);
    }
    if (dwFlags & D3DFONT_FILTERED) {
        if (D3DTEXF_LINEAR != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
        }
        if (D3DTEXF_LINEAR != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
        }
    } else {
        if (D3DSAMP_MINFILTER != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
        }
        if (D3DSAMP_MAGFILTER != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
        }
    }

    return S_OK;
}

HRESULT CDFont::DrawText3D(DWORD dwColor, DWORD dwFlags) {
    if (NULL == m_pVB || NULL == s_hDC || NULL == m_hFont) {
        __ASSERT(0, "NULL Vertex Buffer or DC or Font Handle ");
        return E_FAIL;
    }

    if (m_iPrimitiveCount <= 0) {
        return S_OK;
    }
    if (m_pd3dDevice == NULL || m_Is2D) {
        return E_FAIL;
    }

    if (dwColor != m_dwFontColor) {
        // lock vertex buffer
        __VertexXyzColorT1 * pVertices;
        // m_pVB->Lock( 0, 0, (VOID**)&pVertices, D3DLOCK_NOSYSLOCK );
        m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

        m_dwFontColor = dwColor;
        int iVC = m_iPrimitiveCount * 3;
        for (int i = 0; i < iVC; ++i) {
            pVertices[i].color = m_dwFontColor;
        }

        m_pVB->Unlock();
    }

    // back up render state
    DWORD dwAlphaBlend, dwSrcBlend, dwDestBlend, dwZEnable, dwFog, dwCullMode, dwLgt;
    m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
    m_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
    m_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
    m_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    m_pd3dDevice->GetRenderState(D3DRS_FOGENABLE, &dwFog);
    m_pd3dDevice->GetRenderState(D3DRS_LIGHTING, &dwLgt);

    DWORD dwColorOp, dwColorArg1, dwColorArg2, dwAlphaOp, dwAlphaArg1, dwAlphaArg2, dwMinFilter, dwMagFilter;
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwColorArg2);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOp);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);
    m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwAlphaArg2);
    m_pd3dDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &dwMinFilter);
    m_pd3dDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &dwMagFilter);
    if (dwFlags & D3DFONT_TWOSIDED) {
        // Turn off culling for two-sided text
        m_pd3dDevice->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
        if (D3DCULL_NONE != dwCullMode) {
            m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        }
    }

    // Set up renderstate
    if (TRUE != dwAlphaBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    }
    if (D3DBLEND_SRCALPHA != dwSrcBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    }
    if (D3DBLEND_INVSRCALPHA != dwDestBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    if (D3DZB_FALSE != dwZEnable) {
        m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    }
    if (FALSE != dwFog) {
        m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    if (FALSE != dwLgt) {
        m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    if (D3DTOP_MODULATE != dwColorOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    if (D3DTA_TEXTURE != dwColorArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    if (D3DTA_DIFFUSE != dwColorArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }
    if (D3DTOP_MODULATE != dwAlphaOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    }
    if (D3DTA_TEXTURE != dwAlphaArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    }
    if (D3DTA_DIFFUSE != dwAlphaArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    if (dwFlags & D3DFONT_FILTERED) {
        // Set filter states
        if (D3DTEXF_LINEAR != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        if (D3DTEXF_LINEAR != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        }
    } else {
        if (D3DTEXF_POINT != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
        if (D3DTEXF_POINT != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        }
    }

    // render
    m_pd3dDevice->SetFVF(FVF_XYZCOLORT1);
    m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(__VertexXyzColorT1));
    m_pd3dDevice->SetTexture(0, m_pTexture);
    m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iPrimitiveCount);

    // Restore the modified renderstates
    if (TRUE != dwAlphaBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
    }
    if (D3DBLEND_SRCALPHA != dwSrcBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
    }
    if (D3DBLEND_INVSRCALPHA != dwDestBlend) {
        m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
    }
    if (D3DZB_FALSE != dwZEnable) {
        m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    }
    if (FALSE != dwFog) {
        m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, dwFog);
    }
    if (FALSE != dwLgt) {
        m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, dwLgt);
    }

    if (D3DTOP_MODULATE != dwColorOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
    }
    if (D3DTA_TEXTURE != dwColorArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1);
    }
    if (D3DTA_DIFFUSE != dwColorArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwColorArg2);
    }
    if (D3DTOP_MODULATE != dwAlphaOp) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOp);
    }
    if (D3DTA_TEXTURE != dwAlphaArg1) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);
    }
    if (D3DTA_DIFFUSE != dwAlphaArg2) {
        m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwAlphaArg2);
    }
    if (dwFlags & D3DFONT_FILTERED) {
        if (D3DTEXF_LINEAR != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
        }
        if (D3DTEXF_LINEAR != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
        }
    } else {
        if (D3DSAMP_MINFILTER != dwMinFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
        }
        if (D3DSAMP_MAGFILTER != dwMagFilter) {
            m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
        }
    }
    if ((dwFlags & D3DFONT_TWOSIDED) && D3DCULL_NONE != dwCullMode) {
        m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, dwCullMode);
    }

    return S_OK;
}

BOOL CDFont::GetTextExtent(const std::string & szString, int iStrLen, SIZE * pSize) {
    if (NULL == s_hDC) {
        return FALSE;
    }

    // TODO: implement GetFont -> sub_40E4D0 v1264 or sub_429700 v1298 client
    SelectObject(s_hDC, m_hFont);
    return ::GetTextExtentPoint32(s_hDC, szString.c_str(), iStrLen, pSize);
}

HRESULT CDFont::SetFontColor(DWORD dwColor) {
    if (m_iPrimitiveCount <= 0 || NULL == m_pVB) {
        return E_FAIL;
    }

    if (dwColor != m_dwFontColor) {
        // lock vertex buffer
        HRESULT hr;
        if (m_Is2D) {
            __VertexTransformed * pVertices;
            if (FAILED(hr = m_pVB->Lock(0, 0, (VOID **)&pVertices, 0))) {
                return hr;
            }
            m_dwFontColor = dwColor;
            int iVC = m_iPrimitiveCount * 3;
            for (int i = 0; i < iVC; ++i) {
                pVertices[i].color = m_dwFontColor;
            }
            m_pVB->Unlock();
        } else {
            __VertexXyzColorT1 * pVertices;
            if (FAILED(hr = m_pVB->Lock(0, 0, (VOID **)&pVertices, 0))) {
                return hr;
            }
            m_dwFontColor = dwColor;
            int iVC = m_iPrimitiveCount * 3;
            for (int i = 0; i < iVC; ++i) {
                pVertices[i].color = m_dwFontColor;
            }
            m_pVB->Unlock();
        }
    }
    return S_OK;
}

void CDFont::AddToAlphaManager(DWORD dwColor, float fDist, __Matrix44 & mtxWorld, DWORD dwFlags) {
    if (NULL == m_pVB || 0 >= m_iPrimitiveCount) {
        return;
    }
    SetFontColor(dwColor);

    __AlphaPrimitive * pAP = s_AlphaMgr.Add();
    if (NULL == pAP) {
        return;
    }

    DWORD dwFVF = FVF_XYZCOLORT1;
    DWORD dwFVFSize = sizeof(__VertexXyzColorT1);
    if (m_Is2D) {
        dwFVF = FVF_TRANSFORMED;
        dwFVFSize = sizeof(__VertexTransformed);

        // 위치 색 조정
        D3DXVECTOR2 vDiff = D3DXVECTOR2(mtxWorld._41, mtxWorld._42) - m_PrevLeftTop;
        if (fabs(vDiff.x) > 0.5f || fabs(vDiff.y) > 0.5f || dwColor != m_dwFontColor) {
            // lock vertex buffer
            __VertexTransformed * pVertices;
            // m_pVB->Lock( 0, 0, (VOID**)&pVertices, D3DLOCK_NOSYSLOCK );
            m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

            int iVC = m_iPrimitiveCount * 3;
            if (fabs(vDiff.x) > 0.5f) {
                m_PrevLeftTop.x = mtxWorld._41;
                for (int i = 0; i < iVC; ++i) {
                    pVertices[i].x += vDiff.x;
                }
            }

            if (fabs(vDiff.y) > 0.5f) {
                m_PrevLeftTop.y = mtxWorld._42;
                for (int i = 0; i < iVC; ++i) {
                    pVertices[i].y += vDiff.y;
                }
            }

            if (dwColor != m_dwFontColor) {
                m_dwFontColor = dwColor;
                m_PrevLeftTop.y = mtxWorld._42;
                for (int i = 0; i < iVC; ++i) {
                    pVertices[i].color = m_dwFontColor;
                }
            }

            // if (fZ != 1.0f) { // If the Z value does not reach 1.0f, change it.
            //     for (int i = 0; i < iVC; ++i) {
            //         pVertices[i].z = fZ;
            //     }
            // }

            // Unlock
            m_pVB->Unlock();
        }
    }

    pAP->bUseVB = TRUE;
    pAP->dwBlendDest = D3DBLEND_INVSRCALPHA;
    pAP->dwBlendSrc = D3DBLEND_SRCALPHA;
    pAP->dwFVF = dwFVF;
    pAP->nPrimitiveCount = m_iPrimitiveCount;
    pAP->ePrimitiveType = D3DPT_TRIANGLELIST;
    pAP->dwPrimitiveSize = dwFVFSize;
    pAP->fCameraDistance = fDist;
    pAP->lpTex = m_pTexture;
    pAP->nRenderFlags = RF_NOTZWRITE | RF_NOTUSELIGHT | RF_NOTUSEFOG;
    pAP->nVertexCount = MAX_NUM_VERTICES;
    pAP->pVertices = m_pVB;
    pAP->pwIndices = NULL;
    pAP->MtxWorld = mtxWorld;

    if (!(dwFlags & D3DFONT_FILTERED)) {
        pAP->nRenderFlags |= RF_POINTSAMPLING; // Do not use filtering text.
    }
}