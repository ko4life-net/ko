// ZipArchive.cpp: implementation of the CZipArchive class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ZipArchive.h"
#include <direct.h>
#include <stdlib.h> // for qsort

#ifndef DEF_MEM_LEVEL
#if MAX_MEM_LEVEL >= 8
#define DEF_MEM_LEVEL 8
#else
#define DEF_MEM_LEVEL MAX_MEM_LEVEL
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char CZipArchive::m_gszCopyright[] = {"Zip archive creation and modification Copyright 2000 Tadeusz Dracz"};
CZipArchive::CZipArchive() {
    m_bSlashChange = m_bDetectZlibMemoryLeaks = m_bOemCompatible = true;
    m_centralDir.m_pStorage = &m_storage;
    m_info.m_stream.zalloc = (alloc_func)myalloc;
    m_info.m_stream.zfree = (free_func)myfree;
}

CZipArchive::~CZipArchive() {
    //     Close(); // cannot be here: if an exception is thrown strange things can happen

    if (m_list.GetCount()) {
        // if some memory hasn't been freed due to an error in zlib, so free it now
        POSITION pos = m_list.GetHeadPosition();
        while (pos) {
            delete[] m_list.GetNext(pos);
        }
    }
}

void CZipArchive::Open(LPCTSTR szPathName, int iMode, int iVolumeSize) {
    if (!IsClosed()) {
        TRACE("ZipArchive already opened.\n");
        return;
    }
    m_iFileOpened = nothing;
    m_storage.Open(szPathName, iMode, iVolumeSize);
    m_centralDir.Init();
    if ((iMode == open) || (iMode == openReadOnly)) {
        m_centralDir.Read();
    }
}

bool CZipArchive::IsClosed(bool bArchive) {
    return bArchive ? (m_storage.GetCurrentDisk() == -1) : (m_storage.m_file.m_hFile == CFile::hFileNull);
}

void CZipArchive::ThrowError(int err) {
    AfxThrowZipException(err, IsClosed() ? "" : m_storage.m_file.GetFilePath().GetString());
}

bool CZipArchive::DeleteFile(WORD uIndex) {
    if (m_storage.IsSpanMode()) {
        TRACE("You cannot delete files from the disk spannig archive.\n");
        return false;
    }

    if (m_iFileOpened) {
        TRACE("You cannot delete files if there is a file opened.\n");
        return false;
    }

    if (!m_centralDir.IsValidIndex(uIndex)) {
        return false;
    }

    m_info.Init();
    m_centralDir.RemoveFromDisk();
    DeleteInternal(uIndex);
    m_info.m_pBuffer.Release();
    return true;
}

int CZipArchive::GetNoEntries() {
    return m_centralDir.m_headers.GetSize();
}

bool CZipArchive::GetFileInfo(CFileHeader & fhInfo, WORD uIndex) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }

    if (!m_centralDir.IsValidIndex(uIndex)) {
        return false;
    }

    fhInfo = *(m_centralDir.m_headers[uIndex]);
    if (m_bOemCompatible) {
        fhInfo.m_szFileName.OemToAnsi();
    }
    if (m_bSlashChange) {
        fhInfo.m_szFileName.Replace('/', '\\');
    }
    return true;
}

int CZipArchive::FindFile(CString szFileName, bool bCaseSensitive) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return (int)-1;
    }

    for (WORD i = 0; i < GetNoEntries(); i++) {
        CFileHeader fh;
        GetFileInfo(fh, i);
        CString temp = fh.m_szFileName;
        if ((bCaseSensitive ? temp.Collate(szFileName) : temp.CollateNoCase(szFileName)) == 0) {
            return (int)i;
        }
    }
    return (int)-1;
}

