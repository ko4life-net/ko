// ZipBigFile.h: interface for the CZipBigFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZipBigFile_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
#define AFX_ZipBigFile_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_


class CZipBigFile : public CFile  
{
	DECLARE_DYNAMIC(CZipBigFile)
public:
	ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
	CZipBigFile();
	virtual ~CZipBigFile();

};

#endif // !defined(AFX_ZipBigFile_H__79E0E6BD_25D6_4B82_85C5_AB397D9EC368__INCLUDED_)
