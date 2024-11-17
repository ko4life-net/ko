// N3UIImage.cpp: implementation of the CN3UIImage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3UIImage.h"
#include "N3Texture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIImage::CN3UIImage() {
    m_eType = UI_TYPE_IMAGE;

    m_pVB = NULL;
    m_pTexRef = NULL;
    m_pAnimImagesRef = NULL;

    ZeroMemory(&m_frcUVRect, sizeof(m_frcUVRect));
    m_Color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
    m_fAnimFrame = 30.0f;
    m_iAnimCount = 0;
    m_fCurAnimFrame = 0.0f;
}

CN3UIImage::~CN3UIImage() {
    if (m_pVB) {
        m_pVB->Release();
        m_pVB = NULL;
    }
    s_MngTex.Delete(&m_pTexRef);
    if (m_pAnimImagesRef) {
        delete[] m_pAnimImagesRef;
        m_pAnimImagesRef = NULL;
    }
}

void CN3UIImage::Release() {
    CN3UIBase::Release();
    if (m_pVB) {
        m_pVB->Release();
        m_pVB = NULL;
    }
    s_MngTex.Delete(&m_pTexRef);
    m_fsTexFile.clear();

    ZeroMemory(&m_frcUVRect, sizeof(m_frcUVRect));
    m_Color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
    m_fAnimFrame = 30.0f;
    m_iAnimCount = 0;
    m_fCurAnimFrame = 0.0f;
    if (m_pAnimImagesRef) {
        delete[] m_pAnimImagesRef;
        m_pAnimImagesRef = NULL;
    }
}

void CN3UIImage::Init(CN3UIBase * pParent) {
    CN3UIBase::Init(pParent);
    CreateVB();
}

bool CN3UIImage::CreateVB() {
    HRESULT hr;
    if (m_pVB) {
        m_pVB->Release();
        m_pVB = NULL;
    }
    hr = s_lpD3DDev->CreateVertexBuffer(4 * sizeof(__VertexTransformed), 0, FVF_TRANSFORMED, D3DPOOL_MANAGED, &m_pVB,
                                        NULL);
    return SUCCEEDED(hr);
}

void CN3UIImage::SetVB() {
    if (UISTYLE_IMAGE_ANIMATE & m_dwStyle) // animate image이면 vertex buffer release하기
    {
        if (m_pVB) {
            m_pVB->Release();
            m_pVB = NULL;
        }
    } else {
        if (m_pVB) {
            __VertexTransformed * pVertices;
            m_pVB->Lock(0, 0, (VOID **)&pVertices, 0);

            DWORD dwColor = 0xffffffff;
            float fRHW = 1.0f;
            // -0.5f를 해주지 않으면 가끔 이미지가 한 돗트씩 밀리는 경우가 있다.(왜 그런지는 확실하게 모르겠음)
            pVertices[0].Set((float)m_rcRegion.left - 0.5f, (float)m_rcRegion.top - 0.5f, UI_DEFAULT_Z, fRHW, m_Color,
                             m_frcUVRect.left, m_frcUVRect.top);
            pVertices[1].Set((float)m_rcRegion.right - 0.5f, (float)m_rcRegion.top - 0.5f, UI_DEFAULT_Z, fRHW, m_Color,
                             m_frcUVRect.right, m_frcUVRect.top);
            pVertices[2].Set((float)m_rcRegion.right - 0.5f, (float)m_rcRegion.bottom - 0.5f, UI_DEFAULT_Z, fRHW,
                             m_Color, m_frcUVRect.right, m_frcUVRect.bottom);
            pVertices[3].Set((float)m_rcRegion.left - 0.5f, (float)m_rcRegion.bottom - 0.5f, UI_DEFAULT_Z, fRHW,
                             m_Color, m_frcUVRect.left, m_frcUVRect.bottom);

            m_pVB->Unlock();
        }
    }
}

void CN3UIImage::SetTex(const fs::path & fsFile) {
    m_fsTexFile = fsFile;
    s_MngTex.Delete(&m_pTexRef);
    // animate image일때만 texture 지정하기
    if (!(UISTYLE_IMAGE_ANIMATE & m_dwStyle)) {
        m_pTexRef = s_MngTex.Get(fsFile);
    }
}

void CN3UIImage::SetRegion(const RECT & Rect) {
    CN3UIBase::SetRegion(Rect);
    for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        (*itor)->SetRegion(Rect);
    }
    SetVB();
}

