// EventMgr.cpp: implementation of the CEventMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3ME.h"
#include "EventMgr.h"
#include "MapMng.h"
#include "EventCell.h"
#include "LyTerrain.h"
#include "DlgEditEvent.h"
#include "DlgEditEventAttr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventMgr::CEventMgr() {
    m_MapSize = 0;
    m_ppEvent = NULL;
    m_pRefMapMng = NULL;
    m_bActive = false;
    m_pEvents.clear();
    m_pCurrEvent = NULL;

    m_pDlgEventList = new CDlgEditEvent;
    m_pDlgEventList->Create(IDD_EDIT_EVENT);
    m_pDlgEventList->ShowWindow(FALSE);
    m_pDlgEventList->m_pRefEventMgr = this;
}

CEventMgr::~CEventMgr() {
    if (m_ppEvent) {
        for (int i = 0; i < m_MapSize; i++) {
            if (m_ppEvent[i]) {
                GlobalFree(m_ppEvent[i]);
                //delete[] m_ppEvent[i];
                m_ppEvent[i] = NULL;
            }
        }
        GlobalFree(m_ppEvent);
        //delete[] m_ppEvent;
        m_ppEvent = NULL;
    }

    std::list<CEventCell *>::iterator it;
    for (it = m_pEvents.begin(); it != m_pEvents.end(); it++) {
        delete (*it);
        (*it) = NULL;
    }
    m_pEvents.clear();

    if (m_pCurrEvent) {
        delete m_pCurrEvent;
        m_pCurrEvent = NULL;
    }

    if (m_pDlgEventList) {
        m_pDlgEventList->DestroyWindow();
        delete m_pDlgEventList;
        m_pDlgEventList = NULL;
    }

    m_MapSize = 0;
    m_pRefMapMng = NULL;
    m_bActive = false;
}

BOOL CEventMgr::MouseMsgFilter(LPMSG pMsg) {
    if (!m_pRefMapMng) {
        return FALSE;
    }
    CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();
    if (!m_bActive || !pRefTerrain) {
        return FALSE;
    }

    switch (pMsg->message) {
    case WM_LBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};

        __Vector3 vec;
        if (!pRefTerrain->Pick(point.x, point.y, &vec, NULL)) {
            break;
        }

        m_pCurrEvent->InitRect(vec);
    }
        return TRUE;
    case WM_LBUTTONUP: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};

        __Vector3 vec;
        if (!pRefTerrain->Pick(point.x, point.y, &vec, NULL)) {
            break;
        }

        m_pCurrEvent->AddRect(vec);
    }
        return TRUE;
    case WM_MOUSEMOVE: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (nFlags & MK_LBUTTON) {
            __Vector3 vec;
            if (!pRefTerrain->Pick(point.x, point.y, &vec, NULL)) {
                break;
            }
            m_pCurrEvent->AddRect(vec);
        }
    }
        return TRUE;
    case WM_RBUTTONUP: {
        CDlgEditEventAttr Dlg;
        Dlg.SetEventCell(m_pCurrEvent);
        if (Dlg.DoModal() == IDOK) {
            m_pDlgEventList->SetCurrName(m_pCurrEvent->m_Name);
        }
    }
    }
    return FALSE;
}

void CEventMgr::Render() {
    HRESULT hr;

    D3DXMATRIX mtx;
    D3DXMatrixIdentity(&mtx);

    hr = s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtx); // 월드 행렬 적용..

    // set texture
    hr = s_lpD3DDev->SetTexture(0, NULL);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    // backup state
    DWORD dwZEnable, dwLighting, dwCullMode;
    hr = s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    hr = s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    hr = s_lpD3DDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);

    // set state
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    hr = s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    hr = s_lpD3DDev->SetFVF(FVF_XYZCOLOR);

    //이미 만들어진 길 그리기...
    std::list<CEventCell *>::iterator itEvent;

    CEventCell * pEvent;
    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        pEvent = (*itEvent);
        if (!pEvent) {
            continue;
        }

        pEvent->Render(0xff0000ff);
    }

    //대화상자에서 선택된 길 그리기.
    if (m_pDlgEventList->m_pSelEvent) {
        m_pDlgEventList->m_pSelEvent->Render(0xff00ff00);
    }

    //만들고 있는 길 & 영역 그리기..
    m_pCurrEvent->Render(0xffff0000);

    // restore
    hr = s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    hr = s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwLighting);
    hr = s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);
}

void CEventMgr::UpdateEvent() {
    m_pEvents.push_back(m_pCurrEvent);

    CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();
    CEventCell * pEvent = new CEventCell(pRefTerrain);
    m_pCurrEvent = pEvent;
    m_pDlgEventList->SetCurrName(m_pCurrEvent->m_Name);
}

void CEventMgr::DelEvent(CEventCell * pEvent) {
    std::list<CEventCell *>::iterator itEvent;

    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        if (pEvent == (*itEvent)) {
            delete pEvent;
            m_pEvents.erase(itEvent);
            return;
        }
    }
}

