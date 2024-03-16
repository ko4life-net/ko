//////////////////////////////////////////////////
//    Commented By : Lynus
//    Commented On 2001-04-12 Apr 5:09:28
//
//    CN3SndObjStream class (SoundObj.h)
//
//    End Of Comment (By Lynus On 2001-04-12 ¿ÀÈÄ 5:09:28 )
//////////////////////////////////////////////////

#pragma once

#include "N3SndObj.h"

class CN3SndEng;

class CN3SndObjStream : public CN3SndObj {
  private:
    int m_PlayTime; // The total playing time of the wave file.
    int m_FinalByte;

    DSBUFFERDESC m_dsbd;
    DWORD        m_BufferSize;
    DWORD        m_BlockSize; // Divide the buffer by seconds.

    int   m_CurrState; //Which block are you currently on?
    int   m_PrevState; //Which block were you on just before now?
    DWORD m_CurrPos;   //position in the current buffer
    int   m_PastTime;  //Time remaining in total play time

    //related wave data
    WAVEFORMATEX m_WaveFormat;
    DWORD        m_WaveSize;
    LPSTR        m_pWaveMem;
    HGLOBAL      m_hWaveMem;
    MMCKINFO     mmCkInfoRIFF;
    MMCKINFO     mmCkInfoChunk;
    HMMIO        hMMIO;

  public:
    bool Create(const std::string & szFN, e_SndType eType = SNDTYPE_STREAM);

    //    void Play(float delay = 0.0f, float fFadeInTime = 0.0f);
    //    void Stop(float fFadeOutTime = 0.0f);

    void Tick();
    void Release();

    CN3SndObjStream();
    virtual ~CN3SndObjStream();

  private:
    void RealPlay();

    //void Stop();

    BOOL LoadWave(LPCSTR pFileName);
    BOOL WriteBuffer();
    BOOL InitWriteBuffer();
    void Reset();
};
