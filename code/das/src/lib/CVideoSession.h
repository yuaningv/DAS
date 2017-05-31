#ifndef CVideoSession_h
#define CVideoSession_h

#include "constant.h"

#include "VideoFileSession.h"
#include "StreamMgr.h"


class CVideoSession
{
public:
    CVideoSession();
    ~CVideoSession();

public:
    int init(const QString& strStorage, int iChannel);
    int setScape(const QDateTime& dtBegin, const QDateTime& dtEnd);
    int skipTo(const QDateTime& dtPos);
    int play();
    int pause();

private:
    CVideoFileSession m_videoSession;
};


#endif
