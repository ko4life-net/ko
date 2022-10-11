#include "StdAfx.h"
#include "define.h"
#include <mmsystem.h>

// sungyong 2001.11.06
int GetVarString(TCHAR * tBuf, TCHAR * sBuf, int nSize, int & index) {
    int nLen = 0;

    if (nSize == sizeof(BYTE)) {
        nLen = GetByte(sBuf, index);
    } else {
        nLen = GetShort(sBuf, index);
    }

    GetString(tBuf, sBuf, nLen, index);
    *(tBuf + nLen) = 0;

    return nLen;
}

void SetVarString(TCHAR * tBuf, TCHAR * sBuf, int len, int & index) {
    *(tBuf + index) = (BYTE)len;
    index++;

    CopyMemory(tBuf + index, sBuf, len);
    index += len;
}
// ~sungyong 2001.11.06

void GetString(char * tBuf, char * sBuf, int len, int & index) {
    memcpy(tBuf, sBuf + index, len);
    index += len;
}

BYTE GetByte(char * sBuf, int & index) {
    int t_index = index;
    index++;
    return (BYTE)(*(sBuf + t_index));
}

int GetShort(char * sBuf, int & index) {
    index += 2;
    return *(short *)(sBuf + index - 2);
}

DWORD GetDWORD(char * sBuf, int & index) {
    index += 4;
    return *(DWORD *)(sBuf + index - 4);
}

float Getfloat(char * sBuf, int & index) {
    index += 4;
    return *(float *)(sBuf + index - 4);
}

// jasy added...
__int64 Get_int64(char * sBuf, int & index) {
    index += 8;
    return *(__int64 *)(sBuf + index - 8);
}

void Set_int64(char * tBuf, __int64 receive, int & index) {
    __int64 temp = (__int64)receive;

    CopyMemory(tBuf + index, &temp, 8);
    index += 8;
}
//~

void SetString(char * tBuf, char * sBuf, int len, int & index) {
    memcpy(tBuf + index, sBuf, len);
    index += len;
}

void SetByte(char * tBuf, BYTE sByte, int & index) {
    *(tBuf + index) = (char)sByte;
    index++;
}

void SetShort(char * tBuf, int sShort, int & index) {
    short temp = (short)sShort;

    CopyMemory(tBuf + index, &temp, 2);
    index += 2;
}

void SetDWORD(char * tBuf, DWORD sDWORD, int & index) {
    CopyMemory(tBuf + index, &sDWORD, 4);
    index += 4;
}

void Setfloat(char * tBuf, float sFloat, int & index) {
    CopyMemory(tBuf + index, &sFloat, 4);
    index += 4;
}

int ParseSpace(char * tBuf, char * sBuf) {
    int  i = 0, index = 0;
    BOOL flag = FALSE;

    while (sBuf[index] == ' ' || sBuf[index] == '\t') {
        index++;
    }
    while (sBuf[index] != ' ' && sBuf[index] != '\t' && sBuf[index] != (BYTE)0) {
        tBuf[i++] = sBuf[index++];
        flag = TRUE;
    }
    tBuf[i] = 0;

    while (sBuf[index] == ' ' || sBuf[index] == '\t') {
        index++;
    }
    if (!flag) {
        return 0;
    }
    return index;
}

CString GetProgPath() {
    char Buf[256], Path[256];
    char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

    ::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
    _splitpath(Buf, drive, dir, fname, ext);
    strcpy(Path, drive);
    strcat(Path, dir);
    CString _Path = Path;
    return _Path;
}

void LogFileWrite(LPCTSTR logstr) {
    CString ProgPath, LogFileName;
    CFile   file;
    int     loglength;

    ProgPath = GetProgPath();
    loglength = strlen(logstr);

    LogFileName.Format("%s\\Ebenezer.log", ProgPath);

    file.Open(LogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

    file.SeekToEnd();
    file.Write(logstr, loglength);
    file.Close();
}

void DisplayErrorMsg(SQLHANDLE hstmt) {
    SQLCHAR     SqlState[6], Msg[1024];
    SQLINTEGER  NativeError;
    SQLSMALLINT i, MsgLen;
    SQLRETURN   rc2;

    i = 1;
    while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) !=
           SQL_NO_DATA) {
        TRACE("*** %s, %d, %s, %d ***\n", SqlState, NativeError, Msg, MsgLen);

        i++;
    }
}

int myrand(int min, int max) {
    if (min == max) {
        return min;
    }
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    double gap = max - min + 1;
    double rrr = (double)RAND_MAX / gap;

    double rand_result;

    rand_result = (double)rand() / rrr;

    if ((int)(min + (int)rand_result) < min) {
        return min;
    }
    if ((int)(min + (int)rand_result) > max) {
        return max;
    }

    return (int)(min + (int)rand_result);
}

float TimeGet() {
    static bool          bInit = false;
    static bool          bUseHWTimer = FALSE;
    static LARGE_INTEGER nTime, nFrequency;

    if (bInit == false) {
        if (TRUE == ::QueryPerformanceCounter(&nTime)) {
            ::QueryPerformanceFrequency(&nFrequency);
            bUseHWTimer = TRUE;
        } else {
            bUseHWTimer = FALSE;
        }

        bInit = true;
    }

    if (bUseHWTimer) {
        ::QueryPerformanceCounter(&nTime);
        return (float)((double)(nTime.QuadPart) / (double)nFrequency.QuadPart);
    }

    return (float)timeGetTime();
}
