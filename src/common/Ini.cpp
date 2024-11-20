#include "StdAfx.h"
#include "Ini.h"

CIni::CIni() {
    IniFileSet(fs::mktemp_file("config", ".ini"));
    m_bWriteDefaults = true;
}

CIni::CIni(const fs::path & fsFile, bool bWriteDefaults /* = true*/) {
    IniFileSet(fsFile);
    m_bWriteDefaults = bWriteDefaults;
}

void CIni::IniFileSet(const fs::path & fsFile) {
    m_fsFile = fsFile.is_absolute() ? fsFile : (n3std::get_app_dir() / fsFile);

    memset(m_szFileRef, 0, sizeof(m_szFileRef));
    m_fsFile.string().copy(m_szFileRef, sizeof(m_szFileRef) - 1);
}

int CIni::SetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const {
    return SetInt(szSection, szKey, bDefault ? 1 : 0);
}

int CIni::SetInt(std::string_view szSection, std::string_view szKey, int iDefault) const {
    std::string szDefault = std::format("{:d}", iDefault);
    return WritePrivateProfileString(szSection.data(), szKey.data(), szDefault.c_str(), m_szFileRef);
}

int CIni::SetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const {
    return WritePrivateProfileString(szSection.data(), szKey.data(), szDefault.data(), m_szFileRef);
}

bool CIni::GetBool(std::string_view szSection, std::string_view szKey, bool bDefault) const {
    return GetInt(szSection.data(), szKey.data(), bDefault ? 1 : 0) ? true : false;
}

int CIni::GetInt(std::string_view szSection, std::string_view szKey, int iDefault) const {
    int iRet = iDefault;
    if (m_bWriteDefaults && !Exists(szSection, szKey)) {
        std::string szDefault = std::format("{:d}", iDefault);
        WritePrivateProfileString(szSection.data(), szKey.data(), szDefault.c_str(), m_szFileRef);
    } else {
        iRet = GetPrivateProfileInt(szSection.data(), szKey.data(), iDefault, m_szFileRef);
    }

    return iRet;
}

std::string CIni::GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault) const {
    std::string szBuff(szDefault.length() + 1000, '\0'); // + 1000 to make the buffer big enough for most cases
    GetString(szSection, szKey, szDefault, szBuff.data(), szBuff.length());
    szBuff.erase(szBuff.find('\0'));
    return szBuff;
}

const char * CIni::GetString(std::string_view szSection, std::string_view szKey, std::string_view szDefault,
                             char * szBuffer, size_t iSize) const {
    if (m_bWriteDefaults && !Exists(szSection, szKey)) {
        WritePrivateProfileString(szSection.data(), szKey.data(), szDefault.data(), m_szFileRef);
        snprintf(szBuffer, iSize, "%s", szDefault.data());
    } else {
        GetPrivateProfileString(szSection.data(), szKey.data(), szDefault.data(), szBuffer, (DWORD)iSize, m_szFileRef);
    }

    return szBuffer;
}

bool CIni::Exists(std::string_view szSection, std::string_view szKey) const {
    char             szBuff[10]{};
    std::string_view szDefault = "__UNSET__";
    GetPrivateProfileString(szSection.data(), szKey.data(), szDefault.data(), szBuff, sizeof(szBuff), m_szFileRef);
    return szDefault != szBuff;
}