void CEventMgr::SetCurrEvent(CEventCell * pEvent) {
    if (m_pCurrEvent) {
        delete m_pCurrEvent;
        m_pCurrEvent = NULL;
    }

    m_pCurrEvent = pEvent;
    m_pDlgEventList->SetCurrName(m_pCurrEvent->m_Name);

    std::list<CEventCell *>::iterator itEvent;

    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        if (pEvent == (*itEvent)) {
            m_pEvents.erase(itEvent);
            return;
        }
    }
}

void CEventMgr::SetActive(bool active) {
    if (m_bActive == active) {
        return;
    }

    m_bActive = active;

    if (active) {
        CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();

        if (m_MapSize == 0) {
            CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();
            m_MapSize = pRefTerrain->m_iHeightMapSize;

            if (m_ppEvent) {
                GlobalFree(m_ppEvent); //delete[] m_ppEvent;
            }

            //m_pMapData = (LPMAPDATA)GlobalAlloc(GMEM_FIXED, sizeof(MAPDATA)*m_ti_MapSize*m_ti_MapSize);
            //m_ppEvent = new short* [m_MapSize];
            m_ppEvent = (short **)GlobalAlloc(GMEM_FIXED, sizeof(short *) * m_MapSize);
            for (int x = 0; x < m_MapSize; x++) {
                //m_ppEvent[x] = new short [m_MapSize];
                m_ppEvent[x] = (short *)GlobalAlloc(GMEM_FIXED, sizeof(short) * m_MapSize);
                for (int z = 0; z < m_MapSize; z++) {
                    m_ppEvent[x][z] = 1;
                }
            }
        }
        m_pDlgEventList->ShowWindow(TRUE);

        CEventCell * pEvent = new CEventCell(pRefTerrain);
        if (m_pCurrEvent) {
            delete m_pCurrEvent;
            m_pCurrEvent = NULL;
        }
        m_pCurrEvent = pEvent;
        m_pDlgEventList->SetCurrName(m_pCurrEvent->m_Name);
    } else {
        m_pDlgEventList->ShowWindow(FALSE);

        if (m_pCurrEvent) {
            delete m_pCurrEvent;
            m_pCurrEvent = NULL;
        }
    }
}

void CEventMgr::LoadFromFile(const fs::path & fsFileName) {
    fs::path fsFile = (CN3Base::PathGet() / "event" / fsFileName).replace_extension(".evt");
    HANDLE   hFile = CreateFileW(fsFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    Load(hFile);
    CloseHandle(hFile);
}

void CEventMgr::SaveToFile(const fs::path & fsFileName) {
    fs::path fsCurDirPrev = fs::current_path();
    fs::current_path(CN3Base::PathGet());
    fs::path fsDir("event");
    fs::create_directory(fsDir);

    fs::path fsFile = (CN3Base::PathGet() / fsDir / fsFileName).replace_extension(".evt");
    HANDLE   hFile = CreateFileW(fsFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    Save(hFile);
    CloseHandle(hFile);
    fs::current_path(fsCurDirPrev);
}

bool CEventMgr::Load(HANDLE hFile) {
    if (m_pCurrEvent) {
        delete m_pCurrEvent;
        m_pCurrEvent = NULL;
    }
    CLyTerrain * pRefTerrain = m_pRefMapMng->GetTerrain();
    m_pCurrEvent = new CEventCell(pRefTerrain);
    m_pDlgEventList->SetCurrName(m_pCurrEvent->m_Name);

    std::list<CEventCell *>::iterator itEvent;
    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        delete (*itEvent);
    }

    DWORD dwRWC;
    int   NumEvent;
    ReadFile(hFile, &NumEvent, sizeof(int), &dwRWC, NULL);

    m_pEvents.clear();
    for (int i = 0; i < NumEvent; i++) {
        CEventCell * pEvent = new CEventCell(pRefTerrain);
        pEvent->Load(hFile);
        m_pEvents.push_back(pEvent);
    }

    m_pDlgEventList->ResetAll();

    return true;
}

bool CEventMgr::Save(HANDLE hFile) {
    DWORD dwRWC;
    int   NumEvent = m_pEvents.size();
    WriteFile(hFile, &NumEvent, sizeof(int), &dwRWC, NULL);

    std::list<CEventCell *>::iterator itEvent;

    CEventCell * pEvent;
    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        pEvent = (*itEvent);
        pEvent->Save(hFile);
    }

    return true;
}

void CEventMgr::MakeEventArray() {
    std::list<CEventCell *>::iterator itEvent;

    CEventCell * pEvent;
    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        pEvent = (*itEvent);
        if (pEvent) {
            RECT rt = pEvent->m_Rect;
            for (int x = rt.left; x <= rt.right; x++) {
                for (int z = rt.bottom; z <= rt.top; z++) {
                    m_ppEvent[x][z] = pEvent->m_EventID;
                }
            }
        }
    }
}