bool CZipArchive::OpenFile(WORD uIndex) {
    if (!m_centralDir.IsValidIndex(uIndex)) {
        return false;
    }

    if (m_storage.IsSpanMode() == 1) {
        TRACE("You cannot extract from the span in creation.\n");
        return false;
    }

    if (m_iFileOpened) {
        TRACE("A file already opened.\n");
        return false;
    }

    m_info.Init();
    m_centralDir.OpenFile(uIndex);

    WORD uMethod = CurrentFile()->m_uMethod;
    if ((uMethod != 0) && (uMethod != Z_DEFLATED)) {
        ThrowError(ZIP_BADZIPFILE);
    }

    if (uMethod == Z_DEFLATED) {
        m_info.m_stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;
        int err = inflateInit2(&m_info.m_stream, -MAX_WBITS);
        //            * windowBits is passed < 0 to tell that there is no zlib header.
        //          * Note that in this case inflate *requires* an extra "dummy" byte
        //          * after the compressed stream in order to complete decompression and
        //          * return Z_STREAM_END.
        CheckForError(err);
    }
    m_info.m_uComprLeft = CurrentFile()->m_uComprSize;
    m_info.m_uUncomprLeft = CurrentFile()->m_uUncomprSize;
    m_info.m_uCrc32 = 0;
    m_info.m_stream.total_out = 0;
    m_info.m_stream.avail_in = 0;

    m_iFileOpened = extract;
    return true;
}

int CZipArchive::GetLocalExtraField(char * pBuf, int iSize) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }

    if (m_iFileOpened != extract) {
        TRACE("A file must be opened to get the local extra field.\n");
        return -1;
    }

    int size = m_centralDir.m_pLocalExtraField.GetSize();
    if (!pBuf || !size) {
        return size;
    }

    if (iSize < size) {
        size = iSize;
    }

    memcpy(pBuf, m_centralDir.m_pLocalExtraField, size);
    return size;
}

void * CZipArchive::myalloc(void * opaque, UINT items, UINT size) {
    void * p = new char[size * items];
    if (opaque) {
        CPtrList * list = (CPtrList *)opaque;
        list->AddTail(p);
    }
    return p;
}

void CZipArchive::myfree(void * opaque, void * address) {
    if (opaque) {
        CPtrList * list = (CPtrList *)opaque;
        POSITION   pos = list->Find(address);
        if (pos) {
            list->RemoveAt(pos);
        }
    }
    delete[] address;
}

void CZipArchive::CheckForError(int iErr) {
    if ((iErr == Z_OK) || (iErr == Z_NEED_DICT)) {
        return;
    }

    ThrowError(iErr);
}

CFileHeader * CZipArchive::CurrentFile() {
    ASSERT(m_centralDir.m_pOpenedFile);
    return m_centralDir.m_pOpenedFile;
}

