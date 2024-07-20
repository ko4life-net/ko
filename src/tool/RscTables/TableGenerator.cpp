// TableGenerator.cpp: implementation of the CTableGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TableGenerator.h"
#include <set>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
CTableGenerator::CTableGenerator() {}

CTableGenerator::~CTableGenerator() {}

bool CTableGenerator::OpenSource(const std::string & szEnumFileName, const std::string & szTxtFileName) {
    if (szEnumFileName.empty() || szTxtFileName.empty()) {
        return false;
    }

    HWND   hWnd = ::GetActiveWindow();
    HANDLE hFile =
        CreateFile(szEnumFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, szEnumFileName.c_str(), "File does not exist or cannot be read.", MB_OK);
        return false;
    }

    // ���Ͽ��� ���� �б�
    m_Datas.clear();
    int   iDataCount = 0;
    DWORD dwNum;
    ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
    if (iDataCount <= 0) {
        CloseHandle(hFile);
        MessageBox(hWnd, szEnumFileName.c_str(), "There is no data type.", MB_OK);
        return false;
    }

    m_Datas.resize(iDataCount);
    for (int i = 0; i < iDataCount; ++i) {
        ReadFile(hFile, &(m_Datas[i].m_Type), sizeof(DATA_TYPE), &dwNum, NULL);
    }
    CloseHandle(hFile);

    FILE * pFile = fopen(szTxtFileName.c_str(), "r");
    if (NULL == pFile) {
        MessageBox(hWnd, szTxtFileName.c_str(), "File does not exist or cannot be read.", MB_OK);
        return false;
    }

    // ���� �̸� ���..
    m_szEnmBasic = szEnumFileName;
    m_szTxtBasic = szTxtFileName;

    char                                     szLine[1024];
    std::set<int>                            KeySet;
    typedef typename std::set<int>::iterator it_Key;
    typedef std::pair<it_Key, bool>          pair_Key;

    for (int i = 0; true; i++) {
        if (NULL == fgets(szLine, 1024, pFile)) {
            break;
        }
        if (NULL == szLine[0] || '\r' == szLine[0] || '\n' == szLine[0]) {
            continue;
        }

        int         iOffset = 0;
        int         iVal = 0;
        DWORD       dwVal = 0;
        double      dfVal = 0;
        std::string szText;

        for (int j = 0; j < iDataCount; j++) {
            int iSuccess =
                this->ParseLine(szLine, iOffset, iVal, dwVal, dfVal,
                                szText); // �����ϸ� 0, �߰��ؾ��ϰ� ���̸� -1 �߰��� �ʿ� ���� ���̸�.. -2 ����

            if (iSuccess > 0) {
                if (i == 0) // ù���� �����̴�..
                {
                    m_Datas[j].m_szTitle = szText;
                } else {
                    if (0 == j) // Key �ߺ� �˻�..
                    {
                        pair_Key pk = KeySet.insert(iVal);
                        if (false == pk.second) {
                            char szErr[512];
                            sprintf(szErr, "Key -Duplicate- : Line %d, Key : %d, File : %s", i + 1, iVal,
                                    szTxtFileName.c_str());
                            MessageBox(hWnd, szErr, "Duplicate Key - Failed to append to table.", MB_OK);
                            break;
                        }
                    }

                    m_Datas[j].m_iValues.push_back(iVal);
                    m_Datas[j].m_dwValues.push_back(dwVal);
                    m_Datas[j].m_dfValues.push_back(dfVal);
                    m_Datas[j].m_Texts.push_back(szText);
                }
            } else {
                MessageBox(hWnd, szTxtFileName.c_str(), "The number of columns in Data is small or different.", MB_OK);
                fclose(pFile);
                m_Datas.clear();
                return false;
            }
        }
    }

    fclose(pFile);

    return true;
}

bool CTableGenerator::OpenReference_Enum(const std::string & szEnumFileName) {
    if (szEnumFileName.empty()) {
        return false;
    }

    HWND   hWnd = ::GetActiveWindow();
    HANDLE hFile =
        CreateFile(szEnumFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, szEnumFileName.c_str(), "File does not exist or cannot be read.", MB_OK);
        return false;
    }

    // ���Ͽ��� ���� �б�
    m_DataExts.clear();

    int   iDataCount = 0;
    DWORD dwNum;
    ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
    if (iDataCount <= 0) {
        CloseHandle(hFile);
        MessageBox(hWnd, szEnumFileName.c_str(), "There is no data type.", MB_OK);
        return false;
    }

    m_DataExts.resize(iDataCount);
    for (int i = 0; i < iDataCount; ++i) {
        ReadFile(hFile, &(m_DataExts[i].m_Type), sizeof(DATA_TYPE), &dwNum, NULL);
    }
    CloseHandle(hFile);

    // ���� �̸� ���..
    m_szEnmExt = szEnumFileName;

    return true;
}

