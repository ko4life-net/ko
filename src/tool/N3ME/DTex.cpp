// DTex.cpp: implementation of the CDTex class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3me.h"
#include "LyTerrainDef.h"
#include "N3Base/N3Texture.h"

#include "DTex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDTex::CDTex() {
    m_pTex = NULL;
    m_ID = -1;

    ZeroMemory(m_Attr, NUM_DTEXTILE * NUM_DTEXTILE * sizeof(DTEXATTR));
}

CDTex::~CDTex() {
    Release();
}

//
//    Release..
//
void CDTex::Release() {
    if (m_pTex) {
        m_pTex->Release();
        delete m_pTex;
        m_pTex = NULL;
    }
}

//
//    Init..
//
void CDTex::Init() {
    Release();

    if (!m_pTex) {
        m_pTex = new CN3Texture;
    }
    m_ID = -1;

    int x, z;
    for (x = 0; x < NUM_DTEXTILE; x++) {
        for (z = 0; z < NUM_DTEXTILE; z++) {
            m_Attr[x][z].Group = 0;
            m_Attr[x][z].Attr = x;
        }
    }
}

//
//    SetAttr...
//    Inserting properties into one tile...
//
void CDTex::SetAttr(int x, int y, DTEXATTR attr) {
    m_Attr[x][y] = attr;
}
