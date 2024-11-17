// N3Sun.h: interface for the CN3Sun class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base.h"
#include "N3ColorChange.h"

enum eSUNPART {
    SUNPART_SUN = 0,
    SUNPART_GLOW,
    SUNPART_FLARE,
    NUM_SUNPART
};

class CN3Texture;
class CN3Sun : public CN3Base {
    friend class CN3SkyMng;

  public:
    CN3Sun();
    virtual ~CN3Sun();

  protected:
    struct __SunPart {
        CN3Texture *        pTex;         // texture
        __VertexTransformed pVertices[4]; // vertex

        CN3ColorChange Color; // 색을 담당하는 클래스
        CN3DeltaChange Delta;
    };

    __SunPart m_Parts[NUM_SUNPART];
    float     m_fCurRadian; // 현재 해의 회전위치

    // Operations
  public:
    void SetCurAngle(float fAngle) { m_fCurRadian = D3DXToRadian(fAngle); } // 현재 각도설정
    void Init(const fs::path * pfsTexFiles);
    //    By : Ecli666 ( On 2002-04-04 오전 10:55:52 )
    //
    float GetCurAngle() { return D3DXToDegree(m_fCurRadian); }
    //    ~(By Ecli666 On 2002-04-04 오전 10:55:52 )

    void Release();
    void Render(__Matrix44 & matView, __Matrix44 & matProj);
    void Tick();

  protected:
};
