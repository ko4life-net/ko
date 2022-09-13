// UIStateBar.h: interface for the CUIStateBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

struct __PositionInfo {
    __Vector3 vPos;
    int       iID;
    D3DCOLOR  crType; // ������ ���� ����...
};

struct __DurationMagicImg {
    DWORD                   dwSkillID;
    class CN3UIDBCLButton * pIcon;
    float                   fDuration;
};

typedef typename std::list<__PositionInfo>::iterator       it_PositionInfo;
typedef typename std::list<__DurationMagicImg *>::iterator it_MagicImg;

class CUIStateBar : public CN3UIBase {
  protected:
    CN3UIString *   m_pText_Position;
    CN3UIString *   m_pText_HP;
    CN3UIString *   m_pText_MP;
    CN3UIString *   m_pText_Exp;
    CN3UIString *   m_pText_Fps;
    CN3UIString *   m_pText_SystemTime;
    CN3UIProgress * m_pProgress_HP;
    CN3UIProgress * m_pProgress_MSP;
    CN3UIProgress * m_pProgress_ExpC;
    CN3UIProgress * m_pProgress_ExpP;

    // �̴ϸ�...
    CN3UIBase *   m_pGroup_MiniMap;
    CN3UIImage *  m_pImage_Map; // �� �̹����� �̴ϸ� �ؽ�ó�� �����Ų��..
    CN3UIButton * m_pBtn_ZoomIn;
    CN3UIButton * m_pBtn_ZoomOut;

    float     m_fZoom; // ������ ����..
    float     m_fMapSizeX;
    float     m_fMapSizeZ;
    float     m_fYawPlayer;
    __Vector3 m_vPosPlayer;

    __VertexTransformedColor  m_vArrows[6]; // �÷��̾� ��ġ ȭ��ǥ..
    std::list<__PositionInfo> m_Positions;
    std::list<__PositionInfo> m_PositionsTop; // �� ���� �׸� ��ġ��..

    //�÷��ִ� ������ų ǥ���ϱ�..
    std::list<__DurationMagicImg *> m_pMagic;

    // Attributes
  public:
    bool  OnKeyPress(int iKey);
    DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    void  AddMagic(__TABLE_UPC_SKILL * pSkill, float fDuration);
    void  DelMagic(__TABLE_UPC_SKILL * pSkill);
    void  ClearMagic();
    void  TickMagicIcon();
    void  TickMiniMap();

    bool ToggleMiniMap();

    void UpdateExp(int iExp, int iExpNext, bool bUpdateImmediately);
    void UpdateMSP(int iMSP, int iMSPMax, bool bUpdateImmediately);
    void UpdateHP(int iHP, int iHPMax, bool bUpdateImmediately);

    void UpdatePosition(const __Vector3 & vPos, float fYaw);

    void UpdateFPS(const float fps);
    void UpdateSystemTime();

    void ZoomSet(float fZoom);
    void PositionInfoAdd(int iID, const __Vector3 & vPos, D3DCOLOR crID, bool bDrawTop);
    void PositionInfoClear();
    bool LoadMap(const std::string & szMiniMapFN, float fMapSizeX,
                 float fMapSizeZ); // �̴ϸ� ��Ʈ�� ���� �̸�, ���� �ʺ� ����..(Meter);

    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg); // �޽����� �޴´�.. ������, msg

    virtual void Tick();
    virtual void Render(); // �̴ϸ� ������..
    virtual bool Load(HANDLE hFile);

    virtual void Release();
    CUIStateBar();
    virtual ~CUIStateBar();
};