bool CTableGenerator::OpenReference_Txt(int iIndex, const std::string & szTxtFileName) {
    if (szTxtFileName.empty()) {
        return false;
    }
    if (iIndex < 0 || iIndex >= MAX_ITEM_EXTENSION) {
        return false;
    }
    if (m_DataExts.size() <= 0) {
        return false;
    }

    HWND hWnd = ::GetActiveWindow();

    FILE * pFile = fopen(szTxtFileName.c_str(), "r");
    if (NULL == pFile) {
        MessageBox(hWnd, szTxtFileName.c_str(), "File does not exist or cannot be read.", MB_OK);
        return false;
    }

    char                                     szLine[1024];
    std::set<int>                            KeySet;
    typedef typename std::set<int>::iterator it_Key;
    typedef std::pair<it_Key, bool>          pair_Key;

    for (int i = 0; true; i++) {
        if (NULL == fgets(szLine, 1024, pFile)) {
            break;
        }
        if (NULL == szLine[0] || '\r' == szLine[0] || '\n' == szLine[0]) {
            continue;
        }

        int         iOffset = 0;
        int         iVal = 0;
        DWORD       dwVal = 0;
        double      dfVal = 0;
        std::string szText;

        int iDataCount = m_DataExts.size();
        for (int j = 0; j < iDataCount; j++) {
            int iSuccess =
                this->ParseLine(szLine, iOffset, iVal, dwVal, dfVal,
                                szText); // �����ϸ� 0, �߰��ؾ��ϰ� ���̸� -1 �߰��� �ʿ� ���� ���̸�.. -2 ����

            if (iSuccess > 0) {
                if (i == 0) // ù���� �����̴�..
                {
                    m_DataExts[j].m_szTitle = szText;
                } else {
                    if (0 == j) // Key �ߺ� �˻�..
                    {
                        pair_Key pk = KeySet.insert(iVal);
                        if (false == pk.second) {
                            char szErr[512];
                            sprintf(szErr, "Key -Duplicate- : Line %d, Key : %d, File : %s", i + 1, iVal,
                                    szTxtFileName.c_str());
                            MessageBox(hWnd, szErr, "Duplicate Key - Failed to append to table.", MB_OK);
                            break;
                        }
                    }

                    m_DataExts[j].m_iValues[iIndex].push_back(iVal);
                    m_DataExts[j].m_dwValues[iIndex].push_back(dwVal);
                    m_DataExts[j].m_dfValues[iIndex].push_back(dfVal);
                    m_DataExts[j].m_Texts[iIndex].push_back(szText);
                }
            } else {
                char szErr[512];
                sprintf(szErr, "FThe number of fields does not match or the string is incorrect: Line %d, Field: %d",
                        i + 1, j);
                MessageBox(hWnd, szErr, "Failed to append to the table.", MB_OK);
                fclose(pFile);
                //                m_DataExts[iIndex].clear();
                return false;
            }
        }
    }

    fclose(pFile);

    // ���� �̸� ���..
    m_szTxtExts[iIndex] = szTxtFileName;

    return true;
}