DWORD CZipArchive::ReadFile(void * pBuf, DWORD iSize) {
    if (m_iFileOpened != extract) {
        TRACE("Current file must be opened.\n");
        return 0;
    }

    if (!pBuf || !iSize) {
        return 0;
    }

    m_info.m_stream.next_out = (Bytef *)pBuf;
    m_info.m_stream.avail_out = iSize > m_info.m_uUncomprLeft ? m_info.m_uUncomprLeft : iSize;

    DWORD iRead = 0;

    while (m_info.m_stream.avail_out > 0) {
        if ((m_info.m_stream.avail_in == 0) && (m_info.m_uComprLeft > 0)) {
            DWORD uToRead = m_info.m_pBuffer.GetSize();
            if (m_info.m_uComprLeft < uToRead) {
                uToRead = m_info.m_uComprLeft;
            }

            if (uToRead == 0) {
                return 0;
            }

            m_storage.Read(m_info.m_pBuffer, uToRead, false);
            m_info.m_uComprLeft -= uToRead;

            m_info.m_stream.next_in = (unsigned char *)(char *)m_info.m_pBuffer;
            m_info.m_stream.avail_in = uToRead;
        }

        if (CurrentFile()->m_uMethod == 0) {
            DWORD uToCopy = m_info.m_stream.avail_out < m_info.m_stream.avail_in ? m_info.m_stream.avail_out
                                                                                 : m_info.m_stream.avail_in;

            memcpy(m_info.m_stream.next_out, m_info.m_stream.next_in, uToCopy);

            m_info.m_uCrc32 = crc32(m_info.m_uCrc32, m_info.m_stream.next_out, uToCopy);

            m_info.m_uUncomprLeft -= uToCopy;
            m_info.m_stream.avail_in -= uToCopy;
            m_info.m_stream.avail_out -= uToCopy;
            m_info.m_stream.next_out += uToCopy;
            m_info.m_stream.next_in += uToCopy;
            m_info.m_stream.total_out += uToCopy;
            iRead += uToCopy;
        } else {
            DWORD   uTotal = m_info.m_stream.total_out;
            Bytef * pOldBuf = m_info.m_stream.next_out;
            int     err = inflate(&m_info.m_stream, Z_SYNC_FLUSH);
            DWORD   uToCopy = m_info.m_stream.total_out - uTotal;

            m_info.m_uCrc32 = crc32(m_info.m_uCrc32, pOldBuf, uToCopy);

            m_info.m_uUncomprLeft -= uToCopy;
            iRead += uToCopy;

            if (err == Z_STREAM_END) {
                return iRead;
            }

            CheckForError(err);
        }
    }

    return iRead;
}

void CZipArchive::Close(bool bAfterException) {
    if (IsClosed()) {
        TRACE("ZipArchive is already closed.\n");
        return;
    }

    if (!bAfterException) {
        if (m_iFileOpened == extract) {
            CloseFile(NULL);
        }

        if (m_iFileOpened == compress) {
            CloseNewFile();
        }

        m_centralDir.Write();
    }
    m_centralDir.Clear();

    m_storage.Close(bAfterException);
}

void CZipArchive::SetSpanCallback(CHANGEDISKFUNC pFunc, void * pData) {
    m_storage.m_pChangeDiskFunc = pFunc;
    m_storage.m_pCallbackData = pData;
}

void CZipArchive::SetAdvanced(int iWriteBuffer, int iExtractBuffer, int iSearchBuffer) {
    if (!IsClosed()) {
        TRACE("Set this options before opening the archive.\n");
        return;
    }

    m_storage.m_iWriteBufferSize = iWriteBuffer < 1024 ? 1024 : iWriteBuffer;
    m_centralDir.m_iBufferSize = iSearchBuffer < 1024 ? 1024 : iSearchBuffer;
    m_info.m_iBufferSize = iExtractBuffer < 1024 ? 1024 : iExtractBuffer;
}

bool CZipArchive::CloseFile(CFile & file) {
    CString temp = file.GetFilePath();
    file.Close();
    return CloseFile(temp);
}

bool CZipArchive::CloseFile(LPCTSTR lpszFilePath) {
    if (m_iFileOpened != extract) {
        TRACE("No file opened.\n");
        return false;
    }

    if (m_info.m_uUncomprLeft == 0) {
        if (m_info.m_uCrc32 != CurrentFile()->m_uCrc32) {
            ThrowError(ZIP_BADCRC);
        }
    }

    if (CurrentFile()->m_uMethod == Z_DEFLATED) {
        inflateEnd(&m_info.m_stream);
    }

    bool bRet = true;
    if (lpszFilePath) {
        try {
            CFileStatus fs;
            fs.m_ctime = fs.m_atime = CTime::GetCurrentTime();
            fs.m_attribute = 0;
            fs.m_mtime = CurrentFile()->GetTime();
            CFile::SetStatus(lpszFilePath, fs);
            bRet = SetFileAttributes(lpszFilePath, CurrentFile()->m_uExternalAttr) != 0;
        } catch (CException * e) {
            e->Delete();
            return false;
        }
    }
    m_centralDir.CloseFile();
    m_iFileOpened = nothing;
    m_info.m_pBuffer.Release();
    return bRet;
}

