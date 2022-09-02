// LogWriter.h: interface for the CLogWriter class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

class CLogWriter {
  protected:
    //	static HANDLE s_hFile;
    static std::string s_szFileName;

  public:
    static void Open(const std::string & szFN);
    static void Close();
    static void Write(const char * lpszFormat, ...);

    CLogWriter();
    virtual ~CLogWriter();
};
