// TableGenerator.cpp: implementation of the CTableGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableGenerator.h"
#include <set>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
CTableGenerator::CTableGenerator()
{
}

CTableGenerator::~CTableGenerator()
{
}

bool CTableGenerator::OpenSource(const std::string &szEnumFileName, const std::string &szTxtFileName)
{
	if(szEnumFileName.empty() || szTxtFileName.empty()) return false;

	HWND hWnd = ::GetActiveWindow();
	HANDLE hFile = CreateFile(szEnumFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szEnumFileName.c_str(), "파일이 없거나 읽을 수 없습니다.", MB_OK);
		return false;
	}

	// 파일에서 정보 읽기
	m_Datas.clear();
	int iDataCount = 0;
	DWORD dwNum;
	ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
	if(iDataCount <= 0)
	{
		CloseHandle(hFile);
		MessageBox(hWnd, szEnumFileName.c_str(), "Data Type 이 없습니다.", MB_OK);
		return false;
	}

	m_Datas.assign(iDataCount);
	for(int i=0; i<iDataCount; ++i)
	{
		ReadFile(hFile, &(m_Datas[i].m_Type), sizeof(DATA_TYPE), &dwNum, NULL);
	}
	CloseHandle(hFile);

	FILE* pFile = fopen(szTxtFileName.c_str(), "r");
	if (NULL == pFile)
	{
		MessageBox(hWnd, szTxtFileName.c_str(), "파일이 없거나 읽을 수 없습니다.", MB_OK);
		return false;
	}

	// 파일 이름 기록..
	m_szEnmBasic = szEnumFileName;
	m_szTxtBasic = szTxtFileName;

	char szLine[1024];
	std::set<int> KeySet;
	typedef std::set<int>::iterator it_Key;
	typedef std::pair<it_Key, bool> pair_Key;

	for(i = 0; true; i++)
	{
		if(NULL == fgets(szLine, 1024, pFile)) break;
		if(NULL == szLine[0] || '\r' == szLine[0] || '\n' == szLine[0]) continue;

		int iOffset = 0;
		int iVal = 0;
		DWORD dwVal = 0;
		double dfVal = 0;
		std::string szText;

		for(int j = 0; j < iDataCount; j++)
		{
			int iSuccess = this->ParseLine(szLine, iOffset, iVal, dwVal, dfVal, szText);  // 성공하면 0, 추가해야하고 끝이면 -1 추가할 필요 없고 끝이면.. -2 리턴

			if(iSuccess > 0)
			{
				if(i == 0) // 첫줄은 설명이다..
				{
					m_Datas[j].m_szTitle = szText;
				}
				else
				{
					if(0 == j) // Key 중복 검사..
					{
						pair_Key pk = KeySet.insert(iVal);
						if(false == pk.second)
						{
							char szErr[512];
							sprintf(szErr, "Key 중복 : Line %d, Key : %d, File : %s", i + 1, iVal, szTxtFileName.c_str());
							MessageBox(hWnd, szErr, "Key 중복 - 테이블에 추가 실패", MB_OK);
							break;
						}
					}

					m_Datas[j].m_iValues.push_back(iVal);
					m_Datas[j].m_dwValues.push_back(dwVal);
					m_Datas[j].m_dfValues.push_back(dfVal);
					m_Datas[j].m_Texts.push_back(szText);
				}
			}
			else
			{
				MessageBox(hWnd, szTxtFileName.c_str(), "Data 의 열 갯수가 적거나 다릅니다.", MB_OK);
				fclose(pFile);
				m_Datas.clear();
				return false;
			}
		}
	}

	fclose(pFile);

	return true;
}

bool CTableGenerator::OpenReference_Enum(const std::string &szEnumFileName)
{
	if(szEnumFileName.empty()) return false;

	HWND hWnd = ::GetActiveWindow();
	HANDLE hFile = CreateFile(szEnumFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szEnumFileName.c_str(), "파일이 없거나 읽을 수 없습니다.", MB_OK);
		return false;
	}

	// 파일에서 정보 읽기
	m_DataExts.clear();
	
	int iDataCount = 0;
	DWORD dwNum;
	ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
	if(iDataCount <= 0)
	{
		CloseHandle(hFile);
		MessageBox(hWnd, szEnumFileName.c_str(), "Data Type 이 없습니다.", MB_OK);
		return false;
	}

	m_DataExts.assign(iDataCount);
	for(int i=0; i<iDataCount; ++i)
	{
		ReadFile(hFile, &(m_DataExts[i].m_Type), sizeof(DATA_TYPE), &dwNum, NULL);
	}
	CloseHandle(hFile);

	// 파일 이름 기록..
	m_szEnmExt = szEnumFileName;

	return true;
}

