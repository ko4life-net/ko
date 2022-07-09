// FileHeader.h: interface for the CFileHeader class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEHEADER_H__0081FC65_C9C9_4D48_AF72_DBF37DF5E0CF__INCLUDED_)
#define AFX_FILEHEADER_H__0081FC65_C9C9_4D48_AF72_DBF37DF5E0CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipException.h"
#include "ZipStorage.h"
#include "AutoBuffer.h"

class CFileHeader  
{
public:	
//         central file header signature   4 bytes  (0x02014b50)
	char m_szSignature[4];
//         version made by                 2 bytes
	WORD m_uVersionMadeBy;
//         version needed to extract       2 bytes
	WORD m_uVersionNeeded;
//         general purpose bit flag        2 bytes
	WORD m_uFlag;
//         compression method              2 bytes
	WORD m_uMethod;
//         last mod file time              2 bytes
	WORD m_uModTime;
//         last mod file date              2 bytes
	WORD m_uModDate;
//         crc-32                          4 bytes
	DWORD m_uCrc32;
//         compressed size                 4 bytes
	DWORD m_uComprSize;
//         uncompressed size               4 bytes
	DWORD m_uUncomprSize;
//         filename length                 2 bytes
	WORD m_uFileNameSize;
//         extra field length              2 bytes
	WORD m_uExtraFieldSize;
//         file comment length             2 bytes
	WORD m_uCommentSize;
//         disk number start               2 bytes
	WORD m_uDiskStart;
//         internal file attributes        2 bytes
	WORD m_uInternalAttr;
//         external file attributes        4 bytes
	DWORD m_uExternalAttr;
//         relative offset of local header 4 bytes
	DWORD m_uOffset;
//         filename (variable size)
	CString m_szFileName;
//         extra field (variable size)
	CAutoBuffer m_pExtraField;
//         file comment (variable size)
	CString m_szComment;

	CFileHeader();
	virtual ~CFileHeader();
	CTime GetTime();
	void SetTime(const CTime& time);
	static char m_gszSignature[];
	static char m_gszLocalSignature[];
	DWORD GetSize();
protected:
	void ValidateComment();
	void GetCrcAndSizes(char * pBuffer);
	bool PrepareData(int iLevel, bool bIsSpanMode);
	void WriteLocal(CZipStorage& storage);
	bool CheckCrcAndSizes(char* pBuf);
	friend class CCentralDir;
	friend class CZipArchive;
	bool Read(CZipStorage *pStorage);
	bool ReadLocal(CZipStorage *pStorage, WORD& iLocExtrFieldSize, bool& bIsDataDescr);
	DWORD Write(CZipStorage *pStorage);

};

#endif // !defined(AFX_FILEHEADER_H__0081FC65_C9C9_4D48_AF72_DBF37DF5E0CF__INCLUDED_)
