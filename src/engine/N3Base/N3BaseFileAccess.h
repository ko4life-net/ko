// N3BaseFileAccess.h: interface for the CN3BaseFileAccess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Base.h"
#include <string>

class CN3BaseFileAccess : public CN3Base {
  private:
    fs::path m_fsFile;
    fs::path m_fsFileAbs;

  public:
    int m_iLOD; // level of details

  public:
    static void                   ToRelative(fs::path & fsFile);
    [[nodiscard]] static fs::path ToRelative(const fs::path & fsFile);

    const fs::path & FilePath() const { return m_fsFile; }
    void             FilePathSet(const fs::path & fsFile);

    const fs::path & FilePathAbs() const { return m_fsFileAbs; };

    // Explictly converts the stored file path to Windows format by replacing '/' with '\'.
    // Although Windows accepts both path separators, using '\' ensures compatibility with
    // the official client's path format when writing paths to files.
    [[nodiscard]] fs::path FilePathWin() const { return fs::path(m_fsFile).normalize('/', '\\'); }

    bool         LoadFromFile();
    virtual bool LoadFromFile(const fs::path & fsFile);
    virtual bool Load(HANDLE hFile);

    virtual bool SaveToFile();
    virtual bool SaveToFile(const fs::path & fsFile);
    virtual bool Save(HANDLE hFile);

  public:
    void Release();

    CN3BaseFileAccess();
    virtual ~CN3BaseFileAccess();
};
