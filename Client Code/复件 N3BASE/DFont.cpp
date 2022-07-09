// DFont.cpp: implementation of the CDFont class.
//
//////////////////////////////////////////////////////////////////////
#include <D3DX8.h>
#include <assert.h>
#include "DFont.h"

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };

#define MAX_NUM_VERTICES (50*6)
#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

inline FONT3DVERTEX InitFont3DVertex( const D3DXVECTOR3& p, const D3DXVECTOR3& n,
                                      FLOAT tu, FLOAT tv )
{
    FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
    return v;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDFont::CDFont(const char* strFontName, DWORD dwHeight, DWORD dwFlags)
{
	lstrcpy(m_strFontName, strFontName);
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;

    m_pd3dDevice           = NULL;
    m_pTexture             = NULL;
    m_pVB                  = NULL;

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
	m_iPrimitiveCount = 0;
	m_PrevLeftTop.x = m_PrevLeftTop.y = 0;

	m_hDC = NULL;
	m_hFont = NULL;
	m_dwFontColor = 0xffffffff;
	m_Is2D = TRUE;
	m_Size.cx = 0; m_Size.cy = 0;
}

HRESULT CDFont::SetFont(const char* strFontName, DWORD dwHeight, DWORD dwFlags)
{
	lstrcpy(m_strFontName, strFontName);
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;

	if (m_hFont) {DeleteObject(m_hFont);	m_hFont = NULL;}

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = -MulDiv( m_dwFontHeight, 
        (INT)(GetDeviceCaps(m_hDC, LOGPIXELSY) * m_fTextScale), 72 );
    DWORD dwBold	= (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic	= (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
    m_hFont			= CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==m_hFont ) return E_FAIL;
    SelectObject( m_hDC, m_hFont );
	return S_OK;
}

CDFont::~CDFont()
{
    InvalidateDeviceObjects();
    DeleteDeviceObjects();
}

HRESULT CDFont::InitDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice )
{
	// Keep a local copy of the device
	m_pd3dDevice = pd3dDevice;
	m_fTextScale = 1.0f; // Draw fonts into texture without scaling

	return S_OK;
}

HRESULT CDFont::RestoreDeviceObjects()
{
    HRESULT hr;

	m_iPrimitiveCount = 0;

	assert(NULL==m_hDC && NULL==m_hFont);
	m_hDC = CreateCompatibleDC(NULL);
	if( NULL==m_hDC ) return E_FAIL;
	SetMapMode( m_hDC, MM_TEXT );

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = -MulDiv( m_dwFontHeight, 
        (INT)(GetDeviceCaps(m_hDC, LOGPIXELSY) * m_fTextScale), 72 );
    DWORD dwBold	= (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic	= (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
    m_hFont			= CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==m_hFont ) return E_FAIL;
    SelectObject( m_hDC, m_hFont );

    // Create vertex buffer for the letters
	assert(m_pVB == NULL);
    if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( MAX_NUM_VERTICES*sizeof(FONT3DVERTEX),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_pVB ) ) )
    {
        return hr;
    }

    // Create the state blocks for rendering text
    for( UINT which=0; which<2; which++ )
    {
        m_pd3dDevice->BeginStateBlock();
        m_pd3dDevice->SetTexture( 0, NULL );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
        m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_EDGEANTIALIAS,    FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

        if( which==0 )
            m_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
        else
            m_pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
    }

    return S_OK;
}

