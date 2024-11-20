#pragma once

#include "N3Utils.h"

class CIni {
  public:
    CIni();
    CIni(const fs::path & fsFile, bool bWriteDefaults = true);

    const fs::path & IniFile() const { return m_fsFile; }
    void             IniFileSet(const fs::path & fsFile);

    int SetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const;
    int SetInt(std::string_view szSection, std::string_view szKey, int iDefault) const;
    int SetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const;

    bool         GetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const;
    int          GetInt(std::string_view szSection, std::string_view szKey, int iDefault) const;
    std::string  GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const;
    const char * GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault,
                           char * szBuffer, size_t iSize) const;

  private:
    bool Exists(std::string_view szSection, std::string_view szKey) const;

  public:
    bool m_bWriteDefaults; // Writes default configs if a requested key/value not exists.

  private:
    fs::path m_fsFile;
    char     m_szFileRef[260];
};
