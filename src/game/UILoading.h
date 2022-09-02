// UILoading.h: interface for the UILoading class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CUILoading : public CN3UIBase {
  protected:
    CN3UIString *   m_pText_Version;
    CN3UIString *   m_pText_Info;
    CN3UIProgress * m_pProgress_Loading;

  public:
    bool Load(HANDLE hFile);
    CUILoading();
    virtual ~CUILoading();

    virtual void Render(const std::string & szInfo, int iPercentage);
    virtual void Release();
};
