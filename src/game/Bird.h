// Bird.h: interface for the CBird class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"

class CN3Shape;
class CN3SndObj;

class CBird : public CN3Base {
  public:
    CBird();
    virtual ~CBird();

    // Attributes
  public:
  protected:
    // 파일에서 읽어오는 정보
    CN3Shape * m_pShape;  // Shape pointer
    __Vector3  m_vPivot;  // 중심점(중심점에서 반경 m_fRadius만큼 범위내에서 움직인다.
    float      m_fRadius; // 반경

    //    __Vector3    m_vVelocity;    // 현재 속도(스피드+방향)
    float m_fRadianSpeed; // 속력
    float m_fRadian;      // 현재 타원의 위치각
    float m_fFactor1, m_fFactor2;
    float m_fFactorSpeed1, m_fFactorSpeed2;

    float m_fRadiusY;                         // 높이 반경
    float m_fFactorY1, m_fFactorY2;           // 높이 변화 요소
    float m_fFactorYSpeed1, m_fFactorYSpeed2; // 높이 변화 요소 변화율..

    CN3SndObj * m_pSnd;
    float       m_fSndInterval;

    // Operations
  public:
    virtual void Release();
    void         Tick();
    void         Render();

    int LoadBird(const fs::path & fsFile);

  protected:
};
