// N3UITooltip.h: interface for the CN3UITooltip class.
// CN3UIBase에 EnableTooltip() 과 DestroyTooltip()함수가 아래 항목들을 관리해줍니다.
// 1. tooltip은 부모를 null로 해주고 따로 관리 해야 합니다.
// 2. Save/Load도 따로 관리합니다.
//
// 아래 항목들은 위의 항목 외에도 처리해주어야 할 것들입니다.
// 1. Render는 모든 UI를 다 그린후 Render해야 합니다.
// 2. MouseProc함수를 호출해줘야 합니다.
// 3. Tick함수도 호출해줘야 합니다.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3UIStatic.h"

class CN3UITooltip : public CN3UIStatic {
  public:
    CN3UITooltip();
    virtual ~CN3UITooltip();

    // Attributes
  public:
  protected:
    float    m_fHoverTime; // 마우스가 한곳에서 정지하여 있는 시간(누적)
    bool     m_bSetText;   // 이미 text가 설정되었는가?
    POINT    m_ptCursor;   // 커서의 위치
    D3DCOLOR m_crToolTipColor;

    // Operations
  public:
    void          SetText(const std::string & szText);
    void          SetSingleLineText(const std::string & szText);
    void          SetColor(const D3DCOLOR & crToolTipColor) { m_crToolTipColor = crToolTipColor; };
    virtual void  Release();
    virtual void  Tick();
    virtual void  Render();
    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);

  protected:
};