void CN3UIImage::SetUVRect(float left, float top, float right, float bottom) {
    m_frcUVRect.left = left;
    m_frcUVRect.top = top;
    m_frcUVRect.right = right;
    m_frcUVRect.bottom = bottom;
    SetVB();
}

void CN3UIImage::Tick() {
    CN3UIBase::Tick();
    if (m_iAnimCount > 0) // Animate Image일때 현재 frame 계산
    {
        m_fCurAnimFrame += (s_fSecPerFrm * m_fAnimFrame);
        while (m_fCurAnimFrame >= (float)m_iAnimCount) {
            m_fCurAnimFrame -= ((float)m_iAnimCount);
        }
    }
}

void CN3UIImage::Render() {
    if (!m_bVisible) {
        return;
    }

    if (UISTYLE_IMAGE_ANIMATE & m_dwStyle) // Animate되는 이미지이면
    {
        __ASSERT(m_fCurAnimFrame >= 0.0f && m_fCurAnimFrame < (float)m_iAnimCount, "animate image 가 이상작동");
        __ASSERT(m_pAnimImagesRef, "초기화 이상");
        m_pAnimImagesRef[(int)m_fCurAnimFrame]->Render();
    } else {
        if (m_pVB && m_pTexRef) {
            s_lpD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(__VertexTransformed));
            s_lpD3DDev->SetFVF(FVF_TRANSFORMED);

            s_lpD3DDev->SetTexture(0, m_pTexRef->Get());
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

            s_lpD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
        }

        CN3UIBase::Render();
    }
}

void CN3UIImage::RenderIconWrapper() {
    if (!m_bVisible) {
        return;
    }

    if (m_pVB) {
        s_lpD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(__VertexTransformed));
        s_lpD3DDev->SetFVF(FVF_TRANSFORMED);
        s_lpD3DDev->SetTexture(0, NULL);

        s_lpD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
    }

    CN3UIBase::Render();
}

BOOL CN3UIImage::MoveOffset(int iOffsetX, int iOffsetY) {
    if (FALSE == CN3UIBase::MoveOffset(iOffsetX, iOffsetY)) {
        return FALSE;
    }
    SetVB();
    return TRUE;
}

void CN3UIImage::SetColor(D3DCOLOR color) {
    if (m_Color == color) {
        return;
    }
    m_Color = color;
    if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_pAnimImagesRef) {
        for (int i = 0; i < m_iAnimCount; ++i) {
            m_pAnimImagesRef[i]->SetColor(color);
        }
    }
    SetVB();
}

bool CN3UIImage::Load(HANDLE hFile) {
    if (false == CN3UIBase::Load(hFile)) {
        return false;
    }

    __ASSERT(NULL == m_pTexRef, "Not initialized before loading.");

    DWORD dwNum = 0;

    int iLen = 0;
    ReadFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // 파일 이름 길이
    if (iLen > 0) {
        std::string szFile(iLen, '\0');
        ReadFile(hFile, szFile.data(), iLen, &dwNum, NULL); // 파일 이름
        SetTex(szFile);
    }

    ReadFile(hFile, &m_frcUVRect, sizeof(m_frcUVRect), &dwNum, NULL); // uv좌표
    ReadFile(hFile, &m_fAnimFrame, sizeof(m_fAnimFrame), &dwNum, NULL);

    // Animate 되는 image이면 관련된 변수 세팅
    m_iAnimCount = 0; // animate image 수 정하기
    UIListItor itor;
    for (itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
        if (UI_TYPE_IMAGE == (*itor)->UIType()) {
            m_iAnimCount++;
        }
    }

    if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_iAnimCount > 0) {
        m_pAnimImagesRef = new CN3UIImage *[m_iAnimCount];
        ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage *) * m_iAnimCount);
        int i = 0;
        for (itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
            if (UI_TYPE_IMAGE == (*itor)->UIType()) {
                m_pAnimImagesRef[i] = (CN3UIImage *)(*itor);
            }
            __ASSERT(m_pAnimImagesRef[i]->GetReserved() == (DWORD)i,
                     "animate Image load fail"); // 제대로 정렬이 되지 않았을경우 실패한다.
            ++i;
        }
    }

    SetVB(); // vertex 세팅
    return true;
}