bool CZipArchive::OpenNewFile(CFileHeader & header, int iLevel, const char * lpszFilePath) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }

    if (m_iFileOpened) {
        TRACE("A file already opened.\n");
        return false;
    }

    if (m_storage.IsSpanMode() == -1) {
        TRACE("You cannot add the files to the existing disk spannig archive.\n");
        return false;
    }

    if (GetNoEntries() == (WORD)USHRT_MAX) {
        TRACE("Maximum file count inside archive reached.\n");
        return false;
    }

    bool bRet = true;
    if (lpszFilePath) {
        CFileStatus fs;
        if (!CFile::GetStatus(lpszFilePath, fs)) {
            bRet = false;
        } else {
            header.SetTime(fs.m_mtime);
            header.m_uExternalAttr = ::GetFileAttributes(lpszFilePath); // mfc bug: m_attribute is 1-byte
            if (header.m_uExternalAttr == -1) {
                header.m_uExternalAttr = FILE_ATTRIBUTE_ARCHIVE;
                bRet = false;
            }
        }
    }
    m_info.Init();

    m_centralDir.AddNewFile(header);
    CString * pszFileName = &(CurrentFile()->m_szFileName);
    if (m_bSlashChange) {
        pszFileName->Replace('\\', '/');
    }
    if (m_bOemCompatible) {
        pszFileName->AnsiToOem();
    }

    if (pszFileName->IsEmpty()) {
        pszFileName->Format("file%i", GetNoEntries());
    }

    if (!CurrentFile()->PrepareData(iLevel, m_storage.IsSpanMode() == 1)) {
        ThrowError(ZIP_TOOLONGFILENAME);
    }
    CurrentFile()->WriteLocal(m_storage);

    m_info.m_uComprLeft = 0;
    m_info.m_stream.avail_in = (uInt)0;
    m_info.m_stream.avail_out = (uInt)m_info.m_pBuffer.GetSize();
    m_info.m_stream.next_out = (unsigned char *)(char *)m_info.m_pBuffer;
    m_info.m_stream.total_in = 0;
    m_info.m_stream.total_out = 0;

    if (IsDirectory(CurrentFile()->m_uExternalAttr) && (CurrentFile()->m_uMethod != 0)) {
        CurrentFile()->m_uMethod = 0;
    }

    if (CurrentFile()->m_uMethod == Z_DEFLATED) {
        m_info.m_stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;

        int err = deflateInit2(&m_info.m_stream, iLevel, Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);

        CheckForError(err);
    }
    m_iFileOpened = compress;
    return bRet;
}

CString CZipArchive::GetFilePath(LPCTSTR lpszFilePath) {
    TCHAR szDir[_MAX_DIR];
    TCHAR szDrive[_MAX_DRIVE];
    _tsplitpath(lpszFilePath, szDrive, szDir, NULL, NULL);
    return CString(szDrive) + CString(szDir);
}

CString CZipArchive::GetFileExt(LPCTSTR lpszFilePath) {
    TCHAR szExt[_MAX_EXT];
    _tsplitpath(lpszFilePath, NULL, NULL, NULL, szExt);
    return CString(szExt);
}

CString CZipArchive::GetFileTitle(LPCTSTR lpszFilePath) {
    TCHAR szFname[_MAX_FNAME];
    _tsplitpath(lpszFilePath, NULL, NULL, szFname, NULL);
    return CString(szFname);
}

CString CZipArchive::GetFileDirAndName(LPCTSTR lpszFilePath) {
    TCHAR szDir[_MAX_DIR];
    TCHAR szFname[_MAX_FNAME];
    TCHAR szExt[_MAX_EXT];
    _tsplitpath(lpszFilePath, NULL, szDir, szFname, szExt);
    CString Dir = szDir;
    Dir.TrimLeft("\\");
    return Dir + szFname + szExt;
}

