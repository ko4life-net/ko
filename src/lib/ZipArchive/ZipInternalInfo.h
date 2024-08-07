// ZipInternalInfo.h: interface for the CZipInternalInfo class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ZipAutoBuffer.h"
#include <zlib.h>

class CZipInternalInfo {
  public:
    DWORD          m_iBufferSize;
    z_stream       m_stream;
    DWORD          m_uUncomprLeft;
    DWORD          m_uComprLeft;
    DWORD          m_uCrc32;
    void           Init();
    CZipAutoBuffer m_pBuffer;
    CZipInternalInfo();
    virtual ~CZipInternalInfo();
};