bool CTableGenerator::OpenReference_Txt(int iIndex, const std::string &szTxtFileName)
{
	if(szTxtFileName.empty()) return false;
	if(iIndex < 0 || iIndex >= MAX_ITEM_EXTENSION) return false;
	if(m_DataExts.size() <= 0) return false;

	HWND hWnd = ::GetActiveWindow();

	FILE* pFile = fopen(szTxtFileName.c_str(), "r");
	if (NULL == pFile)
	{
		MessageBox(hWnd, szTxtFileName.c_str(), "파일이 없거나 읽을 수 없습니다.", MB_OK);
		return false;
	}

	char szLine[1024];
	std::set<int> KeySet;
	typedef std::set<int>::iterator it_Key;
	typedef std::pair<it_Key, bool> pair_Key;

	for(int i = 0; true; i++)
	{
		if(NULL == fgets(szLine, 1024, pFile)) break;
		if(NULL == szLine[0] || '\r' == szLine[0] || '\n' == szLine[0]) continue;

		int iOffset = 0;
		int iVal = 0;
		DWORD dwVal = 0;
		double dfVal = 0;
		std::string szText;

		int iDataCount = m_DataExts.size();
		for(int j = 0; j < iDataCount; j++)
		{
			int iSuccess = this->ParseLine(szLine, iOffset, iVal, dwVal, dfVal, szText);  // 성공하면 0, 추가해야하고 끝이면 -1 추가할 필요 없고 끝이면.. -2 리턴

			if(iSuccess > 0)
			{
				if(i == 0) // 첫줄은 설명이다..
				{
					m_DataExts[j].m_szTitle = szText;
				}
				else
				{
					if(0 == j) // Key 중복 검사..
					{
						pair_Key pk = KeySet.insert(iVal);
						if(false == pk.second)
						{
							char szErr[512];
							sprintf(szErr, "Key 중복 : Line %d, Key : %d, File : %s", i + 1, iVal, szTxtFileName.c_str());
							MessageBox(hWnd, szErr, "Key 중복 - 테이블에 추가 실패", MB_OK);
							break;
						}
					}

					m_DataExts[j].m_iValues[iIndex].push_back(iVal);
					m_DataExts[j].m_dwValues[iIndex].push_back(dwVal);
					m_DataExts[j].m_dfValues[iIndex].push_back(dfVal);
					m_DataExts[j].m_Texts[iIndex].push_back(szText);
				}
			}
			else
			{
				char szErr[512];
				sprintf(szErr, "Field 갯수가 맞지 않거나 문자열이 이상합니다 : Line %d, Field : %d", i + 1, j);
				MessageBox(hWnd, szErr, "테이블에 추가 실패", MB_OK);
				fclose(pFile);
//				m_DataExts[iIndex].clear();
				return false;
			}
		}
	}

	fclose(pFile);

	// 파일 이름 기록..
	m_szTxtExts[iIndex] = szTxtFileName;

	return true;
}

