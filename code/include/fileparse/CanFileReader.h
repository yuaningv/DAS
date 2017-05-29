#ifndef __CAN_FILE_H__
#define __CAN_FILE_H__

#include <string>
using namespace std;

class TCanDataItem
{
public:
	unsigned int timeoffset;
	unsigned int canId;
	unsigned char data[8];
};

class CCanFileReader
{
public:
    CCanFileReader();
    virtual ~CCanFileReader();

public:
	int IsReady();
    int Init(const char* fileName);
	int Deinit();

    int SkipTo(unsigned int lOffset);
    int ReadMedia(unsigned char* buffer, unsigned long* length, unsigned long* payload, unsigned long* timestamp);

	unsigned int GetFirstTime(){return m_iFirstTime;}
	unsigned int GetLastTime(){return m_iLastTime;}
	unsigned int GetFileSize() {return m_iFileSize;}
	const char*	 GetFileTime() {return m_sFileName.c_str();}

	int	EndOfFile();

private:
	void InitFileBaseInfo();

private:
    HANDLE m_hFile;
	unsigned int m_iPos;
	unsigned char*	m_pBuffer;
	unsigned int	m_iIndex;
	unsigned int	m_iCount;
	unsigned int	m_bReload;
	unsigned int m_iFileSize;
	unsigned int m_iFirstTime;
	unsigned int m_iLastTime;
	unsigned int m_iCurrTime;

	string			m_sFileName;
};
#endif