CString CZipArchive::GetFileName(LPCTSTR lpszFilePath) {
    TCHAR szExt[_MAX_EXT];
    TCHAR szName[_MAX_FNAME];
    _tsplitpath(lpszFilePath, NULL, NULL, szName, szExt);
    return CString(szName) + CString(szExt);
}

bool CZipArchive::ForceDirectory(LPCTSTR lpDirectory) {
    ASSERT(lpDirectory);
    CString szDirectory = lpDirectory;
    szDirectory.TrimRight("\\");
    if ((GetFilePath(szDirectory) == szDirectory) || (FileExists(szDirectory) == -1)) {
        return true;
    }
    if (!ForceDirectory(GetFilePath(szDirectory))) {
        return false;
    }
    if (!CreateDirectory(szDirectory, NULL)) {
        return false;
    }
    return true;
}

int CZipArchive::FileExists(LPCTSTR lpszName) {
    CFileFind fileFind;
    if (!fileFind.FindFile(lpszName)) {
        if (DirectoryExists(lpszName)) { // if root ex. "C:\"
            return -1;
        }
        return 0;
    }
    fileFind.FindNextFile();
    return fileFind.IsDirectory() ? -1 : 1;
}

bool CZipArchive::DirectoryExists(LPCTSTR lpszDir) {
    char curPath[_MAX_PATH]; /* Get the current working directory: */
    if (!_getcwd(curPath, _MAX_PATH)) {
        return false;
    }
    if (_chdir(lpszDir)) { // retruns 0 if error
        return false;
    }
    _chdir(curPath);
    return true;
}

bool CZipArchive::IsFileDirectory(WORD uIndex) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }

    if (!m_centralDir.IsValidIndex(uIndex)) {
        return false;
    }

    return IsDirectory(m_centralDir.m_headers[uIndex]->m_uExternalAttr);
}

bool CZipArchive::ExtractFile(WORD uIndex, LPCTSTR lpszPath, DWORD nBufSize) {
    if (!nBufSize) {
        return false;
    }

    CFileHeader header;
    GetFileInfo(header, uIndex); // to ensure that slash and oem conversions take place
    CString szFile = lpszPath;
    szFile.TrimRight("\\");
    szFile +=
        "\\" + GetFileDirAndName(header.m_szFileName); // just in case in the archive there are file names with drives
    if (IsFileDirectory(uIndex)) {
        ForceDirectory(szFile);
        SetFileAttributes(szFile, header.m_uExternalAttr);
    } else {
        if (!OpenFile(uIndex)) {
            return false;
        }
        ForceDirectory(GetFilePath(szFile));
        CFile       f(szFile, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyWrite);
        DWORD       iRead;
        CAutoBuffer buf(nBufSize);
        do {
            iRead = ReadFile(buf, buf.GetSize());
            if (iRead) {
                f.Write(buf, iRead);
            }
        } while (iRead == buf.GetSize());
        CloseFile(f);
    }
    return true;
}

void CZipArchive::SetExtraField(char * pBuf, WORD iSize) {
    if (m_iFileOpened != compress) {
        TRACE("A new file must be opened.\n");
        return;
    }
    if (!pBuf || !iSize) {
        return;
    }

    CurrentFile()->m_pExtraField.Allocate(iSize);
    memcpy(CurrentFile()->m_pExtraField, pBuf, iSize);
    CurrentFile()->m_uExtraFieldSize = iSize;
}

