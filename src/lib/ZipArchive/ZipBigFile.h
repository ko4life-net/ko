// ZipBigFile.h: interface for the CZipBigFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CZipBigFile : public CFile {
    DECLARE_DYNAMIC(CZipBigFile)
  public:
    ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
    CZipBigFile();
    virtual ~CZipBigFile();
};
