// FileHeader.cpp: implementation of the CFileHeader class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "FileHeader.h"
#include "AutoBuffer.h"

#include <zlib.h>

#define FILEHEADERSIZE      46
#define LOCALFILEHEADERSIZE 30
#define VERSIONMADEBY       20
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char CFileHeader::m_gszSignature[] = {0x50, 0x4b, 0x01, 0x02};
char CFileHeader::m_gszLocalSignature[] = {0x50, 0x4b, 0x03, 0x04};
CFileHeader::CFileHeader() {
    m_uExternalAttr = FILE_ATTRIBUTE_ARCHIVE;
    m_uModDate = m_uModTime = m_uInternalAttr = 0;
    m_uMethod = Z_DEFLATED;
}

CFileHeader::~CFileHeader() {}

// read the header from the central dir
bool CFileHeader::Read(CZipStorage * pStorage) {
    CAutoBuffer buf(FILEHEADERSIZE);
    pStorage->Read(buf, FILEHEADERSIZE, true);
    memcpy(&m_szSignature, buf, 4);
    memcpy(&m_uVersionMadeBy, buf + 4, 2);
    memcpy(&m_uVersionNeeded, buf + 6, 2);
    memcpy(&m_uFlag, buf + 8, 2);
    memcpy(&m_uMethod, buf + 10, 2);
    memcpy(&m_uModTime, buf + 12, 2);
    memcpy(&m_uModDate, buf + 14, 2);
    memcpy(&m_uCrc32, buf + 16, 4);
    memcpy(&m_uComprSize, buf + 20, 4);
    memcpy(&m_uUncomprSize, buf + 24, 4);
    memcpy(&m_uFileNameSize, buf + 28, 2);
    memcpy(&m_uExtraFieldSize, buf + 30, 2);
    memcpy(&m_uCommentSize, buf + 32, 2);
    memcpy(&m_uDiskStart, buf + 34, 2);
    memcpy(&m_uInternalAttr, buf + 36, 2);
    memcpy(&m_uExternalAttr, buf + 38, 4);
    memcpy(&m_uOffset, buf + 42, 4);
    buf.Release();

    if (memcmp(m_szSignature, m_gszSignature, 4) != 0) {
        return false;
    }

    int iCurDsk = pStorage->GetCurrentDisk();
    pStorage->m_file.Read(m_szFileName.GetBuffer(m_uFileNameSize), m_uFileNameSize);
    m_szFileName.ReleaseBuffer(m_uFileNameSize);
    if (m_uExtraFieldSize) {
        ASSERT(!m_pExtraField.IsAllocated());
        m_pExtraField.Allocate(m_uExtraFieldSize);
        pStorage->m_file.Read(m_pExtraField, m_uExtraFieldSize);
    }
    if (m_uCommentSize) {
        pStorage->m_file.Read(m_szComment.GetBuffer(m_uCommentSize), m_uCommentSize);
        m_szComment.ReleaseBuffer(m_uCommentSize);
    }

    return pStorage->GetCurrentDisk() == iCurDsk; // check that the while header is on the one disk
}

// return CTime representation of m_uModDate, m_uModTime
CTime CFileHeader::GetTime() {
    return CTime(m_uModDate, m_uModTime);
}

// write the header to the central dir
DWORD CFileHeader::Write(CZipStorage * pStorage) {
    DWORD       iSize = GetSize();
    CAutoBuffer buf(iSize);
    memcpy(buf, &m_szSignature, 4);
    memcpy(buf + 4, &m_uVersionMadeBy, 2);
    memcpy(buf + 6, &m_uVersionNeeded, 2);
    memcpy(buf + 8, &m_uFlag, 2);
    memcpy(buf + 10, &m_uMethod, 2);
    memcpy(buf + 12, &m_uModTime, 2);
    memcpy(buf + 14, &m_uModDate, 2);
    memcpy(buf + 16, &m_uCrc32, 4);
    memcpy(buf + 20, &m_uComprSize, 4);
    memcpy(buf + 24, &m_uUncomprSize, 4);
    memcpy(buf + 28, &m_uFileNameSize, 2);
    memcpy(buf + 30, &m_uExtraFieldSize, 2);
    memcpy(buf + 32, &m_uCommentSize, 2);
    memcpy(buf + 34, &m_uDiskStart, 2);
    memcpy(buf + 36, &m_uInternalAttr, 2);
    memcpy(buf + 38, &m_uExternalAttr, 4);
    memcpy(buf + 42, &m_uOffset, 4);

    ASSERT(m_uFileNameSize == m_szFileName.GetLength());
    ASSERT(m_uExtraFieldSize == m_pExtraField.GetSize());
    ASSERT(m_uCommentSize == m_szComment.GetLength());
    memcpy(buf + 46, (LPCTSTR)m_szFileName, m_uFileNameSize);

    if (m_uExtraFieldSize) {
        memcpy(buf + 46 + m_uFileNameSize, m_pExtraField, m_uExtraFieldSize);
    }

    if (m_uCommentSize) {
        memcpy(buf + 46 + m_uFileNameSize + m_uExtraFieldSize, (LPCTSTR)m_szComment, m_uCommentSize);
    }

    pStorage->Write(buf, iSize, true);
    return iSize;
}

