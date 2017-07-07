#ifndef __SYNCSESSION_H__
#define __SYNCSESSION_H__

class CSyncSessionPrivateData;
class CFileSession;

class DLL_EXPORT CSyncSession
{
public:
	CSyncSession();
	virtual ~CSyncSession();

public:
	virtual int AddSession(CFileSession* pSession);
	virtual int RemoveSession(CFileSession* pSession);

	virtual void PushData(CFileSession* pSession, void* pData);

protected:
	virtual void DisptchData(unsigned long long uTime);
	virtual void ClearData();

private:
	CSyncSessionPrivateData* m_pSyncSessionData;
};

#endif