void CEventMgr::SaveInfoTextFile(const fs::path & fsFile) {
    // text 파일 버전...
    FILE * stream = _wfopen(fsFile.c_str(), L"r");
    //if(!stream)    return;

    std::list<CEventCell *> TmpList;

    CEventCell * pEvent = NULL;
    if (stream) {
        while (!feof(stream)) {
            pEvent = new CEventCell;
            fscanf(stream, "%d", &pEvent->m_ZoneID);
            fscanf(stream, "\t%d", &pEvent->m_EventID);
            fscanf(stream, "\t%d", &pEvent->m_EventType);
            for (int i = 0; i < 5; i++) {
                fscanf(stream, "\t%s", &pEvent->m_Con[i]);
            }

            for (int i = 0; i < 5; i++) {
                fscanf(stream, "\t%s", &pEvent->m_Exe[i]);
            }

            fscanf(stream, "\n");

            TmpList.push_back(pEvent);
        }
        fclose(stream);
    }

    int                               zoneid = m_pRefMapMng->m_iZoneID;
    std::list<CEventCell *>::iterator itEvent = TmpList.begin();

    while (itEvent != TmpList.end()) {
        if (pEvent->m_ZoneID == zoneid) {
            itEvent = TmpList.erase(itEvent);
        } else {
            itEvent++;
        }
    }

    stream = _wfopen(fsFile.c_str(), L"w");
    if (!stream) {
        return;
    }

    for (itEvent = TmpList.begin(); itEvent != TmpList.end(); itEvent++) {
        pEvent = (*itEvent);
        if (pEvent) {
            fprintf(stream, "%d", pEvent->m_ZoneID);
            fprintf(stream, "\t%d", pEvent->m_EventID);
            fprintf(stream, "\t%d", pEvent->m_EventType);
            for (int i = 0; i < 5; i++) {
                fprintf(stream, "\t%s", pEvent->m_Con[i]);
            }

            for (int i = 0; i < 5; i++) {
                fprintf(stream, "\t%s", pEvent->m_Exe[i]);
            }

            fprintf(stream, "\n");
        }
    }

    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        pEvent = (*itEvent);
        if (pEvent) {
            fprintf(stream, "%d", pEvent->m_ZoneID);
            fprintf(stream, "\t%d", pEvent->m_EventID);
            fprintf(stream, "\t%d", pEvent->m_EventType);
            for (int i = 0; i < 5; i++) {
                fprintf(stream, "\t%s", pEvent->m_Con[i]);
            }

            for (int i = 0; i < 5; i++) {
                fprintf(stream, "\t%s", pEvent->m_Exe[i]);
            }

            fprintf(stream, "\n");
        }
    }
    fclose(stream);
}

/*
void CEventMgr::SaveInfoTextFile(char* szEvent)
{
    // text 파일 버전...
    FILE* stream = fopen(szEvent, "w");
    if(!stream)    return;

    std::list<CEventCell*>::iterator itEvent;

    CEventCell* pEvent;
    for(itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++)
    {
        pEvent = (*itEvent);
        if(pEvent)
        {
            fprintf(stream, "%d", pEvent->m_ZoneID);
            fprintf(stream, "\t%d", pEvent->m_EventID);
            fprintf(stream, "\t%d", pEvent->m_EventType);
            for(int i=0;i<5;i++)
                fprintf(stream, "\t%s", pEvent->m_Con[i]);

            for(int i=0;i<5;i++)
                fprintf(stream, "\t%s", pEvent->m_Exe[i]);

            fprintf(stream, "\n");
        }
    }
    fclose(stream);
}
*/

bool CEventMgr::MakeGameFile(const fs::path & fsFile, int iSize) {
    HANDLE hGevFile = CreateFileW(fsFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hGevFile) {
        return false;
    }

    MakeEventArray();
    if (!MakeGameFile(hGevFile, iSize)) {
        CloseHandle(hGevFile);
        return false;
    }
    CloseHandle(hGevFile);
    return true;
}

bool CEventMgr::MakeGameFile(HANDLE hFile, int iSize) {
    DWORD dwNum = 0;
    //    WriteFile(hFile, &iSize, sizeof(int), &dwNum, NULL);
    //    for(int x=0;x<iSize;x++)
    //        WriteFile(hFile, m_ppEvent[x], sizeof(short)*iSize, &dwNum, NULL);

    int nEventCellCount = m_pEvents.size();
    WriteFile(hFile, &nEventCellCount, sizeof(int), &dwNum, NULL);

    std::list<CEventCell *>::iterator itEvent;

    CEventCell * pEvent;
    for (itEvent = m_pEvents.begin(); itEvent != m_pEvents.end(); itEvent++) {
        pEvent = (*itEvent);
        if (pEvent) {
            RECT rt = pEvent->m_Rect;
            WriteFile(hFile, &rt, sizeof(RECT), &dwNum, NULL);
            WriteFile(hFile, &pEvent->m_EventType, sizeof(short), &dwNum, NULL);
        } else {
            short sEventType = -1;
            RECT  rt;
            memset(&rt, 0x00, sizeof(rt));
            WriteFile(hFile, &rt, sizeof(RECT), &dwNum, NULL);
            WriteFile(hFile, &sEventType, sizeof(short), &dwNum, NULL);
        }
    }

    return true;
}
