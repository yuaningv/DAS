#ifndef __CSTREAM_MGR_H__
#define __CSTREAM_MGR_H__

class DLL_EXPORT CStreamListener
{
public:
	CStreamListener();
	virtual ~CStreamListener();

public:
	virtual HWND GetWndHandle(){ return NULL; }
	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, unsigned long secs, unsigned long usecs, void* pCustomData){}
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
	
	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, unsigned long secs, unsigned long usecs, void* pCustomData);
protected:
	CStreamMgrPrivateData*	m_data;
};
#endif