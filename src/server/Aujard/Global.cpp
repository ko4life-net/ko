#include "StdAfx.h"
#include "Define.h"

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

void DisplayErrorMsg(SQLHANDLE hstmt) {
    SQLCHAR     SqlState[6], Msg[1024];
    SQLINTEGER  NativeError;
    SQLSMALLINT i, MsgLen;
    SQLRETURN   rc2;
    char        logstr[512];
    memset(logstr, NULL, 512);

    i = 1;
    while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) !=
           SQL_NO_DATA) {
        sprintf(logstr, "*** %s, %d, %s, %d ***\r\n", SqlState, NativeError, Msg, MsgLen);
        n3std::log_file_write(logstr);
        //        TRACE("*** %s, %d, %s, %d ***\n", SqlState,NativeError,Msg,MsgLen);

        i++;
    }
}
