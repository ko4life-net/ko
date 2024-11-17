// N3SndObj2D.h: interface for the CN3SndObj2D class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3SndDef.h"
#include <stdio.h>
#include <string>

class CN3SndMgr;

class CN3SndObj2D {
    friend class CN3SndEng;

  protected:
    LPDIRECTSOUNDBUFFER m_pDSBuff;
    bool                m_bIsLoop;
    int                 m_iVol;
    int                 m_iType; //0:2d, 1:3d, 2:streamming..
    CN3SndMgr *         m_pRefMgr;

    float m_fFadeInTime;
    float m_fFadeOutTime;
    float m_fStartDelayTime;
    float m_fTmpSecPerFrm;
    int   m_ePlayState;

    int m_iMaxVolume;

  public:
    int      m_iID;
    bool     m_bUse;
    fs::path m_fsFile; //파일이름...(ID 대용).

  protected:
    virtual void RealPlay();

  public:
    LPDIRECTSOUNDBUFFER * GetBuff() { return &(m_pDSBuff); }

    void SetRefMgr(CN3SndMgr * pRefMgr) { m_pRefMgr = pRefMgr; }

    bool IsPlaying();

    void SetType(int type) { m_iType = type; }
    int  GetType() { return m_iType; }

    void Looping(bool loop) { m_bIsLoop = loop; }
    bool IsLoop() { return m_bIsLoop; }

    int  GetVolume() { return m_iVol; }
    void SetVolume(int Vol); // range : [0,100]

    void SetMaxVolume(int vol) { m_iMaxVolume = vol; }
    int  GetMaxVolume() { return m_iMaxVolume; }

    virtual void Init();
    virtual void Release();
    virtual void Play(const D3DVECTOR * pvPos = NULL, float delay = 0.0f, float fFadeInTime = 0.0f,
                      bool bImmediately = true);
    virtual void Stop(float fFadeOutTime = 0.0f);
    virtual void Tick();

    virtual void SetConeOutSizeVolume(LONG vol) {}
    virtual void SetMaxDistance(D3DVALUE max) {}
    virtual void SetMinDistance(D3DVALUE min) {}
    virtual void SetPos(const D3DVECTOR * pPos) {}

    CN3SndObj2D();
    virtual ~CN3SndObj2D();
};
