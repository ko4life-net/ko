// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Map.h"
#include "AIServerDlg.h"
#include "Region.h"
#include "Npc.h"
#include "User.h"
#include "RoomEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CRITICAL_SECTION g_region_critical;

CMapInfo::CMapInfo() {
    m_sEvent = 0;
}

CMapInfo::~CMapInfo() {}

MAP::MAP() {
    m_pMain = (CServerDlg *)AfxGetApp()->GetMainWnd();
    m_nMapSize = 0;
    m_fUnitDist = 0.0f;
    m_fHeight = NULL;

    m_sizeRegion.cx = 0;
    m_sizeRegion.cy = 0;
    m_sizeMap.cx = 0;
    m_sizeMap.cy = 0;

    m_ppRegion = NULL;
    //m_pRoomEvent = NULL;
    m_nZoneNumber = 0;
    m_byRoomType = 0;
    m_byRoomEvent = 0;
    m_byRoomStatus = 1;
    m_byInitRoomCount = 0;
    m_sKarusRoom = 0;
    m_sElmoradRoom = 0;
    //    for(int i=0; i<MAX_DUNGEON_BOSS_MONSTER; i++)
    //        m_arDungeonBossMonster[i] = 1;
}

MAP::~MAP() {
    RemoveMapData();
}

void MAP::RemoveMapData() {
    //    int i, j, k;

    if (m_ppRegion) {
        for (int i = 0; i < m_sizeRegion.cx; i++) {
            delete[] m_ppRegion[i];
            m_ppRegion[i] = NULL;
        }
        delete[] m_ppRegion;
        m_ppRegion = NULL;
    }

    if (m_fHeight) {
        for (int i = 0; i < m_nMapSize; i++) {
            delete[] m_fHeight[i];
            m_fHeight[i] = NULL;
        }
        delete[] m_fHeight;
    }

    if (m_pMap) {
        for (int i = 0; i < m_sizeMap.cx; i++) {
            delete[] m_pMap[i];
            m_pMap[i] = NULL;
        }
        delete[] m_pMap;
        m_pMap = NULL;
    }

    if (!m_ObjectEventArray.IsEmpty()) {
        m_ObjectEventArray.DeleteAllData();
    }
    if (!m_arRoomEventArray.IsEmpty()) {
        m_arRoomEventArray.DeleteAllData();
    }

    DeleteCriticalSection(&g_region_critical);
}

BOOL MAP::IsMovable(int dest_x, int dest_y) {
    if (dest_x < 0 || dest_y < 0) {
        return FALSE;
    }

    if (!m_pMap) {
        return FALSE;
    }
    if (dest_x >= m_sizeMap.cx || dest_y >= m_sizeMap.cy) {
        return FALSE;
    }

    BOOL bRet = FALSE;
    if (m_pMap[dest_x][dest_y].m_sEvent == 0) {
        bRet = TRUE;
    } else {
        bRet = FALSE;
    }
    return bRet;
    //return (BOOL)m_pMap[dest_x][dest_y].m_bMove;
}