bool CTableGenerator::Generate(int iIndex, const std::string & szEnumFileName, const std::string & szTxtFileName) {
    int iIndexS = iIndex, iIndexE = iIndex;
    if (-1 == iIndex) {
        iIndexS = 0;
        iIndexE = MAX_ITEM_EXTENSION - 1;
    }

    if (iIndexS < 0 || iIndexS >= MAX_ITEM_EXTENSION || iIndexE < 0 || iIndexE >= MAX_ITEM_EXTENSION) {
        return false;
    }
    if (szTxtFileName.empty()) {
        return false;
    }

    HWND hWnd = ::GetActiveWindow();
    if (m_Datas.empty() || m_Datas[0].m_iValues.empty() || m_DataExts.empty()) {
        MessageBox(hWnd, "You need to read the base item table and the reference table.", "Item table creation failed.",
                   MB_OK);
        return false;
    }

    DWORD  dwRWC = 0;
    HANDLE hFile =
        CreateFile(szTxtFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, szTxtFileName.c_str(), "The file does not exist or is not writable.", MB_OK);
        return false;
    }

    std::vector<DATA_TYPE> DataTypesPrev; // Data Type ����� �س��´�..
    DataTypesPrev = m_DataTypes;
    m_DataTypes.clear();

    int iDTCountBasic = m_Datas.size();
    int iDTCountRef = m_DataExts.size();
    int iDTCount = iDTCountBasic + iDTCountRef;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Item Table ����....
    enum e_ItemGenerationIndex1 {
        IG1_KEY = 0,
        IG1_GEN_TYPE = 1,
        IG1_NAME = 2,
        IG1_REMARK = 3,
        IG1_PIC = 4,
        IG1_ICON = 5,
        IG1_SOUND0 = 6,
        IG1_SOUND1 = 7,
        IG1_ROBE_TYPE = 9,
        IG1_DAMAGE = 13,
        IG1_DELAY = 14,
        IG1_DURATION = 17,
        IG1_BUY_PRICE = 18,
        IG1_AC = 20,
        IG1_EFFECT1 = 22,
        IG1_EFFECT2 = 23,
        IG1_REQ_LEVEL = 24,
        IG1_REQ_RANK = 25,
        IG1_REQ_TITLE = 26,
        IG1_REQ_STR = 27,
        IG1_REQ_STA = 28,
        IG1_REQ_DEX = 29,
        IG1_REQ_INT = 30,
        IG1_REQ_CHA = 31,
        IG1_UNKNOWN = 0xffffffff
    };

    enum e_ItemGenerationIndex2 {
        IG2_KEY = 0,
        IG2_NAME_HEAD = 1,
        IG2_REMARK = 2,
        IG2_TYPE = 3,
        IG2_DAMAGE = 4,
        IG2_DELAY_PERCENT = 5,
        IG2_DURATION = 8,
        IG2_BUY_PRICE = 9,
        IG2_AC = 10,
        IG2_EFFECT1 = 39,
        IG2_EFFECT2 = 40,
        IG2_REQ_LEVEL = 41,
        IG2_REQ_RANK = 42,
        IG2_REQ_TITLE = 43,
        IG2_REQ_STR = 44,
        IG2_REQ_STA = 45,
        IG2_REQ_DEX = 46,
        IG2_REQ_INT = 47,
        IG2_REQ_CHA = 48,
        IG2_UNKNOWN = 0xffffffff
    };

    enum e_ItemGenerationType {
        IGT_WEAPON = 0,
        IGT_SHIELD = 1,
        IGT_CLOTH = 2,
        IGT_HELMET = 3,
        IGT_GLOVES_SHOES = 4,
        IGT_EARRING = 5,
        IGT_AMULET = 6,
        IGT_RING = 7,
        IGT_BELT = 8,
        IGT_UNKNONW = 0xffffffff
    };

    std::string szGrade[10] = {"",         "Bronze ", "Silver ", "Gold ",    "Platinium ",
                               "Crimson ", "Luna ",   "Solar ",  "Ancient ", "Mystic "};

    char szBuff[1024];
    int  iCountBasic = m_Datas[0].m_iValues.size();
    int  iCountRef = 0;
    for (int i = iIndexS; i <= iIndexE; i++) {
        iCountRef += m_DataExts[0].m_iValues[i].size();
    }
    int iCountWhole = iCountBasic * iCountRef;

    std::vector<std::string> * pLineArrays = new std::vector<
        std::string>[iCountWhole + 1]; // ���� ���ۿ� �� ���ڿ� �迭 �غ�.. +1�� �ϴ� ������ ù�ٿ� ���� �������� �Ѵ�.

    int iIndexCur1 = 0;
    int iIndexCur2 = 0;
    int iExt = iIndexS;
    int iCountWhole2 = 0; // ���� ó���� ����..
    int iAddTitle = -1;
    for (int i = 0; i < iCountWhole; i++) {
        int iType = m_Datas[IG1_GEN_TYPE].m_dwValues[iIndexCur1]; // ������ ����Ÿ��..

        // ������ ���� Ÿ�԰� Ȯ�� ���̺��� ������ ��ȣ�� �´°�츸 ó��..
        if (iType == iExt) {
            int                    iGrade = (m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2]) % 10; // ���� ���
            e_ItemGenerationIndex2 eIG2 = IG2_UNKNOWN;

            for (int j = 0; j < iDTCountBasic; j++) {
                DATA_TYPE dt = m_Datas[j].m_Type;
                // �ϵ� �ڵ�...
                switch (j) {
                case IG1_KEY: // �Ϸù�ȣ ����..
                    if (DT_DWORD == dt) {
                        //                        DWORD dwKey = ((m_Datas[j].m_dwValues[iIndexCur1] / 1000) * 1000) + m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2]; // ���ǰ� ���ڸ��� ��ġ�Ѵ�.
                        DWORD dwKey = m_Datas[j].m_dwValues[iIndexCur1] +
                                      m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2]; // �����ش�.
                        sprintf(szBuff, "%d", dwKey);
                    } else {
                        lstrcpy(szBuff, "Invalid Key");
                    }
                    break;

                case IG1_GEN_TYPE:
                    lstrcpy(szBuff, "");
                    break;

                case IG1_NAME: // ������ �̸�
                    if (DT_STRING == dt) {
                        sprintf(szBuff, "%s%s %s", szGrade[iGrade].c_str(),
                                m_DataExts[IG2_NAME_HEAD].m_Texts[iExt][iIndexCur2].c_str(),
                                m_Datas[j].m_Texts[iIndexCur1].c_str());
                    } else {
                        lstrcpy(szBuff, "Invalid Name");
                    }
                    break;

                case IG1_REMARK: // ������ ����.. �����ͷν�� �ʿ����.
                case IG1_PIC:
                case IG1_ICON:
                case IG1_SOUND0:
                case IG1_SOUND1:
                case IG1_ROBE_TYPE:
                    lstrcpy(szBuff, "");
                    break;

                case IG1_DAMAGE: // �ܼ� ���ϱ�.
                case IG1_AC:
                case IG1_EFFECT1:
                case IG1_EFFECT2:
                case IG1_DURATION:
                case IG1_REQ_LEVEL:
                case IG1_REQ_RANK:
                case IG1_REQ_TITLE:
                case IG1_REQ_STR:
                case IG1_REQ_STA:
                case IG1_REQ_DEX:
                case IG1_REQ_INT:
                case IG1_REQ_CHA: {
                    bool bApplyAbility =
                        false; // �䱸 �ɷ�ġ�� 0 �϶� �����ϰ�... ����� ���� 0 ���� ������ 0���� ǥ���Ұ�..

                    if (IG1_DAMAGE == j) {
                        eIG2 = IG2_DAMAGE;
                    } else if (IG1_AC == j) {
                        eIG2 = IG2_AC;
                    } else if (IG1_EFFECT1 == j) {
                        eIG2 = IG2_EFFECT1;
                    } else if (IG1_EFFECT2 == j) {
                        eIG2 = IG2_EFFECT2;
                    } else if (IG1_DURATION == j) {
                        eIG2 = IG2_DURATION;
                    } else if (IG1_REQ_LEVEL == j) {
                        eIG2 = IG2_REQ_LEVEL;
                        bApplyAbility = true;
                    } else if (IG1_REQ_RANK == j) {
                        eIG2 = IG2_REQ_RANK;
                        bApplyAbility = true;
                    } else if (IG1_REQ_TITLE == j) {
                        eIG2 = IG2_REQ_TITLE;
                        bApplyAbility = true;
                    } else if (IG1_REQ_STR == j) {
                        eIG2 = IG2_REQ_STR;
                        bApplyAbility = true;
                    } else if (IG1_REQ_STA == j) {
                        eIG2 = IG2_REQ_STA;
                        bApplyAbility = true;
                    } else if (IG1_REQ_DEX == j) {
                        eIG2 = IG2_REQ_DEX;
                        bApplyAbility = true;
                    } else if (IG1_REQ_INT == j) {
                        eIG2 = IG2_REQ_INT;
                        bApplyAbility = true;
                    } else if (IG1_REQ_CHA == j) {
                        eIG2 = IG2_REQ_CHA;
                        bApplyAbility = true;
                    } else {
                        lstrcpy(szBuff, "Invalid Value");
                        break;
                    }

                    if (DT_STRING != dt) {
                        int iValue = m_Datas[j].m_iValues[iIndexCur1] + m_DataExts[eIG2].m_iValues[iExt][iIndexCur2];
                        if (bApplyAbility) // �䱸 �ɷ�ġ�� ���..
                        {
                            if (0 == m_Datas[j].m_iValues[iIndexCur1]) {
                                iValue = 0; // �䱸 �ɷ�ġ�� 0 �̸� ������� �ʴ´�.
                            }
                            if (iValue < 0) {
                                iValue = 0;
                            }
                        }
                        sprintf(szBuff, "%d", iValue);
                    } else {
                        lstrcpy(szBuff, "Invalid Value");
                    }
                } break;

                case IG1_BUY_PRICE: // ������ ���� - ���ϱ�..
                    if (DT_STRING != dt) {
                        int iValue =
                            m_Datas[j].m_iValues[iIndexCur1] * m_DataExts[IG2_BUY_PRICE].m_iValues[iExt][iIndexCur2];
                        sprintf(szBuff, "%d", iValue);
                    } else {
                        lstrcpy(szBuff, "Invalid BuyPrice");
                    }
                    break;
                case IG1_DELAY: // ���� �ӵ� Percentage ���.
                    eIG2 = IG2_DELAY_PERCENT;
                    if (DT_STRING != dt) {
                        int iValue = 0;
                        if (0 == m_DataExts[eIG2].m_iValues[iExt][iIndexCur2]) // Devide By 0..
                        {
                            //                            lstrcpy(szBuff, "Invalid Percentage");
                            lstrcpy(szBuff, "100");
                        } else {
                            iValue =
                                m_Datas[j].m_iValues[iIndexCur1] * m_DataExts[eIG2].m_iValues[iExt][iIndexCur2] / 100;
                            sprintf(szBuff, "%d", iValue);

                            int iDamage = m_Datas[IG1_DAMAGE].m_iValues[iIndexCur1];
                            if (iDamage > 0) // �����϶� ���� �ӵ��� �˻��� ����..
                            {
                                if (iValue < 50) {
                                    char szErr[256];
                                    sprintf(szErr,
                                            "The weapon's attack speed is less than 50.\n    Item name : %s\n    %dth "
                                            "Extended table, %d column\n    attack speed : %d",
                                            m_Datas[IG1_NAME].m_Texts[iIndexCur1].c_str(), iExt, iIndexCur2, iValue);
                                    MessageBox(::GetActiveWindow(), szErr, "Table Creation Warning", MB_OK);
                                }
                            }
                        }
                    } else {
                        lstrcpy(szBuff, "Invalid Percentage");
                    }
                    break;

                default:
                    switch (dt) {
                    case DT_CHAR:
                    case DT_SHORT:
                    case DT_INT:
                        sprintf(szBuff, "%d", m_Datas[j].m_iValues[iIndexCur1]);
                        break;
                    case DT_BYTE:
                    case DT_WORD:
                    case DT_DWORD:
                        sprintf(szBuff, "%d", m_Datas[j].m_dwValues[iIndexCur1]);
                        break;
                    case DT_STRING:
                        sprintf(szBuff, "%s", m_Datas[j].m_Texts[iIndexCur1].c_str());
                        break;
                    case DT_FLOAT:
                    case DT_DOUBLE:
                        sprintf(szBuff, "%f", m_Datas[j].m_dfValues[iIndexCur1]);
                        break;
                    default:
                        lstrcpy(szBuff, "Invalid Type");
                        break;
                    }
                    break;
                }

                if (lstrlen(szBuff) > 0) {
                    pLineArrays[i + 1].push_back(szBuff); // ���ڿ��� �־�� �ִ´�.

                    if (iAddTitle < 0) {
                        iAddTitle = i;
                    }
                    if (i == iAddTitle) {
                        pLineArrays[0].push_back(m_Datas[j].m_szTitle); // ���� �ֱ�..
                        m_DataTypes.push_back(dt);
                    }
                }
            } // end of for(int j = 0; j < iDTCountBasic; j++)

            for (int j = 0; j < iDTCountRef; j++) {
                DATA_TYPE dt = m_DataExts[j].m_Type;

                switch (j) {
                case IG2_KEY:
                case IG2_NAME_HEAD:
                case IG2_REMARK:
                case IG2_DAMAGE:
                case IG2_DELAY_PERCENT:
                case IG2_DURATION:
                case IG2_BUY_PRICE:
                case IG2_AC:
                case IG2_EFFECT1:
                case IG2_EFFECT2:
                case IG2_REQ_LEVEL:
                case IG2_REQ_RANK:
                case IG2_REQ_TITLE:
                case IG2_REQ_STR:
                case IG2_REQ_STA:
                case IG2_REQ_DEX:
                case IG2_REQ_INT:
                case IG2_REQ_CHA:
                    lstrcpy(szBuff, "");
                    break;

                default:
                    switch (dt) {
                    case DT_CHAR:
                    case DT_SHORT:
                    case DT_INT:
                        sprintf(szBuff, "%d", m_DataExts[j].m_iValues[iExt][iIndexCur2]);
                        break;
                    case DT_BYTE:
                    case DT_WORD:
                    case DT_DWORD:
                        sprintf(szBuff, "%d", m_DataExts[j].m_dwValues[iExt][iIndexCur2]);
                        break;
                    case DT_STRING:
                        sprintf(szBuff, "%s", m_DataExts[j].m_Texts[iExt][iIndexCur2].c_str());
                        break;
                    case DT_FLOAT:
                    case DT_DOUBLE:
                        sprintf(szBuff, "%f", m_DataExts[j].m_dfValues[iExt][iIndexCur2]);
                        break;
                    default:
                        lstrcpy(szBuff, "Invalid Type");
                        break;
                    }
                    break;
                }

                if (lstrlen(szBuff) > 0) {
                    if (i == iAddTitle) {
                        pLineArrays[0].push_back(m_DataExts[j].m_szTitle); // ���� �ֱ�..
                        m_DataTypes.push_back(dt);                         // ������ Ÿ�� �ֱ�..
                    }
                    pLineArrays[i + 1].push_back(szBuff); // ���ڿ��� �־�� �ִ´�.
                }
            }

            iCountWhole2++; // ���� ó���� ���� ����..
        }                   // end of if(iType == iExt) // ���� Ÿ�԰� Ȯ�� ���̺��� ������ ��ȣ�� �´°�츸 ó��..

        // �ε��� ���..
        iIndexCur2++;
        if (iIndexCur2 >= m_DataExts[0].m_iValues[iExt].size()) {
            iIndexCur2 = 0;
            iIndexCur1++;
            if (iIndexCur1 >= m_Datas[0].m_iValues.size()) {
                iIndexCur1 = 0;
                iExt++;
            }
        }
    } // end of for(int i = 0; i < iCountWhole; i++)

    int iColCount = pLineArrays[0].size();
    for (int i = 0; i <= iCountWhole; i++) // �������� ����.
    {
        if (pLineArrays[i].empty()) {
            continue;
        }

        for (int j = 0; j < iColCount; j++) {
            WriteFile(hFile, pLineArrays[i][j].c_str(), pLineArrays[i][j].size(), &dwRWC, NULL);
            if (j < iColCount - 1) {
                WriteFile(hFile, "\t", 1, &dwRWC, NULL); // �� ����..
            } else {
                WriteFile(hFile, "\r\n", 2, &dwRWC, NULL); // �� �ѱ��.
            }
        }
    }
    // Item Table ����....
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    delete[] pLineArrays;
    pLineArrays = NULL;
    CloseHandle(hFile);

    // Data Type ����..
    this->DataTypeSave(szEnumFileName);
    m_DataTypes = DataTypesPrev; // ��� �����ɷ� �������´�.
    return true;
}

