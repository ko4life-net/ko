// NPCPath.h: interface for the CNPCPath class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"
#include <list>

class CLyTerrain;

class CNPCPath : public CN3Base {
  public:
    char m_strPathName[256]; //Length name..
    char m_strNPCName[80];   // NPC name.
    int  m_iNPCID;           // NPC Id ? Type?
    int  m_iNumNPC;          // How many?..

    __Vector3 m_LTStartVertex; // Two corners of the starting area...
    __Vector3 m_RBStartVertex;

    //CRect    m_rcStartRect;        // Area where NPCs appear.
    int m_iRegenTime; // The time it takes for an NPC to die and come back to life.
    int m_iActType;
    int m_iZoneID;

    std::list<__Vector3> m_Path; //Path designation points...start point~end point...

    CLyTerrain * m_pRefTerrain;

    DWORD m_dwColor;
    int   m_iVersion;

    __Vector3 m_LTActVertex; // Two corners of the starting area...
    __Vector3 m_RBActVertex;

    unsigned char m_cAttr_Create;
    unsigned char m_cAttr_Regen;
    unsigned char m_cAttr_Group;
    unsigned char m_cAttr_Option;

  public:
    bool CheckValid();
    void TransPos(float x, float z);
    void Load(HANDLE hFile);
    void Save(HANDLE hFile);
    int  GetSize() { return m_Path.size(); }
    bool GetPath(int idx, __Vector3 * pPos); // Get the idxth point...
    void AddPos(__Vector3 Path);             // Add path.
    void DelPrevPos();
    CNPCPath();
    virtual ~CNPCPath();
};
