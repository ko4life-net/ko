// CentralDir.cpp: implementation of the CCentralDir class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CentralDir.h"
#include "ZipArchive.h"

struct CAutoHandle {
    HANDLE m_hFileMap;
    LPVOID m_pFileMap;
    CAutoHandle() {
        m_hFileMap = NULL;
        m_pFileMap = NULL;
    }
    bool CreateMapping(HANDLE hFile) {
        m_hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, "ZipArchive Mapping File");
        if (!m_hFileMap) {
            return false;
        }
        // Get pointer to memory representing file
        m_pFileMap = MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
        return (m_pFileMap != NULL);
    }
    void RemoveMapping() {
        if (m_pFileMap) {
            UnmapViewOfFile(m_pFileMap);
            m_pFileMap = NULL;
        }
        if (m_hFileMap) {
            CloseHandle(m_hFileMap);
            m_hFileMap = NULL;
        }
    }
    ~CAutoHandle() { RemoveMapping(); }
};

#define CENTRALDIRSIZE 22

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
char CCentralDir::m_gszSignature[] = {0x50, 0x4b, 0x05, 0x06};
CCentralDir::CCentralDir() {
    m_pStorage = NULL;
    m_pOpenedFile = NULL;
    m_iBufferSize = 32768;
}

void CCentralDir::Init() {
    m_bOnDisk = false;
    m_uBytesBeforeZip = m_uCentrDirPos = 0;
    m_pOpenedFile = NULL;
    m_szComment.Empty();
}

CCentralDir::~CCentralDir() {
    Clear();
}

void CCentralDir::Read() {
    ASSERT(m_pStorage);

    m_uCentrDirPos = Locate();
    m_pStorage->m_file.Seek(m_uCentrDirPos, CFile::begin);
    CAutoBuffer buf(CENTRALDIRSIZE);
    m_pStorage->m_file.Read(buf, CENTRALDIRSIZE);
    memcpy(&m_szSignature, buf, 4);
    memcpy(&m_uThisDisk, buf + 4, 2);
    memcpy(&m_uDiskWithCD, buf + 6, 2);
    memcpy(&m_uDiskEntriesNo, buf + 8, 2);
    memcpy(&m_uEntriesNumber, buf + 10, 2);
    memcpy(&m_uSize, buf + 12, 4);
    memcpy(&m_uOffset, buf + 16, 4);
    memcpy(&m_uCommentSize, buf + 20, 2);
    buf.Release();

    m_pStorage->UpdateSpanMode(m_uThisDisk);
    // if m_uThisDisk is not zero, it is enough to say that it is multi archive
    ASSERT((!m_uThisDisk && (m_uEntriesNumber == m_uDiskEntriesNo) && !m_uDiskWithCD) || m_uThisDisk);

    if (!m_pStorage->IsSpanMode() && ((DWORD)m_uCentrDirPos < m_uOffset + m_uSize)) {
        ThrowError(ZIP_BADZIPFILE);
    }

    if (m_uCommentSize) {
        m_pStorage->m_file.Read(m_szComment.GetBuffer(m_uCommentSize), m_uCommentSize);
        m_szComment.ReleaseBuffer(m_uCommentSize);
    }

    m_uBytesBeforeZip = m_pStorage->IsSpanMode() ? 0 : m_uCentrDirPos - m_uSize - m_uOffset;

    if ((!m_uSize && m_uEntriesNumber) || (!m_uEntriesNumber && m_uSize)) {
        ThrowError(ZIP_BADZIPFILE);
    }

    m_bOnDisk = true;
    m_pStorage->ChangeDisk(m_uDiskWithCD);

    if (!m_uSize) {
        return;
    }

    ReadHeaders();
}

// return the location of the beginning of the "end" record in the file
DWORD CCentralDir::Locate() {

    // maximum size of end of central dir record
    long  uMaxRecordSize = 0xffff + CENTRALDIRSIZE;
    DWORD uFileSize = m_pStorage->m_file.GetLength();

    if ((DWORD)uMaxRecordSize > uFileSize) {
        uMaxRecordSize = uFileSize;
    }

    CAutoBuffer buf(m_iBufferSize);

    long uPosInFile = 0;
    int  uRead = 0;
    // backward reading
    while (uPosInFile < uMaxRecordSize) {
        uPosInFile = uRead + m_iBufferSize;
        if (uPosInFile > uMaxRecordSize) {
            uPosInFile = uMaxRecordSize;
        }

        int iToRead = uPosInFile - uRead;

        m_pStorage->m_file.Seek(-uPosInFile, CFile::end);
        m_pStorage->m_file.Read(buf, iToRead);
        // search from the very last bytes to prevent an error if inside archive
        // there are packed other arhives
        for (int i = iToRead - 4; i >= 0; i--) {
            if (!memcmp((char *)buf + i, m_gszSignature, 4)) {
                return uFileSize - (uPosInFile - i);
            }
        }

        uRead += iToRead - 3;
    }

    ThrowError(ZIP_BADZIPFILE);
    return 0;
}

