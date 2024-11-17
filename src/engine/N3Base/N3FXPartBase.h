// N3FXPartBase.h: interface for the CN3FXPartBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"
#include "N3FXDef.h"
#include "N3Texture.h"

class CN3FXBundle;

class CN3FXPartBase : public CN3BaseFileAccess {
  public:
    //멤버 변수들..
    int m_iVersion; //    자료의 버전..
    int m_iBaseVersion;

    CN3FXBundle *   m_pRefBundle;   //    내가 속한 번들의 포인터.
    CN3FXPartBase * m_pRefPrevPart; //    내 앞의 파트..

    int   m_iType; //    형식타입(particle, mesh, board...)
    float m_fLife; //    플레이시간..

    std::string m_strName; //    파트이름..

    __Vector3 m_vVelocity;     //    초당 움직이는 속도..(초속도)
    __Vector3 m_vAcceleration; //    가속도...(중력가속도를 넣고 싶으면 y = -9.8..^^
    __Vector3 m_vRotVelocity;  //    초당 회전할 수 있는 각속도..

    float     m_fCurrLife;
    __Vector3 m_vCurrVelocity;
    __Vector3 m_vCurrPos;

    DWORD     m_dwState; //    현재 파트의 상태..
    __Vector3 m_vPos;    //    번들에서 파트의 위치.

    bool m_bOnGround; //바닥에 붙어서 갈 것인가...

    fs::path      m_fsTexFileBase; // without texture index and ext (fx/billboard/tex|{0,1,2}.dxt)
    CN3Texture ** m_ppRefTex;
    int           m_iNumTex;
    float         m_fTexFPS;

    float m_fFadeOut;
    float m_fFadeIn;

    DWORD m_dwRenderFlag;

    DWORD m_dwSrcBlend;
    DWORD m_dwDestBlend;
    BOOL  m_bAlpha;

    DWORD m_dwZEnable;
    DWORD m_dwZWrite;
    DWORD m_dwLight;
    DWORD m_dwDoubleSide;

  protected:
    virtual bool IsDead();

  public:
    virtual float GetGroundHeight(float x, float z) { return 0.01f; }

    virtual void Init();             //    각종 변수들을 처음 로딩한 상태로 초기화...
    virtual void Start();            //    파트 구동 시작.
    virtual void Stop();             //    파트 구동 멈춤..
    virtual bool Tick();             //    ticktick...
    virtual void Render();           //    화면에 뿌리기..
    virtual bool Load(HANDLE hFile); //    게임파일 불러오기.
    virtual bool Save(HANDLE hFile); //    게임파일 저장하기.
    void         Duplicate(CN3FXPartBase * pSrc);

#ifdef _N3TOOL
  protected:
    virtual bool ParseScript(char * szCommand, char * szBuff0, char * szBuff1, char * szBuff2,
                             char * szBuff3); //실질적인 스크립트 해석 함수..

  public:
    bool DecodeScriptFile(const fs::path & fsFile);
#endif // end of _N3TOOL

  public:
    //생성자 소멸자..
    CN3FXPartBase();
    virtual ~CN3FXPartBase();
};