HRESULT CDFont::InvalidateDeviceObjects()
{
    if (m_pVB) {m_pVB->Release(); m_pVB = NULL;}

    // Delete the state blocks
    if( m_pd3dDevice )
    {
        if( m_dwSavedStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
        if( m_dwDrawTextStateBlock )
            m_pd3dDevice->DeleteStateBlock( m_dwDrawTextStateBlock );
    }
	m_dwSavedStateBlock    = 0L;
	m_dwDrawTextStateBlock = 0L;

	if (m_hFont) {DeleteObject(m_hFont);	m_hFont = NULL;}
	if (m_hDC) {DeleteDC(m_hDC);	m_hDC = NULL;}

	return S_OK;
}

HRESULT CDFont::DeleteDeviceObjects()
{
	if (m_pTexture) {m_pTexture->Release(); m_pTexture = NULL;}
	m_pd3dDevice = NULL;

	return S_OK;
}

HRESULT CDFont::SetText(const char* pszText, DWORD dwFlags)
{
	m_Is2D = (dwFlags & D3DFONT_3D) ? FALSE : TRUE;
	HRESULT hr;
	// \n을 빼고 한줄로 만들어서 글자 길이 계산하기
	int iCount=0, iStrLen = lstrlen(pszText);
	int iTempCount = 0;
	SIZE size;
	if (iStrLen>0)
	{
		char* pszTemp = new char[iStrLen+1];
		while(iCount<iStrLen)
		{
			if ('\n' == pszText[iCount])		// \n
			{
				++iCount;
			}
			else if (pszText[iCount] & 0x80)	// 2BYTE 문자
			{
				memcpy(pszTemp+iTempCount, pszText+iCount, 2);
				iTempCount += 2;
				iCount += 2;
			}
			else								// 1BYTE 문자
			{
				memcpy(pszTemp+iTempCount, pszText+iCount, 1);
				++iTempCount;	++iCount;
			}
		}
		pszTemp[iTempCount] = 0x00;
		GetTextExtentPoint32( m_hDC, pszTemp, lstrlen(pszTemp), &size );
		delete [] pszTemp;
		assert(size.cx>0 && size.cy>0);
		int	iExtent = size.cx*size.cy;

		SIZE size2;	// 한글 반글자의 크기..
		GetTextExtentPoint32( m_hDC, "진", lstrlen("진"), &size2 );
		size2.cx = ((size2.cx/2) + (size2.cx%2));

		if (iExtent > (1048576 - size2.cx*1024))		// 1024*1024
			m_dwTexWidth = m_dwTexHeight = 2048;
		else if (iExtent > (262144 - size2.cx*512))	// 512*512
			m_dwTexWidth = m_dwTexHeight = 1024;
		else if (iExtent > (65536 - size2.cx*256))	// 256*256
			m_dwTexWidth = m_dwTexHeight = 512;
		else if (iExtent > (16384 - size2.cx*128)) // 128*128;
			m_dwTexWidth = m_dwTexHeight = 256;
		else if (iExtent > (4096 - size2.cx*64))	// 64*64;
			m_dwTexWidth = m_dwTexHeight = 128;
		else
			m_dwTexWidth = m_dwTexHeight = 64;
	}
	else
	{
		m_iPrimitiveCount = 0;
		if (m_pTexture) {m_pTexture->Release(); m_pTexture = NULL;}
		return S_OK;
	}

    // Establish the font and texture size
    m_fTextScale  = 1.0f; // Draw fonts into texture without scaling

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS8 d3dCaps;
    m_pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth > d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = (float)d3dCaps.MaxTextureWidth / (float)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

	// 기존 텍스쳐 크기가 새로 만들 텍스쳐 크기와 다를 경우 다시 만든다.
	if (m_pTexture)
	{
		D3DSURFACE_DESC sd;
		ZeroMemory( &sd,  sizeof(sd) );
		m_pTexture->GetLevelDesc(0, &sd);
		if (sd.Width != m_dwTexWidth)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
	}

    // Create a new texture for the font
	if (NULL == m_pTexture)
	{
		hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
										0, D3DFMT_A4R4G4B4,
										D3DPOOL_MANAGED, &m_pTexture );
		if( FAILED(hr) )
			return hr;
	}

    // Prepare to create a bitmap
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int)m_dwTexWidth;
    bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HBITMAP hbmBitmap = CreateDIBSection( m_hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SelectObject( m_hDC, hbmBitmap );

    // Set text properties
    SetTextColor( m_hDC, RGB(255,255,255) );
    SetBkColor(   m_hDC, RGB(0,0,0) );
    SetTextAlign( m_hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.

// 글씨 찍기 및 글씨 찍을 판떼기 만들기
	if (m_Is2D)
	{
		Make2DVertex(size.cy, pszText);
	}
	else
	{
		Make3DVertex(size.cy, pszText, dwFlags);
	}

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
    WORD* pDst16 = (WORD*)d3dlr.pBits;
    BYTE bAlpha; // 4-bit measure of pixel intensity

	DWORD x, y;
    for( y=0; y < m_dwTexHeight; y++ )
    {
        for( x=0; x < m_dwTexWidth; x++ )
        {
            bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth*y + x] & 0xff) >> 4);
            if (bAlpha > 0)
            {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            }
            else
            {
                *pDst16++ = 0x0000;
            }
        }
    }

    // Done updating texture, so clean up used objects
    m_pTexture->UnlockRect(0);
    DeleteObject( hbmBitmap );

	return S_OK;

}

