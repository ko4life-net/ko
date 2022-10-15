// Ini.h: interface for the CIni class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CIni {
  public:
    char m_szFileName[200];
    int  m_nError;
    char m_strError[200];

  public:
    CIni();
    virtual ~CIni();

  public:
    int          GetProfileInt(const char * lpAppName, const char * lpKeyName, int nDefault);
    const char * GetProfileString(const char * lpAppName, const char * lpKeyName, const char * lpDefault);
    int          SetProfileInt(const char * lpAppName, const char * lpKeyName, int nDefault);
    int          SetProfileString(const char * lpAppName, const char * lpKeyName, const char * lpDefault);
    bool         SetPath(const char * filename);
};
