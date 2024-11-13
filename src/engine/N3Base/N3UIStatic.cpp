// N3UIStatic.cpp: implementation of the CN3UIStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3UIStatic.h"
#include "N3UIString.h"
#include "N3UIImage.h"

#include "N3SndMgr.h"
#include "N3SndObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIStatic::CN3UIStatic() {
    m_eType = UI_TYPE_STATIC;

    m_pBuffOutRef = NULL;
    m_pImageBkGnd = NULL;
    m_pSnd_Click = NULL;
}

CN3UIStatic::~CN3UIStatic() {
    CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Click);
}

void CN3UIStatic::Release() {
    CN3UIBase::Release();
    m_pBuffOutRef = NULL;
    m_pImageBkGnd = NULL;
    CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Click);
}

//void CN3UIStatic::Render()
//{
//    if (!m_bVisible) return;
//
//    CN3UIBase::Render();
//
////    if (m_pImageBkGnd) m_pImageBkGnd->Render();
////    if (m_pBuffOutRef) m_pBuffOutRef->Render();
//}

void CN3UIStatic::SetRegion(const RECT & Rect) {
    CN3UIBase::SetRegion(Rect);
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        (*itor)->SetRegion(Rect);
    }
}

bool CN3UIStatic::Load(HANDLE hFile) {
    if (false == CN3UIBase::Load(hFile)) {
        return false;
    }

    // m_pImageBkGnd,  m_pBuffOutRef 설정하기
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        CN3UIBase * pChild = (*itor);
        if (UI_TYPE_IMAGE == pChild->UIType()) {
            m_pImageBkGnd = (CN3UIImage *)pChild;
        } else if (UI_TYPE_STRING == pChild->UIType()) {
            m_pBuffOutRef = (CN3UIString *)pChild;
        }
    }

    // 이전 uif파일을 컨버팅 하려면 사운드 로드 하는 부분 막기
    int   iSndFNLen = 0;
    DWORD dwNum;
    ReadFile(hFile, &iSndFNLen, sizeof(iSndFNLen), &dwNum, NULL); //    사운드 파일 문자열 길이
    if (iSndFNLen > 0) {
        std::string szFile(iSndFNLen, '\0');
        ReadFile(hFile, szFile.data(), iSndFNLen, &dwNum, NULL);
        __ASSERT(NULL == m_pSnd_Click, "memory leak");
        m_pSnd_Click = s_SndMgr.CreateObj(szFile, SNDTYPE_2D);
    }

    return true;
}

const std::string & CN3UIStatic::GetString() {
    if (m_pBuffOutRef) {
        return m_pBuffOutRef->GetString();
    } else {
        return s_szStringTmp;
    }
}

void CN3UIStatic::SetString(const std::string & szString) {
    if (m_pBuffOutRef) {
        m_pBuffOutRef->SetString(szString);
    }
}

DWORD CN3UIStatic::MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld) {
    DWORD dwRet = UI_MOUSEPROC_NONE;
    if (!m_bVisible) {
        return dwRet;
    }

    if ((dwFlags & UI_MOUSE_LBCLICK) && IsIn(ptCur.x, ptCur.y)) // 왼쪽버튼 눌르는 순간 영역 안이면
    {
        if (m_pSnd_Click) {
            m_pSnd_Click->Play(); // 사운드가 있으면 play하기
        }

        if (m_pParent) {
            m_pParent->ReceiveMessage(this, UIMSG_BUTTON_CLICK); // 부모에게 버튼 클릭 통지..
        }
        dwRet |= (UI_MOUSEPROC_DONESOMETHING | UI_MOUSEPROC_INREGION);
        return dwRet;
    }

    dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
    return dwRet;
}

#ifdef _N3TOOL
void CN3UIStatic::operator=(const CN3UIStatic & other) {
    CN3UIBase::operator=(other);

    SetSndClick(other.GetSndFileClick());

    // m_pImageBkGnd,  m_pBuffOutRef 설정하기
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        CN3UIBase * pChild = (*itor);
        if (UI_TYPE_IMAGE == pChild->UIType()) {
            m_pImageBkGnd = (CN3UIImage *)pChild;
        } else if (UI_TYPE_STRING == pChild->UIType()) {
            m_pBuffOutRef = (CN3UIString *)pChild;
        }
    }
}

bool CN3UIStatic::Save(HANDLE hFile) {
    if (false == CN3UIBase::Save(hFile)) {
        return false;
    }

    DWORD       dwNum;
    std::string szFile;
    int         iLen = 0;
    if (m_pSnd_Click) {
        szFile = fs::path(m_pSnd_Click->m_fsFile).normalize('/', '\\').string();
        iLen = szFile.length();
    }
    WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); //    사운드 파일 문자열 길이
    if (iLen > 0) {
        WriteFile(hFile, szFile.c_str(), iLen, &dwNum, NULL);
    }
    return true;
}

void CN3UIStatic::CreateImageAndString() {
    m_pImageBkGnd = new CN3UIImage();
    m_pImageBkGnd->Init(this);
    m_pImageBkGnd->SetRegion(m_rcRegion);

    m_pBuffOutRef = new CN3UIString(); // 화면에 표시할 ui string 생성하고
    m_pBuffOutRef->Init(this);         // 초기화(자동으로 children list 들어간다.)
    m_pImageBkGnd->SetRegion(m_rcRegion);
}

void CN3UIStatic::DeleteImage() {
    if (m_pImageBkGnd) {
        delete m_pImageBkGnd;
        m_pImageBkGnd = NULL;
    }
}

void CN3UIStatic::SetSndClick(const fs::path & fsFile) {
    CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Click);
    fs::path fsFileRel = CN3BaseFileAccess::ToRelative(fsFile);
    if (fsFileRel.empty()) {
        return;
    }

    fs::current_path(CN3Base::PathGet());
    m_pSnd_Click = s_SndMgr.CreateObj(fsFileRel, SNDTYPE_2D);
}

fs::path CN3UIStatic::GetSndFileClick() const {
    return m_pSnd_Click ? m_pSnd_Click->m_fsFile : fs::path();
}
#endif