void CDFont::Make2DVertex(const int iFontHeight, const char* pszText)
{
	int iStrLen = lstrlen(pszText);
	// lock vertex buffer
	FONT2DVERTEX* pVertices = NULL;
	DWORD         dwNumTriangles = 0;
	assert(m_pVB);
	m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	DWORD sx = 0;    // start x y
	DWORD x = 0;    DWORD y = 0;
	float vtx_sx = 0;    float vtx_sy = 0;		//	vertex start x y 
	int iCount = 0; int iTempCount = 0;

	char	szTempChar[3] = "";
	DWORD dwColor = 0xffffffff;			// 폰트의 색
	m_dwFontColor = 0xffffffff;
	SIZE size;

	float fMaxX = 0.0f, fMaxY = 0.0f;	// 글씨가 찍히는 범위의 최대 최소값을 조사하기 위해서.

	while(iCount<iStrLen)
	{
		if ('\n' == pszText[iCount])		// \n
		{
			++iCount;

			// vertex 만들기
			if (sx != x)
			{
				FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
				FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
				FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
				FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

				FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
				FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

				assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.
				if (dwNumTriangles+2 >= MAX_NUM_VERTICES) break;

				FLOAT fLeft = vtx_sx+0-0.5f;	FLOAT fRight  = vtx_sx+w-0.5f;
				FLOAT fTop  = vtx_sy+0-0.5f;	FLOAT fBottom = vtx_sy+h-0.5f;
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fBottom,0.9f,1.0f), dwColor, tx1, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fTop   ,0.9f,1.0f), dwColor, tx2, ty1 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );

//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+0-0.5f,vtx_sy+h-0.5f,0.9f,1.0f), dwColor, tx1, ty2 );
//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+0-0.5f,vtx_sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+w-0.5f,vtx_sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+w-0.5f,vtx_sy+0-0.5f,0.9f,1.0f), dwColor, tx2, ty1 );
//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+w-0.5f,vtx_sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
//				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(vtx_sx+0-0.5f,vtx_sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
				dwNumTriangles += 2;

				if (fMaxX < fRight ) fMaxX = fRight;
				if (fMaxY < fBottom) fMaxY = fBottom;

			}
			// 화면의 다음 줄로 넘기기
			sx = x;
			vtx_sx = 0;	vtx_sy = vtx_sy + ((float)(iFontHeight)) / m_fTextScale;
			continue;
		}
		else if (pszText[iCount] & 0x80)	// 2BYTE 문자
		{
			memcpy(szTempChar, pszText+iCount, 2);
			iCount += 2;
			szTempChar[2] = 0x00;
		}
		else								// 1BYTE 문자
		{
			memcpy(szTempChar, pszText+iCount, 1);
			iCount += 1;
			szTempChar[1] = 0x00;
		}

		GetTextExtentPoint32( m_hDC, szTempChar, lstrlen(szTempChar), &size );
		if ( (x + size.cx) > m_dwTexWidth)	
		{	// vertex 만들고 다음 줄로 넘기기..
			// vertex 만들기
			if (sx != x)
			{
				FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
				FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
				FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
				FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

				FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
				FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

				assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.
				if (dwNumTriangles+2 >= MAX_NUM_VERTICES) break;

				FLOAT fLeft = vtx_sx+0-0.5f;	FLOAT fRight  = vtx_sx+w-0.5f;
				FLOAT fTop  = vtx_sy+0-0.5f;	FLOAT fBottom = vtx_sy+h-0.5f;
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fBottom,0.9f,1.0f), dwColor, tx1, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fTop   ,0.9f,1.0f), dwColor, tx2, ty1 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
				*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );
				dwNumTriangles += 2;

				if (fMaxX < fRight ) fMaxX = fRight;
				if (fMaxY < fBottom) fMaxY = fBottom;

				// 텍스쳐의 다음 줄로 넘기기
				x = sx = 0;	y += iFontHeight;
				vtx_sx = vtx_sx+w;
			}
			else
			{
				x = sx = 0;	y += iFontHeight;
			}
		}
		
		// dc에 찍기
		ExtTextOut( m_hDC, x, y, ETO_OPAQUE, NULL, szTempChar, lstrlen(szTempChar), NULL );		
		x += size.cx;
	}

	// 마지막 남은 vertex 만들기
	if (sx != x)
	{
		FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
		FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
		FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
		FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

		FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
		FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

		assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.

		FLOAT fLeft = vtx_sx+0-0.5f;	FLOAT fRight  = vtx_sx+w-0.5f;
		FLOAT fTop  = vtx_sy+0-0.5f;	FLOAT fBottom = vtx_sy+h-0.5f;
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fBottom,0.9f,1.0f), dwColor, tx1, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fTop   ,0.9f,1.0f), dwColor, tx2, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fRight, fBottom,0.9f,1.0f), dwColor, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(fLeft , fTop   ,0.9f,1.0f), dwColor, tx1, ty1 );
		dwNumTriangles += 2;

		if (fMaxX < fRight ) fMaxX = fRight;
		if (fMaxY < fBottom) fMaxY = fBottom;
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();
	m_iPrimitiveCount = dwNumTriangles;
	m_PrevLeftTop.x = m_PrevLeftTop.y = 0;
	m_Size.cx = (long)fMaxX;	m_Size.cy = (long)fMaxY;
}

