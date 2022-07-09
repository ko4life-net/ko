// N3LinkedList.h: interface for the CN3LinkedList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3LinkedList_h__INCLUDED_)
#define AFX_N3LinkedList_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3DUtil.h"

template <class Type>

class CN3LinkedList
{
public:
	typedef struct __LList
	{
		__LList* pPrev;
		Type Value;
		__LList* pNext;
	} LList;

protected:
	int		m_nCount;
	__LList* m_pDatas; // Linked List

public:
	int		Count() { return m_nCount; }
	int 	Add(Type Data)
	{
		if(NULL == m_pDatas)
		{
			m_pDatas = new __LList;
			
			m_pDatas->pPrev = NULL;
			m_pDatas->Value = Data;
			m_pDatas->pNext = NULL;
		}
		else
		{
			__LList* pData = new __LList;
			__LList* pTmp = m_pDatas;
			while(pTmp->pNext) pTmp = pTmp->pNext;

			pTmp->pNext = pData;
			pData->pPrev = pTmp;
			pData->Value = Data;
			pData->pNext = NULL;
		}

		int nIndex = m_nCount;
		m_nCount++;

		return nIndex;
	}

	int 	Insert(int index, Type Data)
	{
		if(index < 0 || index > m_nCount) return -1;
		if(!m_nCount)
		{
			m_pDatas = new __LList;
			
			m_pDatas->pPrev = NULL;
			m_pDatas->Value = Data;
			m_pDatas->pNext = NULL;

			int nIndex = m_nCount;
			m_nCount++;
			return nIndex;
		}

		__LList* pData = new __LList;
		__LList* pTmp = m_pDatas;
		for(int i = 0; i < index; i++) pTmp = pTmp->pNext;

		__LList* pNext = pTmp->pNext;
		pNext->pPrev = pData;

		pTmp->pNext = pData;
		pData->pPrev = pTmp;
		pData->Value = pTmp->Value;
		pData->pNext = pNext;
		pTmp->Value = Data;

		int nIndex = m_nCount;
		m_nCount++;

		return nIndex;
	}

	void* Position(int index)
	{
		if(index < 0 || index >= m_nCount) return NULL;
		
		__LList* pTmp = m_pDatas;
		for(int i = 0; i < index; i++) pTmp = pTmp->pNext;
		return pTmp;
	}

	Type 	Get(int index)
	{
		if(index < 0 || index >= m_nCount) return NULL;

		__LList* pTmp = m_pDatas;
		for(int i = 0; i < index; i++) pTmp = pTmp->pNext;
		return pTmp->Value;
	}

	Type 	Get(void* pPosition)
	{
		if(NULL == pPosition) return NULL;
		return ((__LList*)pPosition)->Value;
	}

	void Next(void* &pPosition)
	{
		if(NULL == pPosition) return;
		pPosition = ((__LList*)pPosition)->pNext;
	}

	void Prev(void* &pPosition)
	{
		if(NULL == pPosition) return;
		pPosition = ((__LList*)pPosition)->pPrev;
	}

	void 	Set(int index, Type Data)
	{
		if(index < 0 || index >= m_nCount) return;

		__LList* pTmp = m_pDatas;
		for(int i = 0; i < index; i++) { pTmp = pTmp->pNext; }
		
		pTmp->Value = Data;
	}

	int		Delete(void* &pPosition)
	{
		if(NULL == pPosition) return -1;
		__LList* pTmp = (__LList*)pPosition;

		if(pTmp == m_pDatas)
		{
			m_pDatas = m_pDatas->pNext;
			delete pTmp;
			if(m_pDatas) m_pDatas->pPrev = NULL;

			pPosition = m_pDatas;
		}
		else
		{
			__LList* pPrevTmp = pTmp->pPrev;
			__LList* pNextTmp = pTmp->pNext;

			if(pPrevTmp) pPrevTmp->pNext = pNextTmp;
			if(pNextTmp) pNextTmp->pPrev = pPrevTmp;

			delete pTmp;

			pPosition = pNextTmp;
		}
		
		m_nCount--;
		if (m_nCount == 0) Init(); //	Count가 0이면 초기화해야 한다.
		return m_nCount;
	}

	void Init()
	{
		m_nCount = 0;
		m_pDatas = NULL;
	}

	void Release()
	{
		__LList* pTmp = NULL;
		for(int i = 0; i < m_nCount; i++) 
		{
			pTmp = m_pDatas;
			m_pDatas = m_pDatas->pNext;
			delete pTmp;
		}

		this->Init();
	}

	CN3LinkedList() { this->Init(); }
	~CN3LinkedList() { this->Release(); }
};

#endif // !defined(AFX_N3LinkedList_h__INCLUDED_)