bool CTableGenerator::Generate(int iIndex, const std::string& szEnumFileName, const std::string &szTxtFileName)
{
	int iIndexS = iIndex, iIndexE = iIndex;
	if(-1 == iIndex)
	{
		iIndexS = 0;
		iIndexE = MAX_ITEM_EXTENSION - 1;
	}

	if(iIndexS < 0 || iIndexS >= MAX_ITEM_EXTENSION || iIndexE < 0 || iIndexE >= MAX_ITEM_EXTENSION) return false;
	if(szTxtFileName.empty()) return false;

	HWND hWnd = ::GetActiveWindow();
	if(m_Datas.empty() || m_Datas[0].m_iValues.empty() || m_DataExts.empty())
	{
		MessageBox(hWnd, "기본 아이템 테이블과 참조 테이블을 읽어야 합니다.", "아이템 테이블 생성 실패", MB_OK);
		return false;
	}

	DWORD dwRWC = 0;
	HANDLE hFile = CreateFile(szTxtFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szTxtFileName.c_str(), "파일이 없거나 쓸수 없습니다.", MB_OK);
		return false;
	}

	std::vector<DATA_TYPE> DataTypesPrev; // Data Type 백업을 해놓는다..
	DataTypesPrev = m_DataTypes;
	m_DataTypes.clear();

	int iDTCountBasic = m_Datas.size();
	int iDTCountRef = m_DataExts.size();
	int iDTCount = iDTCountBasic + iDTCountRef;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Item Table 조합....
	enum e_ItemGenerationIndex1 {	IG1_KEY = 0,			IG1_GEN_TYPE = 1,		IG1_NAME = 2,			IG1_REMARK = 3,			IG1_PIC = 4,
									IG1_ICON = 5,			IG1_SOUND0 = 6,			IG1_SOUND1 = 7,			IG1_ROBE_TYPE = 9,		IG1_DAMAGE = 13,		
									IG1_DELAY = 14, 		IG1_DURATION = 17,		IG1_BUY_PRICE = 18, 	IG1_AC = 20,			IG1_EFFECT1 = 22,
									IG1_EFFECT2 = 23,		IG1_REQ_LEVEL = 24, 	IG1_REQ_RANK = 25, 		IG1_REQ_TITLE = 26,		IG1_REQ_STR = 27,
									IG1_REQ_STA = 28,		IG1_REQ_DEX = 29,		IG1_REQ_INT = 30, 		IG1_REQ_CHA = 31,
									IG1_UNKNOWN = 0xffffffff };

	enum e_ItemGenerationIndex2 {	IG2_KEY = 0,			IG2_NAME_HEAD = 1,	IG2_REMARK = 2,		IG2_TYPE = 3,		IG2_DAMAGE = 4,
									IG2_DELAY_PERCENT = 5,	IG2_DURATION = 8, 	IG2_BUY_PRICE = 9,	IG2_AC = 10,		IG2_EFFECT1 = 39,
									IG2_EFFECT2 = 40,		IG2_REQ_LEVEL = 41, IG2_REQ_RANK = 42,	IG2_REQ_TITLE = 43, IG2_REQ_STR = 44,
									IG2_REQ_STA = 45,		IG2_REQ_DEX = 46, 	IG2_REQ_INT = 47,	IG2_REQ_CHA = 48, 
									IG2_UNKNOWN = 0xffffffff };

	enum e_ItemGenerationType {		IGT_WEAPON = 0, IGT_SHIELD = 1, IGT_CLOTH = 2, IGT_HELMET = 3, IGT_GLOVES_SHOES = 4, 
									IGT_EARRING = 5, IGT_AMULET = 6, IGT_RING = 7, IGT_BELT = 8,
									IGT_UNKNONW = 0xffffffff };

	std::string szGrade[10] = { "", "브론즈 ", "실버 ", "골든 ", "플레티넘 ", "크림슨 ", "루나 ", "솔라 ", "에인션트 ", "미스틱 " };


	char szBuff[1024];
	int iCountBasic = m_Datas[0].m_iValues.size();
	int iCountRef = 0;
	for(int i = iIndexS; i <= iIndexE; i++) iCountRef += m_DataExts[0].m_iValues[i].size();
	int iCountWhole = iCountBasic * iCountRef;
	
	std::vector<std::string>*	pLineArrays = new std::vector<std::string>[iCountWhole+1]; // 라인 버퍼에 쓸 문자열 배열 준비.. +1을 하는 이유는 첫줄에 설명 넣을려고 한다.

	int iIndexCur1 = 0;
	int iIndexCur2 = 0;
	int iExt = iIndexS;
	int iCountWhole2 = 0; // 실제 처리한 갯수..
	int iAddTitle = -1;
	for(i = 0; i < iCountWhole; i++)
	{
		int iType = m_Datas[IG1_GEN_TYPE].m_dwValues[iIndexCur1]; // 아이템 생성타입..

		// 아이템 생성 타입과 확장 테이블에 적용할 번호가 맞는경우만 처리.. 
		if(iType == iExt) 
		{
			int iGrade = (m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2])%10; // 무기 등급
			e_ItemGenerationIndex2 eIG2 = IG2_UNKNOWN;

			for(int j = 0; j < iDTCountBasic; j++)
			{
				DATA_TYPE dt = m_Datas[j].m_Type;
				// 하드 코딩...
				switch(j) 
				{
				case IG1_KEY: // 일련번호 생성..
					if(DT_DWORD == dt)
					{
//						DWORD dwKey = ((m_Datas[j].m_dwValues[iIndexCur1] / 1000) * 1000) + m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2]; // 뒤의거 세자리를 대치한다.
						DWORD dwKey = m_Datas[j].m_dwValues[iIndexCur1] + m_DataExts[IG2_KEY].m_dwValues[iExt][iIndexCur2]; // 더해준다.
						sprintf(szBuff, "%d", dwKey);
					}
					else
					{
						lstrcpy(szBuff, "Invalid Key");
					}
					break;

				case IG1_GEN_TYPE:
					lstrcpy(szBuff, "");
					break;

				case IG1_NAME: // 아이템 이름
					if(DT_STRING == dt)
					{
						sprintf(szBuff, "%s%s %s", szGrade[iGrade].c_str(), 
							m_DataExts[IG2_NAME_HEAD].m_Texts[iExt][iIndexCur2].c_str(), 
							m_Datas[j].m_Texts[iIndexCur1].c_str());
					}
					else lstrcpy(szBuff, "Invalid Name");
					break;

				case IG1_REMARK: // 아이템 설명.. 데이터로써는 필요없다.
				case IG1_PIC:
				case IG1_ICON:
				case IG1_SOUND0:
				case IG1_SOUND1:
				case IG1_ROBE_TYPE:
					lstrcpy(szBuff, "");
					break;

				case IG1_DAMAGE: // 단순 더하기.
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
				case IG1_REQ_CHA:
					{
						bool bApplyAbility = false; // 요구 능력치가 0 일때 무시하고... 계산한 값이 0 보다 작을때 0으로 표시할것..

						if(IG1_DAMAGE == j)			eIG2 = IG2_DAMAGE;
						else if(IG1_AC == j)		eIG2 = IG2_AC;
						else if(IG1_EFFECT1 == j)	eIG2 = IG2_EFFECT1;
						else if(IG1_EFFECT2 == j)	eIG2 = IG2_EFFECT2;
						else if(IG1_DURATION == j)	eIG2 = IG2_DURATION;
						else if(IG1_REQ_LEVEL == j) { eIG2 = IG2_REQ_LEVEL;	bApplyAbility = true; }
						else if(IG1_REQ_RANK == j)	{ eIG2 = IG2_REQ_RANK;	bApplyAbility = true; }
						else if(IG1_REQ_TITLE == j) { eIG2 = IG2_REQ_TITLE;	bApplyAbility = true; }
						else if(IG1_REQ_STR == j)	{ eIG2 = IG2_REQ_STR;	bApplyAbility = true; }
						else if(IG1_REQ_STA == j)	{ eIG2 = IG2_REQ_STA;	bApplyAbility = true; }
						else if(IG1_REQ_DEX == j)	{ eIG2 = IG2_REQ_DEX;	bApplyAbility = true; }
						else if(IG1_REQ_INT == j)	{ eIG2 = IG2_REQ_INT;	bApplyAbility = true; }
						else if(IG1_REQ_CHA == j)	{ eIG2 = IG2_REQ_CHA;	bApplyAbility = true; }
						else { lstrcpy(szBuff, "Invalid Value"); break; }

						if(DT_STRING != dt) 
						{
							int iValue = m_Datas[j].m_iValues[iIndexCur1] + m_DataExts[eIG2].m_iValues[iExt][iIndexCur2];
							if(bApplyAbility) // 요구 능력치로 계산..
							{
								if(0 == m_Datas[j].m_iValues[iIndexCur1]) iValue = 0; // 요구 능력치가 0 이면 계산하지 않는다.
								if(iValue < 0) iValue = 0;
							}
							sprintf(szBuff, "%d", iValue);
						}
						else
						{
							lstrcpy(szBuff, "Invalid Value");
						}
					}
					break;

				case IG1_BUY_PRICE: // 아이템 가격 - 곱하기..
					if(DT_STRING != dt) 
					{
						int iValue = m_Datas[j].m_iValues[iIndexCur1] * m_DataExts[IG2_BUY_PRICE].m_iValues[iExt][iIndexCur2];
						sprintf(szBuff, "%d", iValue);
					}
					else
					{
						lstrcpy(szBuff, "Invalid BuyPrice");
					}
					break;
				case IG1_DELAY:  // 공격 속도 Percentage 계산.
					eIG2 = IG2_DELAY_PERCENT;
					if(DT_STRING != dt)
					{
						int iValue = 0;
						if(0 == m_DataExts[eIG2].m_iValues[iExt][iIndexCur2]) // Devide By 0..
						{
//							lstrcpy(szBuff, "Invalid Percentage");
							lstrcpy(szBuff, "100");
						}
						else
						{
							iValue = m_Datas[j].m_iValues[iIndexCur1] * m_DataExts[eIG2].m_iValues[iExt][iIndexCur2] / 100;
							sprintf(szBuff, "%d", iValue);

							int iDamage = m_Datas[IG1_DAMAGE].m_iValues[iIndexCur1];
							if(iDamage > 0) // 무기일때 공격 속도를 검사해 본다..
							{
								if(iValue < 50)
								{
									char szErr[256];
									sprintf("무기의 공격속도가 50 미만입니다.\n    아이템 이름 : %s\n    %d번째 확장테이블, %d번째열\n    공격속도 : %d", 
										m_Datas[IG1_NAME].m_Texts[iIndexCur1].c_str(), iExt, iIndexCur2, iValue);
									MessageBox(::GetActiveWindow(), szErr, "테이블 생성 경고", MB_OK);
								}
							}
						}
					}
					else
					{
						lstrcpy(szBuff, "Invalid Percentage");
					}
					break;

				default:
					switch(dt)
					{
					case DT_CHAR: case DT_SHORT: case DT_INT: 	sprintf(szBuff, "%d", m_Datas[j].m_iValues[iIndexCur1]); break;
					case DT_BYTE: case DT_WORD: case DT_DWORD:	sprintf(szBuff, "%d", m_Datas[j].m_dwValues[iIndexCur1]); break;
					case DT_STRING:								sprintf(szBuff, "%s", m_Datas[j].m_Texts[iIndexCur1].c_str()); break;
					case DT_FLOAT: 	case DT_DOUBLE:				sprintf(szBuff, "%f", m_Datas[j].m_dfValues[iIndexCur1]); break;
					default: lstrcpy(szBuff, "Invalid Type"); break;
					}
					break;
				}
				
				if(lstrlen(szBuff) > 0)
				{
					pLineArrays[i+1].push_back(szBuff); // 문자열이 있어야 넣는다.
					
					if(iAddTitle < 0) iAddTitle = i;
					if(i == iAddTitle)
					{
						pLineArrays[0].push_back(m_Datas[j].m_szTitle); // 설명 넣기..
						m_DataTypes.push_back(dt);
					}
				}
			} // end of for(int j = 0; j < iDTCountBasic; j++)

			for(j = 0; j < iDTCountRef; j++)
			{
				DATA_TYPE dt = m_DataExts[j].m_Type;

				switch(j)
				{
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
					switch(dt)
					{
					case DT_CHAR: case DT_SHORT: case DT_INT:	sprintf(szBuff, "%d", m_DataExts[j].m_iValues[iExt][iIndexCur2]); break;
					case DT_BYTE: case DT_WORD: case DT_DWORD:	sprintf(szBuff, "%d", m_DataExts[j].m_dwValues[iExt][iIndexCur2]); break;
					case DT_STRING:								sprintf(szBuff, "%s", m_DataExts[j].m_Texts[iExt][iIndexCur2].c_str()); break;
					case DT_FLOAT: case DT_DOUBLE:				sprintf(szBuff, "%f", m_DataExts[j].m_dfValues[iExt][iIndexCur2]); break;
					default: lstrcpy(szBuff, "Invalid Type"); break;
					}
					break;
				}

				if(lstrlen(szBuff) > 0)
				{
					if(i == iAddTitle)
					{
						pLineArrays[0].push_back(m_DataExts[j].m_szTitle); // 설명 넣기..
						m_DataTypes.push_back(dt); // 데이터 타입 넣기..
					}
					pLineArrays[i+1].push_back(szBuff); // 문자열이 있어야 넣는다.
				}
			}

			iCountWhole2++; // 실제 처리한 갯수 증가..
		} // end of if(iType == iExt) // 무기 타입과 확장 테이블에 적용할 번호가 맞는경우만 처리..

		// 인덱스 계산..
		iIndexCur2++;
		if(iIndexCur2 >= m_DataExts[0].m_iValues[iExt].size())
		{
			iIndexCur2 = 0;
			iIndexCur1++;
			if(iIndexCur1 >= m_Datas[0].m_iValues.size()) 
			{
				iIndexCur1 = 0;
				iExt++;
			}
		}
	} // end of for(i = 0; i < iCountWhole; i++)
	
	int iColCount = pLineArrays[0].size();
	for(i = 0; i <= iCountWhole; i++) // 설명까지 쓴다.
	{
		if(pLineArrays[i].empty()) continue;

		for(int j = 0; j < iColCount; j++)
		{
			WriteFile(hFile, pLineArrays[i][j].c_str(), pLineArrays[i][j].size(), &dwRWC, NULL);
			if(j < iColCount - 1) WriteFile(hFile, "\t", 1, &dwRWC, NULL); // 탭 구분..
			else WriteFile(hFile, "\r\n", 2, &dwRWC, NULL); // 줄 넘기기.
		}
	}
	// Item Table 조합....
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	delete [] pLineArrays; pLineArrays = NULL;
	CloseHandle(hFile);

	// Data Type 저장..
	this->DataTypeSave(szEnumFileName);
	m_DataTypes = DataTypesPrev; // 백업 받은걸로 돌려놓는다.
	return true;
}

