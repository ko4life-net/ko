/**
* \file ZipAutoBuffer.h
* Interface for the CZipAutoBuffer class.
*
* \author Tadeusz Dracz
*/
// Part of the ZipArchive library
//
// Copyright (C) 2000 - 2001 Tadeusz Dracz.
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// For the licensing details see the file License.txt
////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
    A smart buffer freeing its contents on destruction.
*/
class CZipAutoBuffer {
  public:
    operator char *() { return m_pBuffer; }

    // may produce ambiguity on some compilers
    //      operator const char*() const
    //       {
    //            return m_pBuffer;
    //     }
    const char * GetBuffer() const { return m_pBuffer; }
    char *       Allocate(DWORD iSize, bool bZeroMemory = false);
    void         Release();
    DWORD        GetSize() const { return m_iSize; }
    bool         IsAllocated() const { return (m_pBuffer != NULL); }
    CZipAutoBuffer(DWORD iSize, bool bZeroMemory = false);
    CZipAutoBuffer();
    CZipAutoBuffer(const CZipAutoBuffer & buffer);
    virtual ~CZipAutoBuffer();
    CZipAutoBuffer & operator=(const CZipAutoBuffer & buffer);

  protected:
    char * m_pBuffer;
    DWORD  m_iSize;
};
