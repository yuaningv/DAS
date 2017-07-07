#ifndef __CANFILE_SESSION_H__
#define __CANFILE_SESSION_H__

#include "FileSession.h"

class CCanDataProfile;
class CCanFileSessionPrivateData;
class DLL_EXPORT CCanFileSession : public CFileSession
{
public:
    CCanFileSession();
    virtual ~CCanFileSession();

public:
	virtual int Init(const char* storage, CCanDataProfile* pProfile, int channel);
	
protected:
	virtual void ReleaseData(void* pData);
	virtual unsigned int Process();
	virtual unsigned int GetFileDuration(const char* file);

protected:
	CCanFileSessionPrivateData* m_pCanSessionData;
};

class DLL_EXPORT CCanFileExportSession : public CCanFileSession
{
public:
	CCanFileExportSession();
	virtual ~CCanFileExportSession();
	
protected:
	virtual unsigned int Process();
};
#endif