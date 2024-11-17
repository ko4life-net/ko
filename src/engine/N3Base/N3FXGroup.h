// N3FXGroup.h: interface for the CN3FXGroup class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3BaseFileAccess.h"
#include "N3FXDef.h"

#include <list>

class CN3FXGroup : public CN3BaseFileAccess {
  public:
    int                    m_iVersion;
    std::list<__FXBInfo *> FXBList;

  public:
    int         GetCount() { return FXBList.size(); }
    __FXBInfo * GetFXBInfo(int idx);

    virtual bool Load(HANDLE hFile); // 핸들에서 읽어오기..
    bool         Save(HANDLE hFile); // 저장하기..

    CN3FXGroup();
    virtual ~CN3FXGroup();

#ifdef _N3TOOL //툴에서만 쓰는 함수들...
  public:
    bool DecodeScriptFile(const fs::path & fsFile);
#endif // end of _N3TOOL
};
