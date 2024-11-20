#pragma once

#include "N3Utils.h"

#include <simpleini/SimpleIni.h>
#include <shared_mutex>

class CIni {
  public:
    CIni();
    CIni(const fs::path & fsFile, bool bWriteDefaults = true);

    const fs::path & IniFile() const { return m_fsFile; }
    void             IniFileSet(const fs::path & fsFile);

    void SetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const;
    void SetInt(std::string_view szSection, std::string_view szKey, int iDefault) const;
    void SetFloat(std::string_view szSection, std::string_view szKey, float fDefault) const;
    void SetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const;

    bool        GetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const;
    int         GetInt(std::string_view szSection, std::string_view szKey, int iDefault) const;
    float       GetFloat(std::string_view szSection, std::string_view szKey, float fDefault) const;
    std::string GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const;

  private:
    void ReloadFile() const;

  public:
    bool m_bWriteDefaults; // Writes defaults if a requested key does not exists.

  private:
    fs::path m_fsFile;

    mutable CSimpleIniA       m_Ini;
    mutable std::shared_mutex m_mtxIni;

    mutable fs::file_time_type                    m_tpLastWriteTime;
    mutable std::chrono::steady_clock::time_point m_tpLastReload;              // Last reload timestamp
    const std::chrono::seconds                    m_dReloadIntervalSeconds{3}; // Hot reload interval
};
