// N3TableBase.h: interface for the CN3TableBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef _N3GAME
#include "LogWriter.h"
#endif

#include <vector>
#include <map>

template <typename Type> class CN3TableBase {
  public:
    CN3TableBase();
    virtual ~CN3TableBase();

    // Attributes
  protected:
    enum DATA_TYPE {
        DT_NONE,
        DT_CHAR,
        DT_BYTE,
        DT_SHORT,
        DT_WORD,
        DT_INT,
        DT_DWORD,
        DT_STRING,
        DT_FLOAT,
        DT_DOUBLE
    };
    typedef typename std::vector<DATA_TYPE>::iterator it_DataType;

    typedef typename std::map<unsigned int, Type>::iterator   it_Table;
    typedef typename std::map<unsigned int, Type>::value_type val_Table;
    typedef std::pair<it_Table, bool>                         pair_Table;

    std::vector<DATA_TYPE>       m_DataTypes; // Data type of information actually used
    std::map<unsigned int, Type> m_Datas;     //Actually used information

    // Operations
  public:
    void   Release();
    Type * Find(unsigned int dwID) // Find data by ID
    {
        it_Table it = m_Datas.find(dwID);
        if (it == m_Datas.end()) {
            return NULL; // Failed to find it!~!!
        } else {
            return &(it->second);
        }
    }
    int    GetSize() { return m_Datas.size(); }
    Type * GetIndexedData(int index) //Find by index..
    {
        if (index < 0 || m_Datas.empty()) {
            return NULL;
        }
        if (index >= m_Datas.size()) {
            return NULL;
        }

        it_Table it = m_Datas.begin();
        for (int i = 0; i < index; i++, it++) {
            ;
        }
        return &(it->second);
    }
    int IDToIndex(unsigned int dwID) // Returns the Index of the ID.. Used in Skill..
    {
        it_Table it = m_Datas.find(dwID);
        if (it == m_Datas.end()) {
            return -1;// Failed to find!~!!
        }

        it_Table itSkill = m_Datas.begin();
        int      iSize = m_Datas.size();
        for (int i = 0; i < iSize; i++, itSkill++) {
            if (itSkill == it) {
                return i;
            }
        }

        return -1;
    }
    BOOL LoadFromFile(const std::string & szFN);

  protected:
    BOOL Load(HANDLE hFile);
    BOOL WriteData(HANDLE hFile, DATA_TYPE DataType, const char * lpszData);
    BOOL ReadData(HANDLE hFile, DATA_TYPE DataType, void * pData);

    int  SizeOf(DATA_TYPE DataType) const;
    BOOL MakeOffsetTable(std::vector<int> & offsets);
};

//Because it is in the cpp file, a link error occurs. Why?

template <class Type> CN3TableBase<Type>::CN3TableBase() {}

template <class Type> CN3TableBase<Type>::~CN3TableBase() {
    Release();
}

template <class Type> void CN3TableBase<Type>::Release() {
    m_DataTypes.clear(); // Clear saved data type
    m_Datas.clear();     // Clear row data
}

