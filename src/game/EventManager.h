// EventManager.h: interface for the CEventManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CEventCell {
  public:
    short m_sEventType;
    RECT  m_Rect;

    void Load(HANDLE hFile);
    CEventCell();
    virtual ~CEventCell();
};

typedef std::list<CEventCell *> EventList;
typedef EventList::iterator     EventItor;

class CEventManager {
  private:
    EventList m_lstEvents;
    short     m_sEventType;
    RECT      m_rcEvent;

  public:
    void  Behavior(short sEventType, short sPreEventType);
    bool  PtInRect(int x, int z, RECT rc);
    short SetPos(float fX, float fZ);
    void  Release();
    bool  LoadFromFile(const fs::path & fsFile);
    CEventManager();
    virtual ~CEventManager();
};
