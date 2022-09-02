// BigFile.h: interface for the CBigFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CBigFile : public CFile {
    DECLARE_DYNAMIC(CBigFile)
  public:
    ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
    CBigFile();
    virtual ~CBigFile();
};
