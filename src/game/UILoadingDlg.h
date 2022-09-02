// UILoadingDlg.h: interface for the UILoadingDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3UIBase.h"

class CN3UIProgress;

class CUILoadingDlg : public CN3UIBase {
  public:
    bool Load(HANDLE hFile);
    CUILoadingDlg();
    virtual ~CUILoadingDlg();

    virtual void Release();

    CN3UIProgress * GetProcCtrl(char *);
};