bool CZipArchive::WriteNewFile(void * pBuf, DWORD iSize) {
    if (m_iFileOpened != compress) {
        TRACE("A new file must be opened.\n");
        return false;
    }

    m_info.m_stream.next_in = (unsigned char *)pBuf;
    m_info.m_stream.avail_in = iSize;
    CurrentFile()->m_uCrc32 = crc32(CurrentFile()->m_uCrc32, (unsigned char *)pBuf, iSize);

    while (m_info.m_stream.avail_in > 0) {
        if (m_info.m_stream.avail_out == 0) {
            m_storage.Write(m_info.m_pBuffer, m_info.m_uComprLeft, false);
            m_info.m_uComprLeft = 0;
            m_info.m_stream.avail_out = m_info.m_pBuffer.GetSize();
            m_info.m_stream.next_out = (unsigned char *)(char *)m_info.m_pBuffer;
        }

        if (CurrentFile()->m_uMethod == Z_DEFLATED) {
            DWORD uTotal = m_info.m_stream.total_out;
            int   err = deflate(&m_info.m_stream, Z_NO_FLUSH);
            CheckForError(err);
            m_info.m_uComprLeft += m_info.m_stream.total_out - uTotal;
        } else {
            DWORD uToCopy = (m_info.m_stream.avail_in < m_info.m_stream.avail_out) ? m_info.m_stream.avail_in
                                                                                   : m_info.m_stream.avail_out;

            memcpy(m_info.m_stream.next_out, m_info.m_stream.next_in, uToCopy);

            m_info.m_stream.avail_in -= uToCopy;
            m_info.m_stream.avail_out -= uToCopy;
            m_info.m_stream.next_in += uToCopy;
            m_info.m_stream.next_out += uToCopy;
            m_info.m_stream.total_in += uToCopy;
            m_info.m_stream.total_out += uToCopy;
            m_info.m_uComprLeft += uToCopy;
        }
    }

    return true;
}

bool CZipArchive::CloseNewFile() {
    if (m_iFileOpened != compress) {
        TRACE("A new file must be opened.\n");
        return false;
    }

    m_info.m_stream.avail_in = 0;

    int err = Z_OK;
    if (CurrentFile()->m_uMethod == Z_DEFLATED) {
        while (err == Z_OK) {
            if (m_info.m_stream.avail_out == 0) {
                m_storage.Write(m_info.m_pBuffer, m_info.m_uComprLeft, false);
                m_info.m_uComprLeft = 0;
                m_info.m_stream.avail_out = m_info.m_pBuffer.GetSize();
                m_info.m_stream.next_out = (unsigned char *)(char *)m_info.m_pBuffer;
            }
            DWORD uTotal = m_info.m_stream.total_out;
            err = deflate(&m_info.m_stream, Z_FINISH);
            m_info.m_uComprLeft += m_info.m_stream.total_out - uTotal;
        }
    }

    if (err == Z_STREAM_END) {
        err = Z_OK;
    }
    CheckForError(err);

    if (m_info.m_uComprLeft > 0) {
        m_storage.Write(m_info.m_pBuffer, m_info.m_uComprLeft, false);
    }

    if (CurrentFile()->m_uMethod == Z_DEFLATED) {
        err = deflateEnd(&m_info.m_stream);
        CheckForError(err);
    }

    CurrentFile()->m_uComprSize = m_info.m_stream.total_out;
    CurrentFile()->m_uUncomprSize = m_info.m_stream.total_in;

    m_centralDir.CloseNewFile();
    m_iFileOpened = nothing;
    m_info.m_pBuffer.Release();
    return true;
}

void CZipArchive::DeleteFiles(CStringArray & aNames) {
    CWordArray indexes;

    for (WORD i = 0; i < GetNoEntries(); i++) {
        CFileHeader fh;
        GetFileInfo(fh, i);
        for (int j = 0; j < aNames.GetSize(); j++) {
            if (!aNames[j].CollateNoCase(fh.m_szFileName)) {
                indexes.Add(i);
                break;
            }
        }
    }

    DeleteFiles(indexes);
}

