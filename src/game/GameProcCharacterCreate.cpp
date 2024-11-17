// GameProcCharacterCreate.cpp: implementation of the CGameProcCharacterCreate class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "GameProcCharacterCreate.h"

#include "Resource.h"
#include "GameEng.h"
#include "UICharacterCreate.h"
#include "UIMessageBox.h"
#include "UIMessageBoxManager.h"
#include "PlayerMySelf.h"
#include "UIManager.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "PacketDef.h"
#include "GameCursor.h"

#include "N3Base/N3UIString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameProcCharacterCreate::CGameProcCharacterCreate() {
    m_pUICharacterCreate = NULL;

    s_pPlayer->m_InfoBase.eRace = RACE_UNKNOWN;
    s_pPlayer->m_InfoBase.eClass = CLASS_UNKNOWN;
}

CGameProcCharacterCreate::~CGameProcCharacterCreate() {
    delete m_pUICharacterCreate;
}

void CGameProcCharacterCreate::Release() {
    CGameProcedure::Release();

    delete m_pUICharacterCreate;
    m_pUICharacterCreate = NULL;

    SetRect(&m_rcChr, 0, 0, 0, 0);
    m_Tbl_InitValue.Release();

    s_pPlayer->m_ChrInv.Release();
    s_pPlayer->m_ChrInv.PartAlloc(PART_POS_COUNT);
    s_pPlayer->m_ChrInv.PlugAlloc(PLUG_POS_COUNT);
}

void CGameProcCharacterCreate::Init() {
    CGameProcedure::Init();

    SetRect(&m_rcChr, 0, 0, 0, 0);

    m_Tbl_InitValue.LoadFromFile(fs::path("Data") / "NewChrValue.tbl");

    s_pPlayer->m_InfoBase.eRace = RACE_UNKNOWN;
    s_pPlayer->m_InfoBase.eClass = CLASS_UNKNOWN;
    s_pPlayer->m_ChrInv.Release();
    s_pPlayer->m_ChrInv.PartAlloc(PART_POS_COUNT);
    s_pPlayer->m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

    __TABLE_UI_RESRC * pTblUI = s_pTbl_UI->Find(s_pPlayer->m_InfoBase.eNation);
    m_pUICharacterCreate = new CUICharacterCreate();
    m_pUICharacterCreate->Init(s_pUIMgr);
    m_pUICharacterCreate->LoadFromFile(pTblUI->szCharacterCreate);
}

