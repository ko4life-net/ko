// Bird.h: interface for the CBird class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Base.h"

class CN3Shape;
class CN3SndObj;

class CBird : public CN3Base {
  public:
    CBird();
    virtual ~CBird();

    // Attributes
  public:
  protected:
    // Information read from file
    CN3Shape * m_pShape;  // Shape pointer
    __Vector3  m_vPivot;  // Center point (moves within a range of radius m_fRadius from the center point.
    float      m_fRadius; // radius

    //    __Vector3    m_vVelocity;    // Current speed (speed + direction)
    float m_fRadianSpeed; // speed
    float m_fRadian;      // Current position angle of ellipse
    float m_fFactor1, m_fFactor2;
    float m_fFactorSpeed1, m_fFactorSpeed2;

    float m_fRadiusY;                         // height radius
    float m_fFactorY1, m_fFactorY2;           // height change factor
    float m_fFactorYSpeed1, m_fFactorYSpeed2; // Height change factor rate of change...

    CN3SndObj * m_pSnd;
    float       m_fSndInterval;

    // Operations
  public:
    virtual void Release();
    void         Tick();
    void         Render();

    int LoadBird(const std::string & szFN);

  protected:
};