int CTableGenerator::ParseLine(
    const char * szLine, int & iOffset, int & iVal, DWORD & dwVal, double & dfVal,
    std::string & szText) // �����ϸ� 0, �߰��ؾ��ϰ� ���̸� -1 �߰��� �ʿ� ���� ���̸�.. -2 ����
{
    const char * szLine2 = szLine + iOffset;

    if ('\r' == *szLine2 || '\n' == *szLine2) {
        iOffset++;
        return 0;
    }

    iVal = 0;
    dwVal = 0;
    dfVal = 0;
    szText = "";

    while (true) {
        if ('\t' == *szLine2) {
            iVal = atoi(szText.c_str());
            dwVal = atol(szText.c_str());
            dfVal = atof(szText.c_str());
            iOffset++;
            return 1;
        } else if ('\r' == *szLine2 || '\n' == *(szLine2)) // �����ǵ� + CR -> ������ ���̴�..
        {
            iVal = atoi(szText.c_str());
            dwVal = atol(szText.c_str());
            dfVal = atof(szText.c_str());
            iOffset++;
            return 2;
        } else if (NULL == *szLine2) {
            iOffset++;
            return 0;
        }

        szText += *szLine2;
        szLine2++;
        iOffset++;
    }

    return 0;
}

bool CTableGenerator::DataTypeSave(const std::string & szFN) {
    HANDLE hFile = CreateFile(szFN.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HWND   hWnd = ::GetActiveWindow();
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, "The file could not be created.", "Save data types.", MB_OK);
        return false;
    }

    DWORD dwNum;
    int   iDataCount = m_DataTypes.size();
    WriteFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
    for (int i = 0; i < iDataCount; ++i) {
        DATA_TYPE datatype = m_DataTypes[i];
        WriteFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL);
    }

    CloseHandle(hFile);
    return true;
}