void CCentralDir::ThrowError(int err) {
    AfxThrowZipException(err, m_pStorage->m_file.GetFilePath());
}

void CCentralDir::ReadHeaders() {
    m_pStorage->m_file.Seek(m_uOffset + m_uBytesBeforeZip, CFile::begin);

    for (int i = 0; i < m_uEntriesNumber; i++) {
        CFileHeader * pHeader = new CFileHeader;
        m_headers.Add(pHeader); // bezpoœrednio nastêpuje w razie wyj¹tku

        if (!pHeader->Read(m_pStorage)) {
            ThrowError(ZIP_BADZIPFILE);
        }
    }
}

// remove all headers from the central dir
void CCentralDir::Clear() {
    for (int i = 0; i < m_headers.GetSize(); i++) {
        delete m_headers[i];
    }
    m_szComment.Empty();
    m_headers.RemoveAll();
}

bool CCentralDir::IsValidIndex(WORD uIndex) {

    bool ret = uIndex < m_headers.GetSize();
#ifdef _DEBUG
    if (!ret) {
        TRACE("Not a valid index.\n");
    }
#endif
    return ret;
}

// open the file for extracting
void CCentralDir::OpenFile(WORD uIndex) {
    WORD uLocalExtraFieldSize;
    m_pOpenedFile = m_headers[uIndex];
    m_pStorage->ChangeDisk(m_pOpenedFile->m_uDiskStart);
    m_pStorage->m_file.Seek(m_pOpenedFile->m_uOffset + m_uBytesBeforeZip, CFile::begin);
    if (!m_pOpenedFile->ReadLocal(m_pStorage, uLocalExtraFieldSize, m_bCurrDataDescrPresent)) {
        ThrowError(ZIP_BADZIPFILE);
    }

    m_pLocalExtraField.Release(); // just in case
    if (uLocalExtraFieldSize) {
        int iCurrDsk = m_pStorage->GetCurrentDisk();
        m_pLocalExtraField.Allocate(uLocalExtraFieldSize);
        m_pStorage->Read(m_pLocalExtraField, uLocalExtraFieldSize, true);
        if (m_pStorage->GetCurrentDisk() != iCurrDsk) {
            ThrowError(ZIP_BADZIPFILE);
        }
    }
}

void CCentralDir::CloseFile() {
    m_pLocalExtraField.Release();
    if (!m_bCurrDataDescrPresent) {
        return;
    }

    CAutoBuffer buf(12);
    m_pStorage->Read(buf, 4, false);
    // in span mode, files that are divided between disks have bit 3 of flag set
    // which tell about the presence of the data descriptor after the compressed data
    // This signature may be in the disk spanning archive that is one volume only
    // (it is detected as a non disk spanning archive)
    if (memcmp(buf, CZipStorage::m_gszSpanSignature, 4) != 0) { // there is no signature
        m_pStorage->m_file.Seek(-4, CFile::current);
    }

    m_pStorage->Read(buf, 12, false);
    if (!m_pOpenedFile->CheckCrcAndSizes(buf)) {
        ThrowError(ZIP_BADZIPFILE);
    }
    m_bCurrDataDescrPresent = false;
    m_pOpenedFile = NULL;
}

// add new header using the argument as a template
void CCentralDir::AddNewFile(CFileHeader & header) {
    CFileHeader * pHeader = new CFileHeader(header);
    m_pOpenedFile = pHeader;
    m_headers.Add(pHeader);
    RemoveFromDisk();
    m_pStorage->m_file.SeekToEnd();
    // add data descriptor in the disk span archive
    m_bCurrDataDescrPresent = m_pStorage->IsSpanMode() != 0;
}

// called during adding or deleting files; remove the central dir from the disk
void CCentralDir::RemoveFromDisk() {
    if (m_bOnDisk) {
        ASSERT(!m_pStorage
                    ->IsSpanMode()); // you can't add files to the existing disk span archive or to delete them from it
        m_pStorage->m_file.SetLength(m_uBytesBeforeZip + m_uOffset);
        m_bOnDisk = false;
    }
}