///////////////////////////////////////////////////////////////////////
//    각 서버가 담당하고 있는 zone의 Map을 로드한다.
//
BOOL MAP::LoadMap(HANDLE hFile) {
    LoadTerrain(hFile);
    m_N3ShapeMgr.Create((m_nMapSize - 1) * m_fUnitDist, (m_nMapSize - 1) * m_fUnitDist);
    if (!m_N3ShapeMgr.LoadCollisionData(hFile)) {
        return FALSE;
    }

    if ((m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr.Width() ||
        (m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr.Height()) {
        return FALSE;
    }

    int mapwidth = (int)m_N3ShapeMgr.Width();

    m_sizeRegion.cx = (int)(mapwidth / VIEW_DIST) + 1;
    m_sizeRegion.cy = (int)(mapwidth / VIEW_DIST) + 1;

    m_sizeMap.cx = m_nMapSize;
    m_sizeMap.cy = m_nMapSize;

    m_ppRegion = new CRegion *[m_sizeRegion.cx];
    for (int i = 0; i < m_sizeRegion.cx; i++) {
        m_ppRegion[i] = new CRegion[m_sizeRegion.cy];
        m_ppRegion[i]->m_byMoving = 0;
    }

    LoadObjectEvent(hFile);
    LoadMapTile(hFile);

    InitializeCriticalSection(&g_region_critical);

    return TRUE;
}

void MAP::LoadTerrain(HANDLE hFile) {
    DWORD dwRWC;
    ReadFile(hFile, &m_nMapSize, sizeof(int), &dwRWC, NULL); // 가로세로 정보가 몇개씩인가?
    ReadFile(hFile, &m_fUnitDist, sizeof(float), &dwRWC, NULL);

    m_fHeight = new float *[m_nMapSize];
    for (int i = 0; i < m_nMapSize; i++) {
        m_fHeight[i] = new float[m_nMapSize];
    }

    int x, z;
    for (z = 0; z < m_nMapSize; z++) {
        for (x = 0; x < m_nMapSize; x++) {
            ReadFile(hFile, &(m_fHeight[x][z]), sizeof(float), &dwRWC, NULL); // 높이값 읽어오기
        }
    }
}

float MAP::GetHeight(float x, float z) {
    int iX, iZ;
    iX = (int)(x / m_fUnitDist);
    iZ = (int)(z / m_fUnitDist);
    //_ASSERT( iX, iZ가 범위내에 있는 값인지 체크하기);

    float y;
    float h1, h2, h3;
    float dX, dZ;
    dX = (x - iX * m_fUnitDist) / m_fUnitDist;
    dZ = (z - iZ * m_fUnitDist) / m_fUnitDist;

    //    _ASSERT(dX>=0.0f && dZ>=0.0f && dX<1.0f && dZ<1.0f);
    if (!(dX >= 0.0f && dZ >= 0.0f && dX < 1.0f && dZ < 1.0f)) {
        return FLT_MIN;
    }

    if ((iX + iZ) % 2 == 1) {
        if ((dX + dZ) < 1.0f) {
            h1 = m_fHeight[iX][iZ + 1];
            h2 = m_fHeight[iX + 1][iZ];
            h3 = m_fHeight[iX][iZ];

            //if (dX == 1.0f) return h2;

            float h12 = h1 + (h2 - h1) * dX;              // h1과 h2사이의 높이값
            float h32 = h3 + (h2 - h3) * dX;              // h3과 h2사이의 높이값
            y = h32 + (h12 - h32) * ((dZ) / (1.0f - dX)); // 찾고자 하는 높이값
        } else {
            h1 = m_fHeight[iX][iZ + 1];
            h2 = m_fHeight[iX + 1][iZ];
            h3 = m_fHeight[iX + 1][iZ + 1];

            if (dX == 0.0f) {
                return h1;
            }

            float h12 = h1 + (h2 - h1) * dX;              // h1과 h2사이의 높이값
            float h13 = h1 + (h3 - h1) * dX;              // h1과 h3사이의 높이값
            y = h13 + (h12 - h13) * ((1.0f - dZ) / (dX)); // 찾고자 하는 높이값
        }
    } else {
        if (dZ > dX) {
            h1 = m_fHeight[iX][iZ + 1];
            h2 = m_fHeight[iX + 1][iZ + 1];
            h3 = m_fHeight[iX][iZ];

            //if (dX == 1.0f) return h2;

            float h12 = h1 + (h2 - h1) * dX;                     // h1과 h2사이의 높이값
            float h32 = h3 + (h2 - h3) * dX;                     // h3과 h2사이의 높이값
            y = h12 + (h32 - h12) * ((1.0f - dZ) / (1.0f - dX)); // 찾고자 하는 높이값
        } else {
            h1 = m_fHeight[iX][iZ];
            h2 = m_fHeight[iX + 1][iZ];
            h3 = m_fHeight[iX + 1][iZ + 1];

            if (dX == 0.0f) {
                return h1;
            }

            float h12 = h1 + (h2 - h1) * dX;       // h1과 h2사이의 높이값
            float h13 = h1 + (h3 - h1) * dX;       // h1과 h3사이의 높이값
            y = h12 + (h13 - h12) * ((dZ) / (dX)); // 찾고자 하는 높이값
        }
    }
    return y;
}

BOOL MAP::ObjectIntersect(float x1, float z1, float y1, float x2, float z2, float y2) {
    __Vector3 vec1(x1, y1, z1), vec2(x2, y2, z2);
    __Vector3 vDir = vec2 - vec1;
    float     fSpeed = vDir.Magnitude();
    vDir.Normalize();

    return m_N3ShapeMgr.CheckCollision(vec1, vDir, fSpeed);
}

void MAP::RegionUserAdd(int rx, int rz, int uid) {
    if (rx < 0 || rz < 0 || rx >= m_sizeRegion.cx || rz >= m_sizeRegion.cy) {
        return;
    }

    int * pInt = NULL;

    EnterCriticalSection(&g_region_critical);

    pInt = new int;
    *pInt = uid;
    m_ppRegion[rx][rz].m_RegionUserArray.PutData(uid, pInt);

    LeaveCriticalSection(&g_region_critical);
}

Iterator MAP::RegionUserRemove(int rx, int rz, int uid) {
    if (rx < 0 || rz < 0 || rx > m_sizeRegion.cx || rz > m_sizeRegion.cy) {
        return {};
    }

    CRegion *                 region = NULL;
    map<int, int *>::iterator Iter;

    EnterCriticalSection(&g_region_critical);

    region = &m_ppRegion[rx][rz];
    Iter = region->m_RegionUserArray.DeleteData(uid);

    LeaveCriticalSection(&g_region_critical);

    return Iter;
}

void MAP::RegionNpcAdd(int rx, int rz, int nid) {
    if (rx < 0 || rz < 0 || rx >= m_sizeRegion.cx || rz >= m_sizeRegion.cy) {
        return;
    }

    int * pInt = NULL;

    EnterCriticalSection(&g_region_critical);

    pInt = new int;
    *pInt = nid;
    if (!m_ppRegion[rx][rz].m_RegionNpcArray.PutData(nid, pInt)) {
        TRACE("### Map - RegionNpcAdd Fail : x=%d,z=%d, nid=%d ###\n", rx, rz, nid);
    }

    int nSize = m_ppRegion[rx][rz].m_RegionNpcArray.GetSize();
    //TRACE("+++ Map - RegionNpcAdd : x=%d,z=%d, nid=%d, total=%d \n", rx,rz,nid, nSize);

    LeaveCriticalSection(&g_region_critical);
}

Iterator MAP::RegionNpcRemove(int rx, int rz, int nid) {
    if (rx < 0 || rz < 0 || rx > m_sizeRegion.cx || rz > m_sizeRegion.cy) {
        return {};
    }

    CRegion *                 region = NULL;
    map<int, int *>::iterator Iter;

    EnterCriticalSection(&g_region_critical);

    region = &m_ppRegion[rx][rz];
    Iter = region->m_RegionNpcArray.DeleteData(nid);

    LeaveCriticalSection(&g_region_critical);

    return Iter;
}

void MAP::LoadMapTile(HANDLE hFile) {
    //MapTile속성 읽기..
    //    속성이 0이면 못 가는 곳.
    //    1이면 그냥 가는 곳...
    //    그외는 이벤트 ID.
    //
    int      x1 = m_sizeMap.cx;
    int      z1 = m_sizeMap.cy;
    DWORD    dwNum;
    short ** pEvent;
    pEvent = new short *[m_sizeMap.cx];
    for (int a = 0; a < m_sizeMap.cx; a++) {
        pEvent[a] = new short[m_sizeMap.cx];
    }
    // 잠시 막아놓고..
    for (int x = 0; x < m_sizeMap.cx; x++) {
        ReadFile(hFile, pEvent[x], sizeof(short) * m_sizeMap.cy, &dwNum, NULL);
    }

    m_pMap = new CMapInfo *[m_sizeMap.cx];

    for (int i = 0; i < m_sizeMap.cx; i++) {
        m_pMap[i] = new CMapInfo[m_sizeMap.cy];
    }

    int count = 0;
    for (int i = 0; i < m_sizeMap.cy; i++) {
        for (int j = 0; j < m_sizeMap.cx; j++) {
            m_pMap[j][i].m_sEvent = (short)pEvent[j][i];
            //m_pMap[j][i].m_sEvent    = (short)1;
            if (m_pMap[j][i].m_sEvent >= 1) {
                count++;
            }
            //    m_pMap[j][i].m_lUser    = 0;
            //    m_pMap[j][i].m_dwType = 0;
        }
    }
    TRACE("move = %d\n", count);

    /*
    fs::path fsFile = fs::temp_directory_path() / "move1.txt";
    FILE *   stream = _wfopen(fsFile.c_str(), L"w");
    for (int z = m_sizeMap.cy - 1; z >= 0; z--) {
        for (int x = 0; x < m_sizeMap.cx; x++) {
            int v = m_pMap[x][z].m_sEvent;
            fprintf(stream, "%d", v);
        }
        fprintf(stream, "\n");
    }
    fclose(stream);
    */

    if (pEvent) {
        for (int i = 0; i < m_sizeMap.cx; i++) {
            delete[] pEvent[i];
            pEvent[i] = NULL;
        }
        delete[] pEvent;
        pEvent = NULL;
    }
}

int MAP::GetRegionUserSize(int rx, int rz) {
    if (rx < 0 || rz < 0 || rx >= m_sizeRegion.cx || rz >= m_sizeRegion.cy) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);
    CRegion * region = NULL;
    region = &m_ppRegion[rx][rz];
    int nRet = region->m_RegionUserArray.GetSize();
    LeaveCriticalSection(&g_region_critical);

    return nRet;
}

int MAP::GetRegionNpcSize(int rx, int rz) {
    if (rx < 0 || rz < 0 || rx >= m_sizeRegion.cx || rz >= m_sizeRegion.cy) {
        return 0;
    }

    EnterCriticalSection(&g_region_critical);
    CRegion * region = NULL;
    region = &m_ppRegion[rx][rz];
    int nRet = region->m_RegionNpcArray.GetSize();
    LeaveCriticalSection(&g_region_critical);

    return nRet;
}

void MAP::LoadObjectEvent(HANDLE hFile) {
    int             iEventObjectCount = 0, zonenum = 0;
    __Vector3       vPos(0, 0, 0);
    DWORD           dwNum;
    _OBJECT_EVENT * pEvent = NULL;

    ReadFile(hFile, &iEventObjectCount, 4, &dwNum, NULL);
    for (int i = 0; i < iEventObjectCount; i++) {
        pEvent = new _OBJECT_EVENT;
        ReadFile(hFile, &(pEvent->sBelong), 4, &dwNum, NULL); // 소속
        ReadFile(hFile, &(pEvent->sIndex), 2, &dwNum, NULL);  // Event Index
        ReadFile(hFile, &(pEvent->sType), 2, &dwNum, NULL);
        ReadFile(hFile, &(pEvent->sControlNpcID), 2, &dwNum, NULL);
        ReadFile(hFile, &(pEvent->sStatus), 2, &dwNum, NULL);
        ReadFile(hFile, &(pEvent->fPosX), 4, &dwNum, NULL);
        ReadFile(hFile, &(pEvent->fPosY), 4, &dwNum, NULL);
        ReadFile(hFile, &(pEvent->fPosZ), 4, &dwNum, NULL);

        //TRACE("Object - belong=%d, index=%d, type=%d, con=%d, sta=%d\n", pEvent->sBelong, pEvent->sIndex, pEvent->sType, pEvent->sControlNpcID, pEvent->sStatus);

        // 작업할것 : 맵데이터가 바뀌면 Param1이 2이면 성문인것을 판단..  3이면 레버..
        if (pEvent->sType == 1 || pEvent->sType == 2 || pEvent->sType == 3) {
            // sungyong test
            m_pMain->AddObjectEventNpc(pEvent, m_nZoneNumber);
        }

        if (pEvent->sIndex <= 0) {
            continue;
        }
        if (!m_ObjectEventArray.PutData(pEvent->sIndex, pEvent)) {
            TRACE("Object Event PutData Fail - %d\n", pEvent->sIndex);
            delete pEvent;
            pEvent = NULL;
        }
    }
}

BOOL MAP::LoadRoomEvent(int zone_number) {
    DWORD length, count;
    CFile pFile;
    BYTE  byte;
    char  buf[4096];
    char  first[1024];
    char  temp[1024];
    int   index = 0;
    int   t_index = 0, logic = 0, exec = 0;
    int   event_num = 0, nation = 0;

    CRoomEvent * pEvent = NULL;
    fs::path     fsEvtFile = fs::current_path() / "AIServer_MAP" / std::format("{:d}.evt", zone_number);

    if (!pFile.Open(fsEvtFile.string().c_str(), CFile::modeRead)) {
        return FALSE;
    }

    length = pFile.GetLength();
    CArchive in(&pFile, CArchive::load);

    count = 0;

    while (count < length) {
        in >> byte;
        count++;

        if ((char)byte != '\r' && (char)byte != '\n') {
            buf[index++] = byte;
        }

        if (((char)byte == '\n' || count == length) && index > 1) {
            buf[index] = (BYTE)0;
            t_index = 0;

            if (buf[t_index] == ';' || buf[t_index] == '/') { // 주석에 대한 처리
                index = 0;
                continue;
            }

            t_index += ParseSpace(first, buf + t_index);

            if (!strcmp(first, "ROOM")) {
                logic = 0;
                exec = 0;
                t_index += ParseSpace(temp, buf + t_index);
                event_num = atoi(temp);

                if (m_arRoomEventArray.GetData(event_num)) {
                    TRACE("Event Double !!\n");
                    goto cancel_event_load;
                }

                pEvent = NULL;
                pEvent = SetRoomEvent(event_num);
            } else if (!strcmp(first, "TYPE")) {
                t_index += ParseSpace(temp, buf + t_index);
                m_byRoomType = atoi(temp);
            } else if (!strcmp(first, "L")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }
            } else if (!strcmp(first, "E")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }

                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Exec[exec].sNumber = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Exec[exec].sOption_1 = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Exec[exec].sOption_2 = atoi(temp);
                exec++;
            } else if (!strcmp(first, "A")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }

                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Logic[logic].sNumber = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Logic[logic].sOption_1 = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_Logic[logic].sOption_2 = atoi(temp);
                logic++;
                pEvent->m_byCheck = logic;
            } else if (!strcmp(first, "O")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }
            } else if (!strcmp(first, "NATION")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }

                t_index += ParseSpace(temp, buf + t_index);
                nation = atoi(temp);
                if (nation == KARUS_ZONE) {
                    m_sKarusRoom++;
                } else if (nation == ELMORAD_ZONE) {
                    m_sElmoradRoom++;
                }
            } else if (!strcmp(first, "POS")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }

                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iInitMinX = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iInitMinZ = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iInitMaxX = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iInitMaxZ = atoi(temp);
            } else if (!strcmp(first, "POSEND")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }

                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iEndMinX = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iEndMinZ = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iEndMaxX = atoi(temp);
                t_index += ParseSpace(temp, buf + t_index);
                pEvent->m_iEndMaxZ = atoi(temp);
            } else if (!strcmp(first, "END")) {
                if (!pEvent) {
                    goto cancel_event_load;
                }
            }

            index = 0;
        }
    }

    in.Close();
    pFile.Close();

    return TRUE;