// read local header
bool CFileHeader::ReadLocal(CZipStorage * pStorage, WORD & iLocExtrFieldSize, bool & bIsDataDescr) {
    char buf[LOCALFILEHEADERSIZE];
    pStorage->Read(buf, LOCALFILEHEADERSIZE, true);
    if (memcmp(buf, m_gszLocalSignature, 4) != 0) {
        return false;
    }

    // data descriptor present
    bIsDataDescr = (((WORD) * (buf + 6)) & 8) != 0;

    if ((memcmp(buf + 8, &m_uMethod, 2) != 0) || (m_uMethod && (m_uMethod != Z_DEFLATED)) ||
        (memcmp(buf + 26, &m_uFileNameSize, 2) != 0)) {
        return false;
    }

    // jeszcze mo¿naby porównaæ nazwy plików

    if (!bIsDataDescr /* || !pStorage->IsSpanMode()*/) {
        if (!CheckCrcAndSizes(buf + 14)) {
            return false;
        }
    }

    iLocExtrFieldSize = (WORD) * (buf + 28);
    pStorage->m_file.Seek(m_uFileNameSize, CFile::current);

    return true;
}

// set the m_uModDate, m_uModTime values using CTime object
void CFileHeader::SetTime(const CTime & time) {
    WORD year = (WORD)time.GetYear();
    if (year <= 1980) {
        year = 0;
    } else {
        year -= 1980;
    }
    m_uModDate = (WORD)(time.GetDay() + (time.GetMonth() << 5) + (year << 9));
    m_uModTime = (WORD)((time.GetSecond() >> 1) + (time.GetMinute() << 5) + (time.GetHour() << 11));
}
//    the buffer contains crc32, compressed and uncompressed sizes to be compared
//    with the actual values
bool CFileHeader::CheckCrcAndSizes(char * pBuf) {
    return (memcmp(pBuf, &m_uCrc32, 4) == 0) && (memcmp(pBuf + 4, &m_uComprSize, 4) == 0) &&
           (memcmp(pBuf + 8, &m_uUncomprSize, 4) == 0);
}

// write the local header
void CFileHeader::WriteLocal(CZipStorage & storage) {
    // extra field is local by now
    DWORD       iLocalSize = LOCALFILEHEADERSIZE + m_uExtraFieldSize + m_uFileNameSize;
    CAutoBuffer buf(iLocalSize);
    memcpy(buf, m_gszLocalSignature, 4);
    memcpy(buf + 4, &m_uVersionNeeded, 2);
    memcpy(buf + 6, &m_uFlag, 2);
    memcpy(buf + 8, &m_uMethod, 2);
    memcpy(buf + 10, &m_uModTime, 2);
    memcpy(buf + 12, &m_uModDate, 2);
    memcpy(buf + 14, &m_uCrc32, 4);
    memcpy(buf + 18, &m_uComprSize, 4);
    memcpy(buf + 22, &m_uUncomprSize, 4);
    memcpy(buf + 26, &m_uFileNameSize, 2);
    memcpy(buf + 28, &m_uExtraFieldSize, 2);
    memcpy(buf + 30, (LPCTSTR)m_szFileName, m_uFileNameSize);
    memcpy(buf + 30 + m_uFileNameSize, m_pExtraField, m_uExtraFieldSize);

    // possible disk change before writting to the file in the disk spanning mode
    // so write the local header first
    storage.Write(buf, iLocalSize, true);
    // it was only local information, use CZipArchive::SetExtraField to set global extra field
    m_pExtraField.Release();
    m_uExtraFieldSize = 0;

    m_uDiskStart = (WORD)storage.GetCurrentDisk();
    m_uOffset = storage.GetPosition() - iLocalSize;
}

// prepare the data before adding a new file
bool CFileHeader::PrepareData(int iLevel, bool bIsSpanMode) {
    memcpy(m_szSignature, m_gszSignature, 4);
    m_uVersionMadeBy = VERSIONMADEBY;
    m_uVersionNeeded = 20;

    if ((m_uMethod != Z_DEFLATED) && (m_uMethod != 0)) {
        m_uMethod = Z_DEFLATED;
    }

    m_uFlag = 0;
    if (m_uMethod == Z_DEFLATED) {
        switch (iLevel) {
        case 1:
            m_uFlag |= 6;
            break;
        case 2:
            m_uFlag |= 4;
            break;
        case 8:
        case 9:
            m_uFlag |= 2;
            break;
        }
    }

    if (bIsSpanMode) {
        m_uFlag |= 8; // data descriptor present
    }

    m_uCrc32 = 0;
    m_uComprSize = 0;
    m_uUncomprSize = 0;

    bool bRet = true;
    if (m_szFileName.GetLength() > USHRT_MAX) {
        bRet = false;
    }
    m_uFileNameSize = (WORD)m_szFileName.GetLength();
    m_uExtraFieldSize = (m_pExtraField.GetSize() > USHRT_MAX) ? (WORD)USHRT_MAX : (WORD)m_pExtraField.GetSize();
    ValidateComment();
    return bRet;
}

// fill the buffer with the current values
void CFileHeader::GetCrcAndSizes(char * pBuffer) {
    memcpy(pBuffer, &m_uCrc32, 4);
    memcpy(pBuffer + 4, &m_uComprSize, 4);
    memcpy(pBuffer + 8, &m_uUncomprSize, 4);
}

DWORD CFileHeader::GetSize() {
    return FILEHEADERSIZE + m_uExtraFieldSize + m_uFileNameSize + m_uCommentSize;
}

void CFileHeader::ValidateComment() {
    if (m_szComment.GetLength() > USHRT_MAX) {
        m_szComment = m_szComment.Left(USHRT_MAX);
    }
    m_uCommentSize = (WORD)m_szComment.GetLength();
}
