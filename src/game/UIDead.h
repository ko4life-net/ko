// UIDead.h: interface for the CUIDead class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GameDef.h"
#include "N3Base/N3UIBase.h"
#include "UIMessageBox.h"

class CUIDead : public CN3UIBase {
  protected:
    class CN3UIString * m_pTextAlive;
    class CN3UIString * m_pTextTown;
    CUIMessageBox       m_MsgBox;
    bool                m_bProcessing;

  public:
    void  SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
    void  SetVisible(bool bVisible);
    void  MsgRecv_Revival(class DataPack * pDataPack, int & iOffset);
    void  MsgSend_Revival(BYTE byType);
    void  CallBackProc(int iID, DWORD dwFlag);
    DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    bool  ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);
    bool  Load(HANDLE hFile);
    CUIDead();
    virtual ~CUIDead();
};
