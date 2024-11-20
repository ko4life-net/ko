#include "StdAfx.h"
#include "Ini.h"

CIni::CIni() {
    m_Ini.SetUnicode();
    IniFileSet(fs::mktemp_file("config", ".ini"));
    m_bWriteDefaults = true;
}

CIni::CIni(const fs::path & fsFile, bool bWriteDefaults /* = true */) {
    m_Ini.SetUnicode();
    IniFileSet(fsFile);
    m_bWriteDefaults = bWriteDefaults;
}

void CIni::IniFileSet(const fs::path & fsFile) {
    m_fsFile = fsFile.is_absolute() ? fsFile : (n3std::get_app_dir() / fsFile);
    ReloadFile();
}

void CIni::ReloadFile() const {
    if (!fs::exists(m_fsFile)) {
        return;
    }

    std::unique_lock lock(m_mtxIni);

    // Check both last write timestamp and interval to avoid excessive reloads.
    // Defaults written by the app updates the file timestamp, so we also ensure
    // a minimum time interval before reloading.
    auto tpCurrentWriteTime = fs::last_write_time(m_fsFile);
    auto tpNow = std::chrono::steady_clock::now();
    auto tpElapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(tpNow - m_tpLastReload);
    bool bNeedsReload = tpCurrentWriteTime != m_tpLastWriteTime && tpElapsedSeconds >= m_dReloadIntervalSeconds;
    if (!bNeedsReload) {
        return;
    }

    m_Ini.Reset();
    m_Ini.LoadFile(m_fsFile.c_str());
    m_tpLastReload = tpNow;
    m_tpLastWriteTime = tpCurrentWriteTime;
}

void CIni::SetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const {
    SetString(szSection, szKey, bDefault ? "1" : "0");
}

void CIni::SetInt(std::string_view szSection, std::string_view szKey, int iDefault) const {
    SetString(szSection, szKey, std::to_string(iDefault));
}

void CIni::SetFloat(std::string_view szSection, std::string_view szKey, float fDefault) const {
    SetString(szSection, szKey, std::format("{:.04f}", fDefault));
}

void CIni::SetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const {
    std::unique_lock lock(m_mtxIni);
    m_Ini.SetValue(szSection.data(), szKey.data(), szDefault.data());
    m_Ini.SaveFile(m_fsFile.c_str());
}

bool CIni::GetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const {
    return GetInt(szSection, szKey, bDefault ? 1 : 0) ? true : false;
}

int CIni::GetInt(std::string_view szSection, std::string_view szKey, int iDefault) const {
    std::string szValue = GetString(szSection, szKey, std::to_string(iDefault));

    int  iValue = iDefault;
    auto errConv = std::from_chars(szValue.data(), szValue.data() + szValue.size(), iValue);
    if (errConv.ec == std::errc::invalid_argument || errConv.ec == std::errc::result_out_of_range) {
        return iDefault;
    }

    return iValue;
}

float CIni::GetFloat(std::string_view szSection, std::string_view szKey, float fDefault) const {
    std::string szValue = GetString(szSection, szKey, std::format("{:.04f}", fDefault));

    float fValue = fDefault;
    auto  errConv = std::from_chars(szValue.data(), szValue.data() + szValue.size(), fValue);
    if (errConv.ec == std::errc::invalid_argument || errConv.ec == std::errc::result_out_of_range) {
        return fDefault;
    }

    return fValue;
}

std::string CIni::GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const {
    ReloadFile();
    if (m_bWriteDefaults && !m_Ini.KeyExists(szSection.data(), szKey.data())) {
        SetString(szSection, szKey, szDefault);
        return std::string(szDefault);
    }

    std::shared_lock lock(m_mtxIni);
    const char *     szValue = m_Ini.GetValue(szSection.data(), szKey.data(), szDefault.data());
    return szValue ? std::string(szValue) : std::string(szDefault);
}
