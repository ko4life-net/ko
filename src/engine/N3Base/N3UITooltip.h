// N3UITooltip.h: interface for the CN3UITooltip class.
// CN3UIBase�� EnableTooltip() �� DestroyTooltip()�Լ��� �Ʒ� �׸���� �������ݴϴ�.
// 1. tooltip�� �θ� null�� ���ְ� ���� ���� �ؾ� �մϴ�.
// 2. Save/Load�� ���� �����մϴ�.
//
// �Ʒ� �׸���� ���� �׸� �ܿ��� ó�����־�� �� �͵��Դϴ�.
// 1. Render�� ��� UI�� �� �׸��� Render�ؾ� �մϴ�.
// 2. MouseProc�Լ��� ȣ������� �մϴ�.
// 3. Tick�Լ��� ȣ������� �մϴ�.
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
    float m_fHoverTime; // ���콺�� �Ѱ����� �����Ͽ� �ִ� �ð�(����)
    bool  m_bSetText;   // �̹� text�� �����Ǿ��°�?
    POINT m_ptCursor;   // Ŀ���� ��ġ
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
