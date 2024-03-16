// DFont.h: interface for the CDFont class.
// Usage and usage restrictions
// 1. The SetText function requires a lot of calculations. (Call it only when changing the text (important))
// 2. The DrawText function draws on a 2d plane.
// 3. The DrawText3D function is used when drawing in 3D. (3D must be specified in the SetText function.)
// Limitation: Currently, only 50 plates can be created. (If you want to create more than this, you can change MAX_NUM_VERTICES.)
// 4. To write again after calling InvalidateDeviceObjects();, you must call the SetText function again.
// 5. When writing text in 3D space, if you want to align it in the center, enter (D3DFONT_3D|D3DFONT_CENTERED) in the second parameter of the SetText function.
//////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4786)

#pragma once

// TODO: Note that there is some weird mixture of files between engine and game.
// This file is specifically being used by the game's UIs that these UIs not necessarly
// belongs in engine and could be abstracted better, since they all together not being built
// when building the engine as a library.
// I added TODO instead of doing it now to avoid complications during refactoring of
// the directory structure.
#include "N3Base/N3Base.h"

// Font creation flags
#define D3DFONT_BOLD   0x0001
#define D3DFONT_ITALIC 0x0002
#define D3DFONT_3D     0x0008 // 3D text

// set text flag
#define D3DFONT_CENTERED 0x0004 // Only in 3D fonts..

// Font rendering flags (flags used in drawtext or DrawText3D function)
#define D3DFONT_TWOSIDED 0x0010 // Only in 3D fonts..
#define D3DFONT_FILTERED 0x0020 // Take a texture with D3DSAMP_MAGFILTER

class CDFont : public CN3Base {
  public:
    CDFont(const std::string & szFontName, DWORD dwHeight, DWORD dwFlags = 0L);
    virtual ~CDFont();

    // Attributes
  public:
    const std::string & GetFontName() { return m_szFontName; }
    DWORD               GetFontHeight() const { return m_dwFontHeight; }
    int   GetFontHeightInLogicalUnit() const { return -MulDiv(m_dwFontHeight, GetDeviceCaps(s_hDC, LOGPIXELSY), 72); }
    DWORD GetFontFlags() const { return m_dwFontFlags; }
    SIZE  GetSize() const { return m_Size; }
    DWORD GetFontColor() const { return m_dwFontColor; }

  protected:
    static HDC   s_hDC;            // DC handle
    static int   s_iInstanceCount; // Class Instance Count
    static HFONT s_hFontOld;       // default font

    std::string m_szFontName;   // Font properties
    DWORD       m_dwFontHeight; // Font Size
    DWORD       m_dwFontFlags;

    LPDIRECT3DDEVICE9       m_pd3dDevice;  // A D3DDevice used for rendering
    LPDIRECT3DTEXTURE9      m_pTexture;    // The d3d texture for this font
    LPDIRECT3DVERTEXBUFFER9 m_pVB;         // VertexBuffer for rendering text
    DWORD                   m_dwTexWidth;  // Texture dimensions
    DWORD                   m_dwTexHeight; // Texture dimensions
    FLOAT                   m_fTextScale;  // If the font to use is too large, the video card
                                           // Because it exceeds the texture size limit
                                           // In this case, use Scale to enlarge the image.

    // HDC         m_hDC;               // DC handle
    HFONT       m_hFont;             //Font handle
    UINT        m_iPrimitiveCount;  // Number of plates to write on
    D3DXVECTOR2 m_PrevLeftTop;      // In case of DrawText, variable for when the location of the drawing location changes
    DWORD       m_dwFontColor;      // font color
    BOOL        m_Is2D;             // Is it 2D Text?
    SIZE        m_Size;             // Size occupied by written letters (pixel unit, width and height)

    // Operations
  public:
    bool IsSetText() {
        if (m_pTexture) {
            return true;
        }
        return false;
    }
    void    AddToAlphaManager(DWORD dwColor, float fDist, __Matrix44 & mtxWorld, DWORD dwFlags);
    HRESULT SetFontColor(DWORD dwColor);                     // Change the font color.
    HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice); // Initialization function that determines the d3d device (called when Init)
    HRESULT RestoreDeviceObjects();                          // Initialization function to set resources in memory (called when Init)
    HRESULT InvalidateDeviceObjects();                       // Function to invalidate resources, etc. (called when releasing)
    HRESULT DeleteDeviceObjects();                           // Release resources, etc. from memory (called when releasing)

    HRESULT SetText(const std::string & szText, DWORD dwFlags = 0L); // It is important to call this only when the text to be printed changes.
    HRESULT DrawText(FLOAT sx, FLOAT sy, DWORD dwColor, DWORD dwFlags,
                     FLOAT fZ = 1.0f);                // Draw the text stored in the buffer. (2d)
    HRESULT DrawText3D(DWORD dwColor, DWORD dwFlags); // Draw the text stored in the buffer. (3d)

    HRESULT SetFont(const std::string & szFontName, DWORD dwHeight,
                    DWORD dwFlags = 0L); // Called when you want to change the font. (dwHeight enters the point size.)
    BOOL    GetTextExtent(const std::string & szString, int iStrLen, SIZE * pSize);

  protected:
    void Make2DVertex(const int           iFontHeight,
                      const std::string & szText); // Converts the input text into appropriately placed 2D polygons.
    void Make3DVertex(const int iFontHeight, const std::string & szText,
                      DWORD dwFlags); // Creates 3D polygons with appropriately placed input characters.
};
