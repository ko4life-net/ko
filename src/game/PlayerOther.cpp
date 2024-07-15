// PlayerOther.cpp: implementation of the CPlayerOther class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "GameProcedure.h"
#include "PlayerOther.h"
#include "PlayerMySelf.h"

#include "N3Base/DFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayerOther::CPlayerOther() {
    m_ePlayerType = PLAYER_OTHER; // Player Type ... Base, NPC, OTher, MySelf

    m_InfoExt.Init();
    m_bSit = false;
}

CPlayerOther::~CPlayerOther() {}

void CPlayerOther::Tick() {
    if (m_bSit) {
        CPlayerBase::Tick(); // ȸ��, ������ ���ϸ��̼� Tick �� ���� ���� ó��.. ���..
    } else {
        CPlayerNPC::Tick();
    }
}

bool CPlayerOther::Init(e_Race eRace, int iFace, int iHair, DWORD * pdwItemIDs, int * piItenDurabilities) {
    if (NULL == pdwItemIDs || NULL == piItenDurabilities) {
        return false;
    }

    m_InfoBase.eRace = eRace;
    m_InfoExt.iFace = iFace;
    m_InfoExt.iHair = iHair;

    // ���� ��Ŷ�� ���� ĳ���͸� ġ��..(?) �����ش�.. ����������, ���� ������...
    __TABLE_PLAYER_LOOKS * pLooks = s_pTbl_UPC_Looks->Find(eRace); // ���̺��� �⺻ ��Ų ..
    if (NULL == pLooks) {
        CLogWriter::Write("CPlayerOther::Init() Basic Resource Pointer is NULL Race(%d)", eRace);
        return false;
    }
    this->InitChr(pLooks); // ���� ����..

    for (int i = 0; i < MAX_ITEM_SLOT_OPC; i++) {
        std::string          szFN;
        e_PartPosition       ePart = PART_POS_UNKNOWN;
        e_PlugPosition       ePlug = PLUG_POS_UNKNOWN;
        e_ItemSlot           eSlot = ITEM_SLOT_UNKNOWN;
        __TABLE_ITEM_BASIC * pItem = NULL;
        __TABLE_ITEM_EXT *   pItemExt = NULL;

        if (0 == pdwItemIDs[i]) {
            if (0 == i) {
                ePart = PART_POS_UPPER;
                szFN = pLooks->szPartFNs[0];
            } else if (1 == i) {
                ePart = PART_POS_LOWER;
                szFN = pLooks->szPartFNs[1];
            }
            //            else if(2 == i) { ePart = PART_POS_HAIR_HELMET; szFN = pLooks->szPartFNs[5]; }
            else if (3 == i) {
                ePart = PART_POS_HANDS;
                szFN = pLooks->szPartFNs[3];
            } else if (4 == i) {
                ePart = PART_POS_FEET;
                szFN = pLooks->szPartFNs[4];
            } else if (5 == i) {
            } // ����
              //            else if(6 == i) { ePlug = PLUG_POS_RIGHTHAND; }
              //            else if(7 == i) { ePlug = PLUG_POS_LEFTHAND; }
        } else {
            pItem = s_pTbl_Items_Basic->Find(pdwItemIDs[i] / 1000 * 1000); // ���� �÷��̾� ������ ���..
            if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
                pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(pdwItemIDs[i] % 1000);
            }
            if (NULL == pItem || NULL == pItemExt) {
                __ASSERT(0, "NULL Item!!!");
                continue;
            }

            e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, &szFN, NULL, ePart, ePlug,
                                                                       m_InfoBase.eRace); // ���ҽ� ���� �̸��� �����..

            if (0 == i) {
                ePart = PART_POS_UPPER;
                eSlot = ITEM_SLOT_UPPER;
            } else if (1 == i) {
                ePart = PART_POS_LOWER;
                eSlot = ITEM_SLOT_LOWER;
            } else if (2 == i) {
                ePart = PART_POS_HAIR_HELMET;
                eSlot = ITEM_SLOT_HEAD;
            } else if (3 == i) {
                ePart = PART_POS_HANDS;
                eSlot = ITEM_SLOT_GLOVES;
            } else if (4 == i) {
                ePart = PART_POS_FEET;
                eSlot = ITEM_SLOT_SHOES;
            } else if (5 == i) {
            } // ����
            else if (6 == i) {
                ePlug = PLUG_POS_RIGHTHAND;
                eSlot = ITEM_SLOT_HAND_RIGHT;
            } else if (7 == i) {
                ePlug = PLUG_POS_LEFTHAND;
                eSlot = ITEM_SLOT_HAND_LEFT;
            }
        }

        if (PART_POS_UPPER == ePart || PART_POS_LOWER == ePart || PART_POS_HANDS == ePart || PART_POS_FEET == ePart) {
            this->PartSet(ePart, szFN, pItem, pItemExt);
        } else if (PART_POS_HAIR_HELMET == ePart) // �Ӹ�ī�� Ȥ�� ����̸�..
        {
            this->PartSet(ePart, szFN, pItem, pItemExt);
        } else if (5 == i) {
        } else if ((6 == i || 7 == i) && PLUG_POS_UNKNOWN != ePlug) {
            this->PlugSet(ePlug, szFN, pItem, pItemExt);
        }

        if (ITEM_SLOT_UNKNOWN != eSlot) {
            this->DurabilitySet(eSlot, piItenDurabilities[i]);
        }
    }

    // ���� ��������..
    this->InitFace();
    CN3CPart * pPartHairHelmet = this->Part(PART_POS_HAIR_HELMET);
    if (pPartHairHelmet->FileName().empty()) { // ��信 �ش�Ǵ°� ������.. �Ӹ�ī�� ���̱�..
        this->InitHair();
    }

    return true;
}

