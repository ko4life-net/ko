// EventMgr.h: interface for the CEventMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include <list>

class CMapMng;
class CEventCell;
class CDlgEditEvent;

class CEventMgr : public CN3Base {
  public:
    std::list<CEventCell *> m_pEvents;
    CEventCell *            m_pCurrEvent;
    int                     m_MapSize;
    short **                m_ppEvent;
    CMapMng *               m_pRefMapMng;
    bool                    m_bActive; // Is this function activated? 1: Enabled, 0: Disabled..
    CDlgEditEvent *         m_pDlgEventList;

  public:
    bool MakeGameFile(HANDLE hFile, int iSize);
    bool MakeGameFile(char * szEventName, int iSize);
    void LoadFromFile(const char * RealFileName);
    void SaveToFile(const char * RealFileName);
    void SaveInfoTextFile(char * szEvent);
    void MakeEventArray();
    void DelEvent(CEventCell * pEvent);
    void SetCurrEvent(CEventCell * pEvent);
    void UpdateEvent();
    BOOL MouseMsgFilter(LPMSG pMsg);
    void SetActive(bool active);
    void Render();

    virtual bool Load(HANDLE hFile);
    virtual bool Save(HANDLE hFile);

    CEventMgr();
    virtual ~CEventMgr();
};
