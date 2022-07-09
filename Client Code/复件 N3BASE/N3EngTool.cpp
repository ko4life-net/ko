// N3EngTool.cpp: implementation of the CN3EngTool class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Texture.h"
#include "N3Light.h"
#include "N3EngTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3EngTool::CN3EngTool()
{
	// 십자선 생성..
	int i = 0;
	for(i = -10; i < 10; i++)
	{
		m_VAxis[ 0 + i + 10].Set(i * 500.0f, 0, 0, 0xffff0000); // X
		m_VAxis[20 + i + 10].Set(0, i * 500.0f, 0, 0xff00ff00); // Y
		m_VAxis[40 + i + 10].Set(0, 0, i * 500.0f, 0xff0000ff); // Z
	}
	
	m_pVGrids = NULL;
	m_nGridLineCount = 0;
	
//	m_lpD3DDevExtra = NULL;
}

CN3EngTool::~CN3EngTool()
{
	delete [] m_pVGrids; m_pVGrids = NULL;
//	if(m_lpD3DDevExtra) m_lpD3DDevExtra->Release();
//	m_lpD3DDevExtra = NULL;
}

// 월드 축 그리기..
void CN3EngTool::RenderGrid(float fScale)
{
	if(fScale <= 0) fScale = 1.0f;

	DWORD dwAlpha, dwFog, dwLight;
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Matrix44 stm;
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		stm.Identity();
		smtl.Init();
		bInit = true;
	}

	stm.Scale(fScale, fScale, fScale); // 스케일 적용

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &stm);
	s_lpD3DDev->SetTexture(0, NULL);

	s_lpD3DDev->SetVertexShader(FVF_CV);

	if(m_pVGrids) // 그리드 그리기..
	{
		s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, m_nGridLineCount, m_pVGrids, sizeof(__VertexColor)); // X
	}

	// 축 그리기..
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[0], sizeof(__VertexColor)); // X
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[20], sizeof(__VertexColor)); // Y
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[40], sizeof(__VertexColor)); // Z

	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CN3EngTool::RenderAxis()
{
	DWORD dwAlpha, dwFog, dwLight;
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLight);
	
	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	
	static __Matrix44 stm;
	static __Material smtl;
	static bool bInit = false;
	if(false == bInit)
	{
		stm.Identity();
		smtl.Init();
		bInit = true;
	}

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &stm);
	s_lpD3DDev->SetTexture(0, NULL);

	s_lpD3DDev->SetVertexShader(FVF_CV);

	// 축 그리기..
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[0], sizeof(__VertexColor)); // X
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[20], sizeof(__VertexColor)); // Y
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 19, &m_VAxis[40], sizeof(__VertexColor)); // Z

	if(dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if(dwAlpha) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	if(dwLight) s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLight);
}

void CN3EngTool::RenderTexturePreview(CN3Texture *pTex, HWND hWndDiffuse)
{
	if(pTex == NULL) return;
	
	if(hWndDiffuse != NULL)
	{
		// 텍스처 프리뷰
		RECT rcPreview;
		::GetClientRect(hWndDiffuse, &rcPreview);

		int nW = rcPreview.right - rcPreview.left;
		int nH = rcPreview.bottom - rcPreview.top;

		D3DRECT rcClear = { rcPreview.left, rcPreview.top, rcPreview.right, rcPreview.bottom };
		s_lpD3DDev->Clear(1, &rcClear, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,64,64,64), 1.0f, 0);

		if(pTex->Get() != NULL)
		{
			RECT rcTex = rcPreview;
			float fRatioDest = (float)nW / (float)nH;
			float fRatioSrc = (float)(pTex->Width()) / (float)(pTex->Height());

			if(fRatioDest > 1.0f) // 가로가 긴경우
			{
				int nDec = (nW - nH) / 2;
				rcTex.left += nDec;
				rcTex.right -= nDec;
			}
			else if(fRatioDest < 1.0f) // 세로가 긴경우
			{
				int nDec = (nH - nW) / 2;
				rcTex.top += nDec;
				rcTex.bottom -= nDec;
			}


			s_lpD3DDev->BeginScene();

			m_VPreview[0].Set((float)rcTex.left, (float)rcTex.top, 0.5f, 1.0f, 0xffffffff, 0.0f, 0.0f);
			m_VPreview[1].Set((float)rcTex.right, (float)rcTex.top, 0.5f, 1.0f, 0xffffffff, 1.0f, 0.0f);
			m_VPreview[2].Set((float)rcTex.right, (float)rcTex.bottom, 0.5f, 1.0f, 0xffffffff, 1.0f, 1.0f);
			m_VPreview[3] = m_VPreview[0];
			m_VPreview[4] = m_VPreview[2];
			m_VPreview[5].Set((float)rcTex.left, (float)rcTex.bottom, 0.5f, 1.0f, 0xffffffff, 0.0f, 1.0f);

			DWORD dwAlphaUse = FALSE;
			if(	pTex->PixelFormat() != D3DFMT_DXT1 && 
				pTex->PixelFormat() != D3DFMT_X8R8G8B8 )
			{
				s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaUse);
				if(TRUE != dwAlphaUse) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
				s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);
			}

			s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			s_lpD3DDev->SetTexture(0, pTex->Get());

			s_lpD3DDev->SetVertexShader(FVF_TRANSFORMED);
			s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_VPreview, sizeof(__VertexTransformed));

			if(TRUE != dwAlphaUse) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaUse);
			
			s_lpD3DDev->EndScene();
		}
		
		s_lpD3DDev->Present(&rcPreview, &rcPreview, hWndDiffuse, NULL);
	}
}


