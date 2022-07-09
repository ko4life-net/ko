// N3AnimKey.cpp: implementation of the CN3AnimKey class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3AnimKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3AnimKey::CN3AnimKey()
{
	m_eType = KEY_VECTOR3;
	m_pDatas = NULL;
	CN3AnimKey::Release();
}

CN3AnimKey::~CN3AnimKey()
{
	CN3AnimKey::Release();
}

void CN3AnimKey::Release()
{
	m_nCount = 0; // 키 카운트
	m_fSamplingRate = 30.0;
	delete [] m_pDatas; m_pDatas = NULL;
}

void CN3AnimKey::Alloc(int nCount, float fSamplingRate, ANIMATION_KEY_TYPE eType)
{
	__ASSERT(fSamplingRate > 0.0f, "Animation Sampling Rate 는 반드시 0 보다 커야 합니다.");

	if(nCount <= 0) return;
	if(KEY_VECTOR3 != eType && KEY_QUATERNION != eType) return;

	if(m_pDatas != NULL)
	{
		delete [] m_pDatas; m_pDatas = NULL;
		m_nCount = 0;
	}

	m_eType = eType;
	m_nCount = nCount;
	m_fSamplingRate = fSamplingRate;

	if(KEY_VECTOR3 == m_eType)
	{
		m_pDatas = new __Vector3[nCount];
		memset(m_pDatas, 0, sizeof(__Vector3) * nCount);
	}
	else if(KEY_QUATERNION == m_eType)
	{
		m_pDatas = new __Quaternion[nCount];
		memset(m_pDatas, 0, sizeof(__Quaternion) * nCount);
	}
}

bool CN3AnimKey::Load(HANDLE hFile)
{
	if(m_nCount > 0) this->Release();

	DWORD dwRWC = 0;
	ReadFile(hFile, &m_nCount, 4, &dwRWC, NULL); // 키가 몇개 있는지

	// 키값을 파일에서 읽기..
	if(m_nCount > 0)
	{
		ReadFile(hFile, &m_eType, 4, &dwRWC, NULL); // Key Type
		ReadFile(hFile, &m_fSamplingRate, 4, &dwRWC, NULL); // Sampling Rate

		this->Alloc(m_nCount, m_fSamplingRate, m_eType);
		if(KEY_VECTOR3 == m_eType) ReadFile(hFile, m_pDatas, sizeof(__Vector3)*m_nCount, &dwRWC, NULL);
		else if(KEY_QUATERNION == m_eType) ReadFile(hFile, m_pDatas, sizeof(__Quaternion)*m_nCount, &dwRWC, NULL);
	}

	return true;
}

bool CN3AnimKey::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;
	WriteFile(hFile, &m_nCount, 4, &dwRWC, NULL); // 키가 몇개 있는지

	if(m_nCount > 0)
	{
		WriteFile(hFile, &m_eType, 4, &dwRWC, NULL); // Key Type
		WriteFile(hFile, &m_fSamplingRate, 4, &dwRWC, NULL); // Sampling Rate

		if(KEY_VECTOR3 == m_eType) WriteFile(hFile, m_pDatas, sizeof(__Vector3)*m_nCount, &dwRWC, NULL);
		else if(KEY_QUATERNION == m_eType) WriteFile(hFile, m_pDatas, sizeof(__Quaternion)*m_nCount, &dwRWC, NULL);
	}

	return true;
}

/*
void CN3AnimKey::Translate(int nIndexStart, int nIndexEnd, float fDelta, int nKFlags)
{
	if(nIndexStart < 0 || nIndexStart >= m_nCount) return;
	if(nIndexEnd < 0 || nIndexEnd >= m_nCount) return;

	for(int i = nIndexStart; i <= nIndexEnd; i++)
	{
		if(nKFlags & 1) m_pDatas[i].x += fDelta;
		if(nKFlags & 2) m_pDatas[i].y += fDelta;
		if(nKFlags & 4) m_pDatas[i].z += fDelta;
	}
}
*/

void CN3AnimKey::Add(CN3AnimKey &AKSrc, int nIndexS, int nIndexE)
{
	if(AKSrc.Count() <= 0) return;
	if(nIndexE <= nIndexS) return;
	if(this->Type() != AKSrc.Type()) return;

	int nAddCount = nIndexE - nIndexS + 1; // 추가할 갯수

	int nPrevCount = m_nCount;
	void* pVBackup = NULL;
	if(nPrevCount > 0) // backup
	{
		if(KEY_VECTOR3 == m_eType)
		{
			pVBackup = new __Vector3[nPrevCount];
			memcpy(pVBackup, m_pDatas, sizeof(__Vector3) * nPrevCount);
		}
		else if(KEY_QUATERNION == m_eType)
		{
			pVBackup = new __Quaternion[nPrevCount];
			memcpy(pVBackup, m_pDatas, sizeof(__Quaternion) * nPrevCount);
		}
	}

	this->Alloc(nPrevCount + nAddCount, AKSrc.SamplingRate(), AKSrc.Type()); // 새로 할당.
	if(nPrevCount > 0)
	{
		if(KEY_VECTOR3 == m_eType)
		{
			memcpy(m_pDatas, pVBackup, sizeof(__Vector3) * nPrevCount); // 백업받은걸 restore
		}
		else if(KEY_QUATERNION == m_eType)
		{
			memcpy(m_pDatas, pVBackup, sizeof(__Quaternion) * nPrevCount); // 백업받은걸 restore
		}
	}

	if(KEY_VECTOR3 == m_eType)
	{
		for(int i = 0; i < nAddCount; i++) // 추가.
		{
			__Vector3* pvTmp = (__Vector3*)AKSrc.DataGet(nIndexS + i);
			if(pvTmp) ((__Vector3*)m_pDatas)[nPrevCount + i] = *pvTmp;
			else ((__Vector3*)m_pDatas)[nPrevCount + i].Zero();
		}
	}
	else if(KEY_QUATERNION == m_eType)
	{
		for(int i = 0; i < nAddCount; i++) // 추가.
		{
			__Quaternion* pvTmp = (__Quaternion*)AKSrc.DataGet(nIndexS + i);
			if(pvTmp) ((__Quaternion*)m_pDatas)[nPrevCount + i] = *pvTmp;
			else
			{
				((__Quaternion*)m_pDatas)[nPrevCount + i].x = 
				((__Quaternion*)m_pDatas)[nPrevCount + i].y = 
				((__Quaternion*)m_pDatas)[nPrevCount + i].z = 
				((__Quaternion*)m_pDatas)[nPrevCount + i].w = 0;
			}
		}
	}

	delete [] pVBackup; pVBackup = NULL;
}
