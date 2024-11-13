//////////////////////////////////////////////////
//    Commented By : Lynus
//    Commented On 2001-04-12 오후 1:51:00
//
//    CWave Class (WaveFile.h)
//
//    End Of Comment (By Lynus On 2001-04-12 오후 1:51:00 )
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//    Coded By : Lynus
//    Coded On 2001-04-12 오후 1:50:14
//

#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#define WAVEFILE_READ  1
#define WAVEFILE_WRITE 0

class CWaveFile {
  public:
    WAVEFORMATEX * m_pwfx;   // Pointer to WAVEFORMATEX structure
    HMMIO          m_hmmio;  // MM I/O handle for the WAVE
    MMCKINFO       m_ck;     // Multimedia RIFF chunk
    MMCKINFO       m_ckRiff; // Use in opening a WAVE file
    DWORD          m_dwSize; // The size of the wave file
    MMIOINFO       m_mmioinfoOut;
    DWORD          m_dwFlags;
    BOOL           m_bIsReadingFromMemory;
    BYTE *         m_pbData;
    BYTE *         m_pbDataCur;
    ULONG          m_ulDataSize;

  protected:
    HRESULT ReadMMIO();
    HRESULT WriteMMIO(WAVEFORMATEX * pwfxDest);

  public:
    CWaveFile();
    ~CWaveFile();

    HRESULT Open(const fs::path & fsFile, WAVEFORMATEX * pwfx, DWORD dwFlags);
    HRESULT OpenFromMemory(BYTE * pbData, ULONG ulDataSize, WAVEFORMATEX * pwfx, DWORD dwFlags);
    HRESULT Close();

    HRESULT Read(BYTE * pBuffer, DWORD dwSizeToRead, DWORD * pdwSizeRead);
    HRESULT Write(UINT nSizeToWrite, BYTE * pbData, UINT * pnSizeWrote);

    DWORD          GetSize();
    HRESULT        ResetFile();
    WAVEFORMATEX * GetFormat() { return m_pwfx; };
};

//    End Of Code (By Lynus On 2001-04-12 오후 1:50:14 )
//////////////////////////////////////////////////