void CZipArchive::DeleteFiles(CWordArray & aIndexes) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return;
    }

    if (m_storage.IsSpanMode()) {
        TRACE("You cannot delete files from the disk spannig archive.\n");
        return;
    }

    if (m_iFileOpened) {
        TRACE("You cannot delete files if there is a file opened.\n");
        return;
    }

    // sorting the index table using qsort
    int uSize = aIndexes.GetSize();
    if (!uSize) {
        return;
    }
    qsort((void *)&aIndexes[0], uSize, sizeof(WORD), CompareWords);

    m_centralDir.RemoveFromDisk();

    m_info.Init();
    // remove in a reverse order
    for (int i = uSize - 1; i >= 0; i--) {
        DeleteInternal(aIndexes[i]);
    }
    m_info.m_pBuffer.Release();
}

DWORD CZipArchive::RemovePackedFile(DWORD uStartOffset, DWORD uEndOffset) {
    uStartOffset += m_centralDir.m_uBytesBeforeZip;
    uEndOffset += m_centralDir.m_uBytesBeforeZip;
    DWORD BytesToCopy = m_storage.m_file.GetLength() - uEndOffset;

    char * buf = (char *)m_info.m_pBuffer;
    if (BytesToCopy > m_info.m_pBuffer.GetSize()) {
        BytesToCopy = m_info.m_pBuffer.GetSize();
    }

    m_storage.m_file.Seek(uStartOffset, CFile::begin);

    DWORD TotalWritten = 0;
    DWORD size_read;

    do {
        m_storage.m_file.Seek(uEndOffset + TotalWritten, CFile::begin);
        size_read = m_storage.m_file.Read(buf, BytesToCopy);
        if (size_read > 0) {
            m_storage.m_file.Seek(uStartOffset + TotalWritten, CFile::begin);
            m_storage.m_file.Write(buf, size_read);
        }
        TotalWritten += size_read;
    } while (size_read == BytesToCopy);
    DWORD uRemoved = (uEndOffset - uStartOffset);
    m_storage.m_file.SetLength(m_storage.m_file.GetLength() - uRemoved);
    return uRemoved;
}

void CZipArchive::DeleteInternal(WORD uIndex) {
    CFileHeader * pfh = m_centralDir.m_headers[uIndex];
    DWORD         uOtherOffsetChanged = 0;

    if (uIndex == GetNoEntries() - 1) { // last entry or the only one entry
        m_storage.m_file.SetLength(pfh->m_uOffset + m_centralDir.m_uBytesBeforeZip);
    } else {
        uOtherOffsetChanged = RemovePackedFile(pfh->m_uOffset, m_centralDir.m_headers[uIndex + 1]->m_uOffset);
    }

    m_centralDir.RemoveFile(uIndex);

    // teraz uaktualnij offsety w pozosta퀉ch pozycjach central dir
    // (update offsets in file headers in the central dir)
    if (uOtherOffsetChanged) {
        for (int i = uIndex; i < GetNoEntries(); i++) {
            m_centralDir.m_headers[i]->m_uOffset -= uOtherOffsetChanged;
        }
    }
}

bool CZipArchive::IsDriveRemovable(LPCTSTR lpszFilePath) {
    return GetDriveType(GetDrive(lpszFilePath)) == DRIVE_REMOVABLE;
}

