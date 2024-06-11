// DTexGroup.cpp: implementation of the CDTexGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "LyTerrainDef.h"
#include "MainFrm.h"
#include "DTexMng.h"
#include "DTex.h"

#include "DTexGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDTexGroup::CDTexGroup() {
    ZeroMemory(m_Name, 40);
    m_ID = 0;
}

CDTexGroup::~CDTexGroup() {
    Release();
}

//
//    Release..
//
void CDTexGroup::Release() {
    for (int i = DTEX_FULL; i < DTEX_MAX; i++) {
        it_DTexTileAttr it = m_Attributes[i].begin();
        int             iSize = m_Attributes[i].size();
        for (int j = 0; j < iSize; j++, it++) {
            DTEXTILEATTR * pTile = *it;
            delete pTile;
        }
        m_Attributes[i].clear();
    }
}

//
//    Init..
//
void CDTexGroup::Init() {
    Release();

    ZeroMemory(m_Name, 40);
    m_ID = 0;
}

//
//    SetAttr..
//
void CDTexGroup::SetAttr(int attr, DTEXTILEATTR tile) {
    if ((attr >= DTEX_FULL) && (attr < DTEX_MAX)) {
        LPDTEXTILEATTR pTile = new DTEXTILEATTR;
        pTile->TexID = tile.TexID;
        pTile->TileX = tile.TileX;
        pTile->TileY = tile.TileY;
        m_Attributes[attr].push_back(pTile);
    }
}

//
//    DelAttr..
//
void CDTexGroup::DelAttr(int attr, DTEXTILEATTR tile) {
    if ((attr >= DTEX_FULL) && (attr < DTEX_MAX)) {
        it_DTexTileAttr it = m_Attributes[attr].begin();
        int             iSize = m_Attributes[attr].size();
        for (int i = 0; i < iSize; i++, it++) {
            DTEXTILEATTR * pTile = *it;
            if ((pTile->TexID == tile.TexID) && (pTile->TileX == tile.TileX) && (pTile->TileY == tile.TileY)) {
                delete pTile;
                it = m_Attributes[attr].erase(it);
                break;
            }
        }
    }
}

//
//
//
void CDTexGroup::DelAttrByDTexID(int DTexID) {
    it_DTexTileAttr it;
    for (int i = DTEX_FULL; i < DTEX_MAX; i++) {
        //Go through the list and if something like DTexIndx comes up, get rid of it...
        it = m_Attributes[i].begin();
        while (it != m_Attributes[i].end()) {
            DTEXTILEATTR * pTile = *it;
            if (pTile->TexID == DTexID) {
                //Let's erase...
                delete (*it);
                it = m_Attributes[i].erase(it);
            } else {
                it++;
            }
        }
    }
}

//
// Clean the information in DTexMng...
// Set the current group information stored in DTex to NONE..
//
void CDTexGroup::ClearDTex() {
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    CDTexMng *   pDTexMng = pFrm->GetDTexMng();

    for (int i = DTEX_FULL; i < DTEX_MAX; i++) {
        it_DTexTileAttr it = m_Attributes[i].begin();
        int             iSize = m_Attributes[i].size();
        for (int j = 0; j < iSize; j++, it++) {
            DTEXTILEATTR * pTile = *it;
            CDTex *        pDTex = pDTexMng->GetDTexByID(pTile->TexID);

            if (pDTex) {
                pDTex->m_Attr[pTile->TileX][pTile->TileY].Group = 0;
                pDTex->m_Attr[pTile->TileX][pTile->TileY].Attr = pTile->TileX;
            }
        }
    }
}
