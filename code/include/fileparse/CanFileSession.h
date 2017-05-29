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
	virtual unsigned int Process();
	virtual unsigned int GetFileDuration(const char* file);

private:
	CCanFileSessionPrivateData* m_pCanSessionData;
};
#endif