cancel_event_load:
    CString str;
    str.Format("이벤트 정보 읽기 실패(%d)(%d)", zone_number, event_num);
    AfxMessageBox(str);
    in.Close();
    pFile.Close();
    //    DeleteAll();
    return FALSE;
    //return TRUE;
}

int MAP::IsRoomCheck(float fx, float fz) {
    // dungeion work
    // 현재의 존이 던젼인지를 판단, 아니면 리턴처리

    CRoomEvent * pRoom = NULL;
    char         notify[100];
    memset(notify, 0x00, 100);

    int nSize = m_arRoomEventArray.GetSize();
    int nX = (int)fx;
    int nZ = (int)fz;
    int minX = 0, minZ = 0, maxX = 0, maxZ = 0;
    int room_number = 0;

    BOOL bFlag_1 = FALSE, bFlag_2 = FALSE;

    for (int i = 1; i < nSize + 1; i++) {
        pRoom = m_arRoomEventArray.GetData(i);
        if (!pRoom) {
            continue;
        }
        if (pRoom->m_byStatus == 3) {
            continue; // 방이 실행중이거나 깬(clear) 상태라면 검색하지 않음
        }

        bFlag_1 = FALSE;
        bFlag_2 = FALSE;

        if (pRoom->m_byStatus == 1) { // 방이 초기화 상태
            minX = pRoom->m_iInitMinX;
            minZ = pRoom->m_iInitMinZ;
            maxX = pRoom->m_iInitMaxX;
            maxZ = pRoom->m_iInitMaxZ;
        } else if (pRoom->m_byStatus == 2) { // 진행중인 상태
            if (pRoom->m_Logic[0].sNumber != 4) {
                continue; // 목표지점까지 이동하는게 아니라면,,
            }
            minX = pRoom->m_iEndMinX;
            minZ = pRoom->m_iEndMinZ;
            maxX = pRoom->m_iEndMaxX;
            maxZ = pRoom->m_iEndMaxZ;
        }

        if (minX < maxX) {
            if (COMPARE(nX, minX, maxX)) {
                bFlag_1 = TRUE;
            }
        } else {
            if (COMPARE(nX, maxX, minX)) {
                bFlag_1 = TRUE;
            }
        }

        if (minZ < maxZ) {
            if (COMPARE(nZ, minZ, maxZ)) {
                bFlag_2 = TRUE;
            }
        } else {
            if (COMPARE(nZ, maxZ, minZ)) {
                bFlag_2 = TRUE;
            }
        }

        if (bFlag_1 == TRUE && bFlag_2 == TRUE) {
            if (pRoom->m_byStatus == 1) { // 방이 초기화 상태
                pRoom->m_byStatus = 2;    // 진행중 상태로 방상태 변환
                pRoom->m_fDelayTime = TimeGet();
                room_number = i;
                TRACE(" Room Check - number = %d, x=%d, z=%d\n", i, nX, nZ);
                //wsprintf(notify, "** 알림 : [%d Zone][%d] 방에 들어오신것을 환영합니다 **", m_nZoneNumber, pRoom->m_sRoomNumber);
                //m_pMain->SendSystemMsg( notify, m_nZoneNumber, PUBLIC_CHAT, SEND_ALL);
            } else if (pRoom->m_byStatus == 2) { // 진행중인 상태
                pRoom->m_byStatus = 3;           // 클리어 상태로
                //wsprintf(notify, "** 알림 : [%d Zone][%d] 목표지점까지 도착해서 클리어 됩니다ㅇ **", m_nZoneNumber, pRoom->m_sRoomNumber);
                //m_pMain->SendSystemMsg( notify, m_nZoneNumber, PUBLIC_CHAT, SEND_ALL);
            }

            return room_number;
        }
    }

    return room_number;
}

