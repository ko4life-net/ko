// N3SndObj3D.h: interface for the CN3SndObj3D class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3SndObj.h"

class CN3SndObj3D : public CN3SndObj {
  protected:
    LPDIRECTSOUND3DBUFFER m_pDS3DBuff;      // 3D sound buffer
    DS3DBUFFER            m_dsBufferParams; // 3D buffer properties

  public:
    void SetConeOrientation(D3DVECTOR * pDir);
    void SetConeOutSizeVolume(LONG vol);
    void SetMaxDistance(D3DVALUE max);
    void SetMinDistance(D3DVALUE min);
    void SetPos(const D3DVECTOR * pvPos);

    void Init();
    void Release();

    DS3DBUFFER *            Get3DBuffParams() { return &m_dsBufferParams; }
    LPDIRECTSOUND3DBUFFER * Get3DBuff() { return &m_pDS3DBuff; }

    CN3SndObj3D();
    virtual ~CN3SndObj3D();
};
