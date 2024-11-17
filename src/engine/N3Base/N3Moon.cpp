// N3Moon.cpp: implementation of the CN3Moon class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3Moon.h"
#include "My_3DStruct.h"
#include "N3Texture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Moon::CN3Moon() {
    m_pTexture = NULL;
    m_fCurRadian = D3DX_PI;
    m_iMoonPhaseIndex = 0;
}

CN3Moon::~CN3Moon() {
    s_MngTex.Delete(&m_pTexture);
}

void CN3Moon::Release() {
    CN3Base::Release();
    s_MngTex.Delete(&m_pTexture);
    m_fCurRadian = D3DX_PI;
    m_iMoonPhaseIndex = 0;
}

void CN3Moon::Init(const fs::path & fsTexFile) {
    Release();
    m_pTexture = s_MngTex.Get(fsTexFile);
    __ASSERT(m_pTexture, "Texture load failed.");

    m_fMoonRatio = 0.2f;
    const float fZ = 0.9f;
    const float fRHW = 1.0f;
    D3DCOLOR    color = 0xffffffff;
    m_pVertices[0].Set(0, 0, fZ, fRHW, color, 0.0f, 0.0f);
    m_pVertices[1].Set(0, 0, fZ, fRHW, color, 1.0f, 0.0f);
    m_pVertices[2].Set(0, 0, fZ, fRHW, color, 1.0f, 1.0f);
    m_pVertices[3].Set(0, 0, fZ, fRHW, color, 0.0f, 1.0f);

    SetMoonPhase(10);
}

void CN3Moon::Render(__Matrix44 & matView, __Matrix44 & matProj) {
    // 달이 그려지는 화면 좌표 계산하기 (2D로 그리는 이유는 달이 왜곡되어서 보이는것을 막기 위해)
    __Matrix44 matWorld;
    matWorld.RotationZ(m_fCurRadian);
    __Matrix44 matFinal;
    matFinal = matWorld * matView;
    matFinal *= matProj;

    __Vector3 vMoon;
    vMoon.Set(5, 0, 0);
    D3DXVECTOR4 vOut;
    D3DXVec3Transform(&vOut, (D3DXVECTOR3 *)(&vMoon), &matFinal);

    int   Width = s_CameraData.vp.Width;
    int   Height = s_CameraData.vp.Height;
    int   X = s_CameraData.vp.X;
    int   Y = s_CameraData.vp.Y;
    float fRHW = 1.0f / vOut.w;
    vMoon.z = vOut.z * fRHW;
    if (vMoon.z < 0.0f || vMoon.z > 1.0f) {
        return; // 화면 뒤에 그려진다.
    }
    // Mapping Screen Coordinate.
    vMoon.x = X + int((vOut.x * fRHW + 1.0f) * Width * 0.5f);
    vMoon.y = Y + int((-vOut.y * fRHW + 1.0f) * Height * 0.5f);

    RECT rcMoon;
    int  iMoonSize = (Width * m_fMoonRatio) / 2;
    SetRect(&rcMoon, vMoon.x - iMoonSize, vMoon.y - iMoonSize, vMoon.x + iMoonSize, vMoon.y + iMoonSize);
    // clipping with screen.
    if (rcMoon.right < X || rcMoon.bottom < Y || rcMoon.left > X + Width || rcMoon.top > Y + Height) {
        return; // 화면 밖에 그려진다.
    }

    // 2D로 그리기
    m_pVertices[0].x = rcMoon.left;
    m_pVertices[0].y = rcMoon.top;
    m_pVertices[1].x = rcMoon.right;
    m_pVertices[1].y = rcMoon.top;
    m_pVertices[2].x = rcMoon.right;
    m_pVertices[2].y = rcMoon.bottom;
    m_pVertices[3].x = rcMoon.left;
    m_pVertices[3].y = rcMoon.bottom;

    if (m_pTexture) {
        s_lpD3DDev->SetTexture(0, m_pTexture->Get());
    } else {
        s_lpD3DDev->SetTexture(0, NULL);
    }
    s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    s_lpD3DDev->SetFVF(FVF_TRANSFORMED);
    s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(m_pVertices[0]));
}

void CN3Moon::SetMoonPhase(int iIndex) {
    iIndex %= 24;
    m_iMoonPhaseIndex = iIndex;

    int                row = iIndex / 6;
    int                col = iIndex % 6;
    static const float fOffsetX = 1.0f / 6.0f;
    static const float fOffsetY = 1.0f / 4.0f;
    m_pVertices[0].tu = fOffsetX * col;
    m_pVertices[0].tv = fOffsetY * (row);
    m_pVertices[1].tu = fOffsetX * (col + 1);
    m_pVertices[1].tv = fOffsetY * (row);
    m_pVertices[2].tu = fOffsetX * (col + 1);
    m_pVertices[2].tv = fOffsetY * (row + 1);
    m_pVertices[3].tu = fOffsetX * col;
    m_pVertices[3].tv = fOffsetY * (row + 1);
}