void CGameProcCharacterCreate::Render() {
    s_pEng->Clear(0);     // 클리어..
    s_pEng->BeginScene(); // 씬 렌더 ㅅ작...

    s_pUIMgr->Render();

    s_pPlayer->InventoryChrRender(m_rcChr); // 캐릭터 그리기..

    s_pMsgBoxMgr->Render(); //MessageBox를 그려준다.
    if (s_pGameCursor) {
        s_pGameCursor->Render();
    }

    s_pEng->EndScene(); // 씬 렌더 시작...
    s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcCharacterCreate::SetChr() {
    __InfoPlayerBase *   pInfoBase = &(s_pPlayer->m_InfoBase);
    __InfoPlayerMySelf * pInfoExt = &(s_pPlayer->m_InfoExt);
    __TABLE_NEW_CHR *    pTbl = m_Tbl_InitValue.Find(pInfoBase->eRace);

    if (pTbl) {
        pInfoExt->iStrength = pTbl->iStr;
        pInfoExt->iStamina = pTbl->iSta;
        pInfoExt->iDexterity = pTbl->iDex;
        pInfoExt->iIntelligence = pTbl->iInt;
        pInfoExt->iMagicAttak = pTbl->iMAP;
        m_pUICharacterCreate->m_iBonusPoint = pTbl->iBonus;
        m_pUICharacterCreate->m_iMaxBonusPoint = pTbl->iBonus;
    }

    __TABLE_PLAYER_LOOKS * pLooks =
        s_pTbl_UPC_Looks->Find(s_pPlayer->m_InfoBase.eRace); // User Player Character Skin 구조체 포인터..;
    if (NULL == pLooks) {
        return;
    }

    s_pPlayer->InitChr(pLooks);
    s_pPlayer->m_ChrInv.ScaleSet(1, 1, 1); // 스케일을 원래대로 돌린다.

    if (pLooks) // 파트 세팅..
    {
        for (int i = 0; i < PART_POS_COUNT; i++) {
            if (i == PART_POS_FACE) {
                s_pPlayer->InitFace();
                continue;
            }
            if (i == PART_POS_HAIR_HELMET) {
                s_pPlayer->InitHair();
                continue;
            }
            s_pPlayer->PartSet((e_PartPosition)i, pLooks->szPartFNs[i], NULL, NULL);
        }
    }

    m_pUICharacterCreate->Reset();
    m_pUICharacterCreate->UpdateRaceAndClassButtons(pInfoBase->eRace);
    m_pUICharacterCreate->UpdateClassButtons(pInfoBase->eClass);

    s_pPlayer->InventoryChrAnimationInitialize();
    s_pPlayer->Action(PSA_BASIC, true, NULL, true);
}

void CGameProcCharacterCreate::Tick() {
    //    s_pLocalInput->Tick(); // 키보드와 마우스로부터 입력을 받는다.
    //    if(dwMouseFlags & MOUSE_LBDOWN) SetCursor(s_hCursorClick);
    //    else SetCursor(s_hCursorNormal);

    CGameProcedure::Tick();

    DWORD dwMouseFlags = s_pLocalInput->MouseGetFlag();
    m_pUICharacterCreate->Tick();
    m_pUICharacterCreate->MouseProc(dwMouseFlags, s_pLocalInput->MouseGetPos(), s_pLocalInput->MouseGetPosOld());

    s_pEng->s_SndMgr.Tick(); // Sound Engine...

    // Network Msg 처리하기
    DataPack * pDataPack = NULL;
    while (s_pSocket->PktQueueSize() > 0) // 패킷 리스트에 패킷이 있냐????
    {
        int iOffset = 0;
        pDataPack = s_pSocket->PktQueueFront(); // 큐의 첫번째 것을 복사..
        if (false == ProcessPacket(pDataPack, iOffset)) {
            break; // 패킷을 처리할 상황이 아니다.
        }
        delete pDataPack;
        s_pSocket->PktQueuePop(); // 패킷을 큐에서 꺼냄..
    }

    s_pPlayer->InventoryChrTick();
}

bool CGameProcCharacterCreate::MsgSendCharacterCreate() {
    e_ErrorCharacterCreate eErrCode = ERROR_CHARACTER_CREATE_SUCCESS;
    const std::string &    szID = s_pPlayer->IDString();

    int iIDLength = szID.size();
    if (iIDLength <= 0) {
        eErrCode = ERROR_CHARACTER_CREATE_INVALID_NAME;
    } else if (RACE_UNKNOWN == s_pPlayer->m_InfoBase.eRace) {
        eErrCode = ERROR_CHARACTER_CREATE_INVALID_RACE;
    }
    //    else if(RACE_KA_WRINKLETUAREK == s_pPlayer->m_InfoBase.eRace) // 마법사는 선택 불가능..
    //    {
    //        eErrCode = ERROR_CHARACTER_CREATE_NOT_SUPPORTED_RACE;
    //    }
    else if (CLASS_UNKNOWN == s_pPlayer->m_InfoBase.eClass) {
        eErrCode = ERROR_CHARACTER_CREATE_INVALID_CLASS;
    } else if (m_pUICharacterCreate->m_iBonusPoint > 0) {
        eErrCode = ERROR_CHARACTER_CREATE_REMAIN_BONUS_POINT;
    } else {
        // 이름에 빈칸이나 특수문자가 들어 있는지 확인
        bool bHasSpecialLetter = false;
        for (int i = 0; i < iIDLength; i++) {
            // CompadmreString(LOCALE_USER_DEFAULT, NORM_IGNOREWIDTH, id, strlen(id), pUser->m_UserId, strlen(pUser->m_UserId) ) == CSTR_EQUAL )
            if ('~' == szID[i] || '`' == szID[i] || '!' == szID[i] || '@' == szID[i] || '#' == szID[i] ||
                '$' == szID[i] || '%' == szID[i] || '^' == szID[i] || '&' == szID[i] || '*' == szID[i] ||
                '(' == szID[i] || ')' == szID[i] ||
                //                '_' == szID[i] ||
                '-' == szID[i] || '+' == szID[i] || '=' == szID[i] || '|' == szID[i] || '\\' == szID[i] ||
                '<' == szID[i] || '>' == szID[i] || ',' == szID[i] || '.' == szID[i] || '?' == szID[i] ||
                '/' == szID[i] || '{' == szID[i] || '[' == szID[i] || '}' == szID[i] || ']' == szID[i] ||
                '\"' == szID[i] || '\'' == szID[i] || ' ' == szID[i]) {
                bHasSpecialLetter = true;
                eErrCode = ERROR_CHARACTER_CREATE_INVALID_NAME_HAS_SPECIAL_LETTER;
                break;
            }
        }

        if (false == bHasSpecialLetter) {
            __InfoPlayerBase *   pInfoBase = &(s_pPlayer->m_InfoBase);
            __InfoPlayerMySelf * pInfoExt = &(s_pPlayer->m_InfoExt);

            BYTE byBuff[64];
            int  iOffset = 0;
            CAPISocket::MP_AddByte(byBuff, iOffset, N3_NEW_CHARACTER);                  // 커멘드.
            CAPISocket::MP_AddByte(byBuff, iOffset, CGameProcedure::s_iChrSelectIndex); // 캐릭터 인덱스 b
            CAPISocket::MP_AddShort(byBuff, iOffset, iIDLength);                        // Id 길이 s
            CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());           // ID 문자열 str
            CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoBase.eRace);       // 종족 b
            CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->m_InfoBase.eClass);     // 직업 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iFace);                   // 얼굴모양 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iHair);                   // 머리모양 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iStrength);               // 힘 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iStamina);                // 지구력 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iDexterity);              // 민첩 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iIntelligence);           // 지능 b
            CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iMagicAttak);             // 마력 b

            s_pSocket->Send(byBuff, iOffset); // 보낸다

            s_pUIMgr->EnableOperationSet(false); // 패킷이 들어올때까지 UI 를 Disable 시킨다...

            return true;
        }
    }

    ReportErrorCharacterCreate(eErrCode); // 에러 보고...

    return false;
}

