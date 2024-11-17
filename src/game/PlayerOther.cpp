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
        CPlayerBase::Tick(); // 회전, 지정된 에니메이션 Tick 및 색깔 지정 처리.. 등등..
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

    // 이제 패킷에 따라 캐릭터를 치장..(?) 시켜준다.. 아이템장착, 무기 장착등...
    __TABLE_PLAYER_LOOKS * pLooks = s_pTbl_UPC_Looks->Find(eRace); // 테이블에서 기본 스킨 ..
    if (NULL == pLooks) {
        CLogWriter::Write("CPlayerOther::Init() Basic Resource Pointer is NULL Race(%d)", eRace);
        return false;
    }
    this->InitChr(pLooks); // 관절 세팅..

    for (int i = 0; i < MAX_ITEM_SLOT_OPC; i++) {
        fs::path             fsItemFile;
        e_PartPosition       ePart = PART_POS_UNKNOWN;
        e_PlugPosition       ePlug = PLUG_POS_UNKNOWN;
        e_ItemSlot           eSlot = ITEM_SLOT_UNKNOWN;
        __TABLE_ITEM_BASIC * pItem = NULL;
        __TABLE_ITEM_EXT *   pItemExt = NULL;

        if (0 == pdwItemIDs[i]) {
            if (0 == i) {
                ePart = PART_POS_UPPER;
                fsItemFile = pLooks->szPartFNs[0];
            } else if (1 == i) {
                ePart = PART_POS_LOWER;
                fsItemFile = pLooks->szPartFNs[1];
                //} else if (2 == i) {
                //    ePart = PART_POS_HAIR_HELMET;
                //    fsItemFile = pLooks->szPartFNs[5];
            } else if (3 == i) {
                ePart = PART_POS_HANDS;
                fsItemFile = pLooks->szPartFNs[3];
            } else if (4 == i) {
                ePart = PART_POS_FEET;
                fsItemFile = pLooks->szPartFNs[4];
            } else if (5 == i) {
                //} else if (6 == i) {
                //    ePlug = PLUG_POS_RIGHTHAND;
                //} else if (7 == i) {
                //    ePlug = PLUG_POS_LEFTHAND;
            }
        } else {
            pItem = s_pTbl_Items_Basic->Find(pdwItemIDs[i] / 1000 * 1000); // 유저 플레이어 아이템 얻기..
            if (pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION) {
                pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(pdwItemIDs[i] % 1000);
            }
            if (NULL == pItem || NULL == pItemExt) {
                __ASSERT(0, "NULL Item!!!");
                continue;
            }

            e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, &fsItemFile, NULL, ePart,
                                                                       ePlug); // 리소스 파일 이름을 만들고..

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
            } // 망토
            else if (6 == i) {
                ePlug = PLUG_POS_RIGHTHAND;
                eSlot = ITEM_SLOT_HAND_RIGHT;
            } else if (7 == i) {
                ePlug = PLUG_POS_LEFTHAND;
                eSlot = ITEM_SLOT_HAND_LEFT;
            }
        }

        if (PART_POS_UPPER == ePart || PART_POS_LOWER == ePart || PART_POS_HANDS == ePart || PART_POS_FEET == ePart) {
            this->PartSet(ePart, fsItemFile, pItem, pItemExt);
        } else if (PART_POS_HAIR_HELMET == ePart) // 머리카락 혹은 헬멧이면..
        {
            this->PartSet(ePart, fsItemFile, pItem, pItemExt);
        } else if (5 == i) {
        } else if ((6 == i || 7 == i) && PLUG_POS_UNKNOWN != ePlug) {
            this->PlugSet(ePlug, fsItemFile, pItem, pItemExt);
        }

        if (ITEM_SLOT_UNKNOWN != eSlot) {
            this->DurabilitySet(eSlot, piItenDurabilities[i]);
        }
    }

    // 얼굴은 따로하자..
    this->InitFace();
    CN3CPart * pPartHairHelmet = this->Part(PART_POS_HAIR_HELMET);
    if (pPartHairHelmet->FilePath().empty()) { // 헬멧에 해당되는게 없으면.. 머리카락 붙이기..
        this->InitHair();
    }

    return true;
}

void CPlayerOther::InitFace() {
    __TABLE_PLAYER_LOOKS * pItem = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pItem && !pItem->szPartFNs[PART_POS_FACE].empty()) { // 아이템이 있고 얼굴 이름이 있으면..
        fs::path fsFile = pItem->szPartFNs[PART_POS_FACE];
        fsFile = fsFile.parent_path() / (fsFile.stem() + std::format("{:02d}", m_InfoExt.iFace) + fsFile.extension());
        this->PartSet(PART_POS_FACE, fsFile, NULL, NULL);
    }
}

void CPlayerOther::InitHair() {
    __TABLE_PLAYER_LOOKS * pItem = s_pTbl_UPC_Looks->Find(m_InfoBase.eRace);
    if (pItem && !pItem->szPartFNs[PART_POS_HAIR_HELMET].empty()) { // 아이템이 있고 얼굴 이름이 있으면..
        fs::path fsFile = pItem->szPartFNs[PART_POS_HAIR_HELMET];
        fsFile = fsFile.parent_path() / (fsFile.stem() + std::format("{:02d}", m_InfoExt.iHair) + fsFile.extension());
        this->PartSet(PART_POS_HAIR_HELMET, fsFile, NULL, NULL);
    } else {
        m_Chr.PartSet(PART_POS_HAIR_HELMET, fs::path());
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

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // 폰트에 텍스트 지정.
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
            m_pClanFont = new CDFont(szFontID, 12); // 좀 작게 만든다..
            m_pClanFont->InitDeviceObjects(s_lpD3DDev);
            m_pClanFont->RestoreDeviceObjects();
        }

        m_pClanFont->SetText(m_InfoExt.szKnights.c_str(), D3DFONT_BOLD); // 폰트에 텍스트 지정.
        m_pClanFont->SetFontColor(KNIGHTS_FONT_COLOR);
    }
}