void CN3UIImage::operator=(const CN3UIImage & other) {
    CN3UIBase::operator=(other);

    m_Color = other.m_Color;
    m_fAnimFrame = other.m_fAnimFrame;
    m_fCurAnimFrame = other.m_fCurAnimFrame;
    m_frcUVRect = other.m_frcUVRect;
    m_iAnimCount = other.m_iAnimCount;

    if (other.m_pTexRef) {
        m_pTexRef = s_MngTex.Get(other.m_pTexRef->FilePath());
    }
    m_fsTexFile = other.m_fsTexFile;

    // Animate 되는 image이면 관련된 변수 세팅
    m_iAnimCount = m_Children.size(); // animate image 수 정하기
    if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_iAnimCount > 0) {
        m_pAnimImagesRef = new CN3UIImage *[m_iAnimCount];
        ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage *) * m_iAnimCount);
        int i = 0;
        for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
            __ASSERT(UI_TYPE_IMAGE == (*itor)->UIType(), "animate image child의 UI type이 image가 아니다.");
            m_pAnimImagesRef[i] = (CN3UIImage *)(*itor);
            __ASSERT(m_pAnimImagesRef[i]->GetReserved() == (DWORD)i,
                     "animate Image load fail"); // 제대로 정렬이 되지 않았을경우 실패한다.
            ++i;
        }
    }

    SetVB(); // vertex 세팅
}

#ifdef _N3TOOL
bool CN3UIImage::Save(HANDLE hFile) {
    ReorderChildImage(); // child image들 순서대로 정렬
    if (false == CN3UIBase::Save(hFile)) {
        return false;
    }
    DWORD dwNum;
    // texture 정보
    if (m_pTexRef) {
        m_fsTexFile = m_pTexRef->FilePathWin();
    }
    std::string szTexFile = m_fsTexFile.string();
    int         iLen = szTexFile.length();
    WriteFile(hFile, &iLen, sizeof(iLen), &dwNum, NULL); // 파일 길이
    if (iLen > 0) {
        WriteFile(hFile, szTexFile.c_str(), iLen, &dwNum, NULL); // 파일 이름
    }

    WriteFile(hFile, &m_frcUVRect, sizeof(m_frcUVRect), &dwNum, NULL);   // uv좌표
    WriteFile(hFile, &m_fAnimFrame, sizeof(m_fAnimFrame), &dwNum, NULL); // Animate frame

    return true;
}

void CN3UIImage::ChangeImagePath(const fs::path & fsOldFile, const fs::path & fsNewFile) {
    CN3UIBase::ChangeImagePath(fsOldFile, fsNewFile);

    fs::path fsOld = fsOldFile, fsNew = fsNewFile;
    if (!fsOld.empty()) {
        CN3BaseFileAccess::ToRelative(fsOld);
    }
    if (!fsNew.empty()) {
        CN3BaseFileAccess::ToRelative(fsNew);
    }

    if (m_pTexRef) {
        m_fsTexFile = m_pTexRef->FilePath();
    }
    if (!m_fsTexFile.empty()) {
        CN3BaseFileAccess::ToRelative(m_fsTexFile);
    }

    if (m_fsTexFile == fsOld) {
        m_fsTexFile = fsNew;
        s_MngTex.Delete(&m_pTexRef);
        m_pTexRef = s_MngTex.Get(m_fsTexFile);
    }
}

void CN3UIImage::GatherImageFileName(std::set<fs::path> & setImgFile) {
    CN3UIBase::GatherImageFileName(setImgFile); // child 정보

    fs::path fsTexFile = m_fsTexFile;
    if (!fsTexFile.empty()) {
        setImgFile.insert(fsTexFile.lower());
    }
}

// child의 image가 m_dwReserved에 들어가있는 숫자 순서에 맞게 재배치
void CN3UIImage::ReorderChildImage() {
    if (m_iAnimCount <= 0) {
        return;
    }
    CN3UIBase ** pNewList = new CN3UIBase *[m_iAnimCount];
    ZeroMemory(pNewList, sizeof(CN3UIBase *) * m_iAnimCount);

    for (int i = 0; i < m_iAnimCount; ++i) {
        CN3UIBase * pSelChild = NULL;
        for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor) {
            CN3UIBase * pChild = (*itor);
            __ASSERT(UI_TYPE_IMAGE == pChild->UIType(), "image가 아닌 child가 있습니다.");
            if (NULL == pSelChild) {
                pSelChild = pChild;
            } else if (pSelChild->GetReserved() > pChild->GetReserved()) {
                pSelChild = pChild;
            }
        }
        __ASSERT(pSelChild, "제일 작은 m_dwReserved를 가진 child가 없다.");
        pNewList[i] = pSelChild;
        RemoveChild(pSelChild);
    }

    for (int i = 0; i < m_iAnimCount; ++i) {
        m_Children.push_back(pNewList[i]); // 작은 순서대로 넣기
    }

    delete[] pNewList;
}