// Write data type to file..
template <class Type> BOOL CN3TableBase<Type>::WriteData(HANDLE hFile, DATA_TYPE DataType, const char * lpszData) {
    DWORD dwNum;
    switch (DataType) {
    case DT_CHAR: {
        char cWrite;
        if (isdigit(lpszData[0])) {
            int iTemp = atoi(lpszData);
            if (iTemp < -127 || iTemp > 128) {
                return FALSE; //Out of range~
            }
            cWrite = (char)iTemp;
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &cWrite, sizeof(cWrite), &dwNum, NULL);
    } break;
    case DT_BYTE: {
        BYTE byteWrite;
        if (isdigit(lpszData[0])) {
            int iTemp = atoi(lpszData);
            if (iTemp < 0 || iTemp > 255) {
                return FALSE; //Out of range~
            }
            byteWrite = (BYTE)iTemp;
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &byteWrite, sizeof(byteWrite), &dwNum, NULL);
    } break;
    case DT_SHORT: {
        short iWrite;
        if (isdigit(lpszData[0]) || '-' == lpszData[0]) {
            int iTemp = atoi(lpszData);
            if (iTemp < -32767 || iTemp > 32768) {
                return FALSE;//Out of range~
            }
            iWrite = (short)iTemp;
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_WORD: {
        WORD iWrite;
        if (isdigit(lpszData[0])) {
            int iTemp = atoi(lpszData);
            if (iTemp < 0 || iTemp > 65535) {
                return FALSE; //Out of range~
            }
            iWrite = (short)iTemp;
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_INT: {
        int iWrite;
        if (isdigit(lpszData[0]) || '-' == lpszData[0]) {
            iWrite = atoi(lpszData);
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_DWORD: {
        DWORD iWrite;
        if (isdigit(lpszData[0])) {
            iWrite = strtoul(lpszData, NULL, 10);
        } else {
            return FALSE; // No text!
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_STRING: {
        std::string & szString = *((std::string *)lpszData);
        int           iStrLen = szString.size();
        WriteFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);
        if (iStrLen > 0) {
            WriteFile(hFile, &(szString[0]), iStrLen, &dwNum, NULL);
        }
    } break;
    case DT_FLOAT: {
        float fWrite;
        if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0]) {
            fWrite = (float)atof(lpszData);
        } else {
            return FALSE; // No text!
        }
        WriteFile(hFile, &fWrite, sizeof(fWrite), &dwNum, NULL);
    } break;
    case DT_DOUBLE: {
        double dWrite;
        if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0]) {
            dWrite = atof(lpszData);
        }
        WriteFile(hFile, &dWrite, sizeof(dWrite), &dwNum, NULL);
    } break;

    case DT_NONE:
    default:
        __ASSERT(0, "");
    }
    return TRUE;
}

template <class Type> BOOL CN3TableBase<Type>::ReadData(HANDLE hFile, DATA_TYPE DataType, void * pData) {
    DWORD dwNum;
    switch (DataType) {
    case DT_CHAR: {
        ReadFile(hFile, pData, sizeof(char), &dwNum, NULL);
    } break;
    case DT_BYTE: {
        ReadFile(hFile, pData, sizeof(BYTE), &dwNum, NULL);
    } break;
    case DT_SHORT: {
        ReadFile(hFile, pData, sizeof(short), &dwNum, NULL);
    } break;
    case DT_WORD: {
        ReadFile(hFile, pData, sizeof(WORD), &dwNum, NULL);
    } break;
    case DT_INT: {
        ReadFile(hFile, pData, sizeof(int), &dwNum, NULL);
    } break;
    case DT_DWORD: {
        ReadFile(hFile, pData, sizeof(DWORD), &dwNum, NULL);
    } break;
    case DT_STRING: {
        std::string & szString = *((std::string *)pData);

        int iStrLen = 0;
        ReadFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);

        szString = "";
        if (iStrLen > 0) {
            szString.assign(iStrLen, ' ');
            ReadFile(hFile, &(szString[0]), iStrLen, &dwNum, NULL);
        }
    } break;
    case DT_FLOAT: {
        ReadFile(hFile, pData, sizeof(float), &dwNum, NULL);
    } break;
    case DT_DOUBLE: {
        ReadFile(hFile, pData, sizeof(double), &dwNum, NULL);
    } break;

    case DT_NONE:
    default:
        __ASSERT(0, "");
        return FALSE;
    }
    return TRUE;
}

template <class Type> BOOL CN3TableBase<Type>::LoadFromFile(const std::string & szFN) {
    if (szFN.empty()) {
        return FALSE;
    }

    HANDLE hFile = ::CreateFile(szFN.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
#ifdef _N3GAME
        CLogWriter::Write("N3TableBase - Can't open file(read) File Handle error (%s)", szFN.c_str());
#endif
        return FALSE;
    }

    // Decrypt the file... write it to a temporary file...
    std::string szFNTmp = szFN + ".tmp";
    DWORD       dwSizeHigh = 0;
    DWORD       dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
    if (dwSizeLow <= 0) {
        CloseHandle(hFile);
        ::remove(szFNTmp.c_str()); // Clear temporary files..
        return FALSE;
    }

// Read the original file...
    BYTE * pDatas = new BYTE[dwSizeLow];
    DWORD  dwRWC = 0;
    ::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // Read encrypted data...
    CloseHandle(hFile);                                 // close the original file

   // Same key as the key used in the table creation tool..
    WORD key_r = 0x0816;
    WORD key_c1 = 0x6081;
    WORD key_c2 = 0x1608;

    //BYTE Encrypt(BYTE plain)
    //{
    //    BYTE cipher;
    //    cipher = (plain ^ (key_r>>8));
    //    key_r = (cipher + key_r) * key_c1 + key_c2;
    //    return cipher;
    //}

    //BYTE Decrypt(BYTE cipher)
    //{
    //    BYTE plain;
    //    plain = (cipher ^ (m_r>>8));
    //    m_r = (cipher + m_r) * m_c1 + m_c2;
    //    return plain;
    //}

    // Decrypt encryption...
    for (int i = 0; i < dwSizeLow; i++) {
        BYTE byData = (pDatas[i] ^ (key_r >> 8));
        key_r = (pDatas[i] + key_r) * key_c1 + key_c2;
        pDatas[i] = byData;
    }

    // Write to a temporary file... then open it again...
    hFile = ::CreateFile(szFNTmp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // Write decrypted data to temporary file
    CloseHandle(hFile);                                  // Close temporary file
    delete[] pDatas;
    pDatas = NULL;

    hFile = ::CreateFile(szFNTmp.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                         NULL); // Open temporary file in read mode.

    BOOL bResult = Load(hFile);

    CloseHandle(hFile);

    if (FALSE == bResult) {
#ifdef _N3GAME
        CLogWriter::Write("N3TableBase - incorrect table (%s)", szFN.c_str());
#endif
    }

    // Clear temporary files..
    ::remove(szFNTmp.c_str());

    return bResult;
}

template <class Type> BOOL CN3TableBase<Type>::Load(HANDLE hFile) {
    Release();

    // Read how data(column) is structured
    DWORD dwNum;
    int   iDataTypeCount = 0;
    ReadFile(hFile, &iDataTypeCount, 4, &dwNum, NULL); // (Number of columns in Excel)

    std::vector<int> offsets;
    __ASSERT(iDataTypeCount > 0, "Data Type is 0 or less.");
    if (iDataTypeCount > 0) {
        m_DataTypes.insert(m_DataTypes.begin(), iDataTypeCount, DT_NONE);
        ReadFile(hFile, &(m_DataTypes[0]), sizeof(DATA_TYPE) * iDataTypeCount, &dwNum,
                 NULL); // data type corresponding to each column

        if (FALSE == MakeOffsetTable(offsets)) {
            __ASSERT(0, "can't make offset table");
            return FALSE; // Create an offset table for structure variables
        }

        int iSize = offsets
            [iDataTypeCount]; //Among the values returned from the MakeOffstTable function, m_iDataTypeCount contains the actual size of this function.
        if (sizeof(Type) != iSize ||    // The size of the entire type is different from the size of the actual structure
            DT_DWORD != m_DataTypes[0]) // When the first data is not DT_DWORD type (since the first data is a unique ID)
        {
            m_DataTypes.clear();
            __ASSERT(0, "DataType is mismatch or DataSize is incorrect!!");
            return FALSE;
        }
    }

   // read how many rows there are
    int iRC;
    ReadFile(hFile, &iRC, sizeof(iRC), &dwNum, NULL);
    Type Data;
    for (int i = 0; i < iRC; ++i) {
        for (int j = 0; j < iDataTypeCount; ++j) {
            ReadData(hFile, m_DataTypes[j], (char *)(&Data) + offsets[j]);
        }

        unsigned int dwKey = *((unsigned int *)(&Data));
        pair_Table   pt = m_Datas.insert(val_Table(dwKey, Data));

        __ASSERT(pt.second, "CN3TableBase<Type> : Key duplication warning.");
    }
    return TRUE;
}

template <class Type> int CN3TableBase<Type>::SizeOf(DATA_TYPE DataType) const {
    switch (DataType) {
    case DT_CHAR:
        return sizeof(char);
    case DT_BYTE:
        return sizeof(BYTE);
    case DT_SHORT:
        return sizeof(short);
    case DT_WORD:
        return sizeof(WORD);
    case DT_INT:
        return sizeof(int);
    case DT_DWORD:
        return sizeof(DWORD);
    case DT_STRING:
        return sizeof(std::string);
    case DT_FLOAT:
        return sizeof(float);
    case DT_DOUBLE:
        return sizeof(double);
    }
    __ASSERT(0, "");
    return 0;
}

// The structure captures memory by aligning it by 4 bytes. Therefore, the function below is needed.
// After creating and using OffsetTable with the function below, you must delete [] the return value when creating it.
template <class Type> BOOL CN3TableBase<Type>::MakeOffsetTable(std::vector<int> & offsets) {
    if (m_DataTypes.empty()) {
        return false;
    }

    int iDataTypeCount = m_DataTypes.size();
    offsets.clear();
    offsets.resize(iDataTypeCount + 1); // The reason for adding +1 is to put the actual size of the Type in the last value.
    offsets[0] = 0;
    int iPrevDataSize = SizeOf(m_DataTypes[0]);
    for (int i = 1; i < iDataTypeCount; ++i) {
        int iCurDataSize = SizeOf(m_DataTypes[i]);
        if (1 == iCurDataSize % 4) //If the current data is 1 byte, it doesn't matter how many bytes the previous data is.
        {
            offsets[i] = offsets[i - 1] + iPrevDataSize;
        } else if (2 == iCurDataSize % 4) //If the current data is 2 bytes, it must be located at an even address.
        {
            if (0 == ((offsets[i - 1] + iPrevDataSize) % 2)) {
                offsets[i] = offsets[i - 1] + iPrevDataSize;
            } else {
                offsets[i] = offsets[i - 1] + iPrevDataSize + 1;
            }
        } else if (0 == iCurDataSize % 4) // If the current data is 4 bytes, it must be located at an address that is a multiple of 4.
        {
            if (0 == ((offsets[i - 1] + iPrevDataSize) % 4)) {
                offsets[i] = offsets[i - 1] + iPrevDataSize;
            } else {
                offsets[i] = ((int)(offsets[i - 1] + iPrevDataSize + 3) / 4) * 4; // Make it a multiple of 4
            }
        } else {
            __ASSERT(0, "");
        }
        iPrevDataSize = iCurDataSize;
    }

   // Let's put the actual size of the Type in the last value.
    offsets[iDataTypeCount] = ((int)(offsets[iDataTypeCount - 1] + iPrevDataSize + 3) / 4) * 4; // Make it a multiple of 4

    return true;
}
