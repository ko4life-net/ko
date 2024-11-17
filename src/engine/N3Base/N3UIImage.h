// N3UIImage.h: interface for the CN3UIImage class.
// 보강해야 할 점 : animation되는 이미지도 그리도록 가능하게 하자..
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3UIBase.h"

class N3Texture;
class CN3UIImage : public CN3UIBase {
#ifdef _N3TOOL
    friend class CPropertyView; // 툴에서 각 변수들을 접근하기 위해서
#endif

  public:
    CN3UIImage();
    virtual ~CN3UIImage();

    // Attributes
  public:
    __FLOAT_RECT * GetUVRect() { return &m_frcUVRect; }

  protected:
    LPDIRECT3DVERTEXBUFFER9 m_pVB; // vertex buffer
    CN3Texture * m_pTexRef;        // texture 참조 포인터 (s_TexMng에서 관리하므로 참조포인터이다.)
    fs::path m_fsTexFile; // 텍스처 이름.. 따로 갖고 있는 이유는 툴에서 텍스처 부르기가 실패할 경우를 대비해서이다.
    __FLOAT_RECT m_frcUVRect; // uv 좌표를 저장
    D3DCOLOR     m_Color;     // 칼라(배경 이미지가 없을경우 사용된다.)

    float         m_fAnimFrame;     // 1초당 그려지는 이미지의 수
    int           m_iAnimCount;     // Animate되는 이미지 수(Load할 때 child의 숫자로 파악)
    float         m_fCurAnimFrame;  // 현재 그려질 animate frame index (소수는 버리고 사용하자)
    CN3UIImage ** m_pAnimImagesRef; // Animate 될 자식 이미지들 참조 포인터들

    //    Operations
  public:
    D3DCOLOR GetColor() { return m_Color; }

    CN3Texture * GetTex() const { return m_pTexRef; }                         // Texture 포인터 얻기
    void         SetTex(const fs::path & fsFile);                             // Texture 지정
    void         SetUVRect(float left, float top, float right, float bottom); // image의 uv좌표 지정
    void         SetColor(D3DCOLOR color);                                    // 칼라 지정

    virtual void SetRegion(const RECT & Rect); // 영역 지정
    virtual BOOL MoveOffset(int iOffsetX, int iOffsetY);
    virtual void Release(); // Release
    virtual void Tick();    // Tick
    virtual void Render();  // 그리기
    virtual void RenderIconWrapper();
    virtual void Init(CN3UIBase * pParent); // 초기화
    virtual bool Load(HANDLE hFile);

    virtual void operator=(const CN3UIImage & other);

  protected:
    bool         CreateVB(); // 4개의 vertex를 가진 vertex buffer 생성
    virtual void SetVB();    // vertex buffer 다시 세팅
// tool에서 사용하는 함수들
#ifdef _N3TOOL
  public:
    virtual bool Save(HANDLE hFile);
    virtual void ChangeImagePath(const fs::path & fsOldFile, const fs::path & fsNewFile);
    void         GatherImageFileName(std::set<fs::path> & setImgFile);
    fs::path     GetTexFile() { return m_fsTexFile; }

    void         ReorderChildImage();
    void         SetAnimImage(int iAnimCount);
    CN3UIImage * GetChildImage(int iIndex);
    virtual bool ReplaceAllTextures(const std::string & strFind, const std::string & strReplace);
#endif
};