void CGameProcCharacterCreate::ReportErrorCharacterCreate(e_ErrorCharacterCreate eErrCode) {
    std::string szErr;

    if (ERROR_CHARACTER_CREATE_NO_MORE_CHARACTER == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_NO_MORE_CHARACTER, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_NATION_AND_INVALID_RACE == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_INVALID_NATION_RACE, szErr);
    } else if (ERROR_CHARACTER_CREATE_OVERLAPPED_ID == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_OVERLAPPED_ID, szErr);
    } else if (ERROR_CHARACTER_CREATE_DB_CREATE == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_DB_CREATE, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_NAME == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_INVALID_NAME, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_NAME_HAS_SPECIAL_LETTER == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_INVALID_NAME_HAS_SPECIAL_LETTER, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_RACE == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_INVALID_RACE, szErr);
    } else if (ERROR_CHARACTER_CREATE_NOT_SUPPORTED_RACE == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_NOT_SUPPORTED_RACE, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_CLASS == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_INVALID_CLASS, szErr);
    } else if (ERROR_CHARACTER_CREATE_REMAIN_BONUS_POINT == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_REMAIN_BONUS_POINT, szErr);
    } else if (ERROR_CHARACTER_CREATE_INVALID_STAT_POINT == eErrCode) {
        ::_LoadStringFromResource(IDS_ERR_UNKNOWN, szErr);
    } else {
        ::_LoadStringFromResource(IDS_ERR_UNKNOWN, szErr);
    }

    ///
    //    정보를 잘못 입력해서 실패했다는 메시지를 받을 때...
    std::string szTitle;
    ::_LoadStringFromResource(IDS_ERR_CHARACTER_CREATE, szTitle);
    MessageBoxPost(szErr, szTitle, MB_OK);
}

bool CGameProcCharacterCreate::ProcessPacket(DataPack * pDataPack, int & iOffset) {
    int iOffsetPrev = iOffset;
    if (false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) {
        iOffset = iOffsetPrev;
    } else {
        return true;
    }

    int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 커멘드 파싱..
    switch (iCmd)                                                      // 커멘드에 다라서 분기..
    {
    case N3_NEW_CHARACTER: // 캐릭터 선택 메시지..
    {
        BYTE bySuccess = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 커멘드 파싱..
        if (0 == bySuccess) {
            ProcActiveSet((CGameProcedure *)s_pProcCharacterSelect); // 캐릭터 선택창으로 가기..
        } else // 실패하면.. 이유가 0 이 아닌 값으로 온다..
        {
            this->ReportErrorCharacterCreate((e_ErrorCharacterCreate)bySuccess); // 에러 메시지 띄움..
            s_pUIMgr->EnableOperationSet(false); // UI 조작 가능하게 한다... 다시 캐릭터 만들어야 한다..
        }
        s_pUIMgr->EnableOperationSet(false); // 패킷이 들어올때까지 UI 를 Disable 시킨다...
    }
        return true;
    }

    return false;
}
