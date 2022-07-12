// BigFile.h: interface for the CBigFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
#define AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBigFile : public CFile  
{
	DECLARE_DYNAMIC(CBigFile)
public:
	_int64 Seek(_int64 dOff, UINT nFrom);
	CBigFile();
	virtual ~CBigFile();

};

#endif // !defined(AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
