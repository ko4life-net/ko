// N3Star.h: interface for the CN3Star class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base.h"

const int MAX_STAR = 200;

class CN3Star : public CN3Base {
  public:
    CN3Star();
    virtual ~CN3Star();

    // Attributes
  public:
    int GetNumStar() const { return m_iCurNumStar; }

  protected:
    int              m_iCurNumStar;  // 현재 떠있는 별의 수
    int              m_iNextNumStar; // 앞으로 보일 별 숫자
    float            m_fAddTime;     // 별 하나가 추가되거나 없어지는 시간 간격
    BOOL             m_bIncrease;    // 별이 증가할 것인가 감소할 것인가
    __VertexXyzColor m_Stars[MAX_STAR];

    // Operations
  public:
    void         Init();
    virtual void Release();
    virtual void Tick();
    virtual void Render();

    void SetStar(int iNum, float fSec);
};
