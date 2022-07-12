//////////////////////////////////////////////////////////////////////
//
// STLMap.h: interface for the CSTLMap class.
//
// This class can use ordinary map of the standard template library 
//
// Designed by SAMMA	 2002. 03. 15
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_)
#define AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)		// Visual C++ Only

#include <map>
using namespace std;

template <class T1, class T2> class CSTLMap  
{
public:
	typedef map < T1, T2* >::iterator		Iterator;
	typedef map < T1, T2* >::value_type		ValueType;

	map < T1, T2* >		m_UserTypeMap;
	

	int GetSize() { return m_UserTypeMap.size(); };
	
	bool PutData( T1 key_value, T2* pData)
	{
		if( !pData ) return false;
		pair<Iterator, bool> temp_pair = m_UserTypeMap.insert( ValueType( key_value, pData ) );
		return temp_pair.second;
	};
	
	T2* GetData( T1 key_value )
	{
		T2* pData = NULL;
		Iterator iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return NULL;
		else
			pData = (*iter).second;
		
		return pData;
	};

	bool DeleteData( T1 key_value )
	{
		if( m_UserTypeMap.empty() )
			return false;
		
		Iterator iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return false;
		else {
			T2* pData = NULL;
			pData = (*iter).second;

			m_UserTypeMap.erase(iter);

			delete pData;
			pData = NULL;

			return true;
		}
	};

	void DeleteAllData()
	{
		Iterator iter1 = m_UserTypeMap.begin(), iter2 = m_UserTypeMap.end();
		for(; iter1 != iter2; iter1++ )
			delete (*iter1).second;
		
		m_UserTypeMap.clear();
	};

	bool IsExist( T1 key_value )
	{
		Iterator iter = m_UserTypeMap.find( key_value );
		if( iter == m_UserTypeMap.end() )
			return false;
		else
			return true;
	};

	bool IsEmpty() { return m_UserTypeMap.empty(); };

	CSTLMap() {};
	virtual ~CSTLMap() {
		DeleteAllData();
	};
};

#endif // !defined(AFX_STLMap_H__9153F571_6888_4BD4_ABC9_E97416B54096__INCLUDED_)
