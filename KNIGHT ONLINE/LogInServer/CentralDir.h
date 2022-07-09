// CentralDir.h: interface for the CCentralDir class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTRALDIR_H__859029E8_8927_4717_9D4B_E26E5DA12BAE__INCLUDED_)
#define AFX_CENTRALDIR_H__859029E8_8927_4717_9D4B_E26E5DA12BAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ZipException.h"
#include <afxtempl.h>
#include "FileHeader.h"
#include "AutoBuffer.h"

class CCentralDir  
{

public:
//		end of central dir signature    4 bytes  (0x06054b50)
	char m_szSignature[4];
//		number of this disk             2 bytes
	WORD m_uThisDisk;
//		number of the disk with the
//		start of the central directory  2 bytes
	WORD m_uDiskWithCD;
//		total number of entries in
//		the central dir on this disk    2 bytes
	WORD m_uDiskEntriesNo;
//		total number of entries in
//		the central dir                 2 bytes
	WORD m_uEntriesNumber;
//		size of the central directory   4 bytes
	DWORD m_uSize;
//		offset of start of central
//		directory with respect to
//		the starting disk number        4 bytes
	DWORD m_uOffset;
//		zipfile comment length          2 bytes
	WORD m_uCommentSize;
//		zipfile comment (variable size)
	CString m_szComment;

	CFileHeader* m_pOpenedFile;
	void RemoveFile(WORD uIndex);
	void Clear();
	CZipStorage* m_pStorage;
	DWORD m_uCentrDirPos;
	DWORD m_uBytesBeforeZip;
	CCentralDir();
	virtual ~CCentralDir();
	void CloseFile();
	void OpenFile(WORD uIndex);
	bool IsValidIndex(WORD uIndex);
	void Read();
	void Init();
	void CloseNewFile();
	void Write();
	int m_iBufferSize;
	bool m_bOnDisk;
	static char m_gszSignature[];
	CTypedPtrArray<CPtrArray, CFileHeader*> m_headers;
	CAutoBuffer m_pLocalExtraField;
	void AddNewFile(CFileHeader & header);
	void RemoveFromDisk();
	DWORD GetSize(bool bWhole = false);
protected:
	bool RemoveDataDescr(bool bFromBuffer);
	DWORD WriteCentralEnd();
	void WriteHeaders();
	bool m_bCurrDataDescrPresent;
	void ReadHeaders();
	void ThrowError(int err);
	DWORD Locate();	
};

#endif // !defined(AFX_CENTRALDIR_H__859029E8_8927_4717_9D4B_E26E5DA12BAE__INCLUDED_)
