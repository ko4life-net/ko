// UICharacterCreate.h: interface for the CUICharacterCreate class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "N3Base/N3UIBase.h"

class CN3UIButton;
class CN3UIString;
class CN3UIImage;

const int MAX_RACE_SELECT = 4;
const int MAX_CLASS_SELECT = 4;
const int MAX_STATS = 5; // 힘, 체력, 민첩, 지능, 마력

class CUICharacterCreate : public CN3UIBase {
  public:
    int m_iBonusPoint;
    int m_iMaxBonusPoint;

    CN3UIButton * m_pBtn_Races[MAX_RACE_SELECT];
    CN3UIButton * m_pBtn_Classes[MAX_CLASS_SELECT];
    CN3UIImage *  m_pImg_Disable_Classes[MAX_CLASS_SELECT];
    CN3UIImage *  m_pImg_Stats[MAX_STATS];
    CN3UIString * m_pStr_Stats[MAX_STATS];
    CN3UIArea *   m_pArea_Stats[MAX_STATS];

    CN3UIString * m_pStr_Desc;
    CN3UIString * m_pStr_Bonus;

    CN3UIButton * m_pBtn_Face_Left;
    CN3UIButton * m_pBtn_Face_Right;
    CN3UIButton * m_pBtn_Hair_Left;
    CN3UIButton * m_pBtn_Hair_Right;

    CN3UIArea * m_pArea_Character;

    CN3UIEdit * m_pEdit_Name;

  public:
    void Reset();
    void UpdateRaceAndClassButtons(e_Race eRace);
    void UpdateClassButtons(e_Class eClass);

    DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    bool  Load(HANDLE hFile);
    void  Release();
    bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUICharacterCreate();
    virtual ~CUICharacterCreate();
};