CRoomEvent * MAP::SetRoomEvent(int number) {
    CRoomEvent * pEvent = NULL;
    pEvent = m_arRoomEventArray.GetData(number);
    if (pEvent) {
        TRACE("#### SetRoom Error : double event number = %d ####\n", number);
        return NULL;
    }

    pEvent = new CRoomEvent;
    pEvent->m_iZoneNumber = m_nZoneNumber;
    pEvent->m_sRoomNumber = number;
    if (!m_arRoomEventArray.PutData(pEvent->m_sRoomNumber, pEvent)) {
        delete pEvent;
        pEvent = NULL;
        return NULL;
    }

    return pEvent;
}

BOOL MAP::IsRoomStatusCheck() {
    CRoomEvent * pRoom = NULL;
    int          nTotalRoom = m_arRoomEventArray.GetSize() + 1;
    int          nClearRoom = 1;

    if (m_byRoomStatus == 2) { // 방을 초기화중
        m_byInitRoomCount++;
    }

    for (int i = 1; i < nTotalRoom; i++) {
        pRoom = m_arRoomEventArray.GetData(i);
        if (!pRoom) {
            TRACE("#### IsRoomStatusCheck Error : room empty number = %d ####\n", i);
            continue;
            //return NULL;
        }

        if (m_byRoomStatus == 1) { // 방 진행중
            if (pRoom->m_byStatus == 3) {
                nClearRoom += 1;
            }
            if (m_byRoomType == 0) {
                if (nTotalRoom == nClearRoom) { // 방이 다 클리어 되었어여.. 초기화 해줘여,,
                    m_byRoomStatus = 2;
                    TRACE("방이 다 클리어 되었어여.. 초기화 해줘여,, zone=%d, type=%d, status=%d\n", m_nZoneNumber,
                          m_byRoomType, m_byRoomStatus);
                    return TRUE;
                }
            }
        } else if (m_byRoomStatus == 2) { // 방을 초기화중
            if (m_byInitRoomCount >= 10) {
                pRoom->InitializeRoom(); // 실제 방을 초기화
                nClearRoom += 1;
                if (nTotalRoom == nClearRoom) { // 방이 초기화 되었어여..
                    m_byRoomStatus = 3;
                    TRACE("방이 초기화 되었어여..  status=%d\n", m_byRoomStatus);
                    return TRUE;
                }
            }
        } else if (m_byRoomStatus == 3) { // 방 초기화 완료
            m_byRoomStatus = 1;
            m_byInitRoomCount = 0;
            TRACE("방이 다시 시작되었군여..  status=%d\n", m_byRoomStatus);
            return TRUE;
        }
    }
    return FALSE;
}

void MAP::InitializeRoom() {
    CRoomEvent * pRoom = NULL;
    int          nTotalRoom = m_arRoomEventArray.GetSize() + 1;

    for (int i = 1; i < nTotalRoom; i++) {
        pRoom = m_arRoomEventArray.GetData(i);
        if (!pRoom) {
            TRACE("#### InitializeRoom Error : room empty number = %d ####\n", i);
            continue;
        }

        pRoom->InitializeRoom(); // 실제 방을 초기화
        m_byRoomStatus = 1;
        m_byInitRoomCount = 0;
    }
}