void CCentralDir::CloseNewFile() {
    bool        IsSpan = m_pStorage->IsSpanMode() != 0;
    CAutoBuffer buf(12 + 4);
    short       iToWrite = 0;
    if (m_bCurrDataDescrPresent) {
        if (IsSpan) {
            memcpy(buf, m_pStorage->m_gszSpanSignature, 4);
            iToWrite += 4;
        }
    } else /*if (!IsSpan)*/
    {
        m_pStorage->Flush();
        m_pStorage->m_file.Seek(m_pOpenedFile->m_uOffset + 14, CFile::begin);
        // we don't have to restore the pointer, because before adding a new file,
        // the pointer is moved to the end
    }
    m_pOpenedFile->GetCrcAndSizes(buf + iToWrite);
    iToWrite += 12;

    // offset set during writting the local header
    m_pOpenedFile->m_uOffset -= m_uBytesBeforeZip;

    // write the data descriptor and a disk spanning signature at once
    m_pStorage->Write(buf, iToWrite, true);

    if (!m_pStorage->IsSpanMode()) {
        m_pStorage->Flush();
    }

    m_pOpenedFile = NULL;
}

void CCentralDir::Write() {
    if (m_bOnDisk) {
        return;
    }
    if (!m_pStorage->IsSpanMode()) {
        m_pStorage->Flush();
        m_pStorage->m_file.SeekToEnd();
    }
    if (m_szComment.GetLength() > USHRT_MAX) {
        m_szComment = m_szComment.Left(USHRT_MAX);
    }
    m_uCommentSize = (WORD)m_szComment.GetLength();
    m_uEntriesNumber = (WORD)m_headers.GetSize();
    m_uSize = 0;
    bool bDontAllowDiskChange = false;
    // if there is a disk spanning archive in creation and it is only one-volume,
    //    (current disk is 0 so far, no bytes has been written so we know they are
    //  all in the buffer)    make sure that it will be after writting central dir
    // and make it a non disk spanning archive
    if (m_pStorage->IsSpanMode() && !m_pStorage->GetCurrentDisk()) {
        DWORD uVolumeFree = m_pStorage->VolumeLeft();
        // calculate the size of data descriptors already in the buffer or on the disk
        // (they will be removed in the non disk spanning archive):
        // multi span signature at the beginnig (4 bytes) + the size of the data
        // descr. for each file (multi span signature + 12 bytes data)
        // the count of bytes to add: central dir size - total to remove;
        DWORD uToGrow = GetSize(true) - (4 + m_uEntriesNumber * (4 + 12));
        if (uVolumeFree >= uToGrow) // lets make sure it will be one-disk archive
        {
            // can the operation be done only in the buffer?
            if (!m_pStorage->m_iBytesWritten &&             // no bytes on the disk yet
                (m_pStorage->GetFreeInBuffer() >= uToGrow)) // is the buffer big enough?
            {
                RemoveDataDescr(true);
                bDontAllowDiskChange = true; // if the disk change occurs somehow, we'll throw an error later
            } else {
                m_pStorage->Flush();
                m_pStorage->m_file.Flush();
                if (RemoveDataDescr(false)) {
                    bDontAllowDiskChange = true; // if the disk change occurs somehow, we'll throw an error later
                }
            }
        }
    }

    WriteHeaders();
    m_uThisDisk = (WORD)m_pStorage->GetCurrentDisk();
    DWORD uSize = WriteCentralEnd();
    if (bDontAllowDiskChange && (m_pStorage->GetCurrentDisk() != 0)) {
        ThrowError(ZIP_BADZIPFILE);
    }
    // if after adding a central directory there is a disk change,
    // update the information and write it again
    if (m_uThisDisk != m_pStorage->GetCurrentDisk()) {
        m_uThisDisk = (WORD)m_pStorage->GetCurrentDisk();
        if (m_uEntriesNumber) {
            m_uDiskEntriesNo = 0;
        } else {
            m_uDiskWithCD = m_uThisDisk;
            m_uOffset = 0;
        }

        if (m_pStorage->m_uBytesInWriteBuffer >= uSize) {
            // if the data is still in the buffer, simply remove it
            m_pStorage->m_uBytesInWriteBuffer -= uSize;
        } else {
            m_pStorage->Flush();
            m_pStorage->m_iBytesWritten -= uSize;
            m_pStorage->m_file.SeekToBegin();
        }

        WriteCentralEnd();
    }
}