bool CTableGenerator::DataTypeLoad(const std::string & szFN) {
    HWND   hWnd = ::GetActiveWindow();
    HANDLE hFile = CreateFile(szFN.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, "File does not exist or cannot be read.", "Call data type.", MB_OK);
        return false;
    }
    m_DataTypes.clear();

    // ���Ͽ��� ���� �б�
    int   iDataCount = 0;
    DWORD dwNum;
    ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
    if (iDataCount > 0) {
        ASSERT(0 == m_DataTypes.size());
        m_DataTypes.assign(iDataCount, DT_NONE);
        for (int i = 0; i < iDataCount; ++i) {
            DATA_TYPE datatype;
            ReadFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL);
            m_DataTypes[i] = datatype;
        }
    }
    CloseHandle(hFile);

    return true;
}

bool CTableGenerator::Convert2Bin(const std::string & szFN) {
    HWND   hWnd = ::GetActiveWindow();
    FILE * stream = fopen(szFN.c_str(), "r");
    if (NULL == stream) {
        MessageBox(hWnd, "Unreadable file.", "Convert Error", MB_OK);
        return false;
    }
    const int iMaxStrLen = 4096;
    char      line[iMaxStrLen + 1];

    // �� ������ column�� �̸����� ������
    if (fgets(line, iMaxStrLen, stream) == NULL) {
        fclose(stream);
        std::string szMsg = szFN + " - Unable to read file contents.";
        MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
        return false;
    }
    // column�� ��ΰ� Ȯ���غ���
    int    iDataCount = m_DataTypes.size();
    int    iCount = 0;
    char * token = MyToken(line);
    while (NULL != token) {
        ++iCount;
        token = MyToken(NULL);
    }
    if (iCount != iDataCount) {
        fclose(stream);
        std::string szMsg = szFN + " - The file data and the number of data you set do not match.\nPlease check again!";
        MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
        return false;
    }
    // row �� �� ������ �����
    iCount = 0;
    bool bQuotationActived = false;
    while (fgets(line, iMaxStrLen, stream)) {
        char * pcFind = line;
        pcFind = strchr(pcFind, '\"'); // ����ǥ �����ִ��� �˻�.
        while (pcFind) {
            if (*(pcFind - 1) != '\t' && *(pcFind + 1) != '\t' && *(pcFind + 1) != '\n') {
                MessageBox(hWnd, "There is a quote in an unexpected place..", "Convert Error", MB_OK);
                return false;
            }
            bQuotationActived = !bQuotationActived;
            pcFind = strchr(pcFind + 1, '\"');
        }

        if (!bQuotationActived) {
            ++iCount; // ����ǥ �����ִµ� �� �ٲ�� ī��Ʈ�� ���� �ʴ´�.
        }
    }

    // binary file �����ϱ�
    char szFName[_MAX_PATH], szDrv[_MAX_DRIVE], szPath[_MAX_PATH];
    char szDestFN[_MAX_PATH];
    _splitpath(szFN.c_str(), szDrv, szPath, szFName, NULL);
    _makepath(szDestFN, szDrv, szPath, szFName, ".tbl");
    HANDLE hFile = CreateFile(szDestFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        fclose(stream);
        std::string szMsg = szDestFN;
        szMsg += " - The file could not be created.";
        MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
        return false;
    }

    // data(column) �� ������ ��� �Ǿ� �ִ��� �����ϱ�
    DWORD dwNum;
    WriteFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL); // (�������� column ��)
    for (int i = 0; i < iDataCount; ++i) {
        DATA_TYPE datatype = m_DataTypes[i];
        WriteFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL); // ������ column�� �ش��ϴ� data type
    }
    // row �� �������� ����
    WriteFile(hFile, &iCount, sizeof(iCount), &dwNum, NULL);

    // txt ���Ͽ��� table �������� �ٽ� �б�
    int iRet = fseek(stream, 0, SEEK_SET);
    ASSERT(0 == iRet);
    fgets(line, iMaxStrLen, stream); // ó�� ������ �׳� �б� (��, 2��° �ٺ��� �ٽ� �б�)

    // Ű �ߺ� �˻�� ������..
    std::set<int>                            KeySet;
    typedef typename std::set<int>::iterator it_Key;
    typedef std::pair<it_Key, bool>          pair_Key;

    BOOL bCheckEmptyValue = FALSE;
    bQuotationActived = false;
    std::string strValueBuffer; // ���� �ٿ� ������ ��Ʈ���� ������ ����
    int         iRowCount = 0;
    int         iColCount = 0;
    while (iRowCount < iCount) {
        if (!fgets(line, iMaxStrLen, stream)) {
            break;
        }
        int iStrLen = lstrlen(line);
        ASSERT(iStrLen > 0);
        if (line[iStrLen - 1] == '\n') {
            line[iStrLen - 1] = '\0'; // \n���� ���ֱ�
        }

        token = MyToken(line);

        int      iKey = atoi(token);
        pair_Key pk = KeySet.insert(iKey);
        if (false == pk.second) {
            char szErr[512];
            sprintf(szErr, "Key -Duplicate- : Line %d, Key : %d, File : %s", iRowCount + 1, iKey, szFN.c_str());
            MessageBox(hWnd, szErr, "Duplicate Key - Failed to append to table.", MB_OK);
            CloseHandle(hFile);
            fclose(stream);
            ASSERT(0);
            return false;
        }

        //for (int j=0; j<m_iDataCount; ++j)
        while (iColCount < iDataCount) {
            if (bQuotationActived) { // ���ڿ��̸�
                if (NULL == token) {
                    // �ٹٲ�
                    strValueBuffer += "\n";
                    break;
                } else {
                    int iLast = lstrlen(token) - 1;
                    if (iLast >= 0 && '\"' == token[iLast]) { // ����ǥ�� ������ ���ڿ� ���̴�.
                        bQuotationActived = false;
                        strValueBuffer += std::string(token).substr(0, iLast);

                        // ���ۿ� �ִ°� ���
                        if (FALSE == WriteData(hFile, m_DataTypes[iColCount], strValueBuffer.c_str())) {
                            char szErr[512];
                            wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
                            MessageBox(
                                hWnd, szErr,
                                "There are some cases where data logging is ignored. This file will not work properly.",
                                MB_OK);
                            CloseHandle(hFile);
                            fclose(stream);
                            ASSERT(0);
                            return false;
                        }
                        token = MyToken(NULL);
                        ++iColCount;
                        continue;
                    } else { // ����ǥ�� ������ ���ڿ��� ��� ���ذ���.
                        strValueBuffer += token;
                        token = MyToken(NULL);
                        continue;
                    }
                }
            } else if (token && '\"' == token[0]) // bQuotationActived �� false�̰� '\"' == token[0] �̸�
            {                                     // ���ڿ��� ���۵Ǿ���.
                int iLast = lstrlen(token) - 1;
                if (iLast >= 1 && '\"' == token[iLast]) // iLast>=1 �� ������ ��ū�� "�� 2������ Ȯ���� �ϱ� ���ؼ�.
                {                                       // ���� ��ū���� ���ڿ��� ������.
                    strValueBuffer = std::string(token).substr(1, iLast - 1);

                    // ���ۿ� �ִ°� ���
                    if (FALSE == WriteData(hFile, m_DataTypes[iColCount], strValueBuffer.c_str())) {
                        char szErr[512];
                        wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
                        MessageBox(
                            hWnd, szErr,
                            "There are some cases where data logging is ignored. This file will not work properly.",
                            MB_OK);
                        CloseHandle(hFile);
                        fclose(stream);
                        ASSERT(0);
                        return false;
                    }
                    token = MyToken(NULL);
                    ++iColCount;
                    continue;
                } else { // �̹� ��ū���� ���ڿ��� ������ �ʾҴ�.
                    bQuotationActived = true;
                    strValueBuffer = (token + 1); // +1�� ������ ����ǥ�� �������� �ʴ´�.
                    token = MyToken(NULL);
                    continue;
                }
            }

            ASSERT(token || (iColCount + 1) == iDataCount); // �� �������� �����Ͱ� ������ NULL�� �� �� �ֵ�.

            if (FALSE == (token || (NULL == token && DT_STRING == m_DataTypes[iColCount])) &&
                FALSE == bCheckEmptyValue) {
                int iSelect = MessageBox(hWnd,
                                         "There is a blank space in the table other than the string.\nYes : Stop\nNo : "
                                         "Ignore this time only "
                                         "Continue\n Cancel : Continue ignoring all",
                                         "Convert Error", MB_YESNOCANCEL);
                if (IDYES == iSelect) {
                    MessageBox(hWnd, "Data logging was aborted.This file will not work properly.", "Convert Error",
                               MB_OK);
                    CloseHandle(hFile);
                    fclose(stream);
                    ASSERT(0);
                    return false;
                } else if (IDCANCEL == iSelect) {
                    bCheckEmptyValue = TRUE;
                }
            }

            if (FALSE == WriteData(hFile, m_DataTypes[iColCount], token)) {
                char szErr[512];
                wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
                MessageBox(hWnd, szErr,
                           "There are some cases where data logging is ignored. This file will not work properly.",
                           MB_OK);
                CloseHandle(hFile);
                fclose(stream);
                ASSERT(0);
                return false;
            }
            token = MyToken(NULL);
            ++iColCount;
        }
        if (bQuotationActived) {
            continue; // ����ǥ�� ���� ������ ��� ���ڿ��̹Ƿ� �ٽ� ����..
        }
        if (iDataCount != iColCount) {
            char szErr[512];
            wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
            MessageBox(hWnd, szErr,
                       "There is an item that does not match the currently set data item and the recorded data item.",
                       MB_OK);
            CloseHandle(hFile);
            fclose(stream);
            ASSERT(0);
            return false;
        }
        iColCount = 0;
        ++iRowCount;
    }
    CloseHandle(hFile);
    fclose(stream);

    ////////////////////////////////////////////////////////////
    // ��ȣȭ.. ������ Ű�� �����ϴ�...

    hFile = ::CreateFile(szDestFN, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        MessageBox(hWnd, szDestFN, "Failed to create encryption file.", MB_OK);
        remove(szDestFN); // ���� �����..
        return false;
    }

    // ���� ��ȣȭ �ϱ�.. .. ������ ���� ����..
    DWORD dwSizeHigh = 0;
    DWORD dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
    if (dwSizeLow <= 0) {
        CloseHandle(hFile);
        ::remove(szDestFN); // �ӽ� ���� �����..
        return false;
    }

    // ���� ������ �а�..
    BYTE * pDatas = new BYTE[dwSizeLow];
    DWORD  dwRWC = 0;
    ::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // ��ȣȭ�� ������ �а�..
    CloseHandle(hFile);                                 // ���� ���� �ݰ�

    // ���̺� ����� ������ ���� Ű�� ���� Ű..
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

    // ��ȣȭ ���ڵ�...
    for (int i = 0; i < dwSizeLow; i++) {
        BYTE byData = (pDatas[i] ^ (key_r >> 8));
        key_r = (byData + key_r) * key_c1 + key_c2;
        pDatas[i] = byData;
    }

    // �ٽ� ������ ����..
    hFile = ::CreateFile(szDestFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // ���Ͽ� ��ȣȭ ��Ų ������ ����
    CloseHandle(hFile);                                  // ���� �ݱ�
    delete[] pDatas;
    pDatas = NULL;

    if (iCount == iRowCount) {
        char szErr[512];
        wsprintf(szErr, "%d data records completed.", iRowCount);
        MessageBox(hWnd, szErr, "Convert Report", MB_OK);
        return true;
    } else {
        char szErr[512];
        wsprintf(szErr, "Of all %d data, only %d data have been written.\nThis table will not work properly..", iCount,
                 iRowCount);
        MessageBox(hWnd, szErr, "Data count error.", MB_OK);
        return false;
    }
}

