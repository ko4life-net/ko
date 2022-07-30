// BigFile.h: interface for the CBigFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
#define AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_


class CBigFile : public CFile  
{
	DECLARE_DYNAMIC(CBigFile)
public:
	ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
	CBigFile();
	virtual ~CBigFile();

};

#endif // !defined(AFX_BIGFILE_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
