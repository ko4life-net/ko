// ItemBundleMgr.h: interface for the CItemBundleMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMBUNDLEMGR_H__943C585B_1D84_4B91_A578_AA1B228B2809__INCLUDED_)
#define AFX_ITEMBUNDLEMGR_H__943C585B_1D84_4B91_A578_AA1B228B2809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\engine\N3Base\N3Shape.h"
#include "GameBase.h"

class DataPack;
class CItemBundle : public CN3Shape {
  public:
    CItemBundle(int iBundleID, float fTimeToLive);
    virtual ~CItemBundle();

  protected:
    int   m_iBundleID;
    float m_fTimeToLive; // ¾ÆÀÌÅÛÀÌ ¸¸µé¾îÁø ½Ã°£..
  public:
    virtual void Tick();
    bool         IsAlive() const { return m_fTimeToLive > 0.0f; }
    int          GetItemID() const { return m_iBundleID; }
};

typedef std::map<int, CItemBundle *>::iterator it_ItemBundle;

class CItemBundleMgr : public CGameBase {
  public:
    CItemBundleMgr();
    virtual ~CItemBundleMgr();

  protected:
    std::map<int, CItemBundle *> m_Bundles;

  public:
    CItemBundle * Pick(int ixScreen, int iyScreen);
    bool          Add(__Vector3 vPos, int iBundleID, float fTimeToLive);
    bool          Add(CItemBundle * pIB) {
        if (!pIB) {
            return false;
        }
        m_Bundles.insert(std::map<int, CItemBundle *>::value_type(pIB->GetItemID(), pIB));
        return true;
    }
    bool Remove(int iBundleID) {
        it_ItemBundle it = m_Bundles.find(iBundleID);
        if (m_Bundles.end() == it) {
            return false;
        }
        delete (*it).second;
        m_Bundles.erase(it);
        return true;
    }

    CItemBundle * Get(int iBundleID) {
        it_ItemBundle it = m_Bundles.find(iBundleID);
        if (m_Bundles.end() == it) {
            return NULL;
        }
        return (*it).second;
    }

    void Tick();
    void Render();

    void Release();
    void ReleaseBundles();
};

#endif // !defined(AFX_ITEMBUNDLEMGR_H__943C585B_1D84_4B91_A578_AA1B228B2809__INCLUDED_)
