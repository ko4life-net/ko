#ifndef __COMPRESS_H
#define __COMPRESS_H

typedef enum 
{
	COMPRESSING = 1,
	UNCOMPRESSING
} FILEMODE;


class CCompressData : public CObject
{
private:
	char *dataPtr;

public:
	CCompressData() { dataPtr = NULL; };
	virtual ~CCompressData()
	{
		if (dataPtr != NULL)
		{
			delete[] dataPtr;
			dataPtr = NULL;
		}
	};

	char *GetDataPtr() { return dataPtr; }
	void SetDataPtr(char *dp) { dataPtr = dp; }
};


class CCompressManager
{
public:
	int adding_data_count;
	CObList *addList;
	long pnCompressedSize;
	PCHAR  pScratchPad;
	DWORD dwCrc;

	long OrgDataLength;

	char *CompressBufferPtr;
	char *ExtractBufferPtr;

	long nCompressBufferCount;
	long nUnCompressBufferCount;

	long CompressCurPos;
	long UncompressCurPos;

	int Mode;
	int ErrorOccurred;

public:
	CCompressManager();
	virtual ~CCompressManager();

	CObList *GetDataList() { return addList; }
	int PreCompressWork();
	int PreUncompressWork(long nLen, long orgDataLen = 0);

	//Compress Part...
	int AddData(char *data, long nLen = 0L);
	int Compress();
	char *GetCompressionBufferPtr() { return CompressBufferPtr; }
	long GetCompressedDataCount() { return UncompressCurPos; }
	long GetUnCompressDataLength() { return OrgDataLength; }
	long GetCrcValue() { return dwCrc; }
	void FlushAddData();

	//Extract Part...
	void SetCompressionData(char *b_ptr, long nLen = 0);
	int Extract();
	char *GetExtractedBufferPtr() { return ExtractBufferPtr; };
	long GetExtractedBufferCount() { return nUnCompressBufferCount; }
	long GetExtractedDataCount() { return UncompressCurPos; }
	void FlushExtractedData();
};


#endif