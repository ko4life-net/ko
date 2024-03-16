// N3FXPlug.cpp: implementation of the CN3FXPlug class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXPlug.h"
#include "N3FXBundle.h"
#include "N3Chr.h"

////////////////////////////////////////////////////////////////////////////////////
// CN3FXPlugPart
CN3FXPlugPart::CN3FXPlugPart()
    : m_vOffsetPos(0, 0, 0)
    , m_vOffsetDir(0, 0, 1) {
    m_dwType |= OBJ_FX_PLUG_PART;
    m_pFXB = NULL;
    m_nRefIndex = -1;
}

CN3FXPlugPart::~CN3FXPlugPart() {
    delete m_pFXB;
}

void CN3FXPlugPart::Release() {
    CN3BaseFileAccess::Release();

    delete m_pFXB;
    m_pFXB = NULL;
    m_nRefIndex = -1;
    m_vOffsetPos.Set(0, 0, 0);
    m_vOffsetDir.Set(0, 0, 1);
}

bool CN3FXPlugPart::Load(HANDLE hFile) {
    if (false == CN3BaseFileAccess::Load(hFile)) {
        return false;
    }
    __ASSERT(NULL == m_pFXB, "must null");
    DWORD dwNum;
    int   nStrLen;
    ReadFile(hFile, &nStrLen, sizeof(nStrLen), &dwNum, NULL);
    if (nStrLen > 0) {
        char szFN[_MAX_PATH];
        ReadFile(hFile, szFN, nStrLen, &dwNum, NULL);
        szFN[nStrLen] = NULL;

        m_pFXB = new CN3FXBundle();
        if (false == m_pFXB->LoadFromFile(szFN)) {
            delete m_pFXB;
            m_pFXB = NULL;
        } else {
            m_pFXB->Init();
            m_pFXB->Trigger();
        }
    }

    ReadFile(hFile, &m_nRefIndex, sizeof(m_nRefIndex), &dwNum, NULL);
    ReadFile(hFile, &m_vOffsetPos, sizeof(m_vOffsetPos), &dwNum, NULL);
    ReadFile(hFile, &m_vOffsetDir, sizeof(m_vOffsetDir), &dwNum, NULL);

    return true;
}

#ifdef _N3TOOL
bool CN3FXPlugPart::Save(HANDLE hFile) {
    if (false == CN3BaseFileAccess::Save(hFile)) {
        return false;
    }
    __ASSERT(m_pFXB, "no FXB");

    DWORD dwNum;
    int   nStrLen = m_pFXB->FileName().size();
    WriteFile(hFile, &nStrLen, sizeof(nStrLen), &dwNum, NULL);
    WriteFile(hFile, m_pFXB->FileName().c_str(), nStrLen, &dwNum, NULL);
    WriteFile(hFile, &m_nRefIndex, sizeof(m_nRefIndex), &dwNum, NULL);
    WriteFile(hFile, &m_vOffsetPos, sizeof(m_vOffsetPos), &dwNum, NULL);
    WriteFile(hFile, &m_vOffsetDir, sizeof(m_vOffsetDir), &dwNum, NULL);

    return true;
}
#endif

void CN3FXPlugPart::Tick(const __Matrix44 & mtxParent) {
    if (m_pFXB) {
        // location
        m_pFXB->m_vPos = m_vOffsetPos * mtxParent;

       // rotation
        static __Matrix44 mtxRot;
        mtxRot = mtxParent;
        mtxRot.PosSet(0, 0, 0);
        m_pFXB->m_vDir = m_vOffsetDir * mtxRot;

        m_pFXB->Tick();
    }
}

void CN3FXPlugPart::Tick(const CN3Chr * pChr) {
    __ASSERT(pChr, "no chr");
    if (m_pFXB) {
        // location
        const __Matrix44 * pMtxJoint = pChr->MatrixGet(m_nRefIndex);
        if (NULL == pMtxJoint) {
            return;
        }

        static __Matrix44 mtx;
        mtx = *(pMtxJoint);
        mtx *= pChr->m_Matrix;
        m_pFXB->m_vPos = m_vOffsetPos * mtx;

        // rotation
        mtx.PosSet(0, 0, 0);
        m_pFXB->m_vDir = m_vOffsetDir * mtx;

        m_pFXB->Tick();
    }
}

void CN3FXPlugPart::Render() {
    if (m_pFXB) {
        m_pFXB->Render();
    }
}