void CDFont::Make3DVertex(const int iFontHeight, const char* pszText, DWORD dwFlags)
{
	int iStrLen = lstrlen(pszText);
	// 임시 vertex buffer에 넣기
	FONT3DVERTEX	TempVertices[MAX_NUM_VERTICES];
	FONT3DVERTEX* pVertices = TempVertices;
	DWORD         dwNumTriangles = 0;

	DWORD sx = 0;    // start x y
	DWORD x = 0;    DWORD y = 0;
	float vtx_sx = 0;    float vtx_sy = 0;		//	vertex start x y 
	int iCount = 0; int iTempCount = 0;

	char	szTempChar[3] = "";
	DWORD dwColor = 0xffffffff;			// 폰트의 색
	m_dwFontColor = 0xffffffff;
	SIZE size;

	float fMaxX = 0.0f, fMaxY = 0.0f;	// 글씨가 찍히는 범위의 최대 최소값을 조사하기 위해서.

	while(iCount<iStrLen)
	{
		if ('\n' == pszText[iCount])		// \n
		{
			++iCount;

			// vertex 만들기
			if (sx != x)
			{
				FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
				FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
				FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
				FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

				FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
				FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

				assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.
				if (dwNumTriangles+2 >= MAX_NUM_VERTICES) break;

				FLOAT fLeft = vtx_sx+0;	FLOAT fRight  = vtx_sx+w;
				FLOAT fTop  = vtx_sy+0;	FLOAT fBottom = vtx_sy-h;
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fBottom, 0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fTop   , 0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );

//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+0,vtx_sy+h, 0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+0,vtx_sy+0, 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+w,vtx_sy+h, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+w,vtx_sy+0, 0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+w,vtx_sy+h, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
//				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(vtx_sx+0,vtx_sy+0, 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
				dwNumTriangles += 2;
				if (fMaxX < fRight ) fMaxX = fRight;
				if (fMaxY < (-fBottom)) fMaxY = (-fBottom);
			}
			// 화면의 다음 줄로 넘기기
			sx = x;
			vtx_sx = 0;	vtx_sy = vtx_sy - ((float)(iFontHeight)) / m_fTextScale;
			continue;
		}
		else if (pszText[iCount] & 0x80)	// 2BYTE 문자
		{
			memcpy(szTempChar, pszText+iCount, 2);
			iCount += 2;
			szTempChar[2] = 0x00;
		}
		else								// 1BYTE 문자
		{
			memcpy(szTempChar, pszText+iCount, 1);
			iCount += 1;
			szTempChar[1] = 0x00;
		}

		GetTextExtentPoint32( m_hDC, szTempChar, lstrlen(szTempChar), &size );
		if ( (x + size.cx) > m_dwTexWidth)	
		{	// vertex 만들고 다음 줄로 넘기기..
			// vertex 만들기
			if (sx != x)
			{
				FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
				FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
				FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
				FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

				FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
				FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

				assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.
				if (dwNumTriangles+2 >= MAX_NUM_VERTICES) break;

				FLOAT fLeft = vtx_sx+0;	FLOAT fRight  = vtx_sx+w;
				FLOAT fTop  = vtx_sy+0;	FLOAT fBottom = vtx_sy-h;
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fBottom, 0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fTop   , 0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
				*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
				dwNumTriangles += 2;
				if (fMaxX < fRight ) fMaxX = fRight;
				if (fMaxY < (-fBottom)) fMaxY = (-fBottom);

				// 텍스쳐의 다음 줄로 넘기기
				x = sx = 0;	y += iFontHeight;
				vtx_sx = vtx_sx+w;
			}
			else
			{
				x = sx = 0;	y += iFontHeight;
			}
		}
		
		// dc에 찍기
		ExtTextOut( m_hDC, x, y, ETO_OPAQUE, NULL, szTempChar, lstrlen(szTempChar), NULL );		
		x += size.cx;
	}

	// 마지막 남은 vertex 만들기
	if (sx != x)
	{
		FLOAT tx1 = ((FLOAT)(sx))/m_dwTexWidth;
		FLOAT ty1 = ((FLOAT)(y))/m_dwTexHeight;
		FLOAT tx2 = ((FLOAT)(x))/m_dwTexWidth;
		FLOAT ty2 = ((FLOAT)(y+iFontHeight))/m_dwTexHeight;

		FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
		FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

		assert(dwNumTriangles+2 < MAX_NUM_VERTICES);		// Vertex buffer가 모자란다.

		FLOAT fLeft = vtx_sx+0;	FLOAT fRight  = vtx_sx+w;
		FLOAT fTop  = vtx_sy+0;	FLOAT fBottom = vtx_sy-h;
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fBottom, 0), D3DXVECTOR3(0,0,-1), tx1, ty2 );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fTop   , 0), D3DXVECTOR3(0,0,-1), tx2, ty1 );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fRight,fBottom, 0), D3DXVECTOR3(0,0,-1), tx2, ty2 );
		*pVertices++ = InitFont3DVertex( D3DXVECTOR3(fLeft ,fTop   , 0), D3DXVECTOR3(0,0,-1), tx1, ty1 );
		dwNumTriangles += 2;
		if (fMaxX < fRight ) fMaxX = fRight;
		if (fMaxY < (-fBottom)) fMaxY = (-fBottom);
	}

	int i;
	if (dwFlags & D3DFONT_CENTERED)	// 가운데 정렬이면 vertex좌표를 가운데로 계산해서 고쳐넣기
	{
		// 제일 긴 줄 찾기..
		int iRectangleCount = dwNumTriangles/2;

		int iContinueCount = 1;

		float fCX = 0;
		float fCY = 0;
		iCount = 0;
		while (iCount < iRectangleCount)
		{
			iContinueCount = 1;
			fCX = TempVertices[iCount*6 + 3].p.x;
			fCY = TempVertices[iCount*6].p.y;

			while( iCount + iContinueCount < iRectangleCount)
			{
				if (TempVertices[(iCount + iContinueCount)*6].p.y == fCY)
				{	// 다음 사각형과 같은 줄이다.
					fCX = TempVertices[(iCount + iContinueCount)*6 + 3].p.x;
					++iContinueCount;
				}
				else
				{	// 다음 사각형과 다른 줄이다.
					break;
				}
			}
			assert(fCX>0.0f);
			float fDiffX = -fCX/2.0f;
			for (i=iCount; i<iCount+iContinueCount; ++i)
			{
				for (int j=0; j<6; ++j)
					TempVertices[i*6+j].p.x += fDiffX;
			}
			iCount += iContinueCount;
		}
	}

	// Vertex buffer로 옮기기.
	// lock vertex buffer
	m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	iCount = dwNumTriangles*3;
	for (i=0; i<iCount; ++i)
	{
		TempVertices[i].p /= ((float)m_dwFontHeight);			// 일정 크기로 줄이기
		*pVertices++ = TempVertices[i];
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();
	m_iPrimitiveCount = dwNumTriangles;
	m_PrevLeftTop.x = m_PrevLeftTop.y = 0;
	m_Size.cx = (long)(fMaxX/((float)m_dwFontHeight));
	m_Size.cy = (long)(fMaxY/((float)m_dwFontHeight));
}