int CTableGenerator::ParseLine(const char* szLine, int& iOffset, int& iVal, DWORD& dwVal, double& dfVal, std::string& szText) // 성공하면 0, 추가해야하고 끝이면 -1 추가할 필요 없고 끝이면.. -2 리턴
{
	const char* szLine2 = szLine + iOffset;

	if('\r' == *szLine2 || '\n' == *szLine2)
	{
		iOffset++;
		return 0;
	}

	iVal = 0;
	dwVal = 0;
	dfVal = 0;
	szText = "";

	while(true)
	{
		if('\t' == *szLine2)
		{
			iVal = atoi(szText.c_str());
			dwVal = atol(szText.c_str());
			dfVal = atof(szText.c_str());
			iOffset++;
			return 1;
		}
		else if('\r' == *szLine2 || '\n' == *(szLine2)) // 라인피드 + CR -> 라인의 끝이다..
		{
			iVal = atoi(szText.c_str());
			dwVal = atol(szText.c_str());
			dfVal = atof(szText.c_str());
			iOffset++;
			return 2;
		}
		else if(NULL == *szLine2)
		{
			iOffset++;
			return 0;
		}

		szText += *szLine2;
		szLine2++;
		iOffset++;
	}

	return 0;
}

bool CTableGenerator::DataTypeSave(const std::string &szFN)
{
	HANDLE hFile = CreateFile(szFN.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HWND hWnd = ::GetActiveWindow();
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, "파일을 만들 수 없습니다.", "데이터 형식 저장", MB_OK);
		return false;
	}

	DWORD dwNum;
	int iDataCount = m_DataTypes.size();
	WriteFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
	for (int i=0; i<iDataCount;++i)
	{
		DATA_TYPE datatype = m_DataTypes[i];
		WriteFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL);
	}

	CloseHandle(hFile);	
	return true;
}