void CN3EngTool::GridCreate(int nWidth, int nHeight)
{
	if(nWidth <= 0 || nHeight <= 0) return;

	delete [] m_pVGrids;
	m_nGridLineCount = (nWidth+1) + (nHeight+1);
	m_pVGrids = new __VertexColor[m_nGridLineCount * 2];

	// 그리드 생성..
	D3DCOLOR GridColor = 0xff808080;
	int xx = nWidth/2, zz = nHeight/2;
	int nSeq = 0;
	for(int x = -xx; x <= xx; x++)
	{
		m_pVGrids[nSeq++].Set((float)x, 0, (float) zz, GridColor);
		m_pVGrids[nSeq++].Set((float)x, 0, (float)-zz, GridColor);
	}
	for(int z = -zz; z <= zz; z++)
	{
		m_pVGrids[nSeq++].Set((float) xx, 0, (float)z, GridColor);
		m_pVGrids[nSeq++].Set((float)-xx, 0, (float)z, GridColor);
	}
}
/*
bool CN3EngTool::CreateExtraDevice(int nWidth, int nHeight)
{
	if(NULL == s_hWndDevice) return false;

	D3DDEVTYPE DevType = D3DDEVTYPE_HAL;
	D3DPRESENT_PARAMETERS DevParam;
	memset(&DevParam, 0, sizeof(DevParam));
	DevParam.Windowed = TRUE;
	DevParam.hDeviceWindow = s_hWndDevice;
	DevParam.EnableAutoDepthStencil = TRUE;
	DevParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	DevParam.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	s_DevParam.BackBufferWidth = nWidth;
	s_DevParam.BackBufferHeight = nHeight;
	D3DDISPLAYMODE dm;
	m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
	s_DevParam.BackBufferFormat = dm.Format;
	s_DevParam.MultiSampleType = D3DMULTISAMPLE_NONE; // Swap Effect 가 Discard 형태가 아니면 반드시 이런 식이어야 한다.

	int nMC = m_DeviceInfo.nModeCount;
	for(int i = 0; i < nMC; i++)
	{
//		if(	m_DeviceInfo.pModes[i].Width == dwWidth && 
//			m_DeviceInfo.pModes[i].Height == dwHeight && 
		if(	m_DeviceInfo.pModes[i].Format == dm.Format) // 모드가 일치하면
		{
			this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format, &DevParam.AutoDepthStencilFormat); // 깊이와 스텐실 버퍼를 찾는다.
			break;
		}
	}

	HRESULT rval = 0;
	rval = m_lpD3D->CreateDevice(0, DevType, s_hWndBase, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DevParam, &m_lpD3DDevExtra);
	if(rval != D3D_OK)
	{
		rval = m_lpD3D->CreateDevice(0, DevType, s_hWndBase, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &s_DevParam, &m_lpD3DDevExtra);
		if(rval != D3D_OK)
		{
			char szDebug[256];
			D3DXGetErrorString(rval, szDebug, 256);
			MessageBox(s_hWndBase, szDebug, "Direct3D Device 생성 실패", MB_OK);
			this->Release();
			return false;
		}
#if _DEBUG
		OutputDebugString("HardWare TnL 이 지원되지 않습니다. SoftWare TnL 로 진행 합니다.\n");
#endif
	}

	// 기본 라이트 정보 지정..
	for(i = 0; i < 8; i++)
	{
		CN3Light::__Light Lgt;
		_D3DCOLORVALUE LgtColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Lgt.InitPoint(__Vector3(0,0,0), LgtColor);
		m_lpD3DDevExtra->SetLight(i, &Lgt);
	}

	// 기본 뷰와 프로젝션 설정.
	__Matrix44 matView;
	__Vector3 vEye(5,5,-10), vAt(0,0,0), vUp(0,1,0);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	m_lpD3DDevExtra->SetTransform(D3DTS_VIEW, &matView);

	__Matrix44 matProjection;
	::D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45.0f), (float)nHeight/nWidth, 0.11f, 2560.0f);
	m_lpD3DDevExtra->SetTransform(D3DTS_PROJECTION, &matProjection);
	
	D3DVIEWPORT8 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = nWidth;
	vp.Height = nHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	m_lpD3DDevExtra->SetViewport(&vp);

	m_lpD3DDevExtra->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); // Z버퍼 사용가능
	m_lpD3DDevExtra->SetRenderState(D3DRS_LIGHTING, TRUE);

    m_lpD3DDevExtra->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
    m_lpD3DDevExtra->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
//    m_lpD3DDevExtra->SetRenderState( D3DRS_AMBIENT,        0x00444444 );

	m_lpD3DDevExtra->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_lpD3DDevExtra->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기본 알파 펑션 - 안해주면 알파 텍스처들이 빵꾸나기도 한다.
	
	m_lpD3DDevExtra->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_lpD3DDevExtra->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 기본 텍스처 필터 지정.
	float fMipMapLODBias = -1.0f;
	for(i = 0; i < 8; i++)
	{
		m_lpD3DDevExtra->SetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		m_lpD3DDevExtra->SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		m_lpD3DDevExtra->SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		m_lpD3DDevExtra->SetTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fMipMapLODBias)));
	}

	// 클리핑 상태 지정
	D3DCLIPSTATUS8 cs; cs.ClipUnion = cs.ClipIntersection = D3DCS_ALL;
	m_lpD3DDevExtra->SetClipStatus(&cs);	

	return true;
}
*/