// LogWriter.cpp: implementation of the CLogWriter class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "StdAfx.h"
#include "LogWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//HANDLE CLogWriter::s_hFile = NULL;
fs::path CLogWriter::s_fsLogFile;

CLogWriter::CLogWriter() {}

CLogWriter::~CLogWriter() {}

void CLogWriter::Open(const fs::path & fsFile) {
    // if (s_hFile || fsFile.empty()) {
    //     return;
    // }
    if (fsFile.empty()) {
        return;
    }

    s_fsLogFile = fsFile;

    HANDLE hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            return;
        }
    }

    DWORD dwSizeHigh = 0;
    DWORD dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
    if (dwSizeLow > 256000) // 파일 사이즈가 너무 크면 지운다..
    {
        CloseHandle(hFile);
        fs::remove(s_fsLogFile);
        hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            return;
        }
    }

    ::SetFilePointer(hFile, 0, NULL, FILE_END); // 추가 하기 위해서 파일의 끝으로 옮기고..

    char       szBuff[1024];
    SYSTEMTIME time;
    GetLocalTime(&time);
    DWORD dwRWC = 0;

    sprintf(szBuff, "---------------------------------------------------------------------------\r\n");
    int iLength = lstrlen(szBuff);
    WriteFile(hFile, szBuff, iLength, &dwRWC, NULL);

    sprintf(szBuff, "// Begin writing log... [%.2d/%.2d %.2d:%.2d]\r\n", time.wMonth, time.wDay, time.wHour,
            time.wMinute);
    iLength = lstrlen(szBuff);
    WriteFile(hFile, szBuff, iLength, &dwRWC, NULL);

    CloseHandle(hFile);
}

void CLogWriter::Close() {
    HANDLE hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            hFile = NULL;
        }
    }

    if (hFile) {
        ::SetFilePointer(hFile, 0, NULL, FILE_END); // 추가 하기 위해서 파일의 끝으로 옮기고..

        char       szBuff[1024];
        SYSTEMTIME time;
        GetLocalTime(&time);
        DWORD dwRWC = 0;

        sprintf(szBuff, "// End writing log... [%.2d/%.2d %.2d:%.2d]\r\n", time.wMonth, time.wDay, time.wHour,
                time.wMinute);
        int iLength = lstrlen(szBuff);
        WriteFile(hFile, szBuff, iLength, &dwRWC, NULL);

        sprintf(szBuff, "---------------------------------------------------------------------------\r\n");
        iLength = lstrlen(szBuff);
        WriteFile(hFile, szBuff, iLength, &dwRWC, NULL);

        CloseHandle(hFile);
        hFile = NULL;
    }
}

void CLogWriter::Write(const char * lpszFormat, ...) {
    if (s_fsLogFile.empty() || NULL == lpszFormat) {
        return;
    }

    static char       szFinal[1024];
    static SYSTEMTIME time;
    GetLocalTime(&time);
    szFinal[0] = NULL;

    DWORD dwRWC = 0;
    sprintf(szFinal, "    [%.2d:%.2d:%.2d] ", time.wHour, time.wMinute, time.wSecond);

    static char szBuff[1024];
    szBuff[0] = NULL;
    va_list argList;
    va_start(argList, lpszFormat);
    vsprintf(szBuff, lpszFormat, argList);
    va_end(argList);
    N3_DEBUG("CLogWriter::Write: {}", szBuff);

    lstrcat(szFinal, szBuff);
    lstrcat(szFinal, "\r\n");
    int iLength = lstrlen(szFinal);

    HANDLE hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        hFile = CreateFileW(s_fsLogFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            hFile = NULL;
        }
    }

    if (hFile) {
        ::SetFilePointer(hFile, 0, NULL, FILE_END); // 추가 하기 위해서 파일의 끝으로 옮기고..

        WriteFile(hFile, szFinal, iLength, &dwRWC, NULL);
        CloseHandle(hFile);
    }
}