HRESULT CDFont::DrawText( FLOAT sx, FLOAT sy, DWORD dwColor, DWORD dwFlags )
{
	if (m_iPrimitiveCount <= 0) return S_OK;
    if( m_pd3dDevice == NULL || !m_Is2D)
        return E_FAIL;

    // Set up renderstate
    m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT2DVERTEX );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT2DVERTEX) );

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    }

	D3DXVECTOR2 vDiff = D3DXVECTOR2(sx, sy) - m_PrevLeftTop;
	if ( fabs(vDiff.x)>0.5f || fabs(vDiff.y)>0.5f || dwColor != m_dwFontColor)
	{
		// lock vertex buffer
		FONT2DVERTEX* pVertices;
		m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_NOSYSLOCK);

		int i, iVC = m_iPrimitiveCount*3;
		if (fabs(vDiff.x)>0.5f)
		{
			m_PrevLeftTop.x = sx;
			for (i=0; i<iVC; ++i)
			{
				pVertices[i].p.x += vDiff.x;
			}
		}

		if (fabs(vDiff.y)>0.5f)
		{
			m_PrevLeftTop.y = sy;
			for (i=0; i<iVC; ++i)
			{
				pVertices[i].p.y += vDiff.y;
			}
		}

		if (dwColor != m_dwFontColor)
		{
			m_dwFontColor = dwColor;
			m_PrevLeftTop.y = sy;
			for (i=0; i<iVC; ++i)
			{
				pVertices[i].color = m_dwFontColor;
			}
		}

		// Unlock
		m_pVB->Unlock();
	}

	m_pd3dDevice->SetTexture( 0, m_pTexture );

    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_iPrimitiveCount );

    // Restore the modified renderstates
    m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

    return S_OK;
}

