#ifndef __FILESESSION_H__
#define __FILESESSION_H__

class CStreamMgr;
class CFileSessionPrivateData;
class DLL_EXPORT CFileSession
{
public:
	CFileSession();
	virtual ~CFileSession();

public:
	virtual int Init(const char* storage, int channel);
	virtual int Deinit();

	virtual int GetState();
	virtual CStreamMgr* GetStreamMgr();

	virtual int  Play();
	virtual int  Pause();
	virtual int  TearDown();

	virtual int	 SetScape(const char* scape);
	virtual int  SkipTo(const char* pos);

	virtual int	 SetStep(int step);

	virtual unsigned int Process();

protected:
	virtual void SwitchNextFile(const char* time);
	virtual unsigned int GetFileDuration(const char* file);

protected:
	CFileSessionPrivateData* m_data;
};
#endif
