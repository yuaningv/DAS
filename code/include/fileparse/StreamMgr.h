#ifndef __CSTREAM_MGR_H__
#define __CSTREAM_MGR_H__

class DLL_EXPORT CStreamListener
{
public:
	CStreamListener();
	virtual ~CStreamListener();

public:
	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, unsigned long secs, unsigned long usecs, void* pCustomData){}
};

class CStreamMgrPrivateData;
class DLL_EXPORT CStreamMgr
{
public:
	CStreamMgr();
	virtual ~CStreamMgr();

public:
	int AddStream(CStreamListener* pStream);
	int RemoveStream(CStreamListener* pStream);
	
	void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, unsigned long secs, unsigned long usecs, void* pCustomData);
private:
	CStreamMgrPrivateData*	m_data;
};
#endif