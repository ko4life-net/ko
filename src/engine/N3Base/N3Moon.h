// N3Moon.h: interface for the CN3Moon class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base.h"
#include <string>

class CN3Texture;
class CN3Moon : public CN3Base {
#ifdef _N3TOOL
    friend class CN3SkyMng;
#endif

  public:
    CN3Moon();
    virtual ~CN3Moon();
    // Attributes
  public:
    int GetMoonPhaseIndex() const { return m_iMoonPhaseIndex; }

  protected:
    __VertexTransformed m_pVertices[4];
    CN3Texture *        m_pTexture;
    float               m_fCurRadian;      // 현재 달의 회전위치
    float               m_fMoonRatio;      // 달의 pixel싸이즈
    int                 m_iMoonPhaseIndex; // 달의 모양 인덱스 0~23;

    // Operations
  public:
    void Init(const fs::path & fsTexFile);
    void SetMoonPhase(int iIndex);                                          // 달의 모양 설정
    void SetCurAngle(float fAngle) { m_fCurRadian = D3DXToRadian(fAngle); } // 현재 각도설정

    void Render(__Matrix44 & matView, __Matrix44 & matProj);
    void Release();
};
