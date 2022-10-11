// PvsBase.cpp: implementation of the CPvsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "n3indoor.h"

#include "PvsObjFactory.h"
#include "OrganizeView.h"

#include "PvsBase.h"
#include "PortalWall.h"
#include "PortalVol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPvsBase, CObject)

CPvsBase::CPvsBase() {
    m_eState = STATE_NONE;
    m_MtxMove.Identity();
    m_MtxScale.Identity();
}

CPvsBase::~CPvsBase() {}

void CPvsBase::Load(FILE * stream) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fread(&m_MtxMove.m[i][j], sizeof(float), 1, stream);
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fread(&m_MtxScale.m[i][j], sizeof(float), 1, stream);
        }
    }
}

void CPvsBase::Save(FILE * stream) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fwrite(&m_MtxMove.m[i][j], sizeof(float), 1, stream);
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fwrite(&m_MtxScale.m[i][j], sizeof(float), 1, stream);
        }
    }
}
