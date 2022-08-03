// GameProcNationSelect.cpp: implementation of the CGameProcNationSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "PacketDef.h"
#include "GameEng.h"
#include "APISocket.h"

#include "GameProcNationSelect.h"
#include "PlayerMySelf.h"
#include "UINationSelectDlg.h"
#include "UIManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcNationSelect::CGameProcNationSelect()
{
	m_pUINationSelectDlg = NULL;
	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // ���� ������ �������� �ʾҴ�..
}

CGameProcNationSelect::~CGameProcNationSelect()
{
	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

void CGameProcNationSelect::Release()
{
	CGameProcedure::Release();

	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

void CGameProcNationSelect::Init()
{
	CGameProcedure::Init();

	std::string szTemp = "UI\\Co_NationSelect.uif";
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(NATION_ELMORAD);
	if(pTbl)
	{
		szTemp = pTbl->szNationSelect;
	}

	m_pUINationSelectDlg = new CUINationSelectDlg();
	m_pUINationSelectDlg->Init(s_pUIMgr);
	m_pUINationSelectDlg->LoadFromFile(szTemp);
	m_pUINationSelectDlg->m_pProcNationSelectRef = this; // ���� ������ �ֱ�..

	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // ���� ������ �������� �ʾҴ�..
}

void CGameProcNationSelect::Tick()
{
	CGameProcedure::Tick();	// Ű, ���콺 �Է� ���..

	if(NATION_KARUS == s_pPlayer->m_InfoBase.eNation || NATION_ELMORAD == s_pPlayer->m_InfoBase.eNation)
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect); // ������ ������� ĳ���� �������� �ٷ� ����..
}

void CGameProcNationSelect::Render()
{
	DWORD color = 0x00000000; // ����������..
	s_pEng->Clear(color); // Ŭ����..
	s_pEng->s_lpD3DDev->BeginScene();			// �� ���� ����...

	CGameProcedure::Render(); // UI �� �׹��� �⺻���� �͵� ������..

	s_pEng->s_lpD3DDev->EndScene();			// �� ���� ����...
	s_pEng->Present(CN3Base::s_hWndBase);
}


void CGameProcNationSelect::MsgSendNationSelect(e_Nation eNation)
{
	BYTE byBuff[4];										// ��Ŷ ����..
	int iOffset=0;										// ������ ������..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_NATION_SELECT);	// Ŀ���.
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)eNation);		// ���̵� ����..
		
	s_pSocket->Send(byBuff, iOffset);								// ������

	s_pUIMgr->EnableOperationSet(false); // ���� ��Ŷ�� �ޱ� ������ �ƹ��� ���ϰ� �Ѵ�..
}

bool CGameProcNationSelect::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	N3_INFO("[CGameProcNationSelect::ProcessPacket] Opcode: [0x{:02X}:{}]", *(BYTE*)(pDataPack->m_pData + iOffset), PacketToString[*(BYTE*)(pDataPack->m_pData + iOffset)]);
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// Ŀ��� �Ľ�..
	switch ( iCmd )										// Ŀ��忡 �ٶ� �б�..
	{
		case N3_NATION_SELECT:							// ĳ���� ���� �޽���..
		{
			int iNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // ���� - 0 ����.. 1 - ī�罺 2 - ������..

			if( 0 == iNation ) 	s_pPlayer->m_InfoBase.eNation = NATION_NOTSELECTED; // ���� ������ �������� �ʾҴ�..
			else if( 1 == iNation ) s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
			else if( 2 == iNation ) s_pPlayer->m_InfoBase.eNation = NATION_ELMORAD;
		}
		return true;
	}
	
	return false;
}