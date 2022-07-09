// N3Mng.h: interface for the CN3Mng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3Mng_h__INCLUDED_)
#define AFX_N3Mng_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Type>

class CN3Mng
{
protected:
	enum { MAX_RESOURCE = 4096 };
	int		m_nCount;
	Type*	m_pDatas[MAX_RESOURCE];
	unsigned int m_uiRef[MAX_RESOURCE];			// Reference Count.. just like COM(Component Object Model ^o^ )..

public:
	int		Count() { return m_nCount; }

	int 	Add(Type* pData)
	{
		if(pData == NULL) return -1;
#ifdef _DEBUG
		if(lstrlen(pData->FileName()) <= 0)MessageBox(GetActiveWindow(), "Object의 File 이름이 NULL 입니다.", "Resource manager warning", MB_OK);
#endif

		BOOL bDuplicated = FALSE;
		for(int i = 0; i < m_nCount; i++)
		{
			if(pData == m_pDatas[i]) 
			{
				m_uiRef[i]++;					// Ref..
				return i - 1;
			}
		}

		m_pDatas[m_nCount] = pData;
		m_uiRef[m_nCount]++;					// Ref..
		if(FALSE == bDuplicated)
		{
#ifdef _DEBUG
			// 포인터가 다른데 파일 이름이 같은지 확인..
			for(i = 0; i < m_nCount; i++)
				if(pData != m_pDatas[i] && lstrcmpi(pData->FileName(), m_pDatas[i]->FileName()) == 0)
					MessageBox(GetActiveWindow(), pData->FileName(), "Object file name was duplicated. Must different from each other", MB_OK);
#endif
			m_nCount++;
		}

		return m_nCount-1;
	}

	Type* 	Get(int index)
	{
		if(index < 0 || index >= m_nCount) return NULL;
		return m_pDatas[index];
	}

	Type* 	Get(const char* szFN, BOOL bIncreaseRefCount = TRUE)
	{
		if(NULL == szFN || lstrlen(szFN) <= 0) return NULL;

		for(int i = 0; i < m_nCount; i++)
		{
			if(lstrcmpi(szFN, m_pDatas[i]->FileName()) == 0) 
			{
				if(bIncreaseRefCount) m_uiRef[i]++; /*Ref..*/ 
				return m_pDatas[i];
			}
		}

		m_pDatas[m_nCount] = new Type();
		if(m_pDatas[m_nCount]->LoadFromFile(szFN) == false)
		{
			delete m_pDatas[m_nCount];
			m_pDatas[m_nCount] = NULL;
			return NULL;
		}

		if(bIncreaseRefCount) m_uiRef[m_nCount]++;					// Ref..
		m_nCount++;

		return m_pDatas[m_nCount-1];
	}

	bool IsExist(const char* szFN) // 있나 없나~
	{
		if(NULL == szFN || lstrlen(szFN) <= 0) return NULL;
		for(int i = 0; i < m_nCount; i++)
		{
			if(lstrcmpi(szFN, m_pDatas[i]->FileName()) == 0) 
			{
				return true;
			}
		}

		return false;
	}

	int		Delete(int index)
	{
		if(index < 0 || index >= m_nCount) return -1;

		m_uiRef[index]--;
		if(m_uiRef[index] <= 0)
		{
			delete m_pDatas[index];
			m_pDatas[index] = NULL;
			for(int j = index; j < m_nCount; j++)
			{
				m_pDatas[j] = m_pDatas[j+1];
				m_uiRef[j] = m_uiRef[j+1];		// Ref..
			}
			m_nCount--;
		}

		return 0;
	}
	
	int		Delete(const char* szFN)
	{
		if(lstrlen(szFN) <= 0) return -1;

		for(int i = 0; i < m_nCount; i++)
		{
			if(lstrcmpi(szFN, m_pDatas[i]->Name()) == 0)
			{
				return Delete(i);
			}
		}

		return -1;
	}

	int		Delete(Type* pData)
	{
		if(NULL == pData) return -1;

		for(int i = 0; i < m_nCount; i++)
		{
			if(pData == m_pDatas[i])
			{
				return Delete(i);
			}
		}

		return -1;
	}

	void SaveToFiles()
	{
		for(int i = 0; i < m_nCount; i++)
		{
			m_pDatas[i]->SaveToFile();
		}
	}
	
	void Init()
	{
		memset(m_pDatas, 0, sizeof(m_pDatas));
		memset(m_uiRef, 0, sizeof(m_uiRef));		// Ref.. Initialize..
		m_nCount = 0;
	}

	void Release()
	{
		for(int i = 0; i < m_nCount; i++)
		{
			if ( m_pDatas[i] ) delete m_pDatas[i]; m_pDatas[i] = NULL;
			m_uiRef[i] = 0;  // Ref.. Initialize..
		}
		m_nCount = 0;
	}
};

#endif // !defined(AFX_N3Mng_h__INCLUDED_)
