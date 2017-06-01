#ifndef __CSTREAM_MGR_H__
#define __CSTREAM_MGR_H__

class DLL_EXPORT CCustomDateTime
{
public:
	CCustomDateTime(){ 
		year = 0;
		month = 0;
		mday = 0;
		hour = 0;
		min = 0;
		sec = 0;
		msec = 0;
	}
	virtual ~CCustomDateTime(){ ; }

public:
	unsigned int year;
	unsigned int month;
	unsigned int mday;
	unsigned int hour;
	unsigned int min;
	unsigned int sec;
	unsigned int msec;
};

class DLL_EXPORT CStreamListener
{
public:
	CStreamListener();
	virtual ~CStreamListener();

public:
	virtual HWND GetWndHandle(){ return NULL; }
	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, CCustomDateTime* pTime, void* pCustomData){}
};

class CStreamMgrPrivateData;
class DLL_EXPORT CStreamMgr
{
public:
	CStreamMgr();
	virtual ~CStreamMgr();

public:
	virtual int AddStream(CStreamListener* pStream);
	virtual int RemoveStream(CStreamListener* pStream);
	
	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, CCustomDateTime* pTime, void* pCustomData);
protected:
	CStreamMgrPrivateData*	m_data;
};
#endif