CString CZipArchive::GetDrive(LPCTSTR lpszFilePath) {
    TCHAR szDrive[_MAX_DRIVE];
    _tsplitpath(lpszFilePath, szDrive, NULL, NULL, NULL);
    return szDrive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    특정 디렉토리를 제거한체 압축한다. 2001.5.30
//
BOOL CZipArchive::AddNewFile(LPCTSTR lpszFilePath, LPCTSTR strPath, int iLevel, unsigned long nBufSize) {
    CString strFileName = _T("");
    CString strDirPath = _T("");

    if (!nBufSize) {
        return FALSE;
    }

    CFileHeader header;
    //    strFileName = GetFileDirAndName(lpszFilePath);
    //    strFileName.TrimLeft(strPath);
    strFileName.Format("%s", lpszFilePath);
    strDirPath.Format("%s", strPath);
    strFileName.Delete(0, strDirPath.GetLength());

    header.m_szFileName = strFileName;

    if (header.m_szFileName.IsEmpty()) {
        return FALSE;
    }
    if (!OpenNewFile(header, iLevel, lpszFilePath)) {
        return FALSE;
    }

    if (!IsDirectory(header.m_uExternalAttr)) {
        CFile            f;
        CFileException * e = new CFileException;
        BOOL             bRet = f.Open(lpszFilePath, CFile::modeRead | CFile::shareDenyWrite, e);
        e->Delete();
        if (!bRet) {
            return FALSE;
        }

        DWORD       iRead;
        CAutoBuffer buf(nBufSize);
        do {
            iRead = f.Read(buf, nBufSize);
            if (iRead) {
                WriteNewFile(buf, iRead);
            }
        } while (iRead == buf.GetSize());
    }
    CloseNewFile();
    return TRUE;
}

bool CZipArchive::AddNewFile(LPCTSTR lpszFilePath, int iLevel, unsigned long nBufSize) {
    if (!nBufSize) {
        return false;
    }

    CFileHeader header;
    header.m_szFileName = GetFileDirAndName(lpszFilePath);
    if (header.m_szFileName.IsEmpty()) {
        return false;
    }
    if (!OpenNewFile(header, iLevel, lpszFilePath)) {
        return false;
    }

    if (!IsDirectory(header.m_uExternalAttr)) {
        CFile            f;
        CFileException * e = new CFileException;
        BOOL             bRet = f.Open(lpszFilePath, CFile::modeRead | CFile::shareDenyWrite, e);
        e->Delete();
        if (!bRet) {
            return false;
        }

        DWORD       iRead;
        CAutoBuffer buf(nBufSize);
        do {
            iRead = f.Read(buf, nBufSize);
            if (iRead) {
                WriteNewFile(buf, iRead);
            }
        } while (iRead == buf.GetSize());
    }
    CloseNewFile();
    return true;
}

int CZipArchive::GetSpanMode() {
    return m_storage.m_iSpanMode * m_storage.IsSpanMode();
}

CString CZipArchive::GetArchivePath() {
    return m_storage.m_file.GetFilePath();
}

CString CZipArchive::GetGlobalComment() {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return "";
    }

    return m_centralDir.m_szComment;
}

bool CZipArchive::SetGlobalComment(const CString & szComment) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }
    if (m_storage.IsSpanMode() == -1) {
        TRACE("You cannot modify the global comment of the existing disk spanning archive.\n");
        return false;
    }

    m_centralDir.m_szComment = szComment;
    m_centralDir.RemoveFromDisk();
    return true;
}

bool CZipArchive::IsDirectory(DWORD uAttr) {
    return (uAttr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int CZipArchive::GetCurrentDisk() {
    return m_storage.GetCurrentDisk();
}

bool CZipArchive::SetFileComment(WORD uIndex, CString szComment) {
    if (IsClosed()) {
        TRACE("ZipArchive is closed.\n");
        return false;
    }
    if (m_storage.IsSpanMode() == -1) {
        TRACE("You cannot modify the global comment of the existing disk spanning archive.\n");
        return false;
    }

    if (!m_centralDir.IsValidIndex(uIndex)) {
        return false;
    }
    m_centralDir.m_headers[uIndex]->m_szComment = szComment;
    m_centralDir.m_headers[uIndex]->ValidateComment();
    m_centralDir.RemoveFromDisk();
    return true;
}

int CZipArchive::CompareWords(const void * pArg1, const void * pArg2) {
    WORD w1 = *(WORD *)pArg1;
    WORD w2 = *(WORD *)pArg2;
    return w1 == w2 ? 0 : (w1 < w2 ? -1 : 1);
}