bool CTableGenerator::DataTypeLoad(const std::string &szFN)
{
	HWND hWnd = ::GetActiveWindow();
	HANDLE hFile = CreateFile(szFN.c_str(),GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, "파일이 없거나 읽을 수 없습니다.", "데이터 형식 부르기", MB_OK);
		return false;
	}
	m_DataTypes.clear();

	// 파일에서 정보 읽기
	int iDataCount = 0;
	DWORD dwNum;
	ReadFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);
	if (iDataCount>0)
	{
		ASSERT(0 == m_DataTypes.size());
		m_DataTypes.assign(iDataCount, DT_NONE);
		for(int i=0; i<iDataCount; ++i)
		{
			DATA_TYPE datatype;
			ReadFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL);
			m_DataTypes[i] = datatype;
		}
	}
	CloseHandle(hFile);

	return true;
}

bool CTableGenerator::Convert2Bin(const std::string& szFN)
{
	HWND hWnd = ::GetActiveWindow();
	FILE* stream = fopen(szFN.c_str(), "r");
	if (NULL == stream)
	{
		MessageBox(hWnd, "읽을 수 없는 파일입니다.", "Convert Error", MB_OK);
		return false;
	}
	const int iMaxStrLen = 4096;
	char line[iMaxStrLen+1];

	// 맨 윗줄은 column의 이름들이 써있음
	if (fgets(line, iMaxStrLen, stream) == NULL)
	{
		fclose(stream);
		std::string szMsg = szFN + " - 파일 내용을 읽을 수 없습니다.";
		MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
		return false;
	}
	// column이 몇개인가 확인해보기
	int iDataCount = m_DataTypes.size();
	int iCount = 0;
	char* token = MyToken(line);
	while(NULL != token)
	{
		++iCount;
		token = MyToken( NULL);
	}
	if (iCount != iDataCount)
	{
		fclose(stream);
		std::string szMsg = szFN + " - 파일 데이터와 설정한 데이터 수가 일치하지 않습니다.\n다시 확인해 주세요!";
		MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
		return false;
	}
	// row 가 몇 줄인지 세어보기
	iCount = 0;
	bool bQuotationActived = false;
	while(fgets(line, iMaxStrLen, stream))
	{
		char* pcFind = line;
		pcFind = strchr(pcFind, '\"');		// 따옴표 쳐져있는지 검사.
		while(pcFind)
		{
			if ( *(pcFind-1) != '\t' && *(pcFind+1) != '\t' && *(pcFind+1) != '\n')
			{
				MessageBox(hWnd, "예상치 않은 곳에 따옴표가 있습니다.", "Convert Error", MB_OK);
				return false;
			}
			bQuotationActived = !bQuotationActived;
			pcFind = strchr(pcFind+1, '\"');
		}

		if (!bQuotationActived) ++iCount;	// 따옴표 쳐져있는데 줄 바뀌면 카운트를 세지 않는다.
	}

	// binary file 생성하기
	char szFName[_MAX_PATH], szDrv[_MAX_DRIVE], szPath[_MAX_PATH];
	char szDestFN[_MAX_PATH];
	_splitpath(szFN.c_str(), szDrv, szPath, szFName, NULL);
	_makepath(szDestFN, szDrv, szPath, szFName, ".tbl");
	HANDLE hFile = CreateFile(szDestFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		fclose(stream);
		std::string szMsg = szDestFN; szMsg += " - 파일을 생성할 수 없습니다.";
		MessageBox(hWnd, szMsg.c_str(), "Convert Error", MB_OK);
		return false;
	}

	// data(column) 의 구조가 어떻게 되어 있는지 저장하기
	DWORD dwNum;
	WriteFile(hFile, &iDataCount, sizeof(iDataCount), &dwNum, NULL);			// (엑셀에서 column 수)
	for (int i=0; i<iDataCount; ++i)
	{
		DATA_TYPE datatype = m_DataTypes[i];
		WriteFile(hFile, &datatype, sizeof(DATA_TYPE), &dwNum, NULL);	// 각각의 column에 해당하는 data type
	}
	// row 가 몇줄인지 저장
	WriteFile(hFile, &iCount, sizeof(iCount), &dwNum, NULL);

	// txt 파일에서 table 정보부터 다시 읽기
	int iRet = fseek(stream, 0, SEEK_SET);	ASSERT(0 == iRet);
	fgets(line, iMaxStrLen, stream); // 처음 한줄은 그냥 읽기 (즉, 2번째 줄부터 다시 읽기)

	// 키 중복 검사용 데이터..
	std::set<int> KeySet;
	typedef std::set<int>::iterator it_Key;
	typedef std::pair<it_Key, bool> pair_Key;

	BOOL bCheckEmptyValue = FALSE;
	bQuotationActived = false;
	std::string strValueBuffer;	// 여러 줄에 걸쳐진 스트링을 저장할 버퍼
	int iRowCount = 0;
	int iColCount = 0;
	while(iRowCount<iCount)
	{
		if (!fgets(line, iMaxStrLen, stream)) break;
		int iStrLen = lstrlen(line);
		ASSERT(iStrLen>0);
		if (line[iStrLen-1] == '\n')
		{
			line[iStrLen-1] = '\0';		// \n문자 없애기
		}

		token = MyToken( line);

		int iKey = atoi(token);
		pair_Key pk = KeySet.insert(iKey);
		if(false == pk.second)
		{
			char szErr[512];
			sprintf(szErr, "Key 중복 : Line %d, Key : %d, File : %s", iRowCount + 1, iKey, szFN.c_str());
			MessageBox(hWnd, szErr, "Key 중복 - 테이블에 추가 실패", MB_OK);
			CloseHandle(hFile);
			fclose(stream);
			ASSERT(0);
			return false;
		}

		//for (j=0; j<m_iDataCount; ++j)
		while(iColCount<iDataCount)
		{
			if (bQuotationActived)
			{	// 문자열이면
				if (NULL == token)
				{
					// 줄바꿈
					strValueBuffer += "\n";
					break;
				}
				else
				{
					int iLast = lstrlen(token)-1;
					if(iLast>=0 && '\"' == token[iLast])
					{	// 따옴표가 있으면 문자열 끝이다.
						bQuotationActived = false;
						strValueBuffer += std::string(token).substr(0, iLast);

						// 버퍼에 있는것 기록
						if (FALSE == WriteData(hFile, m_DataTypes[iColCount], strValueBuffer.c_str()))
						{
							char szErr[512];
							wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
							MessageBox(hWnd, szErr, "데이타 기록이 무시되는 것이 있습니다. 이 파일은 제대로 작동 되지 않을 것입니다.", MB_OK);
							CloseHandle(hFile);
							fclose(stream);
							ASSERT(0);
							return false;
						}
						token = MyToken(NULL);
						++iColCount;
						continue;
					}
					else
					{	// 따옴표가 없으면 문자열을 계속 더해간다.
						strValueBuffer += token;
						token = MyToken(NULL);
						continue;
					}
				}
			}
			else if(token && '\"' == token[0])	// bQuotationActived 가 false이고 '\"' == token[0] 이면
			{	// 문자열이 시작되었다.
				int iLast = lstrlen(token)-1;
				if(iLast>=1 && '\"' == token[iLast])	// iLast>=1 인 이유는 토큰에 "가 2개임을 확실히 하기 위해서.
				{	// 같은 토큰에서 문자열이 닫혔다.
					strValueBuffer = std::string(token).substr(1, iLast-1);

					// 버퍼에 있는것 기록
					if (FALSE == WriteData(hFile, m_DataTypes[iColCount], strValueBuffer.c_str()))
					{
						char szErr[512];
						wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
						MessageBox(hWnd, szErr, "데이타 기록이 무시되는 것이 있습니다. 이 파일은 제대로 작동 되지 않을 것입니다.", MB_OK);
						CloseHandle(hFile);
						fclose(stream);
						ASSERT(0);
						return false;
					}
					token = MyToken(NULL);
					++iColCount;
					continue;
				}
				else
				{	// 이번 토큰에서 문자열이 닫히지 않았다.
					bQuotationActived = true;
					strValueBuffer = (token+1);	// +1의 이유는 따옴표는 저장하지 않는다.
					token = MyToken(NULL);
					continue;
				}

			}

			ASSERT(token || (iColCount+1) == iDataCount);	// 맨 마지막에 데이터가 없으면 NULL이 올 수 있따.

			if (FALSE == (token || (NULL == token && DT_STRING == m_DataTypes[iColCount])) && FALSE == bCheckEmptyValue)
			{
				int iSelect = MessageBox(hWnd, "테이블에 문자열 외에 빈 칸이 있습니다.\nYes : 멈춤\nNo : 이번만 무시하고 계속\n Cancel : 모두 무시하고 계속", "Convert Error", MB_YESNOCANCEL);
				if (IDYES == iSelect)
				{
					MessageBox(hWnd, "데이타 기록이 중단되었습니다. 이 파일은 제대로 작동 되지 않을 것입니다.", "Convert Error", MB_OK);
					CloseHandle(hFile);
					fclose(stream);
					ASSERT(0);
					return false;
				}
				else if (IDCANCEL == iSelect)	bCheckEmptyValue = TRUE;
			}

			if (FALSE == WriteData(hFile, m_DataTypes[iColCount], token))
			{
				char szErr[512];
				wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
				MessageBox(hWnd, szErr, "데이타 기록이 무시되는 것이 있습니다. 이 파일은 제대로 작동 되지 않을 것입니다.", MB_OK);
				CloseHandle(hFile);
				fclose(stream);
				ASSERT(0);
				return false;
			}
			token = MyToken(NULL);
			++iColCount;
		}
		if (bQuotationActived) continue;	// 따옴표가 열려 있으면 계속 문자열이므로 다시 위로..
		if (iDataCount != iColCount)
		{
			char szErr[512];
			wsprintf(szErr, "File - %s, Line - %d, Field - %d", szFN.c_str(), iRowCount + 1, iColCount);
			MessageBox(hWnd, szErr, "현재 설정된 데이타 항목과 기록되는 데이타 항목이 일치하지 않는 항목이 있습니다.", MB_OK);
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
	// 암호화.. 게임의 키와 동일하다...

	hFile = ::CreateFile(szDestFN, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szDestFN, "암호화 파일 만들기에 실패 했습니다.", MB_OK);
		remove(szDestFN); // 파일 지우고..
		return false;
	}

	// 파일 암호화 하기.. .. 파일을 읽은 다음..
	DWORD dwSizeHigh = 0;
	DWORD dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
	if(dwSizeLow <= 0)
	{
		CloseHandle(hFile);
		::remove(szDestFN); // 임시 파일 지우기..
		return false;
	}

	// 원래 파일을 읽고..
	BYTE* pDatas = new BYTE[dwSizeLow];
	DWORD dwRWC = 0;
	::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // 암호화된 데이터 읽고..
	CloseHandle(hFile); // 원래 파일 닫고

	// 테이블 만드는 툴에서 쓰는 키와 같은 키..
	WORD key_r = 0x0816;
	WORD key_c1 = 0x6081;
	WORD key_c2 = 0x1608;

//BYTE Encrypt(BYTE plain)
//{
//	BYTE cipher;
//	cipher = (plain ^ (key_r>>8));
//	key_r = (cipher + key_r) * key_c1 + key_c2;
//	return cipher;
//}

//BYTE Decrypt(BYTE cipher)
//{
//	BYTE plain;
//	plain = (cipher ^ (m_r>>8));
//	m_r = (cipher + m_r) * m_c1 + m_c2;
//	return plain;
//}
	
	// 암호화 인코딩...
	for(i = 0; i < dwSizeLow; i++)
	{
		BYTE byData = (pDatas[i] ^ (key_r>>8));
		key_r = (byData + key_r) * key_c1 + key_c2;
		pDatas[i] = byData;
	}

	// 다시 파일을 연다..
	hFile = ::CreateFile(szDestFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // 파일에 암호화 시킨 데이터 쓰기
	CloseHandle(hFile); // 파일 닫기
	delete [] pDatas; pDatas = NULL;

















	if (iCount == iRowCount)
	{
		char szErr[512];
		wsprintf(szErr, "%d개의 데이타 기록이 완료되었습니다.", iRowCount);
		MessageBox(hWnd, szErr, "Convert Report", MB_OK);
		return true;
	}
	else
	{
		char szErr[512];
		wsprintf(szErr, "전체 %d개의 데이타중 %d개의 데이타만 기록이 완료되었습니다.\n이 테이블은 제대로 작동되지 않을것입니다.", iCount, iRowCount);
		MessageBox(hWnd, szErr, "데이타 갯수 에러", MB_OK);
		return false;
	}
}

// 파일에 데이타 타입별로 쓰기..
bool CTableGenerator::WriteData(HANDLE hFile, DATA_TYPE DataType, LPCTSTR lpszData)
{
	// 만약 lpszData == NULL이면 datatype은 string일 경우에만 된다.
//	ASSERT(lpszData || (NULL == lpszData && DT_STRING == DataType));

	DWORD dwNum;
	switch(DataType)
	{
	case DT_CHAR:
		{
			char cWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0])  || '-' == lpszData[0])
				{
					int iTemp = atoi(lpszData);
					if (iTemp < -127 || iTemp > 128)
						return false; // 범위가 벗어났어~
					cWrite = (char)iTemp;
				}
				else
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &cWrite, sizeof(cWrite), &dwNum, NULL);
		}
		break;
	case DT_BYTE:
		{
			BYTE byteWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]))
				{
					int iTemp = atoi(lpszData);
					if (iTemp < 0 || iTemp > 255)
						return false; // 범위가 벗어났어~
					byteWrite = (BYTE)iTemp;
				}
				else
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &byteWrite, sizeof(byteWrite), &dwNum, NULL);
		}
		break;
	case DT_SHORT:
		{
			short iWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) || '-' == lpszData[0] )
				{
					int iTemp = atoi(lpszData);
					if (iTemp < -32767 || iTemp > 32768)
						return false; // 범위가 벗어났어~
					iWrite = (short)iTemp;
				}
				else 
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
		}
		break;
	case DT_WORD:
		{
			WORD iWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) )
				{
					int iTemp = atoi(lpszData);
					if (iTemp < 0 || iTemp > 65535) 
						return false; // 범위가 벗어났어~
					iWrite = (short)iTemp;
				}
				else
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
		}
		break;
	case DT_INT:
		{
			int iWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) || '-' == lpszData[0] )
					iWrite = atoi(lpszData);
				else
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
		}
		break;
	case DT_DWORD:
		{
			DWORD iWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) )
					iWrite = strtoul(lpszData, NULL, 10);
				else
					return false;		// 문자는 안되~!
			}

			WriteFile(hFile, &iWrite, sizeof(iWrite), &dwNum, NULL);
		}
		break;
	case DT_STRING:
		{
			int iStrLen = 0;
			if(lpszData)
			{
				iStrLen = lstrlen(lpszData);
			}
			WriteFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);
			if (iStrLen>0) WriteFile(hFile, lpszData, iStrLen, &dwNum, NULL);
		}
		break;
	case DT_FLOAT:
		{
			float fWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0] )
					fWrite = (float)atof(lpszData);
				else
					return false;	// 문자는 안되~!
			}
			WriteFile(hFile, &fWrite, sizeof(fWrite), &dwNum, NULL);
		}
		break;
	case DT_DOUBLE:
		{
			double dWrite = 0;
			if(lpszData)
			{
				if (isdigit(lpszData[0]) || '-' == lpszData[0] || '.' == lpszData[0] )
					dWrite = atof(lpszData);
				else
					return false;
				WriteFile(hFile, &dWrite, sizeof(dWrite), &dwNum, NULL);
			}
		}
		break;

	case DT_NONE:
	default:
		ASSERT(0);
	}
	return true;
}

