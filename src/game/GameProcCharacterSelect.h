// GameProcCharacterSelect.h: interface for the CGameProcCharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameProcedure.h"

enum e_ChrPos {
    POS_CENTER = 1,
    POS_LEFT,
    POS_RIGHT
};
enum e_DoProcProcess {
    PROCESS_ROTATEING = 1,
    PROCESS_PRESELECT,
    PROCESS_SELECTED,
    PROCESS_COMPLETE
};
enum e_ChrClass {
    CLASS_WAR = 1,
    CLASS_ROG,
    CLASS_PRIST,
    CLASS_WIZARD
};
const int CS_El_rf_wa = 65500;

const int SELECT_ANIM_PRE_SELECT = 0;
const int SELECT_ANIM_SELECTED = 1;
const int SELECT_ANIM_DUMMY = 2;

const float KARUS_THETA_MAX = 0.5f;
const float ELMORAD_THERA_MAX = 0.38f;
const float KARUS_INCRESE_OFFSET = 0.02f;
const float ELMORAD_INCRESE_OFFSET = 0.015f;

class CUIManager;
class CUICharacterSelect;

struct __CharacterSelectInfo {
    std::string szID;        // ĳ���� ���̵� ���ڿ� str
    e_Race      eRace;       // ���� b
    e_Class     eClass;      // ���� b
    int         iLevel;      // ���� b
    int         iFace;       // �󱼸�� b
    int         iHair;       // �Ӹ���� b
    int         iZone;       //zone number
    DWORD       dwItemUpper; // ��ü dw
    int         iItemUpperDurability;
    DWORD       dwItemLower; // ��ü dw
    int         iItemLowerDurability;
    DWORD       dwItemHelmet; // ���� dw
    int         iItemHelmetDurability;
    DWORD       dwItemCloak; // ���(����) dw
    int         iItemCloakDurability;
    DWORD       dwItemGloves; // �尩 dw
    int         iItemGlovesDurability;
    DWORD       dwItemShoes; // �Ź� dw
    int         iItemShoesDurability;

    void clear() {
        szID = "";
        eRace = RACE_UNKNOWN;   // ���� b
        eClass = CLASS_UNKNOWN; // ���� b
        iLevel = 0;             // ���� b
        iFace = 0;              // �󱼸�� b
        iHair = 0;              // �Ӹ���� b
        iZone = 0;              //zone number
        dwItemUpper = 0;        // ��ü dw
        dwItemLower = 0;        // ��ü dw
        dwItemHelmet = 0;       // ���� dw
        dwItemCloak = 0;        // ���(����) dw
        dwItemGloves = 0;       // �尩 dw
        dwItemShoes = 0;        // �Ź� dw
    }

    __CharacterSelectInfo() { clear(); }
    ~__CharacterSelectInfo() {}
};

class CGameProcCharacterSelect : public CGameProcedure {
    CN3SndObj * m_pSnd_Rotate;

  public:
    class CN3Shape * m_pActiveBg;

    class CN3Chr *        m_pChrs[MAX_AVAILABLE_CHARACTER];
    __CharacterSelectInfo m_InfoChrs[MAX_AVAILABLE_CHARACTER]; // �̹� ������� ĳ���� ����..

    class CN3Camera * m_pCamera;
    class CN3Light *  m_pLights[8];
    __Vector3         m_vEye;
    __Vector3         m_vEyeBackup;
    __Vector3         m_vAt;
    __Vector3         m_vUp;
    D3DLIGHT9         m_lgt[3];

    CUICharacterSelect * m_pUICharacterSelect;

    e_ChrPos m_eCurPos;
    e_ChrPos m_eDestPos;

    e_DoProcProcess m_eCurProcess;
    float           m_fCurTheta;
    float           m_fFadeOut;
    bool            m_bFadeOutRender;

    bool m_bReceivedCharacterSelect;

  public:
    void CharacterSelectOrCreate();
    void MsgSend_RequestAllCharacterInfo();
    void MsgSend_DeleteChr(const std::string & szKey);
    void MsgSend_CharacterSelect(); // virtual

    int  MsgRecv_VersionCheck(DataPack * pDataPack, int & iOffset); // virtual
    int  MsgRecv_GameServerLogIn(DataPack * pDataPack, int & iOffset) override;
    bool MsgRecv_CharacterSelect(DataPack * pDataPack, int & iOffset); // virtual
    void MsgRecv_AllCharacterInfo(class DataPack * pBuf, int & iOffset);
    void MsgRecv_DeleteChr(DataPack * pBuf, int & iOffset);

    void Release();
    void Init();
    void Tick();
    void Render();

    CGameProcCharacterSelect();
    virtual ~CGameProcCharacterSelect();

    void RotateLeft();
    void RotateRight();

    void AddChr(e_ChrPos eCP, __CharacterSelectInfo * pInfo);
    void AddChrPart(int iPosIndex, const __TABLE_PLAYER_LOOKS * pItemBasic, e_PartPosition ePartPos, DWORD dwItemID,
                    int iItemDurability);

    void DoJobLeft();
    void DojobRight();
    void CheckJobState();
    bool CheckRotateLeft();
    bool CheckRotateCenterToRight();
    bool CheckRotateCenterToLeft();
    bool CheckRotateRight();

    void CharacterSelect();
    void CharacterSelectFailed();

    void DoSelectedChrProc();
    void DoProcPreselect();
    void IncreseLightFactor();
    void DecreseLightFactor();
    void ProcessOnReturn();
    void FadeOutProcess();
    void FadeOutRender();

  protected:
    bool ProcessPacket(DataPack * pDataPack, int & iOffset);
};
