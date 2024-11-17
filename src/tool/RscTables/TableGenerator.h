// TableGenerator.h: interface for the CTableGenerator class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "RscTablesDef.h"
#include <vector>
#include <string>

const int MAX_ITEM_EXTENSION = 24;

class CTableData {
  public:
    DATA_TYPE   m_Type;
    std::string m_szTitle;

    std::vector<std::string> m_Texts;
    std::vector<int>         m_iValues;
    std::vector<DWORD>       m_dwValues;
    std::vector<double>      m_dfValues;

  public:
    void Release() {
        m_Type = DT_NONE;
        m_szTitle = "";

        m_Texts.clear();
        m_iValues.clear();
        m_dwValues.clear();
        m_dfValues.clear();
    }

    CTableData() { m_Type = DT_NONE; }
    virtual ~CTableData() {}
};

class CTableDataExt {
  public:
    DATA_TYPE   m_Type;
    std::string m_szTitle;

    std::vector<std::string> m_Texts[MAX_ITEM_EXTENSION];
    std::vector<int>         m_iValues[MAX_ITEM_EXTENSION];
    std::vector<DWORD>       m_dwValues[MAX_ITEM_EXTENSION];
    std::vector<double>      m_dfValues[MAX_ITEM_EXTENSION];

  public:
    void Release() {
        m_Type = DT_NONE;
        m_szTitle = "";

        for (int i = 0; i < MAX_ITEM_EXTENSION; i++) {
            m_Texts[i].clear();
            m_iValues[i].clear();
            m_dwValues[i].clear();
            m_dfValues[i].clear();
        }
    }

    CTableDataExt() { m_Type = DT_NONE; }
    virtual ~CTableDataExt() {}
};

class CTableGenerator {
  public:
    fs::path m_fsEnmBasicFile;
    fs::path m_fsTxtBasicFile;
    fs::path m_fsEnmExtFile;
    fs::path m_fsTxtExtFiles[MAX_ITEM_EXTENSION];

  protected:
    std::vector<DATA_TYPE>     m_DataTypes;
    std::vector<CTableData>    m_Datas;    // 기본이 되는 아이템 테이블
    std::vector<CTableDataExt> m_DataExts; // 참조할 아이템 테이블

  private:
    int ParseLine(const char * szLine, int & iOffset, int & iVal, DWORD & dwVal, double & dfVal,
                  std::string & szText); // 성공하면 0, 추가해야하고 끝이면 -1 추가할 필요 없고 끝이면.. -2 리턴

    bool   WriteData(HANDLE hFile, DATA_TYPE DataType, LPCTSTR lpszData);
    bool   ReadData(HANDLE hFile, DATA_TYPE DataType, LPTSTR lpszData);
    char * MyToken(LPCTSTR lpszInput);

  public:
    int       DataTypeCount() { return m_DataTypes.size(); }
    DATA_TYPE DataType(int iIndex) {
        if (iIndex < 0 || iIndex >= m_DataTypes.size()) {
            return DT_NONE;
        } else {
            return m_DataTypes[iIndex];
        }
    }
    bool DataTypeSet(int iIndex, DATA_TYPE Type) {
        if (iIndex < 0 || iIndex >= m_DataTypes.size()) {
            return false;
        }
        m_DataTypes[iIndex] = Type;
        return true;
    }
    bool DataTypeDelete(int iIndex);
    bool DataTypeInsert(int iIndex, DATA_TYPE Type);
    bool DataTypeAssign(int iCount, DATA_TYPE Type);
    bool DataTypeLoad(const fs::path & fsFile);
    bool DataTypeSave(const fs::path & fsFile);

    bool Convert2Bin(const fs::path & fsFile);

    bool OpenSource(const fs::path & fsEnumFile, const fs::path & fsTxtFile);
    bool OpenReference_Enum(const fs::path & fsEnumFile);
    bool OpenReference_Txt(int iIndex, const fs::path & fsTxtFile);
    bool Generate(int iIndex, const fs::path & fsEnumFile, const fs::path & fsTxtFile);

    void Release();
    CTableGenerator();
    virtual ~CTableGenerator();
};
