// N3ShapeExtra.h: interface for the CN3ShapeExtra class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Shape.h"
#include <vector>

// std::vector<struct __Rotation>::iterator it_Rot;

class CN3ShapeExtra : public CN3Shape {
  protected:
    struct __Rotation {
        float     fRadianPerSec;  // Rotations per second... Radian...
        __Vector3 vAxis;          // Rotation axis..
        float     fRadianCur;     // Current rotation value..
        float     fRadianToReach; // Value to rotate...

        __Rotation() {
            memset(this, 0, sizeof(__Rotation));
            vAxis.Set(0, 1, 0);
        }
    };

    std::vector<__Rotation> m_Rotations;

  public:
    void RotateTo(int iPart, const __Vector3 & vAxis, float fRadianToReach, float fRadianPerSec,
                  bool bImmediately = false); // Rotate the desired part at a specified speed to a specified angle along the axis.

    bool Load(HANDLE hFile);
    void Tick(float fFrm);

    void Release();
    CN3ShapeExtra();
    virtual ~CN3ShapeExtra();
};
