// N3AnimControl.h: interface for the CN3AnimControl class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#include "N3BaseFileAccess.h"
#include <string>
#include <vector>

typedef struct __AnimData {
    friend CN3AnimControl;

  public:
    std::string szName;

    float fFrmStart;  // start upper body
    float fFrmEnd;    // upper body end
    float fFrmPerSec; // 30 frames per second is standard.

    float fFrmPlugTraceStart;
    float fFrmPlugTraceEnd;

    float fFrmSound0;
    float fFrmSound1;

    float fTimeBlend; // Blending time when connected to other actions
    int iBlendFlags; // If the blending flag is 0, just blending. If it is 1, there is a time delay equal to the blending time when looping.

    float fFrmStrike0;
    float fFrmStrike1;

    __AnimData() {
        fFrmPerSec = 30.0f; // 30 frames per second is standard.

        fFrmStart = fFrmEnd = 0;
        fFrmPlugTraceStart = fFrmPlugTraceEnd = 0;
        fFrmSound0 = fFrmSound1 = 0;
        fTimeBlend = 0.25f; // Basic blending time..
        iBlendFlags =
            0; // If the blending flag is 0, just blending. If it is 1, there is a time delay equal to the blending time when looping.
        fFrmStrike0 = fFrmStrike1 = 0;
    }

    void operator=(const __AnimData & other) {
        fFrmStart = other.fFrmStart;
        fFrmEnd = other.fFrmEnd;
        fFrmPerSec = other.fFrmPerSec;

        fFrmPlugTraceStart = other.fFrmPlugTraceStart;
        fFrmPlugTraceEnd = other.fFrmPlugTraceEnd;

        fFrmSound0 = other.fFrmSound0;
        fFrmSound1 = other.fFrmSound1;

        fTimeBlend = other.fTimeBlend;
        iBlendFlags =
            other
                .iBlendFlags; // If the blending flag is 0, just blending. If it is 1, there is a time delay equal to the blending time when looping.

        fFrmStrike0 = other.fFrmStrike0;
        fFrmStrike1 = other.fFrmStrike1;

        szName = other.szName;
    }

    void Load(HANDLE hFile) {
        if (NULL == hFile || INVALID_HANDLE_VALUE == hFile) {
            return;
        }

        DWORD dwRWC = 0;

        int nL = 0;
        ReadFile(hFile, &nL, 4, &dwRWC,
                 NULL); // Originally, this is where the string pointer was... for compatibility... just...

        ReadFile(hFile, &fFrmStart, 4, &dwRWC, NULL);  // start upper body
        ReadFile(hFile, &fFrmEnd, 4, &dwRWC, NULL);    // upper body end
        ReadFile(hFile, &fFrmPerSec, 4, &dwRWC, NULL); // 30 frames per second is standard.

        ReadFile(hFile, &fFrmPlugTraceStart, 4, &dwRWC, NULL);
        ReadFile(hFile, &fFrmPlugTraceEnd, 4, &dwRWC, NULL);

        ReadFile(hFile, &fFrmSound0, 4, &dwRWC, NULL);
        ReadFile(hFile, &fFrmSound1, 4, &dwRWC, NULL);

        ReadFile(hFile, &fTimeBlend, 4, &dwRWC, NULL);
        ReadFile(
            hFile, &iBlendFlags, 4, &dwRWC,
            NULL); // If the blending flag is 0, just blending. If it is 1, there is a time delay equal to the blending time when looping.

        ReadFile(hFile, &fFrmStrike0, 4, &dwRWC, NULL);
        ReadFile(hFile, &fFrmStrike1, 4, &dwRWC, NULL);

        // Read name...
        szName = "";
        ReadFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL > 0) {
            std::vector<char> buffer(nL, 0);
            ReadFile(hFile, &buffer[0], nL, &dwRWC, NULL);
            szName = std::string(buffer.begin(), buffer.end());
        }
    }
    void Save(HANDLE hFile) {
        if (NULL == hFile || INVALID_HANDLE_VALUE == hFile) {
            return;
        }

        DWORD dwRWC = 0;

        int nL = 0;
        WriteFile(hFile, &nL, 4, &dwRWC,
                  NULL); // Originally, this is where the string pointer was... for compatibility... just...

        WriteFile(hFile, &fFrmStart, 4, &dwRWC, NULL);  // start upper body
        WriteFile(hFile, &fFrmEnd, 4, &dwRWC, NULL);    // upper body end
        WriteFile(hFile, &fFrmPerSec, 4, &dwRWC, NULL); // 30 frames per second is standard.

        WriteFile(hFile, &fFrmPlugTraceStart, 4, &dwRWC, NULL);
        WriteFile(hFile, &fFrmPlugTraceEnd, 4, &dwRWC, NULL);

        WriteFile(hFile, &fFrmSound0, 4, &dwRWC, NULL);
        WriteFile(hFile, &fFrmSound1, 4, &dwRWC, NULL);

        WriteFile(hFile, &fTimeBlend, 4, &dwRWC, NULL);
        WriteFile(
            hFile, &iBlendFlags, 4, &dwRWC,
            NULL); // If the blending flag is 0, just blending. If it is 1, there is a time delay equal to the blending time when looping.

        WriteFile(hFile, &fFrmStrike0, 4, &dwRWC, NULL);
        WriteFile(hFile, &fFrmStrike1, 4, &dwRWC, NULL);

        // Read name...
        nL = szName.size();
        WriteFile(hFile, &nL, 4, &dwRWC, NULL);
        if (nL > 0) {
            WriteFile(hFile, szName.c_str(), nL, &dwRWC, NULL);
        }
    }

#ifdef _N3TOOL
    void Offset(float fFrmOffset) {
        if (fFrmStart != 0) {
            fFrmStart += fFrmOffset;
        }
        if (fFrmEnd != 0) {
            fFrmEnd += fFrmOffset;
        }

        if (fFrmPlugTraceStart != 0) {
            fFrmPlugTraceStart += fFrmOffset;
        }
        if (fFrmPlugTraceEnd != 0) {
            fFrmPlugTraceEnd += fFrmOffset;
        }

        if (fFrmSound0 != 0) {
            fFrmSound0 += fFrmOffset;
        }
        if (fFrmSound1 != 0) {
            fFrmSound1 += fFrmOffset;
        }

        if (fFrmStrike0 != 0) {
            fFrmStrike0 += fFrmOffset;
        }
        if (fFrmStrike1 != 0) {
            fFrmStrike1 += fFrmOffset;
        }
    }
#endif

} __AnimData;

typedef typename std::vector<__AnimData>::iterator it_Ani;

class CN3AnimControl : public CN3BaseFileAccess {
  protected:
    std::vector<__AnimData> m_Datas; // animation Data List

  public:
    __AnimData * DataGet(int index) {
        if (index < 0 || index >= m_Datas.size()) {
            return NULL;
        }
        return &(m_Datas[index]);
    }
    bool Load(HANDLE hFile);
    int  Count() { return m_Datas.size(); }

#ifdef _N3TOOL
    __AnimData * DataGet(const std::string & szName) {
        int iADC = m_Datas.size();
        for (int i = 0; i < iADC; i++) {
            if (szName == m_Datas[i].szName) {
                return &(m_Datas[i]);
            }
        }
        return NULL;
    }
    void         Swap(int nAni1, int nAni2);
    void         Delete(int nIndex);
    __AnimData * Add();
    __AnimData * Insert(int nIndex);
    bool         Save(HANDLE hFile);
#endif
    void Release();

    CN3AnimControl();
    virtual ~CN3AnimControl();
};