void CPlayerOther::InitFace() {
    __TABLE_PLAYER_LOOKS * pItem = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pItem && !pItem->szPartFNs[PART_POS_FACE].empty()) // �������� �ְ� �� �̸��� ������..
    {
        char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
        ::_splitpath(pItem->szPartFNs[PART_POS_FACE].c_str(), NULL, szDir, szFName, szExt);
        sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iFace, szExt);
        this->PartSet(PART_POS_FACE, szBuff, NULL, NULL);
    }
}

void CPlayerOther::InitHair() {
    __TABLE_PLAYER_LOOKS * pItem = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pItem && !pItem->szPartFNs[PART_POS_HAIR_HELMET].empty()) // �������� �ְ� �� �̸��� ������..
    {
        char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
        ::_splitpath(pItem->szPartFNs[PART_POS_HAIR_HELMET].c_str(), NULL, szDir, szFName, szExt);
        sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, m_InfoExt.iHair, szExt);
        this->PartSet(PART_POS_HAIR_HELMET, szBuff, NULL, NULL);
    } else {
        m_Chr.PartSet(PART_POS_HAIR_HELMET, "");
    }
}

void CPlayerOther::KnightsInfoSet(int iID, const std::string & szName, int iGrade, int iRank) {
    CPlayerBase::KnightsInfoSet(iID, szName, iGrade, iRank);

    m_InfoExt.iKnightsID = iID;
    m_InfoExt.szKnights = szName;
    m_InfoExt.iKnightsGrade = iGrade;
    m_InfoExt.iKnightsRank = iRank;

    if (m_InfoExt.szKnights.empty()) {
        if (m_pClanFont) {
            delete m_pClanFont;
        }
        m_pClanFont = NULL;
    }

    float fDist = Distance(CGameProcedure::s_pPlayer->Position());
    if (fDist < SOUND_RANGE_TO_SET) {
        if (!m_pClanFont) {
            std::string szFontID;
            ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
            m_pClanFont = new CDFont(szFontID, 12);
            m_pClanFont->InitDeviceObjects(s_lpD3DDev);
            m_pClanFont->RestoreDeviceObjects();
        }

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // ��Ʈ�� �ؽ�Ʈ ����.
        m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
    }
}

void CPlayerOther::SetSoundAndInitFont() {
    CPlayerBase::SetSoundAndInitFont();

    if (m_InfoExt.szKnights.empty()) {
        delete m_pClanFont;
        m_pClanFont = NULL;
    } else {
        if (!m_pClanFont) {
            std::string szFontID;
            ::_LoadStringFromResource(IDS_FONT_ID, szFontID);
            m_pClanFont = new CDFont(szFontID, 12); // �� �۰� �����..
            m_pClanFont->InitDeviceObjects(s_lpD3DDev);
            m_pClanFont->RestoreDeviceObjects();
        }

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // ��Ʈ�� �ؽ�Ʈ ����.
        m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
    }
}