// binary 파일에서 데이타 타입별로 읽기
bool CTableGenerator::ReadData(HANDLE hFile, DATA_TYPE DataType, LPTSTR lpszData)
{
	return true;
}

// 탭으로 구분된 문자열을 찾는다.
char* CTableGenerator::MyToken(LPCTSTR lpszInput)
{
	static char szLine[4096] = "";
	static char* pszCur = NULL;
	if (lpszInput)
	{
		lstrcpy(szLine, lpszInput);
		pszCur = szLine;
	}
	int iStrLen = 0;
	if (pszCur) iStrLen = lstrlen(pszCur);
	if (iStrLen <= 0) return NULL;
	char* pcFind = strchr(pszCur, '\t');

	if (NULL == pcFind)
	{
		char* pszReturn = pszCur;
		pszCur = NULL;
		return pszReturn;
	}
	else
	{
		*pcFind = '\0';
		char* pszReturn = pszCur;
		pszCur = pcFind + 1;
		return pszReturn;
	}
}

void CTableGenerator::Release()
{
	m_szEnmBasic = "";
	m_szTxtBasic = "";
	m_szEnmExt = "";
	for(int i = 0; i < MAX_ITEM_EXTENSION; i++) m_szTxtExts[i] = "";

	m_DataTypes.clear();
	m_Datas.clear();
	m_DataExts.clear();
}

bool CTableGenerator::DataTypeAssign(int iCount, DATA_TYPE Type)
{
	if(iCount < 0) return false;

	m_DataTypes.clear();
	m_DataTypes.assign(iCount, Type);
	return true;
}

bool CTableGenerator::DataTypeInsert(int iIndex, DATA_TYPE Type)
{
	if(iIndex < 0 || iIndex >= m_DataTypes.size()) return false;

	std::vector<DATA_TYPE>::iterator it = m_DataTypes.begin(), itEnd = m_DataTypes.end();
	for(int i=0; it != itEnd; it++, i++)
	{
		if (iIndex == i)
		{
			it = m_DataTypes.insert(it, DT_STRING);
			return true;
		}
	}
	return false;
}

bool CTableGenerator::DataTypeDelete(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_DataTypes.size()) return false;

	std::vector<DATA_TYPE>::iterator it = m_DataTypes.begin(), itEnd = m_DataTypes.end();
	for(int i=0; it != itEnd; it++, i++)
	{
		if (iIndex == i)
		{
			it = m_DataTypes.erase(it);
			return true;
		}
	}
	
	return false;
}