// ���Ͽ� ����Ÿ Ÿ�Ժ��� ����..
bool CTableGenerator::WriteData(HANDLE hFile, DATA_TYPE DataType, LPCTSTR lpszData) {
    // ���� lpszData == NULL�̸� datatype�� string�� ��쿡�� �ȴ�.
    //    ASSERT(lpszData || (NULL == lpszData && DT_STRING == DataType));

    DWORD dwNum;
    switch (DataType) {
    case DT_CHAR: {
        char cWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0]) || '-' == lpszData[0]) {
                int iTemp = atoi(lpszData);
                if (iTemp < -127 || iTemp > 128) {
                    return false; // ������ �����~
                }
                cWrite = (char)iTemp;
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &cWrite, sizeof(cWrite), &dwNum, NULL);
    } break;
    case DT_BYTE: {
        BYTE byteWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0])) {
                int iTemp = atoi(lpszData);
                if (iTemp < 0 || iTemp > 255) {
                    return false; // ������ �����~
                }
                byteWrite = (BYTE)iTemp;
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &byteWrite, sizeof(byteWrite), &dwNum, NULL);
    } break;
    case DT_SHORT: {
        short iWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0]) || '-' == lpszData[0]) {
                int iTemp = atoi(lpszData);
                if (iTemp < -32767 || iTemp > 32768) {
                    return false; // ������ �����~
                }
                iWrite = (short)iTemp;
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_WORD: {
        WORD iWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0])) {
                int iTemp = atoi(lpszData);
                if (iTemp < 0 || iTemp > 65535) {
                    return false; // ������ �����~
                }
                iWrite = (short)iTemp;
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_INT: {
        int iWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0]) || '-' == lpszData[0]) {
                iWrite = atoi(lpszData);
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_DWORD: {
        DWORD iWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0])) {
                iWrite = strtoul(lpszData, NULL, 10);
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }

        WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
    } break;
    case DT_STRING: {
        int iStrLen = 0;
        if (lpszData) {
            iStrLen = lstrlen(lpszData);
        }
        WriteFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);
        if (iStrLen > 0) {
            WriteFile(hFile, lpszData, iStrLen, &dwNum, NULL);
        }
    } break;
    case DT_FLOAT: {
        float fWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0]) {
                fWrite = (float)atof(lpszData);
            } else {
                return false; // ���ڴ� �ȵ�~!
            }
        }
        WriteFile(hFile, &fWrite, sizeof(fWrite), &dwNum, NULL);
    } break;
    case DT_DOUBLE: {
        double dWrite = 0;
        if (lpszData) {
            if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0]) {
                dWrite = atof(lpszData);
            } else {
                return false;
            }
            WriteFile(hFile, &dWrite, sizeof(dWrite), &dwNum, NULL);
        }
    } break;

    case DT_NONE:
    default:
        ASSERT(0);
    }
    return true;
}