CN3UIImage * CN3UIImage::GetChildImage(int iIndex) {
    if (iIndex >= 0 && iIndex < m_iAnimCount) {
        return m_pAnimImagesRef[iIndex];
    }
    return NULL;
}

void CN3UIImage::SetAnimImage(int iAnimCount) {
    // 이미 설정 되어 있는것이 있으면 지우기
    if (m_pAnimImagesRef) {
        for (int i = 0; i < m_iAnimCount; ++i) { // 자식 지우기
            if (m_pAnimImagesRef[i]) {
                delete m_pAnimImagesRef[i];
                m_pAnimImagesRef[i] = NULL;
            }
        }
        delete[] m_pAnimImagesRef;
        m_pAnimImagesRef = NULL;
    }
    m_iAnimCount = iAnimCount;

    // 0으로 설정하면 보통 image로 전환
    if (0 == m_iAnimCount) {
        SetStyle(m_dwStyle & (~UISTYLE_IMAGE_ANIMATE));
        CreateVB();
        SetVB();
    } else {
        SetStyle(m_dwStyle | UISTYLE_IMAGE_ANIMATE);
        s_MngTex.Delete(&m_pTexRef);
        SetVB();

        m_pAnimImagesRef = new CN3UIImage *[m_iAnimCount];
        ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage *) * m_iAnimCount);
        for (int i = 0; i < m_iAnimCount; ++i) {
            m_pAnimImagesRef[i] = new CN3UIImage();
            m_pAnimImagesRef[i]->Init(this);
            m_pAnimImagesRef[i]->SetReserved(i);
            m_pAnimImagesRef[i]->SetRegion(m_rcRegion);
        }
    }
}

bool CN3UIImage::ReplaceAllTextures(const std::string & strFind, const std::string & strReplace) {
    if (strFind.empty() || strReplace.empty()) {
        return false;
    }

    fs::path fsFindPath(strFind);
    fs::path fsReplacePath(strReplace);

    std::string szFindStem = fsFindPath.stem().string();
    std::string szFindExt = fsFindPath.extension().string();

    std::string szReplaceStem = fsReplacePath.stem().string();
    std::string szReplaceExt = fsReplacePath.extension().string();

    if (szFindStem.empty() || szFindExt.empty() || szReplaceStem.empty() || szReplaceExt.empty()) {
        return false;
    }

    while (m_pTexRef) {
        std::string szTexStem = m_pTexRef->FilePath().stem().string();
        std::string szTexExt = m_pTexRef->FilePath().extension().string();

        fs::path fsNew = m_pTexRef->FilePath().parent_path();
        if (szFindStem == "*") {
            if (szFindExt == ".*") { // *.* ->
                if (szReplaceStem == "*") {
                    fsNew /= szTexStem;
                } else {
                    fsNew /= szReplaceStem;
                }
                if (szReplaceExt == ".*") {
                    fsNew += szTexExt;
                } else {
                    fsNew += szReplaceExt;
                }
            } else { // *.tga ->
                if (!n3std::iequals(szFindExt, szTexExt)) {
                    break; // 확장자가 같지 않으므로 그냥 리턴
                }

                if (szReplaceStem == "*") {
                    fsNew /= szTexStem;
                } else {
                    fsNew /= szReplaceStem;
                }
                if (szReplaceExt == ".*") {
                    fsNew += szTexExt;
                } else {
                    fsNew += szReplaceExt;
                }
            }
        } else {
            if (!n3std::iequals(szFindStem, szTexStem)) {
                break; // 이름이 같지 않으므로 그냥 리턴
            }

            if (szFindExt == ".*") { // abc.* ->
                if (szReplaceStem == "*") {
                    fsNew /= szFindStem;
                } else {
                    fsNew /= szReplaceStem;
                }
                if (szReplaceExt == ".*") {
                    fsNew += szTexExt;
                } else {
                    fsNew += szReplaceExt;
                }
            } else { // 찾는 파일명과 확장자가 지정되어 있을경우 // abc.tga ->
                if (!n3std::iequals(szFindExt, szTexExt)) {
                    break; // 확장자가 같지 않으므로 그냥 리턴
                }

                if (szReplaceStem == "*") {
                    fsNew /= szFindStem;
                } else {
                    fsNew /= szReplaceStem;
                }
                if (szReplaceExt == ".*") {
                    fsNew += szTexExt;
                } else {
                    fsNew += szReplaceExt;
                }
            }
        }
        // 텍스쳐 다시 지정하기
        SetTex(fsNew.string());
        break;
    }
    return CN3UIBase::ReplaceAllTextures(strFind, strReplace);
}
#endif