void CCentralDir::WriteHeaders() {
    m_uDiskEntriesNo = 0;
    m_uDiskWithCD = (WORD)m_pStorage->GetCurrentDisk();
    m_uOffset = m_pStorage->GetPosition() - m_uBytesBeforeZip;
    if (!m_uEntriesNumber) {
        return;
    }

    WORD iDisk = m_uDiskWithCD;
    for (int i = 0; i < m_uEntriesNumber; i++) {
        m_uSize += m_headers[i]->Write(m_pStorage);
        if (m_pStorage->GetCurrentDisk() != iDisk) {
            m_uDiskEntriesNo = 1;
            iDisk = (WORD)m_pStorage->GetCurrentDisk();
            // update the information about the offset and starting disk if the
            // first header was written on the new disk
            if (i == 0) {
                m_uOffset = 0;
                m_uDiskWithCD = iDisk;
            }
        } else {
            m_uDiskEntriesNo++;
        }
    }
}

DWORD CCentralDir::WriteCentralEnd() {
    DWORD       uSize = GetSize();
    CAutoBuffer buf(uSize);
    memcpy(buf, m_gszSignature, 4);
    memcpy(buf + 4, &m_uThisDisk, 2);
    memcpy(buf + 6, &m_uDiskWithCD, 2);
    memcpy(buf + 8, &m_uDiskEntriesNo, 2);
    memcpy(buf + 10, &m_uEntriesNumber, 2);
    memcpy(buf + 12, &m_uSize, 4);
    memcpy(buf + 16, &m_uOffset, 4);
    memcpy(buf + 20, &m_uCommentSize, 2);
    memcpy(buf + 22, (LPCTSTR)m_szComment, m_szComment.GetLength());
    m_pStorage->Write(buf, uSize, true);
    return uSize;
}

void CCentralDir::RemoveFile(WORD uIndex) {
    delete m_headers[uIndex];
    m_headers.RemoveAt(uIndex);
}

DWORD CCentralDir::GetSize(bool bWhole) {
    DWORD uHeaders = 0;
    if (bWhole) {
        for (int i = 0; i < m_headers.GetSize(); i++) {
            uHeaders += m_headers[i]->GetSize();
        }
    }
    return CENTRALDIRSIZE + m_szComment.GetLength() + uHeaders;
}

// remove data descriptors from the write buffer in the disk spanning volume
// that is one-disk only
bool CCentralDir::RemoveDataDescr(bool bFromBuffer) {
    CAutoHandle ah;
    char *      pFile = NULL;
    DWORD       uSize;
    if (bFromBuffer) {
        uSize = m_pStorage->m_uBytesInWriteBuffer;
        pFile = m_pStorage->m_pWriteBuffer;
    } else {
        uSize = m_pStorage->m_file.GetLength();
        if (!ah.CreateMapping((HANDLE)m_pStorage->m_file.m_hFile)) {
            return false;
        }
        pFile = (char *)ah.m_pFileMap;
    }

    DWORD uOffsetToChange = 4;
    DWORD uToCopy = 0;
    DWORD uPosInBuffer = 0;
    for (int i = 0; i < m_headers.GetSize(); i++) {
        // update the flag value in the local and central header
        //         int uDataDescr = (m_headers[i]->m_uFlag & 8) ? (4 + 12) : 0;
        CFileHeader * pHeader = m_headers[i];
        pHeader->m_uFlag &= ~8;
        char * pDest = pFile + pHeader->m_uOffset;
        memcpy(pDest + 6, &pHeader->m_uFlag, 2);
        // update crc32 and sizes' values
        pHeader->GetCrcAndSizes(pDest + 14);

        uToCopy =
            (i == (m_headers.GetSize() - 1) ? uSize : m_headers[i + 1]->m_uOffset) - pHeader->m_uOffset - (4 + 12);

        memmove(pFile + uPosInBuffer, pFile + pHeader->m_uOffset, uToCopy);

        uPosInBuffer += uToCopy;
        pHeader->m_uOffset -= uOffsetToChange;
        uOffsetToChange += (4 + 12);
    }
    if (bFromBuffer) {
        m_pStorage->m_uBytesInWriteBuffer = uPosInBuffer;
    } else {
        m_pStorage->m_iBytesWritten = uPosInBuffer;
        ah.RemoveMapping();
        m_pStorage->m_file.SetLength(uPosInBuffer);
    }
    return true;
}