// binary ���Ͽ��� ����Ÿ Ÿ�Ժ��� �б�
bool CTableGenerator::ReadData(HANDLE hFile, DATA_TYPE DataType, LPTSTR lpszData) {
    return true;
}

// ������ ���е� ���ڿ��� ã�´�.
char * CTableGenerator::MyToken(LPCTSTR lpszInput) {
    static char   szLine[4096] = "";
    static char * pszCur = NULL;
    if (lpszInput) {
        lstrcpy(szLine, lpszInput);
        pszCur = szLine;
    }
    int iStrLen = 0;
    if (pszCur) {
        iStrLen = lstrlen(pszCur);
    }
    if (iStrLen <= 0) {
        return NULL;
    }
    char * pcFind = strchr(pszCur, '\t');

    if (NULL == pcFind) {
        char * pszReturn = pszCur;
        pszCur = NULL;
        return pszReturn;
    } else {
        *pcFind = '\0';
        char * pszReturn = pszCur;
        pszCur = pcFind + 1;
        return pszReturn;
    }
}

void CTableGenerator::Release() {
    m_szEnmBasic = "";
    m_szTxtBasic = "";
    m_szEnmExt = "";
    for (int i = 0; i < MAX_ITEM_EXTENSION; i++) {
        m_szTxtExts[i] = "";
    }

    m_DataTypes.clear();
    m_Datas.clear();
    m_DataExts.clear();
}

bool CTableGenerator::DataTypeAssign(int iCount, DATA_TYPE Type) {
    if (iCount < 0) {
        return false;
    }

    m_DataTypes.clear();
    m_DataTypes.assign(iCount, Type);
    return true;
}

bool CTableGenerator::DataTypeInsert(int iIndex, DATA_TYPE Type) {
    if (iIndex < 0 || iIndex >= m_DataTypes.size()) {
        return false;
    }

    std::vector<DATA_TYPE>::iterator it = m_DataTypes.begin(), itEnd = m_DataTypes.end();
    for (int i = 0; it != itEnd; it++, i++) {
        if (iIndex == i) {
            it = m_DataTypes.insert(it, DT_STRING);
            return true;
        }
    }
    return false;
}

bool CTableGenerator::DataTypeDelete(int iIndex) {
    if (iIndex < 0 || iIndex >= m_DataTypes.size()) {
        return false;
    }

    std::vector<DATA_TYPE>::iterator it = m_DataTypes.begin(), itEnd = m_DataTypes.end();
    for (int i = 0; it != itEnd; it++, i++) {
        if (iIndex == i) {
            it = m_DataTypes.erase(it);
            return true;
        }
    }

    return false;
}
