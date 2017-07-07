#ifndef __TIME_SESSION_H__
#define __TIME_SESSION_H__

#include "FileSession.h"
#include "SyncSession.h"

class DLL_EXPORT CTimeSession : public CFileSession, public CSyncSession
{
public:
	CTimeSession();
	virtual ~CTimeSession();

public:
	virtual int Init();
	
protected:
	virtual unsigned int Process();
};
#endif