void CN3FXPlugPart::SetFXB(const std::string & strFN) {
    if (NULL == m_pFXB) {
        m_pFXB = new CN3FXBundle();
    } else {
        m_pFXB->Release();
    }
    m_pFXB->LoadFromFile(strFN);

    m_vOffsetPos = m_pFXB->m_vPos; //First, get the vPos and vDir values set in FXB and apply them.
    m_vOffsetDir = m_pFXB->m_vDir;

    m_pFXB->Init(); // Make FX appear
    m_pFXB->Trigger();
}

void CN3FXPlugPart::StopFXB(bool bImmediately) {
    if (m_pFXB) {
        m_pFXB->Stop(bImmediately);
    }
}

void CN3FXPlugPart::TriggerFXB() {
    if (m_pFXB) {
        m_pFXB->Trigger();
    }
}

////////////////////////////////////////////////////////////////////////////////////
// CN3FXPlug
CN3FXPlug::CN3FXPlug() {
    m_dwType |= OBJ_FX_PLUG;
}

CN3FXPlug::~CN3FXPlug() {
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        delete m_FXPParts[i];
    }
    m_FXPParts.clear();
}

void CN3FXPlug::Release() {
    CN3BaseFileAccess::Release();

    for (int i = 0; i < m_FXPParts.size(); ++i) {
        delete m_FXPParts[i];
    }
    m_FXPParts.clear();
}

bool CN3FXPlug::Load(HANDLE hFile) {
    if (false == CN3BaseFileAccess::Load(hFile)) {
        return false;
    }
    __ASSERT(0 == m_FXPParts.size(), "must 0");
    DWORD dwNum;
    int   nCount;
    ReadFile(hFile, &nCount, sizeof(nCount), &dwNum, NULL); // Number of Parts
    if (nCount > 0) {
        m_FXPParts.assign(nCount, NULL);
    }
    for (int i = 0; i < nCount; ++i) {
        m_FXPParts[i] = new CN3FXPlugPart();
        m_FXPParts[i]->Load(hFile);
    }
    return true;
}

void CN3FXPlug::Tick(const CN3Chr * pChr) {
    if (NULL == pChr) {
        return;
    }
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        m_FXPParts[i]->Tick(pChr);
    }
}

void CN3FXPlug::Render() {
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        m_FXPParts[i]->Render();
    }
}

void CN3FXPlug::StopAll(bool bImmediately) {
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        m_FXPParts[i]->StopFXB(bImmediately);
    }
}

void CN3FXPlug::TriggerAll() {
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        m_FXPParts[i]->TriggerFXB();
    }
}

#ifdef _N3TOOL
bool CN3FXPlug::Save(HANDLE hFile) {
    if (false == CN3BaseFileAccess::Save(hFile)) {
        return false;
    }

    RemoveFXPParts_HaveNoBundle(); // Delete unbundled parts

    DWORD dwNum;
    int   nCount = m_FXPParts.size();
    WriteFile(hFile, &nCount, sizeof(nCount), &dwNum, NULL); // Number of Parts
    for (int i = 0; i < nCount; ++i) {
        m_FXPParts[i]->Save(hFile);
    }

    return true;
}

void CN3FXPlug::RemoveFXPParts_HaveNoBundle() // Remove unbundled parts
{
    for (int i = 0; i < m_FXPParts.size(); ++i) {
        if (m_FXPParts[i] && NULL == m_FXPParts[i]->GetFXB()) {
            delete m_FXPParts[i]; // If there is no FXB, delete this part.
            m_FXPParts[i] = NULL;
        }
    }

    // Delete pointers that are NULL.
    std::vector<CN3FXPlugPart *>::iterator itor;
    for (itor = m_FXPParts.begin(); itor != m_FXPParts.end();) {
        if (NULL == (*itor)) {
            itor = m_FXPParts.erase(itor);
        } else {
            ++itor;
        }
    }
}

CN3FXPlugPart * CN3FXPlug::FXPPartAdd() {
    CN3FXPlugPart * pPart = new CN3FXPlugPart();
    m_FXPParts.push_back(pPart);
    return pPart;
}

void CN3FXPlug::FXPPartDelete(int nIndex) {
    if (nIndex < 0 || nIndex > m_FXPParts.size()) {
        return;
    }

    std::vector<CN3FXPlugPart *>::iterator itor = m_FXPParts.begin();
    for (int i = 0; i < nIndex; ++i) {
        ++itor;
    }
    delete (*itor);
    m_FXPParts.erase(itor);
}

#endif
