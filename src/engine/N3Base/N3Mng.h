// N3Mng.h: interface for the CN3Mng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable : 4786)

#ifdef _N3GAME
#include "LogWriter.h"
#endif

#include <string>
#include <map>

template <class T> class CN3Mng {
  protected:
    typedef typename std::map<std::string, T *>::iterator   it_Data;
    typedef typename std::map<std::string, T *>::value_type val_Data;

    typedef typename std::map<T *, int>::iterator   it_Ref;
    typedef typename std::map<T *, int>::value_type val_Ref;

    std::map<std::string, T *> m_Datas;
    std::map<T *, int>         m_Refs;

  public:
    int Count() { return m_Datas.size(); }

    int Add(T * pData) {
        if (NULL == pData) {
#ifdef _N3GAME
            CLogWriter::Write("CN3Mng::Add - Null object pointer");
#endif
            return -1;
        }
        if (pData->FileName().empty()) {
#ifdef _N3GAME
            CLogWriter::Write("CN3Mng::Add - Null object file name");
#endif
            return -1;
        }

        std::pair<it_Ref, bool> pairRef = m_Refs.insert(val_Ref(pData, 1));
        if (false == pairRef.second) {
#ifdef _N3GAME
            CLogWriter::Write("CN3Mng::Add - Duplicated object pointer");
#endif
            return -1;
        }

        std::pair<it_Data, bool> pairData = m_Datas.insert(val_Data(pData->FileName(), pData));
        if (false == pairData.second) {
#ifdef _N3GAME
            CLogWriter::Write("CN3Mng::Add - duplicated object's file name.");
#endif
            m_Refs.erase(pairRef.first); // Clear the reference count...
            return -1;
        }

        return m_Datas.size() - 1;
    }

    T * Get(int index, bool bIncreaseRefCount = TRUE) {
        if (index < 0 || index >= m_Datas.size()) {
            return NULL;
        }

        if (bIncreaseRefCount) {
            it_Ref it = m_Refs.begin();
            for (int i = 0; i < index; i++, it++) {
                ;
            }
            it->second++;
        }

        it_Data it = m_Datas.begin();
        for (int i = 0; i < index; i++, it++) {
            ;
        }

        return it->second;
    }

    T * Get(const std::string & szFN, bool bIncreaseRefCount = TRUE, int iLOD = 0) {
        if (szFN.empty()) {
            return NULL;
        }
        std::string szFN2 = szFN;
        CharLower(&(szFN2[0]));

        T *     pData = NULL;
        it_Data it = m_Datas.find(szFN2);
        if (it == m_Datas.end()) // I couldn't find it...
        {
            pData = new T();
            pData->m_iLOD = iLOD; // Apply LOD when loading

            if (false == pData->LoadFromFile(szFN2)) // Failed to read file!!
            {
                delete pData;
                pData = NULL;
            } else {
                int reChk = Add(pData); // If reading is successful, add...
                //    asm
                if (reChk == -1) // When adding, there is data before, so one reference count is added.
                {
                    T * pBakData =
                        pData; // Receives and returns all data from the same file (may not be changed to a new image)
                    it_Data it = m_Datas.find(pBakData->FileName());
                    pData = (*it).second;

                    if (bIncreaseRefCount) {
                        it_Ref it2 = m_Refs.find(pData);
                        if (it2 != m_Refs.end()) // Find reference count...
                        {
                            ((*it2).second)++;
                        }
                    }

                    delete pBakData; // Delete the data read this time as it is no longer needed.
                    pBakData = NULL;
                }
                //    asm
            }
        } else // found..!!
        {
            pData = (*it).second;

            if (bIncreaseRefCount) {
                it_Ref it2 = m_Refs.find(pData);
                if (it2 != m_Refs.end()) // Find reference count...
                {
                    ((*it2).second)++;
                }
            }
        }

        return pData;
    }

    bool IsExist(const std::string & szFN) {
        if (szFN.empty()) {
            return false;
        }

        if (m_Datas.find(szFN) != m_Datas.end()) {
            return true;
        } else {
            return false;
        }
    } // Is it there or not?

    void Delete(T ** ppData, bool bReleaseOrg = true) {
#ifndef _N3GAME
        bReleaseOrg = true; // In the tool, unconditionally
#endif
        if (NULL == ppData || NULL == *ppData) {
            return;
        }

        it_Data it = m_Datas.find((*ppData)->FileName());
        if (it == m_Datas.end()) {
            return; // I couldn't find it...
        } else      //   found..!!
        {
            it_Ref it2 = m_Refs.find(*ppData);
            if (bReleaseOrg && it2 != m_Refs.end()) // Find reference count...
            {
                ((*it2).second)--;
                if (0 == (*it2).second) {
                    delete (*ppData);
                    m_Datas.erase(it);
                    m_Refs.erase(it2);
                }
            }
            *ppData = NULL;
        }
    }

#ifdef _N3TOOL
    void SaveToFiles() {
        it_Data it = m_Datas.begin(), itEnd = m_Datas.end();
        for (; it != itEnd; it++) {
            (*it).second->SaveToFile();
        }
    }
#endif // end of _N3TOOL

    void Release() {
        it_Data it = m_Datas.begin(), itEnd = m_Datas.end();
        for (; it != itEnd; it++) {
            delete ((*it).second);
        }

        m_Datas.clear();
        m_Refs.clear();
    }

    CN3Mng() {}

    ~CN3Mng() { Release(); }
};
