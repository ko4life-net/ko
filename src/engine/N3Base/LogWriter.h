// LogWriter.h: interface for the CLogWriter class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

class CLogWriter {
  protected:
    // static HANDLE      s_hFile;
    static fs::path s_fsLogFile;

  public:
    static void Open(const fs::path & fsFile);
    static void Close();
    static void Write(const char * lpszFormat, ...);

    CLogWriter();
    virtual ~CLogWriter();
};