HRESULT CDFont::Render3DText(DWORD dwColor, DWORD dwFlags )
{
	if (m_iPrimitiveCount <= 0) return S_OK;
    if( m_pd3dDevice == NULL || m_Is2D)
        return E_FAIL;

	D3DMATERIAL8 mtrl, mtrlOld;
	memset(&mtrl, 0, sizeof(mtrl));
	float a = ((dwColor>>24) & 0xff)/255.0f;
	float r = ((dwColor>>16) & 0xff)/255.0f;
	float g = ((dwColor>>8 ) & 0xff)/255.0f;
	float b = ((dwColor>>0 ) & 0xff)/255.0f;
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
	m_pd3dDevice->GetMaterial( &mtrlOld );
	m_pd3dDevice->SetMaterial( &mtrl );

    // Setup renderstate
    m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
    m_pd3dDevice->SetVertexShader( D3DFVF_FONT3DVERTEX );
    m_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(FONT3DVERTEX) );

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    }

    // Turn off culling for two-sided text
    if( dwFlags & D3DFONT_TWOSIDED )
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_pd3dDevice->SetTexture( 0, m_pTexture );
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_iPrimitiveCount );

    // Restore the modified renderstates
    m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );
	m_pd3dDevice->SetMaterial( &mtrlOld );

	return S_OK;
}