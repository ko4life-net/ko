// N3BaseFileAccess.cpp: implementation of the CN3BaseFileAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3BaseFileAccess.h"
#include <vector>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3BaseFileAccess::CN3BaseFileAccess() {
    m_dwType |= OBJ_BASE_FILEACCESS;
    m_iLOD = 0;
}

CN3BaseFileAccess::~CN3BaseFileAccess() {}

void CN3BaseFileAccess::Release() {
    m_fsFile = fs::path();
    m_fsFileAbs = fs::path();
    m_iLOD = 0;
    CN3Base::Release();
}

void CN3BaseFileAccess::ToRelative(fs::path & fsFile) {
    fsFile.make_lower().make_relative(CN3Base::PathGet());
}

fs::path CN3BaseFileAccess::ToRelative(const fs::path & fsFile) {
    fs::path fsFileCopy(fsFile);
    ToRelative(fsFileCopy);
    return fsFileCopy;
}

void CN3BaseFileAccess::FilePathSet(const fs::path & fsFile) {
    m_fsFile = fsFile.lower().generic_string();
    if (m_fsFile.is_absolute()) {
        m_fsFileAbs = m_fsFile;
        m_fsFile.make_relative(CN3Base::PathGet());
    } else {
        m_fsFileAbs = CN3Base::PathGet() / m_fsFile;
    }
}

bool CN3BaseFileAccess::Load(HANDLE hFile) {
    m_szName.clear();

    DWORD dwRWC = 0;
    int   iLen = 0;
    ReadFile(hFile, &iLen, 4, &dwRWC, NULL);
    if (iLen > 0) {
        m_szName.assign(iLen, '\0');
        ReadFile(hFile, m_szName.data(), iLen, &dwRWC, NULL);
    }

    return true;
}

bool CN3BaseFileAccess::LoadFromFile() {
    if (FilePathAbs().empty()) {
#ifdef _N3GAME
        CLogWriter::Write("Can't open file (read)");
#endif
        return false;
    }

    HANDLE hFile =
        ::CreateFileW(FilePathAbs().c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        std::string szErr = std::format("{:s} - Can't open file (read)", FilePathAbs());
#ifdef _N3TOOL
        MessageBox(::GetActiveWindow(), szErr.c_str(), "File Handle error", MB_OK);
#endif
#ifdef _N3GAME
        CLogWriter::Write(szErr);
#endif
        return false;
    }

    bool bSuccess = Load(hFile);
    CloseHandle(hFile);
    return bSuccess;
}

bool CN3BaseFileAccess::LoadFromFile(const fs::path & fsFile) {
    FilePathSet(fsFile);
    return LoadFromFile();
}

bool CN3BaseFileAccess::SaveToFile() {
    if (FilePathAbs().empty()) {
        std::string szErr = m_szName + " Can't open file (write) - NULL String";
        MessageBox(::GetActiveWindow(), szErr.c_str(), "File Open Error", MB_OK);
        return false;
    }

    HANDLE hFile =
        ::CreateFileW(FilePathAbs().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::string szErr = std::format("{:s} - Can't open file(write)", FilePathAbs());
        MessageBox(::GetActiveWindow(), szErr.c_str(), "File Handle error", MB_OK);
        return false;
    }

    bool bSuccess = Save(hFile);
    CloseHandle(hFile);
    return bSuccess;
}

bool CN3BaseFileAccess::SaveToFile(const fs::path & fsFile) {
    FilePathSet(fsFile);
    return SaveToFile();
}

bool CN3BaseFileAccess::Save(HANDLE hFile) {
    DWORD dwRWC = 0;

    int nL = m_szName.length();
    WriteFile(hFile, &nL, 4, &dwRWC, NULL);
    if (nL > 0) {
        WriteFile(hFile, m_szName.c_str(), nL, &dwRWC, NULL);
    }

    return true;
}
