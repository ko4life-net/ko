// UIEndingDisplay.h: interface for the CUIEndingDisplay class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUIEndingDisplay : public CN3UIBase {
  public:
    void Render();
    void Release();

    CUIEndingDisplay();
    virtual ~CUIEndingDisplay();
};
