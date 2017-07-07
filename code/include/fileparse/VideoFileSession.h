#ifndef __VIDEOFILE_SESSION_H__
#define __VIDEOFILE_SESSION_H__

#include "FileSession.h"

class CVideoFileSessionPrivateData;
class DLL_EXPORT CVideoFileSession : public CFileSession
{
public:
	CVideoFileSession();
	virtual ~CVideoFileSession();

public:
	virtual int Init(const char* storage, int channel);

protected:
	virtual void ReleaseData(void* pData);
	virtual unsigned int Process();
	virtual unsigned int GetFileDuration(const char* file);

private:
	CVideoFileSessionPrivateData* m_pVideoSessionData;
};
#endif