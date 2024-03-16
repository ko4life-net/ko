// N3BaseFileAccess.h: interface for the CN3BaseFileAccess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3Base.h"
#include <string>

class CN3BaseFileAccess : public CN3Base {
  protected:
    std::string m_szFileName; // Local path excluding Base Path + file name

  public:
    int m_iLOD; // LOD to use when loading

  public:
    const std::string & FileName() const { return m_szFileName; } // Full Path
    void                FileNameSet(const std::string & szFileName);

    bool         LoadFromFile();                               // Read from file.
    virtual bool LoadFromFile(const std::string & szFileName); // Read from file.
    virtual bool Load(HANDLE hFile);                           // Read from handle..

    virtual bool SaveToFile();                               // Save as current file name.
    virtual bool SaveToFile(const std::string & szFileName); // Save with new name.
    virtual bool Save(HANDLE hFile);                         // Save through handle..

  public:
    void Release();

    CN3BaseFileAccess();
    virtual ~CN3BaseFileAccess();
};
